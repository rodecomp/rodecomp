#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;
class UITextViewer;

class UISkillDescribeWnd : public UIFrameWnd {
public:
  UISkillDescribeWnd();

  /* 00B4 */ UIBitmapButton* m_closeButton;
  /* 00B8 */ UITextViewer* m_viewer;
  /* 00BC */ std::string iconName;
  /* 00D4 */ std::string skillName;
  /* 00EC */ std::string descriptionText;
  /* 0104 */ int m_skillId;
  /* 0108 */ UITextViewer* m_text;
  /* 010C */ UIBitmapButton* m_delayButton;

  void OnCreate(int width, int height) override;
  void Paint() override;
  bool GetCustomTransBoxInfo(void* output) override;
  void DrawTransBox() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
