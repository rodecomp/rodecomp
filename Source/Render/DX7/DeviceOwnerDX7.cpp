#include "Render/DX7/DeviceOwnerDX7.h"
#include "Client/Application/ApplicationWindow.h"
#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/Textures/Surface.h"

#include <cstring>

namespace {
  // INLINED
  HRESULT DeviceError(unsigned code) {
    return static_cast<HRESULT>(0x82000000u | code);
  }

  // OFFSET: 0x00548C00
  // FUNCTION: fn_01862
  DDSURFACEDESC2 SurfaceDescription(DWORD flags, DWORD caps) {
    DDSURFACEDESC2 result{};
    result.dwSize = sizeof(result);
    result.dwFlags = flags;
    result.ddsCaps.dwCaps = caps;
    result.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

    return result;
  }

  // OFFSET: 0x0054AA50
  // FUNCTION: fn_01900
  RECT ClientRectangle(HWND window) {
    RECT result;
    GetClientRect(window, &result);
    POINT point{result.left, result.top};
    ClientToScreen(window, &point);
    result.left = point.x - g_monitorRectangleDX7.left;
    result.top = point.y - g_monitorRectangleDX7.top;
    point = {result.right, result.bottom};
    ClientToScreen(window, &point);
    result.right = point.x - g_monitorRectangleDX7.left;
    result.bottom = point.y - g_monitorRectangleDX7.top;

    return result;
  }

  // OFFSET: 0x0054A970
  // FUNCTION: fn_01899
  HRESULT CALLBACK SelectDepthFormat(DDPIXELFORMAT* candidate, void* context) {
    auto* selected = static_cast<DDPIXELFORMAT*>(context);

    if (!candidate || !selected) {
      return D3DENUMRET_CANCEL;
    }

    if (candidate->dwFlags != selected->dwFlags || candidate->dwZBufferBitDepth <= selected->dwZBufferBitDepth) {
      return D3DENUMRET_OK;
    }

    *selected = *candidate;
    return candidate->dwZBufferBitDepth < 32 ? D3DENUMRET_OK : D3DENUMRET_CANCEL;
  }

  // INLINED
  void ChannelMask(DeviceOwnerDX7::Channel& channel, DWORD mask) {
    channel = {0, 8};

    if (!mask) {
      return;
    }

    while (!(mask & 1)) {
      ++channel.shift;
      mask >>= 1;
    }

    do {
      --channel.expansion;
      mask >>= 1;
    } while (mask);
  }

  // INLINED
  HRESULT SurfaceError(HRESULT status, unsigned code) {
    return status == DDERR_OUTOFVIDEOMEMORY ? status : DeviceError(code);
  }
}

// OFFSET: 0x005493B0
// FUNCTION: fn_01888
DeviceOwnerDX7::DeviceOwnerDX7()
    : window(nullptr), width(0), height(0), primary(nullptr), backBuffer(nullptr), renderTarget(nullptr), depthBuffer(nullptr), draw(nullptr),
      direct3D(nullptr), device(nullptr), depthMemory(0), fullscreen(0), presentationOption(0), captureSurface(nullptr), red{0, 8}, green{0, 8}, blue{0, 8},
      alpha{0, 8}, bitDepth(0), redMask(0), greenMask(0), blueMask(0), alphaMask(0), surfacesLost(false) {
}

// OFFSET: 0x005494B0
// FUNCTION: fn_01889
DeviceOwnerDX7::~DeviceOwnerDX7() {
}

