#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Items/UIEquipmentPropertiesWnd.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include "Contents/Tables/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"

#include "Resource/MSGSTRINGID.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <new>
#include <string>
#include <windows.h>

#include "Utility/StdTraitsEqual.h"

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  CEquipmentPropertiesMgr();
  void SetWindow(UIWindow* window);
  void GetDescriptions(const int& group, std::list<std::string>& lines);

  /* 0000 */ unsigned char padding_0000[0x1FC];
};

// INLINED
inline CEquipmentPropertiesMgr* EquipmentPropertiesMgr() {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) CEquipmentPropertiesMgr;
  }
  return g_equipmentPropertiesMgr;
}
}

template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Paint();
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::OnWheel(int);
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::ClearItems();

// OFFSET: 0x00A0EBF0
// FUNCTION: fn_15152
EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::CUIItemEquipmentPropertiesLine()
    : CUIListItem(), labelText(nullptr), valueText(nullptr), evenRowColor(0), oddRowColor(0) {
}

template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* other : this->items) {
    if (other != item) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* selectedItem =
      dynamic_cast<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    std::advance(node, static_cast<int>(this->selected));
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A0CF00
// FUNCTION: fn_15039
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine*& slot : this->items) {
      EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* item =
          new EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        this->SelectItem(listItem);
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A0D160
// FUNCTION: fn_15040
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A0D370
// FUNCTION: fn_15041
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A0D3F0
// FUNCTION: fn_15042
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::OnScroll(int position) {
  auto it = this->items.begin();
  if (it == this->items.end()) {
    return;
  }
  if (this->data.size() == 0) {
    for (; it != this->items.end(); ++it) {
      (*it)->index = -1;
      (*it)->Show(0);
    }
    return;
  }

  const int first = (*it)->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      if (first - row < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - (first - row), this->items.end());
      }
    }
  }

  it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || row >= this->data.size()) {
      break;
    }
    EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* item = *it;
    if (item) {
      if (this->rebind || row != static_cast<unsigned int>(item->index)) {
        item->index = row;
        if (this->binder) {
          auto node = this->data.begin();
          std::advance(node, static_cast<int>(row));
          this->binder(item, *node);
        }
      }
      item->SetSelected(row == this->selected);
      item->Move(this->itemX, y);
      item->Show(1);
    }
  }

  for (auto end = this->items.end(); it != end; ++it) {
    EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A08410
// FUNCTION: fn_15021
bool CUIEquipmentPropertiesWnd::IsFrameWnd() {
  return true;
}

// OFFSET: 0x00A08420
// FUNCTION: fn_15022
bool CUIEquipmentPropertiesWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x00A08430
// FUNCTION: fn_15023
bool CUIEquipmentPropertiesWnd::IsTransmitMouseInput() {
  return true;
}

// OFFSET: 0x00A084C0
// FUNCTION: fn_15025
template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::CUIListBoxEx()
    : CUIControl(), hoverImage(), pressedImage(), disabledImage(), imageNames(), disabledName(), scrollBar(nullptr),
      itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), items(), data(),
      selected(static_cast<unsigned int>(-1)), selectionFlag(1), rebind(0), onSelect(), binder() {
}

// OFFSET: 0x00A08680
// FUNCTION: fn_15026
template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::~CUIListBoxEx() {
}

// OFFSET: 0x00A08E70
// FUNCTION: fn_15033
void CUIEquipmentPropertiesWnd::Initialize() {
  this->m_ownsBackgroundColor = true;
  this->m_ownsBorderColor = true;
  this->m_resolveSkin = true;
  this->Resize(300, 308);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
  this->SetSkinColors(0, 0, 3, 1);
  this->m_buttonBarStyle = 1;
}

