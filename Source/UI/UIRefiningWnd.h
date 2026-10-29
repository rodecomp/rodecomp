#ifndef RAGNAROK_SOURCE_UI_UIREFININGWND_H
#define RAGNAROK_SOURCE_UI_UIREFININGWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIRefiningWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  int SlotAt(int x, int y);
};

#endif
