#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBITMAPTAB_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBITMAPTAB_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

struct BitmapTabEntry {
  /* 0000 */ int height;
  /* 0004 */ std::string name;
};

class UIBitmapTab : public UIWindow {
public:
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 007C */ std::vector<BitmapTabEntry> tabs;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapTabEntry) == 0x1C);
#endif

#endif
