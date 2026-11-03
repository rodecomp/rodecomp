#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISELECTREPLAYDATAWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISELECTREPLAYDATAWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
class UIReplayListBox;
class UIStaticText;

class UISelectReplayDataWnd : public UIFrameWnd {
public:
  UISelectReplayDataWnd();

  void Slot15(int width, int height) override;
  void Slot18() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool UpdateInfo();

  /* 00B4 */ UIReplayListBox* m_list;
  /* 00B8 */ UIBitmapButton* m_serverButton;
  /* 00BC */ UIStaticText* m_texts[5];

protected:
  void* unknown_controls;
  void* unknown_listContext;
};

#endif
