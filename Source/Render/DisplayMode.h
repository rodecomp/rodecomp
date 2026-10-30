#ifndef RAGNAROK_SOURCE_RENDER_DISPLAYMODE_H
#define RAGNAROK_SOURCE_RENDER_DISPLAYMODE_H

#include <cstddef>
#include <string>
#include <vector>
#include <windows.h>

struct DisplayMode {
  /* 0000 */ unsigned width;
  /* 0004 */ unsigned height;
  /* 0008 */ unsigned bitsPerPixel;
  /* 000C */ std::string description;

  bool Matches(const DisplayMode& other) const;
};

std::vector<DisplayMode> EnumerateDisplayModes(int backend, unsigned deviceIndex);

std::vector<DisplayMode> EnumerateDisplayModesDX7(unsigned deviceIndex);

std::vector<DisplayMode> EnumerateDisplayModesDX9(UINT adapter);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DisplayMode) == 36);
static_assert(offsetof(DisplayMode, height) == 0x0004);
static_assert(offsetof(DisplayMode, bitsPerPixel) == 0x0008);
static_assert(offsetof(DisplayMode, description) == 0x000C);
#endif

#endif
