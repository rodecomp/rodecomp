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
  explicit CUIVScrollBar(UIWindowContext& context);
  ~CUIVScrollBar() override;
  void Resize(int width, int height) override;
  void Show(int visible) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot35(int value) override;
  void Slot53(int resolveSkin) override;
  virtual void Slot54();

  __declspec(noinline) void SetTotal(int value);
  __declspec(noinline) void SetVisible(int value);
  __declspec(noinline) void SetPosition(int value);
  void PageUp();
  void PageDown();
  int PositionFromOffset(int offset);
  void SetHidden(int unused);
  void SetButtonImages(const std::string& up, const std::string& down);
  void SetTransparent(bool transparent);
  void SetThumbImages(const std::string& top, const std::string& middle, const std::string& bottom);
  void SetThumbImage(const std::string& name);
  void SetThumbTooltip(const std::string& text);
  void SetBackground(const std::string& name);
  void SetOnChange(std::function<void(int)> handler);

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
  /* 0080 */ CUIImageButton* upButton;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIVScrollBar, total) == 0x90);
static_assert(offsetof(CUIVScrollBar, dragging) == 0xA8);
static_assert(offsetof(CUIVScrollBar, backgroundName) == 0xAC);
static_assert(offsetof(CUIVScrollBar, onChange) == 0xC8);
#endif

inline CUIVScrollBar::CUIVScrollBar()
    : CUIControl(*static_cast<UIWindowContext*>(nullptr)), total(0), visible(0), step(0), position(0), maximum(0),
      trackLength(0), dragging(0), hidden(0), transparent(0), unknown_00C4(0) {
  this->upButton = nullptr;
  this->downButton = nullptr;
  this->thumb = nullptr;
}

#endif
