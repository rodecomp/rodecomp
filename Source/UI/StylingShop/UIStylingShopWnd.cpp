#include "UI/StylingShop/UIStylingShopWnd.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/MessageTable.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowPolicy.h"
#include <cstring>
#include <mbstring.h>

// OFFSET: 0x00829B50
// FUNCTION: fn_10341
void UIStylingshopTabParentWnd::Slot15(int, int) {
}

// OFFSET: 0x00829B60
// FUNCTION: fn_10342
void UI2ndCostumeWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  for (int y = 230; y < 280; y += 25) {
    CBitmapRes* checkbox = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\checkbox.bmp")));
    if (checkbox) {
      this->DrawBitmap(13, y, &checkbox->image, false);
    }
  }

  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 0, &line->image, false);
  }
}

// OFFSET: 0x00829BE0
// FUNCTION: fn_10343
void UIAccessoryCusWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 5, &line->image, false);
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
    this->DrawBitmap(249, 5, &line->image, false);
  }

  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\bg_styling_color.bmp")));
  if (background) {
    this->DrawBitmap(71, 233, &background->image, false);
  }
}

// OFFSET: 0x00829CA0
// FUNCTION: fn_10346
void UICostumeChangeWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 0, &line->image, false);
  }

  this->DrawCenteredText(43, 248, g_session.GetJobName(this->m_display->job, -1), 161, 0, 11, 0, 0);
}

namespace {
struct UICheckBoxBitmaps : UICheckBox {
  void Apply(const char* checked, const char* unchecked) {
    this->m_checked = checked;
    this->m_unchecked = unchecked;
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_checked.c_str())));
    this->geometry.width = bitmap->image.width;
    this->geometry.height = bitmap->image.height;
    ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str()));
  }
};
}

// OFFSET: 0x00827670
// FUNCTION: fn_10328
void UIShowInfoWnd::ResetCheckBoxes() {
  for (int i = 0; i < 8; ++i) {
    this->m_checks[i]->SetChecked(0);
    static_cast<UICheckBoxBitmaps*>(this->m_checks[i])->Apply(
        "유저인터페이스\\styleshop\\check_able.bmp", "유저인터페이스\\styleshop\\Notcheck.bmp");
  }

  *reinterpret_cast<int*>(this->unknown_00AD) = 0;
  this->unknown_00B1 = 0;
  this->unknown_00BC = 0;
  this->m_list.clear();
}

// OFFSET: 0x00829F30
// FUNCTION: fn_10348
void UIMyStyleWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(-line->image.width - static_cast<int>(static_cast<float>(this->geometry.width) * -0.5f), 0, &line->image, false);
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
void UISlotForSelectWnd::Slot26(int, int) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 121, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0082A0C0
// FUNCTION: fn_10354
void UI2ndCostumeCheckBox::Slot25(int x, int y) {
  if (this->m_disabled == 0 && this->geometry.unknown_0030 != 1) {
    UICheckBox::Slot25(x, y);
  }
}

// OFFSET: 0x0082A0E0
// FUNCTION: fn_10355
void UISlotForSelectWnd::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 6, this->geometry.id, x, y, 0);
  }
}

