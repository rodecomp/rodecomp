#include "Render/DepthAlpha.h"
#include "Render/LightmappedPrimitive.h"

#include <cstring>
#include <new>

// OFFSET: 0x005690E0
// FUNCTION: fn_02248
LightmappedCommand::LightmappedCommand()
    : vertices(nullptr), vertexCount(0), indices(nullptr), indexCount(0), texture(nullptr), lightmap(nullptr),
      primitiveType(5) {
}

// OFFSET: 0x00569020
// FUNCTION: fn_02246
LightmappedPrimitive::LightmappedPrimitive()
    : vertices(this->quad), vertexCount(4), indices(nullptr), indexCount(0), primitiveType(5) {
  for (LightmappedVertex& vertex : this->quad) {
    vertex.x = 0.0f;
    vertex.y = 0.0f;
    vertex.z = 0.0f;
    vertex.rhw = 0.0f;
    vertex.diffuse = 0;
    vertex.specular = 0xFF000000u;
    vertex.u = 0.0f;
    vertex.v = 0.0f;
    // The original leaves the second UV pair and texture pointers untouched.
  }
}

// OFFSET: 0x00568F90
// FUNCTION: fn_02245
LightmappedPrimitive::LightmappedPrimitive(const LightmappedPrimitive& other) : LightmappedPrimitive() {
  std::memcpy(this->quad, other.quad, sizeof(this->quad));
}

// ORIGINAL_SITE: 0x00561113 in fn_02161 (fragment)
LightmappedPrimitivePool::LightmappedPrimitivePool() {
  this->m_commands.resize(8192, LightmappedPrimitive());
  this->m_current = this->m_commands.begin();
}

// OFFSET: 0x00AC1D30
// FUNCTION: fn_17592
LightmappedPrimitive* LightmappedPrimitivePool::Acquire() {
  if (this->m_current == this->m_commands.end()) {
    this->m_commands.emplace_back();
    this->m_current = this->m_commands.end();
    return &this->m_commands.back();
  }

  LightmappedPrimitive* primitive = &*this->m_current++;
  return new (primitive) LightmappedPrimitive;
}

void LightmappedPrimitivePool::Rewind() {
  this->m_current = this->m_commands.begin();
}

// OFFSET: 0x00559BE0
// FUNCTION: fn_02071
void LightmappedQueues::Submit(
  LightmappedPrimitive* primitive,
  int queue,
  bool depthAlpha,
  float depthStart,
  float depthEnd
) {
  if (depthAlpha) {
    ApplyDepthAlpha(*primitive, depthStart, depthEnd);
  }

  if (queue == 0) {
    this->first.push_back(primitive);
  } else if (queue == 1) {
    this->second.push_back(primitive);
  }
}

void LightmappedQueues::Clear() {
  this->first.clear();
  this->second.clear();
}
