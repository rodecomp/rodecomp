#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

struct WindowReplayRecorder;
class UIStaticText;

class UIItemSellWnd : public UIItemBaseWnd {
public:
  UIItemSellWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;

protected:
  union {
    WindowReplayRecorder* m_recorder;
    /* 00F0 */ UIStaticText* total;
  };

public:
  /* 00F4 */ ItemSellWndInfo* saved;
  /* 00F8 */ int hovered;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemSellWnd, saved) == 0xF4);
#endif

#endif
