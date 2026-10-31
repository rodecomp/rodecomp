#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIFRAMEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIFRAMEWND_H

#include "UI/Window/FrameTextCache.h"
#include "UI/Window/UIWindow.h"

struct WindowSnapping;
class WindowManagerWindows;
class UIBitmapButton;
class UICheckBox;
struct UIButtonContext;

struct UIFrameContext {
  WindowSnapping& snapping;
  WindowManagerWindows& windows;
  const int& dragLimitX;
  const int& dragLimitY;
};

class UIFrameWnd : public UIWindow {
public:
  UIFrameWnd(UIWindowContext& windowContext, UIFrameContext& frameContext, unsigned char cached);
  ~UIFrameWnd() override;
  bool Slot02() override;
  void Move(int x, int y) override;
  void Slot12() override;
  void Slot13(int dx, int dy) override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot36() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  std::vector<std::uint32_t> Slot44() override;
  int Slot46() override;
  UIBitmapButton* AddCloseButton(UIButtonContext& controls, const char* tooltip, int width);
  UIBitmapButton* AddBaseButton(UIButtonContext& controls);
  UIBitmapButton* AddBigCloseButton(UIButtonContext& controls, const char* tooltip, int width);
  UIBitmapButton* AddMinimizeButton(UIButtonContext& controls, const char* tooltip, int width);
  UIBitmapButton* AddBitmapButton(UIButtonContext& controls, int id, int x, int y, const char* name, const char* tooltip);
  UIBitmapButton* AddNumberedButton(UIButtonContext& controls, int id, int x, int y, const char* name, const char* tooltip, bool prefix);
  UICheckBox* AddStyledCheckBox(
    UIButtonContext& controls, int id, int width, int height, const char* text, int x, int y, bool checked, const char* tooltip, int fontHeight,
    std::uint32_t color, const char* checkedImage, const char* uncheckedImage
  );
  UICheckBox* AddCheckBox(UIButtonContext& controls, int id, int width, int height, const char* text, int x, int y, int checked, const char* tooltip);
  void DrawBorder(int x, int y, int width, int height);
  void FitLastTextLine(int width, unsigned lineCount, int family, int height, FrameTextLines& lines);
  void DrawTextLines(int x, int y, int family, int lineHeight, const FrameTextLines& lines);
  void SplitHighlightText(
    const char* text, const char* highlight, const int& highlightColor, const int& color, const std::vector<char*>& breaks,
    std::vector<std::pair<int, std::string>>& segments
  );

protected:
  /* 007C */ int m_dragX;
  /* 0080 */ int m_dragY;
  /* 0084 */ int m_originX;
  /* 0088 */ int m_originY;
  /* 008C */ int m_message0;
  /* 0090 */ int m_message1;
  /* 0094 */ FrameTextCache m_textCache;
  UIFrameContext* m_frameContext;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIFrameWnd) == 188);
#endif

#endif
