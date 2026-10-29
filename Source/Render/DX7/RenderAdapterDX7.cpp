#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/RenderAdapter.h"

// OFFSET: 0x0056B8C0
// FUNCTION: fn_02326
std::vector<RenderAdapter> EnumerateAdaptersDX7() {
  std::vector<RenderAdapter> result;

  if (DirectDrawEnumeration::DevicesSeen() == 0 && FAILED(DirectDrawEnumeration::Enumerate(nullptr))) {
    return result;
  }

  const DirectDrawDeviceRecord* devices = DirectDrawEnumeration::Devices();
  const unsigned count = DirectDrawEnumeration::DeviceCount();

  for (unsigned index = 0; index < count; ++index) {
    const auto& device = devices[index];

    if (device.hardware) {
      result.push_back({1, index, device.description, device.drawIdentity, device.renderIdentity, {}, {}});
    }
  }

  return result;
}

// OFFSET: 0x00DD9420
// FUNCTION: fn_23334
HRESULT __cdecl AcceptAnyDirectDrawDevice(const DDCAPS*, const D3DDEVICEDESC7*) {
  return S_OK;
}
