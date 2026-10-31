#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIILLUSTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIILLUSTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UIIllustWnd : public UIFrameWnd {
public:
  UIIllustWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool CanHitTest() override;
  void Slot15(int width, int height) override;
  void Slot18() override;
  void Paint() override;
  void Slot27(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ int m_titled;
  /* 00B8 */ int m_closable;
  /* 00BC */ std::string m_bitmapName;
  UIButtonContext* m_controls;
};

class UIMultiIllustWnd : public UIFrameWnd {
public:
  UIMultiIllustWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool CanHitTest() override;
  void Slot15(int width, int height) override;
  void Slot18() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ int m_titled;
  /* 00B8 */ int m_closable;
  /* 00BC */ std::string m_bitmapName;
  /* 00D4 */ int m_state;
  UIButtonContext* m_controls;
};

#endif
