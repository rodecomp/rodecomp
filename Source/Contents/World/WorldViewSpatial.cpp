#include "Contents/World/WorldModelView.h"
#include "Contents/World/WorldSpatialTree.h"

#include <bit>

namespace {
  // INLINED
  float Horizontal(float x, const float* row) {
    return (x * row[0] + row[1] * 0.0f) + row[2];
  }
}

// OFFSET: 0x00ADADE0
// FUNCTION: fn_17913
WorldViewSpatial::WorldViewSpatial() {
  for (auto& normal : this->planes.normals) {
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 0;
  }
}

// OFFSET: 0x00ADAF20
// FUNCTION: fn_17914
WorldViewSpatial::~WorldViewSpatial() = default;

// OFFSET: 0x00ADB0D0
// FUNCTION: fn_17919
void WorldViewSpatial::Collect(float horizontal, float vertical, const float* matrix, WorldSpatialTree& root) {
  const float negativeX = -horizontal;
  const float negativeY = -vertical;

  for (unsigned plane = 0; plane < 6; ++plane) {
    for (unsigned axis = 0; axis < 4; ++axis) {
      const float* row = matrix + axis * 3;
      float value;

      if (plane == 0 || plane == 1) {
        const float x = plane == 0 ? negativeX : horizontal;
        value = Horizontal(x, row);

        if (axis == 3) {
          value = value + 0.0f;
        }
      } else if (plane == 2 || plane == 3) {
        const float y = plane == 2 ? negativeY : vertical;
        value = (y * row[1] + row[0] * 0.0f) + row[2];

        if (axis == 3) {
          value = value + 0.0f;
        }
      } else if (plane == 4) {
        value = (row[0] * 0.0f + row[1] * 0.0f) + row[2];

        if (axis == 3) {
          value = value - 10.0f;
        }
      } else {
        value = (row[0] * 0.0f + row[1] * 0.0f) - row[2] * 1.0f;

        if (axis == 3) {
          value = value + 1500.0f;
        }
      }

      this->planes.planes[plane][axis] = value;
    }

    for (unsigned axis = 0; axis < 3; ++axis) {
      this->planes.normals[plane][axis] = this->planes.planes[plane][axis];
    }
  }

  this->CollectNode(root, 0, false);
}

// OFFSET: 0x00ADB910
// FUNCTION: fn_17923
void WorldViewSpatial::CollectNode(WorldSpatialTree& node, int depth, bool fullyInside) {
  if (depth == 5) {
    (fullyInside ? this->inside : this->clipped).push_back(&node);
    return;
  }

  for (WorldSpatialTree* child : node.children) {
    if (fullyInside) {
      this->CollectNode(*child, depth + 1, true);
      continue;
    }

    const int visibility = this->planes.ClassifyAxisAligned(*child);

    if (visibility != 0) {
      this->CollectNode(*child, depth + 1, visibility == 1);
    }
  }
}
