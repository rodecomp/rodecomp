#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIDISCONNECTEDSERVERMSGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIDISCONNECTEDSERVERMSGWND_H

#include "UI/Window/UIFrameWnd.h"

struct UITextViewerContext;
class UITextViewer;

class UIDisconnectedServerMsgWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UITextViewer* m_viewer;

protected:
  UITextViewerContext* m_textContext;
};

#endif
