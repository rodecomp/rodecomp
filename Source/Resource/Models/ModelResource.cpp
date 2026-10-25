#include "Client/Application/ErrorDialog.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <cstring>

// OFFSET: 0x00755630
// FUNCTION: fn_07985
// OFFSET: 0x00750B10
// FUNCTION: fn_07950
ModelNode::ModelNode(TextureCache& textures) : model(nullptr), parent(nullptr), alpha(255), m_textures(textures) {
}

// OFFSET: 0x007570F0
// FUNCTION: fn_08002
ModelNode::~ModelNode() {
  for (CTexture* texture : this->textures) {
    if (texture) {
      texture->Release();
      this->m_textures.Remove(texture);
    }
  }
}

// OFFSET: 0x007553E0
// FUNCTION: fn_07982
C3dModelRes::C3dModelRes(ModelResourceContext& context) : major(0), shade(0), alpha(255), m_context(&context) {
}

// OFFSET: 0x00757090
// FUNCTION: fn_08000
C3dModelRes::~C3dModelRes() = default;

// OFFSET: 0x00750A70
// FUNCTION: fn_07949
C3dModelResV1::C3dModelResV1(ModelResourceContext& context) : C3dModelRes(context), animationLength(0) {
}

// OFFSET: 0x00751E70
// FUNCTION: fn_07952
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
