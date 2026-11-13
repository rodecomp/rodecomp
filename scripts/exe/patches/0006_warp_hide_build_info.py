"""WARP "HideBuildInfo": the /buildinfo chat command (CGameMode::ExecuteTalkType) prints an empty build line.

Original, the TT_SHOW_BUILD_INFO case:
    68 d4 19 1a 01         push 0x011A19D4       ; __TIME__, all bytes zeroed by the patch
    68 e0 19 1a 01         push 0x011A19E0       ; __DATE__, all bytes zeroed by the patch
    68 ec 19 1a 01         push 0x011A19EC       ; "build : %s - %s", first byte zeroed by the patch
The source keeps `sprintf_s(text, sizeof(text), "build : %s - %s", __DATE__, __TIME__)`. The original
date and time strings are lost; this patch erases ours the same way.
"""

from postprocess import PatchError

FORMAT = b"build : %s - %s"


def apply(image):
    fmt = image.find_string(FORMAT)
    start, end = image.section(".text")
    pushes = image.find_all(start, end, "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 " + fmt.to_bytes(4, "little").hex(" "))
    if len(pushes) != 1:
        raise PatchError(f"push of {FORMAT!r} with two string arguments found {len(pushes)} times")
    time, date = image.u32(pushes[0] + 1), image.u32(pushes[0] + 6)
    for va in (time, date):
        text = image.read(va, 32).split(b"\0")[0]
        image.write(va, bytes(len(text)), expect=text)
    image.write(fmt, b"\0", expect=FORMAT[:1])
    return f"erase build format {fmt:#x}, date {date:#x}, time {time:#x}"
