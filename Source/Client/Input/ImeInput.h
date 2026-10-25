#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_IMEINPUT_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_IMEINPUT_H

#include "Client/Input/TextInputBuffer.h"

#include <cstdint>
#include <imm.h>
#include <vector>

struct TraditionalKeyInput;

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

  LRESULT Message(HWND unused, UINT message, WPARAM value, LPARAM flags, std::uint8_t& handled);

  const char* Candidate(int index) const;
};

#endif
