#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00ABFB10
// FUNCTION: fn_17558
void GroundGeometry::InitializeLighting(const C3dAttr& terrain) {
  // STATIC: 0x01348128; center, north, south, east, west in that order.
  constexpr int offsets[5][2] = {{0, 0}, {0, -1}, {0, 1}, {1, 0}, {-1, 0}};
  const int columns = 2 * this->width;
  const int rows = 2 * this->height;

  for (int row = 0; row < rows; ++row) {
    for (int column = 0; column < columns; ++column) {
      float average = 0.0f;
      const TerrainHeightCell* cell = terrain.Grid().CellAt(column, row);

      if (cell) {
        average = ((cell->heights[1] + cell->heights[0]) + cell->heights[2] + cell->heights[3]) * 0.25f;
      }

      float minimum;
      float maximum;
      this->CellBounds(column / 2, row / 2, minimum, maximum);
      float red = 1.0f;
      float green = 1.0f;
      float blue = 1.0f;

      if (average >= minimum - 2.5f) {
        unsigned sums[4]{};

        for (const auto& offset : offsets) {
          unsigned char alpha;
          unsigned char sampleRed;
          unsigned char sampleGreen;
          unsigned char sampleBlue;
          this->LightmapSample(column + offset[0], row + offset[1], alpha, sampleRed, sampleGreen, sampleBlue);
          sums[0] += alpha;
          sums[1] += sampleRed;
          sums[2] += sampleGreen;
          sums[3] += sampleBlue;
        }

        const float alpha = (static_cast<float>(sums[0]) / 5.0f) / 255.0f;
        red = ((static_cast<float>(sums[1]) / 5.0f) * 0.0078125f) + alpha;
        green = ((static_cast<float>(sums[2]) / 5.0f) * 0.0078125f) + alpha;
        blue = ((static_cast<float>(sums[3]) / 5.0f) * 0.0078125f) + alpha;
        const float first = green > red ? green : red;
        const float largest = blue > first ? blue : first;

        if (largest > 1.0f) {
          red = red / largest;
          green = green / largest;
          blue = blue / largest;
        }
      }

      if (!this->lighting.empty()) {
        const auto index = static_cast<unsigned>(column) + 2u * static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
        this->lighting[index] = {red, green, blue};
      }
    }
  }
}
