#include "UI/Item/UIEquipmentPropertiesWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"

#include <cstdint>
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
