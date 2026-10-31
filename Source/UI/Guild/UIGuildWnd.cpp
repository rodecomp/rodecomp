#include "UI/Guild/UIGuildWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/CharacterConfigSave.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Guild/UIAgitWnd.h"
#include "UI/Guild/UIGuildHelperWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include <windows.h>
#include <timeapi.h>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <string>

// GLOBAL: gl_24999
int g_guildMemberColumnWidths[6] = {98, 65, 53, 39, 39, 76};

// GLOBAL: gl_25001
int g_guildPositionColumnWidths[6] = {50, 100, 60, 60, 60, 60};

// GLOBAL: gl_24918
int g_guildPositionRowHeight = 19;

// GLOBAL: gl_24919
int g_guildBanishedColumnWidths[2] = {82, 292};

// GLOBAL: gl_24920
int g_guildBanishedRowHeight = 19;

// GLOBAL: gl_24921
int g_guildTotalInfoRowHeight = 19;

// GLOBAL: gl_24990
int g_guildTotalInfoColumnWidths[4] = {151, 82, 70, 70};

struct GuildTotalInfoEntry {
  /* 0000 */ int unknown_0000;
  /* 0004 */ std::string name;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ int unknown_0028;
};

// GLOBAL: gl_24994
std::string g_guildNamelessTag = "<NAMELESS>";

