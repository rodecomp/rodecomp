#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct SkillListWndInfo;

class UINewSkillListWnd : public UIFrameWnd {
public:
  void Slot11() override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x198];
  /* 024C */ SkillListWndInfo* m_saved;
};

#endif
