#ifndef RAGNAROK_SOURCE_UI_NAVIGATION_UIPRIVATEAIRPLANEWND_H
#define RAGNAROK_SOURCE_UI_NAVIGATION_UIPRIVATEAIRPLANEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIButtonContext;
class UIBasicButton;

class UIPrivateAirplaneWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;

protected:
  /* 00B4 */ UIButtonContext* m_controls;
  /* 00B8 */ std::string m_text;
  /* 00D0 */ std::vector<UIBasicButton*> m_buttons;
};

#endif
