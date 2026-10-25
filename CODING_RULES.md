# Coding rules

These apply to rodecomp: Ragnarok Online Ragexe 2026-02-19 (x86), built with MSVC v142 19.29.30159.
`AGENTS.md` and the current server response's `guidance` take precedence over this file.

## 1. No comments in the code

Code carries no explanatory comments: none inside functions, none above them, no file headers, no
commented-out code. Allowed comments are Clank stable-ID annotations (`// FUNCTION:`, `// GLOBAL:`,
`// STRING:`, `// VTABLE:`), existing `// OFFSET:` / `// STATIC:` provenance where permitted below,
exact bare `// INLINED` markers, and field-offset markers (`/* XXXX */`). Use only string literals evidenced in the original executable.

Put explanations in submission notes rather than adding source comments or creating documentation by default.
Preserve existing provenance except `OFFSET` annotations attached to inline-only functions, which must be removed.
If the submission gate rejects otherwise permitted provenance, report the policy conflict rather than stripping it
or disabling the gate. This document does not change the server's accepted syntax.

## 2. Stable IDs identify implementations and data

Every reconstructed standalone function gets its existing stable ID directly above its definition:

```cpp
// FUNCTION: fn_00123
void Player::Update(float dt) {
    ...
}
```

- Stable-ID annotations contain IDs, never addresses. Addresses live in the symbol database.
- Preserve existing `// OFFSET: 0x...` provenance for standalone functions and `// STATIC: 0x...` provenance
  for data. Inline-only functions must not carry `OFFSET` provenance. Do not invent addresses or require
  new address comments as a substitute for stable IDs.
- Use the existing ID of the actual original function. Do not annotate an ordinary destructor with a
  scalar-deleting destructor's ID, or stack a thunk's ID onto its target.
- For symbols missing from the database, use `td_new_symbol` and its returned annotation.
- Declarations do not receive implementation annotations. Preserve existing declaration provenance rather
  than rewriting unrelated code solely for style.
- Prefer non-template definitions in `.cpp` files; retain existing inline accessors and helpers where required
  by the original code and established repository patterns. Do not move bodies merely to satisfy style.
- Shared types, prototypes and data declarations belong in the appropriate existing header. File-local
  helpers and private types may remain in their `.cpp`; do not introduce a second declaration convention.

Variables with static storage use the stable annotation returned by the symbol database:

```cpp
// GLOBAL: gl_00045
int g_resourceReuseDelay;
```

- Preserve existing `STATIC` provenance on data definitions; it is not a replacement for a stable ID.
- Keep file-scope definitions near the top of the `.cpp`, after includes. Tables that depend on local helper
  definitions may follow those helpers. Preserve original initialization and teardown ordering.
- Keep function-local statics local when that matches the original lifetime.
- `#include` lines always go above everything else in the file (after the include guard in a header). A later pass
  that needs a new header adds it to the include block at the top, never next to the new code.
- Helper functions used only by one `.cpp` may stay `static` there, defined below the statics; they need no header
  declaration.

### `// INLINED` for functions that exist only inlined

A function that exists only inlined has exactly one annotation, the bare line `// INLINED`, immediately above its definition:

```cpp
// INLINED
static void BuildFullKeyName(const char* keyname, char* buffer) {
    ...
}
```

- `// INLINED` has no colon, address, range, stable ID, enclosing-function name, or other suffix.
- An inline-only function's annotation block contains only the bare marker: no `// OFFSET:`, `// FUNCTION:`,
  or other provenance comments. Remove other annotations when classifying a function as inline-only; preserve
  supporting addresses and enclosing function IDs in review/submission notes, not source comments.
  This overrides general provenance-preservation rules.
- Do not invent a standalone ID or mark a source-only helper inline without original evidence.
- Inlined code is checked through its callers, not submitted as an invented standalone original.

## 2a. Verification uses Clank

- Read `td_disassemble` as ground truth and `td_decompile` as a starting point, not a specification.
- Run `td_check` on each changed translation unit independently while fixing compilation errors.
- After changing a header, check its includers before submitting. Compile using `compile.json`; Clank does
  not run CMake, CTest or a complete application link during these checks.
- Validate the exact proposed payload against clean current server main, without unrelated working-tree
  changes. Include the real prerequisite interfaces, implementations and affected callers, or land them first.
- Preserve functions that already match. Fix compile errors and structural differences; byte identity is a
  bonus, not permission to distort source or spend unbounded time tuning it.
- Do not add game `// VERIFY:` hashes or use `re/tools/verify.py`; neither is this project's verification workflow.
- Claims, submission scope and peer-review procedure follow `AGENTS.md` and current server guidance.

## 2b. Cast chains mean the types are wrong

A chain of casts (`static_cast<int>(static_cast<std::uint32_t>(...))`, `std::bit_cast` round trips) usually
reproduces the compiler's output instead of the original source. Fix the declared types of the field,
parameter or return value so the code reads naturally.

```cpp
// Wrong: hides that the value is stored as a 32-bit int
return static_cast<int>(static_cast<std::uint32_t>(this->render.instant.Get(key)));

// Right: the getter returns the original type
return this->render.instant.Get(key);
```

Keep a cast only where the original source evidently had one (an explicit narrowing or a sign change).

