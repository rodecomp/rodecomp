#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBITMAPBUTTON2_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBITMAPBUTTON2_H

#include "UI/Buttons/UIBitmapButton.h"

class UIBitmapButton2 : public UIBitmapButton {
public:
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
};

#endif