// OFFSET: 0x00A08F20
// FUNCTION: fn_15034
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  const std::function<void(int)> onChange = [this](int position) { this->OnScroll(position); };
  this->scrollBar->onChange = onChange;
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A09130
// FUNCTION: fn_15035
void CUIEquipmentPropertiesWnd::OnCreate(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(300, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    const std::string right([] {
      constexpr std::array<char, 50> source = CRYPT_NAME("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0x0B7109397F6B1387ULL);
      // GLOBAL: gl_25321
      thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
      return name.Text();
    }());
    const std::string middle([] {
      constexpr std::array<char, 48> source = CRYPT_NAME("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0x03772763F96D9DA5ULL);
      // GLOBAL: gl_25322
      thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
      return name.Text();
    }());
    const std::string left([] {
      constexpr std::array<char, 49> source = CRYPT_NAME("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0xC517FB932D95BB6DULL);
      // GLOBAL: gl_25323
      thread_local EncryptedName<sizeof(source), 0xC517FB932D95BB6DULL> name(source);
      return name.Text();
    }());
    titleBar->SetImages(left, middle, right);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 18);
  title->Move(18, 0);
  title->SetText(LookupMessage(MSI_SEND_TO_ALLY));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x13E33FA1A591AB67ULL);
      // GLOBAL: gl_25324
      thread_local EncryptedName<sizeof(source), 0x13E33FA1A591AB67ULL> name(source);
      return name.Text();
    }()));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  CUITitleBar* titleBar = this->titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* baseButton = new CUIImageButton();
  baseButton->SetSize(11, 11);
  baseButton->Move(3, 3);
  baseButton->SetOnClick([this]() { this->OnClickButton1(); });
  baseButton->disabled = 0;
  baseButton->Invalidate();
  {
    const std::string inactive([] {
      constexpr std::array<char, 48> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0xD97D5DBDBB2F2709ULL);
      // GLOBAL: gl_25325
      thread_local EncryptedName<sizeof(source), 0xD97D5DBDBB2F2709ULL> name(source);
      return name.Text();
    }());
    const std::string pressed([] {
      constexpr std::array<char, 47> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0xCD8F077123FB7D6FULL);
      // GLOBAL: gl_25326
      thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> name(source);
      return name.Text();
    }());
    const std::string hover([] {
      constexpr std::array<char, 47> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x9949BFD527532943ULL);
      // GLOBAL: gl_25327
      thread_local EncryptedName<sizeof(source), 0x9949BFD527532943ULL> name(source);
      return name.Text();
    }());
    const std::string normal([] {
      constexpr std::array<char, 48> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x17A9E5BF39AD4737ULL);
      // GLOBAL: gl_25328
      thread_local EncryptedName<sizeof(source), 0x17A9E5BF39AD4737ULL> name(source);
      return name.Text();
    }());
    baseButton->SetImages(normal, hover, pressed, inactive);
  }
  this->titleBar->AddChild(baseButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(286, 3);
  closeButton->SetOnClick([this]() { this->OnClickClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const std::string inactive([] {
      constexpr std::array<char, 49> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xDD3913D5D7F97BE1ULL);
      // GLOBAL: gl_25329
      thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> name(source);
      return name.Text();
    }());
    const std::string pressed([] {
      constexpr std::array<char, 48> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0xA5FD29D7AB6DC59FULL);
      // GLOBAL: gl_25331
      thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }());
    const std::string hover([] {
      constexpr std::array<char, 48> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0xF14F79170939C7F1ULL);
      // GLOBAL: gl_25333
      thread_local EncryptedName<sizeof(source), 0xF14F79170939C7F1ULL> name(source);
      return name.Text();
    }());
    const std::string normal([] {
      constexpr std::array<char, 49> source = CRYPT_NAME("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xED796785D1DF137FULL);
      // GLOBAL: gl_25335
      thread_local EncryptedName<sizeof(source), 0xED796785D1DF137FULL> name(source);
      return name.Text();
    }());
    closeButton->SetImages(normal, hover, pressed, inactive);
  }
  this->titleBar->SetSecondButton(closeButton);

  auto* listBox = new CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>();
  this->list = listBox;
  listBox->itemWidth = 265;
  listBox->rowHeight = 18;
  listBox->SetSize(278, 270);
  this->list->Move(21, 17);
  this->list->SetOnSelect([this](EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* line, const std::string& text) { this->OnSelectLine(line, text); });
  this->list->SetBinder([this](EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* line, const std::string& text) { this->BindLine(line, text); });
  this->list->SetImages("", "", "", "");
  this->list->itemX = 0;
  this->list->itemY = 0;
  this->list->scrollBar->SetSize(13, 270);
  this->list->scrollBar->Move(265, 0);
  this->list->scrollBar->step = 18;
  {
    auto* list = this->list;
    const std::string down([] {
      constexpr std::array<char, 31> source = CRYPT_NAME("유저인터페이스\\scroll0down.bmp", 0x83EFBFBB37EDBDBDULL);
      // GLOBAL: gl_25336
      thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }());
    const std::string up([] {
      constexpr std::array<char, 29> source = CRYPT_NAME("유저인터페이스\\scroll0up.bmp", 0x050505679903579BULL);
      // GLOBAL: gl_25337
      thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> name(source);
      return name.Text();
    }());
    list->scrollBar->SetButtonImages(up, down);
  }
  {
    auto* list = this->list;
    const std::string background([] {
      constexpr std::array<char, 30> source = CRYPT_NAME("유저인터페이스\\scroll0mid.bmp", 0xBF2B4B31397FF5A7ULL);
      // GLOBAL: gl_25338
      thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }());
    list->scrollBar->SetBackground(background);
  }
  {
    auto* list = this->list;
    const std::string bottom([] {
      constexpr std::array<char, 35> source = CRYPT_NAME("유저인터페이스\\scroll0bar_down.bmp", 0x2F8B0F37118D157FULL);
      // GLOBAL: gl_25339
      thread_local EncryptedName<sizeof(source), 0x2F8B0F37118D157FULL> name(source);
      return name.Text();
    }());
    const std::string middle([] {
      constexpr std::array<char, 34> source = CRYPT_NAME("유저인터페이스\\scroll0bar_mid.bmp", 0x39FF7FB50DEB998FULL);
      // GLOBAL: gl_25340
      thread_local EncryptedName<sizeof(source), 0x39FF7FB50DEB998FULL> name(source);
      return name.Text();
    }());
    const std::string top([] {
      constexpr std::array<char, 33> source = CRYPT_NAME("유저인터페이스\\scroll0bar_up.bmp", 0x2FE993D1DDE1F197ULL);
      // GLOBAL: gl_25341
      thread_local EncryptedName<sizeof(source), 0x2FE993D1DDE1F197ULL> name(source);
      return name.Text();
    }());
    list->scrollBar->SetThumbImages(top, middle, bottom);
  }
  this->list->AddNewItem();
  this->AddChild(this->list);

  this->tabs = new CUITab();
  this->tabs->SetSize(18, 265);
  this->tabs->Move(3, 18);
  {
    CUITab* tabs = this->tabs;
    const std::function<void(CUITabButton*)> handler = [this](CUITabButton* button) { this->OnChangeTab(button); };
    tabs->onChange = handler;
  }
  this->AddChild(this->tabs);

  CUITabButton* allButton = new CUITabButton();
  allButton->SetSize(18, 34);
  allButton->Move(0, 0);
  allButton->SetOnClick([this]() { this->OnClickButton2(); });
  allButton->disabled = 0;
  allButton->Invalidate();
  allButton->SetId(0);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("전체", 0xAD075B8DB599F1CDULL);
      // GLOBAL: gl_25342
      thread_local EncryptedName<sizeof(source), 0xAD075B8DB599F1CDULL> name(source);
      return name.Text();
    }());
    allButton->SetText(label);
  }
  allButton->SetFitText(0);
  allButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x4941FD9F21BBB9FDULL);
      // GLOBAL: gl_25343
      thread_local EncryptedName<sizeof(source), 0x4941FD9F21BBB9FDULL> name(source);
      return name.Text();
    }());
    allButton->SetFont(font);
  }
  allButton->SetFontHeight(12);
  allButton->SetBold(0);
  allButton->SetTextColors(0, 0, 0, 0x808080);
  allButton->SetShadow(0, 0xFFFFFF);
  allButton->SetOutline(0, 0xFFFFFF);
  allButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  allButton->SetOrientation(1);
  this->tabs->AddChild(allButton);

  CUITabButton* statButton = new CUITabButton();
  statButton->SetSize(18, 34);
  statButton->Move(0, 33);
  statButton->SetOnClick([this]() { this->OnClickButton3(); });
  statButton->disabled = 0;
  statButton->Invalidate();
  statButton->SetId(1);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("스탯", 0x35E5656B5DEDCD75ULL);
      // GLOBAL: gl_25344
      thread_local EncryptedName<sizeof(source), 0x35E5656B5DEDCD75ULL> name(source);
      return name.Text();
    }());
    statButton->SetText(label);
  }
  statButton->SetFitText(0);
  statButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0xB7FD8F670FC7B96BULL);
      // GLOBAL: gl_25345
      thread_local EncryptedName<sizeof(source), 0xB7FD8F670FC7B96BULL> name(source);
      return name.Text();
    }());
    statButton->SetFont(font);
  }
  statButton->SetFontHeight(12);
  statButton->SetBold(0);
  statButton->SetTextColors(0, 0, 0, 0x808080);
  statButton->SetShadow(0, 0xFFFFFF);
  statButton->SetOutline(0, 0xFFFFFF);
  statButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  statButton->SetOrientation(1);
  this->tabs->AddChild(statButton);

  CUITabButton* abilityButton = new CUITabButton();
  abilityButton->SetSize(18, 34);
  abilityButton->Move(0, 66);
  abilityButton->SetOnClick([this]() { this->OnClickButton4(); });
  abilityButton->disabled = 0;
  abilityButton->Invalidate();
  abilityButton->SetId(2);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("어빌", 0xE7978FB5A94DD7FDULL);
      // GLOBAL: gl_25346
      thread_local EncryptedName<sizeof(source), 0xE7978FB5A94DD7FDULL> name(source);
      return name.Text();
    }());
    abilityButton->SetText(label);
  }
  abilityButton->SetFitText(0);
  abilityButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x7115D9F973F913C9ULL);
      // GLOBAL: gl_25347
      thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> name(source);
      return name.Text();
    }());
    abilityButton->SetFont(font);
  }
  abilityButton->SetFontHeight(12);
  abilityButton->SetBold(0);
  abilityButton->SetTextColors(0, 0, 0, 0x808080);
  abilityButton->SetShadow(0, 0xFFFFFF);
  abilityButton->SetOutline(0, 0xFFFFFF);
  abilityButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  abilityButton->SetOrientation(1);
  this->tabs->AddChild(abilityButton);

  CUITabButton* battleButton = new CUITabButton();
  battleButton->SetSize(18, 34);
  battleButton->Move(0, 99);
  battleButton->SetOnClick([this]() { this->OnClickButton5(); });
  battleButton->disabled = 0;
  battleButton->Invalidate();
  battleButton->SetId(3);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("전투", 0xCB1D253B7FE1F563ULL);
      // GLOBAL: gl_25348
      thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> name(source);
      return name.Text();
    }());
    battleButton->SetText(label);
  }
  battleButton->SetFitText(0);
  battleButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x793F370985ED674FULL);
      // GLOBAL: gl_25349
      thread_local EncryptedName<sizeof(source), 0x793F370985ED674FULL> name(source);
      return name.Text();
    }());
    battleButton->SetFont(font);
  }
  battleButton->SetFontHeight(12);
  battleButton->SetBold(0);
  battleButton->SetTextColors(0, 0, 0, 0x808080);
  battleButton->SetShadow(0, 0xFFFFFF);
  battleButton->SetOutline(0, 0xFFFFFF);
  battleButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  battleButton->SetOrientation(1);
  this->tabs->AddChild(battleButton);

  CUITabButton* elementButton = new CUITabButton();
  elementButton->SetSize(18, 34);
  elementButton->Move(0, 132);
  elementButton->SetOnClick([this]() { this->OnClickButton6(); });
  elementButton->disabled = 0;
  elementButton->Invalidate();
  elementButton->SetId(4);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("속성", 0xE55723712993051DULL);
      // GLOBAL: gl_25350
      thread_local EncryptedName<sizeof(source), 0xE55723712993051DULL> name(source);
      return name.Text();
    }());
    elementButton->SetText(label);
  }
  elementButton->SetFitText(0);
  elementButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x7DE349D717D7F123ULL);
      // GLOBAL: gl_25351
      thread_local EncryptedName<sizeof(source), 0x7DE349D717D7F123ULL> name(source);
      return name.Text();
    }());
    elementButton->SetFont(font);
  }
  elementButton->SetFontHeight(12);
  elementButton->SetBold(0);
  elementButton->SetTextColors(0, 0, 0, 0x808080);
  elementButton->SetShadow(0, 0xFFFFFF);
  elementButton->SetOutline(0, 0xFFFFFF);
  elementButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  elementButton->SetOrientation(1);
  this->tabs->AddChild(elementButton);

  CUITabButton* raceButton = new CUITabButton();
  raceButton->SetSize(18, 34);
  raceButton->Move(0, 165);
  raceButton->SetOnClick([this]() { this->OnClickButton7(); });
  raceButton->disabled = 0;
  raceButton->Invalidate();
  raceButton->SetId(5);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("종족", 0x0F15B16B5BAFD57FULL);
      // GLOBAL: gl_25352
      thread_local EncryptedName<sizeof(source), 0x0F15B16B5BAFD57FULL> name(source);
      return name.Text();
    }());
    raceButton->SetText(label);
  }
  raceButton->SetFitText(0);
  raceButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x7941D76335CFA14FULL);
      // GLOBAL: gl_25353
      thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> name(source);
      return name.Text();
    }());
    raceButton->SetFont(font);
  }
  raceButton->SetFontHeight(12);
  raceButton->SetBold(0);
  raceButton->SetTextColors(0, 0, 0, 0x808080);
  raceButton->SetShadow(0, 0xFFFFFF);
  raceButton->SetOutline(0, 0xFFFFFF);
  raceButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  raceButton->SetOrientation(1);
  this->tabs->AddChild(raceButton);

  CUITabButton* skillButton = new CUITabButton();
  skillButton->SetSize(18, 34);
  skillButton->Move(0, 198);
  skillButton->SetOnClick([this]() { this->OnClickButton8(); });
  skillButton->disabled = 0;
  skillButton->Invalidate();
  skillButton->SetId(6);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("스킬", 0x7321C3814F8799E1ULL);
      // GLOBAL: gl_25354
      thread_local EncryptedName<sizeof(source), 0x7321C3814F8799E1ULL> name(source);
      return name.Text();
    }());
    skillButton->SetText(label);
  }
  skillButton->SetFitText(0);
  skillButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x839B27DF8F5967B3ULL);
      // GLOBAL: gl_25355
      thread_local EncryptedName<sizeof(source), 0x839B27DF8F5967B3ULL> name(source);
      return name.Text();
    }());
    skillButton->SetFont(font);
  }
  skillButton->SetFontHeight(12);
  skillButton->SetBold(0);
  skillButton->SetTextColors(0, 0, 0, 0x808080);
  skillButton->SetShadow(0, 0xFFFFFF);
  skillButton->SetOutline(0, 0xFFFFFF);
  skillButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  skillButton->SetOrientation(1);
  this->tabs->AddChild(skillButton);

  CUITabButton* specialButton = new CUITabButton();
  specialButton->SetSize(18, 34);
  specialButton->Move(0, 231);
  specialButton->SetOnClick([this]() { this->OnClickButton9(); });
  specialButton->disabled = 0;
  specialButton->Invalidate();
  specialButton->SetId(7);
  {
    const std::string label([] {
      constexpr std::array<char, 5> source = CRYPT_NAME("특수", 0xF3CB6F7711A71B6DULL);
      // GLOBAL: gl_25356
      thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
      return name.Text();
    }());
    specialButton->SetText(label);
  }
  specialButton->SetFitText(0);
  specialButton->SetAlign(6, 6);
  {
    const std::string font([] {
      constexpr std::array<char, 6> source = CRYPT_NAME("Gulim", 0x452FEF0D7FC95565ULL);
      // GLOBAL: gl_25357
      thread_local EncryptedName<sizeof(source), 0x452FEF0D7FC95565ULL> name(source);
      return name.Text();
    }());
    specialButton->SetFont(font);
  }
  specialButton->SetFontHeight(12);
  specialButton->SetBold(0);
  specialButton->SetTextColors(0, 0, 0, 0x808080);
  specialButton->SetShadow(0, 0xFFFFFF);
  specialButton->SetOutline(0, 0xFFFFFF);
  specialButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  specialButton->SetOrientation(1);
  this->tabs->AddChild(specialButton);

  this->resizer = new CUIResizer();
  this->resizer->SetSize(13, 13);
  this->resizer->Move(286, 294);
  {
    CUIResizer* resizer = this->resizer;
    const std::string image([] {
      constexpr std::array<char, 30> source = CRYPT_NAME("유저인터페이스\\btn_resize.bmp", 0x9FED7DA189490D5DULL);
      // GLOBAL: gl_25358
      thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
      return name.Text();
    }());
    resizer->SetImage(image);
  }
  this->resizer->SetOnResize([this](int deltaWidth, int deltaHeight) { this->Grow(deltaWidth, deltaHeight); });
  this->resizer->minWidth = 300;
  this->resizer->minHeight = 308;
  this->resizer->maxWidth = 300;
  this->resizer->maxHeight = 470;
  this->resizer->stepX = 1;
  this->resizer->stepY = 18;
  this->AddChild(this->resizer);
  this->tabs->Select(allButton);
}

