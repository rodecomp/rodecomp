#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELRENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELRENDERING_H

#include "Render/Primitives/RenderPrimitive.h"

class C3dNode;
struct ModelTextureTransform;

struct ModelFaceState {
  /* 0000 */ std::uint32_t colors[3];
  /* 000C */ float uv[3][2];
};

bool ModelTriangleVisible(const RenderVertex* a, const RenderVertex* b, const RenderVertex* c);
void TransformModelUV(float& u, float& v, const ModelTextureTransform& transform);

static_assert(sizeof(ModelFaceState) == 36);

#endif
