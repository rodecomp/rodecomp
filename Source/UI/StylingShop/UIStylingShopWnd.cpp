#include "UI/StylingShop/UIStylingShopWnd.h"

#include "Client/Mode/ModeEvents.h"
#include "Client/Session.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowPolicy.h"

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
