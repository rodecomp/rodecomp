#include "UI/Menu/UIMenuIconWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Script/LuaScript.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstring>
#include <string>

// GLOBAL: gl_24966
static bool s_menuIconExpanded;

// OFFSET: 0x0084A240
// FUNCTION: fn_10607
UIMenuIconWnd::UIMenuIconWnd(UIWindowContext& windowContext, UIFrameContext& frameContext)
    : UIFrameWnd(windowContext, frameContext, 1), unknown_00CC(nullptr), unknown_00D0(nullptr), unknown_00D4(nullptr) {}

// OFFSET: 0x0084A850
// FUNCTION: fn_10612
bool UIMenuIconWnd::ToggleWindow(int id) {
  const bool closed = g_windowMgr.CloseWindow(id);
  if (!closed) {
    g_windowMgr.EnsureWindow(id);
  }

  return !closed;
}

// OFFSET: 0x0084A880
// FUNCTION: fn_10613
UIWindow* UIMenuIconWnd::HitTest(int x, int y) {
  if (!this->CanHitTest() || this->geometry.visible == 0 || x < this->geometry.x) {
    return nullptr;
  }

  if (!s_menuIconExpanded) {
    if (x >= this->geometry.x + this->geometry.width || y < this->geometry.y || y >= this->geometry.y + this->unknown_00D0->geometry.height) {
      return nullptr;
    }
    if (this->unknown_00C4.size() == 0) {
      return this->unknown_00CC;
    }
    return this->unknown_00D0;
  }

  if (x >= this->geometry.x + this->geometry.width || y < this->geometry.y || y >= this->geometry.y + this->geometry.height) {
    return nullptr;
  }

  for (auto child = this->m_children.rbegin(); child != this->m_children.rend(); ++child) {
    UIWindow* hit = (*child)->HitTest((x - this->geometry.x), (y - this->geometry.y));
    if (hit) {
      return hit;
    }
  }

  return this;
}

// OFFSET: 0x0084A960
// FUNCTION: fn_10614
bool UIMenuIconWnd::Slot08() {
  return true;
}

// OFFSET: 0x0084A970
// FUNCTION: fn_10615
void UIMenuIconWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y - 4;
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x0084A9A0
// FUNCTION: fn_10616
void UIMenuIconWnd::Slot15(int width, int height) {
  const int ids[24] = {192, 195, 194, 196, 379, 199, 373, 376, 361, 219, 430, 193, 461, 399, 476, 473, 511, 512, 370, 540, 544, 567, 581, 587};
  std::string names[24] = {
    "status", "equip", "item", "skill", "booking", "party", "guild", "battle", "quest", "map", "navigation", "option",
    "bank", "rec", "mail", "achievement", "tip", "shop", "keyboard", "attendance", "adventurerAgency", "repute", "adventureguide", "probability",
  };

  unsigned int position = 0;
  for (int index = 0; index < 24; ++index) {
    if (ids[index] == 192 && g_session.IsDoramJob(g_session.m_job) == true) {
      names[index] = "status_doram";
    }

    if (ids[index] == 379 || ids[index] == 600) {
      continue;
    }

    UIMenuIcon* icon = new UIMenuIcon();
    icon->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s.bmp", names[index].c_str()).c_str(), 0, 0);
    icon->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s.bmp", names[index].c_str()).c_str(), 1, 0);
    icon->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_press.bmp", names[index].c_str()).c_str(), 2, 0);
    icon->SetSize(icon->BitmapWidth(), icon->BitmapHeight());
    const int row = position / 5 * 43;
    const int x = position % 5 * 42 + 9;
    icon->Move(x, row + 23);
    icon->SetId(ids[index]);
    icon->Enable(0);
    this->AddChild(icon);
    this->unknown_00B4[ids[index]] = icon;

    switch (ids[index]) {
    case 192:
    case 194:
    case 196:
    case 473:
    case 476: {
      UIMenuIcon* badge = new UIMenuIcon();
      badge->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_new.bmp", names[index].c_str()).c_str(), 0, 0);
      badge->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_new.bmp", names[index].c_str()).c_str(), 1, 0);
      badge->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_new_press.bmp", names[index].c_str()).c_str(), 2, 0);
      badge->SetSize(badge->BitmapWidth(), badge->BitmapHeight());
      badge->Move(x, row + 17);
      badge->SetId(ids[index]);
      badge->Enable(0);
      this->AddChild(badge);
      this->unknown_00BC[ids[index]] = badge;
      break;
    }
    }

    ++position;
  }

  this->unknown_00CC = new UIBitmapButton();
  this->unknown_00CC->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_normal.bmp", 0, 0);
  this->unknown_00CC->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_over.bmp", 1, 0);
  this->unknown_00CC->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_press.bmp", 2, 0);
  this->unknown_00CC->SetSize(this->unknown_00CC->BitmapWidth(), this->unknown_00CC->BitmapHeight());
  this->unknown_00CC->Move(0, 4);
  this->unknown_00CC->SetId(510);
  this->unknown_00CC->Enable(0);
  this->AddChild(this->unknown_00CC);
  this->unknown_00D0 = new UIBitmapButton();
  this->unknown_00D0->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_new_normal.bmp", 0, 0);
  this->unknown_00D0->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_new_over.bmp", 1, 0);
  this->unknown_00D0->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_new_press.bmp", 2, 0);
  this->unknown_00D0->SetSize(this->unknown_00D0->BitmapWidth(), this->unknown_00D0->BitmapHeight());
  this->unknown_00D0->Move(0, 0);
  this->unknown_00D0->SetId(510);
  this->unknown_00D0->Enable(0);
  this->AddChild(this->unknown_00D0);
  this->unknown_00D4 = new UIBitmapButton();
  this->unknown_00D4->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_close_normal.bmp", 0, 0);
  this->unknown_00D4->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_close_over.bmp", 1, 0);
  this->unknown_00D4->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_close_press.bmp", 2, 0);
  this->unknown_00D4->SetSize(this->unknown_00D4->BitmapWidth(), this->unknown_00D4->BitmapHeight());
  this->unknown_00D4->Move(0, 4);
  this->unknown_00D4->SetId(201);
  this->unknown_00D4->Enable(0);
  this->AddChild(this->unknown_00D4);
  this->SendMsg(this, 6, s_menuIconExpanded == true ? 510 : 201, 0, 0, 0);
  if (this->unknown_00B4.size() > 20) {
    this->Resize(220, 238);
  }
}

