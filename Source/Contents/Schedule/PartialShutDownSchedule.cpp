#include "Contents/Schedule/PartialShutDownSchedule.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/ShowRankingScheduler.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Timestamp.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <new>

// GLOBAL: gl_24853
double s_partialShutDownNoticeSeconds[6] = {1800.0, 900.0, 600.0, 300.0, 180.0, 60.0};

// OFFSET: 0x00BA8380
// FUNCTION: fn_19719
CPartialShutDownSchedule::CPartialShutDownSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 0) {
  this->noticeIndex = 0;
}

CPartialShutDownSchedule::~CPartialShutDownSchedule() {
  delete this->startTime;
  delete this->endTime;
}

// OFFSET: 0x00BA8530
// FUNCTION: fn_19721
void CPartialShutDownSchedule::UpdateNotice() {
  while (this->noticeIndex < 6) {
    const int diff = static_cast<int>(static_cast<double>(this->seconds) - s_partialShutDownNoticeSeconds[this->noticeIndex]);
    if (diff > 0) {
      this->interval = static_cast<std::int64_t>(diff) * 1000;
      return;
    }

    ++this->noticeIndex;
  }
}

// OFFSET: 0x00BA8580
// FUNCTION: fn_19722
void CPartialShutDownSchedule::SetPeriod(__time32_t endTime, __time32_t startTime) {
  this->startTime = new (std::nothrow) Timestamp(startTime);
  this->endTime = new (std::nothrow) Timestamp(endTime);
  if (!this->startTime || !this->endTime) {
    return;
  }

  if (static_cast<int>(_difftime32(this->endTime->seconds, this->startTime->seconds)) <= 0) {
    return;
  }

  this->seconds = static_cast<std::int64_t>(_difftime32(this->endTime->seconds, this->startTime->seconds));
  if (this->seconds < 3600) {
    this->UpdateNotice();
    return;
  }

  if (this->seconds - 3600 < 3600) {
    const std::chrono::seconds delay = std::chrono::seconds(this->seconds) - std::chrono::hours(1) + std::chrono::minutes(10);
    this->interval = std::chrono::duration_cast<std::chrono::milliseconds>(delay).count();
  }
}

// OFFSET: 0x00BA8710
