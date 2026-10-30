#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIHOTKEYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIHOTKEYWND_H

#include "Contents/Hotkeys/HotkeyInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <map>

class UIButton;

class UIHotKeyWnd : public UIFrameWnd {
public:
  UIHotKeyWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot32(int x, int y) override;
  void UpdatePageButtons();

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ std::map<int, HotkeyInfo> hotkeys[4];
  /* 00FC */ int unknown_00FC;
  /* 0100 */ unsigned char unknown_0100[0x4];
  /* 0104 */ int unknown_0104;
  /* 0108 */ unsigned char unknown_0108[0x4];
  /* 010C */ int unknown_010C;
  /* 0110 */ int unknown_0110;
  /* 0114 */ unsigned char unknown_0114;
  /* 0115 */ unsigned char unknown_0115[0x3];
  /* 0118 */ UIButton* m_prevButton;
  /* 011C */ UIButton* m_nextButton;
};

#endif
