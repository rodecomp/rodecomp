// CGGHash methods whose bodies are virtualised in every known Ragexe build
// (02-19: Load jumps out at 0x004013A9, Check at 0x00401641, VerifyHash at 0x00401786).
// Linked only into the standalone exe built by scripts/exe/build_exe.py; Clank never
// compiles this file.
//
// Not rebuilt: in 02-19 the RagHash.dat check is unreachable. Every caller leads back to
// fn_08197 (0x0076BE2A, creates CGGHashMgr), fn_08192 (0x0076B873) or fn_08194
// (0x0076B973), and none of those has an xref, call, jump, pointer or RVA in the image.
// No build or open project has the original logic, so these only trap if ever reached.

#include "Utility/Hash/GGHash.h"

#include <intrin.h>

BOOL CGGHash::Load(const wchar_t*) {
  __debugbreak();
  return FALSE;
}

int CGGHash::Check() {
  __debugbreak();
  return 0;
}

int CGGHash::VerifyHash(const wchar_t*, DWORD) {
  __debugbreak();
  return 0;
}
