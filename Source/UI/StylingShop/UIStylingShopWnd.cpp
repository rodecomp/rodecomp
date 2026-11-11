#include "Client/Replay/Replay.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/StylingShop/UIStylingShopWnd.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindowMgr.h"
#include <cstring>
#include <mbstring.h>
#include "Resource/MSGSTRINGID.h"

// OFFSET: 0x00829B50
// FUNCTION: fn_10341
void UIStylingshopTabParentWnd::OnCreate(int, int) {
}

// OFFSET: 0x00829B60
// FUNCTION: fn_10342
void UI2ndCostumeWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  for (int y = 230; y < 280; y += 25) {
    CBitmapRes* checkbox = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\checkbox.bmp")));
    if (checkbox) {
      this->DrawBitmap(13, y, checkbox, false);
    }
  }

  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 0, line, false);
  }
}

// OFFSET: 0x00829BE0
// FUNCTION: fn_10343
void UIAccessoryCusWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 5, line, false);
  }
}

// OFFSET: 0x00829C20
// FUNCTION: fn_10344
void UIAccessoryShowCase::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x00829C30
// FUNCTION: fn_10345
void UIBasicStylingWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 5, line, false);
  }

  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\bg_styling_color.bmp")));
  if (background) {
    this->DrawBitmap(71, 233, background, false);
  }
}

// OFFSET: 0x00829CA0
// FUNCTION: fn_10346
void UICostumeChangeWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 0, line, false);
  }

  this->DrawCenteredText(43, 248, g_session.GetJobName(this->m_display->job, -1), 161, 0, 11, 0, 0);
}

// OFFSET: 0x00827670
// FUNCTION: fn_10328
void UIShowInfoWnd::ResetCheckBoxes() {
  for (int i = 0; i < 8; ++i) {
    this->m_checks[i]->SetChecked(0);
    this->m_checks[i]->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
  }

  std::memset(this->m_changed, 0, sizeof(this->m_changed));
  this->costumeChanged = 0;
  for (unsigned int i = 0; i < this->m_list.size(); ++i) {
    this->m_list.clear();
  }
}

// OFFSET: 0x00829F30
// FUNCTION: fn_10348
void UIMyStyleWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(-line->image.width - static_cast<int>(static_cast<float>(this->geometry.width) * -0.5f), 0, line, false);
  }
}

// OFFSET: 0x0082A020
// FUNCTION: fn_10350
void UISlotOfMyStyleWnd::Paint() {
}

// OFFSET: 0x0082A090
// FUNCTION: fn_10352
void UIStylingshopTabParentWnd::Paint() {
}

// OFFSET: 0x0082A0A0
// FUNCTION: fn_10353
void UISlotForSelectWnd::OnLBtnDblClk(int, int) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 121, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0082A0C0
// FUNCTION: fn_10354
void UI2ndCostumeCheckBox::OnLBtnDown(int x, int y) {
  if (this->m_disabled == 0 && this->geometry.state != 1) {
    UICheckBox::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x0082A0E0
// FUNCTION: fn_10355
void UISlotForSelectWnd::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, UM_COMMAND, this->geometry.id, x, y, 0);
  }
}

// OFFSET: 0x0082A110
// FUNCTION: fn_10356
void UIStylingShopButton::OnLBtnUp(int, int) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  this->geometry.state = 2;
  UIWindow* parent = this->parent;
  if (parent) {
    if (g_windowMgr.WindowPolicy(3, parent->geometry.id) != 0) {
      int id = this->geometry.id;
      if (id == 185 && g_modeMgr.gameMode && g_modeMgr.gameMode->cursorType == 2) {
        const char* text = LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT);
        if (g_windowMgr.Confirm(text, 2, true, false, 0, "Message", 280, 120, 0) == 187) {
          g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
        }
      } else {
        if (id == 184 || id == 321) {
          parent->RecordEditTexts();
          id = this->geometry.id;
          parent = this->parent;
        }

        parent->SendMsg(this, UM_COMMAND, id, this->m_argument, 0, 0);
      }
    }
  } else {
    g_modeMgr.gameMode->SendMsg(MM_COMMAND, this->geometry.id, this->m_argument, 0, 0);
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x0082A1C0
// FUNCTION: fn_10357
void UIStylingShopButton::OnMouseHover(int, int) {
  if (!this->m_names[1].empty()) {
    return;
  }

  int screenX = 0;
  int screenY = -20;
  this->ToScreen(screenX, screenY);
  g_modeMgr.gameMode->ShowTooltip("No Color", screenX, screenY, -1, false, true);
}

// OFFSET: 0x0082A210
// FUNCTION: fn_10358
void UIStylingShopButton::OnMouseMove(int x, int y) {
  if (this->m_disabled == 0 && g_windowMgr.Capture() == this && x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height &&
      this->geometry.state != 2) {
    this->geometry.state = 2;
    this->Invalidate();
  }
}

// OFFSET: 0x0082A270
// FUNCTION: fn_10359
void UIStylingShopWnd::OnProcess() {
  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
    int id = it->first->geometry.id;
    if (id == 200) {
      it->second->primaryTabVisible = it->second->geometry.visible == 1;
    } else if (id == 498 || id == 583) {
      it->second->accessoryTabVisible = it->second->geometry.visible == 1;
    }

    if (it->second->geometry.visible != 0) {
      if (it->first->geometry.id == 496) {
        return;
      }

      it->second->m_display->Invalidate();
    }
  }
}

// OFFSET: 0x0082A330
// FUNCTION: fn_10360
void UIAccBitmapButton::OnRBtnUp(int, int) {
  this->parent->SendMsg(this, UM_BMP_RBTN_UP, this->geometry.id, this->m_argument, 0, 0);
}

// OFFSET: 0x0082CCB0
// FUNCTION: fn_10366
int UI2ndCostumeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == ID_CHECK) {
    if (sender == this->m_showCheckBox) {
      if (this->m_display && this->m_hideCheckBox && this->m_showCheckBox->geometry.state != 0) {
        this->m_display->job = 1;
        this->m_hideCheckBox->SetChecked(0);
        this->m_showInfo->Invalidate();
      }
    } else if (sender == this->m_hideCheckBox) {
      if (this->m_display && this->m_showCheckBox && this->m_hideCheckBox->geometry.state != 0) {
        this->m_display->job = 0;
        this->m_showCheckBox->SetChecked(0);
        this->m_showInfo->Invalidate();
      }
    } else {
      this->m_showInfo->Invalidate();
    }
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0082CD70
// FUNCTION: fn_10367
int UIAccessoryCusWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0082D100
// FUNCTION: fn_10369
int UIBasicStylingWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 495) {
    int count = 0;
    g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "HeadPalette", &count);

    bool unselected = true;
    for (int i = 0; i < count; ++i) {
      if (sender == this->m_paletteButtons[i]) {
        unselected = false;
        bool coloring = true;
        if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
          g_session.m_hotkeyScript->Call("StylingShop_GetIsColoring_by_goods", "d>b", this->m_display->head, &coloring);
        }

        if (coloring == true) {
          this->m_display->headPalette = i + 1;
          this->m_display->Invalidate();
        } else {
          g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_BASICSTYLE), 0, false, false, 0, "Message", 280, 120, 0);
        }
      } else {
        this->m_paletteButtons[i]->SetState(0);
      }
    }

    if (!unselected) {
      this->m_defaultPaletteButton->SetState(0);
    } else if (unselected == true && sender == this->m_defaultPaletteButton) {
      this->m_display->headPalette = 0;
      this->m_display->Invalidate();
    }

    this->Invalidate();
    this->m_showInfo->Invalidate();
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0082D3A0
// FUNCTION: fn_10370
int UICostumeChangeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  UIWindow* preview = g_windowMgr.LookupWindow(WID_STYLINGSHOPWND);
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 584: {
    UIDisplayCharacter* display = this->m_display;
    if (display->m_jobs.size() >= 2) {
      std::map<int, int>::iterator it = display->m_jobs.find(display->job);
      if (it == display->m_jobs.end()) {
        --it;
      }
      if (it == display->m_jobs.begin()) {
        it = display->m_jobs.end();
      }
      --it;
      display->job = it->first;
    }
    break;
  }
  case 585: {
    UIDisplayCharacter* display = this->m_display;
    if (display->m_jobs.size() >= 2) {
      std::map<int, int>::iterator it = display->m_jobs.find(display->job);
      if (it != display->m_jobs.end()) {
        ++it;
      }
      if (it == display->m_jobs.end()) {
        it = display->m_jobs.begin();
      }
      display->job = it->first;
    }
    break;
  }
  }

  this->Invalidate();
  if (preview) {
    preview->InvalidateChildren();
  }

  return 0;
}

// OFFSET: 0x0082D980
// FUNCTION: fn_10372
int UIMyStyleWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    if (message != 121) {
      return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }

    UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(WID_STYLINGSHOPWND));
    if (this->m_slots[this->m_selectedSlot]) {
      UIDisplayCharacter* display = this->m_slots[this->m_selectedSlot]->m_display;
      for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = shop->m_tabs.begin(); it != shop->m_tabs.end(); ++it) {
        if (it->first->geometry.id == 496 && it->second->geometry.visible == 1) {
          shop->m_preview->CopyStyle(display);
          shop->SendMsg(nullptr, UM_COMMAND, ID_BASE, 0, 0, 0);
        }
      }
      shop->Invalidate();
    }
  } else {
    switch (argument1) {
    case ID_CLOSE:
      if (g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_REGISTER_BOOKMARK), 2, false, false, 0, "Message", 280, 120, 0) != 188) {
        for (int i = 0; i < 2; ++i) {
          if (sender == this->deleteStyleButtons[i]) {
            this->RemoveChild(this->m_slots[i], 1);
            this->m_slots[i] = nullptr;
            this->deleteStyleButtons[i]->Enable(0);
            this->rotateLeftButtons[i]->Enable(0);
            this->rotateRightButtons[i]->Enable(0);
          }
        }
        this->Invalidate();
      }
      break;
    case ID_SLOT:
      if (this->geometry.visible) {
        this->m_selectedSlot = argument2 / (this->geometry.width / 2);
      }

      for (int i = 0; i < 2; ++i) {
        UISlotOfMyStyleWnd* slot = this->m_slots[i];
        if (slot) {
          UIDisplayCharacter* display = slot->m_display;
          if (i == this->m_selectedSlot) {
            if (display) {
              display->shadowBitmapName = "유저인터페이스\\styleshop\\shadow_chr_on.bmp";
              display->Invalidate();
            }
          } else if (display) {
            display->shadowBitmapName = "유저인터페이스\\styleshop\\shadow_chr_off.bmp";
            display->Invalidate();
          }
          this->m_slots[i]->InvalidateChildren();
        }
      }
      this->Invalidate();
      break;
    case ID_RIGHT:
    case ID_LEFT:
      for (int i = 0; i < 2; ++i) {
        if (sender == this->rotateLeftButtons[i] || sender == this->rotateRightButtons[i]) {
          this->m_slots[i]->m_display->SendMsg(nullptr, UM_COMMAND, argument1, 0, 0, 0);
        }
      }
      break;
    }
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0082DC60
// FUNCTION: fn_10373
int UIShowInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == ID_BUTTON1) {
    this->Invalidate();
  }

  return 0;
}

