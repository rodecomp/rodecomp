#include "Render/DX7/DeviceOwnerDX7.h"
#include "Client/Application/ApplicationWindow.h"
#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/TextureAllocationDX7.h"
#include "Render/Textures/Surface.h"
#include "Render/Renderer.h"
#include "UI/GameSettings/GameSettingsUI.h"

#include <cstdio>
#include <cstring>

namespace {
  // INLINED
  HRESULT DeviceError(unsigned code) {
    return static_cast<HRESULT>(0x82000000u | code);
  }

  __declspec(noinline) void SurfaceDescription(DDSURFACEDESC2* result, DWORD flags, DWORD caps);

  // OFFSET: 0x00548C00
  // FUNCTION: fn_01862
  void SurfaceDescription(DDSURFACEDESC2* result, DWORD flags, DWORD caps) {
    std::memset(result, 0, sizeof(*result));
    result->dwSize = sizeof(*result);
    result->dwFlags = flags;
    result->ddsCaps.dwCaps = caps;
    result->ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
  }

  // OFFSET: 0x0054AA50
  // FUNCTION: fn_01900
  RECT ClientRectangle(HWND window) {
    RECT result;
    GetClientRect(window, &result);
    POINT topLeft{result.left, result.top};
    ClientToScreen(window, &topLeft);
    result.left = topLeft.x - g_monitorRectangleDX7.left;
    result.top = topLeft.y - g_monitorRectangleDX7.top;
    POINT bottomRight{result.right, result.bottom};
    ClientToScreen(window, &bottomRight);
    result.right = bottomRight.x - g_monitorRectangleDX7.left;
    result.bottom = bottomRight.y - g_monitorRectangleDX7.top;

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
    if (candidate->dwZBufferBitDepth >= 32) {
      return D3DENUMRET_CANCEL;
    }

    return D3DENUMRET_OK;
  }

  // INLINED
  __forceinline void ChannelMask(DeviceOwnerDX7::Channel& channel, DWORD mask) {
    while (mask) {
      if (mask & 1) {
        DWORD expansion = 8;

        do {
          --expansion;
          mask >>= 1;
        } while (mask);

        channel.expansion = expansion;
        return;
      }

      ++channel.shift;
      mask >>= 1;
    }
  }

  // INLINED
  HRESULT SurfaceError(HRESULT status, unsigned code) {
    return status == DDERR_OUTOFVIDEOMEMORY ? status : DeviceError(code);
  }
}

// OFFSET: 0x00548D90
// FUNCTION: fn_46358
HRESULT DebugStringDX7(const char* file, long line, HRESULT result, const char* message) {
  char buffer[256];
  std::sprintf(buffer, "%s(%ld): ", file, line);
  OutputDebugStringA(buffer);
  OutputDebugStringA(message);

  if (result != S_OK) {
    std::sprintf(buffer, "(hr=%08lx)\n", static_cast<unsigned long>(result));
    OutputDebugStringA(buffer);
  }

  OutputDebugStringA("\n");
  return result;
}

// OFFSET: 0x005493B0
// FUNCTION: fn_01888
DeviceOwnerDX7::DeviceOwnerDX7() {
  this->window = nullptr;
  this->width = 0;
  this->height = 0;
  this->device = nullptr;
  this->direct3D = nullptr;
  this->renderTarget = nullptr;
  this->depthMemory = 0;
  this->fullscreen = 0;
  this->presentationOption = 0;
  this->red = {0, 8};
  this->green = {0, 8};
  this->blue = {0, 8};
  this->alpha = {0, 8};
  this->bitDepth = 0;
  this->redMask = 0;
  this->greenMask = 0;
  this->blueMask = 0;
  this->alphaMask = 0;
  this->primary = nullptr;
  this->backBuffer = nullptr;
  this->depthBuffer = nullptr;
  this->draw = nullptr;
  this->captureSurface = nullptr;
  this->surfacesLost = false;
}

// OFFSET: 0x005494B0
// FUNCTION: fn_01889
DeviceOwnerDX7::~DeviceOwnerDX7() {
}

