#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H

#include "UI/Window/UIControl.h"

namespace RecommendedQuestInfo {

class CUIQuestInfo : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
};

}

#endif