// OFFSET: 0x00A0C9C0
// FUNCTION: fn_15036
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::OnCreate(int width, int height) {
  this->labelText = new CUIStaticText();
  this->labelText->SetSize(44, 12);
  this->labelText->Move(0, 4);
  this->labelText->SetText("");
  this->labelText->SetFitText(0);
  this->labelText->SetAlign(6, 0);
  this->labelText->SetFont(std::string([] {
    constexpr auto sourceText = CRYPT_NAME("Gulim", 0x2BAD156DE3E1F9D9ULL);
    const auto& source = sourceText.data;
    // GLOBAL: gl_25121
    thread_local EncryptedName<sizeof(source), 0x2BAD156DE3E1F9D9ULL> name(source);
    return name.Text();
  }()));
  this->labelText->SetFontHeight(12);
  this->labelText->SetBold(0);
  this->labelText->SetColor(0xFF2A00);
  this->labelText->SetShadow(0, 0xFFFFFF);
  this->labelText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labelText);

  this->valueText = new CUIStaticText();
  this->valueText->SetSize(216, 12);
  this->valueText->Move(49, 4);
  this->valueText->SetText("");
  this->valueText->SetFitText(1);
  this->valueText->SetAlign(0, 0);
  this->valueText->SetFont(std::string([] {
    constexpr auto sourceText = CRYPT_NAME("Gulim", 0x23BBD58F8B5FF5C9ULL);
    const auto& source = sourceText.data;
    // GLOBAL: gl_25122
    thread_local EncryptedName<sizeof(source), 0x23BBD58F8B5FF5C9ULL> name(source);
    return name.Text();
  }()));
  this->valueText->SetFontHeight(12);
  this->valueText->SetBold(0);
  this->valueText->SetColor(0);
  this->valueText->SetShadow(0, 0xFFFFFF);
  this->valueText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->valueText);
}

