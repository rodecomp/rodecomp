#include "UI/Item/UIEquipmentPropertiesWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"

#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <new>
#include <string>
#include <windows.h>

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  CEquipmentPropertiesMgr();
  void SetWindow(UIWindow* window);
  void GetDescriptions(const int& group, std::list<std::string>& lines);

  /* 0000 */ unsigned char unknown_0000[0x1FC];
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
    : CUIListItem(), unknown_00E8(nullptr), unknown_00EC(nullptr), evenRowColor(0), oddRowColor(0) {
}

template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* other : this->items) {
    if (other != item) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
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
    this->items.resize(count, nullptr);
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
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::LoadSkin(int resolveSkin) {
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

  for (; it != this->items.end(); ++it) {
    (*it)->index = -1;
    (*it)->Show(0);
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
    : CUIControl(), unknown_0084(), unknown_0088(), unknown_008C(), imageNames(), disabledName(), scrollBar(nullptr),
      itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), unknown_00C5{}, items(), data(),
      selected(static_cast<unsigned int>(-1)), unknown_00E0(1), rebind(0), unknown_00E2{}, onSelect(), binder() {
}

// OFFSET: 0x00A08680
// FUNCTION: fn_15026
template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::~CUIListBoxEx() {
}

// OFFSET: 0x00A08E70
// FUNCTION: fn_15033
void CUIEquipmentPropertiesWnd::Initialize() {
  this->Resize(300, 308);
  this->unknown_00B4 = 0x101;
  this->m_resolveSkin = true;
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
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

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
      const char source[] = {
        '\x47', '\xBC', '\xAB', '\x85', '\xF9', '\xC7', '\xB4', '\xC6', '\x41', '\xF7', '\xAB', '\xB3', '\x84', '\xB3', '\x2D', '\x69',
        '\xE6', '\x60', '\x02', '\x1C', '\x66', '\x60', '\x1F', '\x7F', '\xE2', '\x61', '\x0D', '\x1E', '\x5A', '\x6C', '\x2D', '\x7F',
        '\xEE', '\x67', '\x07', '\x1A', '\x5B', '\x68', '\x03', '\x54', '\xF5', '\x7A', '\x0C', '\x17', '\x4D', '\x27', '\x13', '\x66',
        '\xF7', '\x13'
      };
      // GLOBAL: gl_25321
      thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
      return name.Text();
    }());
    const std::string middle([] {
      const char source[] = {
        '\x65', '\x32', '\xAD', '\x03', '\xA3', '\xE9', '\xB2', '\xCE', '\x63', '\x79', '\xAD', '\x35', '\xDE', '\x9D', '\x2B', '\x61',
        '\xC4', '\xEE', '\x04', '\x9A', '\x3C', '\x4E', '\x19', '\x77', '\xC0', '\xEF', '\x0B', '\x98', '\x00', '\x42', '\x2B', '\x77',
        '\xCC', '\xE9', '\x01', '\x9C', '\x01', '\x46', '\x05', '\x5C', '\xC8', '\xF4', '\x09', '\xD7', '\x01', '\x4A', '\x07', '\x03'
      };
      // GLOBAL: gl_25322
      thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
      return name.Text();
    }());
    const std::string left([] {
      const char source[] = {
        '\xAD', '\x14', '\x55', '\xD7', '\x53', '\x35', '\xD2', '\x08', '\xAB', '\x5F', '\x55', '\xE1', '\x2E', '\x41', '\x4B', '\xA7',
        '\x0C', '\xC8', '\xFC', '\x4E', '\xCC', '\x92', '\x79', '\xB1', '\x08', '\xC9', '\xF3', '\x4C', '\xF0', '\x9E', '\x4B', '\xB1',
        '\x04', '\xCF', '\xF9', '\x48', '\xF1', '\x9A', '\x65', '\x9A', '\x01', '\xDE', '\xF3', '\x59', '\xBD', '\x99', '\x7A', '\xB5',
        '\x6D'
      };
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
  title->SetText(LookupMessage(4029));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string([] {
      const char source[] = {'\x20', '\xDE', '\xFD', '\xCC', '\xCC', '\x3F'};
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
      const char source[] = {
        '\xC9', '\x88', '\xEF', '\x41', '\x7D', '\x93', '\xB8', '\x14', '\xCF', '\xC3', '\xEF', '\x77', '\x00', '\xE7', '\x21', '\xBB',
        '\x68', '\x54', '\x46', '\xD8', '\xE2', '\x34', '\x13', '\xAD', '\x6C', '\x55', '\x49', '\xDA', '\xDE', '\x38', '\x21', '\xAA',
        '\x70', '\x54', '\x70', '\xD9', '\xDC', '\x2E', '\x18', '\x86', '\x66', '\x41', '\x49', '\x95', '\xDF', '\x30', '\x0D', '\xD9'
      };
      // GLOBAL: gl_25325
      thread_local EncryptedName<sizeof(source), 0xD97D5DBDBB2F2709ULL> name(source);
      return name.Text();
    }());
    const std::string pressed([] {
      const char source[] = {
        '\xAF', '\xD2', '\x3B', '\xD9', '\xB1', '\xC9', '\x4A', '\x00', '\xA9', '\x99', '\x3B', '\xEF', '\xCC', '\xBD', '\xD3', '\xAF',
        '\x0E', '\x0E', '\x92', '\x40', '\x2E', '\x6E', '\xE1', '\xB9', '\x0A', '\x0F', '\x9D', '\x42', '\x12', '\x62', '\xD3', '\xBE',
        '\x16', '\x0E', '\xA4', '\x41', '\x10', '\x74', '\xEA', '\x92', '\x00', '\x13', '\xD5', '\x41', '\x1C', '\x77', '\x8F'
      };
      // GLOBAL: gl_25326
      thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> name(source);
      return name.Text();
    }());
    const std::string hover([] {
      const char source[] = {
        '\x83', '\x86', '\x93', '\xDD', '\x15', '\x71', '\x8C', '\x54', '\x85', '\xCD', '\x93', '\xEB', '\x68', '\x05', '\x15', '\xFB',
        '\x22', '\x5A', '\x3A', '\x44', '\x8A', '\xD6', '\x27', '\xED', '\x26', '\x5B', '\x35', '\x46', '\xB6', '\xDA', '\x15', '\xEA',
        '\x3A', '\x5A', '\x0C', '\x45', '\xB4', '\xCC', '\x2C', '\xC6', '\x2C', '\x47', '\x7D', '\x45', '\xB8', '\xCF', '\x49'
      };
      // GLOBAL: gl_25327
      thread_local EncryptedName<sizeof(source), 0x9949BFD527532943ULL> name(source);
      return name.Text();
    }());
    const std::string normal([] {
      const char source[] = {
        '\xF7', '\xE8', '\x6D', '\xC3', '\x7F', '\x2B', '\x6C', '\xDA', '\xF1', '\xA3', '\x6D', '\xF5', '\x02', '\x5F', '\xF5', '\x75',
        '\x56', '\x34', '\xC4', '\x5A', '\xE0', '\x8C', '\xC7', '\x63', '\x52', '\x35', '\xCB', '\x58', '\xDC', '\x80', '\xF5', '\x64',
        '\x4E', '\x34', '\xF2', '\x5B', '\xDE', '\x96', '\xCC', '\x48', '\x58', '\x21', '\xCB', '\x17', '\xDD', '\x88', '\xD9', '\x17'
      };
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
      const char source[] = {
        '\x21', '\xD4', '\x39', '\x2D', '\x15', '\xDD', '\xFC', '\x10', '\x27', '\x9F', '\x39', '\x1B', '\x68', '\xA9', '\x65', '\xBF',
        '\x80', '\x08', '\x90', '\xB4', '\x8A', '\x7A', '\x57', '\xA9', '\x84', '\x09', '\x9F', '\xB6', '\xB6', '\x76', '\x65', '\xAE',
        '\x98', '\x08', '\xA6', '\xB4', '\xB9', '\x7C', '\x4A', '\xB8', '\xBE', '\x14', '\x9F', '\xB1', '\xFB', '\x71', '\x54', '\xAD',
        '\xE1'
      };
      // GLOBAL: gl_25329
      thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> name(source);
      return name.Text();
    }());
    const std::string pressed([] {
      const char source[] = {
        '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xC7',
        '\xFE', '\xB6', '\x04', '\xC8', '\x88', '\x40', '\x93', '\xD1', '\xFA', '\xB7', '\x0B', '\xCA', '\xB4', '\x4C', '\xA1', '\xD6',
        '\xE6', '\xB6', '\x32', '\xC8', '\xBB', '\x46', '\x8E', '\xC0', '\xC0', '\xAA', '\x03', '\x85', '\xB5', '\x44', '\x8D', '\xA5'
      };
      // GLOBAL: gl_25331
      thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }());
    const std::string hover([] {
      const char source[] = {
        '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\x93',
        '\x90', '\xB4', '\x50', '\x6A', '\x48', '\x10', '\x21', '\x85', '\x94', '\xB5', '\x5F', '\x68', '\x74', '\x1C', '\x13', '\x82',
        '\x88', '\xB4', '\x66', '\x6A', '\x7B', '\x16', '\x3C', '\x94', '\xAE', '\xA8', '\x57', '\x27', '\x75', '\x14', '\x3F', '\xF1'
      };
      // GLOBAL: gl_25333
      thread_local EncryptedName<sizeof(source), 0xF14F79170939C7F1ULL> name(source);
      return name.Text();
    }());
    const std::string normal([] {
      const char source[] = {
        '\xBF', '\xBC', '\x1F', '\x2B', '\x45', '\xA9', '\xBC', '\x20', '\xB9', '\xF7', '\x1F', '\x1D', '\x38', '\xDD', '\x25', '\x8F',
        '\x1E', '\x60', '\xB6', '\xB2', '\xDA', '\x0E', '\x17', '\x99', '\x1A', '\x61', '\xB9', '\xB0', '\xE6', '\x02', '\x25', '\x9E',
        '\x06', '\x60', '\x80', '\xB2', '\xE9', '\x08', '\x0A', '\x88', '\x20', '\x7C', '\xB9', '\xB7', '\xAB', '\x05', '\x14', '\x9D',
        '\x7F'
      };
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
      const char source[] = {
        '\x7D', '\x12', '\x2D', '\xCD', '\x7B', '\x71', '\x2A', '\x4E', '\x7B', '\x59', '\x2D', '\xFB', '\x06', '\x05', '\xB3', '\xF0',
        '\xDE', '\xCF', '\x82', '\x5B', '\xD7', '\x8F', '\x8B', '\xEC', '\xCA', '\xD3', '\xC3', '\x55', '\xD6', '\xCF', '\xEF'
      };
      // GLOBAL: gl_25336
      thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }());
    const std::string up([] {
      const char source[] = {
        '\x5B', '\xF8', '\xC3', '\x63', '\xA7', '\xCB', '\xC0', '\xC8', '\x5D', '\xB3', '\xC3', '\x55', '\xDA', '\xBF', '\x59', '\x76',
        '\xF8', '\x25', '\x6C', '\xF5', '\x0B', '\x35', '\x70', '\x75', '\xB5', '\x35', '\x6E', '\xE9', '\x67'
      };
      // GLOBAL: gl_25337
      thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> name(source);
      return name.Text();
    }());
    list->scrollBar->SetButtonImages(up, down);
  }
  {
    auto* list = this->list;
    const std::string background([] {
      const char source[] = {
        '\x67', '\x5A', '\xBF', '\xC3', '\xF1', '\x85', '\xEE', '\x72', '\x61', '\x11', '\xBF', '\xF5', '\x8C', '\xF1', '\x77', '\xCC',
        '\xC4', '\x87', '\x10', '\x55', '\x5D', '\x7B', '\x46', '\xD6', '\xC3', '\xDB', '\x1D', '\x54', '\x41', '\x4B'
      };
      // GLOBAL: gl_25338
      thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }());
    list->scrollBar->SetBackground(background);
  }
  {
    auto* list = this->list;
    const std::string bottom([] {
      const char source[] = {
        '\xBF', '\xBA', '\x4D', '\xEB', '\xF7', '\xC1', '\x4E', '\xE2', '\xB9', '\xF1', '\x4D', '\xDD', '\x8A', '\xB5', '\xD7', '\x5C',
        '\x1C', '\x67', '\xE2', '\x7D', '\x5B', '\x3F', '\xE9', '\x4E', '\x0D', '\x4A', '\xE9', '\x7E', '\x40', '\x61', '\xA5', '\x4D',
        '\x12', '\x65', '\x8D'
      };
      // GLOBAL: gl_25339
      thread_local EncryptedName<sizeof(source), 0x2F8B0F37118D157FULL> name(source);
      return name.Text();
    }());
    const std::string middle([] {
      const char source[] = {
        '\x4F', '\x36', '\x2B', '\xF7', '\x75', '\xB1', '\x3A', '\xF4', '\x49', '\x7D', '\x2B', '\xC1', '\x08', '\xC5', '\xA3', '\x4A',
        '\xEC', '\xEB', '\x84', '\x61', '\xD9', '\x4F', '\x9D', '\x58', '\xFD', '\xC6', '\x86', '\x64', '\xD1', '\x51', '\x9D', '\x54',
        '\xFF', '\x99'
      };
      // GLOBAL: gl_25340
      thread_local EncryptedName<sizeof(source), 0x39FF7FB50DEB998FULL> name(source);
      return name.Text();
    }());
    const std::string top([] {
      const char source[] = {
        '\x57', '\x5E', '\x21', '\x27', '\x11', '\x5D', '\x2C', '\xE2', '\x51', '\x15', '\x21', '\x11', '\x6C', '\x29', '\xB5', '\x5C',
        '\xF4', '\x83', '\x8E', '\xB1', '\xBD', '\xA3', '\x8B', '\x4E', '\xE5', '\xAE', '\x94', '\xAD', '\xFF', '\xF1', '\x84', '\x5F',
        '\x97'
      };
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
      const char source[] = {'\x0D', '\x0D', '\x5A', '\x09', '\x8D'};
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
      const char source[] = {'\xBA', '\xCC', '\xD7', '\x48', '\xF2', '\xFD'};
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
      const char source[] = {'\xC8', '\x77', '\x28', '\x95', '\x6B'};
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
      const char source[] = {'\x2C', '\xCC', '\xAB', '\x66', '\x0A', '\x8F'};
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
      const char source[] = {'\x43', '\x39', '\xF7', '\x5D', '\xB5'};
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
      const char source[] = {'\x8E', '\x66', '\x95', '\x1A', '\x94', '\xD9'};
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
      const char source[] = {'\xA3', '\x09', '\x24', '\x8A', '\x3B'};
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
      const char source[] = {'\x08', '\x12', '\x81', '\xEC', '\x64', '\x37'};
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
      const char source[] = {'\xA1', '\xD6', '\x2F', '\x93', '\x71'};
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
      const char source[] = {'\x64', '\x84', '\xBB', '\x7E', '\xBA', '\x49'};
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
      const char source[] = {'\xBE', '\x6B', '\x6E', '\xEC', '\x6B'};
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
      const char source[] = {'\x08', '\xD4', '\xA3', '\x5C', '\x0E', '\xD7'};
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
      const char source[] = {'\x5C', '\x23', '\x42', '\xFC', '\x81'};
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
      const char source[] = {'\xF4', '\x12', '\x35', '\xE6', '\xB2', '\x27'};
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
      const char source[] = {'\xAB', '\xB4', '\x1B', '\xE7', '\x77'};
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
      const char source[] = {'\x22', '\x20', '\xA5', '\x16', '\x60', '\xEF'};
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
      const char source[] = {
        '\x9D', '\xA2', '\x89', '\x73', '\x61', '\xB3', '\x28', '\x52', '\x9B', '\xE9', '\x89', '\x45', '\x1C', '\xC7', '\xB1', '\xFD',
        '\x29', '\x63', '\x16', '\xFB', '\xC4', '\x0E', '\x84', '\xE5', '\x38', '\x23', '\x2B', '\xE4', '\xD1', '\x7D'
      };
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
  this->unknown_00E8 = new CUIStaticText();
  this->unknown_00E8->SetSize(44, 12);
  this->unknown_00E8->Move(0, 4);
  this->unknown_00E8->SetText("");
  this->unknown_00E8->SetFitText(0);
  this->unknown_00E8->SetAlign(6, 0);
  const char titleFontSource[] = {'\x9E', '\x8C', '\x8D', '\x8A', '\x00', '\x15'};
  // GLOBAL: gl_25121
  thread_local EncryptedName<sizeof(titleFontSource), 0x2BAD156DE3E1F9D9ULL> titleFont(titleFontSource);
  this->unknown_00E8->SetFont(std::string(titleFont.Text()));
  this->unknown_00E8->SetFontHeight(12);
  this->unknown_00E8->SetBold(0);
  this->unknown_00E8->SetColor(0xFF2A00);
  this->unknown_00E8->SetShadow(0, 0xFFFFFF);
  this->unknown_00E8->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = new CUIStaticText();
  this->unknown_00EC->SetSize(216, 12);
  this->unknown_00EC->Move(49, 4);
  this->unknown_00EC->SetText("");
  this->unknown_00EC->SetFitText(1);
  this->unknown_00EC->SetAlign(0, 0);
  const char textFontSource[] = {'\x8E', '\x80', '\x33', '\xE2', '\xE2', '\xD5'};
  // GLOBAL: gl_25122
  thread_local EncryptedName<sizeof(textFontSource), 0x23BBD58F8B5FF5C9ULL> textFont(textFontSource);
  this->unknown_00EC->SetFont(std::string(textFont.Text()));
  this->unknown_00EC->SetFontHeight(12);
  this->unknown_00EC->SetBold(0);
  this->unknown_00EC->SetColor(0);
  this->unknown_00EC->SetShadow(0, 0xFFFFFF);
  this->unknown_00EC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00EC);
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
  g_windowMgr.CloseWindow(10001);
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

  line->unknown_00E8->SetColor(0xFF2A00);
  line->unknown_00EC->SetColor(0);
  std::vector<std::string> parts = Split(text, "#");
  if (parts.size() > 1) {
    std::vector<std::string> title = Split(parts[0], "{~}");
    if (title.size() > 1) {
      line->unknown_00E8->SetColor(0x3C00FF);
      line->unknown_00E8->SetText(title[1]);
    } else {
      line->unknown_00E8->SetText(parts[0]);
    }
    line->unknown_00EC->SetText(parts[1]);
  } else {
    line->unknown_00E8->SetText("");
    line->unknown_00EC->SetText(parts[0].c_str(), parts[0].size());
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
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::LoadSkin(int resolveSkin) {
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
  const int group = this->currentTab;
  EquipmentProperties::EquipmentPropertiesMgr()->GetDescriptions(group, lines);

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
  if (message == 60) {
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
