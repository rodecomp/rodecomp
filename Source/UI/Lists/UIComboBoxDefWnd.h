#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXDEFWND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXDEFWND_H

#include "UI/Lists/UIComboBoxEx.h"
#include "UI/Window/UIWindow.h"

class UIBitmapButton;
class UIStaticText;
struct UIButtonContext;

struct UIComboBoxHeaderContext {
  UIComboBoxContext& combo;
  UIButtonContext& buttons;
};

class UIComboBoxDefWnd : public UIWindow {
public:
  UIComboBoxDefWnd();
  UIComboBoxDefWnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls);
  ~UIComboBoxDefWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void AddOption(std::string text, std::uint32_t color);
  void Select(int index);
  void Clear();
  std::string GetSelectedText();

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

class UINewComboBoxCtrl : public UIWindow {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot26(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Clear();
  /* 007C */ UIBitmapButton* button;
  /* 0080 */ UIStaticText* label;

protected:
  UIButtonContext* m_buttons;

public:
  /* 0084 */ unsigned char borderless;
  /* 0085 */ bool dropped;
  /* 0088 */ int red;
  /* 008C */ int green;
  /* 0090 */ int blue;
  /* 0094 */ int borderRed;
  /* 0098 */ int borderGreen;
  /* 009C */ int borderBlue;
  /* 00A0 */ int selected;
  /* 00A4 */ std::vector<std::string> items;
};

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

class UIComboBox_BmpWnd : public UIComboBoxDefWnd {
public:
  void Paint() override;
  void Create(int width, int height, const char* path, const char* name);

  std::string name;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIComboBoxDefWnd) == 176);
static_assert(sizeof(UIComboBoxDef2Wnd) == 192);
#endif

#endif
