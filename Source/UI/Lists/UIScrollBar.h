#ifndef RAGNAROK_SOURCE_UI_LISTS_UISCROLLBAR_H
#define RAGNAROK_SOURCE_UI_LISTS_UISCROLLBAR_H

#include "UI/Window/UIWindow.h"

class UIScrollBar : public UIWindow {
public:
  explicit UIScrollBar(UIWindowContext& windows);
  ~UIScrollBar() override;
  void Resize(int width, int height) override;
  bool Slot08() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int value) override;
  virtual int ThumbLength();
  int ThumbOffset();
  int Position();
  int Travel();
  int Part(int x, int y);
  void Configure(int x, int y, unsigned char vertical, int length);
  void SetPosition(int value);
  void SetRange(int maximum, int visible, int total);

  /* 007C */ int state;
  /* 0080 */ int maximum;
  /* 0084 */ int position;
  /* 0088 */ int vertical;
  /* 008C */ int pressX;
  /* 0090 */ int pressY;
  /* 0094 */ int dragAmount;
  /* 0098 */ int total;
  /* 009C */ int visible;
  /* 00A0 */ int arrowLength;
  /* 00A4 */ int thickness;
  /* 00A8 */ int minimumThumb;
  /* 00AC */ int horizontalStep;
  /* 00B0 */ int verticalStep;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIScrollBar) == sizeof(UIWindow) + 0x38);
#endif

#endif
