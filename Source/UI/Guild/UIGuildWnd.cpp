#include "UI/Guild/UIGuildWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Guild/UIAgitWnd.h"
#include "UI/Guild/UIGuildHelperWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <windows.h>
#include <timeapi.h>

#include <cstring>
#include <cwchar>
#include <string>

// GLOBAL: gl_24918
int g_guildPositionRowHeight = 19;

// GLOBAL: gl_24919
int g_guildBanishedColumnWidths[2] = {82, 292};

// GLOBAL: gl_24920
int g_guildBanishedRowHeight = 19;

// GLOBAL: gl_24921
int g_guildTotalInfoRowHeight = 19;

static GuildInfo& GuildData() {
  return *reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
}

// OFFSET: 0x00957D30
// FUNCTION: fn_13044
int UIGuildBanishedMemberWnd::Tab() {
  return 4;
}

// OFFSET: 0x00957D40
// FUNCTION: fn_13045
int UIGuildInfoManageWnd::Tab() {
  return 0;
}

// OFFSET: 0x00957D50
// FUNCTION: fn_13046
int UIGuildMemberManageWnd::Tab() {
  return 1;
}

// OFFSET: 0x00957D80
// FUNCTION: fn_13049
int UIGuildSkillWnd::Tab() {
  return 3;
}

// OFFSET: 0x00957D90
// FUNCTION: fn_13050
int UIGuildTotalInfoWnd::Tab() {
  return 6;
}

