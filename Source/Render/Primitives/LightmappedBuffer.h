#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDBUFFER_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDBUFFER_H

#include "Render/Primitives/LightmappedPrimitive.h"

struct LightmappedBuffer : LightmappedCommand {
  // INLINED
  ~LightmappedBuffer() {
    this->Clear();
  }

  void Clear();
};

#endif
