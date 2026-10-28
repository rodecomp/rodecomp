#ifndef RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H
#define RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

class UITabControl : public UIWindow {
public:
  int TabCount();
  int TabAt(int x, int y);

  /* 0080 */ int layout;
  /* 0084 */ unsigned char unknown_0084[0x30];
  /* 00B4 */ std::vector<std::string> labels;
  /* 00C0 */ unsigned char unknown_00C0[0xC];
  /* 00CC */ std::vector<int> widths;
};

#endif
