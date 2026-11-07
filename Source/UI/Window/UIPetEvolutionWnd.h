#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPETEVOLUTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPETEVOLUTIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <utility>
#include <vector>

class UITextButton;

class UIPetEvolutionWnd : public UIFrameWnd {
public:
  UIPetEvolutionWnd();
  ~UIPetEvolutionWnd() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void RebuildButtons();

  /* 00B4 */ std::vector<UITextButton*> m_materialButtons;
  /* 00C0 */ int m_petEggId;
  /* 00C4 */ int m_evolvedEggId;
  /* 00C8 */ std::vector<std::pair<int, int>> m_materials;
};

#endif
