#ifndef RAGNAROK_SOURCE_UI_TEXT_TEXTMARKUP_H
#define RAGNAROK_SOURCE_UI_TEXT_TEXTMARKUP_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct TextLink {
  /* 0000 */ std::uint32_t position;
  /* 0004 */ std::byte unknown_0004[8];
  /* 000C */ std::string display;
  /* 0024 */ std::string metadata;
};

bool HasTextLinks(const std::string& text);
std::string ExpandTextEscapes(std::string text);
std::string RemoveTextColors(const char* text, unsigned short codePage);
std::string DisplayTextTag(std::string text, const std::string& begin, const std::string& end);
std::string DisplayItemTextTag(const std::string& text);
std::string DisplayNavigationTextTag(const std::string& text);
std::string DisplayQuestTextTag(const std::string& text);
std::string RemoveTextLinkInfo(std::string text);
void ExtractTextLinks(std::string text, const std::string& begin, const std::string& end, std::vector<TextLink>& links);
void SplitTextLinkArguments(std::string text, std::vector<std::string>& arguments);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TextLink) == 60);
static_assert(offsetof(TextLink, position) == 0x0);
static_assert(offsetof(TextLink, unknown_0004) == 0x4);
static_assert(offsetof(TextLink, display) == 0xC);
static_assert(offsetof(TextLink, metadata) == 0x24);
#endif

#endif
