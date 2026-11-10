#include "Contents/Schedule/PartialShutDownSchedule.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Events/ShowRankingScheduler.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Timestamp.h"

#include "Resource/MSGSTRINGID.h"
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
// FUNCTION: fn_19723
void CPartialShutDownSchedule::Process(const std::uint64_t& now) {
  CSchedule::Process(now);
  const __time32_t currentTime = static_cast<__time32_t>(static_cast<std::int64_t>(now) / 10000000);
  const int seconds = static_cast<int>(_difftime32(this->endTime->seconds, currentTime));
  this->seconds = seconds;

  if (seconds >= 3600) {
    char text[256];
    std::memset(text, 0, 255);
    std::sprintf(text, LookupMessage(MSI_MENU_WEBBROWSERWND), seconds / 3600, (seconds % 3600) / 60);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    return;
  }

  if (this->noticeIndex < 6) {
    if (g_modeMgr.GetGameMode()) {
      char text[256];
      std::memset(text, 0, 255);
      std::sprintf(
        text, LookupMessage(MSI_MENU_MACROWND), this->endTime->Year(), this->endTime->Month(), this->endTime->Day(), this->endTime->Hour(), this->endTime->Minute(),
        this->endTime->Second()
      );
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
      ++this->noticeIndex;
      this->UpdateNotice();
    }
  } else {
    this->active = false;
  }
}
