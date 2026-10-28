#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H

#include "UI/Window/UIFrameWnd.h"

class UIProhibitListWnd : public UIFrameWnd {
public:
  /* 00B4 */ int m_count;

  void Slot19() override;
};

#endif
