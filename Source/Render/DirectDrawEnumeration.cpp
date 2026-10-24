#include "Render/DirectDrawEnumeration.h"

#include <cstdlib>
#include <cstring>
#include <limits>

struct DirectDrawEnumeration::AdapterContext {
  DirectDrawEnumeration& owner;
  DirectDrawDeviceRecord record{};
};

DirectDrawEnumeration::DirectDrawEnumeration(const GUID& selectedDraw, RECT& monitor)
    : m_selectedDraw(selectedDraw), m_monitor(monitor) {
}

// OFFSET: 0x007E9C00
// FUNCTION: fn_09777
HRESULT DirectDrawEnumeration::Enumerate(Filter filter) {
  this->m_filter = filter;
  DirectDrawEnumerateExA(Adapter, this, 7);

  if (this->devicesSeen == 0) {
    return static_cast<HRESULT>(0x81000002u);
  }

  return this->deviceCount == 0 ? static_cast<HRESULT>(0x81000003u) : S_OK;
}

// OFFSET: 0x007EA180
// FUNCTION: fn_09785
void DirectDrawEnumeration::ClearModes() {
  for (unsigned index = 0; index < this->deviceCount; ++index) {
    std::free(this->devices[index].modes);
    this->devices[index].modes = nullptr;
  }
}

// OFFSET: 0x007EA140
// FUNCTION: fn_09784
int __cdecl DirectDrawEnumeration::CompareModes(const void* left, const void* right) {
  const auto& a = *static_cast<const DDSURFACEDESC2*>(left);
  const auto& b = *static_cast<const DDSURFACEDESC2*>(right);

  if (a.dwWidth != b.dwWidth) {
    return a.dwWidth < b.dwWidth ? -1 : 1;
  }

  if (a.dwHeight != b.dwHeight) {
    return a.dwHeight < b.dwHeight ? -1 : 1;
  }

  if (a.ddpfPixelFormat.dwRGBBitCount != b.ddpfPixelFormat.dwRGBBitCount) {
    return a.ddpfPixelFormat.dwRGBBitCount < b.ddpfPixelFormat.dwRGBBitCount ? -1 : 1;
  }

  return 0;
}

// OFFSET: 0x007EA0C0
// FUNCTION: fn_09783
HRESULT CALLBACK DirectDrawEnumeration::Mode(DDSURFACEDESC2* mode, void* context) {
  auto& record = static_cast<AdapterContext*>(context)->record;
  const unsigned count = record.modeCount + 1;
  const std::size_t size = count > std::numeric_limits<std::size_t>::max() / sizeof(*mode)
                             ? std::numeric_limits<std::size_t>::max()
                             : count * sizeof(*mode);
  auto* replacement = static_cast<DDSURFACEDESC2*>(std::malloc(size));

  if (record.modeCount) {
    std::memcpy(replacement, record.modes, record.modeCount * sizeof(*mode));
  }

  std::free(record.modes);
  record.modes = replacement;
  record.modes[record.modeCount] = *mode;
  ++record.modeCount;
  return DDENUMRET_OK;
}

