#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISELCHARFORUSERVERWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISELCHARFORUSERVERWND_H

#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"

#include <cstdint>
#include <string>
#include <vector>

extern int g_charViewCount;

class UISelCharForUServerWnd : public UIFrameWnd {
public:
  UISelCharForUServerWnd();
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Slot27(int x, int y) override;
  void Paint() override;

protected:
  /* 00B4 */ CharSlotInfo m_slots[18];
  /* 192C */ int m_slotEnabled[18];
  /* 1974 */ short unknown_1974[18];
  /* 1998 */ std::uint32_t m_startTime;
  /* 199C */ UIWindow* m_checkBoxes[18];
  /* 19E4 */ int unknown_19E4;
  /* 19E8 */ int unknown_19E8;
  /* 19EC */ std::vector<std::string> unknown_19EC;
  /* 19F8 */ int m_checkedCount;
  /* 19FC */ int m_columns;
};

#endif
