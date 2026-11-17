"""WARP "NoGravityLogo" / "NoGravityAds": the login screen (UIWindowMgr::DrawLoginOverlay, fn_17239) loads no
Gravity logo, age-rating or advertisement texture.

Original .rdata, each `T` after "유저인터페이스\\" zeroed by the patch, so the path ends at the backslash:
    0x010DD1F8  "유저인터페이스\\T_GameGrade.tga"   ; DrawLoginOverlay 0x00AA7748, 0x00AA7D60 (and the renderer)
    0x01158908  "유저인터페이스\\T_테입%d.tga"
    0x01158924  "유저인터페이스\\T_중력성인.tga"
    0x01158944  "유저인터페이스\\T_R%d.tga"
Every unpatched client from 2022-06 to 2025-10 has the `T`; the source keeps the compiled paths.
"""

from postprocess import PatchError

PREFIX = "유저인터페이스\\T".encode("cp949")
TAILS = [b"_GameGrade.tga", "_테입%d.tga".encode("cp949"), "_중력성인.tga".encode("cp949"), b"_R%d.tga"]


def apply(image):
    start, end = image.section(".rdata")
    blank = PREFIX[:-1] + b"\0"
    if all(image.find_all(start, end, (blank + tail + b"\0").hex(" ")) for tail in TAILS):
        return "skipped (source already blanks the Gravity logo and advertisement paths)"
    written = []
    for tail in TAILS:
        va = image.find_string(PREFIX + tail)
        image.write(va + len(PREFIX) - 1, b"\0", expect=b"T")
        written.append(f"{va:#x}")
    if len(written) != len(TAILS):
        raise PatchError("not every Gravity image path was found")
    return f"blank {len(written)} Gravity image paths ({', '.join(written)})"
