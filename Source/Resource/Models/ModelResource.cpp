#include "Client/Application/ErrorDialog.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Models/AnimatedModelResource.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00756D60
// FUNCTION: fn_07997
AnimatedModelRecord::~AnimatedModelRecord() {
  delete[] this->unknown_0084;
}

// OFFSET: 0x00755630
// FUNCTION: fn_07985
ModelNode::ModelNode() : model(nullptr), parent(nullptr), alpha(255) {
}

// OFFSET: 0x007568E0
// FUNCTION: fn_07996
ModelNode::~ModelNode() {
  for (CTexture* texture : this->textures) {
    if (texture) {
      texture->Release();
      this->model->m_context->textures.Remove(texture);
    }
  }

  this->textures.clear();
  this->mesh.reset();
  this->children.clear();
}

// INLINED
ModelNodeV1::ModelNodeV1() = default;

// INLINED
ModelNodeV1::~ModelNodeV1() = default;

// INLINED
ModelNodeV2::ModelNodeV2() = default;

// INLINED
ModelNodeV2::~ModelNodeV2() = default;

// OFFSET: 0x007553E0
// FUNCTION: fn_07982
C3dModelRes::C3dModelRes(ModelResourceContext& context) : major(0), shade(0), alpha(255), m_context(&context) {
}

// OFFSET: 0x007567B0
// FUNCTION: fn_07995
C3dModelRes::~C3dModelRes() = default;

// OFFSET: 0x00755560
// FUNCTION: fn_07983
C3dModelResV1::C3dModelResV1(ModelResourceContext& context) : C3dModelRes(context), animationLength(0) {
}

// INLINED
C3dModelResV1::~C3dModelResV1() = default;

// OFFSET: 0x00750A70
// FUNCTION: fn_07949
C3dModelResV1* C3dModelResV1::Create() {
  return new C3dModelResV1(*this->m_context);
}

// OFFSET: 0x007593C0
// FUNCTION: fn_08012
ModelNode* C3dModelRes::FindNode(const char* name) const {
  for (ModelNode* node : this->nodes) {
    if (node->name == name) {
      return node;
    }
  }

  return nullptr;
}

// OFFSET: 0x00750950
// FUNCTION: fn_07948
void C3dModelRes::Unload() {
  for (ModelNode* node : this->nodes) {
    delete node;
  }

  this->nodes.clear();
  this->rootNames.clear();
  this->meshes.clear();

  for (ModelVolume* volume : this->volumes) {
    delete volume;
  }

  this->volumes.clear();
  this->movieNames.clear();
}

