#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/ScalarConversion.h"

// STATIC: 0x01347EB0
// GLOBAL: gl_25477
float g_groundGridUV[8] = {0, 0, 1, 0, 0, 1, 1, 1};

// OFFSET: 0x00AC0A90
// FUNCTION: fn_17569
bool GroundGeometry::CellRectangle(int column, int row, float* vertices) const {
  if (column < 0) {
    return false;
  }
  const int cellColumn = column >> 1;
  if (cellColumn >= this->width) {
    return false;
  }
  if (row < 0) {
    return false;
  }
  const int cellRow = row >> 1;
  if (cellRow >= this->height) {
    return false;
  }

  const float x0 = static_cast<float>(cellColumn - this->width / 2) * this->cellSize;
  const float z0 = static_cast<float>(cellRow - this->height / 2) * this->cellSize;
  vertices[1] = 0.0f;
  vertices[0] = x0;
  vertices[2] = z0;
  const float z1 = static_cast<float>(cellRow - this->height / 2) * this->cellSize;
  const float x1 = static_cast<float>(cellColumn - this->width / 2 + 1) * this->cellSize;
  vertices[4] = 0.0f;
  vertices[5] = z1;
  vertices[3] = x1;
  const float z2 = static_cast<float>(cellRow - this->height / 2 + 1) * this->cellSize;
  const float x2 = static_cast<float>(cellColumn - this->width / 2) * this->cellSize;
  vertices[7] = 0.0f;
  vertices[8] = z2;
  vertices[6] = x2;
  const float z3 = static_cast<float>(cellRow - this->height / 2 + 1) * this->cellSize;
  const float x3 = static_cast<float>(cellColumn - this->width / 2 + 1) * this->cellSize;
  vertices[10] = 0.0f;
  vertices[11] = z3;
  vertices[9] = x3;

  const TerrainHeightCell* cell = this->terrain->CellAt(column, row);

  if (!cell) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner * 3 + 1] = cell->heights[corner];
  }

  return true;
}

namespace {
  // INLINED
  __forceinline void DrawDecal(
    const GroundGeometry& ground, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv,
    float bias
  ) {
    struct Point {
      // INLINED
      Point() : x(0), y(0), z(0) {
      }

      float x;
      float y;
      float z;
    };
    Point vertices[4];

    if (!ground.CellRectangle(column, row, &vertices[0].x)) {
      return;
    }

    for (int index = 0; index < 4; ++index) {
      Point& vertex = vertices[index];
      if (column & 1) {
        if (vertices[1].x > vertex.x) {
          vertex.x = (vertices[1].x - vertex.x) * 0.5f + vertex.x;
        }
      } else if (vertex.x > vertices[0].x) {
        vertex.x = vertex.x - (vertex.x - vertices[0].x) * 0.5f;
      }

      if (row & 1) {
        if (vertices[2].z > vertex.z) {
          vertex.z = (vertices[2].z - vertex.z) * 0.5f + vertex.z;
        }
      } else if (vertex.z > vertices[0].z) {
        vertex.z = vertex.z - (vertex.z - vertices[0].z) * 0.5f;
      }
    }

    RenderPrimitive* primitive = g_renderer->AcquirePrimitive();

    for (int corner = 0; corner < 4; ++corner) {
      g_renderer->Project(&vertices[corner].x, matrix, primitive->vertices[corner]);
      primitive->vertices[corner].z = primitive->vertices[corner].z - bias;
    }

    const float scaleU = static_cast<float>(texture.ImageWidth()) / static_cast<float>(texture.TextureWidth());
    const float scaleV = static_cast<float>(texture.ImageHeight()) / static_cast<float>(texture.TextureHeight());

    for (unsigned corner = 0; corner < 4; ++corner) {
      primitive->vertices[corner].u = scaleU * uv[corner * 2];
      primitive->vertices[corner].v = scaleV * uv[corner * 2 + 1];
    }

    primitive->sourceBlend = 5;
    primitive->destinationBlend = 6;
    primitive->texture = &texture;

    for (unsigned corner = 0; corner < 4; ++corner) {
      primitive->vertices[corner].diffuse = color;
    }

    g_renderer->Submit(primitive, 1);
  }
}

