#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H

#include "UI/Item/UIItemBaseWnd.h"

struct WindowReplayRecorder;

class UIItemSellWnd2 : public UIItemBaseWnd {
public:
  void Slot26(int x, int y) override;

protected:
  WindowReplayRecorder* m_recorder;
};

#endif
