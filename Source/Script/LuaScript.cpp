#include "UI/Window/WindowId.h"
#include "Contents/Effects/EffectMessageId.h"
#include "Script/LuaScript.h"
#include "Archive/PakRegistry.h"
#include "Client/Session.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/EmitterEffect.h"
#include "Contents/Effects/EvilsPawEffect.h"
#include "Contents/Effects/WindEffect.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Skills/SkillTreeScript.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Script/LuaFileOperations.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindow.h"
#include "UI/UICheckAttendanceWnd.h"
#include "UI/UILapineScrollBar.h"
#include "UI/UILapineUpgradeBoxWnd.h"
#include "UI/Window/UIWindowMgr.h"

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

struct MiniMapPosition {
  MiniMapPosition() {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
};

// GLOBAL: gl_24913
std::map<std::string, MiniMapPosition> g_exceptionMiniMapPosList;

// GLOBAL: gl_24914
std::map<std::string, std::vector<std::string>> g_kafraServiceMapList;

class LuaScriptBag {
public:
  void Add(std::string left, std::string right);

  /* 0000 */ unsigned char padding_0000[0x7C];
  /* 007C */ std::vector<std::pair<std::string, std::string>> pairs;
};

// OFFSET: 0x00AFC8B0
// FUNCTION: fn_18325
void LuaScriptBag::Add(std::string left, std::string right) {
  this->pairs.push_back(std::make_pair(left, right));
}

struct CashShopBannerWindow {
  /* 0000 */ unsigned char padding_0000[0xBC];
  /* 00BC */ LuaScriptBag* banners;
  /* 00C0 */ LuaScriptBag* banners2;
};

// INLINED
static void AddCashShopBanner(CashShopBannerWindow* window, std::string image, std::string url) {
  window->banners->Add(image, url);
}

// INLINED
static void AddCashShopBanner2(CashShopBannerWindow* window, std::string image, std::string url) {
  window->banners2->Add(image, url);
}

// INLINED
static void AddAttendanceBanner(UICheckAttendanceWnd* window, std::string image, std::string url, int action, int argument) {
  window->m_banner->AddBanner(image, url, action, argument);
}


// OFFSET: 0x00AFF7E0
// FUNCTION: fn_18354
int TraceValue(lua_State* state) {
  std::string text = luaL_checkstring(state, 1);
  MessageBoxA(nullptr, text.c_str(), "Lua error", 0);
  return 1;
}

// OFFSET: 0x00AFF140
// FUNCTION: fn_18352
int SetSubChatWndList(lua_State* state) {
  if (g_session.m_noSubChatWnds.size() + g_session.m_subChatWnds.size() >= 10) {
    return 1;
  }

  std::string type = luaL_checkstring(state, 1);
  std::string name = luaL_checkstring(state, 2);
  const int x = static_cast<int>(luaL_checknumber(state, 3));
  const int y = static_cast<int>(luaL_checknumber(state, 4));
  const int width = static_cast<int>(luaL_checknumber(state, 5));
  const int height = static_cast<int>(luaL_checknumber(state, 6));

  SubChatWndInfo info;
  std::memset(info.options, 0, sizeof(info.options));
  info.name = name;
  info.x = x;
  info.y = y;
  info.width = width;
  info.height = height;

  int index;
  if (type == "On") {
    index = static_cast<int>(g_session.m_noSubChatWnds.size());
    g_session.m_noSubChatWnds[index] = info;
  } else {
    index = static_cast<int>(g_session.m_subChatWnds.size());
    g_session.m_subChatWnds[index] = info;
  }

  lua_pushnumber(state, index);
  return 1;
}

// OFFSET: 0x00AFF630
// FUNCTION: fn_18353
int SetSubChatWndOption(lua_State* state) {
  std::string type = luaL_checkstring(state, 1);
  const int index = static_cast<int>(luaL_checknumber(state, 2));
  const int option = static_cast<int>(luaL_checknumber(state, 3));
  const int value = static_cast<int>(luaL_checknumber(state, 4));

  if (type == "On") {
    const auto window = g_session.m_noSubChatWnds.find(index);
    if (window != g_session.m_noSubChatWnds.end()) {
      window->second.options[option - 1] = value != 0;
    }
  } else {
    const auto window = g_session.m_subChatWnds.find(index);
    if (window != g_session.m_subChatWnds.end()) {
      window->second.options[option - 1] = value != 0;
    }
  }

  return 1;
}

// INLINED
static CGameSettingsMgr* GameSettingsInstance() {
  if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
    Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
  }
  return Singleton<CGameSettingsMgr>::s_instance;
}

// OFFSET: 0x00AFF020
// FUNCTION: fn_18351
int SetCmdOnOffList(lua_State* state) {
  const char* name = luaL_checkstring(state, 1);
  const bool enabled = luaL_checknumber(state, 2) != 0.0;
  GameSettingsInstance();
  const int optionId = SetOption(name, enabled);
  if (optionId != 3) {
    if (optionId == 229) {
      const int value = enabled;
      std::memcpy(g_session.interactionFlags, &value, sizeof(value));
    }
    g_session.m_commandOptions.emplace_back(name, optionId);
  }
  return 0;
}

// OFFSET: 0x00AFEF50
// FUNCTION: fn_18350
int GetCmdOnOffValue(lua_State* state) {
  const char* name = luaL_checkstring(state, 1);
  GameSettingsInstance();
  const auto option = FindServiceOption(name);
  lua_pushnumber(state, option.first != 3 ? static_cast<double>(option.second != 0) : -1.0);
  return 1;
}

