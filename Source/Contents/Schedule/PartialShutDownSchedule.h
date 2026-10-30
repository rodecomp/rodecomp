#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_PARTIALSHUTDOWNSCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_PARTIALSHUTDOWNSCHEDULE_H

#include "Contents/Schedule/Schedule.h"

#include <ctime>

class Timestamp;

extern double s_partialShutDownNoticeSeconds[6];

class CPartialShutDownSchedule : public CSchedule {
public:
  CPartialShutDownSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType);
  ~CPartialShutDownSchedule() override;
  void Process(const std::uint64_t& now) override;

  void SetPeriod(__time32_t endTime, __time32_t startTime);
  void UpdateNotice();

  /* 0030 */ Timestamp* startTime;
  /* 0034 */ Timestamp* endTime;
  /* 0038 */ int seconds;
  /* 003C */ int noticeIndex;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPartialShutDownSchedule) == 0x40);
static_assert(offsetof(CPartialShutDownSchedule, startTime) == 0x30);
static_assert(offsetof(CPartialShutDownSchedule, seconds) == 0x38);
static_assert(offsetof(CPartialShutDownSchedule, noticeIndex) == 0x3C);
#endif

#endif
