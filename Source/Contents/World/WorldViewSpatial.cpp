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
  this->camera.target.x = 0.0f;
  this->camera.target.y = 0.0f;
  this->camera.target.z = 0.0f;
  this->camera.desiredTarget.x = 0.0f;
  this->camera.desiredTarget.y = 0.0f;
  this->camera.desiredTarget.z = 0.0f;
  this->backupCurrentTarget.x = 0.0f;
  this->backupCurrentTarget.y = 0.0f;
  this->backupCurrentTarget.z = 0.0f;
  this->backupDesiredTarget.x = 0.0f;
  this->backupDesiredTarget.y = 0.0f;
  this->backupDesiredTarget.z = 0.0f;
  this->frame.eye.x = 0.0f;
  this->frame.eye.y = 0.0f;
  this->frame.eye.z = 0.0f;
  this->frame.up.x = 0.0f;
  this->frame.up.y = 0.0f;
  this->frame.up.z = 0.0f;
  this->camera.yaw = 0.0f;
  this->shake.enabled = 0;
  this->skyBox = nullptr;
  this->shake.start = 0;
  *reinterpret_cast<unsigned char*>(&this->shake.resetFlags) = 0;
}

// OFFSET: 0x00ADAF20
// FUNCTION: fn_17914
WorldViewSpatial::~WorldViewSpatial() = default;

// OFFSET: 0x00ADB0D0
// FUNCTION: fn_17919
void WorldViewSpatial::Collect(float horizontal, float vertical, const float* matrix, WorldSpatialTree& root) {
  float viewPlanes[6][4] = {
    {-horizontal, 0.0f, 1.0f, 0.0f},
    {horizontal, 0.0f, 1.0f, 0.0f},
    {0.0f, -vertical, 1.0f, 0.0f},
    {0.0f, vertical, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, -10.0f},
    {0.0f, 0.0f, -1.0f, 1500.0f},
  };

  for (int i = 0; i < 6; ++i) {
    const float* plane = viewPlanes[i];
    float* out = this->planes.planes[i];

    out[0] = plane[0] * matrix[0] + plane[1] * matrix[1] + plane[2] * matrix[2];
    out[1] = plane[0] * matrix[3] + plane[1] * matrix[4] + plane[2] * matrix[5];
    out[2] = plane[0] * matrix[6] + plane[1] * matrix[7] + plane[2] * matrix[8];
    out[3] = plane[0] * matrix[9] + plane[1] * matrix[10] + plane[2] * matrix[11] + plane[3];

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
        return 0;
      }

      clipped = true;
    }
  }

  return clipped ? 2 : 1;
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

      if (visibility == 2) {
        this->CollectNode(child, depth + 1, false);
      } else if (visibility == 1) {
        this->CollectNode(child, depth + 1, true);
      }
    }
  }
}
