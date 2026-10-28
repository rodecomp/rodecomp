#ifndef RAGNAROK_SOURCE_UI_UIBARGRAPH_H
#define RAGNAROK_SOURCE_UI_UIBARGRAPH_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

struct UIButtonContext;

class UIBarGraph3 : public UIWindow {
};

class UIBarGraph4 : public UIBarGraph3 {
public:
  void Slot28(int x, int y) override;

protected:
  /* 007C */ const int* m_current;
  /* 0080 */ const int* m_maximum;
  UIButtonContext* m_controls;
};

class UIINT64BarGraph : public UIWindow {
public:
  void Slot28(int x, int y) override;

protected:
  /* 007C */ int unknown_007C;
  /* 0080 */ std::int64_t m_current;
  /* 0088 */ std::int64_t m_maximum;
  UIButtonContext* m_controls;
};

#endif
