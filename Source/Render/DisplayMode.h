#ifndef RAGNAROK_SOURCE_RENDER_DISPLAYMODE_H
#define RAGNAROK_SOURCE_RENDER_DISPLAYMODE_H

#include <string>
#include <vector>
#include <windows.h>

struct DisplayMode {
  unsigned width;
  unsigned height;
  unsigned bitsPerPixel;
  std::string description;
};

class DirectDrawEnumeration;

std::vector<DisplayMode> EnumerateDisplayModes(int backend, unsigned deviceIndex, DirectDrawEnumeration& legacy);

std::vector<DisplayMode> EnumerateDisplayModesDX7(DirectDrawEnumeration& enumeration, unsigned deviceIndex);

std::vector<DisplayMode> EnumerateDisplayModesDX9(UINT adapter);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DisplayMode) == 36);
#endif

#endif