// INLINED
inline void C3dModelResV1::ReadNode(ResourceByteReader& input, unsigned minor, const std::vector<std::string>& textureNames) {
  auto node = std::make_unique<ModelNodeV1>();
  node->model = this;
  node->alpha = this->alpha;
  char field[40]{};
  input.Read(field, sizeof(field));
  node->name = field;
  input.Read(field, sizeof(field));
  node->parentName = field;
  int textureCount = 0;
  input.Read(&textureCount, 4);

  for (int index = 0; index < textureCount; ++index) {
    int textureIndex = 0;
    input.Read(&textureIndex, 4);
    CTexture* texture = this->m_context->textures.Load(textureNames[textureIndex].c_str(), 0, false, true, false);
    node->textures.push_back(texture);

    if (texture) {
      texture->Retain();
    }
  }

  float matrix[12];
  input.Read(matrix, sizeof(matrix));
  input.Read(node->position, sizeof(node->position));
  input.Read(&node->angle, 4);
  input.Read(node->axis, sizeof(node->axis));
  input.Read(node->scale, sizeof(node->scale));
  node->mesh = std::make_unique<ModelMesh>();
  ModelMesh& mesh = *node->mesh;
  input.Read(&mesh.vertexCount, 4);
  mesh.vertices = new std::array<float, 3>[mesh.vertexCount]{};
  input.Read(mesh.vertices, static_cast<unsigned>(mesh.vertexCount) * 12u);

  for (int index = 0; index < mesh.vertexCount; ++index) {
    auto& vertex = mesh.vertices[index];
    const float y = ((vertex[0] * matrix[1] + vertex[1] * matrix[4]) + matrix[7] * vertex[2]) + matrix[10];
    const float z = ((vertex[0] * matrix[2] + vertex[1] * matrix[5]) + matrix[8] * vertex[2]) + matrix[11];
    vertex[0] = ((vertex[0] * matrix[0] + vertex[1] * matrix[3]) + vertex[2] * matrix[6]) + matrix[9];
    vertex[1] = y;
    vertex[2] = z;
  }

  input.Read(&mesh.textureVertexCount, 4);
  mesh.textureVertices = new ModelTextureVertex[mesh.textureVertexCount];

  if (minor == 1) {
    float uv[2];

    for (int index = 0; index < mesh.textureVertexCount; ++index) {
      auto& vertex = mesh.textureVertices[index];
      input.Read(uv, sizeof(uv));
      vertex = {0xFFFFFFFFu, uv[0], uv[1]};
    }
  } else {
    input.Read(mesh.textureVertices, static_cast<unsigned>(mesh.textureVertexCount) * 12u);
  }

  input.Read(&mesh.faceCount, 4);
  mesh.faces = new ModelFace[mesh.faceCount];
  unsigned char record[24];

  for (int index = 0; index < mesh.faceCount; ++index) {
    ModelFace& face = mesh.faces[index];
    input.Read(record, minor == 1 ? 20 : 24);
    std::memcpy(face.vertices, record, sizeof(face.vertices));
    std::memcpy(face.textureVertices, record + 6, sizeof(face.textureVertices));
    std::memcpy(&face.texture, record + 12, sizeof(face.texture));
    std::memcpy(&face.twoSided, record + 16, sizeof(face.twoSided));
    std::uint32_t group = 0;

    if (minor != 1) {
      std::memcpy(&group, record + 20, sizeof(group));
    }

    face.smoothing.push_back(group);
  }

  // INLINED
  std::sort(mesh.faces, mesh.faces + mesh.faceCount, [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
  mesh.CalculateNormals();
  int rotationCount;
  input.Read(&rotationCount, 4);

  for (int index = 0; index < rotationCount; ++index) {
    ModelRotationKey key;
    input.Read(&key, sizeof(key));
    node->rotationKeys.push_back(key);
  }

  this->meshes[node->name] = node->mesh.get();
  this->nodes.push_back(node.release());
}

// OFFSET: 0x00750B10
// FUNCTION: fn_07950
bool C3dModelResV1::Load(const char* name) {
  auto& file = this->m_context->file;

  if (!file.Open(name)) {
    file.Close();
    return false;
  }

  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  char magic[5]{};
  input.Read(magic, 4);

  if (std::strcmp(magic, "GRSM") != 0) {
    constexpr char message[] = "모델파일이 아닙니다. 형식을 다시한번 확인해주세요.";
    ShowErrorDialog(this->m_context->renderSystem, message, 0);
    file.Close();

    return false;
  }

  unsigned char major = 0;
  unsigned char minor = 0;
  input.Read(&major, 1);
  this->major = major;
  input.Read(&minor, 1);

  if (major != 1 || minor > 5) {
    constexpr char message[] = "지원되지 않는 버젼입니다. 내용물이 깨져있을 수 있습니다";
    ShowErrorDialog(this->m_context->renderSystem, message, 0);
    file.Close();

    return false;
  }

  input.Read(&this->animationLength, 4);
  input.Read(&this->shade, 4);

  if (minor < 4) {
    this->alpha = 255;
  } else {
    input.Read(&this->alpha, 1);
  }

  unsigned char reserved[16]{};
  input.Read(reserved, sizeof(reserved));
  int textureCount = 0;
  input.Read(&textureCount, 4);
  std::vector<std::string> textureNames;
  char textureName[40]{};

  for (int index = 0; index < textureCount; ++index) {
    input.Read(textureName, sizeof(textureName));
    textureNames.emplace_back(textureName);

    if (std::strstr(textureName, ".bik")) {
      this->movieNames.emplace_back(textureName);
    }
  }

  char rootName[40]{};
  input.Read(rootName, sizeof(rootName));
  int nodeCount = 0;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    this->ReadNode(input, minor, textureNames);
  }

  for (ModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      node->parent = this->FindNode(node->parentName.c_str());
      node->parent->children.push_back(node);
    }
  }

  this->rootNames.emplace_back(rootName);
  int volumeCount = 0;
  input.Read(&volumeCount, 4);

  if (minor >= 2) {
    for (int index = 0; index < volumeCount; ++index) {
      auto* volume = new ModelVolume;
      std::memset(volume, 0, 132);
      volume->flag = 0;
      unsigned char record[40]{};
      input.Read(record, minor == 2 ? 36 : 40);
      std::memcpy(volume, record, 36);

      if (minor > 2) {
        std::memcpy(&volume->flag, record + 36, 4);
      }

      if (minor > 2 && volume->flag == 0) {
        volume->size[0] = volume->size[0] + 3.0f;
        volume->size[2] = volume->size[2] + 3.0f;
      }

      this->volumes.push_back(volume);
    }
  }

  this->sourceName = name;
  file.Close();
  file.Close();

  return true;
}

