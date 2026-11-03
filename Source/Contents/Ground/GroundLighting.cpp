#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/MapSkyMgr.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Resource/World/WorldResource.h"

#include "Utility/Math/ScalarConversion.h"
#include <bit>
#include <cstring>

// OFFSET: 0x00AC1D30
// FUNCTION: fn_17592
void GroundGeometry::DrawLightmapped(const float* matrix, const GroundQuad& quad, bool clip) {
  RenderVertex vertices[4]{};
  float lightUV[4][2];
  float scales[3];
  std::memcpy(scales, g_weatherEffect.lightColor, sizeof(scales));

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner].specular = 0xFF000000u;
    g_renderer->viewport.Project(quad.vertices[corner], matrix, vertices[corner]);
  }

  if (clip && !GroundQuadVisible(vertices)) {
    return;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner].u = quad.vertices[corner][4];
    vertices[corner].v = quad.vertices[corner][5];
    lightUV[corner][0] = quad.vertices[corner][6];
    lightUV[corner][1] = quad.vertices[corner][7];
    std::uint32_t color = std::bit_cast<std::uint32_t>(quad.vertices[corner][3]);

    if (!g_overrideColor && quad.selected != 0) {
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

  const bool enabled = g_serviceOptions.Get(119) != 0;

  if (g_multitexture && enabled) {
    LightmappedPrimitive* primitive = g_renderer->lightmapped.Acquire();

    for (unsigned corner = 0; corner < 4; ++corner) {
      std::memcpy(&primitive->vertices[corner], &vertices[corner], sizeof(RenderVertex));
      primitive->vertices[corner].lightU = lightUV[corner][0];
      primitive->vertices[corner].lightV = lightUV[corner][1];
    }

    primitive->texture = quad.texture;
    primitive->lightmap = quad.lightmapTexture;
    g_renderer->queues.lightmapped.Submit(primitive, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);

    return;
  }

  if (g_serviceOptions.Get(119) != 0) {
    RenderPrimitive* lightmap = g_renderer->primitives.Acquire();
    std::memcpy(lightmap->vertices, vertices, sizeof(vertices));

    for (unsigned corner = 0; corner < 4; ++corner) {
      lightmap->vertices[corner].u = lightUV[corner][0];
      lightmap->vertices[corner].v = lightUV[corner][1];
    }

    lightmap->texture = quad.lightmapTexture;
    g_renderer->queues.Submit(lightmap, 0x1000, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  std::memcpy(primitive->vertices, vertices, sizeof(vertices));
  primitive->texture = quad.texture;
  g_renderer->queues.Submit(primitive, 0x800, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x00AC15A0
// FUNCTION: fn_17585
void GroundGeometry::DrawLit(const float* matrix, const int* rectangle, bool clip) {
  float ambient[3];
  std::memcpy(ambient, g_worldLighting.ambient, sizeof(ambient));
  int row = rectangle[1];

  while (row <= rectangle[3]) {
    int column = rectangle[0];

    while (column <= rectangle[2]) {
      const unsigned index = static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
      GroundCell& cell = this->cells[index];

      if (cell.frame != g_renderer->frame) {
        cell.frame = g_renderer->frame;

        if (cell.top) {
          this->DrawWaterCell(column, row, cell, matrix, ambient);
          this->DrawLightmapped(matrix, *cell.top, clip);
        }

        if (cell.side2) {
          this->DrawLightmapped(matrix, *cell.side2, clip);
        }

        if (cell.side1) {
          this->DrawLightmapped(matrix, *cell.side1, clip);
        }
      }

      ++column;
    }

    ++row;
  }
}

// OFFSET: 0x00AC16C0
// FUNCTION: fn_17586
void GroundGeometry::Slot06(int, int, int) {
}
