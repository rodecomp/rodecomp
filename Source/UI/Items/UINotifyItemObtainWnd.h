#ifndef RAGNAROK_SOURCE_UI_ITEMS_UINOTIFYITEMOBTAINWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UINOTIFYITEMOBTAINWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

class UINotifyItemObtainWnd : public UIFrameWnd {
public:
  UINotifyItemObtainWnd();

  /* 00B4 */ std::string m_itemId;
  /* 00CC */ std::string m_message;
  /* 00E4 */ std::uint32_t m_shownTick;

  bool CanHitTest() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00E8 */ int m_kind;
  /* 00F0 */ ITEM_INFO m_item;
  /* 01E8 */ bool m_identified;
};

#endif
