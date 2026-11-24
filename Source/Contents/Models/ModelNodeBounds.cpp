#include "Contents/Models/ModelNode.h"

#include <cstring>

// OFFSET: 0x00ABB300
// FUNCTION: fn_17514
void C3dNode::AccumulateBounds(float* bounds, const matrix& parent) {
  matrix combined;
  combined.Compose(this->transform, parent);

  const auto* vertices = this->mesh->vertices;
  for (int index = 0; index < this->mesh->vertexCount; ++index) {
    const auto& vertex = vertices[index];
    float x = vertex[0] * combined.values[0];
    x += vertex[1] * combined.values[3];
    x += vertex[2] * combined.values[6];
    x += combined.values[9];
    float y = vertex[1] * combined.values[4];
    y += vertex[0] * combined.values[1];
    y += vertex[2] * combined.values[7];
    y += combined.values[10];
    float z = vertex[0] * combined.values[2];
    z += vertex[1] * combined.values[5];
    z += vertex[2] * combined.values[8];
    z += combined.values[11];

    if (bounds[0] > x) {
      bounds[0] = x;
    }

    if (x > bounds[3]) {
      bounds[3] = x;
    }

    if (bounds[1] > y) {
      bounds[1] = y;
    }

    if (y > bounds[4]) {
      bounds[4] = y;
    }

    if (bounds[2] > z) {
      bounds[2] = z;
    }

    if (z > bounds[5]) {
      bounds[5] = z;
    }
  }

  for (auto childIt = this->children.begin(); childIt != this->children.end(); ++childIt) {
    C3dNode* child = *childIt;
    child->AccumulateBounds(bounds, combined);
  }
}
