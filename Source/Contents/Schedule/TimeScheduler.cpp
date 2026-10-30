#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/Schedule/CheckGDISchedule.h"

#include <new>

extern "C" long long _Xtime_get_ticks();

// INLINED
CTimeScheduler::CTimeScheduler() {
  this->m_initialized = this->Init();
  this->m_unknown_000D = false;
}

// INLINED
CTimeScheduler::~CTimeScheduler() {
  for (CSchedule* schedule : this->m_schedules) {
    if (schedule != nullptr) {
      delete schedule;
    }
  }
  this->m_schedules.clear();
  this->m_unknown_000D = false;
}

// OFFSET: 0x00BA91E0
// FUNCTION: fn_19733
bool CTimeScheduler::Init() {
  return true;
}

// OFFSET: 0x00BA91F0
// FUNCTION: fn_19734
bool CTimeScheduler::Create() {
  if (g_timeScheduler != nullptr) {
    return false;
  }

  CTimeScheduler* scheduler = new (std::nothrow) CTimeScheduler();
  g_timeScheduler = reinterpret_cast<char*>(scheduler);
  if (scheduler != nullptr) {
    if (scheduler->m_initialized) {
      return true;
    }
    delete scheduler;
    g_timeScheduler = nullptr;
  }
  return false;
}

// OFFSET: 0x00BA9340
// FUNCTION: fn_19735
void CTimeScheduler::Destroy() {
  if (g_timeScheduler != nullptr) {
    delete reinterpret_cast<CTimeScheduler*>(g_timeScheduler);
    g_timeScheduler = nullptr;
  }
}

// OFFSET: 0x00BA9360
// FUNCTION: fn_19736
CSchedule* CTimeScheduler::FindSchedule(int serial) {
  for (CSchedule* schedule : this->m_schedules) {
    if (schedule->serial == serial) {
      return schedule;
    }
  }
  return nullptr;
}

// OFFSET: 0x00BA94E0
// FUNCTION: fn_19737
CSchedule* CTimeScheduler::AddSchedule(CSchedule* schedule) {
  if (schedule == nullptr) {
    return nullptr;
  }
  schedule->timestamp = _Xtime_get_ticks();
  this->m_schedules.push_back(schedule);
  return schedule;
}

// OFFSET: 0x00BA9550
// FUNCTION: fn_19738
void CTimeScheduler::Process() {
  if (this->m_unknown_000D) {
    return;
  }

  auto it = this->m_schedules.begin();
  while (it != this->m_schedules.end()) {
    CSchedule* schedule = *it;
    if (schedule == nullptr) {
      ++it;
      continue;
    }

    if (schedule->active) {
      uint64_t now = _Xtime_get_ticks();
      if (!schedule->unknown_0025) {
        if (schedule->CheckTime(now)) {
          schedule->Process(now);
        }
      }
      ++it;
    } else {
      it = this->m_schedules.erase(it);
      delete schedule;
    }
  }
}

// OFFSET: 0x00BA95F0
// FUNCTION: fn_19739
void CTimeScheduler::sub_BA95F0(int value) {
  for (CSchedule* schedule : this->m_schedules) {
    if (schedule->unknown_002C == value) {
      schedule->active = false;
    }
  }
}
