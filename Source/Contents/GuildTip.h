#ifndef RAGNAROK_SOURCE_CONTENTS_GUILDTIP_H
#define RAGNAROK_SOURCE_CONTENTS_GUILDTIP_H

#include "Contents/TipOfTheDay.h"

#include <cstddef>
#include <string>
#include <vector>

class CGuildTip {
public:
  std::vector<StringList> GetTipList();
  int Load();

  /* 0000 */ bool m_bTryLoading = false;
  /* 0004 */ std::vector<StringList> m_tipList;
};

extern CGuildTip g_guildTip;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGuildTip) == 0x10);
static_assert(offsetof(CGuildTip, m_tipList) == 0x4);
#endif

#endif
