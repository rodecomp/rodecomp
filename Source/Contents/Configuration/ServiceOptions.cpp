#include "Contents/Configuration/ServiceOptions.h"
#include "Utility/EncryptedName.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/SkinMgr.h"
#include "File/File.h"
#include "Contents/Sound/SoundMgr.h"
#include "File/ResourcePath.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourcePreload.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Item/UIItemWnd.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/World/World.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Network/ClientOptionPacket.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Script/LuaInstance.h"
#include "Script/LuaScript.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Singleton.h"
#include "Utility/Text/StringUtil.h"

extern "C" {
#include <lua.h>
}

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <functional>
#include <list>
#include <optional>
#include <utility>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

struct GameSettingsOption {
  /* 0000 */ int id = 300;
  /* 0004 */ int tab;
  /* 0008 */ int type;
  /* 000C */ std::string title;
  /* 0024 */ std::string tooltip;
  /* 003C */ int tipBoxId = 0;
  /* 0040 */ std::string description;
  /* 0058 */ unsigned char defaultValue;
  /* 0059 */ unsigned char unknown_0059[3];
  /* 005C */ int unknown_005c = 4698;
  /* 0060 */ int unknown_0060 = 4698;
};

class CGameSettingsMgr : public Singleton<CGameSettingsMgr>, public CLuaInstance {
public:
  CGameSettingsMgr();
  ~CGameSettingsMgr() override;

  /* 000C */ std::vector<GameSettingsOption> options;
  /* 0018 */ std::unordered_map<std::uint32_t, std::function<void(int, unsigned char, bool, int, int)>> actions;
  /* 0038 */ std::unordered_map<std::uint32_t, std::function<void()>> functions;

  bool LoadSettings();
  bool ReadOptionTable();
  void ResetToDefaults();
  void ToggleOption(int id);
  void SetOption(int id, unsigned char value, bool notify);
  void ApplyScriptOption(int id, const char* key);
  bool RunFunction(int id);
  std::optional<GameSettingsOption*> FindOption(int id);
  std::list<int> GetOptionIds(int tab);

private:
  void Init_Func();
};

using GameSettings = CGameSettingsMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GameSettingsOption) == 0x64);
static_assert(offsetof(GameSettingsOption, defaultValue) == 0x58);
static_assert(offsetof(GameSettings, options) == 0x0C);
static_assert(sizeof(GameSettings) == 0x58);
#endif

// GLOBAL: gl_24579
ServiceOptions g_serviceOptions;

// GLOBAL: gl_24569
const float g_jitterDistanceScale = 400.0f;

// GLOBAL: gl_25022
float g_serviceOptionJitterScale = 400.0f;

