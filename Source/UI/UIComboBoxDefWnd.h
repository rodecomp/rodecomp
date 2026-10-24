#ifndef RAGNAROK_SOURCE_UI_UICOMBOBOXDEFWND_H
#define RAGNAROK_SOURCE_UI_UICOMBOBOXDEFWND_H

#include "UI/UIComboBoxEx.h"
#include "UI/UIWindow.h"

class UIBitmapButton;
class UIStaticText;
struct UIButtonContext;

struct UIComboBoxHeaderContext {
  UIComboBoxContext& combo;
  UIButtonContext& buttons;
};

// RTTI UIComboBoxDefWnd, original x86 size 168. Children own list and label.
class UIComboBoxDefWnd : public UIWindow {
public:
  UIComboBoxDefWnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls);
  ~UIComboBoxDefWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot31(int x, int y) override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t a,
    std::intptr_t b,
    std::intptr_t c,
    std::intptr_t d
  ) override;
  void AddOption(std::string text, std::uint32_t color);
  void Select(int index);

  /* 007C */ UIBitmapButton* button;
  /* 0080 */ UIComboBoxEx* list;
  /* 0084 */ UIStaticText* label;
  /* 0088 */ unsigned char borderless;
  /* 0089 */ unsigned char expanded;
  /* 008C */ int red;
  /* 0090 */ int green;
  /* 0094 */ int blue;
  /* 0098 */ int borderRed;
  /* 009C */ int borderGreen;
  /* 00A0 */ int borderBlue;
  /* 00A4 */ int dropdownHeight;

protected:
  UIComboBoxHeaderContext* m_controls;
};

// RTTI UIComboBoxDef2Wnd, original x86 size 184.
class UIComboBoxDef2Wnd : public UIComboBoxDefWnd {
public:
  UIComboBoxDef2Wnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls);
  ~UIComboBoxDef2Wnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00A8 */ int skin;
  /* 00AC */ int alignment;
  /* 00B0 */ int textOffset;
  /* 00B4 */ int previousTextX;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIComboBoxDefWnd) == 176);
static_assert(sizeof(UIComboBoxDef2Wnd) == 192);
#endif

#endif