// OFFSET: 0x005498A0
// FUNCTION: fn_01894
HRESULT DeviceOwnerDX7::CreateSurfaces(const DDSURFACEDESC2* mode, DWORD flags) {
  const bool buffered = (flags & 2) != 0;
  DDSURFACEDESC2 description = SurfaceDescription(DDSD_CAPS, 0);
  HRESULT status;

  if (flags & 1) {
    SetRect(&this->source, 0, 0, mode->dwWidth, mode->dwHeight);
    this->width = this->source.right;
    this->height = this->source.bottom;
    this->destination = this->source;
    const DWORD modeFlags = this->width == 320 && this->height == 200 && mode->ddpfPixelFormat.dwRGBBitCount == 8 ? DDSDM_STANDARDVGAMODE : 0;

    if (FAILED(this->draw->SetDisplayMode(this->width, this->height, mode->ddpfPixelFormat.dwRGBBitCount, mode->dwRefreshRate, modeFlags))) {
      return DeviceError(9);
    }

    description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;

    if (buffered) {
      description.dwFlags |= DDSD_BACKBUFFERCOUNT;
      description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
      description.dwBackBufferCount = 1;
    }

    status = this->draw->CreateSurface(&description, &this->primary, nullptr);

    if (FAILED(status)) {
      return SurfaceError(status, 7);
    }

    if (buffered) {
      DDSCAPS2 caps{DDSCAPS_BACKBUFFER};
      status = this->primary->GetAttachedSurface(&caps, &this->backBuffer);

      if (FAILED(status)) {
        return SurfaceError(status, 10);
      }
    }
  } else {
    GetClientRect(this->window, &this->source);
    this->width = this->source.right;
    this->height = this->source.bottom;
    this->destination = ClientRectangle(this->window);
    description.ddsCaps.dwCaps = buffered ? DDSCAPS_PRIMARYSURFACE : DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;
    status = this->draw->CreateSurface(&description, &this->primary, nullptr);

    if (FAILED(status)) {
      return SurfaceError(status, 7);
    }
  }

  IDirectDrawClipper* clipper;

  if (FAILED(this->draw->CreateClipper(0, &clipper, nullptr))) {
    return DeviceError(8);
  }

  clipper->SetHWnd(0, this->window);
  this->primary->SetClipper(clipper);

  if (clipper) {
    clipper->Release();
  }

  if (!(flags & 1)) {
    if (buffered) {
      description.dwWidth = this->width;
      description.dwHeight = this->height;
      description.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
      description.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
      status = this->draw->CreateSurface(&description, &this->backBuffer, nullptr);

      if (FAILED(status)) {
        return SurfaceError(status, 10);
      }
    } else {
      POINT topLeft{this->source.left, this->source.top};
      ClientToScreen(this->window, &topLeft);
      this->source.left = topLeft.x;
      this->source.top = topLeft.y;
      POINT bottomRight{this->source.right, this->source.bottom};
      ClientToScreen(this->window, &bottomRight);
      this->source.right = bottomRight.x;
      this->source.bottom = bottomRight.y;
    }
  }

  this->primaryDescription = SurfaceDescription(DDSD_CAPS, 0);
  this->primary->GetSurfaceDesc(&this->primaryDescription);
  const auto& format = this->primaryDescription.ddpfPixelFormat;
  this->bitDepth = format.dwRGBBitCount;
  this->redMask = format.dwRBitMask;
  this->greenMask = format.dwGBitMask;
  this->blueMask = format.dwBBitMask;
  this->alphaMask = format.dwRGBAlphaBitMask;
  ChannelMask(this->red, this->redMask);
  ChannelMask(this->green, this->greenMask);
  ChannelMask(this->blue, this->blueMask);
  ChannelMask(this->alpha, this->alphaMask);
  this->renderTarget = buffered ? this->backBuffer : this->primary;
  this->renderTarget->AddRef();
  description = SurfaceDescription(DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT, DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY);
  description.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
  description.dwWidth = this->width;
  description.dwHeight = this->height;
  status = this->draw->CreateSurface(&description, &this->captureSurface, nullptr);

  return FAILED(status) ? E_OUTOFMEMORY : S_OK;
}

