#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/RendererBackBuffer.h"

// OFFSET: 0x0055A080
// FUNCTION: fn_02075
void DeviceOwnerDX7::CaptureBackBuffer() {
  if (this->captureSurface && this->renderTarget) {
    if (FAILED(this->captureSurface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_DDFX, nullptr))) {
      this->captureSurface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_WAIT, nullptr);
    }
  }
}

// OFFSET: 0x0055D850
// FUNCTION: fn_02108
void DeviceOwnerDX7::RestoreBackBuffer() {
  if (this->renderTarget && this->captureSurface) {
    if (FAILED(this->renderTarget->Blt(nullptr, this->captureSurface, nullptr, DDBLT_DDFX, nullptr))) {
      this->renderTarget->Blt(nullptr, this->captureSurface, nullptr, DDBLT_WAIT, nullptr);
    }
  }
}

// OFFSET: 0x0055D3E0
// FUNCTION: fn_02103
bool RendererBackBuffer::Lock(const DeviceOwnerDX7& owner) {
  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);

  if (FAILED(owner.renderTarget->Lock(nullptr, &description, DDLOCK_WAIT, nullptr))) {
    return false;
  }

  this->pixels = description.lpSurface;
  this->bitsPerPixel = description.ddpfPixelFormat.dwRGBBitCount;
  this->pitch = description.lPitch;

  return true;
}

// OFFSET: 0x0055FBB0
// FUNCTION: fn_02122
HRESULT RendererBackBuffer::Unlock(const DeviceOwnerDX7& owner) {
  return owner.renderTarget->Unlock(nullptr);
}
