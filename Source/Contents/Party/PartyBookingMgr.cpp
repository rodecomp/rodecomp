#include "Contents/Party/PartyBookingMgr.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "UI/Window/UISeekPartyListWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <cstring>
#include <ctime>

// OFFSET: 0x00B15C80

// OFFSET: 0x00B16810

// OFFSET: 0x00B16A80
// FUNCTION: fn_18443
void PartyBookingMgr::Clear() {
  this->m_entries.clear();
  this->m_pending.clear();
}

// OFFSET: 0x00B16AD0

// OFFSET: 0x00B16CD0

// OFFSET: 0x00B16CF0

// OFFSET: 0x00B175B0
// FUNCTION: fn_18448
void PartyBookingMgr::OnAdvertisement(PARTY_BOOKING_AD_INFO info) {
  UISeekPartyListWnd* window = static_cast<UISeekPartyListWnd*>(g_windowMgr.LookupWindow(0xA9));
  if (window) {
    if (window->MatchesFilter(info)) {
      if (!this->m_lastPage) {
        this->AddEntries(&info, 1, false);
        window->NotifyNewAd();
      } else {
        std::pair<bool, PartyBookingEntry> result = this->MakeEntry(info);
        if (result.first) {
          this->m_pending.insert(std::pair<const int, PartyBookingEntry>(info.index, result.second));
        }
        window->NotifyNewAd();
      }
    }
    return;
  }

  if (this->m_filterEnabled) {
    PARTY_BOOKING_AD_INFO ad = info;
    if (this->m_filterLevel > 0 && (ad.detail.level < this->m_filterLevel || this->m_filterLevel + 15 < ad.detail.level)) {
      return;
    }

    if (this->m_filterMap > 0 && this->m_filterMap != ad.detail.mapId) {
      return;
    }

    if (this->m_filterJob > 0) {
      int i = 0;
      while (this->m_filterJob != ad.detail.job[i]) {
        if (++i >= 6) {
          return;
        }
      }
    }
  }

  if (this->m_alarm) {
    g_soundMgr->PlayWave("party_alarm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6D9)), 0xFFFF00, 15, 0, 0);
  }
}

// OFFSET: 0x00B17820

// OFFSET: 0x00B17C40
// FUNCTION: fn_18450
void PartyBookingMgr::Reset() {
  this->m_lastPage = true;
  this->m_lastIndex = -1;
  this->unknown_0000 = 0;
  this->m_entries.clear();
  this->m_pending.clear();
}

// OFFSET: 0x00B18210

// OFFSET: 0x00B18230
