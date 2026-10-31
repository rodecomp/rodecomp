#include "Client/Input/Language.h"
#include "UI/Window/InputFocus.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/TextFitting.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Buttons/UINaviLinkButton.h"

#include <algorithm>
#include <cstring>
#include <mbstring.h>
#include <new>

// OFFSET: 0x00854470
// FUNCTION: fn_10748
bool UIEditCtrl::RemoveInvalidLink(std::list<UIWindow*>::iterator& entry) {
  auto* button = static_cast<UINaviLinkButton*>(*entry);
  const std::string name = FormatNavigationLink(button->link, false);
  std::string remaining = this->text;

  if (name.empty()) {
    return true;
  }

  const std::string fitted = FitText(*this->geometry.surface, name, this->geometry.width / 2);
  std::size_t found = remaining.find(fitted);

  if (found == std::string::npos) {
    this->RemoveChild(button, 1);
    entry = this->links.erase(entry);
    return true;
  }

  std::size_t absolute = found;
  const std::size_t bytes = fitted.size();

  while (found != std::string::npos) {
    const bool left = found > 0 && remaining[found - 1] == '<';
    const bool right = found + bytes < remaining.size() && remaining[found + bytes] == '>';

    if (left && right) {
      return false;
    }

    if (left != right) {
      const bool allowed = !this->m_edit->linkInputRestricted || this->m_edit->permittedLinkInput == this->m_edit->focus.window;
      const auto boundary = left ? absolute + bytes : absolute;

      if (allowed && boundary == static_cast<unsigned>(this->m_edit->input.cursor)) {
        const auto begin = left ? absolute - 1 : absolute;
        const auto end = absolute + bytes + (right ? 1 : 0);
        remaining = this->text.substr(0, begin) + this->text.substr(end);
        this->m_edit->input.ResetInput();
        this->m_edit->input.Insert(remaining.c_str());
        this->RemoveChild(button, 1);
        entry = this->links.erase(entry);

        return true;
      }
    }

    remaining = remaining.substr(found + bytes);
    const auto previous = found;
    found = remaining.find(fitted);

    if (found != std::string::npos) {
      absolute = bytes + (absolute == previous ? absolute : absolute + previous);
    }
  }

  return true;
}

// OFFSET: 0x008550B0
// FUNCTION: fn_10749
void UIEditCtrl::SetCursor(std::size_t position) {
  position = std::min(position, this->text.size());
  this->anchor = this->cursor = static_cast<int>(position);
  this->m_edit->input.Select(this->cursor, this->cursor);
}

// OFFSET: 0x00855150
// FUNCTION: fn_10751
void UIEditCtrl::MoveCursorToEnd() {
  if (this->m_edit->focus.window == this) {
    this->m_edit->input.Select(static_cast<int>(this->m_edit->input.text.size()), static_cast<int>(this->m_edit->input.text.size()));
    return;
  }
  this->anchor = static_cast<int>(this->text.size());
  this->cursor = static_cast<int>(this->text.size());
}

// OFFSET: 0x008550E0
// FUNCTION: fn_10750
void UIEditCtrl::SnapLinkSelection(std::size_t prefix, std::size_t bytes) {
  if (this->m_edit->linkInputRestricted && this->m_edit->permittedLinkInput != this->m_edit->focus.window) {
    return;
  }

  const auto cursor = static_cast<unsigned>(this->m_edit->input.cursor);
  const auto end = prefix + bytes + 2;

  if (cursor > prefix && cursor < end) {
    const auto target = prefix + bytes / 2 + 1 <= cursor ? prefix : end;
    this->m_edit->input.Select(static_cast<int>(target), static_cast<int>(target));
  }
}

