#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H

#include "UI/FrameSkin.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIControl.h"

#include <vector>

class UINpcSprWnd;

class CUIRecommendedQuestInfo : public CUIFrameWnd {
public:
  void Slot16() override;
  void OnFirstAction();
  virtual void Slot53();

  /* 00D8 */ CUITitleBar* titleBar;
};

namespace RecommendedQuestInfo {

class CUIQuestInfo : public CUIControl {
public:
  __declspec(noinline) explicit CUIQuestInfo(UIWindowContext& context) : CUIControl(context), textViewer(nullptr) {
  }

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  /* 0084 */ CUITextViewer* textViewer;
};

class CUIScrollWindow : public CUIControl {
public:
  void Slot15(int width, int height) override;
  void Slot16() override;

  /* 0084 */ CUIStaticImage* background;
  /* 0088 */ CUIStaticImage* topImage;
  /* 008C */ CUIStaticImage* bottomImage;
  /* 0090 */ std::vector<CUIQuestInfo*> questInfos;
  /* 009C */ UINpcSprWnd* npcSprite;
};

}

#endif