// OFFSET: 0x005498A0
// FUNCTION: fn_01894
HRESULT DeviceOwnerDX7::CreateSurfaces(const DDSURFACEDESC2* mode, DWORD flags) {
  DDSURFACEDESC2 description;
  HRESULT status;

  if (flags & 1) {
    SetRect(&this->source, 0, 0, mode->dwWidth, mode->dwHeight);
    this->width = this->source.right;
    this->height = this->source.bottom;
    this->destination = this->source;
    DWORD modeFlags = 0;

    if (this->width == 320 && this->height == 200 && mode->ddpfPixelFormat.dwRGBBitCount == 8) {
      modeFlags = DDSDM_STANDARDVGAMODE;
    }

    if (FAILED(this->draw->SetDisplayMode(this->width, this->height, mode->ddpfPixelFormat.dwRGBBitCount, mode->dwRefreshRate, modeFlags))) {
      return DeviceError(9);
    }

    SurfaceDescription(&description, DDSD_CAPS, 0);
    description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;

    if (flags & 2) {
      description.dwFlags |= DDSD_BACKBUFFERCOUNT;
      description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
      description.dwBackBufferCount = 1;
    }

    status = this->draw->CreateSurface(&description, &this->primary, nullptr);

    if (FAILED(status)) {
      if (status != DDERR_OUTOFVIDEOMEMORY) {
        return DeviceError(7);
      }
      return DDERR_OUTOFVIDEOMEMORY;
    }

    if (flags & 2) {
      DDSCAPS2 caps = {DDSCAPS_BACKBUFFER, 0, 0, 0};
      status = this->primary->GetAttachedSurface(&caps, &this->backBuffer);

      if (FAILED(status)) {
        if (status != DDERR_OUTOFVIDEOMEMORY) {
          return DeviceError(10);
        }
        return DDERR_OUTOFVIDEOMEMORY;
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
      clipper = nullptr;
    }
  } else {
    GetClientRect(this->window, &this->source);
    this->width = this->source.right;
    this->height = this->source.bottom;
    this->destination = ClientRectangle(this->window);
    SurfaceDescription(&description, DDSD_CAPS, 0);
    description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    if (!(flags & 2)) {
      description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;
    }
    status = this->draw->CreateSurface(&description, &this->primary, nullptr);

    if (FAILED(status)) {
      if (status != DDERR_OUTOFVIDEOMEMORY) {
        return DeviceError(7);
      }
      return DDERR_OUTOFVIDEOMEMORY;
    }

    IDirectDrawClipper* clipper;

    if (FAILED(this->draw->CreateClipper(0, &clipper, nullptr))) {
      return DeviceError(8);
    }

    clipper->SetHWnd(0, this->window);
    this->primary->SetClipper(clipper);

    if (clipper) {
      clipper->Release();
      clipper = nullptr;
    }

    if (flags & 2) {
      description.dwWidth = this->width;
      description.dwHeight = this->height;
      description.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
      description.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
      status = this->draw->CreateSurface(&description, &this->backBuffer, nullptr);

      if (FAILED(status)) {
        if (status != DDERR_OUTOFVIDEOMEMORY) {
          return DeviceError(10);
        }
        return DDERR_OUTOFVIDEOMEMORY;
      }
    } else {
      ClientToScreen(this->window, reinterpret_cast<POINT*>(&this->source.left));
      ClientToScreen(this->window, reinterpret_cast<POINT*>(&this->source.right));
    }
  }

  SurfaceDescription(&this->primaryDescription, DDSD_CAPS, 0);
  this->primary->GetSurfaceDesc(&this->primaryDescription);
  this->bitDepth = this->primaryDescription.ddpfPixelFormat.dwRGBBitCount;
  this->redMask = this->primaryDescription.ddpfPixelFormat.dwRBitMask;
  this->greenMask = this->primaryDescription.ddpfPixelFormat.dwGBitMask;
  this->blueMask = this->primaryDescription.ddpfPixelFormat.dwBBitMask;
  this->alphaMask = this->primaryDescription.ddpfPixelFormat.dwRGBAlphaBitMask;
  this->red = {0, 8};
  this->green = {0, 8};
  this->blue = {0, 8};
  this->alpha = {0, 8};
  ChannelMask(this->red, this->primaryDescription.ddpfPixelFormat.dwRBitMask);
  ChannelMask(this->green, this->primaryDescription.ddpfPixelFormat.dwGBitMask);
  ChannelMask(this->blue, this->primaryDescription.ddpfPixelFormat.dwBBitMask);
  ChannelMask(this->alpha, this->primaryDescription.ddpfPixelFormat.dwRGBAlphaBitMask);
  if (flags & 2) {
    this->renderTarget = this->backBuffer;
  } else {
    this->renderTarget = this->primary;
  }
  this->renderTarget->AddRef();
  SurfaceDescription(&description, DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT, 0);
  description.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
  description.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  description.dwWidth = this->width;
  description.dwHeight = this->height;
  status = this->draw->CreateSurface(&description, &this->captureSurface, nullptr);

  return FAILED(status) ? E_OUTOFMEMORY : S_OK;
}

// INLINED
static HRESULT CreateDirectDraw(DeviceOwnerDX7* owner, GUID* drawIdentity, DWORD flags) {
  if (FAILED(DirectDrawCreateEx(drawIdentity, reinterpret_cast<void**>(&owner->draw), IID_IDirectDraw7, nullptr))) {
    return DeviceError(1);
  }

  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);
  owner->draw->GetDisplayMode(&description);
  owner->desktopBitDepth = description.ddpfPixelFormat.dwRGBBitCount;
  DWORD cooperative = DDSCL_NORMAL;

  if (owner->fullscreen) {
    cooperative = DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE;
  }

  if (0 == (flags & 16)) {
    cooperative |= DDSCL_FPUSETUP;
  }

  if (FAILED(owner->draw->SetCooperativeLevel(owner->window, cooperative | DDSCL_MULTITHREADED))) {
    return DeviceError(2);
  }

  return S_OK;
}

