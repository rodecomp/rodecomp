#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"

#include <bit>
#include <cstring>

namespace {
  // INLINED
  __forceinline void SubmitGroundQuad(const float* matrix, const GroundQuad& quad, bool clip) {
    RenderVertex vertices[4]{};

    for (auto& vertex : vertices) {
      vertex.specular = 0xFF000000u;
    }

    for (unsigned corner = 0; corner < 4; ++corner) {
      g_renderer->viewport.Project(quad.vertices[corner], matrix, vertices[corner]);
    }

    if (clip && !GroundQuadVisible(vertices)) {
      return;
    }

    for (unsigned corner = 0; corner < 4; ++corner) {
      vertices[corner].diffuse = (std::bit_cast<std::uint32_t>(quad.vertices[corner][3]) & 0x00FFFFFFu) | 0x80000000u;
      vertices[corner].u = quad.vertices[corner][4];
      vertices[corner].v = quad.vertices[corner][5];
    }

    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    std::memcpy(primitive->vertices, vertices, sizeof(vertices));
    primitive->texture = quad.texture;
    g_renderer->queues.Submit(primitive, 0x800, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}

// OFFSET: 0x00AC2530
// FUNCTION: fn_17593
void GroundGeometry::DrawSide(const float* matrix, const GroundQuad& quad, bool clip) {
  SubmitGroundQuad(matrix, quad, clip);
}

// OFFSET: 0x00AC19A0
// FUNCTION: fn_17590
void GroundGeometry::Draw(const float* matrix, const int* rectangle, bool clip) {
  int row = rectangle[1];

  while (row <= rectangle[3]) {
    int column = rectangle[0];

    while (column <= rectangle[2]) {
      const unsigned index = static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
      GroundCell& cell = this->cells[index];

      if (cell.frame != g_renderer->frame) {
        cell.frame = g_renderer->frame;

        if (cell.top) {
          SubmitGroundQuad(matrix, *cell.top, clip);
        }

        if (cell.side2) {
          this->DrawSide(matrix, *cell.side2, clip);
        }

        if (cell.side1) {
          this->DrawSide(matrix, *cell.side1, clip);
        }
      }

      column = column + 1;
    }

    row = row + 1;
  }
}

// OFFSET: 0x00AC1D20
// FUNCTION: fn_17591
void GroundGeometry::Slot05(int, int, int) {
}
