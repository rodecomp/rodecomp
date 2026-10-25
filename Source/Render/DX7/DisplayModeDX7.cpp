#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/DisplayMode.h"

#include <utility>

// OFFSET: 0x0056BDD0
// FUNCTION: fn_02331
std::vector<DisplayMode> EnumerateDisplayModesDX7(DirectDrawEnumeration& enumeration, unsigned deviceIndex) {
  std::vector<DisplayMode> result;

  if (enumeration.devicesSeen == 0 && FAILED(enumeration.Enumerate(nullptr))) {
    return result;
  }

  if (deviceIndex >= enumeration.deviceCount) {
    return result;
  }

  const auto& record = enumeration.devices[deviceIndex];

  for (unsigned index = 0; index < record.modeCount; ++index) {
    const auto& mode = record.modes[index];
    const unsigned width = mode.dwWidth;
    const unsigned height = mode.dwHeight;
    const unsigned bits = mode.ddpfPixelFormat.dwRGBBitCount;

    if (height < 664 || width > 2048 || height > 1440) {
      continue;
    }

    OSVERSIONINFOEXW version{};
    version.dwOSVersionInfoSize = sizeof(version);
    version.dwMajorVersion = 6;
    version.dwMinorVersion = 2;
    DWORDLONG mask = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
    mask = VerSetConditionMask(mask, VER_MINORVERSION, VER_GREATER_EQUAL);
    mask = VerSetConditionMask(mask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
    const bool modern = VerifyVersionInfoW(&version, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, mask) != FALSE;

    if (modern ? bits != 32 : (bits != 16 && bits != 32)) {
      continue;
    }

    std::string description = std::to_string(width) + " x " + std::to_string(height);

    if (!modern) {
      description += " x ";
      description += std::to_string(bits);
    }

    result.push_back({width, height, bits, std::move(description)});
  }

  return result;
}
