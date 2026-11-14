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
    : vertices(nullptr), vertexCount(0), texture(nullptr), indices(nullptr), indexCount(0), renderEnabled(1), renderStateA(0), renderStateB(0), renderStateC(0),
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
  delete[] static_cast<char*>(this->vertices);
  this->vertices = nullptr;
  delete[] static_cast<char*>(this->indices);
  this->indices = nullptr;
}

// OFFSET: 0x00569020
// FUNCTION: fn_02246
LightmappedPrimitive::LightmappedPrimitive() : LightmappedCommand(0) {
  this->vertices = this->quad;
  this->vertexCount = 4;
  this->indices = nullptr;
  this->indexCount = 0;
  this->primitiveType = D3DPT_TRIANGLESTRIP;
}

// OFFSET: 0x00568F90
// FUNCTION: fn_02245
LightmappedPrimitive::LightmappedPrimitive(const LightmappedPrimitive& other) : LightmappedCommand(0) {
  this->vertices = this->quad;
  this->vertexCount = 4;
  this->indices = nullptr;
  this->indexCount = 0;
  this->primitiveType = D3DPT_TRIANGLESTRIP;
  std::memcpy(this->quad, other.quad, sizeof(this->quad));
}

// OFFSET: 0x00569480
// FUNCTION: fn_02258
void LightmappedPrimitive::DrawLightmapped(int unused) {
  g_renderer->DrawLightmappedPrimitive(this);
}

// OFFSET: 0x00569490
// FUNCTION: fn_02259
void LightmappedCommand::DrawAuxiliary(int unused) {
  g_renderer->DrawAuxiliaryPrimitive(this);
}
