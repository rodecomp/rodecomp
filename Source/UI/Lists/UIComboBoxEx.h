#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXEX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXEX_H

#include "UI/Lists/UIListBoxEx.h"

struct UIComboBoxContext {
  UIListBoxContext& list;

  UIWindow* const& popup;
};

class UIComboBoxEx : public UIListBoxEx {
public:
  UIComboBoxEx(UIWindowContext& windows, UIComboBoxContext& combo);
  ~UIComboBoxEx() override;
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Refresh() override;

  /* 00C0 */ int targetId;

protected:
  UIComboBoxContext* m_combo;
  void DrawRows(bool centered);
};

class UIComboBoxEx2 : public UIComboBoxEx {
public:
  UIComboBoxEx2(UIWindowContext& windows, UIComboBoxContext& combo);
  ~UIComboBoxEx2() override;
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00C4 */ int alignment;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIComboBoxEx) == 208);
static_assert(sizeof(UIComboBoxEx2) == 212);
#endif

#endif
