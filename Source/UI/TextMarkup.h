#ifndef RAGNAROK_SOURCE_UI_TEXTMARKUP_H
#define RAGNAROK_SOURCE_UI_TEXTMARKUP_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

// Original link record is 60 bytes on x86. The two words after the byte
// position are not initialized by the extractor; do not infer meanings.
struct TextLink {
  std::uint32_t position;
  std::byte unknown_0004[8];
  std::string display;
  std::string metadata;
};

bool HasTextLinks(const std::string& text);
std::string ExpandTextEscapes(std::string text);
std::string RemoveTextColors(const char* text, unsigned short codePage);
std::string DisplayTextTag(std::string text, const std::string& begin, const std::string& end);
std::string RemoveTextLinkInfo(std::string text);
void ExtractTextLinks(std::string text, const std::string& begin, const std::string& end, std::vector<TextLink>& links);
void SplitTextLinkArguments(std::string text, std::vector<std::string>& arguments);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TextLink) == 60);
#endif

#endif
