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

extern const std::string g_naviTagBegin;
extern const std::string g_naviTagEnd;
extern const std::string g_itemTagBegin;
extern const std::string g_itemTagEnd;
extern const std::string g_tipBoxTagBegin;
extern const std::string g_tipBoxTagEnd;
extern const std::string g_questTagBegin;
extern const std::string g_questTagEnd;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TextLink) == 60);
static_assert(offsetof(TextLink, position) == 0x0);
static_assert(offsetof(TextLink, padding_0004) == 0x4);
static_assert(offsetof(TextLink, display) == 0xC);
static_assert(offsetof(TextLink, metadata) == 0x24);
#endif

#endif
