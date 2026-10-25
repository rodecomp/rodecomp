#ifndef RAGNAROK_SOURCE_RENDER_DX7_DEVICEOWNERDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_DEVICEOWNERDX7_H

#include <cstddef>
#include <cstdint>
#include <d3d.h>
#include <ddraw.h>

struct DeviceOptionsDX7 {
  const int& fullscreen;
  const int& width;
  const int& height;
  const int& bitsPerPixel;
};

struct TextureDX7Context;
struct TextureAllocationDX7;
class CRenderSystemBase;

class DeviceOwnerDX7 {
public:
  DeviceOwnerDX7();
  HRESULT
  Initialize(HWND window, GUID* drawIdentity, const GUID* renderIdentity, const DDSURFACEDESC2* mode, DWORD flags, HWND applicationWindow, const RECT& monitor);
  HRESULT CreateDevice(GUID* drawIdentity, const GUID* renderIdentity, const DDSURFACEDESC2* mode, DWORD flags, const RECT& monitor);
  HRESULT CreateSurfaces(const DDSURFACEDESC2* mode, DWORD flags, const RECT& monitor);
  HRESULT Close();
  HRESULT Clear(DWORD color);
  HRESULT ClearDepth();
  HRESULT Present(bool wait, const RECT& monitor);
  bool RestoreLostSurfaces();
  void MarkSurfacesLost();
  void AdjustTextureDimensions(std::uint32_t& width, std::uint32_t& height) const;
  bool SupportsTextureDimensions(std::uint32_t width, std::uint32_t height) const;
  void CaptureBackBuffer();
  void RestoreBackBuffer();
  void PrepareErrorDialog();
  void RestoreDisplayMode();
  HRESULT RestoreAllSurfaces();
  void SetDialogClipper(int enabled);
  bool CalibrateTextureCoordinates(TextureDX7Context& context);

  int RefreshRendererCapabilities(int mipmaps, TextureAllocationDX7& allocation, bool& multitexture, CRenderSystemBase* renderSystem);
  std::uint16_t PackColor(int red, int green, int blue);
  DWORD BlueMask();
  DWORD BlueExpansion();
  DWORD BlueShift();
  IDirectDraw7* DirectDraw();
  DWORD GreenMask();
  DWORD GreenExpansion();
  DWORD GreenShift();
  DWORD RedMask();
  DWORD RedExpansion();
  DWORD RedShift();
  IDirectDrawSurface7* RenderTarget();

  HWND window;
  DWORD width;
  DWORD height;
  RECT destination;
  RECT source;
  IDirectDrawSurface7* primary;
  IDirectDrawSurface7* backBuffer;
  IDirectDrawSurface7* renderTarget;
  IDirectDrawSurface7* depthBuffer;
  IDirectDraw7* draw;
  IDirect3D7* direct3D;
  IDirect3DDevice7* device;
  D3DDEVICEDESC7 caps;
  DWORD depthMemory;
  DDPIXELFORMAT depthFormat;
  DDSURFACEDESC2 primaryDescription;
  DWORD fullscreen;
  DWORD presentationOption;
  DWORD desktopBitDepth;
  IDirectDrawSurface7* captureSurface;
  struct Channel {
    DWORD shift;
    DWORD expansion;
  } red, green, blue, alpha;
  DWORD bitDepth;
  DWORD redMask;
  DWORD greenMask;
  DWORD blueMask;
  DWORD alphaMask;
  DWORD minTextureWidth;
  DWORD minTextureHeight;
  DWORD maxTextureWidth;
  DWORD maxTextureHeight;
  DWORD maxTextureAspectRatio;
  bool colorKeySupported;
  bool largeTextures;
  std::uint8_t unknown_022E[26];
  bool surfacesLost;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DeviceOwnerDX7) == 588);
static_assert(offsetof(DeviceOwnerDX7, caps) == 72);
static_assert(offsetof(DeviceOwnerDX7, depthFormat) == 312);
static_assert(offsetof(DeviceOwnerDX7, primaryDescription) == 344);
static_assert(offsetof(DeviceOwnerDX7, fullscreen) == 468);
static_assert(offsetof(DeviceOwnerDX7, captureSurface) == 480);
static_assert(offsetof(DeviceOwnerDX7, minTextureWidth) == 536);
static_assert(offsetof(DeviceOwnerDX7, surfacesLost) == 584);
#endif

#endif
