#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICNTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICNTWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
class UIStaticText;
struct UIEditContext;

class UICntWnd : public UIFrameWnd {
public:
  UICntWnd(UIWindowContext& windows, UIEditContext& editContext);
  ~UICntWnd() override;

  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ int m_event;
  /* 00B8 */ UIEditCtrl* m_edit;
  /* 00BC */ UIStaticText* m_text;
  void* unknown_controls;
  UIEditContext* m_editContext;
};

#endif
