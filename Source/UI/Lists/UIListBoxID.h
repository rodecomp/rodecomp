#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXID_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXID_H

#include "UI/Window/UIWindow.h"

#include <map>
#include <string>

class UIScrollBar;

struct IdListLine {
  /* 0000 */ int id;
  /* 0004 */ std::string text;
};

class UIListBoxID : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot35(int value) override;

  /* 007C */ unsigned char unknown_007C[0xC];
  /* 0088 */ std::map<int, IdListLine> lines;
  /* 0090 */ int selected;
  /* 0094 */ int topLine;
  /* 0098 */ unsigned char unknown_0098[0x8];
  /* 00A0 */ int verticalOverflow;
  /* 00A4 */ int unknown_00A4;
  /* 00A8 */ UIScrollBar* verticalBar;
  /* 00AC */ unsigned char unknown_00AC[0x8];
  /* 00B4 */ int lineHeight;
};

class UIComboBoxID : public UIListBoxID {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(IdListLine) == 28);
#endif

#endif
