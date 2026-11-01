#include "Contents/Actors/GroundScope.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/NavigationManager.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/World/World.h"
#include "Contents/World/WorldModelView.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// GLOBAL: gl_24780
int g_groundOverlayA;

// GLOBAL: gl_24781
int g_groundOverlayB;

// GLOBAL: gl_24782
int g_groundOverlaySkip;

// GLOBAL: gl_24783
int g_groundOverlayC;

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
    const float* normal = planes.normals[plane];
    const float second = Absolute(((bounds.axes[1][0] * normal[0] + bounds.axes[1][1] * normal[1]) + bounds.axes[1][2] * normal[2]) * bounds.halfSize[1]);
    const float first = Absolute(((bounds.axes[0][0] * normal[0] + bounds.axes[0][1] * normal[1]) + bounds.axes[0][2] * normal[2]) * bounds.halfSize[0]);
    const float third = Absolute(((bounds.axes[2][0] * normal[0] + bounds.axes[2][1] * normal[1]) + bounds.axes[2][2] * normal[2]) * bounds.halfSize[2]);
    const float radius = (second + first) + third;
    const float distance =
      ((bounds.center[0] * normal[0] + bounds.center[1] * normal[1]) + bounds.center[2] * normal[2]) + planes.planes[plane][3];

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

