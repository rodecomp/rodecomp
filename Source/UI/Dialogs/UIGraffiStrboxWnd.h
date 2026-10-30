#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIGRAFFISTRBOXWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIGRAFFISTRBOXWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIEditContext;
class UIEditCtrl;

class UIGraffiStrboxWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;

public:
  int area[4];
  UIEditCtrl* edit;

protected:
  UIEditContext* m_editContext;
};

#endif
