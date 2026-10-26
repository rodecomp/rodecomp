#include "Client/Registry.h"
#include "Render/DeviceSelection.h"
#include "Render/DisplayMode.h"

#include <algorithm>
#include <d3d9.h>
#include <utility>

// OFFSET: 0x0056A590
// FUNCTION: fn_02291
std::vector<DisplayMode> EnumerateDisplayModes(int backend, unsigned deviceIndex, DirectDrawEnumeration& legacy) {
  if (backend == 1) {
    return EnumerateDisplayModesDX7(legacy, deviceIndex);
  }

  if (backend == 2) {
    return EnumerateDisplayModesDX9(deviceIndex);
  }

  return {};
}

// OFFSET: 0x0056DCF0
// FUNCTION: fn_02372
std::vector<DisplayMode> EnumerateDisplayModesDX9(UINT adapter) {
  IDirect3D9Ex* direct3D = nullptr;
  std::vector<DisplayMode> result;

  if (SUCCEEDED(Direct3DCreate9Ex(D3D_SDK_VERSION, &direct3D))) {
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
    result.reserve(dimensions.size());

    for (const auto& [width, height] : dimensions) {
      result.push_back({width, height, 32, std::to_string(width) + " x " + std::to_string(height)});
    }
  }

  if (direct3D) {
    direct3D->Release();
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
