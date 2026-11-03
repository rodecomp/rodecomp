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

  void Slot15(int width, int height) override;
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void InitHairControls();

  /* 00B4 */ UIBitmapEditCtrl* m_nameEdit;
  /* 00B8 */ CharacterInfo m_info;
  /* 0167 */ unsigned char unknown_0167;
  /* 0168 */ CharSlotInfo m_view;
  /* 02C4 */ UIBitmapButton* unknown_02C4;
  /* 02C8 */ UIBitmapButton* unknown_02C8;
  /* 02CC */ UIScrollBar* unknown_02CC;
  /* 02D0 */ UIScrollBar* unknown_02D0;
  /* 02D4 */ int unknown_02D4;
  /* 02D8 */ int unknown_02D8;

protected:
  void* unknown_controls;
};

#endif