// OFFSET: 0x00AFEBD0
// FUNCTION: fn_18341
int LuaRGB(lua_State* state) {
  const int red = static_cast<int>(luaL_checknumber(state, 1));
  const int green = static_cast<int>(luaL_checknumber(state, 2));
  const int blue = static_cast<int>(luaL_checknumber(state, 3));
  lua_pushnumber(state, RGB(red, green, blue));
  return 1;
}

// OFFSET: 0x00B7FFC0
// FUNCTION: fn_19249
int SetEmitterInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEmitterEffect* effect = static_cast<CEmitterEffect*>(g_effectMgr->At(EF_EMITTER, static_cast<unsigned>(index)));
  if (!effect) {
    return 0;
  }

  const int key = static_cast<int>(luaL_checknumber(state, 2));
  switch (key) {
  case MSG_SET_INIT:
    effect->ResetParticles();
    break;
  case MSG_SET_SPAWNDIR1:
  case MSG_SET_SPAWNDIR2:
  case MSG_SET_GRAVITY:
  case MSG_SET_POS:
  case MSG_SET_EMITRADIUS: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    if (key == MSG_SET_SPAWNDIR1) {
      effect->velocityMin.x = x;
      effect->velocityMin.y = y;
      effect->velocityMin.z = z;
    } else if (key == MSG_SET_SPAWNDIR2) {
      effect->velocityMax.x = x;
      effect->velocityMax.y = y;
      effect->velocityMax.z = z;
    } else if (key == MSG_SET_GRAVITY) {
      effect->acceleration.x = x;
      effect->acceleration.y = y;
      effect->acceleration.z = z;
    } else if (key == MSG_SET_POS) {
      effect->position[0] = x;
      effect->position[1] = y;
      effect->position[2] = z;
    } else if (key == MSG_SET_EMITRADIUS) {
      effect->spread.x = x;
      effect->spread.y = y;
      effect->spread.z = z;
    }
    break;
  }
  case MSG_SET_COLOR: {
    const int red = (std::min)(static_cast<int>(luaL_checknumber(state, 3)), 255);
    const int green = (std::min)(static_cast<int>(luaL_checknumber(state, 4)), 255);
    const int blue = (std::min)(static_cast<int>(luaL_checknumber(state, 5)), 255);
    const int alpha = (std::min)(static_cast<int>(luaL_checknumber(state, 6)), 255);
    effect->red = red;
    effect->green = green;
    effect->blue = blue;
    effect->alpha = alpha;
    break;
  }
  case MSG_SET_EMITRATE:
  case MSG_SET_SIZE:
  case MSG_SET_LIFETIME: {
    const float minimum = static_cast<float>(luaL_checknumber(state, 3));
    const float maximum = static_cast<float>(luaL_checknumber(state, 4));
    if (key == MSG_SET_EMITRATE) {
      effect->rateMin = minimum;
      effect->rateMax = maximum;
    } else if (key == MSG_SET_SIZE) {
      effect->scaleMin = minimum;
      effect->scaleMax = maximum;
    } else if (key == MSG_SET_LIFETIME) {
      effect->lifeMin = minimum;
      effect->lifeMax = maximum;
    }
    break;
  }
  case MSG_SET_TEXTURE: {
    std::string texture = luaL_checkstring(state, 3);
    effect->SetTexture(texture);
    break;
  }
  case MSG_SET_SPEED:
    effect->SetParticleSpeed(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case MSG_SET_SRCMODE:
    effect->sourceBlend = luaL_checknumber(state, 3);
    break;
  case MSG_SET_DESTMODE:
    effect->destinationBlend = luaL_checknumber(state, 3);
    break;
  case MSG_SET_MAXPARTICLE:
    effect->SetCount(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  case MSG_SET_ZENABLE:
    effect->depth = static_cast<int>(luaL_checknumber(state, 3)) != 0;
    break;
  case MSG_SET_BILLBOARD_OFF:
    effect->billboard = static_cast<int>(luaL_checknumber(state, 3)) != 0;
    break;
  case MSG_SET_ROTATE_ANGLE: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    effect->rotation[0] = x;
    effect->rotation[1] = y;
    effect->rotation[2] = z;
    break;
  }
  case MSG_SET_ETERNITY:
    effect->fixedAlpha = static_cast<int>(luaL_checknumber(state, 3)) == 1;
    break;
  case MSG_SET_SCALE: {
    const float width = static_cast<float>(luaL_checknumber(state, 3));
    const float height = static_cast<float>(luaL_checknumber(state, 4));
    effect->width = width;
    effect->height = height;
    break;
  }
  }

  return 0;
}