// OFFSET: 0x0082DC90
// FUNCTION: fn_10374
int UISlotOfMyStyleWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0082E010
// FUNCTION: fn_10376
int UIStylingshopTabParentWnd::SendMsg(UIWindow*, int, std::intptr_t, std::intptr_t, std::intptr_t, std::intptr_t) {
  return 0;
}

// OFFSET: 0x00827610
// FUNCTION: fn_10327
void UIDisplayCharacter::LoadSessionStyle() {
  this->robe = g_session.m_robe;
  this->head = g_session.m_head;
  this->job = g_session.m_bodyCostume;
  this->headPalette = g_session.m_headPalette;
  this->bodyStyle = g_session.m_bodyStyle;
  this->topAccessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[4]);
  this->middleAccessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[8]);
  this->bottomAccessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0]);
}

// OFFSET: 0x00822C60
// FUNCTION: fn_10286
UI2ndCostumeWnd::UI2ndCostumeWnd() : UIStylingshopTabParentWnd(), m_hideCheckBox(nullptr), m_showCheckBox(nullptr) {
}

// OFFSET: 0x00822E00
// FUNCTION: fn_10287
UIBasicStylingWnd::UIBasicStylingWnd() : UIStylingshopTabParentWnd() {
}

// OFFSET: 0x00822EA0
// FUNCTION: fn_10288
UICostumeChangeWnd::UICostumeChangeWnd() : UIStylingshopTabParentWnd() {
}

// OFFSET: 0x00822F20
// FUNCTION: fn_10289
UIDisplayCharacter::UIDisplayCharacter()
    : UIWindow(0), slotNumber(0), direction(0), robe(0), head(0), job(0), headPalette(0), bodyStyle(0), topAccessory(0), middleAccessory(0),
      bottomAccessory(0), hideFavoriteButton(false), topAccessoryItemId(0), middleAccessoryItemId(0), bottomAccessoryItemId(0) {
  this->shadowX = 0;
  this->shadowY = 0;
  this->shadowBitmapName = "유저인터페이스\\styleshop\\shadow_chr_off.bmp";
}

// INLINED
inline UIDisplayCharacter::UIDisplayCharacter(bool hideFavoriteButton)
    : UIWindow(0), slotNumber(0), direction(0), robe(0), head(0), job(0), headPalette(0), bodyStyle(0), topAccessory(0), middleAccessory(0),
      bottomAccessory(0), hideFavoriteButton(hideFavoriteButton), topAccessoryItemId(0), middleAccessoryItemId(0), bottomAccessoryItemId(0) {
  this->shadowX = 0;
  this->shadowY = 0;
  this->shadowBitmapName = "유저인터페이스\\styleshop\\shadow_chr_off.bmp";
}

// OFFSET: 0x00823CE0
// FUNCTION: fn_10298
void UIDisplayCharacter::CopyStyle(const UIDisplayCharacter* other) {
  this->slotNumber = other->slotNumber;
  this->direction = other->direction;
  this->robe = other->robe;
  this->head = other->head;
  this->job = other->job;
  this->headPalette = other->headPalette;
  this->bodyStyle = other->bodyStyle;
  this->topAccessory = other->topAccessory;
  this->middleAccessory = other->middleAccessory;
  this->bottomAccessory = other->bottomAccessory;
  this->topAccessoryItemId = other->topAccessoryItemId;
  this->middleAccessoryItemId = other->middleAccessoryItemId;
  this->bottomAccessoryItemId = other->bottomAccessoryItemId;
  this->Invalidate();
}

// OFFSET: 0x00824760
// FUNCTION: fn_10315
void UIStylingShopWnd::RefreshFavoriteButton() {
  UIStylingshopTabParentWnd* visibleTab = nullptr;
  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
    if (it->second->geometry.visible == 1) {
      if (it->first->geometry.id == 496) {
        return;
      }

      visibleTab = it->second;
    }
  }

  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
    if (it->first->geometry.id == 496) {
      UIMyStyleWnd* myStyle = static_cast<UIMyStyleWnd*>(it->second);
      UIDisplayCharacter* display = visibleTab->m_display;
      bool favorite = false;
      for (int i = 0; i < 2; ++i) {
        UISlotOfMyStyleWnd* slot = myStyle->m_slots[i];
        if (slot) {
          UIDisplayCharacter* saved = slot->m_display;
          if (saved->head == display->head && saved->headPalette == display->headPalette && saved->bodyStyle == display->bodyStyle &&
              saved->topAccessory == display->topAccessory && saved->middleAccessory == display->middleAccessory && saved->bottomAccessory == display->bottomAccessory &&
              saved->job == display->job) {
            favorite = true;
            break;
          }
        }
      }

      visibleTab->UpdateFavoriteButton(favorite);
    }
  }
}

// OFFSET: 0x00824900
// FUNCTION: fn_10316
__declspec(noinline) void UIStylingshopTabParentWnd::UpdateFavoriteButton(const bool& favorite) {
  UIDisplayCharacter* display = this->m_display;
  if (!display) {
    return;
  }

  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(WID_STYLINGSHOPWND));
  if (!shop) {
    return;
  }

  bool full = false;
  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = shop->m_tabs.begin(); it != shop->m_tabs.end(); ++it) {
    if (it->first->geometry.id == 496) {
      UIMyStyleWnd* myStyle = static_cast<UIMyStyleWnd*>(it->second);
      full = true;
      for (int i = 0; i < 2; ++i) {
        if (!myStyle->m_slots[i]) {
          full = false;
          break;
        }
      }
      break;
    }
  }

  if (!full) {
    display->m_favoriteButton->SetTooltip("");
  }

  display->m_favoriteButton->m_argument = 0;
  display->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_off.bmp", 0, 0);
  display->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_off.bmp", 1, 0);
  display->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_off.bmp", 2, 0);
  if (favorite == true) {
    display->m_favoriteButton->m_argument = 1;
    display->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_on.bmp", 0, 0);
    display->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_on.bmp", 1, 0);
    display->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_on.bmp", 2, 0);
  }

  display->m_favoriteButton->Invalidate();
}

// OFFSET: 0x00824BF0
// FUNCTION: fn_10317
bool UIStylingShopWnd::AddToMyStyle() {
  UIDisplayCharacter* display = nullptr;
  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
    if (it->second->geometry.visible == 1) {
      display = it->second->m_display;
    }
  }

  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
    if (it->first->geometry.id == 496) {
      UIMyStyleWnd* myStyle = static_cast<UIMyStyleWnd*>(it->second);
      int index = 0;
      for (; index < 2; ++index) {
        if (myStyle->m_slots[index] == nullptr) {
          break;
        }
      }
      if (index >= 2) {
        return false;
      }

      UISlotOfMyStyleWnd* slot = new UISlotOfMyStyleWnd();
      slot->SetSize(static_cast<int>(myStyle->geometry.width * 0.5 - 3.0), myStyle->geometry.height + 25);
      slot->Move(static_cast<int>(myStyle->geometry.width * 0.5 * index), 0);
      display->slotNumber = index;
      slot->m_display->CopyStyle(display);
      myStyle->AddChildFront(slot);
      myStyle->m_slots[index] = slot;
      myStyle->deleteStyleButtons[index]->Enable(1);
      myStyle->rotateLeftButtons[index]->Enable(1);
      myStyle->rotateRightButtons[index]->Enable(1);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00825790
// FUNCTION: fn_10319
bool UIStylingshopTabParentWnd::IsBuyable() const {
  return this->m_showInfo->m_buyable;
}

// OFFSET: 0x00827750
// FUNCTION: fn_10329
void UIStylingShopWnd::SetBuyEnabled(bool enabled) {
  const char* normal;
  const char* hover;
  const char* pressed;
  if (enabled) {
    this->m_buyButton->SetArgument(0);
    normal = "유저인터페이스\\styleshop\\btn_buy_out.bmp";
    hover = "유저인터페이스\\styleshop\\btn_buy_over.bmp";
    pressed = "유저인터페이스\\styleshop\\btn_buy_down.bmp";
  } else {
    this->m_buyButton->SetArgument(1);
    pressed = "유저인터페이스\\styleshop\\btn_buy_disable.bmp";
    hover = pressed;
    normal = pressed;
  }

  this->m_buyButton->SetBitmap(normal, 0, 0);
  this->m_buyButton->SetBitmap(hover, 1, 0);
  this->m_buyButton->SetBitmap(pressed, 2, 0);
  this->m_buyButton->Invalidate();
}

// OFFSET: 0x0082A030
// FUNCTION: fn_10351
void UIStylingShopWnd::Paint() {
  this->RefreshFavoriteButton();
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(true, LookupMessage(MSI_STYLING_SHOP_DELETE_CHAR), 0);
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\bg_styling.bmp")));
  if (background) {
    this->DrawBitmap(0, 0, background, false);
  }
}

// OFFSET: 0x00825910
// FUNCTION: fn_10320
void UIShowInfoWnd::CheckColoringGoods(bool* coloring) {
  if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
    g_session.m_hotkeyScript->Call("StylingShop_GetIsColoring_by_goods", "d>b", this->m_display->head, coloring);
  }

  const bool* flags = this->m_changed;
  const UIDisplayCharacter* display = this->m_display;
  if (*coloring) {
    if (flags[0] && display->headPalette == g_session.m_headPalette) {
    } else if (flags[1] && display->head == g_session.m_head) {
    } else if (this->costumeChanged == 1 && display->job == g_session.m_bodyCostume) {
    } else if (flags[2] && display->bodyStyle == g_session.m_bodyStyle) {
    } else if (flags[3] && (display->topAccessory == *reinterpret_cast<const int*>(&g_session.accessoryAppearance[4]) || display->topAccessory == 0)) {
    } else if (flags[4] && (display->middleAccessory == *reinterpret_cast<const int*>(&g_session.accessoryAppearance[8]) || display->middleAccessory == 0)) {
    } else if (flags[5] && (display->bottomAccessory == *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0]) || display->middleAccessory == 0)) {
    } else {
      return;
    }
  }

  for (unsigned int i = 0; i < this->m_list.size(); ++i) {
    this->m_checks[i]->SetTooltip("");
    this->m_checks[i]->SetChecked(0);
  }
  this->m_list.clear();
  std::memset(this->m_changed, 0, sizeof(this->m_changed));
  this->costumeChanged = 0;
}

// OFFSET: 0x00826E50
// FUNCTION: fn_10325
void UIDisplayCharacter::LoadCostumes() {
  const int job = g_session.m_job;
  if (g_session.m_bodyCostume == 0 || g_session.m_bodyCostume == 1) {
    g_session.m_bodyCostume = job;
  }

  this->job = g_session.m_bodyCostume;
  int count = 0;
  if (g_session.m_hotkeyScript->Call("StylingShop_GetCostumeSize", "d>d", job, &count) == true) {
    int value = 0;
    int key = 0;
    for (int i = 0; i < count; ++i) {
      if (g_session.m_hotkeyScript->Call("StylingShop_GetChangeCostume", "dd>dd", job, value, &key, &value) == true) {
        this->m_jobs.emplace(key, value);
      }
    }
  }
}

