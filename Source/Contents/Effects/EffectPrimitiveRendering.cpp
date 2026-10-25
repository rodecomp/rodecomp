#include "Contents/Effects/EffectPrimitiveRendering.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/EffectVisibility.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"

// OFFSET: 0x00BCD960
// FUNCTION: fn_19865
void DrawEffectPrimitive15(EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context) {
  const float width = particle.unknown_0354;
  const float left = -width;
  float top = -particle.unknown_0360;
  float bottom = particle.unknown_0360;

  if ((particle.flags & 0x80u) != 0) {
    top = top + top;
    bottom = 0.0f;
  }

  if ((particle.flags & 8u) != 0) {
    bottom = 0.0f;
  }

  const float* matrix = particle.matrix2;
  const float zeroX = matrix[0] * 0.0f;
  const float zeroY = matrix[1] * 0.0f;
  const float zeroZ = matrix[2] * 0.0f;
  const float topX = matrix[3] * top + zeroX;
  const float topY = matrix[4] * top + zeroY;
  const float topZ = matrix[5] * top + zeroZ;
  const float rightX = matrix[6] * width;
  const float rightY = matrix[7] * width;
  const float rightZ = matrix[8] * width;
  const float leftX = matrix[6] * left;
  const float leftY = matrix[7] * left;
  const float leftZ = matrix[8] * left;
  const float bottomX = bottom * matrix[3] + zeroX;
  const float bottomY = bottom * matrix[4] + zeroY;
  const float bottomZ = bottom * matrix[5] + zeroZ;

  const EffectVector3 corners[4] = {
    {(rightX + topX) + matrix[9], (rightY + topY) + matrix[10], (rightZ + topZ) + matrix[11]},
    {(leftX + topX) + matrix[9], (leftY + topY) + matrix[10], (leftZ + topZ) + matrix[11]},
    {(bottomX + rightX) + matrix[9], (bottomY + rightY) + matrix[10], (bottomZ + rightZ) + matrix[11]},
    {(bottomX + leftX) + matrix[9], (bottomY + leftY) + matrix[10], (bottomZ + leftZ) + matrix[11]}
  };

  constexpr unsigned indices[2][3] = {{0, 1, 2}, {3, 2, 1}};
  constexpr float u[2][3] = {{0, 1, 0}, {1, 0, 1}};
  const float vTop = particle.unknown_02E4;
  const float vBottom = particle.unknown_02E8;
  const float v[2][3] = {{vTop, vTop, vBottom}, {vBottom, vBottom, vTop}};

  for (unsigned triangleIndex = 0; triangleIndex < 2; ++triangleIndex) {
    RenderTriangle& triangle = particle.triangles[triangleIndex];

    for (unsigned vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
      const EffectVector3& corner = corners[indices[triangleIndex][vertexIndex]];
      const float position[3] = {corner.x + render.position[0], corner.y + render.position[1], corner.z + render.position[2]};
      RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      context.projection.Project(position, context.cameraMatrix, projected);
      RenderVertex& vertex = triangle.vertices[vertexIndex];
      vertex = projected;
      vertex.diffuse = particle.unknown_01C0;
      vertex.u = u[triangleIndex][vertexIndex];
      vertex.v = v[triangleIndex][vertexIndex];
    }

    triangle.texture = particle.textures[render.unknown_003C];
  }

  for (unsigned index = 0; index < 2; ++index) {
    RenderTriangle& triangle = particle.triangles[index];

    if (triangle.Visible()) {
      context.queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(particle.unknown_01C4), context.commands, context.depthAlpha, context.depthStart, context.depthEnd
      );
    }
  }
}

// OFFSET: 0x00BCE350
// FUNCTION: fn_19866
void RenderEffectPrimitive15(
  EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context, const EffectVisibilityContext& visibility
) {
  if (particle.parent && PartyEffectVisible(*particle.parent, visibility)) {
    DrawEffectPrimitive15(particle, render, context);
  }
}