// OFFSET: 0x00ADC1E0
// FUNCTION: fn_17930
void CView::Render() {
  g_worldViewDrawn = 1;

  const float* matrix = this->frame.view.values;
  this->spatial.Collect(g_renderer->viewport.horizontal, g_renderer->viewport.vertical, matrix, this->scene->spatial);

  g_visibleMinY = 999;
  g_visibleMinX = 999;
  g_visibleMaxY = 0;
  g_visibleMaxX = 0;

  for (WorldSpatialTree* node : this->spatial.clipped) {
    for (C3dActor* model : node->objects) {
      if (model->renderFrame == g_renderer->frame) {
        continue;
      }

      const OrientedBounds& bounds = model->oriented;
      const ModelViewPlanes& planes = this->spatial.planes;
      bool clipped = false;
      bool culled = false;

      for (unsigned plane = 0; plane < 6; ++plane) {
        const float* normal = planes.normals[plane];
        const float second = Absolute(((bounds.axes[1][0] * normal[0] + bounds.axes[1][1] * normal[1]) + bounds.axes[1][2] * normal[2]) * bounds.halfSize[1]);
        const float first = Absolute(((bounds.axes[0][0] * normal[0] + bounds.axes[0][1] * normal[1]) + bounds.axes[0][2] * normal[2]) * bounds.halfSize[0]);
        const float third = Absolute(((bounds.axes[2][0] * normal[0] + bounds.axes[2][1] * normal[1]) + bounds.axes[2][2] * normal[2]) * bounds.halfSize[2]);
        const float radius = (second + first) + third;
        const float distance = ((bounds.center[0] * normal[0] + bounds.center[1] * normal[1]) + bounds.center[2] * normal[2]) + planes.planes[plane][3];

        if (!(distance > radius)) {
          const float negative = -radius;

          if (!(distance > negative)) {
            culled = true;
            break;
          }

          clipped = true;
        }
      }

      int visibility = 0;

      if (!culled) {
        visibility = clipped ? 2 : 1;
      }

      if (g_updateTick > 0) {
        model->Update();
      }

      if (visibility == 1) {
        void** modelVtable = *reinterpret_cast<void***>(model);
        reinterpret_cast<void(__thiscall*)(void*, const float*, bool)>(modelVtable[7])(model, matrix, false);
      } else if (visibility == 2) {
        void** modelVtable = *reinterpret_cast<void***>(model);
        reinterpret_cast<void(__thiscall*)(void*, const float*, bool)>(modelVtable[7])(model, matrix, true);
      }

      model->renderFrame = g_renderer->frame;
    }

    void** groundVtable = *reinterpret_cast<void***>(node->ground);
    reinterpret_cast<void(__thiscall*)(void*, const float*, const int*, bool)>(groundVtable[6])(
      node->ground, matrix, node->groundCells, true);

    if (g_visibleMinX > node->groundCells[0]) {
      g_visibleMinX = node->groundCells[0];
    }

    if (g_visibleMaxX < node->groundCells[2]) {
      g_visibleMaxX = node->groundCells[2];
    }

    if (g_visibleMinY > node->groundCells[1]) {
      g_visibleMinY = node->groundCells[1];
    }

    if (g_visibleMaxY < node->groundCells[3]) {
      g_visibleMaxY = node->groundCells[3];
    }
  }

  for (WorldSpatialTree* node : this->spatial.inside) {
    for (C3dActor* model : node->objects) {
      if (g_updateTick > 0) {
        model->Update();
      }

      if (model->renderFrame != g_renderer->frame) {
        void** modelVtable = *reinterpret_cast<void***>(model);
        reinterpret_cast<void(__thiscall*)(void*, const float*, bool)>(modelVtable[7])(model, matrix, false);
        model->renderFrame = g_renderer->frame;
      }
    }

    void** groundVtable = *reinterpret_cast<void***>(node->ground);
    reinterpret_cast<void(__thiscall*)(void*, const float*, const int*, bool)>(groundVtable[6])(
      node->ground, matrix, node->groundCells, false);

    if (g_visibleMinY > node->groundCells[1]) {
      g_visibleMinY = node->groundCells[1];
    }

    if (g_visibleMaxY < node->groundCells[3]) {
      g_visibleMaxY = node->groundCells[3];
    }

    if (g_visibleMinX > node->groundCells[0]) {
      g_visibleMinX = node->groundCells[0];
    }

    if (g_visibleMaxX < node->groundCells[2]) {
      g_visibleMaxX = node->groundCells[2];
    }
  }

  g_navigationManager.DrawRoute(matrix);

  void** sceneGroundVtable = *reinterpret_cast<void***>(this->scene->ground);
  if (g_groundOverlaySkip == 0) {
    reinterpret_cast<void(__thiscall*)(void*, const float*, int, int, int)>(sceneGroundVtable[14])(
      this->scene->ground, matrix, g_groundOverlayA, g_groundOverlayB, g_groundOverlayC);
  }

  if (g_groundMarkEnabled != 0 && g_groundMarkA != 0 && g_groundMarkB != 0) {
    reinterpret_cast<void(__thiscall*)(void*, const float*, int, int, int)>(sceneGroundVtable[14])(
      this->scene->ground, matrix, g_groundMarkA, g_groundMarkB, 0xfeff0000);
  }

  for (CRenderObject* object : this->scene->objects) {
    object->Render(reinterpret_cast<std::intptr_t>(matrix));
  }

  g_effectMgr->Render();

  const float x = this->camera.target[0] - this->frame.eye[0];
  const float y = this->camera.target[1] - this->frame.eye[1];
  const float z = this->camera.target[2] - this->frame.eye[2];
  *reinterpret_cast<float*>(&g_renderer->unknown_003C) = x;
  *reinterpret_cast<float*>(&g_renderer->unknown_0040) = y;
  *reinterpret_cast<float*>(&g_renderer->unknown_0044) = z;
  const float inverseLength = 1.0f / SquareRoot(x * x + y * y + z * z);
  *reinterpret_cast<float*>(&g_renderer->unknown_003C) *= inverseLength;
  *reinterpret_cast<float*>(&g_renderer->unknown_0040) *= inverseLength;
  *reinterpret_cast<float*>(&g_renderer->unknown_0044) *= inverseLength;

  this->spatial.inside.clear();
  this->spatial.clipped.clear();

  reinterpret_cast<void(__thiscall*)(void*, const float*)>(sceneGroundVtable[4])(this->scene->ground, matrix);
}
