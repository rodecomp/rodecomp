#ifndef RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H
#define RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <map>
#include <set>

class UIDisplayCharacter : public UIWindow {
public:
  /* 007C */ const char* unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int job;
  /* 0094 */ int unknown_0094;
  /* 0098 */ int unknown_0098;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int unknown_00A0;
  /* 00A4 */ int unknown_00A4;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ int unknown_00AC;
  /* 00B0 */ int unknown_00B0;
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::set<int> m_jobs;
};

class UIShowInfoWnd : public UIWindow {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIStylingshopTabParentWnd : public UIWindow {
public:
  /* 007C */ UIDisplayCharacter* m_display;
  /* 0080 */ UIShowInfoWnd* m_showInfo;
  /* 0084 */ bool unknown_0084;
  /* 0085 */ bool unknown_0085;
  /* 0086 */ bool unknown_0086;

  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UICostumeChangeWnd : public UIStylingshopTabParentWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIStylingShopWnd : public UIFrameWnd {
public:
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ std::map<UIWindow*, UIStylingshopTabParentWnd*> m_tabs;

  void Slot19() override;
};

class UI2ndCostumeCheckBox;

class UI2ndCostumeWnd : public UIStylingshopTabParentWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 0088 */ UI2ndCostumeCheckBox* m_hideCheckBox;
  /* 008C */ UI2ndCostumeCheckBox* m_showCheckBox;
};

class UIAccessoryCusWnd : public UIStylingshopTabParentWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
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

class UISlotOfMyStyleWnd : public UIStylingshopTabParentWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
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
