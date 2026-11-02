#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWARPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWARPWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

struct UIButtonContext;
struct UIListBoxContext;
class UIListBox;

class UIChooseWarpWnd : public UIFrameWnd {
public:
  UIChooseWarpWnd(UIWindowContext& windows, UIButtonContext& controls, UIListBoxContext& listContext);
  void Slot15(int width, int height) override;
  void Paint() override;
  std::vector<unsigned char> Slot44() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIListBox* m_list;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ std::vector<std::string> m_choices;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;
};

#endif
