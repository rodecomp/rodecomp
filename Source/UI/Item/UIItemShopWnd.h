#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

struct WindowReplayRecorder;
class UICheckBox;
class UIStaticText;

class UIItemShopWnd : public UIItemBaseWnd {
public:
  UIItemShopWnd();
  UIItemShopWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot15(int width, int height) override;

protected:
  union {
    WindowReplayRecorder* m_recorder;
    /* 00F0 */ UIStaticText* title;
  };

public:
  /* 00F4 */ UICheckBox* checkBox;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ ItemShopWndInfo* saved;
  /* 0100 */ int hovered;
  /* 0104 */ int unknown_0104;
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemShopWnd, saved) == 0xFC);
#endif

#endif
