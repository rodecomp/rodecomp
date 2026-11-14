#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIFRAMEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIFRAMEWND_H

#include "UI/Window/FrameTextCache.h"
#include "UI/Window/UIWindow.h"

#include "UI/Window/WindowManagerWindows.h"
class UIBitmapButton;
class UICheckBox;

class UIFrameWnd : public UIWindow {
public:
  UIFrameWnd(unsigned char cached);
  ~UIFrameWnd() override;
  bool IsFrameWnd() override;
  void Move(int x, int y) override;
  void StoreMoveOrigin() override;
  void MoveFromOrigin(int dx, int dy) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void RefreshSnap() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  std::vector<unsigned char> GetReplayData() override;
  int GetWindowType() override;
  UIBitmapButton* AddCloseButton(const char* tooltip, int width);
  UIBitmapButton* AddBaseButton();
  UIBitmapButton* AddBigCloseButton(const char* tooltip, int width);
  UIBitmapButton* AddMinimizeButton(const char* tooltip, int width);
  UIBitmapButton* AddBitmapButton(int id, int x, int y, const char* name, const char* tooltip);
  UIBitmapButton* AddNumberedButton(int id, int x, int y, const char* name, const char* tooltip, bool prefix);
  UICheckBox* AddStyledCheckBox(int id, int width, int height, const char* text, int x, int y, bool checked, const char* tooltip, int fontHeight,
    std::uint32_t color, const char* checkedImage, const char* uncheckedImage
  );
  UICheckBox* AddCheckBox(int id, int width, int height, const char* text, int x, int y, int checked, const char* tooltip);
  void DrawBorder(int x, int y, int width, int height);
  void DrawSkinBox(const char* name, int x, int y, int width, int height, bool tiled);
  void FitLastTextLine(int width, unsigned lineCount, int family, int height, FrameTextLines& lines);
  void DrawTextLines(int x, int y, int family, int lineHeight, const FrameTextLines& lines);
  int DrawItemName(int x, int y, int width, const ITEM_INFO* item, int unused, int family, int height);
  int DrawWrappedItemName(int x, int y, int width, const ITEM_INFO* item, int unused, int family, int lineHeight);
  int BuildItemName(
    const ITEM_INFO* item, int* highlightColor, std::vector<char*>* breaks, char** cursor, int* remaining, char** highlight, char withSlots,
    char withOptions
  );
  void DrawItemNameText(
    const void* info, const char* text, const char* highlight, std::uint32_t color, std::uint32_t highlightColor, int width,
    const std::vector<char*>* breaks
  );
  int DrawFittedItemName(int x, int y, const ITEM_INFO* item, std::uint32_t color, int unused, int unused2, char option);
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
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIFrameWnd) == 0xB4);
#endif

#endif