// OFFSET: 0x00AC3380
// FUNCTION: fn_17602
void GroundGeometry::DrawCellDecal(
  const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv
) const {
  DrawDecal(*this, matrix, column, row, color, texture, uv, 0.10003052f);
}

// OFFSET: 0x00AC1710
// FUNCTION: fn_17588
void GroundBase::DrawGrid(const float*, int, int, std::uint32_t) const {
}

// OFFSET: 0x00AC1720
// FUNCTION: fn_17589
__declspec(noinline) void GroundGeometry::DrawGridDecal(
  const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv
) const {
  DrawDecal(*this, matrix, column, row, color, texture, uv, 3.0517578e-05f);
}

// OFFSET: 0x00AC16D0
// FUNCTION: fn_17587
void GroundGeometry::DrawGrid(
  const float* matrix, int column, int row, std::uint32_t color
) const {
  this->DrawGridDecal(
    matrix, column, row, color, *g_texMgr.Load("grid.tga", 0, false, true, false), g_groundGridUV
  );
}

// OFFSET: 0x00AC3600
// FUNCTION: fn_17603
void GroundBase::DrawCellDecal(const float*, int, int, std::uint32_t, CTexture&, const float*) const {
}

// OFFSET: 0x00AC3610
// FUNCTION: fn_17604
void GroundGeometry::DrawTerrainOverlay(
  const float* matrix, int column, int row, std::uint32_t color, CTexture* texture
) const {
  struct Point {
    // INLINED
    Point() : x(0), y(0), z(0) {
    }

    float x;
    float y;
    float z;
  };
  Point vertices[4];

  const int cellColumn = static_cast<int>(static_cast<float>(column) * 0.5f);
  const int cellRow = static_cast<int>(static_cast<float>(row) * 0.5f);
  const int rowParity = row % 2;
  const int halfWidth = static_cast<int>(static_cast<float>(this->width) * 0.5f);
  const int halfHeight = static_cast<int>(static_cast<float>(this->height) * 0.5f);
  const float halfSize = this->cellSize * 0.5f;
  const float margin = halfSize * 0.5f;

  if (column < 0 || cellColumn >= this->width || row < 0 || cellRow >= this->height) {
    return;
  }

  const TerrainHeightCell* cell = this->terrain->CellAt(column, row);

  if (!cell) {
    return;
  }

  const float x = static_cast<float>(cellColumn - halfWidth) * this->cellSize + ((column & 1) ? halfSize : 0.0f);
  const float z = static_cast<float>(cellRow - halfHeight) * this->cellSize + (rowParity == 1 ? halfSize : 0.0f);
  const float height0 = cell->heights[0];
  vertices[0].x = x - margin;
  vertices[0].y = height0;
  vertices[0].z = z - margin;
  const float height1 = cell->heights[1];
  vertices[1].x = (x + halfSize) + margin;
  vertices[1].y = height1;
  vertices[1].z = z - margin;
  const float height2 = cell->heights[2];
  vertices[2].x = x - margin;
  vertices[2].y = height2;
  vertices[2].z = (z + halfSize) + margin;
  const float height3 = cell->heights[3];
  vertices[3].x = (x + halfSize) + margin;
  vertices[3].y = height3;
  vertices[3].z = (z + halfSize) + margin;
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();

  for (int corner = 0; corner < 4; ++corner) {
    g_renderer->Project(&vertices[corner].x, matrix, primitive->vertices[corner]);
  }

  primitive->vertices[0].u = 0.0f;
  primitive->vertices[0].v = 0.0f;
  primitive->vertices[1].u = 1.0f;
  primitive->vertices[1].v = 0.0f;
  primitive->vertices[2].u = 0.0f;
  primitive->vertices[2].v = 1.0f;
  primitive->vertices[3].u = 1.0f;
  primitive->vertices[3].v = 1.0f;
  primitive->sourceBlend = 5;
  primitive->destinationBlend = 6;
  primitive->texture = texture;

  for (unsigned corner = 0; corner < 4; ++corner) {
    primitive->vertices[corner].diffuse = color;
  }

  g_renderer->Submit(primitive, 1);
}
