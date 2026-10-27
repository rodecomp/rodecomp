#include "Client/Application/ErrorDialog.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <algorithm>
#include <cstring>

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
  int vertexCount = 0;
  input.Read(&vertexCount, 4);
  mesh.vertices.resize(static_cast<unsigned>(vertexCount));
  input.Read(mesh.vertices.data(), static_cast<unsigned>(vertexCount) * 12u);

  for (auto& vertex : mesh.vertices) {
    const float y = ((vertex[0] * matrix[1] + vertex[1] * matrix[4]) + matrix[7] * vertex[2]) + matrix[10];
    const float z = ((vertex[0] * matrix[2] + vertex[1] * matrix[5]) + matrix[8] * vertex[2]) + matrix[11];
    vertex[0] = ((vertex[0] * matrix[0] + vertex[1] * matrix[3]) + vertex[2] * matrix[6]) + matrix[9];
    vertex[1] = y;
    vertex[2] = z;
  }

  int textureVertexCount = 0;
  input.Read(&textureVertexCount, 4);
  mesh.textureVertices.resize(static_cast<unsigned>(textureVertexCount));

  if (minor == 1) {
    float uv[2];

    for (auto& vertex : mesh.textureVertices) {
      input.Read(uv, sizeof(uv));
      vertex = {0xFFFFFFFFu, uv[0], uv[1]};
    }
  } else {
    input.Read(mesh.textureVertices.data(), static_cast<unsigned>(textureVertexCount) * 12u);
  }

  std::uint32_t faceCount = 0;
  input.Read(&faceCount, 4);
  mesh.faces.resize(faceCount);
  unsigned char record[24];

  for (ModelFace& face : mesh.faces) {
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
  std::sort(mesh.faces.begin(), mesh.faces.end(), [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
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
    constexpr char message[] = "\xB8\xF0\xB5\xA8\xC6\xC4\xC0\xCF\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9. \xC7\xFC\xBD\xC4\xC0\xBB "
                               "\xB4\xD9\xBD\xC3\xC7\xD1\xB9\xF8 \xC8\xAE\xC0\xCE\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.";
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
    constexpr char message[] = "\xC1\xF6\xBF\xF8\xB5\xC7\xC1\xF6 \xBE\xCA\xB4\xC2 \xB9\xF6\xC1\xAF\xC0\xD4\xB4\xCF\xB4\xD9. "
                               "\xB3\xBB\xBF\xEB\xB9\xB0\xC0\xCC \xB1\xFA\xC1\xAE\xC0\xD6\xC0\xBB \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9";
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
