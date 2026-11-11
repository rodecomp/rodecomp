#include "UI/Enchant/EnchantUI.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Items/EnchantMgr.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <iterator>
#include <new>

// GLOBAL: gl_24882
template <>
CEnchantMgr* Singleton<CEnchantMgr>::s_instance = nullptr;

// OFFSET: 0x009F9B40
// FUNCTION: fn_14814
// OFFSET: 0x009F9C10
// FUNCTION: fn_14815
template <class T, class D> void CUIListBoxEx<T, D>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnCreate(int, int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnCreate(int, int);

template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Paint();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Paint();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnWheel(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnWheel(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearItems();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::ClearItems();
template int CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::AddData(const EnchantUI::CEnchantItemData&, bool);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearData();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::UpdateScrollBar();

template <>
__declspec(noinline) void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::AddNewItem();
template <>
__declspec(noinline) void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::AddNewItem();
template <>
__declspec(noinline) CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::CUIListBoxEx();

// OFFSET: 0x009F93D0
// FUNCTION: fn_14806
template <>
EnchantUI::CUIEnchantItem* CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::CreateItem() {
  EnchantUI::CUIEnchantItem* item = new EnchantUI::CUIEnchantItem();
  item->SetSize(this->itemWidth, this->rowHeight);
  item->Show(0);
  item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  item->onHover = [this](CUIListItem* listItem) {
    for (EnchantUI::CUIEnchantItem* other : this->items) {
      other->SetHighlighted(other == listItem);
    }
  };
  item->onPress = [this](CUIListItem* listItem) {
    if (this->layoutFlag) {
      return;
    }
    for (EnchantUI::CUIEnchantItem* other : this->items) {
      if (listItem != other) {
        other->SetSelected(0);
      }
    }
    listItem->SetSelected(1);
    if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
      return;
    }
    EnchantUI::CUIEnchantItem* selectedItem = dynamic_cast<EnchantUI::CUIEnchantItem*>(listItem);
    if (!selectedItem) {
      return;
    }
    this->selected = selectedItem->index;
    if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
      auto node = this->data.begin();
      std::advance(node, static_cast<int>(this->selected));
      this->onSelect(selectedItem, *node);
    }
  };
  this->AddChild(item);
  return item;
}

// OFFSET: 0x00A01FB0
// FUNCTION: fn_14836
template <>
void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (EnchantUI::CUIEnchantItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (EnchantUI::CUIEnchantItem*& slot : this->items) {
      slot = this->CreateItem();
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A022A0
// FUNCTION: fn_14839
template <>
void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (EnchantUI::CUIEnchantItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A026C0
// FUNCTION: fn_14841
template <>
void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x009F9570
// FUNCTION: fn_14807
template <>
EnchantUI::CUITargetItem* CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::CreateItem() {
  EnchantUI::CUITargetItem* item = new EnchantUI::CUITargetItem();
  item->SetSize(this->itemWidth, this->rowHeight);
  item->Show(0);
  item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  item->onHover = [this](CUIListItem* listItem) {
    for (EnchantUI::CUITargetItem* other : this->items) {
      other->SetHighlighted(other == listItem);
    }
  };
  item->onPress = [this](CUIListItem* listItem) {
    if (this->layoutFlag) {
      return;
    }
    for (EnchantUI::CUITargetItem* other : this->items) {
      if (listItem != other) {
        other->SetSelected(0);
      }
    }
    listItem->SetSelected(1);
    if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
      return;
    }
    EnchantUI::CUITargetItem* selectedItem = dynamic_cast<EnchantUI::CUITargetItem*>(listItem);
    if (!selectedItem) {
      return;
    }
    this->selected = selectedItem->index;
    if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
      auto node = this->data.begin();
      std::advance(node, static_cast<int>(this->selected));
      this->onSelect(selectedItem, *node);
    }
  };
  this->AddChild(item);
  return item;
}

// OFFSET: 0x00A020C0
// FUNCTION: fn_14837
template <>
void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (EnchantUI::CUITargetItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (EnchantUI::CUITargetItem*& slot : this->items) {
      slot = this->CreateItem();
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A024B0
// FUNCTION: fn_14840
template <>
void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (EnchantUI::CUITargetItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A02740
// FUNCTION: fn_14842
template <>
void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x006863F0
// FUNCTION: fn_05565
template <>
CEnchantMgr* Singleton<CEnchantMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CEnchantMgr;
  }

  return s_instance;
}

// OFFSET: 0x00A04860
// FUNCTION: fn_14958
CUIEnchantUI::~CUIEnchantUI() {
  CEnchantMgr::GetInstance()->SendCloseEnchant();
}

namespace EnchantUI {
// OFFSET: 0x009F70C0
// FUNCTION: fn_14768
CUIEnchantPage::CUIEnchantPage()
    : CUIControl(), backgroundStorage{}, gameItem(nullptr), slots(), ingredients(), enchantList(nullptr), costText{}, enchantInfo(nullptr),
      group(0), selectedSlot(-1) {
}

// OFFSET: 0x009F72F0
// FUNCTION: fn_14770
CUIMainPage::CUIMainPage() : CUIControl(), firstPage(nullptr), secondPage(nullptr), tab(nullptr) {
}
}

// OFFSET: 0x009F7380
// FUNCTION: fn_14771
CUITab::CUITab() : CUIControl(), current(nullptr) {
}

// OFFSET: 0x009F7420
// FUNCTION: fn_14772
CUITabButton::CUITabButton()
    : CUIImageTextButton(), page(nullptr), useSkinColors(0), normalColor(0xFFFFFF), selectedColor(0xFFFFFF), borderColor(0xCECECE), normalCell{0, 0},
      selectedCell{0, 0}, borderCell{3, 1} {
}

// OFFSET: 0x009F74E0
// FUNCTION: fn_14773
template <>
CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::~CUIListBoxEx() {
}

namespace EnchantUI {
// OFFSET: 0x009F9CE0
// FUNCTION: fn_14816
void CUIEnchantItem::OnCreate(int width, int height) {
  this->itemIcon = new CUIGameItem();
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(2, 3);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->AddChild(this->itemIcon);

  this->name = new CUIStaticText();
  this->name->SetSize(101, 12);
  this->name->Move(28, 9);
  this->name->SetText("");
  this->name->SetFitText(1);
  this->name->SetAlign(0, 6);
  {
    // GLOBAL: gl_25222
    this->name->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x39FF7FB50DEB998FULL)));
  }
  this->name->SetFontHeight(12);
  this->name->SetBold(1);
  this->name->SetColor(0xC89682);
  this->name->SetShadow(0, 0xFFFFFF);
  this->name->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->name);
}

// OFFSET: 0x00A00880
// FUNCTION: fn_14828
void CUITargetItem::OnCreate(int width, int height) {
  this->itemIcon = new CUIGameItem();
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(6, 13);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->AddChild(this->itemIcon);

  this->targetIcon = new CUIStaticImage();
  this->targetIcon->SetSize(12, 12);
  this->targetIcon->Move(166, 7);
  {
    // GLOBAL: gl_25223
    this->targetIcon->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\grade_enchant\\grade_icon1.bmp", 0x75ADC57D87EFBD8FULL)));
  }
  this->AddChild(this->targetIcon);

  this->targetText = new CUIStaticText();
  this->targetText->SetSize(152, 12);
  this->targetText->Move(38, 23);
  this->targetText->SetText("");
  this->targetText->SetFitText(1);
  this->targetText->SetAlign(0, 0);
  {
    // GLOBAL: gl_25224
    this->targetText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xC517FB932D95BB6DULL)));
  }
  this->targetText->SetFontHeight(12);
  this->targetText->SetBold(1);
  this->targetText->SetColor(0x959595);
  this->targetText->SetShadow(0, 0xFFFFFF);
  this->targetText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->targetText);
}

