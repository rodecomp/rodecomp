#ifndef RAGNAROK_SOURCE_UI_LISTS_UIIMGMENU_H
#define RAGNAROK_SOURCE_UI_LISTS_UIIMGMENU_H

#include "UI/Window/UIWindow.h"

class UIImgMenu : public UIWindow {
public:
  int ItemAt(int x, int y);

  /* 0080 */ int count;
  /* 0084 */ int itemWidth;
  /* 0088 */ int itemHeight;
  /* 008C */ int horizontal;
};

#endif
