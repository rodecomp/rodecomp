#include "Render/DX9/DeviceOwnerDX9.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/TextureDX9.h"
#include "Render/Primitives/TextureQuad.h"

#include <algorithm>
#include <bit>
#include <cstdint>

// OFFSET: 0x00566D60
// FUNCTION: fn_02227
void CRendererDX9::DrawBaseQueue() {
  ScopedDeviceStateDX9 restore(this->device->state);
  std::sort(this->queues.queue_0168.begin(), this->queues.queue_0168.end(), [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  });

  for (RenderCommand* command : this->queues.queue_0168) {
    this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
    this->DrawPrimitive(command);
  }
}

// OFFSET: 0x005668B0
// FUNCTION: fn_02226
void CRendererDX9::DrawGroundQueues() {
  ScopedDeviceStateDX9 restore(this->device->state);
  this->device->state.SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFFu);
  this->device->state.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

  if (g_serviceOptions.Get(119) != 0) {
    if (g_multitexture == 1) {
      ScopedDeviceStateDX9 restoreDual(this->device->state);
      this->device->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
      auto& queue = this->queues.lightmapped.first;
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
          this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
          this->device->state.SetTexture(1, command->lightmap ? static_cast<CTextureDX9*>(command->lightmap)->NativeTexture() : nullptr);
          this->DrawLightmappedPrimitive(command);
        }
      }

      this->device->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    } else {
      this->DrawBaseQueue();
      ScopedDeviceStateDX9 restoreLightmap(this->device->state);
      this->device->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      this->device->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
      this->device->state.SetRenderState(D3DRS_ALPHAREF, 1);
      this->device->state.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
      this->device->state.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
      std::sort(this->queues.queue_0174.begin(), this->queues.queue_0174.end(), [](const RenderCommand* left, const RenderCommand* right) {
        return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
      });

      for (RenderCommand* command : this->queues.queue_0174) {
        this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
        this->DrawPrimitive(command);
      }
    }
  } else {
    this->DrawBaseQueue();
  }

  this->device->state.SetRenderState(D3DRS_AMBIENT, 0);
  this->device->state.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}