// OFFSET: 0x009F9880
// FUNCTION: fn_14809
void CUIEnchantPage::OnSelectSlot(int) {
}

// OFFSET: 0x009F99B0
// FUNCTION: fn_14812
CUIIngredient* CUIEnchantPage::CreateIngredient() {
  CUIIngredient* ingredient = new CUIIngredient();
  ingredient->SetSize(70, 50);
  ingredient->Move(4, 200);
  this->AddChild(ingredient);
  return ingredient;
}

// OFFSET: 0x009FB800
// FUNCTION: fn_14818
void CUIIngredient::OnCreate(int width, int height) {
  this->backgroundImage = new CUIStaticImage();
  this->backgroundImage->SetSize(70, 50);
  this->backgroundImage->Move(0, 0);
  {
    // GLOBAL: gl_25225
    this->backgroundImage->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enc_off.bmp", 0x050505679903579BULL)));
  }
  this->AddChild(this->backgroundImage);

  this->sufficientMaterialImage = new CUIStaticImage();
  this->sufficientMaterialImage->SetSize(70, 50);
  this->sufficientMaterialImage->Move(0, 0);
  {
    // GLOBAL: gl_25226
    this->sufficientMaterialImage->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enc_on.bmp", 0xDD3913D5D7F97BE1ULL)));
  }
  this->AddChild(this->sufficientMaterialImage);

  this->itemIcon = new CUIGameItem();
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(6, 10);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->AddChild(this->itemIcon);

  this->ownedText = new CUIStaticText();
  this->ownedText->SetSize(40, 14);
  this->ownedText->Move(30, 17);
  this->ownedText->SetText("");
  this->ownedText->SetFitText(0);
  this->ownedText->SetAlign(6, 6);
  {
    // GLOBAL: gl_25227
    this->ownedText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xBF2B4B31397FF5A7ULL)));
  }
  this->ownedText->SetFontHeight(12);
  this->ownedText->SetBold(1);
  this->ownedText->SetColor(0xF0F0F0);
  this->ownedText->SetShadow(0, 0xFFFFFF);
  this->ownedText->SetOutline(1, 0x965532);
  this->AddChild(this->ownedText);

  this->requiredText = new CUIStaticText();
  this->requiredText->SetSize(40, 14);
  this->requiredText->Move(30, 31);
  this->requiredText->SetText("");
  this->requiredText->SetFitText(0);
  this->requiredText->SetAlign(6, 6);
  {
    // GLOBAL: gl_25228
    this->requiredText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x83EFBFBB37EDBDBDULL)));
  }
  this->requiredText->SetFontHeight(12);
  this->requiredText->SetBold(1);
  this->requiredText->SetColor(0xF0F0F0);
  this->requiredText->SetShadow(0, 0xFFFFFF);
  this->requiredText->SetOutline(1, 0x965532);
  this->AddChild(this->requiredText);
}

// OFFSET: 0x009F9B30
// FUNCTION: fn_14813
void CUIEnchantPage::OnSelectEnchant(CUIEnchantItem* item, const CEnchantItemData& data) {
}
}

// OFFSET: 0x009F9710
// FUNCTION: fn_14808
CUIEnchantUI* CreateEnchantUI() {
  return new CUIEnchantUI();
}

// OFFSET: 0x00492010
// FUNCTION: fn_45547
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateEnchantUI;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10006, factory);
  return true;
}();

namespace EnchantUI {
// OFFSET: 0x009FD340
// FUNCTION: fn_14823
void CUISlot0::OnCreate(int width, int height) {
  CUISlot::OnCreate(width, height);
  this->radioButton->Move(59, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->disabled = 0;
    radioButton->Invalidate();
  }
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->checked = false;
    radioButton->Invalidate();
  }
  this->itemIcon->Move(69, 11);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->description->Move(0, 44);
  this->description->SetTextOffset(2, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

// OFFSET: 0x009FDC30
// FUNCTION: fn_14824
void CUISlot1::OnCreate(int width, int height) {
  CUISlot::OnCreate(width, height);
  this->radioButton->Move(59, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->disabled = 0;
    radioButton->Invalidate();
  }
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->checked = false;
    radioButton->Invalidate();
  }
  this->itemIcon->Move(69, 11);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->description->Move(0, 44);
  this->description->SetTextOffset(2, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

// OFFSET: 0x009FE520
// FUNCTION: fn_14825
void CUISlot2::OnCreate(int width, int height) {
  CUISlot::OnCreate(width, height);
  this->radioButton->Move(3, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->disabled = 0;
    radioButton->Invalidate();
  }
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->checked = false;
    radioButton->Invalidate();
  }
  this->itemIcon->Move(13, 11);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->description->Move(10, 44);
  this->description->SetTextOffset(0, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

// OFFSET: 0x009FEE10
// FUNCTION: fn_14826
void CUISlot3::OnCreate(int width, int height) {
  CUISlot::OnCreate(width, height);
  this->radioButton->Move(3, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->disabled = 0;
    radioButton->Invalidate();
  }
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  {
    CUIRadioButton* radioButton = this->radioButton;
    radioButton->checked = false;
    radioButton->Invalidate();
  }
  this->itemIcon->Move(13, 11);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->description->Move(10, 44);
  this->description->SetTextOffset(0, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

}

namespace EnchantUI {
// OFFSET: 0x009FC080
// FUNCTION: fn_14819
void CUINormal::OnCreate(int width, int height) {
  this->firstPage = new CUINormalRandom;
  this->firstPage->SetSize(470, 377);
  this->firstPage->Move(0, 0);
  this->AddChild(this->firstPage);

  this->secondPage = new CUINormalPerfect;
  this->secondPage->SetSize(470, 377);
  this->secondPage->Move(0, 0);
  this->AddChild(this->secondPage);

  this->tab = new CUITab;
  this->tab->SetSize(76, 20);
  this->tab->Move(7, 1);
  this->AddChild(this->tab);

  CUITabButton* firstButton = new CUITabButton;
  firstButton->SetSize(38, 20);
  firstButton->Move(0, 0);
  firstButton->disabled = 0;
  firstButton->Invalidate();
  {
    // GLOBAL: gl_25233
    // GLOBAL: gl_25232
    // GLOBAL: gl_25231
    // GLOBAL: gl_25230
    firstButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_out.bmp", 0xF3CB6F7711A71B6DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_pressed.bmp", 0x839B27DF8F5967B3ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_out.bmp", 0x9FED7DA189490D5DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_disable.bmp", 0x452FEF0D7FC95565ULL)));
  }
  this->tab->AddChild(firstButton);

  CUITabButton* secondButton = new CUITabButton;
  secondButton->SetSize(38, 20);
  secondButton->Move(38, 0);
  secondButton->disabled = 0;
  secondButton->Invalidate();
  {
    // GLOBAL: gl_25237
    // GLOBAL: gl_25236
    // GLOBAL: gl_25235
    // GLOBAL: gl_25234
    secondButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_out.bmp", 0x176F138D5DD7F365ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_pressed.bmp", 0xFBD599E117C547C1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_out.bmp", 0x0F41EBBDA32D1B17ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_disable.bmp", 0x2D51E3B7236175EDULL)));
  }
  this->tab->AddChild(secondButton);

