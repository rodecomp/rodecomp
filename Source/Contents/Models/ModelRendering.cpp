#include "Contents/Models/ModelActor.h"
#include "Contents/Models/ModelNode.h"
#include "Contents/Models/ModelRendering.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Primitives/RenderVertexPool.h"
#include "Render/Textures/Texture.h"

#include "Utility/Math/ScalarConversion.h"
#include <cstring>

// INLINED
ModelRenderContext::ModelRenderContext(
  const RenderProjection& projection, RenderCommandPool& commands, RenderVertexPool& vertices, RenderQueues& queues, const int& width, const int& height,
  const bool& depthAlpha, const float& depthStart, const float& depthEnd, const bool& overrideColor, const bool& integerColor, const float* overrideScale,
  const float* colorScale
)
    : projection(projection), commands(commands), vertices(vertices), queues(queues), width(width), height(height), depthAlpha(depthAlpha),
      depthStart(depthStart), depthEnd(depthEnd), overrideColor(overrideColor), integerColor(integerColor), overrideScale(overrideScale),
      colorScale(colorScale) {
  for (auto& vertex : this->legacyProjected) {
    vertex.specular = 0xFF000000u;
  }

  for (auto& vertex : this->modernProjected) {
    vertex.specular = 0xFF000000u;
  }
}

// INLINED
bool ModelTriangleVisible(const RenderVertex* a, const RenderVertex* b, const RenderVertex* c, const ModelRenderContext& context) {
  const float firstMinimumX = a->x > b->x ? b->x : a->x;
  const float minimumX = firstMinimumX > c->x ? c->x : firstMinimumX;

  if (minimumX >= static_cast<float>(context.width)) {
    return false;
  }

  const float firstMaximumX = b->x > a->x ? b->x : a->x;
  const float maximumX = c->x > firstMaximumX ? c->x : firstMaximumX;

  if (0.0f > maximumX) {
    return false;
  }

  const float firstMinimumY = a->y > b->y ? b->y : a->y;
  const float minimumY = firstMinimumY > c->y ? c->y : firstMinimumY;

  if (minimumY >= static_cast<float>(context.height)) {
    return false;
  }

  const float firstMaximumY = b->y > a->y ? b->y : a->y;
  const float maximumY = c->y > firstMaximumY ? c->y : firstMaximumY;

  return !(0.0f > maximumY || 0.0f > a->rhw || 0.0f > b->rhw || 0.0f > c->rhw);
}

namespace {
  // INLINED
  unsigned Byte(float value) {
    return static_cast<unsigned>(TruncateToInt(value)) & 255u;
  }

  // INLINED
  std::uint32_t Color(std::uint32_t source, unsigned char alpha, const float* scale) {
    const unsigned red = Byte(static_cast<float>((source >> 16) & 255u) * scale[0]);
    const unsigned green = Byte(static_cast<float>((source >> 8) & 255u) * scale[1]);
    const unsigned blue = Byte(static_cast<float>(source & 255u) * scale[2]);

    return (static_cast<unsigned>(alpha) << 24) | (red << 16) | (green << 8) | blue;
  }

  // INLINED
  float Rotated(float sine, float delta, float cosineProduct, bool subtract) {
    const double product = static_cast<double>(sine) * delta;
    return static_cast<float>(subtract ? cosineProduct - product : product + cosineProduct);
  }
}

// INLINED
inline void TransformModelUV(float& u, float& v, const ModelTextureTransform& transform) {
  const float cosineU = ClientCos(transform.values[4]);
  const float productU = cosineU * (u - 0.5f);
  const float sineU = ClientSin(transform.values[4]);
  const float rotatedU = Rotated(sineU, v - 0.5f, productU, true);
  const float resultU = (rotatedU * transform.values[2] + 0.5f) + transform.values[0];
  const float cosineV = ClientCos(transform.values[4]);
  const float productV = cosineV * (v - 0.5f);
  const float sineV = ClientSin(transform.values[4]);
  const float rotatedV = Rotated(sineV, u - 0.5f, productV, false);
  const float resultV = (rotatedV * transform.values[3] + 0.5f) + transform.values[1];

  u = resultU;
  v = resultV;
}

