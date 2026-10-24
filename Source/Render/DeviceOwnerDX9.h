#ifndef RAGNAROK_SOURCE_RENDER_DEVICEOWNERDX9_H
#define RAGNAROK_SOURCE_RENDER_DEVICEOWNERDX9_H

#include "Render/DeviceStateDX9.h"
#include "Render/SurfaceDX9.h"
#include "Render/TextureDX9.h"

// Original renderer configuration globals, supplied by their configuration owner.
struct DeviceOptionsDX9 {
  const int& fullscreen;
  const int& width;
  const int& height;
  const int& dynamicTextures;
};

// Original 0x690-byte DX9 device owner, distinct from CRenderSystemDX9. Context
// views borrow this owner and become invalid at Close. No original-code calls.
class DeviceOwnerDX9 {
public:
  DeviceOwnerDX9();
  DeviceOwnerDX9(const DeviceOwnerDX9&) = delete;
  DeviceOwnerDX9& operator=(const DeviceOwnerDX9&) = delete;
  HRESULT Initialize(HWND window, UINT adapter, const DeviceOptionsDX9& options);
  HRESULT CreateDevice(UINT adapter, const DeviceOptionsDX9& options);
  HRESULT AcquireBackBuffer();
  // Returns output after power-of-two rounding, caps clamping, and integer
  // aspect-ratio adjustment. Zero/overflowed requests round to one.
  UINT* AdjustTextureDimensions(UINT* output, UINT width, UINT height) const;
  HRESULT Close();
  HRESULT Present(int unused);
  HRESULT ClipFullscreenCursor();
  HRESULT CheckState(class CRenderSystemBase* renderSystem);
  bool Begin(CRenderSystemBase* renderSystem);
  void End();
  bool CaptureBackBuffer();
  bool RestoreBackBuffer();
  SurfaceDX9Context SurfaceContext() const;
  TextureDX9Context TextureContext(
    D3DX9Runtime& d3dx,
    class RenderPrimitivePool& primitives,
    RenderQueues& queues,
    bool depthAlpha,
    float depthStart,
    float depthEnd
  ) const;

  /* 0000 */ HWND window;
  /* 0004 */ IDirect3D9Ex* direct3D;
  /* 0008 */ IDirect3DDevice9Ex* device;
  /* 000C */ DeviceStateDX9 state;
  /* 04DC */ UINT adapter;
  /* 04E0 */ D3DPRESENT_PARAMETERS presentation;
  /* 0518 */ D3DDISPLAYMODEEX displayMode;
  /* 0530 */ D3DSURFACE_DESC backBufferDescription;
  /* 0550 */ D3DCAPS9 caps;
  /* 0680 */ IDirect3DSurface9* backBuffer;
  /* 0684 */ IDirect3DSurface9* capturedBackBuffer;
  /* 0688 */ bool dynamicTextures;
  unsigned char unknown_0689[3];
  /* 068C */ HRESULT lastPresent;

private:
  bool SelectDisplayMode(D3DFORMAT format, UINT adapter, UINT width, UINT height, D3DSCANLINEORDERING ordering);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DeviceOwnerDX9) == 0x690);
#endif

#endif
