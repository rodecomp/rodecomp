#ifndef RAGNAROK_SOURCE_UI_CHAT_CHATNOTIFICATIONS_H
#define RAGNAROK_SOURCE_UI_CHAT_CHATNOTIFICATIONS_H

#include <cstdint>
#include <list>
#include <string>

class UIWindow;
struct ShortcutVisibility;

struct QueuedChatNotification {
  std::string text;
  std::uint32_t color;
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

#endif
