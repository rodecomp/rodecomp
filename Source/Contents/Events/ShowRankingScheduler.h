#ifndef RAGNAROK_SOURCE_CONTENTS_EVENTS_SHOWRANKINGSCHEDULER_H
#define RAGNAROK_SOURCE_CONTENTS_EVENTS_SHOWRANKINGSCHEDULER_H

#include "Contents/Schedule/Schedule.h"
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

struct RankingBoardStyle {
  /* 0000 */ const char* title;
  /* 0004 */ std::uint32_t color;
  /* 0008 */ std::uint32_t background;
};

struct RankingData {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t kind;
  /* 0004 */ std::uint32_t ids[10];
  /* 002C */ int points[10];
  /* 0054 */ int myPoint;
};

extern int g_schedulerSerial;

std::map<int, RankingBoardStyle> RankingBoardStyles();

class CShowRankingScheduler : public CSchedule {
public:
  __declspec(noinline) CShowRankingScheduler(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType);
  ~CShowRankingScheduler() override = default;
  void Process(const std::uint64_t& now) override;

  __declspec(noinline) void SetData(const void* data);
  void Show();

  /* 0030 */ RankingData rankingData;
  /* 0088 */ std::int64_t delay = 0;
  /* 0090 */ std::vector<std::uint32_t> rankingList;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RankingBoardStyle) == 12);
static_assert(sizeof(RankingData) == 88);
static_assert(offsetof(CShowRankingScheduler, rankingData) == 0x30);
static_assert(offsetof(CShowRankingScheduler, rankingList) == 0x90);
#endif

#endif
