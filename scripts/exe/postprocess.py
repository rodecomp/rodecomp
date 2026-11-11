"""Post-link binary patches: reproduce byte edits made to the shipped Ragexe.exe after it was compiled.

The original 2026-02-19 executable was hex-edited after the build (e.g. a switch head overwritten with a
`jmp`, leaving the dead switch in place). No C++ compiles to that, so the source keeps the code as it was
compiled and these patches re-apply the edits to the linked exe. They are local only: clank checks
compare object code and never see them.

Pipeline (scripts/exe/build_exe.py):  compile -> link (Ragexe.exe + .map + .pdb) -> postprocess (this)

Patches live in scripts/exe/patches/NNNN_<name>.py and are applied in ascending NNNN order, each exactly
once on a freshly linked exe. A patch module must provide:

    \"\"\"What the original binary does differently and the evidence (addresses, original bytes).\"\"\"
    def apply(image: Image) -> str     # edit through `image`; return a one-line summary

A patch locates its code by decorated symbol name (resolved from the linker map), verifies the exact bytes
it expects, and raises PatchError otherwise: a patch that no longer fits the code fails the build instead
of writing garbage. Image.write() also drops base relocations inside the written range, because a
rewritten absolute operand would otherwise be rebased by the loader (the exe is /DYNAMICBASE).

    python scripts/exe/postprocess.py                      # patch build/exe/Ragexe.exe in place
    python scripts/exe/postprocess.py --exe X.exe --list   # show the patches in order, apply nothing
"""

import argparse
import importlib.util
import re
import struct
import sys
from pathlib import Path

import pefile


sys.modules.setdefault("postprocess", sys.modules[__name__])  # patches import this module, also when run as a script
EXE_DIR = Path(__file__).resolve().parent
PATCH_DIR = EXE_DIR / "patches"
PATCH_NAME = re.compile(r"^(\d{4})_[a-z0-9_]+\.py$")


class PatchError(Exception):
    pass


class Image:
    """A linked exe open for patching, with its linker map for symbol lookup."""

    def __init__(self, exe: Path):
        self.exe = exe
        self.pe = pefile.PE(str(exe))
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.data = bytearray(self.pe.__data__)
        self.symbols = self._read_map(exe.with_suffix(".map"))
        self._starts = sorted(set(self.symbols.values()))

    def _read_map(self, path: Path):
        if not path.exists():
            raise PatchError(f"{path} not found: link with /MAP (build_exe.py does)")
        symbols = {}
        line = re.compile(r"^\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8})\s", re.I)
        for text in path.read_text(encoding="utf-8", errors="replace").splitlines():
            match = line.match(text)
            if match:
                symbols[match.group(1)] = int(match.group(2), 16)
        return symbols

    def symbol(self, decorated: str) -> int:
        """VA of a decorated symbol from the linker map."""
        if decorated not in self.symbols:
            raise PatchError(f"symbol {decorated} not in the linker map")
        return self.symbols[decorated]

    def function(self, decorated: str):
        """(start VA, end VA) of a function; the end is the next symbol in the map."""
        start = self.symbol(decorated)
        later = [va for va in self._starts if va > start]
        return start, later[0] if later else start + 0x10000

    def offset(self, va: int) -> int:
        return self.pe.get_offset_from_rva(va - self.base)

    def read(self, va: int, size: int) -> bytes:
        offset = self.offset(va)
        return bytes(self.data[offset:offset + size])

    def u32(self, va: int) -> int:
        return struct.unpack("<I", self.read(va, 4))[0]

    def find(self, start: int, end: int, pattern: str) -> int:
        """VA of the single match of a hex pattern (`??` = any byte) in [start, end)."""
        regex = re.compile(b"".join(b"." if b == "??" else re.escape(bytes.fromhex(b)) for b in pattern.split()), re.S)
        lo, hi = self.offset(start), self.offset(start) + (end - start)
        hits = [m.start() for m in regex.finditer(bytes(self.data[lo:hi]))]
        if len(hits) != 1:
            raise PatchError(f"pattern '{pattern}' matched {len(hits)} times in {start:#x}-{end:#x}, expected 1")
        return start + hits[0]

    def write(self, va: int, new: bytes, expect: bytes):
        """Replace `expect` at va with `new` (same length) and drop the base relocations inside it."""
        if len(new) != len(expect):
            raise PatchError("replacement and expected bytes differ in length")
        current = self.read(va, len(expect))
        if current != expect:
            raise PatchError(f"{va:#x}: expected {expect.hex(' ')}, found {current.hex(' ')}")
        offset = self.offset(va)
        self.data[offset:offset + len(new)] = new
        self._drop_relocations(va - self.base, va - self.base + len(new))

    def write_jmp(self, va: int, target: int, expect: bytes):
        """`jmp rel32` (5 bytes) from va to target."""
        self.write(va, b"\xe9" + struct.pack("<i", target - (va + 5)), expect)

    def _drop_relocations(self, lo: int, hi: int):
        for block in getattr(self.pe, "DIRECTORY_ENTRY_BASERELOC", []):
            for entry in block.entries:
                if entry.type != 0 and lo < entry.rva + 4 and entry.rva < hi:
                    word_offset = entry.struct.get_file_offset()
                    struct.pack_into("<H", self.data, word_offset, 0)  # IMAGE_REL_BASED_ABSOLUTE: padding, skipped

    def save(self):
        self.pe.close()
        self.exe.write_bytes(self.data)


def discover():
    patches = []
    for path in sorted(PATCH_DIR.glob("*.py")):
        if not PATCH_NAME.match(path.name):
            raise PatchError(f"{path.name}: patch files are named NNNN_lowercase_name.py")
        patches.append(path)
    orders = [p.name[:4] for p in patches]
    if len(set(orders)) != len(orders):
        raise PatchError(f"duplicate patch order numbers in {PATCH_DIR}")
    return patches


def load(path: Path):
    spec = importlib.util.spec_from_file_location(f"patch_{path.stem}", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    if not callable(getattr(module, "apply", None)) or not (module.__doc__ or "").strip():
        raise PatchError(f"{path.name}: a patch needs a docstring and an apply(image) function")
    return module


def run(exe: Path) -> int:
    """Apply every patch in order to exe; all or nothing. Returns the number applied."""
    image = Image(exe)
    applied = 0
    for path in discover():
        summary = load(path).apply(image)
        print(f"patch {path.stem}: {summary}", flush=True)
        applied += 1
    image.save()
    return applied


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--exe", default=str(EXE_DIR.parent.parent / "build" / "exe" / "Ragexe.exe"))
    parser.add_argument("--list", action="store_true", help="list the patches in order and exit")
    args = parser.parse_args()
    try:
        if args.list:
            for path in discover():
                print(f"{path.stem}: {load(path).__doc__.strip().splitlines()[0]}")
            return
        print(f"applied {run(Path(args.exe))} patches to {args.exe}")
    except PatchError as error:
        sys.exit(f"postprocess failed: {error}")


if __name__ == "__main__":
    main()
