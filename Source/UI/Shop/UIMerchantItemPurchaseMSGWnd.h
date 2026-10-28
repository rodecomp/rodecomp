#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEMSGWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEMSGWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMerchantItemPurchaseMSGWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