// GLOBAL: gl_25051
const char* const g_serviceOptionNames[300] = {
    "TT_NORMAL",
    "TT_BROADCAST",
    "TT_STAT",
    "TT_UNKNOWN",
    "TT_DISCONNECT_CHARACTER",
    "TT_DISCONNECT_ALL_CHARACTER",
    "TT_REQ_NUM_USER",
    "TT_MUSIC_ON_OFF",
    "TT_EFFECT_ON_OFF",
    "TT_EX_MAP_POS",
    "TT_FRAMESKIP_ON_OFF",
    "TT_EFFECT_SOUND_ON_OFF",
    "TT_VOLUME",
    "TT_BVOLUME",
    "TT_EX_HELP",
    "TT_EX_WHISPER_BLOCK_LIST",
    "TT_EX_WHISPER_ALL_BLOCK",
    "TT_EX_WHISPER_ALL_ACCEPT",
    "TT_REQ_WHISPER_PC_EX",
    "TT_REQ_WHISPER_PC_IN",
    "TT_IS_REDCELL",
    "TT_UNUSED_EX_STAND",
    "TT_EX_SIT",
    "TT_EX_MAKE_CHAT_ROOM",
    "TT_EX_EXIT_CHAT_ROOM",
    "TT_REQ_EXCHANGE_ITEM",
    "TT_REQ_MAKE_GROUP",
    "TT_EX_LEAVE_GROUP",
    "TT_REQ_EXPEL_GROUP_MEMBER",
    "TT_REQ_EMOTION_SURPRISE",
    "TT_REQ_EMOTION_QUESTION",
    "TT_REQ_EMOTION_DELIGHT",
    "TT_REQ_EMOTION_THROB",
    "TT_REQ_EMOTION_SWEAT",
    "TT_REQ_EMOTION_AHA",
    "TT_REQ_EMOTION_FRET",
    "TT_REQ_EMOTION_ANGER",
    "TT_REQ_EMOTION_MONEY",
    "TT_REQ_EMOTION_THINK",
    "TT_REQ_EMOTION_SCISSOR",
    "TT_REQ_EMOTION_ROCK",
    "TT_REQ_EMOTION_WRAP",
    "TT_OPEN_TIP",
    "TT_EX_REMEMBER_WARPPOINT",
    "TT_REQ_PK",
    "TT_SKILL_SNAP_ON_OFF",
    "TT_ATTACK_SNAP_ON_OFF",
    "TT_ITEM_SNAP_ON_OFF",
    "TT_REQ_ITEM_CREATE",
    "TT_REQ_MOVETO_MAP",
    "TT_NORMAL_FROM_CHATROOM",
    "TT_EX_SAVE_CHAT",
    "TT_REQ_SAVE_CHAT_FROM_CHATROOM",
    "TT_MISS_EFFECT_ON_OFF",
    "TT_FIXED_CAMERA_ON_OFF",
    "TT_REQ_MAKE_GUILD",
    "TT_REQ_BREAK_GUILD",
    "TT_GUILD_CHAT",
    "TT_TEST_SHOW_IMAGE",
    "TT_TESTPACKET",
    "TT_FOG_ON_OFF",
    "TT_REQ_RESETSTATE",
    "TT_REQ_RESETSKILL",
    "TT_CHANGE_MAPTYPE",
    "TT_SHOW_EMBLEM_ON_OFF",
    "TT_TOGGLE_OBJLIGHT",
    "TT_REQ_EMOTION_THANKS",
    "TT_REQ_EMOTION_KEK",
    "TT_REQ_EMOTION_SORRY",
    "TT_REQ_EMOTION_SMILE",
    "TT_REQ_EMOTION_PROFUSELY_SWEAT",
    "TT_REQ_EMOTION_SCRATCH",
    "TT_REQ_EMOTION_BEST",
    "TT_REQ_EMOTION_STARE_ABOUT",
    "TT_REQ_EMOTION_HUK",
    "TT_REQ_EMOTION_O",
    "TT_REQ_EMOTION_X",
    "TT_REQ_EMOTION_HELP",
    "TT_REQ_EMOTION_GO",
    "TT_REQ_EMOTION_BIGTHROB",
    "TT_REQ_EMOTION_CRY",
    "TT_REQ_EMOTION_KIK",
    "TT_REQ_EMOTION_CHUP",
    "TT_REQ_EMOTION_CHUPCHUP",
    "TT_REQ_EMOTION_HNG",
    "TT_REQ_EMOTION_OK",
    "TT_SHOW_EMOTIONLIST",
    "TT_LOCALBROADCAST",
    "TT_HIDE",
    "TT_UNUSED_NAME_BALLOON",
    "TT_NAMELESS_BROADCAST",
    "TT_NAMELESS_LOCALBROADCAST",
    "TT_NAME_ABOVE_HEAD_ON_OFF",
    "TT_SAVEMONEY",
    "TT_LOADMONEY",
    "TT_REMOVE",
    "TT_SHIFT",
    "TT_RECALL_AID",
    "TT_RECALL_GID",
    "TT_FORCING_MOVE_AID",
    "TT_FORCING_MOVE_GID",
    "TT_BILLING_INFO",
    "TT_NAMELESS_BLUEBROADCAST",
    "TT_NAMELESS_BLUELOCALBROADCAST",
    "TT_REQ_AUTO_SAVE_CHAT",
    "TT_NOSHIFT_ON_OFF",
    "TT_FULL_AURA_ON_OFF",
    "TT_HIDE_AURA_ON_OFF",
    "TT_BOLD_NAME_TYPE_ON_OFF",
    "TT_NOCTRL_ON_OFF",
    "TT_EX_DORIDORI",
    "TT_EX_BINGBING",
    "TT_EX_BANGBANG",
    "TT_SKILLFAIL_ON_OFF",
    "TT_NOTALKMSG_ON_OFF",
    "TT_SET1_ON_OFF",
    "TT_REQ_REPORT_CHAT",
    "TT_REQ_REPORT_CHAT_FROM_CHATROOM",
    "UNUSED_TT_CLIPMOUSE",
    "TT_LIGHTMAP_ON_OFF",
    "TT_QUICKSPELL_ON_OFF",
    "TT_QUICKSPELL2_ON_OFF",
    "TT_QUICKSPELL3_ON_OFF",
    "TT_REQ_EMOTION_STARE",
    "TT_REQ_EMOTION_HUNGRY",
    "TT_REQ_EMOTION_COOL",
    "TT_REQ_EMOTION_MERONG",
    "TT_REQ_EMOTION_SHY",
    "TT_REQ_EMOTION_GOODBOY",
    "TT_REQ_EMOTION_SPTIME",
    "TT_REQ_EMOTION_SEXY",
    "TT_REQ_EMOTION_COMEON",
    "TT_REQ_EMOTION_SLEEPY",
    "TT_REQ_EMOTION_CONGRATULATION",
    "TT_REQ_EMOTION_HPTIME",
    "TT_BATTLE_ON_OFF",
    "TT_NOTRADE_ON_OFF",
    "TT_SIYEON_STATUS",
    "TT_SET2_ON_OFF",
    "TT_SYSTEM_CHECK_ON_OFF",
    "UNUSED_TT_FRAME_ON_OFF",
    "TT_WINDOW_SNAP_ON_OFF",
    "TT_SHOW_PVP_INFO",
    "TT_CHECK_ID",
    "TT_RC_ID",
    "TT_RANDOM_LOADING_SCREEN_ON_OFF",
    "TT_EX_RANK_BLACKSMITH",
    "TT_EX_RANK_ALCHEMIST",
    "TT_NOTALKMSG2_ON_OFF",
    "TT_LEVEL",
    "TT_SP",
    "TT_EX_HI",
    "TT_EX_RANK_TAEKWON",
    "TT_AUTO_MSG_ON_OFF",
    "TT_REQ_EMOTION_SPARK",
    "TT_REQ_EMOTION_CONFUSE",
    "TT_REQ_EMOTION_OHNO",
    "TT_REQ_EMOTION_HUM",
    "TT_REQ_EMOTION_BLABLA",
    "TT_REQ_EMOTION_OTL",
    "TT_REQ_EMOTION_DICE",
    "TT_REQ_KILLER_RANK",
    "TT_TRACEAI_ON_OFF",
    "TT_HOMUN_WAIT",
    "TT_HOMUN_USERAI_ON_OFF",
    "TT_LOGINOUT_ON_OFF",
    "TT_SET3_ON_OFF",
    "TT_SHOPPING_ON_OFF",
    "TT_REQ_MUNAK",
    "TT_REQ_HUNTING",
    "TT_REQ_DEATHKNIGHT",
    "TT_REQ_EMOTION_LUV",
    "TT_REQ_COLLECTOR",
    "TT_MINIGAME_ON_OFF",
    "TT_MER_USERAI_ON_OFF",
    "TT_REQ_EMOTION_MOBILE",
    "TT_REQ_EMOTION_MAIL",
    "TT_REQ_EMOTION_ANTENNA0",
    "TT_REQ_EMOTION_ANTENNA1",
    "TT_REQ_EMOTION_ANTENNA2",
    "TT_REQ_EMOTION_ANTENNA3",
    "TT_COPY_NAME",
    "TT_REQ_ACCEPT_INVITE_PARTY",
    "TT_REQ_REFUSE_INVITE_PARTY",
    "TT_REQ_INVITE_PARTY",
    "TT_REQ_OPEN_EQUIPEDITEM_ONOFF",
    "TT_BATTLECHAT_ON_OFF",
    "TT_REQ_EMOTION_HUM2",
    "TT_REQ_EMOTION_ABS",
    "TT_REQ_EMOTION_OOPS",
    "TT_REQ_EMOTION_SPIT",
    "TT_REQ_EMOTION_ENE",
    "TT_REQ_EMOTION_PANIC",
    "TT_REQ_EMOTION_WHISP",
    "TT_PROCESS_AFFINITY",
    "TT_TINGONLY_ON_OFF",
    "TT_SEEK_PARTY",
    "TT_BATTLEFIELD_ICON_ON_OFF",
    "TT_BATTLEFIELD_WND_ON_OFF",
    "TT_OPEN_SIMPLE_CASH",
    "TT_REQ_RECRUIT",
    "TT_STATEINFO_ON_OFF",
    "TT_SHOW_INFO_TESTLAYER",
    "TT_GPK_DYNCODE_RELOAD",
    "UNUSED_TT_MAKE_DBGINTERFACE",
    "TT_SHOW_WEB_ADDRESSBAR",
    "TT_STOPWATCH",
    "TT_SHOW_BUILD_INFO",
    "TT_WEB_BROWSER",
    "TT_NAVIGATION",
    "TT_NAVIGATION2",
    "TT_REQ_INVITE_GUILD",
    "TT_TEST_MONSTERHP",
    "TT_MONSTERHP_ON_OFF",
    "TT_CLAN_CHAT",
    "TT_REQ_EFST",
    "TT_REQ_BARGAINSALE",
    "TT_NAVIGATION3",
    "TT_SHOW_CASHSHOP_BTN_ON_OFF",
    "TT_SHOW_GOLDPCCAFE_ON_OFF",
    "TT_SHOW_ROULETTE_BTN_ON_OFF",
    "TT_QUAKE_ON_OFF",
    "TT_UNUSED_CONNECT_TWITTER",
    "TT_SHOW_MINIMAP_BUTTON_ONOFF",
    "TT_MACRO_REGISTER",
    "TT_MACRO_PREVIEW",
    "TT_MACRO_DETECTOR",
    "TT_VIEW_CAMERAINFO_ON_OFF",
    "TT_SET_CAMERA",
    "TT_MIN_EFFECT_ON_OFF",
    "TT_RESET_COOLTIME",
    "TT_BLOCK_CALL_ON_OFF",
    "TT_RODEX_ON_OFF",
    "TT_ACHIEVEMENT_ON_OFF",
    "TT_SHOW_EFFECT",
    "TT_STYLINGSHOP",
    "TT_ADVENTURER_AGENCY_SETUP",
    "TT_ADVEN_PARTY_BOARD",
    "TT_INDOOR_DISPLAY_ON_OFF",
    "TT_EX_NO_COS_PLAY",
    "TT_SPRITEMABLE_ON_OFF",
    "TT_EXPAND_SIGHT_ON_OFF",
    "TT_HALFALPHAOBJ_ON_OFF",
    "TT_EMBLEM_FRAME_ON_OFF",
    "TT_INDOOR_MEMO",
    "TT_SKIPCOUNT_TEST",
    "TT_RECORD_TEST",
    "TT_CLASSNAME_POPUP_ON_OFF",
    "TT_TOGGLE_PACKET_LOG",
    "TT_ACCOUNT_LINKED_USER_DATA_TEST_LOG",
    "TT_SHOW_MERCHANT_SHOP_ON_OFF",
    "TT_PRISON_USER",
    "TT_OPEN_ACCOUNT_LIMITED_TOOL",
    "TT_REQ_RESET_PERIOD_EFFECT",
    "TT_REMOVE_ON",
    "TT_REMOVE_OFF",
    "TT_CHANGE_COSTUME_JOB",
    "TT_WEAPON_EFFECT_OUT",
    "TT_WEAPON_EFFECT_IN",
    "TT_ROS_COUNTDOWN",
    "TT_MACRO_CHECKER_MAP",
    "TT_ROS_OPENUI",
    "TT_ROS_OBSERVER",
    "TT_ROS_QUIT",
    "TT_ITEM_COMPARE_ON_OFF",
    "TT_HIDE_PLAYER_ON_OFF",
    "TT_HIDE_SUMMON_ON_OFF",
    "TT_EXTEND_DAMAGE_ON_OFF",
    "TT_SHOW_MAP_NAME_ON_OFF",
    "TT_HIDE_FOOTPRINT_ON_OFF",
    "TT_QUICK_SLOT_ON_OFF",
    "TT_MOUSE_EXCLUSIVE_ON_OFF",
    "TT_TOGGLE_AUTO_HUNT",
    "TT_AUTO_HUNT_MOB_ADD",
    "TT_AUTO_HUNT_MOB_DEL",
    "TT_AUTO_HUNT_MOB_SHOW",
    "TT_TOGGLE_AUTO_HUNT_LOG",
    "TT_AUTO_HUNT_ITEMPICK_WEIGHT",
    "TT_UI_SOUND_ON_OFF",
    "TT_PROBABILITY_TEST",
    "TT_AUTO_HUNT_WINGOFFLY_ADD",
    "TT_AUTO_HUNT_WINGOFFLY_DEL",
    "TT_AUTO_HUNT_WINGOFFLY_CLEAR",
    "TT_AUTO_HUNT_WINGOFFLY_SHOW",
    "TT_CHANGE_LANGUAGE",
    "TT_TRANSLATE_SERVER_ON_OFF",
    "TT_AUTO_MOVE_START",
    "TT_AUTO_MOVE_STOP",
    "TT_AUTO_HUNT_WINGOFBUTTERFLY_ADD",
    "TT_AUTO_HUNT_WINGOFBUTTERFLY_DEL",
    "TT_AUTO_HUNT_WINGOFBUTTERFLY_CLEAR",
    "TT_AUTO_HUNT_WINGOFBUTTERFLY_SHOW",
    "TT_AUTO_HUNT_COMMAND_ONOFF",
    "TT_AUTO_HUNT_COMMAND_VALUE",
    "TT_AUTO_HUNT_COMMAND_ONOFF_SHOW",
    "TT_AUTO_HUNT_COMMAND_VALUE_SHOW",
    "TT_PETINFO_ALERT_ONOFF",
    "TT_SHOW_CARD_ILLUST_ONOFF",
    "TT_NO_CHECK_VALID_CELL_FOR_MOVE",
    "TT_3D_SOUND_ON_OFF"
};

