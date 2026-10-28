#include "Contents/TerrainPathSearch.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00AD85A0

// OFFSET: 0x00AD8DB0
// FUNCTION: fn_17872
int CPathFinder::Height() const {
  return this->attr->height;
}

// OFFSET: 0x00AD8DC0
// FUNCTION: fn_17873
int CPathFinder::Width() const {
  return this->attr->width;
}