// GLOBAL: gl_25131
bool g_guildOnlineFirst;
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
    if (static_cast<unsigned>(row) >= this->m_members.size()) {
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

// OFFSET: 0x00954020
UIGuildWnd::UIGuildWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_saved(nullptr), m_tabs{}, m_controls(&controls) {}
// OFFSET: 0x00953900
// FUNCTION: fn_12995
UIGuildBanishedMemberWnd::UIGuildBanishedMemberWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIGuildWnd(windows, frames, controls) {
}

// OFFSET: 0x009539C0
// FUNCTION: fn_12997
UIGuildInfoManageWnd::UIGuildInfoManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIGuildWnd(windows, frames, controls), unknown_00D8(), unknown_00E0(), unknown_00EC(0), unknown_00F0(0), m_guildId(g_guildId) {
}

// OFFSET: 0x00953B00
// FUNCTION: fn_12999
UIGuildMemberManageWnd::UIGuildMemberManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIGuildWnd(windows, frames, controls), unknown_0150(), m_scrollTop(0), m_hoverRow(10), m_columns(1), m_visibleRows(6), unknown_016C(0),
      m_scrollable(0), m_rowSpacing(0), m_members(), unknown_0180(), m_sortOrders() {
  this->m_buttonTime = 0;
  this->m_scrollBar = nullptr;
  for (int index = 0; index < 6; ++index) {
    this->m_rowButtons[index] = nullptr;
  }
  this->m_sortColumn = -1;

  for (auto& order : this->m_sortOrders) {
    order.second = 0;
  }
}

// OFFSET: 0x00953D10
// FUNCTION: fn_13000
UIGuildNoticeWnd::UIGuildNoticeWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls) : UIGuildWnd(windows, frames, controls) {
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
GuildPositionEntry& UIGuildPositionManageWnd::PositionAt(int index, int* found) {
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
__declspec(noinline) CSkillInfo UIGuildSkillWnd::SkillAt(int index) {
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
  const int rows = this->m_members.size() / this->m_columns + (this->m_members.size() % this->m_columns != 0);
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
// FUNCTION: fn_13053
__declspec(noinline) void UIGuildWnd::CreateTabs() {
  if (this->m_tabs[0]) {
    return;
  }

  if (GuildData().unknown_00A4 == -1) {
    g_modeMgr.gameMode->SendMsg(105, 0, 0, 0, 0);
  }

  const char* names[6];
  names[0] = LookupMessage(340);
  names[1] = LookupMessage(341);
  names[2] = LookupMessage(342);
  names[3] = LookupMessage(343);
  names[4] = LookupMessage(344);
  names[5] = LookupMessage(345);

  const char** name = names;
  UIStaticText** tab = this->m_tabs;
  for (int x = 3; x < 399; x += 66) {
    *tab = new UIStaticText(*this->m_context, *this->m_controls);
    (*tab)->SetSize(62, 15);
    (*tab)->SetTextFitted(*name);
    (*tab)->Move(x, 22);
    this->AddChild(*tab);
    ++name;
    ++tab;
  }

  this->AddCloseButton(*this->m_controls, "Alt+G", 0);
}

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
// FUNCTION: fn_13116
void UIGuildBanishedMemberWnd::UpdateScroll() {
  if (!this->m_scrollBar) {
    return;
  }

  const int rows = GuildData().unknown_00EC.size() / this->m_columns + (GuildData().unknown_00EC.size() % this->m_columns != 0);
  const int visible = this->m_visibleRows;
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

// OFFSET: 0x0095FBF0
// FUNCTION: fn_13083
void UIGuildBanishedMemberWnd::Slot25(int x, int y) {
  UIGuildWnd::Slot25(x, y);
  const int previous = this->m_hoverRow;
  int hover = -1;
  if (x <= this->geometry.width - 14 && x >= 4 && y >= g_guildBanishedRowHeight + 46 && y < (this->m_visibleRows + 1) * g_guildBanishedRowHeight + 46) {
    const int row = (y - g_guildBanishedRowHeight - 46) / g_guildBanishedRowHeight;
    int visible = 0;
    int index = 0;
    const auto& members = GuildData().unknown_00EC;
    for (auto member = members.begin(); member != members.end(); ++member, ++index) {
      if (index >= this->m_scrollTop * this->m_columns) {
        if (visible == this->m_columns * row) {
          hover = index;
          break;
        }
        ++visible;
      }
    }
  }

  this->m_hoverRow = hover;
  if (previous != hover) {
    this->Invalidate();
  }
}

// OFFSET: 0x00961600
// FUNCTION: fn_13125
int UIGuildBanishedMemberWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  switch (message) {
  case 6:
    if (a1 == 201) {
      this->m_frameContext->windows.CloseWindow(59);
      return 0;
    }
    break;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scrollTop;
    int top;
    if (message == 9) {
      top = previous - this->m_visibleRows + 1;
    } else {
      top = a1;
      if (message == 10) {
        top = this->m_visibleRows - 1;
      }
      top += previous;
    }
    this->m_scrollTop = top;
    if (previous != top) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scrollTop - previous;
  }
  case 60:
    this->InvalidateChildren();
    this->UpdateScroll();
    break;
  default:
    return UIGuildWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  return 0;
}

// OFFSET: 0x00960DE0
// FUNCTION: fn_13120
void UIGuildTotalInfoWnd::UpdateScroll() {
  if (!this->m_scrollBar) {
    return;
  }

  const int rows = GuildData().unknown_00F4.size() / this->m_columns + (GuildData().unknown_00F4.size() % this->m_columns != 0);
  const int visible = this->m_visibleRows;
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

// OFFSET: 0x0095FF60
// FUNCTION: fn_13090
void UIGuildTotalInfoWnd::Slot25(int x, int y) {
  UIGuildWnd::Slot25(x, y);
  const int previous = this->m_hoverRow;
  int hover = -1;
  if (x <= this->geometry.width - 14 && x >= 4 && y >= g_guildTotalInfoRowHeight + 46 && y < (this->m_visibleRows + 1) * g_guildTotalInfoRowHeight + 46) {
    const int row = (y - g_guildTotalInfoRowHeight - 46) / g_guildTotalInfoRowHeight;
    int visible = 0;
    int index = 0;
    const auto& entries = GuildData().unknown_00F4;
    for (auto entry = entries.begin(); entry != entries.end(); ++entry, ++index) {
      if (index >= this->m_scrollTop * this->m_columns) {
        if (visible == this->m_columns * row) {
          hover = index;
          break;
        }
        ++visible;
      }
    }
  }

  this->m_hoverRow = hover;
  if (previous != hover) {
    this->Invalidate();
  }
}

// OFFSET: 0x00964C90
// FUNCTION: fn_13134
int UIGuildTotalInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  switch (message) {
  case 6:
    if (a1 == 201) {
      this->m_frameContext->windows.CloseWindow(59);
    }
    return 0;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scrollTop;
    int top;
    if (message == 9) {
      top = previous - this->m_visibleRows + 1;
    } else {
      top = a1;
      if (message == 10) {
        top = this->m_visibleRows - 1;
      }
      top += previous;
    }
    this->m_scrollTop = top;
    if (previous != top) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scrollTop - previous;
  }
  default:
    return UIGuildWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }
}

// OFFSET: 0x0095D3A0
// FUNCTION: fn_13066
void UIGuildTotalInfoWnd::Slot15(int, int) {
  this->CreateTabs();
  if (std::strcmp(g_guildName.c_str(), "") == 0) {
    g_modeMgr.gameMode->SendMsg(107, 0, 0, 0, 0);
  }

  GuildData().unknown_00D0 = 1;
  g_modeMgr.gameMode->SendMsg(107, 5, 0, 0, 0);
  this->m_scrollTop = 0;
  this->m_hoverRow = 0;
  this->m_columns = 1;
  this->m_visibleRows = 12;
  this->m_scrollable = 0;
  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(this->geometry.width - 15, 63, 1, this->geometry.height - 87);
  this->AddChild(this->m_scrollBar);

  for (int index = 0; index < 12; ++index) {
    this->m_rows[index] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_rows[index]->SetSize(g_guildBanishedColumnWidths[0] - 4, 15);
    this->m_rows[index]->SetTextFitted("");
    this->m_rows[index]->Move(8, index * g_guildTotalInfoRowHeight + 63);
    this->AddChild(this->m_rows[index]);
  }

  this->UpdateScroll();
}

// OFFSET: 0x00958740
// FUNCTION: fn_13057
void UIGuildBanishedMemberWnd::Slot15(int, int) {
  this->CreateTabs();
  if (std::strcmp(g_guildName.c_str(), "") == 0) {
    g_modeMgr.gameMode->SendMsg(107, 0, 0, 0, 0);
  }

  GuildData().unknown_00CC = 1;
  g_modeMgr.gameMode->SendMsg(107, 4, 0, 0, 0);
  this->m_scrollTop = 0;
  this->m_hoverRow = 0;
  this->m_columns = 1;
  this->m_visibleRows = 11;
  this->m_scrollable = 0;
  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(this->geometry.width - 15, 63, 1, this->geometry.height - 87);
  this->AddChild(this->m_scrollBar);

  for (int index = 0; index < 11; ++index) {
    this->m_names[index] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_names[index]->SetSize(g_guildBanishedColumnWidths[0] - 4, 15);
    this->m_names[index]->SetTextFitted("");
    this->m_names[index]->Move(8, index * g_guildBanishedRowHeight + 63);
    this->AddChild(this->m_names[index]);

    this->m_reasons[index] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_reasons[index]->SetSize(g_guildBanishedColumnWidths[1] - 4, 15);
    this->m_reasons[index]->SetTextFitted("");
    this->m_reasons[index]->Move(g_guildMemberColumnWidths[0] + 58, index * g_guildBanishedRowHeight + 63);
    this->AddChild(this->m_reasons[index]);
  }

  this->UpdateScroll();
}

// OFFSET: 0x00958FF0
// FUNCTION: fn_13059
void UIGuildInfoManageWnd::Slot15(int, int) {
  this->CreateTabs();
  this->m_allyRow = -1;
  this->m_hostileRow = -1;
  if (std::strcmp(g_guildName.c_str(), "") == 0) {
    g_modeMgr.gameMode->SendMsg(107, 0, 0, 0, 0);
  }

  GuildData().unknown_00CC = 1;
  g_modeMgr.gameMode->SendMsg(107, 4, 0, 0, 0);
  GuildData().unknown_00BC = 1;
  g_modeMgr.gameMode->SendMsg(107, 0, 0, 0, 0);
  GuildData().unknown_00C0 = 1;
  g_modeMgr.gameMode->SendMsg(107, 1, 0, 0, 0);

  {
    const std::string prefix("유저인터페이스\\");
    this->m_editButton = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_editButton->SetBitmap((prefix + "btn_edit" + ".bmp").c_str(), 0, 0);
    this->m_editButton->SetBitmap((prefix + "btn_edit_a" + ".bmp").c_str(), 1, 0);
    this->m_editButton->SetBitmap((prefix + "btn_edit_b" + ".bmp").c_str(), 2, 0);
    this->m_editButton->SetSize(this->m_editButton->BitmapWidth(), this->m_editButton->BitmapHeight());
    if (GuildData().unknown_00B4) {
      this->m_editButton->Move(this->geometry.width - this->m_editButton->geometry.width - 8, 69);
    } else {
      this->m_editButton->Move(-200, -200);
    }
    this->m_editButton->SetId(278);
    this->AddChild(this->m_editButton);

    this->m_disbandButton = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_disbandButton->SetBitmap("유저인터페이스\\btn_disBand.bmp", 0, 0);
    this->m_disbandButton->SetBitmap("유저인터페이스\\btn_disBand_a.bmp", 1, 0);
    this->m_disbandButton->SetBitmap("유저인터페이스\\btn_disBand_b.bmp", 2, 0);
    this->m_disbandButton->SetSize(this->m_disbandButton->BitmapWidth(), this->m_disbandButton->BitmapHeight());
    if (GuildData().unknown_00B4) {
      this->m_disbandButton->Move(this->geometry.width - this->m_disbandButton->geometry.width - 15, this->geometry.height - this->m_disbandButton->geometry.height - 5);
    } else {
      this->m_disbandButton->Move(-200, -200);
    }
    this->m_disbandButton->SetId(449);
    this->AddChild(this->m_disbandButton);
  }

  this->m_agitList = new UIListBoxAgit(*this->m_context, *this->m_listContext);
  this->m_agitList->SetSize(95, 158);
  this->m_agitList->Move(this->TextWidth(this->m_controls->fonts, this->m_controls->locale, LookupMessage(333), 0, 0, 12, 0, 0) + 19, 129);
  this->m_agitList->lineHeight = 13;
  this->m_agitList->SetId(558);
  this->AddChild(this->m_agitList);

  this->m_agitMenu = new UIListBoxAgitMenu(*this->m_context, *this->m_listContext);
  this->m_agitMenu->SetSize(120, 35);
  this->m_agitMenu->SetId(559);
  this->m_agitMenu->Show(0);
  this->m_agitMenu->AddLine(LookupMessage(3629));
  this->m_agitMenu->AddLine(LookupMessage(3630));
  this->AddChild(this->m_agitMenu);
  this->RefreshAgitList();
}

// OFFSET: 0x00957DA0
// FUNCTION: fn_13051
void UIGuildInfoManageWnd::HideAgitMenu() {
  if (this->m_agitMenu && this->m_agitMenu->geometry.visible) {
    this->m_agitMenu->Show(0);
    this->m_agitMenu->m_agitIndex = -1;
    this->Invalidate();
  }
}

// OFFSET: 0x00960380
// FUNCTION: fn_13102
void UIGuildInfoManageWnd::Slot32(int, int) {
  this->HideAgitMenu();
}

// OFFSET: 0x0095FCF0
// FUNCTION: fn_13085
void UIGuildInfoManageWnd::Slot25(int x, int y) {
  UIGuildWnd::Slot25(x, y);
  this->HideAgitMenu();
}

// OFFSET: 0x00960540
// FUNCTION: fn_13107
void UIGuildInfoManageWnd::Slot33(int x, int y) {
  if (x < 201 || x > 287) {
    return;
  }

  if (y > 157 && y < 205) {
    const int count = GuildData().CountRelatedGuilds(0);
    this->m_allyRow = -1;
    this->m_hostileRow = -1;
    if (count != 0) {
      this->m_allyRow = (y - 157) / 16;
      if (count > this->m_allyRow) {
        this->SendMsg(nullptr, 49, 0, 0, 0, 0);
      }
    }
  } else if (y > 231 && y < 279) {
    const int count = GuildData().CountRelatedGuilds(1);
    this->m_allyRow = -1;
    this->m_hostileRow = -1;
    if (count != 0) {
      this->m_hostileRow = (y - 231) / 16;
      if (count > this->m_hostileRow) {
        this->SendMsg(nullptr, 49, 0, 0, 0, 0);
      }
    }
  } else {
    return;
  }

  this->Invalidate();
}

// OFFSET: 0x00957DE0
// FUNCTION: fn_13052
void UIGuildInfoManageWnd::RefreshAgitList() {
  this->m_agitList->Clear();
  const int count = GuildData().GetUnknown0114Count();
  if (count != 0) {
    for (int index = 0; index < count; ++index) {
      const int agitId = GuildData().GetUnknown0114(index);
      char* agitName = nullptr;
      g_luaScript->Call("get_agit_name", "d>s", agitId, &agitName);
      if (strnlen(agitName, 256) != 0) {
        this->m_agitList->AddLine(agitName);
      }
    }
  }
}

// OFFSET: 0x00962310
// FUNCTION: fn_13128
int UIGuildLeaveReasonDescWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  if (a1 == 184) {
    g_modeMgr.gameMode->SendMsg(110, reinterpret_cast<int>(this->m_reasonEdit->Text()), 0, 0, 0);
    this->m_frameContext->windows.CloseWindow(75);
  } else if (a1 == 185) {
    this->m_frameContext->windows.CloseWindow(75);
  }

  return 0;
}

// OFFSET: 0x009652D0
// FUNCTION: fn_13137
std::string UIGuildWnd::StripNamelessTag(const std::string& name) {
  std::string result(name);
  const std::size_t position = name.find(g_guildNamelessTag);
  if (position != std::string::npos) {
    result.clear();
    result = name.substr(0, position);
  }

  return result;
}

// OFFSET: 0x0095E8A0
// FUNCTION: fn_13075
void UIGuildNoticeWnd::Paint() {
  UIGuildWnd::Paint();
  char label[64];
  std::sprintf(label, "%s:", LookupMessage(515));
  auto& controls = *this->m_controls;
  this->DrawTextAt(controls.fonts, controls.locale, 5, 67, label, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 86, LookupMessage(516), 0, 0, 12, 0, 0, 0);
  this->DrawRightAlignedText(this->geometry.width - 7, 266, FormatOwnedString("[%d/%d]", this->m_bodyEdit->Length(), 119).c_str(), 0, 0, 12, 0, 0);
}

// OFFSET: 0x00960E90
// FUNCTION: fn_13121
void UIGuildSkillWnd::UpdateSkillButtons() {
  int row = 0;
  for (int y = 34; y < 272; y += 34, ++row) {
    CSkillInfo skill = this->VisibleSkillAt(row);
    if (GuildData().unknown_00B4 && row < this->m_visibleRows && this->m_scrollTop + row < this->m_skills.size() && g_session.m_guildSkillPoint &&
        skill.available && skill.unknown_0018) {
      this->m_levelUpButtons[row]->Move(78, y + 11);
    } else {
      this->m_levelUpButtons[row]->Move(-200, y - 17);
    }
    this->m_previousButtons[row]->Move(-200, y);
    this->m_nextButtons[row]->Move(-200, y);
  }
}

// OFFSET: 0x00957A10
// FUNCTION: fn_13040
bool UIGuildMemberManageWnd::GetMember(int index, GuildInfo::Member& member) {
  int current = 0;
  for (auto entry = this->m_members.begin(); entry != this->m_members.end(); ++entry, ++current) {
    if (index == current) {
      member = *entry;
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00960660
// FUNCTION: fn_13110
void UIGuildMemberManageWnd::SortMembers(int column) {
  if (this->m_sortColumn != column) {
    this->m_sortColumn = column;
    this->m_sortOrders[this->m_sortColumn] = 1;
  } else if (this->m_sortOrders[this->m_sortColumn] == 1) {
    this->m_sortOrders[this->m_sortColumn] = 2;
  } else {
    this->m_sortOrders[this->m_sortColumn] = 1;
  }

  switch (column) {
  case 0:
    if (this->m_sortOrders[this->m_sortColumn] == 1) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return CompareWideText(left.name.c_str(), right.name.c_str());
      });
    } else if (this->m_sortOrders[this->m_sortColumn] == 2) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return !CompareWideText(left.name.c_str(), right.name.c_str());
      });
    }
    break;
  case 1:
    if (this->m_sortOrders[this->m_sortColumn] == 1) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.positionId < right.positionId;
      });
    } else if (this->m_sortOrders[this->m_sortColumn] == 2) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.positionId > right.positionId;
      });
    }
    break;
  case 2:
    if (this->m_sortOrders[this->m_sortColumn] == 1) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return CompareWideText(g_session.GetJobName(left.job, -1), g_session.GetJobName(right.job, -1));
      });
    } else if (this->m_sortOrders[this->m_sortColumn] == 2) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return !CompareWideText(g_session.GetJobName(left.job, -1), g_session.GetJobName(right.job, -1));
      });
    }
    break;
  case 3:
    if (this->m_sortOrders[this->m_sortColumn] == 1) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.level < right.level;
      });
    } else if (this->m_sortOrders[this->m_sortColumn] == 2) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.level > right.level;
      });
    }
    break;
  case 4:
    if (this->m_sortOrders[this->m_sortColumn] == 1) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.expRatio < right.expRatio;
      });
    } else if (this->m_sortOrders[this->m_sortColumn] == 2) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.expRatio > right.expRatio;
      });
    }
    break;
  case 5:
    if (this->m_sortOrders[this->m_sortColumn] == 1) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.exp < right.exp;
      });
    } else if (this->m_sortOrders[this->m_sortColumn] == 2) {
      this->m_members.sort([](const GuildInfo::Member& left, const GuildInfo::Member& right) {
        return left.exp > right.exp;
      });
    }
    break;
  }

  this->Invalidate();
}

