#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXEX_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXEX_H

#include "UI/Window/UIWindow.h"

#include <map>
#include <string>

class UIScrollBar;

struct ExtendedListLine {
  /* 0000 */ std::string text;
  /* 0018 */ std::uint32_t color = 0;
};

class UIListBoxEx : public UIWindow {
public:
  UIListBoxEx();
  ~UIListBoxEx() override;
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnWheel(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  virtual void Configure(int x, int y, int width, int height, unsigned char transparent);
  virtual void AddLine(const char* text, std::uint32_t color);
  virtual void Clear();
  virtual void Refresh();
  int FindLine(const std::string& text);

  /* 007C */ int backgroundRed;
  /* 0080 */ int backgroundGreen;
  /* 0084 */ int backgroundBlue;
  /* 0088 */ std::map<int, ExtendedListLine> lines;
  /* 0090 */ int selected;
  /* 0094 */ int topLine;
  /* 0098 */ int horizontalOffset;
  /* 009C */ int contentWidth;
  /* 00A0 */ int verticalOverflow;
  /* 00A4 */ int bottomMargin;
  /* 00A8 */ UIScrollBar* verticalBar;
  /* 00AC */ UIScrollBar* horizontalBar;
  /* 00B0 */ int transparent;
  /* 00B4 */ int lineHeight;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ std::uint32_t selectionColor;

protected:
  int LineAt(int x, int y) const;
};

// INLINED
inline int UIListBoxEx::LineAt(int x, int y) const {
  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    return -1;
  }

  const int index = (this->topLine + y / this->lineHeight);
  return index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ExtendedListLine) == 28);
static_assert(sizeof(UIListBoxEx) == 0xC0);
#endif

#endif
