#ifndef RAGNAROK_SOURCE_RESOURCE_WORLD_WORLDRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WORLD_WORLDRESOURCE_H

#include "Contents/World/WorldSpatialTree.h"
#include "Resource/Resource.h"

#include <cstddef>
#include <list>
#include <string>

class CFile;

struct WorldModelRecord {
  /* 0000 */ char name[40];
  /* 0028 */ char model[80];
  /* 0078 */ char node[80];
  /* 00C8 */ float position[3]{};
  /* 00D4 */ float rotation[3]{};
  /* 00E0 */ float scale[3]{};
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
  /* 00A0 */ float position[3]{};
  /* 00AC */ float volume;
  /* 00B0 */ int width;
  /* 00B4 */ int height;
  /* 00B8 */ float range;
  /* 00BC */ float cycle;
};

struct WorldEffectRecord {
  /* 0000 */ char name[80];
  /* 0050 */ float position[3]{};
  /* 005C */ int id;
  /* 0060 */ float delay;
  /* 0064 */ float parameters[4];
};

struct WorldLighting {
  /* 0000 */ float diffuse[3];
  /* 000C */ float secondaryDiffuse[3];
  /* 0018 */ float ambient[3];
};
extern WorldLighting g_worldLighting;

class C3dWorldRes : public CRes {
public:
  C3dWorldRes();
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
  void ReadEnvironment(CFile& input, unsigned version);
  void ReadObjects(CFile& input, unsigned version, unsigned build, char* lastName);
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
static_assert(offsetof(C3dWorldRes, models) == 0x0110);
static_assert(offsetof(C3dWorldRes, effects) == 0x0118);
static_assert(offsetof(C3dWorldRes, sounds) == 0x0120);
static_assert(offsetof(C3dWorldRes, spatial) == 0x0128);
static_assert(offsetof(C3dWorldRes, groundName) == 0x012C);
static_assert(offsetof(C3dWorldRes, terrainName) == 0x0144);
static_assert(offsetof(C3dWorldRes, sourceName) == 0x015C);
static_assert(offsetof(C3dWorldRes, lightDirection) == 0x0194);
static_assert(offsetof(C3dWorldRes, diffuse) == 0x01A0);
static_assert(offsetof(C3dWorldRes, ambient) == 0x01AC);
static_assert(offsetof(C3dWorldRes, flag) == 0x01CC);
static_assert(sizeof(C3dWorldRes) == 0x01D0);
static_assert(sizeof(WorldLighting) == 0x24);
#endif

#endif
