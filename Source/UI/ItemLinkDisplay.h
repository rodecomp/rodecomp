#ifndef RAGNAROK_SOURCE_UI_ITEMLINKDISPLAY_H
#define RAGNAROK_SOURCE_UI_ITEMLINKDISPLAY_H

#include <string>

class CDC;
class FontCache;
struct TextLocale;
struct ItemNaming;
struct TipBoxTitles;

struct ItemLinkDisplay {
  CDC& surface;
  FontCache& fonts;
  const TextLocale& locale;
  ItemNaming& items;
  const TipBoxTitles& tips;
  const int& width;

  bool Name(const std::string& encoded, std::string& output);
  bool Next(std::string& remaining, std::string& encoded, unsigned& position, const char* original);
  std::string Convert(const std::string& text);
};

#endif
