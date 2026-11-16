#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H

#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"

#include <vector>

struct DepthPrimitive {
  /* 0000 */ float depth;
  /* 0004 */ RenderCommand* primitive;
};

struct RenderQueues {
  /* 015C */ std::vector<RenderCommand*> queue_015C;
  /* 0168 */ std::vector<RenderCommand*> queue_0168;
  /* 0174 */ std::vector<RenderCommand*> queue_0174;
  /* 0180 */ std::vector<DepthPrimitive> queue_0180;
  /* 018C */ std::vector<DepthPrimitive> queue_018C;
  /* 0198 */ std::vector<DepthPrimitive> queue_0198;
  /* 01A4 */ std::vector<DepthPrimitive> queue_01A4;
  /* 01B0 */ std::vector<RenderCommand*> queue_01B0;
  /* 01BC */ std::vector<DepthPrimitive> queue_01BC;
  /* 01C8 */ std::vector<DepthPrimitive> queue_01C8;
  /* 01D4 */ std::vector<DepthPrimitive> queue_01D4;
  /* 01E0 */ std::vector<DepthPrimitive> queue_01E0;
  /* 01EC */ std::vector<RenderCommand*> queue_01EC;
  /* 01F8 */ LightmappedQueues lightmapped;
  /* 0210 */ std::vector<RenderCommand*> queue_0210;
  /* 021C */ std::vector<DepthPrimitive> queue_021C;
  /* 0228 */ std::vector<DepthPrimitive> queue_0228;
  /* 0234 */ std::vector<DepthPrimitive> queue_0234;
  /* 0240 */ std::vector<LightmappedCommand*> queue_0240;

  // INLINED
  __forceinline RenderQueues() = default;

  // INLINED
  void Reserve(unsigned count) {
    this->lightmapped.first.reserve(count);
    this->queue_015C.reserve(count);
    this->queue_0168.reserve(count);
    this->queue_0174.reserve(count);
    this->queue_0180.reserve(count);
    this->queue_018C.reserve(count);
    this->queue_0198.reserve(count);
    this->queue_01A4.reserve(count);
    this->queue_01B0.reserve(count);
    this->queue_01BC.reserve(count);
    this->queue_01C8.reserve(count);
    this->queue_01D4.reserve(count);
    this->queue_01E0.reserve(count);
    this->queue_01EC.reserve(count);
    this->lightmapped.second.reserve(count);
    this->queue_0210.reserve(count);
    this->queue_021C.reserve(count);
    this->queue_0228.reserve(count);
    this->queue_0234.reserve(count);
    this->queue_0240.reserve(count);
  }

  // INLINED
  __forceinline ~RenderQueues() = default;

  void Clear();
};

// INLINED
inline void RenderQueues::Clear() {
  this->queue_015C.clear();
  this->queue_0168.clear();
  this->queue_0174.clear();
  this->queue_0180.clear();
  this->queue_018C.clear();
  this->queue_0198.clear();
  this->queue_01A4.clear();
  this->queue_01B0.clear();
  this->queue_01BC.clear();
  this->queue_01C8.clear();
  this->queue_01D4.clear();
  this->queue_01E0.clear();
  this->queue_01EC.clear();
  this->lightmapped.Clear();
  this->queue_0210.clear();
  this->queue_021C.clear();
  this->queue_0228.clear();
  this->queue_0234.clear();
  this->queue_0240.clear();
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DepthPrimitive) == 8);
static_assert(sizeof(RenderQueues) == 240);
static_assert(offsetof(DepthPrimitive, primitive) == 0x0004);
static_assert(offsetof(RenderQueues, lightmapped) + 0x015C == 0x01F8);
static_assert(offsetof(RenderQueues, queue_0240) + 0x015C == 0x0240);
#endif

#endif
