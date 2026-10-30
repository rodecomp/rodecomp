#include "UI/Window/UIBattleMsgOptionWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Lists/UIScrollBar.h"

#include <cstring>

// OFFSET: 0x009335E0
// FUNCTION: fn_12726
void UIBattleMsgOptionWnd::Slot35(int delta) {
  if (this->m_scrollable != 0) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}

// OFFSET: 0x00910C40
// FUNCTION: fn_12510
UIBattleMsgOptionWnd::UIBattleMsgOptionWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->unknown_00B8 = -1;
  this->messages.push_back(LookupMessage(0x50B));

  this->unknown_0108 = -1;
  this->unknown_0110 = 1;
  this->unknown_010C = 0;

  this->messages.push_back(LookupMessage(0x4FD));
  this->messages.push_back(LookupMessage(0x4FE));
  this->messages.push_back(LookupMessage(0x4FF));
  this->messages.push_back(LookupMessage(0x500));
  this->messages.push_back(LookupMessage(0xFBF));
  this->messages.push_back(LookupMessage(0x501));
  this->messages.push_back(LookupMessage(0x502));
  this->messages.push_back(LookupMessage(0x503));
  this->messages.push_back(LookupMessage(0x504));
  this->messages.push_back(LookupMessage(0x505));
  this->messages.push_back(LookupMessage(0x506));
  this->messages.push_back(LookupMessage(0x507));
  this->messages.push_back(LookupMessage(0x508));
  this->messages.push_back(LookupMessage(0x533));
  this->messages.push_back(LookupMessage(0x60D));
  this->messages.push_back(LookupMessage(0x652));
  this->messages.push_back(LookupMessage(0x653));
  this->messages.push_back(LookupMessage(0x654));
  this->messages.push_back(LookupMessage(0x656));
  this->messages.push_back(LookupMessage(0x657));
  this->messages.push_back(LookupMessage(0x93D));
  this->messages.push_back(LookupMessage(0xBA4));
  this->messages.push_back(LookupMessage(0xFBC));
  this->messages.push_back(LookupMessage(0xFBE));

  this->text = LookupMessage(0x509);
}

// OFFSET: 0x0091BA20
// FUNCTION: fn_12606
int UIBattleMsgOptionWnd::RowAt(int x, int y) {
  if (x >= 0 && y >= 17 && y < this->m_visibleRows * 20 + 17) {
    const int column = x / this->geometry.width;
    int row = 0;
    int index = 0;
    for (std::vector<const char*>::iterator it = this->messages.begin(); it != this->messages.end(); ++it, ++index) {
      if (std::strcmp(LookupMessage(0x60D), *it) != 0 && index >= this->unknown_00C4) {
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
