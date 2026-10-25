#include "Contents/Ground/TerrainHeight.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00749040
// FUNCTION: fn_07840
float TerrainHeight::PlaneHeight(float x, float z) const {
  const float size = static_cast<float>(this->cellSize);
  const int columns = this->width;
  const int rows = this->height;
  const int column = TruncateToInt(((static_cast<float>(columns) * size) * 0.5f + x) / size);
  const int row = TruncateToInt(((static_cast<float>(rows) * size) * 0.5f + z) / size);
  const TerrainHeightCell* cell = this->CellAt(column, row);

  if (!cell) {
    return 0.0f;
  }

  const int dx = column - columns / 2;
  const int dz = row - rows / 2;
  const float left = static_cast<float>(dx) * size;
  const float right = static_cast<float>(dx + 1) * size;
  const float top = static_cast<float>(dz) * size;
  const float bottom = static_cast<float>(dz + 1) * size;
  struct Point {
    float x;
    float y;
    float z;
  };
  const Point first{left, cell->heights[0], top};
  const Point second{right, cell->heights[1], top};
  const Point third{left, cell->heights[2], bottom};
  const float normalX = ((third.z - first.z) * second.y + (second.z - third.z) * first.y) + (first.z - second.z) * third.y;
  const float normalZ = ((third.y - first.y) * second.x + (second.y - third.y) * first.x) + (first.y - second.y) * third.x;
  const float constant = (((first.z * third.y - third.z * first.y) * second.x + (third.z * second.y - second.z * third.y) * first.x) +
                          (second.z * first.y - first.z * second.y) * third.x) *
                         -1.0f;
  const float normalY = ((second.x - third.x) * first.z + (third.x - first.x) * second.z) + (first.x - second.x) * third.z;

  return ((normalX * x + normalZ * z) + constant) * -1.0f / normalY;
}