__declspec(noinline) void NotifyServiceOption(unsigned char enabled, int enabledMessage, int disabledMessage, int color);
__declspec(noinline) void ApplyServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage);

// OFFSET: 0x006C48C0
// FUNCTION: fn_45461
void GreetFriends() {
  g_session.HiFriends(nullptr);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("[ To Friends ] : (Hi) *^_^*"), 0x1EFFFF, 0, 0, 0);
}

// OFFSET: 0x006BFE60
// FUNCTION: fn_06299
__declspec(noinline) CGameSettingsMgr::CGameSettingsMgr() {
}

// OFFSET: 0x006C01A0
// FUNCTION: fn_06302
void Noop() {
}

// OFFSET: 0x006C0B80
// FUNCTION: fn_06311
CGameSettingsMgr::~CGameSettingsMgr() {
}

// OFFSET: 0x006C0C40
// FUNCTION: fn_06312
void GameSettings::SetOption(int id, unsigned char value, bool notify) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    if (option.type != 0) {
      return;
    }

    const auto entry = this->actions.find(static_cast<std::uint32_t>(id));
    if (entry == this->actions.end()) {
      ApplyServiceOption(id, value, notify, option.unknown_005c, option.unknown_0060);
      return;
    }

    const std::function<void(int, unsigned char, bool, int, int)> action = entry->second;
    if (action) {
      action(id, value, notify, option.unknown_005c, option.unknown_0060);
    }
    return;
  }
}

// OFFSET: 0x006C0DD0
// FUNCTION: fn_06313
bool GameSettings::RunFunction(int id) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    if (option.type != 1) {
      return false;
    }

    const auto& functions = this->functions;
    const auto entry = functions.find(static_cast<std::uint32_t>(id));
    if (entry == functions.end()) {
      return false;
    }

    const std::function<void()> function = entry->second;
    if (function) {
      function();
      return true;
    }
    return false;
  }
  return false;
}

// OFFSET: 0x006C1030
// FUNCTION: fn_06315
std::optional<GameSettingsOption*> GameSettings::FindOption(int id) {
  auto option = this->options.begin();
  while (option != this->options.end() && id != option->id) {
    ++option;
  }
  if (option != this->options.end()) {
    return &*option;
  }
  return std::nullopt;
}

// OFFSET: 0x006C1070
// FUNCTION: fn_06316
std::list<int> GameSettings::GetOptionIds(int tab) {
  std::list<int> ids;
  for (const GameSettingsOption& option : this->options) {
    if (option.tab == tab) {
      ids.push_back(option.id);
    }
  }
  return ids;
}

