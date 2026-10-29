#ifndef RAGNAROK_SOURCE_UI_QUEST_UIQUESTDISPLAY_H
#define RAGNAROK_SOURCE_UI_QUEST_UIQUESTDISPLAY_H

#include "UI/Window/UIWindow.h"

class UIQuestDisplay : public UIWindow {
public:
  void Slot11() override;

protected:
  /* 007C */ unsigned char unknown_007C[0x14];
  /* 0090 */ int* m_visibleSetting;
};

#endif
