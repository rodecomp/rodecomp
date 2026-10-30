#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H

#include "UI/FrameSkin.h"
#include "UI/Window/UIControl.h"

class CUIRecommendedQuestInfo : public CUIFrameWnd {
public:
  void OnFirstAction();
  virtual void Slot53();
};

namespace RecommendedQuestInfo {

class CUIQuestInfo : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
};

}

#endif
