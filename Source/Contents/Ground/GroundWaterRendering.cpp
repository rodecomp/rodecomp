#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Contents/Effects/EffectPrim.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00AC2B80
// FUNCTION: fn_17594
void GroundGeometry::DrawWaterCell(
  int column, int row, GroundCell& cell, const float* matrix, const float* ambient
) {
  const float level = this->WaterLevel(column, row);
  unsigned partition = 0;

  if (this->waterPartitioned) {
    const int x = std::min(column / (this->width / this->waterColumns), this->waterColumns - 1);
    const int z = std::min(row / (this->height / this->waterRows), this->waterRows - 1);
    partition = static_cast<unsigned>(x) + static_cast<unsigned>(z) * static_cast<unsigned>(this->waterColumns);
  }

  GroundWaterSurface& surface = this->waterSurfaces[partition];
  float middle = level;
  float first = level;
  float last = level;

  if (surface.height != 0.0f) {
    const int sum = column + row;
    const auto wave = [&](int coordinate) {
      const float angle = static_cast<float>(coordinate) * surface.pitch + surface.phase;
      const int degrees = static_cast<int>(angle);
      return ClientSine(degrees) * surface.height + level;
    };
    middle = wave(sum);
    first = wave(sum - 1);
    last = wave(sum + 1);
  }

  bool above = false;

  for (unsigned corner = 0; corner < 4; ++corner) {
    if (cell.top->vertices[corner][1] > middle) {
      above = true;
      break;
    }
  }

  if (!above) {
    return;
  }

  cell.corners[0][1] = first;
  cell.corners[1][1] = middle;
  cell.corners[2][1] = middle;
  cell.corners[3][1] = last;
  RenderVertex vertices[4]{};

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner].specular = 0xFF000000u;
    g_renderer->viewport.Project(cell.corners[corner], matrix, vertices[corner]);
  }

  const float firstArea = (vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y);
  const float secondArea = (vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y);

  if (!(firstArea > secondArea)) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->SetPosition(0, &vertices[0].x);
  primitive->SetPosition(1, &vertices[1].x);
  primitive->SetPosition(2, &vertices[2].x);
  primitive->SetPosition(3, &vertices[3].x);

  if (!g_overrideColor && this->waterType == 4) {
    const std::uint8_t red = static_cast<std::uint8_t>(ambient[0] * 255.0f);
    const std::uint8_t green = static_cast<std::uint8_t>(ambient[1] * 255.0f);
    const std::uint8_t blue = static_cast<std::uint8_t>(ambient[2] * 255.0f);
    primitive->vertices[0].diffuse = 0xFF000000u + (red << 16) + (green << 8) + blue;
    primitive->vertices[1].diffuse = primitive->vertices[0].diffuse;
    primitive->vertices[2].diffuse = primitive->vertices[0].diffuse;
    primitive->vertices[3].diffuse = primitive->vertices[0].diffuse;
  } else {
    for (unsigned corner = 0; corner < 4; ++corner) {
      primitive->vertices[corner].diffuse = 0x90FFFFFFu;
    }
  }

  const float left = static_cast<float>(static_cast<unsigned>(column) & 3u) * 0.25f;
  const float right = (static_cast<float>(static_cast<unsigned>(column) & 3u) + 1.0f) * 0.25f;
  const float top = static_cast<float>(static_cast<unsigned>(row) & 3u) * 0.25f;
  const float bottom = (static_cast<float>(static_cast<unsigned>(row) & 3u) + 1.0f) * 0.25f;

  for (unsigned corner = 0; corner < 4; ++corner) {
    primitive->vertices[corner].u = (corner & 1) ? right : left;
    primitive->vertices[corner].v = (corner & 2) ? bottom : top;
  }

  primitive->depthFlags = (primitive->depthFlags & 0xFF00u) | 1u;
  primitive->texture = surface.texture;
  g_renderer->queues.Submit(primitive, 1, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
