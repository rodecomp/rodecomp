#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBANK_NEWWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBANK_NEWWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
struct UIButtonContext;
struct TextureNames;

class UIBank_NewWnd : public UIFrameWnd {
public:
  long long AdjustAmount(int amount);
  long long TransactionAmount(unsigned char deposit);
  void Paint() override;
  bool IsNumeric(std::string text);

protected:
  /* 00B4 */ UIEditCtrl* m_amountEdit;
  /* 00B8 */ std::string m_status;
  UIButtonContext* m_controls;
  TextureNames* m_textureNames;
};

#endif
