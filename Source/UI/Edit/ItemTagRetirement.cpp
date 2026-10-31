#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Text/TextFitting.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

#pragma pack(push, 1)
struct ItemLinkOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};
#pragma pack(pop)

// INLINED
FocusedItemTagRemoval::FocusedItemTagRemoval(InputFocus& focus) : m_focus(focus) {
}

// OFFSET: 0x00896EB0
// FUNCTION: fn_11519
void FocusedItemTagRemoval::Remove(UIWindow* child) {
  if (this->m_focus.window) {
    this->m_focus.window->RemoveChild(child, 1);
  }
}

// OFFSET: 0x00896EE0
// FUNCTION: fn_11520
void ChatItemTagRemoval::Remove(UIWindow* child) {
  if (g_inputSuggestionsEnabled == 1 && g_chatSuggestions->unknown_00BC != nullptr && g_chatSuggestions->unknown_00BC->Contains(child)) {
    g_chatSuggestions->unknown_00BC->RemoveChild(child, 1);
    return;
  }

  UIWindow* hoverWindow = g_windowMgr.GetHoverWindow();
  if (hoverWindow != nullptr && hoverWindow->Contains(child)) {
    hoverWindow->RemoveChild(child, 1);
  }
}

// OFFSET: 0x00870DF0
// FUNCTION: fn_11104
UIItemTagCtrl::~UIItemTagCtrl() {
  reinterpret_cast<ItemTagRetirement*>(this)->Clear();
}

// OFFSET: 0x00879830
// FUNCTION: fn_11218
bool UIItemTagOnChat::CanInsertTag(const std::string& name, int width) {
  if (g_windowMgr.GetHoverWindow()->geometry.id != 490 && g_windowMgr.GetHoverWindow()->geometry.id != 494 &&
      !g_windowMgr.unknown_0024) {
    reinterpret_cast<ItemTagRetirement*>(this)->Clear();
    return false;
  }

  if (this->tags.size() >= 3) {
    return false;
  }

  std::string tag = "<";
  this->tagWidth = width;
  tag += FitText(*this->geometry.surface, name, width / 2);
  tag += ">";

  UIEditCtrl* edit = static_cast<UIEditCtrl*>(g_windowMgr.GetHoverWindow());
  this->taggedText = edit->text;
  std::string text = this->taggedText += tag;
  return text.size() <= static_cast<unsigned int>(edit->maximum);
}

// OFFSET: 0x00879A10
// FUNCTION: fn_11219
bool UIItemTagOnMail::CanInsertTag(const std::string& name, int width) {
  if (this->tags.size() > 2) {
    return false;
  }

  std::string tag = "<";
  this->tagWidth = width;
  tag += FitText(*this->geometry.surface, name, width / 2);
  tag += ">";

  UIRodexEditCtrl3* contents = reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->m_contents;
  const unsigned int maximum = contents->maximum;
  this->taggedText = contents->text;
  std::string text = this->taggedText += tag;
  return text.size() <= maximum;
}

// OFFSET: 0x0087ABB0
// FUNCTION: fn_11239
bool UIItemTagCtrl::SyncTagsWithText() {
  std::string text = this->taggedText;
  std::list<ItemTagRecord> kept;

  while (text.find("<") != std::string::npos) {
    std::string token = text.substr(text.find("<"));

    if (token.find(">") == std::string::npos) {
      text = text.substr(text.find("<") + 1);
      continue;
    }

    text = text.substr(text.find(">") + 1);
    token = token.substr(0, token.find(">") + 1);

    for (auto entry = this->tags.begin(); entry != this->tags.end(); ++entry) {
      std::string fitted = FitText(*this->geometry.surface, entry->name, this->tagWidth / 2);
      if (token.find(fitted) != std::string::npos) {
        entry->index = kept.size();
        kept.push_back(*entry);
        this->tags.erase(entry);
        break;
      }
    }
  }

  reinterpret_cast<ItemTagRetirement*>(this)->Clear();
  this->tags.swap(kept);
  return true;
}

// INLINED
MailItemTagRemoval::MailItemTagRemoval(UIWindow* const& owner) : m_owner(owner) {
}

// OFFSET: 0x00896F60
// FUNCTION: fn_11521
void MailItemTagRemoval::Remove(UIWindow* child) {
  if (this->m_owner) {
    this->m_owner->RemoveChild(child, 1);
  }
}

// INLINED
MegaPhoneItemTagRemoval::MegaPhoneItemTagRemoval(UIWindow* const& owner) : m_owner(owner) {
}

// OFFSET: 0x00896F80
// FUNCTION: fn_11522
void MegaPhoneItemTagRemoval::Remove(UIWindow* child) {
  if (this->m_owner) {
    this->m_owner->RemoveChild(child, 1);
  }
}

// OFFSET: 0x0087A2F0
// FUNCTION: fn_11231
void ItemTagRetirement::Clear() {
  for (auto entry = this->records.begin(); entry != this->records.end();) {
    this->removal.Remove(entry->child);
    entry = this->records.erase(entry);
  }
}

// OFFSET: 0x00879820
// FUNCTION: fn_11217
bool UIItemTagCtrl::CanInsertTag(const std::string&, int) {
  return true;
}