// OFFSET: 0x0095FD40
// FUNCTION: fn_13086
void UIGuildMemberManageWnd::Slot25(int x, int y) {
  UIGuildWnd::Slot25(x, y);
  if (x >= 6 && x <= 381 && y >= 47 && y <= 66) {
    int left = 6;
    for (int column = 0; column < 6; ++column) {
      const int right = g_guildMemberColumnWidths[column] + left;
      if (left <= x && x <= right) {
        this->SortMembers(column);
        return;
      }
      left = right + 1;
    }
    return;
  }

  const int previous = this->m_hoverRow;
  int row;
  if (static_cast<unsigned>(x - 5) <= 377 && static_cast<unsigned>(y - 66) <= 217) {
    row = (y - 66) / (this->m_rowSpacing + 36) + this->m_columns * this->m_scrollTop;
    if (static_cast<unsigned>(row) >= this->m_members.size()) {
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

// OFFSET: 0x0095F130
// FUNCTION: fn_13077
void UIGuildSkillWnd::Paint() {
  UIGuildWnd::Paint();
  auto& controls = *this->m_controls;
  int drawn = 0;
  int index = 0;
  for (auto skill = this->m_skills.begin(); skill != this->m_skills.end(); ++skill, ++index) {
    if (index < this->m_scrollTop * this->m_columns) {
      continue;
    }

    const int x = drawn % this->m_columns * 32;
    const int y = drawn / this->m_columns * 34;
    if (this->m_hoverIndex == index) {
      unsigned red, green, blue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      if (skill->level == 0) {
        SkinPaletteChannels(ResourceMgr::Instance(), 10, 2, red, green, blue);
      } else if (skill->unknown_0014 == 0) {
        SkinPaletteChannels(ResourceMgr::Instance(), 14, 2, red, green, blue);
      }
      this->FillRegion(x + 104, y + 43, 164, 28, RGB(red, green, blue));
    }

    char text[80];
    g_session.ItemBitmapPath(skill->name, text);
    this->DrawBitmap(x + 44, y + 45, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)))->image, true);
    unsigned gray;
    if (skill->level == 0) {
      gray = 120;
    } else {
      std::sprintf(text, g_serviceType == 12 ? "Nv : %d" : "Lv : %d", skill->level);
      this->DrawTextAt(controls.fonts, controls.locale, x + 113, y + 58, text, 0, 0, 12, 0, 0, 0);
      if (skill->unknown_000C == 0) {
        std::sprintf(text, LookupMessage(286));
      } else {
        std::sprintf(text, "Sp : %d", skill->unknown_0014);
      }
      this->DrawTextAt(controls.fonts, controls.locale, x + 212, y + 58, text, 0, 0, 12, 0, 0, 0);
      gray = 0;
    }

    this->DrawTextAt(controls.fonts, controls.locale, x + 112, y + 46, skill->DisplayName(*g_session.m_hotkeyScript), 0, 0, 12, RGB(gray, gray, gray), 0, 0);
    if (++drawn == this->m_visibleRows * this->m_columns) {
      break;
    }
  }

  char points[64];
  std::sprintf(points, "SkillPoint : %d", g_session.m_guildSkillPoint);
  this->DrawTextAt(controls.fonts, controls.locale, this->geometry.width - 100, this->geometry.height - 20, points, 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0095E9B0
// FUNCTION: fn_13076
void UIGuildPositionManageWnd::Paint() {
  UIGuildWnd::Paint();
  for (int index = 0; index < 11; ++index) {
    if (this->unknown_00D0[index]) {
      this->unknown_00D0[index]->Move(-200, -200);
    }
    if (this->unknown_00FC[index]) {
      this->unknown_00FC[index]->Move(-200, -200);
    }
    if (this->unknown_0180[index]) {
      this->unknown_0180[index]->Move(-200, -200);
    }
    if (this->unknown_0154[index]) {
      this->unknown_0154[index]->Move(-200, -200);
    }
    if (this->unknown_01D8[index]) {
      this->unknown_01D8[index]->Move(-200, -200);
    }
    if (this->unknown_01AC[index]) {
      this->unknown_01AC[index]->Move(-200, -200);
    }
    if (this->unknown_0128[index]) {
      this->unknown_0128[index]->Move(-200, -200);
    }
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(4, 45, this->geometry.width - 22, this->geometry.height - g_guildPositionRowHeight - 69, RGB(red, green, blue));

  const int cellHeight = g_guildPositionRowHeight - 1;
  int widths[6];
  widths[0] = g_guildPositionColumnWidths[0];
  widths[1] = g_guildPositionColumnWidths[1];
  widths[2] = g_guildPositionColumnWidths[2];
  widths[3] = g_guildPositionColumnWidths[3];
  widths[4] = g_guildPositionColumnWidths[4];
  widths[5] = g_guildPositionColumnWidths[5];
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  for (int row = 0; row < 12; ++row) {
    int x = 0;
    for (int column = 0; column < 6; ++column) {
      const int width = widths[column];
      this->FillRegion(x + 5, row * g_guildPositionRowHeight + 46, width, cellHeight, RGB(red, green, blue));
      x += width + 1;
    }
  }

  const char* headers[6];
  headers[0] = LookupMessage(510);
  headers[1] = LookupMessage(511);
  headers[2] = LookupMessage(512);
  headers[3] = LookupMessage(513);
  headers[4] = LookupMessage(2499);
  headers[5] = LookupMessage(514);
  auto& controls = *this->m_controls;
  int x = 0;
  for (int column = 0; column < 6; ++column) {
    this->DrawTextAt(controls.fonts, controls.locale, x + 8, 49, headers[column], 0, 0, 12, 0, 0, 0);
    x += widths[column] + 1;
  }

  int drawn = 0;
  int index = 0;
  for (auto position = this->m_positions.begin(); position != this->m_positions.end(); ++position, ++index) {
    if (index < this->m_scrollTop * this->m_columns) {
      continue;
    }

    const int y = drawn / this->m_columns * g_guildPositionRowHeight + 68;
    if (this->m_hoverIndex == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(5, y - 3, this->geometry.width - 24, 18, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    char buffer[512];
    char path[128];
    std::sprintf(buffer, "%d", position->id);
    this->DrawTextAt(controls.fonts, controls.locale, 8, y, buffer, 0, 0, 12, 0, 0, 0);
    if (this->unknown_0180[drawn]) {
      this->unknown_0180[drawn]->SetText(position->name.c_str());
      this->unknown_0180[drawn]->Move(g_guildPositionColumnWidths[0] + 9, y - 2);
    } else if (this->unknown_01D8[drawn]) {
      this->unknown_01D8[drawn]->SetTextFitted(position->name.c_str());
      this->unknown_01D8[drawn]->Move(g_guildPositionColumnWidths[0] + 9, y - 2);
    }

    if (this->unknown_00D0[drawn]) {
      this->unknown_00D0[drawn]->geometry.unknown_0030 = position->canInvite;
      this->unknown_00D0[drawn]->Invalidate();
      this->unknown_00D0[drawn]->Move(g_guildPositionColumnWidths[1] + g_guildPositionColumnWidths[0] + 10, y);
    } else {
      std::sprintf(path, "유저인터페이스\\checkbox_%d.bmp", position->canInvite != 0);
      this->DrawBitmap(
        g_guildPositionColumnWidths[1] + g_guildPositionColumnWidths[0] + 10, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)))->image,
        false
      );
    }

    if (this->unknown_00FC[drawn]) {
      this->unknown_00FC[drawn]->geometry.unknown_0030 = position->canExpel;
      this->unknown_00FC[drawn]->Invalidate();
      this->unknown_00FC[drawn]->Move(g_guildPositionColumnWidths[0] + 11 + g_guildPositionColumnWidths[1] + g_guildPositionColumnWidths[2], y);
    } else {
      std::sprintf(path, "유저인터페이스\\checkbox_%d.bmp", position->canExpel != 0);
      this->DrawBitmap(
        g_guildPositionColumnWidths[0] + 11 + g_guildPositionColumnWidths[1] + g_guildPositionColumnWidths[2], y,
        &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)))->image, false
      );
    }

    const int storageX =
      g_guildPositionColumnWidths[0] + 12 + g_guildPositionColumnWidths[2] + g_guildPositionColumnWidths[3] + g_guildPositionColumnWidths[1];
    if (this->unknown_0128[drawn]) {
      this->unknown_0128[drawn]->geometry.unknown_0030 = position->canStorage;
      this->unknown_0128[drawn]->Invalidate();
      this->unknown_0128[drawn]->Move(storageX, y);
    } else {
      std::sprintf(path, "유저인터페이스\\checkbox_%d.bmp", position->canStorage != 0);
      this->DrawBitmap(storageX, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)))->image, false);
    }

    const int taxX = g_guildPositionColumnWidths[0] + 13 + g_guildPositionColumnWidths[3] + g_guildPositionColumnWidths[4] + g_guildPositionColumnWidths[2] +
      g_guildPositionColumnWidths[1];
    if (this->unknown_0154[drawn]) {
      std::sprintf(buffer, "%d", position->tax);
      this->unknown_0154[drawn]->SetText(buffer);
      this->unknown_0154[drawn]->Move(taxX, y - 2);
      this->DrawTextAt(
        controls.fonts, controls.locale,
        g_guildPositionColumnWidths[5] / 2 + g_guildPositionColumnWidths[4] + g_guildPositionColumnWidths[3] + g_guildPositionColumnWidths[2] +
          g_guildPositionColumnWidths[1] + g_guildPositionColumnWidths[0] + 13,
        y, "%", 0, 0, 12, 0, 0, 0
      );
    } else if (this->unknown_01AC[drawn]) {
      std::sprintf(buffer, "%d %s", position->tax, "%");
      this->unknown_01AC[drawn]->SetText(buffer, 0);
      this->unknown_01AC[drawn]->Move(taxX, y - 2);
    }

    if (++drawn == this->m_visibleRows * this->m_columns) {
      return;
    }
  }
}