// OFFSET: 0x0095F860
// FUNCTION: fn_13080
void UIGuildWnd::Paint() {
  const int tab = this->Tab();
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 10, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, 24, RGB(red, green, blue));

  for (int x = 1, index = 0; x < 397; x += 66, ++index) {
    if (tab == index) {
      SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    } else {
      SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
    }
    this->FillRegion(x, 18, 64, 23, RGB(red, green, blue));
  }

  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  unsigned borderRed, borderGreen, borderBlue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, borderRed, borderGreen, borderBlue);
  this->FillRegion(0, 41, this->geometry.width, this->geometry.height - 41, RGB(borderRed, borderGreen, borderBlue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, borderRed, borderGreen, borderBlue);
  this->FillRegion(1, 41, this->geometry.width - 2, this->geometry.height - 41, RGB(borderRed, borderGreen, borderBlue));
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x00960020
// FUNCTION: fn_13091
void UIGuildWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (y <= 41) {
    const int current = this->Tab();
    this->Tab();
    const int tab = x / 66;
    if (tab < 6 && current != tab) {
      this->SendMsg(nullptr, 69, tab, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0095FE10
// FUNCTION: fn_13087
void UIGuildNoticeWnd::Slot25(int x, int y) {
  UIGuildWnd::Slot25(x, y);
}

// OFFSET: 0x009602E0
// FUNCTION: fn_13100
void UIGuildMemberManageWnd::Slot19() {
  if (this->m_buttonTime != 0 && timeGetTime() > this->m_buttonTime + 400) {
    UIButton* button = this->m_button;
    this->m_buttonTime = 0;
    button->SetDisabled(0);
    button->Invalidate();
  }
}

// OFFSET: 0x00960330
// FUNCTION: fn_13101
void UIGuildPositionManageWnd::Slot19() {
  if (this->m_buttonTime != 0 && timeGetTime() > this->m_buttonTime + 400) {
    UIButton* button = this->m_button;
    this->m_buttonTime = 0;
    button->SetDisabled(0);
    button->Invalidate();
  }
}

// OFFSET: 0x00958050
// FUNCTION: fn_13054
void UIGuildPositionManageWnd::Invalidate() {
  this->UIWindow::Invalidate();
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(nullptr);
}

// OFFSET: 0x009603C0
// FUNCTION: fn_13103
void UIGuildMemberManageWnd::Slot32(int x, int y) {
  const int previous = this->m_hoverRow;
  int row;
  if (static_cast<unsigned>(x - 5) <= 377 && static_cast<unsigned>(y - 66) <= 217) {
    row = (y - 66) / (this->m_rowSpacing + 36) + this->m_columns * this->m_scrollTop;
    if (static_cast<unsigned>(row) >= this->m_rowCount) {
      row = -1;
    }
  } else {
    row = -1;
  }

  this->m_hoverRow = row;
  if (previous != row) {
    this->Invalidate();
  }
}

// OFFSET: 0x00960620
// FUNCTION: fn_13108
void UIGuildMemberManageWnd::Slot33(int, int) {
  this->SendMsg(nullptr, 49, 0, 0, 0, 0);
}

// OFFSET: 0x00960640
// FUNCTION: fn_13109
void UIGuildSkillWnd::Slot33(int, int) {
  this->SendMsg(nullptr, 49, 0, 0, 0, 0);
}

// OFFSET: 0x00960960
// FUNCTION: fn_13111
void UIGuildBanishedMemberWnd::Slot35(int delta) {
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

// OFFSET: 0x009609B0
// FUNCTION: fn_13112
void UIGuildMemberManageWnd::Slot35(int delta) {
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

// OFFSET: 0x00960A00
// FUNCTION: fn_13113
void UIGuildPositionManageWnd::Slot35(int delta) {
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

// OFFSET: 0x00960A50
// FUNCTION: fn_13114
void UIGuildSkillWnd::Slot35(int delta) {
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

// OFFSET: 0x00960AA0
// FUNCTION: fn_13115
void UIGuildTotalInfoWnd::Slot35(int delta) {
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

UIGuildWnd::UIGuildWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_saved(nullptr), m_tabs{}, m_controls(&controls) {
}

// OFFSET: 0x00953D70
// FUNCTION: fn_13001
UIGuildPositionManageWnd::UIGuildPositionManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIGuildWnd(windows, frames, controls), m_buttonTime(0) {
  for (int index = 0; index < 11; ++index) {
    this->unknown_00D0[index] = 0;
    this->unknown_00FC[index] = 0;
    this->unknown_0128[index] = 0;
    this->unknown_0180[index] = 0;
    this->unknown_0154[index] = 0;
    this->unknown_01D8[index] = 0;
    this->unknown_01AC[index] = 0;
  }
}

// OFFSET: 0x00956050
// FUNCTION: fn_13035
bool CompareWideText(const char* left, const char* right) {
  wchar_t wideLeft[260];
  wchar_t wideRight[260];
  memset(wideLeft, 0, sizeof(wideLeft));
  memset(wideRight, 0, sizeof(wideRight));
  MultiByteToWideChar(CP_ACP, 0, left, -1, wideLeft, 260);
  MultiByteToWideChar(CP_ACP, 0, right, -1, wideRight, 260);
  return wcscmp(wideLeft, wideRight) < 0;
}

// OFFSET: 0x009578F0
// FUNCTION: fn_13039
int UIGuildSkillWnd::SkillIndexAt(int x, int y) {
  if (x >= 40 && y >= 41 && y < this->m_visibleRows * 32 + 41) {
    const int row = (y - 41) / 32;
    int visible = 0;
    int index = 0;
    for (auto skill = this->m_skills.begin(); skill != this->m_skills.end(); ++skill, ++index) {
      if (index >= this->m_scrollTop * this->m_columns) {
        if (visible == this->m_columns * row) {
          return index;
        }
        ++visible;
      }
    }
  }

  return -1;
}

// OFFSET: 0x00957850
// FUNCTION: fn_13038
int UIGuildPositionManageWnd::PositionIndexAt(int x, int y) {
  if (x <= this->geometry.width - 14 && x > 3 && y >= g_guildPositionRowHeight + 46 && y < (this->m_visibleRows + 1) * g_guildPositionRowHeight + 46) {
    const int row = (y - g_guildPositionRowHeight - 46) / g_guildPositionRowHeight;
    int visible = 0;
    int index = 0;
    for (auto position = this->m_positions.begin(); position != this->m_positions.end(); ++position, ++index) {
      if (index >= this->m_scrollTop * this->m_columns) {
        if (visible == this->m_columns * row) {
          return index;
        }
        ++visible;
      }
    }
  }

  return -1;
}

// OFFSET: 0x00957A90
// FUNCTION: fn_13041
const GuildPositionEntry& UIGuildPositionManageWnd::PositionAt(int index, int* found) {
  *found = 1;
  // GLOBAL: gl_24922
  static GuildPositionEntry s_emptyGuildPosition;

  int current = 0;
  for (auto position = this->m_positions.begin(); position != this->m_positions.end(); ++position, ++current) {
    if (index == current) {
      return *position;
    }
  }

  *found = 0;
  return s_emptyGuildPosition;
}

// OFFSET: 0x00957BB0
// FUNCTION: fn_13042
CSkillInfo UIGuildSkillWnd::SkillAt(int index) {
  CSkillInfo result(0);
  int current = 0;
  for (auto skill = this->m_skills.begin(); skill != this->m_skills.end(); ++skill, ++current) {
    if (index == current) {
      result = *skill;
      break;
    }
  }

  return result;
}

// OFFSET: 0x00957CA0
// FUNCTION: fn_13043
CSkillInfo UIGuildSkillWnd::VisibleSkillAt(int row) {
  int index = 0;
  for (auto skill = this->m_skills.begin(); skill != this->m_skills.end(); ++skill, ++index) {
    if (row + this->m_scrollTop == index) {
      return *skill;
    }
  }

  return CSkillInfo(0);
}

// OFFSET: 0x0095FE20
// FUNCTION: fn_13088
void UIGuildPositionManageWnd::Slot25(int x, int y) {
  UIGuildWnd::Slot25(x, y);
  const int previous = this->m_hoverIndex;
  this->m_hoverIndex = this->PositionIndexAt(x, y);
  if (previous != this->m_hoverIndex) {
    this->Invalidate();
  }
}

// OFFSET: 0x00960430
// FUNCTION: fn_13104
void UIGuildPositionManageWnd::Slot32(int x, int y) {
  const int previous = this->m_hoverIndex;
  this->m_hoverIndex = this->PositionIndexAt(x, y);
  if (previous != this->m_hoverIndex) {
    this->Invalidate();
  }
}

// OFFSET: 0x00960470
// FUNCTION: fn_13105
void UIGuildSkillWnd::Slot32(int x, int y) {
  const int previous = this->m_hoverIndex;
  this->m_hoverIndex = this->SkillIndexAt(x, y);
  if (previous != this->m_hoverIndex) {
    this->Invalidate();
  }
}

// OFFSET: 0x00960BA0
// FUNCTION: fn_13117
void UIGuildMemberManageWnd::UpdateScroll() {
  if (!this->m_scrollBar) {
    return;
  }

  this->m_visibleRows = 6 - this->m_rowSpacing * 6 / 36;
  const int visible = this->m_visibleRows;
  const int rows = this->m_rowCount / this->m_columns + (this->m_rowCount % this->m_columns != 0);
  int range = 0;
  if (rows - visible >= 0) {
    range = rows - visible;
  }

  this->m_scrollable = range != 0;
  if (range == 0) {
    this->m_scrollTop = 0;
    this->m_scrollBar->SetRange(0, visible, rows);
  } else {
    this->m_scrollBar->SetRange(range, visible, rows);
    int top = this->m_scrollTop;
    if (top < 0) {
      this->m_scrollTop = 0;
      top = 0;
    }
    const int last = rows - this->m_visibleRows;
    if (last < top) {
      this->m_scrollTop = last;
      top = last;
    }
    this->m_scrollBar->SetPosition(top);
  }

  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x00960C80
// FUNCTION: fn_13118
void UIGuildPositionManageWnd::UpdateScroll() {
  if (!this->m_scrollBar) {
    return;
  }

  const int visible = this->m_visibleRows;
  const int rows = this->m_positions.size() / this->m_columns + (this->m_positions.size() % this->m_columns != 0);
  int range = 0;
  if (rows - visible >= 0) {
    range = rows - visible;
  }

  this->m_scrollable = range != 0;
  if (range == 0) {
    this->m_scrollTop = 0;
    this->m_scrollBar->SetRange(0, visible, rows);
  } else {
    this->m_scrollBar->SetRange(range, visible, rows);
    int top = this->m_scrollTop;
    if (top < 0) {
      this->m_scrollTop = 0;
      top = 0;
    }
    const int last = rows - this->m_visibleRows;
    if (last < top) {
      this->m_scrollTop = last;
      top = last;
    }
    this->m_scrollBar->SetPosition(top);
  }

  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x00960D30
// FUNCTION: fn_13119
void UIGuildSkillWnd::UpdateScroll() {
  if (!this->m_scrollBar) {
    return;
  }

  const int visible = this->m_visibleRows;
  const int rows = this->m_skills.size() / this->m_columns + (this->m_skills.size() % this->m_columns != 0);
  int range = 0;
  if (rows - visible >= 0) {
    range = rows - visible;
  }

  this->m_scrollable = range != 0;
  if (range == 0) {
    this->m_scrollTop = 0;
    this->m_scrollBar->SetRange(0, visible, rows);
  } else {
    this->m_scrollBar->SetRange(range, visible, rows);
    int top = this->m_scrollTop;
    if (top < 0) {
      this->m_scrollTop = 0;
      top = 0;
    }
    const int last = rows - this->m_visibleRows;
    if (last < top) {
      this->m_scrollTop = last;
      top = last;
    }
    this->m_scrollBar->SetPosition(top);
  }

  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x00960230
// FUNCTION: fn_13099
void UIGuildInfoManageWnd::Slot19() {
  const std::string name = g_guildMgr->GetEmblemFilename(this->m_guildId);
  CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(name));
  if (emblem && emblem->gif.frameChanged == true) {
    this->Invalidate();
  }
}

// OFFSET: 0x009656A0
// FUNCTION: fn_13140
void UIGuildWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->unknown_0010 = this->Tab();
  }
}

// OFFSET: 0x00957EF0

// OFFSET: 0x00964D50
// FUNCTION: fn_13135
int UIGuildWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message == 34) {
    this->m_saved = reinterpret_cast<GuildWndInfo*>(a1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;
  }

  if (message == 60) {
    this->InvalidateChildren();
    return 0;
  }

  if (message != 69) {
    return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  int permissions[7] = {0, 1, 2, 4, 16, 128, 64};
  switch (a1) {
  case 1:
  case 2:
  case 4:
  case 5:
    if ((permissions[a1] & GuildData().unknown_00A4) == 0) {
      return 0;
    }
    break;
  case 6:
    return 0;
  }

  this->m_frameContext->windows.CloseWindow(59);
  switch (a1) {
  case 0:
    this->m_frameContext->windows.EnsureWindow(60);
    return 0;
  case 1:
    this->m_frameContext->windows.EnsureWindow(61);
    return 0;
  case 2:
    this->m_frameContext->windows.EnsureWindow(62);
    return 0;
  case 3:
    this->m_frameContext->windows.EnsureWindow(63);
    return 0;
  case 4:
    this->m_frameContext->windows.EnsureWindow(64);
    return 0;
  case 5:
    this->m_frameContext->windows.EnsureWindow(65);
    return 0;
  case 6:
    this->m_frameContext->windows.EnsureWindow(66);
    return 0;
  }

  return 0;
}

// OFFSET: 0x00960AF0

// OFFSET: 0x0095FBF0

// OFFSET: 0x00961600

// OFFSET: 0x00960DE0

// OFFSET: 0x0095FF60

// OFFSET: 0x00964C90

// OFFSET: 0x0095D3A0

// OFFSET: 0x00957DA0

// OFFSET: 0x00960380

// OFFSET: 0x0095FCF0

// OFFSET: 0x00960540

// OFFSET: 0x00957DE0

// OFFSET: 0x00962310