// OFFSET: 0x006C1180
// FUNCTION: fn_06317
bool CGameSettingsMgr::LoadSettings() {
  this->SetGlobal("EFFECT", 1.0);
  this->SetGlobal("CONTROL", 2.0);
  this->SetGlobal("ETC", 4.0);
  this->SetGlobal("ONOFF", 0.0);
  this->SetGlobal("EXE", 1.0);
  for (int i = 0; i < 300; ++i) {
    this->SetGlobal(g_serviceOptionNames[i], static_cast<double>(i));
  }

  this->Init_Func();

  const std::uint8_t source[] = {
      0x5B, 0x6E, 0x4C, 0xE5, 0xD4, 0x87, 0x24, 0x7C, 0x22, 0x2A, 0x19, 0xFF, 0xF1, 0x9E, 0x20, 0x2F,
      0x51, 0x72, 0x41, 0xC6, 0xCE, 0xB7, 0x0E, 0x7F, 0x63, 0x72, 0x42, 0xCD, 0xF4, 0x85, 0x27, 0x60,
      0x4B, 0x5C, 0x4C, 0xCE, 0xD8, 0xB8, 0x24, 0x7B, 0x63, 0x72, 0x43, 0xC4, 0xCE, 0xC5, 0x2D, 0x7A,
      0x75, 0x1B};
  thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> name(source);
  if (!this->LoadFile(name.Text())) {
    return false;
  }
  return this->ReadOptionTable();
}

// OFFSET: 0x006C17A0
// FUNCTION: fn_06318
unsigned char ServiceOptions::Get(int id) const {
  const auto entry = g_serviceOptions.values.find(static_cast<std::uint32_t>(id));
  return entry == g_serviceOptions.values.end() ? 0 : entry->second;
}

// OFFSET: 0x006C1820
// FUNCTION: fn_06319
bool IsServiceOption(int id) {
  switch (id) {
  case 106:
  case 107:
  case 108:
  case 231:
  case 269:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x006C19D0
// FUNCTION: fn_06321
bool GameSettings::ReadOptionTable() {
  lua_State* state = this->m_state;
  if (!state) {
    return false;
  }

  lua_getfield(state, LUA_GLOBALSINDEX, "OptionTbl");
  if (lua_type(state, -1) != LUA_TTABLE) {
    return false;
  }

  this->options.resize(lua_objlen(state, -1));
  lua_pushnil(state);
  while (lua_next(state, -2)) {
    GameSettingsOption option;
    const int index = lua_tointeger(state, -2);

    lua_pushstring(state, "ID");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.id = lua_tointeger(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].ID가 선언되지 않았습니다. 클라이언트로 문의 부탁드립니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Tab");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.tab = lua_tointeger(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Tab이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Type");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.type = lua_tointeger(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Type이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    if (option.type == 0) {
      lua_pushstring(state, "Default");
      lua_gettable(state, -2);
      if (lua_type(state, -1) > LUA_TNIL && lua_isboolean(state, -1)) {
        option.defaultValue = (lua_toboolean(state, -1) != 0) ^ IsServiceOption(option.id);
      } else {
        MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Default가 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "MSGs");
      lua_gettable(state, -2);
      if (lua_type(state, -1) > LUA_TNIL && lua_istable(state, -1)) {
        lua_pushnumber(state, 1.0);
        lua_gettable(state, -2);
        const int enabledMessage = FindMessageId(lua_tostring(state, -1));
        lua_pop(state, 1);
        lua_pushnumber(state, 2.0);
        lua_gettable(state, -2);
        const int disabledMessage = FindMessageId(lua_tostring(state, -1));
        lua_pop(state, 1);
        option.unknown_005c = enabledMessage;
        option.unknown_0060 = disabledMessage;
      }
      lua_pop(state, 1);

      g_serviceOptions.values[static_cast<std::uint32_t>(option.id)] = option.defaultValue;
    }

    lua_pushstring(state, "Title");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isstring(state, -1)) {
      option.title = lua_tostring(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Title이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Tooltip");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isstring(state, -1)) {
      option.tooltip = lua_tostring(state, -1);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "TipBoxID");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.tipBoxId = lua_tointeger(state, -1);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Description");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isstring(state, -1)) {
      option.description = lua_tostring(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Description이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    this->options[index - 1] = std::move(option);
    lua_pop(state, 1);
  }

  lua_pop(state, 1);
  return lua_gettop(state) == 0;
}

// OFFSET: 0x006C2610
// FUNCTION: fn_06322
void GameSettings::ResetToDefaults() {
  for (const GameSettingsOption& option : this->options) {
    if (option.type == 0) {
      this->SetOption(option.id, option.defaultValue, true);
    }
  }
}

// OFFSET: 0x006C27D0
// FUNCTION: fn_06323
void GameSettings::ToggleOption(int id) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    if (option.type == 0) {
      const unsigned char current = g_serviceOptions.Get(id);
      this->SetOption(id, current == 0, true);
      g_windowMgr.RefreshGameSettingOption(id);
    } else if (option.type == 1) {
      const auto entry = this->functions.find(id);
      if (entry != this->functions.end() && entry->second) {
        entry->second();
      }
    }
    break;
  }
}

// OFFSET: 0x006C29A0
// FUNCTION: fn_06324
__declspec(noinline) int SetOption(const char* name, unsigned char value) {
  if (!name || name[0] == '\0') {
    return 3;
  }

  int optionId = 3;
  std::string strings[3];
  g_session.GetTalkType(name, &optionId, strings);
  if (optionId != 3) {
    g_serviceOptions.values[static_cast<std::uint32_t>(optionId)] = value;
  }
  return optionId;
}

// OFFSET: 0x006C0F00
// FUNCTION: fn_06314
std::pair<int, unsigned char> FindServiceOption(const char* name) {
  if (IsNullOrEmpty(name)) {
    return {3, 0};
  }

  int optionId = 3;
  std::string strings[3];
  g_session.GetTalkType(name, &optionId, strings);
  const auto entry = g_serviceOptions.values.find(static_cast<std::uint32_t>(optionId));
  if (entry == g_serviceOptions.values.end()) {
    return {3, 0};
  }
  return {optionId, entry->second};
}

// OFFSET: 0x006C2A80
// FUNCTION: fn_06325
bool ServiceOptions::Set(int id, unsigned char value) {
  if (g_serviceOptions.values.find(static_cast<std::uint32_t>(id)) == g_serviceOptions.values.end()) {
    return false;
  }

  g_serviceOptions.values[static_cast<std::uint32_t>(id)] = value;
  return true;
}

// OFFSET: 0x006C1900
// FUNCTION: fn_06320
__declspec(noinline) void GameSettings::ApplyScriptOption(int id, const char* key) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    g_serviceOptions.Set(id, g_session.GetOptionInteger(key, option.defaultValue) != 0);
    return;
  }
}

namespace {
#pragma pack(push, 1)
struct ServiceOptionPacket {
  std::int16_t id;
};

struct WhisperBlockPacket {
  std::int16_t id;
  unsigned char enabled;
};

struct ConfigPacket {
  std::int16_t id;
  int type;
  int value;
};
#pragma pack(pop)
}

