#include "Contents/Schedule/ShutDownSchedule.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"

// GLOBAL: gl_24854
double s_shutDownNoticeSeconds[6] = {1800.0, 900.0, 600.0, 300.0, 180.0, 60.0};

// OFFSET: 0x00BA8CC0
// FUNCTION: fn_19730
void CShutDownSchedule::UpdateNotice() {
  if (this->noticeIndex >= 6) {
    return;
  }

  while (true) {
    const int diff = static_cast<int>(static_cast<double>(this->seconds) - s_shutDownNoticeSeconds[this->noticeIndex]);
    if (diff >= 0) {
      this->interval = static_cast<std::int64_t>(diff) * 1000;
      return;
    }

    ++this->noticeIndex;
    if (this->noticeIndex >= 6) {
      return;
    }
  }
}

// OFFSET: 0x00BA8E00
// FUNCTION: fn_19731
void CShutDownSchedule::Process(const std::uint64_t& now) {
  if (this->noticeIndex < 6) {
    if (g_modeMgr.GetGameMode()) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2020)), 0xFFFF, 0, 0, 0);
      ++this->noticeIndex;
      this->UpdateNotice();
    }
  } else {
    this->active = false;
  }
}
