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

  /* 0000 */ HWND window;
  /* 0004 */ DWORD width;
  /* 0008 */ DWORD height;
  /* 000C */ RECT destination;
  /* 001C */ RECT source;
  /* 002C */ IDirectDrawSurface7* primary;
  /* 0030 */ IDirectDrawSurface7* backBuffer;
  /* 0034 */ IDirectDrawSurface7* renderTarget;
  /* 0038 */ IDirectDrawSurface7* depthBuffer;
  /* 003C */ IDirectDraw7* draw;
  /* 0040 */ IDirect3D7* direct3D;
  /* 0044 */ IDirect3DDevice7* device;
  /* 0048 */ D3DDEVICEDESC7 caps;
  /* 0134 */ DWORD depthMemory;
  /* 0138 */ DDPIXELFORMAT depthFormat;
  /* 0158 */ DDSURFACEDESC2 primaryDescription;
  /* 01D4 */ DWORD fullscreen;
  /* 01D8 */ DWORD presentationOption;
  /* 01DC */ DWORD desktopBitDepth;
  /* 01E0 */ IDirectDrawSurface7* captureSurface;
  struct Channel {
    /* 0000 */ DWORD shift;
    /* 0004 */ DWORD expansion;
  };
  /* 01E4 */ Channel red;
  /* 01EC */ Channel green;
  /* 01F4 */ Channel blue;
  /* 01FC */ Channel alpha;
  /* 0204 */ DWORD bitDepth;
  /* 0208 */ DWORD redMask;
  /* 020C */ DWORD greenMask;
  /* 0210 */ DWORD blueMask;
  /* 0214 */ DWORD alphaMask;
  /* 0218 */ DWORD minTextureWidth;
  /* 021C */ DWORD minTextureHeight;
  /* 0220 */ DWORD maxTextureWidth;
  /* 0224 */ DWORD maxTextureHeight;
  /* 0228 */ DWORD maxTextureAspectRatio;
  /* 022C */ bool colorKeySupported;
  /* 022D */ bool largeTextures;
  /* 022E */ std::uint8_t unknown_022E[26];
  /* 0248 */ bool surfacesLost;
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
static_assert(sizeof(DeviceOwnerDX7::Channel) == 8);
static_assert(offsetof(DeviceOwnerDX7::Channel, expansion) == 4);
static_assert(offsetof(DeviceOwnerDX7, red) == 0x01E4);
static_assert(offsetof(DeviceOwnerDX7, alpha) == 0x01FC);
#endif

#endif
