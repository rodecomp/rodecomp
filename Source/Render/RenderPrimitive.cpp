#include "Render/RenderPrimitive.h"

#include <cstring>

// OFFSET: 0x00569560
// FUNCTION: fn_02263
bool RenderTriangle::Visible() const {
  for (const RenderVertex& vertex : this->vertices) {
    // COMISS 0, rhw / JNB rejects <= 0, but not unordered values.
    if (vertex.rhw <= 0.0f) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00568F40
// FUNCTION: fn_02244
RenderCommand::RenderCommand()
    : vertices(nullptr), vertexCount(0), texture(nullptr), indices(nullptr), indexCount(0), unknown_0014(1),
      unknown_0018(0), unknown_001C(0), unknown_0020(0), primitiveType(4) {
}

// OFFSET: 0x005691D0
// FUNCTION: fn_02250
RenderPrimitive::RenderPrimitive() : RenderCommand() {
  this->vertices = this->quad;
  this->vertexCount = 4;
  this->primitiveType = 5;
  // Original writes only the low word at+0x14; +0x16 remains untouched.
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
