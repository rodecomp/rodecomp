#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_UIQUICKSLOTWND_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_UIQUICKSLOTWND_H

#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>

struct ITEM_INFO;
class AutomaticSkills;

#pragma pack(push, 1)
struct QuickSlotEntry {
  /* 0000 */ unsigned char skill;
  /* 0001 */ std::uint32_t id;
  /* 0005 */ std::int16_t count;
};
#pragma pack(pop)

struct QuickSlotWndInfo;

class UIQuickSlotWnd : public UIFrameWnd {
public:
  UIQuickSlotWnd();
  void DragAndDrop(int x, int y, std::intptr_t argument) override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  int SlotX(int slot);
  int SlotAt(int x, int y);

protected:
  /* 00B4 */ int m_selected;
  /* 00B8 */ QuickSlotWndInfo* m_saved;
  /* 00BC */ QuickSlotEntry m_entries[2];
};

static_assert(sizeof(QuickSlotEntry) == 7);

#endif
