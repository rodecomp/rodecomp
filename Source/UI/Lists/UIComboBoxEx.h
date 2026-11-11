#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXEX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXEX_H

#include "Resource/Bitmap/TextureNames.h"
#include "UI/Lists/UIListBoxEx.h"

class UIComboBoxEx : public UIListBoxEx {
public:
  __declspec(noinline) UIComboBoxEx();
  ~UIComboBoxEx() override;
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Refresh() override;

  /* 00C0 */ int targetId;

protected:
  void DrawRows(bool centered);
};

class UIComboBoxEx2 : public UIComboBoxEx {
public:
  UIComboBoxEx2();
  ~UIComboBoxEx2() override;
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;

  /* 00C4 */ int alignment;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIComboBoxEx) == 0xC4);
static_assert(sizeof(UIComboBoxEx2) == 0xC8);
#endif

#endif
