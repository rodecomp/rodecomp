#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9Operations.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <cstdint>

namespace {
  bool Before(const DepthPrimitive& left, const DepthPrimitive& right) {
    if (left.depth < right.depth) {
      return true;
    }

    if (right.depth < left.depth) {
      return false;
    }

    return left.depth == right.depth && reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
  }

  bool WritesDepth(const RenderCommand& command) {
    return (command.unknown_0014 & 0xFFu) != 0 && (command.unknown_0014 & 0xFF00u) != 0;
  }
}

// OFFSET: 0x00565DA0
// FUNCTION: fn_02224
void RendererDX9Operations::DrawAlphaQueues(RenderQueues& queues, unsigned char twoPass) {
  ScopedDeviceStateDX9 restore(this->state);
  std::sort(queues.queue_018C.begin(), queues.queue_018C.end(), Before);
  std::sort(queues.queue_01BC.begin(), queues.queue_01BC.end(), Before);
  this->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
  this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->state.SetRenderState(D3DRS_ALPHAREF, 1);
  this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

  const auto draw = [&](const std::vector<DepthPrimitive>& queue, bool alphaTest) {
    for (const DepthPrimitive& entry : queue) {
      RenderCommand& command = *entry.primitive;

      if (alphaTest) {
        this->state.SetRenderState(D3DRS_ALPHAREF, WritesDepth(command) ? 1 : 240);
      } else {
        this->state.SetRenderState(D3DRS_ZWRITEENABLE, WritesDepth(command));
      }

      this->state.SetTexture(0, command.texture ? static_cast<CTextureDX9*>(command.texture)->NativeTexture() : nullptr);
      this->DrawPrimitive(&command);
    }
  };
  const auto drawPair = [&](bool alphaTest) {
    this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    draw(queues.queue_018C, alphaTest);
    this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    draw(queues.queue_01BC, alphaTest);
  };

  if (twoPass == 1) {
    this->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);
    this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    drawPair(true);
    this->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);
    this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
    drawPair(true);
    this->state.SetRenderState(D3DRS_ALPHAREF, 1);
    this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->DrawDepthQueue(queues.queue_01A4, true);
  } else {
    drawPair(false);
  }
}
