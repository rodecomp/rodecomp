#include "Render/Primitives/DepthAlpha.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Renderer.h"

#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <d3d9types.h>
#include <new>

// OFFSET: 0x005690E0
// FUNCTION: fn_02248
LightmappedCommand::LightmappedCommand()
    : vertices(nullptr), vertexCount(0), indices(nullptr), indexCount(0), texture(nullptr), lightmap(nullptr), primitiveType(D3DPT_TRIANGLESTRIP) {
}

// OFFSET: 0x00569090
// FUNCTION: fn_02247
ModelRenderCommand::ModelRenderCommand()
    : vertices(nullptr), vertexCount(0), texture(nullptr), indices(nullptr), indexCount(0), unknown_0014(1), unknown_0018(0), unknown_001C(0), unknown_0020(0),
      primitiveType(D3DPT_TRIANGLELIST) {
}

struct LightmappedBuffer {
  /* 0000 */ void* vertices;
  /* 0004 */ int count;
  /* 0008 */ void* indices;
  void Clear();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(LightmappedBuffer, indices) == 0x8);
#endif

// OFFSET: 0x00569310
// FUNCTION: fn_02254
void LightmappedBuffer::Clear() {
  std::free(this->vertices);
  this->vertices = nullptr;
  std::free(this->indices);
  this->indices = nullptr;
}

// OFFSET: 0x00569020
// FUNCTION: fn_02246
LightmappedPrimitive::LightmappedPrimitive() : vertices(this->quad), vertexCount(4), indices(nullptr), indexCount(0), primitiveType(D3DPT_TRIANGLESTRIP) {
}

// OFFSET: 0x00568F90
// FUNCTION: fn_02245
LightmappedPrimitive::LightmappedPrimitive(const LightmappedPrimitive& other)
    : vertices(this->quad), vertexCount(4), indices(nullptr), indexCount(0), primitiveType(D3DPT_TRIANGLESTRIP) {
  std::memcpy(this->quad, other.quad, sizeof(this->quad));
}

// OFFSET: 0x00569480
// FUNCTION: fn_02258
void LightmappedPrimitive::DrawLightmapped(int unused) {
  g_renderer->DrawLightmappedPrimitive(this);
}

// OFFSET: 0x00569490
// FUNCTION: fn_02259
void LightmappedPrimitive::DrawAuxiliary(int unused) {
  g_renderer->DrawAuxiliaryPrimitive(this);
}

// OFFSET: 0x00559BE0
// FUNCTION: fn_02071
void LightmappedQueues::Submit(LightmappedPrimitive* primitive, int queue, bool depthAlpha, float depthStart, float depthEnd) {
  if (depthAlpha) {
    primitive->ApplyDepthAlpha();
  }

  if (queue == 0) {
    this->first.push_back(primitive);
  } else if (queue == 1) {
    this->second.push_back(primitive);
  }
}
