#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Input/InputCharset.h"
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
bool UIEditCtrl::RemoveInvalidLink(int type, std::list<UIWindow*>::iterator& entry) {
  std::string name;

  if (type == 438) {
    name = FormatNavigationLink(static_cast<UINaviLinkButton*>(*entry)->Link(), false);
  }

  const std::string current = this->text;

  if (name.empty()) {
    return true;
  }

  const std::string fitted = FitText(*this->geometry.surface, name, this->geometry.width / 2);
  std::string remaining = current;
  const int bytes = static_cast<int>(std::strlen(fitted.c_str()));
  int found = static_cast<int>(current.find(fitted));

  if (found == -1) {
    this->RemoveChild(*entry, 1);
    entry = this->links.erase(entry);
    return true;
  }

  int position = found;

  do {
    std::string before;

    if (found > 0) {
      before = remaining.substr(found - 1, 1);
    }

    std::string after = "";

    if (found + bytes >= 0) {
      after = remaining.substr(found + bytes, 1);
    }

    int boundary;

    if (before.find("<") != std::string::npos && after.find(">") == std::string::npos) {
      if (g_windowMgr.unknown_0458 &&
          reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->m_contents != g_windowMgr.GetHoverWindow()) {
        continue;
      }

      boundary = position + bytes;
    } else if (before.find("<") == std::string::npos && after.find(">") != std::string::npos) {
      if (g_windowMgr.unknown_0458 &&
          reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->m_contents != g_windowMgr.GetHoverWindow()) {
        continue;
      }

      boundary = position;
    } else if (before.find("<") != std::string::npos && after.find(">") != std::string::npos) {
      return false;
    } else {
      remaining = remaining.substr(found + bytes);
      position = (position == found ? position : position + found) + bytes;
      found = static_cast<int>(remaining.find(fitted));
      continue;
    }

    if (boundary == g_serverLanguage->cursor) {
      remaining = before.find("<") != std::string::npos ? current.substr(0, position - 1) : current.substr(0, position);
      remaining += current.substr((after.find(">") != std::string::npos ? position + 1 : position) + bytes);
      g_serverLanguage->ResetInput();
      g_serverLanguage->Insert(remaining.c_str());
      this->RemoveChild(*entry, 1);
      entry = this->links.erase(entry);
      return true;
    }
  } while (found != -1);

  return true;
}

// OFFSET: 0x008550B0
// FUNCTION: fn_10749
void UIEditCtrl::SetCursor(std::size_t position) {
  position = std::min(position, this->text.size());
  this->anchor = this->cursor = static_cast<int>(position);
  g_serverLanguage->Select(this->cursor, this->cursor);
}

// OFFSET: 0x00855150
// FUNCTION: fn_10751
void UIEditCtrl::MoveCursorToEnd() {
  if (g_windowMgr.GetHoverWindow() == this) {
    g_serverLanguage->Select(static_cast<int>(g_serverLanguage->text.size()), static_cast<int>(g_serverLanguage->text.size()));
    return;
  }
  this->anchor = static_cast<int>(this->text.size());
  this->cursor = static_cast<int>(this->text.size());
}

// OFFSET: 0x008550E0
// FUNCTION: fn_10750
void UIEditCtrl::SnapLinkSelection(const std::string& prefix, const int& bytes) {
  if (g_windowMgr.unknown_0458 && reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->m_contents != g_windowMgr.GetHoverWindow()) {
    return;
  }

  const int start = static_cast<int>(prefix.size());
  const int end = start + 2 + bytes;
  const int cursor = g_serverLanguage->cursor;

  if (cursor > start && cursor < end) {
    if (start + bytes / 2 + 1 > cursor) {
      g_serverLanguage->Select(end, end);
    } else {
      g_serverLanguage->Select(start, start);
    }
  }
}

// OFFSET: 0x008624B0
// FUNCTION: fn_10904
void UIEditCtrl::UpdateLinks() {
  if (this->links.size() == 0) {
    return;
  }

  bool hasLink = false;

  for (UIWindow* window : this->links) {
    if (window->geometry.id == 438) {
      hasLink = true;
    }
  }

  if (!hasLink) {
    return;
  }

  this->linkedText.clear();
  std::string prefix;
  std::string remaining = this->text;

  for (auto entry = this->links.begin(); entry != this->links.end(); ++entry) {
    if (this->RemoveInvalidLink(438, entry)) {
      remaining = g_serverLanguage->text.c_str();
      break;
    }

    auto* button = static_cast<UINaviLinkButton*>(*entry);
    const std::string name = FormatNavigationLink(button->Link(), false);
    const std::string fitted = FitText(*this->geometry.surface, name, this->geometry.width / 2);
    std::string display = "<" + fitted;
    display += ">";
    const int found = static_cast<int>(remaining.find(display));

    if (found != -1) {
      const std::string head = remaining.substr(0, found);
      prefix += head;
      std::string visible = head;

      if (prefix.size() - 1 < static_cast<unsigned>(this->firstVisible)) {
        visible = prefix.substr(prefix.size() - 1);
      } else {
        visible = prefix.substr(this->firstVisible);
      }

      if (this->firstVisible != 0 && prefix.size() <= static_cast<unsigned>(this->firstVisible)) {
        (*entry)->Move(1000, this->textY - 1);
      } else {
        (*entry)->Move(this->TextWidth(visible.c_str(), 0, 0, 12, 0, 0) + 2, this->textY - 1);
      }

      this->SnapLinkSelection(prefix, static_cast<int>(std::strlen(fitted.c_str())));
      this->linkedText += head;
      this->linkedText += button->Link();
      remaining = remaining.substr(found + display.size());
      prefix += display;
    }
  }

  this->linkedText += remaining;
}

// OFFSET: 0x0085A5D0
// FUNCTION: fn_10779
std::string UIEditCtrl::BracketFittedText(const std::string& name) {
  std::string result;
  result += '<';
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
  g_serverLanguage->ResetInput();
  g_serverLanguage->Insert(combined.c_str());
  this->unknown_0144->taggedText = this->text;
}

