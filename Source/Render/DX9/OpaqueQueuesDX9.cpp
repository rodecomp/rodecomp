#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <cstdint>

// OFFSET: 0x005666E0
// FUNCTION: fn_02225
void CRendererDX9::DrawOpaqueQueues() {
  const auto bindTexture = [this](CTexture* texture) {
    if (texture) {
      this->device->SetTexture(0, static_cast<CTextureDX9*>(texture)->NativeTexture());
    } else {
      this->device->SetTexture(0, nullptr);
    }
  };

  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  std::sort(this->queue_015C.begin(), this->queue_015C.end(), [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  });

  for (RenderCommand* const& command : this->queue_015C) {
    if (command) {
      bindTexture(command->texture);
      this->DrawPrimitive(command);
    }
  }

  this->device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  this->device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  std::sort(this->queue_01B0.begin(), this->queue_01B0.end(), [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  });

  for (RenderCommand* const& command : this->queue_01B0) {
    if (command) {
      bindTexture(command->texture);
      this->DrawPrimitive(command);
    }
  }
}
