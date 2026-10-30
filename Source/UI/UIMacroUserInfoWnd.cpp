#include "UI/UIMacroUserInfoWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/FrameFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x007F0480
// FUNCTION: fn_09845
bool UIMacroUserInfoWnd::Slot08() {
  return true;
}

// OFFSET: 0x007F0EE0
// FUNCTION: fn_09847
void UIMacroUserInfoWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);
}

// OFFSET: 0x007F0EF0
// FUNCTION: fn_09848
void UIMacroReporterWnd::Paint() {
  if (this->m_mode == 0 || this->m_mode == 1) {
    this->m_reportButton->SetDisabled(this->m_entries.empty());
    this->m_reportButton->Invalidate();
  } else if (this->m_mode == 2 || this->m_mode == 3) {
    this->m_reportButton->SetDisabled(!(this->m_namesValid == 1 && this->m_targetValid == 1));
    this->m_reportButton->Invalidate();
  }

  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, "", 0);
  this->DrawButtonBar(1, 0, 1);

  if (this->m_mode != 0 && this->m_mode != 1) {
    return;
  }

  for (int row = 0, y = 56; row < this->m_visibleRows; ++row, y += 22) {
    this->FillRegion(14, y, 214, 18, 0xE6E6E6);
  }

  for (unsigned int index = 0; index < this->m_rows.size(); ++index) {
    this->m_rows[index].first->Enable(0);
    this->m_rows[index].second->Enable(0);
  }

  int y = 57;
  for (unsigned int index = this->m_firstRow; static_cast<int>(index) < this->m_firstRow + this->m_visibleRows; ++index, y += 22) {
    if (index >= this->m_rows.size()) {
      return;
    }

    this->m_rows[index].first->Enable(1);
    this->m_rows[index].first->Move(15, y + 2);
    this->m_rows[index].second->Enable(1);
    this->m_rows[index].second->Move(36, y);
  }
}

// OFFSET: 0x007F10F0
// FUNCTION: fn_09849
void UIMacroUserInfoWnd::Paint() {
  this->ClearSurface(0xFAFAFA);
  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 5, 5, this->m_name.c_str(), 0, 0, 12, 0, 1, 0);
  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 5, 20, this->m_message.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x007F1160
// FUNCTION: fn_09850
void UIMacroReporterWnd::Slot25(int x, int y) {
  if (this->unknown_010C == 1 && y >= 18) {
    return;
  }

  if (g_dragWindowId != this->geometry.id) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_dragWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
  this->ToScreen(x, y);
  this->m_dragX = x;
  this->m_dragY = y;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->m_frameContext->snapping.graph.CaptureOrigins(this);
}

// OFFSET: 0x007F11F0
// FUNCTION: fn_09851
void UIMacroReporterWnd::Slot31(int, int) {
  if (g_windowMgr.Capture() != this) {
    return;
  }

  g_dragWindowId = -1;
  g_windowMgr.ReleaseCapture();
  this->m_frameContext->snapping.graph.Refresh(this);
}

// OFFSET: 0x007F1230
// FUNCTION: fn_09852
void UIMacroReporterWnd::Slot19() {
  for (auto it = this->m_removedRows.begin(); it != this->m_removedRows.end();) {
    if ((*it)->Disabled()) {
      this->RemoveChild(*it, 1);
      it = this->m_removedRows.erase(it);
    } else {
      ++it;
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007F12A0
// FUNCTION: fn_09853
void UIMacroReporterWnd::Slot35(int delta) {
  if (this->m_scrollable == 1) {
    this->SendMsg(this->m_scrollBar, 7, delta < 0 ? 3 : -3, 0, 0, 0);
  }
}

// OFFSET: 0x007F12E0
// FUNCTION: fn_09854
void UIMacroReporterWnd::UpdateScrollBar() {
  const int count = static_cast<int>(this->m_rows.size());
  if (count == 0 || !this->m_scrollBar) {
    return;
  }

  const int maximum = (std::max)(count - this->m_visibleRows, 0);
  this->m_scrollable = maximum > 0;
  if (maximum < 1) {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, this->m_visibleRows, count);
  } else {
    this->m_scrollBar->SetRange(maximum, this->m_visibleRows, count);
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
      this->m_scrollBar->SetPosition(0);
    } else {
      if (maximum < this->m_firstRow) {
        this->m_firstRow = maximum;
      }
      this->m_scrollBar->SetPosition(this->m_firstRow);
    }
  }

  this->m_scrollBar->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x007F01A0
// FUNCTION: fn_09844
bool UIMacroReporterWnd::ValidateTarget(unsigned char notify) {
  std::string text = this->m_target;
  if (text.empty() || text.size() - 4 > 4) {
    if (notify == 1) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(4011)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  const int value = std::atoi(text.c_str());
  char digits[16] = {};
  if (text.size() != 0) {
    std::memset(digits, '0', text.size());
  }

  if (value == 0 && text.compare(digits) != 0) {
    if (notify == 1) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(4011)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  text = "1" + this->m_target;
  char number[16] = {};
  int scale = 1;
  for (std::size_t index = 1; index < text.size(); ++index) {
    scale *= 10;
  }
  _itoa(scale + value, number, 10);

  if (text.compare(number) != 0) {
    if (notify == 1) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(4011)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  return true;
}

// OFFSET: 0x007F26C0
// FUNCTION: fn_09858
void UIMacroReporterWnd::Slot11() {
  if (this->m_state) {
    this->m_state->width = this->geometry.width;
    this->m_state->height = this->geometry.height;
    this->m_state->x = this->geometry.x;
    this->m_state->y = this->geometry.y;
    this->m_state->unknown_0011 = this->unknown_010C;
    this->m_state->entries.clear();
    this->m_state->entries.resize(this->m_entries.size());
    for (unsigned int index = 0; index < this->m_entries.size(); ++index) {
      this->m_state->entries[index] = this->m_entries[index];
    }
  }
}
