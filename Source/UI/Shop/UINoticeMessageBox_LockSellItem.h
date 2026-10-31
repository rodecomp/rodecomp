#ifndef RAGNAROK_SOURCE_UI_SHOP_UINOTICEMESSAGEBOX_LOCKSELLITEM_H
#define RAGNAROK_SOURCE_UI_SHOP_UINOTICEMESSAGEBOX_LOCKSELLITEM_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;

class UINoticeMessageBox_LockSellItem : public UIFrameWnd {
public:
  UINoticeMessageBox_LockSellItem(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ std::uint32_t startTick;
  /* 00B8 */ std::uint32_t duration;

protected:
  UIButtonContext* m_controls;
};

#endif
