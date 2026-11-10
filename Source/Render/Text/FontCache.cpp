#include "Render/Text/FontCache.h"

#include <algorithm>
#include <atlconv.h>
#include <cstring>
#include <iterator>
#include <string>
#include <tuple>

namespace {
  // STATIC: 0x01311F00
  const char* const primary[] = {
    "\xB1\xBC\xB8\xB2",
    "Arial",
    "\x82l\x82r \x82o\x83S\x83V\x83"
    "b\x83N",
    "\xCB\xCE\xCC\xE5",
    "\xB2\xD3\xA9\xFA\xC5\xE9",
    "Tahoma",
    "Arial",
    "Verdana",
    "Verdana",
    "Arial",
    "Arial",
    "Tahoma"
  };
  // STATIC: 0x01311ED0
  const char* const fallback[] = {"Gulim", "Arial", "MS Gothic", "SimSun", "MingLiU", "Tahoma", "Arial", "Verdana", "Verdana", "Arial", "Arial", "Tahoma"};

  // OFFSET: 0x0054F2C0
  // FUNCTION: fn_01956
  int CALLBACK MatchFont(const LOGFONTA* font, const TEXTMETRICA*, DWORD, LPARAM name) {
    return _stricmp(reinterpret_cast<const char*>(name), font->lfFaceName);
  }
}

// GLOBAL: gl_24541
FontCache g_fontCache;

// OFFSET: 0x0054D530
// FUNCTION: fn_01935
FontCache::FontCache() {
  Gdiplus::GdiplusStartup(&this->m_gdiplus, &this->m_startup, nullptr);
}

// OFFSET: 0x0100F120
// FUNCTION: fn_29519
FontCache::~FontCache() {
  for (const FontEntry& entry : this->m_fonts) {
    DeleteObject(entry.font);
    entry.RemovePrivateFont();
  }

  this->m_fonts.clear();
  Gdiplus::GdiplusShutdown(this->m_gdiplus);
}

// OFFSET: 0x0054DC90
// FUNCTION: fn_01947
__declspec(noinline) void FontEntry::CreateSystemFont() {
  LOGFONTA description = {};
  description.lfCharSet = static_cast<BYTE>(this->charset);

  const char* face;
  if (EnumFontFamiliesExA(GetDC(nullptr), &description, MatchFont, reinterpret_cast<LPARAM>(primary[this->family]), 0) == 0) {
    face = primary[this->family];
  } else if (EnumFontFamiliesExA(GetDC(nullptr), &description, MatchFont, reinterpret_cast<LPARAM>(fallback[this->family]), 0) == 0) {
    face = fallback[this->family];
  } else {
    face = nullptr;
  }

  this->font = CreateFontA(
    this->height, 0, 0, 0, this->bold ? FW_BOLD : FW_NORMAL, this->italic != 0, 0, 0, this->charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face
  );
}

// OFFSET: 0x0054DD70
// FUNCTION: fn_01948
HFONT __stdcall CreateEmbeddedFont(int height, std::int16_t family) {
  LOGFONTA description = {};
  description.lfWeight = FW_NORMAL;
  description.lfCharSet = DEFAULT_CHARSET;
  description.lfOutPrecision = OUT_RASTER_PRECIS;
  description.lfQuality = PROOF_QUALITY;

  if (family == 20) {
    std::strcpy(description.lfFaceName, "RixSquirrel_10.eot");
  } else if (family == 21) {
    std::strcpy(description.lfFaceName, "RixLoveangel_10.eot");
  } else if (family == 22) {
    std::strcpy(description.lfFaceName, "RixNHCgogo_10.eot");
  } else if (family == 23) {
    std::strcpy(description.lfFaceName, "RixDiary_10.eot");
  } else if (family == 24) {
    std::strcpy(description.lfFaceName, "RixMiniHeart_10.eot");
  } else if (family == 25) {
    std::strcpy(description.lfFaceName, "RixFreshman_10.eot");
  } else if (family == 26) {
    std::strcpy(description.lfFaceName, "RixKid_10.eot");
  } else if (family == 27) {
    std::strcpy(description.lfFaceName, "RixMagic_10.eot");
  } else if (family == 28) {
    std::strcpy(description.lfFaceName, "RixJJangu_10.eot");
  } else {
    std::strcpy(description.lfFaceName, "RixNHCgogo_10.eot");
  }

  description.lfHeight = -MulDiv(13, 72, 72);
  return CreateFontIndirectA(&description);
}