// OFFSET: 0x0095DB80
// FUNCTION: fn_13072
void UIGuildInfoManageWnd::Paint() {
  UIGuildWnd::Paint();
  auto& controls = *this->m_controls;
  GuildInfo& guild = GuildData();
  char text[1024];
  std::sprintf(text, "%s : %s", LookupMessage(328), guild.unknown_0000.c_str());
  this->DrawTextAt(controls.fonts, controls.locale, 8, 49, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : %d", LookupMessage(329), guild.unknown_0060);
  this->DrawTextAt(controls.fonts, controls.locale, 8, 65, text, 0, 0, 12, 0, 0, 0);
  const std::string master = this->StripNamelessTag(g_guildName);
  std::sprintf(text, "%s : %s", LookupMessage(330), master.c_str());
  this->DrawTextAt(controls.fonts, controls.locale, 8, 81, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : %d / %d", LookupMessage(331), guild.unknown_0064, guild.unknown_006C);
  this->DrawTextAt(controls.fonts, controls.locale, 8, 97, text, 0, 0, 12, 0, 0, 0);

  const int width = this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0);
  std::sprintf(text, "%s", "유저인터페이스\\basic_interface\\grp_online.bmp");
  CBitmapRes* online = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
  int x;
  if (g_serviceType == 0) {
    this->DrawBitmap(128, 95, &online->image, true);
    std::sprintf(text, "   %d", guild.unknown_0068);
    x = 138;
  } else {
    this->DrawBitmap(width + 16, 95, &online->image, true);
    std::sprintf(text, " %d", guild.unknown_0068);
    this->TextWidth(controls.fonts, controls.locale, LookupMessage(330), 0, 0, 12, 0, 0);
    x = online->image.width + 16 + width;
  }
  this->DrawTextAt(controls.fonts, controls.locale, x, 97, text, 0, 0, 12, 0, 0, 0);

  std::sprintf(text, "%s : %d", LookupMessage(332), guild.unknown_0070);
  this->DrawTextAt(controls.fonts, controls.locale, 8, 113, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : %s", LookupMessage(333), guild.unknown_0074.c_str());
  this->DrawTextAt(controls.fonts, controls.locale, 8, 129, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : %d", LookupMessage(335), guild.unknown_008C);
  this->DrawTextAt(controls.fonts, controls.locale, 200, 49, text, 0, 0, 12, guild.unknown_0060 >= 50 ? 0xFF : 0, 0, 0);
  std::sprintf(text, "%s", LookupMessage(336));
  this->DrawTextAt(controls.fonts, controls.locale, 200, 73, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s", LookupMessage(338));
  this->DrawTextAt(controls.fonts, controls.locale, 200, 139, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s", LookupMessage(339));
  this->DrawTextAt(controls.fonts, controls.locale, 200, 213, text, 0, 0, 12, 0, 0, 0);

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
  this->FillRegion(299, 64, 26, 26, RGB(red, green, blue));
  {
    const std::string emblemName = g_guildMgr->GetEmblemFilename(this->m_guildId);
    CBitmapRes* emblem = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(emblemName));
    if (emblem) {
      this->DrawBitmap(300, 65, &emblem->image, true);
    }
  }

  unsigned boxRed, boxGreen, boxBlue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, boxRed, boxGreen, boxBlue);
  this->FillRegion(200, 155, 168, 48, RGB(boxRed, boxGreen, boxBlue));
  int row = 0;
  int y = 157;
  for (auto related = guild.unknown_00E4.begin(); related != guild.unknown_00E4.end(); ++related) {
    if (y > 204) {
      break;
    }
    if (related->unknown_0004 == 0) {
      if (row == this->m_allyRow) {
        unsigned hoverRed, hoverGreen, hoverBlue;
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
        this->FillRegion(204, y, 80, 16, RGB(hoverRed, hoverGreen, hoverBlue));
      }
      this->DrawTextAt(controls.fonts, controls.locale, 204, y, related->unknown_0008.c_str(), 0, 0, 12, 0, 0, 0);
      ++row;
      y += 16;
    }
  }

  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, boxRed, boxGreen, boxBlue);
  this->FillRegion(200, 229, 168, 48, RGB(boxRed, boxGreen, boxBlue));
  row = 0;
  y = 231;
  for (auto related = guild.unknown_00E4.begin(); related != guild.unknown_00E4.end(); ++related) {
    if (y > 278) {
      break;
    }
    if (related->unknown_0004 == 1) {
      if (row == this->m_hostileRow) {
        unsigned hoverRed, hoverGreen, hoverBlue;
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
        this->FillRegion(204, y, 80, 16, RGB(hoverRed, hoverGreen, hoverBlue));
      }
      this->DrawTextAt(controls.fonts, controls.locale, 204, y, related->unknown_0008.c_str(), 0, 0, 12, 0, 0, 0);
      ++row;
      y += 16;
    }
  }
}

// OFFSET: 0x0095FE70
// FUNCTION: fn_13089
void UIGuildSkillWnd::Slot25(int x, int y) {
  const int previous = this->m_hoverIndex;
  this->m_hoverIndex = this->SkillIndexAt(x, y);
  if (previous != this->m_hoverIndex) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling == 0) {
    CSkillInfo skill = this->SkillAt(this->m_hoverIndex);
    if (skill.available != 0 && skill.level != 0 && skill.unknown_000C != 0) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (gameMode) {
        int level = reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill.id);
        if (level == 0) {
          level = skill.level;
        }
        gameMode->drag.BeginSkill(
          reinterpret_cast<WindowManagerWindows&>(g_windowMgr), ResourceMgr::Instance(), 8, skill.id, skill.name, level, -1
        );
      }
    }

    UIGuildWnd::Slot25(x, y);
  }
}

