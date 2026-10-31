#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBANK_NEWWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBANK_NEWWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
struct UIButtonContext;
struct UIEditContext;

class UIBank_NewWnd : public UIFrameWnd {
public:
  UIBank_NewWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& editContext);
  bool Slot03() override;
  void Slot15(int width, int height) override;
  long long AdjustAmount(int amount);
  long long TransactionAmount(unsigned char deposit);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool IsNumeric(std::string text);
  void AnimateLoading();

protected:
  /* 00B4 */ UIEditCtrl* m_amountEdit;
  /* 00B8 */ std::string m_status;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ UIWindow* m_loading[4];
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
};

#endif
