#ifndef RAGNAROK_SOURCE_UI_TEXT_TEXTMARKUP_H
#define RAGNAROK_SOURCE_UI_TEXT_TEXTMARKUP_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct TextLink {
  /* 0000 */ std::uint32_t position;
  /* 0004 */ std::byte padding_0004[8];
  /* 000C */ std::string display;
  /* 0024 */ std::string metadata;
};

bool HasTextLinks(const std::string& text);
std::string ExpandTextEscapes(const std::string& text);
std::string RemoveTextColors(const char* text);
const std::string DisplayTextTag(const std::string& source, const std::string& begin, const std::string& end);
const std::string DisplayItemTextTag(const std::string& text);
const std::string DisplayNavigationTextTag(const std::string& text);
const std::string DisplayQuestTextTag(const std::string& text);
const std::string DisplayTipBoxTextTag(const std::string& text);
const std::string DisplayUrlTextTag(const std::string& text);
const std::string RemoveTextLinkInfo(const std::string& source);
void __stdcall ExtractTextLinks(const std::string& source, const std::string& begin, const std::string& end, std::vector<TextLink>& links);
void __stdcall ExtractNavigationTextLinks(const std::string& text, std::vector<TextLink>& links);
void ExtractTipBoxTextLinks(const std::string& text, std::vector<TextLink>& links);
void ExtractUrlTextLinks(const std::string& text, std::vector<TextLink>& links);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TextLink) == 60);
static_assert(offsetof(TextLink, position) == 0x0);
static_assert(offsetof(TextLink, padding_0004) == 0x4);
static_assert(offsetof(TextLink, display) == 0xC);
static_assert(offsetof(TextLink, metadata) == 0x24);
#endif

#endif
