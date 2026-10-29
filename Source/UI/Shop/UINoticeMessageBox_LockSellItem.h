#ifndef RAGNAROK_SOURCE_UI_SHOP_UINOTICEMESSAGEBOX_LOCKSELLITEM_H
#define RAGNAROK_SOURCE_UI_SHOP_UINOTICEMESSAGEBOX_LOCKSELLITEM_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;

class UINoticeMessageBox_LockSellItem : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;

  /* 00B4 */ std::uint32_t startTick;
  /* 00B8 */ std::uint32_t duration;

protected:
  UIButtonContext* m_controls;
};

#endif