// OFFSET: 0x006C5DB0
// FUNCTION: fn_06368
void SendServiceOptionPacket() {
  ServiceOptionPacket packet;
  packet.id = 0xAE8;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C5E70
// FUNCTION: fn_06370
void NotifyServiceOption(unsigned char enabled, int enabledMessage, int disabledMessage, int color) {
  std::string text;
  if (enabled != 0) {
    if (enabledMessage != 0x125A) {
      text = LookupMessage(enabledMessage);
    }
  } else if (disabledMessage != 0x125A) {
    text = LookupMessage(disabledMessage);
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0, 0);
  }
}

// OFFSET: 0x006C6050
// FUNCTION: fn_06372
void NotifyGameModeOption() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x11F, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C6080
// FUNCTION: fn_06373
void NotifyGameModeOptionAlt() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x11F, 2, 0, 0, 0);
  }
}

// OFFSET: 0x006C60B0
// FUNCTION: fn_06374
void NotifyGameModeMenu() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x4B, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C40C0
// FUNCTION: fn_06350
void RotatePlayerDirection(bool clockwise) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  CPlayer* player = mode->scene->player;
  if (!player) {
    return;
  }

  float angle = static_cast<float>((8 - player->Get8Dir(player->render.angle)) * 45) - mode->camera->camera.yaw;
  if (clockwise) {
    angle -= 45.0f;
    player->render.angle = angle;
    if (angle < 0.0f) {
      angle += 360.0f;
      player->render.angle = angle;
    }
  } else {
    angle += 45.0f;
    player->render.angle = angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
      player->render.angle = angle;
    }
  }

  mode->SendMsg(0x8B, (static_cast<int>(angle) + 180) / 45 % 8, 0, 0, 0);
}

// OFFSET: 0x006C41B0
// FUNCTION: fn_06351
void SendConfigServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    ConfigPacket packet;
    packet.id = 0x2D8;
    packet.type = 1;
    packet.value = value;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x006C4220
// FUNCTION: fn_06352
void ApplyServiceOptionFlags(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  SetServiceFlags(value, value);
}

// OFFSET: 0x006C4270
// FUNCTION: fn_06353
void ApplyServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }
}

// OFFSET: 0x006C5A10
// FUNCTION: fn_06362
void ApplyServiceOptionItemWnd(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (g_windowMgr.unknown_01D4 != nullptr) {
    static_cast<UIItemWnd*>(g_windowMgr.unknown_01D4)->RefreshOptionBox();
  }
}

// OFFSET: 0x006C42B0
// FUNCTION: fn_06354
void ToggleSitStand() {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == 933) {
      return;
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  CPlayer* player = mode->scene->player;
  if (!player) {
    return;
  }

  const int motion = (player->render.curMotion != 2) + 1;
  player->render.curMotion = motion;
  player->render.angle = static_cast<float>(std::ceil(player->render.angle));
  mode->SendMsg(0x8B, (static_cast<int>(player->render.angle) + 180) / 45 % 8, motion, 0, 0);

  if (player->render.unknown_0070 == 6) {
    std::list<unsigned long>& times = mode->unknown_05D4;
    times.pop_front();
    times.push_back(timeGetTime());
    const unsigned long elapsed = times.back() - times.front();
    if (elapsed - 1501 <= 1498) {
      mode->SendMsg(0xA9, 0, 0, 0, 0);
      for (unsigned long& time : times) {
        time = 0;
      }
    }
  }
}

// OFFSET: 0x006C44A0
// FUNCTION: fn_06355
void ApplyServiceOptionXmasWeather(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  const std::string mapName = mode->MapName();
  if (mapName == "xmas_fild01.rsw" || mapName == "xmas.rsw") {
    if (g_serviceOptions.Get(8)) {
      g_weatherEffect.Snow();
    } else {
      g_weatherEffect.ExtendEffect();
    }
  }
}

// OFFSET: 0x006C4660
// FUNCTION: fn_06357
void ApplyServiceOptionMapFog(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ApplyMapFog(value);
}

// OFFSET: 0x006C46C0
// FUNCTION: fn_06358
void ApplyServiceOptionWithNotice(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (g_serviceOptions.Get(107)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBB4)), 0x64FFFF, 0, 0, 0);
      return;
    }

    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->RefreshActorJobEffects(false);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }
}

// OFFSET: 0x006C48F0
// FUNCTION: fn_06360
void ApplyServiceOptionActorRefresh(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->RefreshActorJobEffects(false);
}

// OFFSET: 0x006C4750
// FUNCTION: fn_06359
void ShowHelpMessages() {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  int count = 0;
  g_session.m_hotkeyScript->Call("GetTableSizeForC", "s>d", "HelpMsgStr", &count);
  for (int index = 1; index <= count; ++index) {
    const char* text = nullptr;
    if (g_session.m_hotkeyScript->Call("getHelpMsg", "d>s", index, &text) && std::strlen(text) != 0) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF64, 0, 0, 0);
    }
  }
}

// OFFSET: 0x006C5D10
// FUNCTION: fn_06366
void ApplyServiceOptionSessionFlag(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  *reinterpret_cast<int*>(g_session.unknown_5454) = value;
}

// OFFSET: 0x006C5D60
// FUNCTION: fn_06367
void ApplyServiceOptionCursorClip(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (value) {
    ClipApplicationCursor(hWnd);
  } else {
    ReleaseApplicationCursor();
  }
}

// OFFSET: 0x006C5DF0
// FUNCTION: fn_06369
void ApplyServiceOptionAskOnDisable(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(0x128) == value) {
    return;
  }

  if (!value &&
      g_windowMgr.Confirm(LookupMessage(0x1104), 2, true, false, 0, "Message", 0x118, 0x78, 0) == 0xBC) {
    g_serviceOptions.Set(0x128, 1);
    g_windowMgr.RefreshGameSettingOption(0x128);
    return;
  }

  g_session.SendClientOption(value);
}

// OFFSET: 0x006C5FC0
// FUNCTION: fn_06371
void ApplyServiceOptionWindow159(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0x9F)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C60F0
// FUNCTION: fn_06375
void ApplyServiceOptionWindow190(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(0xBE)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C6150
// FUNCTION: fn_06376
void ApplyServiceOptionWindow267(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(0x10B)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C61B0
// FUNCTION: fn_06377
void ApplyServiceOptionWindows189(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  g_windowMgr.SetWindows189Visible(value);
}

// OFFSET: 0x006C6200
// FUNCTION: fn_06378
void ApplyServiceOptionNotifyWindow(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01C0)) {
    window->SendMsg(nullptr, 6, 0x1F6, 0, 0, 0);
  }
}

// OFFSET: 0x006C6260
// FUNCTION: fn_06379
void ApplyServiceOptionWindow268(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(0x10C)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C6300
// FUNCTION: fn_06380
void ApplyServiceOptionHideWindow(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode && mode->unknown_05D0 && !g_serviceOptions.Get(id)) {
    mode->unknown_05D0->Show(0);
  }
}

