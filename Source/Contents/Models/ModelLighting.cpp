#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/Models/ModelNode.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/Textures/Texture.h"

#include "Utility/Math/ScalarConversion.h"
#include <cmath>
#include <cstring>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

union ModelColor {
  std::uint32_t value;
  struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
  };
};

// GLOBAL: gl_25474
static ModelColor g_modelFaceColors[90000];

namespace {
  // INLINED
  float InverseLength(float squared) {
    return 1.0f / SquareRoot(squared);
  }

  // INLINED
  __forceinline void Light(const float* normal, const float* values, const float* direction, const float* diffuse, const float* ambient, float* output) {
    const float x = ((normal[0] * values[0] + normal[1] * values[3]) + normal[2] * values[6]) + 0.0f;
    const float y = ((normal[0] * values[1] + normal[1] * values[4]) + normal[2] * values[7]) + 0.0f;
    const float z = ((normal[0] * values[2] + normal[1] * values[5]) + normal[2] * values[8]) + 0.0f;
    const float inverse = InverseLength((y * y + x * x) + z * z);
    const float dot = ((y * inverse) * direction[1] + (x * inverse) * direction[0]) + (z * inverse) * direction[2];
    const float amount = dot > 0.0f ? dot : 0.0f;

    for (unsigned component = 0; component < 3; ++component) {
      const float color = amount * diffuse[component] + ambient[component];
      output[component] = color < 1.0f ? color : 1.0f;
    }
  }

  // INLINED
  unsigned char Scale(float factor, float light, unsigned char channel) {
    const float value = (factor * light) * static_cast<float>(channel);
    return static_cast<unsigned char>(255.0f < value ? 255.0f : value);
  }

  // INLINED
  ModelColor LitColor(std::uint32_t source, float alpha, bool shaded, const float* light) {
    const float factor = shaded ? 0.5f : 1.0f;
    ModelColor color;
    color.value = source;
    color.a = static_cast<unsigned char>(alpha * 255.0f);
    color.r = Scale(factor, light[0], color.r);
    color.g = Scale(factor, light[1], color.g);
    color.b = Scale(factor, light[2], color.b);
    return color;
  }

  // INLINED
  __forceinline void GroundColor(const C3dNode* node, const GroundGeometry& ground, const float* matrix, const std::array<float, 3>& vertex, std::uint32_t* output) {
    const float x = (((vertex[0] * matrix[0] + vertex[1] * matrix[3]) + vertex[2] * matrix[6]) + matrix[9]) + node->model->position[0];
    const float y = (((vertex[0] * matrix[1] + vertex[1] * matrix[4]) + vertex[2] * matrix[7]) + matrix[10]) + node->model->position[1];
    const float z = (((vertex[0] * matrix[2] + vertex[1] * matrix[5]) + vertex[2] * matrix[8]) + matrix[11]) + node->model->position[2];
    ground.WorldColor(x, y, z, output);
  }

  // INLINED
  int AddChannel(std::uint32_t sampled, std::uint32_t channel) {
    const int value = static_cast<int>(sampled) + channel;
    return value > 255 ? 255 : value;
  }
}

