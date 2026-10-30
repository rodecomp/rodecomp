#ifndef RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H
#define RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

struct UIButtonContext;
class UIEditCtrl;

class UITabControl : public UIWindow {
public:
  UITabControl(UIWindowContext& windows, UIButtonContext& controls);
  int TabCount();
  int TabAt(int x, int y);
  void FillTab(int index, int x, int y, int width, int height, std::uint32_t color);
  void DrawVerticalTab(int index);
  void DrawHorizontalTab(int index);
  void DrawTabBorder(int x, int y, int width, int height, std::uint32_t color);
  void RecalculateWidths();
  void FitToTabs();
  void SelectTab(int index);
  void RemoveTab(int index);
  void ClearTabs();
  void SetFlag(int index, bool value);
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;

  /* 0080 */ int layout;
  /* 0084 */ int tabHeight;
  /* 0088 */ int tabPadding;
  /* 008C */ unsigned char uniformWidths;
  /* 008D */ unsigned char unknown_008D[0x3];
  /* 0090 */ std::uint32_t activeFillColor;
  /* 0094 */ std::uint32_t inactiveFillColor;
  /* 0098 */ std::uint32_t activeBorderColor;
  /* 009C */ std::uint32_t inactiveBorderColor;
  /* 00A0 */ std::uint32_t activeTextColor;
  /* 00A4 */ std::uint32_t inactiveTextColor;
  /* 00A8 */ unsigned char unknown_00A8;
  /* 00A9 */ unsigned char unknown_00A9;
  /* 00AC */ int dragX;
  /* 00B0 */ int dragY;
  /* 00B4 */ std::vector<std::string> labels;
  /* 00C0 */ std::vector<std::uint32_t> colors;
  /* 00CC */ std::vector<int> widths;
  /* 00D8 */ UIEditCtrl* edit;
  /* 00DC */ std::vector<std::string> tooltips;
  /* 007C */ int unknown_007C;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ std::vector<bool> flags;

private:
  UIButtonContext* m_controls;
};

#endif