// OFFSET: 0x0087A5A0
// FUNCTION: fn_11235
std::string UIItemTagCtrl::AppendTag(const std::string& name) {
  std::string tag = "<";
  tag += FitText(*this->geometry.surface, name, this->tagWidth / 2);
  tag += ">";
  this->taggedText += tag;
  return this->taggedText;
}

// OFFSET: 0x0089CF10
// FUNCTION: fn_11595
void UIItemTagOnMail::SetLines(const std::vector<std::string>& lines) {
  this->m_lines.clear();

  for (std::size_t index = 0; index < lines.size(); ++index) {
    this->m_lines.push_back(lines[index]);
  }
}

// OFFSET: 0x0089CFE0
// FUNCTION: fn_11597
void UIItemTagCtrl::sub_89CFE0(const ITEM_INFO& item, unsigned int color) {
  std::string tag = "<";
  tag += item.DisplayName(0);
  tag += ">";

  UIItemTagButton* button = new UIItemTagButton(*this->m_context, this->m_edit->buttons);

  std::string label = "<";
  label += FitText(*this->geometry.surface, item.DisplayName(0), this->tagWidth / 2);
  label += ">";

  button->Configure(label.c_str(), color, 0, 12, 0, 0);
  button->SetTooltip(tag.c_str());
  button->SetId(0x1D0);
  button->Move(0, 0);
  reinterpret_cast<ITEM_INFO&>(button->unknown_00F8) = item;
  this->AddTagButton(button);

  UITextViewer* links = reinterpret_cast<UITextViewer*>(this);
  std::string link;
  char delimiters[] = "!#$%&'()*+,-/";
  if (links->ItemLinkNumberText(item.unknown_0000[2]).size() <= 5) {
    for (unsigned int index = 0; index < 5 - links->ItemLinkNumberText(item.unknown_0000[2]).size(); ++index) {
      link += "0";
    }
  }

  link += links->ItemLinkNumberText(item.unknown_0000[2]);

  bool equipment;
  switch (item.unknown_0000[0]) {
  case 4:
  case 5:
  case 8:
  case 9:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 21:
    equipment = true;
    break;
  default:
    equipment = false;
    break;
  }

  link += equipment ? "1" : "0";
  link += links->ItemLinkNumberText(std::atoi(item.itemId.c_str()));

  if (item.unknown_0060 != 0) {
    link += delimiters[3];
    link += links->ItemLinkNumberText(item.unknown_0060);
  }

  link += delimiters[4];
  link += links->ItemLinkNumberText(item.unknown_0070);
  link += delimiters[5];
  link += links->ItemLinkNumberText(item.unknown_0088);

  const int* cards = reinterpret_cast<const int*>(item.unknown_001C);
  int slots = ItemSlotsCount(item);
  if (slots == 0) {
    slots = 4;
  }

  for (int index = 0; index < 4; ++index) {
    if (cards[index] != 0) {
      slots = 4;
    }
  }

  for (int index = 0; index < slots; ++index) {
    link += delimiters[7];
    link += links->ItemLinkNumberText(index < 4 ? cards[index] : 0);
  }

  const ItemLinkOption* options = reinterpret_cast<const ItemLinkOption*>(item.unknown_009C);
  for (int index = 0; index < item.unknown_0098; ++index) {
    link += delimiters[9];
    link += links->ItemLinkNumberText(options[index].id);
    link += delimiters[10];
    link += links->ItemLinkNumberText(options[index].parameter);
    link += delimiters[11];
    link += links->ItemLinkNumberText(options[index].value);
  }

  ItemTagRecord record;
  char number[64];
  std::memset(number, 0, sizeof(number));
  record.child = button;
  StringFormat(number, sizeof(number), _TRUNCATE, "%s", links->ItemLinkNumberText(std::atoi(item.itemId.c_str())).c_str());
  StringFormat(record.number, sizeof(record.number), _TRUNCATE, "%s", number);
  StringFormat(record.name, sizeof(record.name), _TRUNCATE, "%s", item.DisplayName(0).c_str());
  StringFormat(record.link, sizeof(record.link), _TRUNCATE, "%s", link.c_str());
  this->tags.push_back(record);
}

// OFFSET: 0x00881080
// FUNCTION: fn_11294
void UIItemTagCtrl::SetTaggedText(const std::string& text, int width) {
  this->taggedText = text;
  this->tagWidth = width;
}

// OFFSET: 0x00879BC0
// FUNCTION: fn_11220
bool UIItemTagOnMegaPhone::CanInsertTag(const std::string& name, int width) {
  if (this->tags.size() > 2) {
    return false;
  }

  std::string tag = "<";
  this->tagWidth = width;
  tag += FitText(*this->geometry.surface, name, width / 2);
  tag += ">";

  unsigned bytes = 0;
  this->textWidth = reinterpret_cast<UIItemTagCtrl*>(g_windowMgr.unknown_0318)->MeasureText(tag, bytes);

  UIEditCtrl* edit = reinterpret_cast<UINpcEditDialogWnd*>(g_windowMgr.unknown_0318)->edit;
  const unsigned int maximum = edit->maximum;
  this->taggedText = edit->text;
  std::string text = this->taggedText += tag;
  return text.size() <= maximum;
}
