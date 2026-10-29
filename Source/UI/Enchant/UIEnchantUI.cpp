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
// OFFSET: 0x009F99B0
// FUNCTION: fn_14812
CUIIngredient* CUIEnchantPage::CreateIngredient() {
  CUIIngredient* ingredient = new CUIIngredient(*this->m_context);
  ingredient->SetSize(70, 50);
  ingredient->Move(4, 200);
  this->AddChild(ingredient);
  return ingredient;
}

// OFFSET: 0x009F9B30
// FUNCTION: fn_14813
void CUIEnchantPage::Slot54(int argument1, int argument2) {
}

// OFFSET: 0x00A02D80
// FUNCTION: fn_14845
void CUIEnchantPage::Slot59(int value) {
  this->unknown_00B0 = value;
}

// OFFSET: 0x00A05190
// FUNCTION: fn_14970
void CUIUpgradeEnchantPage::Slot58(int value) {
  if (CUIMainPage* mainPage = dynamic_cast<CUIMainPage*>(this->parent)) {
    mainPage->firstPage->Slot59(value);
    mainPage->secondPage->Slot59(value);
  }
}

// OFFSET: 0x00A05CE0
// FUNCTION: fn_14980
void CUISlot::Paint() {
  this->ClearSurface(0xFFFF00FF);
}
}

// OFFSET: 0x00A05DF0
// FUNCTION: fn_14983
void CUIEnchantUI::Slot16() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
}

namespace EnchantUI {
// OFFSET: 0x00A06050
// FUNCTION: fn_14988
void CUISlot0::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(0);
}

// OFFSET: 0x00A06090
// FUNCTION: fn_14989
void CUISlot1::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(1);
}

// OFFSET: 0x00A060D0
// FUNCTION: fn_14990
void CUISlot2::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(2);
}

// OFFSET: 0x00A06110
// FUNCTION: fn_14991
void CUISlot3::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(3);
}

// OFFSET: 0x00A06150
// FUNCTION: fn_14992
void CUISlot::Slot16() {
  this->itemIcon->hitEnabled = 0;
  this->description->scrollBar->SetHidden(1);
}
}

// OFFSET: 0x00A06190
// FUNCTION: fn_14994
void CUIEnchantUI::Slot25(int x, int y) {
  if (this->titleBar->IsPointInside(x, y)) {
    CUIFrameWnd::Slot25(x, y);
  }
}

namespace EnchantUI {
// OFFSET: 0x00A061C0
// FUNCTION: fn_14995
void CUIEnchantItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A061F0
// FUNCTION: fn_14996
void CUITargetItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06220
// FUNCTION: fn_14997
void CUIEnchantItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06250
// FUNCTION: fn_14998
void CUISlot::Slot32(int x, int y) {
  if (this->itemIcon->IsPointInside(x, y)) {
    this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
  }
}

// OFFSET: 0x00A06290
// FUNCTION: fn_14999
void CUITargetItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A062C0
// FUNCTION: fn_15000
void CUISlot::SelectSlot(UIWindow* button) {
  if (CUIEnchantPage* page = dynamic_cast<CUIEnchantPage*>(this->parent)) {
    page->Slot58(button->geometry.id);
  }
}

// OFFSET: 0x00A06A50
// FUNCTION: fn_15005
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
