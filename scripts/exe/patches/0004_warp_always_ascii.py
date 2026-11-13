"""WARP "AlwaysAscii": ValidateCharacterName (fn_23577) accepts every byte, not only 7-bit ASCII.

Original 0x00DFA650, the loop body of `if (name[index] < 0) return false;`:
    80 3c 10 00            cmp byte ptr [eax+edx],0
    90 90                  nop; nop             ; patched; compiled as 7c 0f  jl 0x00DFA661 (return false)
    40                     inc eax
The source keeps the compiled check. Without this patch character names containing a byte >= 0x80
(e.g. CP949 Korean) are rejected.
"""

SYMBOL = "?ValidateCharacterName@@YG_NPBD@Z"
CHECK = "80 3c ?? 00 7c ?? 40"


def apply(image):
    start, end = image.function(SYMBOL)
    head = image.find(start, end, CHECK)
    jump = image.read(head + 4, 2)
    image.write(head + 4, b"\x90\x90", expect=jump)
    return f"nop {head + 4:#x} (ValidateCharacterName accepts non-ASCII bytes)"
