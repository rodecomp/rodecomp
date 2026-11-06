#include "Contents/Models/ModelActor.h"
#include "Contents/Models/ModelNode.h"
#include "Contents/Models/ModelRendering.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Primitives/RenderVertexPool.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/MapSkyMgr.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Resource/World/WorldResource.h"
#include "Render/Textures/Texture.h"
#include "Render/SpriteGeometry.h"
#include "Client/Mode/ModeMgr.h"

#include <algorithm>
#include <bit>
#include <cstring>

// INLINED
bool ModelTriangleVisible(const RenderVertex* a, const RenderVertex* b, const RenderVertex* c) {
  if (std::min(std::min(a->x, b->x), c->x) >= static_cast<float>(g_renderer->viewport.width)) {
    return false;
  }

  if (0.0f > std::max(std::max(a->x, b->x), c->x)) {
    return false;
  }

  if (std::min(std::min(a->y, b->y), c->y) >= static_cast<float>(g_renderer->viewport.height)) {
    return false;
  }

  if (0.0f > std::max(std::max(a->y, b->y), c->y)) {
    return false;
  }

  return !(0.0f > a->rhw || 0.0f > b->rhw || 0.0f > c->rhw);
}

namespace {
  struct ProjectedVertex : RenderVertex {
    // INLINED
    ProjectedVertex() : RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0} {
    }
  };
}

// INLINED
inline void TransformModelUV(float& u, float& v, const ModelTextureTransform& transform) {
  const float resultU = (ClientCos(transform.values[4]) * (u - 0.5f) - ClientSin(transform.values[4]) * (v - 0.5f)) * transform.values[2] + 0.5f + transform.values[0];
  const float resultV = (ClientCos(transform.values[4]) * (v - 0.5f) + ClientSin(transform.values[4]) * (u - 0.5f)) * transform.values[3] + 0.5f + transform.values[1];

  u = resultU;
  v = resultV;
}

