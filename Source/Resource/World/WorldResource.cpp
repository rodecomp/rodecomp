#include "Client/Application/ErrorDialog.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/World/WorldResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <array>
#include <bit>
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
// FUNCTION: fn_08076
C3dWorldRes::C3dWorldRes(WorldResourceContext& context) : spatial(nullptr), lightDirection{}, diffuse{}, ambient{}, flag(0), m_context(&context) {
}

// OFFSET: 0x00763550
// OFFSET: 0x00762FE0
// FUNCTION: fn_08077
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

// INLINED
inline void C3dWorldRes::ReadEnvironment(ResourceByteReader& input, unsigned version) {
  if (version < 0x206u) {
    if (version >= 0x103u) {
      input.Read(&this->waterLevel, sizeof(this->waterLevel));
    }

    if (version >= 0x108u) {
      input.Read(&this->waterType, sizeof(this->waterType));
      input.Read(&this->waveHeight, sizeof(this->waveHeight));
      input.Read(&this->waveSpeed, sizeof(this->waveSpeed));
      input.Read(&this->wavePitch, sizeof(this->wavePitch));
    } else {
      this->waterType = 0;
      this->waveHeight = 1.0f;
      this->waveSpeed = 2.0f;
      this->wavePitch = 50.0f;
    }

    if (version >= 0x109u) {
      input.Read(&this->waterAnimation, sizeof(this->waterAnimation));
    } else {
      this->waterAnimation = 3;
    }
  }

  if (version >= 0x105u) {
    input.Read(&this->longitude, sizeof(this->longitude));
    input.Read(&this->latitude, sizeof(this->latitude));
    input.Read(this->diffuse, sizeof(this->diffuse));
    input.Read(this->ambient, sizeof(this->ambient));
  } else {
    this->latitude = 45;
    this->longitude = 45;

    for (float& value : this->diffuse) {
      value = 1.0f;
    }

    for (float& value : this->ambient) {
      value = 0.3f;
    }
  }

  constexpr float pi = 3.141592f;
  AffineMatrix matrix;
  matrix.SetXRotation((static_cast<float>(this->latitude) / 180.0f) * pi);
  matrix.RotateY((static_cast<float>(this->longitude) / 180.0f) * pi);

  for (unsigned component = 0; component < 3; ++component) {
    this->lightDirection[component] =
      ((matrix.values[component] * 0.0f + matrix.values[component + 3]) + matrix.values[component + 6] * 0.0f) + matrix.values[component + 9];
  }

  if (version >= 0x107u) {
    unsigned unused;
    input.Read(&unused, sizeof(unused));
  }

  if (version >= 0x106u) {
    for (int& bound : this->bounds) {
      input.Read(&bound, sizeof(bound));
    }
  } else {
    this->bounds[0] = -500;
    this->bounds[1] = 500;
    this->bounds[2] = -500;
    this->bounds[3] = 500;
  }
}

// INLINED
inline void C3dWorldRes::ReadObjects(ResourceByteReader& input, unsigned version, unsigned build, char* lastName) {
  int count;
  input.Read(&count, sizeof(count));

  for (int index = 0; index < count; ++index) {
    int type;
    input.Read(&type, sizeof(type));

    switch (type) {
    case 1: {
      int animation = 0;
      int unknown = 0;
      int modelIndex = -1;
      float speed = 1.0f;
      unsigned char flag = 0;

      if (version >= 0x103u) {
        input.Read(lastName, 40);
        input.Read(&animation, sizeof(animation));
        input.Read(&speed, sizeof(speed));
        input.Read(&unknown, sizeof(unknown));

        if (version >= 0x206u && build >= 186) {
          input.Read(&flag, 1);
        }

        if (version >= 0x207u) {
          input.Read(&modelIndex, sizeof(modelIndex));
        }

        if (speed > 100.0f || !(0.0f < speed)) {
          speed = 1.0f;
        }
      }

      std::array<unsigned char, 196> wire;
      std::memset(wire.data() + 160, 0, 36);
      input.Read(wire.data(), static_cast<unsigned>(wire.size()));
      auto* record = new WorldModelRecord;
      std::strcpy(record->name, lastName);
      std::strcpy(record->model, reinterpret_cast<const char*>(wire.data()));
      std::strcpy(record->node, reinterpret_cast<const char*>(wire.data() + 80));
      std::memcpy(record->position, wire.data() + 160, 12);
      std::memcpy(record->rotation, wire.data() + 172, 12);
      std::memcpy(record->scale, wire.data() + 184, 12);
      record->animationSpeed = speed;
      record->animation = animation;
      record->unknown_00F0 = unknown;
      record->flag = flag != 0;
      record->index = modelIndex;
      this->models.push_back(record);
      break;
    }

    case 2: {
      unsigned char light[108];
      std::memset(light + 72, 0, 12);
      input.Read(light, sizeof(light));
      break;
    }

    case 3: {
      auto* record = new WorldSoundRecord;
      std::memset(record->position, 0, sizeof(record->position));

      if (version < 0x200u) {
        unsigned char wire[188];
        std::memset(wire + 160, 0, 12);
        input.Read(wire, sizeof(wire));
        std::strncpy(record->name, reinterpret_cast<const char*>(wire), 79);
        std::strncpy(record->file, reinterpret_cast<const char*>(wire + 80), 79);
        std::memcpy(record->position, wire + 160, 12);
        std::memcpy(&record->volume, wire + 172, 4);
        std::memcpy(&record->width, wire + 176, 4);
        std::memcpy(&record->height, wire + 180, 4);
        std::memcpy(&record->range, wire + 184, 4);
        record->cycle = 4.0f;
      } else {
        input.Read(record, sizeof(*record));
      }

      this->sounds.push_back(record);
      break;
    }

    case 4: {
      auto* record = new WorldEffectRecord;
      std::memset(record->position, 0, sizeof(record->position));
      input.Read(record, sizeof(*record));
      this->effects.push_back(record);
      break;
    }

    default:
      break;
    }
  }
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