// OFFSET: 0x0054A000
// FUNCTION: fn_01895
HRESULT DeviceOwnerDX7::CreateDevice(GUID* drawIdentity, const GUID* renderIdentity, const DDSURFACEDESC2* mode, DWORD flags) {
  this->depthMemory =
    std::memcmp(renderIdentity, &IID_IDirect3DHALDevice, sizeof(GUID)) == 0 || std::memcmp(renderIdentity, &IID_IDirect3DTnLHalDevice, sizeof(GUID)) == 0
      ? DDSCAPS_VIDEOMEMORY
      : DDSCAPS_SYSTEMMEMORY;

  if (FAILED(DirectDrawCreateEx(drawIdentity, reinterpret_cast<void**>(&this->draw), IID_IDirectDraw7, nullptr))) {
    return DeviceError(1);
  }

  auto description = SurfaceDescription(0, 0);
  this->draw->GetDisplayMode(&description);
  this->desktopBitDepth = description.ddpfPixelFormat.dwRGBBitCount;
  DWORD cooperative = this->fullscreen ? DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE : DDSCL_NORMAL;

  if (!(flags & 16)) {
    cooperative |= DDSCL_FPUSETUP;
  }

  if (FAILED(this->draw->SetCooperativeLevel(this->window, cooperative | DDSCL_MULTITHREADED))) {
    return DeviceError(2);
  }

  const HRESULT status = this->CreateSurfaces(mode, flags);

  if (FAILED(status)) {
    return status;
  }

  if (renderIdentity) {
    if (FAILED(this->draw->QueryInterface(IID_IDirect3D7, reinterpret_cast<void**>(&this->direct3D)))) {
      return DeviceError(3);
    }

    if (FAILED(this->direct3D->CreateDevice(*renderIdentity, this->renderTarget, &this->device))) {
      return DeviceError(4);
    }

    D3DVIEWPORT7 viewport{0, 0, this->width, this->height, 0.0f, 1.0f};

    if (FAILED(this->device->SetViewport(&viewport))) {
      return DeviceError(6);
    }

    this->depthFormat = {};
    this->depthFormat.dwFlags = DDPF_ZBUFFER | ((flags & 8) ? DDPF_STENCILBUFFER : 0);
    this->direct3D->EnumZBufferFormats(*renderIdentity, SelectDepthFormat, &this->depthFormat);

    if (this->depthFormat.dwSize != sizeof(this->depthFormat)) {
      return DeviceError(5);
    }

    this->device->GetCaps(&this->caps);

    if (!(this->caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR)) {
      description = SurfaceDescription(DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT, this->depthMemory | DDSCAPS_ZBUFFER);
      description.dwWidth = this->width;
      description.dwHeight = this->height;
      description.ddpfPixelFormat = this->depthFormat;
      const HRESULT depthStatus = this->draw->CreateSurface(&description, &this->depthBuffer, nullptr);

      if (FAILED(depthStatus)) {
        return SurfaceError(depthStatus, 5);
      }

      if (FAILED(this->renderTarget->AddAttachedSurface(this->depthBuffer)) || FAILED(this->device->SetRenderTarget(this->renderTarget, 0))) {
        return DeviceError(5);
      }
    }
  }

  return S_OK;
}

// OFFSET: 0x0054A7D0
// FUNCTION: fn_01897
HRESULT DeviceOwnerDX7::Close() {
  ULONG deviceReferences = 0;

  if (this->device) {
    deviceReferences = this->device->Release();

    if (static_cast<std::int32_t>(deviceReferences) > 0) {
      do {
        deviceReferences = this->device->Release();
      } while (deviceReferences);
    }
  }

  this->device = nullptr;

  if (!this->fullscreen && this->backBuffer) {
    this->backBuffer->Release();
    this->backBuffer = nullptr;
  }

  if (this->depthBuffer) {
    this->depthBuffer->Release();
    this->depthBuffer = nullptr;
  }

  if (this->renderTarget) {
    this->renderTarget->Release();
    this->renderTarget = nullptr;
  }

  if (this->primary) {
    this->primary->Release();
    this->primary = nullptr;
  }

  if (this->direct3D) {
    this->direct3D->Release();
    this->direct3D = nullptr;
  }

  if (this->captureSurface) {
    this->captureSurface->Release();
    this->captureSurface = nullptr;
  }

  ULONG drawReferences = 0;

  if (this->draw) {
    this->draw->SetCooperativeLevel(this->window, DDSCL_NORMAL);
    drawReferences = this->draw->Release();
  }

  this->draw = nullptr;
  return drawReferences || deviceReferences ? DeviceError(11) : S_OK;
}

// OFFSET: 0x0054A8B0
// FUNCTION: fn_01898
void DeviceOwnerDX7::DestroySurface(CSurface* surface) {
  delete surface;
}

