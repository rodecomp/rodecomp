#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9.h"

// OFFSET: 0x00567E80
// FUNCTION: fn_02232
bool CRendererDX9::LockBackBuffer() {
  const DeviceOwnerDX9& owner = *this->device;
  D3DLOCKED_RECT locked;

  if (FAILED(owner.backBuffer->LockRect(&locked, nullptr, 0))) {
    return false;
  }

  this->backBuffer.pixels = locked.pBits;
  this->backBuffer.pitch = locked.Pitch;

  switch (owner.backBufferDescription.Format) {
  case D3DFMT_A8R8G8B8:
  case D3DFMT_X8R8G8B8:
    this->backBuffer.bitsPerPixel = 32;
    break;
  case D3DFMT_R5G6B5:
  case D3DFMT_X1R5G5B5:
  case D3DFMT_A1R5G5B5:
  case D3DFMT_A4R4G4B4:
    this->backBuffer.bitsPerPixel = 16;
    break;
  default:
    this->backBuffer.bitsPerPixel = 0;
    break;
  }

  return true;
}

// OFFSET: 0x00568ED0
// FUNCTION: fn_02243
HRESULT CRendererDX9::UnlockBackBuffer() {
  return this->device->backBuffer->UnlockRect();
}
