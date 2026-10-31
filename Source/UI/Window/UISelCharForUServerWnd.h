#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISELCHARFORUSERVERWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISELCHARFORUSERVERWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UISelCharForUServerWnd : public UIFrameWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x18E8];
  /* 199C */ UIWindow* m_checkBoxes[23];
  /* 19F8 */ int m_checkedCount;
};

#endif
