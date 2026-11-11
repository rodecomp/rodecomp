#include "Client/Application/Registry.h"
#include "Render/Device/DeviceSelection.h"
#include "Render/Device/DisplayMode.h"

#include <algorithm>
#include <atlbase.h>
#include <d3d9.h>
#include <format>
#include <utility>

// OFFSET: 0x00569E60
// FUNCTION: fn_02272
bool DisplayMode::Matches(const DisplayMode& other) const {
  return this->width == other.width && this->height == other.height && this->bitsPerPixel == other.bitsPerPixel;
}

// OFFSET: 0x0056A590
// FUNCTION: fn_02291
std::vector<DisplayMode> EnumerateDisplayModes(int backend, unsigned deviceIndex) {
  switch (backend) {
  case 1:
    return EnumerateDisplayModesDX7(deviceIndex);
  case 2:
    return EnumerateDisplayModesDX9(deviceIndex);
  default:
    return {};
  }
}

// OFFSET: 0x0056DCF0
// FUNCTION: fn_02372
std::vector<DisplayMode> EnumerateDisplayModesDX9(UINT adapter) {
  CComPtr<IDirect3D9Ex> direct3D;
  if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &direct3D))) {
    return {};
  }

  const UINT count = direct3D->GetAdapterModeCount(adapter, D3DFMT_X8R8G8B8);
  std::vector<std::pair<unsigned, unsigned>> dimensions;
  dimensions.reserve(count);

  for (UINT index = 0; index < count; ++index) {
    D3DDISPLAYMODE mode;

    if (SUCCEEDED(direct3D->EnumAdapterModes(adapter, D3DFMT_X8R8G8B8, index, &mode)) && mode.Height >= 664) {
      dimensions.emplace_back(mode.Width, mode.Height);
    }
  }

  std::sort(dimensions.begin(), dimensions.end());
  dimensions.erase(std::unique(dimensions.begin(), dimensions.end()), dimensions.end());

  std::vector<DisplayMode> result;
  result.reserve(dimensions.size());

  for (const auto& [width, height] : dimensions) {
    const std::string description = std::format("{} x {}", width, height);
    result.push_back({width, height, 32, description});
  }

  return result;
}

// OFFSET: 0x0056A7E0
// FUNCTION: fn_02297
LSTATUS SaveDisplayDeviceSettings() {
  HKEY key = nullptr;
  LSTATUS status = RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr);

  if (status == ERROR_SUCCESS) {
    RegSetValueExA(key, "GUIDDEVICE", 0, REG_BINARY, g_guidDevice, 16);
    RegSetValueExA(key, "GUIDDRIVER", 0, REG_BINARY, g_guidDriver, 16);
    status = RegSetValueExA(key, "DEVICENAME", 0, REG_SZ, reinterpret_cast<const BYTE*>(g_deviceName), 40);
  }

  return status;
}
