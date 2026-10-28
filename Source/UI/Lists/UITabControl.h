#ifndef RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H
#define RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

struct UIButtonContext;

class UITabControl : public UIWindow {
public:
  int TabCount();
  int TabAt(int x, int y);
  void Slot28(int x, int y) override;

  /* 0080 */ int layout;
  /* 0084 */ unsigned char unknown_0084[0x30];
  /* 00B4 */ std::vector<std::string> labels;
  /* 00C0 */ unsigned char unknown_00C0[0xC];
  /* 00CC */ std::vector<int> widths;
  /* 00D8 */ unsigned char unknown_00D8[0x4];
  /* 00DC */ std::vector<std::string> tooltips;

private:
  UIButtonContext* m_controls;
};

#endif
