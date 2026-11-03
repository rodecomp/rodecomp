#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H

#include "UI/Window/UIFrameWnd.h"

class UIButton;
class UIEditCtrl;
class UIStaticText;

class UIItemDropCntWnd : public UIFrameWnd {
public:
  UIItemDropCntWnd();

  /* 00B4 */ UIEditCtrl* m_edit;
  /* 00B8 */ std::uint32_t unknown_00B8;

  void Show(int visible) override;
  void Paint() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIPasswordWnd : public UIItemDropCntWnd {
public:
  UIPasswordWnd();

  /* 00BC */ int unknown_00BC;

  void Show(int visible) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void sub_90C9B0(int);
  void Slot15(int width, int height) override;
};

class UIStorePasswordInputWnd : public UIFrameWnd {
public:
  UIStorePasswordInputWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

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
  UISecondPasswordInputWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void ShuffleKeypad();
  int sub_90C350(unsigned int target);
  void sub_90C380();
  void sub_90C970(int value);
  void CreateKeypad(unsigned int seed);
  void sub_90DE10();

  /* 010C */ int unknown_010C;
  /* 0110 */ unsigned char m_digits[10];
  /* 011C */ int unknown_011C;
  /* 0120 */ int unknown_0120;
  /* 0124 */ UIButton* unknown_0124[11];
  /* 0150 */ UIButton* unknown_0150[5];
  /* 0164 */ UIStaticText* unknown_0164;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIPasswordWnd) == 0xC0);
static_assert(offsetof(UISecondPasswordInputWnd, unknown_0124) == 0x124);
static_assert(offsetof(UISecondPasswordInputWnd, unknown_0164) == 0x164);
static_assert(sizeof(UISecondPasswordInputWnd) == 0x168);
#endif

extern int g_passwordWndMode;

#endif
