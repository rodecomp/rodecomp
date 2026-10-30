#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowPlacement;

class UIChooseWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;

  /* 00B4 */ unsigned char unknown_00B4[0x18];
  /* 00CC */ WindowPlacement* m_placement;
};

#endif
