#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPETEVOLUTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPETEVOLUTIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <utility>
#include <vector>

class UITextButton;
struct UIButtonContext;

class UIPetEvolutionWnd : public UIFrameWnd {
public:
  UIPetEvolutionWnd(UIWindowContext& windows, UIFrameContext& frames);
  ~UIPetEvolutionWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00B4 */ std::vector<UITextButton*> m_materialButtons;
  /* 00C0 */ int m_petEggId;
  /* 00C4 */ int m_evolvedEggId;
  /* 00C8 */ std::vector<std::pair<int, int>> m_materials;

protected:
  UIButtonContext* m_controls;
};

#endif
