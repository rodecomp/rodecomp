#include "Render/Text/FontCache.h"

#include <algorithm>
#include <atlconv.h>
#include <cstring>
#include <iterator>
#include <string>

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

// OFFSET: 0x0054D890
// FUNCTION: fn_46418
FontCache::~FontCache() {
  for (auto entry = this->m_fonts.begin(); entry != this->m_fonts.end(); ++entry) {
    DeleteObject(entry->font);
    entry->RemovePrivateFont();
  }

  this->m_fonts.clear();
  Gdiplus::GdiplusShutdown(this->m_gdiplus);
}

// OFFSET: 0x0054D4F0
// FUNCTION: fn_46417
FontEntry::FontEntry(int family, int height, int charset, unsigned char bold, unsigned char italic)
    : family(family), height(height), charset(charset), bold(bold), italic(italic), font(nullptr), privateResources(0) {
}

// OFFSET: 0x0054D980
// FUNCTION: fn_46419
bool FontEntry::operator==(const FontEntry& other) const {
  return this->family == other.family && this->height == other.height && this->charset == other.charset &&
         this->bold == other.bold && this->italic == other.italic;
}

// OFFSET: 0x0054D9C0
// FUNCTION: fn_46420
bool FontEntry::operator<(const FontEntry& other) const {
  if (this->family < other.family) {
    return true;
  }
  if (this->family > other.family) {
    return false;
  }
  if (this->height < other.height) {
    return true;
  }
  if (this->height > other.height) {
    return false;
  }
  return this->charset < other.charset;
}

// OFFSET: 0x0054F770
// FUNCTION: fn_45913
const char* FontEntry::FindSystemFace() const {
  LOGFONTA description = {};
  description.lfCharSet = static_cast<BYTE>(this->charset);

  if (EnumFontFamiliesExA(GetDC(nullptr), &description, MatchFont, reinterpret_cast<LPARAM>(primary[this->family]), 0) == 0) {
    return primary[this->family];
  }
  if (EnumFontFamiliesExA(GetDC(nullptr), &description, MatchFont, reinterpret_cast<LPARAM>(fallback[this->family]), 0) == 0) {
    return fallback[this->family];
  }
  return nullptr;
}

// OFFSET: 0x0054DC90
// FUNCTION: fn_01947
__declspec(noinline) void FontEntry::CreateSystemFont() {
  const char* face = this->FindSystemFace();
  const DWORD italic = this->italic != 0;
  const int weight = this->bold ? FW_BOLD : FW_NORMAL;
  this->font = CreateFontA(
    this->height, 0, 0, 0, weight, italic, 0, 0, this->charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face
  );
}

// OFFSET: 0x0054DD70
// FUNCTION: fn_01948
__declspec(noinline) HFONT __stdcall CreateEmbeddedFont(int height, std::int16_t family) {
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
    strncpy_s(description.lfFaceName, this->PrivateFontFamilyName(path).c_str(), _TRUNCATE);
    this->font = CreateFontIndirectA(&description);
  }

  return this->privateResources > 0;
}

// OFFSET: 0x0054E120
// FUNCTION: fn_45912
void FontEntry::LoadEmbeddedFont(std::int16_t familyIndex) {
  this->font = CreateEmbeddedFont(12, familyIndex);
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
  FontEntry entry(family, height, static_cast<int>(charset), bold, italic);
  const auto found = std::find(this->m_fonts.begin(), this->m_fonts.end(), entry);
  if (found != this->m_fonts.end()) {
    return found->font;
  }

  if (family == 20) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixSquirrel_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 21) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixLoveangel_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 22) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixNHCgogo_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 23) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixDiary_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 24) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixMiniHeart_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 25) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixFreshman_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 26) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixKid_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 27) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixMagic_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family == 28) {
    LOGFONTA description;
    description.lfHeight = 0;
    description.lfWidth = 0;
    description.lfEscapement = description.lfOrientation = 0;
    description.lfWeight = FW_NORMAL;
    description.lfItalic = 0;
    description.lfUnderline = 0;
    description.lfStrikeOut = 0;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    description.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    for (char& character : description.lfFaceName) {
      character = 0;
    }
    std::strcpy(description.lfFaceName, "RixJJangu_10.eot");
    description.lfHeight = -MulDiv(13, 72, 72);
    entry.font = CreateFontIndirectA(&description);
  } else if (family != 29 || !entry.CreatePrivateFont()) {
    entry.CreateSystemFont();
  }

  this->m_fonts.push_back(entry);

  std::sort(this->m_fonts.begin(), this->m_fonts.end());
  return entry.font;
}

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
std::string FontEntry::PrivateFontFamilyName(const std::string& path) const {
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