  firstButton->page = this->firstPage;
  secondButton->page = this->secondPage;
  this->tab->Select(firstButton);
}

// OFFSET: 0x009FF700
// FUNCTION: fn_14827
void CUISlot::OnCreate(int width, int height) {
  this->radioButton = new CUIRadioButton();
  this->radioButton->SetSize(44, 44);
  this->radioButton->Move(59, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  this->radioButton->disabled = 0;
  this->radioButton->Invalidate();
  {
    // GLOBAL: gl_25192
    // GLOBAL: gl_25191
    // GLOBAL: gl_25190
    // GLOBAL: gl_25189
    this->radioButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_socket_out.bmp", 0x23BBD58F8B5FF5C9ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_socket_select.bmp", 0x0B7109397F6B1387ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_socket_over.bmp", 0x03772763F96D9DA5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_socket_out.bmp", 0x17A9E5BF39AD4737ULL)));
  }
  this->radioButton->checked = false;
  this->radioButton->Invalidate();
  this->AddChild(this->radioButton);

  this->itemIcon = new CUIGameItem();
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(69, 11);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->AddChild(this->itemIcon);

  this->description = new CUITextViewer();
  this->description->SetSize(96, 39);
  this->description->Move(0, 44);
  this->description->SetTextOffset(2, 0);
  {
    // GLOBAL: gl_25193
    this->description->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x13E33FA1A591AB67ULL)));
  }
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  {
    // GLOBAL: gl_25195
    // GLOBAL: gl_25194
    this->description->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0up.bmp", 0xCD8F077123FB7D6FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0down.bmp", 0x9949BFD527532943ULL)));
  }
  {
    // GLOBAL: gl_25196
    this->description->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0mid.bmp", 0xED796785D1DF137FULL)));
  }
  {
    // GLOBAL: gl_25199
    // GLOBAL: gl_25198
    // GLOBAL: gl_25197
    this->description->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_up.bmp", 0xD97D5DBDBB2F2709ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_mid.bmp", 0xA5FD29D7AB6DC59FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_down.bmp", 0xF14F79170939C7F1ULL)));
  }
  this->AddChild(this->description);
}

// OFFSET: 0x00A00D50
// FUNCTION: fn_14829
void CUIUpgrade::OnCreate(int width, int height) {
  this->firstPage = new CUIUpgradeRandom;
  this->firstPage->SetSize(470, 377);
  this->firstPage->Move(0, 0);
  this->AddChild(this->firstPage);

  this->secondPage = new CUIUpgradePerfect;
  this->secondPage->SetSize(470, 377);
  this->secondPage->Move(0, 0);
  this->AddChild(this->secondPage);

  this->tab = new CUITab;
  this->tab->SetSize(76, 20);
  this->tab->Move(7, 1);
  this->AddChild(this->tab);

  CUITabButton* firstButton = new CUITabButton;
  firstButton->SetSize(38, 20);
  firstButton->Move(0, 0);
  firstButton->disabled = 0;
  firstButton->Invalidate();
  {
    // GLOBAL: gl_25243
    // GLOBAL: gl_25242
    // GLOBAL: gl_25241
    // GLOBAL: gl_25240
    firstButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_out.bmp", 0x4F99DF03C3EF31B7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_pressed.bmp", 0x9171997D6963C9F7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_out.bmp", 0xCFF3899D95AF355FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_random_disable.bmp", 0xE52DF30DCBAF7125ULL)));
  }
  this->tab->AddChild(firstButton);

  CUITabButton* secondButton = new CUITabButton;
  secondButton->SetSize(38, 20);
  secondButton->Move(38, 0);
  secondButton->disabled = 0;
  secondButton->Invalidate();
  {
    // GLOBAL: gl_25247
    // GLOBAL: gl_25246
    // GLOBAL: gl_25245
    // GLOBAL: gl_25244
    secondButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_out.bmp", 0x4F637DC51307B759ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_pressed.bmp", 0x2DCB7567394DC1ABULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_out.bmp", 0xB39FA7917BCFBB8DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_confirm_disable.bmp", 0x8D1D0FF1AF2B13F7ULL)));
  }
  this->tab->AddChild(secondButton);

  firstButton->page = this->firstPage;
  secondButton->page = this->secondPage;
  this->tab->Select(firstButton);
}
}

// OFFSET: 0x00A021D0
// FUNCTION: fn_14838
void CUIRadioButton::SetOnSelect(std::function<void(CUIRadioButton*)> callback) {
  this->onSelect = callback;
}

namespace EnchantUI {
// OFFSET: 0x00A02D80
// FUNCTION: fn_14845
void CUIEnchantPage::SetSlot(int value) {
  this->selectedSlot = value;
}

// OFFSET: 0x00A05190
// FUNCTION: fn_14970
void CUIUpgradeEnchantPage::OnSelectSlot(int value) {
  if (CUIMainPage* mainPage = dynamic_cast<CUIMainPage*>(this->parent)) {
    mainPage->firstPage->SetSlot(value);
    mainPage->secondPage->SetSlot(value);
  }
}

// OFFSET: 0x00A06B00
// FUNCTION: fn_15006
void CUIUpgradeEnchantPage::SetSlot(int value) {
  this->selectedSlot = value;
  for (const auto& slot : this->slots) {
    CUISlot* item = slot.second;
    const bool selected = value == slot.first;
    auto* radioButton = item->radioButton;
    radioButton->checked = selected;
    radioButton->Invalidate();
    item->description->SetTextColor(selected ? 0x644031 : 0xB6752E);
    item->description->SetBold(selected);
    item->description->Relayout();
  }
  this->UpdateEnchantInfo();
}

// OFFSET: 0x00A05290
// FUNCTION: fn_14972
int CUIEnchantPage::FindEmptySlot() {
  const ITEM_INFO* info = this->gameItem->GetItemInfo();
  const std::vector<int>& slots = this->enchantInfo->slots;
  auto it = std::find_if(slots.begin(), slots.end(), [item = *info](int slot) {
    if (slot >= 4) {
      return true;
    }
    int card;
    std::memcpy(&card, item.cards + slot * 4, sizeof(card));
    return card == 0;
  });
  if (it != slots.end()) {
    return *it;
  }
  return -1;
}

// OFFSET: 0x00A053D0
// FUNCTION: fn_14973
ITEM_INFO* CUITargetItem::GetItemInfo() {
  return this->itemIcon->GetItemInfo();
}

// OFFSET: 0x00A053E0
// FUNCTION: fn_14974
std::optional<EnchantOptionInfo*> CUIEnchantPage::FindOption(uint32_t key) {
  if (this->enchantInfo == nullptr) {
    return std::nullopt;
  }

  auto& options = this->enchantInfo->options;
  auto it = options.find(key);
  if (it == options.end()) {
    return std::nullopt;
  }
  return &it->second;
}

// OFFSET: 0x00A05CE0
// FUNCTION: fn_14980
void CUISlot::Paint() {
  this->ClearSurface(0xFFFF00FF);
}
}

// OFFSET: 0x00A05DF0
// FUNCTION: fn_14983
void CUIEnchantUI::OnInit() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
}

