#include "Client/Application/ErrorDialog.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/Direct3DErrors.h"

#include <string>

namespace {
  // STATIC: 0x01387430
  // GLOBAL: gl_24348
  HRESULT lastReported = D3D_OK;
  constexpr HRESULT invalidDevice = static_cast<HRESULT>(0x8200000Eu);
}

// OFFSET: 0x0054BB60
// FUNCTION: fn_01913
HRESULT DeviceOwnerDX9::CheckState(CRenderSystemBase* renderSystem) {
  if (!this->device) {
    return invalidDevice;
  }

  if (this->lastPresent == D3D_OK) {
    lastReported = D3D_OK;
    return D3D_OK;
  }

  HRESULT result = this->device->CheckDeviceState(this->window);

  if (result == S_PRESENT_OCCLUDED) {
    Sleep(16);
    this->lastPresent = result;
    return result;
  }

  if (result == S_PRESENT_MODE_CHANGED) {
    if (!this->device) {
      this->lastPresent = invalidDevice;
      return invalidDevice;
    }

    result = this->device->ResetEx(&this->presentation, this->presentation.Windowed == 1 ? nullptr : &this->displayMode);
  } else if (FAILED(result) && lastReported != result) {
    const std::string message = std::string(Direct3DErrorName(result)) + " : " + Direct3DErrorDescription(result);
    ShowErrorDialog(renderSystem, message.c_str(), 0);
    lastReported = result;
    this->lastPresent = result;

    return result;
  }

  if (result == D3D_OK) {
    result = this->AcquireBackBuffer();
  }

  this->lastPresent = result;
  return result;
}

// OFFSET: 0x0054BAD0
// FUNCTION: fn_01911
bool DeviceOwnerDX9::Begin(CRenderSystemBase* renderSystem) {
  if (!this->device || this->CheckState(renderSystem) != D3D_OK || FAILED(this->device->BeginScene())) {
    return false;
  }

  this->state.Begin();
  return true;
}

// OFFSET: 0x0054C050
// FUNCTION: fn_01916
void DeviceOwnerDX9::End() {
  if (this->device) {
    this->state.End();
    this->device->EndScene();
  }
}
