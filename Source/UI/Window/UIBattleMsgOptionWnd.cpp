#include "UI/Window/UIBattleMsgOptionWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>
#include <windows.h>

// OFFSET: 0x009335E0
// FUNCTION: fn_12726
void UIBattleMsgOptionWnd::Slot35(int delta) {
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

// OFFSET: 0x00910C40
// FUNCTION: fn_12510
UIBattleMsgOptionWnd::UIBattleMsgOptionWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->unknown_00B8 = -1;
  this->messages.push_back(LookupMessage(0x50B));

  this->unknown_0108 = -1;
  this->unknown_0110 = 1;
  this->unknown_010C = 0;

  this->messages.push_back(LookupMessage(0x4FD));
  this->messages.push_back(LookupMessage(0x4FE));
  this->messages.push_back(LookupMessage(0x4FF));
  this->messages.push_back(LookupMessage(0x500));
  this->messages.push_back(LookupMessage(0xFBF));
  this->messages.push_back(LookupMessage(0x501));
  this->messages.push_back(LookupMessage(0x502));
  this->messages.push_back(LookupMessage(0x503));
  this->messages.push_back(LookupMessage(0x504));
  this->messages.push_back(LookupMessage(0x505));
  this->messages.push_back(LookupMessage(0x506));
  this->messages.push_back(LookupMessage(0x507));
  this->messages.push_back(LookupMessage(0x508));
  this->messages.push_back(LookupMessage(0x533));
  this->messages.push_back(LookupMessage(0x60D));
  this->messages.push_back(LookupMessage(0x652));
  this->messages.push_back(LookupMessage(0x653));
  this->messages.push_back(LookupMessage(0x654));
  this->messages.push_back(LookupMessage(0x656));
  this->messages.push_back(LookupMessage(0x657));
  this->messages.push_back(LookupMessage(0x93D));
  this->messages.push_back(LookupMessage(0xBA4));
  this->messages.push_back(LookupMessage(0xFBC));
  this->messages.push_back(LookupMessage(0xFBE));

  this->text = LookupMessage(0x509);
}

// OFFSET: 0x0091EC40
// FUNCTION: fn_12636
void UIBattleMsgOptionWnd::Slot15(int, int height) {
  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->m_scrollBar);
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "ESC", 0);
  this->m_allCheckBox = new UICheckBox();
  this->m_allCheckBox->SetSize(70, 12);
  this->m_allCheckBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", "all on");
  this->m_allCheckBox->Move(20, this->geometry.height - 16);
  this->m_allCheckBox->SetChecked(this->unknown_010C);
  this->AddChild(this->m_allCheckBox);
  this->m_visibleRows = (height - 38) / 20;
  this->UpdateScrollBar();
}

// OFFSET: 0x0091BA20
// FUNCTION: fn_12606
int UIBattleMsgOptionWnd::RowAt(int x, int y) {
  if (x >= 0 && y >= 17 && y < this->m_visibleRows * 20 + 17) {
    const int column = x / this->geometry.width;
    int row = 0;
    int index = 0;
    for (std::vector<const char*>::iterator it = this->messages.begin(); it != this->messages.end(); ++it, ++index) {
      if (std::strcmp(LookupMessage(0x60D), *it) != 0 && index >= this->unknown_00C4) {
        if (row == (y - 17) / 20 + column) {
          return index;
        }
        ++row;
      }
    }
  }

  return -1;
}

// OFFSET: 0x009305F0
// FUNCTION: fn_12685
void UIBattleMsgOptionWnd::Slot26(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x00930A80
// FUNCTION: fn_12692
void UIBattleMsgOptionWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
    return;
  }

  const int previous = this->unknown_00B8;
  this->unknown_00B8 = this->RowAt(x, y);
  if (previous != this->unknown_00B8) {
    this->Invalidate();
  }

  if (this->unknown_00B8 != -1) {
    this->unknown_00EC[this->unknown_00B8] = this->unknown_00EC[this->unknown_00B8] == 0;
    (this->unknown_0110 ? g_session.m_noSubChatWnds : g_session.m_subChatWnds)[this->unknown_0108].options[this->unknown_00B8] =
      this->unknown_00EC[this->unknown_00B8];
    this->unknown_010C = 1;
    for (int i = 0; i < 25; ++i) {
      if (this->unknown_00EC[i] == 0) {
        this->unknown_010C = 0;
        break;
      }
    }
    this->m_allCheckBox->geometry.unknown_0030 = this->unknown_010C;
    this->m_allCheckBox->Invalidate();
    this->Invalidate();
  }
}

// OFFSET: 0x009317A0
// FUNCTION: fn_12707
void UIBattleMsgOptionWnd::Slot28(int x, int y) {
  const int previous = this->unknown_00B8;
  this->unknown_00B8 = this->RowAt(x, y);
  if (previous != this->unknown_00B8) {
    this->Invalidate();
  }
}

