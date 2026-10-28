#ifndef RAGNAROK_SOURCE_UI_UISTATICITEM_H
#define RAGNAROK_SOURCE_UI_UISTATICITEM_H

#include "UI/Window/UIWindow.h"

class UIStaticItem : public UIWindow {
};

class UIStaticItem_Illust : public UIStaticItem {
public:
  void Slot28(int x, int y) override;
};

#endif
