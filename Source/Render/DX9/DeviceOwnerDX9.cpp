#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "UI/GameSettings/GameSettingsUI.h"

#include <algorithm>
#include <cstring>

namespace {
  constexpr HRESULT invalidDevice = static_cast<HRESULT>(0x8200000Eu);
  constexpr HRESULT deviceError = static_cast<HRESULT>(0x82000000u);
}

struct DisplayModeSelectorDX9 {
  /* 0000 */ const D3DFORMAT& format;
  /* 0004 */ DeviceOwnerDX9* owner;
  /* 0008 */ const DWORD& adapter;
  /* 000C */ const UINT& width;
  /* 0010 */ const UINT& height;

  bool Select(D3DSCANLINEORDERING ordering);
};

// OFFSET: 0x0054B7C0
// FUNCTION: fn_01906
DeviceOwnerDX9::DeviceOwnerDX9() {
  this->adapter = 0;
  this->window = nullptr;
  this->direct3D = nullptr;
  this->device = nullptr;
  std::memset(&this->presentation, 0, sizeof(this->presentation) + sizeof(this->displayMode) + sizeof(this->backBufferDescription) + sizeof(this->caps));
  this->backBuffer = nullptr;
  this->capturedBackBuffer = nullptr;
  this->dynamicTextures = false;
  this->lastPresent = 0;
}

// OFFSET: 0x0054B830
// FUNCTION: fn_01907
void DeviceOwnerDX9Noop() {}

// OFFSET: 0x0054B840
// FUNCTION: fn_01908
bool DisplayModeSelectorDX9::Select(D3DSCANLINEORDERING ordering) {
  D3DDISPLAYMODEFILTER filter{sizeof(filter), format, ordering};
  UINT refresh = 0;
  const UINT count = this->owner->direct3D->GetAdapterModeCountEx(this->adapter, &filter);

  for (UINT index = 0; index < count; ++index) {
    D3DDISPLAYMODEEX mode{sizeof(mode)};

    if (SUCCEEDED(this->owner->direct3D->EnumAdapterModesEx(this->adapter, &filter, index, &mode)) && mode.Width == this->width &&
        mode.Height == this->height && mode.RefreshRate >= refresh) {
      refresh = mode.RefreshRate;
      this->owner->displayMode = mode;
    }
  }

  return refresh != 0;
}

// OFFSET: 0x0054B980
// FUNCTION: fn_01910
UINT* DeviceOwnerDX9::AdjustTextureDimensions(UINT* output, UINT width, UINT height) const {
  const auto roundUp = [](UINT value) {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;

    return value + (value == 0);
  };

  UINT adjustedWidth = roundUp(width);
  UINT adjustedHeight = roundUp(height);
  const DWORD ratio = this->caps.MaxTextureAspectRatio;

  adjustedWidth = std::min<UINT>(adjustedWidth, this->caps.MaxTextureWidth);
  adjustedHeight = std::min<UINT>(adjustedHeight, this->caps.MaxTextureHeight);

  if (ratio != 0) {
    if (adjustedWidth / adjustedHeight > ratio) {
      adjustedHeight = adjustedWidth / ratio;
    }

    if (adjustedHeight / adjustedWidth > ratio) {
      adjustedWidth = adjustedHeight / ratio;
    }
  }

  output[0] = adjustedWidth;
  output[1] = adjustedHeight;
  return output;
}

// OFFSET: 0x0054BD10
// FUNCTION: fn_01914
HRESULT DeviceOwnerDX9::CreateDevice(UINT selectedAdapter) {
  HRESULT result = Direct3DCreate9Ex(D3D_SDK_VERSION, &this->direct3D);

  if (FAILED(result)) {
    return result;
  }

  this->adapter = selectedAdapter;
  int width = g_configuredScreenWidth;
  int height = g_configuredScreenHeight;

  if (g_configuredFullscreen == 0) {
    RECT rectangle;
    GetClientRect(this->window, &rectangle);
    width = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;
  }

  std::memset(&this->presentation, 0, sizeof(this->presentation));
  this->presentation.BackBufferWidth = width;
  this->presentation.BackBufferHeight = height;
  this->presentation.BackBufferCount = 1;
  this->presentation.BackBufferFormat = D3DFMT_X8R8G8B8;
  this->presentation.MultiSampleType = D3DMULTISAMPLE_NONE;
  this->presentation.MultiSampleQuality = 0;
  this->presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
  this->presentation.hDeviceWindow = this->window;
  this->presentation.Windowed = g_configuredFullscreen == 0;
  this->presentation.EnableAutoDepthStencil = TRUE;
  this->presentation.AutoDepthStencilFormat = D3DFMT_D24S8;
  this->presentation.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER | D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

  if (!this->presentation.Windowed) {
    const DWORD adapter = selectedAdapter;
    const D3DFORMAT format = D3DFMT_X8R8G8B8;
    DisplayModeSelectorDX9 selectMode{format, this, adapter, width, height};
    if (!selectMode.Select(D3DSCANLINEORDERING_PROGRESSIVE) && !selectMode.Select(D3DSCANLINEORDERING_INTERLACED)) {
      D3DDISPLAYMODEEX mode = {sizeof(D3DDISPLAYMODEEX)};
      D3DDISPLAYROTATION rotation = D3DDISPLAYROTATION_IDENTITY;

      if (FAILED(this->direct3D->GetAdapterDisplayModeEx(adapter, &mode, &rotation))) {
        return deviceError;
      }

      this->displayMode = mode;
    }

    this->presentation.BackBufferWidth = this->displayMode.Width;
    this->presentation.BackBufferHeight = this->displayMode.Height;
    this->presentation.FullScreen_RefreshRateInHz = this->displayMode.RefreshRate;
  }

  result = this->direct3D->CreateDeviceEx(
    selectedAdapter, D3DDEVTYPE_HAL, this->window, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &this->presentation,
    this->presentation.Windowed ? nullptr : &this->displayMode, &this->device
  );

  if (FAILED(result)) {
    return static_cast<HRESULT>(0x82000004u);
  }

  if (FAILED(this->AcquireBackBuffer())) {
    return static_cast<HRESULT>(0x8200000Au);
  }

  if (FAILED(this->device->GetDeviceCaps(&this->caps))) {
    return deviceError;
  }

  return D3D_OK;
}

