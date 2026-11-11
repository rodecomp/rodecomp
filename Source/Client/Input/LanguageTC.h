#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_LANGUAGETC_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_LANGUAGETC_H

#include "Client/Input/Language.h"

#include <array>
#include <cstdint>
#include <list>
#include <string>

struct TraditionalKeyMapping {
  /* 0000 */ std::uint8_t category;
  /* 0001 */ std::uint8_t bytes[3];
};

// VTABLE: vt_01605
struct CLanguageTC : CLanguageBase {
  /* 00B8 */ std::list<std::uint8_t> keys;
  /* 00C0 */ std::array<TraditionalKeyMapping, 256> mappings;
  /* 04C0 */ unsigned char keyStrokeEnabled;
  /* 04C1 */ std::uint8_t keyStroke;
  /* 04C2 */ unsigned char padding_04C2[2];

  CLanguageTC();
  ~CLanguageTC() override;
  const char* DisplayText() const override;
  int DisplayLength() const override;
  void ProcessKey(unsigned char virtualKey) override;
  void RefreshComposition() override;
  void EndComposition() override;
  void ResetCandidate() override;

  int GetKeyStrokeSize() const;
};

static_assert(sizeof(TraditionalKeyMapping) == 4);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLanguageTC, keys) == 0xB8);
static_assert(offsetof(CLanguageTC, mappings) == 0xC0);
static_assert(offsetof(CLanguageTC, keyStroke) == 0x4C1);
static_assert(sizeof(CLanguageTC) == 0x4C4);
#endif

#endif
