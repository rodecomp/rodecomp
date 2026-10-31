#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIWAITWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIWAITWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
class UITextViewer;

class UIWaitWnd : public UIFrameWnd {
public:
  /* 00B4 */ UITextViewer* m_textViewer;
  /* 00B8 */ UIBitmapButton* m_button;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetMessage(const char* text, int, int);
};

#endif
