#ifndef RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H

#include "UI/Window/UIWindow.h"

class TableBox : public UIWindow {
public:
  TableBox();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ unsigned char padding_007C[0x4];
  /* 0080 */ std::uint32_t borderColor;
};

class UITableBox : public UIWindow {
public:
  UITableBox();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int padding_007C;
  /* 0080 */ int padding_0080;
  /* 0084 */ int padding_0084;
};

#endif
