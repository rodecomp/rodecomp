#ifndef RAGNAROK_SOURCE_UI_GAUGES_UIPCGAGE_H
#define RAGNAROK_SOURCE_UI_GAUGES_UIPCGAGE_H

#include "UI/Window/UIWindow.h"

#include <cstddef>
#include <cstdint>

class UIGage : public UIWindow {
public:
  /* 0080 */ std::uint32_t colors[7];
};

class UIPcGage : public UIGage {
public:
  __declspec(noinline) void SetValues(int current, int maximum);

  /* 009C */ int unknown_009C;
  /* 00A0 */ int current;
  /* 00A4 */ int maximum;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ int unknown_00AC;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIGage, colors) == 0x80);
static_assert(offsetof(UIPcGage, current) == 0xA0);
static_assert(sizeof(UIPcGage) == 0xB0);
#endif

#endif