#include "Contents/Items/ItemDescription.h"

// OFFSET: 0x0082E0B0
// FUNCTION: fn_10377
void UIShowInfoWnd::ArrangeCheckBoxes(const int& layout) {
  switch (layout) {
  case 496:
    this->m_checkTop = 1;
    this->paymentFontSize = 11;
    this->paymentTextX = 24;
    this->paymentTextYOffset = 0;
    this->m_offBitmap = "유저인터페이스\\styleshop\\checkbox_mini_off.bmp";
    for (int row = static_cast<int>(this->m_list.size()); row < 4; ++row) {
      for (int column = static_cast<int>(this->m_list.size()); column < 2; ++column) {
        this->m_checks[row * 2 + column]->SetBitmaps("유저인터페이스\\styleshop\\checkbox_mini_on.bmp", "유저인터페이스\\styleshop\\checkbox_mini_non.bmp");
        this->m_checks[row * 2 + column]->Move(column * 105 + 7, row > 0 ? this->geometry.height / 8 * row + this->m_checkTop : this->m_checkTop);
      }
    }
    break;
  case 200:
    this->m_checkTop = 6;
    this->paymentFontSize = 12;
    this->showPaymentItems = 1;
    this->showDottedLines = 1;
    this->showPayments = 1;
    this->paymentIconX = 41;
    this->paymentIconYOffset = -2;
    this->paymentTextX = 68;
    this->paymentTextYOffset = 6;
    this->m_offBitmap = "유저인터페이스\\styleshop\\checkbox_off.bmp";
    for (int i = static_cast<int>(this->m_list.size()); i < 8; ++i) {
      this->m_checks[i]->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
      this->m_checks[i]->Move(20, this->geometry.height / 8 * i + this->m_checkTop);
    }
    break;
  case 497:
    this->m_checkTop = 1;
    this->paymentFontSize = 11;
    this->paymentTextX = 24;
    this->paymentTextYOffset = 0;
    this->m_offBitmap = "유저인터페이스\\styleshop\\checkbox_mini_off.bmp";
    for (int row = static_cast<int>(this->m_list.size()); row < 4; ++row) {
      for (int column = static_cast<int>(this->m_list.size()); column < 2; ++column) {
        this->m_checks[row * 2 + column]->SetBitmaps("유저인터페이스\\styleshop\\checkbox_mini_on.bmp", "유저인터페이스\\styleshop\\checkbox_mini_non.bmp");
        this->m_checks[row * 2 + column]->Move(column * 105 + 7, row > 0 ? this->geometry.height / 8 * row + this->m_checkTop : this->m_checkTop);
      }
    }
    break;
  case 498:
  case 583:
    this->m_checkTop = 6;
    this->paymentFontSize = 12;
    this->showPaymentItems = 1;
    this->showDottedLines = 1;
    this->showPayments = 1;
    this->paymentIconX = 41;
    this->paymentIconYOffset = -2;
    this->paymentTextX = 68;
    this->paymentTextYOffset = 6;
    this->m_offBitmap = "유저인터페이스\\styleshop\\checkbox_off.bmp";
    for (int i = static_cast<int>(this->m_list.size()); i < 8; ++i) {
      this->m_checks[i]->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
      this->m_checks[i]->Move(20, this->geometry.height / 8 * i + this->m_checkTop);
    }
    break;
  }
}

// OFFSET: 0x0082E500
// FUNCTION: fn_10378
__declspec(noinline) void ToggleStylingAccessory(int itemId, int slot) {
  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(WID_STYLINGSHOPWND));
  UIStylingshopTabParentWnd* tab = nullptr;
  for (auto it = shop->m_tabs.begin(); it != shop->m_tabs.end(); ++it) {
    if (it->first->geometry.id == 497) {
      tab = it->second;
      break;
    }
  }

  ITEM_DESC desc = FindItemDescription(itemId);
  UIDisplayCharacter* display;
  switch (slot) {
  case 0:
    display = tab->m_display;
    if (desc.viewId != display->topAccessory) {
      display->topAccessoryItemId = itemId;
      display->topAccessory = desc.viewId;
    } else {
      display->topAccessoryItemId = 0;
      display->topAccessory = 0;
    }
    break;
  case 1:
    display = tab->m_display;
    if (desc.viewId != display->middleAccessory) {
      display->middleAccessoryItemId = itemId;
      display->middleAccessory = desc.viewId;
    } else {
      display->middleAccessoryItemId = 0;
      display->middleAccessory = 0;
    }
    break;
  case 2:
    display = tab->m_display;
    if (desc.viewId != display->bottomAccessory) {
      display->bottomAccessoryItemId = itemId;
      display->bottomAccessory = desc.viewId;
    } else {
      display->bottomAccessoryItemId = 0;
      display->bottomAccessory = 0;
    }
    break;
  }

  tab->InvalidateChildren();
}

// OFFSET: 0x0082E830
// FUNCTION: fn_10379
void UIDisplayCharacter::SelectNextStyle(const char* table, int* goods) {
  int size = 0;
  g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", table, &size);
  if (size == 0) {
    return;
  }

  if (!g_session.IsThirdJobOrHigher(g_session.GetDisplayJob()) && (std::string(table) == "BodyPalette" || std::string(table) == "DoramBodyPalette")) {
    size = 3;
  }

  if (g_session.GetDisplayJob() > JT_2004_JOB_BEGIN && g_session.GetDisplayJob() <= JT_THIEF_H && *goods == 1 && g_session.m_bodyStyle == 1 &&
    std::strcmp(table, "BodyPalette") == 0) {
    *goods = 0;
  }

  int sequence = 0;
  g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", table, *goods, &sequence);
  ++sequence;
  if (sequence > size) {
    sequence = 1;
  }
  g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", table, sequence, goods);

  if (g_session.GetDisplayJob() > JT_2004_JOB_BEGIN && g_session.GetDisplayJob() <= JT_THIEF_H && *goods == 0 && g_session.m_bodyStyle == 1) {
    *goods = 1;
  }
}

// OFFSET: 0x0082ED80
// FUNCTION: fn_10380
void UIDisplayCharacter::SelectPreviousStyle(const char* table, int* goods) {
  int size = 0;
  g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", table, &size);
  if (size == 0) {
    return;
  }

  if (!g_session.IsThirdJobOrHigher(g_session.GetDisplayJob()) && (std::string(table) == "BodyPalette" || std::string(table) == "DoramBodyPalette")) {
    size = 3;
  }

  if (g_session.GetDisplayJob() > JT_2004_JOB_BEGIN && g_session.GetDisplayJob() <= JT_THIEF_H && *goods == 1 && g_session.m_bodyStyle == 1) {
    *goods = 0;
  }

  int sequence = 0;
  g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", table, *goods, &sequence);
  --sequence;
  if (sequence < 1) {
    sequence = size;
  }
  if (sequence <= 0) {
    sequence = 1;
  }
  g_session.m_hotkeyScript->Call("StylingShop_GetGoods", "sd>d", table, sequence, goods);

  if (g_session.GetDisplayJob() > JT_2004_JOB_BEGIN && g_session.GetDisplayJob() <= JT_THIEF_H && *goods == 0 && g_session.m_bodyStyle == 1) {
    *goods = 1;
  }
}

// OFFSET: 0x008264A0
// FUNCTION: fn_10323
std::string UIDisplayCharacter::GetStyleTable(const int& type) {
  std::string table;
  switch (type) {
  case 1:
    table = "HeadPalette";
    break;
  case 2:
    table = "HeadStyle";
    break;
  case 3:
    table = "BodyPalette";
    break;
  case 7:
    table = "BodyStyle";
    break;
  case 4:
    table = "TopAccessory";
    break;
  case 5:
    table = "MidAccessory";
    break;
  case 6:
    table = "BottomAccessory";
    break;
  }

  switch (type) {
  case 1:
  case 4:
  case 5:
  case 6:
    return table;
  default:
    if (!g_session.IsDoramJob(g_session.GetDisplayJob())) {
      return table;
    }
    table = "Doram" + table;
    return table;
  }
}

// OFFSET: 0x00827900
// FUNCTION: fn_10330
bool UIShowInfoWnd::CheckPayment(const int& index, const int& zeny, const int& itemId, const int& costumeItemId) {
  if (zeny) {
    UICheckBox* check = this->m_checks[index];
    if (g_session.m_zeny >= zeny) {
      check->SetTooltip("");
      return true;
    }

    check->SetTooltip(LookupMessage(g_session.m_zeny + g_session.m_bankBalance < zeny ? 2780 : 2820));
    return false;
  }

  if (itemId) {
    if (std::atoi(LookupClientItem(g_session.m_inventoryItems, itemId, 0).itemId.c_str()) != itemId) {
      return false;
    }
  } else if (costumeItemId) {
    if (std::atoi(LookupClientItem(g_session.m_inventoryItems, costumeItemId, 0).itemId.c_str()) != costumeItemId) {
      return false;
    }
    this->m_checks[index]->SetBitmaps("유저인터페이스\\styleshop\\check_disable.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
  }

  return true;
}

// OFFSET: 0x0082B680
// FUNCTION: fn_10363
void UIShowInfoWnd::GetPayment(const int& type, int* zeny, int* itemId, int* costumeItemId) {
  switch (type) {
  case 1:
    g_session.m_hotkeyScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->headPalette, zeny, itemId, costumeItemId);
    break;
  case 2:
    g_session.m_hotkeyScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->head, zeny, itemId, costumeItemId);
    break;
  case 3:
    g_session.m_hotkeyScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->bodyStyle, zeny, itemId, costumeItemId);
    break;
  case 4:
    g_session.m_hotkeyScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->topAccessoryItemId, zeny, itemId, costumeItemId);
    break;
  case 5:
    g_session.m_hotkeyScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->middleAccessoryItemId, zeny, itemId, costumeItemId);
    break;
  case 6:
    g_session.m_hotkeyScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->bottomAccessoryItemId, zeny, itemId, costumeItemId);
    break;
  case 7:
    g_session.m_hotkeyScript->Call("StylingShop_GetPaymentChangeCostume", "dd>ddd", g_session.m_job, this->m_display->job, zeny, itemId, costumeItemId);
    break;
  }
}

// OFFSET: 0x0082C890
// FUNCTION: fn_10365
void UIShowInfoWnd::UpdateChangedStyles() {
  bool coloring = true;
  this->CheckColoringGoods(&coloring);
  if (!this->m_changed[0] && this->m_display->headPalette != g_session.m_headPalette && coloring == true) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(1);
    this->m_changed[0] = 1;
  }

  if (!this->m_changed[1] && this->m_display->head != g_session.m_head) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(2);
    this->m_changed[1] = 1;
  }

  if (!this->m_changed[2] && this->m_display->bodyStyle != g_session.m_bodyStyle) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(3);
    this->m_changed[2] = 1;
  }

  int sequence = 0;
  g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", "TopAccessory", this->m_display->topAccessoryItemId, &sequence);
  if (!this->m_changed[3] && this->m_display->topAccessory != *reinterpret_cast<const int*>(&g_session.accessoryAppearance[4]) && this->m_display->topAccessory != 0 &&
      sequence != 0) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(4);
    this->m_changed[3] = 1;
  }

  g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", "MidAccessory", this->m_display->middleAccessoryItemId, &sequence);
  if (!this->m_changed[4] && this->m_display->middleAccessory != *reinterpret_cast<const int*>(&g_session.accessoryAppearance[8]) && this->m_display->middleAccessory != 0 &&
      sequence != 0) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(5);
    this->m_changed[4] = 1;
  }

  g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", "BottomAccessory", this->m_display->bottomAccessoryItemId, &sequence);
  if (!this->m_changed[5] && this->m_display->bottomAccessory != *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0]) && this->m_display->bottomAccessory != 0 &&
      sequence != 0) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(6);
    this->m_changed[5] = 1;
  }

  if (!this->costumeChanged && this->m_display->job != g_session.m_bodyCostume) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(7);
    this->costumeChanged = 1;
  }
}

