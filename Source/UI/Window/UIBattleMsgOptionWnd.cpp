#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
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

#include "Resource/MSGSTRINGID.h"
#include <cstring>
#include <windows.h>

// OFFSET: 0x009335E0
// FUNCTION: fn_12726
void UIBattleMsgOptionWnd::OnWheel(int delta) {
  if (this->m_scrollable != 0) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
  }
}

// OFFSET: 0x00910C40
// FUNCTION: fn_12510
UIBattleMsgOptionWnd::UIBattleMsgOptionWnd() : UIFrameWnd(0) {
  this->hoveredOption = -1;
  this->messages.push_back(LookupMessage(MSI_ST_CHAT));

  this->chatWindowIndex = -1;
  this->useMainChatOptions = 1;
  this->allChecked = 0;

  this->messages.push_back(LookupMessage(MSI_VIEW_CHAT_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_WHISPER_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_PARTY_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_GUILD_MSG));
  this->messages.push_back(LookupMessage(MSI_REFINING_POSSIBLE));
  this->messages.push_back(LookupMessage(MSI_VIEW_GET_ITEM_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_EQUIP_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_CHANGE_STATUS_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_GET_ITEM_PARTY_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_CHANGE_STATUS_PARTY_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_FAIL_SKILL_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_PARTY_SETUP_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_DAMAGED_EQUIP_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_SIEGE_INFO_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_SEEK_PARTY));
  this->messages.push_back(LookupMessage(MSI_VIEW_COMBAT_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_PARTY_COMBAT_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_GETTING_EXP_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_QUEST_INFO_MSG));
  this->messages.push_back(LookupMessage(MSI_VIEW_BATTLEFIELD_INFO_MSG));
  this->messages.push_back(LookupMessage(MSI_STR_MAKECHARACTER));
  this->messages.push_back(LookupMessage(MSI_RESULT_FAILED_ASSISTANT_POS));
  this->messages.push_back(LookupMessage(MSI_ALLY_CHAT_MSG));
  this->messages.push_back(LookupMessage(MSI_PAY_CONFIRM_BY_NYANGVINE_FRUIT));

  this->text = LookupMessage(MSI_BATTLE_CHAT_WND_OPTION);
}

