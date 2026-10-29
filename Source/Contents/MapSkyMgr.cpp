#include "Contents/MapSkyMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/World/World.h"
#include "File/MapName.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"

#include <cstdlib>

struct MapSkyCloud : CEffect {
  /* 00CC */ unsigned char unknown_00CC[0x28];
  /* 00F4 */ unsigned char colorR;
  /* 00F5 */ unsigned char colorG;
  /* 00F6 */ unsigned char colorB;
  /* 00F7 */ unsigned char unknown_00F7;
  /* 00F8 */ float velocityX;
  /* 00FC */ float velocityY;
  /* 0100 */ float velocityZ;
  /* 0104 */ float unknown_0104;
  /* 0108 */ float unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ int unknown_0110;
  /* 0114 */ int unknown_0114;
  /* 0118 */ int unknown_0118;
  /* 011C */ float unknown_011C;
  /* 0120 */ float spanX;
  /* 0124 */ float spanZ;
  /* 0128 */ unsigned char flag;
};

// GLOBAL: gl_24482
CWeatherEffect g_weatherEffect;

// GLOBAL: gl_24633
float g_depthAverageHalf = 0.5f;

// OFFSET: 0x005B8D50
// FUNCTION: fn_03431
CMapSkyMgr::CMapSkyMgr() : m_script(nullptr) {
  this->m_script = new LuaScript();
  this->m_script->Open();
}

// OFFSET: 0x005B8DD0
// FUNCTION: fn_03432
void CMapSkyMgr::Unused() {
}

// OFFSET: 0x005B8F00
// FUNCTION: fn_03435
bool CMapSkyMgr::LoadData() {
  return this->m_script->Load("Lua Files\\MapSkyData\\MapSkyData", true, false);
}

// OFFSET: 0x00C7B790
// FUNCTION: fn_20367
void CWeatherEffect::Play(char kind) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  this->actor = mode->scene->player;

  int effectId = 0;
  switch (kind) {
  case 0:
    effectId = 0xe5;
    break;
  case 1:
    effectId = 0xe6;
    break;
  case 2:
    effectId = 0xe9;
    break;
  case 3:
    effectId = 0x203;
    break;
  case 4:
    effectId = 0x204;
    break;
  case 5:
    effectId = 0x250;
    break;
  case 7:
    effectId = 0x2b9;
    break;
  case 8:
    effectId = 0x2ba;
    break;
  case 9:
    effectId = 0x3b1;
    break;
  case 10:
    effectId = 0x415;
    break;
  case 11:
    effectId = 0x45b;
    break;
  case 15:
    effectId = 0x479;
    break;
  default:
    return;
  }

  static_cast<EffectActor*>(this->actor)->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
}