namespace EnchantUI {
// OFFSET: 0x00A06050
// FUNCTION: fn_14988
void CUISlot0::OnInit() {
  CUISlot::OnInit();
  this->radioButton->SetId(0);
}

// OFFSET: 0x00A06090
// FUNCTION: fn_14989
void CUISlot1::OnInit() {
  CUISlot::OnInit();
  this->radioButton->SetId(1);
}

// OFFSET: 0x00A060D0
// FUNCTION: fn_14990
void CUISlot2::OnInit() {
  CUISlot::OnInit();
  this->radioButton->SetId(2);
}

// OFFSET: 0x00A06110
// FUNCTION: fn_14991
void CUISlot3::OnInit() {
  CUISlot::OnInit();
  this->radioButton->SetId(3);
}

// OFFSET: 0x00A06150
// FUNCTION: fn_14992
void CUISlot::OnInit() {
  this->itemIcon->hitEnabled = 0;
  this->description->scrollBar->SetHidden(1);
}
}

// OFFSET: 0x00A06190
// FUNCTION: fn_14994
void CUIEnchantUI::OnLBtnDown(int x, int y) {
  if (this->titleBar->IsPointInside(x, y)) {
    CUIFrameWnd::OnLBtnDown(x, y);
  }
}

namespace EnchantUI {
// OFFSET: 0x00A061C0
// FUNCTION: fn_14995
void CUIEnchantItem::OnMouseHover(int x, int y) {
  this->itemIcon->OnMouseHover(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A061F0
// FUNCTION: fn_14996
void CUITargetItem::OnMouseHover(int x, int y) {
  this->itemIcon->OnMouseHover(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06220
// FUNCTION: fn_14997
void CUIEnchantItem::OnRBtnDown(int x, int y) {
  this->itemIcon->OnRBtnDown(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06250
// FUNCTION: fn_14998
void CUISlot::OnRBtnDown(int x, int y) {
  if (this->itemIcon->IsPointInside(x, y)) {
    this->itemIcon->OnRBtnDown(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
  }
}

// OFFSET: 0x00A06290
// FUNCTION: fn_14999
void CUITargetItem::OnRBtnDown(int x, int y) {
  this->itemIcon->OnRBtnDown(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A062C0
// FUNCTION: fn_15000
void CUISlot::SelectSlot(UIWindow* button) {
  if (CUIEnchantPage* page = dynamic_cast<CUIEnchantPage*>(this->parent)) {
    page->OnSelectSlot(button->geometry.id);
  }
}

// OFFSET: 0x00A06A50
// FUNCTION: fn_15005
void CUITargetItem::SetSelected(bool selected) {
  if (selected) {
    if (this->selected != 1) {
      this->itemIcon->Move(this->itemIcon->geometry.x + 1, this->itemIcon->geometry.y + 1);
      this->targetIcon->Move(this->targetIcon->geometry.x + 1, this->targetIcon->geometry.y + 1);
      this->targetText->Move(this->targetText->geometry.x + 1, this->targetText->geometry.y + 1);
    }
  } else if (this->selected == 1) {
    this->itemIcon->Move(this->itemIcon->geometry.x - 1, this->itemIcon->geometry.y - 1);
    this->targetIcon->Move(this->targetIcon->geometry.x - 1, this->targetIcon->geometry.y - 1);
    this->targetText->Move(this->targetText->geometry.x - 1, this->targetText->geometry.y - 1);
  }
  CUIListItem::SetSelected(selected);
}

// OFFSET: 0x00A04D70
// FUNCTION: fn_14965
void CUINormalPerfect::RequestEnchant() {
  CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* list = this->enchantList;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto it = list->data.begin();
    for (int i = list->selected; i > 0; --i) {
      ++it;
    }
    CEnchantMgr::GetInstance()->RequestPerfectEnchant(
      this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->identity[1]), static_cast<int16_t>(this->selectedSlot), it->itemId
    );
  }
}

// OFFSET: 0x00A04E60
// FUNCTION: fn_14966
void CUINormalRandom::RequestEnchant() {
  CEnchantMgr::GetInstance()->RequestRandomEnchant(
    this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->identity[1]), static_cast<int16_t>(this->selectedSlot)
  );
}

// OFFSET: 0x00A04F10
// FUNCTION: fn_14967
void CUIReset::RequestEnchant() {
  CEnchantMgr::GetInstance()->RequestResetEnchant(this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->identity[1]));
}

// OFFSET: 0x00A04FB0
// FUNCTION: fn_14968
void CUIUpgradePerfect::RequestEnchant() {
  if (this->isUpgrade) {
    CEnchantMgr::GetInstance()->RequestUpgradeEnchant(
      this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->identity[1]), static_cast<int16_t>(this->selectedSlot)
    );
    return;
  }

  CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* list = this->enchantList;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto it = list->data.begin();
    std::advance(it, static_cast<int>(list->selected));
    CEnchantMgr::GetInstance()->RequestUpgradePerfectEnchant(
      this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->identity[1]), static_cast<int16_t>(this->selectedSlot), it->itemId
    );
  }
}

// OFFSET: 0x00A050E0
// FUNCTION: fn_14969
void CUIUpgradeRandom::RequestEnchant() {
  CEnchantMgr::GetInstance()->RequestUpgradeRandomEnchant(
    this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->identity[1]), static_cast<int16_t>(this->selectedSlot)
  );
}
}

// OFFSET: 0x00A08110
// FUNCTION: fn_15017
bool CUIEnchantUI::CanHitTest() {
  return !CEnchantMgr::GetInstance()->m_inputBlocked;
}

// OFFSET: 0x009F6D40
// FUNCTION: fn_14766
template <>
CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x009F6F00
// FUNCTION: fn_14767
template <>
CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x009F7670
// FUNCTION: fn_14774
template <>
CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::~CUIListBoxEx() {
}

// OFFSET: 0x00A05970
// FUNCTION: fn_14978
void CUIEnchantUI::OnSelectTarget(EnchantUI::CUITargetItem* item, const ITEM_INFO& info) {
  this->normalPage->LoadTarget(this->enchantInfo, this->group, *item->itemIcon->GetItemInfo());
  this->upgradePage->LoadTarget(this->enchantInfo, this->group, *item->itemIcon->GetItemInfo());
  this->resetPage->LoadTarget(this->enchantInfo, this->group, *item->itemIcon->GetItemInfo());
  this->Invalidate();
}

// OFFSET: 0x00A06C40
// FUNCTION: fn_15008
void CUIEnchantUI::SetEnchantGroup(uint64_t group) {
  this->enchantInfo = CEnchantMgr::GetInstance()->GetEnchantInfo(group);
  if (!this->enchantInfo) {
    g_windowMgr.CloseWindow(WID_ENCHANTUI);
    return;
  }

  this->group = group;
  this->textViewer->Clear();
  this->textViewer->AddLine(this->enchantInfo->caution);
  for (int itemId : this->enchantInfo->targetItems) {
    const std::vector<int> indices = g_session.FindInventoryIndicesByItemId(itemId, 1);
    for (int index : indices) {
      if (g_session.HasSwapEquipmentItem(index)) {
        continue;
      }
      const ITEM_INFO target = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
      if (static_cast<std::uint32_t>(target.refiningLevel) < this->enchantInfo->condition1) {
        continue;
      }
      if (static_cast<std::uint32_t>(target.grade) < this->enchantInfo->condition2) {
        continue;
      }
      if (!this->enchantInfo->approveRandomOption && target.optionCount > 0) {
        continue;
      }
      this->targetList->data.push_back(target);
    }
    this->targetList->UpdateScrollBar();
  }
}

namespace EnchantUI {
// OFFSET: 0x00A06BE0
// FUNCTION: fn_15007
void CUIEnchantItem::SetFrozen(bool frozen) {
  auto* const icon = this->itemIcon;
  icon->transparent = frozen;
  icon->Invalidate();
  this->name->SetColor(frozen ? 0xAAAAAA : 0xC89682);
}

// OFFSET: 0x00A05860
// FUNCTION: fn_14977
void CUIEnchantPage::UpdateEnchantItem(CUIEnchantItem* item, const CEnchantItemData& data) {
  {
    ITEM_INFO info;
    info.SetItemId(data.itemId);
    info.identified = 1;
    item->itemIcon->SetItem(info);
    item->name->SetText(info.DisplayName(0));
  }

  item->SetFrozen(data.frozen);
}

}

// OFFSET: 0x00A05B90
// FUNCTION: fn_14979
void CUIEnchantUI::UpdateTargetItem(EnchantUI::CUITargetItem* item, const ITEM_INFO& info) {
  item->itemIcon->SetItem(info);
  if (0 < info.grade) {
    item->targetIcon->SetImage(std::format("유저인터페이스\\grade_enchant\\grade_icon{}.bmp", info.grade));
  }
  item->targetIcon->Show(0 < info.grade);
  item->targetText->SetText(info.DisplayName(0));
}

namespace EnchantUI {
// OFFSET: 0x00A05E20
// FUNCTION: fn_14984
void CUIMainPage::OnInit() {
  CUITabButton* button = this->tab->FindButton(this->firstPage);
  if (button != nullptr) {
    button->disabled = 1;
    button->Invalidate();
  }

  button = this->tab->FindButton(this->secondPage);
  if (button != nullptr) {
    button->disabled = 1;
    button->Invalidate();
  }
}
}

// OFFSET: 0x009F3160
// FUNCTION: fn_14752
void CUIEnchantUI::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = true;
  this->Resize(690, 470);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x00A06170
// FUNCTION: fn_14993
void EnchantUI::CUIUpgradeRandom::OnInit() {
  EnchantUI::CUIEnchantPage::OnInit();
  auto* const list = this->enchantList;
  list->layoutFlag = 1;
  list->Invalidate();
}

namespace EnchantUI {
// OFFSET: 0x00A05CF0
// FUNCTION: fn_14981
void CUIEnchantPage::OnInit() {
  for (int i = 0; i < 8u; ++i) {
    CUIIngredient* ingredient = this->CreateIngredient();
    ingredient->Move(ingredient->geometry.x + i % 4 * 74, ingredient->geometry.y + i / 4 * 52);
    this->ingredients.push_back(ingredient);
  }

  this->ResetSlots();
}

// OFFSET: 0x00A05E80
// FUNCTION: fn_14985
void CUINormalPerfect::OnInit() {
  CUIEnchantPage::OnInit();
  for (const auto& slot : this->slots) {
    auto* const radioButton = slot.second->radioButton;
    radioButton->disabled = 1;
    radioButton->Invalidate();
  }
}

// OFFSET: 0x00A05F10
// FUNCTION: fn_14986
void CUINormalRandom::OnInit() {
  CUIEnchantPage::OnInit();
  for (const auto& slot : this->slots) {
    auto* const radioButton = slot.second->radioButton;
    radioButton->disabled = 1;
    radioButton->Invalidate();
  }

  auto* const list = this->enchantList;
  list->layoutFlag = 1;
  list->Invalidate();
}

// OFFSET: 0x00A05FB0
// FUNCTION: fn_14987
void CUIReset::OnInit() {
  CUIEnchantPage::OnInit();
  for (const auto& slot : this->slots) {
    auto* const radioButton = slot.second->radioButton;
    radioButton->disabled = 1;
    radioButton->Invalidate();
  }

  auto* const list = this->enchantList;
  list->layoutFlag = 1;
  list->Invalidate();
}

// OFFSET: 0x00A06350
// FUNCTION: fn_15002
void CUIEnchantPage::UpdateEnchantInfo() {
  this->enchantList->ClearData();
  for (CUIIngredient* ingredient : this->ingredients) {
    ingredient->Show(0);
  }

  this->costText->SetText(std::to_string(0));
  {
    CUIImageButton* button = this->enchantButton;
    button->disabled = 1;
    button->Invalidate();
  }

  const bool ready = this->LoadEnchantList();
  if (CUIMainPage* mainPage = dynamic_cast<CUIMainPage*>(this->parent)) {
    if (CUITabButton* button = mainPage->tab->FindButton(this)) {
      button->disabled = !ready;
      button->Invalidate();
    }
  }
}

// OFFSET: 0x00A06520
// FUNCTION: fn_15003
void CUIReset::UpdateEnchantInfo() {
  for (CUIIngredient* ingredient : this->ingredients) {
    ingredient->Show(0);
  }

  this->costText->SetText(std::to_string(0));
  {
    CUIImageButton* button = this->enchantButton;
    button->disabled = 1;
    button->Invalidate();
  }
  this->enchantList->ClearData();
  this->LoadEnchantList();
}

// OFFSET: 0x00A06800
// FUNCTION: fn_15004
void CUIEnchantPage::ResetSlots() {
  std::for_each(this->slots.begin(), this->slots.end(), [](const auto& slot) {
    slot.second->Show(0);
  });

  for (CUIIngredient* ingredient : this->ingredients) {
    ingredient->Show(0);
  }

  this->enchantList->ClearData();
  this->costText->SetText(std::to_string(0));
  {
    CUIImageButton* button = this->enchantButton;
    button->disabled = 1;
    button->Invalidate();
  }
  this->SetSlot(-1);
}

// OFFSET: 0x00A06F80
// FUNCTION: fn_15009
bool CUINormalPerfect::LoadEnchantList() {
  const int slot = this->FindEmptySlot();
  if (slot == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(slot);
  if (!option) {
    return false;
  }

  this->SetSlot(slot);
  if ((*option)->perfectEnchants.size() == 0) {
    return false;
  }

  for (const auto& entry : (*option)->perfectEnchants) {
    const bool lackZeny = g_session.m_zeny < entry.second.cost;
    const bool lackMaterial = std::any_of(entry.second.materials.begin(), entry.second.materials.end(), [](const std::pair<const int, int>& material) {
      return static_cast<unsigned int>(g_session.GetInventoryItemCount(material.first)) < static_cast<unsigned int>(material.second);
    });
    const bool frozen = lackZeny || lackMaterial;
    this->enchantList->AddData({entry.first, frozen}, false);
  }

  this->enchantList->UpdateScrollBar();
  return true;
}
}

namespace EnchantUI {
// OFFSET: 0x00A05470
// FUNCTION: fn_14975
void CUINormalPerfect::OnSelectEnchant(CUIEnchantItem* item, const CEnchantItemData& data) {
  for (CUIIngredient* ingredient : this->ingredients) {
    ingredient->Show(0);
  }

  this->costText->SetText(std::to_string(0));

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->selectedSlot);
  if (option && (*option)->perfectEnchants.size() != 0) {
    const int itemId = std::atoi(item->itemIcon->GetItemInfo()->itemId.c_str());
    auto it = (*option)->perfectEnchants.find(itemId);
    if (it != (*option)->perfectEnchants.end()) {
      this->SetCost(it->second);
    }
  }
}

// OFFSET: 0x00A05640
// FUNCTION: fn_14976
void CUIUpgradePerfect::OnSelectEnchant(CUIEnchantItem* item, const CEnchantItemData& data) {
  for (CUIIngredient* ingredient : this->ingredients) {
    ingredient->Show(0);
  }

  this->costText->SetText(std::to_string(0));

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->selectedSlot);
  if (option && (*option)->perfectUpgrades.size() != 0) {
    const int baseItemId = std::atoi(this->slots[this->selectedSlot]->itemIcon->GetItemInfo()->itemId.c_str());
    auto base = (*option)->perfectUpgrades.find(baseItemId);
    if (base != (*option)->perfectUpgrades.end()) {
      const int itemId = std::atoi(item->itemIcon->GetItemInfo()->itemId.c_str());
      auto it = base->second.find(itemId);
      if (it != base->second.end()) {
        this->SetCost(it->second);
      }
    }
  }
}

// OFFSET: 0x00A072E0
// FUNCTION: fn_15011
bool CUIReset::LoadEnchantList() {
  this->SetCost(*reinterpret_cast<const EnchantPerfectInfo*>(&this->enchantInfo->resetCost));
  if (!this->enchantInfo->resetEnabled) {
    {
      CUIImageButton* button = this->enchantButton;
      button->disabled = 1;
      button->Invalidate();
    }
    return false;
  }

  const std::vector<int>& slots = this->enchantInfo->slots;
  const bool empty = std::none_of(slots.begin(), slots.end(), [item = *this->gameItem->GetItemInfo()](int slot) {
    if (slot >= 4) {
      return false;
    }
    int card;
    std::memcpy(&card, item.cards + slot * 4, sizeof(card));
    return card != 0;
  });
  if (empty) {
    {
      CUIImageButton* button = this->enchantButton;
      button->disabled = empty;
      button->Invalidate();
    }
  }
  return !empty;
}

// OFFSET: 0x00A079E0
// FUNCTION: fn_15014
bool CUIEnchantPage::SetItem(const ITEM_INFO& item) {
  if (item.identity[1] <= 0) {
    return false;
  }

  this->gameItem->SetItem(item);
  for (const auto& entry : this->slots) {
    CUISlot* slot = entry.second;
    int card = 0;
    if (entry.first < 4) {
      std::memcpy(&card, item.cards + entry.first * 4, sizeof(card));
    }
    if (card > 0) {
      ITEM_INFO cardInfo;
      cardInfo.SetItemId(card);
      cardInfo.identified = 1;
      slot->itemIcon->SetItem(cardInfo);
      slot->radioButton->tooltip = slot->itemIcon->name;
      slot->description->Clear();
      slot->description->AddLine(cardInfo.DisplayName(0));
      slot->Show(1);
    } else {
      slot->Show(0);
    }
  }
  return true;
}

// OFFSET: 0x00A07BA0
// FUNCTION: fn_15015
void CUIIngredient::SetMaterial(int itemId, int required) {
  ITEM_INFO info;
  info.SetItemId(itemId);
  info.identified = 1;
  this->itemIcon->SetItem(info);

  const int owned = g_session.GetInventoryItemCount(itemId);
  this->ownedText->SetText(std::to_string(owned));
  this->requiredText->SetText(std::format("/{}", required));

  const bool enough = owned >= required;
  this->sufficientMaterialImage->Show(enough);
  const std::uint32_t color = enough ? 0xF0F0F0 : 0x5A3CE6;
  this->ownedText->SetColor(color);
  this->ownedText->SetOutline(enough, 0x965532);
  this->requiredText->SetColor(color);
  this->requiredText->SetOutline(enough, 0x965532);
}

// OFFSET: 0x00A07F00
// FUNCTION: fn_15016
void CUIEnchantPage::SetCost(const EnchantPerfectInfo& info) {
  auto material = info.materials.begin();
  for (CUIIngredient* ingredient : this->ingredients) {
    if (material != info.materials.end()) {
      ingredient->Show(1);
      ingredient->SetMaterial(material->first, material->second);
      ++material;
    } else {
      ingredient->Show(0);
    }
  }

  this->costText->SetText(std::to_string(info.cost));

  const bool lackZeny = g_session.m_zeny < info.cost;
  const bool lackMaterial = std::any_of(this->ingredients.begin(), this->ingredients.end(), [](CUIIngredient* ingredient) {
    return ingredient->geometry.visible && !ingredient->sufficientMaterialImage->geometry.visible;
  });
  {
    CUIImageButton* button = this->enchantButton;
    button->disabled = lackZeny || lackMaterial;
    button->Invalidate();
  }
}

// OFFSET: 0x00A07100
// FUNCTION: fn_15010
bool CUINormalRandom::LoadEnchantList() {
  const int slot = this->FindEmptySlot();
  if (slot == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(slot);
  if (!option) {
    return false;
  }

  this->SetSlot(slot);
  if ((*option)->enchants.size() == 0) {
    return false;
  }

  auto grade = (*option)->enchants.find(this->gameItem->GetItemInfo()->grade);
  if (grade == (*option)->enchants.end()) {
    return false;
  }

  std::list<int> itemIds;
  for (const auto& entry : grade->second) {
    itemIds.push_back(entry.first);
  }
  for (int itemId : itemIds) {
    this->enchantList->AddData({itemId, false}, false);
  }

  this->enchantList->UpdateScrollBar();
  this->SetCost(*reinterpret_cast<const EnchantPerfectInfo*>(*option));
  return true;
}

// OFFSET: 0x00A075E0
// FUNCTION: fn_15013
bool CUIUpgradeRandom::LoadEnchantList() {
  if (this->selectedSlot == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->selectedSlot);
  if (!option || (*option)->randomUpgrades.size() == 0) {
    return false;
  }

  const int baseItemId = std::atoi(this->slots[this->selectedSlot]->itemIcon->GetItemInfo()->itemId.c_str());
  auto base = (*option)->randomUpgrades.find(baseItemId);
  if (base == (*option)->randomUpgrades.end()) {
    return false;
  }

  std::list<int> itemIds;
  for (const auto& entry : base->second.rates) {
    itemIds.push_back(entry.first);
  }
  for (int itemId : itemIds) {
    this->enchantList->AddData({itemId, false}, false);
  }

  this->enchantList->UpdateScrollBar();
  this->SetCost(*reinterpret_cast<const EnchantPerfectInfo*>(&base->second));
  return true;
}

// OFFSET: 0x00A07440
// FUNCTION: fn_15012
bool CUIUpgradePerfect::LoadEnchantList() {
  if (this->selectedSlot == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->selectedSlot);
  if (!option) {
    return false;
  }

  this->isUpgrade = (*option)->isUpgrade;
  const auto& upgrades = (*option)->perfectUpgrades;
  if (upgrades.size() == 0) {
    return false;
  }

  const int baseItemId = std::atoi(this->slots[this->selectedSlot]->itemIcon->GetItemInfo()->itemId.c_str());
  auto base = upgrades.find(baseItemId);
  if (base == upgrades.end() || base->second.size() == 0) {
    return false;
  }

  for (const auto& entry : base->second) {
    const bool lackZeny = g_session.m_zeny < entry.second.cost;
    const bool lackMaterial = std::any_of(entry.second.materials.begin(), entry.second.materials.end(), [](const std::pair<const int, int>& material) {
      return static_cast<unsigned int>(g_session.GetInventoryItemCount(material.first)) < static_cast<unsigned int>(material.second);
    });
    const bool frozen = lackZeny || lackMaterial;
    this->enchantList->AddData({entry.first, frozen}, false);
  }

  this->enchantList->UpdateScrollBar();
  return true;
}

// OFFSET: 0x009F9FE0
// FUNCTION: fn_14817
void CUIEnchantPage::OnCreate(int width, int height) {
  this->background = new CUIStaticImage;
  this->background->SetSize(470, 377);
  this->background->Move(0, 0);
  {
    // GLOBAL: gl_25259
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpage.bmp", 0x2FE993D1DDE1F197ULL)));
  }
  this->AddChild(this->background);

  this->gameItem = new CUIGameItem;
  this->gameItem->SetMode(1);
  this->gameItem->SetScale(100);
  this->gameItem->Move(112, 5);
  this->gameItem->showNameTooltip = 1;
  this->gameItem->allowItemInfo = 1;
  this->AddChild(this->gameItem);

  CUISlot* slot = new CUISlot0;
  slot->SetSize(106, 84);
  slot->Move(6, 20);
  this->AddChild(slot);
  this->slots[0] = slot;

  slot = new CUISlot1;
  slot->SetSize(106, 84);
  slot->Move(29, 105);
  this->AddChild(slot);
  this->slots[1] = slot;

  slot = new CUISlot2;
  slot->SetSize(106, 84);
  slot->Move(164, 105);
  this->AddChild(slot);
  this->slots[2] = slot;

  slot = new CUISlot3;
  slot->SetSize(106, 84);
  slot->Move(187, 20);
  this->AddChild(slot);
  this->slots[3] = slot;

  CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* enchantList = new CUIListBoxEx<CUIEnchantItem, CEnchantItemData>;
  this->enchantList = enchantList;
  enchantList->itemWidth = 150;
  enchantList->rowHeight = 30;
  enchantList->SetSize(169, 377);
  this->enchantList->Move(300, 0);
  this->enchantList->SetOnSelect([this](CUIEnchantItem* item, const CEnchantItemData& data) { this->OnSelectEnchant(item, data); });
  this->enchantList->SetBinder([this](CUIEnchantItem* item, const CEnchantItemData& data) { this->UpdateEnchantItem(item, data); });
  {
    // GLOBAL: gl_25263
    // GLOBAL: gl_25262
    // GLOBAL: gl_25261
    // GLOBAL: gl_25260
    this->enchantList->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_enc_item.bmp", 0xAD075B8DB599F1CDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_enc_item_press.bmp", 0x2F8B0F37118D157FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_enc_item_over.bmp", 0x35E5656B5DEDCD75ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\btn_enc_item.bmp", 0x4941FD9F21BBB9FDULL)));
  }
  {
    auto* const list = this->enchantList;
    list->itemX = 3;
    list->itemY = 0;
  }
  this->enchantList->scrollBar->SetSize(16, 377);
  this->enchantList->scrollBar->Move(153, 0);
  this->enchantList->scrollBar->step = 30;
  {
    // GLOBAL: gl_25265
    // GLOBAL: gl_25264
    this->enchantList->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_arrow_up.bmp", 0xE7978FB5A94DD7FDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_arrow_dowm.bmp", 0xB7FD8F670FC7B96BULL)));
  }
  {
    // GLOBAL: gl_25266
    this->enchantList->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_bg.bmp", 0xCB1D253B7FE1F563ULL)));
  }
  {
    // GLOBAL: gl_25267
    this->enchantList->SetScrollBarThumbImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_btn.bmp", 0x7115D9F973F913C9ULL)));
  }
  this->enchantList->AddNewItem();
  this->AddChild(this->enchantList);

  this->costText = new CUIStaticText;
  this->costText->SetSize(100, 15);
  this->costText->Move(150, 309);
  this->costText->SetText("");
  this->costText->SetFitText(0);
  this->costText->SetAlign(2, 6);
  {
    // GLOBAL: gl_25268
    this->costText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xE55723712993051DULL)));
  }
  this->costText->SetFontHeight(14);
  this->costText->SetBold(1);
  this->costText->SetColor(0);
  this->costText->SetShadow(0, 0xFFFFFF);
  this->costText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->costText);

  this->enchantButton = new CUIImageButton;
  this->enchantButton->SetSize(140, 40);
  this->enchantButton->Move(80, 330);
  this->enchantButton->SetOnClick([this]() { this->RequestEnchant(); });
  {
    CUIImageButton* button = this->enchantButton;
    button->disabled = 0;
    button->Invalidate();
  }
  this->enchantButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_enc.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_enc_press.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_enc_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_enc_dis.bmp")
  );
  this->AddChild(this->enchantButton);
}

