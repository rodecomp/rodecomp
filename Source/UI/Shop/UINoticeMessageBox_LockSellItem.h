#ifndef RAGNAROK_SOURCE_UI_SHOP_UINOTICEMESSAGEBOX_LOCKSELLITEM_H
#define RAGNAROK_SOURCE_UI_SHOP_UINOTICEMESSAGEBOX_LOCKSELLITEM_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UINoticeMessageBox_LockSellItem : public UIFrameWnd {
public:
  void Slot19() override;

  /* 00B4 */ std::uint32_t startTick;
  /* 00B8 */ std::uint32_t duration;
};

#endif
