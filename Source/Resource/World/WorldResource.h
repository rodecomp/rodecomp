#ifndef RAGNAROK_SOURCE_RESOURCE_WORLD_WORLDRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WORLD_WORLDRESOURCE_H

#include "Contents/World/WorldSpatialTree.h"
#include "Resource/Resource.h"

#include <list>
#include <string>

class ResourceBytes;
struct ResourceByteReader;

struct WorldModelRecord {
  char name[40];
  char model[80];
  char node[80];
  float position[3];
  float rotation[3];
  float scale[3];
  int animation;
  int unknown_00F0;
  float animationSpeed;
  unsigned char unknown_00F8[12];
  unsigned char flag;
  unsigned char unknown_0105[3];
  int index;
};

struct WorldSoundRecord {
  char name[80];
  char file[80];
  float position[3];
  float volume;
  int width;
  int height;
  float range;
  float cycle;
};

struct WorldEffectRecord {
  char name[80];
  float position[3];
  int id;
  float delay;
  float parameters[4];
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
static_assert(sizeof(C3dWorldRes) == 468);
#endif

#endif
