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

// OFFSET: 0x00896EB0
// FUNCTION: fn_11519
void FocusedItemTagRemoval::Remove(UIWindow* child) {
  if (this->m_focus.window) {
    this->m_focus.window->RemoveChild(child, 1);
  }
}

// OFFSET: 0x00879830

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