// OFFSET: 0x0082D550
// FUNCTION: fn_10371
int UIDisplayCharacter::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(WID_STYLINGSHOPWND));
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_LEFT:
    this->direction = this->direction < 7 ? this->direction + 1 : 0;
    break;
  case ID_RIGHT:
    if (this->direction <= 0) {
      this->direction = 7;
    } else {
      --this->direction;
    }
    break;
  case ID_PREVIEW:
    this->SelectPreviousStyle(this->GetStyleTable(3).c_str(), &this->bodyStyle);
    break;
  case ID_CANCEL:
    this->LoadSessionStyle();
    break;
  case ID_NEXTVIEW:
    this->SelectNextStyle(this->GetStyleTable(3).c_str(), &this->bodyStyle);
    break;
  case 450:
    this->SelectPreviousStyle(this->GetStyleTable(2).c_str(), &this->head);
    break;
  case 451:
    this->SelectNextStyle(this->GetStyleTable(2).c_str(), &this->head);
    break;
  case 495:
    shop->SendMsg(nullptr, 495, 0, 0, 0, 0);
    break;
  case ID_BOOKMARK:
    if (this->m_favoriteButton->m_argument == 0) {
      if (shop->AddToMyStyle() == true) {
        g_windowMgr.Confirm(LookupMessage(MSI_PRIVATE_AIRPLANE_FAIL_LOCALIZING), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        this->m_favoriteButton->SetTooltip(LookupMessage(MSI_STYLING_SHOP_SUGGEST_TO_GO_TO_BANK));
      }
    }
    break;
  }

  this->Invalidate();
  shop->InvalidateChildren();
  return 0;
}


// OFFSET: 0x0082A350
// FUNCTION: fn_10361
void UIMyStyleWnd::SaveLikeList() {
  char formatted[512];
  char slotText[512];
  char text[512];
  std::string likeList = "";
  CFile file;
  if (file.Open("savedata\\LikeListInfoFile.lua", 0, false, false)) {
    int size = static_cast<int>(file.GetSize());
    char* data = new char[size + 1];
    file.Read(data, size);
    data[size] = '\0';
    file.Close();

    struct LikeStyle {
      int slotNum;
      int padding_0004;
      int robe;
      int headStyle;
      int bodyStyle;
      int headPaletteNum;
      int bodyPaletteNum;
      int topAccNum;
      int midAccNum;
      int bottomAccNum;
    };
    LikeStyle style = {};
    char characterName[24] = {};
    char* line = data;
    for (int i = 0; i < size; ++i) {
      switch (data[i]) {
      case '*':
        if (_mbsstr(reinterpret_cast<const unsigned char*>(characterName), reinterpret_cast<const unsigned char*>(g_session.DecodePlayerName())) != nullptr || style.slotNum > 2) {
          break;
        }
        {
          memset(text, 0, sizeof(text));
          sprintf_s(text, "characterName:%s\nslotNum:%d\nheadPaletteNum:%d\nheadStyle:%d\nbodyStyle:%d\nbodyPaletteNum:%d\ntopAccNum:%d\nmidAccNum:%d\nbottomAccNum:%d\nRobe:%d\n*\n", characterName, style.slotNum, style.headPaletteNum, style.headStyle, style.bodyStyle, style.bodyPaletteNum, style.topAccNum, style.midAccNum, style.bottomAccNum, style.robe);
          likeList += text;
          SecureZeroMemory(&style, sizeof(style));
          style.slotNum = 3;
        }
      case '\n':
      case '\r':
        line = &data[i + 1];
        break;
      case ':':
        data[i] = '\0';
        if (_mbsstr(reinterpret_cast<const unsigned char*>("characterName"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          ++i;
          char value[24] = {};
          for (char* out = value; data[i] != '\n' && data[i] != '\r'; ++i, ++out) {
            *out += data[i];
          }
          strcpy_s(characterName, sizeof(characterName), value);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("slotNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.slotNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("headPaletteNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.headPaletteNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("headStyle"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.headStyle = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("bodyPaletteNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.bodyPaletteNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("topAccNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.topAccNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("midAccNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.midAccNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("bottomAccNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.bottomAccNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("Robe"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.robe = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("bodyStyle"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.bodyStyle = atoi(&data[++i]);
        }
        break;
      }
    }
    if (data != nullptr) {
      delete[] data;
    }
  }

  for (int i = 0; i < 2; ++i) {
    UISlotOfMyStyleWnd* slot = this->m_slots[i];
    if (slot != nullptr) {
      memset(slotText, 0, sizeof(slotText));
      memset(formatted, 0, sizeof(formatted));
      UIDisplayCharacter* display = slot->m_display;
      sprintf_s(formatted, "characterName:%s\nslotNum:%d\nheadPaletteNum:%d\nheadStyle:%d\nbodyStyle:%d\nbodyPaletteNum:%d\ntopAccNum:%d\nmidAccNum:%d\nbottomAccNum:%d\nRobe:%d\n*\n", g_session.DecodePlayerName(), display->slotNumber, display->headPalette, display->head, display->job, display->bodyStyle, display->topAccessory, display->middleAccessory, display->bottomAccessory, display->robe);
      strcpy_s(slotText, sizeof(slotText), formatted);
      likeList += slotText;
    }
  }

  FILE* fp = fopen("savedata\\LikeListInfoFile.lua", "w");
  if (fp != nullptr) {
    fprintf(fp, likeList.c_str());
    fclose(fp);
  }
}

// OFFSET: 0x008261E0
// FUNCTION: fn_10322
void UIShowInfoWnd::DrawPaymentItem(const int& type, const int& zeny, const int& itemId, bool enabled, const int& row) {
  if (!this->showPaymentItems) {
    return;
  }

  CBitmapRes* bitmap = nullptr;
  int id = itemId;
  if (zeny != 0) {
    char path[128];
    memset(path, 0, sizeof(path));
    if (type == 4 || type == 5 || type == 6) {
      if (type == 4) {
        id = this->m_display->topAccessoryItemId;
      } else if (type == 5) {
        id = this->m_display->middleAccessoryItemId;
      } else {
        id = this->m_display->bottomAccessoryItemId;
      }

      char number[32] = {};
      std::sprintf(number, "%d", id);
      g_session.IconPath(path, number, 1);
    } else {
      strcpy_s(path, sizeof(path), "유저인터페이스\\styleshop\\icon_zeny.bmp");
    }

    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  } else if (id != 0) {
    if (type == 4) {
      id = this->m_display->topAccessoryItemId;
    } else if (type == 5) {
      id = this->m_display->middleAccessoryItemId;
    } else if (type == 6) {
      id = this->m_display->bottomAccessoryItemId;
    }

    char path[128];
    memset(path, 0, sizeof(path));
    char number[32] = {};
    std::sprintf(number, "%d", id);
    g_session.IconPath(path, number, 1);
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  }

  if (!enabled) {
    this->DrawAuxiliary(1, this->paymentIconX, this->geometry.height / 8 * row + this->paymentIconYOffset, reinterpret_cast<BitmapImage*>(bitmap), true, false);
    return;
  }

  if (bitmap) {
    this->DrawBitmap(this->paymentIconX, this->geometry.height / 8 * row + this->paymentIconYOffset, bitmap, false);
  }
}

// OFFSET: 0x00823AA0
// FUNCTION: fn_10297
UIShowInfoWnd::~UIShowInfoWnd() {
}

// OFFSET: 0x00823180
// FUNCTION: fn_10290
UIMyStyleWnd::UIMyStyleWnd() : UIStylingshopTabParentWnd(), m_selectedSlot(0), m_slots{} {
}

// OFFSET: 0x00823210
// FUNCTION: fn_10291
UIShowInfoWnd::UIShowInfoWnd()
    : UIWindow(0), m_display(nullptr), showPayments(0), showPaymentItems(0), showDottedLines(0), m_checkTop(5), paymentFontSize(12), m_changed{},
      costumeChanged(0), paymentIconX(0), paymentIconYOffset(0), paymentTextX(0), paymentTextYOffset(0), m_offBitmap(nullptr) {
}

// OFFSET: 0x008232F0
// FUNCTION: fn_10292
UISlotOfMyStyleWnd::UISlotOfMyStyleWnd() : UIStylingshopTabParentWnd() {
}

// OFFSET: 0x00823370
// FUNCTION: fn_10293
UIStylingShopWnd::UIStylingShopWnd()
    : UIFrameWnd(0) {
}

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UIStaticBitmap.h"

// OFFSET: 0x0082CDA0
// FUNCTION: fn_10368
int UIAccessoryShowCase::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  char table[64];
  UIWindow* shop = g_windowMgr.LookupWindow(WID_STYLINGSHOPWND);
  switch (message) {
  case UM_COMMAND:
    if (argument1 == ID_BUTTON1 && sender) {
      UIAccBitmapButton* button = static_cast<UIAccBitmapButton*>(sender);
      for (unsigned int i = 0; i < this->m_itemFrames.size(); ++i) {
        this->m_itemFrames[i]->SetBitmap(i == button->Argument() - 1 ? "유저인터페이스\\styleshop\\shadow_item_on.bmp" : "유저인터페이스\\styleshop\\shadow_item_off.bmp");
      }

      memset(table, 0, sizeof(table));
      if (button->m_slot == 0) {
        strcpy_s(table, sizeof(table), "TopAccessory");
      } else if (button->m_slot == 1) {
        strcpy_s(table, sizeof(table), "MidAccessory");
      } else if (button->m_slot == 2) {
        strcpy_s(table, sizeof(table), "BottomAccessory");
      }

      bool apply = true;
      if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
        g_session.m_hotkeyScript->Call("StylingShop_GetIsDoram_by_goods", "sd>b", table, button->m_itemId, &apply);
      }

      if (apply == true) {
        ToggleStylingAccessory(button->m_itemId, button->m_slot);
      }
    }
    break;

  case UM_BMP_RBTN_UP:
    if (sender) {
      UIAccBitmapButton* button = static_cast<UIAccBitmapButton*>(sender);
      memset(table, 0, sizeof(table));
      if (button->m_slot == 0) {
        strcpy_s(table, sizeof(table), "TopAccessory");
      } else if (button->m_slot == 1) {
        strcpy_s(table, sizeof(table), "MidAccessory");
      } else if (button->m_slot == 2) {
        strcpy_s(table, sizeof(table), "BottomAccessory");
      }

      ITEM_DESC desc = FindItemDescription(button->m_itemId);
      ITEM_INFO item;
      item.SetItemId(button->m_itemId);
      item.SetName(desc.identifiedName);
      item.identified = 1;
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      if (window) {
        window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      }
    }
    break;

  default:
    shop->Invalidate();
    shop->InvalidateChildren();
    return UIScrollWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x0082DCC0
// FUNCTION: fn_10375
int UIStylingShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_BASE:
  case 496:
  case 497:
  case 498:
  case 583:
    if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) || g_session.IsTreeGroupTwo(g_session.GetDisplayJob()) || argument1 != 583) {
      std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it;
      for (it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
        if (it->second->geometry.visible == 1) {
          if (it->first->geometry.id != 496) {
            static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(WID_STYLINGSHOPWND))->m_preview->CopyStyle(it->second->m_display);
          }
          break;
        }
      }

      for (it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
        if (it->first->geometry.id == argument1) {
          if (it->first->geometry.id != 496) {
            it->second->m_display->CopyStyle(this->m_preview);
          }
          if (it->first->geometry.id == 498) {
            UI2ndCostumeWnd* costume = static_cast<UI2ndCostumeWnd*>(it->second);
            costume->m_hideCheckBox->SetChecked(costume->m_display->job == 0);
            costume->m_showCheckBox->SetChecked(costume->m_hideCheckBox->geometry.state == 0);
          }
          it->second->Show(1);
          it->second->InvalidateChildren();
        } else {
          static_cast<UIButton*>(it->first)->SetState(0);
          it->second->Show(0);
        }
      }
      this->Invalidate();
    }
    break;
  case ID_CLOSE:
    g_windowMgr.CloseWindow(WID_STYLINGSHOPWND);
    break;
  case ID_BUY:
    if (this->m_buyButton->Argument() == 0 &&
        g_windowMgr.Confirm(LookupMessage(MSI_ASK_MOVE_BY_PRIVATE_AIRPLANE), 2, false, false, 0, "Message", 280, 120, 0) != 188) {
      for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
        UIStylingshopTabParentWnd* tab = it->second;
        if (tab->geometry.visible == 0) {
          continue;
        }
        if (it->first->geometry.id == 496) {
          UIMyStyleWnd* myStyle = static_cast<UIMyStyleWnd*>(tab);
          UISlotOfMyStyleWnd* slot = myStyle->m_slots[myStyle->m_selectedSlot];
          if (slot && slot->m_showInfo && slot->m_showInfo->SendStyleChange() == true) {
            myStyle->RemoveChild(myStyle->m_slots[myStyle->m_selectedSlot], 1);
            myStyle->m_slots[myStyle->m_selectedSlot] = nullptr;
            myStyle->deleteStyleButtons[myStyle->m_selectedSlot]->Enable(0);
          }
        } else if (tab->m_showInfo) {
          tab->m_showInfo->SendStyleChange();
        }
      }
      this->Invalidate();
      return 0;
    }
    break;
  }

  this->Invalidate();
  return 0;
}

#pragma pack(push, 1)
struct StyleChangeHeader {
  /* 0000 */ unsigned short packetType;
  /* 0002 */ unsigned short packetLength;
  /* 0004 */ unsigned short count;
};

struct StyleChangeItem {
  /* 0000 */ int kind;
  /* 0004 */ int value;
};

#pragma pack(pop)

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Login/FuncDrawWNDChar.h"
#include "Utility/Text/GroupedInteger.h"
#include <new>
#include <string>
#include <timeapi.h>

// OFFSET: 0x00827AE0
// FUNCTION: fn_10331
void UIStylingShopWnd::AddTab(const char* normal, const char* hover, const char* pressed, const int& id, UIStylingshopTabParentWnd* tab, const char* tooltip) {
  UIStylingShopButton* button = new (std::nothrow) UIStylingShopButton();
  button->SetBitmap(normal, 0, 0);
  button->SetBitmap(hover, 1, 0);
  button->SetBitmap(pressed, 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(3, static_cast<int>(this->m_tabs.size()) * button->BitmapHeight() + 21);
  button->SetId(id);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  tab->SetSize(this->geometry.width - (button->geometry.x + button->BitmapWidth()), this->geometry.height - 47);
  tab->Move(static_cast<int>(this->geometry.width * 0.1), 17);
  tab->Show(0);
  this->AddChild(tab);
  this->m_tabs.insert(std::make_pair(static_cast<UIWindow*>(button), tab));
}

// OFFSET: 0x00829220
// FUNCTION: fn_10339
void UISlotOfMyStyleWnd::OnCreate(int, int) {
  this->m_display = new (std::nothrow) UIDisplayCharacter(true);

  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  this->m_display->SetSize(this->geometry.width - line->image.width, static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.73f)));
  this->m_display->Move(0, 0);
  this->m_display->LoadCostumes();
  this->AddChild(this->m_display);

  this->m_showInfo = new (std::nothrow) UIShowInfoWnd();
  this->m_showInfo->SetSize(this->geometry.width, static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.36f)));
  const int layout = 496;
  this->m_showInfo->ArrangeCheckBoxes(layout);
  this->m_showInfo->Move(0, static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.8f)) - 10);
  this->m_showInfo->m_display = this->m_display;
  this->AddChild(this->m_showInfo);
}