// INLINED
static HRESULT CreateDirect3D(DeviceOwnerDX7* owner, const GUID* renderIdentity) {
  if (FAILED(owner->draw->QueryInterface(IID_IDirect3D7, reinterpret_cast<void**>(&owner->direct3D)))) {
    return DeviceError(3);
  }

  if (FAILED(owner->direct3D->CreateDevice(*renderIdentity, owner->renderTarget, &owner->device))) {
    return DeviceError(4);
  }

  D3DVIEWPORT7 viewport{};
  viewport.dwWidth = owner->width;
  viewport.dwHeight = owner->height;
  viewport.dvMaxZ = 1.0f;

  if (FAILED(owner->device->SetViewport(&viewport))) {
    return DeviceError(6);
  }

  return S_OK;
}

// INLINED
static HRESULT CreateDepth(DeviceOwnerDX7* owner, const GUID* renderIdentity, DWORD flags) {
  std::memset(&owner->depthFormat, 0, sizeof(owner->depthFormat));
  if (flags & 8) {
    owner->depthFormat.dwFlags = DDPF_ZBUFFER | DDPF_STENCILBUFFER;
  } else {
    owner->depthFormat.dwFlags = DDPF_ZBUFFER;
  }
  owner->direct3D->EnumZBufferFormats(*renderIdentity, SelectDepthFormat, &owner->depthFormat);

  if (owner->depthFormat.dwSize != sizeof(owner->depthFormat)) {
    return DeviceError(5);
  }

  owner->device->GetCaps(&owner->caps);

  if (!(owner->caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR)) {
    DDSURFACEDESC2 description;
    SurfaceDescription(&description, DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT, 0);
    description.ddsCaps.dwCaps = owner->depthMemory | DDSCAPS_ZBUFFER;
    description.dwWidth = owner->width;
    description.dwHeight = owner->height;
    description.ddpfPixelFormat = owner->depthFormat;
    const HRESULT status = owner->draw->CreateSurface(&description, &owner->depthBuffer, nullptr);

    if (FAILED(status)) {
      if (status != DDERR_OUTOFVIDEOMEMORY) {
        return DeviceError(5);
      }

      return DDERR_OUTOFVIDEOMEMORY;
    }

    if (FAILED(owner->renderTarget->AddAttachedSurface(owner->depthBuffer))) {
      return DeviceError(5);
    }

    if (FAILED(owner->device->SetRenderTarget(owner->renderTarget, 0))) {
      return DeviceError(5);
    }
  }

  return S_OK;
}

