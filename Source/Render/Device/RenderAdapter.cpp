#include "Contents/Configuration/ServiceOptions.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RenderSystemDX9.h"
#include "Render/Device/RenderAdapter.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Device/DeviceSelection.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "UI/GameSettings/GameSettingsUI.h"

#include <atlbase.h>
#include <cstring>

// OFFSET: 0x005699A0
// FUNCTION: fn_02268
RenderSystemHolder::RenderSystemHolder() : system(nullptr) {
}

// OFFSET: 0x00569F20
// FUNCTION: fn_02275
CSurface* RenderSystemHolder::CreateSurface(std::uint32_t width, std::uint32_t height) {
  return this->system ? this->system->CreateSurface(width, height) : nullptr;
}

// OFFSET: 0x00569FF0
// FUNCTION: fn_02283
std::vector<RenderAdapter> EnumerateRenderAdapters(int backend) {
  switch (backend) {
  case RENDER_BACKEND_DX7:
    return EnumerateAdaptersDX7();
  case RENDER_BACKEND_DX9:
    return EnumerateAdaptersDX9();
  default:
    return {};
  }
}

// OFFSET: 0x00569DA0
// FUNCTION: fn_02271
bool RenderAdapter::Matches(const RenderAdapter& selected) const {
  if (this->backend != selected.backend) {
    return false;
  }

  switch (this->backend) {
  case RENDER_BACKEND_DX7:
    return std::memcmp(&this->drawGuid, &selected.drawGuid, sizeof(GUID)) == 0 && std::memcmp(&this->renderGuid, &selected.renderGuid, sizeof(GUID)) == 0;
  case RENDER_BACKEND_DX9:
    return std::memcmp(&this->deviceGuid, &selected.deviceGuid, sizeof(GUID)) == 0 && this->deviceName == selected.deviceName;
  default:
    return false;
  }
}

__declspec(noinline) RenderAdapter SelectedRenderAdapter();

// OFFSET: 0x0056A0F0
// FUNCTION: fn_02284
RenderAdapter SelectedRenderAdapter() {
  RenderAdapter selected{};
  selected.backend = g_configuredRenderBackend;

  if (selected.backend == RENDER_BACKEND_DEFAULT) {
    selected.backend = RenderSystemHolder::PreferredBackend();
  }

  switch (selected.backend) {
  case RENDER_BACKEND_DX7:
    std::memcpy(&selected.drawGuid, g_guidDriver, sizeof(GUID));
    std::memcpy(&selected.renderGuid, g_guidDevice, sizeof(GUID));
    break;
  case RENDER_BACKEND_DX9:
    selected.deviceGuid = g_configuredDeviceGuid;
    selected.deviceName = g_configuredDeviceName;
    break;
  }

  return selected;
}

__declspec(noinline) std::vector<RenderAdapter> EnumerateAdaptersDX9();

// OFFSET: 0x0056D770
// FUNCTION: fn_02368
std::vector<RenderAdapter> EnumerateAdaptersDX9() {
  CComPtr<IDirect3D9Ex> direct3D;
  const HRESULT result = Direct3DCreate9Ex(D3D_SDK_VERSION, &direct3D);

  if (FAILED(result)) {
    return {};
  }

  const UINT count = direct3D->GetAdapterCount();
  std::vector<RenderAdapter> adapters;
  adapters.reserve(count);

  for (UINT ordinal = 0; ordinal < count; ++ordinal) {
    D3DADAPTER_IDENTIFIER9 identifier;

    if (SUCCEEDED(direct3D->GetAdapterIdentifier(ordinal, 0, &identifier))) {
      adapters.push_back({2, ordinal, identifier.Description, g_defaultDrawGuidDX7, g_defaultDrawGuidDX7, identifier.DeviceIdentifier, identifier.DeviceName});
    }
  }

  return adapters;
}

// OFFSET: 0x0056E270
// FUNCTION: fn_02375
HRESULT CRenderSystemDX9::Initialize(HWND window) {
  const std::vector<RenderAdapter> adapters = EnumerateAdaptersDX9();

  if (adapters.size() == 0) {
    MessageBoxA(nullptr, "No compatible devices", "Error", MB_OK);
    return D3DERR_NOTAVAILABLE;
  }

  const RenderAdapter selected = SelectedRenderAdapter();
  UINT ordinal = D3DADAPTER_DEFAULT;

  for (size_t i = 0; i < adapters.size(); ++i) {
    const RenderAdapter& adapter = adapters[i];

    if (adapter.Matches(selected)) {
      ordinal = adapter.ordinal;
      break;
    }
  }

  return this->m_device->Initialize(window, ordinal);
}
