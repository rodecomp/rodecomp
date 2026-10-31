#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UISelectCartWnd : public UIFrameWnd {
public:
  UISelectCartWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  void Slot25(int x, int y) override;
  void AddCart(int index, int type);

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
};

#endif
