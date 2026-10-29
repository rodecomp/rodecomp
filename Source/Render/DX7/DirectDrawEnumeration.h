#ifndef RAGNAROK_SOURCE_RENDER_DX7_DIRECTDRAWENUMERATION_H
#define RAGNAROK_SOURCE_RENDER_DX7_DIRECTDRAWENUMERATION_H

#include <cstddef>
#include <d3d.h>
#include <ddraw.h>

struct DirectDrawDeviceRecord {
  /* 0000 */ char description[40];
  /* 0028 */ GUID* renderGuid;
  /* 002C */ D3DDEVICEDESC7 device;
  /* 0118 */ DWORD hardware;
  /* 011C */ GUID* drawGuid;
  /* 0120 */ DDCAPS driverCaps;
  /* 029C */ DDCAPS emulationCaps;
  /* 0418 */ DDSURFACEDESC2 selectedMode;
  /* 0494 */ DWORD primary;
  /* 0498 */ DWORD unknown_0498;
  /* 049C */ GUID renderIdentity;
  /* 04AC */ GUID drawIdentity;
  /* 04BC */ DDSURFACEDESC2* modes;
  /* 04C0 */ unsigned modeCount;
  /* 04C4 */ unsigned selectedIndex;
  /* 04C8 */ DWORD primaryAdapter;
  /* 04CC */ DWORD hasStereoMode;
};

class DirectDrawEnumeration {
public:
  using Filter = HRESULT(__cdecl*)(const DDCAPS*, const D3DDEVICEDESC7*);
  static HRESULT Enumerate(Filter filter);
  static DirectDrawDeviceRecord* Devices();
  static unsigned DeviceCount();
  static unsigned DevicesSeen();
  static void ClearModes();

private:
  static BOOL WINAPI Adapter(GUID* guid, char* description, char* name, void* context, HMONITOR monitor);
  static HRESULT CALLBACK Mode(DDSURFACEDESC2* mode, void* context);
  static HRESULT CALLBACK Device(char* description, char* name, D3DDEVICEDESC7* device, void* context);
  static int __cdecl CompareModes(const void* left, const void* right);
};

extern RECT g_monitorRectangleDX7;
extern DirectDrawDeviceRecord g_directDrawModes[20];
extern unsigned g_directDrawModesSeen;
extern unsigned g_directDrawModeCount;
extern DirectDrawEnumeration::Filter g_directDrawFilter;

HRESULT __cdecl AcceptAnyDirectDrawDevice(const DDCAPS* driverCaps, const D3DDEVICEDESC7* device);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DDCAPS) == 380);
static_assert(sizeof(D3DDEVICEDESC7) == 236);
static_assert(sizeof(DirectDrawDeviceRecord) == 1232);
static_assert(offsetof(DirectDrawDeviceRecord, modes) == 1212);
static_assert(offsetof(DirectDrawDeviceRecord, driverCaps) == 0x0120);
static_assert(offsetof(DirectDrawDeviceRecord, selectedMode) == 0x0418);
static_assert(offsetof(DirectDrawDeviceRecord, renderIdentity) == 0x049C);
static_assert(offsetof(DirectDrawDeviceRecord, hasStereoMode) == 0x04CC);
#endif

#endif
