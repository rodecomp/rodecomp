#include "UI/Window/UISeekPartyListWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009431D0
// FUNCTION: fn_12815
void UISeekPartyListWnd::NotifyNewAd() {
  if (this->m_alarmCheck->geometry.unknown_0030 != 0) {
    g_soundMgr->PlayWave("party_alarm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6D9)), 0xFFFF00, 15, 0, 0);
  }
}

// OFFSET: 0x009435F0
// FUNCTION: fn_12825
bool UISeekPartyListWnd::MatchesFilter(PARTY_BOOKING_AD_INFO info) {
  if (this->m_filterLevel > 0 && (this->m_filterLevel > info.detail.level || this->m_filterLevel + 15 < info.detail.level)) {
    return false;
  }

  if (this->m_filterMap > 0 && this->m_filterMap != info.detail.mapId) {
    return false;
  }

  if (this->m_filterJob > 0) {
    for (int i = 0; i < 6; ++i) {
      if (this->m_filterJob == info.detail.job[i]) {
        return true;
      }
    }
    return false;
  }

  return true;
}

// OFFSET: 0x00933630
// FUNCTION: fn_12727
void UISeekPartyListWnd::Slot35(int delta) {
  if (this->m_scrollable != 0) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}
