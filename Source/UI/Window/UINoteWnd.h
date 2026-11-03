#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINOTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINOTEWND_H

#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"

class UIStaticText_LF;

class UINoteWnd_Scroll : public UIScrollWnd {
public:
  // INLINED
  UINoteWnd_Scroll(int width, int height) : UIScrollWnd(width, height, 0), m_text(nullptr) {
    this->SetSize(width, height);
  }
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

  /* 00B0 */ UIStaticText_LF* m_text;
};

class UINoteWnd : public UIFrameWnd {
public:
  UINoteWnd();
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ UINoteWnd_Scroll* m_scroll;
};

#endif
