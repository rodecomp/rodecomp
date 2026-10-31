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

  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetMacroText(const char* text, int index);

  UIBitmapButton* AddCloseButton(const char* tooltip, int width = 0);
};

#endif
