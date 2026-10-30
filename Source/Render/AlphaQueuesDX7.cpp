#include "Render/DX7/QueuedDrawingDX7.h"

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
    return (command.unknown_0014 & 0xFFu) != 0 && (command.unknown_0014 & 0xFF00u) != 0;
  }
}

// OFFSET: 0x0055B160
// FUNCTION: fn_02089
void QueuedDrawingDX7::DrawAlphaQueues() {
  const auto before = [](const DepthPrimitive& left, const DepthPrimitive& right) {
    return Before(left, right);
  };

  std::sort(this->queues.queue_018C.begin(), this->queues.queue_018C.end(), before);
  std::sort(this->queues.queue_01BC.begin(), this->queues.queue_01BC.end(), before);

  DWORD zWrite = 0;
  DWORD alphaTest = 0;
  DWORD alphaReference = 0;
  DWORD alphaFunction = 0;
  DWORD alphaBlend = 0;
  DWORD sourceBlend = 0;
  DWORD destinationBlend = 0;
  this->device->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &zWrite);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHAREF, &alphaReference);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHAFUNC, &alphaFunction);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend);
  this->device->GetRenderState(D3DRENDERSTATE_SRCBLEND, &sourceBlend);
  this->device->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destinationBlend);

  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 1);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);

  const auto drawAlphaTested = [this](const DepthPrimitive& entry) {
    RenderCommand* command = entry.primitive;
    this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, WritesDepth(*command) ? 1 : 240);
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  };

  if (this->twoPass == 1) {
    this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
    this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
    std::for_each(this->queues.queue_018C.begin(), this->queues.queue_018C.end(), drawAlphaTested);
    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
    std::for_each(this->queues.queue_01BC.begin(), this->queues.queue_01BC.end(), drawAlphaTested);

    this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
    this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_LESS);
    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
    std::for_each(this->queues.queue_018C.begin(), this->queues.queue_018C.end(), drawAlphaTested);
    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
    std::for_each(this->queues.queue_01BC.begin(), this->queues.queue_01BC.end(), drawAlphaTested);

    this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 1);
    this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    std::sort(this->queues.queue_01A4.begin(), this->queues.queue_01A4.end(), before);

    for (auto it = this->queues.queue_01A4.begin(); it != this->queues.queue_01A4.end(); ++it) {
      RenderCommand* command = it->primitive;
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      command->Draw(reinterpret_cast<int>(this->device));
    }
  } else {
    const auto drawDepthWriting = [this](const DepthPrimitive& entry) {
      RenderCommand* command = entry.primitive;
      this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, WritesDepth(*command));
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      command->Draw(reinterpret_cast<int>(this->device));
    };

    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
    std::for_each(this->queues.queue_018C.begin(), this->queues.queue_018C.end(), drawDepthWriting);
    this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
    std::for_each(this->queues.queue_01BC.begin(), this->queues.queue_01BC.end(), drawDepthWriting);
  }

  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, zWrite);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, alphaReference);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunction);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, sourceBlend);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, destinationBlend);
  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
}

// OFFSET: 0x0055B710
// FUNCTION: fn_02090
void __stdcall Nullsub99() {
}
