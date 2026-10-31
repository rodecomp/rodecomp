#include "Render/RendererDX7.h"

#include <timeapi.h>

// OFFSET: 0x00559420
// FUNCTION: fn_02065
CRendererDX7::CRendererDX7(CRenderSystemBase& system) : CRenderer(system) {
  this->frame = 0;
  this->unknown_025C = 0;
  this->time = timeGetTime();
  this->unknown_0264 = 0;
  this->unknown_0268 = 0;
  this->unknown_026C = 0;
  this->fog.enabled = 0;
  this->fog.supported = 0;
  this->unknown_004C = 0;

  RenderQueues& queues = this->queues;
  if (queues.lightmapped.first.capacity() < 1000) {
    queues.lightmapped.first.reserve(1000);
  }
  if (queues.queue_015C.capacity() < 1000) {
    queues.queue_015C.reserve(1000);
  }
  if (queues.queue_0168.capacity() < 1000) {
    queues.queue_0168.reserve(1000);
  }
  if (queues.queue_0174.capacity() < 1000) {
    queues.queue_0174.reserve(1000);
  }
  if (queues.queue_0180.capacity() < 1000) {
    queues.queue_0180.reserve(1000);
  }
  if (queues.queue_018C.capacity() < 1000) {
    queues.queue_018C.reserve(1000);
  }
  if (queues.queue_0198.capacity() < 1000) {
    queues.queue_0198.reserve(1000);
  }
  if (queues.queue_01A4.capacity() < 1000) {
    queues.queue_01A4.reserve(1000);
  }
  if (queues.queue_01B0.capacity() < 1000) {
    queues.queue_01B0.reserve(1000);
  }
  if (queues.queue_01BC.capacity() < 1000) {
    queues.queue_01BC.reserve(1000);
  }
  if (queues.queue_01C8.capacity() < 1000) {
    queues.queue_01C8.reserve(1000);
  }
  if (queues.queue_01D4.capacity() < 1000) {
    queues.queue_01D4.reserve(1000);
  }
  if (queues.queue_01E0.capacity() < 1000) {
    queues.queue_01E0.reserve(1000);
  }
  if (queues.queue_01EC.capacity() < 1000) {
    queues.queue_01EC.reserve(1000);
  }
  if (queues.lightmapped.second.capacity() < 1000) {
    queues.lightmapped.second.reserve(1000);
  }
  if (queues.queue_0210.capacity() < 1000) {
    queues.queue_0210.reserve(1000);
  }
  if (queues.queue_021C.capacity() < 1000) {
    queues.queue_021C.reserve(1000);
  }
  if (queues.queue_0228.capacity() < 1000) {
    queues.queue_0228.reserve(1000);
  }
  if (queues.queue_0240.capacity() < 1000) {
    queues.queue_0240.reserve(1000);
  }
  this->twoPass = 0;
  if (queues.queue_0234.capacity() < 1000) {
    queues.queue_0234.reserve(1000);
  }
}

// OFFSET: 0x0055EC00
// FUNCTION: fn_02113
void CRendererDX7::SetClearColor(unsigned color) {
  this->clearColor = color;
}

// OFFSET: 0x0055ED40
// FUNCTION: fn_02115
void CRendererDX7::SetCamera(const float* eye, const float* target, const float* up) {
}

// OFFSET: 0x0055F690
// FUNCTION: fn_02117
void CRendererDX7::SetTextureFormat(unsigned format) {
  this->unknown_024C = format;
}