// OFFSET: 0x00B80640
// FUNCTION: fn_19251
int SetMagicFloorInfo(lua_State* state) {
  EffectLists* effects = g_effectMgr;
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  const double value = luaL_checknumber(state, 2);
  CEffect* effect = effects->At(EF_MAGIC_FLOOR, static_cast<unsigned>(index));
  if (!effect) {
    return 0;
  }

  const int key = static_cast<int>(value);
  switch (key) {
  case MSG_SET_INIT:
    effect->ResetParticles();
    break;
  case MSG_SET_AREA:
    *reinterpret_cast<float*>(&effect->areaParameters[8]) = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_SIZE2:
    reinterpret_cast<float&>(effect->scaleParameter) = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_ANGLE:
    *reinterpret_cast<int*>(effect->angleParameter) = luaL_checknumber(state, 3);
    break;
  case MSG_SET_RISEANGLE:
    reinterpret_cast<float&>(effect->rotationParameter) = static_cast<float>(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  case MSG_SET_ALPHA:
    *reinterpret_cast<int*>(&effect->alphaParameters[0]) = luaL_checknumber(state, 3);
    break;
  case MSG_SET_FDA:
    *reinterpret_cast<int*>(&effect->alphaParameters[4]) = luaL_checknumber(state, 3);
    break;
  case MSG_SET_HEIGHT0:
  case MSG_SET_HEIGHT1:
  case MSG_SET_HEIGHT2:
  case MSG_SET_HEIGHT3:
  case MSG_SET_HEIGHT4:
  case MSG_SET_HEIGHT5:
  case MSG_SET_HEIGHT6:
  case MSG_SET_HEIGHT7:
  case MSG_SET_HEIGHT8:
  case MSG_SET_HEIGHT9:
  case MSG_SET_HEIGHT10:
  case MSG_SET_HEIGHT11:
  case MSG_SET_HEIGHT12:
  case MSG_SET_HEIGHT13:
  case MSG_SET_HEIGHT14:
  case MSG_SET_HEIGHT15:
  case MSG_SET_HEIGHT16:
  case MSG_SET_HEIGHT17:
  case MSG_SET_HEIGHT18:
  case MSG_SET_HEIGHT19:
  case MSG_SET_HEIGHT20:
    reinterpret_cast<float*>(&effect->alphaParameters[8])[key - 21] = static_cast<float>(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  }

  return 0;
}

// OFFSET: 0x00B804E0
// FUNCTION: fn_19250
int SetEvilsPawInfo(lua_State* state) {
  EffectLists* effects = g_effectMgr;
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  const double key = luaL_checknumber(state, 2);
  CEvilsPawEffect* effect = static_cast<CEvilsPawEffect*>(effects->At(EF_EVILS_PAW, static_cast<unsigned>(index)));
  if (!effect) {
    return 0;
  }

  switch (static_cast<int>(key)) {
  case MSG_SET_INIT:
    effect->ResetParticles();
    break;
  case MSG_SET_OFFSET_POS: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    effect->offset.x = x;
    effect->offset.y = y;
    effect->offset.z = z;
    break;
  }
  case MSG_SET_SIZE:
    effect->size = static_cast<int>(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case MSG_SET_SPEED:
    effect->frameDelay = static_cast<int>(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  }

  return 0;
}

// OFFSET: 0x00B807E0
// FUNCTION: fn_19252
int SetOtherEffectInfo(lua_State* state) {
  EffectLists* effects = g_effectMgr;
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  const int index = static_cast<int>(luaL_checknumber(state, 2));
  CEffect* effect = effects->At(id, static_cast<unsigned>(index));
  if (!effect) {
    return 0;
  }

  const int key = static_cast<int>(luaL_checknumber(state, 3));
  switch (key) {
  case MSG_SET_INIT:
    effect->ResetParticles();
    break;
  case MSG_SET_AREA:
    *reinterpret_cast<float*>(&effect->areaParameters[8]) = static_cast<float>(luaL_checknumber(state, 4));
    break;
  case MSG_SET_SIZE2:
    reinterpret_cast<float&>(effect->scaleParameter) = static_cast<float>(luaL_checknumber(state, 4));
    break;
  case MSG_SET_ANGLE:
    *reinterpret_cast<int*>(effect->angleParameter) = luaL_checknumber(state, 4);
    break;
  case MSG_SET_RISEANGLE:
    reinterpret_cast<float&>(effect->rotationParameter) = static_cast<float>(static_cast<int>(luaL_checknumber(state, 4)));
    break;
  case MSG_SET_ALPHA:
    *reinterpret_cast<int*>(&effect->alphaParameters[0]) = luaL_checknumber(state, 4);
    break;
  case MSG_SET_FDA:
    *reinterpret_cast<int*>(&effect->alphaParameters[4]) = luaL_checknumber(state, 4);
    break;
  case MSG_SET_HEIGHT0:
  case MSG_SET_HEIGHT1:
  case MSG_SET_HEIGHT2:
  case MSG_SET_HEIGHT3:
  case MSG_SET_HEIGHT4:
  case MSG_SET_HEIGHT5:
  case MSG_SET_HEIGHT6:
  case MSG_SET_HEIGHT7:
  case MSG_SET_HEIGHT8:
  case MSG_SET_HEIGHT9:
  case MSG_SET_HEIGHT10:
  case MSG_SET_HEIGHT11:
  case MSG_SET_HEIGHT12:
  case MSG_SET_HEIGHT13:
  case MSG_SET_HEIGHT14:
  case MSG_SET_HEIGHT15:
  case MSG_SET_HEIGHT16:
  case MSG_SET_HEIGHT17:
  case MSG_SET_HEIGHT18:
  case MSG_SET_HEIGHT19:
  case MSG_SET_HEIGHT20:
    reinterpret_cast<float*>(&effect->alphaParameters[8])[key - 21] = static_cast<float>(static_cast<int>(luaL_checknumber(state, 4)));
    break;
  }

  return 0;
}

// OFFSET: 0x00B7FA30
// FUNCTION: fn_19247
int SetAnimatedEmitterInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEmitterEffect* effect = static_cast<CEmitterEffect*>(g_effectMgr->At(EF_ANIMATED_EMITTER, static_cast<unsigned>(index)));
  if (!effect) {
    return 0;
  }

  const int key = static_cast<int>(luaL_checknumber(state, 2));
  switch (key) {
  case MSG_SET_INIT:
    effect->ResetParticles();
    break;
  case MSG_SET_SPAWNDIR1:
  case MSG_SET_SPAWNDIR2:
  case MSG_SET_GRAVITY:
  case MSG_SET_POS:
  case MSG_SET_EMITRADIUS: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    if (key == MSG_SET_SPAWNDIR1) {
      effect->velocityMin.x = x;
      effect->velocityMin.y = y;
      effect->velocityMin.z = z;
    } else if (key == MSG_SET_SPAWNDIR2) {
      effect->velocityMax.x = x;
      effect->velocityMax.y = y;
      effect->velocityMax.z = z;
    } else if (key == MSG_SET_GRAVITY) {
      effect->acceleration.x = x;
      effect->acceleration.y = y;
      effect->acceleration.z = z;
    } else if (key == MSG_SET_POS) {
      effect->position[0] = x;
      effect->position[1] = y;
      effect->position[2] = z;
    } else if (key == MSG_SET_EMITRADIUS) {
      effect->spread.x = x;
      effect->spread.y = y;
      effect->spread.z = z;
    }
    break;
  }
  case MSG_SET_COLOR: {
    const int red = (std::min)(static_cast<int>(luaL_checknumber(state, 3)), 255);
    const int green = (std::min)(static_cast<int>(luaL_checknumber(state, 4)), 255);
    const int blue = (std::min)(static_cast<int>(luaL_checknumber(state, 5)), 255);
    const int alpha = (std::min)(static_cast<int>(luaL_checknumber(state, 6)), 255);
    effect->red = red;
    effect->green = green;
    effect->blue = blue;
    effect->alpha = alpha;
    break;
  }
  case MSG_SET_EMITRATE:
  case MSG_SET_SIZE:
  case MSG_SET_LIFETIME: {
    const float minimum = static_cast<float>(luaL_checknumber(state, 3));
    const float maximum = static_cast<float>(luaL_checknumber(state, 4));
    if (key == MSG_SET_EMITRATE) {
      effect->rateMin = minimum;
      effect->rateMax = maximum;
    } else if (key == MSG_SET_SIZE) {
      effect->scaleMin = minimum;
      effect->scaleMax = maximum;
    } else if (key == MSG_SET_LIFETIME) {
      effect->lifeMin = minimum;
      effect->lifeMax = maximum;
    }
    break;
  }
  case MSG_SET_TEXTURE: {
    std::string texture = luaL_checkstring(state, 3);
    effect->SetTexture(texture);
    break;
  }
  case MSG_SET_SPEED:
    effect->SetParticleSpeed(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case MSG_SET_SRCMODE:
    effect->sourceBlend = luaL_checknumber(state, 3);
    break;
  case MSG_SET_DESTMODE:
    effect->destinationBlend = luaL_checknumber(state, 3);
    break;
  case MSG_SET_MAXPARTICLE:
    effect->SetCount(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  case MSG_SET_ZENABLE:
    effect->depth = static_cast<int>(luaL_checknumber(state, 3)) != 0;
    break;
  }

  return 0;
}

// OFFSET: 0x00B7FE50
// FUNCTION: fn_19248
int SetEZ2STRInfo(lua_State* state) {
  EffectLists* effects = g_effectMgr;
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  const double key = luaL_checknumber(state, 2);
  CEffect* effect = effects->At(1412, static_cast<unsigned>(index));
  if (!effect) {
    return 0;
  }

  switch (static_cast<int>(key)) {
  case MSG_SET_STR_FILENAME: {
    std::string name = luaL_checkstring(state, 3);
    effect->SetResource(reinterpret_cast<int>(name.c_str()));
    break;
  }
  case MSG_SET_RENDER_FLAG: {
    const unsigned int flags = static_cast<unsigned int>(luaL_checknumber(state, 3));
    effect->SetFlags(flags);
    break;
  }
  case MSG_SET_SCALE_RATIO: {
    const float scale = static_cast<float>(luaL_checknumber(state, 3));
    effect->SetScale(scale);
    break;
  }
  case MSG_SET_ALPHA_RATIO: {
    const float value = static_cast<float>(luaL_checknumber(state, 3));
    const float alpha = value >= 1.0f ? 1.0f : (std::max)(value, 0.0f);
    effect->SetAlpha(alpha);
    break;
  }
  }

  return 0;
}

// OFFSET: 0x00B80990
// FUNCTION: fn_19253
int SetWindEffectInfo(lua_State* state) {
  EffectLists* effects = g_effectMgr;
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  const double key = luaL_checknumber(state, 2);
  CWindEffect* effect = static_cast<CWindEffect*>(effects->At(EF_UNK_2343, static_cast<unsigned>(index)));
  if (!effect) {
    return 0;
  }

  switch (static_cast<int>(key)) {
  case MSG_SET_INIT:
    effect->ResetParticles();
    break;
  case MSG_SET_POS: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    effect->position[0] = x;
    effect->position[1] = y;
    effect->position[2] = z;
    break;
  }
  case MSG_SET_NUMBER:
    effect->ringCount = static_cast<unsigned short>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_COLOR: {
    const double alpha = luaL_checknumber(state, 3);
    const double red = luaL_checknumber(state, 4);
    const double green = luaL_checknumber(state, 5);
    const double blue = luaL_checknumber(state, 6);
    effect->green = static_cast<unsigned char>(static_cast<int>(green));
    effect->maxAlpha = static_cast<unsigned char>(static_cast<int>(alpha));
    effect->blue = static_cast<unsigned char>(static_cast<int>(blue));
    effect->red = static_cast<unsigned char>(static_cast<int>(red));
    break;
  }
  case MSG_SET_SIZE:
    effect->particleScale = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_RISEANGLE:
    effect->width = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_HEIGHT0:
    effect->height = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_SPEED:
    effect->angleSpeed = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_FDA:
    effect->arcLength = static_cast<unsigned short>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_ROTATE_ANGLE: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float z = static_cast<float>(luaL_checknumber(state, 4));
    effect->rotation.x = x;
    effect->rotation.y = 0.0f;
    effect->rotation.z = z;
    break;
  }
  case MSG_SET_SRCMODE:
    effect->sourceBlend = static_cast<int>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_DESTMODE:
    effect->destinationBlend = static_cast<int>(luaL_checknumber(state, 3));
    break;
  case MSG_SET_TEXTURE: {
    std::string name = luaL_checkstring(state, 3);
    std::string texture = name;
    effect->textureName = texture;
    break;
  }
  }

  return 0;
}

// OFFSET: 0x00BA2F00
// FUNCTION: fn_19675
int AddEntryQueue(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  std::string name = luaL_checkstring(state, 2);
  std::string displayName = luaL_checkstring(state, 3);
  const int minLevel = static_cast<int>(luaL_checknumber(state, 4));
  const int maxLevel = static_cast<int>(luaL_checknumber(state, 5));
  const int queueFlagA = static_cast<int>(luaL_checknumber(state, 6));
  const int queueFlagB = static_cast<int>(luaL_checknumber(state, 7));
  const int queueFlagC = static_cast<int>(luaL_checknumber(state, 8));
  const int queueFlagD = static_cast<int>(luaL_checknumber(state, 9));
  const int limitType = static_cast<int>(luaL_checknumber(state, 10));
  const int limitValue = static_cast<int>(luaL_checknumber(state, 11));
  const int limitValueEnd = static_cast<int>(luaL_checknumber(state, 12));
  std::string detailsText = luaL_checkstring(state, 13);
  std::string rulesText = luaL_checkstring(state, 14);
  std::string rewardText = luaL_checkstring(state, 15);
  std::string extraText = luaL_checkstring(state, 16);
  std::string bitmapName = luaL_checkstring(state, 17);

  EntryQueueInfo* info = new EntryQueueInfo;
  info->id = id;
  strcpy_s(info->name, sizeof(info->name), name.c_str());
  strcpy_s(info->displayName, sizeof(info->displayName), displayName.c_str());
  info->minLevel = minLevel;
  info->maxLevel = maxLevel;
  info->queueFlagA = static_cast<unsigned char>(queueFlagA);
  info->queueFlagB = static_cast<unsigned char>(queueFlagB);
  info->queueFlagC = static_cast<unsigned char>(queueFlagC);
  info->queueFlagD = static_cast<unsigned char>(queueFlagD);
  info->limitType = static_cast<unsigned char>(limitType);
  info->limitValue = limitValue;
  info->limitValueEnd = limitValueEnd;
  strcpy_s(info->description, sizeof(info->description), detailsText.c_str());
  strcpy_s(info->rulesText, sizeof(info->rulesText), rulesText.c_str());
  strcpy_s(info->rewardText, sizeof(info->rewardText), rewardText.c_str());
  strcpy_s(info->extraText, sizeof(info->extraText), extraText.c_str());
  strcpy_s(info->bitmapName, sizeof(info->bitmapName), bitmapName.c_str());

  g_entryQueueList.insert(std::make_pair(id, info));
  return 0;
}

// OFFSET: 0x00AFFF60
// FUNCTION: fn_18358
int AddIngredient(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  const int quantity = static_cast<int>(luaL_checknumber(state, 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_LAPINEBOXWND)) {
    static_cast<UILapineBoxWnd*>(window)->AddMaterial(id, quantity);
  }
  return 0;
}

// OFFSET: 0x00AFFFC0
// FUNCTION: fn_18359
int AddIngredientLapineUpgradeBox(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_LAPINEUPGRADEBOXWND)) {
    static_cast<UILapineUpgradeBoxWnd*>(window)->AddMaterial(id);
  }
  return 0;
}

