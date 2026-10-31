#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISELECTSERVERWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISELECTSERVERWND_H

#include "UI/Window/UIFrameWnd.h"
#include <string>

class UIListBox;

class UISelectServerWnd : public UIFrameWnd {
public:
  UISelectServerWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);

  void Slot15(int width, int height) override;
  void Slot18() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIListBox* m_serverList;
  /* 00B8 */ UIWindow* m_okButton;
  /* 00BC */ std::string m_valueName;
  /* 00D4 */ int m_selectedServer;
  /* 00D8 */ int m_event;

protected:
  UIButtonContext* m_controls;
};

#endif
