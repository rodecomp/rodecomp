#ifndef RAGNAROK_SOURCE_UI_CHAT_CHATSUGGESTIONS_H
#define RAGNAROK_SOURCE_UI_CHAT_CHATSUGGESTIONS_H

#include <array>
#include <cstdint>
#include <vector>

class UIWindow;
class WindowManagerWindows;
struct WindowAncestry;
class UIEditCtrl;
struct InputFocus;
struct ItemTagRetirement;

struct ChatSuggestions {
  UIWindow& chat;
  UIWindow* const& window;

  bool Visible() const;

  void Show(std::uint8_t visible);
};

struct ChatInputLayout {
  UIWindow& chat;
  const std::uint8_t& open;
  const int& inputHeight;
  std::array<UIWindow**, 6> children;

  void Apply();
};

struct ChatHeaderLayout {
  const std::uint8_t& enabled;
  const std::uint32_t& mode;
  const int& width;
  std::array<UIWindow**, 4> buttons;

  void Apply();
};

struct ChatRowLayout {
  ChatInputLayout& input;
  ChatHeaderLayout& header;
  WindowManagerWindows& windows;
  WindowAncestry* const& capture;
  UIWindow* const& popup26;
  const std::vector<UIWindow*>& rows;
  const int& selected;
  const int& headerHeight;
  const int& headerCellWidth;
  UIWindow* const& headerChild;
  UIWindow* const& suggestions;

  void Resize(int count);
};

struct ChatInputFocus {
  ChatInputLayout& layout;
  ChatRowLayout& rows;
  InputFocus& focus;
  std::uint8_t& open;
  const std::uint8_t& alternate;
  UIEditCtrl* const& primary;
  UIEditCtrl* const& secondary;
  UIWindow*& pressedRoot;
  ItemTagRetirement* const& tags;

  void Toggle();

  void DismissOutside(ChatSuggestions& suggestions, UIWindow* companion, int button, int x, int y);
};

#endif
