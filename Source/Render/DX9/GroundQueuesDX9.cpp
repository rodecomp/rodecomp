#include "Render/DX9/DeviceOwnerDX9.h"
#include "UI/GameSettings/TalkType.h"
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
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  std::sort(this->queue_0168.begin(), this->queue_0168.end(), [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  });

  for (RenderCommand*& command : this->queue_0168) {
    CTexture* texture = command->texture;
    if (texture) {
      this->device->SetTexture(0, static_cast<CTextureDX9*>(texture)->NativeTexture());
    } else {
      this->device->SetTexture(0, nullptr);
    }
    this->DrawPrimitive(command);
  }
}

// OFFSET: 0x005668B0
// FUNCTION: fn_02226
void CRendererDX9::DrawGroundQueues() {
  const auto bindTexture = [this](UINT stage, CTexture* texture) {
    if (texture) {
      this->device->SetTexture(stage, static_cast<CTextureDX9*>(texture)->NativeTexture());
    } else {
      this->device->SetTexture(stage, nullptr);
    }
  };

  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFFu);
  this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

  if (g_serviceOptions.Get(TT_LIGHTMAP_ON_OFF) != 0) {
    if (g_multitexture == 1) {
      ScopedDeviceStateDX9 restoreDual = this->device->CaptureState();
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
      auto& queue = this->lightmappedFirst;
      std::sort(queue.begin(), queue.end(), [](const LightmappedPrimitive* left, const LightmappedPrimitive* right) {
        const auto first = reinterpret_cast<std::uintptr_t>(left->texture);
        const auto second = reinterpret_cast<std::uintptr_t>(right->texture);

        if (first != second) {
          return first < second;
        }

        return reinterpret_cast<std::uintptr_t>(left->lightmap) < reinterpret_cast<std::uintptr_t>(right->lightmap);
      });

      for (LightmappedPrimitive*& command : queue) {
        if (command->vertexCount > 0) {
          bindTexture(0, command->texture);
          bindTexture(1, command->lightmap);
          this->DrawLightmappedPrimitive(command);
        }
      }

      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    } else {
      this->DrawBaseQueue();
      ScopedDeviceStateDX9 restoreLightmap = this->device->CaptureState();
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
      this->device->SetRenderState(D3DRS_ALPHAREF, 1);
      this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
      this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
      std::sort(this->queue_0174.begin(), this->queue_0174.end(), [](const RenderCommand* left, const RenderCommand* right) {
        return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
      });

      for (RenderCommand*& command : this->queue_0174) {
        bindTexture(0, command->texture);
        this->DrawPrimitive(command);
      }
    }
  } else {
    this->DrawBaseQueue();
  }

  this->device->SetRenderState(D3DRS_AMBIENT, 0);
  this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}
