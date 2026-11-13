"""WARP "SendClientFlags": login packets carry 0x80000000 instead of the clientinfo.xml langtype.

WARP added a section (.xdiff at 0x04A12000) holding the dword 0x80000000 and redirected every
`mov eax,[g_serviceFlag07]` of CLoginMode::OnChangeState (fn_22297) to it, e.g. 0x00D95C99:
    a1 00 20 a1 04         mov eax,[0x04A12000]  ; patched; compiled as a1 7c 3a 6d 01  mov eax,[g_serviceFlag07]
    89 45 8c               mov [ebp-74h],eax     ; AccountLoginPacket.version
The 0x80000000 bit tells the server the client is WARP-patched (PreviewInShop/PreviewInDealer
flags would be bits 0/1; both are off). The source reads g_serviceFlag07 as compiled.
"""

import struct

from postprocess import PatchError

SYMBOL = "?OnChangeState@CLoginMode@@UAEXH@Z"
VERSION = "?g_serviceFlag07@@3HA"
FLAGS = 0x80000000


def apply(image):
    start, end = image.function(SYMBOL)
    address = image.symbol(VERSION).to_bytes(4, "little").hex(" ")
    sites = [va + 1 for va in image.find_all(start, end, "a1 " + address)]
    for reg in (0x0D, 0x15, 0x1D, 0x2D, 0x35, 0x3D):  # mov ecx/edx/ebx/ebp/esi/edi, [disp32]
        sites += [va + 2 for va in image.find_all(start, end, f"8b {reg:02x} " + address)]
    if not sites:
        raise PatchError(f"no read of {VERSION} in {SYMBOL}")
    flags = image.add(struct.pack("<I", FLAGS), align=4)
    for site in sites:
        image.write(site, struct.pack("<I", flags), expect=image.read(site, 4), relocs=(0,))
    return f"{len(sites)} read(s) of g_serviceFlag07 -> {FLAGS:#x} at {flags:#x}"