// OFFSET: 0x006C6380
// FUNCTION: fn_06381
void EnableWhisperBlock() {
  g_whisperBlockMgr->Clear();
  WhisperBlockPacket packet;
  packet.id = 0xD0;
  packet.enabled = 1;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C63D0
// FUNCTION: fn_06382
void DisableWhisperBlock() {
  WhisperBlockPacket packet;
  packet.id = 0xD0;
  packet.enabled = 0;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C6410
// FUNCTION: fn_06383
void SendWhisperBlockPacket() {
  ServiceOptionPacket packet;
  packet.id = 0xD3;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C6450
// FUNCTION: fn_06384
void ApplyServiceOptionAndSnap(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  g_windowMgr.UpdateSnapRectangles();
}

// OFFSET: 0x006C5A80
// FUNCTION: fn_06363
void ApplyServiceOptionWeatherFlag(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  g_weatherEffect.unknown_0001 = value ^ 1;
}

// OFFSET: 0x006C5AD0
// FUNCTION: fn_06364
void ApplyServiceOptionWindow27() {
  if (g_windowMgr.unknown_0304 != 0) {
    g_windowMgr.Confirm(LookupMessage(0x24B), 0, true, false, 0, "Message", 0x118, 0x78, 0);
    return;
  }

  if (g_windowMgr.unknown_02FC != 0) {
    CMode* mode = g_modeMgr.GetGameMode();
    if (mode) {
      mode->SendMsg(0x52, 0, 0, 0, 0);
    }
    return;
  }

  g_windowMgr.EnsureWindow(0x1B);
}

// GLOBAL: gl_24912
template <>
CGameSettingsMgr* Singleton<CGameSettingsMgr>::s_instance = nullptr;

// OFFSET: 0x00AB1040
// FUNCTION: fn_17330
template <>
CGameSettingsMgr* Singleton<CGameSettingsMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CGameSettingsMgr;
  }

  return s_instance;
}

// OFFSET: 0x006C45F0
// FUNCTION: fn_06356
void ApplyServiceOptionJitterScale(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (value) {
    g_serviceOptionJitterScale = 730.0f;
  } else {
    g_serviceOptionJitterScale = g_jitterDistanceScale;
  }
}

// OFFSET: 0x006C45D0
// FUNCTION: fn_45463
void RequestModeAction48() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x30, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C5A10
// FUNCTION: fn_06362
void ApplyItemWndServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (g_windowMgr.unknown_01D4) {
    static_cast<UIItemWnd*>(g_windowMgr.unknown_01D4)->RefreshOptionBox();
  }
}

// OFFSET: 0x006C5A60
// FUNCTION: fn_45464
void RequestModeAction61() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x3D, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C6020
// FUNCTION: fn_45465
void NotifyGameModeOptionFirst() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x11F, 1, 0, 0, 0);
  }
}

// OFFSET: 0x006C62C0
// FUNCTION: fn_45462
void RequestPlayerAction31() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (CPlayer* player = mode->scene->player) {
    player->SendMsg(nullptr, 0x1F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C5B40
// FUNCTION: fn_06365
void ShowPlayerLocation() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(mode->scene->player);
  if (!player) {
    return;
  }

  int column = 0;
  int row = 0;
  mode->WorldCell(player->render.position.x, player->render.position.z, column, row);

  std::string name = mode->MapName();
  if (name.size() > 4) {
    name.resize(name.size() - 4);
  }

  const std::string text = std::format("{}({}) : {}, {}", g_session.GetMapDisplayName(mode->MapName().c_str()), name.c_str(), column, row);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0x64FFFF, 0, 0, 0);
  if (g_serviceType != 10) {
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text.c_str()), 0x64FFFF, 0, 0, 0);
  }
}

static void ShowNoticeType6() {
  g_windowMgr.SendNotice(6, 0, 0, 0, 0, 0);
}

// OFFSET: 0x006C4950
// FUNCTION: fn_06361
void CGameSettingsMgr::Init_Func() {
  this->actions[0x6B] = &ApplyServiceOptionActorRefresh;
  this->actions[0x6A] = &ApplyServiceOptionWithNotice;
  this->actions[0x08] = &ApplyServiceOptionXmasWeather;
  this->actions[0x77] = &ApplyServiceOptionWeatherFlag;
  this->actions[0x3C] = &ApplyServiceOptionMapFog;
  this->actions[0xF1] = &ApplyServiceOptionJitterScale;
  this->actions[0x10F] = &ApplyServiceOptionCursorClip;
  this->actions[0x10E] = &ApplyServiceOptionWindow159;
  this->actions[0x108] = &ApplyItemWndServiceOption;
  this->actions[0x128] = &ApplyServiceOptionAskOnDisable;
  this->actions[0x6C] = &ApplyServiceOptionFlags;
  this->actions[0x8D] = &ApplyServiceOptionAndSnap;
  this->actions[0xC9] = &ApplyServiceOptionHideWindow;
  this->actions[0xDA] = &ApplyServiceOptionWindow190;
  this->actions[0xDB] = &ApplyServiceOptionWindow267;
  this->actions[0xDC] = &ApplyServiceOptionWindow268;
  this->actions[0xDF] = &ApplyServiceOptionNotifyWindow;
  this->actions[0xE5] = &ApplyServiceOptionSessionFlag;
  this->actions[0xE7] = &SendConfigServiceOption;
  this->actions[0xFA] = &ApplyServiceOptionWindows189;

  this->functions[0xEF] = &SendServiceOptionPacket;
  this->functions[0x97] = &GreetFriends;
  this->functions[0x0E] = &ShowHelpMessages;
  this->functions[0x16] = &RequestPlayerAction31;
  this->functions[0x17] = &ApplyServiceOptionWindow27;
  this->functions[0x18] = &RequestModeAction48;
  this->functions[0x1B] = &RequestModeAction61;
  this->functions[0x09] = &ShowPlayerLocation;
  this->functions[0x2B] = &NotifyGameModeMenu;
  this->functions[0x33] = &ShowNoticeType6;
  this->functions[0x6E] = &ToggleSitStand;
  this->functions[0x6F] = []() { RotatePlayerDirection(false); };
  this->functions[0x70] = []() { RotatePlayerDirection(true); };
  this->functions[0x0F] = &SendWhisperBlockPacket;
  this->functions[0x11] = &EnableWhisperBlock;
  this->functions[0x10] = &DisableWhisperBlock;
  this->functions[0x92] = &NotifyGameModeOption;
  this->functions[0x93] = &NotifyGameModeOptionFirst;
  this->functions[0x98] = &NotifyGameModeOptionAlt;
}

// GLOBAL: gl_25208
bool g_showBattleFieldIcon;

// GLOBAL: gl_25209
int g_configuredOldWidth;

// GLOBAL: gl_25210
int g_configuredOldHeight;

// GLOBAL: gl_25211
int g_configuredDeviceCount;

// GLOBAL: gl_25212
int g_configuredModeCount;

// GLOBAL: gl_25213
int g_renderSystemReset;

// GLOBAL: gl_25214
GUID g_configuredDeviceGuid;

// GLOBAL: gl_25215
char g_configuredDeviceName[0x20];

// GLOBAL: gl_25216
int g_viewportWidth = 1024;

// GLOBAL: gl_25217
int g_viewportHeight = 768;

// GLOBAL: gl_24767
extern const GUID g_defaultDrawGuidDX7 = {};

