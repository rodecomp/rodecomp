#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELRENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELRENDERING_H

#include "Render/Primitives/RenderPrimitive.h"

#include <array>

struct RenderProjection;
struct RenderQueues;
class RenderCommandPool;
class RenderVertexPool;
class C3dNode;
struct ModelTextureTransform;

struct ModelFaceState {
  /* 0000 */ std::uint32_t colors[3];
  /* 000C */ float uv[3][2];
};

struct ModelLightingContext {
  const float* position;
  const std::uint32_t* color;
  const float* direction;
  const float* diffuse;
  const float* ambient;
  const int* shade;
  const class GroundGeometry& ground;

  std::array<std::uint32_t, 90000> colors{};
};

struct ModelRenderContext {
  const RenderProjection& projection;
  RenderCommandPool& commands;
  RenderVertexPool& vertices;
  RenderQueues& queues;
  const int& width;
  const int& height;
  const bool& depthAlpha;
  const float& depthStart;
  const float& depthEnd;
  const bool& overrideColor;
  const bool& integerColor;
  const float* overrideScale;
  const float* colorScale;

  std::array<RenderVertex, 30000> legacyProjected{};
  std::array<RenderVertex, 30000> modernProjected{};

  ModelRenderContext(
    const RenderProjection& projection, RenderCommandPool& commands, RenderVertexPool& vertices, RenderQueues& queues, const int& width, const int& height,
    const bool& depthAlpha, const float& depthStart, const float& depthEnd, const bool& overrideColor, const bool& integerColor, const float* overrideScale,
    const float* colorScale
  );
};

bool ModelTriangleVisible(const RenderVertex* a, const RenderVertex* b, const RenderVertex* c, const ModelRenderContext& context);
void TransformModelUV(float& u, float& v, const ModelTextureTransform& transform);
void DrawModelNode(C3dNode& node, const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context, bool modern);

static_assert(sizeof(ModelFaceState) == 36);

#endif
