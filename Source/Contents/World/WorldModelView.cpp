#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/World/WorldModelView.h"
#include "Contents/World/WorldSpatialTree.h"

#include <cmath>

namespace {
  // INLINED
  float Absolute(float value) {
    return std::fabs(value);
  }

  // INLINED
  void IncludeRectangle(const int* rectangle, int* output) {
    if (output[0] > rectangle[0]) {
      output[0] = rectangle[0];
    }

    if (output[2] < rectangle[2]) {
      output[2] = rectangle[2];
    }

    if (output[1] > rectangle[1]) {
      output[1] = rectangle[1];
    }

    if (output[3] < rectangle[3]) {
      output[3] = rectangle[3];
    }
  }
}

// OFFSET: 0x00ADB730
// FUNCTION: fn_17921
int ModelViewPlanes::ClassifyAxisAligned(const WorldSpatialTree& bounds) const {
  bool clipped = false;

  for (unsigned plane = 0; plane < 6; ++plane) {
    const float* normal = this->normals[plane];
    const float radius = (Absolute(normal[0] * bounds.halfSize[0]) + Absolute(normal[1] * bounds.halfSize[1])) + Absolute(normal[2] * bounds.halfSize[2]);
    const float distance = ((normal[0] * bounds.center[0] + normal[1] * bounds.center[1]) + normal[2] * bounds.center[2]) + this->planes[plane][3];

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

// OFFSET: 0x00ADB7F0
// FUNCTION: fn_17922
int ModelViewPlanes::ClassifyOriented(const OrientedBounds& bounds) const {
  bool clipped = false;

  for (unsigned plane = 0; plane < 6; ++plane) {
    const float* normal = this->normals[plane];
    const float second = Absolute(((normal[0] * bounds.axes[1][0] + normal[1] * bounds.axes[1][1]) + normal[2] * bounds.axes[1][2]) * bounds.halfSize[1]);
    const float first = Absolute(((normal[0] * bounds.axes[0][0] + normal[1] * bounds.axes[0][1]) + normal[2] * bounds.axes[0][2]) * bounds.halfSize[0]);
    const float third = Absolute(((normal[0] * bounds.axes[2][0] + normal[1] * bounds.axes[2][1]) + normal[2] * bounds.axes[2][2]) * bounds.halfSize[2]);
    const float radius = (second + first) + third;
    const float distance = ((normal[0] * bounds.center[0] + normal[1] * bounds.center[1]) + normal[2] * bounds.center[2]) + this->planes[plane][3];

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

// INLINED
int ClassifyModelView(const OrientedBounds& bounds, const ModelViewPlanes& planes) {
  bool clipped = false;

  for (unsigned plane = 0; plane < 6; ++plane) {
    const float x = planes.planes[plane][0];
    const float y = planes.normals[plane][1];
    const float z = planes.normals[plane][2];
    const float second = Absolute(((bounds.axes[1][0] * x + bounds.axes[1][1] * y) + bounds.axes[1][2] * z) * bounds.halfSize[1]);
    const float first = Absolute(((bounds.axes[0][0] * x + bounds.axes[0][1] * y) + bounds.axes[0][2] * z) * bounds.halfSize[0]);
    const float third = Absolute(((bounds.axes[2][0] * x + bounds.axes[2][1] * y) + bounds.axes[2][2] * z) * bounds.halfSize[2]);
    const float radius = (second + first) + third;
    const float distance =
      ((bounds.center[0] * x + bounds.center[1] * planes.planes[plane][1]) + bounds.center[2] * planes.planes[plane][2]) + planes.planes[plane][3];

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

// INLINED
void DrawWorldModelLeaves(
  std::list<WorldSpatialTree*>& clipped, std::list<WorldSpatialTree*>& inside, const ModelViewPlanes& planes, const float* matrix, unsigned frame, int updates,
  GroundGeometry& ground, ModelRenderContext& models, GroundLightingContext& lighting, int* visibleRectangle
) {
  visibleRectangle[0] = visibleRectangle[1] = 999;
  visibleRectangle[2] = visibleRectangle[3] = 0;

  for (WorldSpatialTree* node : clipped) {
    for (C3dActor* model : node->objects) {
      if (model->renderFrame == frame) {
        continue;
      }

      const int visibility = ClassifyModelView(model->oriented, planes);

      if (updates > 0) {
        model->Update();
      }

      if (visibility == 1 || visibility == 2) {
        model->Draw(matrix, visibility == 2, models);
      }

      model->renderFrame = frame;
    }

    ground.DrawLit(matrix, node->groundCells, true, lighting);
    IncludeRectangle(node->groundCells, visibleRectangle);
  }

  for (WorldSpatialTree* node : inside) {
    for (C3dActor* model : node->objects) {
      if (updates > 0) {
        model->Update();
      }

      if (model->renderFrame != frame) {
        model->Draw(matrix, false, models);
        model->renderFrame = frame;
      }
    }

    ground.DrawLit(matrix, node->groundCells, false, lighting);
    IncludeRectangle(node->groundCells, visibleRectangle);
  }
}
