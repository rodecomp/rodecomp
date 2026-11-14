#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
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

// OFFSET: 0x00896EB0
// FUNCTION: fn_11519
void UIItemTagCtrl::RemoveTagChild(UIWindow* child) {
  UIWindow* hoverWindow = g_windowMgr.GetHoverWindow();
  if (hoverWindow) {
    hoverWindow->RemoveChild(child, 1);
  }
}

// OFFSET: 0x00896EE0
// FUNCTION: fn_11520
void UIItemTagOnChat::RemoveTagChild(UIWindow* child) {
  if (g_windowMgr.inputSuggestionsEnabled == 1) {
    UIEditCtrl* chatEdit = ChatWindow()->m_chatEdit;
    if (chatEdit != nullptr && std::find(chatEdit->m_children.begin(), chatEdit->m_children.end(), child) != chatEdit->m_children.end()) {
      chatEdit->RemoveChild(child, 1);
      return;
    }
  }

  UIWindow* hoverWindow = g_windowMgr.GetHoverWindow();
  if (hoverWindow != nullptr && std::find(hoverWindow->m_children.begin(), hoverWindow->m_children.end(), child) != hoverWindow->m_children.end()) {
    hoverWindow->RemoveChild(child, 1);
  }
}

// OFFSET: 0x0086D670
// FUNCTION: fn_11050
UIItemTagCtrl::UIItemTagCtrl() : previousTagCount(0), tagWidth(0) {
  this->taggedText.clear();
  this->linkedText.clear();
}

// OFFSET: 0x0086D780
// FUNCTION: fn_11051
UIItemTagOnChat::UIItemTagOnChat() {
}

// OFFSET: 0x0086D7E0
// FUNCTION: fn_11052
UIItemTagOnMail::UIItemTagOnMail() {
}

// OFFSET: 0x0086D860
// FUNCTION: fn_11053
UIItemTagOnMegaPhone::UIItemTagOnMegaPhone() {
}

// OFFSET: 0x00870DF0
// FUNCTION: fn_11104
UIItemTagCtrl::~UIItemTagCtrl() {
  this->ClearTags();
}

// OFFSET: 0x00879830
// FUNCTION: fn_11218
bool UIItemTagOnChat::CanInsertTag(const std::string& name, int width) {
  if (g_windowMgr.GetHoverWindow()->geometry.id != 490 && g_windowMgr.GetHoverWindow()->geometry.id != 494 &&
      !g_windowMgr.inputSuggestionsEnabled) {
    this->ClearTags();
    return false;
  }

  if (this->tags.size() >= 3) {
    return false;
  }

  std::string tag = "<";
  this->tagWidth = width;
  tag += this->FitTextToWidth(name, width / 2);
  tag += ">";

  UIEditCtrl* edit = static_cast<UIEditCtrl*>(g_windowMgr.GetHoverWindow());
  this->taggedText = edit->text;
  this->taggedText += tag;
  return this->taggedText.size() <= static_cast<unsigned int>(edit->maximum);
}

// OFFSET: 0x00879A10
// FUNCTION: fn_11219
bool UIItemTagOnMail::CanInsertTag(const std::string& name, int width) {
  if (this->tags.size() > 2) {
    return false;
  }

  std::string tag = "<";
  this->tagWidth = width;
  tag += this->FitTextToWidth(name, width / 2);
  tag += ">";

  UIRodexEditCtrl3* contents = static_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->m_contents;
  const unsigned int maximum = contents->maximum;
  this->taggedText = contents->text;
  this->taggedText += tag;
  return this->taggedText.size() <= maximum;
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
      std::string fitted = this->FitTextToWidth(entry->name, this->tagWidth / 2);
      if (token.find(fitted) != std::string::npos) {
        entry->index = kept.size();
        kept.push_back(*entry);
        this->tags.erase(entry);
        break;
      }
    }
  }

  this->ClearTags();
  this->tags.swap(kept);
  return true;
}

// OFFSET: 0x00896F60
// FUNCTION: fn_11521
void UIItemTagOnMail::RemoveTagChild(UIWindow* child) {
  if (g_windowMgr.rodexComposeWnd) {
    g_windowMgr.rodexComposeWnd->RemoveChild(child, 1);
  }
}

// OFFSET: 0x00896F80
// FUNCTION: fn_11522
void UIItemTagOnMegaPhone::RemoveTagChild(UIWindow* child) {
  if (g_windowMgr.npcTextEditDialogWnd) {
    g_windowMgr.npcTextEditDialogWnd->RemoveChild(child, 1);
  }
}