// OFFSET: 0x0054B920
// FUNCTION: fn_01909
HRESULT DeviceOwnerDX9::AcquireBackBuffer() {
  IDirect3DSurface9* surface = nullptr;
  HRESULT result = this->device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);

  if (FAILED(result)) {
    return result;
  }

  result = surface->GetDesc(&this->backBufferDescription);

  if (FAILED(result)) {
    return result;
  }

  if (this->backBuffer) {
    this->backBuffer->Release();
  }

  this->backBuffer = surface;
  return D3D_OK;
}

// OFFSET: 0x0054C140
// FUNCTION: fn_01918
HRESULT DeviceOwnerDX9::Initialize(HWND value, UINT selectedAdapter) {
  if (!value) {
    return E_INVALIDARG;
  }

  this->window = value;
  const HRESULT result = this->CreateDevice(selectedAdapter);

  if (FAILED(result)) {
    this->Close();
    return result;
  }

  this->state.Initialize(this->device);
  this->SetInitialStates();
  this->UpdateMipmapFiltering();
  this->ClipFullscreenCursor();

  return D3D_OK;
}

// OFFSET: 0x0054C2A0
// FUNCTION: fn_01919
void DeviceOwnerDX9::SetInitialStates() {
  this->state.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  this->state.SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  this->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  this->state.SetRenderState(D3DRS_ALPHAREF, 240);
  this->state.SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  this->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);
  this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  this->state.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  this->state.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  this->state.SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  this->state.SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  this->state.SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  this->state.SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  this->state.SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
  this->state.SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
  this->state.SetSamplerState(1, D3DSAMP_BORDERCOLOR, 0xFF000000u);
}

// OFFSET: 0x0054BFA0
// FUNCTION: fn_01915
HRESULT DeviceOwnerDX9::Close() {
  if (this->backBuffer) {
    this->backBuffer->Release();
    this->backBuffer = nullptr;
  }

  if (this->capturedBackBuffer) {
    this->capturedBackBuffer->Release();
    this->capturedBackBuffer = nullptr;
  }

  ULONG references = 0;

  if (this->device) {
    references = this->device->Release();
    this->device = nullptr;
  }

  if (this->direct3D) {
    this->direct3D->Release();
    this->direct3D = nullptr;
  }

  return references > 0 ? static_cast<HRESULT>(0x8200000Bu) : D3D_OK;
}

// OFFSET: 0x0054C020
// FUNCTION: fn_45500
void DeviceOwnerDX9::UpdateMipmapFiltering() {
  if (!(this->caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)) {
    this->dynamicTextures = false;
  } else {
    this->dynamicTextures = g_textureFiltering != 0;
  }
}

// OFFSET: 0x0054C070
// FUNCTION: fn_01917
void DeviceOwnerDX9::RestoreDisplayMode() {
}

// OFFSET: 0x0054C490
// FUNCTION: fn_01920
HRESULT DeviceOwnerDX9::ClipFullscreenCursor() {
  if (!this->presentation.Windowed) {
    RECT rectangle;
    GetWindowRect(this->window, &rectangle);
    ClipCursor(&rectangle);
  }

  return D3D_OK;
}

// OFFSET: 0x0054C510
// FUNCTION: fn_01921
HRESULT DeviceOwnerDX9::Present(int) {
  if (!this->device) {
    return invalidDevice;
  }

  this->lastPresent = this->device->Present(nullptr, nullptr, nullptr, nullptr);
  return this->lastPresent;
}

