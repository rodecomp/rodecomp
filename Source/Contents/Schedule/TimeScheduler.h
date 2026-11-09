#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_TIMESCHEDULER_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_TIMESCHEDULER_H

#include "Contents/Schedule/Schedule.h"

#include <cstddef>
#include <cstdint>
#include <list>

// VTABLE: vt_01790
class CTimeScheduler {
public:
  /* 0004 */ std::list<CSchedule*> m_schedules;
  /* 000C */ bool m_initialized;
  /* 000D */ bool m_processingStopped;

  CTimeScheduler();
  virtual ~CTimeScheduler();

  static bool Create();
  static void Destroy();

  bool Init();
  CSchedule* FindSchedule(int serial);
  CSchedule* FindScheduleByKey(int key);
  void RemoveAllSchedules();
  CSchedule* CreateSchedule(const std::int64_t& interval, int scheduleType);
  CSchedule* AddSchedule(CSchedule* schedule);
  void Process();
  void RemoveSchedulesByKey(int value);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTimeScheduler) == 0x10);
static_assert(offsetof(CTimeScheduler, m_schedules) == 0x04);
static_assert(offsetof(CTimeScheduler, m_initialized) == 0x0C);
static_assert(offsetof(CTimeScheduler, m_processingStopped) == 0x0D);
#endif

extern CTimeScheduler* g_timeScheduler;

#endif
