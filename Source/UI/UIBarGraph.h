#ifndef RAGNAROK_SOURCE_UI_UIBARGRAPH_H
#define RAGNAROK_SOURCE_UI_UIBARGRAPH_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <functional>


class UIBarGraph : public UIWindow {
public:
  explicit UIBarGraph() : UIWindow(0), m_value(0) {
  }

  void Paint() override;

  void SetValue(float value);

  /* 007C */ float m_value;
};

class UIBarGraph2 : public UIBarGraph {
public:
  UIBarGraph2();

  void Paint() override;

  /* 0080 */ int m_vertical;
  /* 0084 */ int m_backgroundRed;
  /* 0088 */ int m_backgroundGreen;
  /* 008C */ int m_backgroundBlue;
  /* 0090 */ int m_fillRed;
  /* 0094 */ int m_fillGreen;
  /* 0098 */ int m_fillBlue;
};

class UIBarGraph3 : public UIWindow {
public:
  UIBarGraph3();
  ~UIBarGraph3() override;

protected:
  /* 007C */ const int* m_current;
  /* 0080 */ const int* m_maximum;
};

class UIBarGraph4 : public UIBarGraph3 {
public:
  explicit UIBarGraph4() : UIBarGraph3() {
  }

  void Paint() override;
  void Slot28(int x, int y) override;

  void SetValues(const int* current, const int* maximum);

  /* 0084 */ bool m_plainColor;

protected:
  void* unknown_controls;
};

class UIINT64BarGraph : public UIWindow {
public:
  UIINT64BarGraph();

  void Paint() override;
  void Slot28(int x, int y) override;

  void SetValues(std::int64_t current, std::int64_t maximum);
  void FillBackground(std::uint32_t color);

  /* 007C */ unsigned char m_plainColor;

protected:
  /* 0080 */ std::int64_t m_current;
  /* 0088 */ std::int64_t m_maximum;
  void* unknown_controls;
};

class UIPetInfoBarGraph : public UIWindow {
public:
  UIPetInfoBarGraph();

  void Paint() override;
  void SetValue(int value);

  /* 007C */ int unknown_007C;
};

class UIBarGraphHomun : public UIWindow {
public:
  UIBarGraphHomun();

  void Paint() override;

  void SetValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum);

protected:
  /* 0080 */ std::function<std::int64_t()> m_current;
  /* 00A8 */ std::function<std::int64_t()> m_maximum;
};

class UIBarGraphPlayer : public UIWindow {
public:
  UIBarGraphPlayer();
  ~UIBarGraphPlayer() override;

  void Paint() override;

  void SetValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum);

protected:
  /* 0080 */ std::function<std::int64_t()> m_current;
  /* 00A8 */ std::function<std::int64_t()> m_maximum;
};

class UIBarGraphPlayerAP : public UIBarGraphPlayer {
public:
  explicit UIBarGraphPlayerAP() : UIBarGraphPlayer() {
  }

  void Paint() override;
};

#endif
