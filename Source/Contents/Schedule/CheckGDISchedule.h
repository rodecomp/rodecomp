#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_CHECKGDISCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_CHECKGDISCHEDULE_H

#include "Contents/Schedule/Schedule.h"

extern char* g_timeScheduler;

class CCheckGDISchedule : public CSchedule {
public:
  CCheckGDISchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType);
  void Process(const std::uint64_t& now) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCheckGDISchedule) == 0x30);
#endif

#endif
