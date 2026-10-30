#ifndef RAGNAROK_SOURCE_UI_QUEST_UIQUESTDISPLAY_H
#define RAGNAROK_SOURCE_UI_QUEST_UIQUESTDISPLAY_H

#include "UI/Window/UIWindow.h"
#include <string>
#include <vector>

struct QuestDisplayInfo;

struct QuestDisplayMenuItem {
  std::string text;
  int command;
};

class UIQuestDisplay : public UIWindow {
public:
  bool Slot08() override;
  void Slot11() override;
  void Slot33(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Create(int width, int height);

protected:
  /* 0080 */ int m_selectedIndex;
  /* 0084 */ std::vector<QuestDisplayMenuItem> m_menuItems;
  /* 0090 */ int* m_visibleSetting;
};

#endif
