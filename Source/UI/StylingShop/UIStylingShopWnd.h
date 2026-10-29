#ifndef RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H
#define RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/UIScrollWnd.h"
#include "UI/Window/UIWindow.h"

class UIStylingshopTabParentWnd : public UIWindow {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
};

class UI2ndCostumeWnd : public UIWindow {
public:
  void Paint() override;
};

class UIAccessoryCusWnd : public UIWindow {
public:
  void Paint() override;
};

class UIAccessoryShowCase : public UIScrollWnd {
public:
  void Paint() override;
};

class UIBasicStylingWnd : public UIWindow {
public:
  void Paint() override;
};

class UIMyStyleWnd : public UIWindow {
public:
  void Paint() override;
};

class UISlotOfMyStyleWnd : public UIWindow {
public:
  void Paint() override;
};

class UISlotForSelectWnd : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
};

class UI2ndCostumeCheckBox : public UICheckBox {
public:
  void Slot25(int x, int y) override;
};

class UIStylingShopButton : public UIBitmapButton {
public:
  void Slot27(int x, int y) override;
};

class UIAccBitmapButton : public UIBitmapButton {
public:
  void Slot33(int x, int y) override;
};

#endif