// OFFSET: 0x008294D0
// FUNCTION: fn_10340
void UIStylingShopWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);

  UIBasicStylingWnd* basic = new (std::nothrow) UIBasicStylingWnd();
  this->AddTab("유저인터페이스\\styleshop\\tab_hair_off.bmp", "유저인터페이스\\styleshop\\tab_hair_off.bmp", "유저인터페이스\\styleshop\\tab_hair_on.bmp", 200, basic,
               LookupMessage(MSI_SKILL_FAIL_MHP_TOO_FEW));
  basic->Show(1);
  static_cast<UIButton*>(this->m_tabs.begin()->first)->SetState(2);

  UIAccessoryCusWnd* accessory = new (std::nothrow) UIAccessoryCusWnd();
  this->AddTab("유저인터페이스\\styleshop\\tab_accessories_off.bmp", "유저인터페이스\\styleshop\\tab_accessories_off_over.bmp",
               "유저인터페이스\\styleshop\\tab_accessories_on.bmp", 497, accessory, LookupMessage(MSI_STYLING_SHOP_IS_NOT_COLORING_HEAD));

  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true || g_session.IsTreeGroupTwo(g_session.GetDisplayJob()) == 1) {
    UICostumeChangeWnd* costume = new UICostumeChangeWnd();
    this->AddTab("유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_on_over.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_on.bmp", 583,
                 costume, LookupMessage(MSI_STYLING_SHOP_OPENING_COMMENT));
  } else {
    UICostumeChangeWnd* costume = new UICostumeChangeWnd();
    this->AddTab("유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", 583,
                 costume, LookupMessage(MSI_STYLING_SHOP_OPENING_COMMENT));
  }

  UIMyStyleWnd* myStyle = new (std::nothrow) UIMyStyleWnd();
  this->AddTab("유저인터페이스\\styleshop\\tab_favorite_off.bmp", "유저인터페이스\\styleshop\\tab_favorite_off_over.bmp", "유저인터페이스\\styleshop\\tab_favorite_on.bmp", 496,
               myStyle, LookupMessage(MSI_STYLING_SHOP_SUCCESS));

  this->m_preview = new UIDisplayCharacter();
  this->m_preview->SetSize(120, 120);
  this->m_preview->Show(0);
  this->AddChild(this->m_preview);

  this->m_buyButton = static_cast<UIStylingShopButton*>(new (std::nothrow) UIBitmapButton());
  this->m_buyButton->SetBitmap("유저인터페이스\\styleshop\\btn_buy_out.bmp", 0, 0);
  this->m_buyButton->SetBitmap("유저인터페이스\\styleshop\\btn_buy_over.bmp", 1, 0);
  this->m_buyButton->SetBitmap("유저인터페이스\\styleshop\\btn_buy_down.bmp", 2, 0);
  this->m_buyButton->SetSize(this->m_buyButton->BitmapWidth(), this->m_buyButton->BitmapHeight());
  this->m_buyButton->Move(396, 306);
  this->m_buyButton->SetId(224);
  this->AddChild(this->m_buyButton);

  char zeny[256] = {};
  std::string text(LookupMessage(MSI_BANK_6));
  text += " ";
  text += FormatGroupedInteger(g_session.m_zeny, zeny, 256);
  text += "\tz";

  UIStaticText* label = new (std::nothrow) UIStaticText();
  label->SetSize(200, 12);
  label->Move(9, 310);
  label->SetFont(0, 11, 0, 0);
  label->SetColor(132, 90, 90);
  label->SetText(text.c_str(), 0);
  this->AddChild(label);
}

// OFFSET: 0x00829D10
// FUNCTION: fn_10347
void UIDisplayCharacter::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* shadow = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->shadowBitmapName)));
  if (shadow) {
    this->DrawBitmap(this->shadowX, this->shadowY, shadow, false);
  }

  CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  if (!player) {
    return;
  }

  FuncDrawWNDChar draw(
    this, this->geometry.width / 2, this->geometry.height - 33, static_cast<unsigned char>(player->sex), static_cast<short>(player->GetEffectiveJob(-1)),
    player->appearanceJob, static_cast<short>(this->job), static_cast<short>(this->head), static_cast<short>(this->bottomAccessory),
    static_cast<short>(this->topAccessory), static_cast<short>(this->middleAccessory), this->robe, player->isRiding, player->GetMadogearType(),
    g_session.GetBodyStyle(player->GetEffectiveJob(-1), this->bodyStyle, this->job), this->headPalette, this->direction, 0, this->animationStartTick
  );
  draw.Draw(false);
}

#include "Render/Text/TextContext.h"
#include "UI/Text/PixelTextWrapping.h"
#include "Utility/Text/FormatString.h"
#include <cstdio>
#include <vector>

