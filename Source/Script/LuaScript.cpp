#include "Script/LuaScript.h"
#include "Archive/PakRegistry.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/EmitterEffect.h"
#include "Contents/Effects/EvilsPawEffect.h"
#include "Contents/Effects/WindEffect.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Script/LuaFileOperations.h"
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

// GLOBAL: gl_24791
LuaScript* g_luaScript;

// GLOBAL: gl_24913
std::map<std::string, MiniMapPosition> g_exceptionMiniMapPosList;

// GLOBAL: gl_24914
std::map<std::string, std::vector<std::string>> g_kafraServiceMapList;

struct LuaScriptPair {
  std::string left;
  std::string right;
};

class LuaScriptBag {
public:
  void Add(std::string left, std::string right);

  /* 0000 */ unsigned char unknown_0000[0x7C];
  /* 007C */ std::vector<LuaScriptPair> pairs;
};

// OFFSET: 0x00AFC8B0
// FUNCTION: fn_18325
void LuaScriptBag::Add(std::string left, std::string right) {
  this->pairs.push_back(LuaScriptPair{left, right});
}

struct CashShopBannerWindow {
  /* 0000 */ unsigned char unknown_0000[0xBC];
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

int AddSkill(lua_State* state);
int AddNeededSkill(lua_State* state);

// OFFSET: 0x00AFF7E0
// FUNCTION: fn_18354
int TraceValue(lua_State* state) {
  std::string text = luaL_checkstring(state, 1);
  MessageBoxA(nullptr, text.c_str(), "Lua Error", 0);
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
  if (type == "No") {
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

  if (type == "No") {
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

// OFFSET: 0x00AFF020
// FUNCTION: fn_18351
int SetCmdOnOffList(lua_State* state) {
  const char* name = luaL_checkstring(state, 1);
  const bool enabled = luaL_checknumber(state, 2) != 0.0;
  Singleton<CGameSettingsMgr>::GetInstance();
  const int optionId = SetOption(name, enabled);
  if (optionId != 3) {
    if (optionId == 229) {
      const int value = enabled;
      std::memcpy(g_session.unknown_5454, &value, sizeof(value));
    }
    g_session.m_commandOptions.emplace_back(name, optionId);
  }
  return 0;
}

// OFFSET: 0x00AFEF50
// FUNCTION: fn_18350
int GetCmdOnOffValue(lua_State* state) {
  const char* name = luaL_checkstring(state, 1);
  Singleton<CGameSettingsMgr>::GetInstance();
  const auto option = FindServiceOption(name);
  lua_pushnumber(state, option.first == 3 ? -1.0 : static_cast<double>(option.second != 0));
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
  case 0:
    effect->Slot07();
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    if (key == 1) {
      effect->velocityMin.x = x;
      effect->velocityMin.y = y;
      effect->velocityMin.z = z;
    } else if (key == 2) {
      effect->velocityMax.x = x;
      effect->velocityMax.y = y;
      effect->velocityMax.z = z;
    } else if (key == 3) {
      effect->acceleration.x = x;
      effect->acceleration.y = y;
      effect->acceleration.z = z;
    } else if (key == 4) {
      effect->position[0] = x;
      effect->position[1] = y;
      effect->position[2] = z;
    } else if (key == 5) {
      effect->spread.x = x;
      effect->spread.y = y;
      effect->spread.z = z;
    }
    break;
  }
  case 6: {
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
  case 7:
  case 8:
  case 9: {
    const float minimum = static_cast<float>(luaL_checknumber(state, 3));
    const float maximum = static_cast<float>(luaL_checknumber(state, 4));
    if (key == 7) {
      effect->rateMin = minimum;
      effect->rateMax = maximum;
    } else if (key == 8) {
      effect->scaleMin = minimum;
      effect->scaleMax = maximum;
    } else if (key == 9) {
      effect->lifeMin = minimum;
      effect->lifeMax = maximum;
    }
    break;
  }
  case 10: {
    std::string texture = luaL_checkstring(state, 3);
    effect->SetTexture(texture);
    break;
  }
  case 11:
    effect->SetParticleSpeed(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case 12:
    effect->sourceBlend = luaL_checknumber(state, 3);
    break;
  case 13:
    effect->destinationBlend = luaL_checknumber(state, 3);
    break;
  case 14:
    effect->SetCount(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  case 43:
    effect->depth = static_cast<int>(luaL_checknumber(state, 3)) != 0;
    break;
  case 45:
    effect->billboard = static_cast<int>(luaL_checknumber(state, 3)) != 0;
    break;
  case 46: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    effect->rotation[0] = x;
    effect->rotation[1] = y;
    effect->rotation[2] = z;
    break;
  }
  case 51:
    effect->fixedAlpha = static_cast<int>(luaL_checknumber(state, 3)) == 1;
    break;
  case 52: {
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
  case 0:
    effect->Slot07();
    break;
  case 16:
    *reinterpret_cast<float*>(&effect->unknown_003C[8]) = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case 42:
    reinterpret_cast<float&>(effect->unknown_0048) = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case 17:
    *reinterpret_cast<int*>(effect->unknown_004C) = luaL_checknumber(state, 3);
    break;
  case 18:
    reinterpret_cast<float&>(effect->unknown_0050) = static_cast<float>(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  case 19:
    *reinterpret_cast<int*>(&effect->unknown_0054[0]) = luaL_checknumber(state, 3);
    break;
  case 20:
    *reinterpret_cast<int*>(&effect->unknown_0054[4]) = luaL_checknumber(state, 3);
    break;
  default:
    if (key >= 21 && key <= 41) {
      reinterpret_cast<float*>(&effect->unknown_0054[8])[key - 21] = static_cast<float>(static_cast<int>(luaL_checknumber(state, 3)));
    }
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
  case 0:
    effect->Slot07();
    break;
  case 8:
    effect->size = static_cast<int>(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case 11:
    effect->frameDelay = static_cast<int>(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case 44: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    effect->offset.x = x;
    effect->offset.y = y;
    effect->offset.z = z;
    break;
  }
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
  case 0:
    effect->Slot07();
    break;
  case 16:
    *reinterpret_cast<float*>(&effect->unknown_003C[8]) = static_cast<float>(luaL_checknumber(state, 4));
    break;
  case 42:
    reinterpret_cast<float&>(effect->unknown_0048) = static_cast<float>(luaL_checknumber(state, 4));
    break;
  case 17:
    *reinterpret_cast<int*>(effect->unknown_004C) = luaL_checknumber(state, 4);
    break;
  case 18:
    reinterpret_cast<float&>(effect->unknown_0050) = static_cast<float>(static_cast<int>(luaL_checknumber(state, 4)));
    break;
  case 19:
    *reinterpret_cast<int*>(&effect->unknown_0054[0]) = luaL_checknumber(state, 4);
    break;
  case 20:
    *reinterpret_cast<int*>(&effect->unknown_0054[4]) = luaL_checknumber(state, 4);
    break;
  default:
    if (key >= 21 && key <= 41) {
      reinterpret_cast<float*>(&effect->unknown_0054[8])[key - 21] = static_cast<float>(static_cast<int>(luaL_checknumber(state, 4)));
    }
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
  case 0:
    effect->Slot07();
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    if (key == 1) {
      effect->velocityMin.x = x;
      effect->velocityMin.y = y;
      effect->velocityMin.z = z;
    } else if (key == 2) {
      effect->velocityMax.x = x;
      effect->velocityMax.y = y;
      effect->velocityMax.z = z;
    } else if (key == 3) {
      effect->acceleration.x = x;
      effect->acceleration.y = y;
      effect->acceleration.z = z;
    } else if (key == 4) {
      effect->position[0] = x;
      effect->position[1] = y;
      effect->position[2] = z;
    } else if (key == 5) {
      effect->spread.x = x;
      effect->spread.y = y;
      effect->spread.z = z;
    }
    break;
  }
  case 6: {
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
  case 7:
  case 8:
  case 9: {
    const float minimum = static_cast<float>(luaL_checknumber(state, 3));
    const float maximum = static_cast<float>(luaL_checknumber(state, 4));
    if (key == 7) {
      effect->rateMin = minimum;
      effect->rateMax = maximum;
    } else if (key == 8) {
      effect->scaleMin = minimum;
      effect->scaleMax = maximum;
    } else if (key == 9) {
      effect->lifeMin = minimum;
      effect->lifeMax = maximum;
    }
    break;
  }
  case 10: {
    std::string texture = luaL_checkstring(state, 3);
    effect->SetTexture(texture);
    break;
  }
  case 11:
    effect->SetParticleSpeed(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case 12:
    effect->sourceBlend = luaL_checknumber(state, 3);
    break;
  case 13:
    effect->destinationBlend = luaL_checknumber(state, 3);
    break;
  case 14:
    effect->SetCount(static_cast<int>(luaL_checknumber(state, 3)));
    break;
  case 43:
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
  case 47: {
    std::string name = luaL_checkstring(state, 3);
    effect->Slot20(reinterpret_cast<int>(name.c_str()));
    break;
  }
  case 48:
    effect->Slot21(static_cast<unsigned int>(luaL_checknumber(state, 3)));
    break;
  case 49:
    effect->Slot22(static_cast<float>(luaL_checknumber(state, 3)));
    break;
  case 50: {
    const float alpha = static_cast<float>(luaL_checknumber(state, 3));
    effect->Slot23(alpha >= 1.0f ? 1.0f : (std::max)(0.0f, alpha));
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
  case 0:
    effect->Slot07();
    break;
  case 4: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float y = static_cast<float>(luaL_checknumber(state, 4));
    const float z = static_cast<float>(luaL_checknumber(state, 5));
    effect->position[0] = x;
    effect->position[1] = y;
    effect->position[2] = z;
    break;
  }
  case 53:
    effect->ringCount = static_cast<unsigned short>(luaL_checknumber(state, 3));
    break;
  case 6: {
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
  case 8:
    effect->particleScale = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case 18:
    effect->width = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case 21:
    effect->height = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case 11:
    effect->angleSpeed = static_cast<float>(luaL_checknumber(state, 3));
    break;
  case 20:
    effect->arcLength = static_cast<unsigned short>(luaL_checknumber(state, 3));
    break;
  case 46: {
    const float x = static_cast<float>(luaL_checknumber(state, 3));
    const float z = static_cast<float>(luaL_checknumber(state, 4));
    effect->rotation.x = x;
    effect->rotation.y = 0.0f;
    effect->rotation.z = z;
    break;
  }
  case 12:
    effect->sourceBlend = static_cast<int>(luaL_checknumber(state, 3));
    break;
  case 13:
    effect->destinationBlend = static_cast<int>(luaL_checknumber(state, 3));
    break;
  case 10: {
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
  const int value020C = static_cast<int>(luaL_checknumber(state, 4));
  const int value0210 = static_cast<int>(luaL_checknumber(state, 5));
  const int value0214 = static_cast<int>(luaL_checknumber(state, 6));
  const int value0215 = static_cast<int>(luaL_checknumber(state, 7));
  const int value0216 = static_cast<int>(luaL_checknumber(state, 8));
  const int value0217 = static_cast<int>(luaL_checknumber(state, 9));
  const int value0218 = static_cast<int>(luaL_checknumber(state, 10));
  const int value021C = static_cast<int>(luaL_checknumber(state, 11));
  const int value0220 = static_cast<int>(luaL_checknumber(state, 12));
  std::string text0224 = luaL_checkstring(state, 13);
  std::string text0328 = luaL_checkstring(state, 14);
  std::string text042C = luaL_checkstring(state, 15);
  std::string text0530 = luaL_checkstring(state, 16);
  std::string text0634 = luaL_checkstring(state, 17);

  EntryQueueInfo* info = new EntryQueueInfo;
  info->id = id;
  strcpy_s(info->name, sizeof(info->name), name.c_str());
  strcpy_s(info->displayName, sizeof(info->displayName), displayName.c_str());
  info->unknown_020C = value020C;
  info->unknown_0210 = value0210;
  info->unknown_0214 = static_cast<unsigned char>(value0214);
  info->unknown_0215 = static_cast<unsigned char>(value0215);
  info->unknown_0216 = static_cast<unsigned char>(value0216);
  info->unknown_0217 = static_cast<unsigned char>(value0217);
  info->unknown_0218 = static_cast<unsigned char>(value0218);
  info->unknown_021C = value021C;
  info->unknown_0220 = value0220;
  strcpy_s(info->unknown_0224, sizeof(info->unknown_0224), text0224.c_str());
  strcpy_s(info->unknown_0328, sizeof(info->unknown_0328), text0328.c_str());
  strcpy_s(info->unknown_042C, sizeof(info->unknown_042C), text042C.c_str());
  strcpy_s(info->unknown_0530, sizeof(info->unknown_0530), text0530.c_str());
  strcpy_s(info->unknown_0634, sizeof(info->unknown_0634), text0634.c_str());

  g_entryQueueList.insert(std::make_pair(id, info));
  return 0;
}

// OFFSET: 0x00AFFF60
// FUNCTION: fn_18358
int AddIngredient(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  const int quantity = static_cast<int>(luaL_checknumber(state, 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(0x122)) {
    static_cast<UILapineBoxWnd*>(window)->AddMaterial(id, quantity);
  }
  return 0;
}

// OFFSET: 0x00AFFFC0
// FUNCTION: fn_18359
int AddIngredientLapineUpgradeBox(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  if (UIWindow* window = g_windowMgr.LookupWindow(0x12e)) {
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
  if (UIWindow* window = g_windowMgr.LookupWindow(0x13e)) {
    AddCashShopBanner(reinterpret_cast<CashShopBannerWindow*>(window), image, url);
  }

  return 0;
}

// OFFSET: 0x00AFF8F0
// FUNCTION: fn_18355
int AddCashshopBanner2(lua_State* state) {
  std::string image = luaL_checkstring(state, 1);
  std::string url = luaL_checkstring(state, 2);
  if (UIWindow* window = g_windowMgr.LookupWindow(0x13e)) {
    AddCashShopBanner2(reinterpret_cast<CashShopBannerWindow*>(window), image, url);
  }

  return 0;
}

// OFFSET: 0x00AFFD30
// FUNCTION: fn_18357
int AddCheckAttendanceBanner(lua_State* state) {
  std::string image = luaL_checkstring(state, 1);
  std::string url = luaL_checkstring(state, 2);
  const int action = lua_toboolean(state, 3);
  const int argument = lua_toboolean(state, 4);
  if (UIWindow* window = g_windowMgr.LookupWindow(0x148)) {
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

// OFFSET: 0x00AFE4B0
// FUNCTION: fn_45455
void LuaScript::Register() {
  lua_State* state = this->m_state;
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
  lua_pushnumber(state, 4.0);
  lua_setglobal(state, "SKFLAG_NOREDUCT");
  lua_pushnumber(state, 4096.0);
  lua_setglobal(state, "SKFLAG_DISABLE_FIXEDCASTING_REDUCTION");
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
    prefix = "LuaFiles514\\";
  }

  std::string path = prefix + name;
  path += ".lub";

  if (resourcePath) {
    if (!ResourceExists(path.c_str(), looseOnly)) {
      path = prefix + name + ".lua";
      if (!ResourceExists(path.c_str(), looseOnly)) {
        return false;
      }
    }
  } else {
    if (!g_pakRegistry.Exists(path.c_str(), looseOnly)) {
      path = prefix + name + ".lua";
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
    char option = *signature++;
    if (option == '>') {
      break;
    }

    switch (option) {
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
    default:
      this->Error(function.c_str(), "invalid option (%c)", option);
      break;
    }

    ++inputCount;
  }

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

  if (outputCount) {
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

    char option = *signature++;
    if (option == '>') {
      break;
    }

    switch (option) {
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
    default:
      this->Error(function.c_str(), "invalid option (%c)", option);
      break;
    }

    ++inputCount;
  }

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

  if (outputCount) {
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
    return {};
  }

  lua_getglobal(this->m_state, name);
  if (!lua_isstring(this->m_state, -1)) {
    return {};
  }

  std::string value = lua_tostring(this->m_state, -1);
  lua_pop(this->m_state, 1);
  return value;
}

// OFFSET: 0x00AFD490
// FUNCTION: fn_18331
bool LuaScript::HasTable(const char* name) {
  if (!this->m_state) {
    return false;
  }

  lua_getglobal(this->m_state, name);

  return lua_type(this->m_state, -1) == LUA_TTABLE;
}

// OFFSET: 0x00AFD4D0
// FUNCTION: fn_18332
std::list<int> LuaScript::TableKeys(const char* name, int) {
  std::list<int> keys;

  if (!this->m_state) {
    return keys;
  }

  lua_getglobal(this->m_state, name);

  if (lua_type(this->m_state, -1) == LUA_TTABLE) {
    lua_pushnil(this->m_state);

    while (lua_next(this->m_state, -2)) {
      keys.push_back(static_cast<int>(lua_tonumber(this->m_state, -2)));
      lua_pop(this->m_state, 1);
    }

    lua_pop(this->m_state, 1);
    keys.sort();
  }

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

  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return value;
  }

  if (keyType == 0) {
    lua_pushnumber(this->m_state, static_cast<int>(key));
    lua_gettable(this->m_state, -2);
  } else if (keyType == 1) {
    lua_pushstring(this->m_state, reinterpret_cast<const char*>(key));
    lua_gettable(this->m_state, -2);
  } else if (keyType != 2) {
    lua_pushnumber(this->m_state, static_cast<int>(key));
    lua_gettable(this->m_state, -2);
    return value;
  }

  if (lua_isnumber(this->m_state, -1)) {
    value = static_cast<int>(lua_tonumber(this->m_state, -1));
    lua_pop(this->m_state, 1);
  }

  return value;
}

// OFFSET: 0x0059CD60
// FUNCTION: fn_02980
void LuaScript::Register(std::string name, int (*function)(lua_State*)) {
  lua_register(this->m_state, name.c_str(), function);
}