// OFFSET: 0x0082A110
// OFFSET: 0x03DE9BE0
// FUNCTION: fn_10356
void UIStylingShopButton::Slot31(int, int) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  this->geometry.unknown_0030 = 2;
  UIWindow* parent = this->parent;
  if (parent) {
    if (WindowPolicy(this->m_buttonContext->policy, this->m_buttonContext->restricted, 3, parent->geometry.id) != 0) {
      int id = this->geometry.id;
      if (id == 185 && this->m_context->mode && *this->m_buttonContext->activeModeType == 2) {
        const char* text = this->m_buttonContext->services.MessageText(17);
        if (this->m_buttonContext->services.Confirm(text, 2, 1, 0, "Message", 280, 120) == 187) {
          this->m_context->mode->SendModeMessage(2, 0, 0, 0, 0);
        }
      } else {
        if (id == 184 || id == 321) {
          this->m_buttonContext->services.Submit(parent);
          id = this->geometry.id;
          parent = this->parent;
        }

        parent->SendMsg(this, 6, id, this->m_argument, 0, 0);
      }
    }
  } else {
    this->m_context->mode->SendModeMessage(0, this->geometry.id, this->m_argument, 0, 0);
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x0082A1C0
// FUNCTION: fn_10357
void UIStylingShopButton::Slot28(int, int) {
  if (!this->m_names[1].empty()) {
    return;
  }

  int screenX = 0;
  int screenY = -20;
  for (const UIWindow* window = this; window; window = window->parent) {
    screenX += window->geometry.x;
    screenY += window->geometry.y;
  }

  this->m_buttonContext->services.Tooltip("No Color", screenX, screenY, -1, false, true);
}

// OFFSET: 0x0082A210
// FUNCTION: fn_10358
void UIStylingShopButton::Slot27(int x, int y) {
  if (this->m_disabled == 0 && g_windowMgr.Capture() == this && x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height &&
      this->geometry.unknown_0030 != 2) {
    this->geometry.unknown_0030 = 2;
    this->Invalidate();
  }
}

// OFFSET: 0x0082A270
// FUNCTION: fn_10359
void UIStylingShopWnd::Slot19() {
  for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
    int id = it->first->geometry.id;
    if (id == 200) {
      it->second->unknown_0084 = it->second->geometry.visible == 1;
    } else if (id == 498 || id == 583) {
      it->second->unknown_0086 = it->second->geometry.visible == 1;
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
void UIAccBitmapButton::Slot33(int, int) {
  this->parent->SendMsg(this, 101, this->geometry.id, this->m_argument, 0, 0);
}

// OFFSET: 0x0082CCB0
// FUNCTION: fn_10366
int UI2ndCostumeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 213) {
    if (sender == this->m_showCheckBox) {
      if (this->m_display && this->m_hideCheckBox && this->m_showCheckBox->geometry.unknown_0030 != 0) {
        this->m_display->job = 1;
        this->m_hideCheckBox->SetChecked(0);
        this->m_showInfo->Invalidate();
      }
    } else if (sender == this->m_hideCheckBox) {
      if (this->m_display && this->m_showCheckBox && this->m_hideCheckBox->geometry.unknown_0030 != 0) {
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
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0082D100
// FUNCTION: fn_10369
int UIBasicStylingWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 495) {
    int count = 0;
    g_luaScript->Call("StylingShop_GetSizeInTable", "s>d", "HeadPalette", &count);

    bool unselected = true;
    for (int i = 0; i < count; ++i) {
      if (sender == this->m_paletteButtons[i]) {
        unselected = false;
        bool coloring = true;
        if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
          g_luaScript->Call("StylingShop_GetIsColoring_by_goods", "d>b", this->m_display->unknown_008C, &coloring);
        }

        if (coloring == true) {
          this->m_display->unknown_0094 = i + 1;
          this->m_display->Invalidate();
        } else {
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2834), 0, false, false, 0, "Message", 280, 120, 0);
        }
      } else {
        this->m_paletteButtons[i]->SetState(0);
      }
    }

    if (!unselected) {
      this->m_defaultPaletteButton->SetState(0);
    } else if (sender == this->m_defaultPaletteButton) {
      this->m_display->unknown_0094 = 0;
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
  UIWindow* preview = g_windowMgr.LookupWindow(281);
  if (message != 6) {
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
  if (message != 6) {
    if (message != 121) {
      return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }

    UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(281));
    if (this->m_slots[this->m_selectedSlot]) {
      UIDisplayCharacter* display = this->m_slots[this->m_selectedSlot]->m_display;
      for (std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it = shop->m_tabs.begin(); it != shop->m_tabs.end(); ++it) {
        if (it->first->geometry.id == 496 && it->second->geometry.visible == 1) {
          shop->m_preview->CopyStyle(display);
          shop->SendMsg(nullptr, 6, 200, 0, 0, 0);
        }
      }
      shop->Invalidate();
    }
  } else {
    switch (argument1) {
    case 201:
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2816), 2, false, false, 0, "Message", 280, 120, 0) != 188) {
        for (int i = 0; i < 2; ++i) {
          if (sender == this->unknown_0094[i]) {
            this->RemoveChild(this->m_slots[i], 1);
            this->m_slots[i] = nullptr;
            this->unknown_0094[i]->Enable(0);
            this->unknown_009C[i]->Enable(0);
            this->unknown_00A4[i]->Enable(0);
          }
        }
        this->Invalidate();
      }
      break;
    case 204:
      if (this->geometry.visible) {
        this->m_selectedSlot = argument2 / (this->geometry.width / 2);
      }

      for (int i = 0; i < 2; ++i) {
        UISlotOfMyStyleWnd* slot = this->m_slots[i];
        if (slot) {
          UIDisplayCharacter* display = slot->m_display;
          if (i == this->m_selectedSlot) {
            if (display) {
              display->unknown_007C = "유저인터페이스\\styleshop\\shadow_chr_on.bmp";
              display->Invalidate();
            }
          } else if (display) {
            display->unknown_007C = "유저인터페이스\\styleshop\\shadow_chr_off.bmp";
            display->Invalidate();
          }
          this->m_slots[i]->InvalidateChildren();
        }
      }
      this->Invalidate();
      break;
    case 226:
    case 227:
      for (int i = 0; i < 2; ++i) {
        if (sender == this->unknown_009C[i] || sender == this->unknown_00A4[i]) {
          this->m_slots[i]->m_display->SendMsg(nullptr, 6, argument1, 0, 0, 0);
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
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 189) {
    this->Invalidate();
  }

  return 0;
}

// OFFSET: 0x0082DC90
// FUNCTION: fn_10374
int UISlotOfMyStyleWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
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
  this->unknown_0088 = g_session.m_robe;
  this->unknown_008C = g_session.m_head;
  this->job = g_session.m_bodyCostume;
  this->unknown_0094 = g_session.m_headPalette;
  this->unknown_0098 = g_session.m_bodyStyle;
  this->unknown_009C = *reinterpret_cast<const int*>(&g_session.unknown_0EF4[4]);
  this->unknown_00A0 = *reinterpret_cast<const int*>(&g_session.unknown_0EF4[8]);
  this->unknown_00A4 = *reinterpret_cast<const int*>(&g_session.unknown_0EF4[0]);
}

// OFFSET: 0x00822C60
// FUNCTION: fn_10286
UI2ndCostumeWnd::UI2ndCostumeWnd(UIWindowContext& context) : UIStylingshopTabParentWnd(context), m_hideCheckBox(nullptr), m_showCheckBox(nullptr) {
}

// OFFSET: 0x00822E00
// FUNCTION: fn_10287
UIBasicStylingWnd::UIBasicStylingWnd(UIWindowContext& context) : UIStylingshopTabParentWnd(context) {
}

// OFFSET: 0x00822EA0
// FUNCTION: fn_10288
UICostumeChangeWnd::UICostumeChangeWnd(UIWindowContext& context) : UIStylingshopTabParentWnd(context) {
}

// OFFSET: 0x00822F20
// FUNCTION: fn_10289
UIDisplayCharacter::UIDisplayCharacter(UIWindowContext& context) : UIWindow(context, 0) {
  this->unknown_0080 = 0;
  this->unknown_0084 = 0;
  this->unknown_0088 = 0;
  this->unknown_008C = 0;
  this->job = 0;
  this->unknown_0094 = 0;
  this->unknown_0098 = 0;
  this->unknown_009C = 0;
  this->unknown_00A0 = 0;
  this->unknown_00A4 = 0;
  *reinterpret_cast<unsigned char*>(&this->unknown_00B0) = 0;
  this->unknown_00BC = 0;
  this->unknown_00C0 = 0;
  this->unknown_00C4 = 0;
  this->unknown_00B4 = 0;
  this->unknown_00B8 = 0;
  this->unknown_007C = "유저인터페이스\\styleshop\\shadow_chr_off.bmp";
}

// OFFSET: 0x00823CE0
// FUNCTION: fn_10298
void UIDisplayCharacter::CopyStyle(const UIDisplayCharacter* other) {
  this->unknown_0080 = other->unknown_0080;
  this->unknown_0084 = other->unknown_0084;
  this->unknown_0088 = other->unknown_0088;
  this->unknown_008C = other->unknown_008C;
  this->job = other->job;
  this->unknown_0094 = other->unknown_0094;
  this->unknown_0098 = other->unknown_0098;
  this->unknown_009C = other->unknown_009C;
  this->unknown_00A0 = other->unknown_00A0;
  this->unknown_00A4 = other->unknown_00A4;
  this->unknown_00BC = other->unknown_00BC;
  this->unknown_00C0 = other->unknown_00C0;
  this->unknown_00C4 = other->unknown_00C4;
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
          if (saved->unknown_008C == display->unknown_008C && saved->unknown_0094 == display->unknown_0094 && saved->unknown_0098 == display->unknown_0098 &&
              saved->unknown_009C == display->unknown_009C && saved->unknown_00A0 == display->unknown_00A0 && saved->unknown_00A4 == display->unknown_00A4 &&
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
void UIStylingshopTabParentWnd::UpdateFavoriteButton(const bool& favorite) {
  UIDisplayCharacter* display = this->m_display;
  if (!display) {
    return;
  }

  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(281));
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
      while (myStyle->m_slots[index]) {
        ++index;
        if (index >= 2) {
          return false;
        }
      }

      UISlotOfMyStyleWnd* slot = new UISlotOfMyStyleWnd(*this->m_context);
      slot->SetSize(static_cast<int>(myStyle->geometry.width * 0.5 - 3.0), myStyle->geometry.height + 25);
      slot->Move(static_cast<int>(myStyle->geometry.width * 0.5 * index), 0);
      display->unknown_0080 = index;
      slot->m_display->CopyStyle(display);
      myStyle->AddChildFront(slot);
      myStyle->m_slots[index] = slot;
      myStyle->unknown_0094[index]->Enable(1);
      myStyle->unknown_009C[index]->Enable(1);
      myStyle->unknown_00A4[index]->Enable(1);
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
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2813), 0);
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\bg_styling.bmp")));
  if (background) {
    this->DrawBitmap(0, 0, &background->image, false);
  }
}

