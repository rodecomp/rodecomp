#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeEvents.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Script/LuaScript.h"
#include "UI/Window/FrameFocus.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowPolicy.h"

#include <cstring>

// OFFSET: 0x0084EDD0
// FUNCTION: fn_10653
UIButton::UIButton(UIWindowContext& windowContext, UIButtonContext& buttonContext)
    : UIWindow(windowContext, 0), m_disabled(0), m_argument(0), m_buttonContext(&buttonContext) {
  this->geometry.unknown_0030 = 0;
}

// OFFSET: 0x00851340
// FUNCTION: fn_10690
UIButton::~UIButton() = default;

// OFFSET: 0x0085C450
// FUNCTION: fn_10800
void UIButton::Paint() {
  if (this->m_drawLabel == 0) {
    return;
  }

  static const char* const backgrounds[] = {
    "유저인터페이스\\sml_button_up.bmp",
    "유저인터페이스\\sml_button_act.bmp",
    "유저인터페이스\\sml_button_down.bmp"
  };

  CBitmapRes* bitmap;

  switch (this->geometry.unknown_0030) {
  case 0:
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(backgrounds[0]));
    break;
  case 1:
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(backgrounds[1]));
    break;
  case 2:
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(backgrounds[2]));
    break;
  }

  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  this->DrawTextAt(this->m_buttonContext->fonts, this->m_buttonContext->locale, 0, 0, this->label.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0085EB20
// FUNCTION: fn_10839
void UIButton::Slot25(int, int) {
  if (this->m_disabled != 0) {
    return;
  }

  this->m_buttonContext->focus.Set(this);
  this->geometry.unknown_0030 = 2;
  this->Invalidate();
}

// OFFSET: 0x0085E8B0
// FUNCTION: fn_10830
void UIButton::Slot26(int x, int y) {
  this->Slot25(x, y);
}

// OFFSET: 0x0085F600
// FUNCTION: fn_10868
void UIButton::Slot27(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  unsigned state = 0;
  if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    state = this->m_buttonContext->focus.Get() == this ? 2 : 1;
  }

  if (this->geometry.unknown_0030 != state) {
    this->geometry.unknown_0030 = state;
    this->Invalidate();
  }

  UIWindow* parent = this->parent;
  if (parent == this->m_buttonContext->hoverParent) {
    parent->SendMsg(this, 6, 271, state != 0, 0, 0);
  }
}

// OFFSET: 0x0085F180
// FUNCTION: fn_10858
void UIButton::Slot28(int x, int y) {
  UIWindow* parent = this->parent;
  if (parent && (this->geometry.id == 201 || this->geometry.id == 202) && parent->geometry.id != 203) {
    if (this->geometry.id == 202 && parent->geometry.id != 0) {
      this->m_tooltip.clear();
    } else {
      const char* text = nullptr;
      if (this->m_buttonContext->behavior.Call("GetBeHaviorOfWindow", "d>s", parent->geometry.id, &text)) {
        this->m_tooltip.assign(text, std::strlen(text));
      }
    }
  }

  if (!this->m_tooltip.empty()) {
    int screenX = 0, screenY = -20;

    for (const UIWindow* window = this; window; window = window->parent) {
      screenX = (screenX + window->geometry.x);
      screenY = (screenY + window->geometry.y);
    }

    this->m_buttonContext->services.Tooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  }

  if (this->m_disabled == 0) {
    this->Slot27(x, y);
  }
}

// OFFSET: 0x0085F900
// FUNCTION: fn_10874
void UIButton::Slot29(int x, int y) {
  if (x < this->geometry.x || x > (this->geometry.x + this->geometry.width) || y < this->geometry.y || y > (this->geometry.y + this->geometry.height)) {
    if (this->geometry.unknown_0030 != 0) {
      this->geometry.unknown_0030 = 0;
      this->Invalidate();
    }
  }
}

// OFFSET: 0x0085F950
// FUNCTION: fn_10875
void UIButton::ResetCursor(int, int) {
  this->m_context->cursor.Set(this->m_disabled != 0 ? 0 : 2, this->m_context->cursorTick);
}

// OFFSET: 0x0085EED0
// OFFSET: 0x03DE9C61
// FUNCTION: fn_10852
void UIButton::Slot31(int x, int y) {
  FrameFocus& focus = this->m_buttonContext->focus;
  if (this->m_disabled != 0) {
    if (focus.Get() == this) {
      focus.Clear();
    }

    return;
  }

  if (focus.Get() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;
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
  }

  this->Invalidate();
  focus.Clear();
}

// OFFSET: 0x005FEAF0
// FUNCTION: fn_04132
void UIButton::Enable(unsigned char value) {
  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x00869730
// FUNCTION: fn_10967
void UIButton::SetTooltip(const char* text) {
  if (text) {
    this->m_tooltip = text;
  }
}
