#include "Contents/World/WorldRayQuery.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Utility/Math/Vector3d.h"

#include <cmath>

// OFFSET: 0x00AC6450
// FUNCTION: fn_17650
WorldRayQuery::WorldRayQuery() {
  this->origin[0] = 0.0f;
  this->origin[1] = 0.0f;
  this->origin[2] = 0.0f;
  this->direction[0] = 0.0f;
  this->direction[1] = 0.0f;
  this->direction[2] = 0.0f;
}

// OFFSET: 0x00AC7C50
// FUNCTION: fn_17661
int WorldRayQuery::Intersects(const float* bounds) const {
  float entry = -999999.0f;
  float exit = 999999.0f;
  const vector3d center = {(bounds[3] + bounds[0]) * 0.5f, (bounds[4] + bounds[1]) * 0.5f, (bounds[5] + bounds[2]) * 0.5f};
  const vector3d extent = {bounds[3] - center.x, bounds[4] - center.y, bounds[5] - center.z};
  const vector3d offset = {center.x - this->origin[0], center.y - this->origin[1], center.z - this->origin[2]};

  if (std::fabs(this->direction[0]) > 0.00001f) {
    const float inverse = 1.0f / this->direction[0];
    const float first = (offset.x + extent.x) * inverse;
    const float second = (offset.x - extent.x) * inverse;

    if (first > second) {
      if (second > entry) {
        entry = second;
      }

      if (first < exit) {
        exit = first;
      }
    } else {
      if (first > entry) {
        entry = first;
      }

      if (second < exit) {
        exit = second;
      }
    }

    if (entry > exit) {
      return 0;
    }

    if (exit < 0.0f) {
      return 0;
    }
  } else {
    const float scaled = extent.x * 0.7f;

    if (-offset.x - scaled > 0.0f) {
      return 0;
    }

    if (scaled - offset.x < 0.0f) {
      return 0;
    }
  }

  if (std::fabs(this->direction[1]) > 0.00001f) {
    const float inverse = 1.0f / this->direction[1];
    const float first = (offset.y + extent.y) * inverse;
    const float second = (offset.y - extent.y) * inverse;

    if (first > second) {
      if (second > entry) {
        entry = second;
      }

      if (first < exit) {
        exit = first;
      }
    } else {
      if (first > entry) {
        entry = first;
      }

      if (second < exit) {
        exit = second;
      }
    }

    if (entry > exit) {
      return 0;
    }

    if (exit < 0.0f) {
      return 0;
    }
  } else {
    const float scaled = extent.y * 0.7f;

    if (-offset.y - scaled > 0.0f) {
      return 0;
    }

    if (scaled - offset.y < 0.0f) {
      return 0;
    }
  }

  if (std::fabs(this->direction[2]) > 0.00001f) {
    const float inverse = 1.0f / this->direction[2];
    const float first = (offset.z + extent.z) * inverse;
    const float second = (offset.z - extent.z) * inverse;

    if (first > second) {
      if (second > entry) {
        entry = second;
      }

      if (first < exit) {
        exit = first;
      }
    } else {
      if (first > entry) {
        entry = first;
      }

      if (second < exit) {
        exit = second;
      }
    }

    if (entry > exit) {
      return 0;
    }

    if (exit < 0.0f) {
      return 0;
    }
  } else {
    const float scaled = extent.z * 0.7f;

    if (-offset.z - scaled > 0.0f) {
      return 0;
    }

    if (scaled - offset.z < 0.0f) {
      return 0;
    }
  }

  if (entry > 0.0f) {
    if (entry > this->limit) {
      return 0;
    }
  } else if (exit > this->limit) {
    return 0;
  }

  return 1;
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
    for (int index = 0; index < 4; ++index) {
      if (root->children[index] && this->Intersects(root->children[index]->minimum)) {
        this->CollectNode(root->children[index], 1);
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
    for (int index = 0; index < 4; ++index) {
      if (node->children[index] && this->Intersects(node->children[index]->minimum)) {
        this->CollectNode(node->children[index], depth + 1);
      }
    }
  }
}
