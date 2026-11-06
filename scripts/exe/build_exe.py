"""Build every unit in data/compile.json and link them into Ragexe.exe (+ .pdb).

Optional and local: clank checks and merges never run this. It uses only the
toolchain `td toolchain install` provides (the root and runner come from
.td/config.json), with the exact compiler flags of data/compile.json. Vendor
DLL imports (Bink, Granny, Stove, ...) come from scripts/exe/imports/*.def, the
exact import table of the original executable, so a declaration that does not
match the real DLL export fails at link time.

    python scripts/exe/build_exe.py                     # compile + link, fail on any error
    python scripts/exe/build_exe.py --force-multiple    # accept duplicate definitions
    python scripts/exe/build_exe.py --link-only         # relink objects from a previous run
"""

import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
EXE_DIR = Path(__file__).resolve().parent
MSVC = "VC/Tools/MSVC/14.29.30133"
SDK_LIB = "Program Files/Windows Kits/10/Lib/10.0.19041.0"
SYSTEM_LIBS = [
    "kernel32.lib", "user32.lib", "gdi32.lib", "advapi32.lib", "winmm.lib", "ws2_32.lib",
    "ole32.lib", "oleaut32.lib", "uuid.lib", "shell32.lib", "imm32.lib", "version.lib",
    "comctl32.lib", "comdlg32.lib", "shlwapi.lib", "wininet.lib", "gdiplus.lib", "bcrypt.lib",
    "dxguid.lib", "ddraw.lib", "d3d9.lib", "dinput8.lib", "dbghelp.lib", "crypt32.lib",
    "iphlpapi.lib", "psapi.lib", "wintrust.lib", "winhttp.lib", "netapi32.lib",
]


class Toolchain:
    """The td toolchain, run the way td runs it (native on Windows, wine elsewhere)."""

    def __init__(self):
        config = json.loads((ROOT / ".td" / "config.json").read_text(encoding="utf-8"))
        runner = config["runners"]["ragnarok"]
        self.kind = runner["type"]
        if self.kind not in ("native", "wine"):
            sys.exit(f"runner '{self.kind}' is not supported; use td's native (Windows) or wine runner")
        self.root = Path(runner["toolchainRoot"])
        self.bin = self.root / MSVC / "bin" / "Hostx64" / "x86"
        crt = self.root / MSVC / "lib" / "x86"
        if not (crt / "msvcrt.lib").exists() or not (self.root / SDK_LIB / "um" / "x86" / "kernel32.Lib").exists():
            sys.exit(f"{self.root} has no link libraries (msvcrt.lib, kernel32.lib): run 'td toolchain install' to update it")
        self.env = dict(os.environ)
        self.env.update(runner.get("env", {}))
        self.env["LIB"] = ";".join(self.path(p) for p in (
            crt, self.root / MSVC / "atlmfc" / "lib" / "x86",
            self.root / SDK_LIB / "ucrt" / "x86", self.root / SDK_LIB / "um" / "x86"))

    def path(self, p):
        if self.kind == "native":
            return str(p)
        return subprocess.run(["winepath", "-w", str(p)], capture_output=True, text=True, check=True).stdout.strip()

    def run(self, tool, args, env=None, cwd=None):
        exe = str(self.bin / tool)
        command = [exe, *args] if self.kind == "native" else ["wine", exe, *args]
        return subprocess.run(command, env=env or self.env, cwd=cwd, capture_output=True, text=True, errors="replace")


def build_import_libs(tc, out_dir):
    lib_dir = out_dir / "importlibs"
    lib_dir.mkdir(exist_ok=True)
    libs = []
    for definition in sorted((EXE_DIR / "imports").glob("*.def")):
        lib = lib_dir / (definition.stem + ".lib")
        result = tc.run("lib.exe", ["/nologo", "/MACHINE:X86", f"/DEF:{tc.path(definition)}", f"/OUT:{tc.path(lib)}"])
        if result.returncode != 0:
            sys.exit(f"import library {definition.name} failed:\n{result.stdout}{result.stderr}")
        libs.append(lib)
    return libs

# Units only the standalone exe needs: stand-ins for third-party code the
# decompilation does not reproduce. Each is compiled with the flags of the
# listed data/compile.json unit plus any extra include directories (relative to
# the repository). Clank does not know about these files.
EXE_ONLY_UNITS = {
    "scripts/exe/nProtect/GameGuard.cpp": ("Source/Security/NProtectGameGuardMgr.cpp", []),
    "scripts/exe/jsoncons/JsonCppLoaderGlue.cpp": ("Source/Utility/JsonCppLoader.cpp", ["scripts/exe/jsoncons/include"]),
}


