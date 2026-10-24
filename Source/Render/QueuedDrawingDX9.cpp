#include "Render/DeviceStateDX9.h"
#include "Render/RenderQueues.h"
#include "Render/RendererDX9Operations.h"
#include "Render/RendererFogDX9.h"
#include "Render/TextureDX9.h"

#include <algorithm>
#include <bit>
#include <cstdint>

// OFFSET: 0x00567180
// FUNCTION: fn_02228
void RendererDX9Operations::DrawAuxiliaryQueue(RenderQueues& queues) {
  auto& queue = queues.queue_0240;

  if (queue.empty()) {
    return;
  }

  ScopedDeviceStateDX9 restore(this->state);
  std::sort(queue.begin(), queue.end(), [](const LightmappedPrimitive* left, const LightmappedPrimitive* right) {
    const auto first = reinterpret_cast<std::uintptr_t>(left->texture);
    const auto second = reinterpret_cast<std::uintptr_t>(right->texture);

    if (first != second) {
      return first < second;
    }

    return reinterpret_cast<std::uintptr_t>(left->lightmap) < reinterpret_cast<std::uintptr_t>(right->lightmap);
  });
  this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->state.SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  this->state.SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->state.SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  for (LightmappedPrimitive* command : queue) {
    if (std::bit_cast<std::int32_t>(command->vertexCount) > 0) {
      this->state.SetTexture(
        0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr
      );
      this->state.SetTexture(
        1, command->lightmap ? static_cast<CTextureDX9*>(command->lightmap)->NativeTexture() : nullptr
      );
      this->DrawLightmappedPrimitive(command);
    }
  }
}

// OFFSET: 0x00567610
// FUNCTION: fn_02229
void RendererDX9Operations::DrawQueues(
  RenderQueues& queues,
  const ServiceOptions& options,
  unsigned char multitexture,
  unsigned char twoPass,
  RendererFogDX9& fog,
  const float& aspect,
  const FogConfiguration& configuration
) {
  fog.Apply(this->context, this->state, *this->context.d3dx, aspect, configuration);
  this->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->state.SetRenderState(D3DRS_ALPHAREF, 207);
  this->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
  this->state.SetRenderState(D3DRS_ZENABLE, 1);
  this->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);
  this->DrawOpaqueQueues(queues);
  this->DrawGroundQueues(queues, options, multitexture);
  this->state.SetRenderState(D3DRS_ALPHAREF, 1);
  this->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
  this->DrawAuxiliaryQueue(queues);

  const auto drawCommand = [&](RenderCommand& command) {
    this->state.SetTexture(0, command.texture ? static_cast<CTextureDX9*>(command.texture)->NativeTexture() : nullptr);
    this->SetTextureMode(command.unknown_0020);
    this->state.SetRenderState(D3DRS_SRCBLEND, command.unknown_0018);
    this->state.SetRenderState(D3DRS_DESTBLEND, command.unknown_001C);
    this->DrawPrimitive(&command);
  };

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_FOGENABLE, 0);
    this->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);

    for (RenderCommand* command : queues.queue_0210) {
      if (command) {
        drawCommand(*command);
      }
    }
  }

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_ZENABLE, 0);
    this->DrawDepthQueue(queues.queue_021C, true);
  }

  {
    ScopedDeviceStateDX9 restore(this->state);
    std::sort(
      queues.queue_0180.begin(), queues.queue_0180.end(), [](const DepthPrimitive& left, const DepthPrimitive& right) {
        if (left.depth < right.depth) {
          return true;
        }

        if (right.depth < left.depth) {
          return false;
        }

        return left.depth == right.depth &&
               reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
      }
    );
    this->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
    this->state.SetRenderState(D3DRS_ZWRITEENABLE, 1);
    this->state.SetRenderState(D3DRS_ALPHAREF, 159);
    this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->DrawDepthQueue(queues.queue_0180, false);
    this->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);
    this->state.SetRenderState(D3DRS_ALPHAREF, 159);
    this->state.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
    this->DrawDepthQueue(queues.queue_0180, false);
  }

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->DrawDepthQueue(queues.queue_01C8, true);
  }

  this->state.SetRenderState(D3DRS_ZWRITEENABLE, 0);
  this->DrawAlphaQueues(queues, twoPass);

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_ALPHAREF, 207);
    this->DrawDepthQueue(queues.queue_0228, true);
  }

  this->state.SetRenderState(D3DRS_FOGENABLE, 0);

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->DrawDepthQueue(queues.queue_01D4, true);
  }

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->state.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->state.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    for (RenderCommand* command : queues.queue_01EC) {
      drawCommand(*command);
    }
  }

  this->state.SetRenderState(D3DRS_ZENABLE, 0);

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->DrawDepthQueue(queues.queue_01E0, true);
  }

  {
    ScopedDeviceStateDX9 restore(this->state);
    this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    this->DrawDepthQueue(queues.queue_0198, false);
  }

  {
    ScopedDeviceStateDX9 restore(this->state);

    for (const DepthPrimitive& entry : queues.queue_0234) {
      RenderCommand& command = *entry.primitive;
      this->state.SetTexture(
        0, command.texture ? static_cast<CTextureDX9*>(command.texture)->NativeTexture() : nullptr
      );
      this->SetTextureMode(command.unknown_0020);
      // COMISS/JB selects command blend factors for unordered depths too.
      const bool selected = !(entry.depth >= 0.99f);
      this->state.SetRenderState(D3DRS_SRCBLEND, selected ? command.unknown_0018 : D3DBLEND_SRCALPHA);
      this->state.SetRenderState(D3DRS_DESTBLEND, selected ? command.unknown_001C : D3DBLEND_INVSRCALPHA);
      this->DrawPrimitive(&command);
    }
  }
}
