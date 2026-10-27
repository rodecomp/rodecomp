#ifndef RAGNAROK_SOURCE_UI_CHAT_CHATNOTIFICATIONS_H
#define RAGNAROK_SOURCE_UI_CHAT_CHATNOTIFICATIONS_H

#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/UIWindow.h"

#include <cstddef>
#include <cstring>
#include <vector>
#include <cstdint>
#include <list>
#include <string>

class UIWindow;
struct ShortcutVisibility;

struct QueuedChatNotification {
  /* 0000 */ std::string text;
  /* 0018 */ std::uint32_t color;
};

struct ChatNotifications {
  UIWindow* const& chat;
  std::list<QueuedChatNotification>& queued;
  const int& restricted;
  const bool& allowsRestricted;
  const unsigned short& codePage;
  const bool& inlineCharset;
  ShortcutVisibility& shortcuts;

  void Text(const char* text, std::uint32_t color, const char* extra, int value, bool alternate);
  void Refresh();
};

// INLINED
inline void ChatNotifications::Text(const char* text, std::uint32_t color, const char* extra, int value, bool alternate) {
  if (this->restricted != 0 && !this->allowsRestricted) {
    return;
  }

  if (color == 0) {
    color = 0xFFFFFF;
  }

  if (this->chat) {
    if (std::strcmp(text, "No Msg") != 0 && std::strcmp(text, "NO MSG") != 0) {
      this->chat->SendMsg(nullptr, alternate ? 115 : 37, reinterpret_cast<std::intptr_t>(text), color, reinterpret_cast<std::intptr_t>(extra), value);
    }

    return;
  }

  std::vector<std::string> lines;
  WrapPlainText(text, lines, 94, this->codePage, this->inlineCharset);

  for (const auto& line : lines) {
    this->queued.push_back({line, color});
  }
}

// INLINED
inline void ChatNotifications::Refresh() {
  this->shortcuts.Show(1);
  if (this->chat) {
    this->chat->SendMsg(nullptr, 16, 0, 0, 0, 0);
  }
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(QueuedChatNotification) == 0x1C);
static_assert(offsetof(QueuedChatNotification, color) == 0x0018);
#endif

#endif
