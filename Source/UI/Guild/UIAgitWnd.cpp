#include "UI/Guild/UIAgitWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/ReplayPacketRecorder.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>

// OFFSET: 0x0095D580
// FUNCTION: fn_13068
void UIAgitMessageBox::Paint() {
  auto& controls = this->m_dialog->buttons;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(3640), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x00961260
// FUNCTION: fn_13122
int UIAgitMessageBox::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    const int response = static_cast<int>(a);
    this->m_dialog->recorder.Record(this->geometry.id, response, b, c, d);
    if (response == 187) {
      g_modeMgr.GetGameMode()->SendMsg(315, this->m_agitId, 0, 0, 0);
      g_windowMgr.Schedule(this);
    } else if (response == 188) {
      g_windowMgr.Schedule(this);
    }
  }

  return 188;
}

// OFFSET: 0x0095D570
// FUNCTION: fn_13067
void UIListBoxAgit::Slot15(int width, int height) {
  UIListBox::Slot15(width, height);
}

// OFFSET: 0x00960090
// FUNCTION: fn_13092
void UIListBoxAgit::Slot25(int x, int y) {
  if (this->parent) {
    UIListBox::Slot25(x, y);
    this->parent->SendMsg(nullptr, 163, 0, 0, 0, 0);
  }
}

// OFFSET: 0x009604B0
// FUNCTION: fn_13106
void UIListBoxAgit::Slot32(int x, int y) {
  if (this->parent) {
    UIListBox::Slot25(x, y);
    if (this->selected < 0) {
      this->parent->SendMsg(nullptr, 163, 0, 0, 0, 0);
      return;
    }
    this->parent->SendMsg(nullptr, 162, this->geometry.width + this->geometry.x - 3, this->geometry.y - y % this->lineHeight + y + 2, 0, 0);
  }
}

// OFFSET: 0x0095F9E0
// FUNCTION: fn_13081
void UIListBoxAgitMenu::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  const auto* leftUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")));
  this->DrawBitmap(0, 0, &leftUp->image, false);
  const auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, &leftDown->image, false);
  const auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, &rightUp->image, false);
  const auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, &rightDown->image, false);

  SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
  auto& owner = *this->m_listContext;
  for (int index = 0; index < 2; ++index) {
    if (index == this->selected) {
      this->FillRegion(3, this->lineHeight * index + 1, this->geometry.width - 6, this->lineHeight - 2, RGB(red, green, blue));
    }
    this->DrawTextAt(owner.fonts, owner.locale, 3, this->lineHeight * index + 1, this->lines[index].c_str(), 0, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x009600D0
// FUNCTION: fn_13093
void UIListBoxAgitMenu::Slot25(int x, int y) {
  UIListBox::Slot25(x, y);
  this->SendMsg(nullptr, 48, this->selected, 0, 0, 0);
  if (this->parent) {
    this->parent->SendMsg(nullptr, 163, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00960160
// FUNCTION: fn_13096
void UIListBoxAgitMenu::Slot28(int x, int y) {
  const int previous = this->selected;
  int line;
  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    line = -1;
  } else {
    line = y / this->lineHeight;
    if (line < 0) {
      line = 0;
    }
    if (line > 1) {
      line = 1;
    }
  }

  this->selected = line;
  if (previous != line) {
    this->Invalidate();
  }
}
