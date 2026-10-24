#ifndef RAGNAROK_SOURCE_RESOURCE_TERRAINRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_TERRAINRESOURCE_H

#include "Contents/TerrainHeight.h"
#include "Resource/Resource.h"

#include <vector>

class ResourceBytes;

struct TerrainResourceContext {
  ResourceBytes& file;
  CRenderSystemBase*& renderSystem;
};

// RTTI C3dAttr. Unload clears dimensions but
// retains cells; destruction frees storage. Context follows original fields.
class C3dAttr : public CRes {
public:
  explicit C3dAttr(TerrainResourceContext& context);
  ~C3dAttr() override;
  C3dAttr* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  TerrainHeight Grid() const;

  /* 0110 */ int width;
  /* 0114 */ int height;
  /* 0118 */ int cellSize;

private:
  // The original's trivial cell constructor leaves new records unwritten.
  struct Cell : TerrainHeightCell {
    Cell() noexcept {
    }
  };
  /* 011C */ std::vector<Cell> m_cells;
  TerrainResourceContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(C3dAttr) == 300);
#endif

#endif
