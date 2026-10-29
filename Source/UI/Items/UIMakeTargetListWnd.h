#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMakeTargetListWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