// OFFSET: 0x0054DEE0
// FUNCTION: fn_01949
__declspec(noinline) bool FontEntry::CreatePrivateFont() {
  std::string path;
  if (this->family == 29) {
    if (this->bold == 1) {
      path = "system\\font\\SCDream6.otf";
    } else {
      path = "system\\font\\SCDream4.otf";
    }
  }

  this->privateResources = AddFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
  if (this->privateResources != 0) {
    LOGFONTA description = {};
    description.lfHeight = -MulDiv(this->height, 72, 72);
    description.lfWeight = this->bold ? FW_BOLD : FW_NORMAL;
    description.lfItalic = this->italic != 0;
    description.lfCharSet = static_cast<BYTE>(this->charset);
    strncpy_s(description.lfFaceName, PrivateFontFamilyName(path).c_str(), _TRUNCATE);
    this->font = CreateFontIndirectA(&description);
  }

  return this->privateResources > 0;
}

// OFFSET: 0x0054E140
// FUNCTION: fn_01950
void FontEntry::RemovePrivateFont() const {
  if (this->privateResources > 0 && this->family == 29) {
    if (this->bold == 1) {
      const std::string path("system\\font\\SCDream6.otf");
      RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
    } else {
      const std::string path("system\\font\\SCDream4.otf");
      RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
    }
  }
}

// OFFSET: 0x0054F360
// FUNCTION: fn_01958
HFONT FontCache::Get(int family, int height, unsigned charset, unsigned char bold, unsigned char italic) {
  FontEntry entry{family, height, charset, bold, italic, nullptr, 0};
  const auto found = std::find_if(this->m_fonts.begin(), this->m_fonts.end(), [&entry](const FontEntry& font) {
    return font.family == entry.family && font.height == entry.height && font.charset == entry.charset && font.bold == entry.bold &&
           font.italic == entry.italic;
  });
  if (found != this->m_fonts.end()) {
    return found->font;
  }

  if (family == 20) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixSquirrel_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 21) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixLoveangel_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 22) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixNHCgogo_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 23) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixDiary_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 24) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixMiniHeart_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 25) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixFreshman_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 26) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixKid_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 27) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixMagic_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 28) {
    LOGFONTA description = {};
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    std::strcpy(description.lfFaceName, "RixJJangu_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family != 29 || !entry.CreatePrivateFont()) {
    entry.CreateSystemFont();
  }

  this->m_fonts.push_back(entry);

  std::sort(this->m_fonts.begin(), this->m_fonts.end(), [](const FontEntry& a, const FontEntry& b) {
    return std::tie(a.family, a.height, a.charset) < std::tie(b.family, b.height, b.charset);
  });
  return entry.font;
}

extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

// OFFSET: 0x0054F830
// FUNCTION: fn_01959
int FallbackFontIndex(const std::string& name) {
  const char* const* entry = std::begin(fallback);

  for (; entry != std::end(fallback); ++entry) {
    if (name == *entry) {
      break;
    }
  }

  return entry != std::end(fallback) ? static_cast<int>(entry - std::begin(fallback)) : 0;
}

// OFFSET: 0x0054F8A0
// FUNCTION: fn_01960
std::string __stdcall PrivateFontFamilyName(const std::string& path) {
  std::wstring wide;
  wide.assign(path.begin(), path.end());
  Gdiplus::PrivateFontCollection collection;
  collection.AddFontFile(wide.c_str());
  Gdiplus::FontFamily family;
  int found = 0;
  collection.GetFamilies(1, &family, &found);
  WCHAR name[LF_FACESIZE];
  std::memset(name, 0, sizeof(name));
  family.GetFamilyName(name, 0);
  return std::string(ATL::CW2A(name, CP_THREAD_ACP));
}
