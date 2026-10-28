#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLNAMECHANGEWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLNAMECHANGEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UISkillNameChangeWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x08];
  /* 00BC */ std::string m_name;

  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