// OFFSET: 0x00A0EE90
// FUNCTION: fn_15155
void CUIEquipmentPropertiesWnd::OnClickButton1() {
}

// OFFSET: 0x00A0EEA0
// FUNCTION: fn_15156
void CUIEquipmentPropertiesWnd::OnClickButton3() {
}

// OFFSET: 0x00A0EEB0
// FUNCTION: fn_15157
void CUIEquipmentPropertiesWnd::OnClickButton4() {
}

// OFFSET: 0x00A0EEC0
// FUNCTION: fn_15158
void CUIEquipmentPropertiesWnd::OnClickButton5() {
}

// OFFSET: 0x00A0EED0
// FUNCTION: fn_15159
void CUIEquipmentPropertiesWnd::OnClickButton6() {
}

// OFFSET: 0x00A0EEE0
// FUNCTION: fn_15160
void CUIEquipmentPropertiesWnd::OnClickButton7() {
}

// OFFSET: 0x00A0EEF0
// FUNCTION: fn_15161
void CUIEquipmentPropertiesWnd::OnClickButton8() {
}

// OFFSET: 0x00A0EF00
// FUNCTION: fn_15162
void CUIEquipmentPropertiesWnd::OnClickButton9() {
}

// OFFSET: 0x00A0EF10
// FUNCTION: fn_45511
void CUIEquipmentPropertiesWnd::OnClickClose() {
  g_windowMgr.CloseWindow(WID_EQUIPMENTPROPERTIESWND);
}

