#ifndef RAGNAROK_SOURCE_RENDER_RENDERADAPTER_H
#define RAGNAROK_SOURCE_RENDER_RENDERADAPTER_H

#include <cstddef>
#include <string>
#include <vector>
#include <windows.h>

class DeviceOwnerDX9;
class DirectDrawEnumeration;
struct DeviceOptionsDX9;

struct RenderAdapter {
  /* 0000 */ int backend;
  /* 0004 */ UINT ordinal;
  /* 0008 */ std::string description;
  /* 0020 */ GUID drawGuid;
  /* 0030 */ GUID renderGuid;
  /* 0040 */ GUID deviceGuid;
  /* 0050 */ std::string deviceName;

  bool Matches(const RenderAdapter& selected) const;
};

struct AdapterConfiguration {
  const int& backend;
  const GUID& drawGuid;
  const GUID& renderGuid;
  const GUID& deviceGuid;
  const char* deviceName;
};

RenderAdapter SelectedRenderAdapter(const AdapterConfiguration& configuration, bool serviceUsesDX7);
std::vector<RenderAdapter> EnumerateRenderAdapters(int backend, DirectDrawEnumeration& legacy);
std::vector<RenderAdapter> EnumerateAdaptersDX7(DirectDrawEnumeration& enumeration);
std::vector<RenderAdapter> EnumerateAdaptersDX9();
HRESULT InitializeConfiguredDeviceDX9(
  DeviceOwnerDX9& owner, HWND window, const DeviceOptionsDX9& options, const AdapterConfiguration& configuration, bool serviceUsesDX7
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderAdapter) == 104);
static_assert(offsetof(RenderAdapter, ordinal) == 0x0004);
static_assert(offsetof(RenderAdapter, description) == 0x0008);
static_assert(offsetof(RenderAdapter, drawGuid) == 0x0020);
static_assert(offsetof(RenderAdapter, renderGuid) == 0x0030);
static_assert(offsetof(RenderAdapter, deviceGuid) == 0x0040);
static_assert(offsetof(RenderAdapter, deviceName) == 0x0050);
#endif

#endif