// OFFSET: 0x00B00010
// FUNCTION: fn_18361
int LuaReturnZero() {
  return 0;
}

// OFFSET: 0x00B00020
// FUNCTION: fn_18362
int LuaReturnOne() {
  return 1;
}

// OFFSET: 0x00AFFB10
// FUNCTION: fn_18356
int AddCashshopBanner(lua_State* state) {
  std::string image = luaL_checkstring(state, 1);
  std::string url = luaL_checkstring(state, 2);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND)) {
    AddCashShopBanner(reinterpret_cast<CashShopBannerWindow*>(window), image, url);
  }

  return 0;
}

// OFFSET: 0x00AFF8F0
// FUNCTION: fn_18355
int AddCashshopBanner2(lua_State* state) {
  std::string image = luaL_checkstring(state, 1);
  std::string url = luaL_checkstring(state, 2);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND)) {
    AddCashShopBanner2(reinterpret_cast<CashShopBannerWindow*>(window), image, url);
  }

  return 0;
}

// OFFSET: 0x00AFFD30
// FUNCTION: fn_18357
int AddCheckAttendanceBanner(lua_State* state) {
  std::string image = luaL_checkstring(state, 1);
  std::string url = luaL_checkstring(state, 2);
  const int action = luaL_checkinteger(state, 3);
  const int argument = luaL_checkinteger(state, 4);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_CHECKATTENDANCEWND)) {
    AddAttendanceBanner(static_cast<UICheckAttendanceWnd*>(window), image, url, action, argument);
  }

  return 0;
}

