"""DecodeMessageRot (fn_18371) returns its argument unchanged: the shipped exe skips the decode loop.

Original 0x00B00893:
    83 3d 10 3b 6d 01 00   cmp dword ptr [g_serviceType],0
    eb 61                  jmp 0x00B008FD        ; patched; compiled as 74 61  je 0x00B008FD (return str)
    8b 4d 08               mov ecx,[ebp+8]       ; dead from here on: strlen + the letter-mirroring loop
The source keeps the compiled `if (g_serviceType == 0) return str;` guard and the loop. Without this
patch a non-Korean service type mirrors every message string (a<->z, A<->Z, 0xB0<->0xBF).
"""

from postprocess import PatchError

SYMBOL = "?DecodeMessageRot@@YAPADPAD@Z"
SERVICE_TYPE = "?g_serviceType@@3HA"
GUARD = "83 3d ?? ?? ?? ?? 00 74 ??"


def apply(image):
    start, end = image.function(SYMBOL)
    service_type = image.symbol(SERVICE_TYPE)
    head = image.find(start, end, GUARD)
    if image.u32(head + 2) != service_type:
        raise PatchError(f"{head:#x}: the compare does not read g_serviceType ({service_type:#x})")
    image.write(head + 7, b"\xeb", expect=b"\x74")
    return f"jmp {head + 7:#x} (DecodeMessageRot always returns its argument)"