// OFFSET: 0x009FCDE0
// FUNCTION: fn_14820
void CUINormalPerfect::OnCreate(int width, int height) {
  CUIEnchantPage::OnCreate(width, height);
  {
    // GLOBAL: gl_25269
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageY.bmp", 0x0F15B16B5BAFD57FULL)));
  }
}

// OFFSET: 0x009FCFB0
// FUNCTION: fn_14821
void CUINormalRandom::OnCreate(int width, int height) {
  CUIEnchantPage::OnCreate(width, height);
  {
    // GLOBAL: gl_25270
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageB.bmp", 0x793F370985ED674FULL)));
  }
}

// OFFSET: 0x009FD180
// FUNCTION: fn_14822
void CUIReset::OnCreate(int width, int height) {
  CUIEnchantPage::OnCreate(width, height);
  {
    // GLOBAL: gl_25271
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpage.bmp", 0x7941D76335CFA14FULL)));
  }
}

// OFFSET: 0x00A01AD0
// FUNCTION: fn_14830
void CUIUpgradePerfect::OnCreate(int width, int height) {
  CUIUpgradeEnchantPage::OnCreate(width, height);
  {
    // GLOBAL: gl_25272
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageY.bmp", 0x7321C3814F8799E1ULL)));
  }
}

