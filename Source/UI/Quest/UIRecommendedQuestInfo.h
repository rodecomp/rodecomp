#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRECOMMENDEDQUESTINFO_H

#include "UI/FrameSkin.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIControl.h"

#include <vector>

class UINpcSprWnd;
class CUIStaticText;

namespace RecommendedQuestInfo {
class CUIScrollWindow;
}

class CUIRecommendedQuestInfo : public CUIFrameWnd {
public:
  __declspec(noinline) CUIRecommendedQuestInfo();
  void Slot15(int width, int height) override;
  void Slot16() override;
  void OnFirstAction();
  void OnClose();
  virtual void Slot53();

  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ CUIStaticImage* background;
  /* 00E0 */ CUIStaticText* titleText;
  /* 00E4 */ RecommendedQuestInfo::CUIScrollWindow* scrollWindow;
  /* 00E8 */ CUIImageButton* closeButton;
};

namespace RecommendedQuestInfo {

class CUIQuestInfo : public CUIControl {
public:
  __declspec(noinline) explicit CUIQuestInfo(UIWindowContext& context);

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  /* 0084 */ CUITextViewer* textViewer;
};

class CUIScrollWindow : public CUIControl {
public:
  __declspec(noinline) explicit CUIScrollWindow(UIWindowContext& context);
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