// OFFSET: 0x00825910
// FUNCTION: fn_10320
void UIShowInfoWnd::CheckColoringGoods(bool* coloring) {
  if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
    g_luaScript->Call("StylingShop_GetIsColoring_by_goods", "d>b", this->m_display->unknown_008C, coloring);
  }

  const unsigned char* flags = this->unknown_00AD;
  const unsigned char wingFlag = reinterpret_cast<const unsigned char*>(&this->unknown_00B1)[0];
  const unsigned char accessoryFlag = reinterpret_cast<const unsigned char*>(&this->unknown_00B1)[1];
  const UIDisplayCharacter* display = this->m_display;
  if (*coloring) {
    if (flags[0] == 1 && display->unknown_0094 == g_session.m_headPalette) {
    } else if (flags[1] == 1 && display->unknown_008C == g_session.m_head) {
    } else if (this->unknown_00BC == 1 && display->job == g_session.m_bodyCostume) {
    } else if (flags[2] == 1 && display->unknown_0098 == g_session.m_bodyStyle) {
    } else if (flags[3] == 1 && (display->unknown_009C == *reinterpret_cast<const int*>(&g_session.unknown_0EF4[4]) || display->unknown_009C == 0)) {
    } else if (wingFlag == 1 && (display->unknown_00A0 == *reinterpret_cast<const int*>(&g_session.unknown_0EF4[8]) || display->unknown_00A0 == 0)) {
    } else if (accessoryFlag == 1 && (display->unknown_00A4 == *reinterpret_cast<const int*>(&g_session.unknown_0EF4[0]) || display->unknown_00A0 == 0)) {
    } else {
      return;
    }
  }

  for (unsigned int i = 0; i < this->m_list.size(); ++i) {
    this->m_checks[i]->SetTooltip("");
    this->m_checks[i]->SetChecked(0);
  }
  this->m_list.clear();
  *reinterpret_cast<int*>(this->unknown_00AD) = 0;
  this->unknown_00B1 = 0;
  this->unknown_00BC = 0;
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
  if (g_luaScript->Call("StylingShop_GetCostumeSize", "d>d", job, &count) == true) {
    int value = 0;
    int key = 0;
    for (int i = 0; i < count; ++i) {
      if (g_luaScript->Call("StylingShop_GetChangeCostume", "dd>dd", job, value, &key, &value) == true) {
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
  case 497:
    this->m_checkTop = 1;
    this->unknown_00A8 = 11;
    this->unknown_00C8 = 24;
    this->unknown_00CC = 0;
    this->m_offBitmap = "유저인터페이스\\styleshop\\checkbox_mini_off.bmp";
    for (int row = static_cast<int>(this->m_list.size()); row < 4; ++row) {
      for (int column = static_cast<int>(this->m_list.size()); column < 2; ++column) {
        UICheckBox* check = this->m_checks[row * 2 + column];
        check->SetBitmaps("유저인터페이스\\styleshop\\checkbox_mini_on.bmp", "유저인터페이스\\styleshop\\checkbox_mini_non.bmp");
        int y = this->m_checkTop;
        if (row > 0) {
          y += this->geometry.height / 8 * row;
        }
        check->Move(column * 105 + 7, y);
      }
    }
    break;
  case 200:
  case 498:
  case 583:
    this->m_checkTop = 6;
    this->unknown_00A8 = 12;
    this->unknown_00A1 = 1;
    this->unknown_00A2 = 1;
    this->unknown_00A0 = 1;
    this->unknown_00C0 = 41;
    this->unknown_00C4 = -2;
    this->unknown_00C8 = 68;
    this->unknown_00CC = 6;
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
void ToggleStylingAccessory(int itemId, int slot) {
  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(281));
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
    if (desc.unknown_0040 == display->unknown_009C) {
      display->unknown_00BC = 0;
      display->unknown_009C = 0;
    } else {
      display->unknown_00BC = itemId;
      display->unknown_009C = desc.unknown_0040;
    }
    break;
  case 1:
    display = tab->m_display;
    if (desc.unknown_0040 == display->unknown_00A0) {
      display->unknown_00C0 = 0;
      display->unknown_00A0 = 0;
    } else {
      display->unknown_00C0 = itemId;
      display->unknown_00A0 = desc.unknown_0040;
    }
    break;
  case 2:
    display = tab->m_display;
    if (desc.unknown_0040 == display->unknown_00A4) {
      display->unknown_00C4 = 0;
      display->unknown_00A4 = 0;
    } else {
      display->unknown_00C4 = itemId;
      display->unknown_00A4 = desc.unknown_0040;
    }
    break;
  }

  tab->InvalidateChildren();
}

// OFFSET: 0x0082E830
// FUNCTION: fn_10379
void UIDisplayCharacter::SelectNextStyle(const char* table, int* goods) {
  int size = 0;
  g_luaScript->Call("StylingShop_GetSizeInTable", "s>d", table, &size);
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
  g_luaScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", table, *goods, &sequence);
  ++sequence;
  if (sequence > size) {
    sequence = 1;
  }
  g_luaScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", table, sequence, goods);

  if (g_session.GetDisplayJob() > JT_2004_JOB_BEGIN && g_session.GetDisplayJob() <= JT_THIEF_H && *goods == 0 && g_session.m_bodyStyle == 1) {
    *goods = 1;
  }
}

// OFFSET: 0x0082ED80
// FUNCTION: fn_10380
void UIDisplayCharacter::SelectPreviousStyle(const char* table, int* goods) {
  int size = 0;
  g_luaScript->Call("StylingShop_GetSizeInTable", "s>d", table, &size);
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
  g_luaScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", table, *goods, &sequence);
  --sequence;
  if (sequence < 1) {
    sequence = size;
  }
  if (sequence <= 0) {
    sequence = 1;
  }
  g_luaScript->Call("StylingShop_GetGoods", "sd>d", table, sequence, goods);

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
  case 4:
    table = "TopAccessory";
    break;
  case 5:
    table = "MidAccessory";
    break;
  case 6:
    table = "BottomAccessory";
    break;
  case 7:
    table = "BodyStyle";
    break;
  }

  switch (type) {
  case 1:
  case 4:
  case 5:
  case 6:
    return table;
  default:
    if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
      table = "Doram" + table;
    }
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
    g_luaScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->unknown_0094, zeny, itemId, costumeItemId);
    break;
  case 2:
    g_luaScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->unknown_008C, zeny, itemId, costumeItemId);
    break;
  case 3:
    g_luaScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->unknown_0098, zeny, itemId, costumeItemId);
    break;
  case 4:
    g_luaScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->unknown_00BC, zeny, itemId, costumeItemId);
    break;
  case 5:
    g_luaScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->unknown_00C0, zeny, itemId, costumeItemId);
    break;
  case 6:
    g_luaScript->Call("StylingShop_GetPayment_by_goods", "sd>ddd", this->m_display->GetStyleTable(type).c_str(), this->m_display->unknown_00C4, zeny, itemId, costumeItemId);
    break;
  case 7:
    g_luaScript->Call("StylingShop_GetPaymentChangeCostume", "dd>ddd", g_session.m_job, this->m_display->job, zeny, itemId, costumeItemId);
    break;
  }
}

