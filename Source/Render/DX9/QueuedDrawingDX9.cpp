#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/RendererFogDX9.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <bit>
#include <cstdint>

namespace {

// INLINED
void BindQueueTexture(CRendererDX9& renderer, UINT stage, CTexture* texture) {
  if (texture) {
    renderer.device->SetTexture(stage, static_cast<CTextureDX9*>(texture)->NativeTexture());
  } else {
    renderer.device->SetTexture(stage, nullptr);
  }
}

}

// OFFSET: 0x00567180
// FUNCTION: fn_02228
void CRendererDX9::DrawAuxiliaryQueue() {
  auto& queue = this->queue_0240;

  if (queue.empty()) {
    return;
  }

  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  std::sort(queue.begin(), queue.end(), [](const LightmappedCommand* left, const LightmappedCommand* right) {
    const auto first = reinterpret_cast<std::uintptr_t>(left->texture);
    const auto second = reinterpret_cast<std::uintptr_t>(right->texture);

    if (first != second) {
      return first < second;
    }

    return reinterpret_cast<std::uintptr_t>(left->lightmap) < reinterpret_cast<std::uintptr_t>(right->lightmap);
  });
  this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
  this->device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  this->device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  this->device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  for (LightmappedCommand*& command : queue) {
    if (command->vertexCount > 0) {
      BindQueueTexture(*this, 0, command->texture);
      BindQueueTexture(*this, 1, command->lightmap);
      this->DrawAuxiliaryPrimitive(command);
    }
  }
}

// OFFSET: 0x00565C30
// FUNCTION: fn_46690
inline void CRendererDX9::DrawDepthSortedAlphaQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  std::sort(this->queue_0180.begin(), this->queue_0180.end());
  this->device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->device->SetRenderState(D3DRS_ZWRITEENABLE, 1);
  this->device->SetRenderState(D3DRS_ALPHAREF, 159);
  this->device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  this->DrawDepthQueue(false, this->queue_0180);
  this->device->SetRenderState(D3DRS_ZWRITEENABLE, 0);
  this->device->SetRenderState(D3DRS_ALPHAREF, 159);
  this->device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
  this->DrawDepthQueue(false, this->queue_0180);
}

// OFFSET: 0x00566120
// FUNCTION: fn_46691
inline void CRendererDX9::DrawNoDepthAlphaQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->DrawDepthQueue(false, this->queue_0198);
}

// OFFSET: 0x00566200
// FUNCTION: fn_46692
inline void CRendererDX9::DrawHighAlphaRefQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_ALPHAREF, 207);
  this->DrawDepthQueue(true, this->queue_0228);
}

// OFFSET: 0x00566440
// FUNCTION: fn_46693
inline void CRendererDX9::DrawDepthSortedAdditiveQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  this->DrawDepthQueue(true, this->queue_01C8);
}

// OFFSET: 0x00566520
// FUNCTION: fn_46694
inline void CRendererDX9::DrawAdditiveQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  this->DrawDepthQueue(true, this->queue_01D4);
}

// OFFSET: 0x00566600
// FUNCTION: fn_46695
inline void CRendererDX9::DrawNoDepthAdditiveQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  this->DrawDepthQueue(true, this->queue_01E0);
}

// OFFSET: 0x005673F0
// FUNCTION: fn_46696
inline void CRendererDX9::DrawTopmostAlphaQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();
  this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  this->DrawDepthQueue(true, this->queue_021C);
}

// OFFSET: 0x00566C00
// FUNCTION: fn_46701
inline void CRendererDX9::DrawNoDepthCustomBlendQueue() {
  ScopedDeviceStateDX9 restore = this->device->CaptureState();

  for (const DepthPrimitive& entry : this->queue_0234) {
    RenderCommand* command = entry.primitive;
    const float depth = entry.depth;
    BindQueueTexture(*this, 0, command->texture);
    this->SetTextureMode(command->textureMode);

    if (depth >= 0.99f) {
      this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    } else {
      this->device->SetRenderState(D3DRS_SRCBLEND, command->sourceBlend);
      this->device->SetRenderState(D3DRS_DESTBLEND, command->destinationBlend);
    }
    this->DrawPrimitive(command);
  }
}

// OFFSET: 0x00567610
// FUNCTION: fn_02229
void CRendererDX9::DrawQueues() {
  this->ApplyFog();
  this->device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->device->SetRenderState(D3DRS_ALPHAREF, 207);
  this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
  this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  this->device->SetRenderState(D3DRS_ZWRITEENABLE, 1);

  this->DrawOpaqueQueues();
  this->DrawGroundQueues();

  this->device->SetRenderState(D3DRS_ALPHAREF, 1);
  this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
  this->DrawAuxiliaryQueue();

  const auto drawCommand = [&](RenderCommand* const& command) {
    BindQueueTexture(*this, 0, command->texture);
    this->SetTextureMode(command->textureMode);
    this->device->SetRenderState(D3DRS_SRCBLEND, command->sourceBlend);
    this->device->SetRenderState(D3DRS_DESTBLEND, command->destinationBlend);
    this->DrawPrimitive(command);
  };

  {
    ScopedDeviceStateDX9 restore = this->device->CaptureState();
    this->device->SetRenderState(D3DRS_FOGENABLE, 0);
    this->device->SetRenderState(D3DRS_ZWRITEENABLE, 0);

    for (RenderCommand*& command : this->queue_0210) {
      if (command) {
        drawCommand(command);
      }
    }
  }

  this->DrawTopmostAlphaQueue();

  this->DrawDepthSortedAlphaQueue();

  this->DrawDepthSortedAdditiveQueue();

  this->device->SetRenderState(D3DRS_ZWRITEENABLE, 0);
  this->DrawAlphaQueues();

  this->DrawHighAlphaRefQueue();

  this->device->SetRenderState(D3DRS_FOGENABLE, 0);

  this->DrawAdditiveQueue();

  {
    ScopedDeviceStateDX9 restore = this->device->CaptureState();
    this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    this->device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    this->device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    for (RenderCommand*& command : this->queue_01EC) {
      drawCommand(command);
    }
  }

  this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

  this->DrawNoDepthAdditiveQueue();

  this->DrawNoDepthAlphaQueue();

  this->DrawNoDepthCustomBlendQueue();
}