// OFFSET: 0x007577E0
// FUNCTION: fn_08006
void C3dGrannyModelRes::RebindMeshes(const C3dGrannyModelRes& target) {
  for (int index = 0; index < this->instance->meshCount; ++index) {
    GrannyMeshEntry& entry = this->instance->meshes[index];
    GrannyFreeMeshBinding(entry.binding);
    entry.binding = GrannyNewMeshBinding(entry.mesh, this->skeleton, target.skeleton);
  }
}

// OFFSET: 0x00758E40
// FUNCTION: fn_08009
void C3dGrannyModelRes::InstantiateModel(GrannyModelInstance* target, const granny_model* model) {
  const int boneCount = model->skeleton->boneCount;
  if (this->maxBoneCount < boneCount) {
    this->maxBoneCount = boneCount;
  }

  target->instance = GrannyInstantiateModel(model);
  target->worldPose = GrannyNewWorldPose(boneCount);
  target->meshCount = model->meshBindingCount;
  target->meshes = new GrannyMeshEntry[target->meshCount];

  for (int index = 0; index < target->meshCount; ++index) {
    GrannyMeshEntry& entry = target->meshes[index];
    granny_mesh* mesh = model->meshBindings[index].mesh;
    entry.mesh = mesh;
    granny_skeleton* skeleton = GrannyGetSourceSkeleton(target->instance);
    entry.binding = GrannyNewMeshBinding(mesh, skeleton, skeleton);

    const unsigned int indexCount = GrannyGetMeshIndexCount(mesh);
    entry.indices = new std::uint16_t[indexCount];
    if (indexCount != 0) {
      GrannyCopyMeshIndices(mesh, 2, entry.indices);
    }

    const int vertexBytes = GrannyGetMeshVertexCount(mesh) * 32;
    entry.vertices = nullptr;
    if (!GrannyMeshIsRigid(mesh)) {
      if (this->deformBufferSize < vertexBytes) {
        this->deformBufferSize = vertexBytes;
      }

      entry.deformer = GrannyNewMeshDeformer(GrannyGetMeshVertexType(mesh), GrannyPNT332VertexType, 2);
      if (!entry.deformer) {
        ShowErrorDialog(this->m_context->renderSystem, "Granny didn't find a matching deformer for the vertex format used by mesh", 0);
      }
    } else {
      entry.vertices = new unsigned char[vertexBytes];
      GrannyCopyMeshVertices(mesh, GrannyPNT332VertexType, entry.vertices);
      entry.deformer = nullptr;
    }

    entry.materialCount = mesh->materialBindingCount;
    entry.textures = new GrannyTextureEntry*[entry.materialCount];

    for (int material = 0; material < entry.materialCount; ++material) {
      GrannyTextureEntry* found = nullptr;
      const granny_texture* texture = GrannyGetMaterialTextureByType(mesh->materialBindings[material].material, 2);
      if (texture) {
        for (int textureIndex = 0; textureIndex < this->textureCount; ++textureIndex) {
          if (std::strcmp(this->textures[textureIndex].name, texture->fromFileName) == 0) {
            found = &this->textures[textureIndex];
            break;
          }
        }
      }

      entry.textures[material] = found;
    }
  }
}

// OFFSET: 0x00759090
// FUNCTION: fn_08010
CTexture* C3dGrannyModelRes::CreateTexture(const granny_texture* texture, int) {
  const bool hasAlpha = GrannyTextureHasAlpha(texture);
  const int width = texture->width;
  const int height = texture->height;
  unsigned char* source = new unsigned char[height * width * 4];
  unsigned char* pixels = new unsigned char[height * width * 4];
  GrannyCopyTextureImage(texture, 0, 0, &GrannyRGBA8888PixelFormat, width, height, width * 4, source);

  int format;
  if (!hasAlpha) {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        const int offset = (y * width + x) * 4;
        pixels[offset + 0] = source[offset + 2];
        pixels[offset + 1] = source[offset + 1];
        pixels[offset + 2] = source[offset + 0];
        pixels[offset + 3] = 0xFF;
      }
    }

    format = 0;
  } else {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        const int offset = (y * width + x) * 4;
        pixels[offset + 0] = source[offset + 2];
        pixels[offset + 1] = source[offset + 1];
        pixels[offset + 2] = source[offset + 0];
        pixels[offset + 3] = source[offset + 3];
      }
    }

    format = 1;
  }

  CTexture* result = this->m_context->textures.Upload(width, height, reinterpret_cast<const std::uint32_t*>(pixels), format, 0, false, false, false);
  delete[] pixels;
  delete[] source;
  return result;
}

// OFFSET: 0x00759330
// FUNCTION: fn_08011
void C3dGrannyModelRes::FreeControl(granny_control* control) {
  if (control) {
    GrannySetControlActive(control, false);
    GrannyFreeControl(control);
  }
}
