#ifndef RAGNAROK_SOURCE_UI_EMOTION_UIEMOTIONWND_H
#define RAGNAROK_SOURCE_UI_EMOTION_UIEMOTIONWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UIEmotionWnd : public UIFrameWnd {
public:
  /* 00B4 */ int unknown_00B4[10];
  /* 00DC */ UIEditCtrl* m_edits[10];
  /* 0104 */ int* unknown_0104;
  /* 0108 */ int unknown_0108;

  void Slot25(int x, int y) override;

  void SetMacroText(const char* text, int index);
};

#endif
