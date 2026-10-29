#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIWEAPONREFINEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIWEAPONREFINEWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIWeaponRefineWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
