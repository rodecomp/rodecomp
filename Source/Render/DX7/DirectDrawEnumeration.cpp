#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/Device/DeviceSelection.h"

#include <cstdlib>
#include <cstring>
#include <limits>

// GLOBAL: gl_24698
RECT g_monitorRectangleDX7;

// GLOBAL: gl_24690
DirectDrawDeviceRecord g_directDrawModes[20];

// GLOBAL: gl_24691
unsigned g_directDrawModesSeen;

// GLOBAL: gl_24689
unsigned g_directDrawModeCount;

// GLOBAL: gl_24714
DirectDrawEnumeration::Filter g_directDrawFilter;

// OFFSET: 0x007E9C00
// FUNCTION: fn_09777
HRESULT DirectDrawEnumeration::Enumerate(Filter filter) {
  g_directDrawFilter = filter;
  DirectDrawEnumerateExA(Adapter, nullptr, 7);

  if (g_directDrawModesSeen == 0) {
    return static_cast<HRESULT>(0x81000002u);
  }

  return g_directDrawModeCount == 0 ? static_cast<HRESULT>(0x81000003u) : S_OK;
}

// OFFSET: 0x007EA180
// FUNCTION: fn_09785
void DirectDrawEnumeration::ClearModes() {
  for (unsigned index = 0; index < g_directDrawModeCount; ++index) {
    if (g_directDrawModes[index].modes) {
      delete[] g_directDrawModes[index].modes;
      g_directDrawModes[index].modes = nullptr;
    }
  }
}

// OFFSET: 0x007EA090
// FUNCTION: fn_09780
DirectDrawDeviceRecord* DirectDrawEnumeration::Devices() {
  return g_directDrawModes;
}

// OFFSET: 0x007EA0A0
// FUNCTION: fn_09781
unsigned DirectDrawEnumeration::DeviceCount() {
  return g_directDrawModeCount;
}

// OFFSET: 0x007EA0B0
// FUNCTION: fn_09782
unsigned DirectDrawEnumeration::DevicesSeen() {
  return g_directDrawModesSeen;
}

// OFFSET: 0x007EA140
// FUNCTION: fn_09784
int __cdecl DirectDrawEnumeration::CompareModes(const void* left, const void* right) {
  const auto& a = *static_cast<const DDSURFACEDESC2*>(left);
  const auto& b = *static_cast<const DDSURFACEDESC2*>(right);

  if (a.dwWidth < b.dwWidth) {
    return -1;
  }
  if (a.dwWidth > b.dwWidth) {
    return 1;
  }
  if (a.dwHeight < b.dwHeight) {
    return -1;
  }
  if (a.dwHeight > b.dwHeight) {
    return 1;
  }
  if (a.ddpfPixelFormat.dwRGBBitCount < b.ddpfPixelFormat.dwRGBBitCount) {
    return -1;
  }
  if (a.ddpfPixelFormat.dwRGBBitCount > b.ddpfPixelFormat.dwRGBBitCount) {
    return 1;
  }

  return 0;
}

// OFFSET: 0x007EA0C0
// FUNCTION: fn_09783
HRESULT CALLBACK DirectDrawEnumeration::Mode(DDSURFACEDESC2* mode, void* context) {
  auto& record = *static_cast<DirectDrawDeviceRecord*>(context);
  DDSURFACEDESC2* replacement = new DDSURFACEDESC2[record.modeCount + 1];
  std::memcpy(replacement, record.modes, record.modeCount * sizeof(DDSURFACEDESC2));
  delete[] record.modes;
  record.modes = replacement;
  record.modes[record.modeCount] = *mode;
  ++record.modeCount;

  return DDENUMRET_OK;
}