// OFFSET: 0x00DFE4E0
// FUNCTION: fn_23604
void CSession::LoadOptionInfo() {
  g_session.m_hotkeyScript->Call("SetCmdOnOffList", "");
  ResourceExists("OptionInfo.lua", 1);
  g_textureFiltering = this->GetOptionInteger("Trilinear", 0);

  const int effectVolume = this->GetOptionInteger("Effect_Volume", 100);
  CSoundMgr::SetVolume(g_soundMgr, this->GetOptionInteger("Bgm_Volume", 100));
  g_soundMgr->SetTargetVolume(CSoundMgr::Volume(g_soundMgr));
  g_soundMgr->SetSampleVolume(effectVolume);
  g_soundMgr->Set3DVolume(effectVolume);

  this->m_autoOpenWhisperWindow = this->GetOptionInteger("AutoOpen1to1Window", 1);
  this->m_autoOpenFriendWhisperWindow = this->GetOptionInteger("AutoOpen1to1Window_Friend", 1);
  this->m_whisperOpenSound = this->GetOptionInteger("PlaySound_Open1to1Window", 1);
  this->m_simplifiedSkillList = this->GetOptionInteger("Simplicity_SkillList", 0);
  this->m_showSkillDescription = this->GetOptionInteger("Show_SkillDescript", 0);
  g_windowMgr.chatModeChanged = this->GetOptionInteger("ChangeChatMode", 1) != 0;
  this->m_lockMouse = this->GetOptionInteger("LockMouse", 0) != 0;
  this->m_channelCopId = this->GetOptionInteger("ChannelCopID", 0);

  g_outdoorCameraPitch = this->GetOptionFloat("Outdoor_ViewLatitude", -45.0f);
  g_outdoorCameraDistance = this->GetOptionFloat("Outdoor_ViewDistance", 300.0f);
  g_indoorCameraPitch = this->GetOptionFloat("Indoor_ViewLatitude", -45.0f);
  g_indoorCameraDistance = this->GetOptionFloat("Indoor_ViewDistance", 300.0f);

  if (g_indoorCameraPitch < -55.0f) {
    g_indoorCameraPitch = -55.0f;
  } else if (g_indoorCameraPitch > -35.0f) {
    g_indoorCameraPitch = -35.0f;
  }

  if (g_indoorCameraDistance < g_cameraMinDistance) {
    g_indoorCameraDistance = g_cameraMinDistance;
  }

  if (g_indoorCameraDistance > g_cameraBaseDistance) {
    g_indoorCameraDistance = g_cameraBaseDistance;
  }

  Singleton<CGameSettingsMgr>::GetInstance();
  ::SetOption(LookupMessage(0xDB9), this->GetOptionInteger(LookupMessage(0xDB9), 0) != 0);
  g_serviceOptionJitterScale = g_serviceOptions.Get(0xF1) ? 730.0f : g_jitterDistanceScale;

  if (g_outdoorCameraPitch < -65.0f) {
    g_outdoorCameraPitch = -65.0f;
  } else if (g_outdoorCameraPitch > -25.0f) {
    g_outdoorCameraPitch = -25.0f;
  }

  if (g_outdoorCameraDistance < g_cameraMinDistance) {
    g_outdoorCameraDistance = g_cameraMinDistance;
  }

  if (g_outdoorCameraDistance > g_serviceOptionJitterScale) {
    g_outdoorCameraDistance = g_serviceOptionJitterScale;
  }

  g_skinInfo.m_name = this->GetOptionString("SkinName", "<Basic Skin>");
  this->unknown_1708 = g_serviceOptions.Get(0x2E);
  g_showBattleFieldIcon = this->GetOptionInteger("ShowBattleFieldIcon", 1) != 0;
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(ConfineCursor, "MouseExclusive");

  const int resetPolicy = this->RenderSystemResetPolicy();
  const int defaultBackend = this->DefaultRenderBackend();
  g_renderSystemReset = this->GetOptionInteger("RenderSystemReset", 1);

  if (resetPolicy != g_renderSystemReset) {
    g_configuredRenderBackend = defaultBackend;
    g_renderSystemReset = resetPolicy;
  } else {
    g_configuredRenderBackend = this->GetOptionInteger("RENDERSYSTEM", defaultBackend);
  }

  g_configuredDeviceGuid = this->GetOptionGuid("DX9DEVICEID", g_defaultDrawGuidDX7);
  strncpy_s(g_configuredDeviceName, sizeof(g_configuredDeviceName), this->GetOptionString("DX9DEVICENAME", "").c_str(), _TRUNCATE);

  g_configuredFullscreen = this->GetOptionInteger("ISFULLSCREENMODE", 0);
  g_configuredScreenWidth = this->GetOptionInteger("WIDTH", 1024);
  g_configuredScreenHeight = this->GetOptionInteger("HEIGHT", 768);
  g_configuredOldWidth = this->GetOptionInteger("OLD_WIDTH", 0);
  g_configuredOldHeight = this->GetOptionInteger("OLD_HEIGHT", 0);

  if (static_cast<unsigned int>(g_configuredScreenHeight) < 664) {
    g_configuredScreenWidth = 1024;
    g_configuredScreenHeight = 768;
  }

  g_viewportWidth = g_configuredScreenWidth;
  g_viewportHeight = g_configuredScreenHeight;
  g_configuredBitsPerPixel = this->GetOptionInteger("BITPERPIXEL", 16);

  OSVERSIONINFOA version = {sizeof(version)};
  GetVersionExA(&version);

  if (version.dwMajorVersion >= 6 && version.dwMinorVersion >= 2) {
    g_configuredBitsPerPixel = 32;
  }

  g_configuredDeviceCount = this->GetOptionInteger("DEVICECNT", 0);
  g_configuredModeCount = this->GetOptionInteger("MODECNT", 0);

  g_effectDetailLevel = this->GetOptionInteger("SPRITEMODE", 2);
  switch (g_effectDetailLevel) {
  case 0:
    g_spriteHalfWidth = 1;
    [[fallthrough]];
  case 1:
    g_spriteHalfHeight = 1;
    break;
  default:
    g_spriteHalfWidth = 0;
    g_spriteHalfHeight = 0;
    break;
  }

  g_textureDetailLevel = this->GetOptionInteger("TEXTUREMODE", 2);
  switch (g_textureDetailLevel) {
  case 0:
    g_textureReduction = 4;
    break;
  case 1:
    g_textureReduction = 2;
    break;
  default:
    g_textureReduction = 1;
    break;
  }

  this->m_lockItemDrop = this->GetOptionInteger("bLockItemDropFromItemWnd", 0) != 0;
  this->m_skipPriceWarning = this->GetOptionInteger("bWarningPriceCheck", 0) == 0;
  this->m_muteSellSound = this->GetOptionInteger("bSoundEffectInItemSellWnd", 0) == 0;
  this->m_guildMemberListSort = this->GetOptionInteger("bGuildMemberListSort", 1) != 0;
  this->m_blockPrivateTab = this->GetOptionInteger("bBlockPrivateTab", 0) != 0;
  this->m_itemDefaultSort = this->GetOptionInteger("bItemDefaultSort", 0) != 0;
  this->m_itemAscendingOrder = this->GetOptionInteger("bItemAscendingOrder", 0) != 0;
  this->m_itemDescendingOrder = this->GetOptionInteger("bItemDescendingOrder", 0) != 0;
  this->m_showMenuIcon = this->GetOptionInteger("bShowMenuIcon", 1) != 0;

  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0xF3, LookupMessage(0xDC6));
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10B, "bExtendDamage");
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10C, "bShowMapName");
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10D, "bShowOnlyMineFootprint");

  this->m_damageSkinIndex = this->GetOptionInteger("DamageSkin", 0);
  this->m_damageSkinMode = this->GetOptionInteger("DamageDir", 0);
  this->m_priorityClass = this->GetOptionInteger("PriorityClass", 32);

  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10A, "bSummonTranslucent");
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x109, "bHidePlayer");
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10E, "bQuickSlot");
}

