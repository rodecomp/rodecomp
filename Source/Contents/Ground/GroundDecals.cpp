#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00AC0A90
// FUNCTION: fn_17569
bool GroundGeometry::CellRectangle(const C3dAttr& terrain, int column, int row, float* vertices) const {
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height) {
    return false;
  }

  const int cellColumn = column >> 1;
  const int cellRow = row >> 1;

  vertices[1] = 0.0f;
  vertices[0] = static_cast<float>(cellColumn - this->width / 2) * this->cellSize;
  vertices[2] = static_cast<float>(cellRow - this->height / 2) * this->cellSize;
  vertices[4] = 0.0f;
  vertices[3] = static_cast<float>(cellColumn - this->width / 2 + 1) * this->cellSize;
  vertices[5] = static_cast<float>(cellRow - this->height / 2) * this->cellSize;
  vertices[7] = 0.0f;
  vertices[6] = static_cast<float>(cellColumn - this->width / 2) * this->cellSize;
  vertices[8] = static_cast<float>(cellRow - this->height / 2 + 1) * this->cellSize;
  vertices[10] = 0.0f;
  vertices[9] = static_cast<float>(cellColumn - this->width / 2 + 1) * this->cellSize;
  vertices[11] = static_cast<float>(cellRow - this->height / 2 + 1) * this->cellSize;

  const TerrainHeightCell* cell = terrain.CellAt(column, row);

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
    const GroundGeometry& ground, const C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv,
    float bias
  ) {
    float vertices[4][3];

    for (auto& vertex : vertices) {
      vertex[0] = 0.0f;
      vertex[1] = 0.0f;
      vertex[2] = 0.0f;
    }

    if (!ground.CellRectangle(terrain, column, row, vertices[0])) {
      return;
    }

    for (auto& vertex : vertices) {
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

    const float scaleU = static_cast<float>(texture.Width()) / static_cast<float>(texture.TextureWidth());
    const float scaleV = static_cast<float>(texture.Height()) / static_cast<float>(texture.TextureHeight());

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
  DrawDecal(*this, *this->terrain, matrix, column, row, color, texture, uv, 0.10003052f);
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
  DrawDecal(*this, *this->terrain, matrix, column, row, color, texture, uv, 3.0517578e-05f);
}

// OFFSET: 0x00AC16D0
// FUNCTION: fn_17587
void GroundGeometry::DrawGrid(
  const float* matrix, int column, int row, std::uint32_t color
) const {
  // STATIC: 0x01347EB0
  // GLOBAL: gl_25477
  static const float uv[8] = {0, 0, 1, 0, 0, 1, 1, 1};
  this->DrawGridDecal(
    matrix, column, row, color, *g_texMgr.Load("grid.tga", 0, false, true, false), uv
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
  const float z = static_cast<float>(cellRow - halfHeight) * this->cellSize + ((row & 1) ? halfSize : 0.0f);
  const float left = x - margin;
  const float right = (x + halfSize) + margin;
  const float top = z - margin;
  const float bottom = (z + halfSize) + margin;
  const float vertices[4][3] = {
    {left, cell->heights[0], top}, {right, cell->heights[1], top}, {left, cell->heights[2], bottom}, {right, cell->heights[3], bottom}
  };
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();

  for (unsigned corner = 0; corner < 4; ++corner) {
    g_renderer->viewport.Project(vertices[corner], matrix, primitive->vertices[corner]);
    primitive->vertices[corner].u = static_cast<float>(corner & 1);
    primitive->vertices[corner].v = static_cast<float>(corner >> 1);
    primitive->vertices[corner].diffuse = color;
  }

  primitive->sourceBlend = 5;
  primitive->destinationBlend = 6;
  primitive->texture = texture;
  g_renderer->queues.Submit(primitive, 1, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
