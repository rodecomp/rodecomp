#ifndef RAGNAROK_SOURCE_UI_GAUGES_UIPCGAGE_H
#define RAGNAROK_SOURCE_UI_GAUGES_UIPCGAGE_H

#include "UI/Window/UIWindow.h"

#include <cstddef>
#include <cstdint>
#include <functional>

class UIGage : public UIWindow {
public:
  UIGage();
  bool CanHitTest() override;
  __declspec(noinline) void SetColor(std::uint32_t color);

  /* 0080 */ std::uint32_t colors[7];
};

class UIPcGage : public UIGage {
public:
  UIPcGage();
  void Paint() override;
  __declspec(noinline) void SetValues(int current, int maximum);
  void SetLimits(int first, int second);

  /* 009C */ int unknown_009C;
  /* 00A0 */ int current;
  /* 00A4 */ int maximum;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ int unknown_00AC;
};

class UIRechargeGage : public UIGage {
public:
  void Paint() override;
  void SetValues(int current, int maximum);

  /* 009C */ int current;
  /* 00A0 */ int maximum;
};

class UIPlayerGage : public UIGage {
public:
  bool Slot03() override;
  void SetFirstValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum);
  void SetSecondValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum);
  void SetThirdValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum);
  void SetFourthValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum);

  /* 009C */ int unknown_009C;
  /* 00A0 */ std::function<std::int64_t()> m_firstCurrent;
  /* 00C8 */ std::function<std::int64_t()> m_firstMaximum;
  /* 00F0 */ std::function<std::int64_t()> m_secondCurrent;
  /* 0118 */ std::function<std::int64_t()> m_secondMaximum;
  /* 0140 */ std::function<std::int64_t()> m_thirdCurrent;
  /* 0168 */ std::function<std::int64_t()> m_thirdMaximum;
  /* 0190 */ std::function<std::int64_t()> m_fourthCurrent;
  /* 01B8 */ std::function<std::int64_t()> m_fourthMaximum;
};

class UIMonsterGage : public UIPcGage {
public:
  void Paint() override;
  void SetMode(int mode);

  /* 00B0 */ int m_threshold;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIGage, colors) == 0x80);
static_assert(offsetof(UIPcGage, current) == 0xA0);
static_assert(sizeof(UIPcGage) == 0xB0);
#endif

#endif
