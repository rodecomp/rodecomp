#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_LANGUAGETC_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_LANGUAGETC_H

#include <array>
#include <cstdint>
#include <list>
#include <string>

struct TraditionalKeyMapping {
  /* 0000 */ std::uint8_t category;
  /* 0001 */ std::uint8_t bytes[3];
};

struct CLanguageTC {
  std::list<std::uint8_t>& keys;
  std::array<TraditionalKeyMapping, 256>& mappings;
  std::string& display;
  std::uint8_t& unknown04C1;

  void InitializeMappings();
  void Key(int virtualKey);
  void DefaultKey(int virtualKey);
  const char* Display();
  const char* DisplayText() const;
  int DisplayLength() const;
  const char* NextChar(const char* current, int unused);
  const char* NextCharArabic(const char* current, int unused);
  int GetKeyStrokeSize() const;
  void ClearKeys();
  void ResetState();
  void ResetCandidate();
  void RetainSingleKey();
  void FilterCandidate(std::string& candidate);
};

static_assert(sizeof(TraditionalKeyMapping) == 4);

#endif
