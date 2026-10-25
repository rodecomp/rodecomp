#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/ScalarConversion.h"

#include <bit>

// OFFSET: 0x00AC0A90
// FUNCTION: fn_17569
bool GroundGeometry::CellRectangle(const C3dAttr& terrain, int column, int row, float* vertices) const {
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height) {
    return false;
  }

  const int x = (column >> 1) - this->width / 2;
  const int z = (row >> 1) - this->height / 2;
  const int offsets[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner * 3 + 1] = 0.0f;
    vertices[corner * 3] = static_cast<float>(x + offsets[corner][0]) * this->cellSize;
    vertices[corner * 3 + 2] = static_cast<float>(z + offsets[corner][1]) * this->cellSize;
  }

  return this->OverlayHeights(terrain, vertices, column, row);
}

namespace {
  void DrawDecal(
    const GroundGeometry& ground, const C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv,
    float bias, GroundRenderContext& context
  ) {
    float vertices[4][3]{};

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

    RenderPrimitive* primitive = context.primitives.Acquire();

    for (unsigned corner = 0; corner < 4; ++corner) {
      context.projection.Project(vertices[corner], matrix, primitive->vertices[corner]);
      primitive->vertices[corner].z = primitive->vertices[corner].z - bias;
    }

    const float scaleU = static_cast<float>(texture.Width()) / static_cast<float>(texture.TextureWidth());
    const float scaleV = static_cast<float>(texture.Height()) / static_cast<float>(texture.TextureHeight());

    for (unsigned corner = 0; corner < 4; ++corner) {
      primitive->vertices[corner].u = scaleU * uv[corner * 2];
      primitive->vertices[corner].v = scaleV * uv[corner * 2 + 1];
      primitive->vertices[corner].diffuse = color;
    }

    primitive->unknown_0018 = 5;
    primitive->unknown_001C = 6;
    primitive->texture = &texture;
    context.queues.Submit(primitive, 1, context.depthAlpha, context.depthStart, context.depthEnd);
  }
}

// OFFSET: 0x00AC3380
// FUNCTION: fn_17602
void GroundGeometry::DrawCellDecal(
  const C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv, GroundRenderContext& context
) const {
  DrawDecal(*this, terrain, matrix, column, row, color, texture, uv, 0.10003052f, context);
}

// OFFSET: 0x00AC1720
// FUNCTION: fn_17589
void GroundGeometry::DrawGridDecal(
  const C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv, GroundRenderContext& context
) const {
  DrawDecal(*this, terrain, matrix, column, row, color, texture, uv, 3.0517578e-05f, context);
}

// OFFSET: 0x00AC16D0
// FUNCTION: fn_17587
void GroundGeometry::DrawGrid(
  const C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, TextureCache& textures, GroundRenderContext& context
) const {
  CTexture* texture = textures.Load("grid.tga", 0, false, true, false);
  // STATIC: 0x01347EB0
  constexpr float uv[8] = {0, 0, 1, 0, 0, 1, 1, 1};
  this->DrawGridDecal(terrain, matrix, column, row, color, *texture, uv, context);
}

// OFFSET: 0x00AC3610
// FUNCTION: fn_17604
void GroundGeometry::DrawTerrainOverlay(
  const C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture* texture, GroundRenderContext& context
) const {
  const auto half = [](int value) { return TruncateToInt(static_cast<float>(value) * 0.5f); };
  const int cellColumn = half(column);
  const int cellRow = half(row);
  const int halfWidth = half(this->width);
  const int halfHeight = half(this->height);
  const float halfSize = this->cellSize * 0.5f;
  const float margin = halfSize * 0.5f;

  if (column < 0 || cellColumn >= this->width || row < 0 || cellRow >= this->height) {
    return;
  }

  const TerrainHeightCell* cell = terrain.Grid().CellAt(column, row);

  if (!cell) {
    return;
  }

  const float x = static_cast<float>(cellColumn - halfWidth) * this->cellSize + ((column & 1) ? halfSize : 0.0f);
  const float z = static_cast<float>(cellRow - halfHeight) * this->cellSize + (row % 2 == 1 ? halfSize : 0.0f);
  const float left = x - margin;
  const float right = (x + halfSize) + margin;
  const float top = z - margin;
  const float bottom = (z + halfSize) + margin;
  const float vertices[4][3] = {
    {left, cell->heights[0], top}, {right, cell->heights[1], top}, {left, cell->heights[2], bottom}, {right, cell->heights[3], bottom}
  };
  RenderPrimitive* primitive = context.primitives.Acquire();

  for (unsigned corner = 0; corner < 4; ++corner) {
    context.projection.Project(vertices[corner], matrix, primitive->vertices[corner]);
    primitive->vertices[corner].u = static_cast<float>(corner & 1);
    primitive->vertices[corner].v = static_cast<float>(corner >> 1);
    primitive->vertices[corner].diffuse = color;
  }

  primitive->unknown_0018 = 5;
  primitive->unknown_001C = 6;
  primitive->texture = texture;
  context.queues.Submit(primitive, 1, context.depthAlpha, context.depthStart, context.depthEnd);
}