// OFFSET: 0x0091EC40
// FUNCTION: fn_12636
void UIBattleMsgOptionWnd::OnCreate(int, int height) {
  this->m_scrollBar = new UIScrollBar();
  this->m_scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->m_scrollBar);
  this->AddBaseButton();
  this->AddCloseButton("ESC", 0);
  this->m_allCheckBox = new UICheckBox();
  this->m_allCheckBox->SetSize(70, 12);
  this->m_allCheckBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", " all on");
  this->m_allCheckBox->Move(20, this->geometry.height - 16);
  this->m_allCheckBox->SetChecked(this->allChecked);
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
      if (std::strcmp(LookupMessage(MSI_VIEW_SEEK_PARTY), *it) != 0 && index >= this->scrollPosition) {
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
void UIBattleMsgOptionWnd::OnLBtnDblClk(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x00930A80
// FUNCTION: fn_12692
void UIBattleMsgOptionWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  const int previous = this->hoveredOption;
  this->hoveredOption = this->RowAt(x, y);
  if (previous != this->hoveredOption) {
    this->Invalidate();
  }

  if (this->hoveredOption != -1) {
    this->optionStates[this->hoveredOption] = this->optionStates[this->hoveredOption] == 0;
    (this->useMainChatOptions ? g_session.m_noSubChatWnds : g_session.m_subChatWnds)[this->chatWindowIndex].options[this->hoveredOption] =
      this->optionStates[this->hoveredOption];
    this->allChecked = 1;
    for (int i = 0; i < 25; ++i) {
      if (this->optionStates[i] == 0) {
        this->allChecked = 0;
        break;
      }
    }
    this->m_allCheckBox->geometry.state = this->allChecked;
    this->m_allCheckBox->Invalidate();
    this->Invalidate();
  }
}

// OFFSET: 0x009317A0
// FUNCTION: fn_12707
void UIBattleMsgOptionWnd::OnMouseHover(int x, int y) {
  const int previous = this->hoveredOption;
  this->hoveredOption = this->RowAt(x, y);
  if (previous != this->hoveredOption) {
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
    if (this->scrollPosition < 0) {
      this->scrollPosition = 0;
    }
    if (this->scrollPosition > total - this->m_visibleRows) {
      this->scrollPosition = total - this->m_visibleRows;
    }
    this->m_scrollBar->SetPosition(this->scrollPosition);
  } else {
    this->scrollPosition = 0;
    this->m_scrollBar->SetRange(0, visible, total);
  }
  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x0092B9C0
// FUNCTION: fn_12659
void UIBattleMsgOptionWnd::Paint() {

  const int titleHeight = this->DrawTitleBar(true, this->text.c_str(), 0);
  this->DrawPanelBackground(titleHeight, 0, "");

  const int leftHeight = this->geometry.height - 42;
  const int leftCount = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int i = 0, y = 19; i < leftCount; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  const int rightHeight = this->geometry.height - 38;
  const int rightCount = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int i = 0, y = 17; i < rightCount; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(18, 17, 1, this->geometry.height - 21, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);

  int index = 0;
  int row = 1;
  int y = 22;
  for (std::vector<const char*>::iterator it = this->messages.begin(); it != this->messages.end(); ++it, ++index) {
    if (std::strcmp(LookupMessage(MSI_VIEW_SEEK_PARTY), *it) == 0 || index < this->scrollPosition) {
      continue;
    }

    if (this->hoveredOption == index) {
      unsigned selectRed, selectGreen, selectBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, selectRed, selectGreen, selectBlue);
      this->FillRegion(25, y - 3, this->geometry.width - 40, 20, RGB(selectRed, selectGreen, selectBlue));
      this->FillRegion(26, y - 2, this->geometry.width - 42, 18, 0xFFFFFF);
    }

    this->DrawTextAt(50, y, *it, 0, 0, 12, 0, 0, 0);
    const bool online = this->optionStates[index] != 0;
    auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(online ? "유저인터페이스\\basic_interface\\grp_online.bmp" : "유저인터페이스\\basic_interface\\grp_offline.bmp")
    ));
    this->DrawBitmap(27, y, icon, true);

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
  case UM_COMMAND:
    switch (a) {
    case ID_OPTION: {
      if (this->chatWindowIndex == b && this->useMainChatOptions == (c != 0)) {
        if (d != 2) {
          g_windowMgr.CloseWindow(WID_BATTLEMSGOPTIONWND);
          return 0;
        }
      } else if (d == 1) {
        return 0;
      }

      this->chatWindowIndex = b;
      this->useMainChatOptions = c != 0;
      SubChatWndInfo info;
      info = (this->useMainChatOptions ? g_session.m_noSubChatWnds : g_session.m_subChatWnds)[this->chatWindowIndex];
      if (!info.name.empty()) {
        this->allChecked = 1;
        for (int i = 0; i < 25; ++i) {
          this->optionStates[i] = info.options[i];
          if (!info.options[i]) {
            this->allChecked = 0;
          }
        }
        this->m_allCheckBox->geometry.state = this->allChecked;
        this->m_allCheckBox->Invalidate();
        this->text = info.name;
        this->text += LookupMessage(MSI_SET_CHATINFO);
        this->Invalidate();
      }
      this->UpdateScrollBar();
      return 0;
    }
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_BATTLEMSGOPTIONWND);
      return 0;
    case ID_CHECK:
      this->allChecked = b;
      for (int i = 0; i < 25; ++i) {
        const bool checked = this->allChecked != 0;
        this->optionStates[i] = checked;
        (this->useMainChatOptions ? g_session.m_noSubChatWnds : g_session.m_subChatWnds)[this->chatWindowIndex].options[i] = checked;
      }
      this->Invalidate();
      return 0;
    }
    return 0;

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->scrollPosition;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->scrollPosition = previous - this->m_visibleRows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->scrollPosition = previous + this->m_visibleRows - 1;
    } else {
      this->scrollPosition = previous + static_cast<int>(a);
    }
    if (previous != this->scrollPosition) {
      this->UpdateScrollBar();
      this->Invalidate();
    }
    return this->scrollPosition - previous;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
