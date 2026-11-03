#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/RenderAdapter.h"
#include "Client/Session.h"
#include "Render/DeviceSelection.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "UI/GameSettings/GameSettingsUI.h"

#include <cstring>

// OFFSET: 0x00569FF0
// FUNCTION: fn_02283
std::vector<RenderAdapter> EnumerateRenderAdapters(int backend) {
  if (backend == 1) {
    return EnumerateAdaptersDX7();
  }

  if (backend == 2) {
    return EnumerateAdaptersDX9();
  }

  return {};
}

// OFFSET: 0x00569DA0
// FUNCTION: fn_02271
bool RenderAdapter::Matches(const RenderAdapter& selected) const {
  if (this->backend != selected.backend) {
    return false;
  }

  if (this->backend == 1) {
    return std::memcmp(&this->drawGuid, &selected.drawGuid, sizeof(GUID)) == 0 && std::memcmp(&this->renderGuid, &selected.renderGuid, sizeof(GUID)) == 0;
  }

  if (this->backend == 2) {
    return std::memcmp(&this->deviceGuid, &selected.deviceGuid, sizeof(GUID)) == 0 && this->deviceName == selected.deviceName;
  }

  return false;
}

// OFFSET: 0x0056A0F0
// FUNCTION: fn_02284
RenderAdapter SelectedRenderAdapter() {
  RenderAdapter selected{};
  selected.backend = g_configuredRenderBackend;

  if (selected.backend == 0) {
    selected.backend = g_session.DefaultRenderBackend() == 1 ? 1 : 2;
  }

  if (selected.backend == 1) {
    std::memcpy(&selected.drawGuid, g_guidDriver, sizeof(GUID));
    std::memcpy(&selected.renderGuid, g_guidDevice, sizeof(GUID));
  } else if (selected.backend == 2) {
    selected.deviceGuid = g_configuredDeviceGuid;
    selected.deviceName = g_configuredDeviceName;
  }

  return selected;
}

// OFFSET: 0x0056D770
// FUNCTION: fn_02368
std::vector<RenderAdapter> EnumerateAdaptersDX9() {
  IDirect3D9Ex* direct3D = nullptr;
  const HRESULT result = Direct3DCreate9Ex(D3D_SDK_VERSION, &direct3D);
  std::vector<RenderAdapter> adapters;

  if (SUCCEEDED(result)) {
    const UINT count = direct3D->GetAdapterCount();
    adapters.reserve(count);

    for (UINT ordinal = 0; ordinal < count; ++ordinal) {
      D3DADAPTER_IDENTIFIER9 identifier;

      if (SUCCEEDED(direct3D->GetAdapterIdentifier(ordinal, 0, &identifier))) {
        adapters.push_back({2, ordinal, identifier.Description, {}, {}, identifier.DeviceIdentifier, identifier.DeviceName});
      }
    }
  }

  if (direct3D) {
    direct3D->Release();
  }

  return adapters;
}

// OFFSET: 0x0056E270
// FUNCTION: fn_02375
HRESULT InitializeConfiguredDeviceDX9(DeviceOwnerDX9& owner, HWND window) {
  const std::vector<RenderAdapter> adapters = EnumerateAdaptersDX9();

  if (adapters.empty()) {
    MessageBoxA(nullptr, "No compatible devices", "Error", MB_OK);
    return D3DERR_NOTAVAILABLE;
  }

  const RenderAdapter selected = SelectedRenderAdapter();
  UINT ordinal = D3DADAPTER_DEFAULT;

  for (const RenderAdapter& adapter : adapters) {
    if (adapter.Matches(selected)) {
      ordinal = adapter.ordinal;
      break;
    }
  }

  return owner.Initialize(window, ordinal);
}