// OFFSET: 0x00933680
// FUNCTION: fn_12728
void UIBattleMsgOptionWnd::UpdateScrollBar() {
  if (!this->m_scrollBar) {
    return;
  }

  const int total = static_cast<int>(this->messages.size()) - (this->messages.empty() ? 1 : 0);
  const int visible = this->m_visibleRows;
  const int range = total - visible < 0 ? 0 : total - visible;
  this->m_scrollable = range != 0;
  if (range != 0) {
    this->m_scrollBar->SetRange(range, visible, total);
    if (this->unknown_00C4 < 0) {
      this->unknown_00C4 = 0;
    }
    if (this->unknown_00C4 > total - this->m_visibleRows) {
      this->unknown_00C4 = total - this->m_visibleRows;
    }
    this->m_scrollBar->SetPosition(this->unknown_00C4);
  } else {
    this->unknown_00C4 = 0;
    this->m_scrollBar->SetRange(0, visible, total);
  }
  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x0092B9C0
// FUNCTION: fn_12659
void UIBattleMsgOptionWnd::Paint() {
  auto& controls = *this->m_controls;

  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, this->text.c_str(), 0);
  this->DrawPanelBackground(titleHeight, 0, "");

  const int leftHeight = this->geometry.height - 42;
  const int leftCount = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int i = 0, y = 19; i < leftCount; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  const int rightHeight = this->geometry.height - 38;
  const int rightCount = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int i = 0, y = 17; i < rightCount; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(18, 17, 1, this->geometry.height - 21, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);

  int index = 0;
  int row = 1;
  int y = 22;
  for (std::vector<const char*>::iterator it = this->messages.begin(); it != this->messages.end(); ++it, ++index) {
    if (std::strcmp(LookupMessage(0x60D), *it) == 0 || index < this->unknown_00C4) {
      continue;
    }

    if (this->unknown_00B8 == index) {
      unsigned selectRed, selectGreen, selectBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, selectRed, selectGreen, selectBlue);
      this->FillRegion(25, y - 3, this->geometry.width - 40, 20, RGB(selectRed, selectGreen, selectBlue));
      this->FillRegion(26, y - 2, this->geometry.width - 42, 18, 0xFFFFFF);
    }

    this->DrawTextAt(controls.fonts, controls.locale, 50, y, *it, 0, 0, 12, 0, 0, 0);
    const bool online = this->unknown_00EC[index] != 0;
    auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(online ? "유저인터페이스\\basic_interface\\grp_online.bmp" : "유저인터페이스\\basic_interface\\grp_offline.bmp")
    ));
    this->DrawBitmap(27, y, &icon->image, true);

    if (row == this->m_visibleRows) {
      return;
    }
    y += 20;
    ++row;
  }
}

// OFFSET: 0x00934780
// FUNCTION: fn_12742
int UIBattleMsgOptionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    if (a == 193) {
      if (this->unknown_0108 == b && this->unknown_0110 == (c != 0)) {
        if (d != 2) {
          this->m_frameContext->windows.CloseWindow(132);
          return 0;
        }
      } else if (d == 1) {
        return 0;
      }

      this->unknown_0108 = b;
      this->unknown_0110 = c != 0;
      SubChatWndInfo info;
      info = (this->unknown_0110 ? g_session.m_noSubChatWnds : g_session.m_subChatWnds)[this->unknown_0108];
      if (!info.name.empty()) {
        this->unknown_010C = 1;
        for (int i = 0; i < 25; ++i) {
          this->unknown_00EC[i] = info.options[i];
          if (!info.options[i]) {
            this->unknown_010C = 0;
          }
        }
        this->m_allCheckBox->geometry.unknown_0030 = this->unknown_010C;
        this->m_allCheckBox->Invalidate();
        this->text = info.name;
        this->text += LookupMessage(0x6F1);
        this->Invalidate();
      }
      this->UpdateScrollBar();
      return 0;
    }

    if (a == 201) {
      this->m_frameContext->windows.CloseWindow(132);
      return 0;
    }

    if (a == 213) {
      this->unknown_010C = b;
      for (int i = 0; i < 25; ++i) {
        const bool checked = this->unknown_010C != 0;
        this->unknown_00EC[i] = checked;
        (this->unknown_0110 ? g_session.m_noSubChatWnds : g_session.m_subChatWnds)[this->unknown_0108].options[i] = checked;
      }
      this->Invalidate();
      return 0;
    }
    return 0;

  case 7:
  case 9:
  case 10: {
    const int previous = this->unknown_00C4;
    if (message == 9) {
      this->unknown_00C4 = previous - this->m_visibleRows + 1;
    } else if (message == 10) {
      this->unknown_00C4 = previous + this->m_visibleRows - 1;
    } else {
      this->unknown_00C4 = previous + static_cast<int>(a);
    }
    if (previous != this->unknown_00C4) {
      this->UpdateScrollBar();
      this->Invalidate();
    }
    return this->unknown_00C4 - previous;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
