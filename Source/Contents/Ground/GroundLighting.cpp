#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"

#include "Utility/Math/ScalarConversion.h"
#include <bit>
#include <cstring>

// OFFSET: 0x00AC1D30
// FUNCTION: fn_17592
void DrawLightmappedGround(const float* matrix, const GroundQuad& quad, bool clip, GroundLightingContext& context) {
  RenderVertex vertices[4]{};
  float lightUV[4][2];
  float scales[3];
  std::memcpy(scales, context.colorScale, sizeof(scales));

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner].specular = 0xFF000000u;
    context.render.projection.Project(quad.vertices[corner], matrix, vertices[corner]);
  }

  if (clip && !GroundQuadVisible(vertices, context.render)) {
    return;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner].u = quad.vertices[corner][4];
    vertices[corner].v = quad.vertices[corner][5];
    lightUV[corner][0] = quad.vertices[corner][6];
    lightUV[corner][1] = quad.vertices[corner][7];
    std::uint32_t color = std::bit_cast<std::uint32_t>(quad.vertices[corner][3]);

    if (!context.overrideColor && quad.selected != 0) {
      color = (color & 0x00FFFFFFu) | 0x80000000u;
    } else {
      for (unsigned channel = 0; channel < 3; ++channel) {
        const unsigned shift = 16 - channel * 8;
        const float value = static_cast<float>((color >> shift) & 255u) * scales[channel];
        const unsigned byte = static_cast<unsigned>(TruncateToInt(value)) & 255u;
        color = (color & ~(255u << shift)) | (byte << shift);
      }
    }

    vertices[corner].diffuse = color;
  }

  const bool enabled = context.options.Get(119) != 0;

  if (context.multitexture && enabled) {
    LightmappedPrimitive* primitive = context.primitives.Acquire();

    for (unsigned corner = 0; corner < 4; ++corner) {
      std::memcpy(&primitive->vertices[corner], &vertices[corner], sizeof(RenderVertex));
      primitive->vertices[corner].lightU = lightUV[corner][0];
      primitive->vertices[corner].lightV = lightUV[corner][1];
    }

    primitive->texture = quad.texture;
    primitive->lightmap = quad.lightmapTexture;
    context.render.queues.lightmapped.Submit(primitive, 0, context.render.depthAlpha, context.render.depthStart, context.render.depthEnd);

    return;
  }

  if (context.options.Get(119) != 0) {
    RenderPrimitive* lightmap = context.render.primitives.Acquire();
    std::memcpy(lightmap->vertices, vertices, sizeof(vertices));

    for (unsigned corner = 0; corner < 4; ++corner) {
      lightmap->vertices[corner].u = lightUV[corner][0];
      lightmap->vertices[corner].v = lightUV[corner][1];
    }

    lightmap->texture = quad.lightmapTexture;
    context.render.queues.Submit(lightmap, 0x1000, context.render.depthAlpha, context.render.depthStart, context.render.depthEnd);
  }

  RenderPrimitive* primitive = context.render.primitives.Acquire();
  std::memcpy(primitive->vertices, vertices, sizeof(vertices));
  primitive->texture = quad.texture;
  context.render.queues.Submit(primitive, 0x800, context.render.depthAlpha, context.render.depthStart, context.render.depthEnd);
}

// OFFSET: 0x00AC15A0
// FUNCTION: fn_17585
void GroundGeometry::DrawLit(const float* matrix, const int* rectangle, bool clip, GroundLightingContext& context) {
  int row = rectangle[1];

  while (row <= rectangle[3]) {
    int column = rectangle[0];

    while (column <= rectangle[2]) {
      const unsigned index = static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
      GroundCell& cell = this->cells[index];

      if (cell.frame != context.render.frame) {
        cell.frame = context.render.frame;

        if (cell.top) {
          this->DrawWaterCell(column, row, cell, matrix, context.waterAmbient, context.overrideColor, context.render);
          DrawLightmappedGround(matrix, *cell.top, clip, context);
        }

        if (cell.side2) {
          DrawLightmappedGround(matrix, *cell.side2, clip, context);
        }

        if (cell.side1) {
          DrawLightmappedGround(matrix, *cell.side1, clip, context);
        }
      }

      ++column;
    }

    ++row;
  }
}
