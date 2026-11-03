#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <cstdint>

// OFFSET: 0x005666E0
// FUNCTION: fn_02225
void CRendererDX9::DrawOpaqueQueues(RenderQueues& queues) {
  ScopedDeviceStateDX9 restore(this->device->state);
  this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  const auto before = [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  };
  std::sort(queues.queue_015C.begin(), queues.queue_015C.end(), before);

  for (RenderCommand* command : queues.queue_015C) {
    if (command) {
      this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
      this->DrawPrimitive(command);
    }
  }

  this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  std::sort(queues.queue_01B0.begin(), queues.queue_01B0.end(), before);

  for (RenderCommand* command : queues.queue_01B0) {
    if (command) {
      this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
      this->DrawPrimitive(command);
    }
  }
}