// INLINED
void DrawModelNode(C3dNode& node, const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context, bool modern) {
  AffineMatrix camera;
  std::memcpy(camera.values, matrix, sizeof(camera.values));
  AffineMatrix combined;
  combined.Compose(node.transform, camera);
  const float overrideScale[3] = {context.overrideScale[0], context.overrideScale[1], context.overrideScale[2]};
  const float colorScale[3] = {context.colorScale[0], context.colorScale[1], context.colorScale[2]};
  const unsigned char alpha = node.model->alpha;
  auto& projected = modern ? context.modernProjected : context.legacyProjected;

  for (int index = 0; index < node.mesh->vertexCount; ++index) {
    context.projection.Project(node.mesh->vertices[index].data(), combined.values, projected[index]);
  }

  RenderVertex* output = context.vertices.Acquire(static_cast<unsigned>(node.mesh->faceCount) * 3u);
  int index = 0;

  while (index < node.mesh->faceCount) {
    const unsigned texture = node.mesh->faces[index].texture;
    RenderVertex* first = output;

    std::uint16_t flags = 0;

    do {
      const ModelFace& face = node.mesh->faces[index];
      const RenderVertex& a = projected[face.vertices[0]];
      const RenderVertex& b = projected[face.vertices[1]];
      const RenderVertex& c = projected[face.vertices[2]];
      bool visible = !clip || ModelTriangleVisible(&a, &b, &c, context);
      const float left = (b.y - a.y) * (c.x - a.x);
      const float right = (c.y - a.y) * (b.x - a.x);
      const bool winding = modern ? right > left : left > right;
      const bool accepted = modern ? (winding ? reversed == 0 : reversed == 1) : (winding ? reversed == 0 : reversed != 0);
      visible = visible && (accepted || (modern ? face.twoSided == 1 : face.twoSided != 0));

      if (visible) {
        output[0] = a;
        output[1] = b;
        output[2] = c;
        const ModelFaceState& state = node.faceState[index];

        for (unsigned corner = 0; corner < 3; ++corner) {
          output[corner].u = state.uv[corner][0];
          output[corner].v = state.uv[corner][1];
        }

        if (modern) {
          const auto& transforms = static_cast<C3dNodeV2&>(node).textureTransforms;
          const auto found = transforms.find(static_cast<int>(texture));

          if (found != transforms.end()) {
            for (unsigned corner = 0; corner < 3; ++corner) {
              TransformModelUV(output[corner].u, output[corner].v, found->second);
            }

            flags = 0x20;
          }
        }

        for (unsigned corner = 0; corner < 3; ++corner) {
          if (context.overrideColor) {
            output[corner].diffuse = Color(state.colors[corner], alpha, overrideScale);
          } else if (node.unknown_0088) {
            output[corner].diffuse = (state.colors[corner] & 0x00FFFFFFu) | 0x80000000u;
          } else if (context.integerColor) {
            const int* values = node.faceValues.data() + index * 9u + corner * 3u;
            output[corner].diffuse = (static_cast<unsigned>(alpha) << 24) | (Byte(static_cast<float>(values[0]) * colorScale[0]) << 16) |
                                     (Byte(static_cast<float>(values[1]) * colorScale[1]) << 8) | Byte(static_cast<float>(values[2]) * colorScale[2]);
          } else {
            output[corner].diffuse = Color(state.colors[corner], alpha, colorScale);
          }
        }

        output += 3;
      }

      ++index;
    } while (index < node.mesh->faceCount && node.mesh->faces[index].texture == texture);

    if (output != first) {
      RenderCommand* command = context.commands.Acquire();
      command->vertices = first;
      command->vertexCount = static_cast<unsigned>(output - first);
      command->texture = node.textures[texture % node.textures.size()];
      command->unknown_0014 = static_cast<std::uint16_t>((command->unknown_0014 & 0x00FFu) | (static_cast<unsigned>(commandMode) << 8));

      if (alpha != 255 || command->texture->Format() == TextureFormat::A4R4G4B4) {
        flags |= 1;
      }

      context.queues.Submit(command, flags, context.depthAlpha, context.depthStart, context.depthEnd);
    }
  }

  node.unknown_0088 = 0;

  for (C3dNode* child : node.children) {
    child->Draw(combined.values, reversed, clip, commandMode, context);
  }
}

// OFFSET: 0x00AB7D40
// FUNCTION: fn_17499
void C3dNodeV1::Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context) {
  DrawModelNode(*this, matrix, reversed, clip, commandMode, context, false);
}

// OFFSET: 0x00AB84B0
// FUNCTION: fn_17500
void C3dNodeV2::Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context) {
  DrawModelNode(*this, matrix, reversed, clip, commandMode, context, true);
}
