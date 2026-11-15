"""WARP "AlwaysAscii": ValidateCharacterName (fn_23577) accepts every byte, not only 7-bit ASCII.

Original 0x00DFA650, the loop body of `if (name[index] < 0) return false;`:
    80 3c 10 00            cmp byte ptr [eax+edx],0
    90 90                  nop; nop             ; patched; compiled as 7c 0f  jl 0x00DFA661 (return false)
    40                     inc eax
The source keeps the compiled check. Without this patch character names containing a byte >= 0x80
(e.g. CP949 Korean) are rejected.

The reconstructed source currently reads `name[index] == 0` (compiled as 74 ?? je), which can never
fire inside strlen's bounds, so it already accepts every byte; the patch is then a no-op.
"""

SYMBOL = "?ValidateCharacterName@@YG_NPBD@Z"
CHECK = "80 3c ?? 00 7c ?? 40"
ZERO_CHECK = "80 3c ?? 00 74 ?? 40"


def apply(image):
    start, end = image.function(SYMBOL)
    if image.find_all(start, end, ZERO_CHECK):
        return "skipped (ValidateCharacterName already only rejects NUL, accepts non-ASCII bytes)"
    head = image.find(start, end, CHECK)
    jump = image.read(head + 4, 2)
    image.write(head + 4, b"\x90\x90", expect=jump)
    return f"nop {head + 4:#x} (ValidateCharacterName accepts non-ASCII bytes)"
