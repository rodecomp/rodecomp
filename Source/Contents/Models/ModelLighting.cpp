#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelNode.h"
#include "Render/Textures/Texture.h"

#include "Utility/Math/ScalarConversion.h"
#include <cmath>
#include <cstring>

namespace {
  // INLINED
  unsigned Byte(float value) {
    return static_cast<unsigned>(TruncateToInt(value)) & 255u;
  }

  // INLINED
  float InverseLength(float squared) {
    const float root = std::sqrt(squared);
    return static_cast<float>(1.0 / static_cast<double>(root));
  }

  // INLINED
  void Light(const float* normal, const float* matrix, const ModelLightingContext& context, float* output) {
    const float x = ((normal[0] * matrix[0] + normal[1] * matrix[3]) + normal[2] * matrix[6]) + 0.0f;
    const float y = ((normal[0] * matrix[1] + normal[1] * matrix[4]) + normal[2] * matrix[7]) + 0.0f;
    const float z = ((normal[0] * matrix[2] + normal[1] * matrix[5]) + normal[2] * matrix[8]) + 0.0f;
    const float inverse = InverseLength((y * y + x * x) + z * z);
    const float dot = ((y * inverse) * context.direction[1] + (x * inverse) * context.direction[0]) + (z * inverse) * context.direction[2];
    const float amount = dot > 0.0f ? dot : 0.0f;

    for (unsigned component = 0; component < 3; ++component) {
      const float color = amount * context.diffuse[component] + context.ambient[component];
      output[component] = color < 1.0f ? color : 1.0f;
    }
  }

  // INLINED
  std::uint32_t LitColor(std::uint32_t color, float alpha, bool shaded, const float* light) {
    std::uint32_t result = Byte(alpha * 255.0f) << 24;
    const float factor = shaded ? 0.5f : 1.0f;

    for (unsigned component = 0; component < 3; ++component) {
      const unsigned shift = 16u - component * 8u;
      const float value = (factor * light[component]) * static_cast<float>((color >> shift) & 255u);
      const float limited = 255.0f < value ? 255.0f : value;
      result |= Byte(limited) << shift;
    }

    return result;
  }
}

// OFFSET: 0x00AB78B0
// FUNCTION: fn_17491
C3dNode* C3dNode::Find(const char* selected) {
  if (this->name == selected) {
    return this;
  }

  for (C3dNode* child : this->children) {
    if (C3dNode* found = child->Find(selected)) {
      return found;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AB94A0
// FUNCTION: fn_17509
void C3dNode::OverrideColors() {
  this->unknown_0088 = 1;

  for (C3dNode* child : this->children) {
    child->OverrideColors();
  }
}

// OFFSET: 0x00ABB450
// FUNCTION: fn_17515
void C3dNode::SampleGroundColors(const float* matrix, const float* position, const GroundGeometry& ground) {
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
void C3dNode::Prepare(const float* matrix, ModelLightingContext& context) {
  AffineMatrix parentMatrix;
  std::memcpy(parentMatrix.values, matrix, sizeof(parentMatrix.values));
  AffineMatrix combined;
  combined.Compose(this->transform, parentMatrix);

  for (int index = 0; index < this->mesh->faceCount; ++index) {
    const ModelFace& face = this->mesh->faces[index];
    float light[3];

    if (*context.shade == 1) {
      Light(this->mesh->faceNormals[index].data(), combined.values, context, light);
    }

    for (unsigned corner = 0; corner < 3; ++corner) {
      std::uint32_t& color = context.colors[index * 3u + corner];

      if (*context.shade == 0) {
        color = (*context.color & 0x00FFFFFFu) | (Byte(this->alpha * 255.0f) << 24);
      } else if (*context.shade == 1 || *context.shade == 2) {
        if (*context.shade == 2) {
          Light(this->mesh->vertexNormals[index * 3u + corner].data(), combined.values, context, light);
        }

        const bool shaded = this->mesh->textureVertices[face.textureVertices[corner]].shaded != 0;
        color = LitColor(*context.color, this->alpha, shaded, light);
      }
    }
  }

  int index = 0;

  while (index < this->mesh->faceCount) {
    const unsigned texture = this->mesh->faces[index].texture;
    CTexture* image = this->textures[texture % this->textures.size()];

    const float u = static_cast<float>(static_cast<double>(image->CTexture::Width())) / static_cast<float>(static_cast<double>(image->TextureWidth()));
    const float v = static_cast<float>(static_cast<double>(image->CTexture::Height())) / static_cast<float>(static_cast<double>(image->TextureHeight()));

    do {
      const ModelFace& face = this->mesh->faces[index];
      ModelFaceState& state = this->faceState[index];

      for (unsigned corner = 0; corner < 3; ++corner) {
        state.colors[corner] = context.colors[index * 3u + corner];
      }

      for (unsigned corner = 0; corner < 3; ++corner) {
        if (this->mesh->textureVertexCount == 0) {
          state.uv[corner][0] = u;
          state.uv[corner][1] = v;
        } else {
          const auto& uv = this->mesh->textureVertices[face.textureVertices[corner]];
          state.uv[corner][1] = v * uv.v;
          state.uv[corner][0] = u * uv.u;
        }
      }

      ++index;
    } while (index < this->mesh->faceCount && this->mesh->faces[index].texture == texture);
  }

  this->SampleGroundColors(combined.values, context.position, context.ground);

  for (C3dNode* child : this->children) {
    child->Prepare(combined.values, context);
  }
}
