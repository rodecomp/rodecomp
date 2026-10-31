#ifndef RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H

#include "UI/Window/UIWindow.h"

class TableBox : public UIWindow {
public:
  explicit TableBox(UIWindowContext& windows);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ unsigned char unknown_007C[0x4];
  /* 0080 */ std::uint32_t borderColor;
};

class UITableBox : public UIWindow {
public:
  explicit UITableBox(UIWindowContext& windows);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int unknown_0084;
};

#endif
