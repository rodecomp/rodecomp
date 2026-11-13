"""CSession::SetServiceFlags (fn_17819) stores both flags for every service type.

Original 0x00AD5298:
    85 c0                  test eax,eax
    eb 14                  jmp 0x00AD52B0        ; patched; compiled as 74 14  je 0x00AD52B0 (store flags)
    83 f8 06               cmp eax,6             ; dead from here on: the 6/0Ah/0Ch/2 service-type chain
The source keeps the compiled `if (serviceType == 0 || ... == 2)` guard. Without this patch only the
Korean, 6, 0Ah, 0Ch and 2 service types set g_serviceFlag05/06.
"""

from postprocess import PatchError

SYMBOL = "?SetServiceFlags@CSession@@QAEHHH@Z"
SERVICE_TYPE = "?g_serviceType@@3HA"
GUARD = "a1 ?? ?? ?? ?? 85 c0 74 ??"


def apply(image):
    start, end = image.function(SYMBOL)
    service_type = image.symbol(SERVICE_TYPE)
    head = image.find(start, end, GUARD)
    if image.u32(head + 1) != service_type:
        raise PatchError(f"{head:#x}: the load does not read g_serviceType ({service_type:#x})")
    image.write(head + 7, b"\xeb", expect=b"\x74")
    return f"jmp {head + 7:#x} (SetServiceFlags always stores both flags)"
