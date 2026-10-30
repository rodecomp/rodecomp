#ifndef RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_TABLEBOX_H

#include "UI/Window/UIWindow.h"

class TableBox : public UIWindow {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
