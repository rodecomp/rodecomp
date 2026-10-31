#ifndef RAGNAROK_SOURCE_UI_LISTS_UIIMGMENU_H
#define RAGNAROK_SOURCE_UI_LISTS_UIIMGMENU_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIImgMenu : public UIWindow {
public:
  int ItemAt(int x, int y);
  void Move(int x, int y) override;
  void Paint() override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0080 */ int count;
  /* 0084 */ int itemWidth;
  /* 0088 */ int itemHeight;
  /* 008C */ int horizontal;
  /* 0090 */ bool* unknown_0090;
  /* 0094 */ bool* unknown_0094;
  /* 0098 */ std::string bitmapName;
  /* 00B0 */ std::string baseName;
  /* 00C8 */ int unknown_00C8;
  /* 007C */ int hovered;
};

#endif
