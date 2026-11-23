#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/World/WorldResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Resource/ResourceMgr.h"

#include <compare>
#include <cstdio>
#include <cstring>
#include <tuple>

// STATIC: 0x016FDA64
// GLOBAL: gl_24387

WorldLighting g_worldLighting{};

namespace {
  struct WorldModelFileRecord {
    /* 0000 */ char model[80];
    /* 0050 */ char node[80];
    /* 00A0 */ vector3d position{};
    /* 00AC */ vector3d rotation{};
    /* 00B8 */ vector3d scale{};
  };

  struct WorldLightFileRecord {
    /* 0000 */ char name[80];
    /* 0050 */ vector3d position{0.0f, 0.0f, 0.0f};
    /* 005C */ unsigned char colorAndRange[16];
  };

  struct WorldLegacySoundFileRecord {
    /* 0000 */ char name[80];
    /* 0050 */ char file[80];
    /* 00A0 */ vector3d position{0.0f, 0.0f, 0.0f};
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
  std::strong_ordering CompareWorldVersion(const std::tuple<int, int>& targetVersion, const std::tuple<unsigned char&, unsigned char&>& fileVersion) {
    return targetVersion <=> fileVersion;
  }

  // INLINED
  inline void SetLightColor(float color[3], vector3d value) {
    color[0] = value.x;
    color[1] = value.y;
    color[2] = value.z;
  }