// OFFSET: 0x009419B0
// FUNCTION: fn_12793
int AddItmeTokafraServiceMapList(lua_State* state) {
  std::string mapName = luaL_checkstring(state, 1);
  std::string serviceMapName = luaL_checkstring(state, 2);
  g_kafraServiceMapList[mapName].push_back(serviceMapName);
  return 0;
}

// OFFSET: 0x00941860
// FUNCTION: fn_12791
void LuaScriptNoop941860() {}

// OFFSET: 0x00941870
// FUNCTION: fn_12792
int AddItmeToExceptionMiniMapPosList(lua_State* state) {
  std::string mapName = luaL_checkstring(state, 1);
  const int x = static_cast<int>(luaL_checknumber(state, 2));
  const int y = static_cast<int>(luaL_checknumber(state, 3));
  MiniMapPosition& position = g_exceptionMiniMapPosList[mapName];
  position.x = static_cast<float>(x);
  position.y = static_cast<float>(y);
  return 0;
}

// OFFSET: 0x00933CB0
// FUNCTION: fn_12736
__declspec(noinline) void RegisterLuaKafraServiceMaps(lua_State* state) {
  lua_register(state, "AddItmeTokafraServiceMapList", AddItmeTokafraServiceMapList);
  lua_register(state, "AddItmeToExceptionMiniMapPosList", AddItmeToExceptionMiniMapPosList);
}

