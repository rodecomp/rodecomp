#ifndef RAGNAROK_SOURCE_UI_LISTS_UIVSCROLLBAR_H
#define RAGNAROK_SOURCE_UI_LISTS_UIVSCROLLBAR_H

#include "UI/Lists/UIThumb.h"
#include "UI/Window/UIControl.h"

#include <cstddef>
#include <functional>
#include <string>

class CUIVScrollBar : public CUIControl {
public:
  CUIVScrollBar();
  ~CUIVScrollBar() override;
  void Resize(int width, int height) override;
  void Show(int visible) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnWheel(int value) override;
  void LoadSkin(bool resolveSkin) override;
  virtual void UpdateThumb();
  virtual void ScrollBy(int value);

  __declspec(noinline) void SetTotal(int value);
  __declspec(noinline) void SetVisible(int value);
  __declspec(noinline) void SetPosition(int value);
  __declspec(noinline) void PageUp();
  __declspec(noinline) void PageDown();
  int PositionFromOffset(int offset);
  void SetHidden(int unused);
  __declspec(noinline) void SetButtonImages(const std::string& up, const std::string& down);
  void SetTransparent(bool transparent);
  __declspec(noinline) void SetThumbImages(const std::string& top, const std::string& middle, const std::string& bottom);
  __declspec(noinline) void SetThumbImage(const std::string& name);
  void SetThumbTooltip(const std::string& text);
  __declspec(noinline) void SetBackground(const std::string& name);
  void SetOnChange(std::function<void(int)> handler);


  /* 0080 */ CUIImageButton* upButton;
  /* 0084 */ CUIImageButton* downButton;
  /* 0088 */ CUIThumb* thumb;
  /* 008C */ CResPtr<CBitmapRes> background;
  /* 0090 */ int total;
  /* 0094 */ int visible;
  /* 0098 */ int step;
  /* 009C */ int position;
  /* 00A0 */ int maximum;
  /* 00A4 */ int trackLength;
  /* 00A8 */ unsigned char dragging;
  /* 00A9 */ unsigned char hidden;
  /* 00AA */ unsigned char transparent;
  /* 00AC */ std::string backgroundName;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::function<void(int)> onChange;
};

class CUIHScrollBar : public CUIVScrollBar {
public:
  CUIHScrollBar();
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnWheel(int value) override;
  void UpdateThumb() override;

  void SetButtonImages(const std::string& left, const std::string& right);
  void SetThumbImages(const std::string& top, const std::string& middle, const std::string& bottom);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIVScrollBar, total) == 0x90);
static_assert(offsetof(CUIVScrollBar, dragging) == 0xA8);
static_assert(offsetof(CUIVScrollBar, backgroundName) == 0xAC);
static_assert(offsetof(CUIVScrollBar, onChange) == 0xC8);
#endif

// OFFSET: 0x009BE480
// FUNCTION: fn_13948
inline CUIVScrollBar::CUIVScrollBar()
    : CUIControl(), downButton(nullptr), thumb(nullptr), total(1), visible(1), step(1), position(0), maximum(0), trackLength(1), dragging(0),
      hidden(0), transparent(0), upButton(nullptr) {
}

#endif
