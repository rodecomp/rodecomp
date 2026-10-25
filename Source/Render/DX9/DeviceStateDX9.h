#ifndef RAGNAROK_SOURCE_RENDER_DX9_DEVICESTATEDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_DEVICESTATEDX9_H

#include <d3d9.h>

struct DeviceStateDX9 {
  IDirect3DDevice9* device;
  DWORD fvf;
  DWORD render[210];
  DWORD textureStage[2][33];
  DWORD sampler[2][14];
  IDirect3DBaseTexture9* textures[2];

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
  DeviceStateDX9* m_state;
  DWORD m_render[210];
  DWORD m_textureStage[2][33];
  DWORD m_sampler[2][14];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DeviceStateDX9) == 1232);
static_assert(sizeof(ScopedDeviceStateDX9) == 1220);
#endif

#endif
