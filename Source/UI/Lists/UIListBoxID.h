#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXID_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXID_H

#include "UI/Window/UIWindow.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

class UIScrollBar;

struct IdListLine {
  /* 0000 */ int id;
  /* 0004 */ std::string text;
};

class UIListBoxID : public UIWindow {
public:
  __declspec(noinline) UIListBoxID();
  ~UIListBoxID() override;
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnWheel(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual void Configure(int x, int y, int width, int height, unsigned char transparent);
  virtual void AddLine(const char* text, int id);
  virtual void Clear();
  virtual void Refresh();

  /* 007C */ unsigned char backgroundChannels[0xC];
  /* 0088 */ std::map<int, IdListLine> lines;
  /* 0090 */ int selected;
  /* 0094 */ int topLine;
  /* 0098 */ int horizontalOffset;
  /* 009C */ int contentWidth;
  /* 00A0 */ int verticalOverflow;
  /* 00A4 */ int bottomMargin;
  /* 00A8 */ UIScrollBar* verticalBar;
  /* 00AC */ UIScrollBar* horizontalBar;
  /* 00B0 */ int transparent;
  /* 00B4 */ int lineHeight;
  /* 00B8 */ unsigned char padding_00B8[0x4];
  /* 00BC */ std::uint32_t selectionColor;
};

class UIListBoxTitle : public UIListBoxID {
public:
  UIListBoxTitle();
  void Paint() override;

  /* 00C0 */ int titleColor;
};

class UIComboBoxID : public UIListBoxID {
public:
  UIComboBoxID();
  ~UIComboBoxID() override = default;
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh() override;
  void AddLine(const char* text, int id, int data);

  /* 00C0 */ int targetId;
  /* 00C4 */ std::map<int, int> lineData;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(IdListLine) == 28);
static_assert(sizeof(UIListBoxID) == 0xC0);
static_assert(offsetof(UIListBoxID, selectionColor) == 0xBC);
static_assert(offsetof(UIListBoxTitle, titleColor) == 0xC0);
static_assert(offsetof(UIComboBoxID, targetId) == 0xC0);
#endif

#endif
