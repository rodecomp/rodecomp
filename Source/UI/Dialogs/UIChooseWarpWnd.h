#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWARPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWARPWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIListBox;

class UIChooseWarpWnd : public UIFrameWnd {
public:
  UIChooseWarpWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  std::vector<unsigned char> GetReplayData() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIListBox* m_list;
  /* 00B8 */ int skillId;
  /* 00BC */ std::vector<std::string> m_choices;
};

#endif
