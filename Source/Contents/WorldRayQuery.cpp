#include "Contents/WorldRayQuery.h"
#include "Contents/WorldSpatialTree.h"

#include <bit>
#include <emmintrin.h>

// OFFSET: 0x00AC7C50
// FUNCTION: fn_17661
bool WorldRayQuery::Intersects(const float* bounds) const {
  const float centers[3] = {
    (bounds[3] + bounds[0]) * 0.5f, (bounds[4] + bounds[1]) * 0.5f, (bounds[5] + bounds[2]) * 0.5f
  };
  const float extents[3] = {bounds[3] - centers[0], bounds[4] - centers[1], bounds[5] - centers[2]};
  const float offsets[3] = {centers[0] - this->origin[0], centers[1] - this->origin[1], centers[2] - this->origin[2]};
  float entry = -999999.0f;
  float exit = 999999.0f;
  constexpr float epsilon = 0.00001f;
  constexpr float parallelScale = 0.7f;

  for (unsigned axis = 0; axis < 3; ++axis) {
    const float direction = this->direction[axis];
    const float magnitude = std::bit_cast<float>(std::bit_cast<unsigned>(direction) & 0x7FFFFFFFu);

    // COMISS/JBE takes the parallel path for unordered directions too.
    if (!(magnitude > epsilon)) {
      const float extent = extents[axis] * parallelScale;

      if (-offsets[axis] - extent > 0.0f || extent - offsets[axis] < 0.0f) {
        return false;
      }
    } else {
      const float inverse = 1.0f / direction;
      const float first = (offsets[axis] + extents[axis]) * inverse;
      const float second = (offsets[axis] - extents[axis]) * inverse;
      const bool orderedDescending = first > second;
      const float near = orderedDescending ? second : first;
      const float far = orderedDescending ? first : second;
      // Operand order matters for unordered values and signed zero.
      entry = _mm_cvtss_f32(_mm_max_ss(_mm_set_ss(near), _mm_set_ss(entry)));
      exit = _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(far), _mm_set_ss(exit)));

      if (entry > exit || exit < 0.0f) {
        return false;
      }
    }
  }

  // Inside-origin queries use the exit distance, not a clamped zero entry.
  return !((entry > 0.0f ? entry : exit) > this->limit);
}

// OFFSET: 0x00AC7750
// FUNCTION: fn_17658
void WorldRayQuery::Collect(const float* ray, WorldSpatialTree* root) {
  this->direction[0] = ray[3];
  this->direction[1] = ray[4];
  this->direction[2] = ray[5];
  this->origin[0] = ray[0];
  this->origin[1] = ray[1];
  this->origin[2] = ray[2];
  this->limit = 999999.0f;

  if (root) {
    for (WorldSpatialTree* child : root->children) {
      if (child && this->Intersects(child->minimum)) {
        this->CollectNode(child, 1);
      }
    }
  }
}

// OFFSET: 0x00AC7F70
// FUNCTION: fn_17663
void WorldRayQuery::CollectNode(WorldSpatialTree* node, int depth) {
  if (!node) {
    return;
  }

  if (depth == 5) {
    this->nodes.push_back(node);
  } else {
    for (WorldSpatialTree* child : node->children) {
      if (child && this->Intersects(child->minimum)) {
        this->CollectNode(child, depth + 1);
      }
    }
  }
}
