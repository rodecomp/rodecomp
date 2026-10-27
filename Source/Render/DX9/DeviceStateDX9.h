#ifndef RAGNAROK_SOURCE_RENDER_DX9_DEVICESTATEDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_DEVICESTATEDX9_H

#include <cstddef>
#include <d3d9.h>

struct DeviceStateDX9 {
  /* 0000 */ IDirect3DDevice9* device;
  /* 0004 */ DWORD fvf;
  /* 0008 */ DWORD render[210];
  /* 0350 */ DWORD textureStage[2][33];
  /* 0458 */ DWORD sampler[2][14];
  /* 04C8 */ IDirect3DBaseTexture9* textures[2];

  DeviceStateDX9();
  void Initialize(IDirect3DDevice9* device);
  void SetInitialStates();
  void Begin();
  void End();
  HRESULT SetFVF(DWORD value);
  HRESULT SetRenderState(D3DRENDERSTATETYPE state, DWORD value);
  HRESULT SetSamplerState(UINT stage, D3DSAMPLERSTATETYPE state, DWORD value);
  HRESULT SetTexture(UINT stage, IDirect3DBaseTexture9* texture);
  HRESULT SetTextureStageState(UINT stage, D3DTEXTURESTAGESTATETYPE state, DWORD value);
};

class ScopedDeviceStateDX9 {
public:
  explicit ScopedDeviceStateDX9(DeviceStateDX9& state);
  ~ScopedDeviceStateDX9();
  ScopedDeviceStateDX9(const ScopedDeviceStateDX9&) = delete;
  ScopedDeviceStateDX9& operator=(const ScopedDeviceStateDX9&) = delete;

private:
  /* 0000 */ DeviceStateDX9* m_state;
  /* 0004 */ DWORD m_render[210];
  /* 034C */ DWORD m_textureStage[2][33];
  /* 0454 */ DWORD m_sampler[2][14];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DeviceStateDX9) == 1232);
static_assert(offsetof(DeviceStateDX9, device) == 0x0000);
static_assert(offsetof(DeviceStateDX9, fvf) == 0x0004);
static_assert(offsetof(DeviceStateDX9, render) == 0x0008);
static_assert(offsetof(DeviceStateDX9, textureStage) == 0x0350);
static_assert(offsetof(DeviceStateDX9, sampler) == 0x0458);
static_assert(offsetof(DeviceStateDX9, textures) == 0x04C8);
static_assert(sizeof(ScopedDeviceStateDX9) == 1220);
#endif

#endif
