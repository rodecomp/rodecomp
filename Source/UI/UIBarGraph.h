#ifndef RAGNAROK_SOURCE_UI_UIBARGRAPH_H
#define RAGNAROK_SOURCE_UI_UIBARGRAPH_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <functional>

struct UIButtonContext;

class UIBarGraph : public UIWindow {
public:
  void SetValue(float value);

  /* 007C */ float m_value;
};

class UIBarGraph3 : public UIWindow {
};

class UIBarGraph4 : public UIBarGraph3 {
public:
  void Slot28(int x, int y) override;

  void SetValues(const int* current, const int* maximum);

protected:
  /* 007C */ const int* m_current;
  /* 0080 */ const int* m_maximum;
  UIButtonContext* m_controls;
};

class UIINT64BarGraph : public UIWindow {
public:
  void Slot28(int x, int y) override;

  void SetValues(std::int64_t current, std::int64_t maximum);

protected:
  /* 007C */ int unknown_007C;
  /* 0080 */ std::int64_t m_current;
  /* 0088 */ std::int64_t m_maximum;
  UIButtonContext* m_controls;
};

class UIPetInfoBarGraph : public UIWindow {
public:
  void SetValue(int value);

  /* 007C */ int unknown_007C;
};

class UIBarGraphHomun : public UIWindow {
public:
  explicit UIBarGraphHomun(UIWindowContext& context);

protected:
  /* 0080 */ std::function<std::int64_t()> m_current;
  /* 00A8 */ std::function<std::int64_t()> m_maximum;
};

class UIBarGraphPlayer : public UIWindow {
public:
  explicit UIBarGraphPlayer(UIWindowContext& context);

protected:
  /* 0080 */ std::function<std::int64_t()> m_current;
  /* 00A8 */ std::function<std::int64_t()> m_maximum;
};

#endif