// OFFSET: 0x00AB78B0
// FUNCTION: fn_17491
C3dNode* C3dNode::Find(const char* selected) {
  if (this->name.compare(selected) == 0) {
    return this;
  }

  for (std::list<C3dNode*>::iterator it = this->children.begin(); it != this->children.end(); ++it) {
    if (C3dNode* found = (*it)->Find(selected)) {
      return found;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AB94A0
// FUNCTION: fn_17509
void C3dNode::OverrideColors() {
  this->overrideColors = 1;

  for (std::list<C3dNode*>::iterator it = this->children.begin(); it != this->children.end(); ++it) {
    (*it)->OverrideColors();
  }
}

// OFFSET: 0x00ABB450
// FUNCTION: fn_17515
void C3dNode::SampleGroundColors(const CRenderer*, const float* matrix, const GroundGeometry& ground) {
  const float* position = this->model->position;

  for (int index = 0; index < this->mesh->faceCount; ++index) {
    const ModelFace& face = this->mesh->faces[index];

    for (unsigned corner = 0; corner < 3; ++corner) {
      const auto& vertex = this->mesh->vertices[face.vertices[corner]];
      const float x = (((vertex[0] * matrix[0] + vertex[1] * matrix[3]) + vertex[2] * matrix[6]) + matrix[9]) + position[0];
      const float y = (((vertex[0] * matrix[1] + vertex[1] * matrix[4]) + vertex[2] * matrix[7]) + matrix[10]) + position[1];
      const float z = (((vertex[0] * matrix[2] + vertex[1] * matrix[5]) + vertex[2] * matrix[8]) + matrix[11]) + position[2];
      std::uint32_t sampled[3];
      ground.WorldColor(x, y, z, sampled);

      for (unsigned component = 0; component < 3; ++component) {
        const unsigned shift = 16u - component * 8u;
        const unsigned base = (this->faceState[index].colors[corner] >> shift) & 255u;
        const int value = static_cast<int>(sampled[component]) + base;
        this->faceValues[index * 9u + corner * 3u + component] = value > 255 ? 255 : value;
      }
    }
  }
}

// OFFSET: 0x00ABBB00
// FUNCTION: fn_17521
void C3dNode::Prepare(const float* matrix, const float* direction, const float* diffuse, const float* ambient, int shade) {
  struct matrix combined;
  combined.Compose(this->transform, *reinterpret_cast<const struct matrix*>(matrix));
  const struct matrix rotation = combined;

  switch (shade) {
  case 2: {
    const ModelFace* faces = this->mesh->faces;
    const ModelTextureVertex* textureVertices = this->mesh->textureVertices;
    const std::array<float, 3>* normals = this->mesh->vertexNormals;
    ModelColor* color = g_modelFaceColors;

    for (int index = 0; index < this->mesh->faceCount; ++index) {
      for (unsigned corner = 0; corner < 3; ++corner) {
        float light[3];
        Light(normals[index * 3 + corner].data(), rotation.values, direction, diffuse, ambient, light);
        const bool shaded = textureVertices[faces[index].textureVertices[corner]].shaded > 0u;
        *color++ = LitColor(this->model->color, this->alpha, shaded, light);
      }
    }
    break;
  }
  case 1: {
    const ModelFace* faces = this->mesh->faces;
    const ModelTextureVertex* textureVertices = this->mesh->textureVertices;
    const std::array<float, 3>* normals = this->mesh->faceNormals;
    ModelColor* color = g_modelFaceColors;

    for (int index = 0; index < this->mesh->faceCount; ++index) {
      float light[3];
      Light(normals[index].data(), rotation.values, direction, diffuse, ambient, light);

      for (unsigned corner = 0; corner < 3; ++corner) {
        const bool shaded = textureVertices[faces[index].textureVertices[corner]].shaded > 0u;
        *color++ = LitColor(this->model->color, this->alpha, shaded, light);
      }
    }
    break;
  }
  case 0: {
    ModelColor* color = g_modelFaceColors;

    for (int index = 0; index < this->mesh->faceCount; ++index) {
      for (unsigned corner = 0; corner < 3; ++corner) {
        color->value = this->model->color;
        color->a = static_cast<unsigned char>(this->alpha * 255.0f);
        ++color;
      }
    }
    break;
  }
  }

  const ModelFace* face = this->mesh->faces;
  const ModelFace* const end = face + this->mesh->faceCount;
  const ModelTextureVertex* textureVertices = this->mesh->textureVertices;
  ModelFaceState* state = this->faceState.data();
  const ModelColor* colors = g_modelFaceColors;

  while (face != end) {
    const std::uint16_t texture = face->texture;
    CTexture* image = this->textures[texture % this->textures.size()];

    const float u = static_cast<float>(static_cast<double>(image->ImageWidth())) / static_cast<float>(static_cast<double>(image->TextureWidth()));
    const float v = static_cast<float>(static_cast<double>(image->ImageHeight())) / static_cast<float>(static_cast<double>(image->TextureHeight()));

    while (face != end && face->texture == texture) {
      for (unsigned corner = 0; corner < 3; ++corner) {
        state->colors[corner] = colors[corner].value;
      }

      if (this->mesh->textureVertexCount == 0) {
        for (unsigned corner = 0; corner < 3; ++corner) {
          state->uv[corner][0] = u;
          state->uv[corner][1] = v;
        }
      } else {
        for (unsigned corner = 0; corner < 3; ++corner) {
          const ModelTextureVertex& uv = textureVertices[face->textureVertices[corner]];
          state->uv[corner][1] = v * uv.v;
          state->uv[corner][0] = u * uv.u;
        }
      }

      ++face;
      ++state;
      colors += 3;
    }
  }

  this->SampleGroundColors(g_renderer, combined.values, *static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->ground);

  for (std::list<C3dNode*>::iterator it = this->children.begin(); it != this->children.end(); ++it) {
    (*it)->Prepare(combined.values, direction, diffuse, ambient, shade);
  }
}
