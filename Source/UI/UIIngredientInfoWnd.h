#ifndef RAGNAROK_SOURCE_UI_UIINGREDIENTINFOWND_H
#define RAGNAROK_SOURCE_UI_UIINGREDIENTINFOWND_H

#include "UI/Window/UIFrameWnd.h"

class UIIngredientInfoWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
};

#endif
