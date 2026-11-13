#ifndef RAGNAROK_SOURCE_UI_LOGIN_UIPASSWORDWND_H
#define RAGNAROK_SOURCE_UI_LOGIN_UIPASSWORDWND_H

#include "UI/Window/UIFrameWnd.h"

enum PasswordWndMode : int {
  PASSWORDWND_MODE_OK = 0,
  PASSWORDWND_MODE_PASSED = 1,
  PASSWORDWND_MODE_ASK = 2,
  PASSWORDWND_MODE_EXPIRED = 3,
  PASSWORDWND_MODE_NOTSET = 4,
  PASSWORDWND_MODE_NEW = 5,
};

class UIBitmapButton;
class UIEditCtrl;
class UIStaticText;

class UIItemDropCntWnd : public UIFrameWnd {
public:
  UIItemDropCntWnd();

  /* 00B4 */ UIEditCtrl* m_edit;
  /* 00B8 */ UIStaticText* itemTitle;

  void Show(int visible) override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIPasswordWnd : public UIItemDropCntWnd {
public:
  UIPasswordWnd();

  /* 00BC */ int roomId;

  void Show(int visible) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void sub_90C9B0(int);
  void OnCreate(int width, int height) override;
};

class UIStorePasswordInputWnd : public UIFrameWnd {
public:
  UIStorePasswordInputWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ unsigned char padding_00B4[0x8];
  /* 00BC */ int padding_00BC;
  /* 00C0 */ int padding_00C0;
  /* 00C4 */ unsigned char padding_00C4[0xC];
  /* 00D0 */ char currentPassword[9];
  /* 00D9 */ char newPassword[9];
  /* 00E2 */ char confirmPassword[9];
  /* 00EC */ int inputDigitCount;
  /* 00F0 */ int maskTextX;
  /* 00F4 */ int passwordStage;
  /* 00F8 */ int activeInputY;
  /* 00FC */ int currentPasswordY;
  /* 0100 */ int newPasswordY;
  /* 0104 */ int confirmPasswordY;
  /* 0108 */ unsigned char isChangingPassword;
};

class UISecondPasswordInputWnd : public UIStorePasswordInputWnd {
public:
  UISecondPasswordInputWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void ShuffleKeypad();
  int FindKeypadDigit(unsigned int target);
  void DisableButtons();
  void SetInputMode(int value);
  void CreateKeypad(unsigned int seed);
  void EnableKeypad();

  /* 010C */ int passwordLength;
  /* 0110 */ unsigned char m_digits[10];
  /* 011C */ int inputMode;
  /* 0120 */ int padding_0120;
  /* 0124 */ UIBitmapButton* keypadButtons[11];
  /* 0150 */ UIBitmapButton* actionButtons[5];
  /* 0164 */ UIStaticText* instructionText;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIPasswordWnd) == 0xC0);
static_assert(offsetof(UISecondPasswordInputWnd, keypadButtons) == 0x124);
static_assert(offsetof(UISecondPasswordInputWnd, instructionText) == 0x164);
static_assert(sizeof(UISecondPasswordInputWnd) == 0x168);
#endif


#endif
