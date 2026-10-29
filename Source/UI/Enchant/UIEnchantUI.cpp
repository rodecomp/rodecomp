#include "UI/Enchant/EnchantUI.h"
#include "Render/Renderer.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UITextViewerControl.h"

template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Paint();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Paint();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Slot35(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Slot35(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearItems();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::ClearItems();
template int CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::AddData(const EnchantUI::CEnchantItemData&, bool);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearData();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::UpdateScrollBar();

namespace EnchantUI {
// OFFSET: 0x00A05CE0
// FUNCTION: fn_14980
void CUISlot::Paint() {
  this->ClearSurface(0xFFFF00FF);
}
}

// OFFSET: 0x00A05DF0
void CUIEnchantUI::Slot16() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
}

namespace EnchantUI {
// OFFSET: 0x00A06050
void CUISlot0::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(0);
}

// OFFSET: 0x00A06090
void CUISlot1::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(1);
}

// OFFSET: 0x00A060D0
void CUISlot2::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(2);
}

// OFFSET: 0x00A06110
void CUISlot3::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(3);
}

// OFFSET: 0x00A06150
void CUISlot::Slot16() {
  this->itemIcon->hitEnabled = 0;
  this->description->scrollBar->SetHidden(1);
}
}

// OFFSET: 0x00A06190
void CUIEnchantUI::Slot25(int x, int y) {
  if (this->titleBar->IsPointInside(x, y)) {
    CUIFrameWnd::Slot25(x, y);
  }
}

namespace EnchantUI {
// OFFSET: 0x00A061C0
void CUIEnchantItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A061F0
void CUITargetItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06220
void CUIEnchantItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06250
void CUISlot::Slot32(int x, int y) {
  if (this->itemIcon->IsPointInside(x, y)) {
    this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
  }
}

// OFFSET: 0x00A06290
void CUITargetItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06A50
void CUITargetItem::SetSelected(bool selected) {
  if (selected) {
    if (this->selected != 1) {
      this->itemIcon->Move(this->itemIcon->geometry.x + 1, this->itemIcon->geometry.y + 1);
      this->unknown_00EC->Move(this->unknown_00EC->geometry.x + 1, this->unknown_00EC->geometry.y + 1);
      this->unknown_00F0->Move(this->unknown_00F0->geometry.x + 1, this->unknown_00F0->geometry.y + 1);
    }
  } else if (this->selected == 1) {
    this->itemIcon->Move(this->itemIcon->geometry.x - 1, this->itemIcon->geometry.y - 1);
    this->unknown_00EC->Move(this->unknown_00EC->geometry.x - 1, this->unknown_00EC->geometry.y - 1);
    this->unknown_00F0->Move(this->unknown_00F0->geometry.x - 1, this->unknown_00F0->geometry.y - 1);
  }
  CUIListItem::SetSelected(selected);
}
}
