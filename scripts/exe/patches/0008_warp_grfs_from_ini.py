"""WARP "GRFsFromIni": WinMain (fn_24380) loads the GRF archives listed in .\\DATA.INI instead of data.grf.

Original 0x00E5F069, after the "data.grf" name is decrypted into [esi]:
    90                     nop                   ; patched; compiled as 56  push esi
    b9 88 56 6d 01         mov ecx,offset g_pakRegistry
    e8 9c ad f8 02         call 0x03DE9E10       ; patched; compiled as call PakRegistry::Add
0x03DE9E10 (in WARP's .data6 section) is the loader below, copied byte for byte with its absolute
operands re-pointed. It reads keys "9" down to "0" of section [Data] in .\\DATA.INI and calls
g_pakRegistry.Add() for each one present; if none is, it writes 2=data.grf and retries.
The source keeps the compiled `g_pakRegistry.Add("data.grf")`.
"""

import struct

from postprocess import PatchError

WINMAIN = "_WinMain@16"
REGISTRY = "?g_pakRegistry@@3VPakRegistry@@A"
ADD = "?Add@PakRegistry@@QAE_NPBD@Z"

# 0x03DE9E10-0x03DE9F10 of the shipped exe; dwords at the offsets in FIXUPS are re-pointed.
LOADER = bytes.fromhex(
    "c8800000606828a21501ff15b0720c0185c00f84cb0000008b3d28720c01e8190000004765745072697661"
    "746550726f66696c65537472696e6741008bd850ffd785c00f849a0000008945f6e81b000000577269746550"
    "72697661746550726f66696c65537472696e6741008bc350ffd785c0746e8945fa33d266c745fe3900526"
    "8fc9ede036a748d5d81538d45fe505068f79ede03ff55f68d4dfe668b098d5d81663b0b5a740e5253b988"
    "566d01e80cfbcffc5a42fe4dfe807dfe3073c185d2752068fc9ede0368079fde0366c745fe32008d45fe50"
    "68f79ede03ff55fa85c0759761c9c30044617461002e5c444154412e494e4900646174612e67726600"
)
KERNEL32 = b"kernel32.dll\0"
DATA, INI, GRF = 0xE7, 0xEC, 0xF7  # "Data", ".\\DATA.INI", "data.grf" inside LOADER
CALL_ADD = 0xAF                    # e8 rel32 PakRegistry::Add


def apply(image):
    if len(LOADER) != 0x100:
        raise PatchError("loader blob is corrupt")
    imports = {(entry.dll.lower(), imp.name): imp.address
               for entry in image.pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports}
    try:
        get_module = imports[(b"kernel32.dll", b"GetModuleHandleA")]
        get_proc = imports[(b"kernel32.dll", b"GetProcAddress")]
    except KeyError as missing:
        raise PatchError(f"import {missing} not found") from None
    registry, add = image.symbol(REGISTRY), image.symbol(ADD)

    base = image.allocate(len(LOADER) + len(KERNEL32))
    code = bytearray(LOADER + KERNEL32)
    fixups = {6: base + len(LOADER), 12: get_module, 26: get_proc, 130: base + INI, 146: base + DATA,
              171: registry, 196: base + INI, 201: base + GRF, 216: base + DATA}
    for offset, value in fixups.items():
        struct.pack_into("<I", code, offset, value)
    struct.pack_into("<i", code, CALL_ADD + 1, add - (base + CALL_ADD + 5))
    image.write(base, bytes(code), expect=bytes(len(code)), relocs=tuple(fixups))

    start, end = image.function(WINMAIN)
    site = image.find(start, end, "56 b9 " + registry.to_bytes(4, "little").hex(" ") + " e8 ?? ?? ?? ??")
    target = site + 11 + struct.unpack("<i", image.read(site + 7, 4))[0]
    if target != add:
        raise PatchError(f"{site:#x}: the call goes to {target:#x}, not PakRegistry::Add")
    image.write(site, b"\x90", expect=b"\x56")
    image.write(site + 6, b"\xe8" + struct.pack("<i", base - (site + 11)), expect=image.read(site + 6, 5))
    return f"call {site + 6:#x} -> DATA.INI loader {base:#x}"
