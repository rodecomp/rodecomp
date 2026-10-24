#ifndef RAGNAROK_SOURCE_CLIENT_IMEINPUT_H
#define RAGNAROK_SOURCE_CLIENT_IMEINPUT_H

#include "Client/TextInputBuffer.h"

#include <cstdint>
#include <imm.h>
#include <vector>

struct TraditionalKeyInput;

// Borrowed CLanguage fields, with optional concrete CLanguageTC key view.
// Base/TH/ARB composition hooks are established empty entries.
struct ImeInput {
  TextInputBuffer& input;
  std::uint16_t& language;
  unsigned& codePage;
  const bool& languageCodePage;
  const unsigned& fallbackCodePage;
  std::vector<unsigned char>& candidateStorage;
  CANDIDATELIST*& candidateList;
  unsigned& selection;
  unsigned& pageSize;
  int& compositionCursor;
  TraditionalKeyInput* traditional;

  // Original ignores the passed HWND and uses the global main-window handle
  // borrowed by input. Flags are truncated to the low WORD.
  LRESULT Message(HWND unused, UINT message, WPARAM value, LPARAM flags, std::uint8_t& handled);
  // Shared 256-byte output; later successful lookups overwrite prior strings.
  // Original checks index < count only (no negative-index guard).
  const char* Candidate(int index) const;
};

#endif
