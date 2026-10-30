#include "UI/Guild/UIGuildWnd.h"
#include "Contents/GuildMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIButton.h"

#include <windows.h>
#include <timeapi.h>

#include <cwchar>
#include <string>

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
    : UIFrameWnd(windows, frames, 0), m_saved(nullptr), unknown_00B8(0), unknown_00BC(0), unknown_00C0(0), unknown_00C4(0), unknown_00C8(0),
      unknown_00CC(0), m_controls(&controls) {
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

// OFFSET: 0x00957BB0

// OFFSET: 0x00957CA0

// OFFSET: 0x00960230

// OFFSET: 0x00960470

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
