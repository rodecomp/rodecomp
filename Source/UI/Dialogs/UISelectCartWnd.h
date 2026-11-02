#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UISelectCartWnd : public UIFrameWnd {
public:
  UISelectCartWnd(UIWindowContext& windows);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
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
