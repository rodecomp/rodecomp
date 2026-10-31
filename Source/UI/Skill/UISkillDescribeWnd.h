#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLDESCRIBEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;
class UITextViewer;

class UISkillDescribeWnd : public UIFrameWnd {
public:
  UISkillDescribeWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 00B4 */ UIBitmapButton* m_closeButton;
  /* 00B8 */ UITextViewer* m_viewer;
  /* 00BC */ std::string unknown_00BC;
  /* 00D4 */ std::string unknown_00D4;
  /* 00EC */ std::string unknown_00EC;
  /* 0104 */ int m_skillId;
  /* 0108 */ UITextViewer* m_text;
  /* 010C */ UIBitmapButton* m_delayButton;

  void Slot15(int width, int height) override;
  void Paint() override;
  bool Slot42(void* output) override;
  void Slot43() override;
};

#endif