// OFFSET: 0x0054AAD0
// FUNCTION: fn_01901
HRESULT DeviceOwnerDX7::Initialize(HWND value, GUID* drawIdentity, const GUID* renderIdentity, const DDSURFACEDESC2* mode, DWORD flags) {
  this->window = value;

  if (!value || !renderIdentity || (!mode && (flags & 1))) {
    return E_INVALIDARG;
  }

  this->fullscreen = flags & 1;

  if (this->fullscreen) {
    SetWindowLongA(hWnd, GWL_STYLE, static_cast<LONG>(WS_POPUP));
    ShowWindow(hWnd, SW_MAXIMIZE);
  }

  const HRESULT status = this->CreateDevice(drawIdentity, renderIdentity, mode, flags);

  if (FAILED(status)) {
    this->Close();

    if (status == E_FAIL) {
      return DeviceError(0);
    }
  }

  if (this->device) {
    const struct {
      D3DRENDERSTATETYPE state;
      DWORD value;
    } states[] = {
      {D3DRENDERSTATE_CULLMODE, D3DCULL_NONE},
      {D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL},
      {D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE},
      {D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL},
      {D3DRENDERSTATE_ALPHAREF, 240},
      {D3DRENDERSTATE_COLORKEYENABLE, TRUE},
      {D3DRENDERSTATE_ZENABLE, D3DZB_TRUE},
      {D3DRENDERSTATE_ZWRITEENABLE, TRUE},
      {D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA},
      {D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA}
    };

    for (const auto& state : states) {
      this->device->SetRenderState(state.state, state.value);
    }

    const struct {
      DWORD stage;
      D3DTEXTURESTAGESTATETYPE state;
      DWORD value;
    } stages[] = {{0, D3DTSS_TEXCOORDINDEX, 0},           {0, D3DTSS_COLOROP, D3DTOP_MODULATE},
                  {0, D3DTSS_COLORARG1, D3DTA_TEXTURE},   {0, D3DTSS_COLORARG2, D3DTA_DIFFUSE},
                  {0, D3DTSS_ALPHAOP, D3DTOP_MODULATE},   {0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE},
                  {0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE},   {1, D3DTSS_TEXCOORDINDEX, 1},
                  {1, D3DTSS_COLORARG1, D3DTA_TEXTURE},   {1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR},
                  {1, D3DTSS_COLORARG2, D3DTA_CURRENT},   {1, D3DTSS_ALPHAOP, D3DTOP_DISABLE},
                  {2, D3DTSS_COLOROP, D3DTOP_DISABLE},    {2, D3DTSS_ALPHAOP, D3DTOP_DISABLE},
                  {0, D3DTSS_MAGFILTER, D3DTFG_LINEAR},   {0, D3DTSS_MINFILTER, D3DTFN_LINEAR},
                  {1, D3DTSS_MAGFILTER, D3DTFG_LINEAR},   {1, D3DTSS_MINFILTER, D3DTFN_LINEAR},
                  {0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP}, {1, D3DTSS_ADDRESS, D3DTADDRESS_BORDER},
                  {1, D3DTSS_BORDERCOLOR, 0xFF000000}};

    for (const auto& state : stages) {
      this->device->SetTextureStageState(state.stage, state.state, state.value);
    }
  }

  return status;
}

