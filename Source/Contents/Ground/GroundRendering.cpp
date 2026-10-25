#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"

#include <bit>
#include <cstring>

namespace {
  // INLINED
  float Minimum4(float a, float b, float c, float d) {
    const float first = !(a > b) ? a : b;
    const float second = !(c > d) ? c : d;
    return first > second ? second : first;
  }

  // INLINED
  float Maximum4(float a, float b, float c, float d) {
    const float first = !(b > a) ? a : b;
    const float second = !(d > c) ? c : d;
    return second > first ? second : first;
  }
}

// INLINED
bool GroundQuadVisible(const RenderVertex* vertices, const GroundRenderContext& context) {
  const float minimumX = Minimum4(vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x);

  if (minimumX >= static_cast<float>(context.width)) {
    return false;
  }

  const float maximumX = Maximum4(vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x);

  if (0.0f > maximumX) {
    return false;
  }

  const float minimumY = Minimum4(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y);

  if (minimumY >= static_cast<float>(context.height)) {
    return false;
  }

  const float maximumY = Maximum4(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y);

  if (0.0f > maximumY) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    if (0.0f > vertices[corner].rhw) {
      return false;
    }
  }

  return true;
}

namespace {
  // INLINED
  void Submit(const float* matrix, const GroundQuad& quad, bool clip, GroundRenderContext& context) {
    RenderVertex vertices[4]{};

    for (unsigned corner = 0; corner < 4; ++corner) {
      vertices[corner].specular = 0xFF000000u;
      context.projection.Project(quad.vertices[corner], matrix, vertices[corner]);
    }

    if (clip && !GroundQuadVisible(vertices, context)) {
      return;
    }

    for (unsigned corner = 0; corner < 4; ++corner) {
      vertices[corner].diffuse = (std::bit_cast<std::uint32_t>(quad.vertices[corner][3]) & 0x00FFFFFFu) | 0x80000000u;
      vertices[corner].u = quad.vertices[corner][4];
      vertices[corner].v = quad.vertices[corner][5];
    }

    RenderPrimitive* primitive = context.primitives.Acquire();
    std::memcpy(primitive->vertices, vertices, sizeof(vertices));
    primitive->texture = quad.texture;
    context.queues.Submit(primitive, 0x800, context.depthAlpha, context.depthStart, context.depthEnd);
  }
}

// OFFSET: 0x00AC2530
// FUNCTION: fn_17593
void DrawGroundSide(const float* matrix, const GroundQuad& quad, bool clip, GroundRenderContext& context) {
  Submit(matrix, quad, clip, context);
}

// OFFSET: 0x00AC19A0
// FUNCTION: fn_17590
void GroundGeometry::Draw(const float* matrix, const int* rectangle, bool clip, GroundRenderContext& context) {
  int row = rectangle[1];

  while (row <= rectangle[3]) {
    int column = rectangle[0];

    while (column <= rectangle[2]) {
      const unsigned index = static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
      GroundCell& cell = this->cells[index];

      if (cell.frame != context.frame) {
        cell.frame = context.frame;

        if (cell.top) {
          Submit(matrix, *cell.top, clip, context);
        }

        if (cell.side2) {
          DrawGroundSide(matrix, *cell.side2, clip, context);
        }

        if (cell.side1) {
          DrawGroundSide(matrix, *cell.side1, clip, context);
        }
      }

      column = column + 1;
    }

    row = row + 1;
  }
}
