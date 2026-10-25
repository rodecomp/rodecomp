#include "UI/Chat/ChatNotifications.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/UIWindow.h"

#include <cstring>
#include <vector>

// OFFSET: 0x00AA9100
// FUNCTION: fn_17253

void ChatNotifications::Text(const char* text, std::uint32_t color, const char* extra, int value, bool alternate) {
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

// OFFSET: 0x00AA9100
// FUNCTION: fn_17253

void ChatNotifications::Refresh() {
  this->shortcuts.Show(1);
  if (this->chat) {
    this->chat->SendMsg(nullptr, 16, 0, 0, 0, 0);
  }
}
