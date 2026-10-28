#include "Render/DX7/QueuedDrawingDX7.h"

#include <algorithm>
#include <cstdint>

// OFFSET: 0x0055B0C0
// FUNCTION: fn_02088
void QueuedDrawingDX7::DrawDepthSortedQueue() {
  std::sort(this->queues.queue_0180.begin(), this->queues.queue_0180.end(), [](const DepthPrimitive& left, const DepthPrimitive& right) {
    if (left.depth < right.depth) {
      return true;
    }

    if (right.depth < left.depth) {
      return false;
    }

    return left.depth == right.depth && reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
  });

  for (auto it = this->queues.queue_0180.begin(); it != this->queues.queue_0180.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }
}

// OFFSET: 0x0055B9E0
// FUNCTION: fn_02091
void QueuedDrawingDX7::DrawOpaqueQueues() {
  const auto before = [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  };

  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
  std::sort(this->queues.queue_015C.begin(), this->queues.queue_015C.end(), before);

  for (auto it = this->queues.queue_015C.begin(); it != this->queues.queue_015C.end(); ++it) {
    if (RenderCommand* command = *it) {
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      command->Draw(reinterpret_cast<int>(this->device));
    }
  }

  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
  std::sort(this->queues.queue_01B0.begin(), this->queues.queue_01B0.end(), before);

  for (auto it = this->queues.queue_01B0.begin(); it != this->queues.queue_01B0.end(); ++it) {
    if (RenderCommand* command = *it) {
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      command->Draw(reinterpret_cast<int>(this->device));
    }
  }

  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
}

// OFFSET: 0x0055BC40
// FUNCTION: fn_02092
void QueuedDrawingDX7::DrawGroundBaseQueue() {
  std::sort(this->queues.queue_0168.begin(), this->queues.queue_0168.end(), [](const RenderCommand* left, const RenderCommand* right) {
    return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
  });

  for (auto it = this->queues.queue_0168.begin(); it != this->queues.queue_0168.end(); ++it) {
    RenderCommand* command = *it;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }
}