// OFFSET: 0x0082BB70
// FUNCTION: fn_10364
void UIShowInfoWnd::DrawPayments() {
  int positions[8][2];
  if (dynamic_cast<UIStylingshopTabParentWnd*>(this->parent)->primaryTabVisible == true ||
      dynamic_cast<UIStylingshopTabParentWnd*>(this->parent)->accessoryTabVisible == true) {
    for (int i = 0; i < 8; ++i) {
      positions[i][0] = this->paymentTextX + 3;
      positions[i][1] = this->paymentTextYOffset + this->geometry.height / 8 * i;
    }
  } else {
    for (int i = 0; i < 4; ++i) {
      const int left = this->paymentTextX - 4;
      const int right = this->paymentTextX + 101;
      if (i == 0) {
        positions[i * 2][0] = left;
        positions[i * 2][1] = this->paymentTextYOffset;
        positions[i * 2 + 1][0] = right;
        positions[i * 2 + 1][1] = this->paymentTextYOffset;
      } else {
        positions[i * 2][0] = left;
        positions[i * 2][1] = this->geometry.height / 8 * i + this->paymentTextYOffset;
        positions[i * 2 + 1][0] = right;
        positions[i * 2 + 1][1] = this->geometry.height / 8 * i + this->paymentTextYOffset;
      }
    }
  }

  this->m_buyable = true;
  int totalZeny = 0;
  int row = 0;
  for (std::list<int>::iterator it = this->m_list.begin(); it != this->m_list.end(); ++it, ++row) {
    const int x = positions[row][0];
    const int y = positions[row][1];
    CBitmapRes* off = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_offBitmap)));
    if (off) {
      this->DrawBitmap(this->m_checks[row]->geometry.x, this->m_checks[row]->geometry.y, off, false);
    }

    std::uint32_t color = 0xBFBFBF;
    int zeny = 0;
    int itemId = 0;
    int costumeItemId = 0;
    this->GetPayment(*it, &zeny, &itemId, &costumeItemId);

    bool payable;
    if (zeny) {
      const int total = zeny + totalZeny;
      const int none = 0;
      payable = this->CheckPayment(row, total, itemId, none);
    } else {
      const int none = 0;
      payable = this->CheckPayment(row, zeny, itemId, none);
    }

    if (payable == true) {
      color = 0x5A5A84;
      if (this->m_checks[row]->geometry.state != 0) {
        totalZeny += zeny;
      }
      this->m_checks[row]->SetChecked(1);
    } else {
      this->m_buyable = false;
      this->m_checks[row]->SetChecked(0);
    }

    if (zeny) {
      this->DrawPaymentItem(*it, zeny, itemId, payable, row);
      char text[16] = {};
      std::sprintf(text, "%d\tZeny", zeny);
      if (!payable) {
        const int width = this->TextWidth(text, this->paymentFontSize, 0, 12, 0, 0);
        this->FillRegion(x - 2, y + 5, width + 2, 1, color);
      }
      this->DrawTextAt(x, y, text, 0, 0, this->paymentFontSize, color, 0, 0);
    } else if (itemId) {
      this->DrawPaymentItem(*it, zeny, itemId, payable, row);
      if (!payable) {
        this->m_checks[row]->SetTooltip(LookupMessage(MSI_STYLING_SHOP_FAIL_TO_ADD_CHAR_TO_LIKE));
        if (costumeItemId) {
          const int noZeny = 0;
          const int noItem = 0;
          const bool costumePayable = this->CheckPayment(row, noZeny, noItem, costumeItemId);
          if (costumePayable == true) {
            this->DrawPaymentItem(*it, zeny, itemId, true, row);
            this->m_checks[row]->SetTooltip(LookupMessage(MSI_STYLING_SHOP_ADJUST_NEW_CHAR_STYLE));
            ITEM_DESC description = FindItemDescription(costumeItemId);
            this->DrawTextAt(x, y, description.identifiedName, 0, 0, this->paymentFontSize, color, 0, 0);
          } else {
            ITEM_DESC description = FindItemDescription(itemId);
            if (!costumePayable) {
              const int width =
                  this->TextWidth(description.identifiedName, this->paymentFontSize, 0, 12, 0, 0);
              this->FillRegion(x - 2, y + 5, width + 2, 1, color);
            }
            this->DrawTextAt(x, y, description.identifiedName, 0, 0, this->paymentFontSize, color, 0, 0);
          }
        }
      } else {
        ITEM_DESC description = FindItemDescription(itemId);
        this->DrawTextAt(x, y, description.identifiedName, 0, 0, this->paymentFontSize, color, 0, 0);
      }
    } else {
      char text[260];
      CBitmapRes* icon = nullptr;
      std::memset(text, 0, sizeof(text));
      if (*it == 1) {
        icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\icon_color_off.bmp")));
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_CASHSHOP_FREE));
      } else if (*it == 7) {
        icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\icon_zeny.bmp")));
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d\tZeny", zeny);
      }

      if (this->showPaymentItems == 1) {
        this->DrawBitmap(this->paymentIconX, this->geometry.height / 8 * row + this->paymentIconYOffset, icon, false);
      }
      this->DrawTextAt(x, y, text, 0, 0, this->paymentFontSize, 0x5A5A84, 0, 0);
    }
  }

  if (this->m_list.size() < 8) {
    for (int i = static_cast<int>(this->m_list.size()); i < 8; ++i) {
      this->m_checks[i]->SetChecked(0);
    }
  }

  if (this->m_list.size() == 0) {
    for (int i = 0; i < 8; ++i) {
      positions[i][0] = this->paymentTextX;
      positions[i][1] = this->paymentTextYOffset + this->geometry.height / 8 * i;
    }
    this->m_buyable = false;

    if (this->showPayments == 1) {
      std::vector<std::string> lines;
      this->WrapPixelText(LookupMessage(MSI_UPGRADESKILLERROR_MORE_BASE_LEVEL), lines, this->geometry.width - 20);
      for (unsigned int i = 0; i < lines.size(); ++i) {
        this->DrawTextAt(20, positions[i][1], lines[i].c_str(), 0, 0, 11, 0, 0, 0);
      }
    }
  }

  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(WID_STYLINGSHOPWND));
  if (!shop) {
    return;
  }

  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = shop->m_tabs.begin(); it != shop->m_tabs.end(); ++it) {
    if (it->first->geometry.id == 496) {
      if (it->second->geometry.visible != 0) {
        for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator tab = shop->m_tabs.begin(); tab != shop->m_tabs.end(); ++tab) {
          if (tab->first->geometry.id == 496) {
            UIMyStyleWnd* myStyle = static_cast<UIMyStyleWnd*>(tab->second);
            bool buyable;
            if (myStyle->m_selectedSlot < 2 && myStyle->m_slots[myStyle->m_selectedSlot]) {
              buyable = myStyle->m_slots[myStyle->m_selectedSlot]->m_showInfo->m_buyable;
            } else {
              buyable = false;
            }
            shop->SetBuyEnabled(buyable);
          }
        }
        shop->Invalidate();
        return;
      }
      break;
    }
  }

  shop->SetBuyEnabled(this->m_buyable);
  shop->Invalidate();
}

// OFFSET: 0x00829F90
// FUNCTION: fn_10349
void UIShowInfoWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (!this->m_display) {
    return;
  }

  if (this->showDottedLines == 1) {
    for (int i = 0; i < 8; ++i) {
      CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_1px_light.bmp")));
      if (line) {
        UICheckBox* check = this->m_checks[i];
        this->DrawBitmap(check->geometry.x, check->geometry.y + check->geometry.width, line, false);
      }
    }
  }

  this->UpdateChangedStyles();
  this->DrawPayments();
}
// OFFSET: 0x00824EE0
// FUNCTION: fn_10318
bool UIShowInfoWnd::SendStyleChange() {
  char packet[2048];
  StyleChangeHeader header = {0xBF7, 0, 0};
  SecureZeroMemory(&header, sizeof(header));
  header.packetType = 0xBF7;
  const unsigned short count = static_cast<unsigned short>(this->m_list.size());
  const short length = count * 8 + 6;
  header.packetLength = length;

  memset(&packet[sizeof(header)], 0, sizeof(packet) - sizeof(header));
  *reinterpret_cast<StyleChangeHeader*>(packet) = header;
  reinterpret_cast<StyleChangeHeader*>(packet)->count = count;

  bool changed = false;
  int index = 0;
  for (std::list<int>::iterator it = this->m_list.begin(); it != this->m_list.end(); ++it, ++index) {
    int kind = 0;
    int value = 0;
    const int type = *it;
    if (type == 1) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        int table = 1;
        g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", this->m_display->GetStyleTable(table).c_str(), this->m_display->headPalette, &value);
      }
    } else if (type == 2) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        int table = 2;
        g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", this->m_display->GetStyleTable(table).c_str(), this->m_display->head, &value);
        kind = g_session.IsDoramJob(g_session.GetDisplayJob()) == true ? 6 : 1;
      }
    } else if (type == 3) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        int table = 3;
        g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", this->m_display->GetStyleTable(table).c_str(), this->m_display->bodyStyle, &value);
        kind = g_session.IsDoramJob(g_session.GetDisplayJob()) == true ? 8 : 2;
      }
    } else if (type == 4) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        int table = 4;
        g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", this->m_display->GetStyleTable(table).c_str(), this->m_display->topAccessoryItemId, &value);
        kind = 3;
      }
    } else if (type == 5) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        int table = 5;
        g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", this->m_display->GetStyleTable(table).c_str(), this->m_display->middleAccessoryItemId, &value);
        kind = 4;
      }
    } else if (type == 6) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        int table = 6;
        g_session.m_hotkeyScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", this->m_display->GetStyleTable(table).c_str(), this->m_display->bottomAccessoryItemId, &value);
        kind = 5;
      }
    } else if (type == 7) {
      if (this->m_checks[index]->geometry.state != 0) {
        changed = true;
        UIDisplayCharacter* display = this->m_display;
        std::map<int, int>::iterator job = display->m_jobs.find(display->job);
        if (job != display->m_jobs.end()) {
          value = job->second;
          kind = 9;
        }
      }
    }

    const int offset = index * 8 + 6;
    StyleChangeItem item = {kind, value};
    memcpy_s(&packet[offset], length - offset, &item, sizeof(item));
  }

  if (changed == true) {
    GetRagConnection().Send(length, packet);
    return true;
  }

  return false;
}

// OFFSET: 0x00825CA0
// FUNCTION: fn_10321
void UIAccessoryShowCase::LoadAccessories(int, int) {
  int loaded = 0;
  int skipped = 0;
  int count = 0;

  g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "TopAccessory", &count);
  int slot = 0;
  this->AddAccessories(loaded, skipped, "TopAccessory", count, slot);

  g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "MidAccessory", &count);
  slot = 1;
  this->AddAccessories(loaded, skipped, "MidAccessory", count, slot);

  g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "BottomAccessory", &count);
  slot = 2;
  this->AddAccessories(loaded, skipped, "BottomAccessory", count, slot);
}

// OFFSET: 0x008267E0
// FUNCTION: fn_10324
void UIAccessoryShowCase::AddAccessories(int& loaded, int& skipped, const char* table, const int& count, const int& slot) {
  const int first = loaded;
  int order = 1;
  for (unsigned int index = first + 1; index < static_cast<unsigned int>(count + first + 1); ++index, ++order) {
    int itemId = 0;
    g_session.m_hotkeyScript->Call("StylingShop_GetGoods", "sd>d", table, order, &itemId);
    if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
      bool available = false;
      g_session.m_hotkeyScript->Call("StylingShop_GetIsDoram_by_goods", "sd>b", table, itemId, &available);
      if (!available) {
        ++skipped;
        continue;
      }
    }

    char path[128] = {};
    char id[32] = {};
    std::sprintf(id, "%d", itemId);
    g_session.IconPath(path, id, 1);

    UIAccBitmapButton* button = new (std::nothrow) UIAccBitmapButton();
    button->SetBitmap(path, 0, 0);
    button->SetBitmap(path, 1, 0);
    button->SetBitmap(path, 2, 0);
    const int columns = this->geometry.width / (button->BitmapWidth() + 7) - 1;

    UIStaticBitmap* frame = new UIStaticBitmap();
    frame->SetBitmap("유저인터페이스\\styleshop\\shadow_item_off.bmp");
    frame->SetSize(frame->geometry.width, frame->geometry.height);
    frame->Move(static_cast<int>((button->BitmapWidth() + 7) * ((index - skipped - 1) % columns) + this->geometry.width * 0.03),
                static_cast<int>((button->BitmapHeight() + 7) * ((index - skipped - 1) / columns) + this->geometry.height * 0.03 + 10.0));
    this->AddScrollChild(frame);
    this->m_itemFrames.push_back(frame);

    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(static_cast<int>((button->BitmapWidth() + 7) * ((index - skipped - 1) % columns) + this->geometry.width * 0.03 + 2.5),
                 static_cast<int>((button->BitmapHeight() + 7) * ((index - skipped - 1) / columns) + this->geometry.height * 0.03));
    button->SetId(189);
    button->SetArgument(index);

    ITEM_DESC description = FindItemDescription(itemId);
    button->SetTooltip(description.identifiedName);
    button->m_itemId = itemId;
    button->m_slot = slot;
    this->AddScrollChild(button);
    ++loaded;
  }
}

