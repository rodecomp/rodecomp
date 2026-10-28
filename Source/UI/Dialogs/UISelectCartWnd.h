#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UISelectCartWnd : public UIFrameWnd {
public:
  void Paint() override;
  void Slot25(int x, int y) override;

protected:
  UIButtonContext* m_controls;
};

#endif