// OFFSET: 0x0054A000
// FUNCTION: fn_01895
HRESULT DeviceOwnerDX7::CreateDevice(GUID* drawIdentity, const GUID* renderIdentity, const DDSURFACEDESC2* mode, DWORD flags) {
  if (IsEqualIID(*renderIdentity, IID_IDirect3DHALDevice)) {
    this->depthMemory = DDSCAPS_VIDEOMEMORY;
  } else if (IsEqualIID(*renderIdentity, IID_IDirect3DTnLHalDevice)) {
    this->depthMemory = DDSCAPS_VIDEOMEMORY;
  } else {
    this->depthMemory = DDSCAPS_SYSTEMMEMORY;
  }

  HRESULT status = CreateDirectDraw(this, drawIdentity, flags);

  if (FAILED(status)) {
    return status;
  }

  status = this->CreateSurfaces(mode, flags);

  if (FAILED(status)) {
    return status;
  }

  if (!renderIdentity) {
    return S_OK;
  }

  status = CreateDirect3D(this, renderIdentity);

  if (FAILED(status)) {
    return status;
  }

  return CreateDepth(this, renderIdentity, flags);
}

// OFFSET: 0x0054A7D0
// FUNCTION: fn_01897
HRESULT DeviceOwnerDX7::Close() {
  ULONG drawReferences = 0;
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

  if (this->draw) {
    this->draw->SetCooperativeLevel(this->window, DDSCL_NORMAL);
    drawReferences = this->draw->Release();
  }

  this->draw = nullptr;
  return drawReferences || deviceReferences ? DeviceError(11) : S_OK;
}

// OFFSET: 0x0054A8B0
// FUNCTION: fn_01898
void DeviceOwnerDX7::DestroyRenderer(CRenderer* renderer) {
  delete renderer;
}

// OFFSET: 0x0054AAD0
// FUNCTION: fn_01901
HRESULT DeviceOwnerDX7::Initialize(HWND value, GUID* drawIdentity, const GUID* renderIdentity, const DDSURFACEDESC2* mode, DWORD flags) {
  this->window = value;

  if (!value || !renderIdentity) {
    return E_INVALIDARG;
  }

  if (!mode && static_cast<bool>(flags & 1)) {
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
    this->device->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    this->device->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
    this->device->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE);

    this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 240);
    this->device->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE);

    this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
    this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);

    this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
    this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

    this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    this->device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
    this->device->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
    this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
    this->device->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTFN_LINEAR);
    this->device->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
    this->device->SetTextureStageState(1, D3DTSS_ADDRESS, D3DTADDRESS_BORDER);
    this->device->SetTextureStageState(1, D3DTSS_BORDERCOLOR, 0xFF000000);
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

// OFFSET: 0x00549640
// FUNCTION: fn_45922
HRESULT DeviceOwnerDX7::RestoreSurfaces() {
  IDirectDrawSurface7* surfaces[] = {this->primary, this->backBuffer, this->depthBuffer};
  HRESULT status = S_OK;

  for (unsigned index = 0; index < 3; ++index) {
    IDirectDrawSurface7* surface = surfaces[index];

    if (surface) {
      status = surface->IsLost();

      if (status != S_OK) {
        surface->Restore();
      }
    }

    if (status == DDERR_SURFACELOST) {
      this->draw->RestoreAllSurfaces();
      return DDERR_SURFACELOST;
    }
  }

  return status;
}

// OFFSET: 0x00549790
// FUNCTION: fn_45923
HRESULT DeviceOwnerDX7::BlitBackBuffer() {
  if (!this->primary) {
    return DeviceError(14);
  }

  if (!this->backBuffer) {
    return this->primary->IsLost();
  }

  if (this->fullscreen) {
    return this->backBuffer->Blt(nullptr, this->primary, nullptr, DDBLT_WAIT, nullptr);
  }

  this->destination = ClientRectangle(this->window);
  return this->backBuffer->Blt(&this->source, this->primary, &this->destination, DDBLT_WAIT, nullptr);
}

// OFFSET: 0x0054A650
// FUNCTION: fn_45924
HRESULT DeviceOwnerDX7::CreateDepthBuffer() {
  this->device->GetCaps(&this->caps);

  if (!(this->caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR)) {
    DDSURFACEDESC2 description;
    SurfaceDescription(&description, DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT, 0);
    description.ddsCaps.dwCaps = this->depthMemory | DDSCAPS_ZBUFFER;
    description.dwWidth = this->width;
    description.dwHeight = this->height;
    description.ddpfPixelFormat = this->depthFormat;
    const HRESULT status = this->draw->CreateSurface(&description, &this->depthBuffer, nullptr);

    if (FAILED(status)) {
      if (status == DDERR_OUTOFVIDEOMEMORY) {
        return status;
      }
    } else if (SUCCEEDED(this->renderTarget->AddAttachedSurface(this->depthBuffer)) && SUCCEEDED(this->device->SetRenderTarget(this->renderTarget, 0))) {
      return S_OK;
    }
    return DeviceError(5);
  }

  return S_OK;
}

