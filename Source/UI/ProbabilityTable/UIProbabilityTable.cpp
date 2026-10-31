#include "UI/ProbabilityTable/UIProbabilityTable.h"

#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <string_view>

// GLOBAL: gl_25157
static const std::string g_probabilityHighlightBegin = "^FF0000<B>";
// GLOBAL: gl_25158
static const std::string g_probabilityHighlightEnd = "^000000</B>";

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

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const std::uint8_t (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

struct ProbabilityTableFreezeCallback {
  virtual ~ProbabilityTableFreezeCallback() = default;

  /* 0004 */ CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* list;

  void operator()(CUIListItem* const& item);
};

struct ProbabilityTableSelectCallback {
  virtual ~ProbabilityTableSelectCallback() = default;

  /* 0004 */ CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* list;

  void operator()(CUIListItem* const& item);
};

// OFFSET: 0x00A4CD60
// FUNCTION: fn_16148
void ProbabilityTableFreezeCallback::operator()(CUIListItem* const& item) {
  CUIListItem* selectedItem = item;
  for (ProbabilityTable::CUIItem* row : this->list->items) {
    row->SetHighlighted(selectedItem == row);
  }
}

// OFFSET: 0x00A4CE10
// FUNCTION: fn_16152
void ProbabilityTableSelectCallback::operator()(CUIListItem* const& item) {
  CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* list = this->list;
  CUIListItem* selectedItem = item;
  if (!list->unknown_00C4) {
    for (ProbabilityTable::CUIItem* row : list->items) {
      if (selectedItem != row) {
        row->SetSelected(false);
      }
    }

    selectedItem->SetSelected(true);
    if (selectedItem->index != static_cast<int>(list->selected) || !list->unknown_00E0) {
      ProbabilityTable::CUIItem* probabilityItem = dynamic_cast<ProbabilityTable::CUIItem*>(selectedItem);
      if (probabilityItem) {
        list->selected = probabilityItem->index;
        if (list->onSelect && probabilityItem->index >= 0 &&
            static_cast<unsigned int>(probabilityItem->index) < list->data.size()) {
          auto node = list->data.begin();
          for (int index = probabilityItem->index; index > 0; --index) {
            ++node;
          }
          list->onSelect(probabilityItem, *node);
        }
      }
    }
  }
}

template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::OnScroll(int position);

template <>
bool CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::RebindIndex(int index);

template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::AddNewItem();

template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Slot53(int resolveSkin);

template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Resize(int width, int height);

// OFFSET: 0x00A47EF0
// FUNCTION: fn_16104
bool UnusedTrueA47EF0() {
  return true;
}

// OFFSET: 0x00A47F00
// FUNCTION: fn_16105
bool UnusedTrueA47F00() {
  return true;
}

// OFFSET: 0x00A47F10
// FUNCTION: fn_16106
bool UnusedTrueA47F10() {
  return true;
}

// OFFSET: 0x00A47FA0
// FUNCTION: fn_16108
template <>
CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A48160
// FUNCTION: fn_16109
template <>
CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::~CUIListBoxEx() {
}

// OFFSET: 0x00A486C0
// FUNCTION: fn_16114
CUIProbabilityTable* CreateProbabilityTable() {
  return new CUIProbabilityTable();
}

// OFFSET: 0x00A48740
// FUNCTION: fn_16115
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::ClearItems() {
  for (ProbabilityTable::CUIItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A487D0
// FUNCTION: fn_16116
void CUIProbabilityTable::Slot53() {
  this->unknown_00B4 = 0;
  this->Resize(345, 455);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A48860
// FUNCTION: fn_16117
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->onChange = [this](int position) { this->OnScroll(position); };
  this->AddChild(this->scrollBar);
}

namespace ProbabilityTable {

// OFFSET: 0x00A4D620
// FUNCTION: fn_16190
CUIItem::CUIItem() : CUIListItem(), nameText(nullptr), probabilityText(nullptr), backgroundColor(0xFFFFFFFF) {
}

// OFFSET: 0x00A48A70
// FUNCTION: fn_16118
void CUIItem::Slot15(int width, int height) {
  this->nameText = new CUITextViewer(*this->m_context);
  this->nameText->SetSize(231, 29);
  this->nameText->Move(1, 0);
  this->nameText->SetTextOffset(0, 6);
  const std::uint8_t nameFontSource[] = {0xA8, 0x04, 0xB3, 0x7A, 0x00, 0x4F};
  thread_local EncryptedName<sizeof(nameFontSource), 0xEDC74F6D13DF71EFULL> nameFont(nameFontSource);
  this->nameText->SetFont(std::string(nameFont.Text()));
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetTextColor(0);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->nameText->SetLineSpacing(2);
  this->nameText->SetBackgroundColor(0xFFFFFF);
  this->nameText->SetPadding(10, 1, 0, 0);
  this->nameText->SetScrollBarSize(13, 29);
  this->nameText->SetScrollStep(29);
  this->nameText->SetScrollBarButtonImages(std::string(), std::string());
  this->nameText->SetScrollBarBackground(std::string());
  this->nameText->SetScrollBarThumbImage(std::string());
  this->AddChild(this->nameText);

  this->probabilityText = new CUITextViewer(*this->m_context);
  this->probabilityText->SetSize(78, 29);
  this->probabilityText->Move(233, 0);
  this->probabilityText->SetTextOffset(2, 6);
  const std::uint8_t probabilityFontSource[] = {0xE2, 0xE8, 0x01, 0x90, 0x0E, 0x27};
  thread_local EncryptedName<sizeof(probabilityFontSource), 0x03772763F96D9DA5ULL> probabilityFont(probabilityFontSource);
  this->probabilityText->SetFont(std::string(probabilityFont.Text()));
  this->probabilityText->SetFontHeight(12);
  this->probabilityText->SetBold(0);
  this->probabilityText->SetTextColor(0);
  this->probabilityText->SetOutline(0, 0xFFFFFF);
  this->probabilityText->SetLineSpacing(6);
  this->probabilityText->SetBackgroundColor(0xFFFFFF);
  this->probabilityText->SetPadding(0, 1, 10, 0);
  this->probabilityText->SetScrollBarSize(13, 29);
  this->probabilityText->SetScrollStep(29);
  this->probabilityText->SetScrollBarButtonImages("", "");
  this->probabilityText->SetScrollBarBackground("");
  this->probabilityText->SetScrollBarThumbImage("");
  this->AddChild(this->probabilityText);
}

}

// OFFSET: 0x00A4C090
// FUNCTION: fn_16122
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (ProbabilityTable::CUIItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->rowHeight <= this->geometry.height) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count, nullptr);
    for (ProbabilityTable::CUIItem*& slot : this->items) {
      ProbabilityTable::CUIItem* item = new ProbabilityTable::CUIItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (ProbabilityTable::CUIItem* other : this->items) {
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

// OFFSET: 0x00A4C2F0
// FUNCTION: fn_16123
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Slot53(int resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (ProbabilityTable::CUIItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A4C500
// FUNCTION: fn_16124
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A4BFF0
// FUNCTION: fn_16120
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A4C000
// FUNCTION: fn_16121
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A4C580
// FUNCTION: fn_16125
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::OnScroll(int position) {
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
    ProbabilityTable::CUIItem* item = *it;
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

// OFFSET: 0x00A4C780
// FUNCTION: fn_16126
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (ProbabilityTable::CUIItem* other : this->items) {
    if (other != item) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  ProbabilityTable::CUIItem* selectedItem = dynamic_cast<ProbabilityTable::CUIItem*>(item);
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

// OFFSET: 0x00A4D6A0
// FUNCTION: fn_16191
CUIProbabilityTable::CUIProbabilityTable()
    : m_titleBar(nullptr), m_nameHeader(nullptr), m_searchEdit(nullptr), unknown_00E4{}, m_itemName(nullptr), m_probabilityHeader(nullptr), m_list(nullptr),
      m_itemId(0) {
}

// OFFSET: 0x00A4DC90
// FUNCTION: fn_16195
void CUIProbabilityTable::OnButtonClick() {
}

// OFFSET: 0x00A4E010
// FUNCTION: fn_45473
void CUIProbabilityTable::OnClose() {
  g_windowMgr.CloseWindow(0x271C);
}

// OFFSET: 0x00A4DCA0
// FUNCTION: fn_16196
void CUIProbabilityTable::Search() {
  CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* list = this->m_list;
  if (list->data.size() == 0) {
    return;
  }

  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto node = list->data.begin();
    for (int index = static_cast<int>(list->selected); index > 0; --index) {
      ++node;
    }
    RemoveAll(node->name, g_probabilityHighlightBegin);
    RemoveAll(node->name, g_probabilityHighlightEnd);
    this->m_list->RebindIndex(this->m_list->selected);
  }

  const std::string_view keyword(this->m_searchEdit->edit->Text());
  if (keyword.size() == 0) {
    this->m_lastSearch = keyword;
    return;
  }

  list = this->m_list;
  auto matches = [keyword](const ProbabilityTable::CItemData& data) {
    return FindSubstring(data.name.c_str(), static_cast<int>(data.name.size()), keyword.data()) != -1;
  };

  auto start = list->data.begin();
  if (keyword == this->m_lastSearch) {
    const int next = list->data.size() - 1 != list->selected ? static_cast<int>(list->selected) + 1 : 0;
    start = std::next(list->data.begin(), next);
  }

  auto found = std::find_if(start, list->data.end(), matches);
  if (found == list->data.end() && keyword == this->m_lastSearch) {
    found = std::find_if(list->data.begin(), list->data.end(), matches);
  }

  if (found == list->data.end()) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x1012), 0, true, false, 0, "Message", 280, 120, 0);
  } else {
    HighlightKeyword(found->name, keyword, g_probabilityHighlightBegin, g_probabilityHighlightEnd);

    CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* current = this->m_list;
    const int index = static_cast<int>(std::distance(list->data.begin(), found));
    current->scrollBar->SetPosition(current->rowHeight * index);
    for (ProbabilityTable::CUIItem* item : current->items) {
      if (index == item->index) {
        current->SelectItem(item);
        break;
      }
    }
    this->m_list->RebindIndex(this->m_list->selected);
  }

  this->m_lastSearch = keyword;
}

// OFFSET: 0x00A4E280
// FUNCTION: fn_16203
void CUIProbabilityTable::Slot16() {
  UIWindow* anchor = g_windowMgr.LookupWindow(12);
  if (anchor) {
    this->Move(anchor->geometry.x + anchor->geometry.width, anchor->geometry.y);
  } else {
    this->Move(g_renderer->viewport.centerX, (g_renderer->viewport.height - this->geometry.height) / 2);
  }

  const char* title = LookupMessage(0x1017);
  this->m_titleBar->SetTitle(title, std::strlen(title));
  const char* name = LookupMessage(0x1018);
  this->m_nameHeader->SetText(name, std::strlen(name));
  const char* probability = LookupMessage(0x1019);
  this->m_probabilityHeader->SetText(probability, std::strlen(probability));
}

// OFFSET: 0x00A4E030
// FUNCTION: fn_16198
void CUIProbabilityTable::OnPrepareItem(ProbabilityTable::CUIItem*, const ProbabilityTable::CItemData&) {
}

// OFFSET: 0x00A4E040
// FUNCTION: fn_16199
void BindItem(ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data) {
  item->nameText->Clear();
  item->probabilityText->Clear();
  item->nameText->SetText(data.name);
  item->nameText->SetBackgroundColor(data.color);
  item->probabilityText->SetBackgroundColor(data.color);
  item->probabilityText->SetText(FormatOwnedString("%.4lf%%", data.probability));
}

// OFFSET: 0x00A493D0
// FUNCTION: fn_16119
void CUIProbabilityTable::Slot15(int width, int height) {
  this->m_titleBar = new CUITitleBar(*this->m_context);
  this->m_titleBar->SetSize(345, 17);
  this->m_titleBar->Move(0, 0);
  const std::uint8_t titleRight[] = {
    0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0xA7,
    0x0C, 0xC8, 0xFC, 0x4E, 0xCC, 0x92, 0x79, 0xB1, 0x08, 0xC9, 0xF3, 0x4C, 0xF0, 0x9E, 0x4B, 0xB1,
    0x04, 0xCF, 0xF9, 0x48, 0xF1, 0x9A, 0x65, 0x9A, 0x1F, 0xD2, 0xF2, 0x45, 0xE7, 0xD5, 0x75, 0xA8,
    0x1D, 0xBB
  };
  const std::uint8_t titleMiddle[] = {
    0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x41,
    0xA8, 0x86, 0x36, 0xE8, 0xD0, 0xBC, 0xD5, 0x57, 0xAC, 0x87, 0x39, 0xEA, 0xEC, 0xB0, 0xE7, 0x57,
    0xA0, 0x81, 0x33, 0xEE, 0xED, 0xB4, 0xC9, 0x7C, 0xA4, 0x9C, 0x3B, 0xA5, 0xED, 0xB8, 0xCB, 0x23
  };
  const std::uint8_t titleLeft[] = {
    0x47, 0xBC, 0xAB, 0x85, 0xF9, 0xC7, 0xB4, 0xC6, 0x41, 0xF7, 0xAB, 0xB3, 0x84, 0xB3, 0x2D, 0x69,
    0xE6, 0x60, 0x02, 0x1C, 0x66, 0x60, 0x1F, 0x7F, 0xE2, 0x61, 0x0D, 0x1E, 0x5A, 0x6C, 0x2D, 0x7F,
    0xEE, 0x67, 0x07, 0x1A, 0x5B, 0x68, 0x03, 0x54, 0xEB, 0x76, 0x0D, 0x0B, 0x17, 0x6B, 0x1C, 0x7B,
    0x87
  };
  this->m_titleBar->SetImages(std::string(DecryptName<0x0B7109397F6B1387ULL>(titleLeft)), std::string(DecryptName<0x23BBD58F8B5FF5C9ULL>(titleMiddle)), std::string(DecryptName<0xC517FB932D95BB6DULL>(titleRight)));
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(310, 17);
  title->Move(18, 0);
  const std::uint8_t titleTextSource[] = {0x8B, 0x87, 0xE4, 0xDB, 0xF5, 0x0F, 0xB1, 0x29, 0xF0, 0x29};
  const char* titleText = DecryptName<0x9949BFD527532943ULL>(titleTextSource);
  title->SetText(titleText, std::strlen(titleText));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  const std::uint8_t titleFont[] = {0x28, 0x08, 0x97, 0x4A, 0x1C, 0x07};
  title->SetFont(std::string(DecryptName<0xCD8F077123FB7D6FULL>(titleFont)));
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->m_titleBar->titleText = title;
  this->m_titleBar->AddChild(title);

  CUIImageButton* systemButton = new CUIImageButton(*this->m_context);
  systemButton->SetSize(11, 11);
  systemButton->Move(3, 3);
  systemButton->SetOnClick([this]() { this->OnButtonClick(); });
  systemButton->disabled = 0;
  systemButton->Invalidate();
  const std::uint8_t systemInactive[] = {
    0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xC7,
    0xFE, 0xB6, 0x04, 0xC8, 0x88, 0x40, 0x93, 0xD1, 0xFA, 0xB7, 0x0B, 0xCA, 0xB4, 0x4C, 0xA1, 0xD6,
    0xE6, 0xB6, 0x32, 0xC9, 0xB6, 0x5A, 0x98, 0xFA, 0xF0, 0xA3, 0x0B, 0x85, 0xB5, 0x44, 0x8D, 0xA5
  };
  const std::uint8_t systemPressed[] = {
    0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x93,
    0x90, 0xB4, 0x50, 0x6A, 0x48, 0x10, 0x21, 0x85, 0x94, 0xB5, 0x5F, 0x68, 0x74, 0x1C, 0x13, 0x82,
    0x88, 0xB4, 0x66, 0x6B, 0x76, 0x0A, 0x2A, 0xAE, 0x9E, 0xA9, 0x17, 0x6B, 0x7A, 0x09, 0x4F
  };
  const std::uint8_t systemHover[] = {
    0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x75,
    0x56, 0x34, 0xC4, 0x5A, 0xE0, 0x8C, 0xC7, 0x63, 0x52, 0x35, 0xCB, 0x58, 0xDC, 0x80, 0xF5, 0x64,
    0x4E, 0x34, 0xF2, 0x5B, 0xDE, 0x96, 0xCC, 0x48, 0x58, 0x29, 0x83, 0x5B, 0xD2, 0x95, 0xA9
  };
  const std::uint8_t systemNormal[] = {
    0xA7, 0x04, 0x51, 0x5F, 0x61, 0xF1, 0x26, 0xDE, 0xA1, 0x4F, 0x51, 0x69, 0x1C, 0x85, 0xBF, 0x71,
    0x06, 0xD8, 0xF8, 0xC6, 0xFE, 0x56, 0x8D, 0x67, 0x02, 0xD9, 0xF7, 0xC4, 0xC2, 0x5A, 0xBF, 0x60,
    0x1E, 0xD8, 0xCE, 0xC7, 0xC0, 0x4C, 0x86, 0x4C, 0x08, 0xCD, 0xF7, 0x8B, 0xC3, 0x52, 0x93, 0x13
  };
  systemButton->SetImages(
    std::string(DecryptName<0x13E33FA1A591AB67ULL>(systemNormal)), std::string(DecryptName<0x17A9E5BF39AD4737ULL>(systemHover)),
    std::string(DecryptName<0xF14F79170939C7F1ULL>(systemPressed)), std::string(DecryptName<0xA5FD29D7AB6DC59FULL>(systemInactive))
  );
  this->m_titleBar->AddChild(systemButton);

  CUIImageButton* closeButton = new CUIImageButton(*this->m_context);
  closeButton->SetSize(11, 11);
  closeButton->Move(331, 3);
  closeButton->SetOnClick([this]() { this->OnClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  const std::uint8_t closeInactive[] = {
    0x67, 0x5A, 0xBF, 0xC3, 0xF1, 0x85, 0xEE, 0x72, 0x61, 0x11, 0xBF, 0xF5, 0x8C, 0xF1, 0x77, 0xDD,
    0xC6, 0x86, 0x16, 0x5A, 0x6E, 0x22, 0x45, 0xCB, 0xC2, 0x87, 0x19, 0x58, 0x52, 0x2E, 0x77, 0xCC,
    0xDE, 0x86, 0x20, 0x5A, 0x5D, 0x24, 0x58, 0xDA, 0xF8, 0x9A, 0x19, 0x5F, 0x1F, 0x29, 0x46, 0xCF,
    0xA7
  };
  const std::uint8_t closePressed[] = {
    0x7D, 0x12, 0x2D, 0xCD, 0x7B, 0x71, 0x2A, 0x4E, 0x7B, 0x59, 0x2D, 0xFB, 0x06, 0x05, 0xB3, 0xE1,
    0xDC, 0xCE, 0x84, 0x54, 0xE4, 0xD6, 0x81, 0xF7, 0xD8, 0xCF, 0x8B, 0x56, 0xD8, 0xDA, 0xB3, 0xF0,
    0xC4, 0xCE, 0xB2, 0x54, 0xD7, 0xD0, 0x9C, 0xE6, 0xE2, 0xD2, 0x83, 0x19, 0xD9, 0xD2, 0x9F, 0x83
  };
  const std::uint8_t closeHover[] = {
    0xBF, 0xBC, 0x1F, 0x2B, 0x45, 0xA9, 0xBC, 0x20, 0xB9, 0xF7, 0x1F, 0x1D, 0x38, 0xDD, 0x25, 0x8F,
    0x1E, 0x60, 0xB6, 0xB2, 0xDA, 0x0E, 0x17, 0x99, 0x1A, 0x61, 0xB9, 0xB0, 0xE6, 0x02, 0x25, 0x9E,
    0x06, 0x60, 0x80, 0xB2, 0xE9, 0x08, 0x0A, 0x88, 0x20, 0x7C, 0xB1, 0xFF, 0xE7, 0x0A, 0x09, 0xED
  };
  const std::uint8_t closeNormal[] = {
    0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21, 0xBB,
    0x68, 0x54, 0x46, 0xD8, 0xE2, 0x34, 0x13, 0xAD, 0x6C, 0x55, 0x49, 0xDA, 0xDE, 0x38, 0x21, 0xAA,
    0x70, 0x54, 0x70, 0xD8, 0xD1, 0x32, 0x0E, 0xBC, 0x56, 0x48, 0x49, 0xDD, 0x93, 0x3F, 0x10, 0xA9,
    0x09
  };
  closeButton->SetImages(
    std::string(DecryptName<0xD97D5DBDBB2F2709ULL>(closeNormal)), std::string(DecryptName<0xED796785D1DF137FULL>(closeHover)),
    std::string(DecryptName<0x83EFBFBB37EDBDBDULL>(closePressed)), std::string(DecryptName<0xBF2B4B31397FF5A7ULL>(closeInactive))
  );
  this->m_titleBar->SetSecondButton(closeButton);

  this->m_nameHeader = new CUIStaticText(*this->m_context);
  this->m_nameHeader->SetSize(160, 18);
  this->m_nameHeader->Move(10, 30);
  const std::uint8_t nameHeaderSource[] = {0x50, 0xC3, 0x45, 0x6D, 0x12, 0xA3, 0x19, 0x6D, 0x2A, 0xC0, 0x0F, 0xD7};
  const char* nameHeaderText = DecryptName<0xDD3913D5D7F97BE1ULL>(nameHeaderSource);
  this->m_nameHeader->SetText(nameHeaderText, std::strlen(nameHeaderText));
  this->m_nameHeader->SetFitText(0);
  this->m_nameHeader->SetAlign(2, 6);
  const std::uint8_t nameHeaderFont[] = {0xDC, 0x22, 0x6F, 0xF0, 0x0A, 0x05};
  this->m_nameHeader->SetFont(std::string(DecryptName<0x050505679903579BULL>(nameHeaderFont)));
  this->m_nameHeader->SetFontHeight(12);
  this->m_nameHeader->SetBold(0);
  this->m_nameHeader->SetColor(0);
  this->m_nameHeader->SetShadow(0, 0xFFFFFF);
  this->m_nameHeader->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_nameHeader);

  this->m_searchEdit = new CUIEditBoxEx(*this->m_context);
  this->m_searchEdit->Create(130, 18, false);
  this->m_searchEdit->Move(180, 30);
  this->m_searchEdit->SetText("");
  this->m_searchEdit->SetTextColor(0);
  this->m_searchEdit->SetBackgroundColor(0xF7F7F7);
  this->m_searchEdit->SetBorderColor(0xC6C6C6);
  this->m_searchEdit->SetBorderWidth(1);
  this->m_searchEdit->SetMaximum(8);
  this->m_searchEdit->SetEditBounds(0, 0, 0, 0);
  this->m_searchEdit->SetMask(0);
  this->m_searchEdit->SetAlignment(0);
  this->m_searchEdit->SetOnSubmit([this]() { this->Search(); });
  this->AddChild(this->m_searchEdit);

  this->m_searchButton = new CUIImageButton(*this->m_context);
  this->m_searchButton->SetSize(25, 18);
  this->m_searchButton->Move(310, 30);
  this->m_searchButton->SetOnClick([this]() { this->Search(); });
  this->m_searchButton->disabled = 0;
  this->m_searchButton->Invalidate();
  const std::uint8_t searchInactive[] = {
    0x4F, 0x36, 0x2B, 0xF7, 0x75, 0xB1, 0x3A, 0xF4, 0x49, 0x7D, 0x2B, 0xC1, 0x08, 0xC5, 0xA3, 0x57,
    0xEE, 0xEF, 0x82, 0x6A, 0xD4, 0x0B, 0x96, 0x56, 0xE1, 0xC6, 0x82, 0x63, 0xC1, 0x1A, 0x8D, 0x5F,
    0xEE, 0xFA, 0x8E, 0x3E, 0xE9, 0x1D, 0x8B, 0x57, 0xD0, 0xEA, 0x8E, 0x6C, 0xC7, 0x1C, 0x97, 0x5B,
    0xEE, 0xEB, 0xB4, 0x63, 0xDA, 0x0D, 0x92, 0x58, 0xE3, 0xB7, 0x89, 0x60, 0xC5, 0x7F
  };
  const std::uint8_t searchPressed[] = {
    0x57, 0x5E, 0x21, 0x27, 0x11, 0x5D, 0x2C, 0xE2, 0x51, 0x15, 0x21, 0x11, 0x6C, 0x29, 0xB5, 0x41,
    0xF6, 0x87, 0x88, 0xBA, 0xB0, 0xE7, 0x80, 0x40, 0xF9, 0xAE, 0x88, 0xB3, 0xA5, 0xF6, 0x9B, 0x49,
    0xF6, 0x92, 0x84, 0xEE, 0x8D, 0xF1, 0x9D, 0x41, 0xC8, 0x82, 0x84, 0xBC, 0xA3, 0xF0, 0x81, 0x4D,
    0xF6, 0x83, 0xBE, 0xB2, 0xA7, 0xF6, 0x9B, 0x01, 0xF5, 0x9C, 0x91, 0xDD
  };
  const std::uint8_t searchHover[] = {
    0x0D, 0x5E, 0x59, 0x4F, 0x4D, 0x95, 0xC2, 0x60, 0x0B, 0x15, 0x59, 0x79, 0x30, 0xE1, 0x5B, 0xC3,
    0xAC, 0x87, 0xF0, 0xD2, 0xEC, 0x2F, 0x6E, 0xC2, 0xA3, 0xAE, 0xF0, 0xDB, 0xF9, 0x3E, 0x75, 0xCB,
    0xAC, 0x92, 0xFC, 0x86, 0xD1, 0x39, 0x73, 0xC3, 0x92, 0x82, 0xFC, 0xD4, 0xFF, 0x38, 0x6F, 0xCF,
    0xAC, 0x83, 0xC6, 0xC5, 0xFF, 0x3E, 0x74, 0xDE, 0xE3, 0x93, 0xF4, 0xC5, 0x8D
  };
  const std::uint8_t searchNormal[] = {
    0xBF, 0xBA, 0x4D, 0xEB, 0xF7, 0xC1, 0x4E, 0xE2, 0xB9, 0xF1, 0x4D, 0xDD, 0x8A, 0xB5, 0xD7, 0x41,
    0x1E, 0x63, 0xE4, 0x76, 0x56, 0x7B, 0xE2, 0x40, 0x11, 0x4A, 0xE4, 0x7F, 0x43, 0x6A, 0xF9, 0x49,
    0x1E, 0x76, 0xE8, 0x22, 0x6B, 0x6D, 0xFF, 0x41, 0x20, 0x66, 0xE8, 0x70, 0x45, 0x6C, 0xE3, 0x4D,
    0x1E, 0x67, 0xD2, 0x7F, 0x58, 0x7D, 0xE6, 0x4E, 0x13, 0x3B, 0xEF, 0x7C, 0x47, 0x0F
  };
  this->m_searchButton->SetImages(
    std::string(DecryptName<0x2F8B0F37118D157FULL>(searchNormal)), std::string(DecryptName<0xAD075B8DB599F1CDULL>(searchHover)),
    std::string(DecryptName<0x2FE993D1DDE1F197ULL>(searchPressed)), std::string(DecryptName<0x39FF7FB50DEB998FULL>(searchInactive))
  );
  this->AddChild(this->m_searchButton);

  this->m_itemName = new CUIStaticText(*this->m_context);
  this->m_itemName->SetSize(220, 24);
  this->m_itemName->Move(16, 61);
  this->m_itemName->SetText("", 0);
  this->m_itemName->SetFitText(1);
  this->m_itemName->SetAlign(6, 6);
  const std::uint8_t itemNameFont[] = {0x2C, 0xCC, 0xAB, 0x66, 0x0A, 0x8F};
  this->m_itemName->SetFont(std::string(DecryptName<0xB7FD8F670FC7B96BULL>(itemNameFont)));
  this->m_itemName->SetFontHeight(12);
  this->m_itemName->SetBold(1);
  this->m_itemName->SetColor(0);
  this->m_itemName->SetShadow(0, 0xFFFFFF);
  this->m_itemName->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_itemName);

  this->m_probabilityHeader = new CUIStaticText(*this->m_context);
  this->m_probabilityHeader->SetSize(68, 24);
  this->m_probabilityHeader->Move(248, 61);
  const std::uint8_t probabilityHeaderSource[] = {0x35, 0x79, 0xFA, 0x55, 0xB5};
  const char* probabilityHeaderText = DecryptName<0xE7978FB5A94DD7FDULL>(probabilityHeaderSource);
  this->m_probabilityHeader->SetText(probabilityHeaderText, std::strlen(probabilityHeaderText));
  this->m_probabilityHeader->SetFitText(1);
  this->m_probabilityHeader->SetAlign(6, 6);
  const std::uint8_t probabilityHeaderFont[] = {0xBA, 0xCC, 0xD7, 0x48, 0xF2, 0xFD};
  this->m_probabilityHeader->SetFont(std::string(DecryptName<0x4941FD9F21BBB9FDULL>(probabilityHeaderFont)));
  this->m_probabilityHeader->SetFontHeight(12);
  this->m_probabilityHeader->SetBold(0);
  this->m_probabilityHeader->SetColor(0);
  this->m_probabilityHeader->SetShadow(0, 0xFFFFFF);
  this->m_probabilityHeader->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_probabilityHeader);

  this->m_list = new CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>(*this->m_context);
  this->m_list->itemWidth = 312;
  this->m_list->rowHeight = 30;
  this->m_list->SetSize(325, 360);
  this->m_list->Move(10, 85);
  this->m_list->SetOnSelect([this](ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data) { this->OnPrepareItem(item, data); });
  this->m_list->SetBinder([this](ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data) { BindItem(item, data); });
  this->m_list->SetImages("", "", "", "");
  this->m_list->itemX = 0;
  this->m_list->itemY = 0;
  this->m_list->scrollBar->SetSize(13, 360);
  this->m_list->scrollBar->Move(312, 0);
  this->m_list->scrollBar->step = 30;
  const std::uint8_t scrollDown[] = {
    0x8F, 0xC8, 0x2D, 0x7F, 0xC9, 0xF9, 0xFA, 0xB4, 0x89, 0x83, 0x2D, 0x49, 0xB4, 0x8D, 0x63, 0x0A,
    0x2C, 0x15, 0x82, 0xE9, 0x65, 0x07, 0x5B, 0x16, 0x38, 0x09, 0xC3, 0xE7, 0x64, 0x47, 0x3F
  };
  const std::uint8_t scrollUp[] = {
    0xB5, 0x62, 0x2D, 0xA7, 0xAB, 0xAB, 0x20, 0xF8, 0xB3, 0x29, 0x2D, 0x91, 0xD6, 0xDF, 0xB9, 0x46,
    0x16, 0xBF, 0x82, 0x31, 0x07, 0x55, 0x90, 0x45, 0x5B, 0xAF, 0x80, 0x2D, 0x6B
  };
  this->m_list->scrollBar->SetButtonImages(std::string(DecryptName<0x35E5656B5DEDCD75ULL>(scrollUp)), std::string(DecryptName<0x793F370985ED674FULL>(scrollDown)));
  const std::uint8_t scrollMiddle[] = {
    0xDD, 0xAA, 0x53, 0xD3, 0xB1, 0xED, 0x92, 0x28, 0xDB, 0xE1, 0x53, 0xE5, 0xCC, 0x99, 0x0B, 0x96,
    0x7E, 0x77, 0xFC, 0x45, 0x1D, 0x13, 0x3A, 0x8C, 0x79, 0x2B, 0xF1, 0x44, 0x01, 0x23
  };
  this->m_list->scrollBar->SetBackground(std::string(DecryptName<0xE55723712993051DULL>(scrollMiddle)));
  const std::uint8_t thumbBottom[] = {
    0x8F, 0x0E, 0x0F, 0xCF, 0xA3, 0x19, 0x84, 0xB4, 0x89, 0x45, 0x0F, 0xF9, 0xDE, 0x6D, 0x1D, 0x0A,
    0x2C, 0xD3, 0xA0, 0x59, 0x0F, 0xE7, 0x23, 0x18, 0x3D, 0xFE, 0xAB, 0x5A, 0x14, 0xB9, 0x6F, 0x1B,
    0x22, 0xD1, 0xCF
  };
  const std::uint8_t thumbMiddle[] = {
    0xA3, 0x5A, 0x21, 0x85, 0xFB, 0xEB, 0xD8, 0x06, 0xA5, 0x11, 0x21, 0xB3, 0x86, 0x9F, 0x41, 0xB8,
    0x00, 0x87, 0x8E, 0x13, 0x57, 0x15, 0x7F, 0xAA, 0x11, 0xAA, 0x8C, 0x16, 0x5F, 0x0B, 0x7F, 0xA6,
    0x13, 0xF5
  };
  const std::uint8_t thumbTop[] = {
    0x09, 0xBC, 0x39, 0x89, 0x39, 0x17, 0xD0, 0xBC, 0x0F, 0xF7, 0x39, 0xBF, 0x44, 0x63, 0x49, 0x02,
    0xAA, 0x61, 0x96, 0x1F, 0x95, 0xE9, 0x77, 0x10, 0xBB, 0x4C, 0x8C, 0x03, 0xD7, 0xBB, 0x78, 0x01,
    0xC9
  };
  this->m_list->scrollBar->SetThumbImages(
    std::string(DecryptName<0x7115D9F973F913C9ULL>(thumbTop)), std::string(DecryptName<0xCB1D253B7FE1F563ULL>(thumbMiddle)), std::string(DecryptName<0x7941D76335CFA14FULL>(thumbBottom))
  );
  this->m_list->AddNewItem();
  this->AddChild(this->m_list);
}

namespace ProbabilityTable {

// OFFSET: 0x00A4E110
// FUNCTION: fn_16200
void CUIItem::Paint() {
  this->ClearSurface(this->backgroundColor);
  this->FillRegion(0, this->geometry.height - 1, this->geometry.width, 1, 0xFFCEADA5);
  this->FillRegion(0, 0, 1, this->geometry.height, 0xFFCEADA5);
  this->FillRegion(232, 0, 1, this->geometry.height, 0xFFCEADA5);
  this->FillRegion(this->geometry.width - 1, 0, 1, this->geometry.height, 0xFFCEADA5);
}

}

// OFFSET: 0x00A4E180
// FUNCTION: fn_16201
void CUIProbabilityTable::Paint() {
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFF808080);
  this->FillRegion(10, 60, 312, 25, 0xFF99E5FF);
  this->FillRegion(10, 60, 312, 1, 0xFFCEADA5);
  for (int y = 84; y <= 444; y += 30) {
    this->FillRegion(10, y, 312, 1, 0xFFCEADA5);
  }
  this->FillRegion(10, 60, 1, 385, 0xFFCEADA5);
  this->FillRegion(242, 60, 1, 385, 0xFFCEADA5);
  this->FillRegion(321, 60, 1, 385, 0xFFCEADA5);
}

namespace ProbabilityTable {

// OFFSET: 0x00A4E250
// FUNCTION: fn_16202
void CUIItem::Slot16() {
  this->nameText->scrollBar->SetHidden(1);
  this->probabilityText->scrollBar->SetHidden(1);
}

// OFFSET: 0x00A4E350
// FUNCTION: fn_16204
void CUIItem::Slot25(int, int) {
}

}

// OFFSET: 0x00A4E360
// FUNCTION: fn_16205
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::UpdateScrollBar() {
  this->scrollBar->SetTotal(this->data.size() * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}