// OFFSET: 0x0095F460

// FUNCTION: fn_13079
void UIGuildTotalInfoWnd::Paint() {
  UIGuildWnd::Paint();
  for (int index = 0; index < 12; ++index) {
    this->m_rows[index]->Move(-200, -200);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(4, 45, this->geometry.width - 22, this->geometry.height - 69, RGB(red, green, blue));

  const int cellHeight = g_guildTotalInfoRowHeight - 1;
  int widths[4];
  widths[0] = g_guildTotalInfoColumnWidths[0];
  widths[1] = g_guildTotalInfoColumnWidths[1];
  widths[2] = g_guildTotalInfoColumnWidths[2];
  widths[3] = g_guildTotalInfoColumnWidths[3];
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  for (int row = 0; row < 12; ++row) {
    int x = 0;
    for (int column = 0; column < 4; ++column) {
      const int width = widths[column];
      this->FillRegion(x + 5, row * g_guildTotalInfoRowHeight + 46, width, cellHeight, RGB(red, green, blue));
      x += width + 1;
    }
  }

  const char* headers[4];
  headers[0] = LookupMessage(517);
  headers[1] = LookupMessage(518);
  headers[2] = LookupMessage(519);
  headers[3] = LookupMessage(520);
  auto& controls = *this->m_controls;
  int x = 0;
  for (int column = 0; column < 4; ++column) {
    this->DrawTextAt(controls.fonts, controls.locale, x + 8, 49, headers[column], 0, 0, 12, 0, 0, 0);
    x += widths[column] + 1;
  }

  const auto& entries = *reinterpret_cast<const std::list<GuildTotalInfoEntry>*>(&GuildData().unknown_00F4);
  int drawn = 0;
  int index = 0;
  for (auto entry = entries.begin(); entry != entries.end(); ++entry, ++index) {
    if (index < this->m_scrollTop * this->m_columns) {
      continue;
    }

    const int y = drawn / this->m_columns * g_guildBanishedRowHeight + 68;
    if (this->m_hoverRow == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(5, y - 3, this->geometry.width - 24, 18, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    char buffer[512];
    this->m_rows[drawn]->SetTextFitted(entry->name.c_str());
    this->m_rows[drawn]->Move(8, y);
    std::sprintf(buffer, "%d", entry->unknown_001C);
    this->DrawTextAt(controls.fonts, controls.locale, g_guildTotalInfoColumnWidths[0] + 9, y, buffer, 0, 0, 12, 0, 0, 0);
    std::sprintf(buffer, "%d/%d", entry->unknown_0020, entry->unknown_0024);
    this->DrawTextAt(
      controls.fonts, controls.locale, g_guildTotalInfoColumnWidths[0] + g_guildTotalInfoColumnWidths[1] + 10, y, buffer, 0, 0, 12, 0, 0, 0
    );
    std::sprintf(buffer, "%d", entry->unknown_0028);
    this->DrawTextAt(
      controls.fonts, controls.locale,
      g_guildTotalInfoColumnWidths[0] + g_guildTotalInfoColumnWidths[1] + g_guildTotalInfoColumnWidths[2] + 11, y, buffer, 0, 0, 12, 0, 0, 0
    );
    if (++drawn == this->m_visibleRows * this->m_columns) {
      return;
    }
  }
}

// OFFSET: 0x00959F10
// FUNCTION: fn_13061
void UIGuildMemberManageWnd::Slot15(int, int) {
  this->CreateTabs();
  if (std::strcmp(g_guildName.c_str(), "") == 0) {
    g_modeMgr.gameMode->SendMsg(107, 0, 0, 0, 0);
  }

  GuildData().unknown_00C0 = 1;
  g_modeMgr.gameMode->SendMsg(107, 1, 0, 0, 0);
  this->m_members = g_guildMembers;
  this->m_rowSpacing += 8;

  for (int row = 0; row < 6; ++row) {
    this->m_rowTexts[1][row] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_rowTexts[1][row]->SetSize(g_guildMemberColumnWidths[0] / 2 + 2, 15);
    this->m_rowTexts[1][row]->SetTextFitted("");
    this->m_rowTexts[1][row]->Move(8, (this->m_rowSpacing + 36) * row + 63);
    this->AddChild(this->m_rowTexts[1][row]);

    this->m_rowTexts[2][row] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_rowTexts[2][row]->SetSize(g_guildMemberColumnWidths[1] - 16, 15);
    this->m_rowTexts[2][row]->SetTextFitted("");
    this->m_rowTexts[2][row]->Move(g_guildMemberColumnWidths[0] + 8, (this->m_rowSpacing + 36) * row + 63);
    this->AddChild(this->m_rowTexts[2][row]);

    this->m_rowTexts[3][row] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_rowTexts[3][row]->SetSize(39, 15);
    this->m_rowTexts[3][row]->SetTextFitted("");
    this->m_rowTexts[3][row]->Move(g_guildMemberColumnWidths[0] + 58, (this->m_rowSpacing + 36) * row + 63);
    this->AddChild(this->m_rowTexts[3][row]);

    this->m_rowTexts[0][row] = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_rowTexts[0][row]->SetSize(39, 15);
    this->m_rowTexts[0][row]->SetTextFitted("");
    this->m_rowTexts[0][row]->Move(g_guildMemberColumnWidths[0] + 101, (this->m_rowSpacing + 36) * row + 63);
    this->AddChild(this->m_rowTexts[0][row]);
  }

  if (GuildData().unknown_00B4) {
    const std::string prefix("유저인터페이스\\basic_interface\\");
    UIBitmapButton** button = this->m_rowButtons;
    for (int y = 82; y < 298; y += 36, ++button) {
      *button = new UIBitmapButton(*this->m_context, *this->m_controls);
      (*button)->SetBitmap((prefix + "txtbox_btn_a" + ".bmp").c_str(), 0, 0);
      (*button)->SetBitmap((prefix + "txtbox_btn_b" + ".bmp").c_str(), 1, 0);
      (*button)->SetBitmap((prefix + "txtbox_btn_c" + ".bmp").c_str(), 2, 0);
      (*button)->SetSize((*button)->BitmapWidth(), (*button)->BitmapHeight());
      (*button)->Move(142, y);
      (*button)->SetId(273);
      this->AddChild(*button);
    }
  }

  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(this->geometry.width - 16, 43, 1, this->m_rowSpacing + 241);
  this->AddChild(this->m_scrollBar);

  {
    const std::string prefix("유저인터페이스\\");
    int x = this->geometry.width - 61;
    int y = this->geometry.height - 23;
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_button = button;
    button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    button->SetBitmap((prefix + "btn_ok_dis" + ".bmp").c_str(), 3, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    if (!GuildData().unknown_00B4 || this->unknown_0180.empty()) {
      x = -200;
      y = -200;
    }
    this->m_button->Move(x, y);
    this->m_button->SetId(184);
    this->AddChild(this->m_button);
    this->m_message0 = 184;
  }

  this->m_onlineCheck = new UICheckBox(*this->m_context, *this->m_controls);
  this->m_onlineCheck->SetSize(240, 12);
  this->m_onlineCheck->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(2864));
  this->m_onlineCheck->FitToText();
  this->m_onlineCheck->Move(12, this->geometry.height - 18);
  this->m_onlineCheck->SetChecked(g_guildOnlineFirst);
  this->AddChild(this->m_onlineCheck);
  this->UpdateScroll();
}

// OFFSET: 0x00956130
// FUNCTION: fn_13036
void UIAgitMessageBox::sub_956130(const char* text, int width, int height) {
  this->SetSize(width, height);
  this->Slot15(width, height);
  this->Slot22(width, height);

  UITextViewer* viewer = new UITextViewer();
  viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned int red = 0;
  unsigned int green = 0;
  unsigned int blue = 0;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  viewer->backgroundColor = 0xFF000000 | (blue << 16) | (green << 8) | red;
  viewer->Invalidate();
  this->AddChild(viewer);
  viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text), width / 6, 0, 0);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {187, 188};

  for (int index = 0; index < 2; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->geometry.width - (button->BitmapWidth() + 5) * (2 - index), height - 24);
    button->SetId(ids[index]);
    this->AddChild(button);
  }
}

