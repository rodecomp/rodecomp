#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLNAMECHANGEWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLNAMECHANGEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIEditCtrl;

class UISkillNameChangeWnd : public UIFrameWnd {
public:
  UISkillNameChangeWnd();
  /* 00B4 */ UIEditCtrl* m_edit;
  /* 00B8 */ int m_skillId;
  /* 00BC */ std::string m_name;

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
