#include "Contents/Models/ModelNode.h"

#include <cstring>

// OFFSET: 0x00ABB300
// FUNCTION: fn_17514
void C3dNode::AccumulateBounds(float* bounds, const float* matrix) {
  AffineMatrix parentMatrix;
  std::memcpy(parentMatrix.values, matrix, sizeof(parentMatrix.values));
  AffineMatrix combined;
  combined.Compose(this->transform, parentMatrix);

  for (const auto& vertex : this->mesh->vertices) {
    const float x = ((vertex[0] * combined.values[0] + vertex[1] * combined.values[3]) + vertex[2] * combined.values[6]) + combined.values[9];
    const float y = ((vertex[1] * combined.values[4] + vertex[0] * combined.values[1]) + vertex[2] * combined.values[7]) + combined.values[10];
    const float z = ((vertex[0] * combined.values[2] + vertex[1] * combined.values[5]) + vertex[2] * combined.values[8]) + combined.values[11];

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

  for (C3dNode* child : this->children) {
    child->AccumulateBounds(bounds, combined.values);
  }
}