// OFFSET: 0x00A01CA0
// FUNCTION: fn_14831
void CUIUpgradeRandom::OnCreate(int width, int height) {
  CUIUpgradeEnchantPage::OnCreate(width, height);
  {
    // GLOBAL: gl_25273
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageB.bmp", 0x7DE349D717D7F123ULL)));
  }
}
}

// OFFSET: 0x009F31B0
// FUNCTION: fn_14753
void CUIEnchantUI::OnCreate(int width, int height) {
  this->background = new CUIStaticImage;
  this->background->SetSize(690, 470);
  this->background->Move(0, 0);
  {
    // GLOBAL: gl_25281
    this->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enchantUI.bmp", 0x33133B977BCD03CDULL)));
  }
  this->AddChild(this->background);

  this->titleBar = new CUITitleBar;
  this->titleBar->SetSize(690, 17);
  this->titleBar->Move(0, 0);
  {
    // GLOBAL: gl_25284
    // GLOBAL: gl_25283
    // GLOBAL: gl_25282
    this->titleBar->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0x2FEB8739E5975BD1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0x6BA1C1132F033B75ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0xA12DAB392545D72FULL)));
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText;
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText("");
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    // GLOBAL: gl_25285
    title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x234977DDB54BC71DULL)));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* closeButton = new CUIImageButton;
  closeButton->SetSize(11, 11);
  closeButton->Move(676, 3);
  closeButton->SetOnClick([this]() { this->OnClickClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    // GLOBAL: gl_25286
    const std::string disabled(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xA597A7E7492B39D5ULL));
    // GLOBAL: gl_25289
    // GLOBAL: gl_25288
    // GLOBAL: gl_25287
    closeButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xEF6D1D893F2B2DC5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x39A39F87DD5F535BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x6FE153231D879777ULL)), disabled);
  }
  this->titleBar->SetSecondButton(closeButton);

  this->targetList = new CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>;
  this->targetList->itemWidth = 200;
  this->targetList->rowHeight = 50;
  this->targetList->SetSize(220, 400);
  this->targetList->Move(0, 17);
  this->targetList->SetOnSelect([this](EnchantUI::CUITargetItem* item, const ITEM_INFO& info) { this->OnSelectTarget(item, info); });
  this->targetList->SetBinder([this](EnchantUI::CUITargetItem* item, const ITEM_INFO& info) { this->UpdateTargetItem(item, info); });
  {
    // GLOBAL: gl_25290
    const std::string disabled(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enc_gear.bmp", 0x8783C33941E15117ULL));
    // GLOBAL: gl_25293
    // GLOBAL: gl_25292
    // GLOBAL: gl_25291
    this->targetList->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enc_gear.bmp", 0xFFABE3D5D1BBEF73ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enc_gear_press.bmp", 0x0399459DAF454F0FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_enc_gear_over.bmp", 0xD5A78DB38D8DAFABULL)), disabled);
  }
  this->targetList->itemX = 4;
  this->targetList->itemY = 0;
  this->targetList->scrollBar->SetSize(16, 400);
  this->targetList->scrollBar->Move(204, 0);
  this->targetList->scrollBar->step = 50;
  {
    // GLOBAL: gl_25295
    // GLOBAL: gl_25294
    this->targetList->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_arrow_up.bmp", 0x271B45771F83DD9FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_arrow_dowm.bmp", 0xC70319B5EB61598FULL)));
  }
  {
    // GLOBAL: gl_25296
    this->targetList->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_bg.bmp", 0x0B8745519B1F95D7ULL)));
  }
  {
    // GLOBAL: gl_25297
    this->targetList->SetScrollBarThumbImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\scroll_btn.bmp", 0x01CB4F1DF9D3F931ULL)));
  }
  this->targetList->AddNewItem();
  this->AddChild(this->targetList);

  this->tab = new CUITab;
  this->tab->SetSize(290, 22);
  this->tab->Move(225, 18);
  this->AddChild(this->tab);

  CUITabButton* normalButton = new CUITabButton;
  normalButton->SetSize(65, 22);
  normalButton->Move(48, 0);
  normalButton->disabled = 0;
  normalButton->Invalidate();
  {
    // GLOBAL: gl_25298
    const std::string disabled(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_common_out.bmp", 0xE33F2FF779878559ULL));
    // GLOBAL: gl_25301
    // GLOBAL: gl_25300
    // GLOBAL: gl_25299
    normalButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_common_out.bmp", 0xC5B1BBCD9BC9C997ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_common_select.bmp", 0xC1BB8111EDD183E1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_common_out.bmp", 0x1767DDE18D779D8BULL)), disabled);
  }
  this->tab->AddChild(normalButton);

  CUITabButton* upgradeButton = new CUITabButton;
  upgradeButton->SetSize(65, 22);
  upgradeButton->Move(113, 0);
  upgradeButton->disabled = 0;
  upgradeButton->Invalidate();
  {
    // GLOBAL: gl_25302
    const std::string disabled(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_grow_out.bmp", 0x79E915E9AF6F55DDULL));
    // GLOBAL: gl_25305
    // GLOBAL: gl_25304
    // GLOBAL: gl_25303
    upgradeButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_grow_out.bmp", 0xF15F8785CDE369BFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_grow_select.bmp", 0x792FFD1325418FB5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_grow_out.bmp", 0x49D33529F1556333ULL)), disabled);
  }
  this->tab->AddChild(upgradeButton);

  CUITabButton* resetButton = new CUITabButton;
  resetButton->SetSize(65, 22);
  resetButton->Move(178, 0);
  resetButton->disabled = 0;
  resetButton->Invalidate();
  {
    // GLOBAL: gl_25309
    // GLOBAL: gl_25308
    // GLOBAL: gl_25307
    // GLOBAL: gl_25306
    resetButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_reset_out.bmp", 0xB7AFBD2FD9FD7501ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_reset_select.bmp", 0x192D654D29337BE5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_reset_out.bmp", 0xB3E307A5F753D5E3ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\tab_enc_reset_out.bmp", 0x0B23D10F2F49D72BULL)));
  }
  this->tab->AddChild(resetButton);

  this->normalPage = new EnchantUI::CUINormal;
  this->normalPage->SetSize(470, 377);
  this->normalPage->Move(220, 40);
  this->AddChild(this->normalPage);
  {
    // GLOBAL: gl_25310
    this->normalPage->firstPage->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageB.bmp", 0x136F2F9915D1DBCFULL)));
  }
  {
    // GLOBAL: gl_25311
    this->normalPage->secondPage->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageY.bmp", 0x6BDF334F2BFDAD51ULL)));
  }

  this->upgradePage = new EnchantUI::CUIUpgrade;
  this->upgradePage->SetSize(470, 377);
  this->upgradePage->Move(220, 40);
  this->AddChild(this->upgradePage);
  {
    // GLOBAL: gl_25312
    this->upgradePage->firstPage->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageB.bmp", 0x674D9F3975573D41ULL)));
  }
  {
    // GLOBAL: gl_25313
    this->upgradePage->secondPage->background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\EnchantUI\\bg_subpageY.bmp", 0xADC51BF9C98B413DULL)));
  }

  this->resetPage = new EnchantUI::CUIReset;
  this->resetPage->SetSize(470, 377);
  this->resetPage->Move(220, 40);
  this->AddChild(this->resetPage);

  this->textViewer = new CUITextViewer;
  this->textViewer->SetSize(675, 47);
  this->textViewer->Move(10, 421);
  this->textViewer->SetTextOffset(0, 0);
  {
    // GLOBAL: gl_25314
    this->textViewer->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xDF8117FF49CFF551ULL)));
  }
  this->textViewer->SetFontHeight(12);
  this->textViewer->SetBold(1);
  this->textViewer->SetTextColor(0x503CE6);
  this->textViewer->SetOutline(0, 0xFFFFFF);
  this->textViewer->SetLineSpacing(2);
  this->textViewer->SetBackgroundColor(0xFFFF00FF);
  this->textViewer->SetPadding(11, 3, 3, 5);
  this->textViewer->SetScrollBarSize(13, 47);
  this->textViewer->SetScrollStep(13);
  {
    // GLOBAL: gl_25316
    // GLOBAL: gl_25315
    this->textViewer->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0up.bmp", 0x114901C7E37F6D49ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0down.bmp", 0x7D5D0F110B6B8371ULL)));
  }
  {
    // GLOBAL: gl_25317
    this->textViewer->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0mid.bmp", 0xDB27CB557D038759ULL)));
  }
  {
    // GLOBAL: gl_25320
    // GLOBAL: gl_25319
    // GLOBAL: gl_25318
    this->textViewer->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_up.bmp", 0x3FA3AFA7035DE3FFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_mid.bmp", 0x87E3D1C50D572DEFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_down.bmp", 0x51BB39B763BBBD8DULL)));
  }
  this->AddChild(this->textViewer);

  normalButton->page = this->normalPage;
  upgradeButton->page = this->upgradePage;
  resetButton->page = this->resetPage;
  this->tab->Select(normalButton);
}

void CUIEnchantUI::OnClickClose() {
  g_windowMgr.CloseWindow(WID_ENCHANTUI);
}