// OFFSET: 0x00A0EF20
// FUNCTION: fn_15163
void CUIEquipmentPropertiesWnd::OnSelectLine(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* line, const std::string& text) {
}

// OFFSET: 0x00A0EF30
// FUNCTION: fn_15164
void CUIEquipmentPropertiesWnd::BindLine(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* line, const std::string& text) {
  if (text.size() == 0) {
    return;
  }

  line->labelText->SetColor(0xFF2A00);
  line->valueText->SetColor(0);
  std::vector<std::string> parts = Split(text, "#");
  if (parts.size() > 1) {
    std::vector<std::string> title = Split(parts[0], "{~}");
    if (title.size() > 1) {
      line->labelText->SetColor(0x3C00FF);
      line->labelText->SetText(title[1]);
    } else {
      line->labelText->SetText(parts[0]);
    }
    line->valueText->SetText(parts[1]);
  } else {
    line->labelText->SetText("");
    line->valueText->SetText(parts[0].c_str(), parts[0].size());
  }
}

// OFFSET: 0x00A0F0F0
// FUNCTION: fn_15165
void CUIEquipmentPropertiesWnd::Paint() {
  CUIFrameWnd::Paint();
  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
  if (left) {
    const int bottom = this->list->geometry.height + 1;
    for (int y = 0; y < bottom; y += left->image.height) {
      this->DrawBitmap(0, this->list->geometry.y + y, left, false);
    }
  }
  this->FillRegion(this->list->geometry.x - 1, this->list->geometry.y + this->list->geometry.height, this->list->geometry.width + 2, 1, this->m_borderColor);
  this->FillRegion(this->list->geometry.x - 1, this->list->geometry.y, 1, this->list->geometry.height + 1, this->m_borderColor);
}