// OFFSET: 0x00AB7D40
// FUNCTION: fn_17499
void C3dNodeV1::Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode) {
  // GLOBAL: gl_24586
  static ProjectedVertex projected[30000];
  struct matrix camera;
  std::memcpy(camera.values, matrix, sizeof(camera.values));
  struct matrix combined;
  combined.Compose(this->transform, camera);
  float ambient[3];
  std::memcpy(ambient, g_worldLighting.ambient, sizeof(ambient));
  float lightColor[3];
  std::memcpy(lightColor, g_weatherEffect.lightColor, sizeof(lightColor));

  const int vertexCount = this->mesh->vertexCount;
  const std::array<float, 3>* meshVertices = this->mesh->vertices;

  if (g_weatherEffect.unknown_0001) {
    g_modeMgr.GetGameMode();
  }

  const unsigned char alpha = this->model->alpha;

  for (int index = 0; index < vertexCount; ++index) {
    g_renderer->viewport.Project(meshVertices[index].data(), combined.values, projected[index]);
  }

  RenderVertex* vertices = g_renderer->vertices.Acquire(static_cast<unsigned>(this->mesh->faceCount) * 3u);
  RenderVertex* output = vertices;
  ModelFace* face = this->mesh->faces;
  ModelFace* const faceEnd = face + this->mesh->faceCount;
  ModelFaceState* state = this->faceState.empty() ? nullptr : &this->faceState[0];

  while (face != faceEnd) {
    const std::uint16_t texture = face->texture;
    std::uint16_t flags = 0;

    while (face != faceEnd && face->texture == texture) {
      const RenderVertex& a = projected[face->vertices[0]];
      const RenderVertex& b = projected[face->vertices[1]];
      const RenderVertex& c = projected[face->vertices[2]];

      if (!clip || ModelTriangleVisible(&a, &b, &c)) {
        const bool winding = (b.y - a.y) * (c.x - a.x) > (c.y - a.y) * (b.x - a.x);

        if ((winding && reversed == 0) || (!winding && reversed != 0) || face->twoSided != 0) {
          output[0] = a;
          output[1] = b;
          output[2] = c;

          for (int corner = 0; corner < 3; ++corner) {
            output[corner].u = state->uv[corner][0];
            output[corner].v = state->uv[corner][1];
          }

          if (g_overrideColor) {
            for (int corner = 0; corner < 3; ++corner) {
              SpriteColor color = std::bit_cast<SpriteColor>(state->colors[corner]);
              color.alpha = alpha;
              color.red = static_cast<std::uint8_t>(color.red * ambient[0]);
              color.green = static_cast<std::uint8_t>(color.green * ambient[1]);
              color.blue = static_cast<std::uint8_t>(color.blue * ambient[2]);
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          } else if (this->unknown_0088) {
            for (int corner = 0; corner < 3; ++corner) {
              SpriteColor color = std::bit_cast<SpriteColor>(state->colors[corner]);
              color.alpha = 0x80;
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          } else if (g_weatherEffect.unknown_0001) {
            const int faceIndex = static_cast<int>(state - (this->faceState.empty() ? nullptr : &this->faceState[0]));

            for (int corner = 0; corner < 3; ++corner) {
              const int* values = &this->faceValues[faceIndex * 9 + corner * 3];
              SpriteColor color;
              color.alpha = alpha;
              color.red = static_cast<std::uint8_t>(values[0] * lightColor[0]);
              color.green = static_cast<std::uint8_t>(values[1] * lightColor[1]);
              color.blue = static_cast<std::uint8_t>(values[2] * lightColor[2]);
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          } else {
            for (int corner = 0; corner < 3; ++corner) {
              SpriteColor color = std::bit_cast<SpriteColor>(state->colors[corner]);
              color.alpha = alpha;
              color.red = static_cast<std::uint8_t>(color.red * lightColor[0]);
              color.green = static_cast<std::uint8_t>(color.green * lightColor[1]);
              color.blue = static_cast<std::uint8_t>(color.blue * lightColor[2]);
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          }

          output += 3;
        }
      }

      ++face;
      ++state;
    }

    const int count = static_cast<int>(output - vertices);

    if (count != 0) {
      RenderCommand* command = g_renderer->commands.Acquire();
      command->vertices = vertices;
      command->vertexCount = count;
      command->texture = this->textures[texture % this->textures.size()];
      command->unknown_0014 = static_cast<std::uint16_t>((command->unknown_0014 & 0x00FFu) | (static_cast<unsigned>(commandMode) << 8));

      if (alpha < 255 || command->texture->Format() == TextureFormat::A4R4G4B4) {
        flags |= 1;
      }

      g_renderer->queues.Submit(command, flags, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      vertices = output;
    }
  }

  this->unknown_0088 = 0;

  for (std::list<C3dNode*>::iterator it = this->children.begin(); it != this->children.end(); ++it) {
    (*it)->Draw(combined.values, reversed, clip, commandMode);
  }
}

// OFFSET: 0x00AB84B0
// FUNCTION: fn_17500
void C3dNodeV2::Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode) {
  // GLOBAL: gl_24587
  static ProjectedVertex projected[30000];
  const struct matrix& camera = *reinterpret_cast<const struct matrix*>(matrix);
  struct matrix combined;
  combined.Compose(this->transform, camera);
  float ambient[3];
  std::memcpy(ambient, g_worldLighting.ambient, sizeof(ambient));
  float lightColor[3];
  std::memcpy(lightColor, g_weatherEffect.lightColor, sizeof(lightColor));

  const int vertexCount = this->mesh->vertexCount;
  const std::array<float, 3>* meshVertices = this->mesh->vertices;

  if (g_weatherEffect.unknown_0001) {
    g_modeMgr.GetGameMode();
  }

  const unsigned char alpha = this->model->alpha;

  for (int index = 0; index < vertexCount; ++index) {
    g_renderer->viewport.Project(meshVertices[index].data(), combined.values, projected[index]);
  }

  RenderVertex* vertices = g_renderer->vertices.Acquire(static_cast<unsigned>(this->mesh->faceCount) * 3u);
  RenderVertex* output = vertices;
  ModelFace* face = this->mesh->faces;
  ModelFace* const faceEnd = face + this->mesh->faceCount;
  ModelFaceState* state = this->faceState.empty() ? nullptr : &this->faceState[0];

  while (face != faceEnd) {
    const std::uint16_t texture = face->texture;
    std::uint16_t flags = 0;

    while (face != faceEnd && face->texture == texture) {
      const RenderVertex& a = projected[face->vertices[0]];
      const RenderVertex& b = projected[face->vertices[1]];
      const RenderVertex& c = projected[face->vertices[2]];

      if (!clip || ModelTriangleVisible(&a, &b, &c)) {
        const bool winding = (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);

        if ((winding == true && reversed == 0) || (winding == false && reversed == 1) || face->twoSided == 1) {
          output[0] = a;
          output[1] = b;
          output[2] = c;

          for (int corner = 0; corner < 3; ++corner) {
            output[corner].u = state->uv[corner][0];
            output[corner].v = state->uv[corner][1];
          }

          for (int corner = 0; corner < 3; ++corner) {
            const auto found = this->textureTransforms.find(face->texture);

            if (found != this->textureTransforms.end()) {
              TransformModelUV(output[corner].u, output[corner].v, found->second);
              flags = 0x20;
            }
          }

          if (g_overrideColor) {
            for (int corner = 0; corner < 3; ++corner) {
              SpriteColor color = std::bit_cast<SpriteColor>(state->colors[corner]);
              color.alpha = alpha;
              color.red = static_cast<std::uint8_t>(color.red * ambient[0]);
              color.green = static_cast<std::uint8_t>(color.green * ambient[1]);
              color.blue = static_cast<std::uint8_t>(color.blue * ambient[2]);
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          } else if (this->unknown_0088) {
            for (int corner = 0; corner < 3; ++corner) {
              SpriteColor color = std::bit_cast<SpriteColor>(state->colors[corner]);
              color.alpha = 0x80;
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          } else if (g_weatherEffect.unknown_0001) {
            const int faceIndex = static_cast<int>(state - (this->faceState.empty() ? nullptr : &this->faceState[0]));

            for (int corner = 0; corner < 3; ++corner) {
              const int* values = &this->faceValues[faceIndex * 9 + corner * 3];
              SpriteColor color;
              color.alpha = alpha;
              color.red = static_cast<std::uint8_t>(values[0] * lightColor[0]);
              color.green = static_cast<std::uint8_t>(values[1] * lightColor[1]);
              color.blue = static_cast<std::uint8_t>(values[2] * lightColor[2]);
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          } else {
            for (int corner = 0; corner < 3; ++corner) {
              SpriteColor color = std::bit_cast<SpriteColor>(state->colors[corner]);
              color.alpha = alpha;
              color.red = static_cast<std::uint8_t>(color.red * lightColor[0]);
              color.green = static_cast<std::uint8_t>(color.green * lightColor[1]);
              color.blue = static_cast<std::uint8_t>(color.blue * lightColor[2]);
              output[corner].diffuse = std::bit_cast<std::uint32_t>(color);
            }
          }

          output += 3;
        }
      }

      ++face;
      ++state;
    }

    const int count = static_cast<int>(output - vertices);

    if (count != 0) {
      RenderCommand* command = g_renderer->commands.Acquire();
      command->vertices = vertices;
      command->vertexCount = count;
      command->texture = this->textures[texture % this->textures.size()];
      command->unknown_0014 = static_cast<std::uint16_t>((command->unknown_0014 & 0x00FFu) | (static_cast<unsigned>(commandMode) << 8));

      if (alpha < 255 || command->texture->Format() == TextureFormat::A4R4G4B4) {
        flags |= 1;
      }

      g_renderer->queues.Submit(command, flags, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      vertices = output;
    }
  }

  this->unknown_0088 = 0;

  for (std::list<C3dNode*>::iterator it = this->children.begin(); it != this->children.end(); ++it) {
    (*it)->Draw(combined.values, reversed, clip, commandMode);
  }
}
