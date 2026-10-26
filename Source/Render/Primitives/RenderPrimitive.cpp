#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"

#include <cstring>
#include <d3d9types.h>
#include <new>

// GLOBAL: gl_24481
CRenderer* g_renderer;

// OFFSET: 0x00569270
// FUNCTION: fn_02251
RenderVertex* RenderVertex::Reset() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
  this->rhw = 0;
  this->diffuse = 0;
  this->specular = 0xFF000000;
  this->u = 0;
  this->v = 0;

  return this;
}

// OFFSET: 0x00569560
// FUNCTION: fn_02263
bool RenderTriangle::Visible() const {
  for (const RenderVertex& vertex : this->vertices) {
    if (vertex.rhw <= 0.0f) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00568F40
// FUNCTION: fn_02244
RenderCommand::RenderCommand()
    : vertices(nullptr), vertexCount(0), texture(nullptr), indices(nullptr), indexCount(0), unknown_0014(1), unknown_0018(0), unknown_001C(0), unknown_0020(0),
      primitiveType(D3DPT_TRIANGLELIST) {
}

// OFFSET: 0x005691D0
// FUNCTION: fn_02250
RenderPrimitive::RenderPrimitive() : RenderCommand() {
  this->vertices = this->quad;
  this->vertexCount = 4;
  this->primitiveType = D3DPT_TRIANGLESTRIP;

  for (RenderVertex& vertex : this->quad) {
    vertex.x = 0;
    vertex.y = 0;
    vertex.z = 0;
    vertex.rhw = 0;
    vertex.diffuse = 0;
    vertex.specular = 0xFF000000;
    vertex.u = 0;
    vertex.v = 0;
  }
}

// OFFSET: 0x00569120
// FUNCTION: fn_02249
RenderPrimitive::RenderPrimitive(const RenderPrimitive& other) : RenderPrimitive() {
  std::memcpy(this->quad, other.quad, sizeof(this->quad));
}

// OFFSET: 0x005692F0
// FUNCTION: fn_02253
RenderCommand* RenderCommand::FreeVertices() {
  if (this->vertices != nullptr) {
    ::operator delete(this->vertices);
    this->vertices = nullptr;
  }

  return this;
}
