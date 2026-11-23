#include "Contents/Ground/GroundGeometry.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/TextureQuad.h"
#include "Resource/World/WorldResource.h"

#include "Utility/Math/ScalarConversion.h"
#include <bit>
#include <cstring>

namespace {
  struct LitGroundVertex : RenderVertex {
    // INLINED
    LitGroundVertex() {
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
}

// OFFSET: 0x00AC1D30
// FUNCTION: fn_17592
void GroundGeometry::DrawLightmapped(const float* matrix, const GroundQuad& quad, bool clip) {
  LitGroundVertex vertices[4];
  float lightUV[4][2];
  float scales[3];
  std::memcpy(scales, g_weatherEffect.lightColor, sizeof(scales));

  for (unsigned corner = 0; corner < 4; ++corner) {
    g_renderer->Project(quad.vertices[corner].position, matrix, vertices[corner]);
  }

  if (clip && !GroundQuadVisible(vertices)) {
    return;
  }

  if (g_overrideColor) {
    for (int corner = 0; corner < 4; ++corner) {
      vertices[corner].u = quad.vertices[corner].u;
      vertices[corner].v = quad.vertices[corner].v;
      lightUV[corner][0] = quad.vertices[corner].lightU;
      lightUV[corner][1] = quad.vertices[corner].lightV;

      SpriteColor channels = quad.vertices[corner].colorChannels;
      channels.red = static_cast<std::uint8_t>(channels.red * scales[0]);
      channels.green = static_cast<std::uint8_t>(channels.green * scales[1]);
      channels.blue = static_cast<std::uint8_t>(channels.blue * scales[2]);
      vertices[corner].diffuseChannels = channels;
    }
  } else if (quad.selected != 0) {
    for (int corner = 0; corner < 4; ++corner) {
      vertices[corner].u = quad.vertices[corner].u;
      vertices[corner].v = quad.vertices[corner].v;
      lightUV[corner][0] = quad.vertices[corner].lightU;
      lightUV[corner][1] = quad.vertices[corner].lightV;
      vertices[corner].diffuseChannels = quad.vertices[corner].colorChannels;
      vertices[corner].diffuseChannels.alpha = 0x80;
    }
  } else {
    for (int corner = 0; corner < 4; ++corner) {
      vertices[corner].u = quad.vertices[corner].u;
      vertices[corner].v = quad.vertices[corner].v;
      lightUV[corner][0] = quad.vertices[corner].lightU;
      lightUV[corner][1] = quad.vertices[corner].lightV;

      SpriteColor channels = quad.vertices[corner].colorChannels;
      channels.red = static_cast<std::uint8_t>(channels.red * scales[0]);
      channels.green = static_cast<std::uint8_t>(channels.green * scales[1]);
      channels.blue = static_cast<std::uint8_t>(channels.blue * scales[2]);
      vertices[corner].diffuseChannels = channels;
    }
  }

  const bool enabled = g_serviceOptions.Get(TT_LIGHTMAP_ON_OFF);

  if (g_multitexture && enabled) {
    LightmappedPrimitive* primitive = g_renderer->lightmapped.Acquire();

    for (unsigned corner = 0; corner < 4; ++corner) {
      std::memcpy(&primitive->vertices[corner], &vertices[corner], sizeof(RenderVertex));
      LightmappedVertex& vertex = primitive->vertices[corner];
      vertex.lightU = lightUV[corner][0];
      vertex.lightV = lightUV[corner][1];
    }

    primitive->texture = quad.texture;
    primitive->lightmap = quad.lightmapTexture;
    static_cast<CRendererDX9*>(g_renderer)->Submit(primitive, 0);

    return;
  }

  if (g_serviceOptions.Get(TT_LIGHTMAP_ON_OFF) != 0) {
    RenderPrimitive* lightmap = g_renderer->AcquirePrimitive();
    std::memcpy(lightmap->vertices, vertices, sizeof(vertices));

    for (unsigned corner = 0; corner < 4; ++corner) {
      RenderVertex& vertex = lightmap->vertices[corner];
      vertex.u = lightUV[corner][0];
      vertex.v = lightUV[corner][1];
    }

    lightmap->texture = quad.lightmapTexture;
    g_renderer->Submit(lightmap, 0x1000);
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  std::memcpy(primitive->vertices, vertices, sizeof(vertices));
  primitive->texture = quad.texture;
  g_renderer->Submit(primitive, 0x800);
}

// OFFSET: 0x00AC15A0
// FUNCTION: fn_17585
void GroundGeometry::DrawLit(const float* matrix, const int* rectangle, bool clip) {
  vector3d ambient;
  EffectPoint corners[4];
  std::memcpy(&ambient, g_worldLighting.ambient, sizeof(ambient));
  int row = rectangle[1];

  while (row <= rectangle[3]) {
    int column = rectangle[0];

    while (column <= rectangle[2]) {
      const unsigned index = static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
      GroundCell& cell = this->cells[index];

      if (cell.frame != g_renderer->frame) {
        cell.frame = g_renderer->frame;

        if (cell.top) {
          this->DrawWaterCell(column, row, cell, matrix, &ambient.x);
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
void GroundBase::DrawLit(const float*, const int*, bool) {
}
