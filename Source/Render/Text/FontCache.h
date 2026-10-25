#ifndef RAGNAROK_SOURCE_RENDER_TEXT_FONTCACHE_H
#define RAGNAROK_SOURCE_RENDER_TEXT_FONTCACHE_H

#include <algorithm>
#include <vector>
#include <windows.h>

namespace Gdiplus {
  using std::max;
  using std::min;
}

struct FontEntry {
  int family;
  int height;
  unsigned charset;
  unsigned char bold;
  unsigned char italic;
  HFONT font;
  int privateResources;
};

class FontCache {
public:
  FontCache();
  ~FontCache();
  HFONT Get(int family, int height, unsigned charset, unsigned char bold, unsigned char italic);

private:
  std::vector<FontEntry> m_fonts;
  ULONG_PTR m_gdiplus;
  static void CreateSystemFont(FontEntry& entry);
  static bool CreatePrivateFont(FontEntry& entry);
  FontCache(const FontCache&) = delete;
  FontCache& operator=(const FontCache&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FontEntry) == 24);
#endif

#endif
