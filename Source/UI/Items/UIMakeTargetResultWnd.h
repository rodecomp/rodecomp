#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETRESULTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETRESULTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMakeTargetResultWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;

protected:
  UIButtonContext* m_controls;
};

#endif