// OFFSET: 0x00E01770
// FUNCTION: fn_23651
void CSession::SaveOptionInfo() {
  std::string path = "SaveData\\OptionInfo.lua";
  this->m_hotkeyScript->Call("SaveToFileCmdOnOffValueEx", "d", 0);

  CFile input;
  if (!input.Open(path.c_str(), 0, false, true)) {
    return;
  }

  const std::uint32_t size = input.GetSize();
  char* buffer = new char[size + 1];
  input.Read(buffer, size);
  buffer[size] = '\0';
  input.Close();

  CFile output;
  if (!output.Open(path.c_str(), 0x1001, true, true)) {
    return;
  }

  output.Write(buffer, size);
  delete[] buffer;

  this->WriteOptionInteger(output, "Trilinear", g_textureFiltering);
  this->WriteOptionInteger(output, "Bgm_Volume", g_soundMgr->targetVolume);
  this->WriteOptionInteger(output, "Effect_Volume", g_soundMgr->sampleVolume);
  this->WriteOptionInteger(output, "AutoOpen1to1Window", this->m_autoOpenWhisperWindow);
  this->WriteOptionInteger(output, "AutoOpen1to1Window_Friend", this->m_autoOpenFriendWhisperWindow);
  this->WriteOptionInteger(output, "PlaySound_Open1to1Window", this->m_whisperOpenSound);
  this->WriteOptionFloat(output, "Outdoor_ViewLatitude", g_outdoorCameraPitch);
  this->WriteOptionFloat(output, "Outdoor_ViewDistance", g_outdoorCameraDistance);
  this->WriteOptionFloat(output, "Indoor_ViewLatitude", g_indoorCameraPitch);
  this->WriteOptionFloat(output, "Indoor_ViewDistance", g_indoorCameraDistance);
  this->WriteOptionString(output, "SkinName", g_skinInfo.Name());
  this->WriteOptionInteger(output, "ChannelCopID", this->m_channelCopId);

  RECT rect;
  GetWindowRect(hWnd, &rect);
  this->WriteOptionInteger(output, "Window_XPos", rect.left);
  this->WriteOptionInteger(output, "Window_YPos", rect.top);
  this->WriteOptionInteger(output, "Simplicity_SkillList", this->m_simplifiedSkillList);
  this->WriteOptionInteger(output, "Show_SkillDescript", this->m_showSkillDescription);
  this->WriteOptionInteger(output, "ChangeChatMode", g_windowMgr.chatModeChanged);
  this->WriteOptionInteger(output, "LockMouse", this->m_lockMouse);
  this->WriteOptionInteger(output, "ShowBattleFieldIcon", g_showBattleFieldIcon);
  this->WriteOptionInteger(output, "MouseExclusive", g_serviceOptions.Get(ConfineCursor));
  this->WriteOptionInteger(output, "RenderSystemReset", g_renderSystemReset);
  this->WriteOptionInteger(output, "RENDERSYSTEM", g_configuredRenderBackend);
  this->WriteOptionGuid(output, "DX9DEVICEID", g_configuredDeviceGuid);
  this->WriteOptionString(output, "DX9DEVICENAME", g_configuredDeviceName);
  this->WriteOptionInteger(output, "ISFULLSCREENMODE", g_configuredFullscreen);
  this->WriteOptionInteger(output, "WIDTH", g_configuredScreenWidth);
  this->WriteOptionInteger(output, "HEIGHT", g_configuredScreenHeight);
  this->WriteOptionInteger(output, "OLD_WIDTH", g_configuredScreenWidth);
  this->WriteOptionInteger(output, "OLD_HEIGHT", g_configuredScreenHeight);
  this->WriteOptionInteger(output, "BITPERPIXEL", g_configuredBitsPerPixel);
  this->WriteOptionInteger(output, "DEVICECNT", g_configuredDeviceCount);
  this->WriteOptionInteger(output, "MODECNT", g_configuredModeCount);
  this->WriteOptionInteger(output, "SPRITEMODE", g_effectDetailLevel);
  this->WriteOptionInteger(output, "TEXTUREMODE", g_textureDetailLevel);
  this->WriteOptionInteger(output, "bLockItemDropFromItemWnd", this->m_lockItemDrop);
  this->WriteOptionInteger(output, "bWarningPriceCheck", !this->m_skipPriceWarning);
  this->WriteOptionInteger(output, "bSoundEffectInItemSellWnd", !this->m_muteSellSound);
  this->WriteOptionInteger(output, "bGuildMemberListSort", this->m_guildMemberListSort != 0);
  this->WriteOptionInteger(output, "bBlockPrivateTab", this->m_blockPrivateTab != 0);
  this->WriteOptionInteger(output, "bShowMenuIcon", this->m_showMenuIcon != 0);
  this->WriteOptionInteger(output, "bItemDefaultSort", this->m_itemDefaultSort != 0);
  this->WriteOptionInteger(output, "bItemAscendingOrder", this->m_itemAscendingOrder != 0);
  this->WriteOptionInteger(output, "bItemDescendingOrder", this->m_itemDescendingOrder != 0);
  this->WriteOptionInteger(output, LookupMessage(0xDC6), g_serviceOptions.Get(0xF3));
  this->WriteOptionInteger(output, "bExtendDamage", g_serviceOptions.Get(0x10B));
  this->WriteOptionInteger(output, "bShowMapName", g_serviceOptions.Get(0x10C));
  this->WriteOptionInteger(output, "bShowOnlyMineFootprint", g_serviceOptions.Get(0x10D));
  this->WriteOptionInteger(output, LookupMessage(0xDB9), g_serviceOptions.Get(0xF1));
  this->WriteOptionInteger(output, "DamageSkin", this->m_damageSkinIndex);
  this->WriteOptionInteger(output, "DamageDir", this->m_damageSkinMode);
  this->WriteOptionInteger(output, "PriorityClass", this->m_priorityClass);
  this->WriteOptionInteger(output, "bSummonTranslucent", g_serviceOptions.Get(0x10A));
  this->WriteOptionInteger(output, "bHidePlayer", g_serviceOptions.Get(0x109));
  this->WriteOptionInteger(output, "bQuickSlot", g_serviceOptions.Get(0x10E));
  output.Close();
}
