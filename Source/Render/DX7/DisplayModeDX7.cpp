#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/Device/DisplayMode.h"

#include <VersionHelpers.h>
#include <fmt/core.h>
#include <format>

// OFFSET: 0x0056BDD0
// FUNCTION: fn_02331
std::vector<DisplayMode> EnumerateDisplayModesDX7(unsigned deviceIndex) {
  if (DirectDrawEnumeration::DevicesSeen() == 0 && FAILED(DirectDrawEnumeration::Enumerate(AcceptAnyDirectDrawDevice))) {
    return {};
  }

  if (DirectDrawEnumeration::DeviceCount() <= deviceIndex) {
    return {};
  }

  std::vector<DisplayMode> result;

  const auto& record = DirectDrawEnumeration::Devices()[deviceIndex];

  for (unsigned index = 0; index < record.modeCount; ++index) {
    const unsigned width = record.modes[index].dwWidth;
    const unsigned height = record.modes[index].dwHeight;
    const unsigned bits = record.modes[index].ddpfPixelFormat.dwRGBBitCount;

    if (height < 664 || width > 2048 || height > 1440) {
      continue;
    }

    if (IsWindows8OrGreater()) {
      if (bits == 32) {
        const std::string description = fmt::format("{} x {}", width, height);
        result.push_back({width, height, 32, description});
      }
    } else if (bits == 16 || bits == 32) {
      const std::string description = fmt::format("{} x {} x {}", width, height, bits);
      result.push_back({width, height, bits, description});
    }
  }

  return result;
}