#include "Contents/Items/ItemDescription.h"

// OFFSET: 0x0082AAC0
// FUNCTION: fn_10362
void UIMyStyleWnd::LoadLikeList() {
  CFile file;
  if (file.Open("savedata\\LikeListInfoFile.lua", 0, false, false)) {
    int size = static_cast<int>(file.GetSize());
    char* data = new char[size + 1];
    file.Read(data, size);
    data[size] = '\0';
    file.Close();

    struct LikeStyle {
      int slotNum;
      int padding_0004;
      int robe;
      int headStyle;
      int bodyStyle;
      int headPaletteNum;
      int bodyPaletteNum;
      int topAccNum;
      int midAccNum;
      int bottomAccNum;
    };
    LikeStyle style = {};
    char characterName[24] = {};
    char* line = data;
    for (int i = 0; i < size; ++i) {
      switch (data[i]) {
      case '*': {
        if (_mbsstr(reinterpret_cast<const unsigned char*>(characterName), reinterpret_cast<const unsigned char*>(g_session.DecodePlayerName())) == nullptr || style.slotNum > 2) {
          break;
        }

        UIDisplayCharacter* display = new UIDisplayCharacter();
        display->slotNumber = style.slotNum;
        display->robe = style.robe;
        display->head = style.headStyle;
        display->job = style.bodyStyle;
        display->headPalette = style.headPaletteNum;
        display->bodyStyle = style.bodyPaletteNum;
        display->topAccessory = style.topAccNum;
        display->middleAccessory = style.midAccNum;
        display->bottomAccessory = style.bottomAccNum;

        int goods = 0;
        int count = 0;
        if (style.topAccNum != 0) {
          g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "TopAccessory", &count);
          for (int index = 1; index < count + 1; ++index) {
            g_session.m_hotkeyScript->Call("StylingShop_GetGoods", "sd>d", "TopAccessory", index, &goods);
            ITEM_DESC description = FindItemDescription(goods);
            if (style.topAccNum == description.viewId) {
              display->topAccessoryItemId = goods;
              break;
            }
          }
        }
        if (style.midAccNum != 0) {
          g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "MidAccessory", &count);
          for (int index = 1; index < count + 1; ++index) {
            g_session.m_hotkeyScript->Call("StylingShop_GetGoods", "sd>d", "MidAccessory", index, &goods);
            ITEM_DESC description = FindItemDescription(goods);
            if (style.midAccNum == description.viewId) {
              display->middleAccessoryItemId = goods;
              break;
            }
          }
        }
        if (style.bottomAccNum != 0) {
          g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", "BottomAccessory", &count);
          for (int index = 1; index < count + 1; ++index) {
            g_session.m_hotkeyScript->Call("StylingShop_GetGoods", "sd>d", "BottomAccessory", index, &goods);
            ITEM_DESC description = FindItemDescription(goods);
            if (style.bottomAccNum == description.viewId) {
              display->bottomAccessoryItemId = goods;
              break;
            }
          }
        }

        UISlotOfMyStyleWnd* slot = new UISlotOfMyStyleWnd();
        int width = static_cast<int>(roundf(static_cast<float>(this->geometry.width) * 0.5f));
        slot->SetSize(width, this->geometry.height + 25);
        slot->Move(width * style.slotNum, 0);
        slot->m_display->CopyStyle(display);
        this->AddChild(slot);
        this->m_slots[style.slotNum] = slot;
        delete display;

        SecureZeroMemory(&style, sizeof(style));
        style.slotNum = 3;
      }
      case '\n':
      case '\r':
        line = &data[i + 1];
        break;
      case ':':
        data[i] = '\0';
        if (_mbsstr(reinterpret_cast<const unsigned char*>("characterName"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          ++i;
          char value[24] = {};
          for (char* out = value; data[i] != '\n' && data[i] != '\r'; ++i, ++out) {
            *out += data[i];
          }
          strcpy_s(characterName, sizeof(characterName), value);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("slotNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.slotNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("headPaletteNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.headPaletteNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("headStyle"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.headStyle = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("bodyPaletteNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.bodyPaletteNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("topAccNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.topAccNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("midAccNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.midAccNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("bottomAccNum"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.bottomAccNum = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("Robe"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.robe = atoi(&data[++i]);
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>("bodyStyle"), reinterpret_cast<const unsigned char*>(line)) != nullptr) {
          style.bodyStyle = atoi(&data[++i]);
        }
        break;
      }
    }
    if (data != nullptr) {
      delete[] data;
    }
  }
}

// OFFSET: 0x00828CA0
// FUNCTION: fn_10337
void UIMyStyleWnd::OnCreate(int, int) {
  this->LoadLikeList();

  UISlotForSelectWnd* select = new UISlotForSelectWnd();
  select->SetSize(this->geometry.width, this->geometry.height);
  select->Move(0, 0);
  select->SetId(204);
  this->AddChild(select);

  for (int i = 0; i < 2; ++i) {
    this->deleteStyleButtons[i] = new (std::nothrow) UIBitmapButton();
    this->deleteStyleButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_delete_out.bmp", 0, 0);
    this->deleteStyleButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_delete_over.bmp", 1, 0);
    this->deleteStyleButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_delete_press.bmp", 2, 0);
    this->deleteStyleButtons[i]->SetSize(this->deleteStyleButtons[i]->BitmapWidth(), this->deleteStyleButtons[i]->BitmapHeight());
    this->deleteStyleButtons[i]->Move(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.5f)) * i + 7, 4);
    this->deleteStyleButtons[i]->SetId(201);
    if (this->m_slots[i] == nullptr) {
      this->deleteStyleButtons[i]->Enable(0);
    }
    this->AddChild(this->deleteStyleButtons[i]);
  }

  for (int i = 0; i < 2; ++i) {
    this->rotateLeftButtons[i] = new (std::nothrow) UIBitmapButton();
    this->rotateLeftButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_l_out.bmp", 0, 0);
    this->rotateLeftButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_l_over.bmp", 1, 0);
    this->rotateLeftButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_l_press.bmp", 2, 0);
    this->rotateLeftButtons[i]->SetSize(this->rotateLeftButtons[i]->BitmapWidth(), this->rotateLeftButtons[i]->BitmapHeight());
    float ratio = static_cast<float>(i) * 0.5f + 0.25f;
    this->rotateLeftButtons[i]->Move(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * ratio)) - this->rotateLeftButtons[i]->BitmapWidth(), 213);
    this->rotateLeftButtons[i]->SetId(227);
    if (this->m_slots[i] == nullptr) {
      this->rotateLeftButtons[i]->Enable(0);
    }
    this->AddChild(this->rotateLeftButtons[i]);

    this->rotateRightButtons[i] = new (std::nothrow) UIBitmapButton();
    this->rotateRightButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_r_out.bmp", 0, 0);
    this->rotateRightButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_r_over.bmp", 1, 0);
    this->rotateRightButtons[i]->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_r_press.bmp", 2, 0);
    this->rotateRightButtons[i]->SetSize(this->rotateRightButtons[i]->BitmapWidth(), this->rotateRightButtons[i]->BitmapHeight());
    this->rotateRightButtons[i]->Move(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * ratio)), 213);
    this->rotateRightButtons[i]->SetId(226);
    if (this->m_slots[i] == nullptr) {
      this->rotateRightButtons[i]->Enable(0);
    }
    this->AddChild(this->rotateRightButtons[i]);
  }
}
// OFFSET: 0x008280F0
// FUNCTION: fn_10333
void UIAccessoryCusWnd::OnCreate(int, int) {
  this->m_display = new (std::nothrow) UIDisplayCharacter();
  this->m_display->SetSize(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.552f)),
                           static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.8f)));
  this->m_display->LoadCostumes();
  this->m_display->Move(0, 0);
  this->AddChild(this->m_display);

  this->m_showInfo = new (std::nothrow) UIShowInfoWnd();
  this->m_showInfo->SetSize(this->m_display->geometry.width, static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.4f)));
  const int layout = 497;
  this->m_showInfo->ArrangeCheckBoxes(layout);
  this->m_showInfo->Move(0, this->m_display->geometry.height);
  this->m_showInfo->m_display = this->m_display;
  this->AddChild(this->m_showInfo);

  UIAccessoryShowCase* showCase =
    new UIAccessoryShowCase(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.46f)), this->geometry.height);
  showCase->Move(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.6f)), 13);
  this->AddChild(showCase);
}