// OFFSET: 0x008624B0
// FUNCTION: fn_10904
void UIEditCtrl::UpdateLinks() {
  if (std::none_of(this->links.begin(), this->links.end(), [](const UIWindow* window) { return window->geometry.id == 438; })) {
    return;
  }

  this->linkedText.clear();
  std::string prefix;
  std::string remaining = this->text;

  for (auto entry = this->links.begin(); entry != this->links.end();) {
    if (this->RemoveInvalidLink(entry)) {
      remaining.assign(this->m_edit->input.text.c_str());
      break;
    }

    auto* button = static_cast<UINaviLinkButton*>(*entry);
    const std::string name = FormatNavigationLink(button->link, false);
    const std::string fitted = FitText(*this->geometry.surface, name, this->geometry.width / 2);
    const std::string display = "<" + fitted + ">";
    const auto found = remaining.find(display);

    if (found != std::string::npos) {
      const std::string head = remaining.substr(0, found);
      prefix += head;
      const auto end = prefix.size() - 1;
      std::string visible;

      if (end >= static_cast<unsigned>(this->firstVisible)) {
        visible = prefix.substr(static_cast<unsigned>(this->firstVisible));
      } else {
        visible = prefix.substr(end, 1);
      }

      if (this->firstVisible != 0 && prefix.size() <= static_cast<unsigned>(this->firstVisible)) {
        button->Move(1000, this->textY - 1);
      } else {
        button->Move(this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, visible.c_str(), 0, 0, 12, 0, 0) + 2, this->textY - 1);
      }

      this->SnapLinkSelection(prefix.size(), fitted.size());
      this->linkedText += head;
      this->linkedText += button->link;
      remaining = remaining.substr(found + display.size());
      prefix += display;
    }

    ++entry;
  }

  this->linkedText += remaining;
}

// OFFSET: 0x0085A5D0
// FUNCTION: fn_10779
std::string UIEditCtrl::BracketFittedText(const std::string& name) {
  std::string result;
  result = '<';
  result += FitText(*this->geometry.surface, name, this->geometry.width / 2);
  result += '>';
  return result;
}

// OFFSET: 0x0085A870
// FUNCTION: fn_10780
void UIChatEditCtrl::InsertItemTag(int tagId) {
  if (!this->unknown_0144) {
    this->unknown_0144 = new (std::nothrow) UIItemTagOnChat();
    if (!this->unknown_0144) {
      return;
    }
  }

  if (static_cast<int>(this->unknown_0144->tags.size()) >= 3 || this->unknown_00F0 + this->text.size() > static_cast<unsigned>(this->maximum)) {
    return;
  }

  const std::string tag = this->unknown_0144->sub_874A40(tagId, 0x7F00E4);
  const std::string combined = this->text + tag;
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(combined.c_str());
  this->unknown_0144->taggedText = this->text;
}

// INLINED
void UIEditCtrl::InsertLink(const char* value) {
  const std::string name = FormatNavigationLink(value, false);

  const std::string display = this->BracketFittedText(name);
  std::string before;
  bool removed = false;
  std::size_t position = std::string::npos;
  std::size_t oldBytes = 0;

  for (auto entry = this->links.begin(); entry != this->links.end();) {
    UIWindow* window = *entry;

    if (window->geometry.id == 438) {
      removed = true;
      const auto* button = static_cast<const UINaviLinkButton*>(window);
      const std::string fitted = FitText(
        *this->geometry.surface, FormatNavigationLink(button->link, false), this->geometry.width / 2
      );
      const std::string old = "<" + fitted + ">";

      oldBytes = old.size();
      position = this->text.find(old);

      if (position != std::string::npos) {
        before = this->text.substr(0, position);
      }

      this->RemoveChild(window, 1);
      entry = this->links.erase(entry);
    } else {
      ++entry;
    }
  }

  if (!_mbsstr(reinterpret_cast<const unsigned char*>(value), reinterpret_cast<const unsigned char*>("<NAVIL>")) &&
      !_mbsstr(reinterpret_cast<const unsigned char*>(value), reinterpret_cast<const unsigned char*>("</NAVIL>"))) {
    this->SetText(value);
    return;
  }

  std::string transformed;
  DisplayNavigationLink(transformed, value);
  const std::string prospective = this->text + transformed;

  if (static_cast<unsigned>(this->maximum) < prospective.size()) {
    return;
  }

  if (removed) {
    if (position != std::string::npos) {
      this->text = before + display + this->text.substr(position + oldBytes);
    }
  } else {
    this->text += transformed;
  }

  this->SetText(this->text.c_str());
  this->anchor = this->cursor = static_cast<int>(this->text.size());
  this->m_edit->input.Select(this->cursor, this->cursor);

  auto* button = new UINaviLinkButton(*this->m_context, this->m_edit->buttons, this->m_edit->navigation);

  button->Configure(display.c_str(), 0x009E68F0, 0, 12, 0, 0);
  button->SetId(438);
  ExtractNavigationLink(button->link, value);
  this->AddChild(button);
  this->links.push_back(button);
}
