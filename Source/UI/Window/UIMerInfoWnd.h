#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

struct UIButtonContext;
struct WindowResponseRecorder;

class UIMerInfoWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ MerInfoWndInfo* saved;

protected:
  UIButtonContext* m_controls;
  WindowResponseRecorder* m_recorder;
};

#endif
