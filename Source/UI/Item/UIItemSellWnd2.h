#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

struct WindowReplayRecorder;

class UIItemSellWnd2 : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;

protected:
  WindowReplayRecorder* m_recorder;

public:
  /* 00F4 */ WindowPlacement* saved;
  /* 00F8 */ int hovered;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemSellWnd2, saved) == 0xF4);
#endif

#endif
