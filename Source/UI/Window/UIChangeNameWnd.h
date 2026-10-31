#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHANGENAMEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHANGENAMEWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
class UIBitmapButton;
class UIBitmapEditCtrl;

class UIChangeNameWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int IsNameWithinLimit(std::string name, int limit);

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ UIBitmapButton* m_okButton;
  /* 00B8 */ UIBitmapButton* m_cancelButton;
  /* 00BC */ UIBitmapEditCtrl* m_edit;
};

#endif