// OFFSET: 0x0082C890
// FUNCTION: fn_10365
void UIShowInfoWnd::UpdateChangedStyles() {
  bool coloring = true;
  this->CheckColoringGoods(&coloring);
  if (!this->unknown_00AD[0] && this->m_display->unknown_0094 != g_session.m_headPalette && coloring == true) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(1);
    this->unknown_00AD[0] = 1;
  }

  if (!this->unknown_00AD[1] && this->m_display->unknown_008C != g_session.m_head) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(2);
    this->unknown_00AD[1] = 1;
  }

  if (!this->unknown_00AD[2] && this->m_display->unknown_0098 != g_session.m_bodyStyle) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(3);
    this->unknown_00AD[2] = 1;
  }

  int sequence = 0;
  g_luaScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", "TopAccessory", this->m_display->unknown_00BC, &sequence);
  if (!this->unknown_00AD[3] && this->m_display->unknown_009C != *reinterpret_cast<const int*>(&g_session.unknown_0EF4[4]) && this->m_display->unknown_009C != 0 &&
      sequence != 0) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(4);
    this->unknown_00AD[3] = 1;
  }

  unsigned char* accessoryChanged = reinterpret_cast<unsigned char*>(&this->unknown_00B1);
  g_luaScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", "MidAccessory", this->m_display->unknown_00C0, &sequence);
  if (!accessoryChanged[0] && this->m_display->unknown_00A0 != *reinterpret_cast<const int*>(&g_session.unknown_0EF4[8]) && this->m_display->unknown_00A0 != 0 &&
      sequence != 0) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(5);
    accessoryChanged[0] = 1;
  }

  g_luaScript->Call("StylingShop_GetSeqnumber_by_goods", "sd>d", "BottomAccessory", this->m_display->unknown_00C4, &sequence);
  if (!accessoryChanged[1] && this->m_display->unknown_00A4 != *reinterpret_cast<const int*>(&g_session.unknown_0EF4[0]) && this->m_display->unknown_00A4 != 0 &&
      sequence != 0) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(6);
    accessoryChanged[1] = 1;
  }

  if (!this->unknown_00BC && this->m_display->job != g_session.m_bodyCostume) {
    this->m_checks[this->m_list.size()]->SetChecked(1);
    this->m_list.push_back(7);
    this->unknown_00BC = 1;
  }
}

