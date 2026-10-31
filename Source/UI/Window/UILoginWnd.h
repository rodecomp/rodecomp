#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIEditCtrl;

class UILoginWnd : public UIFrameWnd {
public:
  ~UILoginWnd() override;

  void Slot11() override;
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  void Slot27(int x, int y) override;
  void SaveLogin();
  void LoadLogin();
  void OpenWebPage();

  /* 00B4 */ UIEditCtrl* idEdit;
  /* 00B8 */ UIEditCtrl* passwordEdit;
  /* 00BC */ unsigned char unknown_00BC[0x4];
  /* 00C0 */ bool idPlaceholder;
  /* 00C1 */ bool passwordPlaceholder;
  /* 00C2 */ unsigned char unknown_00C2[0x6];
  /* 00C8 */ int saveId;
  /* 00CC */ unsigned char unknown_00CC[0xC];
  /* 00D8 */ std::vector<std::string> unknown_00D8;
  /* 00E4 */ unsigned char unknown_00E4[0xC];
};

#endif
