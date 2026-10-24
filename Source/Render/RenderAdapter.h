#ifndef RAGNAROK_SOURCE_RENDER_RENDERADAPTER_H
#define RAGNAROK_SOURCE_RENDER_RENDERADAPTER_H

#include <string>
#include <vector>
#include <windows.h>

class DeviceOwnerDX9;
class DirectDrawEnumeration;
struct DeviceOptionsDX9;

struct RenderAdapter {
  int backend;
  UINT ordinal;
  std::string description;
  GUID drawGuid;
  GUID renderGuid;
  GUID deviceGuid;
  std::string deviceName;

  bool Matches(const RenderAdapter& selected) const;
};

// Original selected backend/identity globals, borrowed from configuration.
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
  DeviceOwnerDX9& owner,
  HWND window,
  const DeviceOptionsDX9& options,
  const AdapterConfiguration& configuration,
  bool serviceUsesDX7
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderAdapter) == 104);
#endif

#endif
