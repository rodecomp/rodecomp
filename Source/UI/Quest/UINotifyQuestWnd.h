#ifndef RAGNAROK_SOURCE_UI_QUEST_UINOTIFYQUESTWND_H
#define RAGNAROK_SOURCE_UI_QUEST_UINOTIFYQUESTWND_H

#include "UI/Window/UIFrameWnd.h"

class UINotifyQuestWnd : public UIFrameWnd {
public:
  UINotifyQuestWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void OnCreate(int width, int height) override;
};

#endif
