#ifndef RAGNAROK_SOURCE_CONTENTS_JOBCODEFORITEM_H
#define RAGNAROK_SOURCE_CONTENTS_JOBCODEFORITEM_H

#include "Utility/Singleton.h"

#include <map>
#include <set>

class CJobCodeForItem : public Singleton<CJobCodeForItem> {
public:
  /* 0004 */ std::map<unsigned int, std::set<int>> m_jobGroups;
  /* 000C */ std::map<unsigned long, std::set<int>> m_jobCodes;

  template <typename... Groups>
  void AddJobCodeFromGroups(unsigned long code, Groups... groups);
  template <typename... Jobs>
  void AddJobCode(unsigned long code, Jobs... jobs);
  template <typename... Jobs>
  void AddJobGroup(unsigned int group, Jobs... jobs);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CJobCodeForItem) == 0x14);
#endif

#endif