// OFFSET: 0x00A0F1B0
// FUNCTION: fn_15166
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::Paint() {
  this->ClearSurface((this->index & 1) ? this->oddRowColor : this->evenRowColor);
  this->FillRegion(44, 0, 1, this->geometry.height, this->oddRowColor);
}

// OFFSET: 0x00A0F1F0
// FUNCTION: fn_15167
void CUIEquipmentPropertiesWnd::OnInit() {
  EquipmentProperties::EquipmentPropertiesMgr()->SetWindow(this);
}

// OFFSET: 0x00A0F270
// FUNCTION: fn_15168
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::OnInit() {
  this->LoadSkin(1);
}

// OFFSET: 0x00A0F280
// FUNCTION: fn_15169
void CUIEquipmentPropertiesWnd::Grow(int width, int height) {
  this->Resize(this->geometry.width + width, this->geometry.height + height);
  this->resizer->Move(this->resizer->geometry.x + width, this->resizer->geometry.y + height);
  this->titleBar->Resize(this->titleBar->geometry.width + width, this->titleBar->geometry.height);
  this->list->Resize(this->list->geometry.width + width, this->list->geometry.height + height);
}

// OFFSET: 0x00A0F2F0
// FUNCTION: fn_15170
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::LoadSkin(bool resolveSkin) {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->evenRowColor = RGB(red, green, blue);
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 10, red, green, blue);
  this->oddRowColor = RGB(red, green, blue);
}

