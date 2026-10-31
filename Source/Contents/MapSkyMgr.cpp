#include "Contents/MapSkyMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/World/World.h"
#include "File/MapName.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cctype>
#include <cstdlib>
#include <new>

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

// GLOBAL: gl_25254
std::vector<std::string> g_registeredLowercaseNames;

static_assert(sizeof(std::vector<std::string>) == 12);

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

// OFFSET: 0x00C7B200
// FUNCTION: fn_20362
CWeatherEffect::CWeatherEffect()
    : darken(0), unknown_0001(1), lightning(0), actor(nullptr), targetColor{}, effect(nullptr), counter(0) {
  this->color[0] = 1.0f;
  this->color[1] = 1.0f;
  this->color[2] = 1.0f;
  this->fog[0] = 0.0f;
  this->fog[1] = 0.0f;
  this->fog[2] = 0.0f;
  this->lightColor[0] = 1.0f;
  this->lightColor[1] = 1.0f;
  this->lightColor[2] = 1.0f;
}

// OFFSET: 0x00C7B530
// FUNCTION: fn_20365
void CWeatherEffect::AddMapWeather(std::string mapName, std::string weather) {
  std::pair<std::string, std::string> entry;
  entry.second = weather;
  entry.first = mapName;
  this->mapWeathers.push_back(std::move(entry));
}

// OFFSET: 0x00C7B6F0
// FUNCTION: fn_20366
vector3d CWeatherEffect::GetWindDirection() const {
  vector3d direction = {0.0f, 0.0f, 0.0f};
  const float sine = ClientSine((this->counter >> 3) + 90);
  const float cosine = ClientCosine((this->counter >> 3) + 90);
  direction.z = 0.0f;
  direction.x = cosine * 1000.0f + sine * 0.0f;
  direction.y = cosine * 0.0f - sine * 1000.0f;
  return direction;
}

// OFFSET: 0x00C7B790
// FUNCTION: fn_20367
void CWeatherEffect::Play(char kind) {
  if (this->effect && this->effect->GetDuration() - this->effect->state.stateCnt <= 300) {
    return;
  }

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

  this->effect = static_cast<EffectActor*>(this->actor)->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
}

// OFFSET: 0x00C7B9E0
// FUNCTION: fn_20368
void CWeatherEffect::StartDarken() {
  this->darken = 1;
}

