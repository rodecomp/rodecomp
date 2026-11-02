#ifndef RAGNAROK_SOURCE_UI_EDIT_UIEDITPAINT_INL
#define RAGNAROK_SOURCE_UI_EDIT_UIEDITPAINT_INL

#include "Client/Input/Language.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"

#include <Windows.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

// INLINED
__forceinline void UIEditCtrl::PaintTextWithPolicy(const std::string* placeholder, std::uint32_t placeholderColor, bool placeholderEnabled) {
  int x = 3;
  if (this->invalidatesParent == 0 && this->firstVisible != 0) {
    x -= this->TextWidth(this->text.c_str(), this->firstVisible, 0, 12, 0, 0);
  }

  std::string display;
  if (g_windowMgr.GetHoverWindow() == this) {
    this->Synchronize();
    unsigned begin;
    unsigned end;
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
      begin = std::min(this->anchor, this->cursor);
      end = std::max(this->anchor, this->cursor);
    } else {
      const char* prefix = g_serverLanguage->Prefix(false);
      display = prefix + this->text;
      const unsigned prefixBytes = static_cast<unsigned>(std::strlen(prefix));
      begin = std::min(this->anchor, this->cursor) + prefixBytes;
      end = std::max(this->anchor, this->cursor) + g_serverLanguage->compositionCursor + prefixBytes;
    }

    const int left = begin ? this->TextWidth(display.c_str(), begin, 0, 12, 0, 0) : 0;
    const int right = end ? this->TextWidth(display.c_str(), end, 0, 12, 0, 0) : 0;

    if (std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor) != 0) {
      this->DrawSelection(left + x, right - left);
    } else if (g_serverLanguage->compositionCursor != 0) {
      this->MoveCandidateWindow(left);
      this->MoveReadingWindow(right);
      if (((timeGetTime() / 500) & 1) == 0) {
        this->DrawSelection(left + x, std::max(right - left, 2));
      }
    } else {
      if (this->secondColor == 0xFFFFFFFF) {
        this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
      } else {
        this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
      }
      if (((timeGetTime() / 500) & 1) == 0 && IsInputSuggestionAllowed()) {
        this->DrawTextAt(left + x - 2, this->textY, "|", 0, 0, 12, this->textColor, 0, 0);
      }
      if (this->invalidatesParent != 0 && this->parent) {
        this->parent->Invalidate();
      }
      this->UpdateLinks();
      return;
    }

    if (this->secondColor == 0xFFFFFFFF) {
      this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
    } else {
      this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
    }
    this->UpdateLinks();
    return;
  }

  if (this->mask != 0) {
    display.assign(this->text.size(), static_cast<char>(this->mask));
  } else {
    display = this->text;
  }
  if (this->secondColor == 0xFFFFFFFF) {
    if (placeholderEnabled) {
      this->DrawTextAt(x, this->textY, display == "" ? placeholder->c_str() : display.c_str(), 0, 0, 12, placeholderColor, 0, 0);
    } else {
      this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
    }
  } else {
    this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
  }
  if (this->invalidatesParent != 0 && this->parent) {
    this->parent->Invalidate();
  }
  this->UpdateLinks();
}

#endif
