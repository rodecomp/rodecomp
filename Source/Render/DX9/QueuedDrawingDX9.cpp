#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/RendererFogDX9.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <bit>
#include <cstdint>

// OFFSET: 0x00567180
// FUNCTION: fn_02228
void CRendererDX9::DrawAuxiliaryQueue() {
  auto& queue = this->queues.queue_0240;

  if (queue.empty()) {
    return;
  }

  ScopedDeviceStateDX9 restore(this->device->state);
  std::sort(queue.begin(), queue.end(), [](const LightmappedPrimitive* left, const LightmappedPrimitive* right) {
    const auto first = reinterpret_cast<std::uintptr_t>(left->texture);
    const auto second = reinterpret_cast<std::uintptr_t>(right->texture);

    if (first != second) {
      return first < second;
    }

    return reinterpret_cast<std::uintptr_t>(left->lightmap) < reinterpret_cast<std::uintptr_t>(right->lightmap);
  });
  this->device->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  this->device->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->device->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  this->device->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->device->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->device->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->device->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  this->device->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->device->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->device->state.SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
  this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  this->device->state.SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->device->state.SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  for (LightmappedPrimitive*& command : queue) {
    if (command->vertexCount > 0) {
      this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
      this->device->state.SetTexture(1, command->lightmap ? static_cast<CTextureDX9*>(command->lightmap)->NativeTexture() : nullptr);
      this->DrawAuxiliaryPrimitive(command);
    }
  }
}

// OFFSET: 0x00567610
// FUNCTION: fn_02229
void CRendererDX9::DrawQueues(
  RenderQueues& queues, unsigned char twoPass
) {
  this->ApplyFog();
  this->device->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->device->state.SetRenderState(D3DRS_ALPHAREF, 207);
  this->device->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
  this->device->state.SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);

  this->DrawOpaqueQueues(queues);
  this->DrawGroundQueues();

  this->device->state.SetRenderState(D3DRS_ALPHAREF, 1);
  this->device->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
  this->DrawAuxiliaryQueue();

  const auto drawCommand = [&](RenderCommand& command) {
    this->device->state.SetTexture(0, command.texture ? static_cast<CTextureDX9*>(command.texture)->NativeTexture() : nullptr);
    this->SetTextureMode(command.textureMode);
    this->device->state.SetRenderState(D3DRS_SRCBLEND, command.sourceBlend);
    this->device->state.SetRenderState(D3DRS_DESTBLEND, command.destinationBlend);
    this->DrawPrimitive(&command);
  };

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_FOGENABLE, 0);
    this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);

    for (RenderCommand* command : queues.queue_0210) {
      if (command) {
        drawCommand(*command);
      }
    }
  }

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    this->DrawDepthQueue(true, queues.queue_021C);
  }

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    std::sort(queues.queue_0180.begin(), queues.queue_0180.end(), [](const DepthPrimitive& left, const DepthPrimitive& right) {
      if (left.depth < right.depth) {
        return true;
      }

      if (right.depth < left.depth) {
        return false;
      }

      return left.depth == right.depth && reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
    });
    this->device->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
    this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);
    this->device->state.SetRenderState(D3DRS_ALPHAREF, 159);
    this->device->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->DrawDepthQueue(false, queues.queue_0180);
    this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);
    this->device->state.SetRenderState(D3DRS_ALPHAREF, 159);
    this->device->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
    this->DrawDepthQueue(false, queues.queue_0180);
  }

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->DrawDepthQueue(true, queues.queue_01C8);
  }

  this->device->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);
  this->DrawAlphaQueues(queues, twoPass);

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_ALPHAREF, 207);
    this->DrawDepthQueue(true, queues.queue_0228);
  }

  this->device->state.SetRenderState(D3DRS_FOGENABLE, 0);

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->DrawDepthQueue(true, queues.queue_01D4);
  }

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->device->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    for (RenderCommand* command : queues.queue_01EC) {
      drawCommand(*command);
    }
  }

  this->device->state.SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->DrawDepthQueue(true, queues.queue_01E0);
  }

  {
    ScopedDeviceStateDX9 restore(this->device->state);
    this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    this->DrawDepthQueue(false, queues.queue_0198);
  }

  {
    ScopedDeviceStateDX9 restore(this->device->state);

    for (const DepthPrimitive& entry : queues.queue_0234) {
      RenderCommand& command = *entry.primitive;
      this->device->state.SetTexture(0, command.texture ? static_cast<CTextureDX9*>(command.texture)->NativeTexture() : nullptr);
      this->SetTextureMode(command.textureMode);

      const bool selected = !(entry.depth >= 0.99f);
      this->device->state.SetRenderState(D3DRS_SRCBLEND, selected ? command.sourceBlend : D3DBLEND_SRCALPHA);
      this->device->state.SetRenderState(D3DRS_DESTBLEND, selected ? command.destinationBlend : D3DBLEND_INVSRCALPHA);
      this->DrawPrimitive(&command);
    }
  }
}