// OFFSET: 0x00AFC830
// FUNCTION: fn_18323
LuaScript::LuaScript() : m_state(nullptr) {
}

// OFFSET: 0x00AFC860
// FUNCTION: fn_18324
LuaScript::~LuaScript() {
  this->Close();
}

// OFFSET: 0x00AFDDB0
// FUNCTION: fn_18336
void LuaScript::Close() {
  if (this->m_state) {
    lua_close(this->m_state);
    this->m_state = nullptr;
  }
}

// INLINED
static void SetGlobalNumber(lua_State* state, const char* name, double value) {
  lua_pushnumber(state, value);
  lua_setglobal(state, name);
}

// OFFSET: 0x00AFE4B0
// FUNCTION: fn_45455
void LuaScript::Register() {
  lua_register(this->m_state, "TraceValue", TraceValue);
  lua_register(this->m_state, "SetSubChatWndList", SetSubChatWndList);
  lua_register(this->m_state, "SetSubChatWndOption", SetSubChatWndOption);
  lua_register(this->m_state, "c_SetCmdOnOffList", SetCmdOnOffList);
  lua_register(this->m_state, "c_GetCmdOnOffValue", GetCmdOnOffValue);
  lua_register(this->m_state, "c_AddSkillList", AddSkill);
  lua_register(this->m_state, "c_AddNeedSkillList", AddNeededSkill);
  lua_register(this->m_state, "RGB", LuaRGB);
  lua_register(this->m_state, "c_ChangeDirectoryOfDatas", LuaChangeDirectoryOfDatas);
  lua_register(this->m_state, "SetEmitterInfo", SetEmitterInfo);
  lua_register(this->m_state, "SetMagicFloorInfo", SetMagicFloorInfo);
  lua_register(this->m_state, "SetEvilsPawInfo", SetEvilsPawInfo);
  lua_register(this->m_state, "SetOtherEffectInfo", SetOtherEffectInfo);
  lua_register(this->m_state, "SetAnimatedEmitterInfo", SetAnimatedEmitterInfo);
  lua_register(this->m_state, "SetEZ2STRInfo", SetEZ2STRInfo);
  lua_register(this->m_state, "SetWindEffectInfo", SetWindEffectInfo);
  RegisterLuaKafraServiceMaps(this->m_state);
  lua_register(this->m_state, "AddEntryQueue", AddEntryQueue);
  lua_register(this->m_state, "add_ingredient", AddIngredient);
  lua_register(this->m_state, "add_ingredient_lapine_upgrade_box", AddIngredientLapineUpgradeBox);
  lua_register(this->m_state, "add_cashshop_banner", AddCashshopBanner);
  lua_register(this->m_state, "add_cashshop_banner2", AddCashshopBanner2);
  lua_register(this->m_state, "add_checkAttendance_banner", AddCheckAttendanceBanner);
  SetGlobalNumber(this->m_state, "SKFLAG_NOREDUCT", 4.0);
  SetGlobalNumber(this->m_state, "SKFLAG_DISABLE_FIXEDCASTING_REDUCTION", 4096.0);
}

// OFFSET: 0x00AFDDD0
// FUNCTION: fn_18337
void LuaScript::Open() {
  if (this->m_state) {
    lua_close(this->m_state);
    this->m_state = nullptr;
  }

  this->m_state = luaL_newstate();
  if (!this->m_state) {
    return;
  }

  luaL_openlibs(this->m_state);
  return this->Register();
}

// OFFSET: 0x00AFDD60
// FUNCTION: fn_18335
bool LuaScript::Execute(const void* bytes, std::size_t size) {
  return this->m_state && luaL_loadbuffer(this->m_state, static_cast<const char*>(bytes), size, "buf") == 0 && lua_pcall(this->m_state, 0, 0, 0) == 0;
}