// OFFSET: 0x005B90F0
// FUNCTION: fn_03436
void CMapSkyMgr::CreateClouds(const std::string& mapName) {
  int typeCount = 0;
  this->m_script->Call("GetCloudTypeNum", "s>d", mapName.c_str(), &typeCount);

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  C3dAttr* terrain = mode->scene->terrain;
  const int mapWidth = terrain->width * terrain->cellSize;
  const int mapHeight = terrain->height * terrain->cellSize;

  for (int type = 1; type <= typeCount; ++type) {
    int count = 0;
    int range = 0x82;
    int red = 0;
    int green = 0;
    int blue = 0;
    double velX = 0.0;
    double velY = 0.0;
    double velZ = 0.0;
    int param0 = 0;
    int param1 = 0;
    double param2 = 0.0;
    int param3 = 0;
    int param4 = 0;
    double param5 = 0.0;
    double posY = 0.0;
    double param7 = 0.0;
    unsigned char flag = 1;

    this->m_script->Call(
      "GetCloudInfo",
      "sd>dddddlllddlddlllb",
      mapName.c_str(),
      type,
      &count,
      &range,
      &red,
      &green,
      &blue,
      &velX,
      &velY,
      &velZ,
      &param0,
      &param1,
      &param2,
      &param3,
      &param4,
      &param5,
      &posY,
      &param7,
      &flag
    );

    const float spanX = static_cast<float>(mapWidth) / static_cast<float>(count + 1);
    const float spanZ = static_cast<float>(mapHeight) / static_cast<float>(count + 1);
    if (count <= 0) {
      continue;
    }

    const int halfWidth = static_cast<int>(static_cast<float>(mapWidth) * g_depthAverageHalf);
    const int halfHeight = static_cast<int>(static_cast<float>(mapHeight) * g_depthAverageHalf);

    for (int index = 0; index < count; ++index) {
      const int signX = rand() % 2;
      int offsetX = 0;
      if (halfWidth != 0) {
        offsetX = rand() % halfWidth;
        if (offsetX < 0) {
          offsetX = 0;
        }
      }

      const float x = static_cast<float>((1 - 2 * signX) * offsetX);

      const int signZ = rand() % 2;
      int offsetZ = 0;
      if (halfHeight != 0) {
        offsetZ = rand() % halfHeight;
        if (offsetZ < 0) {
          offsetZ = 0;
        }
      }

      const float z = static_cast<float>((1 - 2 * signZ) * offsetZ);
      const float y = static_cast<float>(posY);

      CEffect* spawned = g_effectMgr->Spawn(0, 0x47a, x, y, z, 0, -1, 0, 0);
      if (!spawned) {
        continue;
      }

      auto* cloud = static_cast<MapSkyCloud*>(spawned);
      cloud->range = range;
      cloud->colorR = static_cast<unsigned char>(red);
      cloud->colorG = static_cast<unsigned char>(green);
      cloud->colorB = static_cast<unsigned char>(blue);
      cloud->velocityX = static_cast<float>(velX);
      cloud->velocityY = static_cast<float>(velY);
      cloud->velocityZ = static_cast<float>(velZ);
      cloud->unknown_010C = param0;
      cloud->unknown_0110 = param1;
      cloud->unknown_0114 = param3;
      cloud->unknown_0118 = param4;
      cloud->unknown_0104 = static_cast<float>(param2);
      cloud->unknown_0108 = static_cast<float>(param5);
      cloud->unknown_011C = static_cast<float>(param7);
      cloud->spanX = spanX;
      cloud->spanZ = spanZ;
      cloud->flag = flag;
      cloud->Slot07();
    }
  }
}

// OFFSET: 0x005B95F0
// FUNCTION: fn_03437
void CMapSkyMgr::Apply(const std::string& mapName) {
  std::string sky = ScriptMapName(mapName);
  unsigned char exists = 0;
  this->m_script->Call("IsExistMapSkyData", "s>b", sky.c_str(), &exists);
  if (exists == 0) {
    g_renderer->SetClearColor(0xFF000000u);
    return;
  }

  int red = 0;
  int green = 0;
  int blue = 0;
  this->m_script->Call("GetBackgroundColor", "s>ddd", sky.c_str(), &red, &green, &blue);
  g_renderer->SetClearColor(
    0xFF000000u | ((static_cast<unsigned>(red) & 255u) << 16) | ((static_cast<unsigned>(green) & 255u) << 8) | (static_cast<unsigned>(blue) & 255u)
  );

  unsigned char drawStar = 0;
  this->m_script->Call("IsDrawStar", "s>b", sky.c_str(), &drawStar);
  if (drawStar == 1) {
    g_weatherEffect.Play(9);
  }

  this->CreateClouds(sky);

  int oldCount = 0;
  this->m_script->Call("GetOldCloudTypeNum", "s>d", sky.c_str(), &oldCount);
  for (int index = 1; index <= oldCount; ++index) {
    int info = -1;
    this->m_script->Call("GetOldCloudInfo", "sd>d", sky.c_str(), index, &info);
    if (info != -1) {
      g_weatherEffect.Play(static_cast<char>(info));
    }
  }

  unsigned char fog = 0;
  this->m_script->Call("IsBackGroundFog", "s>b", sky.c_str(), &fog);
  if (fog == 1) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->unknown_00FC = true;
  }
}
