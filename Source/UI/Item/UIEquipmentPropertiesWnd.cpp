#include "UI/Item/UIEquipmentPropertiesWnd.h"

#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"

#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <string>
#include <windows.h>

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  static CEquipmentPropertiesMgr* GetInstance();
  void SetWindow(UIWindow* window);
  void GetDescriptions(const int& group, std::list<std::string>& lines);
};
}

template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Paint();
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Slot35(int);
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::ClearItems();

void CUIListItem::SetImageResources(CBitmapRes* normal, CBitmapRes* hover, CBitmapRes* pressed, CBitmapRes* disabled) {
  auto& images = *reinterpret_cast<std::map<int, CBitmapRes*>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images.clear();
  images[0] = normal;
  images[1] = hover;
  images[2] = pressed;
  this->disabledImage = disabled;
  this->Invalidate();
}

// OFFSET: 0x00A0EBF0
// FUNCTION: fn_15152
EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::CUIItemEquipmentPropertiesLine(UIWindowContext& context)
    : CUIListItem(context), unknown_00E8(nullptr), unknown_00EC(nullptr), evenRowColor(0), oddRowColor(0) {
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

  if (this->rowHeight <= this->geometry.height) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count, nullptr);
    for (EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine*& slot : this->items) {
      EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* item =
          new EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine(*this->m_context);
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
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Slot53(int resolveSkin) {
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
bool CUIEquipmentPropertiesWnd::Slot02() {
  return true;
}

// OFFSET: 0x00A08420
// FUNCTION: fn_15022
bool CUIEquipmentPropertiesWnd::Slot03() {
  return true;
}

// OFFSET: 0x00A08430
// FUNCTION: fn_15023
bool CUIEquipmentPropertiesWnd::Slot08() {
  return true;
}

// OFFSET: 0x00A084C0
// FUNCTION: fn_15025
template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), unknown_0084(), unknown_0088(), unknown_008C(), imageNames(), disabledName(), scrollBar(nullptr),
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
void CUIEquipmentPropertiesWnd::Slot53() {
  this->Resize(300, 308);
  this->unknown_00B4 = 0x101;
  this->m_resolveSkin = true;
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
  this->SetSkinColors(0, 0, 3, 1);
  this->m_buttonBarStyle = 1;
}

// OFFSET: 0x00A08F20
// FUNCTION: fn_15034
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
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
void CUIEquipmentPropertiesWnd::Slot15(int width, int height) {
  this->titleBar = new CUITitleBar(*this->m_context);
  this->titleBar->SetSize(300, 17);
  this->titleBar->Move(0, 0);
  {
    const std::uint8_t rightSource[] = {
      0x47, 0xBC, 0xAB, 0x85, 0xF9, 0xC7, 0xB4, 0xC6, 0x41, 0xF7, 0xAB, 0xB3, 0x84, 0xB3, 0x2D, 0x69,
      0xE6, 0x60, 0x02, 0x1C, 0x66, 0x60, 0x1F, 0x7F, 0xE2, 0x61, 0x0D, 0x1E, 0x5A, 0x6C, 0x2D, 0x7F,
      0xEE, 0x67, 0x07, 0x1A, 0x5B, 0x68, 0x03, 0x54, 0xF5, 0x7A, 0x0C, 0x17, 0x4D, 0x27, 0x13, 0x66,
      0xF7, 0x13
    };
    // GLOBAL: gl_25321
    thread_local EncryptedName<sizeof(rightSource), 0x0B7109397F6B1387ULL> rightName(rightSource);
    const std::string right(rightName.Text());
    const std::uint8_t middleSource[] = {
      0x65, 0x32, 0xAD, 0x03, 0xA3, 0xE9, 0xB2, 0xCE, 0x63, 0x79, 0xAD, 0x35, 0xDE, 0x9D, 0x2B, 0x61,
      0xC4, 0xEE, 0x04, 0x9A, 0x3C, 0x4E, 0x19, 0x77, 0xC0, 0xEF, 0x0B, 0x98, 0x00, 0x42, 0x2B, 0x77,
      0xCC, 0xE9, 0x01, 0x9C, 0x01, 0x46, 0x05, 0x5C, 0xC8, 0xF4, 0x09, 0xD7, 0x01, 0x4A, 0x07, 0x03
    };
    // GLOBAL: gl_25322
    thread_local EncryptedName<sizeof(middleSource), 0x03772763F96D9DA5ULL> middleName(middleSource);
    const std::string middle(middleName.Text());
    const std::uint8_t leftSource[] = {
      0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0xA7,
      0x0C, 0xC8, 0xFC, 0x4E, 0xCC, 0x92, 0x79, 0xB1, 0x08, 0xC9, 0xF3, 0x4C, 0xF0, 0x9E, 0x4B, 0xB1,
      0x04, 0xCF, 0xF9, 0x48, 0xF1, 0x9A, 0x65, 0x9A, 0x01, 0xDE, 0xF3, 0x59, 0xBD, 0x99, 0x7A, 0xB5,
      0x6D
    };
    // GLOBAL: gl_25323
    thread_local EncryptedName<sizeof(leftSource), 0xC517FB932D95BB6DULL> leftName(leftSource);
    const std::string left(leftName.Text());
    this->titleBar->SetImages(left, middle, right);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 18);
  title->Move(18, 0);
  const char* titleText = LookupMessage(4029);
  title->SetText(titleText, std::strlen(titleText));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const std::uint8_t titleFontSource[] = {0x20, 0xDE, 0xFD, 0xCC, 0xCC, 0x3F};
    // GLOBAL: gl_25324
    thread_local EncryptedName<sizeof(titleFontSource), 0x13E33FA1A591AB67ULL> titleFontName(titleFontSource);
    title->SetFont(std::string(titleFontName.Text()));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* baseButton = new CUIImageButton(*this->m_context);
  baseButton->SetSize(11, 11);
  baseButton->Move(3, 3);
  baseButton->SetOnClick([this]() { this->OnClickButton1(); });
  baseButton->disabled = 0;
  baseButton->Invalidate();
  {
    const std::uint8_t inactiveSource[] = {
      0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21, 0xBB,
      0x68, 0x54, 0x46, 0xD8, 0xE2, 0x34, 0x13, 0xAD, 0x6C, 0x55, 0x49, 0xDA, 0xDE, 0x38, 0x21, 0xAA,
      0x70, 0x54, 0x70, 0xD9, 0xDC, 0x2E, 0x18, 0x86, 0x66, 0x41, 0x49, 0x95, 0xDF, 0x30, 0x0D, 0xD9
    };
    // GLOBAL: gl_25325
    thread_local EncryptedName<sizeof(inactiveSource), 0xD97D5DBDBB2F2709ULL> inactiveName(inactiveSource);
    const std::string inactive(inactiveName.Text());
    const std::uint8_t pressedSource[] = {
      0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0xAF,
      0x0E, 0x0E, 0x92, 0x40, 0x2E, 0x6E, 0xE1, 0xB9, 0x0A, 0x0F, 0x9D, 0x42, 0x12, 0x62, 0xD3, 0xBE,
      0x16, 0x0E, 0xA4, 0x41, 0x10, 0x74, 0xEA, 0x92, 0x00, 0x13, 0xD5, 0x41, 0x1C, 0x77, 0x8F
    };
    // GLOBAL: gl_25326
    thread_local EncryptedName<sizeof(pressedSource), 0xCD8F077123FB7D6FULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xFB,
      0x22, 0x5A, 0x3A, 0x44, 0x8A, 0xD6, 0x27, 0xED, 0x26, 0x5B, 0x35, 0x46, 0xB6, 0xDA, 0x15, 0xEA,
      0x3A, 0x5A, 0x0C, 0x45, 0xB4, 0xCC, 0x2C, 0xC6, 0x2C, 0x47, 0x7D, 0x45, 0xB8, 0xCF, 0x49
    };
    // GLOBAL: gl_25327
    thread_local EncryptedName<sizeof(hoverSource), 0x9949BFD527532943ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x75,
      0x56, 0x34, 0xC4, 0x5A, 0xE0, 0x8C, 0xC7, 0x63, 0x52, 0x35, 0xCB, 0x58, 0xDC, 0x80, 0xF5, 0x64,
      0x4E, 0x34, 0xF2, 0x5B, 0xDE, 0x96, 0xCC, 0x48, 0x58, 0x21, 0xCB, 0x17, 0xDD, 0x88, 0xD9, 0x17
    };
    // GLOBAL: gl_25328
    thread_local EncryptedName<sizeof(normalSource), 0x17A9E5BF39AD4737ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    baseButton->SetImages(normal, hover, pressed, inactive);
  }
  this->titleBar->AddChild(baseButton);

  CUIImageButton* closeButton = new CUIImageButton(*this->m_context);
  closeButton->SetSize(11, 11);
  closeButton->Move(286, 3);
  closeButton->SetOnClick([this]() { this->OnClickClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const std::uint8_t inactiveSource[] = {
      0x21, 0xD4, 0x39, 0x2D, 0x15, 0xDD, 0xFC, 0x10, 0x27, 0x9F, 0x39, 0x1B, 0x68, 0xA9, 0x65, 0xBF,
      0x80, 0x08, 0x90, 0xB4, 0x8A, 0x7A, 0x57, 0xA9, 0x84, 0x09, 0x9F, 0xB6, 0xB6, 0x76, 0x65, 0xAE,
      0x98, 0x08, 0xA6, 0xB4, 0xB9, 0x7C, 0x4A, 0xB8, 0xBE, 0x14, 0x9F, 0xB1, 0xFB, 0x71, 0x54, 0xAD,
      0xE1
    };
    // GLOBAL: gl_25329
    thread_local EncryptedName<sizeof(inactiveSource), 0xDD3913D5D7F97BE1ULL> inactiveName(inactiveSource);
    const std::string inactive(inactiveName.Text());
    const std::uint8_t pressedSource[] = {
      0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xC7,
      0xFE, 0xB6, 0x04, 0xC8, 0x88, 0x40, 0x93, 0xD1, 0xFA, 0xB7, 0x0B, 0xCA, 0xB4, 0x4C, 0xA1, 0xD6,
      0xE6, 0xB6, 0x32, 0xC8, 0xBB, 0x46, 0x8E, 0xC0, 0xC0, 0xAA, 0x03, 0x85, 0xB5, 0x44, 0x8D, 0xA5
    };
    // GLOBAL: gl_25331
    thread_local EncryptedName<sizeof(pressedSource), 0xA5FD29D7AB6DC59FULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x93,
      0x90, 0xB4, 0x50, 0x6A, 0x48, 0x10, 0x21, 0x85, 0x94, 0xB5, 0x5F, 0x68, 0x74, 0x1C, 0x13, 0x82,
      0x88, 0xB4, 0x66, 0x6A, 0x7B, 0x16, 0x3C, 0x94, 0xAE, 0xA8, 0x57, 0x27, 0x75, 0x14, 0x3F, 0xF1
    };
    // GLOBAL: gl_25333
    thread_local EncryptedName<sizeof(hoverSource), 0xF14F79170939C7F1ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0xBF, 0xBC, 0x1F, 0x2B, 0x45, 0xA9, 0xBC, 0x20, 0xB9, 0xF7, 0x1F, 0x1D, 0x38, 0xDD, 0x25, 0x8F,
      0x1E, 0x60, 0xB6, 0xB2, 0xDA, 0x0E, 0x17, 0x99, 0x1A, 0x61, 0xB9, 0xB0, 0xE6, 0x02, 0x25, 0x9E,
      0x06, 0x60, 0x80, 0xB2, 0xE9, 0x08, 0x0A, 0x88, 0x20, 0x7C, 0xB9, 0xB7, 0xAB, 0x05, 0x14, 0x9D,
      0x7F
    };
    // GLOBAL: gl_25335
    thread_local EncryptedName<sizeof(normalSource), 0xED796785D1DF137FULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    closeButton->SetImages(normal, hover, pressed, inactive);
  }
  this->titleBar->SetSecondButton(closeButton);

  this->list = new CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>(*this->m_context);
  this->list->itemWidth = 265;
  this->list->rowHeight = 18;
  this->list->SetSize(278, 270);
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
    const std::uint8_t downSource[] = {
      0x7D, 0x12, 0x2D, 0xCD, 0x7B, 0x71, 0x2A, 0x4E, 0x7B, 0x59, 0x2D, 0xFB, 0x06, 0x05, 0xB3, 0xF0,
      0xDE, 0xCF, 0x82, 0x5B, 0xD7, 0x8F, 0x8B, 0xEC, 0xCA, 0xD3, 0xC3, 0x55, 0xD6, 0xCF, 0xEF
    };
    // GLOBAL: gl_25336
    thread_local EncryptedName<sizeof(downSource), 0x83EFBFBB37EDBDBDULL> downName(downSource);
    const std::string down(downName.Text());
    const std::uint8_t upSource[] = {
      0x5B, 0xF8, 0xC3, 0x63, 0xA7, 0xCB, 0xC0, 0xC8, 0x5D, 0xB3, 0xC3, 0x55, 0xDA, 0xBF, 0x59, 0x76,
      0xF8, 0x25, 0x6C, 0xF5, 0x0B, 0x35, 0x70, 0x75, 0xB5, 0x35, 0x6E, 0xE9, 0x67
    };
    // GLOBAL: gl_25337
    thread_local EncryptedName<sizeof(upSource), 0x050505679903579BULL> upName(upSource);
    const std::string up(upName.Text());
    this->list->scrollBar->SetButtonImages(up, down);
  }
  {
    const std::uint8_t backgroundSource[] = {
      0x67, 0x5A, 0xBF, 0xC3, 0xF1, 0x85, 0xEE, 0x72, 0x61, 0x11, 0xBF, 0xF5, 0x8C, 0xF1, 0x77, 0xCC,
      0xC4, 0x87, 0x10, 0x55, 0x5D, 0x7B, 0x46, 0xD6, 0xC3, 0xDB, 0x1D, 0x54, 0x41, 0x4B
    };
    // GLOBAL: gl_25338
    thread_local EncryptedName<sizeof(backgroundSource), 0xBF2B4B31397FF5A7ULL> backgroundName(backgroundSource);
    this->list->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const std::uint8_t thumbBottomSource[] = {
      0xBF, 0xBA, 0x4D, 0xEB, 0xF7, 0xC1, 0x4E, 0xE2, 0xB9, 0xF1, 0x4D, 0xDD, 0x8A, 0xB5, 0xD7, 0x5C,
      0x1C, 0x67, 0xE2, 0x7D, 0x5B, 0x3F, 0xE9, 0x4E, 0x0D, 0x4A, 0xE9, 0x7E, 0x40, 0x61, 0xA5, 0x4D,
      0x12, 0x65, 0x8D
    };
    // GLOBAL: gl_25339
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x2F8B0F37118D157FULL> thumbBottomName(thumbBottomSource);
    const std::string bottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
      0x4F, 0x36, 0x2B, 0xF7, 0x75, 0xB1, 0x3A, 0xF4, 0x49, 0x7D, 0x2B, 0xC1, 0x08, 0xC5, 0xA3, 0x4A,
      0xEC, 0xEB, 0x84, 0x61, 0xD9, 0x4F, 0x9D, 0x58, 0xFD, 0xC6, 0x86, 0x64, 0xD1, 0x51, 0x9D, 0x54,
      0xFF, 0x99
    };
    // GLOBAL: gl_25340
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x39FF7FB50DEB998FULL> thumbMiddleName(thumbMiddleSource);
    const std::string middle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
      0x57, 0x5E, 0x21, 0x27, 0x11, 0x5D, 0x2C, 0xE2, 0x51, 0x15, 0x21, 0x11, 0x6C, 0x29, 0xB5, 0x5C,
      0xF4, 0x83, 0x8E, 0xB1, 0xBD, 0xA3, 0x8B, 0x4E, 0xE5, 0xAE, 0x94, 0xAD, 0xFF, 0xF1, 0x84, 0x5F,
      0x97
    };
    // GLOBAL: gl_25341
    thread_local EncryptedName<sizeof(thumbTopSource), 0x2FE993D1DDE1F197ULL> thumbTopName(thumbTopSource);
    const std::string top(thumbTopName.Text());
    this->list->scrollBar->SetThumbImages(top, middle, bottom);
  }
  this->list->AddNewItem();
  this->AddChild(this->list);

  this->tabs = new CUITab(*this->m_context);
  this->tabs->SetSize(18, 265);
  this->tabs->Move(3, 18);
  this->tabs->SetOnChange([this](CUITabButton* button) { this->OnChangeTab(button); });
  this->AddChild(this->tabs);

  CUITabButton* allButton = new CUITabButton(*this->m_context);
  allButton->SetSize(18, 34);
  allButton->Move(0, 0);
  allButton->SetOnClick([this]() { this->OnClickButton2(); });
  allButton->disabled = 0;
  allButton->Invalidate();
  allButton->SetId(0);
  {
    const std::uint8_t allLabelSource[] = {0x0D, 0x0D, 0x5A, 0x09, 0x8D};
    // GLOBAL: gl_25342
    thread_local EncryptedName<sizeof(allLabelSource), 0xAD075B8DB599F1CDULL> allLabelName(allLabelSource);
    allButton->SetText(std::string(allLabelName.Text()));
  }
  allButton->SetFitText(0);
  allButton->SetAlign(6, 6);
  {
    const std::uint8_t allFontSource[] = {0xBA, 0xCC, 0xD7, 0x48, 0xF2, 0xFD};
    // GLOBAL: gl_25343
    thread_local EncryptedName<sizeof(allFontSource), 0x4941FD9F21BBB9FDULL> allFontName(allFontSource);
    allButton->SetFont(std::string(allFontName.Text()));
  }
  allButton->SetFontHeight(12);
  allButton->SetBold(0);
  allButton->SetTextColors(0, 0, 0, 0x808080);
  allButton->SetShadow(0, 0xFFFFFF);
  allButton->SetOutline(0, 0xFFFFFF);
  allButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  allButton->SetOrientation(1);
  this->tabs->AddChild(allButton);

  CUITabButton* statButton = new CUITabButton(*this->m_context);
  statButton->SetSize(18, 34);
  statButton->Move(0, 33);
  statButton->SetOnClick([this]() { this->OnClickButton3(); });
  statButton->disabled = 0;
  statButton->Invalidate();
  statButton->SetId(1);
  {
    const std::uint8_t statLabelSource[] = {0xC8, 0x77, 0x28, 0x95, 0x6B};
    // GLOBAL: gl_25344
    thread_local EncryptedName<sizeof(statLabelSource), 0x35E5656B5DEDCD75ULL> statLabelName(statLabelSource);
    statButton->SetText(std::string(statLabelName.Text()));
  }
  statButton->SetFitText(0);
  statButton->SetAlign(6, 6);
  {
    const std::uint8_t statFontSource[] = {0x2C, 0xCC, 0xAB, 0x66, 0x0A, 0x8F};
    // GLOBAL: gl_25345
    thread_local EncryptedName<sizeof(statFontSource), 0xB7FD8F670FC7B96BULL> statFontName(statFontSource);
    statButton->SetFont(std::string(statFontName.Text()));
  }
  statButton->SetFontHeight(12);
  statButton->SetBold(0);
  statButton->SetTextColors(0, 0, 0, 0x808080);
  statButton->SetShadow(0, 0xFFFFFF);
  statButton->SetOutline(0, 0xFFFFFF);
  statButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  statButton->SetOrientation(1);
  this->tabs->AddChild(statButton);

  CUITabButton* abilityButton = new CUITabButton(*this->m_context);
  abilityButton->SetSize(18, 34);
  abilityButton->Move(0, 66);
  abilityButton->SetOnClick([this]() { this->OnClickButton4(); });
  abilityButton->disabled = 0;
  abilityButton->Invalidate();
  abilityButton->SetId(2);
  {
    const std::uint8_t abilityLabelSource[] = {0x43, 0x39, 0xF7, 0x5D, 0xB5};
    // GLOBAL: gl_25346
    thread_local EncryptedName<sizeof(abilityLabelSource), 0xE7978FB5A94DD7FDULL> abilityLabelName(abilityLabelSource);
    abilityButton->SetText(std::string(abilityLabelName.Text()));
  }
  abilityButton->SetFitText(0);
  abilityButton->SetAlign(6, 6);
  {
    const std::uint8_t abilityFontSource[] = {0x8E, 0x66, 0x95, 0x1A, 0x94, 0xD9};
    // GLOBAL: gl_25347
    thread_local EncryptedName<sizeof(abilityFontSource), 0x7115D9F973F913C9ULL> abilityFontName(abilityFontSource);
    abilityButton->SetFont(std::string(abilityFontName.Text()));
  }
  abilityButton->SetFontHeight(12);
  abilityButton->SetBold(0);
  abilityButton->SetTextColors(0, 0, 0, 0x808080);
  abilityButton->SetShadow(0, 0xFFFFFF);
  abilityButton->SetOutline(0, 0xFFFFFF);
  abilityButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  abilityButton->SetOrientation(1);
  this->tabs->AddChild(abilityButton);

  CUITabButton* battleButton = new CUITabButton(*this->m_context);
  battleButton->SetSize(18, 34);
  battleButton->Move(0, 99);
  battleButton->SetOnClick([this]() { this->OnClickButton5(); });
  battleButton->disabled = 0;
  battleButton->Invalidate();
  battleButton->SetId(3);
  {
    const std::uint8_t battleLabelSource[] = {0xA3, 0x09, 0x24, 0x8A, 0x3B};
    // GLOBAL: gl_25348
    thread_local EncryptedName<sizeof(battleLabelSource), 0xCB1D253B7FE1F563ULL> battleLabelName(battleLabelSource);
    battleButton->SetText(std::string(battleLabelName.Text()));
  }
  battleButton->SetFitText(0);
  battleButton->SetAlign(6, 6);
  {
    const std::uint8_t battleFontSource[] = {0x08, 0x12, 0x81, 0xEC, 0x64, 0x37};
    // GLOBAL: gl_25349
    thread_local EncryptedName<sizeof(battleFontSource), 0x793F370985ED674FULL> battleFontName(battleFontSource);
    battleButton->SetFont(std::string(battleFontName.Text()));
  }
  battleButton->SetFontHeight(12);
  battleButton->SetBold(0);
  battleButton->SetTextColors(0, 0, 0, 0x808080);
  battleButton->SetShadow(0, 0xFFFFFF);
  battleButton->SetOutline(0, 0xFFFFFF);
  battleButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  battleButton->SetOrientation(1);
  this->tabs->AddChild(battleButton);

  CUITabButton* elementButton = new CUITabButton(*this->m_context);
  elementButton->SetSize(18, 34);
  elementButton->Move(0, 132);
  elementButton->SetOnClick([this]() { this->OnClickButton6(); });
  elementButton->disabled = 0;
  elementButton->Invalidate();
  elementButton->SetId(4);
  {
    const std::uint8_t elementLabelSource[] = {0xA1, 0xD6, 0x2F, 0x93, 0x71};
    // GLOBAL: gl_25350
    thread_local EncryptedName<sizeof(elementLabelSource), 0xE55723712993051DULL> elementLabelName(elementLabelSource);
    elementButton->SetText(std::string(elementLabelName.Text()));
  }
  elementButton->SetFitText(0);
  elementButton->SetAlign(6, 6);
  {
    const std::uint8_t elementFontSource[] = {0x64, 0x84, 0xBB, 0x7E, 0xBA, 0x49};
    // GLOBAL: gl_25351
    thread_local EncryptedName<sizeof(elementFontSource), 0x7DE349D717D7F123ULL> elementFontName(elementFontSource);
    elementButton->SetFont(std::string(elementFontName.Text()));
  }
  elementButton->SetFontHeight(12);
  elementButton->SetBold(0);
  elementButton->SetTextColors(0, 0, 0, 0x808080);
  elementButton->SetShadow(0, 0xFFFFFF);
  elementButton->SetOutline(0, 0xFFFFFF);
  elementButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  elementButton->SetOrientation(1);
  this->tabs->AddChild(elementButton);

  CUITabButton* raceButton = new CUITabButton(*this->m_context);
  raceButton->SetSize(18, 34);
  raceButton->Move(0, 165);
  raceButton->SetOnClick([this]() { this->OnClickButton7(); });
  raceButton->disabled = 0;
  raceButton->Invalidate();
  raceButton->SetId(5);
  {
    const std::uint8_t raceLabelSource[] = {0xBE, 0x6B, 0x6E, 0xEC, 0x6B};
    // GLOBAL: gl_25352
    thread_local EncryptedName<sizeof(raceLabelSource), 0x0F15B16B5BAFD57FULL> raceLabelName(raceLabelSource);
    raceButton->SetText(std::string(raceLabelName.Text()));
  }
  raceButton->SetFitText(0);
  raceButton->SetAlign(6, 6);
  {
    const std::uint8_t raceFontSource[] = {0x08, 0xD4, 0xA3, 0x5C, 0x0E, 0xD7};
    // GLOBAL: gl_25353
    thread_local EncryptedName<sizeof(raceFontSource), 0x7941D76335CFA14FULL> raceFontName(raceFontSource);
    raceButton->SetFont(std::string(raceFontName.Text()));
  }
  raceButton->SetFontHeight(12);
  raceButton->SetBold(0);
  raceButton->SetTextColors(0, 0, 0, 0x808080);
  raceButton->SetShadow(0, 0xFFFFFF);
  raceButton->SetOutline(0, 0xFFFFFF);
  raceButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  raceButton->SetOrientation(1);
  this->tabs->AddChild(raceButton);

  CUITabButton* skillButton = new CUITabButton(*this->m_context);
  skillButton->SetSize(18, 34);
  skillButton->Move(0, 198);
  skillButton->SetOnClick([this]() { this->OnClickButton8(); });
  skillButton->disabled = 0;
  skillButton->Invalidate();
  skillButton->SetId(6);
  {
    const std::uint8_t skillLabelSource[] = {0x5C, 0x23, 0x42, 0xFC, 0x81};
    // GLOBAL: gl_25354
    thread_local EncryptedName<sizeof(skillLabelSource), 0x7321C3814F8799E1ULL> skillLabelName(skillLabelSource);
    skillButton->SetText(std::string(skillLabelName.Text()));
  }
  skillButton->SetFitText(0);
  skillButton->SetAlign(6, 6);
  {
    const std::uint8_t skillFontSource[] = {0xF4, 0x12, 0x35, 0xE6, 0xB2, 0x27};
    // GLOBAL: gl_25355
    thread_local EncryptedName<sizeof(skillFontSource), 0x839B27DF8F5967B3ULL> skillFontName(skillFontSource);
    skillButton->SetFont(std::string(skillFontName.Text()));
  }
  skillButton->SetFontHeight(12);
  skillButton->SetBold(0);
  skillButton->SetTextColors(0, 0, 0, 0x808080);
  skillButton->SetShadow(0, 0xFFFFFF);
  skillButton->SetOutline(0, 0xFFFFFF);
  skillButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  skillButton->SetOrientation(1);
  this->tabs->AddChild(skillButton);

  CUITabButton* specialButton = new CUITabButton(*this->m_context);
  specialButton->SetSize(18, 34);
  specialButton->Move(0, 231);
  specialButton->SetOnClick([this]() { this->OnClickButton9(); });
  specialButton->disabled = 0;
  specialButton->Invalidate();
  specialButton->SetId(7);
  {
    const std::uint8_t specialLabelSource[] = {0xAB, 0xB4, 0x1B, 0xE7, 0x77};
    // GLOBAL: gl_25356
    thread_local EncryptedName<sizeof(specialLabelSource), 0xF3CB6F7711A71B6DULL> specialLabelName(specialLabelSource);
    specialButton->SetText(std::string(specialLabelName.Text()));
  }
  specialButton->SetFitText(0);
  specialButton->SetAlign(6, 6);
  {
    const std::uint8_t specialFontSource[] = {0x22, 0x20, 0xA5, 0x16, 0x60, 0xEF};
    // GLOBAL: gl_25357
    thread_local EncryptedName<sizeof(specialFontSource), 0x452FEF0D7FC95565ULL> specialFontName(specialFontSource);
    specialButton->SetFont(std::string(specialFontName.Text()));
  }
  specialButton->SetFontHeight(12);
  specialButton->SetBold(0);
  specialButton->SetTextColors(0, 0, 0, 0x808080);
  specialButton->SetShadow(0, 0xFFFFFF);
  specialButton->SetOutline(0, 0xFFFFFF);
  specialButton->SetSkinColors(0, 0, 5, 2, 3, 1);
  specialButton->SetOrientation(1);
  this->tabs->AddChild(specialButton);

  this->resizer = new CUIResizer(*this->m_context);
  this->resizer->SetSize(13, 13);
  this->resizer->Move(286, 294);
  {
    const std::uint8_t resizeSource[] = {
      0x9D, 0xA2, 0x89, 0x73, 0x61, 0xB3, 0x28, 0x52, 0x9B, 0xE9, 0x89, 0x45, 0x1C, 0xC7, 0xB1, 0xFD,
      0x29, 0x63, 0x16, 0xFB, 0xC4, 0x0E, 0x84, 0xE5, 0x38, 0x23, 0x2B, 0xE4, 0xD1, 0x7D
    };
    // GLOBAL: gl_25358
    thread_local EncryptedName<sizeof(resizeSource), 0x9FED7DA189490D5DULL> resizeName(resizeSource);
    this->resizer->SetImage(std::string(resizeName.Text()));
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
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::Slot15(int width, int height) {
  this->unknown_00E8 = new CUIStaticText(*this->m_context);
  this->unknown_00E8->SetSize(44, 12);
  this->unknown_00E8->Move(0, 4);
  this->unknown_00E8->SetText("", 0);
  this->unknown_00E8->SetFitText(0);
  this->unknown_00E8->SetAlign(6, 0);
  const std::uint8_t titleFontSource[] = {0x9E, 0x8C, 0x8D, 0x8A, 0x00, 0x15};
  // GLOBAL: gl_25121
  thread_local EncryptedName<sizeof(titleFontSource), 0x2BAD156DE3E1F9D9ULL> titleFont(titleFontSource);
  this->unknown_00E8->SetFont(std::string(titleFont.Text()));
  this->unknown_00E8->SetFontHeight(12);
  this->unknown_00E8->SetBold(0);
  this->unknown_00E8->SetColor(0xFF2A00);
  this->unknown_00E8->SetShadow(0, 0xFFFFFF);
  this->unknown_00E8->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = new CUIStaticText(*this->m_context);
  this->unknown_00EC->SetSize(216, 12);
  this->unknown_00EC->Move(49, 4);
  this->unknown_00EC->SetText("", 0);
  this->unknown_00EC->SetFitText(1);
  this->unknown_00EC->SetAlign(0, 0);
  const std::uint8_t textFontSource[] = {0x8E, 0x80, 0x33, 0xE2, 0xE2, 0xD5};
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
      line->unknown_00E8->SetText(title[1].c_str(), title[1].size());
    } else {
      line->unknown_00E8->SetText(parts[0].c_str(), parts[0].size());
    }
    line->unknown_00EC->SetText(parts[1].c_str(), parts[1].size());
  } else {
    line->unknown_00E8->SetText("", 0);
    line->unknown_00EC->SetText(parts[0].c_str(), parts[0].size());
  }
}

// OFFSET: 0x00A0F0F0
// FUNCTION: fn_15165
void CUIEquipmentPropertiesWnd::Paint() {
  CUIFrameWnd::Paint();
  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
  if (left) {
    const int bottom = this->list->geometry.height + 1;
    for (int y = 0; y < bottom; y += left->image.height) {
      this->DrawBitmap(0, this->list->geometry.y + y, &left->image, false);
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
void CUIEquipmentPropertiesWnd::Slot16() {
  EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->SetWindow(this);
}

// OFFSET: 0x00A0F270
// FUNCTION: fn_15168
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::Slot16() {
  this->Slot53(1);
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
void EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine::Slot53(int resolveSkin) {
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
  EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->GetDescriptions(group, lines);

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
CUIEquipmentPropertiesWnd::CUIEquipmentPropertiesWnd()
    : titleBar(nullptr), list(nullptr), tabs(nullptr), resizer(nullptr), currentTab(-1) {
}

// OFFSET: 0x00A08D60
// FUNCTION: fn_15031
CUIEquipmentPropertiesWnd* CreateEquipmentPropertiesWnd() {
  return new CUIEquipmentPropertiesWnd();
}