// OFFSET: 0x007E9C40
// FUNCTION: fn_09778
HRESULT CALLBACK DirectDrawEnumeration::Device(char*, char* name, D3DDEVICEDESC7* device, void* context) {
  const auto& source = *static_cast<DirectDrawDeviceRecord*>(context);
  ++g_directDrawModesSeen;
  auto& record = g_directDrawModes[g_directDrawModeCount];
  std::memset(&record, 0, sizeof(record));
  record.hardware = device->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION;
  record.device = *device;
  record.primaryAdapter = source.primaryAdapter;
  record.driverCaps = source.driverCaps;
  record.emulationCaps = source.emulationCaps;
  record.renderIdentity = device->deviceGUID;
  record.renderGuid = &record.renderIdentity;
  record.modes = new DDSURFACEDESC2[source.modeCount];

  if (source.drawGuid) {
    record.drawIdentity = source.drawIdentity;
    record.drawGuid = &record.drawIdentity;
    lstrcpynA(record.description, source.description, 39);
  } else {
    record.drawGuid = nullptr;
    lstrcpynA(record.description, name, 39);
  }

  if (record.drawGuid && !record.hardware) {
    delete[] record.modes;
    return D3DENUMRET_OK;
  }

  if (g_directDrawFilter && FAILED(g_directDrawFilter(&record.driverCaps, &record.device))) {
    delete[] record.modes;
    return D3DENUMRET_OK;
  }

  for (unsigned index = 0; index < source.modeCount; ++index) {
    const DDSURFACEDESC2 mode = source.modes[index];
    const DWORD bits = mode.ddpfPixelFormat.dwRGBBitCount;
    const DWORD supported = record.device.dwDeviceRenderBitDepth;
    const bool accepted = (bits == 32 && (supported & DDBD_32)) || (bits == 24 && (supported & DDBD_24)) || (bits == 16 && (supported & DDBD_16));

    if (accepted) {
      record.modes[record.modeCount++] = mode;

      if (mode.ddsCaps.dwCaps2 & DDSCAPS2_STEREOSURFACELEFT) {
        record.hasStereoMode = 1;
      }
    }
  }

  if (record.modeCount == 0) {
    delete[] record.modes;
    return D3DENUMRET_OK;
  }

  for (unsigned index = 0; index < record.modeCount; ++index) {
    if (record.modes[index].dwWidth == 1024 && record.modes[index].dwHeight == 768 && record.modes[index].ddpfPixelFormat.dwRGBBitCount == 16) {
      record.selectedMode = record.modes[index];
      record.selectedIndex = index;
    }
  }

  ++g_directDrawModeCount;
  record.primary = record.primaryAdapter;
  return D3DENUMRET_OK;
}

// OFFSET: 0x007E9EA0
// FUNCTION: fn_09779
BOOL WINAPI DirectDrawEnumeration::Adapter(GUID* guid, char* description, char*, void*, HMONITOR monitor) {

  if (guid && monitor && std::memcmp(g_guidDriver, guid, sizeof(GUID)) == 0) {
    MONITORINFOEXA information;
    information.cbSize = sizeof(information);
    GetMonitorInfoA(monitor, &information);
    g_monitorRectangleDX7 = information.rcMonitor;
  }

  IDirectDraw7* draw;

  if (FAILED(DirectDrawCreateEx(guid, reinterpret_cast<void**>(&draw), IID_IDirectDraw7, nullptr))) {
    return TRUE;
  }

  IDirect3D7* render;

  if (FAILED(draw->QueryInterface(IID_IDirect3D7, reinterpret_cast<void**>(&render)))) {
    draw->Release();
    return TRUE;
  }

  DirectDrawDeviceRecord record{};
  lstrcpynA(record.description, description, 39);
  record.driverCaps.dwSize = sizeof(DDCAPS);
  record.emulationCaps.dwSize = sizeof(DDCAPS);
  draw->GetCaps(&record.driverCaps, &record.emulationCaps);

  if (guid) {
    record.drawIdentity = *guid;
    record.drawGuid = &record.drawIdentity;
  }

  if ((record.driverCaps.dwCaps2 & DDCAPS2_CANRENDERWINDOWED) && !record.drawGuid) {
    record.primaryAdapter = 1;
  }

  draw->EnumDisplayModes(0, nullptr, &record, Mode);
  std::qsort(record.modes, record.modeCount, sizeof(DDSURFACEDESC2), CompareModes);
  render->EnumDevices(Device, &record);
  if (record.modes) {
    delete record.modes;
    record.modes = nullptr;
  }
  render->Release();
  draw->Release();

  return TRUE;
}
