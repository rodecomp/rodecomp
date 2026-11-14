#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowPlacement;

#include <list>

class UITagTextViewer;
class UIBitmapButton;
struct SayDialogWndInfo;

class UISayDialogWnd : public UIFrameWnd {
public:
  UISayDialogWnd();
  /* 00B4 */ UITagTextViewer* m_viewer;
  /* 00B8 */ std::list<UIBitmapButton*> m_buttons;
  /* 00C0 */ std::list<UIWindow*> padding_00C0;
  /* 00C8 */ int m_responseMessage;
  /* 00CC */ int m_responseId;
  /* 00D0 */ SayDialogWndInfo* m_placement;
  /* 00D4 */ int m_responsePending;

  void OnCreate(int width, int height) override;
  void Resize(int width, int height) override;
  void StoreInfo() override;
  void OnProcess() override;
  void Paint() override;
  std::vector<unsigned char> GetReplayData() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  __declspec(noinline) int ButtonX(UIBitmapButton* button);
};

#endif