// OFFSET: 0x008270A0
// FUNCTION: fn_10326
void UIDisplayCharacter::CreateButtons() {
  UIBitmapButton* upperLeft = new (std::nothrow) UIBitmapButton();
  upperLeft->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_out.bmp", 0, 0);
  upperLeft->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_over.bmp", 1, 0);
  upperLeft->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_over.bmp", 2, 0);
  upperLeft->SetSize(upperLeft->BitmapWidth(), upperLeft->BitmapHeight());
  upperLeft->Move(11, 90);
  upperLeft->SetId(450);
  this->AddChild(upperLeft);

  UIBitmapButton* upperRight = new (std::nothrow) UIBitmapButton();
  upperRight->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_out.bmp", 0, 0);
  upperRight->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_over.bmp", 1, 0);
  upperRight->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_over.bmp", 2, 0);
  upperRight->SetSize(upperRight->BitmapWidth(), upperRight->BitmapHeight());
  upperRight->Move(231, 90);
  upperRight->SetId(451);
  this->AddChild(upperRight);

  UIBitmapButton* lowerLeft = new (std::nothrow) UIBitmapButton();
  lowerLeft->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_out.bmp", 0, 0);
  lowerLeft->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_over.bmp", 1, 0);
  lowerLeft->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_over.bmp", 2, 0);
  lowerLeft->SetSize(lowerLeft->BitmapWidth(), lowerLeft->BitmapHeight());
  lowerLeft->Move(11, 117);
  lowerLeft->SetId(354);
  this->AddChild(lowerLeft);

  UIBitmapButton* lowerRight = new (std::nothrow) UIBitmapButton();
  lowerRight->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_out.bmp", 0, 0);
  lowerRight->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_over.bmp", 1, 0);
  lowerRight->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_over.bmp", 2, 0);
  lowerRight->SetSize(lowerRight->BitmapWidth(), lowerRight->BitmapHeight());
  lowerRight->Move(231, 117);
  lowerRight->SetId(355);
  this->AddChild(lowerRight);

  this->m_favoriteButton = new (std::nothrow) UIBitmapButton();
  this->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_off.bmp", 0, 0);
  this->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_off_over.bmp", 1, 0);
  this->m_favoriteButton->SetBitmap("유저인터페이스\\styleshop\\btn_favorite_on.bmp", 2, 0);
  this->m_favoriteButton->SetSize(this->m_favoriteButton->BitmapWidth(), this->m_favoriteButton->BitmapHeight());
  this->m_favoriteButton->Move(7, 174);
  this->m_favoriteButton->SetId(356);
  this->AddChild(this->m_favoriteButton);

  UIBitmapButton* refresh = new (std::nothrow) UIBitmapButton();
  refresh->SetBitmap("유저인터페이스\\styleshop\\btn_refresh_out.bmp", 0, 0);
  refresh->SetBitmap("유저인터페이스\\styleshop\\btn_refresh_over.bmp", 1, 0);
  refresh->SetBitmap("유저인터페이스\\styleshop\\btn_refresh_press.bmp", 2, 0);
  refresh->SetSize(refresh->BitmapWidth(), refresh->BitmapHeight());
  refresh->Move(7, 193);
  refresh->SetId(185);
  this->AddChild(refresh);

  UIBitmapButton* rotateLeft = new (std::nothrow) UIBitmapButton();
  rotateLeft->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_l_out.bmp", 0, 0);
  rotateLeft->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_l_over.bmp", 1, 0);
  rotateLeft->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_l_press.bmp", 2, 0);
  rotateLeft->SetSize(rotateLeft->BitmapWidth(), rotateLeft->BitmapHeight());
  rotateLeft->Move(-rotateLeft->BitmapWidth() - static_cast<int>(this->geometry.width * -0.5), 212);
  rotateLeft->SetId(227);
  this->AddChild(rotateLeft);

  UIBitmapButton* rotateRight = new (std::nothrow) UIBitmapButton();
  rotateRight->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_r_out.bmp", 0, 0);
  rotateRight->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_r_over.bmp", 1, 0);
  rotateRight->SetBitmap("유저인터페이스\\styleshop\\btn_rotatearrow_r_press.bmp", 2, 0);
  rotateRight->SetSize(rotateRight->BitmapWidth(), rotateRight->BitmapHeight());
  rotateRight->Move(static_cast<int>(this->geometry.width * 0.5), 212);
  rotateRight->SetId(226);
  this->AddChild(rotateRight);
}

// OFFSET: 0x00828B90
// FUNCTION: fn_10336
void UIDisplayCharacter::OnCreate(int width, int height) {
  this->Resize(width, height);
  this->animationStartTick = timeGetTime();
  if (!this->hideFavoriteButton) {
    this->CreateButtons();
  }

  this->robe = g_session.m_robe;
  this->head = g_session.m_head;
  this->job = g_session.m_bodyCostume;
  this->headPalette = g_session.m_headPalette;
  this->bodyStyle = g_session.m_bodyStyle;
  this->topAccessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[4]);
  this->middleAccessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[8]);
  this->bottomAccessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0]);
  this->shadowX = static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.55f) - 40.0f);
  this->shadowY = static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.877f) - 17.0f);
}

// OFFSET: 0x00829080
// FUNCTION: fn_10338
void UIShowInfoWnd::OnCreate(int, int) {
  for (int i = 0; i < 8; ++i) {
    this->m_checks[i] = new (std::nothrow) UICheckBox();
    this->m_checks[i]->SetSize(17, 13);
    this->m_checks[i]->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
    this->m_checks[i]->SetChecked(0);
    this->m_checks[i]->Move(static_cast<int>(this->geometry.width * 0.1), this->geometry.height / 8 * i + this->m_checkTop);
    this->m_checks[i]->SetId(189);
    this->AddChild(this->m_checks[i]);
  }

  for (int i = 0; i < 8; ++i) {
    this->m_checks[i]->SetChecked(0);
    this->m_checks[i]->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
  }

  std::memset(this->m_changed, 0, sizeof(this->m_changed));
  this->costumeChanged = 0;
  for (unsigned int i = 0; i < this->m_list.size(); ++i) {
    this->m_list.clear();
  }
}

// OFFSET: 0x00827D00
// FUNCTION: fn_10332
void UI2ndCostumeWnd::OnCreate(int, int) {
  this->m_display = new (std::nothrow) UIDisplayCharacter();
  this->m_display->SetSize(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.552f)),
                           static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.8f)));
  this->m_display->Move(0, 0);
  this->AddChild(this->m_display);

  this->m_hideCheckBox = new (std::nothrow) UI2ndCostumeCheckBox();
  this->m_hideCheckBox->SetSize(17, 13);
  this->m_hideCheckBox->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
  this->m_hideCheckBox->SetChecked(0);
  this->m_hideCheckBox->Move(13, 230);
  this->AddChild(this->m_hideCheckBox);

  UIStaticText* hideText = new (std::nothrow) UIStaticText();
  hideText->SetSize(100, 12);
  hideText->Move(this->m_hideCheckBox->geometry.x + this->m_hideCheckBox->geometry.width + 5, 235);
  hideText->SetText(LookupMessage(MSI_SKILL_FAIL_MHP_TOO_FEW), 0);
  hideText->SetFont(0, 11, 0, 0);
  this->AddChild(hideText);

  this->m_showCheckBox = new (std::nothrow) UI2ndCostumeCheckBox();
  this->m_showCheckBox->SetSize(17, 13);
  this->m_showCheckBox->SetBitmaps("유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
  this->m_showCheckBox->SetChecked(0);
  this->m_showCheckBox->Move(13, 255);
  this->AddChild(this->m_showCheckBox);

  UIStaticText* showText = new (std::nothrow) UIStaticText();
  showText->SetSize(100, 12);
  showText->Move(this->m_showCheckBox->geometry.x + this->m_showCheckBox->geometry.width + 5, 260);
  showText->SetText(LookupMessage(MSI_STYLING_SHOP_OPENING_COMMENT), 0);
  showText->SetFont(0, 11, 0, 0);
  this->AddChild(showText);

  this->m_showInfo = new (std::nothrow) UIShowInfoWnd();
  this->m_showInfo->SetSize(this->geometry.width - this->m_display->geometry.width, this->geometry.height);
  const int layout = 498;
  this->m_showInfo->ArrangeCheckBoxes(layout);
  this->m_showInfo->Move(this->m_display->geometry.width, 13);
  this->m_showInfo->m_display = this->m_display;
  this->AddChild(this->m_showInfo);

  this->m_hideCheckBox->SetChecked(this->m_display->job == 0);
  this->m_showCheckBox->SetChecked(this->m_hideCheckBox->geometry.state == 0);
}

// OFFSET: 0x00828380
// FUNCTION: fn_10334
void UIBasicStylingWnd::OnCreate(int, int) {
  this->m_display = new (std::nothrow) UIDisplayCharacter();
  this->m_display->SetSize(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.552f)),
                           static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.8f)));
  this->m_display->LoadCostumes();
  this->m_display->Move(0, 0);
  this->AddChild(this->m_display);

  int count = 0;
  const int type = 1;
  g_session.m_hotkeyScript->Call("StylingShop_GetSizeInTable", "s>d", this->m_display->GetStyleTable(type).c_str(), &count);
  for (int i = 0; i < count; ++i) {
    char normal[64];
    char hover[64];
    char pressed[64];
    std::memset(normal, 0, sizeof(normal));
    std::memset(hover, 0, sizeof(hover));
    std::memset(pressed, 0, sizeof(pressed));
    sprintf_s(normal, "유저인터페이스\\styleshop\\color%02d_off.bmp", i + 1);
    sprintf_s(hover, "유저인터페이스\\styleshop\\color%02d_off_over.bmp", i + 1);
    sprintf_s(pressed, "유저인터페이스\\styleshop\\color%02d_on.bmp", i + 1);

    UIStylingShopButton* button = new (std::nothrow) UIStylingShopButton();
    button->SetBitmap(normal, 0, 0);
    button->SetBitmap(hover, 1, 0);
    button->SetBitmap(pressed, 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move((button->BitmapWidth() + 4) * (i % 4) + 108, (button->BitmapHeight() + 4) * (i / 4) + 237);
    button->SetId(495);
    this->AddChild(button);
    this->m_paletteButtons.push_back(button);
  }

  UIStylingShopButton* defaultButton = new (std::nothrow) UIStylingShopButton();
  defaultButton->SetBitmap("유저인터페이스\\styleshop\\color00_off.bmp", 0, 0);
  defaultButton->SetBitmap("유저인터페이스\\styleshop\\color00_on.bmp", 2, 0);
  defaultButton->SetSize(defaultButton->BitmapWidth(), defaultButton->BitmapHeight());
  defaultButton->Move(77, 256);
  defaultButton->SetId(495);
  this->AddChild(defaultButton);
  this->m_defaultPaletteButton = defaultButton;
  g_windowMgr.LookupWindow(WID_STYLINGSHOPWND);

  this->m_showInfo = new (std::nothrow) UIShowInfoWnd();
  this->m_showInfo->SetSize(this->geometry.width - this->m_display->geometry.width, this->geometry.height);
  const int layout = 200;
  this->m_showInfo->ArrangeCheckBoxes(layout);
  this->m_showInfo->Move(this->m_display->geometry.width, 13);
  this->m_showInfo->m_display = this->m_display;
  this->AddChild(this->m_showInfo);
}

// OFFSET: 0x008288C0
// FUNCTION: fn_10335
void UICostumeChangeWnd::OnCreate(int, int) {
  this->m_display = new UIDisplayCharacter();
  this->m_display->SetSize(static_cast<int>(std::round(static_cast<float>(this->geometry.width) * 0.552f)),
                           static_cast<int>(std::round(static_cast<float>(this->geometry.height) * 0.8f)));
  this->m_display->LoadCostumes();
  this->m_display->Move(0, 0);
  this->AddChild(this->m_display);

  this->m_showInfo = new UIShowInfoWnd();
  this->m_showInfo->SetSize(this->geometry.width - this->m_display->geometry.width, this->geometry.height);
  const int layout = 583;
  this->m_showInfo->ArrangeCheckBoxes(layout);
  this->m_showInfo->Move(this->m_display->geometry.width, 13);
  this->m_showInfo->m_display = this->m_display;
  this->AddChild(this->m_showInfo);

  UIBitmapButton* previous = new UIBitmapButton();
  previous->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_out.bmp", 0, 0);
  previous->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_over.bmp", 1, 0);
  previous->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_l_upper_over.bmp", 2, 0);
  previous->SetSize(previous->BitmapWidth(), previous->BitmapHeight());
  previous->Move(19, 248);
  previous->SetId(584);
  previous->Enable(1);
  this->AddChild(previous);

  UIBitmapButton* next = new UIBitmapButton();
  next->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_out.bmp", 0, 0);
  next->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_over.bmp", 1, 0);
  next->SetBitmap("유저인터페이스\\styleshop\\arrow_chr_r_upper_over.bmp", 2, 0);
  next->SetSize(next->BitmapWidth(), next->BitmapHeight());
  next->Move(220, 248);
  next->SetId(585);
  next->Enable(1);
  this->AddChild(next);
}
