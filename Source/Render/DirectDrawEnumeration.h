#ifndef RAGNAROK_SOURCE_RENDER_DIRECTDRAWENUMERATION_H
#define RAGNAROK_SOURCE_RENDER_DIRECTDRAWENUMERATION_H

#include <array>
#include <cstddef>
#include <d3d.h>
#include <ddraw.h>

struct DirectDrawDeviceRecord {
  char description[40];
  GUID* renderGuid;
  D3DDEVICEDESC7 device;
  DWORD hardware;
  GUID* drawGuid;
  DDCAPS driverCaps;
  DDCAPS emulationCaps;
  DDSURFACEDESC2 selectedMode;
  DWORD primary;
  DWORD unknown_0498;
  GUID renderIdentity;
  GUID drawIdentity;
  DDSURFACEDESC2* modes;
  unsigned modeCount;
  unsigned selectedIndex;
  DWORD primaryAdapter;
  DWORD hasStereoMode;
};

// Original shared device table and counters. ClearModes releases mode buffers
// without resetting discovery counts or retained descriptor metadata.
class DirectDrawEnumeration {
public:
  using Filter = HRESULT(__cdecl*)(const DDCAPS*, const D3DDEVICEDESC7*);
  DirectDrawEnumeration(const GUID& selectedDraw, RECT& monitor);
  HRESULT Enumerate(Filter filter);
  void ClearModes();

  std::array<DirectDrawDeviceRecord, 20> devices{};
  unsigned devicesSeen = 0;
  unsigned deviceCount = 0;

private:
  struct AdapterContext;
  const GUID& m_selectedDraw;
  RECT& m_monitor;
  Filter m_filter = nullptr;
  static BOOL WINAPI Adapter(GUID* guid, char* description, char* name, void* context, HMONITOR monitor);
  static HRESULT CALLBACK Mode(DDSURFACEDESC2* mode, void* context);
  static HRESULT CALLBACK Device(char* description, char* name, D3DDEVICEDESC7* device, void* context);
  static int __cdecl CompareModes(const void* left, const void* right);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DDCAPS) == 380);
static_assert(sizeof(D3DDEVICEDESC7) == 236);
static_assert(sizeof(DirectDrawDeviceRecord) == 1232);
static_assert(offsetof(DirectDrawDeviceRecord, modes) == 1212);
#endif

#endif
