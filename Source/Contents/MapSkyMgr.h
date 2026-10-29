#ifndef RAGNAROK_SOURCE_CONTENTS_MAPSKYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAPSKYMGR_H

#include "Script/LuaScript.h"

#include <string>

class CWeatherEffect {
public:
  void Play(char kind);

  /* 0000 */ unsigned char unknown_0000[0x10];
  /* 0010 */ void* actor;
  /* 0014 */ unsigned char unknown_0014[0x30];
  /* 0044 */ void* unknown_0044;
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
