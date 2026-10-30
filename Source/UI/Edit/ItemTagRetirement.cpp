#include "UI/Chat/ChatSuggestions.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Text/TextFitting.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// INLINED
FocusedItemTagRemoval::FocusedItemTagRemoval(InputFocus& focus) : m_focus(focus) {
}

// GLOBAL: gl_24635
bool g_inputSuggestionsEnabled;

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

// OFFSET: 0x00881080
// FUNCTION: fn_11294
void UIItemTagCtrl::SetTaggedText(const std::string& text, int width) {
  this->taggedText = text;
  this->tagWidth = width;
}
