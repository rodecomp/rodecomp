#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISERACHSTOREINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISERACHSTOREINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UISerachStoreInfoWnd : public UIFrameWnd {
public:
  UISerachStoreInfoWnd();

protected:
  /* 00B4 */ UIWindow* unknown_00B4[10];
  /* 00DC */ int unknown_00DC;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ unsigned char unknown_00EC;
  /* 00ED */ unsigned char unknown_00ED;
  /* 00F0 */ std::string unknown_00F0;
};

#endif
