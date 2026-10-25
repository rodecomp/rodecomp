#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_TEXTINPUTBUFFER_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_TEXTINPUTBUFFER_H

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

  void SelectionRange(int& begin, int& end) const;

  void RefreshComposition(unsigned codePage);

  void CopySelection(bool password);
  void Paste(UIWindow* blockedFocus, WindowManagerWindows& windows);

  void MoveBack(unsigned codePage);

  void MoveForward(unsigned codePage);

  void Character(unsigned value, LPARAM unused, unsigned codePage);

  void KeyDown(
    unsigned key, LPARAM unused, unsigned codePage, bool password, UIWindow* blockedPasteFocus, WindowManagerWindows& windows, UIWindow* const& chatArrows
  );
};

#endif
