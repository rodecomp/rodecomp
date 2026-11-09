#include "Contents/Ground/GroundGeometry.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/GroundResource.h"

#include "Utility/Math/AffineMatrix.h"
#include <bit>

namespace {
  struct NormalVector {
    // INLINED
    NormalVector() : values{0.0f, 0.0f, 0.0f} {
    }

    /* 0000 */ float values[3];
  };

  struct CellNormals {
    /* 0000 */ NormalVector faces[2];
    /* 0018 */ NormalVector side2;
    /* 0024 */ NormalVector side1;
  };

  // INLINED
  float InverseLength(float squared) {
    const float length = SquareRoot(squared);
    return 1.0f / length;
  }

  // INLINED
  void NormalizeGroundNormal(float* vector, bool yFirst) {
    const float first = yFirst ? vector[1] * vector[1] + vector[0] * vector[0] : vector[0] * vector[0] + vector[1] * vector[1];
    const float inverse = InverseLength(first + vector[2] * vector[2]);
    vector[0] = inverse * vector[0];
    vector[1] = inverse * vector[1];
    vector[2] = inverse * vector[2];
  }

  // INLINED
  void Add(float* result, const float* value) {
    result[0] = result[0] + value[0];
    result[1] = result[1] + value[1];
    result[2] = result[2] + value[2];
  }

  struct GroundCellRecord {
    /* 0000 */ float heights[4];
    /* 0010 */ int tiles[3];
  };

  struct GroundTileRecord {
    /* 0000 */ float u[4];
    /* 0010 */ float v[4];
    /* 0020 */ std::int16_t texture;
    /* 0022 */ std::uint16_t lightmap;
    /* 0024 */ std::uint32_t color;
  };

  // INLINED
  __forceinline const GroundCellRecord& CellRecord(const CGndRes& resource, int index) {
    return reinterpret_cast<const GroundCellRecord*>(resource.cells)[index];
  }

  // INLINED
  __forceinline const GroundTileRecord& TileRecord(const CGndRes& resource, int index) {
    return reinterpret_cast<const GroundTileRecord*>(resource.tiles)[index];
  }

  union VertexColor {
    /* 0000 */ std::uint32_t value;
    struct {
      /* 0000 */ unsigned char blue;
      /* 0001 */ unsigned char green;
      /* 0002 */ unsigned char red;
      /* 0003 */ unsigned char alpha;
    };
  };

  // INLINED
  __forceinline void SetPoint(float* point, float x, float y, float z) {
    point[0] = x;
    point[1] = y;
    point[2] = z;
  }

  // INLINED
  __forceinline void TriangleNormal(float* normal, const float* a, const float* b, const float* c) {
    const float u[3] = {c[0] - b[0], c[1] - b[1], c[2] - b[2]};
    const float v[3] = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};
    SetPoint(normal, u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
  }

  // INLINED
  __forceinline float Clamp(float value) {
    return value < 1.0f ? value : 1.0f;
  }

  // INLINED
  __forceinline void Shade(VertexColor& color, float red, float green, float blue) {
    color.alpha = 0xFF;
    color.red = static_cast<unsigned char>(color.red * red);
    color.green = static_cast<unsigned char>(color.green * green);
    color.blue = static_cast<unsigned char>(color.blue * blue);
  }
}

// INLINED
GroundBase::GroundBase() {
  this->waterStateA = 0;
  this->cellSize = 10.0f;
  this->waterLevel = 0.0f;
  this->waterAnimation = 3;
  this->waterPitch = 50;
  this->waterPhase = 2;
  this->waterHeight = 1.0f;
  this->waterType = 0;
  this->terrain = nullptr;
  this->waterStateB = 0;
  this->loaded = 0;
}

// INLINED
GroundBase::~GroundBase() = default;

// OFFSET: 0x00ABD640
// FUNCTION: fn_17546
GroundGeometry::GroundGeometry() : waterColumns(1), waterRows(1) {
}

