#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETLISTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

struct WindowResponseRecorder;
class UIListBox;

class UIMakeTargetListWnd : public UIFrameWnd {
public:
  UIMakeTargetListWnd(WindowResponseRecorder& recorder);
  std::vector<unsigned char> BuildReplayRecords();
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIListBox* m_list;

protected:
  void* unknown_controls;
  void* unknown_listContext;

public:
  /* 00B8 */ std::vector<int> itemIds;

protected:
  WindowResponseRecorder* m_recorder;
};

#endif
