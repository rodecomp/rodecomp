#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

class UIBarGraph4;
class UIBarGraphHomun;
class UIBitmapButton;
class UICheckBox;
class UIEditCtrl2;
class UIINT64BarGraph;
class UINormalMB;
struct ForbiddenWords;
struct WindowResponseRecorder;
struct UIButtonContext;
struct UIEditContext;

class UIHomunInfoWnd : public UIFrameWnd {
public:
  UIHomunInfoWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIEditCtrl2* nameEdit;
  /* 00B8 */ UIBitmapButton* button;
  /* 00BC */ UIBarGraphHomun* hpBar;
  /* 00C0 */ UIBarGraphHomun* spBar;
  /* 00C4 */ UIINT64BarGraph* expBar;
  /* 00C8 */ UIBarGraph4* intimacyBar;
  /* 00CC */ HomunInfoWndInfo* saved;
  /* 00D0 */ UINormalMB* unknown_00D0;
  /* 00D4 */ UICheckBox* unknown_00D4;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
  WindowResponseRecorder* m_recorder;
  ForbiddenWords* m_forbiddenWords;
};

#endif
