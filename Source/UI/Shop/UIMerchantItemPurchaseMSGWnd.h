#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEMSGWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEMSGWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
class UIStaticText;
struct UIButtonContext;

class UIMerchantItemPurchaseMSGWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00B4 */ UIStaticText* title;
  /* 00B8 */ UIStaticText* firstLine;
  /* 00BC */ UIStaticText* thirdLine;
  /* 00C0 */ UIStaticText* secondLine;
  /* 00C4 */ UIBitmapButton* buyButton;
  /* 00C8 */ UIBitmapButton* cancelButton;

protected:
  UIButtonContext* m_controls;
};

#endif
