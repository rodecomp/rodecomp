#include "Client/Application/ErrorDialog.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/World/WorldResource.h"

#include <cstdio>
#include <cstring>

// STATIC: 0x016FDA64
// GLOBAL: gl_24387

WorldLighting g_worldLighting{};

namespace {
  // OFFSET: 0x00763010
  // FUNCTION: fn_08075
  signed char CompareWorldVersion(const int targetVersion[2], const unsigned char* const fileVersion[2]) {
    const int targetMajor = targetVersion[1];
    const int fileMajor = static_cast<int>(*fileVersion[1]);

    if (targetMajor != fileMajor) {
      return targetMajor < fileMajor ? -1 : 1;
    }

    const int targetMinor = targetVersion[0];
    const int fileMinor = static_cast<int>(*fileVersion[0]);

    if (targetMinor != fileMinor) {
      return targetMinor < fileMinor ? -1 : 1;
    }

    return 0;
  }
}

// OFFSET: 0x00763060
C3dWorldRes::C3dWorldRes(WorldResourceContext& context) : spatial(nullptr), lightDirection{}, diffuse{}, ambient{}, flag(0), m_context(&context) {
}

// OFFSET: 0x00763550
// FUNCTION: fn_08077
// OFFSET: 0x00762FE0
// FUNCTION: fn_08074
C3dWorldRes::~C3dWorldRes() = default;

// OFFSET: 0x00761940
// FUNCTION: fn_08071
C3dWorldRes* C3dWorldRes::Create() {
  return new C3dWorldRes(*this->m_context);
}

// OFFSET: 0x00761B40
// FUNCTION: fn_08072
void C3dWorldRes::Unload() {
  for (WorldModelRecord* model : this->models) {
    delete model;
  }

  this->models.clear();

  for (WorldEffectRecord* effect : this->effects) {
    delete effect;
  }

  this->effects.clear();

  for (WorldSoundRecord* sound : this->sounds) {
    delete sound;
  }

  delete this->spatial;
  this->spatial = nullptr;
  this->sounds.clear();
}

// OFFSET: 0x00761CC0
// FUNCTION: fn_08073
bool C3dWorldRes::Load(const char* name) {
  this->Unload();
  auto& file = this->m_context->file;
  struct CloseFile {
    ResourceBytes& file;

    ~CloseFile() {
      file.Close();
    }
  } close{file};

  if (!file.Open(name)) {
    return false;
  }

  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  char magic[5];
  input.Read(magic, 4);
  magic[4] = 0;

  if (std::strcmp(magic, "GRSW") != 0) {
    ShowErrorDialog(this->m_context->renderSystem, "Invalid Format", 0);
    return false;
  }

  unsigned char major = 0;
  unsigned char minor = 0;
  input.Read(&major, 1);
  input.Read(&minor, 1);
  const unsigned version = static_cast<unsigned>(major) * 256u + minor;

  if (version > 0x207u) {
    char message[256];
    std::snprintf(message, sizeof(message), "Unsupported Version\n current version: %d.%d < %d.%d", 2, 7, major, minor);
    ShowErrorDialog(this->m_context->renderSystem, message, 0);

    return false;
  }

  this->major = major;
  this->minor = minor;
  unsigned build = 0;

  if (major == 2 && minor >= 5) {
    input.Read(&build, sizeof(build));
  }

  if (major == 2 && minor >= 2) {
    input.Read(&this->flag, 1);
  }

  char lastName[40];
  input.Read(lastName, sizeof(lastName));
  input.Read(lastName, sizeof(lastName));
  this->groundName = lastName;

  if (version >= 0x104u) {
    input.Read(lastName, sizeof(lastName));
    this->terrainName = lastName;
  }

  input.Read(lastName, sizeof(lastName));
  this->sourceName = lastName;
  this->ReadEnvironment(input, version);

  const int targetVersion[2] = {7, 2};
  const unsigned char* const fileVersion[2] = {&minor, &major};

  if (CompareWorldVersion(targetVersion, fileVersion) <= 0 && build >= 221) {
    unsigned count;
    input.Read(&count, sizeof(count));

    const unsigned position = input.position + 4u * count;
    input.position = position <= input.size ? position : input.size;
  }

  this->ReadObjects(input, version, build, lastName);

  if (major == 2 && minor >= 1) {
    this->spatial = new WorldSpatialTree;
    this->spatial->Subdivide(0);
    this->spatial->ReadBounds(input, 0);
  }

  file.Close();
  std::memcpy(this->m_context->lighting.ambient, this->ambient, sizeof(this->ambient));
  std::memcpy(this->m_context->lighting.diffuse, this->diffuse, sizeof(this->diffuse));
  std::memcpy(this->m_context->lighting.secondaryDiffuse, this->diffuse, sizeof(this->diffuse));

  return true;
}
