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

  // INLINED
  const char* PrivatePath(unsigned char bold) {
    return bold == 1 ? "system\\font\\SCDream6.otf" : "system\\font\\SCDream4.otf";
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

    if (entry.privateResources > 0 && entry.family == 29) {
      RemoveFontResourceExA(PrivatePath(entry.bold), FR_PRIVATE, nullptr);
    }
  }

  this->m_fonts.clear();
  Gdiplus::GdiplusShutdown(this->m_gdiplus);
}

// OFFSET: 0x0054DC90
// FUNCTION: fn_01947
void FontCache::CreateSystemFont(FontEntry& entry) {
  LOGFONTA description = {};
  description.lfCharSet = static_cast<BYTE>(entry.charset);
  const char* face = primary[entry.family];

  if (EnumFontFamiliesExA(GetDC(nullptr), &description, MatchFont, reinterpret_cast<LPARAM>(face), 0) != 0) {
    face = fallback[entry.family];
    if (EnumFontFamiliesExA(GetDC(nullptr), &description, MatchFont, reinterpret_cast<LPARAM>(face), 0) != 0) {
      face = nullptr;
    }
  }

  entry.font = CreateFontA(
    entry.height, 0, 0, 0, entry.bold ? FW_BOLD : FW_NORMAL, entry.italic != 0, 0, 0, entry.charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
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

  switch (family) {
  case 20:
    std::strcpy(description.lfFaceName, "RixSquirrel_10.eot");
    break;
  case 21:
    std::strcpy(description.lfFaceName, "RixLoveangel_10.eot");
    break;
  case 23:
    std::strcpy(description.lfFaceName, "RixDiary_10.eot");
    break;
  case 24:
    std::strcpy(description.lfFaceName, "RixMiniHeart_10.eot");
    break;
  case 25:
    std::strcpy(description.lfFaceName, "RixFreshman_10.eot");
    break;
  case 26:
    std::strcpy(description.lfFaceName, "RixKid_10.eot");
    break;
  case 27:
    std::strcpy(description.lfFaceName, "RixMagic_10.eot");
    break;
  case 28:
    std::strcpy(description.lfFaceName, "RixJJangu_10.eot");
    break;
  default:
    std::strcpy(description.lfFaceName, "RixNHCgogo_10.eot");
    break;
  }

  description.lfHeight = -MulDiv(13, 72, 72);
  return CreateFontIndirectA(&description);
}

// OFFSET: 0x0054DEE0
// FUNCTION: fn_01949
bool FontCache::CreatePrivateFont(FontEntry& entry) {
  const char* path = entry.family == 29 ? PrivatePath(entry.bold) : "";
  entry.privateResources = AddFontResourceExA(path, FR_PRIVATE, nullptr);
  if (entry.privateResources != 0) {
    LOGFONTA description = {};
    description.lfHeight = -MulDiv(entry.height, 72, 72);
    description.lfWeight = entry.bold ? FW_BOLD : FW_NORMAL;
    description.lfItalic = entry.italic != 0;
    description.lfCharSet = static_cast<BYTE>(entry.charset);

    std::wstring wide;

    for (const char* byte = path; *byte; ++byte) {
      wide.push_back(static_cast<wchar_t>(*byte));
    }

    Gdiplus::PrivateFontCollection collection;
    collection.AddFontFile(wide.c_str());
    Gdiplus::FontFamily family;
    int found = 0;
    collection.GetFamilies(1, &family, &found);
    WCHAR name[32] = {};
    family.GetFamilyName(name, 0);
    char narrow[128] = {};
    WideCharToMultiByte(CP_THREAD_ACP, 0, name, -1, narrow, sizeof(narrow), nullptr, nullptr);
    strncpy_s(description.lfFaceName, narrow, _TRUNCATE);
    entry.font = CreateFontIndirectA(&description);
  }

  return entry.privateResources > 0;
}

// OFFSET: 0x0054E140
// FUNCTION: fn_01950
void FontEntry::RemovePrivateFont() const {
  if (this->privateResources > 0 && this->family == 29) {
    const std::string path = this->bold == 1 ? std::string("system\\font\\SCDream6.otf") : std::string("system\\font\\SCDream4.otf");
    RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
  }
}

// OFFSET: 0x0054F360
// FUNCTION: fn_01958
HFONT FontCache::Get(int family, int height, unsigned charset, unsigned char bold, unsigned char italic) {
  for (const FontEntry& entry : this->m_fonts) {
    if (entry.family == family && entry.height == height && entry.charset == charset && entry.bold == bold && entry.italic == italic) {
      return entry.font;
    }
  }

  FontEntry entry{family, height, charset, bold, italic, nullptr, 0};
  if (family >= 20 && family <= 28) {
    static const char* const faces[] = {"RixSquirrel_10.eot", "RixLoveangel_10.eot", "RixNHCgogo_10.eot", "RixDiary_10.eot", "RixMiniHeart_10.eot",
                                        "RixFreshman_10.eot", "RixKid_10.eot",       "RixMagic_10.eot",   "RixJJangu_10.eot"};
    LOGFONTA description = {};
    description.lfHeight = -MulDiv(13, 72, 72);
    description.lfWeight = FW_NORMAL;
    description.lfCharSet = DEFAULT_CHARSET;
    description.lfOutPrecision = OUT_RASTER_PRECIS;
    description.lfQuality = PROOF_QUALITY;
    strcpy_s(description.lfFaceName, faces[family - 20]);
    entry.font = CreateFontIndirectA(&description);
  } else if (family != 29 || !CreatePrivateFont(entry)) {
    CreateSystemFont(entry);
  }

  this->m_fonts.push_back(entry);

  std::sort(this->m_fonts.begin(), this->m_fonts.end(), [](const FontEntry& a, const FontEntry& b) {
    return std::tie(a.family, a.height, a.charset) < std::tie(b.family, b.height, b.charset);
  });
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

  return entry == std::end(fallback) ? 0 : static_cast<int>(entry - std::begin(fallback));
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
