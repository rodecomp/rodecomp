#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/World/WorldResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdio>
#include <cstring>

// STATIC: 0x016FDA64
// GLOBAL: gl_24387

WorldLighting g_worldLighting{};

namespace {
  struct WorldModelFileRecord {
    /* 0000 */ char model[80];
    /* 0050 */ char node[80];
    /* 00A0 */ float position[3]{};
    /* 00AC */ float rotation[3]{};
    /* 00B8 */ float scale[3]{};
  };

  struct WorldLightFileRecord {
    /* 0000 */ char name[80];
    /* 0050 */ float position[3]{};
    /* 005C */ unsigned char unknown_005C[16];
  };

  struct WorldLegacySoundFileRecord {
    /* 0000 */ char name[80];
    /* 0050 */ char file[80];
    /* 00A0 */ float position[3]{};
    /* 00AC */ float volume;
    /* 00B0 */ int width;
    /* 00B4 */ int height;
    /* 00B8 */ float range;
  };

  static_assert(sizeof(WorldModelFileRecord) == 0xC4);
  static_assert(sizeof(WorldLightFileRecord) == 0x6C);
  static_assert(sizeof(WorldLegacySoundFileRecord) == 0xBC);

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

  // OFFSET: 0x00763740
  // FUNCTION: fn_08079
  unsigned char IsUnsupportedWorldVersion(unsigned major, unsigned minor, unsigned build, char* message, unsigned capacity) {
    return 0;
  }
}

// OFFSET: 0x00763060
// FUNCTION: fn_08076
C3dWorldRes::C3dWorldRes() : spatial(nullptr), lightDirection{}, diffuse{}, ambient{}, flag(0) {
}

// OFFSET: 0x00763550
// FUNCTION: fn_08077
C3dWorldRes::~C3dWorldRes() = default;

// OFFSET: 0x00761940
// FUNCTION: fn_08071
C3dWorldRes* C3dWorldRes::Create() {
  return new C3dWorldRes;
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
inline void C3dWorldRes::ReadEnvironment(CFile& input, unsigned version) {
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
inline void C3dWorldRes::ReadObjects(CFile& input, unsigned version, unsigned build, char* lastName) {
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

        if (speed > 100.0f || speed <= 0.0f) {
          speed = 1.0f;
        }
      }

      WorldModelFileRecord wire;
      input.Read(&wire, sizeof(wire));
      auto* record = new WorldModelRecord;

      if (record) {
        std::strcpy(record->name, lastName);
        std::strcpy(record->model, wire.model);
        std::strcpy(record->node, wire.node);
        std::memcpy(record->position, wire.position, sizeof(record->position));
        std::memcpy(record->rotation, wire.rotation, sizeof(record->rotation));
        std::memcpy(record->scale, wire.scale, sizeof(record->scale));
        record->animationSpeed = speed;
        record->animation = animation;
        record->unknown_00F0 = unknown;
        record->flag = flag != 0;
        record->index = modelIndex;
        this->models.push_back(record);
      }
      break;
    }

    case 2: {
      WorldLightFileRecord light;
      input.Read(&light, sizeof(light));
      break;
    }

    case 3: {
      WorldSoundRecord* record;

      if (version < 0x200u) {
        WorldLegacySoundFileRecord wire;
        input.Read(&wire, sizeof(wire));
        record = new WorldSoundRecord;
        std::strncpy(record->name, wire.name, 79);
        std::strncpy(record->file, wire.file, 79);
        std::memcpy(record->position, wire.position, sizeof(record->position));
        record->volume = wire.volume;
        record->width = wire.width;
        record->height = wire.height;
        record->range = wire.range;
        record->cycle = 4.0f;
      } else {
        record = new WorldSoundRecord;
        input.Read(record, sizeof(*record));
      }

      this->sounds.push_back(record);
      break;
    }

    case 4: {
      auto* record = new WorldEffectRecord;
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
  CFile input;

  if (!input.Open(name, 0, true, false)) {
    return false;
  }

  char magic[5];
  input.Read(magic, 4);
  magic[4] = 0;

  if (std::strcmp(magic, "GRSW") != 0) {
    ShowErrorDialog(GetRenderSystemHolder()->system, "Invalid Format", 0);
    return false;
  }

  unsigned char major = 0;
  unsigned char minor = 0;
  input.Read(&major, 1);
  input.Read(&minor, 1);
  const unsigned version = static_cast<unsigned>(major) * 256u + minor;

  char message[256];

  if (version > 0x207u) {
    std::snprintf(message, sizeof(message), "Unsupported Version\n current version: %d.%d < %d.%d", 2, 7, major, minor);
    ShowErrorDialog(GetRenderSystemHolder()->system, message, 0);

    return false;
  }

  this->major = major;
  this->minor = minor;
  unsigned build = 0;

  if (major == 2 && minor >= 5) {
    input.Read(&build, sizeof(build));
  }

  if (IsUnsupportedWorldVersion(major, minor, build, message, sizeof(message)) == 1) {
    ShowErrorDialog(GetRenderSystemHolder()->system, message, 0);
    return false;
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

    input.Seek(4u * count, FILE_CURRENT);
  }

  this->ReadObjects(input, version, build, lastName);

  if (major == 2 && minor >= 1) {
    this->spatial = new WorldSpatialTree;
    this->spatial->Subdivide(0);
    this->spatial->ReadBounds(input, 0);
  }

  input.Close();
  std::memcpy(g_worldLighting.ambient, this->ambient, sizeof(this->ambient));
  std::memcpy(g_worldLighting.diffuse, this->diffuse, sizeof(this->diffuse));
  std::memcpy(g_worldLighting.secondaryDiffuse, this->diffuse, sizeof(this->diffuse));

  return true;
}
