#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H

#include "UI/Window/UIFrameWnd.h"

class UIButton;
class UIEditCtrl;
class UIStaticText;
struct UIButtonContext;
struct UIEditContext;

class UIItemDropCntWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIEditCtrl* m_edit;
  /* 00B8 */ std::uint32_t unknown_00B8;

  void Paint() override;
};

class UIPasswordWnd : public UIItemDropCntWnd {
public:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ unsigned char unknown_00C0[0x5C];
  /* 011C */ int unknown_011C;
  /* 0120 */ unsigned char unknown_0120[4];
  /* 0124 */ UIButton* unknown_0124[11];
  /* 0150 */ UIButton* unknown_0150[5];
  /* 0164 */ UIStaticText* unknown_0164;

  void Show(int visible) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void sub_90C970(int value);
  void sub_90C9B0(int);
  void sub_90DE10();
  void Slot15(int width, int height) override;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
};

extern int g_passwordWndMode;

#endif
