#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_SHUTDOWNSCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_SHUTDOWNSCHEDULE_H

#include "Contents/Schedule/Schedule.h"

extern double s_shutDownNoticeSeconds[6];

class CShutDownSchedule : public CSchedule {
public:
  void Process(const std::uint64_t& now) override;
  void UpdateNotice();

  /* 0030 */ void* unknown_0030;
  /* 0034 */ std::uint32_t unknown_0034;
  /* 0038 */ int seconds;
  /* 003C */ int noticeIndex;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CShutDownSchedule) == 0x40);
static_assert(offsetof(CShutDownSchedule, seconds) == 0x38);
static_assert(offsetof(CShutDownSchedule, noticeIndex) == 0x3C);
#endif

#endif
