#ifndef RAGNAROK_SOURCE_RENDER_TEXT_FONTCACHE_H
#define RAGNAROK_SOURCE_RENDER_TEXT_FONTCACHE_H

#include <algorithm>
#include <cstddef>
#include <vector>
#include <windows.h>
#include <objidl.h>

namespace Gdiplus {
  using std::max;
  using std::min;
}

#include <gdiplus.h>

struct FontEntry {
  /* 0000 */ int family;
  /* 0004 */ int height;
  /* 0008 */ unsigned charset;
  /* 000C */ unsigned char bold;
  /* 000D */ unsigned char italic;
  /* 0010 */ HFONT font;
  /* 0014 */ int privateResources;
};

class FontCache {
public:
  FontCache();
  ~FontCache();
  HFONT Get(int family, int height, unsigned charset, unsigned char bold, unsigned char italic);

private:
  /* 0000 */ std::vector<FontEntry> m_fonts;
  /* 000C */ Gdiplus::GdiplusStartupInput m_startup;
  /* 001C */ ULONG_PTR m_gdiplus;
  static void CreateSystemFont(FontEntry& entry);
  static bool CreatePrivateFont(FontEntry& entry);
  FontCache(const FontCache&) = delete;
  FontCache& operator=(const FontCache&) = delete;

  friend struct FontCacheLayout;
};

struct FontCacheLayout {
#if defined(_WIN32) && !defined(_WIN64)
  static_assert(offsetof(FontCache, m_fonts) == 0x0000);
  static_assert(offsetof(FontCache, m_startup) == 0x000C);
  static_assert(offsetof(FontCache, m_gdiplus) == 0x001C);
#endif
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FontEntry) == 24);
static_assert(offsetof(FontEntry, family) == 0x0000);
static_assert(offsetof(FontEntry, height) == 0x0004);
static_assert(offsetof(FontEntry, charset) == 0x0008);
static_assert(offsetof(FontEntry, bold) == 0x000C);
static_assert(offsetof(FontEntry, italic) == 0x000D);
static_assert(offsetof(FontEntry, font) == 0x0010);
static_assert(offsetof(FontEntry, privateResources) == 0x0014);
#endif

#endif
