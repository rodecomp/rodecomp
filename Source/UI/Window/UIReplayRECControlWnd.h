#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYRECCONTROLWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYRECCONTROLWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIButton;
class UIStaticText;
struct InputFocus;

class UIReplayRECControlWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Slot11() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void ApplyMode();
  void UpdateRecordTime();

  /* 00B4 */ int mode;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ UIWindow* window_00BC;
  /* 00C0 */ UIWindow* window_00C0;
  /* 00C4 */ UIWindow* window_00C4;
  /* 00C8 */ UIButton* button_00C8;
  /* 00CC */ UIWindow* window_00CC;
  /* 00D0 */ UIButton* button_00D0;
  /* 00D4 */ UIWindow* tabs[5];
  /* 00E8 */ UIStaticText* window_00E8;
  /* 00EC */ UIWindow* window_00EC;
  /* 00F0 */ std::uint32_t recordStart;
  /* 00F4 */ UIWindow* options[10];
  /* 011C */ unsigned char recordOptions[10];
  /* 0126 */ unsigned char unknown_0126[2];
  /* 0128 */ UIWindow* window_0128;
  /* 012C */ UIWindow* window_012C;
  /* 0130 */ bool replayPathEnabled;
  /* 0131 */ bool replayOption119;
  /* 0132 */ unsigned char unknown_0132[2];
  /* 0134 */ UIWindow* window_0134;
  /* 0138 */ ReplayRECControlWndInfo* saved;

protected:
  InputFocus* m_focus;
};

#endif
