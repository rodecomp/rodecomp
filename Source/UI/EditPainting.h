#ifndef RAGNAROK_SOURCE_UI_EDITPAINTING_H
#define RAGNAROK_SOURCE_UI_EDITPAINTING_H

#include <cstdint>
#include <list>
#include <string>

class UIWindow;
class CDC;
class FontCache;
struct TextLocale;
struct TextInputBuffer;
struct InputCharset;
struct EditViewport;
struct BitmapImage;

// Actual inline-link positioning mutates child windows and input selection.
// The concrete link owner must implement it; no default implementation exists.
class EditLinkLayout {
public:
  virtual void Update() = 0;

protected:
  ~EditLinkLayout() = default;
};

struct EditPainting {
  std::string& text;
  const int& anchor;
  const int& cursor;
  const int& mask;
  const int& firstVisible;
  const int& y;
  const int& invalidatesParent;
  const std::uint32_t& color;
  const std::uint32_t& secondColor;
  const bool& focused;
  const bool& chatInputFocused;
  const bool& caretEnabled;
  const std::uint32_t& tick;
  const int& candidateCursor; // input owner +0x80
  const int& width;
  const int& height;
  const int& screenWidth;
  const int& screenHeight;
  TextInputBuffer& input;
  const InputCharset& charset;
  EditViewport& viewport;
  CDC& surface;
  FontCache& fonts;
  const TextLocale& locale;
  const BitmapImage& colorChip;
  UIWindow* const& parent;
  UIWindow* const& candidateWindow;
  UIWindow* const& readingWindow;
  const int& absoluteX;
  const int& absoluteY;
  const std::list<UIWindow*>& linkWindows;
  EditLinkLayout& links;
  // Original UIEditCtrl4 painter shares selection/candidate drawing but uses
  // its own prefix, placeholder, and caret policy.
  bool alternate = false;
  const std::string* placeholder = nullptr;
  std::uint32_t placeholderColor = 0;
  bool placeholderEnabled = false;

  void Draw();
};

#endif
