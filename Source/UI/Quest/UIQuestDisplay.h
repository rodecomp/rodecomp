#ifndef RAGNAROK_SOURCE_UI_QUEST_UIQUESTDISPLAY_H
#define RAGNAROK_SOURCE_UI_QUEST_UIQUESTDISPLAY_H

#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include <string>
#include <vector>

struct QuestDisplayInfo;

class UIQuestDisplay : public UIWindow {
public:
  UIQuestDisplay();
  bool IsTransmitMouseInput() override;
  void StoreInfo() override;
  void Paint() override;
  void OnRBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Create(int width, int height);

protected:
  /* 007C */ unsigned int unknown_007C;
  /* 0080 */ int m_selectedIndex;
  /* 0084 */ std::vector<MiniPartyInfoEntry> m_menuItems;
  /* 0090 */ int* m_visibleSetting;
};

#endif
