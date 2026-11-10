#include "UI/Window/WindowId.h"
#include "UI/ProbabilityTable/UIProbabilityTable.h"

#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Contents/Tables/MessageTable.h"
#include "Render/Renderer.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Window/UIRegisterMgr.h"

#include "Resource/MSGSTRINGID.h"
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
  // INLINED
  template <typename Byte>
  explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
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
const char* DecryptName(const char (&source)[Length]) {
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
  if (!list->layoutFlag) {
    for (ProbabilityTable::CUIItem* row : list->items) {
      if (selectedItem != row) {
        row->SetSelected(false);
      }
    }

    selectedItem->SetSelected(true);
    if (selectedItem->index != static_cast<int>(list->selected) || !list->selectionFlag) {
      ProbabilityTable::CUIItem* probabilityItem = dynamic_cast<ProbabilityTable::CUIItem*>(selectedItem);
      if (probabilityItem) {
        const int selectedIndex = probabilityItem->index;
        list->selected = selectedIndex;
        if (list->onSelect && selectedIndex >= 0 &&
            static_cast<unsigned int>(selectedIndex) < list->data.size()) {
          auto node = list->data.begin();
          for (int index = selectedIndex; index > 0; --index) {
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
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::LoadSkin(bool resolveSkin);

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
CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
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

// OFFSET: 0x00496610
// FUNCTION: fn_45551
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateProbabilityTable;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10012, factory);
  return true;
}();

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
void CUIProbabilityTable::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->Resize(345, 455);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x00A48860
// FUNCTION: fn_16117
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
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
void CUIItem::OnCreate(int width, int height) {
  this->nameText = new CUITextViewer();
  this->nameText->SetSize(231, 29);
  this->nameText->Move(1, 0);
  this->nameText->SetTextOffset(0, 6);
  this->nameText->SetFont(std::string(DecryptName<0xEDC74F6D13DF71EFULL>({'\xA8', '\x04', '\xB3', '\x7A', '\x00', '\x4F'})));
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetTextColor(0);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->nameText->SetLineSpacing(2);
  this->nameText->SetBackgroundColor(0xFFFFFF);
  this->nameText->SetPadding(10, 1, 0, 0);
  this->nameText->SetScrollBarSize(13, 29);
  this->nameText->SetScrollStep(29);
  this->nameText->SetScrollBarButtonImages("", "");
  this->nameText->SetScrollBarBackground("");
  this->nameText->SetScrollBarThumbImage("");
  this->AddChild(this->nameText);

  this->probabilityText = new CUITextViewer();
  this->probabilityText->SetSize(78, 29);
  this->probabilityText->Move(233, 0);
  this->probabilityText->SetTextOffset(2, 6);
  this->probabilityText->SetFont(std::string(DecryptName<0x03772763F96D9DA5ULL>({'\xE2', '\xE8', '\x01', '\x90', '\x0E', '\x27'})));
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

  this->ClearItems();

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
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
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::LoadSkin(bool resolveSkin) {
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
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A4C580
// FUNCTION: fn_16125
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::OnScroll(int position) {
  if (this->items.empty()) {
    return;
  }
  if (this->data.size() == 0) {
    for (ProbabilityTable::CUIItem* item : this->items) {
      item->index = -1;
      item->Show(0);
    }
    return;
  }

  const int first = this->items.front()->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      const unsigned int shift = row - first;
      if (shift < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + shift, this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      const unsigned int shift = first - row;
      if (shift < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - shift, this->items.end());
      }
    }
  }

  auto it = this->items.begin();
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

  for (auto end = this->items.end(); it != end; ++it) {
    ProbabilityTable::CUIItem* item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A4C780
// FUNCTION: fn_16126
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (ProbabilityTable::CUIItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  ProbabilityTable::CUIItem* selectedItem = dynamic_cast<ProbabilityTable::CUIItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    int count = static_cast<int>(this->selected);
    if (count > 0) {
      do {
        ++node;
        --count;
      } while (count > 0);
    }
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A4D6A0
// FUNCTION: fn_16191
CUIProbabilityTable::CUIProbabilityTable()
    : m_titleBar(nullptr), m_nameHeader(nullptr), m_searchEdit(nullptr), m_searchButtonStorage{}, m_itemName(nullptr), m_probabilityHeader(nullptr), m_list(nullptr),
      m_itemId(0) {
}

// OFFSET: 0x00A4DC90
// FUNCTION: fn_16195
void CUIProbabilityTable::OnButtonClick() {
}

// OFFSET: 0x00A4E010
// FUNCTION: fn_45473
void CUIProbabilityTable::OnClose() {
  g_windowMgr.CloseWindow(WID_PROBABILITYTABLE);
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
    return FindSubstring(data.name, keyword) != -1;
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
    g_windowMgr.Confirm(LookupMessage(MSI_REFINING_BLACKSMITH_BLESSING_MSG3), 0, true, false, 0, "Message", 280, 120, 0);
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
void CUIProbabilityTable::OnInit() {
  UIWindow* anchor = g_windowMgr.LookupWindow(WID_ITEMCOLLECTIONWND);
  if (anchor) {
    this->Move(anchor->geometry.x + anchor->geometry.width, anchor->geometry.y);
  } else {
    this->Move(g_renderer->viewport.centerX, (g_renderer->viewport.height - this->geometry.height) / 2);
  }

  this->m_titleBar->SetTitle(LookupMessage(MSI_REFINING_PROBABILITY_URL));
  this->m_nameHeader->SetText(LookupMessage(MSI_GRADE_ENCHANT_PROBABILITY_URL));
  this->m_probabilityHeader->SetText(LookupMessage(MSI_SELLINGSLOT_SUCCESS));
}

// OFFSET: 0x00A4E030
// FUNCTION: fn_16198
void CUIProbabilityTable::OnPrepareItem(ProbabilityTable::CUIItem*, const ProbabilityTable::CItemData&) {
}

// OFFSET: 0x00A4E040
// FUNCTION: fn_16199
void CUIProbabilityTable::BindItem(ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data) {
  item->nameText->Clear();
  item->probabilityText->Clear();
  item->nameText->AddLine(data.name);
  item->nameText->SetBackgroundColor(data.color);
  item->probabilityText->SetBackgroundColor(data.color);
  item->probabilityText->AddLine(FormatOwnedString("%.4lf%%", data.probability));
}

// OFFSET: 0x00A493D0
// FUNCTION: fn_16119
void CUIProbabilityTable::OnCreate(int width, int height) {
  this->m_titleBar = new CUITitleBar();
  this->m_titleBar->SetSize(345, 17);
  this->m_titleBar->Move(0, 0);
  this->m_titleBar->SetImages(std::string(DecryptName<0x0B7109397F6B1387ULL>({
    '\x47', '\xBC', '\xAB', '\x85', '\xF9', '\xC7', '\xB4', '\xC6', '\x41', '\xF7', '\xAB', '\xB3', '\x84', '\xB3', '\x2D', '\x69',
    '\xE6', '\x60', '\x02', '\x1C', '\x66', '\x60', '\x1F', '\x7F', '\xE2', '\x61', '\x0D', '\x1E', '\x5A', '\x6C', '\x2D', '\x7F',
    '\xEE', '\x67', '\x07', '\x1A', '\x5B', '\x68', '\x03', '\x54', '\xEB', '\x76', '\x0D', '\x0B', '\x17', '\x6B', '\x1C', '\x7B',
    '\x87'
  })), std::string(DecryptName<0x23BBD58F8B5FF5C9ULL>({
    '\x09', '\x5A', '\x9F', '\x71', '\x4F', '\x1B', '\x7E', '\xEE', '\x0F', '\x11', '\x9F', '\x47', '\x32', '\x6F', '\xE7', '\x41',
    '\xA8', '\x86', '\x36', '\xE8', '\xD0', '\xBC', '\xD5', '\x57', '\xAC', '\x87', '\x39', '\xEA', '\xEC', '\xB0', '\xE7', '\x57',
    '\xA0', '\x81', '\x33', '\xEE', '\xED', '\xB4', '\xC9', '\x7C', '\xA4', '\x9C', '\x3B', '\xA5', '\xED', '\xB8', '\xCB', '\x23'
  })), std::string(DecryptName<0xC517FB932D95BB6DULL>({
    '\xAD', '\x14', '\x55', '\xD7', '\x53', '\x35', '\xD2', '\x08', '\xAB', '\x5F', '\x55', '\xE1', '\x2E', '\x41', '\x4B', '\xA7',
    '\x0C', '\xC8', '\xFC', '\x4E', '\xCC', '\x92', '\x79', '\xB1', '\x08', '\xC9', '\xF3', '\x4C', '\xF0', '\x9E', '\x4B', '\xB1',
    '\x04', '\xCF', '\xF9', '\x48', '\xF1', '\x9A', '\x65', '\x9A', '\x1F', '\xD2', '\xF2', '\x45', '\xE7', '\xD5', '\x75', '\xA8',
    '\x1D', '\xBB'
  })));
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(310, 17);
  title->Move(18, 0);
  title->SetText(DecryptName<0x9949BFD527532943ULL>({'\x8B', '\x87', '\xE4', '\xDB', '\xF5', '\x0F', '\xB1', '\x29', '\xF0', '\x29'}));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont(std::string(DecryptName<0xCD8F077123FB7D6FULL>({'\x28', '\x08', '\x97', '\x4A', '\x1C', '\x07'})));
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  CUITitleBar* titleBar = this->m_titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* systemButton = new CUIImageButton();
  systemButton->SetSize(11, 11);
  systemButton->Move(3, 3);
  systemButton->SetOnClick([this]() { this->OnButtonClick(); });
  systemButton->disabled = 0;
  systemButton->Invalidate();
  systemButton->SetImages(
    std::string(DecryptName<0x13E33FA1A591AB67ULL>({
    '\xA7', '\x04', '\x51', '\x5F', '\x61', '\xF1', '\x26', '\xDE', '\xA1', '\x4F', '\x51', '\x69', '\x1C', '\x85', '\xBF', '\x71',
    '\x06', '\xD8', '\xF8', '\xC6', '\xFE', '\x56', '\x8D', '\x67', '\x02', '\xD9', '\xF7', '\xC4', '\xC2', '\x5A', '\xBF', '\x60',
    '\x1E', '\xD8', '\xCE', '\xC7', '\xC0', '\x4C', '\x86', '\x4C', '\x08', '\xCD', '\xF7', '\x8B', '\xC3', '\x52', '\x93', '\x13'
  })), std::string(DecryptName<0x17A9E5BF39AD4737ULL>({
    '\xF7', '\xE8', '\x6D', '\xC3', '\x7F', '\x2B', '\x6C', '\xDA', '\xF1', '\xA3', '\x6D', '\xF5', '\x02', '\x5F', '\xF5', '\x75',
    '\x56', '\x34', '\xC4', '\x5A', '\xE0', '\x8C', '\xC7', '\x63', '\x52', '\x35', '\xCB', '\x58', '\xDC', '\x80', '\xF5', '\x64',
    '\x4E', '\x34', '\xF2', '\x5B', '\xDE', '\x96', '\xCC', '\x48', '\x58', '\x29', '\x83', '\x5B', '\xD2', '\x95', '\xA9'
  })),
    std::string(DecryptName<0xF14F79170939C7F1ULL>({
    '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\x93',
    '\x90', '\xB4', '\x50', '\x6A', '\x48', '\x10', '\x21', '\x85', '\x94', '\xB5', '\x5F', '\x68', '\x74', '\x1C', '\x13', '\x82',
    '\x88', '\xB4', '\x66', '\x6B', '\x76', '\x0A', '\x2A', '\xAE', '\x9E', '\xA9', '\x17', '\x6B', '\x7A', '\x09', '\x4F'
  })), std::string(DecryptName<0xA5FD29D7AB6DC59FULL>({
    '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xC7',
    '\xFE', '\xB6', '\x04', '\xC8', '\x88', '\x40', '\x93', '\xD1', '\xFA', '\xB7', '\x0B', '\xCA', '\xB4', '\x4C', '\xA1', '\xD6',
    '\xE6', '\xB6', '\x32', '\xC9', '\xB6', '\x5A', '\x98', '\xFA', '\xF0', '\xA3', '\x0B', '\x85', '\xB5', '\x44', '\x8D', '\xA5'
  }))
  );
  this->m_titleBar->AddChild(systemButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(331, 3);
  closeButton->SetOnClick([this]() { this->OnClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  closeButton->SetImages(
    std::string(DecryptName<0xD97D5DBDBB2F2709ULL>({
    '\xC9', '\x88', '\xEF', '\x41', '\x7D', '\x93', '\xB8', '\x14', '\xCF', '\xC3', '\xEF', '\x77', '\x00', '\xE7', '\x21', '\xBB',
    '\x68', '\x54', '\x46', '\xD8', '\xE2', '\x34', '\x13', '\xAD', '\x6C', '\x55', '\x49', '\xDA', '\xDE', '\x38', '\x21', '\xAA',
    '\x70', '\x54', '\x70', '\xD8', '\xD1', '\x32', '\x0E', '\xBC', '\x56', '\x48', '\x49', '\xDD', '\x93', '\x3F', '\x10', '\xA9',
    '\x09'
  })), std::string(DecryptName<0xED796785D1DF137FULL>({
    '\xBF', '\xBC', '\x1F', '\x2B', '\x45', '\xA9', '\xBC', '\x20', '\xB9', '\xF7', '\x1F', '\x1D', '\x38', '\xDD', '\x25', '\x8F',
    '\x1E', '\x60', '\xB6', '\xB2', '\xDA', '\x0E', '\x17', '\x99', '\x1A', '\x61', '\xB9', '\xB0', '\xE6', '\x02', '\x25', '\x9E',
    '\x06', '\x60', '\x80', '\xB2', '\xE9', '\x08', '\x0A', '\x88', '\x20', '\x7C', '\xB1', '\xFF', '\xE7', '\x0A', '\x09', '\xED'
  })),
    std::string(DecryptName<0x83EFBFBB37EDBDBDULL>({
    '\x7D', '\x12', '\x2D', '\xCD', '\x7B', '\x71', '\x2A', '\x4E', '\x7B', '\x59', '\x2D', '\xFB', '\x06', '\x05', '\xB3', '\xE1',
    '\xDC', '\xCE', '\x84', '\x54', '\xE4', '\xD6', '\x81', '\xF7', '\xD8', '\xCF', '\x8B', '\x56', '\xD8', '\xDA', '\xB3', '\xF0',
    '\xC4', '\xCE', '\xB2', '\x54', '\xD7', '\xD0', '\x9C', '\xE6', '\xE2', '\xD2', '\x83', '\x19', '\xD9', '\xD2', '\x9F', '\x83'
  })), std::string(DecryptName<0xBF2B4B31397FF5A7ULL>({
    '\x67', '\x5A', '\xBF', '\xC3', '\xF1', '\x85', '\xEE', '\x72', '\x61', '\x11', '\xBF', '\xF5', '\x8C', '\xF1', '\x77', '\xDD',
    '\xC6', '\x86', '\x16', '\x5A', '\x6E', '\x22', '\x45', '\xCB', '\xC2', '\x87', '\x19', '\x58', '\x52', '\x2E', '\x77', '\xCC',
    '\xDE', '\x86', '\x20', '\x5A', '\x5D', '\x24', '\x58', '\xDA', '\xF8', '\x9A', '\x19', '\x5F', '\x1F', '\x29', '\x46', '\xCF',
    '\xA7'
  }))
  );
  this->m_titleBar->SetSecondButton(closeButton);

  this->m_nameHeader = new CUIStaticText();
  this->m_nameHeader->SetSize(160, 18);
  this->m_nameHeader->Move(10, 30);
  this->m_nameHeader->SetText(DecryptName<0xDD3913D5D7F97BE1ULL>({'\x50', '\xC3', '\x45', '\x6D', '\x12', '\xA3', '\x19', '\x6D', '\x2A', '\xC0', '\x0F', '\xD7'}));
  this->m_nameHeader->SetFitText(0);
  this->m_nameHeader->SetAlign(2, 6);
  this->m_nameHeader->SetFont(std::string(DecryptName<0x050505679903579BULL>({'\xDC', '\x22', '\x6F', '\xF0', '\x0A', '\x05'})));
  this->m_nameHeader->SetFontHeight(12);
  this->m_nameHeader->SetBold(0);
  this->m_nameHeader->SetColor(0);
  this->m_nameHeader->SetShadow(0, 0xFFFFFF);
  this->m_nameHeader->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_nameHeader);

  this->m_searchEdit = new CUIEditBoxEx();
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

  this->m_searchButton = new CUIImageButton();
  this->m_searchButton->SetSize(25, 18);
  this->m_searchButton->Move(310, 30);
  this->m_searchButton->SetOnClick([this]() { this->Search(); });
  this->m_searchButton->disabled = 0;
  this->m_searchButton->Invalidate();
  this->m_searchButton->SetImages(
    std::string(DecryptName<0x2F8B0F37118D157FULL>({
    '\xBF', '\xBA', '\x4D', '\xEB', '\xF7', '\xC1', '\x4E', '\xE2', '\xB9', '\xF1', '\x4D', '\xDD', '\x8A', '\xB5', '\xD7', '\x41',
    '\x1E', '\x63', '\xE4', '\x76', '\x56', '\x7B', '\xE2', '\x40', '\x11', '\x4A', '\xE4', '\x7F', '\x43', '\x6A', '\xF9', '\x49',
    '\x1E', '\x76', '\xE8', '\x22', '\x6B', '\x6D', '\xFF', '\x41', '\x20', '\x66', '\xE8', '\x70', '\x45', '\x6C', '\xE3', '\x4D',
    '\x1E', '\x67', '\xD2', '\x7F', '\x58', '\x7D', '\xE6', '\x4E', '\x13', '\x3B', '\xEF', '\x7C', '\x47', '\x0F'
  })), std::string(DecryptName<0xAD075B8DB599F1CDULL>({
    '\x0D', '\x5E', '\x59', '\x4F', '\x4D', '\x95', '\xC2', '\x60', '\x0B', '\x15', '\x59', '\x79', '\x30', '\xE1', '\x5B', '\xC3',
    '\xAC', '\x87', '\xF0', '\xD2', '\xEC', '\x2F', '\x6E', '\xC2', '\xA3', '\xAE', '\xF0', '\xDB', '\xF9', '\x3E', '\x75', '\xCB',
    '\xAC', '\x92', '\xFC', '\x86', '\xD1', '\x39', '\x73', '\xC3', '\x92', '\x82', '\xFC', '\xD4', '\xFF', '\x38', '\x6F', '\xCF',
    '\xAC', '\x83', '\xC6', '\xC5', '\xFF', '\x3E', '\x74', '\xDE', '\xE3', '\x93', '\xF4', '\xC5', '\x8D'
  })),
    std::string(DecryptName<0x2FE993D1DDE1F197ULL>({
    '\x57', '\x5E', '\x21', '\x27', '\x11', '\x5D', '\x2C', '\xE2', '\x51', '\x15', '\x21', '\x11', '\x6C', '\x29', '\xB5', '\x41',
    '\xF6', '\x87', '\x88', '\xBA', '\xB0', '\xE7', '\x80', '\x40', '\xF9', '\xAE', '\x88', '\xB3', '\xA5', '\xF6', '\x9B', '\x49',
    '\xF6', '\x92', '\x84', '\xEE', '\x8D', '\xF1', '\x9D', '\x41', '\xC8', '\x82', '\x84', '\xBC', '\xA3', '\xF0', '\x81', '\x4D',
    '\xF6', '\x83', '\xBE', '\xB2', '\xA7', '\xF6', '\x9B', '\x01', '\xF5', '\x9C', '\x91', '\xDD'
  })), std::string(DecryptName<0x39FF7FB50DEB998FULL>({
    '\x4F', '\x36', '\x2B', '\xF7', '\x75', '\xB1', '\x3A', '\xF4', '\x49', '\x7D', '\x2B', '\xC1', '\x08', '\xC5', '\xA3', '\x57',
    '\xEE', '\xEF', '\x82', '\x6A', '\xD4', '\x0B', '\x96', '\x56', '\xE1', '\xC6', '\x82', '\x63', '\xC1', '\x1A', '\x8D', '\x5F',
    '\xEE', '\xFA', '\x8E', '\x3E', '\xE9', '\x1D', '\x8B', '\x57', '\xD0', '\xEA', '\x8E', '\x6C', '\xC7', '\x1C', '\x97', '\x5B',
    '\xEE', '\xEB', '\xB4', '\x63', '\xDA', '\x0D', '\x92', '\x58', '\xE3', '\xB7', '\x89', '\x60', '\xC5', '\x7F'
  }))
  );
  this->AddChild(this->m_searchButton);

  this->m_itemName = new CUIStaticText();
  this->m_itemName->SetSize(220, 24);
  this->m_itemName->Move(16, 61);
  this->m_itemName->SetText("");
  this->m_itemName->SetFitText(1);
  this->m_itemName->SetAlign(6, 6);
  this->m_itemName->SetFont(std::string(DecryptName<0xB7FD8F670FC7B96BULL>({'\x2C', '\xCC', '\xAB', '\x66', '\x0A', '\x8F'})));
  this->m_itemName->SetFontHeight(12);
  this->m_itemName->SetBold(1);
  this->m_itemName->SetColor(0);
  this->m_itemName->SetShadow(0, 0xFFFFFF);
  this->m_itemName->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_itemName);

  this->m_probabilityHeader = new CUIStaticText();
  this->m_probabilityHeader->SetSize(68, 24);
  this->m_probabilityHeader->Move(248, 61);
  this->m_probabilityHeader->SetText(DecryptName<0xE7978FB5A94DD7FDULL>({'\x35', '\x79', '\xFA', '\x55', '\xB5'}));
  this->m_probabilityHeader->SetFitText(1);
  this->m_probabilityHeader->SetAlign(6, 6);
  this->m_probabilityHeader->SetFont(std::string(DecryptName<0x4941FD9F21BBB9FDULL>({'\xBA', '\xCC', '\xD7', '\x48', '\xF2', '\xFD'})));
  this->m_probabilityHeader->SetFontHeight(12);
  this->m_probabilityHeader->SetBold(0);
  this->m_probabilityHeader->SetColor(0);
  this->m_probabilityHeader->SetShadow(0, 0xFFFFFF);
  this->m_probabilityHeader->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_probabilityHeader);

  CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* listBox = new CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>();
  this->m_list = listBox;
  listBox->itemWidth = 312;
  listBox->rowHeight = 30;
  listBox->SetSize(325, 360);
  this->m_list->Move(10, 85);
  this->m_list->SetOnSelect([this](ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data) { this->OnPrepareItem(item, data); });
  this->m_list->SetBinder([this](ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data) { this->BindItem(item, data); });
  this->m_list->SetImages("", "", "", "");
  this->m_list->itemX = 0;
  this->m_list->itemY = 0;
  this->m_list->scrollBar->SetSize(13, 360);
  this->m_list->scrollBar->Move(312, 0);
  this->m_list->scrollBar->step = 30;
  this->m_list->SetScrollBarButtonImages(std::string(DecryptName<0x35E5656B5DEDCD75ULL>({
    '\xB5', '\x62', '\x2D', '\xA7', '\xAB', '\xAB', '\x20', '\xF8', '\xB3', '\x29', '\x2D', '\x91', '\xD6', '\xDF', '\xB9', '\x46',
    '\x16', '\xBF', '\x82', '\x31', '\x07', '\x55', '\x90', '\x45', '\x5B', '\xAF', '\x80', '\x2D', '\x6B'
  })), std::string(DecryptName<0x793F370985ED674FULL>({
    '\x8F', '\xC8', '\x2D', '\x7F', '\xC9', '\xF9', '\xFA', '\xB4', '\x89', '\x83', '\x2D', '\x49', '\xB4', '\x8D', '\x63', '\x0A',
    '\x2C', '\x15', '\x82', '\xE9', '\x65', '\x07', '\x5B', '\x16', '\x38', '\x09', '\xC3', '\xE7', '\x64', '\x47', '\x3F'
  })));
  this->m_list->SetScrollBarBackground(std::string(DecryptName<0xE55723712993051DULL>({
    '\xDD', '\xAA', '\x53', '\xD3', '\xB1', '\xED', '\x92', '\x28', '\xDB', '\xE1', '\x53', '\xE5', '\xCC', '\x99', '\x0B', '\x96',
    '\x7E', '\x77', '\xFC', '\x45', '\x1D', '\x13', '\x3A', '\x8C', '\x79', '\x2B', '\xF1', '\x44', '\x01', '\x23'
  })));
  this->m_list->SetScrollBarThumbImages(
    std::string(DecryptName<0x7115D9F973F913C9ULL>({
    '\x09', '\xBC', '\x39', '\x89', '\x39', '\x17', '\xD0', '\xBC', '\x0F', '\xF7', '\x39', '\xBF', '\x44', '\x63', '\x49', '\x02',
    '\xAA', '\x61', '\x96', '\x1F', '\x95', '\xE9', '\x77', '\x10', '\xBB', '\x4C', '\x8C', '\x03', '\xD7', '\xBB', '\x78', '\x01',
    '\xC9'
  })), std::string(DecryptName<0xCB1D253B7FE1F563ULL>({
    '\xA3', '\x5A', '\x21', '\x85', '\xFB', '\xEB', '\xD8', '\x06', '\xA5', '\x11', '\x21', '\xB3', '\x86', '\x9F', '\x41', '\xB8',
    '\x00', '\x87', '\x8E', '\x13', '\x57', '\x15', '\x7F', '\xAA', '\x11', '\xAA', '\x8C', '\x16', '\x5F', '\x0B', '\x7F', '\xA6',
    '\x13', '\xF5'
  })), std::string(DecryptName<0x7941D76335CFA14FULL>({
    '\x8F', '\x0E', '\x0F', '\xCF', '\xA3', '\x19', '\x84', '\xB4', '\x89', '\x45', '\x0F', '\xF9', '\xDE', '\x6D', '\x1D', '\x0A',
    '\x2C', '\xD3', '\xA0', '\x59', '\x0F', '\xE7', '\x23', '\x18', '\x3D', '\xFE', '\xAB', '\x5A', '\x14', '\xB9', '\x6F', '\x1B',
    '\x22', '\xD1', '\xCF'
  }))
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
void CUIItem::OnInit() {
  this->nameText->scrollBar->SetHidden(1);
  this->probabilityText->scrollBar->SetHidden(1);
}

// OFFSET: 0x00A4E350
// FUNCTION: fn_16204
void CUIItem::OnLBtnDown(int, int) {
}

}
