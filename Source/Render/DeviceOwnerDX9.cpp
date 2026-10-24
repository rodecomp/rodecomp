#include "Render/DeviceOwnerDX9.h"

#include <cstring>

namespace {
  constexpr HRESULT invalidDevice = static_cast<HRESULT>(0x8200000Eu);
  constexpr HRESULT deviceError = static_cast<HRESULT>(0x82000000u);
}

// OFFSET: 0x0054B7C0
// FUNCTION: fn_01906
DeviceOwnerDX9::DeviceOwnerDX9()
    : window(nullptr), direct3D(nullptr), device(nullptr), state{}, adapter(0), backBuffer(nullptr),
      capturedBackBuffer(nullptr), dynamicTextures(false), lastPresent(0) {
  std::memset(
    &this->presentation, 0,
    sizeof(this->presentation) + sizeof(this->displayMode) + sizeof(this->backBufferDescription) + sizeof(this->caps)
  );
}

// OFFSET: 0x0054B840
// FUNCTION: fn_01908
bool DeviceOwnerDX9::SelectDisplayMode(
  D3DFORMAT format,
  UINT selectedAdapter,
  UINT width,
  UINT height,
  D3DSCANLINEORDERING ordering
) {
  D3DDISPLAYMODEFILTER filter{sizeof(filter), format, ordering};
  const UINT count = this->direct3D->GetAdapterModeCountEx(selectedAdapter, &filter);
  UINT refresh = 0;

  for (UINT index = 0; index < count; ++index) {
    D3DDISPLAYMODEEX mode{};
    mode.Size = sizeof(mode);

    if (
      SUCCEEDED(this->direct3D->EnumAdapterModesEx(selectedAdapter, &filter, index, &mode)) && mode.Width == width &&
      mode.Height == height && mode.RefreshRate >= refresh
    ) {
      refresh = mode.RefreshRate;
      this->displayMode = mode;
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

  if (this->caps.MaxTextureWidth < adjustedWidth) {
    adjustedWidth = this->caps.MaxTextureWidth;
  }

  if (this->caps.MaxTextureHeight < adjustedHeight) {
    adjustedHeight = this->caps.MaxTextureHeight;
  }

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
HRESULT DeviceOwnerDX9::CreateDevice(UINT selectedAdapter, const DeviceOptionsDX9& options) {
  HRESULT result = Direct3DCreate9Ex(D3D_SDK_VERSION, &this->direct3D);

  if (FAILED(result)) {
    return result;
  }

  this->adapter = selectedAdapter;
  UINT width = options.width;
  UINT height = options.height;

  if (options.fullscreen == 0) {
    RECT rectangle;
    GetClientRect(this->window, &rectangle);
    width = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;
  }

  this->presentation = {};
  this->presentation.BackBufferWidth = width;
  this->presentation.BackBufferHeight = height;
  this->presentation.BackBufferFormat = D3DFMT_X8R8G8B8;
  this->presentation.BackBufferCount = 1;
  this->presentation.MultiSampleType = D3DMULTISAMPLE_NONE;
  this->presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
  this->presentation.hDeviceWindow = this->window;
  this->presentation.Windowed = options.fullscreen == 0;
  this->presentation.EnableAutoDepthStencil = TRUE;
  this->presentation.AutoDepthStencilFormat = D3DFMT_D24S8;
  this->presentation.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER | D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

  if (!this->presentation.Windowed) {
    if (
      !this->SelectDisplayMode(D3DFMT_X8R8G8B8, selectedAdapter, width, height, D3DSCANLINEORDERING_PROGRESSIVE) &&
      !this->SelectDisplayMode(D3DFMT_X8R8G8B8, selectedAdapter, width, height, D3DSCANLINEORDERING_INTERLACED)
    ) {
      D3DDISPLAYMODEEX mode{};
      mode.Size = sizeof(mode);
      D3DDISPLAYROTATION rotation = D3DDISPLAYROTATION_IDENTITY;

      if (FAILED(this->direct3D->GetAdapterDisplayModeEx(selectedAdapter, &mode, &rotation))) {
        return deviceError;
      }

      this->displayMode = mode;
    }

    this->presentation.BackBufferWidth = this->displayMode.Width;
    this->presentation.BackBufferHeight = this->displayMode.Height;
    this->presentation.FullScreen_RefreshRateInHz = this->displayMode.RefreshRate;
  }

  result = this->direct3D->CreateDeviceEx(
    selectedAdapter, D3DDEVTYPE_HAL, this->window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
    &this->presentation, this->presentation.Windowed ? nullptr : &this->displayMode, &this->device
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
    // Original does not release the acquired local surface on this failure.
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
HRESULT DeviceOwnerDX9::Initialize(HWND value, UINT selectedAdapter, const DeviceOptionsDX9& options) {
  if (!value) {
    return E_INVALIDARG;
  }

  this->window = value;
  const HRESULT result = this->CreateDevice(selectedAdapter, options);

  if (FAILED(result)) {
    this->Close();
    return result;
  }

  this->state.Initialize(this->device);
  this->state.SetInitialStates();
  this->dynamicTextures = (this->caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) != 0 && options.dynamicTextures != 0;
  this->ClipFullscreenCursor();
  return D3D_OK;
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

  return references != 0 ? static_cast<HRESULT>(0x8200000Bu) : D3D_OK;
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

SurfaceDX9Context DeviceOwnerDX9::SurfaceContext() const {
  return {this->device, this->backBufferDescription, this->backBuffer};
}

TextureDX9Context DeviceOwnerDX9::TextureContext(
  D3DX9Runtime& d3dx,
  RenderPrimitivePool& primitives,
  RenderQueues& queues,
  bool depthAlpha,
  float depthStart,
  float depthEnd
) const {
  return {this->direct3D, this->device,
          this->adapter,  this->presentation.BackBufferFormat,
          this->caps,     this->dynamicTextures,
          &d3dx,          &primitives,
          &queues,        depthAlpha,
          depthStart,     depthEnd};
}
