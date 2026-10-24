# Coding rules

These apply to rodecomp: Ragnarok Online Ragexe 2026-02-19 (x86), built with MSVC v142 19.29.30159.
`AGENTS.md` and the current server response's `guidance` take precedence over this file.

## 1. No comments in the code

Code carries no explanatory comments: none inside functions, none above them, no file headers, no
commented-out code. Allowed comments are Clank stable-ID annotations (`// FUNCTION:`, `// GLOBAL:`,
`// STRING:`, `// VTABLE:`), existing `// OFFSET:` / `// STATIC:` provenance, `// INLINED` markers,
and field-offset markers (`/* XXXX */`). Use only string literals evidenced in the original executable.

Put explanations in submission notes rather than adding source comments or creating documentation by default.
Preserve existing provenance. If the submission gate rejects it, report the policy conflict rather than
stripping it or disabling the gate. This document does not change the server's accepted syntax.

## 2. Stable IDs identify implementations and data

Every reconstructed standalone function gets its existing stable ID directly above its definition:

```cpp
// FUNCTION: fn_00123
void Player::Update(float dt) {
    ...
}
```

- Stable-ID annotations contain IDs, never addresses. Addresses live in the symbol database.
- Preserve existing `// OFFSET: 0x...` and `// STATIC: 0x...` provenance; do not invent addresses or
  require new address comments as a substitute for stable IDs.
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

### `// INLINED:` for functions that exist only inlined

A function that exists only inlined is marked `// INLINED`, not given an invented standalone function ID:

```cpp
// INLINED
static void BuildFullKeyName(const char* keyname, char* buffer) {
    ...
}
```

- Preserve existing evidenced inline ranges when present. Do not guess ranges for interleaved instructions.
- When the copies cannot be delimited (many copies interleaved with surrounding code, or a copy that compiles to
  no instructions), write a bare `// INLINED` without ranges.
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
- Do not add WoW `// VERIFY:` hashes or use `re/tools/verify.py`; neither is this project's verification workflow.
- Claims, submission scope and peer-review procedure follow `AGENTS.md` and current server guidance.

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
- Keep dependencies small and avoid cycles. Do not introduce WoW-specific DB or event headers.

## 6. Diagnostic file and line arguments preserve original behavior

Preserve evidenced diagnostic strings, file paths and line values when the original passes them explicitly.
Do not substitute `__FILE__` or `__LINE__` merely to follow a different project's convention, and do not
invent diagnostic literals. Distinguish compiler-generated diagnostics from application arguments using
the original instructions.

## 7. Platform and build scope

- This target is Windows x86. Keep code in its assigned existing translation unit and subsystem headers.
- Use the original Win32, DirectDraw, Direct3D, Lua and codec ABIs where evidenced. Preserve calling
  conventions, ownership, field widths and failure behavior.
- Do not introduce WoW architecture macros, parallel `Win32/` / `Win64/` trees or a new portability layer.
- Compile individual units with the configured Clank toolchain and `compile.json`, not a new build system.
- Missing original instructions are a blocker, not permission to invent a fallback or partial implementation.