// OFFSET: 0x0082D550
// FUNCTION: fn_10371
int UIDisplayCharacter::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  UIStylingShopWnd* shop = static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(281));
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 185:
    this->LoadSessionStyle();
    break;
  case 226:
    if (this->unknown_0084 > 0) {
      --this->unknown_0084;
    } else {
      this->unknown_0084 = 7;
    }
    break;
  case 227:
    this->unknown_0084 = this->unknown_0084 < 7 ? this->unknown_0084 + 1 : 0;
    break;
  case 354:
    this->SelectPreviousStyle(this->GetStyleTable(3).c_str(), &this->unknown_0098);
    break;
  case 355:
    this->SelectNextStyle(this->GetStyleTable(3).c_str(), &this->unknown_0098);
    break;
  case 356:
    if (this->m_favoriteButton->m_argument == 0) {
      if (shop->AddToMyStyle() == true) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2819), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        this->m_favoriteButton->SetTooltip(LookupMessage(2817));
      }
    }
    break;
  case 450:
    this->SelectPreviousStyle(this->GetStyleTable(2).c_str(), &this->unknown_008C);
    break;
  case 451:
    this->SelectNextStyle(this->GetStyleTable(2).c_str(), &this->unknown_008C);
    break;
  case 495:
    shop->SendMsg(nullptr, 495, 0, 0, 0, 0);
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
  std::string likeList;
  CFile file;
  if (file.Open("savedata\\LikeListInfoFile.lua", 0, false, false)) {
    int size = static_cast<int>(file.GetSize());
    char* data = new char[size + 1];
    file.Read(data, size);
    data[size] = '\0';
    file.Close();

    struct LikeStyle {
      int slotNum;
      int unknown_04;
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
          char* bytes = reinterpret_cast<char*>(&style);
          for (int j = 0; j < sizeof(style); ++j) {
            bytes[j] = 0;
          }
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
      sprintf_s(formatted, "characterName:%s\nslotNum:%d\nheadPaletteNum:%d\nheadStyle:%d\nbodyStyle:%d\nbodyPaletteNum:%d\ntopAccNum:%d\nmidAccNum:%d\nbottomAccNum:%d\nRobe:%d\n*\n", g_session.DecodePlayerName(), display->unknown_0080, display->unknown_0094, display->unknown_008C, display->job, display->unknown_0098, display->unknown_009C, display->unknown_00A0, display->unknown_00A4, display->unknown_0088);
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
  if (!this->unknown_00A1) {
    return;
  }

  CBitmapRes* bitmap = nullptr;
  int id = itemId;
  if (zeny != 0) {
    char path[128];
    memset(path, 0, sizeof(path));
    if (type == 4 || type == 5 || type == 6) {
      if (type == 4) {
        id = this->m_display->unknown_00BC;
      } else if (type == 5) {
        id = this->m_display->unknown_00C0;
      } else {
        id = this->m_display->unknown_00C4;
      }

      char number[32] = {};
      std::sprintf(number, "%d", id);
      ItemDescriptions descriptions{g_itemDescriptions};
      descriptions.IconPath(path, number, 1);
    } else {
      strcpy_s(path, sizeof(path), "유저인터페이스\\styleshop\\icon_zeny.bmp");
    }

    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  } else if (id != 0) {
    if (type == 4) {
      id = this->m_display->unknown_00BC;
    } else if (type == 5) {
      id = this->m_display->unknown_00C0;
    } else if (type == 6) {
      id = this->m_display->unknown_00C4;
    }

    char path[128];
    memset(path, 0, sizeof(path));
    char number[32] = {};
    std::sprintf(number, "%d", id);
    ItemDescriptions descriptions{g_itemDescriptions};
    descriptions.IconPath(path, number, 1);
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  }

  if (!enabled) {
    this->DrawAuxiliary(1, this->unknown_00C0, this->geometry.height / 8 * row + this->unknown_00C4, bitmap ? &bitmap->image : nullptr, true, false);
    return;
  }

  if (bitmap) {
    this->DrawBitmap(this->unknown_00C0, this->geometry.height / 8 * row + this->unknown_00C4, &bitmap->image, false);
  }
}

// OFFSET: 0x00823AA0
// FUNCTION: fn_10297
UIShowInfoWnd::~UIShowInfoWnd() {
}

// OFFSET: 0x00823180
// FUNCTION: fn_10290
UIMyStyleWnd::UIMyStyleWnd(UIWindowContext& context) : UIStylingshopTabParentWnd(context), m_selectedSlot(0), m_slots{} {
}

// OFFSET: 0x00823210
// FUNCTION: fn_10291
UIShowInfoWnd::UIShowInfoWnd(UIWindowContext& context)
    : UIWindow(context, 0), m_display(nullptr), unknown_00A0(0), unknown_00A1(0), unknown_00A2(0), m_checkTop(5), unknown_00A8(12), unknown_00AD{},
      unknown_00B1(0), unknown_00BC(0), unknown_00C0(0), unknown_00C4(0), unknown_00C8(0), unknown_00CC(0), m_offBitmap(nullptr) {
}

// OFFSET: 0x008232F0
// FUNCTION: fn_10292
UISlotOfMyStyleWnd::UISlotOfMyStyleWnd(UIWindowContext& context) : UIStylingshopTabParentWnd(context) {
}

// OFFSET: 0x00823370
// FUNCTION: fn_10293
UIStylingShopWnd::UIStylingShopWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

#include "Contents/Items/ItemInfo.h"
#include "UI/UIStaticBitmap.h"

// OFFSET: 0x0082CDA0
// FUNCTION: fn_10368
int UIAccessoryShowCase::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  char table[64];
  UIWindow* shop = g_windowMgr.LookupWindow(281);
  if (message == 6) {
    if (argument1 == 189 && sender) {
      UIAccBitmapButton* button = static_cast<UIAccBitmapButton*>(sender);
      for (unsigned int i = 0; i < this->m_itemFrames.size(); ++i) {
        this->m_itemFrames[i]->SetBitmap(i == button->Argument() - 1 ? "유저인터페이스\\styleshop\\shadow_item_on.bmp" : "유저인터페이스\\styleshop\\shadow_item_off.bmp");
      }

      memset(table, 0, sizeof(table));
      switch (button->m_slot) {
      case 0:
        strcpy_s(table, sizeof(table), "TopAccessory");
        break;
      case 1:
        strcpy_s(table, sizeof(table), "MidAccessory");
        break;
      case 2:
        strcpy_s(table, sizeof(table), "BottomAccessory");
        break;
      }

      bool apply = true;
      if (g_session.IsDoramJob(g_session.GetDisplayJob()) == true) {
        g_luaScript->Call("StylingShop_GetIsDoram_by_goods", "sd>b", table, button->m_itemId, &apply);
      }

      if (apply == true) {
        ToggleStylingAccessory(button->m_itemId, button->m_slot);
      }
    }
  } else if (message == 101) {
    if (sender) {
      UIAccBitmapButton* button = static_cast<UIAccBitmapButton*>(sender);
      memset(table, 0, sizeof(table));
      switch (button->m_slot) {
      case 0:
        strcpy_s(table, sizeof(table), "TopAccessory");
        break;
      case 1:
        strcpy_s(table, sizeof(table), "MidAccessory");
        break;
      case 2:
        strcpy_s(table, sizeof(table), "BottomAccessory");
        break;
      }

      ITEM_DESC desc = FindItemDescription(button->m_itemId);
      ITEM_INFO item;
      item.SetItemId(button->m_itemId);
      item.SetName(desc.identifiedName);
      item.identified = 1;
      UIWindow* window = g_windowMgr.EnsureWindow(12);
      if (window) {
        window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      }
    }
  } else {
    shop->Invalidate();
    shop->InvalidateChildren();
    return UIScrollWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x0082DCC0
// FUNCTION: fn_10375
int UIStylingShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 200:
  case 496:
  case 497:
  case 498:
  case 583:
    if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) || g_session.IsTreeGroupTwo(g_session.GetDisplayJob()) || argument1 != 583) {
      std::map<UIWindow*, UIStylingshopTabParentWnd*>::iterator it;
      for (it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
        if (it->second->geometry.visible == 1) {
          if (it->first->geometry.id != 496) {
            static_cast<UIStylingShopWnd*>(g_windowMgr.LookupWindow(281))->m_preview->CopyStyle(it->second->m_display);
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
            costume->m_showCheckBox->SetChecked(costume->m_hideCheckBox->geometry.unknown_0030 == 0);
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
  case 201:
    g_windowMgr.CloseWindow(281);
    break;
  case 224:
    if (this->m_buyButton->Argument() == 0 &&
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2818), 2, false, false, 0, "Message", 280, 120, 0) != 188) {
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
            myStyle->unknown_0094[myStyle->m_selectedSlot]->Enable(0);
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

struct StyleChangePacket {
  /* 0000 */ StyleChangeHeader header;
  /* 0006 */ unsigned char items[2042];
};
#pragma pack(pop)

// OFFSET: 0x00824EE0

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "Utility/Text/GroupedInteger.h"
#include <new>
#include <string>

// OFFSET: 0x00827AE0
// FUNCTION: fn_10331
void UIStylingShopWnd::AddTab(const char* normal, const char* hover, const char* pressed, const int& id, UIStylingshopTabParentWnd* tab, const char* tooltip) {
  UIStylingShopButton* button = new (std::nothrow) UIStylingShopButton(*this->m_context, *this->m_controls);
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
void UISlotOfMyStyleWnd::Slot15(int, int) {
  UIDisplayCharacter* display = new (std::nothrow) UIDisplayCharacter(*this->m_context);
  if (display) {
    *reinterpret_cast<unsigned char*>(&display->unknown_00B0) = 1;
  }
  this->m_display = display;

  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  this->m_display->SetSize(this->geometry.width - line->image.width, static_cast<int>(roundf(static_cast<float>(this->geometry.height) * 0.73f)));
  this->m_display->Move(0, 0);
  this->m_display->LoadCostumes();
  this->AddChild(this->m_display);

  this->m_showInfo = new (std::nothrow) UIShowInfoWnd(*this->m_context);
  this->m_showInfo->SetSize(this->geometry.width, static_cast<int>(roundf(static_cast<float>(this->geometry.height) * 0.36f)));
  const int layout = 496;
  this->m_showInfo->ArrangeCheckBoxes(layout);
  this->m_showInfo->Move(0, static_cast<int>(roundf(static_cast<float>(this->geometry.height) * 0.8f)) - 10);
  this->m_showInfo->m_display = this->m_display;
  this->AddChild(this->m_showInfo);
}

// OFFSET: 0x008294D0
// FUNCTION: fn_10340
void UIStylingShopWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);

  UIBasicStylingWnd* basic = new (std::nothrow) UIBasicStylingWnd(*this->m_context);
  this->AddTab("유저인터페이스\\styleshop\\tab_hair_off.bmp", "유저인터페이스\\styleshop\\tab_hair_off.bmp", "유저인터페이스\\styleshop\\tab_hair_on.bmp", 200, basic,
               LookupMessage(2837));
  basic->Show(1);
  static_cast<UIButton*>(this->m_tabs.begin()->first)->SetState(2);

  UIAccessoryCusWnd* accessory = new (std::nothrow) UIAccessoryCusWnd(*this->m_context);
  this->AddTab("유저인터페이스\\styleshop\\tab_accessories_off.bmp", "유저인터페이스\\styleshop\\tab_accessories_off_over.bmp",
               "유저인터페이스\\styleshop\\tab_accessories_on.bmp", 497, accessory, LookupMessage(2831));

  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) != true && g_session.IsTreeGroupTwo(g_session.GetDisplayJob()) != 1) {
    UICostumeChangeWnd* costume = new UICostumeChangeWnd(*this->m_context);
    this->AddTab("유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", 583,
                 costume, LookupMessage(2832));
  } else {
    UICostumeChangeWnd* costume = new UICostumeChangeWnd(*this->m_context);
    this->AddTab("유저인터페이스\\styleshop\\tab_2ndcostume_off.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_on_over.bmp", "유저인터페이스\\styleshop\\tab_2ndcostume_on.bmp", 583,
                 costume, LookupMessage(2832));
  }

  UIMyStyleWnd* myStyle = new (std::nothrow) UIMyStyleWnd(*this->m_context);
  this->AddTab("유저인터페이스\\styleshop\\tab_favorite_off.bmp", "유저인터페이스\\styleshop\\tab_favorite_off_over.bmp", "유저인터페이스\\styleshop\\tab_favorite_on.bmp", 496,
               myStyle, LookupMessage(2833));

  this->m_preview = new UIDisplayCharacter(*this->m_context);
  this->m_preview->SetSize(120, 120);
  this->m_preview->Show(0);
  this->AddChild(this->m_preview);

  this->m_buyButton = static_cast<UIStylingShopButton*>(new (std::nothrow) UIBitmapButton(*this->m_context, *this->m_controls));
  this->m_buyButton->SetBitmap("유저인터페이스\\styleshop\\btn_buy_out.bmp", 0, 0);
  this->m_buyButton->SetBitmap("유저인터페이스\\styleshop\\btn_buy_over.bmp", 1, 0);
  this->m_buyButton->SetBitmap("유저인터페이스\\styleshop\\btn_buy_down.bmp", 2, 0);
  this->m_buyButton->SetSize(this->m_buyButton->BitmapWidth(), this->m_buyButton->BitmapHeight());
  this->m_buyButton->Move(396, 306);
  this->m_buyButton->SetId(224);
  this->AddChild(this->m_buyButton);

  char zeny[256] = {};
  std::string text(LookupMessage(2773));
  text += " ";
  text += FormatGroupedInteger(g_session.m_zeny, zeny, 256);
  text += "\tz";

  UIStaticText* label = new (std::nothrow) UIStaticText(*this->m_context, *this->m_controls);
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
  CBitmapRes* shadow = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->unknown_007C)));
  if (shadow) {
    this->DrawBitmap(this->unknown_00B4, this->unknown_00B8, &shadow->image, false);
  }

  CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  if (!player) {
    return;
  }

  FuncDrawWNDChar draw(
    this, this->geometry.width / 2, this->geometry.height - 33, static_cast<unsigned char>(player->unknown_0260), static_cast<short>(player->GetEffectiveJob(-1)),
    player->unknown_04D4, static_cast<short>(this->job), static_cast<short>(this->unknown_008C), static_cast<short>(this->unknown_00A4),
    static_cast<short>(this->unknown_009C), static_cast<short>(this->unknown_00A0), this->unknown_0088, player->unknown_02CC, player->Slot56(),
    g_session.GetBodyStyle(player->GetEffectiveJob(-1), this->unknown_0098, this->job), this->unknown_0094, this->unknown_0084, 0, this->unknown_00A8
  );
  draw.Draw(false);
}
