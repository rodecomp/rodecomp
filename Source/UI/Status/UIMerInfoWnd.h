#ifndef RAGNAROK_SOURCE_UI_STATUS_UIMERINFOWND_H
#define RAGNAROK_SOURCE_UI_STATUS_UIMERINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

class UIBitmapButton;
class UIBarGraphPlayer;

class UIMerInfoWnd : public UIFrameWnd {
public:
  UIMerInfoWnd();

  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ int padding_00B4;
  /* 00B8 */ UIBitmapButton* button;
  /* 00BC */ UIBarGraphPlayer* hpBar;
  /* 00C0 */ UIBarGraphPlayer* spBar;
  /* 00C4 */ MerInfoWndInfo* saved;
};

#endif