// OFFSET: 0x00C7B9F0
// FUNCTION: fn_20369
void CWeatherEffect::Fireworks() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  this->actor = mode->scene->player;
  static_cast<EffectActor*>(this->actor)->LaunchEffect(0x12d, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

  for (int count = 0; count < 160; ++count) {
    static_cast<EffectActor*>(this->actor)->LaunchEffect(0x129, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x00C7BB20
// FUNCTION: fn_20370
void CWeatherEffect::Snow() {
  if (this->effect && this->effect->GetDuration() - this->effect->state.stateCnt <= 300) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  this->actor = mode->scene->player;
  static_cast<EffectActor*>(this->actor)->LaunchEffect(0xa2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
}

// OFFSET: 0x00C7BBA0
// FUNCTION: fn_20371
void CWeatherEffect::StartLightning() {
  this->lightning = 1;
  this->counter = 0;
}

// OFFSET: 0x00C7BBB0
// FUNCTION: fn_20372
void CWeatherEffect::Process() {
  if (this->darken == 1) {
    if (this->color[0] > g_depthAverageHalf) {
      this->color[0] -= 0.005f;
    }
    if (this->color[1] > g_depthAverageHalf) {
      this->color[1] -= 0.005f;
    }
  } else if (this->lightning == 1) {
    ++this->counter;
    if (this->counter > 1000) {
      this->lightning = 0;
    }
    if ((this->counter / 3 & 1) == 0) {
      this->color[0] = 0.2f;
      this->color[1] = 0.2f;
    } else {
      this->color[0] = 1.0f;
      this->color[1] = 1.0f;
    }
  } else {
    for (int index = 0; index < 3; ++index) {
      if (this->color[index] < 1.0f && this->color[index] < this->targetColor[index]) {
        this->color[index] += 0.005f;
      }
    }
  }

  if (this->effect && !static_cast<EffectActor*>(this->actor)->state.Attachments().Contains(reinterpret_cast<const RagEffectState*>(this->effect))) {
    this->effect = nullptr;
  }

  this->lightColor[0] = 1.0f - (1.0f - this->fog[0]) * (1.0f - this->color[0]);
  this->lightColor[1] = 1.0f - (1.0f - this->fog[1]) * (1.0f - this->color[1]);
  this->lightColor[2] = 1.0f - (1.0f - this->fog[2]) * (1.0f - this->color[2]);
}

// OFFSET: 0x00C7BD30
// FUNCTION: fn_20373
void CWeatherEffect::ResetEffect() {
  this->effect = nullptr;
}

// OFFSET: 0x00C7BD40
// FUNCTION: fn_20374
void CWeatherEffect::ApplyMapWeather(std::string mapName) {
  if (!g_serviceOptions.Get(8)) {
    return;
  }

  for (auto entry = this->mapWeathers.begin(); entry != this->mapWeathers.end(); ++entry) {
    if (entry->first != mapName) {
      continue;
    }

    if (entry->second == "snow") {
      if (!this->effect || this->effect->GetDuration() - this->effect->state.stateCnt > 300) {
        CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
        this->actor = mode->scene->player;
        this->effect = static_cast<EffectActor*>(this->actor)->LaunchEffect(0xa2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    } else if (entry->second == "sakura") {
      if (!this->effect || this->effect->GetDuration() - this->effect->state.stateCnt > 300) {
        CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
        this->actor = mode->scene->player;
        this->effect = static_cast<EffectActor*>(this->actor)->LaunchEffect(0xa3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    } else if (entry->second == "pokjuk") {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      this->actor = mode->scene->player;
      static_cast<EffectActor*>(this->actor)->LaunchEffect(0x12d, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

      for (int count = 0; count < 160; ++count) {
        static_cast<EffectActor*>(this->actor)->LaunchEffect(0x129, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
  }
}

// OFFSET: 0x00C7C010
// FUNCTION: fn_20375
void CWeatherEffect::Unused() {
}

// OFFSET: 0x00C7C040
// FUNCTION: fn_20376
void CWeatherEffect::StopDarken() {
  this->darken = 0;
}

// OFFSET: 0x00C7C070
// FUNCTION: fn_20377
void CWeatherEffect::ExtendEffect() {
  if (this->effect) {
    this->effect->SetStateDuration(this->effect->state.stateCnt + 300);
  }
}

// OFFSET: 0x00C7C090
// FUNCTION: fn_20378
void CWeatherEffect::StopLightning() {
  this->lightning = 0;
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

// GLOBAL: gl_24480
CMapSkyMgr* g_mapSkyMgr;

// OFFSET: 0x00E5CFB0
// FUNCTION: fn_24375
CMapSkyMgr* GetMapSkyMgr() {
  if (!g_mapSkyMgr) {
    g_mapSkyMgr = new (std::nothrow) CMapSkyMgr;
  }

  return g_mapSkyMgr;
}

// OFFSET: 0x00E5D100
// FUNCTION: fn_24378
void IgnoreNull() {
}

// OFFSET: 0x00E5D110
// FUNCTION: fn_24379
void RegisterLowercaseName(const char* name) {
  if (!name) {
    return;
  }

  std::string lowered(name);
  for (char& character : lowered) {
    character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
  }

  for (const std::string& entry : g_registeredLowercaseNames) {
    if (entry == lowered) {
      return;
    }
  }

  g_registeredLowercaseNames.push_back(lowered.c_str());
}
