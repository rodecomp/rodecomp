#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

struct WindowReplayRecorder;

class UIItemSellWnd2 : public UIItemBaseWnd {
public:
  UIItemSellWnd2(UIWindowContext& windows, UIFrameContext& frames);
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  WindowReplayRecorder* m_recorder;

public:
  /* 00F4 */ ItemSellWndInfo* saved;
  /* 00F8 */ int hovered;
  /* 00FC */ int unknown_00FC;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemSellWnd2, saved) == 0xF4);
#endif

#endif
