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
  UIItemDropCntWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 00B4 */ UIEditCtrl* m_edit;
  /* 00B8 */ std::uint32_t unknown_00B8;

  void Paint() override;
};

class UIPasswordWnd : public UIItemDropCntWnd {
public:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ unsigned char unknown_00C0[0x38];
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ int unknown_00FC;
  /* 0100 */ int unknown_0100;
  /* 0104 */ int unknown_0104;
  /* 0108 */ unsigned char unknown_0108[8];
  /* 0110 */ unsigned char m_digits[10];
  /* 011A */ unsigned char unknown_011A[2];
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
  void CreateKeypad(unsigned int seed);
  void sub_90DE10();
  void Slot15(int width, int height) override;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
};

class UIStorePasswordInputWnd : public UIFrameWnd {
public:
  UIStorePasswordInputWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 00B4 */ unsigned char unknown_00B4[0x8];
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ unsigned char unknown_00C4[0xC];
  /* 00D0 */ char unknown_00D0[9];
  /* 00D9 */ char unknown_00D9[9];
  /* 00E2 */ char unknown_00E2[9];
  /* 00EC */ int unknown_00EC;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ int unknown_00FC;
  /* 0100 */ int unknown_0100;
  /* 0104 */ int unknown_0104;
  /* 0108 */ unsigned char unknown_0108;
};

class UISecondPasswordInputWnd : public UIStorePasswordInputWnd {
public:
  UISecondPasswordInputWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 010C */ int unknown_010C;
  /* 0110 */ unsigned char unknown_0110[0xC];
  /* 011C */ int unknown_011C;
  /* 0120 */ int unknown_0120;
  /* 0124 */ UIButton* unknown_0124[10];
  /* 014C */ int unknown_014C;
  /* 0150 */ int unknown_0150;
  /* 0154 */ int unknown_0154;
  /* 0158 */ int unknown_0158;
  /* 015C */ unsigned char unknown_015C[0x8];
  /* 0164 */ int unknown_0164;
};

extern int g_passwordWndMode;

#endif
