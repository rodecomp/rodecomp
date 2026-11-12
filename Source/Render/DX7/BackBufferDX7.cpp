#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/RendererDX7.h"

// OFFSET: 0x00549570
// FUNCTION: fn_45504
void DeviceOwnerDX7::CaptureBackBufferImpl() {
  if (this->captureSurface && this->renderTarget) {
    if (FAILED(this->captureSurface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_ASYNC, nullptr))) {
      this->captureSurface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_WAIT, nullptr);
    }
  }
}

// OFFSET: 0x0055A080
// FUNCTION: fn_02075
bool CRendererDX7::CaptureBackBuffer() {
  DeviceOwnerDX7* owner = g_deviceOwnerDX7;
  if (owner) {
    owner->CaptureBackBufferImpl();
  }
  return true;
}

// OFFSET: 0x0054AEB0
// FUNCTION: fn_45505
void DeviceOwnerDX7::RestoreBackBufferImpl() {
  if (this->renderTarget && this->captureSurface) {
    if (FAILED(this->renderTarget->Blt(nullptr, this->captureSurface, nullptr, DDBLT_ASYNC, nullptr))) {
      this->renderTarget->Blt(nullptr, this->captureSurface, nullptr, DDBLT_WAIT, nullptr);
    }
  }
}

// OFFSET: 0x0055D850
// FUNCTION: fn_02108
bool CRendererDX7::RestoreBackBuffer() {
  DeviceOwnerDX7* owner = g_deviceOwnerDX7;
  if (owner) {
    owner->RestoreBackBufferImpl();
  }
  return true;
}

// OFFSET: 0x0055D3E0
// FUNCTION: fn_02103
int CRendererDX7::LockBackBuffer() {
  DDSURFACEDESC2 description = {sizeof(description)};
  IDirectDrawSurface7* renderTarget = g_deviceOwnerDX7 ? g_deviceOwnerDX7->renderTarget : nullptr;

  if (FAILED(renderTarget->Lock(nullptr, &description, DDLOCK_WAIT, nullptr))) {
    return 0;
  }

  this->backBuffer.pixels = description.lpSurface;
  this->backBuffer.bitsPerPixel = description.ddpfPixelFormat.dwRGBBitCount;
  this->backBuffer.pitch = description.lPitch;

  return 1;
}

// OFFSET: 0x0055FBB0
// FUNCTION: fn_02122
HRESULT CRendererDX7::UnlockBackBuffer() {
  IDirectDrawSurface7* renderTarget = g_deviceOwnerDX7 ? g_deviceOwnerDX7->renderTarget : nullptr;
  return renderTarget->Unlock(nullptr);
}
