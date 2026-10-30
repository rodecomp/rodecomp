#include "UI/Window/UISeekPartyListWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/PartyBookingMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"

// GLOBAL: gl_24771
std::map<int, PartyBookingEntry> g_friendRows;

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

// OFFSET: 0x0091BB10
// FUNCTION: fn_12607
int UISeekPartyListWnd::HitTestRow(int x, int y) {
  if (x > this->geometry.width - 0x14 || x < 0x14) {
    return -1;
  }

  if (y < 0x78 || y > this->geometry.height - 0x1e) {
    return -1;
  }

  const int row = (y - 0x78) / 0x1e;
  int found = 0;
  int index = 0;
  const int firstIndex = this->m_firstRow * this->m_columnCount;
  for (auto it = g_friendRows.begin(); it != g_friendRows.end(); ++it, ++index) {
    if (index < firstIndex) {
      continue;
    }

    if (found == this->m_columnCount * row) {
      return index;
    }

    ++found;
  }

  return -1;
}

// OFFSET: 0x00930720
// FUNCTION: fn_12687
void UISeekPartyListWnd::Slot26(int x, int y) {
  if (y < 0x78) {
    return;
  }

  const int previous = this->m_hoverIndex;
  const int index = this->HitTestRow(x, y);
  this->m_hoverIndex = index;
  if (previous != index) {
    this->Invalidate();
  }

  this->SendMsg(nullptr, 6, 0x17d, 0, 0, 0);
}

// OFFSET: 0x00931090
// FUNCTION: fn_12698
void UISeekPartyListWnd::Slot25(int x, int y) {
  if (y < 0x78) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  const int previous = this->m_hoverIndex;
  const int index = this->HitTestRow(x, y);
  this->m_hoverIndex = index;
  if (previous != index) {
    this->Invalidate();
  }
}

// OFFSET: 0x00933740
// FUNCTION: fn_12729
void UISeekPartyListWnd::UpdateScrollBar() {
  if (!this->m_scrollBar) {
    return;
  }

  const int count = g_friendRows.size();
  const int rows = count / this->m_columnCount + (count % this->m_columnCount != 0 ? 1 : 0);

  const int visible = this->m_visibleRows;
  int range = rows - visible;
  if (range < 0) {
    range = 0;
  }

  this->m_scrollable = range != 0;
  if (range != 0) {
    this->m_scrollBar->SetRange(range, visible, rows);
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
    }
    if (this->m_firstRow > rows - this->m_visibleRows) {
      this->m_firstRow = rows - this->m_visibleRows;
    }
    this->m_scrollBar->SetPosition(this->m_firstRow);
  } else {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, visible, rows);
  }

  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x00943F40

// OFFSET: 0x00943830
// FUNCTION: fn_12829
void UISeekPartyListWnd::InitJobFilter() {
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E6), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DB), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DC), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DD), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DE), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DF), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E0), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E1), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E2), 0);

  int counts[9] = {0, 0, 6, 13, 6, 7, 6, 13, 8};
  int message = 0;
  this->m_jobOptions[0].push_back(std::string(LookupMessage(0x6E6)));

  for (int i = 0; i < 4; ++i) {
    std::string name(LookupMessage(0x6B0 + i));
    this->m_jobOptions[1].push_back(name);
  }

  for (int category = 2; category < 9; ++category) {
    for (int i = 0; i < counts[category]; ++i) {
      std::string name(LookupMessage(0x65D + message));
      this->m_jobOptions[category].push_back(name);
      ++message;
    }
  }

  this->m_jobCategoryCombo->Select(0);
  this->SelectJobCategory(0);
}

// OFFSET: 0x00945CF0

// OFFSET: 0x00945FB0
