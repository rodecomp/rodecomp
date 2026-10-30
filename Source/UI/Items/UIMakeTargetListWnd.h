#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIListBoxContext;
class UIListBox;

class UIMakeTargetListWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00B4 */ UIListBox* m_list;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;
};

#endif
