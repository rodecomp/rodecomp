#include "Render/Primitives/DepthAlpha.h"
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
  for (std::size_t i = 0; i < 3; ++i) {
    if (this->vertices[i].rhw <= 0.0f) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00568F40
// FUNCTION: fn_02244
RenderCommand::RenderCommand() {
  this->vertices = nullptr;
  this->vertexCount = 0;
  this->texture = nullptr;
  this->indices = nullptr;
  this->indexCount = 0;
  this->depthTest = 1;
  this->commandMode = 0;
  this->sourceBlend = 0;
  this->destinationBlend = 0;
  this->textureMode = 0;
  this->primitiveType = D3DPT_TRIANGLELIST;
}

namespace {
  struct QuadVertex : RenderVertex {
    // INLINED
    QuadVertex() {
      this->Reset();
    }
  };
}

// OFFSET: 0x005691D0
// FUNCTION: fn_02250
RenderPrimitive::RenderPrimitive() : RenderCommand(0) {
  this->vertexCount = 4;
  this->vertices = this->quad;
  this->texture = nullptr;
  this->indices = nullptr;
  this->indexCount = 0;
  this->depthTest = 1;
  this->commandMode = 0;
  this->sourceBlend = 0;
  this->destinationBlend = 0;
  this->textureMode = 0;
  new (this->quad) QuadVertex[4];
  this->primitiveType = D3DPT_TRIANGLESTRIP;
}

// OFFSET: 0x00569120
// FUNCTION: fn_02249
RenderPrimitive::RenderPrimitive(const RenderPrimitive& other) : RenderPrimitive() {
  *this = other;
}

// OFFSET: 0x005692F0
// FUNCTION: fn_02253
void RenderCommand::FreeVertices() {
  if (this->vertices != nullptr) {
    delete this->vertices;
    this->vertices = nullptr;
  }
}

// OFFSET: 0x00569360
// FUNCTION: fn_02255
RenderPrimitive& RenderPrimitive::operator=(const RenderPrimitive& other) {
  std::memcpy(this->quad, other.quad, sizeof(this->quad));
  return *this;
}

// OFFSET: 0x00569380
// FUNCTION: fn_02256
void RenderCommand::AllocateTriangles(std::uint32_t triangleCount) {
  struct Vertex : RenderVertex {
    Vertex() {
      this->Reset();
    }
  };
  this->vertices = new Vertex[triangleCount * 3];
}

// OFFSET: 0x00569470
// FUNCTION: fn_02257
void RenderCommand::Draw(int unused) {
  g_renderer->DrawPrimitive(this);
}

// OFFSET: 0x005694A0
// FUNCTION: fn_02260
void RenderCommand::ApplyDepthAlpha() {
  for (int index = 0; index < this->vertexCount; ++index) {
    this->vertices[index].specular = DepthAlpha(this->vertices[index].rhw);
  }
}

// OFFSET: 0x00569580
// FUNCTION: fn_02264
void RenderCommand::SetPosition(std::uint32_t index, const float* position) {
  this->vertices[index].x = position[0];
  this->vertices[index].y = position[1];
  this->vertices[index].z = position[2];
  this->vertices[index].rhw = position[3];
}
