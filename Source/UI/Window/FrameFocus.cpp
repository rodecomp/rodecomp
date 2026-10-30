#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIDropDownBox.h"
#include <algorithm>
#include "Client/Session.h"
// STATIC: 0x014576A0
// GLOBAL: gl_24367
int g_dragWindowId = -1;

// OFFSET: 0x00A96A60
// FUNCTION: fn_17192
bool UIWindowMgr::IsOrderFixed(int id) {
  static constexpr int fixed[] = {0,   1,   2,   3,   4,   6,   7,   9,   14,  15,  16,  17,  18,  20,  21,  22,  23,    24,   26,  28,  29,  31,  32,
                                  33,  34,  35,  36,  39,  42,  45,  49,  50,  56,  58,  60,  61,  62,  63,  64,  65,    66,   67,  70,  73,  74,  78,
                                  79,  80,  81,  90,  91,  94,  95,  99,  100, 101, 104, 105, 107, 109, 111, 115, 116,   123,  124, 127, 128, 130, 131,
                                  133, 136, 137, 138, 144, 154, 156, 158, 159, 160, 161, 164, 175, 176, 177, 179, 180,   183,  186, 187, 189, 190, 196,
                                  198, 199, 200, 204, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 253,   257,  258, 262, 267, 268, 269,
                                  271, 277, 278, 284, 285, 286, 289, 295, 301, 307, 325, 332, 343, 358, 359, 600, 10011, 10032};
  if (id == 145) {
    UIWindow* window = g_windowMgr.LookupWindow(145);
    return !window || window->geometry.visible == 0;
  }

  return std::binary_search(std::begin(fixed), std::end(fixed), id);
}

// OFFSET: 0x00A8F720
// FUNCTION: fn_17144
void UIWindowMgr::RemoveOrder(int id) {
  if (this->IsOrderFixed(id)) {
    return;
  }

  for (auto entry = this->windowHistory.begin(); entry != this->windowHistory.end(); ++entry) {
    if (*entry == id) {
      this->windowHistory.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00A95540
// FUNCTION: fn_17180
void UIWindowMgr::BringToFront(int id) {
  if (this->IsOrderFixed(id)) {
    return;
  }

  this->RemoveOrder(id);
  if (id == 203) {
    this->windowHistory.push_back(id);
  } else {
    this->windowHistory.push_front(id);
  }
}

// OFFSET: 0x00AA9B30
// FUNCTION: fn_17255
void UIWindowMgr::SetCapture(UIWindow* window) {
  this->inputWindow = window;
}

// OFFSET: 0x00AA66C0
// FUNCTION: fn_17232
void UIWindowMgr::ReleaseCapture() {
  this->inputWindow = nullptr;
}

// OFFSET: 0x00A913F0
// FUNCTION: fn_17158
UIWindow* UIWindowMgr::Capture() const {
  return this->inputWindow;
}

// OFFSET: 0x00A96A40
// FUNCTION: fn_17191
bool UIWindowMgr::IsDropDownChild(const UIWindow* window) {
  if (!this->window_0A30 || !this->window_0A30->geometry.visible) {
    return false;
  }

  return static_cast<CUIDropDownBox*>(this->window_0A30)->HasChild(window);
}

// OFFSET: 0x00A96E60
// FUNCTION: fn_17194
bool UIWindowMgr::IsHoverParentVisible() {
  if (this->hoverWindow) {
    UIWindow* parent = this->hoverWindow->parent;
    if (parent && parent != this->window_01C8 && parent->geometry.visible) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00A96E90
// FUNCTION: fn_17195
bool UIWindowMgr::IsFriend(const char* name) {
  return g_session.IsFriendName(name) != 0;
}

// OFFSET: 0x00A96EB0
// FUNCTION: fn_17196
bool UIWindowMgr::IsSharedWindowId(int id) {
  switch (id) {
  case 9:
  case 13:
  case 16:
  case 17:
  case 18:
  case 20:
  case 45:
  case 112:
  case 113:
  case 114:
  case 115:
  case 116:
  case 117:
  case 118:
  case 119:
  case 120:
  case 121:
  case 122:
  case 123:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00A97070
// FUNCTION: fn_17198
bool UIWindowMgr::HasNavigationBlocker() {
  return this->window_4F260 || this->window_4F264 || this->window_4F268 || this->window_4F26C;
}

// OFFSET: 0x00A97410
// FUNCTION: fn_17203
bool UIWindowMgr::HasPendingDialog() {
  return this->unknown_0284 || this->unknown_02CC || this->unknown_02D0 || this->unknown_02D4 || this->unknown_02D8 || this->unknown_02DC ||
         this->unknown_02E0 || this->unknown_02E4 || this->unknown_02E8 || this->unknown_02EC || this->unknown_03B4 || this->unknown_01F0 ||
         this->unknown_01F4;
}

// OFFSET: 0x00A97490
// FUNCTION: fn_17204
void UIWindowMgr::CancelActiveDialog() {
  UIWindow* window = this->window_03DC;
  if (window || (window = this->unknown_03B0) != nullptr) {
    window->SendMsg(nullptr, 20, 0, 0, 0, 0);
    return;
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    if ((*entry)->Slot02()) {
      (*entry)->SendMsg(nullptr, 20, 0, 0, 0, 0);
    }
  }
}