## 3. Every field carries its offset

Every recovered instance data member of a struct, class or union is prefixed with its evidenced x86 offset:

```cpp
class CResourceCache {
    public:
    /* 0000 */ int32_t m_refCount;
    /* 0004 */ CTexture* m_texture;
    /* 0008 */ CRes* m_resource;
};
```

- Format: `/* XXXX */ type name;`, four uppercase hex digits without `0x`, and more digits only
  when the offset needs them (`/* 1A2B0 */`).
- Preserve established names. For new fields, prefer descriptive names when supported by evidence;
  otherwise retain offset-based names such as `unknown_0010` or the local equivalent.
- Offsets are those of Ragexe 2026-02-19's x86 layout. Verify known sizes and offsets with x86 assertions;
  do not invent complete layouts from a recovered prefix or add unsupported x64 assumptions.

## 4. Member order in a class

For new declarations, prefer the following grouping, without reordering existing fields or virtual slots:

1. static fields
2. static functions
3. fields
4. virtual functions
5. functions

Nested types may precede the groups that use them. Original data layout and virtual-table order always
take precedence over declaration style; avoid unrelated restyling.

```cpp
class CResourceCache {
    public:
    static uint32_t s_optFlags;

    static void AnimateThread(void* arg);

    /* 0000 */ int32_t m_refCount;
    /* 0004 */ CTexture* m_texture;

    virtual ~CResourceCache();

    void AdvanceTime(uint32_t delta);
};
```

## 5. Shared types follow existing subsystem headers

- Reuse the appropriate header under `Source/` for shared enums and data types.
- Use an existing `Types.h` if the subsystem has one; do not create a repository-wide `Types.h` migration.
- Keep dependencies small and avoid cycles. Do not introduce game-specific DB or event headers.

## 6. Diagnostic file and line arguments preserve original behavior

Preserve evidenced diagnostic strings, file paths and line values when the original passes them explicitly.
Do not substitute `__FILE__` or `__LINE__` merely to follow a different project's convention, and do not
invent diagnostic literals. Distinguish compiler-generated diagnostics from application arguments using
the original instructions.

## 7. Platform and build scope

- This target is Windows x86. Keep code in its assigned existing translation unit and subsystem headers.
- Use the original Win32, DirectDraw, Direct3D, Lua and codec ABIs where evidenced. Preserve calling
  conventions, ownership, field widths and failure behavior.
- Compile individual units with the configured Clank toolchain and `compile.json`, not a new build system.
- Missing original instructions are a blocker, not permission to invent a fallback or partial implementation.

## 8. Reconstruct source, not instructions

- Prefer the original programmer's types and algorithms over instruction-shaped C++. Byte identity is a bonus,
  never a reason for cryptic code. Use compiler intrinsics only with clear evidence that the original source
  used them; an optimized SIMD instruction alone is not evidence. Scalar data is not `__m128i`.
- Recover records and members instead of dereferencing integer pointers cast onto byte buffers. At a real
  binary-format boundary, use a typed record or `memcpy` for unaligned fields; preserve layout and aliasing.
- Apply namespace and API conventions consistently throughout a subsystem, including declarations and callers.
- Group cohesive implementations and their headers in subsystem folders, e.g. `Client/Browser/`. Group by
  actual ownership and dependencies, not a loose keyword; update includes and build metadata with every move.
- Use SDK/library constants and identifiers directly: `WS_CHILD | WS_VISIBLE`, `CGID_DocHostCommandHandler`.
  Do not duplicate documented definitions. Name game-specific choices with an enum, or `constexpr` when an enum
  is unsuitable; do not turn serialized data, hashes or lookup tables into invented symbolic constants.
- Do not include `<mmsystem.h>`; include the specific Windows API headers actually required. Formatter-control
  comments are forbidden too. Preserve include dependencies without `clang-format` suppression comments.
- Separate different logical phases with a blank line; keep related assignments together. Use a 160-column
  limit, rather than wrapping short calls into instruction-like fragments. Formatting must not change behavior.
- Leave exactly one blank line after an include block before declarations, including in headers. Never keep
  more than one consecutive blank line; whitespace-only lines count as blank.

### Reconstruct missing types

Repeated offset reads or consecutive field-sized `memcpy`s on flat integers/buffers suggest a missing struct.
Reconstruct it when the data forms a logical object; use named members, verify layout, and copy whole records
where appropriate. Preserve packing, byte order and aliasing; do not invent structs for unrelated scalars.

```cpp
#pragma pack(push, 1)
struct PakEntryFile {
  /* 0000 */ std::uint32_t compressedSize;
  /* 0004 */ std::uint32_t alignedSize;
  /* 0008 */ std::uint32_t originalSize;
  /* 000C */ std::uint8_t flags;
  /* 000D */ std::uint32_t dataOffset;
};
#pragma pack(pop)

static_assert(sizeof(PakEntryFile) == 17);
static_assert(offsetof(PakEntryFile, dataOffset) == 13);

PakEntryFile fileEntry;
std::memcpy(&fileEntry, metadata, sizeof(fileEntry));
cursor = metadata + sizeof(fileEntry);
entry.compressedSize = fileEntry.compressedSize;
entry.dataOffset = fileEntry.dataOffset;
```
