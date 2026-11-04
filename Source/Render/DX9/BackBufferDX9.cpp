#include "Render/DX9/DeviceOwnerDX9.h"

// OFFSET: 0x0054BA50
// FUNCTION: fn_45506
bool DeviceOwnerDX9::CaptureBackBuffer() {
  if (!this->device) {
    return false;
  }

  if (!this->capturedBackBuffer && FAILED(this->device->CreateOffscreenPlainSurface(
                                     this->backBufferDescription.Width, this->backBufferDescription.Height, this->backBufferDescription.Format,
                                     D3DPOOL_SYSTEMMEM, &this->capturedBackBuffer, nullptr
                                   ))) {
    return false;
  }

  if (FAILED(this->device->GetRenderTargetData(this->backBuffer, this->capturedBackBuffer))) {
    if (this->capturedBackBuffer) {
      this->capturedBackBuffer->Release();
      this->capturedBackBuffer = nullptr;
    }

    return false;
  }

  return true;
}

// OFFSET: 0x005650F0
// FUNCTION: fn_02213
ScopedDeviceStateDX9 DeviceOwnerDX9::SaveState() {
  return ScopedDeviceStateDX9(this->state);
}

// OFFSET: 0x0054C440
// FUNCTION: fn_45507
bool DeviceOwnerDX9::RestoreBackBuffer() {
  if (!this->device || !this->capturedBackBuffer) {
    return false;
  }

  if (FAILED(this->device->UpdateSurface(this->capturedBackBuffer, nullptr, this->backBuffer, nullptr))) {
    if (this->capturedBackBuffer) {
      this->capturedBackBuffer->Release();
      this->capturedBackBuffer = nullptr;
    }

    return false;
  }

  return true;
}
