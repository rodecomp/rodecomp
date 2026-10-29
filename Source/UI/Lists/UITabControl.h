#ifndef RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H
#define RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

struct UIButtonContext;
class UIEditCtrl;

class UITabControl : public UIWindow {
public:
  int TabCount();
  int TabAt(int x, int y);
  void FillTab(int index, int x, int y, int width, int height, std::uint32_t color);
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;

  /* 0080 */ int layout;
  /* 0084 */ unsigned char unknown_0084[0x25];
  /* 00A9 */ unsigned char unknown_00A9;
  /* 00AC */ int dragX;
  /* 00B0 */ int dragY;
  /* 00B4 */ std::vector<std::string> labels;
  /* 00C0 */ std::vector<std::uint32_t> colors;
  /* 00CC */ std::vector<int> widths;
  /* 00D8 */ UIEditCtrl* edit;
  /* 00DC */ std::vector<std::string> tooltips;
  /* 007C */ int unknown_007C;

private:
  UIButtonContext* m_controls;
};

#endif
