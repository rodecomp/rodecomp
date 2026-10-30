#ifndef RAGNAROK_SOURCE_CONTENTS_MAPSKYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAPSKYMGR_H

#include "Script/LuaScript.h"
#include "Utility/Math/Vector3d.h"

#include <string>
#include <utility>
#include <vector>

struct CRagEffect;

class CWeatherEffect {
public:
  CWeatherEffect();

  void AddMapWeather(std::string mapName, std::string weather);
  vector3d GetWindDirection() const;
  void Play(char kind);
  void StartDarken();
  void StopDarken();
  void Fireworks();
  void Snow();
  void StartLightning();
  void StopLightning();
  void Process();
  void ResetEffect();
  void ApplyMapWeather(std::string mapName);
  void ExtendEffect();

  /* 0000 */ unsigned char darken;
  /* 0001 */ unsigned char unknown_0001;
  /* 0002 */ unsigned char lightning;
  /* 0004 */ std::vector<std::pair<std::string, std::string>> mapWeathers;
  /* 0010 */ void* actor;
  /* 0014 */ float color[3];
  /* 0020 */ float targetColor[3];
  /* 002C */ float fog[3];
  /* 0038 */ float lightColor[3];
  /* 0044 */ CRagEffect* effect;
  /* 0048 */ unsigned char unknown_0048[0x10];
  /* 0058 */ int counter;
};

class CMapSkyMgr {
public:
  CMapSkyMgr();
  // INLINED
  virtual ~CMapSkyMgr() {
    if (this->m_script) {
      this->m_script->Close();
      delete this->m_script;
      this->m_script = nullptr;
    }
  }

  bool LoadData();
  void Unused();
  void CreateClouds(const std::string& mapName);
  void Apply(const std::string& mapName);

private:
  /* 0004 */ LuaScript* m_script;
};

extern CWeatherEffect g_weatherEffect;
extern float g_depthAverageHalf;

#endif