// INLINED
GroundGeometry::~GroundGeometry() = default;

// OFFSET: 0x00AC3010
// FUNCTION: fn_17595
void GroundGeometry::ClearGeometry() {
  this->tiles.clear();
  this->lighting.clear();
  this->cells.clear();
}

// OFFSET: 0x00AC0200
// FUNCTION: fn_17563
void GroundGeometry::Reset(int columns, int rows) {
  this->ClearGeometry();
  this->width = columns;
  this->height = rows;
  const std::array<float, 3> white = {1.0f, 1.0f, 1.0f};
  this->cells.resize(static_cast<unsigned>(columns) * static_cast<unsigned>(rows));
  this->lighting.resize(4u * static_cast<unsigned>(columns) * static_cast<unsigned>(rows), white);
  this->waterStateC = 0;
  this->waterStateD = 0;
}

// OFFSET: 0x00AC0350
// FUNCTION: fn_17564
void GroundBase::Reset(int, int) {
}

// OFFSET: 0x00ABDEF0
// FUNCTION: fn_17556
void GroundGeometry::Build(CGndRes& resource, const float* lightDirection, const float* diffuse, const float* ambient) {
  this->loaded = resource.loaded;
  this->Reset(resource.width, resource.height);
  CellNormals* normals = new CellNormals[resource.width * resource.height];
  this->cellSize = resource.cellSize;
  this->lightmapSet.Build(resource);
  this->tiles.clear();
  this->tiles.resize(static_cast<unsigned>(resource.tileCount));

  for (int row = 0; row < this->height; ++row) {
    for (int column = 0; column < this->width; ++column) {
      const int index = row * this->width + column;
      GroundCell& cell = this->cells[index];
      CellNormals& cellNormals = normals[index];
      const GroundCellRecord& record = CellRecord(resource, index);

      for (int corner = 0; corner < 4; ++corner) {
        cell.heights[corner] = record.heights[corner];
      }

      SetPoint(cell.corners[0], static_cast<float>(column - this->width / 2) * this->cellSize, 0.0f,
               static_cast<float>(row - this->height / 2) * this->cellSize);
      SetPoint(cell.corners[1], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, 0.0f,
               static_cast<float>(row - this->height / 2) * this->cellSize);
      SetPoint(cell.corners[2], static_cast<float>(column - this->width / 2) * this->cellSize, 0.0f,
               static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
      SetPoint(cell.corners[3], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, 0.0f,
               static_cast<float>(row - this->height / 2 + 1) * this->cellSize);

      const int topIndex = record.tiles[0];

      if (topIndex >= 0 && topIndex < resource.tileCount) {
        const GroundTileRecord& tileRecord = TileRecord(resource, topIndex);
        GroundQuad& tile = this->tiles[topIndex];
        tile.color = tileRecord.color;
        tile.texture = g_texMgr.Load(resource.textureNames[tileRecord.texture], 0, false, true, false);
        SetPoint(tile.vertices[0], static_cast<float>(column - this->width / 2) * this->cellSize, cell.heights[0],
                 static_cast<float>(row - this->height / 2) * this->cellSize);
        SetPoint(tile.vertices[1], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, cell.heights[1],
                 static_cast<float>(row - this->height / 2) * this->cellSize);
        SetPoint(tile.vertices[2], static_cast<float>(column - this->width / 2) * this->cellSize, cell.heights[2],
                 static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
        SetPoint(tile.vertices[3], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, cell.heights[3],
                 static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
        const float scaleU = static_cast<float>(tile.texture->ImageWidth()) / static_cast<float>(tile.texture->TextureWidth());
        const float scaleV = static_cast<float>(tile.texture->ImageHeight()) / static_cast<float>(tile.texture->TextureHeight());

        for (int corner = 0; corner < 4; ++corner) {
          tile.vertices[corner][4] = scaleU * tileRecord.u[corner];
          tile.vertices[corner][5] = scaleV * tileRecord.v[corner];
        }

        GroundLightmap* lightmap = &this->lightmapSet.lightmaps[tileRecord.lightmap];
        tile.lightmap = lightmap;
        tile.lightmapTexture = lightmap->texture;

        for (int corner = 0; corner < 4; ++corner) {
          tile.vertices[corner][6] = lightmap->uv[corner][0];
          tile.vertices[corner][7] = lightmap->uv[corner][1];
        }

        for (int channel = 0; channel < 3; ++channel) {
          const unsigned value = 2u * tile.lightmap->color[channel];
          cell.color[channel] = static_cast<unsigned char>(value > 255 ? 255 : value);
        }

        NormalVector points[4];
        SetPoint(points[0].values, 0.0f, cell.heights[0], 0.0f);
        SetPoint(points[1].values, this->cellSize, cell.heights[1], 0.0f);
        SetPoint(points[2].values, 0.0f, cell.heights[2], this->cellSize);
        SetPoint(points[3].values, this->cellSize, cell.heights[3], this->cellSize);
        TriangleNormal(cellNormals.faces[0].values, points[0].values, points[1].values, points[2].values);
        NormalizeGroundNormal(cellNormals.faces[0].values, true);
        TriangleNormal(cellNormals.faces[1].values, points[2].values, points[1].values, points[3].values);
        NormalizeGroundNormal(cellNormals.faces[1].values, true);
        cell.top = &tile;
      }

      if (row < this->height - 1) {
        const GroundCellRecord& below = CellRecord(resource, (row + 1) * this->width + column);
        const int sideIndex = record.tiles[1];

        if (sideIndex >= 0 && sideIndex < resource.tileCount) {
          const GroundTileRecord& tileRecord = TileRecord(resource, sideIndex);
          GroundQuad& tile = this->tiles[sideIndex];
          tile.color = tileRecord.color;
          tile.texture = g_texMgr.Load(resource.textureNames[tileRecord.texture], 0, false, true, false);
          SetPoint(tile.vertices[0], static_cast<float>(column - this->width / 2) * this->cellSize, record.heights[2],
                   static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
          SetPoint(tile.vertices[1], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, record.heights[3],
                   static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
          SetPoint(tile.vertices[2], static_cast<float>(column - this->width / 2) * this->cellSize, below.heights[0],
                   static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
          SetPoint(tile.vertices[3], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, below.heights[1],
                   static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
          const float scaleU = static_cast<float>(tile.texture->ImageWidth()) / static_cast<float>(tile.texture->TextureWidth());
          const float scaleV = static_cast<float>(tile.texture->ImageHeight()) / static_cast<float>(tile.texture->TextureHeight());

          for (int corner = 0; corner < 4; ++corner) {
            tile.vertices[corner][4] = scaleU * tileRecord.u[corner];
            tile.vertices[corner][5] = scaleV * tileRecord.v[corner];
          }

          GroundLightmap* lightmap = &this->lightmapSet.lightmaps[tileRecord.lightmap];
          tile.lightmap = lightmap;
          tile.lightmapTexture = lightmap->texture;

          for (int corner = 0; corner < 4; ++corner) {
            tile.vertices[corner][6] = lightmap->uv[corner][0];
            tile.vertices[corner][7] = lightmap->uv[corner][1];
          }

          SetPoint(cellNormals.side2.values, 0.0f, 0.0f, below.heights[0] <= record.heights[2] ? 1.0f : -1.0f);
          cell.side2 = &tile;
        }
      }

      if (column < this->width - 1) {
        const GroundCellRecord& right = CellRecord(resource, row * this->width + column + 1);
        const int sideIndex = record.tiles[2];

        if (sideIndex >= 0 && sideIndex < resource.tileCount) {
          const GroundTileRecord& tileRecord = TileRecord(resource, sideIndex);
          GroundQuad& tile = this->tiles[sideIndex];
          tile.color = tileRecord.color;
          tile.texture = g_texMgr.Load(resource.textureNames[tileRecord.texture], 0, false, true, false);
          SetPoint(tile.vertices[0], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, record.heights[3],
                   static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
          SetPoint(tile.vertices[1], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, record.heights[1],
                   static_cast<float>(row - this->height / 2) * this->cellSize);
          SetPoint(tile.vertices[2], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, right.heights[2],
                   static_cast<float>(row - this->height / 2 + 1) * this->cellSize);
          SetPoint(tile.vertices[3], static_cast<float>(column - this->width / 2 + 1) * this->cellSize, right.heights[0],
                   static_cast<float>(row - this->height / 2) * this->cellSize);
          const float scaleU = static_cast<float>(tile.texture->ImageWidth()) / static_cast<float>(tile.texture->TextureWidth());
          const float scaleV = static_cast<float>(tile.texture->ImageHeight()) / static_cast<float>(tile.texture->TextureHeight());

          for (int corner = 0; corner < 4; ++corner) {
            tile.vertices[corner][4] = scaleU * tileRecord.u[corner];
            tile.vertices[corner][5] = scaleV * tileRecord.v[corner];
          }

          GroundLightmap* lightmap = &this->lightmapSet.lightmaps[tileRecord.lightmap];
          tile.lightmap = lightmap;
          tile.lightmapTexture = lightmap->texture;

          for (int corner = 0; corner < 4; ++corner) {
            tile.vertices[corner][6] = lightmap->uv[corner][0];
            tile.vertices[corner][7] = lightmap->uv[corner][1];
          }

          cell.frame = 0xFFFFFFFFu;
          SetPoint(cellNormals.side1.values, right.heights[0] <= record.heights[2] ? 1.0f : -1.0f, 0.0f, 0.0f);
          cell.side1 = &tile;
        }
      }
    }
  }

  for (int row = 0; row < this->height; ++row) {
    for (int column = 0; column < this->width; ++column) {
      const int index = row * this->width + column;
      GroundCell& cell = this->cells[index];
      const CellNormals& cellNormals = normals[index];
      NormalVector smooth[4];
      const GroundCell* northwest;
      const CellNormals* northwestNormals;

      if (column > 0 && row > 0) {
        northwest = &this->cells[(row - 1) * this->width + column - 1];
        northwestNormals = &normals[(row - 1) * this->width + column - 1];
      } else {
        northwest = nullptr;
        northwestNormals = nullptr;
      }

      const GroundCell* north;
      const CellNormals* northNormals;

      if (row > 0) {
        north = &this->cells[(row - 1) * this->width + column];
        northNormals = &normals[(row - 1) * this->width + column];
      } else {
        north = nullptr;
        northNormals = nullptr;
      }

      const GroundCell* northeast;
      const CellNormals* northeastNormals;

      if (column < this->width - 1 && row > 0) {
        northeast = &this->cells[(row - 1) * this->width + column + 1];
        northeastNormals = &normals[(row - 1) * this->width + column + 1];
      } else {
        northeast = nullptr;
        northeastNormals = nullptr;
      }

      const GroundCell* west;
      const CellNormals* westNormals;

      if (column > 0) {
        west = &this->cells[row * this->width + column - 1];
        westNormals = &normals[row * this->width + column - 1];
      } else {
        west = nullptr;
        westNormals = nullptr;
      }

      const GroundCell* east;
      const CellNormals* eastNormals;

      if (column < this->width - 1) {
        east = &this->cells[row * this->width + column + 1];
        eastNormals = &normals[row * this->width + column + 1];
      } else {
        east = nullptr;
        eastNormals = nullptr;
      }

      const GroundCell* southwest;
      const CellNormals* southwestNormals;

      if (column > 0 && row < this->height - 1) {
        southwest = &this->cells[(row + 1) * this->width + column - 1];
        southwestNormals = &normals[(row + 1) * this->width + column - 1];
      } else {
        southwest = nullptr;
        southwestNormals = nullptr;
      }

      const GroundCell* south;
      const CellNormals* southNormals;

      if (row < this->height - 1) {
        south = &this->cells[(row + 1) * this->width + column];
        southNormals = &normals[(row + 1) * this->width + column];
      } else {
        south = nullptr;
        southNormals = nullptr;
      }

      const GroundCell* southeast;
      const CellNormals* southeastNormals;

      if (column < this->width - 1 && row < this->height - 1) {
        southeast = &this->cells[(row + 1) * this->width + column + 1];
        southeastNormals = &normals[(row + 1) * this->width + column + 1];
      } else {
        southeast = nullptr;
        southeastNormals = nullptr;
      }

      SetPoint(smooth[0].values, 0.0f, 0.0f, 0.0f);
      Add(smooth[0].values, cellNormals.faces[0].values);

      if (north && north->heights[2] == cell.heights[0]) {
        Add(smooth[0].values, northNormals->faces[0].values);
        Add(smooth[0].values, northNormals->faces[1].values);
      }

      if (west && west->heights[1] == cell.heights[0]) {
        Add(smooth[0].values, westNormals->faces[0].values);
        Add(smooth[0].values, westNormals->faces[1].values);
      }

      if (northwest && northwest->heights[3] == cell.heights[0]) {
        Add(smooth[0].values, northwestNormals->faces[1].values);
      }

      SetPoint(smooth[1].values, 0.0f, 0.0f, 0.0f);
      Add(smooth[1].values, cellNormals.faces[0].values);
      Add(smooth[1].values, cellNormals.faces[1].values);

      if (east && east->heights[0] == cell.heights[1]) {
        Add(smooth[1].values, eastNormals->faces[0].values);
      }

      if (north && north->heights[3] == cell.heights[1]) {
        Add(smooth[1].values, northNormals->faces[1].values);
      }

      if (northeast && northeast->heights[2] == cell.heights[1]) {
        Add(smooth[1].values, northeastNormals->faces[0].values);
        Add(smooth[1].values, northeastNormals->faces[1].values);
      }

      SetPoint(smooth[2].values, 0.0f, 0.0f, 0.0f);
      Add(smooth[2].values, cellNormals.faces[0].values);
      Add(smooth[2].values, cellNormals.faces[1].values);

      if (west && west->heights[3] == cell.heights[2]) {
        Add(smooth[2].values, westNormals->faces[1].values);
      }

      if (south && south->heights[0] == cell.heights[2]) {
        Add(smooth[2].values, southNormals->faces[0].values);
      }

      if (southwest && southwest->heights[1] == cell.heights[2]) {
        Add(smooth[2].values, southwestNormals->faces[0].values);
        Add(smooth[2].values, southwestNormals->faces[1].values);
      }

      SetPoint(smooth[3].values, 0.0f, 0.0f, 0.0f);
      Add(smooth[3].values, cellNormals.faces[0].values);

      if (east && east->heights[2] == cell.heights[3]) {
        Add(smooth[3].values, eastNormals->faces[0].values);
        Add(smooth[3].values, eastNormals->faces[1].values);
      }

      if (south && south->heights[1] == cell.heights[3]) {
        Add(smooth[3].values, southNormals->faces[0].values);
        Add(smooth[3].values, southNormals->faces[1].values);
      }

      if (southeast && southeast->heights[0] == cell.heights[3]) {
        Add(smooth[3].values, southeastNormals->faces[0].values);
      }

      if (cell.top) {
        for (int corner = 0; corner < 4; ++corner) {
          float* normal = smooth[corner].values;
          NormalizeGroundNormal(normal, false);
          const float dot = normal[1] * lightDirection[1] + normal[0] * lightDirection[0] + normal[2] * lightDirection[2];
          const float amount = dot > 0.0f ? dot : 0.0f;
          const float red = Clamp(amount * diffuse[0] + ambient[0]);
          const float green = Clamp(amount * diffuse[1] + ambient[1]);
          const float blue = Clamp(amount * diffuse[2] + ambient[2]);
          VertexColor color;
          color.value = 0xFFFFFFFFu;
          const GroundCell* source = nullptr;

          switch (corner) {
          case 0:
            source = &cell;
            break;
          case 1:
            source = east;
            break;
          case 2:
            source = south;
            break;
          case 3:
            source = southeast;
            break;
          }

          if (source && source->top) {
            color.value = source->top->color;
          }

          Shade(color, red, green, blue);
          cell.top->vertices[corner][3] = std::bit_cast<float>(color.value);
        }
      }

      if (cell.side2) {
        for (int corner = 0; corner < 4; ++corner) {
          const float* normal = cellNormals.side2.values;
          const float dot = lightDirection[1] * normal[1] + normal[0] * lightDirection[0] + lightDirection[2] * normal[2];
          const float amount = dot > 0.0f ? dot : 0.0f;
          const float red = Clamp(amount * diffuse[0] + ambient[0]);
          const float green = Clamp(amount * diffuse[1] + ambient[1]);
          const float blue = Clamp(amount * diffuse[2] + ambient[2]);
          VertexColor color;
          color.value = 0xFFFFFFFFu;
          const GroundCell* source = nullptr;

          switch (corner) {
          case 0:
          case 2:
            source = south;
            break;
          case 1:
          case 3:
            source = southeast;
            break;
          }

          if (source && source->top) {
            color.value = source->top->color;
          }

          Shade(color, red, green, blue);
          cell.side2->vertices[corner][3] = std::bit_cast<float>(color.value);
        }
      }

      if (cell.side1) {
        for (int corner = 0; corner < 4; ++corner) {
          const float* normal = cellNormals.side1.values;
          const float dot = lightDirection[1] * normal[1] + normal[0] * lightDirection[0] + lightDirection[2] * normal[2];
          const float amount = dot > 0.0f ? dot : 0.0f;
          const float red = Clamp(amount * diffuse[0] + ambient[0]);
          const float green = Clamp(amount * diffuse[1] + ambient[1]);
          const float blue = Clamp(amount * diffuse[2] + ambient[2]);
          VertexColor color;
          color.value = 0xFFFFFFFFu;
          const GroundCell* source = nullptr;

          switch (corner) {
          case 0:
          case 2:
            source = southeast;
            break;
          case 1:
          case 3:
            source = east;
            break;
          }

          if (source && source->top) {
            color.value = source->top->color;
          }

          Shade(color, red, green, blue);
          cell.side1->vertices[corner][3] = std::bit_cast<float>(color.value);
        }
      }
    }
  }

  delete[] normals;
}

// OFFSET: 0x00ABFB00
// FUNCTION: fn_17557
void GroundBase::Build(CGndRes&, const float*, const float*, const float*) {
}

// OFFSET: 0x00AC0990
// FUNCTION: fn_17566
void GroundGeometry::RenderLod(int) {
}

// OFFSET: 0x00AC09A0
// FUNCTION: fn_17567
void GroundBase::RenderLod(int) {
}

// OFFSET: 0x00AC1510
// FUNCTION: fn_17582
void GroundBase::UpdateWater() {
}

// OFFSET: 0x00AC1520
// FUNCTION: fn_17583
bool GroundGeometry::IsNewVer() {
  return true;
}

// OFFSET: 0x00AC3080
// FUNCTION: fn_17598
void GroundBase::SetGndRiseAmount(float, int, int) {
}

// OFFSET: 0x00AC1530
// FUNCTION: fn_17584
bool GroundBase::IsNewVer() {
  return false;
}

// OFFSET: 0x00AC3030
// FUNCTION: fn_17596
void GroundBase::ClearGeometry() {
}

// OFFSET: 0x00AC3370
// FUNCTION: fn_17601
void GroundBase::ImportWater(const CGndRes&) {
}

// OFFSET: 0x00AC38A0
// FUNCTION: fn_17605
void GroundBase::DrawTerrainOverlay(const float*, int, int, std::uint32_t, CTexture*) const {
}