  // OFFSET: 0x00763740
  // FUNCTION: fn_08079
  __declspec(noinline) unsigned char IsUnsupportedWorldVersion(unsigned major, unsigned minor, unsigned build, char* message, unsigned capacity) {
    return 0;
  }
}

// OFFSET: 0x00763060
// FUNCTION: fn_08076
C3dWorldRes::C3dWorldRes() : lightDirection{0.0f, 0.0f, 0.0f}, diffuse{0.0f, 0.0f, 0.0f}, ambient{0.0f, 0.0f, 0.0f}, flag(0) {
  this->spatial = nullptr;
}

// OFFSET: 0x00761940
// FUNCTION: fn_08071
C3dWorldRes* C3dWorldRes::Create() {
  return new C3dWorldRes;
}

// OFFSET: 0x00761B40
// FUNCTION: fn_08072
void C3dWorldRes::Unload() {
  for (auto model = this->models.begin(); model != this->models.end(); ++model) {
    delete *model;
  }

  this->models.erase(this->models.begin(), this->models.end());

  for (auto effect = this->effects.begin(); effect != this->effects.end(); ++effect) {
    delete *effect;
  }

  this->effects.erase(this->effects.begin(), this->effects.end());

  for (auto sound = this->sounds.begin(); sound != this->sounds.end(); ++sound) {
    delete *sound;
  }

  if (this->spatial) {
    delete this->spatial;
    this->spatial = nullptr;
  }

  this->sounds.erase(this->sounds.begin(), this->sounds.end());
}

// OFFSET: 0x00761CC0
// FUNCTION: fn_08073
bool C3dWorldRes::Load(const char* name) {
  this->Unload();
  CFile input;

  if (!input.Open(name, 0, true, false)) {
    return false;
  }

  char signature[] = "GRSW";
  char magic[5];
  input.Read(magic, 4);
  magic[4] = 0;

  if (std::strcmp(magic, signature) != 0) {
    ShowErrorDialog("Invalid Format", 0);
    return false;
  }

  unsigned char major = 0;
  input.Read(&major, 1);
  unsigned char minor = 0;
  input.Read(&minor, 1);

  char message[256];

  if (major > 2 || (major == 2 && minor > 7)) {
    sprintf_s(message, sizeof(message), "Unsupported Version\n current version: %d.%d < %d.%d", 2, 7, major, minor);
    ShowErrorDialog(message, 0);

    return false;
  }

  this->major = major;
  this->minor = minor;
  unsigned build = 0;

  if (major == 2 && minor >= 5) {
    input.Read(&build, sizeof(build));
  }

  if (IsUnsupportedWorldVersion(major, minor, build, message, sizeof(message)) == 1) {
    ShowErrorDialog(message, 0);
    return false;
  }

  if (major == 2 && minor >= 2) {
    input.Read(&this->flag, 1);
  }

  char lastName[40];
  input.Read(lastName, sizeof(lastName));
  input.Read(lastName, sizeof(lastName));
  this->groundName = std::string(lastName);

  if ((major == 1 && minor >= 4) || major >= 2) {
    input.Read(lastName, sizeof(lastName));
    this->terrainName = std::string(lastName);
  }

  input.Read(lastName, sizeof(lastName));
  this->sourceName = std::string(lastName);

  if ((major == 2 && minor < 6) || major < 2) {
    if ((major == 1 && minor >= 3) || major >= 2) {
      input.Read(&this->waterLevel, sizeof(this->waterLevel));
    }

    if ((major == 1 && minor >= 8) || major >= 2) {
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

    if ((major == 1 && minor >= 9) || major >= 2) {
      input.Read(&this->waterAnimation, sizeof(this->waterAnimation));
    } else {
      this->waterAnimation = 3;
    }
  }

  if ((major == 1 && minor >= 5) || major >= 2) {
    input.Read(&this->longitude, sizeof(this->longitude));
    input.Read(&this->latitude, sizeof(this->latitude));
    input.Read(&this->diffuse, sizeof(this->diffuse));
    input.Read(&this->ambient, sizeof(this->ambient));
  } else {
    this->latitude = 45;
    this->longitude = 45;

    this->diffuse = vector3d{1.0f, 1.0f, 1.0f};
    this->ambient = vector3d{0.3f, 0.3f, 0.3f};
  }

  constexpr float pi = 3.141592f;
  matrix matrix;
  matrix.MakeXRotation((static_cast<float>(this->latitude) / 180.0f) * pi);
  matrix.AppendYRotation((static_cast<float>(this->longitude) / 180.0f) * pi);

  for (unsigned component = 0; component < 3; ++component) {
    (&this->lightDirection.x)[component] =
      ((matrix.values[component] * 0.0f + matrix.values[component + 3]) + matrix.values[component + 6] * 0.0f) + matrix.values[component + 9];
  }

  if ((major == 1 && minor >= 7) || major >= 2) {
    unsigned unused;
    input.Read(&unused, sizeof(unused));
  }

  if ((major == 1 && minor >= 6) || major >= 2) {
    input.Read(&this->bounds[0], sizeof(this->bounds[0]));
    input.Read(&this->bounds[1], sizeof(this->bounds[1]));
    input.Read(&this->bounds[2], sizeof(this->bounds[2]));
    input.Read(&this->bounds[3], sizeof(this->bounds[3]));
  } else {
    this->bounds[0] = -500;
    this->bounds[1] = 500;
    this->bounds[2] = -500;
    this->bounds[3] = 500;
  }

  if (CompareWorldVersion(std::tuple<int, int>(2, 7), std::tie(major, minor)) <= 0 && build >= 221) {
    unsigned count;
    input.Read(&count, sizeof(count));

    input.Seek(4u * count, FILE_CURRENT);
  }

  int count;
  input.Read(&count, sizeof(count));

  for (int index = 0; index < count; ++index) {
    int type;
    input.Read(&type, sizeof(type));

    switch (type) {
    case 1: {
      int animation;
      int animationType;
      int modelIndex = -1;
      float speed;
      unsigned char flag;

      if ((major == 1 && minor >= 3) || major >= 2) {
        input.Read(lastName, 40);
        input.Read(&animation, sizeof(animation));
        input.Read(&speed, sizeof(speed));
        input.Read(&animationType, sizeof(animationType));

        if (major > 2 || (major == 2 && minor >= 6 && build >= 186)) {
          input.Read(&flag, 1);
        } else {
          flag = 0;
        }

        if (CompareWorldVersion(std::tuple<int, int>(2, 7), std::tie(major, minor)) <= 0) {
          input.Read(&modelIndex, sizeof(modelIndex));
        }

        if (speed > 100.0f || speed <= 0.0f) {
          speed = 1.0f;
        }
      } else {
        animation = 0;
        animationType = 0;
        speed = 1.0f;
      }

      WorldModelFileRecord wire;
      input.Read(&wire, sizeof(wire));
      auto* record = new WorldModelRecord;

      if (record) {
        std::strcpy(record->name, lastName);
        std::strcpy(record->model, wire.model);
        std::strcpy(record->node, wire.node);
        record->animationSpeed = speed;
        record->animation = animation;
        record->animationType = animationType;
        record->position = wire.position;
        record->rotation = wire.rotation;
        record->scale = wire.scale;
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

    case 4: {
      auto* record = new WorldEffectRecord;
      input.Read(record, sizeof(*record));
      this->effects.push_back(record);
      break;
    }

    case 3: {
      if (major >= 2) {
        auto* record = new WorldSoundRecord;
        input.Read(record, sizeof(*record));
        this->sounds.push_back(record);
      } else {
        WorldLegacySoundFileRecord wire;
        input.Read(&wire, sizeof(wire));
        auto* record = new WorldSoundRecord;
        std::strncpy(record->name, wire.name, 79);
        std::strncpy(record->file, wire.file, 79);
        record->position[0] = wire.position.x;
        record->position[1] = wire.position.y;
        record->position[2] = wire.position.z;
        record->volume = wire.volume;
        record->width = wire.width;
        record->height = wire.height;
        record->range = wire.range;
        record->cycle = 4.0f;
        this->sounds.push_back(record);
      }

      break;
    }

    default:
      break;
    }
  }

  if (major == 2 && minor >= 1) {
    this->spatial = new WorldSpatialTree;
    this->spatial->Subdivide(0);
    this->spatial->ReadBounds(input, 0);
  }

  input.Close();
  SetLightColor(g_worldLighting.ambient, this->ambient);
  SetLightColor(g_worldLighting.diffuse, this->diffuse);
  SetLightColor(g_worldLighting.secondaryDiffuse, this->diffuse);

  return true;
}

// INLINED
C3dWorldRes::C3dWorldRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25564
C3dWorldRes g_rswPrototype("rsw", "");