// OFFSET: 0x0095D640
// FUNCTION: fn_13070
void UIGuildBanishedMemberWnd::Paint() {
  UIGuildWnd::Paint();
  for (int index = 0; index < 11; ++index) {
    this->m_names[index]->Move(-200, -200);
    this->m_reasons[index]->Move(-200, -200);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(4, 45, this->geometry.width - 22, this->geometry.height - g_guildBanishedRowHeight - 69, RGB(red, green, blue));

  const int cellHeight = g_guildBanishedRowHeight - 1;
  int widths[2];
  widths[0] = g_guildBanishedColumnWidths[0];
  widths[1] = g_guildBanishedColumnWidths[1];
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  for (int row = 0; row < 12; ++row) {
    int x = 0;
    for (int column = 0; column < 2; ++column) {
      const int width = widths[column];
      this->FillRegion(x + 5, row * g_guildBanishedRowHeight + 46, width, cellHeight, RGB(red, green, blue));
      x += width + 1;
    }
  }

  const char* headers[2];
  headers[0] = LookupMessage(407);
  headers[1] = LookupMessage(462);
  auto& controls = *this->m_controls;
  int x = 0;
  for (int column = 0; column < 2; ++column) {
    this->DrawTextAt(controls.fonts, controls.locale, x + 8, 49, headers[column], 0, 0, 12, 0, 0, 0);
    x += widths[column] + 1;
  }

  const auto& members = GuildData().unknown_00EC;
  int drawn = 0;
  int index = 0;
  for (auto member = members.begin(); member != members.end(); ++member, ++index) {
    if (index < this->m_scrollTop * this->m_columns) {
      continue;
    }

    const int y = drawn / this->m_columns * g_guildBanishedRowHeight + 68;
    if (this->m_hoverRow == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(5, y - 3, this->geometry.width - 24, 18, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    this->m_names[drawn]->SetTextFitted(this->StripNamelessTag(member->name).c_str());
    this->m_names[drawn]->Move(8, y);

    char reason[260];
    memset(reason, 0, sizeof(reason));
    strncpy_s(reason, sizeof(reason), member->unknown_0034.c_str(), member->unknown_0034.size());
    if (this->m_forbiddenWords->Contains(reason) == true) {
      std::string masked;
      this->m_forbiddenWords->Mask(reason, &masked);
    }
    this->m_reasons[drawn]->SetTextFitted(reason);
    this->m_reasons[drawn]->Move(g_guildBanishedColumnWidths[0] + 9, y);
    if (++drawn == this->m_visibleRows * this->m_columns) {
      return;
    }
  }
}

// OFFSET: 0x00956950
// FUNCTION: fn_13037
bool UIGuildMemberManageWnd::DrawMember(GuildInfo::Member& member, int& index, int row) {
  if (index < this->m_scrollTop * this->m_columns) {
    ++index;
    return false;
  }

  if (row >= 6 - this->m_rowSpacing * 6 / 36) {
    return false;
  }

  if (g_modeMgr.GetGameMode()) {
    const std::string playerName = LookupPlayerName(member.characterId);
    member.name = playerName;
  }
  const std::string& name = member.name;
  if (member.name.size() == 0) {
    member.name = LookupMessage(581);
    member.name += g_guildNamelessTag;
  }

  if (member.online == 1) {
    this->FillRegion(6, (this->m_rowSpacing + 36) * row + 67, 375, this->m_rowSpacing + 35, 0xE4FFE2);
  }

  if (this->m_hoverRow == index) {
    unsigned red, green, blue;
    SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
    this->FillRegion(6, (this->m_rowSpacing + 36) * row + 67, 375, this->m_rowSpacing + 35, RGB(red, green, blue));
  }

  if (member.online == 1) {
    const std::string headAct = g_session.GetHeadActName(member.job, member.unknown_002C, &member.unknown_0020);
    const std::string headSpr = g_session.GetHeadSprName(member.job, member.unknown_002C, &member.unknown_0020);
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(headAct.c_str()));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(headSpr.c_str()));
    const std::uint32_t* palette = sprite->palette;
    const SpriteFrameCell& cell = action->Frame(0, 0).Cell(1);
    if (cell.image == -1) {
      return false;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    if (member.unknown_0024 != 0) {
      const std::string headPal = g_session.GetHeadPalName(member.unknown_0020, member.job, member.unknown_002C, member.unknown_0024);
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(headPal.c_str()))->palette;
    }

    SpriteAtlasRecord record{};
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    }

    this->geometry.surface->DrawSurface(
      (this->m_rowSpacing - image->width + 36) / 2 + 6, (this->m_rowSpacing + 36) * row + 70, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
    );
  }

  const int y = this->m_rowSpacing / 2 + (this->m_rowSpacing + 36) * row + 79;
  this->m_rowTexts[1][row]->SetTextFittedCodePage(this->StripNamelessTag(name).c_str());
  this->m_rowTexts[1][row]->Move(43, y - this->m_rowSpacing / 2);
  this->m_rowTexts[2][row]->SetTextFittedCodePage(member.positionName.c_str());
  this->m_rowTexts[2][row]->Move(g_guildMemberColumnWidths[0] + 11, y);
  if (this->m_rowButtons[row]) {
    this->m_rowButtons[row]->Move(150, y - 4);
  }

  int x = g_guildMemberColumnWidths[0] + g_guildMemberColumnWidths[1] + 8;
  const std::string job(g_session.GetJobName(member.job, member.unknown_002C));
  this->m_rowTexts[3][row]->SetTextFittedCodePage(job.c_str());
  this->m_rowTexts[3][row]->Move(x + 4, y);
  x += g_guildMemberColumnWidths[2] + 1;
  this->DrawRightAlignedText(g_guildMemberColumnWidths[3] + x - 4, y, FormatOwnedString("%d", member.level).c_str(), 0, 0, 12, 0, 0);
  x += g_guildMemberColumnWidths[3] + 1;
  this->DrawRightAlignedText(g_guildMemberColumnWidths[4] + x - 4, y, FormatOwnedString("%3.0f%%", member.expRatio).c_str(), 0, 0, 12, 0, 0);
  x += g_guildMemberColumnWidths[4] + 1;
  if (member.exp < 0x7FFFFFFF) {
    this->DrawRightAlignedText(g_guildMemberColumnWidths[5] + x - 4, y, FormatOwnedString("%d", member.exp).c_str(), 0, 0, 12, 0, 0);
  } else {
    this->DrawRightAlignedText(g_guildMemberColumnWidths[5] + x - 4, y, FormatOwnedString("%d", member.exp).c_str(), 0, 0, 12, 0xFF, 0);
  }

  char lastLogin[128];
  const __time32_t* lastLoginTime = reinterpret_cast<const __time32_t*>(member.unknown_0074);
  if (*lastLoginTime != 0) {
    strftime(lastLogin, sizeof(lastLogin), LookupMessage(3011), _localtime32(lastLoginTime));
  } else {
    lastLogin[0] = '\0';
  }
  auto& controls = *this->m_controls;
  this->DrawTextAt(controls.fonts, controls.locale, 8, y + 17, FormatOwnedString(LookupMessage(3012), lastLogin).c_str(), 0, 0, 11, 0x404040, 0, 0);
  return true;
}

