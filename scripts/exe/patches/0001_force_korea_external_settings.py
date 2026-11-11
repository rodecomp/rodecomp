"""CSession::LoadExternalSettings (fn_23603) always loads the Korean ExternalSettings script.

Original 0x00DFD08F, the head of `switch (g_serviceType)`:
    e9 10 00 00 00         jmp 0x00DFD0A4        ; patched; compiled as a1 10 3b 6d 01  mov eax,[g_serviceType]
    83 f8 14               cmp eax,14h           ; dead from here on
    0f 87 75 02 00 00      ja  default
    ff 24 85 8c e4 df 00   jmp [0x00DFE48C+eax*4]
The jump table at 0x00DFE48C is intact and its entry 0 (case 0, Korea) is 0x00DFD0A4, the jmp target.
Without this patch a non-Korean service type loads e.g. service_usa\\ExternalSettings_usa and fails on
GetTableIntValueForC.
"""

import struct

from postprocess import PatchError

SYMBOL = "?LoadExternalSettings@CSession@@QAEXXZ"
SERVICE_TYPE = "?g_serviceType@@3HA"
SWITCH_HEAD = "a1 ?? ?? ?? ?? 83 f8 14 0f 87 ?? ?? ?? ?? ff 24 85 ?? ?? ?? ??"


def apply(image):
    start, end = image.function(SYMBOL)
    service_type = image.symbol(SERVICE_TYPE)
    head = image.find(start, end, SWITCH_HEAD)
    case0 = image.u32(image.u32(head + 17))
    if not start <= case0 < end:
        raise PatchError(f"jump table entry 0 ({case0:#x}) lies outside {SYMBOL}")
    image.write_jmp(head, case0, expect=b"\xa1" + struct.pack("<I", service_type))
    return f"jmp {head:#x} -> case 0 {case0:#x} (skip switch on g_serviceType)"
