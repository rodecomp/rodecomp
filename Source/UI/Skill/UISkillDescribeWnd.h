#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UISkillDescribeWnd : public UIFrameWnd {
public:
  UISkillDescribeWnd();

  /* 00B4 */ unsigned char unknown_00B4[0x8];
  /* 00BC */ std::string unknown_00BC;
  /* 00D4 */ std::string unknown_00D4;
  /* 00EC */ std::string unknown_00EC;
  /* 0104 */ int m_skillId;
  /* 0108 */ UIWindow* m_text;

  void Paint() override;
  bool Slot42(void* output) override;
  void Slot43() override;
};

#endif
