#include "Render/DirectDrawEnumeration.h"
#include "Render/RenderAdapter.h"

// OFFSET: 0x0056B8C0
// FUNCTION: fn_02326
std::vector<RenderAdapter> EnumerateAdaptersDX7(DirectDrawEnumeration& enumeration) {
  std::vector<RenderAdapter> result;

  if (enumeration.devicesSeen == 0 && FAILED(enumeration.Enumerate(nullptr))) {
    return result;
  }

  for (unsigned index = 0; index < enumeration.deviceCount; ++index) {
    const auto& device = enumeration.devices[index];

    if (device.hardware) {
      result.push_back({1, index, device.description, device.drawIdentity, device.renderIdentity, {}, {}});
    }
  }

  return result;
}
