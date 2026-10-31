#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIEditCtrl;

class UILoginWnd : public UIFrameWnd {
public:
  UILoginWnd(UIWindowContext& windows, UIFrameContext& frames);
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
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ bool idPlaceholder;
  /* 00C1 */ bool passwordPlaceholder;
  /* 00C2 */ unsigned char unknown_00C2[0x2];
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int saveId;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ std::vector<std::string> unknown_00D8;
  /* 00E4 */ unsigned char unknown_00E4[0x4];
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ const char* unknown_00EC;
};

#endif
