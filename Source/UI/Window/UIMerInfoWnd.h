#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

struct UIButtonContext;
class UIBitmapButton;
class UIBarGraphPlayer;
struct WindowResponseRecorder;

class UIMerInfoWnd : public UIFrameWnd {
public:
  UIMerInfoWnd(UIWindowContext& windows, UIFrameContext& frames);

  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ UIBitmapButton* button;
  /* 00BC */ UIBarGraphPlayer* hpBar;
  /* 00C0 */ UIBarGraphPlayer* spBar;
  /* 00C4 */ MerInfoWndInfo* saved;

protected:
  UIButtonContext* m_controls;
  WindowResponseRecorder* m_recorder;
};

#endif
