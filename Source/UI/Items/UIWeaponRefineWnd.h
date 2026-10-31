#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIWEAPONREFINEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIWEAPONREFINEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

struct UIButtonContext;
struct WindowResponseRecorder;
struct UIListBoxContext;
class UIListBox;

class UIWeaponRefineWnd : public UIFrameWnd {
public:
  UIWeaponRefineWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIListBoxContext& listContext, WindowResponseRecorder& recorder);
  std::vector<unsigned char> BuildReplayRecords();
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIListBox* m_list;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;

public:
  /* 00B8 */ std::vector<int> itemIds;
  /* 00C4 */ std::vector<int> indices;

protected:
  WindowResponseRecorder* m_recorder;
};

#endif
