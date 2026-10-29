#include "Contents/Ground/GroundGeometry.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/GroundResource.h"

#include "Utility/Math/ScalarConversion.h"
#include <bit>
#include <cmath>
#include <cstring>

namespace {
  struct CellNormals {
    float values[4][3]{};
  };

  // INLINED
  float InverseLength(float squared) {
    const float length = std::sqrt(squared);
    return 1.0f / length;
  }

  // INLINED
  void Normalize(float* vector, bool yFirst) {
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

  // INLINED
  std::uint32_t
  LitColor(const float* normal, bool normalize, bool side, const float* direction, const float* diffuse, const float* ambient, const GroundQuad* source) {
    float vector[3] = {normal[0], normal[1], normal[2]};

    if (normalize) {
      Normalize(vector, false);
    }

    const float dot = side ? (direction[1] * vector[1] + vector[0] * direction[0]) + direction[2] * vector[2]
                           : (vector[1] * direction[1] + vector[0] * direction[0]) + vector[2] * direction[2];
    const float amount = dot > 0.0f ? dot : 0.0f;
    const std::uint32_t color = source ? source->color : 0xFFFFFFFFu;
    std::uint32_t result = 0xFF000000u;

    for (unsigned channel = 0; channel < 3; ++channel) {
      const float illumination = amount * diffuse[channel] + ambient[channel];
      const float light = illumination < 1.0f ? illumination : 1.0f;
      const unsigned shift = 16 - channel * 8;
      const float scaled = static_cast<float>((color >> shift) & 255u) * light;
      const unsigned value = static_cast<unsigned>(TruncateToInt(scaled)) & 255u;
      result |= value << shift;
    }

    return result;
  }
}

// OFFSET: 0x00ABD640
// FUNCTION: fn_17546
GroundGeometry::GroundGeometry() : cellSize(10.0f), loaded(0) {
}

// INLINED
GroundGeometry::~GroundGeometry() {
  this->ClearLightmaps();
}

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
  this->cells.resize(static_cast<unsigned>(columns) * static_cast<unsigned>(rows));
  this->lighting.resize(4u * static_cast<unsigned>(columns) * static_cast<unsigned>(rows), {1.0f, 1.0f, 1.0f});
  this->unknown_005C = 0;
  this->unknown_0058 = 0;
}

// INLINED
GroundSpatial GroundGeometry::Spatial() const {
  return {this->width, this->height, this->cellSize, this->cells};
}

