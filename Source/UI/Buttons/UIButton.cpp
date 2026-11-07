#include "Client/Replay.h"
#include "Client/Session.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeEvents.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UISpriteButton.h"

#include <cstring>

// OFFSET: 0x0084EDD0
// FUNCTION: fn_10653
UIButton::UIButton() : UIWindow(0) {
  this->geometry.unknown_0030 = 0;
  this->m_disabled = 0;
  this->m_argument = 0;
}

// OFFSET: 0x00851340
// FUNCTION: fn_10690
UIButton::~UIButton() = default;

// OFFSET: 0x00851C70
// FUNCTION: fn_10698
UISpriteButton::~UISpriteButton() = default;

// OFFSET: 0x0085C450
// FUNCTION: fn_10800
void UIButton::Paint() {
  if (this->m_drawLabel == 0) {
    return;
  }

  CBitmapRes* bitmap;

  switch (this->geometry.unknown_0030) {
  case 0:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sml_button_up.bmp")));
    break;
  case 1:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sml_button_act.bmp")));
    break;
  case 2:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sml_button_down.bmp")));
    break;
  }

  this->DrawBitmap(0, 0, bitmap, false);
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

// OFFSET: 0x0085ED70
// FUNCTION: fn_10847
void UISpriteButton2::Slot25(int x, int y) {
  UIButton::Slot25(x, y);
  if (this->m_target) {
    this->m_target->Slot25(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085EDC0
// FUNCTION: fn_10848
void UISpriteButton::Slot25(int x, int y) {
  UIButton::Slot25(x, y);
  this->Root()->Slot25(this->geometry.x + x, this->geometry.y + y);
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

  if (g_windowMgr.Capture() == this) {
    if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
      if (this->geometry.unknown_0030 != 2) {
        this->geometry.unknown_0030 = 2;
        this->Invalidate();
      }
    } else if (this->geometry.unknown_0030 != 0) {
      this->geometry.unknown_0030 = 0;
      this->Invalidate();
    }
  } else if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    if (this->geometry.unknown_0030 != 1) {
      this->geometry.unknown_0030 = 1;
      this->Invalidate();
    }
  } else if (this->geometry.unknown_0030 != 0) {
    this->geometry.unknown_0030 = 0;
    this->Invalidate();
  }

  UIWindow* parent = this->parent;
  if (parent == g_windowMgr.unknown_01DC) {
    parent->SendMsg(this, 6, 271, this->geometry.unknown_0030 != 0, 0, 0);
  }
}

// OFFSET: 0x0085F7A0
// FUNCTION: fn_10870
void UISpriteButton2::Slot27(int x, int y) {
  UIButton::Slot27(x, y);
  if (this->m_target) {
    this->m_target->Slot27(this->geometry.x + x, this->geometry.y + y);
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
      if (g_session.m_hotkeyScript->Call("GetBeHaviorOfWindow", "d>s", parent->geometry.id, &text)) {
        this->m_tooltip.assign(text, std::strlen(text));
      }
    }
  }

  if (!this->m_tooltip.empty()) {
    int screenX = 0, screenY = -20;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  }

  if (this->m_disabled == 0) {
    this->Slot27(x, y);
  }
}

// OFFSET: 0x0085F900
// FUNCTION: fn_10874
void UIButton::Slot29(int x, int y) {
  if (x < this->geometry.x || (this->geometry.x + this->geometry.width) < x || y < this->geometry.y || (this->geometry.y + this->geometry.height) < y) {
    if (this->geometry.unknown_0030 != 0) {
      this->geometry.unknown_0030 = 0;
      this->Invalidate();
    }
  }
}

// OFFSET: 0x0085F950
// FUNCTION: fn_10875
void UIButton::ResetCursor(int x, int y) {
  if (this->m_disabled != 0) {
    UIWindow::ResetCursor(x, y);
    return;
  }

  g_modeMgr.gameMode->SetCursorAction(2);
}

// OFFSET: 0x0085EED0
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
      if (g_windowMgr.WindowPolicy(3, parent->geometry.id) != 0) {
        int id = this->geometry.id;
        if (id == 185 && g_modeMgr.gameMode && g_modeMgr.gameMode->cursorType == 2) {
          const char* text = LookupMessage(17);
          if (g_windowMgr.Confirm(text, 2, true, false, 0, "Message", 280, 120, 0) == 187) {
            g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
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
      g_modeMgr.gameMode->SendMsg(0, this->geometry.id, this->m_argument, 0, 0);
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
