#ifndef RAGNAROK_SOURCE_UI_TEXTTAGS_H
#define RAGNAROK_SOURCE_UI_TEXTTAGS_H

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

// VTABLE: vt_01210
struct CTagPrototype {
  /* 0004 */ int type;
  /* 0008 */ std::string opening;
  /* 0020 */ std::string closing;

  virtual ~CTagPrototype();

  CTagPrototype(int type, std::string opening, std::string closing);
};

// VTABLE: vt_01213
struct STagString {
  /* 0004 */ int tag;
  /* 0008 */ std::size_t position;
  /* 000C */ std::string text;

  virtual ~STagString();

  STagString(int tag, std::size_t position, std::string text);
};

// INLINED
inline CTagPrototype::~CTagPrototype() {
}

// INLINED
inline CTagPrototype::CTagPrototype(int type, std::string opening, std::string closing) : type(type), opening(opening), closing(closing) {
}

// INLINED
inline STagString::~STagString() {
}

// INLINED
inline STagString::STagString(int tag, std::size_t position, std::string text) : tag(tag), position(position), text(std::move(text)) {
}

extern std::string navigationTagOpening;
extern std::string navigationTagClosing;
extern std::string itemTagOpening;
extern std::string itemTagClosing;
extern std::string itemLinkTagOpening;
extern std::string itemLinkTagClosing;
extern std::vector<CTagPrototype> g_textTagRegistry;

std::vector<CTagPrototype> BuildTextTagRegistry();
std::vector<STagString> ExtractTextTags(std::string text);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CTagPrototype, type) == 0x04);
static_assert(offsetof(CTagPrototype, opening) == 0x08);
static_assert(offsetof(CTagPrototype, closing) == 0x20);
static_assert(sizeof(CTagPrototype) == 0x38);
static_assert(offsetof(STagString, tag) == 0x04);
static_assert(offsetof(STagString, position) == 0x08);
static_assert(offsetof(STagString, text) == 0x0C);
static_assert(sizeof(STagString) == 0x24);
#endif

#endif
