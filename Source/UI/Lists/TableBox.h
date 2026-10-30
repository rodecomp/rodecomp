#ifndef RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H

#include "UI/Window/UIWindow.h"

class TableBox : public UIWindow {
public:
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UITableBox : public UIWindow {
public:
  void Paint() override;
};

#endif
