#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBITMAPBUTTON2_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBITMAPBUTTON2_H

#include "UI/Buttons/UIBitmapButton.h"

class UIBitmapButton2 : public UIBitmapButton {
public:
  void DragAndDrop(int argument1, int argument2, std::intptr_t argument3) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
};

#endif
