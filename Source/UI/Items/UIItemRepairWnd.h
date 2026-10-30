#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMREPAIRWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMREPAIRWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <vector>

struct UIButtonContext;
struct WindowResponseRecorder;
struct UIListBoxContext;
class UIListBox;

#pragma pack(push, 1)
struct UIItemRepairEntry {
  /* 0000 */ std::int16_t index;
  /* 0002 */ std::int32_t itemId;
  /* 0006 */ unsigned char unknown_0006[0x12];
};
#pragma pack(pop)

static_assert(sizeof(UIItemRepairEntry) == 0x18);

class UIItemRepairWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;

public:
  /* 00B4 */ UIListBox* m_list;
  /* 00B8 */ std::vector<int> itemIds;
  /* 00C4 */ std::vector<int> indices;
  /* 00D0 */ std::list<UIItemRepairEntry> entries;

protected:
  WindowResponseRecorder* m_recorder;
};

#endif