// OFFSET: 0x00ABDEF0
// FUNCTION: fn_17556
void GroundGeometry::Build(CGndRes& resource, const float* lightDirection, const float* diffuse, const float* ambient) {
  this->loaded = resource.loaded;
  this->Reset(resource.width, resource.height);
  std::vector<CellNormals> normals(this->cells.size());
  this->cellSize = resource.cellSize;
  this->BuildLightmaps(resource);
  this->tiles.resize(static_cast<unsigned>(resource.tileCount));

  for (int row = 0; row < this->height; ++row) {
    for (int column = 0; column < this->width; ++column) {
      const unsigned index = static_cast<unsigned>(row) * static_cast<unsigned>(this->width) + static_cast<unsigned>(column);
      GroundCell& cell = this->cells[index];
      const unsigned char* record = resource.cells + 28u * index;
      std::memcpy(cell.heights, record, sizeof(cell.heights));
      const float left = static_cast<float>(column - this->width / 2) * this->cellSize;
      const float right = static_cast<float>(column - this->width / 2 + 1) * this->cellSize;
      const float top = static_cast<float>(row - this->height / 2) * this->cellSize;
      const float bottom = static_cast<float>(row - this->height / 2 + 1) * this->cellSize;
      const float corners[4][3] = {{left, 0, top}, {right, 0, top}, {left, 0, bottom}, {right, 0, bottom}};
      std::memcpy(cell.corners, corners, sizeof(corners));
      int tileIndices[3];
      std::memcpy(tileIndices, record + 16, sizeof(tileIndices));

      for (unsigned face = 0; face < 3; ++face) {
        if ((face == 1 && row >= this->height - 1) || (face == 2 && column >= this->width - 1)) {
          continue;
        }

        const int tileIndex = tileIndices[face];

        if (tileIndex < 0 || tileIndex >= resource.tileCount) {
          continue;
        }

        GroundQuad& tile = this->tiles[tileIndex];
        const unsigned char* tileBytes = resource.tiles + 40u * static_cast<unsigned>(tileIndex);
        std::memcpy(&tile.color, tileBytes + 36, sizeof(tile.color));
        std::int16_t textureIndex;
        std::uint16_t lightmapIndex;
        std::memcpy(&textureIndex, tileBytes + 32, sizeof(textureIndex));
        std::memcpy(&lightmapIndex, tileBytes + 34, sizeof(lightmapIndex));
        tile.texture = g_texMgr.Load(resource.textureNames[textureIndex], 0, false, true, false);
        float positions[4][3];

        if (face == 0) {
          for (unsigned corner = 0; corner < 4; ++corner) {
            positions[corner][0] = cell.corners[corner][0];
            positions[corner][1] = cell.heights[corner];
            positions[corner][2] = cell.corners[corner][2];
          }
        } else if (face == 1) {
          float next[4];
          std::memcpy(next, resource.cells + 28u * (index + this->width), sizeof(next));
          const float values[4][3] = {{left, cell.heights[2], bottom}, {right, cell.heights[3], bottom}, {left, next[0], bottom}, {right, next[1], bottom}};
          std::memcpy(positions, values, sizeof(positions));
          normals[index].values[2][2] = next[0] <= cell.heights[2] ? 1.0f : -1.0f;
        } else {
          float next[4];
          std::memcpy(next, resource.cells + 28u * (index + 1), sizeof(next));
          const float values[4][3] = {{right, cell.heights[3], bottom}, {right, cell.heights[1], top}, {right, next[2], bottom}, {right, next[0], top}};
          std::memcpy(positions, values, sizeof(positions));
          normals[index].values[3][0] = next[0] <= cell.heights[2] ? 1.0f : -1.0f;
        }

        const float scaleU = static_cast<float>(tile.texture->Width()) / static_cast<float>(tile.texture->TextureWidth());
        const float scaleV = static_cast<float>(tile.texture->Height()) / static_cast<float>(tile.texture->TextureHeight());
        GroundLightmap& lightmap = this->lightmaps[lightmapIndex];

        tile.lightmap = &lightmap;
        tile.lightmapTexture = lightmap.texture;

        for (unsigned corner = 0; corner < 4; ++corner) {
          std::memcpy(tile.vertices[corner], positions[corner], sizeof(positions[corner]));
          float u;
          float v;
          std::memcpy(&u, tileBytes + corner * 4, sizeof(u));
          std::memcpy(&v, tileBytes + 16 + corner * 4, sizeof(v));
          tile.vertices[corner][4] = scaleU * u;
          tile.vertices[corner][5] = scaleV * v;
          tile.vertices[corner][6] = lightmap.uv[corner][0];
          tile.vertices[corner][7] = lightmap.uv[corner][1];
        }

        if (face == 0) {
          for (unsigned channel = 0; channel < 3; ++channel) {
            const unsigned value = 2u * lightmap.color[channel];
            cell.color[channel] = static_cast<unsigned char>(value > 255 ? 255 : value);
          }

          float* first = normals[index].values[0];
          float* second = normals[index].values[1];
          const float ab = cell.heights[2] - cell.heights[0];
          const float ac = cell.heights[2] - cell.heights[1];

          first[0] = ac * (this->cellSize - 0.0f) - (this->cellSize - 0.0f) * ab;
          first[1] = (this->cellSize - 0.0f) * 0.0f - (this->cellSize - 0.0f) * (0.0f - this->cellSize);
          first[2] = ab * (0.0f - this->cellSize) - ac * 0.0f;
          Normalize(first, true);
          const float db = cell.heights[3] - cell.heights[1];
          const float dc = cell.heights[3] - cell.heights[2];
          const float dz = this->cellSize - 0.0f;
          const float dx = this->cellSize - 0.0f;
          const float zero = this->cellSize - this->cellSize;

          second[0] = db * zero - dx * dc;
          second[1] = dz * dx - zero * zero;
          second[2] = zero * dc - dz * db;
          Normalize(second, true);
          cell.top = &tile;
        } else if (face == 1) {
          cell.side2 = &tile;
        } else {
          cell.frame = 0xFFFFFFFFu;
          cell.side1 = &tile;
        }
      }
    }
  }

  for (int row = 0; row < this->height; ++row) {
    for (int column = 0; column < this->width; ++column) {
      const unsigned index = static_cast<unsigned>(row) * static_cast<unsigned>(this->width) + static_cast<unsigned>(column);
      GroundCell& cell = this->cells[index];
      const auto neighbor = [&](int x, int y) -> const GroundCell* {
        return x >= 0 && x < this->width && y >= 0 && y < this->height
                 ? &this->cells[static_cast<unsigned>(y) * static_cast<unsigned>(this->width) + static_cast<unsigned>(x)]
                 : nullptr;
      };
      const GroundCell* north = neighbor(column, row - 1);
      const GroundCell* west = neighbor(column - 1, row);
      const GroundCell* northwest = neighbor(column - 1, row - 1);
      const GroundCell* northeast = neighbor(column + 1, row - 1);
      const GroundCell* east = neighbor(column + 1, row);
      const GroundCell* south = neighbor(column, row + 1);
      const GroundCell* southwest = neighbor(column - 1, row + 1);
      const GroundCell* southeast = neighbor(column + 1, row + 1);
      float smooth[4][3]{};
      const auto add = [&](unsigned corner, const GroundCell* source, unsigned first, unsigned count) {
        const auto sourceIndex = static_cast<unsigned>(source - this->cells.data());

        for (unsigned normal = first; normal < first + count; ++normal) {
          Add(smooth[corner], normals[sourceIndex].values[normal]);
        }
      };
      add(0, &cell, 0, 1);

      if (north && north->heights[2] == cell.heights[0]) {
        add(0, north, 0, 2);
      }

      if (west && west->heights[1] == cell.heights[0]) {
        add(0, west, 0, 2);
      }

      if (northwest && northwest->heights[3] == cell.heights[0]) {
        add(0, northwest, 1, 1);
      }

      add(1, &cell, 0, 2);

      if (east && east->heights[0] == cell.heights[1]) {
        add(1, east, 0, 1);
      }

      if (north && north->heights[3] == cell.heights[1]) {
        add(1, north, 1, 1);
      }

      if (northeast && northeast->heights[2] == cell.heights[1]) {
        add(1, northeast, 0, 2);
      }

      add(2, &cell, 0, 2);

      if (west && west->heights[3] == cell.heights[2]) {
        add(2, west, 1, 1);
      }

      if (south && south->heights[0] == cell.heights[2]) {
        add(2, south, 0, 1);
      }

      if (southwest && southwest->heights[1] == cell.heights[2]) {
        add(2, southwest, 0, 2);
      }

      add(3, &cell, 0, 1);

      if (east && east->heights[2] == cell.heights[3]) {
        add(3, east, 0, 2);
      }

      if (south && south->heights[1] == cell.heights[3]) {
        add(3, south, 0, 2);
      }

      if (southeast && southeast->heights[0] == cell.heights[3]) {
        add(3, southeast, 0, 1);
      }

      if (cell.top) {
        const GroundCell* sources[4] = {&cell, east, south, southeast};

        for (unsigned corner = 0; corner < 4; ++corner) {
          const GroundQuad* source = sources[corner] ? sources[corner]->top : nullptr;
          const auto color = LitColor(smooth[corner], true, false, lightDirection, diffuse, ambient, source);
          cell.top->vertices[corner][3] = std::bit_cast<float>(color);
        }
      }

      if (cell.side2) {
        for (unsigned corner = 0; corner < 4; ++corner) {
          const GroundCell* source = (corner & 1) ? southeast : south;
          const auto color = LitColor(normals[index].values[2], false, true, lightDirection, diffuse, ambient, source ? source->top : nullptr);
          cell.side2->vertices[corner][3] = std::bit_cast<float>(color);
        }
      }

      if (cell.side1) {
        for (unsigned corner = 0; corner < 4; ++corner) {
          const GroundCell* source = (corner & 1) ? east : southeast;
          const auto color = LitColor(normals[index].values[3], false, true, lightDirection, diffuse, ambient, source ? source->top : nullptr);
          cell.side1->vertices[corner][3] = std::bit_cast<float>(color);
        }
      }
    }
  }
}
