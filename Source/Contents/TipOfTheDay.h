#ifndef RAGNAROK_SOURCE_CONTENTS_TIPOFTHEDAY_H
#define RAGNAROK_SOURCE_CONTENTS_TIPOFTHEDAY_H

#include <cstddef>
#include <string>
#include <vector>

using StringList = std::vector<std::string>;

class CTipOfTheDay {
public:
  int Load();
  int Size();
  StringList& GetTip(int index);

  /* 0000 */ bool m_bTryLoading = false;
  /* 0004 */ std::vector<StringList> m_tipList;
};

extern CTipOfTheDay g_tipOfTheDay;
extern bool g_tipOfTheDayEnglish;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTipOfTheDay) == 0x10);
static_assert(offsetof(CTipOfTheDay, m_tipList) == 0x4);
#endif

#endif