// OFFSET: 0x0095E320

// OFFSET: 0x00963BF0
// FUNCTION: fn_13130
int UIGuildNoticeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message == 6) {
    if (a1 == 201) {
      this->m_frameContext->windows.CloseWindow(59);
    } else if (a1 == 277) {
      std::string body(this->m_bodyEdit->Text());
      if (this->m_forbiddenWords->Contains(this->m_titleEdit->Text()) == true || this->m_forbiddenWords->Contains(body.c_str()) == true) {
        this->m_controls->services.Confirm(LookupMessage(14), 0, 1, 0, "Message", 280, 120);
      } else {
        if (GuildData().unknown_00B4) {
          g_modeMgr.gameMode->SendMsg(118, g_guildId, reinterpret_cast<int>(this->m_titleEdit->Text()), reinterpret_cast<int>(body.c_str()), 0);
        }
        this->InvalidateChildren();
      }
    }
  } else if (message == 60) {
    this->InvalidateChildren();
  } else {
    return UIGuildWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  return 0;
}

// OFFSET: 0x00964500
// FUNCTION: fn_13132
int UIGuildSkillWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  switch (message) {
  case 6:
    switch (a1) {
    case 201:
      this->m_frameContext->windows.CloseWindow(59);
      return 0;
    case 184:
      if (this->m_hoverIndex != -1) {
        CSkillInfo skill = this->SkillAt(this->m_hoverIndex);
        if (skill.level != 0) {
          this->SendMsg(nullptr, 58, reinterpret_cast<std::intptr_t>(&skill), 0, 0, 0);
        }
        return 0;
      }
      break;
    case 239:
    case 240:
    case 241:
    case 242:
    case 243:
    case 244:
    case 245:
      if (g_session.m_guildSkillPoint != 0) {
        CSkillInfo skill = this->SkillAt(this->m_scrollTop + a1 - 239);
        if (skill.available != 0) {
          g_modeMgr.gameMode->SendMsg(67, skill.id, 0, 0, 0);
        }
        return 0;
      }
      break;
    case 253:
    case 254:
    case 255:
    case 256:
    case 257:
    case 258:
    case 259: {
      CSkillInfo skill = this->SkillAt(this->m_scrollTop + a1 - 253);
      if (skill.available != 0) {
        int level = reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill.id) - 1;
        if (level <= 0) {
          level = 1;
        }
        if (level > skill.level) {
          level = skill.level;
        }
        g_session.m_skillBundle.SetUnknownValue(skill.id, level);
        this->Invalidate();
      }
      return 0;
    }
    case 260:
    case 261:
    case 262:
    case 263:
    case 264:
    case 265:
    case 266: {
      CSkillInfo skill = this->SkillAt(this->m_scrollTop + a1 - 260);
      if (skill.available != 0) {
        int level = reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill.id) + 1;
        if (level <= 0) {
          level = 1;
        }
        if (level > skill.level) {
          level = skill.level;
        }
        g_session.m_skillBundle.SetUnknownValue(skill.id, level);
        this->Invalidate();
      }
      return 0;
    }
    }
    break;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scrollTop;
    int top;
    if (message == 9) {
      top = previous - this->m_visibleRows + 1;
    } else if (message == 10) {
      top = this->m_visibleRows - 1 + previous;
    } else {
      top = a1 + previous;
    }
    this->m_scrollTop = top;
    if (previous != top) {
      this->UpdateScroll();
      this->UpdateSkillButtons();
      this->Invalidate();
    }
    return this->m_scrollTop - previous;
  }
  case 23: {
    this->m_skills.clear();
    const int count = static_cast<int>(g_session.m_additionalSkills.size());
    for (int index = 0; index < count; ++index) {
      CSkillInfo skill = g_session.AdditionalSkillAt(index);
      if (skill.available != 0 && (skill.unknown_0018 != 0 || skill.level != 0)) {
        this->m_skills.push_back(skill);
      }
    }
    this->Invalidate();
    this->UpdateScroll();
    this->UpdateSkillButtons();
    return 0;
  }
  case 48:
    if (this->m_hoverIndex != -1) {
      CSkillInfo skill = this->SkillAt(this->m_hoverIndex);
      if (skill.available != 0) {
        const int command = this->m_menuItems[a1].value;
        if (command == 3) {
          this->m_frameContext->windows.EnsureWindow(46)->SendMsg(nullptr, 61, skill.id, 0, 0, 0);
        } else if (command == 14) {
          this->m_frameContext->windows.EnsureWindow(55)->SendMsg(nullptr, 68, skill.id, 0, 0, 0);
        }
      }
      return 0;
    }
    break;
  case 49:
    this->m_menuItems.clear();
    if (g_windowMgr.unknown_0210 != 0) {
      this->m_frameContext->windows.CloseWindow(18);
    }
    if (this->m_hoverIndex != -1) {
      CSkillInfo skill = this->SkillAt(this->m_hoverIndex);
      if (skill.available != 0) {
        const std::string text = LookupMessage(322);
        this->m_menuItems.push_back(MiniPartyInfoEntry(text.c_str(), 3));
      }
    }
    if (!this->m_menuItems.empty()) {
      UIWindow* menu = this->m_frameContext->windows.EnsureWindow(18);
      for (std::size_t index = 0; index < this->m_menuItems.size(); ++index) {
        menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(this->m_menuItems[index].text.c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, 83, 63, 0, 0, 0);
      menu->SendMsg(nullptr, 30, 0, 0, 0, 0);

      int x = std::max(1, g_mouse.x + 1);
      int maxX = g_renderer->viewport.width - menu->geometry.width;
      if (x > maxX) {
        x = maxX;
      }
      int y = std::max(1, g_mouse.y + 1);
      int maxY = g_renderer->viewport.height - menu->geometry.height;
      if (y > maxY) {
        y = maxY;
      }
      menu->Move(x, y);
      return 0;
    }
    break;
  case 58: {
    const CSkillInfo* skill = reinterpret_cast<const CSkillInfo*>(a1);
    if (skill->level != 0) {
      g_modeMgr.gameMode->SendMsg(113, a1, reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill->id), 0, 0);
    }
    break;
  }
  default:
    return UIGuildWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  return 0;
}

// OFFSET: 0x00953F60
// FUNCTION: fn_13003
UIGuildTipWnd::UIGuildTipWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

// OFFSET: 0x00953FC0
// FUNCTION: fn_13004
UIGuildTotalInfoWnd::UIGuildTotalInfoWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIGuildWnd(windows, frames, controls) {
}

