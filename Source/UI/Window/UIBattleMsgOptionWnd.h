#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBATTLEMSGOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBATTLEMSGOPTIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIBattleMsgOptionWnd : public UIFrameWnd {
public:
  UIBattleMsgOptionWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ unsigned char unknown_00BC[0xC];
  /* 00C8 */ std::vector<const char*> messages;
  /* 00D4 */ std::string text;
  /* 00EC */ unsigned char unknown_00EC[0x1C];
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ unsigned char unknown_0110;
  /* 0111 */ unsigned char unknown_0111[0x7];
};

#endif
