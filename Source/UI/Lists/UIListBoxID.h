#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXID_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXID_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <cstddef>
#include <map>
#include <string>

class UIScrollBar;

struct IdListLine {
  /* 0000 */ int id;
  /* 0004 */ std::string text;
};

class UIListBoxID : public UIWindow {
public:
  UIListBoxID();
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot35(int value) override;
  virtual void Configure(int x, int y, int width, int height, unsigned char transparent);
  virtual void Clear();
  virtual void Refresh();

  /* 007C */ unsigned char unknown_007C[0xC];
  /* 0088 */ std::map<int, IdListLine> lines;
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
  /* 00B8 */ std::uint32_t selectionColor;
};

class UIComboBoxID : public UIListBoxID {
public:
  UIComboBoxID();
  void Resize(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Refresh() override;

  /* 00C0 */ int targetId;
  /* 00C4 */ std::map<int, int> unknown_00C4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(IdListLine) == 28);
static_assert(sizeof(UIListBoxID) == 0xC0);
static_assert(offsetof(UIComboBoxID, targetId) == 0xC0);
#endif

#endif
