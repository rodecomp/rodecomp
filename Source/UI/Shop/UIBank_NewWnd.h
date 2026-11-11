#ifndef RAGNAROK_SOURCE_UI_SHOP_UIBANK_NEWWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIBANK_NEWWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UIBank_NewWnd : public UIFrameWnd {
public:
  UIBank_NewWnd();
  bool NeedsFrameRefresh() override;
  void OnCreate(int width, int height) override;
  long long AdjustAmount(int amount);
  long long TransactionAmount(unsigned char deposit);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool IsNumeric(std::string text);

protected:
  /* 00B4 */ UIEditCtrl* m_amountEdit;
  /* 00B8 */ std::string m_status;
};

#endif
