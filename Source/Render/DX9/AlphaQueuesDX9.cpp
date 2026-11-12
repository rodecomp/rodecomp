#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <cstdint>

namespace {
  // INLINED
  bool Before(const DepthPrimitive& left, const DepthPrimitive& right) {
    if (left.depth < right.depth) {
      return true;
    }

    if (right.depth < left.depth) {
      return false;
    }

    return left.depth == right.depth && reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
  }

  // INLINED
  bool WritesDepth(const RenderCommand& command) {
    return (command.depthFlags & 0xFFu) != 0 && (command.depthFlags & 0xFF00u) != 0;
  }
}

// OFFSET: 0x00565DA0
// FUNCTION: fn_02224
void CRendererDX9::DrawAlphaQueues(RenderQueues& queues, unsigned char twoPass) {
  ScopedDeviceStateDX9 restore(this->device->state);
  const auto before = [](const DepthPrimitive& left, const DepthPrimitive& right) {
    return Before(left, right);
  };

  std::sort(queues.queue_018C.begin(), queues.queue_018C.end(), before);
  std::sort(queues.queue_01BC.begin(), queues.queue_01BC.end(), before);
  this->device->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
  this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->device->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->device->state.SetRenderState(D3DRS_ALPHAREF, 1);
  this->device->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

  if (twoPass == 1) {
    const auto drawAlphaTested = [this](const DepthPrimitive& entry) {
      RenderCommand* command = entry.primitive;
      this->device->state.SetRenderState(D3DRS_ALPHAREF, WritesDepth(*command) ? 1 : 240);
      this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
      this->DrawPrimitive(command);
    };

    this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);
    this->device->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    std::for_each(queues.queue_018C.begin(), queues.queue_018C.end(), drawAlphaTested);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    std::for_each(queues.queue_01BC.begin(), queues.queue_01BC.end(), drawAlphaTested);

    this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);
    this->device->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    std::for_each(queues.queue_018C.begin(), queues.queue_018C.end(), drawAlphaTested);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    std::for_each(queues.queue_01BC.begin(), queues.queue_01BC.end(), drawAlphaTested);

    this->device->state.SetRenderState(D3DRS_ALPHAREF, 1);
    this->device->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->DrawDepthQueue(true, queues.queue_01A4);
  } else {
    const auto drawDepthWriting = [this](const DepthPrimitive& entry) {
      RenderCommand* command = entry.primitive;
      this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, WritesDepth(*command));
      this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
      this->DrawPrimitive(command);
    };

    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    std::for_each(queues.queue_018C.begin(), queues.queue_018C.end(), drawDepthWriting);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    std::for_each(queues.queue_01BC.begin(), queues.queue_01BC.end(), drawDepthWriting);
  }
}
