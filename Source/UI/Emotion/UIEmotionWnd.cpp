#include "UI/Emotion/UIEmotionWnd.h"

#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x008A3040
// FUNCTION: fn_11646
void UIEmotionWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008A3110
// FUNCTION: fn_11648
void UIEmotionWnd::SetMacroText(const char* text, int index) {
  if (index == -1) {
    UIWindow* hover = g_windowMgr.GetHoverWindow();
    for (int i = 0; i < 10; ++i) {
      if (hover == this->m_edits[i]) {
        index = i;
      }
    }
    if (index == -1) {
      return;
    }
  }

  this->m_edits[index]->SetText(text);
}
