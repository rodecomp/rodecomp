#ifndef RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H
#define RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <map>

class UIDisplayCharacter : public UIWindow {
public:
  /* 007C */ const char* unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int job;
};

class UIStylingshopTabParentWnd : public UIWindow {
public:
  /* 007C */ UIDisplayCharacter* m_display;
  /* 0080 */ int unknown_0080;
  /* 0084 */ bool unknown_0084;
  /* 0085 */ bool unknown_0085;
  /* 0086 */ bool unknown_0086;

  void Slot15(int width, int height) override;
  void Paint() override;
};

class UICostumeChangeWnd : public UIStylingshopTabParentWnd {
public:
  void Paint() override;
};

class UIStylingShopWnd : public UIFrameWnd {
public:
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ std::map<UIWindow*, UIStylingshopTabParentWnd*> m_tabs;

  void Slot19() override;
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
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UIAccBitmapButton : public UIBitmapButton {
public:
  void Slot33(int x, int y) override;
};

#endif
