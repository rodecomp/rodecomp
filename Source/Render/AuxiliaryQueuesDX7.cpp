#include "Render/DX7/QueuedDrawingDX7.h"

#include <algorithm>
#include <cstdint>

// OFFSET: 0x0055BE40
// FUNCTION: fn_02093
void QueuedDrawingDX7::DrawAuxiliaryQueue() {
  auto& queue = this->queues.queue_0240;

  if (queue.empty()) {
    return;
  }

  std::sort(queue.begin(), queue.end(), [](const LightmappedPrimitive* left, const LightmappedPrimitive* right) {
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
  this->device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
  this->device->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
  this->device->SetTextureStageState(1, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);

  for (auto it = queue.begin(); it != queue.end(); ++it) {
    LightmappedPrimitive* command = *it;

    if (command->vertexCount > 0) {
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      this->BindLightmap(static_cast<CTextureDX7*>(command->lightmap));
      command->DrawAuxiliary(reinterpret_cast<int>(this->device));
    }
  }

  this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
  this->device->SetTextureStageState(1, D3DTSS_ADDRESS, D3DTADDRESS_BORDER);
  this->device->SetTexture(1, nullptr);
}
