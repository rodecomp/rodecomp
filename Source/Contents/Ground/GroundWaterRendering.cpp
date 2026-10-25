#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <bit>
#include <cstring>

// OFFSET: 0x00AC2B80
// FUNCTION: fn_17594
void GroundGeometry::DrawWaterCell(
  int column, int row, GroundCell& cell, const float* matrix, const float* ambient, bool overrideColor, GroundRenderContext& context
) {
  const float level = this->water.Level(this->width, this->height, column, row);
  unsigned partition = 0;

  if (this->water.partitioned) {
    const int x = std::min(column / (this->width / this->water.columns), this->water.columns - 1);
    const int z = std::min(row / (this->height / this->water.rows), this->water.rows - 1);
    partition = static_cast<unsigned>(x) + static_cast<unsigned>(z) * static_cast<unsigned>(this->water.columns);
  }

  GroundWaterSurface& surface = this->water.surfaces[partition];
  float middle = level;
  float first = level;
  float last = level;

  if (surface.height != 0.0f) {
    const int sum = column + row;
    const auto wave = [&](int coordinate) {
      const float angle = static_cast<float>(coordinate) * surface.pitch + surface.phase;
      const int degrees = TruncateToInt(angle);
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
    context.projection.Project(cell.corners[corner], matrix, vertices[corner]);
  }

  const float firstArea = (vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y);
  const float secondArea = (vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y);

  if (!(firstArea > secondArea)) {
    return;
  }

  RenderPrimitive* primitive = context.primitives.Acquire();
  std::memcpy(primitive->vertices, vertices, sizeof(vertices));
  std::uint32_t color = 0x90FFFFFFu;

  if (!overrideColor && this->water.type == 4) {
    color = 0xFF000000u;

    for (unsigned channel = 0; channel < 3; ++channel) {
      const float scaled = ambient[channel] * 255.0f;
      color |= (static_cast<unsigned>(TruncateToInt(scaled)) & 255u) << (16 - 8 * channel);
    }
  }

  const float left = static_cast<float>(static_cast<unsigned>(column) & 3u) * 0.25f;
  const float right = (static_cast<float>(static_cast<unsigned>(column) & 3u) + 1.0f) * 0.25f;
  const float top = static_cast<float>(static_cast<unsigned>(row) & 3u) * 0.25f;
  const float bottom = (static_cast<float>(static_cast<unsigned>(row) & 3u) + 1.0f) * 0.25f;

  for (unsigned corner = 0; corner < 4; ++corner) {
    primitive->vertices[corner].diffuse = color;
    primitive->vertices[corner].u = (corner & 1) ? right : left;
    primitive->vertices[corner].v = (corner & 2) ? bottom : top;
  }

  primitive->unknown_0014 = (primitive->unknown_0014 & 0xFF00u) | 1u;
  primitive->texture = surface.texture;
  context.queues.Submit(primitive, 1, context.depthAlpha, context.depthStart, context.depthEnd);
}