// OFFSET: 0x00AFDE60
// FUNCTION: fn_18338
bool LuaScript::Load(const char* name, bool resourcePath, bool looseOnly) {
  if (!this->m_state) {
    return false;
  }

  std::string prefix;
  if (!looseOnly) {
    prefix = std::string("LuaFiles514\\");
  }

  std::string path = prefix + std::string(name) + ".lub";

  if (resourcePath) {
    if (!ResourceExists(path.c_str(), looseOnly)) {
      path = prefix + std::string(name) + ".lua";
      if (!ResourceExists(path.c_str(), looseOnly)) {
        return false;
      }
    }
  } else {
    if (!g_pakRegistry.Exists(path.c_str(), looseOnly)) {
      path = prefix + std::string(name) + ".lua";
      if (!g_pakRegistry.Exists(path.c_str(), looseOnly)) {
        return false;
      }
    }
  }

  CFile file;
  if (!file.Open(path.c_str(), 0, resourcePath, looseOnly)) {
    return false;
  }

  const std::uint32_t size = file.GetSize();
  char* buffer = new char[size + 1];
  file.Read(buffer, size);
  buffer[size] = '\0';
  file.Close();

  if (luaL_loadbuffer(this->m_state, buffer, size, "buf") != 0 || lua_pcall(this->m_state, 0, 0, 0) != 0) {
    std::string error = lua_tostring(this->m_state, -1);
    this->Error(name, "%s\n", error.c_str());
    lua_settop(this->m_state, -2);
    delete[] buffer;
    return false;
  }

  delete[] buffer;
  return true;
}

// OFFSET: 0x00AFD1D0
// FUNCTION: fn_18328
void LuaScript::Error(const char* caption, const char* format, ...) {
  char text[500];
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf_s(text, sizeof(text), _TRUNCATE, format, arguments);
  va_end(arguments);
  MessageBoxA(nullptr, text, caption, MB_OK);
}

// OFFSET: 0x00AFDC60
// FUNCTION: fn_18334
bool LuaScript::TableField(const char* signature, ...) {
  va_list arguments;
  va_start(arguments, signature);

  if (*signature == 'd') {
    lua_pushnumber(this->m_state, va_arg(arguments, int));
    lua_gettable(this->m_state, -2);
  } else if (*signature == 's') {
    lua_pushstring(this->m_state, va_arg(arguments, const char*));
    lua_gettable(this->m_state, -2);
  }

  ++signature;

  if (*signature == 'd' && lua_isnumber(this->m_state, -1)) {
    *va_arg(arguments, int*) = static_cast<int>(lua_tonumber(this->m_state, -1));
    lua_pop(this->m_state, 1);
  }

  if (*signature == 's' && lua_isstring(this->m_state, -1)) {
    *va_arg(arguments, const char**) = lua_tostring(this->m_state, -1);
    lua_pop(this->m_state, 1);
  }

  if (*signature == 'l' && lua_isnumber(this->m_state, -1)) {
    *va_arg(arguments, double*) = lua_tonumber(this->m_state, -1);
    lua_pop(this->m_state, 1);
  }

  va_end(arguments);
  return true;
}

// OFFSET: 0x00AFC9A0
// FUNCTION: fn_18326
bool LuaScript::Call(std::string function, const char* signature, ...) {
  if (!this->m_state) {
    return false;
  }

  va_list arguments;
  va_start(arguments, signature);
  lua_getglobal(this->m_state, function.c_str());
  int inputCount = 0;

  while (*signature) {
    luaL_checkstack(this->m_state, 1, "too many arguments");
    switch (*signature++) {
    case 'b':
      lua_pushboolean(this->m_state, static_cast<unsigned char>(va_arg(arguments, int)));
      break;
    case 'd':
      lua_pushnumber(this->m_state, va_arg(arguments, int));
      break;
    case 'l':
      lua_pushnumber(this->m_state, va_arg(arguments, double));
      break;
    case 's':
      lua_pushstring(this->m_state, va_arg(arguments, const char*));
      break;
    case '>':
      goto endwhile;
    default:
      this->Error(function.c_str(), "invalid option (%c)", signature[-1]);
      break;
    }

    ++inputCount;
  }
endwhile:

  int outputCount = static_cast<int>(std::strlen(signature));
  if (lua_pcall(this->m_state, inputCount, outputCount, 0)) {
    std::string error = lua_tostring(this->m_state, -1);
    this->Error(function.c_str(), "%s\n", error.c_str());
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }

    va_end(arguments);
    return false;
  }

  if (outputCount) {
    for (int output = -outputCount; *signature; ++output) {
      switch (*signature++) {
      case 'b':
        *va_arg(arguments, bool*) = lua_toboolean(this->m_state, output) != 0;
        break;
      case 'd':
        *va_arg(arguments, int*) = static_cast<int>(lua_tonumber(this->m_state, output));
        break;
      case 'l':
        *va_arg(arguments, double*) = lua_tonumber(this->m_state, output);
        break;
      case 's':
        *va_arg(arguments, const char**) = lua_tostring(this->m_state, output);
        break;
      default:
        this->Error(function.c_str(), "invalid option (%c)", signature[-1]);
        break;
      }
    }

    lua_pop(this->m_state, outputCount);
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }
  }

  va_end(arguments);
  return true;
}

