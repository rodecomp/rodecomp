#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Render/Renderer.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <cstring>

struct WaterVertex : RenderVertex {
  // INLINED
  WaterVertex() {
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
static __forceinline RenderPrimitive* BuildWaterPrimitive(
  const GroundGeometry& ground, GroundCell& cell, const float* matrix, const float* ambient, float middle, float first, float last
) {
  WaterVertex vertices[4];

  if (!(cell.top->vertices[0].position[1] > middle || cell.top->vertices[1].position[1] > middle ||
        cell.top->vertices[2].position[1] > middle || cell.top->vertices[3].position[1] > middle)) {
    return nullptr;
  }

  cell.corners[0][1] = first;
  cell.corners[1][1] = middle;
  cell.corners[2][1] = middle;
  cell.corners[3][1] = last;

  for (unsigned corner = 0; corner < 4; ++corner) {
    g_renderer->Project(cell.corners[corner], matrix, vertices[corner]);
  }

  const float firstArea = (vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y);
  const float secondArea = (vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y);

  if (!(firstArea > secondArea)) {
    return nullptr;
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->SetPosition(0, &vertices[0].x);
  primitive->SetPosition(1, &vertices[1].x);
  primitive->SetPosition(2, &vertices[2].x);
  primitive->SetPosition(3, &vertices[3].x);

  if (!g_overrideColor && ground.waterType == 4) {
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
  return primitive;
}

// INLINED
static __forceinline void SetWaterTextureCoordinates(RenderPrimitive& primitive, int column, int row, float unit) {
  const float x = static_cast<float>(static_cast<unsigned>(column) & 3u);
  const float y = static_cast<float>(static_cast<unsigned>(row) & 3u);
  const float left = x * 0.25f;
  const float right = (x + 1.0f) * 0.25f;
  const float top = y * 0.25f;
  const float bottom = (y + 1.0f) * 0.25f;

  primitive.vertices[0].u = left;
  primitive.vertices[0].v = top;
  primitive.vertices[1].u = right;
  primitive.vertices[1].v = top;
  primitive.vertices[2].u = left;
  primitive.vertices[2].v = bottom;
  primitive.vertices[3].u = right;
  primitive.vertices[3].v = bottom;
}

// OFFSET: 0x00AC1540
// FUNCTION: fn_45865
__forceinline int GroundGeometry::WaterPartition(int column, int row) const {
  const int x = std::min(column / (this->width / this->waterColumns), this->waterColumns - 1);
  const int z = std::min(row / (this->height / this->waterRows), this->waterRows - 1);
  return z * this->waterColumns + x;
}

// OFFSET: 0x00AC2B80
// FUNCTION: fn_17594
void GroundGeometry::DrawWaterCell(
  int column, int row, GroundCell& cell, const float* matrix, const float* ambient
) {
  const float level = this->WaterLevel(column, row);
  unsigned partition;

  if (this->waterPartitioned) {
    partition = static_cast<unsigned>(this->WaterPartition(column, row));
  } else {
    partition = 0;
  }

  const GroundWater* const surfaces = &this->waterSurfaces[0];
  float middle;
  float first;
  float last;

  if (0.0f != surfaces[partition].parameters.height) {
    const auto waveHeight = [surfaces, partition, level](int diagonal) {
      const float angle = static_cast<float>(diagonal) * surfaces[partition].parameters.pitch + surfaces[partition].phase;
      const float wave = ClientSine(static_cast<int>(angle));
      return level + wave * surfaces[partition].parameters.height;
    };
    const int diagonal = column + row;
    middle = waveHeight(diagonal);
    first = waveHeight(diagonal - 1);
    last = waveHeight(diagonal + 1);
  } else {
    middle = level;
    first = level;
    last = level;
  }

  RenderPrimitive* primitive = BuildWaterPrimitive(*this, cell, matrix, ambient, middle, first, last);
  if (!primitive) {
    return;
  }
  SetWaterTextureCoordinates(*primitive, column, row, g_projectionDepthNumerator);

  *reinterpret_cast<unsigned char*>(&primitive->depthFlags) = 1;
  primitive->texture = this->waterSurfaces[partition].texture;
  g_renderer->Submit(primitive, 1);
}

// OFFSET: 0x00AC27C0
// FUNCTION: fn_46292
void GroundGeometry::DrawLegacyWaterCell(int row, int column, GroundCell& cell, const float* matrix, const float* ambient) {
  const float level = this->WaterLevel(column, row);
  float middle;
  float first;
  float last;
  if (this->waterHeight != 0.0f) {
    const auto waveHeight = [this, level](int diagonal) {
      const float angle = static_cast<float>(diagonal * this->waterPitch + this->waterWavePhase);
      return ClientSine(static_cast<int>(angle)) * this->waterHeight + level;
    };
    const int diagonal = column + row;
    middle = waveHeight(diagonal);
    first = waveHeight(diagonal - 1);
    last = waveHeight(diagonal + 1);
  } else {
    middle = level;
    first = level;
    last = level;
  }

  RenderPrimitive* primitive = BuildWaterPrimitive(*this, cell, matrix, ambient, middle, first, last);
  if (!primitive) {
    return;
  }
  SetWaterTextureCoordinates(*primitive, column, row, g_projectionDepthNumerator);
  if (!this->waterTexture) {
    this->UpdateWater();
  }
  primitive->texture = this->waterTexture;
  g_renderer->Submit(primitive, 1);
}
