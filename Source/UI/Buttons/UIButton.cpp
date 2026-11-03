#include "Client/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeEvents.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UIButton.h"

#include <cstring>

// OFFSET: 0x0084EDD0
// FUNCTION: fn_10653
UIButton::UIButton()
    : UIWindow(0), m_disabled(0), m_argument(0) {
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
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(backgrounds[0]));
    break;
  case 1:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(backgrounds[1]));
    break;
  case 2:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(backgrounds[2]));
    break;
  }

  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  this->DrawTextAt(0, 0, this->label.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0085EB20
// FUNCTION: fn_10839
void UIButton::Slot25(int, int) {
  if (this->m_disabled != 0) {
    return;
  }

  g_windowMgr.SetCapture(this);
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
    state = g_windowMgr.Capture() == this ? 2 : 1;
  }

  if (this->geometry.unknown_0030 != state) {
    this->geometry.unknown_0030 = state;
    this->Invalidate();
  }

  UIWindow* parent = this->parent;
  if (parent == g_windowMgr.unknown_01DC) {
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
      if (g_luaScript->Call("GetBeHaviorOfWindow", "d>s", parent->geometry.id, &text)) {
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

    static_cast<CMode*>(g_charViewMode)->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
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
  static_cast<CMode*>(g_charViewMode)->SetCursorAction(this->m_disabled != 0 ? 0 : 2);
}

// OFFSET: 0x0085EED0
// OFFSET: 0x03DE9C61
// FUNCTION: fn_10852
void UIButton::Slot31(int x, int y) {
  if (this->m_disabled != 0) {
    if (g_windowMgr.Capture() == this) {
      g_windowMgr.ReleaseCapture();
    }

    return;
  }

  if (g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;
    UIWindow* parent = this->parent;
    if (parent) {
      if (WindowPolicy(g_windowMgr.policy[0].actions, g_replayAnimationScaling != 0, 3, parent->geometry.id) != 0) {
        int id = this->geometry.id;
        if (id == 185 && g_charViewMode && static_cast<CMode*>(g_charViewMode)->cursorType == 2) {
          const char* text = LookupMessage(17);
          if (g_windowMgr.Confirm(text, 2, true, false, 0, "Message", 280, 120, 0) == 187) {
            static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
          }
        } else {
          if (id == 184 || id == 321) {
            parent->RecordEditTexts();
            id = this->geometry.id;
            parent = this->parent;
          }

          parent->SendMsg(this, 6, id, this->m_argument, 0, 0);
        }
      }
    } else {
      static_cast<CMode*>(g_charViewMode)->SendMsg(0, this->geometry.id, this->m_argument, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
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

// OFFSET: 0x00867BB0
// FUNCTION: fn_10947
void UIButton::SetState(int state) {
  this->geometry.unknown_0030 = state;
  this->Invalidate();
}