// OFFSET: 0x007E9C40
// FUNCTION: fn_09778
HRESULT CALLBACK DirectDrawEnumeration::Device(char*, char* name, D3DDEVICEDESC7* device, void* context) {
  auto& adapter = *static_cast<AdapterContext*>(context);
  auto& owner = adapter.owner;
  const auto& source = adapter.record;
  ++owner.devicesSeen;
  auto& record = owner.devices[owner.deviceCount];
  std::memset(&record, 0, sizeof(record));
  record.hardware = device->dwDevCaps & 0x80000;
  record.device = *device;
  record.primaryAdapter = source.primaryAdapter;
  record.driverCaps = source.driverCaps;
  record.emulationCaps = source.emulationCaps;
  record.renderIdentity = device->deviceGUID;
  record.renderGuid = &record.renderIdentity;
  record.modes = static_cast<DDSURFACEDESC2*>(std::malloc(source.modeCount * sizeof(DDSURFACEDESC2)));

  if (source.drawGuid) {
    record.drawIdentity = source.drawIdentity;
    record.drawGuid = &record.drawIdentity;
    lstrcpynA(record.description, source.description, 39);
  } else {
    lstrcpynA(record.description, name, 39);
  }

  if (
    (record.drawGuid && !record.hardware) ||
    (owner.m_filter && FAILED(owner.m_filter(&record.driverCaps, &record.device)))
  ) {
    std::free(record.modes);
    return D3DENUMRET_OK;
  }

  for (unsigned index = 0; index < source.modeCount; ++index) {
    const auto& mode = source.modes[index];
    const DWORD bits = mode.ddpfPixelFormat.dwRGBBitCount;
    const DWORD supported = record.device.dwDeviceRenderBitDepth;
    const bool accepted =
      (bits == 32 && (supported & 0x100)) || (bits == 24 && (supported & 0x200)) || (bits == 16 && (supported & 0x400));

    if (accepted) {
      record.modes[record.modeCount++] = mode;

      if (mode.ddsCaps.dwCaps2 & DDSCAPS2_STEREOSURFACELEFT) {
        record.hasStereoMode = 1;
      }
    }
  }

  if (record.modeCount == 0) {
    std::free(record.modes);
    return D3DENUMRET_OK;
  }

  for (unsigned index = 0; index < record.modeCount; ++index) {
    const auto& mode = record.modes[index];

    if (mode.dwWidth == 1024 && mode.dwHeight == 768 && mode.ddpfPixelFormat.dwRGBBitCount == 16) {
      record.selectedMode = mode;
      record.selectedIndex = index;
    }
  }

  ++owner.deviceCount;
  record.primary = record.primaryAdapter;
  return D3DENUMRET_OK;
}

// OFFSET: 0x007E9EA0
// FUNCTION: fn_09779
BOOL WINAPI DirectDrawEnumeration::Adapter(GUID* guid, char* description, char*, void* context, HMONITOR monitor) {
  auto& owner = *static_cast<DirectDrawEnumeration*>(context);

  if (guid && monitor && std::memcmp(guid, &owner.m_selectedDraw, sizeof(GUID)) == 0) {
    MONITORINFOEXA information;
    information.cbSize = sizeof(information);
    GetMonitorInfoA(monitor, &information);
    owner.m_monitor = information.rcMonitor;
  }

  IDirectDraw7* draw = nullptr;

  if (FAILED(DirectDrawCreateEx(guid, reinterpret_cast<void**>(&draw), IID_IDirectDraw7, nullptr))) {
    return TRUE;
  }

  IDirect3D7* render = nullptr;

  if (FAILED(draw->QueryInterface(IID_IDirect3D7, reinterpret_cast<void**>(&render)))) {
    draw->Release();
    return TRUE;
  }

  AdapterContext adapter{owner};
  auto& record = adapter.record;
  lstrcpynA(record.description, description, 39);
  record.driverCaps.dwSize = sizeof(DDCAPS);
  record.emulationCaps.dwSize = sizeof(DDCAPS);
  draw->GetCaps(&record.driverCaps, &record.emulationCaps);

  if (guid) {
    record.drawIdentity = *guid;
    record.drawGuid = &record.drawIdentity;
  }

  if ((record.driverCaps.dwCaps2 & 0x80000) && !record.drawGuid) {
    record.primaryAdapter = 1;
  }

  draw->EnumDisplayModes(0, nullptr, &adapter, Mode);
  std::qsort(record.modes, record.modeCount, sizeof(DDSURFACEDESC2), CompareModes);
  render->EnumDevices(Device, &adapter);
  std::free(record.modes);
  render->Release();
  draw->Release();
  return TRUE;
}