// OFFSET: 0x00A0F370
// FUNCTION: fn_15171
void CUIEquipmentPropertiesWnd::SetTab(const int& tab) {
  this->currentTab = tab;
  std::list<std::string> lines;
  EquipmentProperties::EquipmentPropertiesMgr()->GetDescriptions(static_cast<int>(this->currentTab), lines);

  auto* listBox = this->list;
  if (listBox->data != lines) {
    listBox->data.swap(lines);
    listBox->UpdateScrollBar();
  }
}

// OFFSET: 0x00A0F550
// FUNCTION: fn_15172
int CUIEquipmentPropertiesWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == UM_REFRESH) {
    this->SetTab(this->currentTab);
  }
  return 0;
}

// OFFSET: 0x00A0F570
// FUNCTION: fn_15173
void CUIEquipmentPropertiesWnd::OnChangeTab(CUITabButton* button) {
  if (button && button->geometry.id != this->currentTab) {
    const int tab = button->geometry.id;
    this->SetTab(tab);
  }
}

// OFFSET: 0x00A0F5A0
// FUNCTION: fn_15174
void CUIEquipmentPropertiesWnd::OnClickButton2() {
}

// OFFSET: 0x00A0EAF0
// FUNCTION: fn_15151
__declspec(noinline) CUIEquipmentPropertiesWnd::CUIEquipmentPropertiesWnd()
    : titleBar(nullptr), list(nullptr), tabs(nullptr), resizer(nullptr), currentTab(-1) {
}

// OFFSET: 0x00A0ED60
// FUNCTION: fn_15153
CUIEquipmentPropertiesWnd::~CUIEquipmentPropertiesWnd() {
  EquipmentProperties::EquipmentPropertiesMgr()->SetWindow(nullptr);
}

// OFFSET: 0x00A08D60
// FUNCTION: fn_15031
CUIEquipmentPropertiesWnd* CreateEquipmentPropertiesWnd() {
  return new CUIEquipmentPropertiesWnd();
}

// OFFSET: 0x00492C70
// FUNCTION: fn_45548
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateEquipmentPropertiesWnd;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10001, factory);
  return true;
}();
