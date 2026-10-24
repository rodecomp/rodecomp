#ifndef RAGNAROK_SOURCE_CLIENT_TEXTINPUTBUFFER_H
#define RAGNAROK_SOURCE_CLIENT_TEXTINPUTBUFFER_H

#include <string>
#include <windows.h>

class UIWindow;
class WindowManagerWindows;

class TextInputSuggestions {
public:
  virtual void Update(const char* text) = 0;

protected:
  ~TextInputSuggestions() = default;
};

// Borrowed fields of the original input owner, not a replacement IME singleton.
struct TextInputBuffer {
  std::string& text;
  int& cursor;
  int& anchor;
  std::string& composition;
  std::string& candidate;
  int& compositionState;
  HWND& window;
  const bool& suggestionsEnabled;
  UIWindow* const& focused;
  UIWindow* const& chatInput;
  const bool& chatOpen;
  TextInputSuggestions& suggestions;

  void CancelComposition();
  void DeleteSelection();
  void Insert(const char* value);
  void Select(int anchor, int cursor);
  void CopyDisplayText(std::string& output) const;
  void Selection(int& anchor, int& cursor) const;
  // Sorted selection endpoints, preserving sequential output writes/aliasing.
  void SelectionRange(int& begin, int& end) const;
  // Reads GCS_COMPSTR through the native IME, converts with the caller's language
  // codepage and publishes candidate.
  // Suggestions concatenate committed text + candidate, truncated to 255 bytes;
  // missing IME context clears candidate without publishing suggestions.
  void RefreshComposition(unsigned codePage);
  // Copies CF_TEXT with original GlobalAlloc/clipboard ownership. Password
  // mode masks through text end (not selection end), matching the reference;
  // callers must retain its original selection/storage preconditions.
  void CopySelection(bool password);
  void Paste(UIWindow* blockedFocus, WindowManagerWindows& windows);
  // Only cursor moves; anchor/selection remains unchanged. Negative cursor
  // becomes zero; positive cursor uses CharPrevExA with the low codepage WORD.
  void MoveBack(unsigned codePage);
  // Unsigned cursor comparison: out-of-range (including negative) clamps to
  // text length; in-range advances via CharNextExA. Anchor is unchanged.
  void MoveForward(unsigned codePage);
  // Original ANSI character payload is a NUL-terminated byte sequence in the
  // low DWORD (not Unicode). Control bytes <=31 are ignored except backspace.
  void Character(unsigned value, LPARAM unused, unsigned codePage);
  // Shift wins over Control. The borrowed chat-arrow child is the actual
  // Chat owner child; visible children receive message 176 instead of movement.
  void KeyDown(
    unsigned key,
    LPARAM unused,
    unsigned codePage,
    bool password,
    UIWindow* blockedPasteFocus,
    WindowManagerWindows& windows,
    UIWindow* const& chatArrows
  );
};

#endif
