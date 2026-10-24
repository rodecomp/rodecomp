#ifndef RAGNAROK_SOURCE_CONTENTS_MODELRENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_MODELRENDERING_H

#include "Render/RenderPrimitive.h"

#include <array>

struct RenderProjection;
struct RenderQueues;
class RenderCommandPool;
class RenderVertexPool;
class C3dNode;
struct ModelTextureTransform;

// Runtime face data is colors then three UV pairs, not nine numeric positions.
struct ModelFaceState {
  std::uint32_t colors[3];
  float uv[3][2];
};

struct ModelLightingContext {
  const float* position;
  const std::uint32_t* color;
  const float* direction;
  const float* diffuse;
  const float* ambient;
  const int* shade;
  const class GroundGeometry& ground;
  // Shared color scratch; unknown shade values retain it.
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
  // Distinct original static scratch arrays, initialized once to 30000 vertices.
  // Projection changes only x/y/z/rhw; the other fields retain their values.
  std::array<RenderVertex, 30000> legacyProjected{};
  std::array<RenderVertex, 30000> modernProjected{};

  ModelRenderContext(
    const RenderProjection& projection,
    RenderCommandPool& commands,
    RenderVertexPool& vertices,
    RenderQueues& queues,
    const int& width,
    const int& height,
    const bool& depthAlpha,
    const float& depthStart,
    const float& depthEnd,
    const bool& overrideColor,
    const bool& integerColor,
    const float* overrideScale,
    const float* colorScale
  );
};

bool ModelTriangleVisible(
  const RenderVertex* a,
  const RenderVertex* b,
  const RenderVertex* c,
  const ModelRenderContext& context
);
void TransformModelUV(float& u, float& v, const ModelTextureTransform& transform);
void DrawModelNode(
  C3dNode& node,
  const float* matrix,
  unsigned char reversed,
  bool clip,
  unsigned char commandMode,
  ModelRenderContext& context,
  bool modern
);

static_assert(sizeof(ModelFaceState) == 36);

#endif
