#ifndef RAGNAROK_SOURCE_CONTENTS_SHOWRANKINGSCHEDULER_H
#define RAGNAROK_SOURCE_CONTENTS_SHOWRANKINGSCHEDULER_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

struct RankingBoardStyle {
  /* 0000 */ const char* title;
  /* 0004 */ std::uint32_t color;
  /* 0008 */ std::uint32_t background;
};

extern int g_schedulerSerial;

std::map<int, RankingBoardStyle> RankingBoardStyles();

class CShowRankingScheduler {
public:
  CShowRankingScheduler(const std::uint64_t& start, const std::uint64_t& end, int extra);
  virtual ~CShowRankingScheduler() = default;

  const void* SetData(const void* data);
  void Process(const std::uint64_t& now);
  void Show();

  /* 0004 */ std::uint8_t schedule[44];
  /* 0030 */ std::uint8_t rankingData[88];
  /* 0088 */ std::uint32_t unknown_0088 = 0;
  /* 008C */ std::uint32_t unknown_008C = 0;
  /* 0090 */ std::vector<std::uint32_t> rankingList;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RankingBoardStyle) == 12);
static_assert(offsetof(CShowRankingScheduler, rankingData) == 0x30);
static_assert(offsetof(CShowRankingScheduler, rankingList) == 0x90);
#endif

#endif