// OFFSET: 0x005496D0
// FUNCTION: fn_01892
HRESULT DeviceOwnerDX7::Clear(DWORD color) {
  D3DRECT rectangle{0, 0, static_cast<LONG>(this->width), static_cast<LONG>(this->height)};
  return this->device->Clear(1, &rectangle, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

// OFFSET: 0x00549730
// FUNCTION: fn_01893
HRESULT DeviceOwnerDX7::ClearDepth() {
  D3DRECT rectangle{0, 0, static_cast<LONG>(this->width), static_cast<LONG>(this->height)};
  return this->device->Clear(1, &rectangle, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
}

// OFFSET: 0x0054AF10
// FUNCTION: fn_01903
void DeviceOwnerDX7::MarkSurfacesLost() {
  this->surfacesLost = true;
}

// OFFSET: 0x005495C0
// FUNCTION: fn_01891
bool DeviceOwnerDX7::RestoreLostSurfaces() {
  if (!this->surfacesLost) {
    return false;
  }

  const HRESULT primaryStatus = this->primary->IsLost();
  const HRESULT depthStatus = this->depthBuffer ? this->depthBuffer->IsLost() : S_OK;

  if (primaryStatus == DDERR_SURFACELOST && depthStatus == DDERR_SURFACELOST) {
    if (this->depthBuffer) {
      this->depthBuffer->Restore();
    }

    this->draw->RestoreAllSurfaces();
    this->surfacesLost = false;
  }

  return true;
}

// OFFSET: 0x0054AF20
// FUNCTION: fn_01904
HRESULT DeviceOwnerDX7::Present(bool wait) {
  if (!this->primary) {
    return DeviceError(14);
  }

  if (!this->backBuffer) {
    return this->primary->IsLost();
  }

  if (wait && (!this->fullscreen || this->presentationOption)) {
    this->destination = ClientRectangle(this->window);
    this->draw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, nullptr);
    return this->primary->Blt(&this->destination, this->backBuffer, &this->source, DDBLT_WAIT, nullptr);
  }

  GetClientRect(this->window, &this->destination);
  POINT topLeft{this->destination.left, this->destination.top};
  ClientToScreen(this->window, &topLeft);
  this->destination.left = topLeft.x;
  this->destination.top = topLeft.y;
  POINT bottomRight{this->destination.right, this->destination.bottom};
  ClientToScreen(this->window, &bottomRight);
  this->destination.right = bottomRight.x;
  this->destination.bottom = bottomRight.y;

  if (wait) {
    this->draw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, nullptr);
    return this->primary->Blt(nullptr, this->backBuffer, nullptr, DDBLT_WAIT, nullptr);
  }

  return this->primary->Blt(&this->destination, this->backBuffer, &this->source, DDBLT_WAIT, nullptr);
}

// OFFSET: 0x005494C0
// FUNCTION: fn_01890
void DeviceOwnerDX7::AdjustTextureDimensions(std::uint32_t& width, std::uint32_t& height) const {
  std::uint32_t selectedWidth = 1;

  while (selectedWidth < width) {
    selectedWidth *= 2;
  }

  if (selectedWidth < this->minTextureWidth) {
    selectedWidth = this->minTextureWidth;
  }

  if (selectedWidth > this->maxTextureWidth) {
    selectedWidth = this->maxTextureWidth;
  }

  std::uint32_t selectedHeight = 1;

  while (selectedHeight < height) {
    selectedHeight *= 2;
  }

  if (selectedHeight < this->minTextureHeight) {
    selectedHeight = this->minTextureHeight;
  }

  if (selectedHeight > this->maxTextureHeight) {
    selectedHeight = this->maxTextureHeight;
  }

  const auto aspect = this->maxTextureAspectRatio;

  if (aspect) {
    if (selectedWidth / selectedHeight > aspect) {
      selectedHeight = selectedWidth / aspect;
    }

    if (selectedHeight / selectedWidth > aspect) {
      selectedWidth = selectedHeight / aspect;
    }
  }

  width = selectedWidth;
  height = selectedHeight;
}

// OFFSET: 0x0054AE50
// FUNCTION: fn_01902
bool DeviceOwnerDX7::SupportsTextureDimensions(std::uint32_t width, std::uint32_t height) const {
  return this->minTextureWidth <= width && width <= this->maxTextureWidth && this->minTextureHeight <= height && height <= this->maxTextureHeight;
}

// OFFSET: 0x0056B890
// FUNCTION: fn_02324
void DeviceOwnerDX7::PrepareErrorDialog() {
  if (this->direct3D) {
    this->direct3D->EvictManagedTextures();
  }
}

// OFFSET: 0x0056B8B0
// FUNCTION: fn_02325
void DeviceOwnerDX7::RestoreDisplayMode() {
  if (this->draw) {
    if (this->backBuffer) {
      this->backBuffer->Unlock(nullptr);
    }

    if (this->primary) {
      this->primary->Unlock(nullptr);
    }

    this->draw->FlipToGDISurface();
  }
}

// OFFSET: 0x0056BB90
// FUNCTION: fn_02327
IDirect3D7* DeviceOwnerDX7::Direct3D() {
  return this->direct3D;
}

// OFFSET: 0x0056C4B0
// FUNCTION: fn_02336
HRESULT DeviceOwnerDX7::RestoreAllSurfaces() {
  return this->draw->RestoreAllSurfaces();
}

// OFFSET: 0x0054A8D0
// FUNCTION: fn_45420
void DeviceOwnerDX7::SetDialogClipper(int enabled) {
  if (!this->fullscreen) {
    return;
  }

  IDirectDrawClipper* clipper = nullptr;

  if (enabled == 1) {
    this->draw->CreateClipper(0, &clipper, nullptr);
    clipper->SetHWnd(0, this->window);
  }

  this->primary->SetClipper(clipper);

  if (clipper) {
    clipper->Release();
  }
}
