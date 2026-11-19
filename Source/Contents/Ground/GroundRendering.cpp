#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/TextureQuad.h"

#include <cstring>

namespace {
  struct GroundVertex : RenderVertex {
    // INLINED
    GroundVertex() {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->rhw = 0.0f;
      this->diffuse = 0;
      this->specular = 0xFF000000u;
      this->u = 0.0f;
      this->v = 0.0f;
    }
  };

  // INLINED
  __forceinline void SubmitGroundQuad(const float* matrix, const GroundQuad& quad, bool clip) {
    GroundVertex vertices[4];

    for (unsigned corner = 0; corner < 4; ++corner) {
      g_renderer->Project(quad.vertices[corner].position, matrix, vertices[corner]);
    }

    if (clip && !GroundQuadVisible(vertices)) {
      return;
    }

    for (unsigned corner = 0; corner < 4; ++corner) {
      vertices[corner].u = quad.vertices[corner].u;
      vertices[corner].v = quad.vertices[corner].v;
      vertices[corner].diffuse = quad.vertices[corner].color;
      vertices[corner].diffuseChannels.alpha = 0x80;
    }

    RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
    std::memcpy(primitive->vertices, vertices, sizeof(vertices));
    primitive->texture = quad.texture;
    g_renderer->Submit(primitive, 0x800);
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
void GroundBase::Draw(const float*, const int*, bool) {
}
