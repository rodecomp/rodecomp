#ifndef RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H
#define RAGNAROK_SOURCE_UI_LISTS_UITABCONTROL_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

class UIEditCtrl;

class UITabControl : public UIWindow {
public:
  UITabControl();
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
  void AddTab(const char* label, const char* tooltip);
  void SetFlag(int index, bool value);
  __declspec(noinline) void HideEdit();
  void Configure(
    int layout, int tabHeight, int tabPadding, unsigned char uniformWidths, unsigned char skipTabFill, unsigned char allowTabRename, std::uint32_t activeFillColor,
    std::uint32_t inactiveFillColor, std::uint32_t activeBorderColor, std::uint32_t inactiveBorderColor, std::uint32_t activeTextColor,
    std::uint32_t inactiveTextColor
  );
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Paint() override;
  void DragAndDrop(int argument1, int argument2, std::intptr_t argument3) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;

  /* 007C */ int activeTabIndex;
  /* 0080 */ int layout;
  /* 0084 */ int tabHeight;
  /* 0088 */ int tabPadding;
  /* 008C */ unsigned char uniformWidths;
  /* 008D */ unsigned char padding_008D[0x3];
  /* 0090 */ std::uint32_t activeFillColor;
  /* 0094 */ std::uint32_t inactiveFillColor;
  /* 0098 */ std::uint32_t activeBorderColor;
  /* 009C */ std::uint32_t inactiveBorderColor;
  /* 00A0 */ std::uint32_t activeTextColor;
  /* 00A4 */ std::uint32_t inactiveTextColor;
  /* 00A8 */ unsigned char allowTabRename;
  /* 00A9 */ unsigned char skipTabFill;
  /* 00AC */ int dragX;
  /* 00B0 */ int dragY;
  /* 00B4 */ std::vector<std::string> labels;
  /* 00C0 */ std::vector<std::uint32_t> colors;
  /* 00CC */ std::vector<int> widths;
  /* 00D8 */ UIEditCtrl* edit;
  /* 00DC */ std::vector<std::string> tooltips;
  /* 00E8 */ int fontSize;
  /* 00EC */ std::vector<bool> flags;
};

#endif
