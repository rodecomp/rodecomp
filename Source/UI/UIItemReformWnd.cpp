#include "UI/UIItemReformWnd.h"

#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdint>
#include <string>

// OFFSET: 0x005FCD10
// FUNCTION: fn_04107
void UIItemReformWnd_List::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x005FD600
// FUNCTION: fn_04111
void UIItemReformWnd_DetailBtn::Paint() {
  this->ClearSurface(0xFFFF00FF);
  std::string text = LookupMessage(3854);
  UIButtonContext& controls = *this->m_buttonContext;
  const unsigned int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 0, 0);
  this->DrawCenteredText(0, 0, text.c_str(), this->geometry.width, 0, 12, 0, 0);
  this->FillRegion((this->geometry.width - textWidth) * 0.5, 12, textWidth, 1, 0);
}

// OFFSET: 0x005FD770
// FUNCTION: fn_04112
void UIItemReformWnd_DetailMsg::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFF);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, this->geometry.height - 4, 0xC8C8C8);
  this->FillRegion(8, 8, this->geometry.width - 16, this->geometry.height - 16, 0xF7F7F7);
}

// OFFSET: 0x005FD7E0
// FUNCTION: fn_04113
void UIItemReformWnd_Item::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->item) {
    ResourceMgr& resources = ResourceMgr::Instance();
    const std::string name = FormatOwnedString("유저인터페이스\\item\\%s.bmp", ItemResourcePath(*this->item));
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(resources.Load(Resolve(name.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, true);
  }
}

// OFFSET: 0x005FDA60
// FUNCTION: fn_04117
void UIItemReformWnd_List::Slot31(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count && this->selectedIndex != index) {
    this->selectedIndex = index;
    if (this->parent) {
      this->parent->SendMsg(nullptr, 6, 336, reinterpret_cast<std::intptr_t>(&this->items[index]), 0, 0);
    }
    this->Invalidate();
  }
}

// OFFSET: 0x005FDB00
// FUNCTION: fn_04118
void UIItemReformWnd_DetailBtn::Slot47() {
  if (this->parent) {
    this->parent->SendMsg(this, 167, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDBE0
// FUNCTION: fn_04120
void UIItemReformWnd_List::Slot28(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count && this->hoverIndex != index) {
    this->hoverIndex = index;
    this->Invalidate();
  }
}

// OFFSET: 0x005FDC50
// FUNCTION: fn_04121
void UIItemReformWnd_DetailBtn::Slot48() {
  if (this->parent) {
    this->parent->SendMsg(this, 168, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDDA0
// FUNCTION: fn_04127
void UIItemReformWnd_List::Slot35(int delta) {
  if (delta < 0) {
    this->SendMsg(this->scrollBar, 10, 0, 0, 0, 0);
  } else {
    this->SendMsg(this->scrollBar, 9, 0, 0, 0, 0);
  }
}
