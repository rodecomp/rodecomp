#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIACCOUNTLIMITEDTOOLWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIACCOUNTLIMITEDTOOLWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIEditContext;
struct UITextViewerContext;
class UIBitmapButton;
class UIEditCtrl;
class UIStaticText;
class UITextViewer;

class UIAccountLimitedToolWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UIStaticText* m_accountIdText;
  /* 00B8 */ UITextViewer* m_viewer;
  /* 00BC */ UIEditCtrl* m_edits[15];
  /* 00F8 */ UIBitmapButton* m_buttons[3];

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
  UITextViewerContext* m_textContext;
};

#endif
