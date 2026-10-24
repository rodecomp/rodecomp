#include "Render/DeviceOwnerDX9.h"
#include "Render/RendererBackBuffer.h"

// OFFSET: 0x00567E80
// FUNCTION: fn_02232
bool RendererBackBuffer::Lock(const DeviceOwnerDX9& owner) {
  D3DLOCKED_RECT locked;

  if (FAILED(owner.backBuffer->LockRect(&locked, nullptr, 0))) {
    return false;
  }

  this->pixels = locked.pBits;
  this->pitch = locked.Pitch;

  switch (owner.backBufferDescription.Format) {
  case D3DFMT_A8R8G8B8:
  case D3DFMT_X8R8G8B8:
    this->bitsPerPixel = 32;
    break;
  case D3DFMT_R5G6B5:
  case D3DFMT_X1R5G5B5:
  case D3DFMT_A1R5G5B5:
  case D3DFMT_A4R4G4B4:
    this->bitsPerPixel = 16;
    break;
  default:
    this->bitsPerPixel = 0;
    break;
  }

  return true;
}

// OFFSET: 0x00568ED0
// FUNCTION: fn_02243
HRESULT RendererBackBuffer::Unlock(const DeviceOwnerDX9& owner) {
  return owner.backBuffer->UnlockRect();
}
