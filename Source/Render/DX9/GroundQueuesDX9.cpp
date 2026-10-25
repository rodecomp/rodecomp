#include "Contents/Configuration/ServiceOptions.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9Operations.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <bit>
#include <cstdint>

namespace {
  bool TextureBefore(const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  }

  // OFFSET: 0x00566D60
  // FUNCTION: fn_02227
  void DrawBaseQueue(RendererDX9Operations& renderer, std::vector<RenderCommand*>& queue) {
    ScopedDeviceStateDX9 restore(renderer.state);
    std::sort(queue.begin(), queue.end(), TextureBefore);

    for (RenderCommand* command : queue) {
      renderer.state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
      renderer.DrawPrimitive(command);
    }
  }
}

// OFFSET: 0x005668B0
// FUNCTION: fn_02226
void RendererDX9Operations::DrawGroundQueues(RenderQueues& queues, const ServiceOptions& options, unsigned char multitexture) {
  ScopedDeviceStateDX9 restore(this->state);
  this->state.SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFFu);
  this->state.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

  if (options.Get(119) != 0) {
    if (multitexture == 1) {
      ScopedDeviceStateDX9 restoreDual(this->state);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
      auto& queue = queues.lightmapped.first;
      std::sort(queue.begin(), queue.end(), [](const LightmappedPrimitive* left, const LightmappedPrimitive* right) {
        const auto first = reinterpret_cast<std::uintptr_t>(left->texture);
        const auto second = reinterpret_cast<std::uintptr_t>(right->texture);

        if (first != second) {
          return first < second;
        }

        return reinterpret_cast<std::uintptr_t>(left->lightmap) < reinterpret_cast<std::uintptr_t>(right->lightmap);
      });

      for (LightmappedPrimitive* command : queue) {
        if (command->vertexCount > 0) {
          this->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
          this->state.SetTexture(1, command->lightmap ? static_cast<CTextureDX9*>(command->lightmap)->NativeTexture() : nullptr);
          this->DrawLightmappedPrimitive(command);
        }
      }

      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    } else {
      DrawBaseQueue(*this, queues.queue_0168);
      ScopedDeviceStateDX9 restoreLightmap(this->state);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
      this->state.SetRenderState(D3DRS_ALPHAREF, 1);
      this->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
      this->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
      std::sort(queues.queue_0174.begin(), queues.queue_0174.end(), TextureBefore);

      for (RenderCommand* command : queues.queue_0174) {
        this->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
        this->DrawPrimitive(command);
      }
    }
  } else {
    DrawBaseQueue(*this, queues.queue_0168);
  }

  this->state.SetRenderState(D3DRS_COLORWRITEENABLE, 0);
  this->state.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}
