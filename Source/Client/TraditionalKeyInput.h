#ifndef RAGNAROK_SOURCE_CLIENT_TRADITIONALKEYINPUT_H
#define RAGNAROK_SOURCE_CLIENT_TRADITIONALKEYINPUT_H

#include <array>
#include <cstdint>
#include <list>
#include <string>

struct TraditionalKeyMapping {
  std::uint8_t category;
  std::uint8_t bytes[3];
};

// Borrowed CLanguageTC fields; not a substitute for its base-language/IME
// lifecycle. Key sequence is +0xB8, mappings +0xC0, display +0x58, flag +0x4C1.
struct TraditionalKeyInput {
  std::list<std::uint8_t>& keys;
  std::array<TraditionalKeyMapping, 256>& mappings;
  std::string& display;
  std::uint8_t& unknown04C1;

  // Unmapped byte triples are deliberately untouched; their category becomes 4.
  void InitializeMappings();
  void Key(int virtualKey);
  void DefaultKey(int virtualKey);
  const char* Display();
  const char* DisplayText() const;
  int DisplayLength() const;
  const char* NextChar(const char* current, int unused);
  const char* NextCharArabic(const char* current, int unused);
  int Count() const;
  void ClearKeys();
  void ResetState();
  void ResetCandidate();
  void RetainSingleKey();
  void FilterCandidate(std::string& candidate);
};

static_assert(sizeof(TraditionalKeyMapping) == 4);

#endif
