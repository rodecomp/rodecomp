#ifndef RAGNAROK_SOURCE_RESOURCE_WORLD_WORLDRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WORLD_WORLDRESOURCE_H

#include "Contents/World/WorldSpatialTree.h"
#include "Resource/Resource.h"

#include <cstddef>
#include <list>
#include <string>

class ResourceBytes;
struct ResourceByteReader;

struct WorldModelRecord {
  /* 0000 */ char name[40];
  /* 0028 */ char model[80];
  /* 0078 */ char node[80];
  /* 00C8 */ float position[3];
  /* 00D4 */ float rotation[3];
  /* 00E0 */ float scale[3];
  /* 00EC */ int animation;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ float animationSpeed;
  /* 00F8 */ unsigned char unknown_00F8[12];
  /* 0104 */ unsigned char flag;
  /* 0105 */ unsigned char unknown_0105[3];
  /* 0108 */ int index;
};

struct WorldSoundRecord {
  /* 0000 */ char name[80];
  /* 0050 */ char file[80];
  /* 00A0 */ float position[3];
  /* 00AC */ float volume;
  /* 00B0 */ int width;
  /* 00B4 */ int height;
  /* 00B8 */ float range;
  /* 00BC */ float cycle;
};

struct WorldEffectRecord {
  /* 0000 */ char name[80];
  /* 0050 */ float position[3];
  /* 005C */ int id;
  /* 0060 */ float delay;
  /* 0064 */ float parameters[4];
};

struct WorldLighting {
  float diffuse[3];
  float secondaryDiffuse[3];
  float ambient[3];
};
extern WorldLighting g_worldLighting;

struct WorldResourceContext {
  ResourceBytes& file;
  CRenderSystemBase*& renderSystem;
  WorldLighting& lighting;
};

class C3dWorldRes : public CRes {
public:
  explicit C3dWorldRes(WorldResourceContext& context);
  ~C3dWorldRes() override;
  C3dWorldRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ std::list<WorldModelRecord*> models;
  /* 0118 */ std::list<WorldEffectRecord*> effects;
  /* 0120 */ std::list<WorldSoundRecord*> sounds;
  /* 0128 */ WorldSpatialTree* spatial;
  /* 012C */ std::string groundName;
  /* 0144 */ std::string terrainName;
  /* 015C */ std::string sourceName;
  /* 0174 */ float waterLevel;
  /* 0178 */ int waterType;
  /* 017C */ float waveHeight;
  /* 0180 */ float waveSpeed;
  /* 0184 */ float wavePitch;
  /* 0188 */ int waterAnimation;
  /* 018C */ int longitude;
  /* 0190 */ int latitude;
  /* 0194 */ float lightDirection[3];
  /* 01A0 */ float diffuse[3];
  /* 01AC */ float ambient[3];
  /* 01B8 */ unsigned char major;
  /* 01B9 */ unsigned char minor;
  /* 01BA */ unsigned char unknown_01BA[2];
  /* 01BC */ int bounds[4];
  /* 01CC */ unsigned char flag;
  /* 01CD */ unsigned char unknown_01CD[3];

private:
  void ReadEnvironment(ResourceByteReader& input, unsigned version);
  void ReadObjects(ResourceByteReader& input, unsigned version, unsigned build, char* lastName);
  WorldResourceContext* m_context;
};

static_assert(sizeof(WorldModelRecord) == 268);
static_assert(sizeof(WorldSoundRecord) == 192);
static_assert(sizeof(WorldEffectRecord) == 116);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(WorldModelRecord, position) == 0x00C8);
static_assert(offsetof(WorldModelRecord, animationSpeed) == 0x00F4);
static_assert(offsetof(WorldModelRecord, flag) == 0x0104);
static_assert(offsetof(WorldModelRecord, index) == 0x0108);
static_assert(offsetof(WorldSoundRecord, position) == 0x00A0);
static_assert(offsetof(WorldSoundRecord, cycle) == 0x00BC);
static_assert(offsetof(WorldEffectRecord, position) == 0x0050);
static_assert(offsetof(WorldEffectRecord, id) == 0x005C);
static_assert(offsetof(WorldEffectRecord, delay) == 0x0060);
static_assert(offsetof(WorldEffectRecord, parameters) == 0x0064);
static_assert(sizeof(C3dWorldRes) == 468);
#endif

#endif
