#include "UI/Window/FrameFocus.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

// STATIC: 0x014576A0
// GLOBAL: gl_24367
int g_dragWindowId = -1;

// OFFSET: 0x00A96A60
// FUNCTION: fn_17192
bool FrameFocus::IsOrderFixed(int id) {
  static constexpr int fixed[] = {0,   1,   2,   3,   4,   6,   7,   9,   14,  15,  16,  17,  18,  20,  21,  22,  23,    24,   26,  28,  29,  31,  32,
                                  33,  34,  35,  36,  39,  42,  45,  49,  50,  56,  58,  60,  61,  62,  63,  64,  65,    66,   67,  70,  73,  74,  78,
                                  79,  80,  81,  90,  91,  94,  95,  99,  100, 101, 104, 105, 107, 109, 111, 115, 116,   123,  124, 127, 128, 130, 131,
                                  133, 136, 137, 138, 144, 154, 156, 158, 159, 160, 161, 164, 175, 176, 177, 179, 180,   183,  186, 187, 189, 190, 196,
                                  198, 199, 200, 204, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 253,   257,  258, 262, 267, 268, 269,
                                  271, 277, 278, 284, 285, 286, 289, 295, 301, 307, 325, 332, 343, 358, 359, 600, 10011, 10032};
  if (id == 145) {
    UIWindow* window = this->windows.LookupWindow(145);
    return !window || window->geometry.visible == 0;
  }

  return std::binary_search(std::begin(fixed), std::end(fixed), id);
}

// OFFSET: 0x00A8F720
// FUNCTION: fn_17144
void FrameFocus::RemoveOrder(int id) {
  if (this->IsOrderFixed(id)) {
    return;
  }

  for (auto entry = this->order.begin(); entry != this->order.end(); ++entry) {
    if (*entry == id) {
      this->order.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00A95540
// FUNCTION: fn_17180
void FrameFocus::BringToFront(int id) {
  if (this->IsOrderFixed(id)) {
    return;
  }

  this->RemoveOrder(id);
  if (id == 203) {
    this->order.push_back(id);
  } else {
    this->order.push_front(id);
  }
}

// OFFSET: 0x00AA9B30
// FUNCTION: fn_17255
void FrameFocus::Set(UIWindow* window) {
  this->focused = window;
}

// OFFSET: 0x00AA66C0
// FUNCTION: fn_17232
void FrameFocus::Clear() {
  this->focused = nullptr;
}

// OFFSET: 0x00A913F0
// FUNCTION: fn_17158
UIWindow* FrameFocus::Get() const {
  return this->focused;
}