// OFFSET: 0x0054AF10
// FUNCTION: fn_01903
void DeviceOwnerDX7::MarkSurfacesLost() {
  this->surfacesLost = true;
}

// OFFSET: 0x005495C0
// FUNCTION: fn_01891
int DeviceOwnerDX7::RestoreLostSurfaces() {
  if (this->surfacesLost) {
    HRESULT status[2] = {};
    status[0] = this->primary->IsLost();
    if (this->depthBuffer) {
      status[1] = this->depthBuffer->IsLost();
    }

    if (status[0] == DDERR_SURFACELOST && status[1] == DDERR_SURFACELOST) {
      if (this->depthBuffer) {
        this->depthBuffer->Restore();
      }

      this->draw->RestoreAllSurfaces();
      this->surfacesLost = false;
    }

    return true;
  }

  return false;
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

  if (wait) {
    if (this->fullscreen && !this->presentationOption) {
      GetClientRect(this->window, &this->destination);
      ClientToScreen(this->window, reinterpret_cast<POINT*>(&this->destination.left));
      ClientToScreen(this->window, reinterpret_cast<POINT*>(&this->destination.right));
      this->draw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, nullptr);
      return this->primary->Blt(nullptr, this->backBuffer, nullptr, DDBLT_WAIT, nullptr);
    }

    this->destination = ClientRectangle(this->window);
    this->draw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, nullptr);
    return this->primary->Blt(&this->destination, this->backBuffer, &this->source, DDBLT_WAIT, nullptr);
  }

  GetClientRect(this->window, &this->destination);
  ClientToScreen(this->window, reinterpret_cast<POINT*>(&this->destination.left));
  ClientToScreen(this->window, reinterpret_cast<POINT*>(&this->destination.right));
  return this->primary->Blt(&this->destination, this->backBuffer, &this->source, DDBLT_WAIT, nullptr);
}

// OFFSET: 0x005494C0
// FUNCTION: fn_01890
void DeviceOwnerDX7::AdjustTextureDimensions(std::uint32_t& width, std::uint32_t& height) const {
  DWORD selectedWidth = 1;

  while (selectedWidth < width) {
    selectedWidth *= 2;
  }

  selectedWidth = std::max(selectedWidth, this->minTextureWidth);
  selectedWidth = std::min(selectedWidth, this->maxTextureWidth);

  DWORD selectedHeight = 1;

  while (selectedHeight < height) {
    selectedHeight *= 2;
  }

  selectedHeight = std::max(selectedHeight, this->minTextureHeight);
  selectedHeight = std::min(selectedHeight, this->maxTextureHeight);

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
void CRenderSystemDX7::RestoreDisplayMode() {
  this->m_device->RestoreDisplayMode();
}

// OFFSET: 0x0054AA10
// FUNCTION: fn_45502
void DeviceOwnerDX7::PrepareErrorDialog() {
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

// OFFSET: 0x0056B8B0
// FUNCTION: fn_02325
void CRenderSystemDX7::PrepareErrorDialog() {
  this->m_device->PrepareErrorDialog();
}

// OFFSET: 0x0056BB90
// FUNCTION: fn_02327
IDirect3D7* DeviceOwnerDX7::Direct3D() {
  return this->direct3D;
}

// OFFSET: 0x0054AF00
// FUNCTION: fn_45503
HRESULT DeviceOwnerDX7::RestoreAllSurfaces() {
  return this->draw->RestoreAllSurfaces();
}

// OFFSET: 0x0056C4B0
// FUNCTION: fn_02336
HRESULT CRenderSystemDX7::TestDevice() {
  return this->m_device->RestoreAllSurfaces();
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

// OFFSET: 0x0054A930
// FUNCTION: fn_45501
void DeviceOwnerDX7::UpdateMipmapFiltering() {
  if (g_textureFiltering != 0 && (this->caps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR)) {
    g_textureAllocationDX7.descriptorFlags = DDSD_MIPMAPCOUNT;
    g_textureAllocationDX7.surfaceCaps = DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
  } else {
    g_textureAllocationDX7.descriptorFlags = 0;
    g_textureAllocationDX7.surfaceCaps = 0;
  }
}
