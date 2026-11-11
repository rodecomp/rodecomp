#ifndef RAGNAROK_SOURCE_UI_LOGIN_UILOGINWND_H
#define RAGNAROK_SOURCE_UI_LOGIN_UILOGINWND_H

#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Dialogs/UIRPImageWnd2.h"

#include <string>
#include <vector>

class UILoginWnd : public UIFrameWnd {
public:
  UILoginWnd();
  ~UILoginWnd() override;

  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnDestroy() override;
  void OnProcess() override;
  void Paint() override;
  void OnMouseMove(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SaveLogin();
  void LoadLogin();
  void OpenWebPage();

  /* 00B4 */ UIEditCtrl_Custom* idEdit;
  /* 00B8 */ UIEditCtrl_Custom* passwordEdit;
  /* 00BC */ UIWindow* backgroundWindow;
  /* 00C0 */ bool idPlaceholder;
  /* 00C1 */ bool passwordPlaceholder;
  /* 00C2 */ unsigned char padding_00C2[0x2];
  /* 00C4 */ int padding_00C4;
  /* 00C8 */ int saveId;
  /* 00CC */ int padding_00CC;
  /* 00D0 */ int padding_00D0;
  /* 00D4 */ int padding_00D4;
  /* 00D8 */ std::vector<std::string> savedAccounts;
  /* 00E4 */ int accountListOpen;
  /* 00E8 */ UIComboBoxDef2Wnd* loginTypeCombo;
  /* 00EC */ const char* loginExtra;
};

extern const char* g_webPageUrl;

#endif