def exe_units(config):
    """unit -> (shape index, extra include directories)."""
    units = {unit: (shape, []) for unit, shape in config["units"].items()}
    for unit, (flags_of, includes) in EXE_ONLY_UNITS.items():
        units[unit] = (config["units"][flags_of], includes)
    return units


def compile_units(tc, config, out_dir, jobs):
    root = tc.path(ROOT).replace("\\", "/")

    def expand(value):
        return value.replace("${TC}", tc.path(tc.root)).replace("${SRC}", root)

    env = dict(tc.env)
    for key, value in config["env"].items():
        env[key] = expand(value)

    def build(item):
        unit, (shape, includes) = item
        obj = out_dir / (unit.replace("/", "_") + ".obj")
        extra = [f"/I{tc.path(ROOT / directory)}" for directory in includes]
        args = [*(expand(a) for a in config["shapes"][shape]), *extra, "/Z7", f"/Fo{tc.path(obj)}", tc.path(ROOT / unit)]
        result = tc.run("cl.exe", args, env=env, cwd=ROOT)
        return unit, obj, result.returncode, result.stdout + result.stderr

    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        results = list(pool.map(build, exe_units(config).items()))
    failures = [r for r in results if r[2] != 0]
    for unit, _, _, output in failures:
        print(f"== {unit}")
        print("\n".join(line for line in output.splitlines() if " error " in line or "fatal error" in line))
    print(f"compiled {len(results) - len(failures)}/{len(results)} units")
    return [r[1] for r in results if r[2] == 0], len(failures)


def link(tc, objects, out_dir, extra_args):
    rsp = out_dir / "link.rsp"
    rsp.write_text("\n".join([*(f'"{tc.path(o)}"' for o in objects), *extra_args]), encoding="utf-8")
    exe = out_dir / "Ragexe.exe"
    outputs = (exe, exe.with_suffix(".pdb"))
    for stale in outputs:
        stale.unlink(missing_ok=True)
    args = ["/nologo", "/MACHINE:X86", "/SUBSYSTEM:WINDOWS", "/LARGEADDRESSAWARE",
            "/DEBUG:FULL", f"/PDB:{tc.path(exe.with_suffix('.pdb'))}", "/INCREMENTAL:NO", "/OPT:REF", "/OPT:ICF",
            "/ERRORLIMIT:0", f"/OUT:{tc.path(exe)}", f"@{tc.path(rsp)}", *SYSTEM_LIBS]
    result = tc.run("link.exe", args)
    if result.returncode != 0:
        for partial in outputs:
            partial.unlink(missing_ok=True)
    return result.returncode, result.stdout + result.stderr, exe


def summarize(log, out_dir):
    (out_dir / "link.log").write_text(log, encoding="utf-8")
    duplicates = sorted(set(re.findall(r'(?:error LNK2005|warning LNK4006): "([^"]+)"', log)))
    unresolved = sorted(set(a or b for a, b in re.findall(r'unresolved external symbol (?:"[^"]+" \((\S+)\)|(\S+))', log)))
    print(f"link log: {out_dir / 'link.log'}")
    print(f"duplicate definitions: {len(duplicates)}")
    for symbol in duplicates:
        print(f"  {symbol}")
    print(f"unresolved symbols: {len(unresolved)}")
    codes = Counter(re.findall(r"error (LNK\d+)", log))
    if codes:
        print("linker errors: " + ", ".join(f"{code} x{count}" for code, count in codes.most_common()))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--out", default=str(ROOT / "build" / "exe"), help="output directory")
    parser.add_argument("--jobs", type=int, default=os.cpu_count(), help="parallel compiles")
    parser.add_argument("--force-multiple", action="store_true",
                        help="link despite duplicate definitions (/FORCE:MULTIPLE)")
    parser.add_argument("--link-only", action="store_true", help="reuse the objects from a previous run")
    args = parser.parse_args()

    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)
    config = json.loads((ROOT / "data" / "compile.json").read_text(encoding="utf-8"))
    tc = Toolchain()
    print(f"toolchain: {tc.root} ({tc.kind})")

    if args.link_only:
        objects = [out_dir / (unit.replace("/", "_") + ".obj") for unit in exe_units(config)]
        compile_failures = 0
    else:
        objects, compile_failures = compile_units(tc, config, out_dir, args.jobs)

    extra = ["/FORCE:MULTIPLE"] if args.force_multiple else []
    code, log, exe = link(tc, [*objects, *build_import_libs(tc, out_dir)], out_dir, extra)
    summarize(log, out_dir)
    if code == 0:
        print(f"built {exe}")
    sys.exit(1 if code != 0 or compile_failures else 0)


if __name__ == "__main__":
    main()
