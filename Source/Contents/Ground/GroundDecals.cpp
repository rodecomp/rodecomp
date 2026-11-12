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
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height) {
    return false;
  }

  const int cellColumn = column >> 1;
  const int cellRow = row >> 1;

  const float x0 = static_cast<float>(cellColumn - this->width / 2) * this->cellSize;
  const float z0 = static_cast<float>(cellRow - this->height / 2) * this->cellSize;
  vertices[1] = 0.0f;
  vertices[0] = x0;
  vertices[2] = z0;
  const float x1 = static_cast<float>(cellColumn - this->width / 2 + 1) * this->cellSize;
  const float z1 = static_cast<float>(cellRow - this->height / 2) * this->cellSize;
  vertices[4] = 0.0f;
  vertices[3] = x1;
  vertices[5] = z1;
  const float x2 = static_cast<float>(cellColumn - this->width / 2) * this->cellSize;
  const float z2 = static_cast<float>(cellRow - this->height / 2 + 1) * this->cellSize;
  vertices[7] = 0.0f;
  vertices[6] = x2;
  vertices[8] = z2;
  const float x3 = static_cast<float>(cellColumn - this->width / 2 + 1) * this->cellSize;
  const float z3 = static_cast<float>(cellRow - this->height / 2 + 1) * this->cellSize;
  vertices[10] = 0.0f;
  vertices[9] = x3;
  vertices[11] = z3;

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
    float vertices[4][3];

    for (auto& vertex : vertices) {
      vertex[0] = 0.0f;
      vertex[1] = 0.0f;
      vertex[2] = 0.0f;
    }

    if (!ground.CellRectangle(column, row, vertices[0])) {
      return;
    }

    for (int index = 0; index < 4; ++index) {
      float* vertex = vertices[index];
      if (column & 1) {
        if (vertices[1][0] > vertex[0]) {
          vertex[0] = (vertices[1][0] - vertex[0]) * 0.5f + vertex[0];
        }
      } else if (vertex[0] > vertices[0][0]) {
        vertex[0] = vertex[0] - (vertex[0] - vertices[0][0]) * 0.5f;
      }

      if (row & 1) {
        if (vertices[2][2] > vertex[2]) {
          vertex[2] = (vertices[2][2] - vertex[2]) * 0.5f + vertex[2];
        }
      } else if (vertex[2] > vertices[0][2]) {
        vertex[2] = vertex[2] - (vertex[2] - vertices[0][2]) * 0.5f;
      }
    }

    RenderPrimitive* primitive = g_renderer->primitives.Acquire();

    for (unsigned corner = 0; corner < 4; ++corner) {
      g_renderer->viewport.Project(vertices[corner], matrix, primitive->vertices[corner]);
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

    g_renderer->queues.Submit(primitive, 1, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
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
  float vertices[4][3];

  for (auto& vertex : vertices) {
    vertex[0] = 0.0f;
    vertex[1] = 0.0f;
    vertex[2] = 0.0f;
  }

  const int cellColumn = static_cast<int>(static_cast<float>(column) * 0.5f);
  const int cellRow = static_cast<int>(static_cast<float>(row) * 0.5f);
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
  const float z = static_cast<float>(cellRow - halfHeight) * this->cellSize + (row % 2 == 1 ? halfSize : 0.0f);
  const float left = x - margin;
  const float right = (x + halfSize) + margin;
  const float top = z - margin;
  const float bottom = (z + halfSize) + margin;
  vertices[0][0] = left;
  vertices[0][1] = cell->heights[0];
  vertices[0][2] = top;
  vertices[1][0] = right;
  vertices[1][1] = cell->heights[1];
  vertices[1][2] = top;
  vertices[2][0] = left;
  vertices[2][1] = cell->heights[2];
  vertices[2][2] = bottom;
  vertices[3][0] = right;
  vertices[3][1] = cell->heights[3];
  vertices[3][2] = bottom;
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();

  for (unsigned corner = 0; corner < 4; ++corner) {
    g_renderer->viewport.Project(vertices[corner], matrix, primitive->vertices[corner]);
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

  g_renderer->queues.Submit(primitive, 1, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
