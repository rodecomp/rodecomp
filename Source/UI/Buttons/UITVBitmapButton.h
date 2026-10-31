#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITVBITMAPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITVBITMAPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UITVBitmapButton : public UIBitmapButton {
  friend class UITreeView;

public:
  UITVBitmapButton();
  // INLINED
  UITVBitmapButton(UIWindowContext& windowContext, UIButtonContext& buttonContext) : UIBitmapButton(windowContext, buttonContext), m_value(0) {
  }
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

protected:
  /* 0120 */ int m_value;
};

#endif
