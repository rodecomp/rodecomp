#ifndef RAGNAROK_SOURCE_UI_CHATSUGGESTIONS_H
#define RAGNAROK_SOURCE_UI_CHATSUGGESTIONS_H

#include <array>
#include <cstdint>
#include <vector>

class UIWindow;
class WindowManagerWindows;
struct WindowAncestry;
class UIEditCtrl;
struct InputFocus;
struct ItemTagRetirement;

// Borrowed chat window and its suggestion child at +0x13C. This does not
// replace the chat window's layout, focus-toggle or suggestion-data owners.
struct ChatSuggestions {
  UIWindow& chat;
  UIWindow* const& window;

  bool Visible() const;
  // Show forwards the full byte to the virtual, then moves the child to the
  // end of the chat child list. Hide changes visibility without reordering.
  void Show(std::uint8_t visible);
};

// Borrowed chat children in original movement order: +C0,+BC,+B8,+E0,+E4,+E8.
// Reread geometry and child pointers between virtual Move calls.
struct ChatInputLayout {
  UIWindow& chat;
  const std::uint8_t& open;
  const int& inputHeight;
  std::array<UIWindow**, 6> children;

  void Apply();
};

// Borrowed header buttons +104,+100,+128,+12C; width anchor is chat+108.
struct ChatHeaderLayout {
  const std::uint8_t& enabled;
  const std::uint32_t& mode;
  const int& width;
  std::array<UIWindow**, 4> buttons;

  void Apply();
};

// Chat +F4 row views borrow windows owned by the chat hierarchy. Popup close
// is still the manager's complete close operation, not a substitute callback.
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

  // Counts above 15 become zero; negative counts become 15. Popup close
  // precedes capture rejection. Preserve bottom anchoring and final header
  // then input layout, even when the shared chat-open byte hides the inputs.
  void Resize(int count);
};

// Borrowed chat +B4 primary / +BC alternate edits, manager byte flags and
// alternate edit's +144 item-tag owner. The tag parser remains separate.
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

  // Closing an empty alternate edit hides inputs and clears pressedRoot,
  // not input focus. A nonempty alternate edit keeps open/pressedRoot;
  // every path still drains the alternate edit's existing tag owner.
  void Toggle();
  // Outside-click suggestion check. Both rectangles use
  // inclusive endpoints, including a missing companion's zero rectangle.
  void DismissOutside(ChatSuggestions& suggestions, UIWindow* companion, int button, int x, int y);
};

#endif
