#include "Contents/Camera/Camera.h"
#include "Contents/World/WorldModelView.h"
#include "Contents/World/WorldSpatialTree.h"

#include <bit>
#include <cmath>

// OFFSET: 0x00ADADE0
// FUNCTION: fn_17913
WorldViewSpatial::WorldViewSpatial() {
}

// OFFSET: 0x00ADAC70
// FUNCTION: fn_17912
CView::CView() {
  this->camera.yaw = 0.0f;
  this->shake.enabled = 0;
  this->skyBox = nullptr;
  this->shake.start = 0;
  *reinterpret_cast<unsigned char*>(&this->shake.resetFlags) = 0;
}

// OFFSET: 0x00ADAF20
// FUNCTION: fn_17914
WorldViewSpatial::~WorldViewSpatial() = default;

struct plane3d {
  float x;
  float y;
  float z;
  float w;

  // INLINED
  plane3d() {}
  // INLINED
  void Set(float nx, float ny, float nz, float nw) {
    x = nx;
    y = ny;
    z = nz;
    w = nw;
  }
};

// OFFSET: 0x00ADB0D0
// FUNCTION: fn_17919
void WorldViewSpatial::Collect(float horizontal, float vertical, const float* matrix, WorldSpatialTree& root) {
  plane3d viewPlanes[6];
  viewPlanes[0].Set(-horizontal, 0.0f, 1.0f, 0.0f);
  viewPlanes[1].Set(horizontal, 0.0f, 1.0f, 0.0f);
  viewPlanes[2].Set(0.0f, -vertical, 1.0f, 0.0f);
  viewPlanes[3].Set(0.0f, vertical, 1.0f, 0.0f);
  viewPlanes[4].Set(0.0f, 0.0f, 1.0f, -10.0f);
  viewPlanes[5].Set(0.0f, 0.0f, -1.0f, 1500.0f);

  for (int i = 0; i < 6; ++i) {
    const plane3d& plane = viewPlanes[i];
    float* out = this->planes.planes[i];

    out[0] = plane.x * matrix[0] + plane.y * matrix[1] + plane.z * matrix[2];
    out[1] = plane.x * matrix[3] + plane.y * matrix[4] + plane.z * matrix[5];
    out[2] = plane.x * matrix[6] + plane.y * matrix[7] + plane.z * matrix[8];
    out[3] = plane.x * matrix[9] + plane.y * matrix[10] + plane.z * matrix[11] + plane.w;

    this->planes.normals[i].x = out[0];
    this->planes.normals[i].y = out[1];
    this->planes.normals[i].z = out[2];
  }

  this->CollectNode(&root, 0, false);
}

// OFFSET: 0x00ADB730
// FUNCTION: fn_17921
int WorldViewSpatial::ClassifyAxisAligned(const WorldSpatialTree& bounds) const {
  bool clipped = false;

  for (int plane = 0; plane < 6; ++plane) {
    const float radius = (std::fabs(this->planes.normals[plane].x * bounds.halfSize[0]) + std::fabs(this->planes.normals[plane].y * bounds.halfSize[1])) + std::fabs(this->planes.normals[plane].z * bounds.halfSize[2]);
    const float distance = ((this->planes.normals[plane].x * bounds.center[0] + this->planes.normals[plane].y * bounds.center[1]) + this->planes.normals[plane].z * bounds.center[2]) + this->planes.planes[plane][3];

    if (!(distance > radius)) {
      const float negative = -radius;

      if (!(distance > negative)) {
        return MODEL_VIEW_OUTSIDE;
      }

      clipped = true;
    }
  }

  return clipped ? MODEL_VIEW_CLIPPED : MODEL_VIEW_INSIDE;
}

// OFFSET: 0x00ADB910
// FUNCTION: fn_17923
void WorldViewSpatial::CollectNode(WorldSpatialTree* node, int depth, bool fullyInside) {
  if (depth == 5) {
    if (fullyInside) {
      this->inside.push_back(node);
    } else {
      this->clipped.push_back(node);
    }
    return;
  }

  for (int index = 0; index < 4; ++index) {
    WorldSpatialTree* child = node->children[index];
    if (fullyInside) {
      this->CollectNode(child, depth + 1, fullyInside);
    } else {
      const int visibility = this->ClassifyAxisAligned(*child);

      if (visibility == MODEL_VIEW_CLIPPED) {
        this->CollectNode(child, depth + 1, false);
      } else if (visibility == MODEL_VIEW_INSIDE) {
        this->CollectNode(child, depth + 1, true);
      }
    }
  }
}
