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
