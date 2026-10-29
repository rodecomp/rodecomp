#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMREPAIRWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMREPAIRWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIItemRepairWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
