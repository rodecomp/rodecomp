#include "Render/DeviceStateDX9.h"
#include "Render/RenderQueues.h"
#include "Render/RendererDX9Operations.h"
#include "Render/TextureDX9.h"

#include <algorithm>
#include <cstdint>

// OFFSET: 0x005666E0
// FUNCTION: fn_02225
void RendererDX9Operations::DrawOpaqueQueues(RenderQueues& queues) {
  ScopedDeviceStateDX9 restore(this->state);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  const auto before = [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  };
  std::sort(queues.queue_015C.begin(), queues.queue_015C.end(), before);

  for (RenderCommand* command : queues.queue_015C) {
    if (command) {
      this->state.SetTexture(
        0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr
      );
      this->DrawPrimitive(command);
    }
  }

  this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  std::sort(queues.queue_01B0.begin(), queues.queue_01B0.end(), before);

  for (RenderCommand* command : queues.queue_01B0) {
    if (command) {
      this->state.SetTexture(
        0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr
      );
      this->DrawPrimitive(command);
    }
  }
}