// OFFSET: 0x00954120
// FUNCTION: fn_13006
UIListBoxAgitMenu::UIListBoxAgitMenu(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list) {
}

// OFFSET: 0x0095BF90
// FUNCTION: fn_13064
void UIGuildSkillWnd::Slot15(int, int height) {
  this->CreateTabs();
  if (std::strcmp(g_guildName.c_str(), "") == 0) {
    g_modeMgr.gameMode->SendMsg(107, 0, 0, 0, 0);
  }

  GuildData().unknown_00C8 = 1;
  g_modeMgr.gameMode->SendMsg(107, 3, 0, 0, 0);
  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(this->geometry.width - 15, 41, 1, this->geometry.height - 70);
  this->AddChild(this->m_scrollBar);
  this->m_columns = 1;
  this->m_visibleRows = (height - 62) / 32;

  {
    int ids[7] = {239, 240, 241, 242, 243, 244, 245};
    std::string normal("유저인터페이스\\basic_interface\\skill_up_a.bmp");
    std::string hover("유저인터페이스\\basic_interface\\skill_up_b.bmp");
    std::string pressed("유저인터페이스\\basic_interface\\skill_up_c.bmp");
    UIBitmapButton** button = this->m_levelUpButtons;
    int* id = ids;
    for (int y = 45; y < 283; y += 34, ++button, ++id) {
      *button = new UIBitmapButton(*this->m_context, *this->m_controls);
      (*button)->SetBitmap(normal.c_str(), 0, 0);
      (*button)->SetBitmap(hover.c_str(), 1, 0);
      (*button)->SetBitmap(pressed.c_str(), 2, 0);
      (*button)->SetSize((*button)->BitmapWidth(), (*button)->BitmapHeight());
      (*button)->Move(78, y);
      (*button)->SetId(*id);
      this->AddChild(*button);
    }
  }

  {
    int ids[7] = {253, 254, 255, 256, 257, 258, 259};
    std::string bitmap("유저인터페이스\\basic_interface\\arw_left.bmp");
    UIBitmapButton** button = this->m_previousButtons;
    int* id = ids;
    for (int y = 58; y < 296; y += 34, ++button, ++id) {
      *button = new UIBitmapButton(*this->m_context, *this->m_controls);
      (*button)->SetBitmap(bitmap.c_str(), 0, 0);
      (*button)->SetBitmap(bitmap.c_str(), 1, 0);
      (*button)->SetBitmap(bitmap.c_str(), 2, 0);
      (*button)->SetSize((*button)->BitmapWidth(), (*button)->BitmapHeight());
      (*button)->Move(112, y);
      (*button)->SetId(*id);
      this->AddChild(*button);
    }
  }

  {
    int ids[7] = {260, 261, 262, 263, 264, 265, 266};
    std::string bitmap("유저인터페이스\\basic_interface\\arw_right.bmp");
    UIBitmapButton** button = this->m_nextButtons;
    int* id = ids;
    for (int y = 58; y < 296; y += 34, ++button, ++id) {
      *button = new UIBitmapButton(*this->m_context, *this->m_controls);
      (*button)->SetBitmap(bitmap.c_str(), 0, 0);
      (*button)->SetBitmap(bitmap.c_str(), 1, 0);
      (*button)->SetBitmap(bitmap.c_str(), 2, 0);
      (*button)->SetSize((*button)->BitmapWidth(), (*button)->BitmapHeight());
      (*button)->Move(192, y);
      (*button)->SetId(*id);
      this->AddChild(*button);
    }
  }

  {
    const std::string prefix("유저인터페이스\\");
    const char* names[2][3] = {
        {"btn_use", "btn_use_a", "btn_use_b"},
        {"btn_close", "btn_close_a", "btn_close_b"},
    };
    int ids[2] = {184, 201};
    int positions[2][2] = {
        {46, this->geometry.height - 24},
        {92, this->geometry.height - 24},
    };
    UIBitmapButton** button = this->m_actionButtons;
    for (int index = 0; index < 2; ++index, ++button) {
      *button = new UIBitmapButton(*this->m_context, *this->m_controls);
      (*button)->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      (*button)->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      (*button)->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      (*button)->SetSize((*button)->BitmapWidth(), (*button)->BitmapHeight());
      (*button)->Move(positions[index][0], positions[index][1]);
      (*button)->SetId(ids[index]);
      this->AddChild(*button);
    }

    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00963E10
// FUNCTION: fn_13131
int UIGuildPositionManageWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  switch (message) {
  case 6:
    switch (a1) {
    case 184:
      if (this->m_positionMap.size() != 0) {
        reinterpret_cast<std::map<int, GuildPositionEntry>&>(GuildData().unknown_010C) = this->m_positionMap;
        g_modeMgr.gameMode->SendMsg(104, 0, 0, 0, 0);
        this->m_positionMap.clear();
        this->m_button->Move(-200, -200);
        this->m_button->SetDisabled(1);
        this->m_button->Invalidate();
        this->m_buttonTime = timeGetTime();
        this->InvalidateChildren();
        return 0;
      }
      return 0;
    case 201:
      this->m_frameContext->windows.CloseWindow(59);
      return 0;
    case 213:
      if (this->unknown_00D0[0] && this->unknown_00FC[0] && this->unknown_0128[0]) {
        for (int index = 0; index < 11; ++index) {
          if (this->unknown_00D0[index] == sender) {
            int found;
            GuildPositionEntry& position = this->PositionAt(this->m_scrollTop + index, &found);
            if (found) {
              position.canInvite = a2;
              this->m_positionMap[position.unknown_0004] = position;
            }
          }
          if (this->unknown_00FC[index] == sender) {
            int found;
            GuildPositionEntry& position = this->PositionAt(this->m_scrollTop + index, &found);
            if (found) {
              position.canExpel = a2;
              this->m_positionMap[position.unknown_0004] = position;
            }
          }
          if (this->unknown_0128[index] == sender) {
            int found;
            GuildPositionEntry& position = this->PositionAt(this->m_scrollTop + index, &found);
            if (found) {
              position.canStorage = a2;
              this->m_positionMap[position.unknown_0004] = position;
            }
          }
        }
        break;
      }
      return 0;
    case 275:
      if (this->unknown_0180[0]) {
        int row = a1 - 275;
        for (int index = 0; index < 11; ++index) {
          if (this->unknown_0180[index] == sender) {
            row = index;
            break;
          }
        }

        int found;
        GuildPositionEntry& position = this->PositionAt(this->m_scrollTop + row, &found);
        if (found) {
          position.name = this->unknown_0180[row]->Text();
          this->m_positionMap[position.unknown_0004] = position;
        }
        break;
      }
      return 0;
    case 276:
      if (this->unknown_0154[0]) {
        int row = a1 - 276;
        for (int index = 0; index < 11; ++index) {
          if (this->unknown_0154[index] == sender) {
            row = index;
            break;
          }
        }

        int found;
        GuildPositionEntry& position = this->PositionAt(this->m_scrollTop + row, &found);
        if (found) {
          position.tax = std::atoi(this->unknown_0154[row]->Text());
          if (position.tax < 0 || position.tax > 50) {
            g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(3460), 50, 0).c_str()), 0xFF, 0, 0, 0);
            if (position.tax < 0) {
              position.tax = 0;
            } else if (position.tax > 50) {
              position.tax = 50;
            }
          }
          this->m_positionMap[position.unknown_0004] = position;
        }
        break;
      }
      return 0;
    default:
      return 0;
    }

    if (GuildData().unknown_00B4 && this->m_positionMap.size()) {
      this->m_button->Move(this->geometry.width - 61, this->geometry.height - 23);
    } else {
      this->m_button->Move(-200, -200);
    }
    this->InvalidateChildren();
    return 0;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scrollTop;
    int top;
    if (message == 9) {
      top = previous - this->m_visibleRows + 1;
    } else {
      top = message == 10 ? this->m_visibleRows - 1 : a1;
      top += previous;
    }
    this->m_scrollTop = top;
    if (previous != top) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scrollTop - previous;
  }
  case 60:
    this->m_positions = reinterpret_cast<const std::list<GuildPositionEntry>&>(GuildData().unknown_00D4);
    this->InvalidateChildren();
    if (GuildData().unknown_00B4 && this->m_positionMap.size()) {
      this->m_button->Move(this->geometry.width - 61, this->geometry.height - 23);
    } else {
      this->m_button->Move(-200, -200);
    }
    this->UpdateScroll();
    return 0;
  default:
    return UIGuildWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }
}
