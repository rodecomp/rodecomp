#ifndef RAGNAROK_SOURCE_CONTENTS_SHOWRANKINGSCHEDULER_H
#define RAGNAROK_SOURCE_CONTENTS_SHOWRANKINGSCHEDULER_H

#include <cstddef>
#include <cstdint>
#include <vector>

class CShowRankingScheduler {
public:
  virtual ~CShowRankingScheduler() = default;

  const void* SetData(const void* data);

  /* 0004 */ std::uint8_t schedule[44];
  /* 0030 */ std::uint8_t rankingData[88];
  /* 0088 */ std::uint32_t unknown_0088 = 0;
  /* 008C */ std::uint32_t unknown_008C = 0;
  /* 0090 */ std::vector<std::uint32_t> rankingList;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CShowRankingScheduler, rankingData) == 0x30);
static_assert(offsetof(CShowRankingScheduler, rankingList) == 0x90);
#endif

#endif
