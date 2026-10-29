#ifndef RAGNAROK_SOURCE_UI_TEXT_UIACHWND_H
#define RAGNAROK_SOURCE_UI_TEXT_UIACHWND_H

#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindow.h"

class UIAchListWnd : public UIWindow {
public:
  void Slot31(int x, int y) override;
};

class UIAchTab : public UIWindow {
public:
  void Slot31(int x, int y) override;
};

class UIAchTV : public UITextViewer {
public:
  /* 00EC */ unsigned char unknown_00EC;

  void Slot25(int x, int y) override;
};

#endif