// OFFSET: 0x00AFCD90
// FUNCTION: fn_18327
bool LuaScript::CallTable(std::string table, std::string function, const char* signature, ...) {
  if (!this->m_state) {
    return false;
  }

  va_list arguments;
  va_start(arguments, signature);

  lua_getglobal(this->m_state, table.c_str());
  lua_getfield(this->m_state, -1, function.c_str());

  int inputCount = 0;

  while (*signature) {
    luaL_checkstack(this->m_state, 1, "too many arguments");
    switch (*signature++) {
    case 'b':
      lua_pushboolean(this->m_state, static_cast<unsigned char>(va_arg(arguments, int)));
      break;
    case 'd':
      lua_pushnumber(this->m_state, va_arg(arguments, int));
      break;
    case 'l':
      lua_pushnumber(this->m_state, va_arg(arguments, double));
      break;
    case 's':
      lua_pushstring(this->m_state, va_arg(arguments, const char*));
      break;
    case '>':
      goto endwhile;
    default:
      this->Error(function.c_str(), "invalid option (%c)", signature[-1]);
      break;
    }

    ++inputCount;
  }
endwhile:

  int outputCount = static_cast<int>(std::strlen(signature));
  if (lua_pcall(this->m_state, inputCount, outputCount, 0)) {
    std::string error = lua_tostring(this->m_state, -1);
    this->Error(function.c_str(), "%s\n", error.c_str());
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }

    va_end(arguments);
    return false;
  }

  if (outputCount) {
    for (int output = -outputCount; *signature; ++output) {
      switch (*signature++) {
      case 'b':
        *va_arg(arguments, bool*) = lua_toboolean(this->m_state, output) != 0;
        break;
      case 'd':
        *va_arg(arguments, int*) = static_cast<int>(lua_tonumber(this->m_state, output));
        break;
      case 'l':
        *va_arg(arguments, double*) = lua_tonumber(this->m_state, output);
        break;
      case 's':
        *va_arg(arguments, const char**) = lua_tostring(this->m_state, output);
        break;
      default:
        this->Error(function.c_str(), "invalid option (%c)", signature[-1]);
        break;
      }
    }

    lua_settop(this->m_state, -1 - outputCount);
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }
  }

  va_end(arguments);
  return true;
}

// OFFSET: 0x00AFD2A0
// FUNCTION: fn_18329
int LuaScript::GlobalInteger(const char* name) {
  if (!this->m_state) {
    return 0;
  }

  lua_getglobal(this->m_state, name);
  if (!lua_isnumber(this->m_state, -1)) {
    return -1;
  }

  int value = static_cast<int>(lua_tonumber(this->m_state, -1));
  lua_pop(this->m_state, 1);
  return value;
}

// OFFSET: 0x00AFD310
// FUNCTION: fn_18330
std::string LuaScript::GlobalString(const char* name) {
  if (!this->m_state) {
    return "";
  }

  lua_getglobal(this->m_state, name);
  if (!lua_isstring(this->m_state, -1)) {
    return "";
  }

  std::string value = lua_tostring(this->m_state, -1);
  lua_pop(this->m_state, 1);
  return value;
}

// OFFSET: 0x00AFD490
// FUNCTION: fn_18331
bool LuaScript::HasTable(const char* name) {
  if (this->m_state) {
    lua_getglobal(this->m_state, name);
    if (lua_type(this->m_state, -1) == LUA_TTABLE) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00AFD4D0
// FUNCTION: fn_18332
std::list<int> LuaScript::TableKeys(std::string_view name) {
  std::list<int> keys;

  if (!this->m_state) {
    return keys;
  }

  lua_getglobal(this->m_state, name.data());

  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return keys;
  }

  lua_pushnil(this->m_state);

  while (lua_next(this->m_state, -2)) {
    keys.push_back(lua_tointeger(this->m_state, -2));
    lua_pop(this->m_state, 1);
  }

  lua_pop(this->m_state, 1);
  keys.sort();
  return keys;
}

// OFFSET: 0x00AFD830
// FUNCTION: fn_18333
int LuaScript::TableInteger(std::string table, int keyType, std::intptr_t key) {
  if (!this->m_state) {
    return 0;
  }

  int value = -1;
  lua_getglobal(this->m_state, table.c_str());

  if (lua_type(this->m_state, -1) == LUA_TTABLE) {
    std::string signature;

    if (keyType == 0) {
      signature = "d";
    } else if (keyType == 1) {
      signature = "s";
    } else if (keyType == 2) {
      signature = "l";
    }

    signature += "d";
    this->TableField(signature.c_str(), key, &value);
  }

  return value;
}

// OFFSET: 0x0059CD60
// FUNCTION: fn_02980
void LuaScript::Register(std::string name, int (*function)(lua_State*)) {
  lua_register(this->m_state, name.c_str(), function);
}

// OFFSET: 0x00A91200
// FUNCTION: fn_17157
int UIWindowMgr::GetHotkeyBehavior() {
  int behavior = -1;
  const int mode = static_cast<int>(g_session.m_options.Get(10));
  g_session.m_hotkeyScript->Call("GetBehaviorOfHotKey2", "ddd>d", mode, this->keyCode, this->modifierKey, &behavior);
  if (behavior != -1) {
    return behavior;
  }

  if (this->modifierKey == VK_SHIFT && !g_windowMgr.inputSuggestionsEnabled) {
    UIWindow* parent = this->hoverWindow ? this->hoverWindow->parent : nullptr;
    if (!parent || parent == this->chatWnd || parent->geometry.visible == 0) {
      const int count = HotkeyCount(*g_session.m_hotkeyScript, 0);
      for (int index = 0; index < count; ++index) {
        const HotkeyInfo hotkey = GetHotkeyInfo(*g_session.m_hotkeyScript, 0, index);
        if (hotkey.key1 == this->keyCode && g_session.m_shortcutSlots[index].enabled != 0) {
          return index;
        }
      }
    }
  }

  return -1;
}
