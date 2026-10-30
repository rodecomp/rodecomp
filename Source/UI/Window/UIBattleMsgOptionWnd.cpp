#include "UI/Window/UIBattleMsgOptionWnd.h"

#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"

#include <cstring>
#include <windows.h>

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

// OFFSET: 0x0092B9C0
// FUNCTION: fn_12659
void UIBattleMsgOptionWnd::Paint() {
  auto& controls = *this->m_controls;

  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, this->text.c_str(), 0);
  this->DrawPanelBackground(titleHeight, 0, "");

  const int leftHeight = this->geometry.height - 42;
  const int leftCount = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int i = 0, y = 19; i < leftCount; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  const int rightHeight = this->geometry.height - 38;
  const int rightCount = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int i = 0, y = 17; i < rightCount; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(18, 17, 1, this->geometry.height - 21, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);

  int index = 0;
  int row = 1;
  int y = 22;
  for (std::vector<const char*>::iterator it = this->messages.begin(); it != this->messages.end(); ++it, ++index) {
    if (std::strcmp(LookupMessage(0x60D), *it) == 0 || index < this->unknown_00C4) {
      continue;
    }

    if (this->unknown_00B8 == index) {
      unsigned selectRed, selectGreen, selectBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, selectRed, selectGreen, selectBlue);
      this->FillRegion(25, y - 3, this->geometry.width - 40, 20, RGB(selectRed, selectGreen, selectBlue));
      this->FillRegion(26, y - 2, this->geometry.width - 42, 18, 0xFFFFFF);
    }

    this->DrawTextAt(controls.fonts, controls.locale, 50, y, *it, 0, 0, 12, 0, 0, 0);
    const bool online = this->unknown_00EC[index] != 0;
    auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(online ? "유저인터페이스\\basic_interface\\grp_online.bmp" : "유저인터페이스\\basic_interface\\grp_offline.bmp")
    ));
    this->DrawBitmap(27, y, &icon->image, true);

    if (row == this->m_visibleRows) {
      return;
    }
    y += 20;
    ++row;
  }
}
