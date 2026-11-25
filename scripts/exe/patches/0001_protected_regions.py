"""Themida's regions: the code between the SDK's markers is copied from the target.

The shipped exe went through Themida after the link. Where the source marks a region (Source/Security/
ThemidaSDK.h: VM_START/VM_END, MUTATE_START/MUTATE_END, ...), the compiler emits  EB 10 "WL  " <id>
00*7 "WL  "  before and after its code. Themida moved that code into sections of its own and left, in
the same bytes: a jmp or call rel32 into its section over the START marker, random bytes over the rest
of the marker and the code, and nine 2-byte  mov reg,reg  over the END marker. E.g. 0x0065B2E0
(GPakEx::RC5::Enc::ProcessAndXorBlock): 0x0065B2EA  E9 69 8F A3 01  20 10 00 00 00 00 00 00 00 57 4C 20
20, 103 random bytes, 0x0065B363  89 F6 89 ED 89 C9 89 DB 89 C9 89 D2 89 C9 89 FF 89 D2, then the
function's own epilogue.

No compiler or protector run produces those bytes again, so each marked region of the linked exe takes
the target's (.td/target.exe), with the jump aimed at the address it reaches there. The function is
found in the target by its decorated name (.td/symbols.json). A region whose bounds are not where the
target's are waits: it stays as compiled and the result names it, because its function does not match
the target yet (the verifier is what judges that). Without the target every region stays as compiled,
which runs: a marker is a jump over itself.
"""

import json
import struct
from pathlib import Path

import pefile

from postprocess import PatchError

TD = Path(__file__).resolve().parents[3] / ".td"
MARKER = "eb 10 57 4c 20 20 ?? 00 00 00 00 00 00 00 57 4c 20 20"
LENGTH = 18


def regions(image, markers):
    """(function start, [(start, end)]) for each function with markers; a region runs from its START
    marker's first byte to the byte after its END marker."""
    starts = sorted(set(image.symbols.values()))
    found = {}
    opened = None
    for va in markers:
        function = max(s for s in starts if s <= va)
        marker_id = image.read(va + 6, 1)[0]
        if opened is None:
            if marker_id & 1:
                raise PatchError(f"{va:#x}: an END marker without its START")
            opened = (function, va, marker_id)
        else:
            if opened[0] != function or marker_id != opened[2] + 1:
                raise PatchError(f"{opened[1]:#x}: the region is not closed by its END marker in the same function")
            found.setdefault(function, []).append((opened[1], va + LENGTH))
            opened = None
    if opened:
        raise PatchError(f"{opened[1]:#x}: the region has no END marker")
    return found


def original_addresses():
    database = json.loads((TD / "symbols.json").read_text(encoding="utf-8"))
    column = {name: index for index, name in enumerate(database["columns"])}
    return {row[column["decorated"]]: row[column["addr"]] for row in database["symbols"]
            if row[column["decorated"]] and row[column["addr"]]}


def apply(image):
    markers = image.find_all(*image.section(".text"), MARKER)
    if not markers:
        return "the linked code marks no region"
    found = regions(image, markers)
    count = sum(len(r) for r in found.values())
    if not (TD / "target.exe").exists() or not (TD / "symbols.json").exists():
        return f"{count} marked regions left as compiled: .td/target.exe and .td/symbols.json are not here"

    target = pefile.PE(str(TD / "target.exe"), fast_load=True)
    base = target.OPTIONAL_HEADER.ImageBase
    known = original_addresses()
    names = {}
    for name, va in image.symbols.items():
        names.setdefault(va, []).append(name)

    def outside_the_file(va):
        section = target.get_section_by_rva(va - base)
        return section is not None and section.Name.rstrip(b"\0") != b".text" and va - base - section.VirtualAddress >= section.SizeOfRawData

    copied = 0
    done = 0
    waiting = []
    for function, parts in found.items():
        originals = {known[name] for name in names[function] if name in known}
        if len(originals) != 1:
            raise PatchError(f"{function:#x} ({', '.join(names[function])}): {len(originals)} addresses in the target for a function with marked regions")
        original = originals.pop()
        for start, end in parts:
            there = original + (start - function)
            theirs = target.get_data(there - base, end - start)
            destination = there + 5 + struct.unpack_from("<i", theirs, 1)[0]
            if theirs[0] not in (0xE8, 0xE9) or not outside_the_file(destination):
                waiting.append(f"{start:#x}: the target has no protected region at {there:#x}")
                continue
            tail = theirs[-LENGTH:]
            if any(tail[i] != 0x89 or tail[i + 1] < 0xC0 or (tail[i + 1] >> 3) & 7 != tail[i + 1] & 7 for i in range(0, LENGTH, 2)):
                waiting.append(f"{start:#x}: the target's region at {there:#x} does not end {end - start} bytes on")
                continue
            new = theirs[:1] + struct.pack("<i", destination - (start + 5)) + theirs[5:]
            image.write(start, new, image.read(start, end - start))
            copied += len(new)
            done += 1
    result = f"protected regions copied from the target: {done} of {count}, {copied} bytes"
    if waiting:
        result += f"; {len(waiting)} left as compiled, their bounds are not the target's: " + "; ".join(waiting)
    return result