// OFFSET: 0x0084B940
// FUNCTION: fn_10617
void UIMenuIconWnd::Paint() {
  this->m_children.clear();
  this->unknown_00CC->Enable(0);
  this->unknown_00D0->Enable(0);
  this->unknown_00D4->Enable(0);

  UIButton* toggle;
  if (s_menuIconExpanded) {
    toggle = this->unknown_00D4;
  } else if (this->unknown_00C4.size() == 0) {
    toggle = this->unknown_00CC;
  } else {
    toggle = this->unknown_00D0;
  }
  toggle->Enable(1);

  for (auto it = this->unknown_00B4.begin(); it != this->unknown_00B4.end(); ++it) {
    if (!s_menuIconExpanded) {
      it->second->Enable(0);
      if (this->unknown_00BC.find(it->first) != this->unknown_00BC.end()) {
        this->unknown_00BC[it->first]->Enable(0);
      }
    } else {
      this->DrawNamed(this->m_context->primitives, 0, 13, "유저인터페이스\\menu_icon\\bg_menu.tga", 0xFFFFFFFF, 0, 0);
      const bool pinned = std::find(this->unknown_00C4.begin(), this->unknown_00C4.end(), it->first) != this->unknown_00C4.end();
      it->second->Enable(!pinned);
      if (this->unknown_00BC.find(it->first) != this->unknown_00BC.end()) {
        this->unknown_00BC[it->first]->Enable(pinned);
      }
    }
  }
}

// OFFSET: 0x0084BD20
// FUNCTION: fn_10618
void UIMenuIconWnd::Slot25(int x, int y) {}

// OFFSET: 0x0084BD30
// FUNCTION: fn_10619
void UIMenuIconWnd::Slot31(int x, int y) {}

// OFFSET: 0x0084BD40
// FUNCTION: fn_10620
void UIMenuIcon::Slot27(int x, int y) {
  int behaviorId = 10041;
  int message = 4698;
  bool known = true;
  switch (this->geometry.id) {
  case 192:
    behaviorId = 11;
    message = 105;
    break;
  case 193:
    message = 265;
    break;
  case 194:
    behaviorId = 8;
    message = 106;
    break;
  case 195:
    behaviorId = 10;
    message = 104;
    break;
  case 196:
    behaviorId = 37;
    message = 283;
    break;
  case 199:
    behaviorId = 69;
    message = 103;
    break;
  case 219:
    behaviorId = 140;
    message = 2859;
    break;
  case 361:
    behaviorId = 134;
    message = 1317;
    break;
  case 370:
    message = 3128;
    break;
  case 373:
    behaviorId = 59;
    message = 3134;
    break;
  case 376:
    behaviorId = 157;
    message = 2003;
    break;
  case 399:
    behaviorId = 198;
    message = 2350;
    break;
  case 430:
    behaviorId = 203;
    message = 2353;
    break;
  case 461:
    behaviorId = 251;
    message = 3135;
    break;
  case 473:
    behaviorId = 270;
    message = 2646;
    break;
  case 476:
    behaviorId = 263;
    message = 3136;
    break;
  case 511:
    behaviorId = 315;
    message = 3239;
    break;
  case 512:
    behaviorId = 318;
    message = 3137;
    break;
  case 540:
    behaviorId = 328;
    message = 3473;
    break;
  case 544:
    behaviorId = 324;
    message = 3514;
    break;
  case 567:
    behaviorId = 346;
    message = 3827;
    break;
  case 581:
    behaviorId = 10002;
    message = 4053;
    break;
  case 587:
    message = 4119;
    break;
  default:
    known = false;
    break;
  }

  if (known) {
    const char* behavior = nullptr;
    g_luaScript->Call("GetBeHaviorOfWindow", "d>s", behaviorId, &behavior);
    if (std::strlen(behavior) == 0) {
      this->m_tooltip = LookupMessage(message);
    } else {
      const std::string text = FormatOwnedString("%s (%s)", LookupMessage(message), behavior);
      this->m_tooltip = text;
    }
  }

  if (this->geometry.id == 193) {
    const std::string text = FormatOwnedString("%s (%s)", LookupMessage(message), "Esc");
    this->m_tooltip = text;
  }
}

// OFFSET: 0x0084C250
// FUNCTION: fn_10621
void UIMenuIconWnd::Slot27(int x, int y) {}
