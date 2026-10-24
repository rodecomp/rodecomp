#ifndef RAGNAROK_SOURCE_UI_CHATNOTIFICATIONS_H
#define RAGNAROK_SOURCE_UI_CHATNOTIFICATIONS_H

#include <cstdint>
#include <list>
#include <string>

class UIWindow;
struct ShortcutVisibility;

// Queued chat message records: string followed by a 32-bit color.
struct QueuedChatNotification {
  std::string text;
  std::uint32_t color;
};

// Complete kind-1/19 text and kind-3 refresh branches of UIWindowMgr's message
// dispatcher. Other dispatcher kinds belong to their corresponding owners.
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
