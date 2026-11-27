#ifndef RAGNAROK_SOURCE_CONTENTS_MAP_MAPSKYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAP_MAPSKYMGR_H

#include "Script/LuaScript.h"
#include "Utility/Math/Vector3d.h"

#include <string>
#include <utility>
#include <vector>

struct CRagEffect;

enum WeatherEffectKind : char {
  WEATHER_EFFECT_CLOUD = 0,
  WEATHER_EFFECT_CLOUD2 = 1,
  WEATHER_EFFECT_CLOUD3 = 2,
  WEATHER_EFFECT_CLOUD4 = 3,
  WEATHER_EFFECT_CLOUD5 = 4,
  WEATHER_EFFECT_CLOUD6 = 5,
  WEATHER_EFFECT_CLOUD7 = 7,
  WEATHER_EFFECT_CLOUD8 = 8,
  WEATHER_EFFECT_STARS = 9,
  WEATHER_EFFECT_CLOUD10 = 10,
  WEATHER_EFFECT_CN_METEOR = 11,
  WEATHER_EFFECT_HAMMER_OF_GOD = 15,
};

class CWeatherEffect {
public:
  CWeatherEffect();

  void AddMapWeather(std::string mapName, std::string weather);
  vector3d GetWindDirection() const;
  void Play(char kind);
  void StartDarken();
  void StopDarken();
  void Fireworks();
  void Maple();
  void Sakura();
  void Snow();
  void StartLightning();
  void StopLightning();
  void Process();
  void ResetEffect();
  void ApplyMapWeather(std::string mapName);
  void ExtendEffect();
  void Unused();

  /* 0000 */ unsigned char darken;
  /* 0001 */ unsigned char enabled;
  /* 0002 */ unsigned char lightning;
  /* 0004 */ std::vector<std::pair<std::string, std::string>> mapWeathers;
  /* 0010 */ void* actor;
  /* 0014 */ float color[3];
  /* 0020 */ float targetColor[3];
  /* 002C */ float fog[3];
  /* 0038 */ float lightColor[3];
  /* 0044 */ CRagEffect* effect;
  /* 0048 */ unsigned char padding_0048[0x10];
  /* 0058 */ int counter;
};

class CMapSkyMgr {
public:
  __declspec(noinline) CMapSkyMgr();
  virtual ~CMapSkyMgr();

  bool LoadData();
  void ApplyBackgroundFog(const std::string& mapName);
  void ApplyBackgroundColor(const std::string& mapName);
  void Unused();
  void CreateClouds(const std::string& mapName);
  void Apply(const std::string& mapName);

private:
  /* 0004 */ LuaScript* m_script;
};

extern CWeatherEffect g_weatherEffect;
extern float g_depthAverageHalf;
extern CMapSkyMgr* g_mapSkyMgr;

CMapSkyMgr* GetMapSkyMgr();
void IgnoreNull();

#endif
