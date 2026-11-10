#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMAKECHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMAKECHARWND_H

#include "Client/Mode/CLoginMode.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"

class UIBitmapButton;
class UIBitmapEditCtrl;
class UIScrollBar;

class UIMakeCharWnd : public UIFrameWnd {
public:
  UIMakeCharWnd();

  void OnCreate(int width, int height) override;
  void OnDestroy() override;
  void OnProcess() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void InitHairControls();

  /* 00B4 */ UIBitmapEditCtrl* m_nameEdit;
  /* 00B8 */ CharacterInfo m_info;
  /* 0167 */ unsigned char padding_0167;
  /* 0168 */ CharSlotInfo m_view;
  /* 02C4 */ UIBitmapButton* m_rotateLeftButton;
  /* 02C8 */ UIBitmapButton* m_rotateRightButton;
  /* 02CC */ UIScrollBar* m_hairStyleBar;
  /* 02D0 */ UIScrollBar* m_hairColorBar;
  /* 02D4 */ int m_hairStyle;
  /* 02D8 */ int m_hairColor;
};

#endif
