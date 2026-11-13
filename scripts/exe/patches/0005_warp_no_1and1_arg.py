"""WARP "No1and1Arg": WinMain (fn_24380) runs without a "1rag1"-style command-line argument.

Original 0x00E5E81B, the "1rag1" check (name decrypted with key 0x6BC977B3B5993503):
    68 b3 77 c9 6b         push 6BC977B3h
    68 03 35 99 b5         push 0B5993503h
    ...                    call EncryptedName::Decrypt; push esi; push edi; call strstr; add esp,8
    85 c0                  test eax,eax
    90 e9 7b 03 00 00      nop; jmp 0x00E5EBC3   ; patched; compiled as 0f 85 7b 03 00 00  jnz 0x00E5EBC3
    8b 83 f8 13 01 00      mov eax,[ebx+113F8h]  ; dead from here on: the "1sak1", "1gld1", "2rag2",
                                                  ; "1eve1" and "1pk1" checks, then
                                                  ; ShowErrorDialog(g_runPatcherMessage) + return 0
The source keeps the compiled `if (!strstr(commandLine, "1rag1")) { ... }` chain; the jnz target is the
code after it. Without this patch the client refuses to start unless launched with one of the six
arguments.
"""

from postprocess import PatchError

SYMBOL = "_WinMain@16"
KEY = "68 b3 77 c9 6b 68 03 35 99 b5"
CHECK = "85 c0 0f 85 ?? ?? ?? ??"


def apply(image):
    start, end = image.function(SYMBOL)
    key = image.find(start, end, KEY)
    test = image.find(key, key + 0x40, CHECK)
    jnz = test + 2
    rel = int.from_bytes(image.read(jnz + 2, 4), "little", signed=True)
    target = jnz + 6 + rel
    if not start <= target < end:
        raise PatchError(f"jnz target {target:#x} lies outside {SYMBOL}")
    image.write(jnz, b"\x90\xe9" + image.read(jnz + 2, 4), expect=b"\x0f\x85" + image.read(jnz + 2, 4))
    return f"jmp {jnz:#x} -> {target:#x} (skip the 1rag1/1sak1/... argument checks)"
