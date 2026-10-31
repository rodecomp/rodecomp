#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UITab.h"

#include <cstdio>

// OFFSET: 0x0085DFC0
// FUNCTION: fn_10821
void UITab::Paint() {
  char name[128];

  if (this->style == 0) {
    std::sprintf(name, "유저인터페이스\\basic_interface\\tab_itm_%02d.bmp", this->geometry.unknown_0030 + 1);
  } else if (this->style == 1) {
    std::sprintf(name, "유저인터페이스\\basic_interface\\tab_cmd_%02d.bmp", this->geometry.unknown_0030 + 1);
  } else if (this->style == 2) {
    std::sprintf(name, "유저인터페이스\\basic_interface\\tab_que_%02d.bmp", this->geometry.unknown_0030 + 1);
  } else if (this->style == 3) {
    std::sprintf(name, "유저인터페이스\\basic_interface\\tab_item_%02d.bmp", this->geometry.unknown_0030 + 1);
  } else {
    return;
  }

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x0085EE70
// FUNCTION: fn_10851
void UITab::Slot25(int, int y) {
  if (!this->parent) {
    return;
  }

  int index = y / (this->geometry.height / this->tabCount);
  if (index < 0) {
    index = 0;
  }

  if (index > this->tabCount - 1) {
    index = this->tabCount - 1;
  }

  if (this->geometry.unknown_0030 != static_cast<std::uint32_t>(index)) {
    this->geometry.unknown_0030 = index;
    this->Invalidate();
    this->parent->SendMsg(this, 22, this->geometry.unknown_0030, 0, 0, 0);
  }
}

// OFFSET: 0x00865200
// FUNCTION: fn_10921
int UITab::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 13) {
    this->geometry.unknown_0030 = argument1;
    this->Invalidate();
  }

  return 0;
}

// OFFSET: 0x00850680
// FUNCTION: fn_10679
UITab::UITab(UIWindowContext& context) : UIWindow(context, 0) {
  this->geometry.unknown_0030 = 0;
  this->tabCount = 3;
  this->style = 0;
}
