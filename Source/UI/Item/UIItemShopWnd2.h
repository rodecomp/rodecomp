#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSHOPWND2_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSHOPWND2_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

struct WindowReplayRecorder;

class UIItemShopWnd2 : public UIItemBaseWnd {
public:
  UIItemShopWnd2(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;

protected:
  WindowReplayRecorder* m_recorder;

public:
  /* 00F4 */ unsigned char unknown_00F4[0x4];
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ WindowPlacement* saved;
  /* 0100 */ int hovered;
  /* 0104 */ int unknown_0104;
  /* 0108 */ int unknown_0108;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemShopWnd2, saved) == 0xFC);
#endif

#endif
