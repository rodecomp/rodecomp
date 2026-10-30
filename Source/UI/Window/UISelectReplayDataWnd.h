#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISELECTREPLAYDATAWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISELECTREPLAYDATAWND_H

#include "UI/Window/UIFrameWnd.h"

class UISelectReplayDataWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
