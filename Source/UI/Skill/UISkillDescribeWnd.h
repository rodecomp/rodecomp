#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H

#include "UI/Window/UIFrameWnd.h"

class UISkillDescribeWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x54];
  /* 0108 */ UIWindow* m_text;

  void Paint() override;
  bool Slot42(void* output) override;
};

#endif
