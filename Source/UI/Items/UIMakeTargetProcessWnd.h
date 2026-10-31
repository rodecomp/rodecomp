#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETPROCESSWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETPROCESSWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <vector>

struct UIButtonContext;
struct WindowResponseRecorder;

class UIMakeTargetProcessWnd : public UIFrameWnd {
public:
  UIMakeTargetProcessWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, WindowResponseRecorder& recorder);
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> BuildReplayRecords();
  ITEM_INFO GetItemAt(int x, int y);

  /* 00B4 */ int makeType;
  /* 00B8 */ ITEM_INFO materials[3];

protected:
  UIButtonContext* m_controls;
  WindowResponseRecorder* m_recorder;
};

#endif