// OFFSET: 0x0087A2F0
// FUNCTION: fn_11231
void UIItemTagCtrl::ClearTags() {
  for (auto entry = this->tags.begin(); entry != this->tags.end();) {
    this->RemoveTagChild(entry->child);
    entry = this->tags.erase(entry);
  }

  this->tags.clear();
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
  tag += this->FitTextToWidth(name, this->tagWidth / 2);
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
void UIItemTagCtrl::AddItemTag(const ITEM_INFO& item, unsigned int color) {
  std::string tag = "<";
  tag += item.DisplayName(0);
  tag += ">";

  UIItemTagButton* button = new UIItemTagButton();

  std::string label = "<";
  label += this->FitTextToWidth(item.DisplayName(0), this->tagWidth / 2);
  label += ">";

  button->Configure(label.c_str(), color, 0, 12, 0, 0);
  button->SetTooltip(tag.c_str());
  button->SetId(0x1D0);
  button->Move(0, 0);
  button->item = item;
  this->AddTagButton(button);

  std::string link;
  char delimiters[] = "!#$%&'()*+,-/";
  if (this->ItemLinkNumberText(item.identity[2]).size() <= 5) {
    for (unsigned int index = 0; index < 5 - this->ItemLinkNumberText(item.identity[2]).size(); ++index) {
      link += "0";
    }
  }

  link += this->ItemLinkNumberText(item.identity[2]);

  link += Equipment(item) ? "1" : "0";
  link += this->ItemLinkNumberText(std::atoi(item.itemId.c_str()));

  if (item.refiningLevel != 0) {
    link += delimiters[3];
    link += this->ItemLinkNumberText(item.refiningLevel);
  }

  link += delimiters[4];
  link += this->ItemLinkNumberText(item.sprite);
  link += delimiters[5];
  link += this->ItemLinkNumberText(item.grade);

  const std::uint32_t* cards = item.cards;
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
    link += this->ItemLinkNumberText(index < 4 ? cards[index] : 0);
  }

  const ItemLinkOption* options = reinterpret_cast<const ItemLinkOption*>(item.options);
  for (int index = 0; index < item.optionCount; ++index) {
    link += delimiters[9];
    link += this->ItemLinkNumberText(options[index].id);
    link += delimiters[10];
    link += this->ItemLinkNumberText(options[index].parameter);
    link += delimiters[11];
    link += this->ItemLinkNumberText(options[index].value);
  }

  ItemTagRecord record;
  char number[64];
  std::memset(number, 0, sizeof(number));
  record.child = button;
  StringFormat(number, sizeof(number), _TRUNCATE, "%s", this->ItemLinkNumberText(std::atoi(item.itemId.c_str())).c_str());
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
  tag += this->FitTextToWidth(name, width / 2);
  tag += ">";

  unsigned bytes = 0;
  this->textWidth = static_cast<UINpcTextEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->MeasureText(tag, bytes);

  UIEditCtrl* edit = static_cast<UINpcEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->edit;
  const unsigned int maximum = edit->maximum;
  this->taggedText = edit->text;
  this->taggedText += tag;
  return this->taggedText.size() <= maximum;
}

// OFFSET: 0x008962C0
// FUNCTION: fn_11517
void UIItemTagCtrl::UpdateItemTags() {
  this->linkedText.clear();
  if (this->previousTagCount != static_cast<int>(this->tags.size()) && this->SyncTagsWithText()) {
    this->previousTagCount = static_cast<int>(this->tags.size());
  }

  std::string prefix;
  std::string remaining = this->taggedText;
  if (!this->tags.empty()) {
    for (auto entry = this->tags.begin(); entry != this->tags.end();) {
      if (this->RemoveInvalidTag(entry)) {
        break;
      }

      if (!this->geometry.surface) {
        this->Resize(this->tagWidth / 2, 15);
      }
      const std::string fitted = this->FitTextToWidth(entry->name, this->tagWidth / 2);
      std::string display = "<";
      display += fitted;
      display += ">";

      const std::size_t found = remaining.find(display);
      if (found != std::string::npos) {
        const std::string head = remaining.substr(0, found);
        prefix += head;

        std::string visible = head;
        std::size_t firstVisible = 0;
        int x = 0;
        UIWindow* hover = g_windowMgr.GetHoverWindow();
        if (hover != nullptr) {
          switch (hover->geometry.id) {
          case 0x1EA:
          case 0x1EC:
          case 0x1ED:
          case 0x1EE:
            firstVisible = static_cast<UIEditCtrl*>(hover)->firstVisible;
            break;
          }

          if (firstVisible <= prefix.size()) {
            visible = prefix.substr(firstVisible);
          }

          if (firstVisible != 0 && prefix.size() <= firstVisible) {
            x = 1000;
          }
        }

        this->PlaceTagButton(entry->child, visible, x, 0);

        const int bytes = static_cast<int>(std::strlen(fitted.c_str()));
        if (!g_windowMgr.rodexComposeWnd || static_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->m_contents == g_windowMgr.GetHoverWindow()) {
          const int cursor = g_serverLanguage->cursor;
          const int start = static_cast<int>(prefix.size());
          const int end = start + bytes + 2;
          if (cursor > start && cursor < end) {
            const int target = start + bytes / 2 + 1 > cursor ? end : start;
            g_serverLanguage->Select(target, target);
          }
        }

        this->linkedText += head;
        this->linkedText += itemLinkTagOpening;
        this->linkedText += entry->link;
        this->linkedText += itemLinkTagClosing;
        remaining = remaining.substr(found + display.size());
        prefix += display;
      }

      ++entry;
    }
  }

  this->linkedText += remaining;
}
