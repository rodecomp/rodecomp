#ifndef RAGNAROK_SOURCE_UI_EMOTION_UIEMOTIONWND_H
#define RAGNAROK_SOURCE_UI_EMOTION_UIEMOTIONWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
class UIStaticText;
class UIBitmapButton;
struct EmotionWndInfo;

class UIEmotionWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIStaticText* m_labels[10];
  /* 00DC */ UIEditCtrl* m_edits[10];
  /* 0104 */ EmotionWndInfo* m_saved;
  /* 0108 */ UIBitmapButton* viewButton;

  // INLINED
  UIEmotionWnd() : UIFrameWnd(0), m_labels(), m_edits(), m_saved(nullptr), viewButton(nullptr) {
  }

  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetMacroText(const char* text, int index);

  UIBitmapButton* AddCloseButton(const char* tooltip, int width = 0);
};

#endif
