#include "Client/Application/ErrorDialog.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <cstring>

// OFFSET: 0x00751ED0
// FUNCTION: fn_07953
C3dModelResV2::C3dModelResV2(ModelResourceContext& context) : C3dModelRes(context), framesPerSecond(0.0f), animationLength(0) {
}

// OFFSET: 0x00752190
// FUNCTION: fn_07955
C3dModelResV2::~C3dModelResV2() = default;

// OFFSET: 0x00751ED0
// FUNCTION: fn_07953
C3dModelResV2* C3dModelResV2::Create() {
  return new C3dModelResV2(*this->m_context);
}

// OFFSET: 0x00751F70
// FUNCTION: fn_07954
bool C3dModelResV2::Load(const char* name) {
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

  if (minor >= 1 && minor <= 3) {
    this->ReadVersion(input, minor);
  }

  file.Close();
  this->sourceName = name;
  file.Close();

  return true;
}

// OFFSET: 0x00759ED0
// FUNCTION: fn_08020
// OFFSET: 0x0075AB20
// FUNCTION: fn_08021
// OFFSET: 0x0075B8E0
// FUNCTION: fn_08022
void C3dModelResV2::ReadVersion(ResourceByteReader& input, unsigned minor) {
  input.Read(&this->animationLength, 4);
  input.Read(&this->shade, 4);
  input.Read(&this->alpha, 1);
  input.Read(&this->framesPerSecond, 4);
  std::vector<std::string> textureNames;
  int textureCount = 0;
  input.Read(&textureCount, 4);

  if (minor < 3) {
    for (int index = 0; index < textureCount; ++index) {
      unsigned bytes = 0;
      input.Read(&bytes, 4);
      char name[40]{};
      input.Read(name, bytes);
      textureNames.emplace_back(name);
    }
  } else {
    for (int index = 0; index < textureCount; ++index) {
      unsigned bytes = 0;
      input.Read(&bytes, 4);
      char name[40]{};
      input.Read(name, bytes);
      this->rootNames.emplace_back(name);
    }
  }

  if (minor < 3) {
    int rootCount = 0;
    input.Read(&rootCount, 4);

    for (int index = 0; index < rootCount; ++index) {
      unsigned bytes = 0;
      input.Read(&bytes, 4);
      char name[40]{};
      input.Read(name, bytes);
      this->rootNames.emplace_back(name);
    }
  }

  int nodeCount = 0;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    this->ReadNode(input, minor, textureNames);
  }

  for (ModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      ModelNode* parent = this->FindNode(node->parentName.c_str());

      if (parent) {
        node->parent = parent;
        parent->children.push_back(node);
      }
    }
  }

  int volumeCount = 0;
  input.Read(&volumeCount, 4);

  for (int index = 0; index < volumeCount; ++index) {
    auto* volume = new ModelVolume;
    std::memset(volume, 0, 132);
    input.Read(volume, sizeof(ModelVolume));
    this->volumes.push_back(volume);
  }
}
