#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H

#include "UI/Window/UIFrameWnd.h"

class UIItemDropCntWnd : public UIFrameWnd {
public:
  void Paint() override;
};

class UIPasswordWnd : public UIItemDropCntWnd {
public:
  void Paint() override;
};

#endif
