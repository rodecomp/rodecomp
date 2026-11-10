#include "Client/Application/ErrorDialog.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/Direct3DErrors.h"

#include <format>

namespace {
  // STATIC: 0x01387430
  // GLOBAL: gl_24348
  HRESULT lastReported = D3D_OK;
  constexpr HRESULT invalidDevice = static_cast<HRESULT>(0x8200000Eu);
}

// OFFSET: 0x0054BB60
// FUNCTION: fn_01913
__declspec(noinline) HRESULT DeviceOwnerDX9::CheckState() {
  if (!this->device) {
    return invalidDevice;
  }

  HRESULT result;

  if (this->lastPresent != D3D_OK) {
    result = this->device->CheckDeviceState(this->window);

    if (result == S_PRESENT_OCCLUDED) {
      Sleep(16);
    } else {
      if (result == S_PRESENT_MODE_CHANGED) {
        if (!this->device) {
          result = invalidDevice;
        } else if (this->presentation.Windowed == 1) {
          result = this->device->ResetEx(&this->presentation, nullptr);
        } else {
          result = this->device->ResetEx(&this->presentation, &this->displayMode);
        }
      } else if (FAILED(result)) {
        if (lastReported != result) {
          const std::string message = std::format("{} : {}", Direct3DErrorName(result), Direct3DErrorDescription(result));
          ShowErrorDialog(message.c_str(), 0);
          lastReported = result;
        }
      }

      if (result == D3D_OK) {
        result = this->AcquireBackBuffer();
      }
    }

    this->lastPresent = result;
  } else {
    lastReported = D3D_OK;
    result = D3D_OK;
  }

  return result;
}

// OFFSET: 0x0054BAD0
// FUNCTION: fn_01911
bool DeviceOwnerDX9::Begin(CRenderSystemBase* renderSystem) {
  if (!this->device || this->CheckState() != D3D_OK || FAILED(this->device->BeginScene())) {
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
