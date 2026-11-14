#include "Contents/Actors/ActorEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "UI/Window/WindowId.h"
#include "Network/Packets/ActorNamePacket.h"
#include "Client/Mode/CGameMode.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "Client/Mode/GameModeEnums.h"
#include "Contents/Items/ItemId.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillUnitId.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Npc.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/DummyEffectLauncher.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Skills/SkillEffects.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/MousePointer.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"
#include "UI/Text/UIBroadcastBalloon.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Contents/Actors/GrannyGameActor.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"
#include "Client/Application/Launcher.h"
#include "UI/Navigation/NavigationLink.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "Contents/Map/MapInfoMgr.h"
#include <timeapi.h>
#include "File/File.h"
#include "Client/Stove/StoveMgr.h"
#include "Contents/Events/ShowRankingScheduler.h"
#include "Render/Primitives/TextureQuad.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "Utility/Text/HexString.h"
#include "Client/Application/Timer.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Client/Input/Language.h"
#include "UI/Chat/UIChatRoomMakeWnd.h"
#include "UI/Gauges/UIPcGage.h"
#include "Render/Projection/ScreenGeometry.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Items/UIRefiningWnd.h"
#include "UI/Items/UIGradeEnchantWnd.h"
#include "UI/Items/UISwapEquipmentWnd.h"
#include "UI/Monster/UIMonsterInfoWnd.h"
#include "Contents/Clan/ClanInfo.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/MercenaryAI.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Guild/UIGuildWnd.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "File/ShortcutFilename.h"
#include "UI/Dialogs/UICntWnd.h"
#include "UI/Dialogs/UINormalMB.h"
#include <bit>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mbstring.h>
#include <new>
#include "Client/Application/ApplicationWindow.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Network/TextEncoding.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

extern "C" long long _Xtime_get_ticks();

#pragma pack(push, 1)
struct GameModeTextRecord {
  /* 0000 */ std::uint32_t header;
  /* 0004 */ std::int16_t length;
  /* 0006 */ int firstLength;
  /* 000A */ int secondLength;
  /* 000E */ int roomOptionA;
  /* 0012 */ int roomOptionB;
  /* 0016 */ int roomOptionC;
  /* 001A */ int roomOptionD;
  /* 001E */ char buffer[0x172];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GameModeReplayPacket {
  /* 0000 */ std::uint32_t header;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ unsigned char info[0x19];
  /* 001F */ unsigned char data[0xDF];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GameModePacketText40 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ char text[40];
};
#pragma pack(pop)

template __declspec(noinline) std::vector<char>::vector(const std::size_t, const char&, const std::allocator<char>&);

namespace {
struct RodexSendMailRequest {
  /* 0000 */ int recipientId;
  /* 0004 */ std::string recipient;
  /* 001C */ std::string title;
  /* 0034 */ std::string body;
  /* 004C */ int zeny;
};
}

class ICashEmotionAction {
public:
  virtual ~ICashEmotionAction();
  virtual bool Execute(int packId, int argument) = 0;

  bool ExecuteNamed(const char* name);
};

class CCashEmotionAction_Play : public ICashEmotionAction {
public:
  CCashEmotionAction_Play();
  ~CCashEmotionAction_Play() override;
  bool Execute(int packId, int emotionId) override;

protected:
  /* 0004 */ int m_diceEmotion;
  /* 0008 */ int m_yutEmotion;
};

#pragma pack(push, 1)
struct GameModePacketHeader {
  /* 0000 */ std::uint16_t packetId;
};

struct GameModePacketByte {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint8_t argument;
};

struct GameModePacketWord {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t argument;
};

struct GameModePacketDword {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t argument;
};

struct GameModePacketDwordByte {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t argument;
  /* 0006 */ std::uint8_t flag;
};

struct GameModePacketDwordByteByte {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t argument;
  /* 0006 */ std::uint8_t firstFlag;
  /* 0007 */ std::uint8_t secondFlag;
};

struct GameModePacketDwordByteWord {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t dwordArgument;
  /* 0006 */ std::uint8_t byteArgument;
  /* 0007 */ std::uint16_t wordArgument;
};

struct GameModePacketByteInt64 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint8_t argument;
  /* 0003 */ std::int64_t value;
};

struct GameModePacketDwordDwordByte {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t firstArgument;
  /* 0006 */ std::uint32_t secondArgument;
  /* 000A */ std::uint8_t flag;
};

struct GameModePacketDwordDwordDword {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t firstArgument;
  /* 0006 */ std::uint32_t secondArgument;
  /* 000A */ std::uint32_t thirdArgument;
};

struct GameModePacketPayload24 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint8_t payload[24];
};

struct GameModePacketText100 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ char text[100];
};

struct GameModePacketDwordPayload24 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t argument;
  /* 0006 */ std::uint8_t payload[24];
};

struct GameModePacketDwordText60Text120 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t argument;
  /* 0006 */ char firstText[60];
  /* 0042 */ char secondText[120];
};

struct GameModePacketWordWord {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t firstArgument;
  /* 0004 */ std::uint16_t secondArgument;
};

struct GameModePacketWordDword {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t wordArgument;
  /* 0004 */ std::uint32_t dwordArgument;
};

struct GameModePacketWordWordWord {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t firstArgument;
  /* 0004 */ std::uint16_t secondArgument;
  /* 0006 */ std::uint16_t trailingValue;
};

struct GameModePacketByteWord {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t wordArgument;
  /* 0004 */ std::uint8_t byteArgument;
};

struct GameModePacketByte3 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint8_t firstByte;
  /* 0003 */ std::uint8_t secondByte;
  /* 0004 */ std::uint8_t thirdByte;
};

struct GameModePacketDwordWord {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t dwordArgument;
  /* 0006 */ std::uint16_t wordArgument;
};

struct GameModePacketDwordDword {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t firstArgument;
  /* 0006 */ std::uint32_t secondArgument;
};

struct GameModePacketDwordWordDword {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t value;
  /* 0006 */ std::uint16_t firstArgument;
  /* 0008 */ std::uint32_t secondArgument;
};

struct GameModePacketWordText16 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t argument;
  /* 0004 */ char text[16];
};

struct GameModePacketText16Words {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ char text[16];
  /* 0012 */ std::uint16_t firstArgument;
  /* 0014 */ std::uint16_t secondArgument;
};

struct GameModePacketC5 {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint32_t argument;
  /* 0006 */ std::uint8_t flag;
};

struct GameModePacketBB {
  /* 0000 */ std::uint16_t packetId;
  /* 0002 */ std::uint16_t argument1;
  /* 0004 */ std::uint8_t argument2;
};
#pragma pack(pop)

static_assert(sizeof(GameModeReplayPacket) == 0xFE);

static_assert(sizeof(GameModeTextRecord) == 0x190);
static_assert(offsetof(GameModeTextRecord, buffer) == 0x1E);
static_assert(sizeof(GameModePacketByte) == 3);
static_assert(sizeof(GameModePacketWord) == 4);
static_assert(sizeof(GameModePacketDword) == 6);
static_assert(sizeof(GameModePacketDwordByte) == 7);
static_assert(sizeof(GameModePacketDwordByteByte) == 8);
static_assert(sizeof(GameModePacketDwordByteWord) == 9);
static_assert(sizeof(GameModePacketByteInt64) == 11);
static_assert(sizeof(GameModePacketDwordDwordByte) == 11);
static_assert(sizeof(GameModePacketDwordDwordDword) == 14);
static_assert(sizeof(GameModePacketPayload24) == 26);
static_assert(sizeof(GameModePacketText100) == 102);
static_assert(sizeof(GameModePacketDwordPayload24) == 30);
static_assert(sizeof(GameModePacketDwordText60Text120) == 186);
static_assert(sizeof(GameModePacketWordWord) == 6);
static_assert(sizeof(GameModePacketWordDword) == 8);
static_assert(sizeof(GameModePacketWordWordWord) == 8);
static_assert(sizeof(GameModePacketByteWord) == 5);
static_assert(sizeof(GameModePacketByte3) == 5);
static_assert(sizeof(GameModePacketDwordWord) == 8);
static_assert(sizeof(GameModePacketDwordDword) == 10);
static_assert(sizeof(GameModePacketDwordWordDword) == 12);
static_assert(sizeof(GameModePacketWordText16) == 20);
static_assert(sizeof(GameModePacketText16Words) == 22);
static_assert(sizeof(GameModePacketC5) == 7);
static_assert(sizeof(GameModePacketBB) == 5);

// GLOBAL: gl_24661
std::vector<ActiveSkillEffect> g_activeSkillEffects;

// OFFSET: 0x00CCF480
// FUNCTION: fn_21242
void CGameMode::AddPendingItemPickup(int value) {
  this->pickupReqItemNaidList.push_back(value);
}

// OFFSET: 0x00CCF530
// FUNCTION: fn_21243
bool CGameMode::IsOverWeight(bool notify) {
  int maxWeight = g_session.m_maxWeight;
  if (maxWeight < 1) {
    maxWeight = 1;
  }

  if (g_session.m_weight * 100 / maxWeight >= 90) {
    if (notify) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_ATTACK_OVERWEIGHT)), 255, 0, 0, 0);
    }

    return true;
  }

  return false;
}

// OFFSET: 0x00CCFCF0
// FUNCTION: fn_21247
__declspec(noinline) void CGameMode::CompleteGuildMemberStatusLookup(std::uint32_t id, const std::string& text) {
  auto it = this->guildMemberStatusCache.find(id);
  if (it != this->guildMemberStatusCache.end()) {
    this->SendMsg(MM_GUILD_MEMBER_STATUS_CHANGE, reinterpret_cast<int>(text.c_str()), it->second, 0, 0);
    this->guildMemberStatusCache.erase(it);
  }
}

// OFFSET: 0x00CD52F0
// FUNCTION: fn_21279
int CGameMode::GetSkillAttackRange(int id, int level) {
  if (id == 10000) {
    return 5;
  }

  int range = 1;
  CSkillInfo skill = g_session.LookupSkill(id);
  if (!skill.available) {
    skill = g_session.m_skillBundle.Pending(id);
    if (!skill.available) {
      return range;
    }

    level = skill.level;
  }

  int reduction = 0;
  g_session.m_hotkeyScript->Call("GetSkillAttackRange", "ddd>dd", id, level, skill.level, &range, &reduction);
  range += skill.range - reduction;
  return range;
}

// OFFSET: 0x00CD54D0
// FUNCTION: fn_21280
int CGameMode::TerrainHeight() const {
  if (!this->scene || !this->scene->terrain) {
    return 0;
  }

  return this->scene->terrain->height;
}

// OFFSET: 0x00CD54F0
// FUNCTION: fn_21281
int CGameMode::TerrainWidth() const {
  if (!this->scene || !this->scene->terrain) {
    return 0;
  }

  return this->scene->terrain->width;
}

// OFFSET: 0x00CD5550
// FUNCTION: fn_21283
std::string CGameMode::LocationText() {
  char text[120];
  std::memset(text, 0, sizeof(text));

  int column;
  int row;
  const auto* player = static_cast<const CRenderObject*>(this->scene->player);
  this->WorldCell(player->render.position.x, player->render.position.z, column, row);

  std::string name = this->mapName;
  if (name.size() > 4) {
    name.resize(name.size() - 4);
  }

  std::sprintf(text, "Location: %s ( %d , %d )", name.c_str(), column, row);
  std::string result(text);
  return result;
}

// OFFSET: 0x00CD5800
// FUNCTION: fn_21284
bool CGameMode::HasInventoryItem(int id) {
  const ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, id, 0);
  return item.identity[1] > 0 && item.quantity > 0;
}

// OFFSET: 0x00CD58B0
// FUNCTION: fn_21285
bool CGameMode::HasResurrectionItem() {
  ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, 7621, 0);
  const CWorld* world = this->scene;
  if (world->isPKZone || world->isSiegeMode || world->isEventPVPMode) {
    return false;
  }

  if (item.identity[1] > 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 6833, 0);
  if (item.identity[1] > 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 6316, 0);
  if (item.identity[1] > 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 6293, 0);
  if (item.identity[1] > 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 25310, 0);
  return item.identity[1] > 0 && item.quantity > 0;
}

// OFFSET: 0x00CD5CD0
// FUNCTION: fn_21287
void CGameMode::SendMousePointerState(int value) {
  if (this->mousePointer) {
    this->mousePointer->SendMsg(nullptr, AM_HIDEMOUSE, value, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CECE00
// FUNCTION: fn_21360
void CGameMode::ClearGroundAlphaCheck() {
  this->checkGroundAlpha = 0;
}

// OFFSET: 0x00CECE10
// FUNCTION: fn_21361
void CGameMode::SetDragIds(int source, int id) {
  if (id != -1) {
    this->drag.source = source;
    this->drag.id = static_cast<std::uint32_t>(id);
  }
}

// OFFSET: 0x00CED190
// FUNCTION: fn_21363
__declspec(noinline) void CGameMode::UpdatePetFriendlyNoticeIds(int id, bool add) {
  if (add == true) {
    this->petFriendlyNoticeIds.insert(id);
  } else {
    this->petFriendlyNoticeIds.erase(id);
  }
}

// OFFSET: 0x00CF0070
// FUNCTION: fn_21410
const char* UIWindowMgr::ChatInputText() {
  return this->chatInputText.c_str();
}

// OFFSET: 0x00CF0320
// FUNCTION: fn_21412
CPlayer* CGameMode::Player() {
  if (!this->scene) {
    return nullptr;
  }
  return this->scene->player;
}

// OFFSET: 0x00CF0330
// FUNCTION: fn_21413
MinimapMarker* CGameMode::FindMinimapMarker(std::uint32_t key) {
  const auto it = this->minimapMarkers.find(key);
  if (it == this->minimapMarkers.end()) {
    return nullptr;
  }
  return &it->second;
}

// OFFSET: 0x00CF05E0
// FUNCTION: fn_21415
std::string CGameMode::GetMapResource(unsigned int index) {
  if (!this->mapResources.empty() && index <= this->mapResources.size()) {
    return this->mapResources[index];
  }

  return "";
}

// OFFSET: 0x00CF06B0
// FUNCTION: fn_21416
float CGameMode::CameraDesiredYaw() {
  if (!this->camera) {
    return 0.0f;
  }
  return this->camera->camera.desiredYaw;
}

// OFFSET: 0x00CF06D0
// FUNCTION: fn_45499
int ConfirmGameModeActionB8() {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXCHANGED_FAILED)), 0xFF, 0, 0, 0);
    return 0;
  }
  g_modeMgr.gameMode->SendMsg(MM_COMMAND_HOMUN, 1, 0, 0, 0);
  return 0;
}

// OFFSET: 0x00CF0380
// FUNCTION: fn_21414
std::string CGameMode::GetActiveSkillEffectTimeText(int skillId, int* activeSkillId) {
  std::string output;
  *activeSkillId = skillId;

  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    const int effectId = static_cast<int>(effect.id);
    if (skillId != (effectId == 0x2A ? 0x0C : effectId)) {
      continue;
    }

    *activeSkillId = effectId;
    const std::uint32_t expiration = effect.expiration;
    if (expiration == 999999 || expiration == 0) {
      break;
    }

    int remainingSeconds;
    if (g_replayAnimationScaling != 0) {
      const int remainingMilliseconds = static_cast<int>(expiration) - static_cast<int>(Replay::Instance().elapsed);
      if (remainingMilliseconds < 1000) {
        remainingSeconds = 0;
      } else {
        remainingSeconds = remainingMilliseconds / 1000;
      }
    } else {
      remainingSeconds = static_cast<int>((expiration - timeGetTime()) / 1000);
    }

    char buffer[100];
    const int hours = remainingSeconds / 3600;
    if (hours != 0) {
      std::sprintf(buffer, "%d %s", hours, LookupMessage(MSI_TIME));
      output = buffer;
    }

    int secondsPerMinute = 60;
    const int remainder = remainingSeconds - hours * 3600;
    const int minutes = remainder / secondsPerMinute;
    const int seconds = remainder % secondsPerMinute;
    if (minutes != 0) {
      std::sprintf(buffer, " %d %s", minutes, LookupMessage(MSI_MINUTE));
      output = output + buffer;
    }
    if (seconds != 0) {
      std::sprintf(buffer, " %d %s", seconds, LookupMessage(MSI_SECOND));
      output = output + buffer;
    }

    break;
  }

  return output;
}

// OFFSET: 0x00CF0A80
// FUNCTION: fn_21420
bool CGameMode::IsActiveSkillEffectId(std::uint32_t id) {
  if (this->IsExcludedSkillEffectId(id)) {
    return false;
  }

  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00D07720
// FUNCTION: fn_21582
int CGameMode::IsExcludedSkillEffectId(std::uint32_t id) {
  return id == 0x28 || id == 0x2A;
}

// OFFSET: 0x00D07740
// FUNCTION: fn_21583
int CGameMode::SubD07740(int id, int defaultResult) {
  switch (id) {
  case JT_BARD:
  case JT_BARD_H:
  case JT_BARD_B:
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_TROUBADOUR:
    return 1;
  case JT_DANCER:
  case JT_DANCER_H:
  case JT_DANCER_B:
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
  case JT_TROUVERE:
    return 0;
  }

  return defaultResult;
}

// OFFSET: 0x00CEB8E0
// FUNCTION: fn_21352
void CGameMode::RecordChatRoomSettings() {
  if (Replay::Instance().state != 2) {
    return;
  }

  GameModeTextRecord record;
  std::memset(record.buffer, 0, sizeof(record.buffer));
  const std::uint16_t length = static_cast<std::uint16_t>(this->chatRoom.title.size() + offsetof(GameModeTextRecord, buffer) + this->chatRoom.password.size());
  const int titleLength = static_cast<int>(this->chatRoom.title.size());
  const int passwordLength = static_cast<int>(this->chatRoom.password.size());
  record.header = 0x038808AE;
  record.length = length;
  record.firstLength = titleLength;
  record.secondLength = passwordLength;
  record.roomOptionA = this->chatRoom.type;
  record.roomOptionB = this->chatRoom.memberCount;
  record.roomOptionC = this->chatRoom.memberLimit;
  record.roomOptionD = this->chatRoom.id;

  std::memcpy(record.buffer, this->chatRoom.title.data(), titleLength);
  std::memcpy(record.buffer + titleLength, this->chatRoom.password.data(), passwordLength);

  Replay::Instance().Record(length, &record, 0x2A, 0);
}

#pragma pack(push, 1)
struct GameModeChunkHeader {
  /* 0000 */ std::int16_t type;
  /* 0002 */ std::uint32_t size;
};
#pragma pack(pop)

static_assert(sizeof(GameModeChunkHeader) == 6);

// INLINED
__forceinline std::vector<unsigned char> MakeEmptyChunk(std::int16_t type) {
  std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader), 0);
  auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
  header->type = type;
  header->size = 0;
  return chunk;
}

// INLINED
template <class Record>
__forceinline std::vector<unsigned char> MakeRecordChunk(std::int16_t type, Record value) {
  std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader) + sizeof(Record));
  auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
  header->type = type;
  header->size = sizeof(Record);
  *reinterpret_cast<Record*>(chunk.data() + sizeof(GameModeChunkHeader)) = value;
  return chunk;
}

// INLINED
template <class Element>
__forceinline std::vector<unsigned char> MakeDataChunk(std::int16_t type, const std::vector<Element>& payload) {
  std::vector<unsigned char> chunk(payload.size() + sizeof(GameModeChunkHeader), 0);
  auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
  header->type = type;
  header->size = payload.size();
  if (payload.size() != 0) {
    std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), payload.data(), payload.size());
  }
  return chunk;
}

// OFFSET: 0x00CEC6A0
// FUNCTION: fn_21356
unsigned CGameMode::ReadReplayGroundSkills(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  unsigned offset = 6;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (17000 != header.type) {
    offset = 0xFFFFFFFFu;
  } else {
    while (offset < data.size()) {
      header = {};
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
      offset += 6;

      switch (header.type) {
      case 0xCD: {
        std::vector<unsigned char> record(0x10);
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->Zc_Skill_Entry(record.data());
        break;
      }
      case 0xCE: {
        std::vector<unsigned char> record(0x61);
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->Zc_Skill_Entry2(record.data());
        break;
      }
      case 0x4269:
        return offset;
      default:
        offset += header.size;
        break;
      }
    }
  }

  return offset;
}

// OFFSET: 0x00CEFE50
// FUNCTION: fn_21407
void UIWindowMgr::ClearChatInputText() {
  this->chatInputText.assign("", 0);
}

// GLOBAL: gl_24846
int g_followUpdateTick;

// GLOBAL: gl_24847
int g_followLostTick;

// OFFSET: 0x00CECB60
// FUNCTION: fn_21358
void CGameMode::SetFollowTicks(int updateTick, int lostTick) {
  g_followUpdateTick = updateTick != -1 ? updateTick : g_followUpdateTick;
  g_followLostTick = lostTick != -1 ? lostTick : g_followLostTick;
}

// OFFSET: 0x00CF0AC0
// FUNCTION: fn_21421
bool CGameMode::IsMenuWindowOpen() const {
  return g_windowMgr.LookupWindow(WID_LAPINEBOXWND) != nullptr || g_windowMgr.LookupWindow(WID_LAPINEUPGRADEBOXWND) != nullptr
      || g_windowMgr.LookupWindow(WID_ITEMREFORMWND) != nullptr || g_windowMgr.LookupWindow(WID_ENCHANTUI) != nullptr
      || g_windowMgr.LookupWindow(WID_RUNESYSTEMWND) != nullptr;
}

// OFFSET: 0x00CF0B30
// FUNCTION: fn_21422
bool CGameMode::ShowRestrictedNotice(int grade) {
  if (g_serviceMode == 3 || g_serviceType == ServiceGermany || grade < 2) {
    return false;
  }

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_SAME_SENTANCE)), 255, 0, 0, 0);
  g_windowMgr.chatInputText.assign("", 0);
  return true;
}

void __stdcall RecordWhisper(const void* info, const unsigned char* data);
int ConfirmGameModeAction96();

// OFFSET: 0x00CF10B0
// FUNCTION: fn_45487
int CGameMode::SendMsg(int event, int argument1, int argument2, int argument3, int argument4) {
  switch (event) {
    case MM_SHOWMOUSEPOINTER:
      this->SendMousePointerPosition(static_cast<float>(argument1), static_cast<float>(argument2), argument3);
      return 0;
    case MM_HIDEMOUSEPOINTER:
      this->SendMousePointerState(argument1);
      return 0;
    case MM_ITEMTHROW:
      if (!this->IsMenuWindowOpen() &&
          this->waitingWearEquipAck + 2000U <= timeGetTime() &&
          this->waitingUseItemAck == 0 && this->waitingItemThrowAck == 0 && argument1 != 0 &&
          this->isPlayerDead == 0) {
        this->waitingItemThrowAck = 1;
        const std::uint16_t packet[3] = {
          0x363, static_cast<std::uint16_t>(argument1), static_cast<std::uint16_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet[0]), packet);
      }
      this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    case MM_DISCONNECT_CHARACTER: {
      const GameModePacketDword packet{0xCC, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_DISCONNECT_ALL_CHARACTER: {
      const std::uint16_t packet = 0xCE;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_REQ_NEXT_SCRIPT: {
      const GameModePacketDword packet{0xB9, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CHOOSE_MENU: {
      const GameModePacketDwordByte packet{
        0xB8, static_cast<std::uint32_t>(argument2), static_cast<std::uint8_t>(argument1)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_RESTART: {
      const GameModePacketByte packet{0xB2, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_NUM_USER: {
      const std::uint16_t packet = 0xC1;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_REQ_EXIT_ROOM:
      if (g_windowMgr.chatRoomWnd != nullptr) {
        const std::uint16_t packet = 0xE3;
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      }
      return 0;
    case MM_PROCESS_TALK_TYPE:
      this->ExecuteTalkType(argument1, reinterpret_cast<std::string*>(argument2));
      return 0;
    case MM_REQ_ROLE_CHANGE: {
      char text[24];
      std::strcpy(text, reinterpret_cast<const char*>(argument1));
      GameModePacketDwordPayload24 packet;
      packet.packetId = ID_CZ_REQ_ROLE_CHANGE;
      packet.argument = static_cast<std::uint32_t>(argument2);
      std::memcpy(packet.payload, text, sizeof(text));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_EXPEL_MEMBER: {
      char text[24];
      std::strcpy(text, reinterpret_cast<const char*>(argument1));
      GameModePacketPayload24 packet;
      packet.packetId = ID_CZ_REQ_EXPEL_MEMBER;
      std::memcpy(packet.payload, text, sizeof(text));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CONCLUDE_EXCHANGE_ITEM: {
      const std::uint16_t packet = 0xEB;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_CANCEL_EXCHANGE_ITEM: {
      const std::uint16_t packet = 0xED;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_EXEC_EXCHANGE_ITEM: {
      const std::uint16_t packet = 0xEF;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_ADD_ITEM_TO_STORE: {
      g_session.FlagInventoryItem(static_cast<std::uint32_t>(argument1));
      const GameModePacketWordDword packet{
        g_session.m_buyingStoreMode != 0 ? 0x9E1 : 0x364,
        static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MOVE_ITEM_FROM_STORE_TO_BODY: {
      g_session.FlagCartItem(static_cast<std::uint32_t>(argument1));
      const GameModePacketWordDword packet{
        g_session.m_buyingStoreMode != 0 ? 0x9E2 : 0x365,
        static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CLOSE_STORE: {
      const std::uint16_t packet = g_session.m_buyingStoreMode != 0 ? 0x9BE : 0x193;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_CANCEL_LOCKON: {
      const std::uint16_t packet = 0x118;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_SELECT_WARPPOINT: {
      GameModePacketWordText16 packet{0x11B, static_cast<std::uint16_t>(argument2), {}};
      std::strncpy(packet.text, reinterpret_cast<const char*>(argument1), sizeof(packet.text));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REMEMBER_WARPPOINT: {
      const std::uint16_t packet = 0x11D;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_MOVE_ITEM_FROM_BODY_TO_CART:
      if (!this->IsMenuWindowOpen()) {
        g_session.FlagInventoryItem(static_cast<std::uint32_t>(argument1));
        const GameModePacketWordDword packet{
          0x126, static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_MOVE_ITEM_FROM_CART_TO_BODY:
      if (!this->IsMenuWindowOpen()) {
        g_session.FlagMerchantShopItem(static_cast<std::uint32_t>(argument1));
        const GameModePacketWordDword packet{
          0x127, static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_MOVE_ITEM_FROM_STORE_TO_CART:
      if (!this->IsMenuWindowOpen()) {
        g_session.FlagCartItem(static_cast<std::uint32_t>(argument1));
        const GameModePacketWordDword packet{
          g_session.m_buyingStoreMode != 0 ? 0x9E4 : 0x128,
          static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_MOVE_ITEM_FROM_CART_TO_STORE:
      if (!this->IsMenuWindowOpen()) {
        g_session.FlagMerchantShopItem(static_cast<std::uint32_t>(argument1));
        const GameModePacketWordDword packet{
          g_session.m_buyingStoreMode != 0 ? 0x9E3 : 0x129,
          static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_REQ_CARTOFF: {
      const GameModePacketByte packet{0xB35, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_CLOSESTORE: {
      const std::uint16_t packet = 0x12E;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_UPGRADE_SKILLLEVEL: {
      this->isReqUpgradeSkillLevel = 1;
      const GameModePacketWord packet{0x112, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MOVETO_MAP: {
      GameModePacketText16Words packet{
        0x140, {}, static_cast<std::uint16_t>(argument2), static_cast<std::uint16_t>(argument3)
      };
      std::strncpy(packet.text, reinterpret_cast<const char*>(argument1), sizeof(packet.text));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_WEAR_EQUIP_ARROW:
      if (!this->IsMenuWindowOpen() &&
          this->waitingWearEquipAck + 2000U <= timeGetTime() &&
          this->isPlayerDead == 0) {
        this->waitingWearEquipAck = timeGetTime();
        const GameModePacketWordDword packet{
          static_cast<std::uint16_t>(ID_CZ_REQ_WEAR_EQUIP_V5), static_cast<std::uint16_t>(argument1),
          static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_CLOSE_DIALOG: {
      this->isOnQuest = 0;
      g_windowMgr.CloseWindow(WID_SAYDIALOGWND);
      g_windowMgr.CloseWindow(WID_CHOOSEWND);
      const GameModePacketDword packet{0x146, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REFRESH_MUSIC:
      if (g_serviceOptions.Get(TT_MUSIC_ON_OFF) != 0) {
        if (g_soundMgr->secondaryEnabled == 0) {
          g_soundMgr->PlayStream(nullptr, 0, false);
        }
      } else {
        g_soundMgr->secondaryEnabled = 0;
        g_soundMgr->StopStreams();
      }
      return 0;
    case MM_ITEM_CREATE:
      if (argument1 != 0) {
        GameModePacketText100 packet;
        packet.packetId = ID_CZ_ITEM_CREATE_EX;
        std::memset(packet.text, 0, sizeof(packet.text));
        StringFormat(packet.text, sizeof(packet.text), static_cast<std::size_t>(-1), "%s",
                     reinterpret_cast<const char*>(argument1));
        GetRagConnection().Send(0x66, &packet);
      }
      return 0;
    case MM_CHANGE_GROUPEXPOPTION: {
      const GameModePacketDword packet{0x102, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_GUILD_MENU: {
      const GameModePacketDword packet{0x14F, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_OPEN_MEMBER_INFO: {
      const GameModePacketDword packet{0x157, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_JOIN_GUILD: {
      const GameModePacketDwordDword packet{
        0x16B, static_cast<std::uint32_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_GUILD_MENUINTERFACE: {
      const std::uint16_t packet = 0x14D;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_TESTPACKET: {
      const GameModePacketWord packet{0xF3, 2};
      GetRagConnection().Send(4, &packet);
      return 0;
    }
    case MM_REQ_GIVE_MANNER_POINT:
      if (g_serviceMode != 3) {
        int duration = argument3;
        if (duration < 0) {
          duration = 0;
        } else if (duration > 30000) {
          duration = 30000;
        }
        const GameModePacketDwordByteWord packet{
          0x149, static_cast<std::uint32_t>(argument1), static_cast<std::uint8_t>(argument2),
          static_cast<std::uint16_t>(duration)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_GUILD_NOTICE: {
      GameModePacketDwordText60Text120 packet;
      packet.packetId = ID_CZ_GUILD_NOTICE;
      packet.argument = static_cast<std::uint32_t>(argument1);
      if (g_inlineCharset == 0) {
        std::strncpy(packet.firstText, reinterpret_cast<const char*>(argument2), 0x3B);
        std::strncpy(packet.secondText, reinterpret_cast<const char*>(argument3), 0x77);
      } else {
        std::strcpy(packet.firstText, g_serverLanguage->Prefix(false));
        std::strncpy(packet.firstText + 3, reinterpret_cast<const char*>(argument2), 0x38);
        std::strcpy(packet.secondText, g_serverLanguage->Prefix(false));
        std::strncpy(packet.secondText + 3, reinterpret_cast<const char*>(argument3), 0x74);
      }
      packet.firstText[0x3B] = 0;
      packet.secondText[0x77] = 0;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_ITEMIDENTIFY: {
      const GameModePacketWord packet{0x178, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_DISCONNECT: {
      const std::uint16_t packet[2] = {0x18A, 0};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet[0]), packet);
      return 0;
    }
    case MM_REQ_ITEMCOMPOSITION: {
      const GameModePacketWordWord packet{
        0x17C, static_cast<std::uint16_t>(argument1), static_cast<std::uint16_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_HOSTILE_GUILD: {
      const GameModePacketDword packet{0x180, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_ITEMCOMPOSITION_LIST: {
      this->cardItemIndex = argument1;
      const GameModePacketWord packet{0x17A, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_ALLY_GUILD: {
      const GameModePacketDwordDword packet{
        0x172, static_cast<std::uint32_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_DELETE_RELATED_GUILD: {
      const GameModePacketDwordDword packet{
        0x183, static_cast<std::uint32_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_MAKING_ITEM: {
      GameModePacketDwordDwordDword packet{0x18E, 0, 0, 0};
      if (argument1 != 0) {
        const ITEM_INFO* materials = reinterpret_cast<const ITEM_INFO*>(argument2);
        packet.firstArgument = static_cast<std::uint32_t>(std::atoi(materials[0].itemId.c_str()));
        packet.secondArgument = static_cast<std::uint32_t>(std::atoi(materials[1].itemId.c_str()));
        packet.thirdArgument = static_cast<std::uint32_t>(std::atoi(materials[2].itemId.c_str()));
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_BEGIN_DRAG_FROM_METALPROCESSWND: {
      const char* itemId = reinterpret_cast<const char*>(argument1);
      this->drag.Clear();
      this->drag.source = 0xE;
      this->drag.name = itemId;
      this->drag.resource = g_session.ItemDragResource(itemId, 1);
      return 0;
    }
    case MM_ITEMPICKUP: {
      const GameModePacketDword packet{0x362, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQUEST_ACT:
      if (this->scene->player->isRiding != 0 && (argument1 == 0 || argument1 == 7)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_ATTACK_IN_RIDING_STATE)), 0xFF, 0, 0, 0);
      } else {
        const GameModePacketDwordByte packet{
          0x437, static_cast<std::uint32_t>(argument2), static_cast<std::uint8_t>(argument1)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_REQUEST_MOVE:
      if (!this->IsMenuWindowOpen()) {
        const GameModePacketByte3 packet{
          0x35F,
          static_cast<std::uint8_t>(argument1 >> 2),
          static_cast<std::uint8_t>(((static_cast<std::uint32_t>(argument2) >> 4) & 0x3F) |
                                    (static_cast<std::uint32_t>(argument1) << 6)),
          static_cast<std::uint8_t>(static_cast<std::uint32_t>(argument2) << 4)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_CHANGE_DIRECTION: {
      const GameModePacketByteWord packet{
        0x361, static_cast<std::uint16_t>(argument2), static_cast<std::uint8_t>(argument1)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_RESET_PARAMETER: {
      if (argument1 == 1) {
        g_session.m_skillBundle.ResetFirstLevelChange();
      }
      const std::uint16_t packet = 0x197;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_REQ_CHANGE_MAPTYPE: {
      const GameModePacketWordWordWord packet{
        0x198, static_cast<std::uint16_t>(argument1), static_cast<std::uint16_t>(argument2),
        static_cast<std::uint16_t>(argument3 != 0)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CHANGE_EFFECTSTATE: {
      const GameModePacketDword packet{0x19D, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_PET_RENAME: {
      GameModePacketPayload24 packet;
      packet.packetId = ID_CZ_RENAME_PET;
      const char* text = reinterpret_cast<const char*>(argument1);
      const std::size_t length = std::strlen(text);
      if (length < sizeof(packet.payload)) {
        std::memcpy(packet.payload, text, length + 1);
      } else {
        std::memcpy(packet.payload, text, sizeof(packet.payload) - 1);
        packet.payload[sizeof(packet.payload) - 1] = 0;
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_PETEGG_SELECT_FROM_LIST: {
      const GameModePacketWord packet{0x1A7, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_PETEGG_INFO: {
      const GameModePacketWord packet{0x1A8, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_PETDECEIVE_SHOOT: {
      const GameModePacketDword packet{0x19F, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_GIVE_MANNER_POINT_PLUS:
      this->SendMsg(MM_REQ_GIVE_MANNER_POINT, this->menuTargetAid, 0, argument1, 0);
      g_session.DeleteProhibitionLog(this->menuTargetAid, argument1);
      return 0;
    case MM_REQ_GIVE_MANNER_POINT_MINUS:
      this->SendMsg(MM_REQ_GIVE_MANNER_POINT, this->menuTargetAid, 1, argument1, 0);
      g_session.AddProhibitionLog(this->menuTargetAid, argument1);
      return 0;
    case MM_ARROW_FROM_LIST: {
      const GameModePacketDword packet{0x1AE, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_SELECT_CART:
      if (argument2 != 0) {
        const GameModePacketDwordByte packet{
          0x980, static_cast<std::uint32_t>(argument2), static_cast<std::uint8_t>(argument1)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      } else {
        const GameModePacketWord packet{0x1AF, static_cast<std::uint16_t>(argument1)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_REMOVE_TO_AID: {
      const GameModePacketDword packet{
        0x843, static_cast<std::uint32_t>(std::atoi(reinterpret_cast<const char*>(argument1)))
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_SHIFT_TO_GID: {
      GameModePacketPayload24 packet{0x1BB, {}};
      std::strcpy(reinterpret_cast<char*>(packet.payload), reinterpret_cast<const char*>(argument1));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_RECALL_AID: {
      const GameModePacketDword packet{
        0x842, static_cast<std::uint32_t>(std::atoi(reinterpret_cast<const char*>(argument1)))
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_RECALL_GID: {
      GameModePacketPayload24 packet{0x1BD, {}};
      std::strcpy(reinterpret_cast<char*>(packet.payload), reinterpret_cast<const char*>(argument1));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_SKILL_SELECT_RESPONSE: {
      const GameModePacketDwordWord packet{
        0x443, static_cast<std::uint32_t>(argument2), static_cast<std::uint16_t>(argument1)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_OPEN_EQUIPEDITEM_CHANGE: {
      const GameModePacketDwordDword packet{0x2D8, 0, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_BILLING_INFO: {
      const std::uint16_t packet = 0x1C0;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_SELECT_SPELL: {
      const GameModePacketDword packet{0x1CE, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_JOIN_COUPLE: {
      const GameModePacketDwordDwordDword packet{
        0x1E3,
        this->lastCoupleIds[0],
        this->lastCoupleIds[1],
        static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_JOIN_BABY: {
      const GameModePacketDwordDwordDword packet{
        0x1F7,
        this->lastCoupleIds[0],
        this->lastCoupleIds[1],
        static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REPAIRITEM_SELECT_FROM_LIST: {
      GameModePacketPayload24 packet;
      packet.packetId = ID_CZ_REQ_ITEMREPAIR2;
      if (argument1 == -1) {
        packet.payload[0] = 0xFF;
        packet.payload[1] = 0xFF;
      } else {
        std::memcpy(packet.payload, reinterpret_cast<const void*>(argument1), sizeof(packet.payload));
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_DORIDORI: {
      const std::uint16_t packet = 0x1E7;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_SEND_TEXT_TO_NPC:
      this->SendNpcInputText(
        argument1, reinterpret_cast<const char*>(argument2), reinterpret_cast<const char*>(argument3), argument4
      );
      return 0;
    case MM_PARTY_REQ_ACK_MASTER_TO_JOIN: {
      const GameModePacketDwordDwordByte packet{
        0xAF8, static_cast<std::uint32_t>(argument1),
        static_cast<std::uint32_t>(*reinterpret_cast<const int*>(argument3)),
        static_cast<std::uint8_t>(argument2 != 0)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_PARTY_REQ_MASTER_TO_JOIN: {
      const GameModePacketDwordDword packet{
        0xAE6, static_cast<std::uint32_t>(*reinterpret_cast<const int*>(argument2)),
        static_cast<std::uint32_t>(*reinterpret_cast<const int*>(argument1))
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_OPEN_SEEKPARTY_MEMBER:
      g_windowMgr.EnsureWindow(WID_SEEKPARTYMBWND);
      return 0;
    case MM_OPEN_SEEKPARTY_LIST:
      g_windowMgr.EnsureWindow(WID_SEEKPARTYLISTWND);
      return 0;
    case MM_GROUPINFO_CHANGE_V2: {
      const GameModePacketDwordByteByte packet{
        0x7D7, static_cast<std::uint32_t>(argument1), static_cast<std::uint8_t>(argument2),
        static_cast<std::uint8_t>(argument3)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_GROUP_CHANGE_MASTER: {
      g_adventurerAgencyMgr->RequestStop();
      const GameModePacketDword packet{0x7DA, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_PAUSE_REPLAY:
      this->UpdateReplayPause(true);
      Replay::Instance().SetState(7);
      return 0;
    case MM_RESUME_REPLAY:
      this->UpdateReplayPause(false);
      Replay::Instance().SetState(6);
      return 0;
    case MM_INCREASE_REPLAY_SPEED:
      Replay::Instance().IncreaseSpeed();
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->UpdateReplayActionSpeeds();
      return 0;
    case MM_DECREASE_REPLAY_SPEED:
      Replay::Instance().DecreaseSpeed();
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->UpdateReplayActionSpeeds();
      return 0;
    case MM_RESTART_REPLAY:
      g_loginStartMode = 0x19;
      Replay::Instance().SetState(6);
      g_modeMgr.Switch(0, "login.rsw");
      return 0;
    case MM_EXIT_REPLAY:
      g_loginStartMode = 0x1A;
      Replay::Instance().SetState(6);
      g_modeMgr.Switch(0, "login.rsw");
      return 0;
    case MM_REQ_CLOSE_BUYING_STORE: {
      const std::uint16_t packet = 0x815;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_TRYCOLLECTION: {
      const GameModePacketDword packet{0x8B5, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_RANKING: {
      const GameModePacketWord packet{0x97C, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CANCEL_VENDING_MAKE: {
      const std::uint16_t packet = 0xA8C;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_CLOSE_OFFLINE_STORE: {
      const GameModePacketDword packet{0xA80, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MEMORIALDUNGEON_COMMAND: {
      const GameModePacketDword packet{0x2CF, 3};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_RESET_SKILL: {
      g_session.m_skillBundle.ResetFirstLevelChange();
      const GameModePacketByte packet{0xBB1, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_OPEN_MAIL: {
      const GameModePacketByteInt64 packet{
        0x9EA, static_cast<std::uint8_t>(argument2), static_cast<std::int64_t>(argument1)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_DELETE_MAIL: {
      const GameModePacketByteInt64 packet{
        0x9F5, static_cast<std::uint8_t>(argument2), static_cast<std::int64_t>(argument1)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CONFIG_CALL: {
      const GameModePacketDwordDword packet{
        0x2D8, 5, argument1 == 0 ? 0x3C00u : 0u
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_JOIN_BABY: {
      const GameModePacketDword packet{0x1F9, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_DELETE_FRIEND: {
      const GameModePacketDwordDword packet{
        0x203, static_cast<std::uint32_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CHECK_ID: {
      GameModePacketPayload24 packet{0x213, {}};
      std::memcpy(packet.payload, reinterpret_cast<const void*>(argument1), sizeof(packet.payload));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      char text[256];
      std::sprintf(text, "[ %s ]", reinterpret_cast<const char*>(packet.payload));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x6EFA9B, 0, 0, 0);
      return 0;
    }
    case MM_RC_ID: {
      GameModePacketPayload24 packet{0x212, {}};
      std::memcpy(packet.payload, reinterpret_cast<const void*>(argument1), sizeof(packet.payload));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REFINEITEM_SELECT_FROM_LIST: {
      const GameModePacketDword packet{0x222, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_COMMAND_HOMUN: {
      const GameModePacketByte packet{0x22D, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MER_RENAME: {
      GameModePacketPayload24 packet;
      packet.packetId = ID_CZ_RENAME_MER;
      const char* text = reinterpret_cast<const char*>(argument1);
      const std::size_t length = std::strlen(text);
      if (length < sizeof(packet.payload)) {
        std::memcpy(packet.payload, text, length + 1);
      } else {
        std::memcpy(packet.payload, text, sizeof(packet.payload) - 1);
        packet.payload[sizeof(packet.payload) - 1] = 0;
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_COMMAND_MER: {
      const GameModePacketByte packet{0x29F, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_STARPLACE_ACCEPT: {
      const GameModePacketByte packet{0x254, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_GET_MAIL_LIST: {
      const std::uint16_t packet = 0x23F;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_RESET_MAIL_ITEM: {
      const GameModePacketWord packet{0x246, static_cast<std::uint16_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_RETURN_MAIL: {
      GameModePacketDwordPayload24 packet{0x273, static_cast<std::uint32_t>(argument1), {}};
      std::memcpy(packet.payload, reinterpret_cast<const void*>(argument2), sizeof(packet.payload));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_GET_MAIL_ITEM: {
      const GameModePacketDword packet{0x244, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_ITEM_FROM_LIST: {
      const GameModePacketDwordWord packet{
        0x25B, static_cast<std::uint32_t>(argument1), static_cast<std::uint16_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CANCEL_BUYING_STORE_MAKE: {
      const std::uint16_t packet = 0xA94;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_REQ_OPEN_MSGBOX_EXTEND_BODYITEM_SIZE: {
      const std::uint16_t packet = 0xB14;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_REQ_EXTEND_BODYITEM_SIZE: {
      const std::uint16_t packet = 0xB16;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_CLOSE_MSGBOX_EXTEND_BODYITEM_SIZE: {
      const std::uint16_t packet = 0xB19;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_OPEN_ACCOUNT_LIMITED_TOOL: {
      const std::uint16_t packet = 0xB4F;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_CLOSE_ACCOUNT_LIMITED_TOOL: {
      const std::uint16_t packet = 0xB51;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_ADVANCED_STATUS_CHANGE:
      if (this->waitingReqStatusAck == 0 && this->isPlayerDead == 0) {
        this->waitingReqStatusAck = 1;
        const GameModePacketWordWord packet{
          0xB24, static_cast<std::uint16_t>(argument1), static_cast<std::uint16_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_REQ_MOVE_GUILD_AGIT:
      if (argument1 != 0) {
        const GameModePacketByte packet{0xB28, static_cast<std::uint8_t>(argument1)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_REQ_AGIT_INVESTMENT:
      if (argument1 != 0) {
        const GameModePacketByte packet{0xB2C, static_cast<std::uint8_t>(argument1)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_REBIRTH: {
      const std::uint16_t packet = 0x292;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_RELEASE_QUEST_SH:
      this->isOnQuest = 0;
      g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
      g_windowMgr.CloseWindow(WID_ITEMCOLLECTIONWND);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND_MODE_1);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND_MODE_1);
      g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_2);
      return 0;
    case MM_REQ_HUNTING: {
      const std::uint16_t packet = 0x279;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_SETLOCKONMOUSE:
      this->lockOnMouse = argument1;
      if (argument1 == 0) {
        this->lastPcGid = 0;
        this->lastMonGid = 0;
      }
      return 0;
    case MM_REQ_STATUS:
      if (this->waitingReqStatusAck == 0) {
        this->waitingReqStatusAck = 1;
        if (this->isPlayerDead == 0) {
          const std::uint16_t packet = 0xBA;
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
        }
      }
      return 0;
    case MM_STATUS_CHANGE:
      if (this->waitingReqStatusAck == 0) {
        this->waitingReqStatusAck = 1;
        if (this->isPlayerDead == 0) {
          const GameModePacketBB packet{
            0xBB, static_cast<std::uint16_t>(argument1), static_cast<std::uint8_t>(argument2)
          };
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
        }
      }
      return 0;
    case MM_CANCEL_DRAG:
      if (g_replayAnimationScaling == 0 || this->replayDragState[0] == 0) {
        this->drag.source = 0;
        this->drag.id = 0;
        this->drag.quantity = 0;
        g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
      }
      return 0;
    case MM_QUERYRSWNAME:
      return reinterpret_cast<int>(this->mapName);
    case MM_PLAYER_EMOTION_END:
      this->isEmotionRequested = 0;
      return 0;
    case MM_ACK_SELL:
      if (this->isPlayerDead == 0) {
        const GameModePacketC5 packet{0xC5, static_cast<std::uint32_t>(argument1), 1};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_ACK_BUY:
      if (this->isPlayerDead == 0) {
        const GameModePacketC5 packet{0xC5, static_cast<std::uint32_t>(argument1), 0};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_DELETEPARTYPOSALL:
      this->partyPosList.clear();
      return 0;
    case MM_CANCEL_USE_SKILL:
      this->skillUseType = nullptr;
      this->skillId = 0;
      this->skillUseLevel = 0;
      g_pickInvert = 0;
      return 0;
    case MM_DELETEGUILDPOSALL:
      this->guildPosList.clear();
      return 0;
    case MM_GET_SKILL_USE_TYPE:
      return reinterpret_cast<int>(this->skillUseType);
    case MM_GET_SKILL_USE_LEVEL:
      return this->skillUseLevel;
    case MM_SEND_NUMBER_TO_NPC:
      if (this->scene->GetGameActorByAID(argument1) != nullptr) {
        const GameModePacketDwordDword packet{
          0x143, static_cast<std::uint32_t>(argument1), static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_INITWHISPERMENU:
      this->lastWhisperMenuCharacterName = reinterpret_cast<const char*>(argument1);
      this->actorMenuIds.clear();
      this->actorMenuIds.push_back(0xB);
      this->actorMenuIds.push_back(0x15);
      return 0;
    case MM_QUIT_TO_IDENTRY:
      g_charServerAccountIdPending = 0;
      g_loginStartMode = 3;
      g_modeMgr.Switch(0, "login.rsw");
      return 0;
    case MM_NPC_TRADE_QUIT: {
      const std::uint16_t packet = 0x9D4;
      GetRagConnection().Send(2, &packet);
      return 0;
    }
    case MM_NPC_MARKET_CLOSE: {
      const std::uint16_t packet = 0x9D8;
      GetRagConnection().Send(2, &packet);
      return 0;
    }
    case MM_CANCEL_WRITE_RODEX: {
      const std::uint16_t packet = 0xA03;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      g_session.RestoreSelectionItems();
      g_session.ClearItemQuantities();
      return 0;
    }
    case MM_NPC_BARTER_MARKET_CLOSE: {
      const std::uint16_t packet = 0xB12;
      GetRagConnection().Send(2, &packet);
      return 0;
    }
    case MM_NPC_EXPANDED_BARTER_MARKET_CLOSE: {
      const std::uint16_t packet = 0xB58;
      GetRagConnection().Send(2, &packet);
      return 0;
    }
    case MM_REQUEST_CASH_SHOP:
      if (IsNavigationActive()) {
        int value = argument1;
        if (value < 0 || value >= 0xB) {
          value = 0;
        }
        const GameModePacketDword packet{
          static_cast<std::uint16_t>(ID_CZ_SE_CASHSHOP_OPEN_V2), static_cast<std::uint32_t>(value)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_INVENTORY_TAB: {
      const GameModePacketBB packet{
        0x907, static_cast<std::uint16_t>(argument1), static_cast<std::uint8_t>(argument2 == 1)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MOVE_ITEM_TO_PERSONAL_FROM_STORAGE:
      if (!this->IsMenuWindowOpen()) {
        g_session.FlagMerchantShopItem(static_cast<std::uint32_t>(argument1));
        const GameModePacketDwordWordDword packet{
          static_cast<std::uint16_t>(ID_CZ_MOVE_ITEM_TO_PERSONAL), 2, static_cast<std::uint16_t>(argument1),
          static_cast<std::uint32_t>(argument2)
        };
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    case MM_ITEMTHROW_THROUGH_DROPCNTWND:
      this->SendMsg(MM_ITEMTHROW, static_cast<int>(this->drag.id), argument1, 0, 0);
      return 0;
    case MM_QUIT_GAME:
      this->SendMsg(MM_REQ_DISCONNECT, 0, 0, 0, 0);
      return 0;
    case 0x24:
    case 0x55:
    case 0x62:
    case 0x9F:
    case 0xA0:
    case 0xA6:
    case 0x127:
      return 0;
    case MM_CHATMSG: {
      if (g_windowMgr.chatInputText.empty() || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char text[0x16A];
      std::strcpy(text, g_windowMgr.chatInputText.c_str());
      if (g_forbiddenWords.Contains(text)) {
        std::string replacement;
        g_forbiddenWords.Mask(text, &replacement);
        g_windowMgr.chatInputText.assign(text);
      }
      if (this->lastChat == text) {
        ++this->sameChatRepeatCount;
      } else {
        this->sameChatRepeatCount = 0;
      }
      if (this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
        return 0;
      }
      this->lastChat.assign(text);
      char extendedPair[0x16A];
      char extendedByte[0x16A];
      char shortenedByte[0x16A];
      char shortenedPair[0x16A];
      std::memcpy(extendedPair, text, sizeof(text));
      const int length = static_cast<int>(std::strlen(text));
      if (length > 6) {
        if (length + 3 < 0x16A) {
          extendedPair[length + 2] = extendedPair[length];
          extendedPair[length + 3] = extendedPair[length + 1];
          extendedPair[length] = extendedPair[length - 2];
          extendedPair[length + 1] = extendedPair[length - 1];
        }
        std::memcpy(extendedByte, text, sizeof(text));
        if (length + 1 < 0x16A) {
          extendedByte[length + 1] = extendedByte[length];
          extendedByte[length] = extendedByte[length - 1];
        }
        std::memcpy(shortenedByte, text, sizeof(text));
        if (length + 1 < 0x16A) {
          shortenedByte[length - 1] = shortenedByte[length];
          shortenedByte[length] = shortenedByte[length + 1];
        }
        std::memcpy(shortenedPair, text, sizeof(text));
        if (length + 1 < 0x16A) {
          shortenedPair[length - 2] = shortenedPair[length];
          shortenedPair[length - 1] = shortenedPair[length + 1];
        }
      }
      std::uint32_t* chatTicks = this->recordChatTime;
      if (this->recordChatCount == 10) {
        for (int i = 0; i < 10; ++i) {
          this->recordChat[i] = this->recordChat[i + 1];
          chatTicks[i] = chatTicks[i + 1];
        }
      }
      this->recordChat[this->recordChatCount].assign(text);
      chatTicks[this->recordChatCount] = timeGetTime();
      ++this->recordChatCount;
      if (this->recordChatCount > 10) {
        this->recordChatCount = 10;
      }
      int matches = 0;
      for (int i = 0; i <= this->recordChatCount; ++i) {
        if (i == 10) {
          const std::uint32_t elapsed = timeGetTime() - chatTicks[0];
          if (elapsed != 0 && elapsed < 10000) {
            this->RequestAdminStatus();
            this->strikeCount = 0;
            break;
          }
          if (elapsed != 0 && elapsed < 30000) {
            for (int j = 0; j < this->recordChatCount; ++j) {
              if (this->recordChat[j].size() < 20) {
                break;
              }
              if (j == this->recordChatCount - 1) {
                g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("TOO FAST CHATTING!"), 0x6E96FF, 0, 0, 0);
                this->RequestAdminStatus();
                this->strikeCount = 0;
              }
            }
          }
          if (g_session.IsSuperNoviceClass(g_session.GetDisplayJob())) {
            char first[0x16A];
            char middle[0x400];
            char last[0x16A];
            if (g_serviceType == ServiceKorea) {
              std::sprintf(first, LookupMessage(MSI_SUPERNOVICE1));
              const char* suffix = LookupMessage(MSI_SUPERNOVICE3);
              const char* name = g_session.DecodePlayerName();
              const char* prefix = LookupMessage(MSI_SUPERNOVICE2);
              std::sprintf(middle, "%s %s%s", prefix, name, suffix);
              std::sprintf(last, LookupMessage(MSI_SUPERNOVICE4));
            } else {
              std::sprintf(first, GetMessageString(0x316));
              const char* suffix = GetMessageString(0x318);
              const char* name = g_session.DecodePlayerName();
              const char* prefix = GetMessageString(0x317);
              std::sprintf(middle, "%s %s%s", prefix, name, suffix);
              std::sprintf(last, GetMessageString(0x319));
            }
            if (this->recordChat[9] == last && this->recordChat[7] == first &&
                this->scene->player->render.state != 3) {
              if ((g_session.IsSuperNoviceJob(g_session.GetDisplayJob()) &&
                   g_session.m_level >= g_session.m_maxBaseLevel) ||
                  (g_session.IsSuperNovice2Job(g_session.GetDisplayJob()) &&
                   g_session.m_level >= g_session.m_maxBaseLevelExtend2)) {
                g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MSG_SUPERNOVICE_CHOPOKGI_CAPPEDLEVEL)), 0xFF, 0, 0, 0);
                break;
              }
              const std::uint16_t packet = 0x1ED;
              GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
            }
          }
        }
        if (length > 6 &&
            (this->recordChat[i] == text || this->recordChat[i] == extendedPair ||
             this->recordChat[i] == extendedByte || this->recordChat[i] == shortenedByte ||
             this->recordChat[i] == shortenedPair) &&
            chatTicks[0] < timeGetTime() && timeGetTime() - chatTicks[i] < 30000 && ++matches >= 3) {
          std::uint32_t* repeatTicks = this->strikeTime;
          if (this->strikeCount == 3) {
            repeatTicks[0] = repeatTicks[1];
            repeatTicks[1] = repeatTicks[2];
            this->strikeCount = 2;
          }
          repeatTicks[this->strikeCount] = timeGetTime();
          if (this->strikeCount >= 2 && repeatTicks[2] - repeatTicks[0] < 90000) {
            this->RequestAdminStatus();
            this->strikeCount = 0;
          } else if (this->strikeCount < 3) {
            ++this->strikeCount;
          }
          break;
        }
      }
      if (g_navigationLinkParser->HasUnownedItemLink(g_windowMgr.chatInputText) == true) {
        return 0;
      }
      char message[0x169]{};
      const char* input = g_windowMgr.chatInputText.c_str();
      const char* prefix = g_serverLanguage->Prefix(false);
      const char* name = g_session.DecodePlayerName();
      StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), "%s : %s%s", name, prefix, input);
      g_windowMgr.ClearChatInputText();
      const std::size_t size = std::strlen(message);
      unsigned char packet[0x16D];
      const std::uint16_t header[2] = {0xF3, static_cast<std::uint16_t>(size + 4)};
      std::memcpy(packet, header, sizeof(header));
      std::memcpy(packet + 4, message, size);
      GetRagConnection().Send(static_cast<int>(size + 4), packet);
      return 0;
    }
    case MM_REQ_SELL: {
      const int count = g_session.GetList4Size();
      const std::uint32_t size = 4U + static_cast<std::uint32_t>(count) * 4U;
      if (size > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t header[2] = {0xC9, static_cast<std::uint16_t>(size)};
      std::memcpy(packet, header, sizeof(header));
      for (int i = 0; i < count; ++i) {
        const ITEM_INFO item = g_session.GetItem1730At(i);
        const std::uint16_t entry[2] = {
          static_cast<std::uint16_t>(item.identity[1]), static_cast<std::uint16_t>(item.quantity)
        };
        std::memcpy(packet + 4 + i * 4, entry, sizeof(entry));
      }
      if (count != 0 && this->isPlayerDead == 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(size), packet);
      }
      g_session.ClearTradeItems();
      g_windowMgr.CloseWindow(WID_CHOOSESELLBUYWND);
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_ITEMSELLWND);
      return 0;
    }
    case MM_ADDPARTYPOS: {
      const std::uint32_t accountId = static_cast<std::uint32_t>(argument1);
      this->partyPosList[accountId].x = argument2;
      this->partyPosList[accountId].y = argument3;
      const MessengerMember member = g_session.PartyByAccount(accountId);
      this->partyPosList[accountId].color = member.color;
      return 0;
    }
    case MM_USING_SKILL: {
      if (g_replayAnimationScaling != 0) {
        return 0;
      }
      const auto* skill = reinterpret_cast<const CSkillInfo*>(argument1);
      if (argument3 == 0 && this->scene->player->isRiding != 0 &&
          !g_session.IsHomunculusSkill(skill->id) && !g_session.IsMercenarySkill(skill->id)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_USE_SKILL_IN_RIDING_STATE)), 0xFF, 0, 0, 0);
        g_session.m_automaticSkills.Complete(skill->id);
        return 0;
      }
      this->SendMsg(MM_CANCEL_USE_SKILL, 0, 0, 0, 0);
      this->ignoreSkillInterval = argument4 == -1;
      switch (skill->type) {
        case 1:
          this->SendMsg(MM_HANG_USE_SKILL, 2, argument1, argument2, 0);
          return 0;
        case 2:
          this->SendMsg(MM_HANG_USE_SKILL, 1, argument1, argument2, 0);
          return 0;
        case 4: {
          std::uint32_t target;
          if (g_session.IsHomunculusSkill(skill->id)) {
            target = g_session.m_homunAid;
          } else {
            const bool mercenary = g_session.IsMercenarySkill(skill->id) != 0;
            target = mercenary ? g_session.m_mercenaryAid : g_session.m_aid;
          }
          this->SendMsg(MM_USE_SKILL, skill->id, target, argument2, argument3);
          this->SendMsg(MM_HANG_USE_SKILL, 0, 0, 0, 0);
          return 0;
        }
        case 8:
          this->SendMsg(MM_HANG_USE_SKILL, 3, argument1, argument2, 0);
          return 0;
        case 16:
          this->SendMsg(MM_HANG_USE_SKILL, 4, argument1, argument2, 0);
          return 0;
        case 32:
          this->SendMsg(MM_HANG_USE_SKILL, 5, argument1, argument2, 0);
          return 0;
        default:
          return 0;
      }
    }
    case MM_REQ_JOIN_GUILD: {
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(argument1));
      if (actor == nullptr || actor->hasSp != 0) {
        return 0;
      }
      const GameModePacketDwordDwordDword packet{
        0x168, static_cast<std::uint32_t>(argument1), g_session.m_aid, g_session.m_gid
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_JOIN_GUILD2: {
      if (g_guildInfo.m_guildId == 0 || g_guildInfo.canInvite == 0) {
        return 0;
      }
      const char* name = reinterpret_cast<const char*>(argument1);
      if (std::strlen(name) >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      GameModePacketPayload24 packet;
      packet.packetId = ID_CZ_REQ_JOIN_GUILD2;
      std::strncpy(reinterpret_cast<char*>(packet.payload), name, sizeof(packet.payload));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      const std::string notice = FormatOwnedString(LookupMessage(MSI_REQ_JOIN_GUILD), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice.c_str()), 0xFFFF00, 0, 0, 0);
      return 0;
    }
    case MM_NPC_BARTER_MARKET_PURCHASE: {
      unsigned char packet[0x800]{};
      const int count = g_barterMarketItemMgr->GetAllItemCount();
      const std::uint32_t size = 4U + static_cast<std::uint32_t>(count) * 14U;
      if (size > sizeof(packet)) {
        return 0;
      }
      const std::uint16_t header[2] = {0xB0F, static_cast<std::uint16_t>(size)};
      std::memcpy(packet, header, sizeof(header));
      unsigned char* output = packet + sizeof(header);
      for (const BarterMarketItem& item : *g_barterMarketItemMgr->GetAllItems()) {
        const int id = std::atoi(item.name.c_str());
        const std::uint16_t index = static_cast<std::uint16_t>(g_session.FindInventoryIndexByItemId(item.field_2c));
        std::memcpy(output, &id, 4);
        std::memcpy(output + 4, &item.value, 4);
        std::memcpy(output + 8, &index, 2);
        std::memcpy(output + 10, &item.id, 4);
        output += 14;
      }
      if (count > 0 && this->isPlayerDead == 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(size), packet);
      }
      return 0;
    }
    case MM_ADDREGISTERPOS:
      this->actorPosList[static_cast<std::uint32_t>(argument1)].first = argument2;
      this->actorPosList[static_cast<std::uint32_t>(argument1)].second = argument3;
      return 0;
    case MM_ACK_EXCHANGE_ITEM: {
      this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      g_session.ClearTradeItems();
      const GameModePacketByte packet{0xE6, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_JOIN_GROUP: {
      const char* name = reinterpret_cast<const char*>(argument1);
      if (std::strlen(name) >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      GameModePacketPayload24 packet;
      packet.packetId = ID_CZ_PARTY_JOIN_REQ;
      std::strncpy(reinterpret_cast<char*>(packet.payload), name, sizeof(packet.payload));
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      if (g_session.partyLeader != 0) {
        const std::string message = std::string(name) + LookupMessage(MSI_CHARACTER_PARTY_INVITATION_TO_JOIN);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF00, 0, 0, 0);
      }
      return 0;
    }
    case MM_REQ_CHANGE_GUILD_POSITIONINFO: {
      if (g_guildInfo.isMaster == 0) {
        return 0;
      }
      auto& positions = g_guildInfo.pendingPositionChanges;
      const std::int16_t length = static_cast<std::int16_t>(4U + 40U * positions.size());
      if (length > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t packetId = 0x161;
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &length, sizeof(length));
      unsigned char* output = packet + 4;
      for (const auto& entry : positions) {
        const auto& position = entry.second;
        const int flags = (position.canInvite != 0 ? 1 : 0) | (position.canExpel != 0 ? 0x10 : 0) |
                          (position.canStorage != 0 ? 0x100 : 0);
        std::memcpy(output, &position.positionId, 4);
        std::memcpy(output + 4, &flags, 4);
        std::memcpy(output + 8, &position.id, 4);
        std::memcpy(output + 12, &position.tax, 4);
        std::strncpy(reinterpret_cast<char*>(output + 16), position.name.c_str(), 23);
        output[39] = 0;
        output += 40;
      }
      positions.clear();
      GetRagConnection().Send(length, packet);
      return 0;
    }
    case MM_GUILD_MEMBER_STATUS_CHANGE: {
      if (!g_serviceOptions.Get(TT_LOGINOUT_ON_OFF) || (argument2 != 0 && argument2 != 1)) {
        return 0;
      }
      char message[1024];
      std::sprintf(message, LookupMessage(argument2 == 0 ? MSI_GUILD_MEMBER_STATUS_OFFLINE : MSI_GUILD_MEMBER_STATUS_ONLINE), argument1);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 4, 0, 0);
      return 0;
    }
    case MM_CONFORM_MAKEGROUP: {
      if (g_forbiddenWords.Contains(g_session.partyRequestName.c_str())) {
        g_windowMgr.Confirm(LookupMessage(MSI_BAD_SENTANGE), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      unsigned char packet[28];
      const std::uint16_t packetId = 0x1E8;
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::strncpy(reinterpret_cast<char*>(packet + 2), g_session.partyRequestName.c_str(), 24);
      packet[26] = static_cast<std::uint8_t>(argument2);
      packet[27] = static_cast<std::uint8_t>(argument3);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), packet);
      return 0;
    }
    case MM_REQ_TRADE_BUYING_STORE: {
      g_session.CopyItems1760To1770();
      const int count = g_session.GetList10Size();
      const std::uint32_t size = 12U + static_cast<std::uint32_t>(count) * 8U;
      if (size > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t packetId = 0x819;
      const std::uint16_t length = static_cast<std::uint16_t>(size);
      std::memcpy(packet, &packetId, 2);
      std::memcpy(packet + 2, &length, 2);
      std::memcpy(packet + 4, &argument1, 4);
      std::memcpy(packet + 8, &argument2, 4);
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1760At(index);
        if (item.identity[1] != 0) {
          const std::uint16_t itemIndex = static_cast<std::uint16_t>(item.identity[1]);
          const int itemId = std::atoi(item.itemId.c_str());
          const std::uint16_t quantity = static_cast<std::uint16_t>(item.quantity);
          std::memcpy(packet + 12 + index * 8, &itemIndex, 2);
          std::memcpy(packet + 14 + index * 8, &itemId, 4);
          std::memcpy(packet + 18 + index * 8, &quantity, 2);
        }
      }
      if (count == 0) {
        return 0;
      }
      GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
      return 0;
    }
    case MM_START_REPLAY_RECORDING: {
      Replay::Instance().ResetFile();
      SYSTEMTIME time;
      GetLocalTime(&time);
      const std::uint32_t key0 = time.wYear | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(time.wMonth)) << 16) |
                                 (static_cast<std::uint32_t>(static_cast<std::uint8_t>(time.wDay)) << 24);
      const std::uint32_t key1 = (static_cast<std::uint32_t>(static_cast<std::uint8_t>(time.wHour)) << 8) |
                                 (static_cast<std::uint32_t>(static_cast<std::uint8_t>(time.wMinute)) << 16) |
                                 (static_cast<std::uint32_t>(static_cast<std::uint8_t>(time.wSecond)) << 24);
      int column;
      int row;
      this->WorldCell(this->scene->player->render.position.x, this->scene->player->render.position.z, column, row);
      g_windowMgr.InitializeWindowPolicy();
      char characterName[64]{};
      std::sprintf(characterName, "%s", g_session.DecodePlayerName());
      Replay::Instance().SetHeaderInfo(key0, key1, characterName, g_session.m_navigationPlayerMap, g_session.m_aid,
                                     column, row, g_session.m_playerDir, g_serviceType, g_serviceMode, g_session.GetCurrentSex());
      ReplayFileList::Instance().SetReplay(&Replay::Instance());
      int windowIds[0x100];
      const int windowCount = g_windowMgr.GetRenderedWindowIds(windowIds);
      const int result = ReplayFileList::Instance().Open(0x1F, windowCount, reinterpret_cast<int>(windowIds));
      if (result == 0) {
        Replay::Instance().SetState(1);
        return 0;
      }
      if (result == -2) {
        Replay::Instance().SetState(1);
        g_windowMgr.Confirm(LookupMessage(MSI_REPLAY_THESAMEFILEEXISTS), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      char message[50]{};
      std::sprintf(message, LookupMessage(MSI_REPLAY_RECORDSTART));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x62B0FF, 0, 0, 0);
      Replay::Instance().SetState(2);
      {
        auto records = g_navigationManager.WriteReplayState();
        Replay::Instance().WriteSection(0x18, records, 0xE);
      }
      {
        auto records = g_session.BuildCharacterReplayRecords();
        Replay::Instance().WriteSection(3, records, 0xE);
      }
      {
        auto records = g_session.BuildStatsReplayRecords();
        Replay::Instance().WriteSection(4, records, 0xE);
      }
      {
        auto records = BuildRenewQuestReplayRecords();
        Replay::Instance().WriteSection(6, records, 0xE);
      }
      {
        auto records = g_session.BuildMessengerReplayRecords();
        Replay::Instance().WriteSection(7, records, 0xE);
      }
      {
        auto records = g_session.BuildItemReplayRecords();
        Replay::Instance().WriteSection(8, records, 0xE);
      }
      {
        auto records = g_session.BuildCompanionReplayRecords();
        Replay::Instance().WriteSection(9, records, 0xE);
      }
      {
        auto records = g_session.BuildExtraReplayRecords();
        Replay::Instance().WriteSection(0xA, records, 0xE);
      }
      {
        auto records = g_session.BuildReservedReplayRecords();
        Replay::Instance().WriteSection(0xB, records, 0xE);
      }
      {
        auto records = BuildEntryQueueReplayRecords();
        Replay::Instance().WriteSection(0xC, records, 0xE);
      }
      {
        auto records = g_session.BuildShortcutReplayRecords();
        Replay::Instance().WriteSection(0xD, records, 0xE);
      }
      {
        auto records = g_session.BuildShopReplayRecords();
        Replay::Instance().WriteSection(0xE, records, 0xE);
      }
      {
        auto records = this->SaveNameCardState();
        Replay::Instance().WriteSection(0x16, records, 0xE);
      }
      {
        auto records = this->SaveActiveSkillEffects();
        Replay::Instance().WriteSection(0x12, records, 0xE);
      }
      {
        auto records = this->SaveActorStates();
        Replay::Instance().WriteSection(0xF, records, 0xE);
      }
      {
        auto records = this->SaveGroundItems();
        Replay::Instance().WriteSection(0x10, records, 0xE);
      }
      {
        auto records = this->WriteReplaySkillState();
        Replay::Instance().WriteSection(0x11, records, 0xE);
      }
      {
        auto records = this->SaveQuestState();
        Replay::Instance().WriteSection(0x17, records, 0xE);
      }
      {
        auto records = g_windowMgr.SaveReplaySettings();
        Replay::Instance().WriteSection(0x14, records, 0xE);
      }
      {
        auto records = g_windowMgr.SaveReplayWindows();
        Replay::Instance().WriteSection(0x13, records, 0xE);
      }
      {
        auto records = g_windowMgr.SaveReplayReserved();
        Replay::Instance().WriteSection(0x15, records, 0xE);
      }
      Replay::Instance().WriteHeader(0xB);
      Replay::Instance().WriteFinalSection();
      Replay::Instance().RecordCachedPackets();
      Replay::Instance().RestartClock();
      return 0;
    }
    case MM_DROP: {
      if (this->drag.source == 0) {
        return 0;
      }
      if (this->drag.source == 1) {
        if (!g_session.HasInventoryItem(this->drag.id) ||
            this->waitingWearEquipAck + 2000U > timeGetTime()) {
          this->drag.source = 0;
          return 0;
        }
      } else if (this->drag.source == 2 && !g_session.IsSlotItemIdentity(this->drag.id)) {
        this->drag.source = 0;
        return 0;
      }
      if (g_windowMgr.DropAt(argument1, argument2, &this->drag)) {
        return 0;
      }
      if (this->drag.source == 0x10 || this->drag.source == 7) {
        if (this->drag.skill) {
          UIWindow* window = g_windowMgr.LookupWindow(this->drag.source == 0x10 ? 0x9F : 0x24);
          if (window) {
            window->RecordDrop(argument1, argument2, this->drag, -1);
            window->DragAndDrop(argument1, argument2, reinterpret_cast<std::intptr_t>(&this->drag));
          }
        } else {
          ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, this->drag.id);
          if (item.identity[1] > 0) {
            g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
            g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CAN_DROP_ITEM_TO_ITEMWND)), 0xFF, 0, 0, 0);
          }
        }
        this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return 0;
      }
      if (this->drag.source == 1) {
        if (g_windowMgr.itemWnd == nullptr) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OPEN_ITEMWND_TO_THROW)), 0xFF, 0, 0, 0);
          this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
          return 0;
        }
        if (g_session.m_lockItemDrop) {
          this->drag.source = 0;
          return 0;
        }
      }
      if (this->drag.source == 0x1C) {
        UIWindow* window = g_windowMgr.LookupWindow(WID_REFININGWND);
        if (window) {
          static_cast<UIRefiningWnd*>(window)->Reset();
        }
        this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (this->drag.source == 0x21) {
        UIWindow* window = g_windowMgr.LookupWindow(WID_GRADEENCHANTWND);
        if (window) {
          static_cast<UIGradeEnchantWnd*>(window)->ResetItem();
        }
        this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (this->isOnQuest == 0 && !g_windowMgr.LookupWindow(WID_EXCHANGEUI) &&
          !g_windowMgr.equipWnd && !g_windowMgr.itemStoreWnd && !g_windowMgr.rodexComposeWnd &&
          !g_windowMgr.LookupWindow(WID_LAPINEBOXWND) && !g_windowMgr.LookupWindow(WID_LAPINEUPGRADEBOXWND) &&
          !g_windowMgr.LookupWindow(WID_REFININGWND) && !g_windowMgr.LookupWindow(WID_SWAPEQUIPMENTWND) &&
          !g_windowMgr.LookupWindow(WID_BARTERMARKETWND) && !g_windowMgr.LookupWindow(WID_GRADEENCHANTWND)) {
        if (this->drag.source != 1) {
          this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
          g_windowMgr.RetireTransientWindow();
          return 0;
        }
        if (this->ConfirmDraggedItemUse()) {
          return 0;
        }
        if (this->drag.quantity == 1) {
          this->SendMsg(MM_ITEMTHROW, this->drag.id, 1, 0, 0);
        } else {
          g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
          g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 0, 0, 0, 0);
        }
        g_windowMgr.RetireTransientWindow();
        return 0;
      }
      this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      if (g_windowMgr.LookupWindow(WID_SWAPEQUIPMENTWND)) {
        return 0;
      }
      if (g_windowMgr.equipWnd) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CLOSEEQUIPWND)), 0xFF, 0, 0, 0);
      }
      if (g_windowMgr.rodexComposeWnd) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UISCRIPT_ERROR_GUIDE_MSG)), 0xFF, 0, 0, 0);
      }
      g_windowMgr.RetireTransientWindow();
      return 0;
    }
    case MM_REQ_ENTER_ROOM: {
      UIWindow* window = g_windowMgr.chatRoomWnd;
      std::uint32_t& lastTick = this->chatRoomRequestTick;
      if (timeGetTime() > lastTick + 1000U) {
        this->isWaitingEnterRoom = 0;
      }
      if (window || this->isWaitingEnterRoom) {
        return 0;
      }
      this->isWaitingEnterRoom = 1;
      lastTick = timeGetTime();
      GameModePacketDwordDwordDword packet;
      packet.packetId = ID_CZ_REQ_ENTER_ROOM;
      packet.firstArgument = static_cast<std::uint32_t>(argument1);
      if (argument2) {
        char text[8];
        std::strcpy(text, reinterpret_cast<const char*>(argument2));
        std::memcpy(&packet.secondArgument, text, sizeof(text));
      } else {
        packet.secondArgument = 0;
        packet.thirdArgument = 0;
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_JOIN_GROUP: {
      if (argument2 == 0) {
        g_session.partyName.clear();
      } else {
        g_windowMgr.EnsureWindow(WID_MESSENGERGROUPWND);
      }
      const GameModePacketDwordByte packet{0x2C7, static_cast<std::uint32_t>(argument1), static_cast<std::uint8_t>(argument2 != 0)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_USESKILL_LEVEL_PLUS: {
      if (CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, this->skillId)) {
        CSkillInfo skill = g_session.m_skillBundle.Lookup(this->skillId);
        if (skill.available) {
          this->skillUseLevel += argument1;
          if (this->skillUseLevel > skill.level) {
            this->skillUseLevel = skill.level;
          }
          if (this->skillUseLevel <= 0) {
            this->skillUseLevel = 1;
          }
        }
      }
      return 0;
    }
    case MM_REQ_DISORGANIZE_GUILD: {
      if (g_guildInfo.isMaster == 0) {
        return 0;
      }
      GameModePacketText40 packet;
      packet.packetId = ID_CZ_REQ_DISORGANIZE_GUILD;
      const char* name = reinterpret_cast<const char*>(argument1);
      std::strncpy(packet.text, name, 39);
      packet.text[39] = 0;
      if (std::strcmp(name, g_guildInfo.guildName.c_str()) != 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_DISORGANIZE_GUILD_FAILURE), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_COMMAND_PET: {
      if (argument3 == 1) {
        const int foodId = g_session.GetPetFood(g_session.petJob);
        int quantity;
        {
          ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, foodId, 0);
          quantity = item.quantity;
        }
        if (quantity <= 0) {
          ITEM_INFO item;
          item.SetItemId(foodId);
          char name[260]{};
          int capacity = sizeof(name);
          item.GetIdDisplayName(name, capacity, 0);
          std::string text = FormatOwnedString(LookupMessage(MSI_NOT_EXIST_PET_FOOD), name);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
          return 0;
        }
      }
      const GameModePacketByte packet{0x1A1, static_cast<std::uint8_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_CANCEL_ITEMLISTWIN: {
      const std::uint32_t packet[3] = {0x000C07E4, static_cast<std::uint32_t>(argument1), 0};
      GetRagConnection().Send(12, packet);
      g_session.ClearTradeItems();
      return 0;
    }
    case MM_OPEN_BUYING_STORE_EXTENDED: {
      unsigned char packet[0x800]{};
      const int count = g_session.GetList11Size();
      const std::uint16_t packetId = 0xA92;
      const std::uint16_t length = static_cast<std::uint16_t>(96 + count * 10);
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &length, sizeof(length));
      std::memcpy(packet + 4, &argument2, sizeof(argument2));
      std::strncpy(reinterpret_cast<char*>(packet + 12), reinterpret_cast<const char*>(argument1), 79);
      packet[91] = 0;
      if (g_windowMgr.buyingShopMakeWnd) {
        const auto* window = static_cast<const UIMerchantShopMakeWnd*>(g_windowMgr.buyingShopMakeWnd);
        std::memcpy(packet + 92, &window->selectedRow, sizeof(window->selectedRow));
        std::memcpy(packet + 94, &window->selectedColumn, sizeof(window->selectedColumn));
      }
      if (!g_replayAnimationScaling) {
        g_session.storeTitle = reinterpret_cast<const char*>(packet + 12);
      }
      unsigned char* output = packet + 96;
      for (int index = 0; index < count; ++index) {
        ITEM_INFO item = g_session.GetItem1768At(index);
        if (item.identity[1] > 0) {
          const int itemId = std::atoi(item.itemId.c_str());
          const std::uint16_t quantity = static_cast<std::uint16_t>(item.quantity);
          std::memcpy(output, &itemId, sizeof(itemId));
          std::memcpy(output + 4, &quantity, sizeof(quantity));
          std::memcpy(output + 6, &item.prices[1], sizeof(item.prices[1]));
          output += 10;
        }
      }
      GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
      return 0;
    }
    case MM_DELETEREGISTERPOS:
      this->actorPosList.erase(static_cast<std::uint32_t>(argument1));
      return 0;
    case MM_REQ_LEAVE_GROUP: {
      if (g_session.memorialDungeonState == 1 || g_session.memorialDungeonState == 2) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MUST_PASSWORD_EXCEED_CHAR)), 0xFF, 0, 0, 0);
      }
      const int count = g_session.PartyCount();
      if (g_session.partyLeader != 0) {
        if (count > 1) {
          bool online = false;
          bool transferred = false;
          for (int index = 0; index < count; ++index) {
            const MessengerMember member = g_session.PartyAt(index);
            if (member.accountId != g_session.m_aid && member.offline == 0) {
              online = true;
              if (member.map == this->mapName) {
                g_modeMgr.gameMode->SendMsg(MM_GROUP_CHANGE_MASTER, member.accountId, 0, 0, 0);
                transferred = true;
                break;
              }
            }
          }
          if (!transferred) {
            if (online) {
              g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEFAULT)), 0xFF, 0, 0, 0);
              return 0;
            }
            if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_PROPERTY_FREE_NO_COUNT), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
              return 0;
            }
          }
        } else if (count == 1) {
          g_adventurerAgencyMgr->RequestStop();
        }
      }
      if (g_windowMgr.partyInvitationToWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_PARTYINVITATIONTOWND);
      }
      const std::uint16_t packet = 0x100;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_USE_SKILL: {
      if (g_windowMgr.LookupWindow(WID_COLLECTIONSYSTEMWND) != nullptr) {
        g_session.m_automaticSkills.Complete(argument1);
        return 0;
      }
      if (argument4 == 0 && this->scene->player->isRiding != 0 &&
          !g_session.IsHomunculusSkill(argument1) && !g_session.IsMercenarySkill(argument1)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_USE_SKILL_IN_RIDING_STATE)), 0xFF, 11, 0, 0);
        g_session.m_automaticSkills.Complete(argument1);
        return 0;
      }
      if (this->IsMenuWindowOpen()) {
        return 0;
      }
      this->RecordReplayMessage(event, argument1, argument2, argument3);
      if (argument1 == 10000) {
        if (static_cast<std::uint32_t>(argument2) == g_session.petGid) {
          return 0;
        }
        const CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(argument2));
        if (actor != nullptr && actor->objectType == PC_TYPE) {
          return 0;
        }
        g_windowMgr.EnsureWindow(WID_PETTAMINGDECEIVEWND)->SendMsg(nullptr, UM_SET_TARGET, 0, argument2, 0, 0);
        return 0;
      }
      if (argument1 == 20000) {
        const GameModePacketDword packet{0x1E5, static_cast<std::uint32_t>(argument2)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
        return 0;
      }
      if (argument1 == 20001) {
        char message[260];
        std::sprintf(message, "[ (val1 == 20001) ]");
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x6E96FF, 0, 0, 0);
        const GameModePacketDword packet{0x1F9, static_cast<std::uint32_t>(argument2)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
        return 0;
      }
      if (argument1 == 40000) {
        if (static_cast<std::uint32_t>(argument2) != g_session.petGid) {
          g_windowMgr.EnsureWindow(WID_COLLECTIONSYSTEMWND)->SendMsg(nullptr, UM_SET_TARGET, 0, argument2, 0, 0);
        }
        return 0;
      }
      if (g_windowMgr.merchantItemMyShopWnd != nullptr) {
        return 0;
      }
      if (this->ignoreSkillInterval == 0 && !g_session.IsRequestReady(argument1)) {
        char message[260];
        const char* name = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, argument1);
        std::sprintf(message, LookupMessage(MSI_SKILLINTERVAL2), name);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFF, 11, 0, 0);
        g_session.m_automaticSkills.Complete(argument1);
        return 0;
      }
      if (argument3 == 0) {
        const CSkillInfo skill = g_session.LookupSkill(argument1);
        if (skill.available != 0) {
          argument3 = skill.level;
        } else {
          const CSkillInfo pending = g_session.m_skillBundle.Pending(argument1);
          argument3 = pending.available != 0 ? pending.level : 0;
        }
      }
      if ((!g_session.IsHomunculusSkill(argument1) && !g_session.IsMercenarySkill(argument1) &&
           this->scene->player->render.state == 6) || g_windowMgr.chatRoomWnd != nullptr) {
        g_session.m_automaticSkills.Complete(argument1);
        return 0;
      }
      if (argument1 == 154 || argument1 == 41) {
        if (g_session.m_sp < (argument1 == 154 ? 40 : 30)) {
          char message[256];
          std::sprintf(message, LookupMessage(MSI_USESKILL_FAIL_SP_INSUFFICIENT));
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFF, 11, 0, 0);
          return 0;
        }
        if (g_session.replayPacketMode <= 0) {
          char message[256];
          std::sprintf(message, LookupMessage(MSI_MUST_EQUIP_CART));
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFF, 11, 0, 0);
          return 0;
        }
        if (argument1 == 154) {
          g_windowMgr.EnsureWindow(WID_SELECTCARTWND);
        }
      }
      const std::uint16_t id = static_cast<std::uint16_t>(argument1);
      const std::uint16_t level = static_cast<std::uint16_t>(argument3);
      if (g_session.m_automaticSkills.Contains(id)) {
        g_session.m_automaticSkills.Use(static_cast<std::int16_t>(id), static_cast<std::int16_t>(level), argument2);
        return 0;
      }
      unsigned char packet[10];
      const std::uint16_t packetId = 0x438;
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &level, sizeof(level));
      std::memcpy(packet + 4, &id, sizeof(id));
      std::memcpy(packet + 6, &argument2, sizeof(argument2));
      if (id == 0x5D) {
        g_session.m_monsterInfoActorId = argument2;
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), packet);
      return 0;
    }
    case MM_SHOW_IMAGE: {
      const std::string text(reinterpret_cast<const char*>(argument1));
      this->miniMapTooltipId = argument2;
      this->miniMapTooltipText = text;
      if (text.empty() && argument2 == -1 && argument3 == 0) {
        this->miniMapTooltipId = 11;
      }
      LowercaseText(this->miniMapTooltipText);
      if (this->miniMapTooltipText.find(".bmp") == std::string::npos &&
          this->miniMapTooltipText.find(".png") == std::string::npos) {
        this->miniMapTooltipText.append(".bmp", 4);
      }
      this->UpdateMiniMapTooltip();
      return 0;
    }
    case MM_ADDGUILDPOS:
      this->guildPosList[static_cast<std::uint32_t>(argument1)].x = argument2;
      this->guildPosList[static_cast<std::uint32_t>(argument1)].y = argument3;
      this->guildPosList[static_cast<std::uint32_t>(argument1)].color = 0xFFFFA6D2;
      return 0;
    case MM_NPC_MARKET_PURCHASE: {
      unsigned char packet[2048]{};
      const int count = g_session.GetList2Size();
      const std::uint32_t length = 4U + static_cast<std::uint32_t>(count) * 8U;
      if (length > sizeof(packet)) {
        return 0;
      }
      const std::uint16_t packetId = 0x9D6;
      const std::uint16_t packetLength = static_cast<std::uint16_t>(length);
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &packetLength, sizeof(packetLength));
      if (count > 0) {
        for (int index = 0; index < count; ++index) {
          const ITEM_INFO item = g_session.GetItem1720At(index);
          const int id = std::atoi(item.itemId.c_str());
          std::memcpy(packet + 4 + index * 8, &id, sizeof(id));
          std::memcpy(packet + 8 + index * 8, &item.quantity, sizeof(item.quantity));
        }
        if (this->isPlayerDead == 0) {
          GetRagConnection().Send(static_cast<std::int16_t>(packetLength), packet);
        }
        g_windowMgr.LookupWindow(WID_PARAITEMPURCHASEWND)->Show(0);
        g_windowMgr.CloseWindow(WID_ITEMCOLLECTIONWND);
      } else {
        g_windowMgr.CloseWindow(WID_PARAITEMPURCHASEWND);
        g_modeMgr.gameMode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      }
      g_session.ClearTradeItems();
      g_windowMgr.CloseWindow(WID_PARAITEMSHOPWND);
      return 0;
    }
    case MM_SWAP_EQUIPMENT: {
      if (timeGetTime() - g_session.m_swapEquipmentTick < 10000U) {
        const char* suffix = LookupMessage(MSI_CONNECT_SNS_FAIL_CONNECT);
        const std::string message = FormatOwnedString("%d%s", 10U - (timeGetTime() - g_session.m_swapEquipmentTick) / 1000U, suffix);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xC8FF00, 7, 0, 0);
        return 0;
      }
      g_session.m_swapEquipmentTick = timeGetTime();
      const std::uint16_t packet = 0xA9C;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_STOVE_AUTHENTICATION: {
      if (g_stoveMgr->m_pendingOperation != 0) {
        return 0;
      }
      g_stoveMgr->m_pendingOperation = argument1;
      unsigned char packet[610]{};
      const std::uint16_t packetId = 0xC0F;
      std::memcpy(packet, &packetId, sizeof(packetId));
      const std::string memberNumber(g_stoveMgr->m_memberNumber);
      const unsigned long long member = std::stoull(memberNumber, nullptr, 10);
      std::memcpy(packet + 2, &member, sizeof(member));
      StringFormat(reinterpret_cast<char*>(packet + 10), 600, static_cast<std::size_t>(-1), g_stoveMgr->GetAccessToken());
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), packet);
      return 0;
    }
    case MM_TAKE_OFF_EQUIP: {
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI) != nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_USE_WHEN_OPENED_EXCHANGEWND)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (this->IsMenuWindowOpen() ||
          this->waitingTakeoffEquipAck + 2000U > timeGetTime() ||
          this->isPlayerDead != 0) {
        return 0;
      }
      ITEM_INFO item(0);
      item = argument2 != 0 ? g_session.GetCostumeItemAt(argument1) : g_session.GetEquippedItemAt(argument1);
      if (item.identity[1] == 0 || item.quantity <= 0) {
        return 0;
      }
      this->waitingTakeoffEquipAck = timeGetTime();
      const GameModePacketWord packet{0xAB, static_cast<std::uint16_t>(item.identity[1])};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 1;
    }
    case MM_TOGGLE_PLAYERGAGE: {
      if (this->itemWindow != nullptr) {
        g_windowMgr.Schedule(this->itemWindow);
        this->itemWindow = nullptr;
        return 0;
      }
      UIPlayerGage* gauge = new UIPlayerGage;
      this->itemWindow = gauge;
      gauge->SetFirstValues(
        [] { return static_cast<std::int64_t>(g_session.hp); },
        [] { return static_cast<std::int64_t>(g_session.maxHp); }
      );
      gauge->SetSecondValues(
        [] { return static_cast<std::int64_t>(g_session.m_sp); },
        [] { return static_cast<std::int64_t>(g_session.m_maxSp); }
      );
      if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
        gauge->SetFourthValues(
          [] { return static_cast<std::int64_t>(g_session.GetTraitStat(0xE8)); },
          [] { return static_cast<std::int64_t>(g_session.GetTraitStat(0xE9)); }
        );
        gauge->SetSize(60, 13);
      } else {
        gauge->SetFourthValues({}, {});
        gauge->SetSize(60, 9);
      }
      CGameActor* player = this->scene->player;
      gauge->Move(player->render.screenX - 30,
        static_cast<int>(static_cast<float>(ScaleByScreenHeight(12)) * player->render.distanceScale +
                         static_cast<float>(player->render.screenY)));
      g_windowMgr.AddRenderedFront(gauge);
      return 0;
    }
    case MM_HANG_USE_SKILL: {
      if (g_windowMgr.LookupWindow(WID_COLLECTIONSYSTEMWND) != nullptr || g_windowMgr.merchantItemMyShopWnd != nullptr) {
        return 0;
      }
      this->skillUseType = reinterpret_cast<void*>(argument1);
      if (argument2 == 0) {
        return 0;
      }
      const CSkillInfo* skill = reinterpret_cast<const CSkillInfo*>(argument2);
      this->skillId = skill->id;
      this->skillAttackRange = skill->range;
      if (CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, skill->id)) {
        this->skillUseLevel = argument3;
      } else {
        const CSkillInfo learned = g_session.LookupSkill(this->skillId);
        this->skillUseLevel = learned.available != 0 ? learned.level : 0;
      }
      g_pickInvert = IsSupportSkill(this->skillId);
      return 0;
    }
    case MM_MAKE_SKILL_USED_MSG: {
      bool allowed = argument3 >= 0 && argument1 >= 0;
      switch (argument1) {
        case MC_IDENTIFY:
        case MC_VENDING:
        case TF_HIDING:
        case AS_CLOAKING:
        case ST_CHASEWALK:
        case GC_CLOAKINGEXCEED:
        case RA_CAMOUFLAGE:
        case SC_INVISIBILITY:
        case SR_CRESCENTELBOW_AUTOSPELL:
        case ALL_BUYING_STORE:
        case KO_YAMIKUMO:
        case ABC_CHAIN_REACTION_SHOT_ATK:
        case ABC_FROM_THE_ABYSS_ATK:
          allowed = false;
          break;
      }
      CGameActor* actor = reinterpret_cast<CGameActor*>(argument2);
      if (g_session.IsMonsterActor(actor) == 1) {
        allowed = true;
      }
      if (actor == nullptr || !allowed) {
        return 0;
      }
      char text[64];
      char skillName[64];
      if (argument1 == WE_MALE || argument1 == WE_FEMALE) {
        const std::string actorName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(argument3);
        std::strcpy(text, actorName.c_str());
        std::strcpy(skillName, CSkillInfo::DisplayName(*g_session.m_hotkeyScript, argument1));
        if (g_serviceType == ServiceKorea) {
          std::strcat(skillName, " \xA2\xBD");
        }
        std::strcat(text, " !!  ");
        std::strcat(text, LookupMessage(MSI_LOVE_SKILL));
        const int address = reinterpret_cast<int>(text);
        actor->SendMsg(nullptr, AM_CHAT, address, 0, 0, 0, 0, 0);
        return 0;
      }
      if (argument1 == WE_CALLPARTNER) {
        std::sprintf(text, "%s", this->coupleName);
        std::strcpy(skillName, CSkillInfo::DisplayName(*g_session.m_hotkeyScript, argument1));
        if (g_serviceType == ServiceKorea) {
          std::strcat(skillName, " \xA2\xBD");
        }
        std::strcat(text, " !!  ");
        std::strcat(text, skillName);
        const int address = reinterpret_cast<int>(text);
        actor->SendMsg(nullptr, AM_CHAT, address, 0, 0, 0, 0, 0);
        return 0;
      }
      if (argument1 == DC_SCREAM || argument1 == BA_FROSTJOKER || argument1 == ITEM_ENCHANTARMS ||
          (g_session.IsMonsterActor(actor) == 1 && actor->render.visible == 0)) {
        return 0;
      }
      std::strcpy(skillName, CSkillInfo::DisplayName(*g_session.m_hotkeyScript, argument1));
      std::strcat(skillName, " !!");
      const int address = reinterpret_cast<int>(skillName);
      actor->SendMsg(nullptr, AM_CHAT, address, 0, 0, 0, 0, 0);
      return 0;
    }
    case MM_REQ_CHANGE_MEMBERPOS: {
      if (g_guildInfo.isMaster == 0) {
        return 0;
      }
      const std::int16_t length = static_cast<std::int16_t>(4U + 12U * g_guildInfo.pendingMemberPositions.size());
      if (length > 2048) {
        return 0;
      }
      unsigned char packet[2048];
      const std::uint16_t header[2] = {0x155, static_cast<std::uint16_t>(length)};
      std::memcpy(packet, header, sizeof(header));
      unsigned char* output = packet + sizeof(header);
      for (const auto& entry : g_guildInfo.pendingMemberPositions) {
        std::memcpy(output, &entry.second, sizeof(entry.second));
        output += sizeof(entry.second);
      }
      g_guildInfo.pendingMemberPositions.clear();
      GetRagConnection().Send(length, packet);
      return 0;
    }
    case MM_REQ_ALLY_GUILD: {
      const GameModePacketDwordDwordDword packet{
        0x170, static_cast<std::uint32_t>(argument1), g_session.m_aid, g_session.m_gid
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_ADD_MAIL_ITEM: {
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen()) {
        return 0;
      }
      const GameModePacketWordWord packet{
        0xA04, static_cast<std::uint16_t>(argument1), static_cast<std::uint16_t>(argument2)
      };
      g_session.m_requestedCommandValue = static_cast<std::int16_t>(argument2);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_OPEN_WRITE_RODEX: {
      GameModePacketPayload24 packet{0xA08, {}};
      if (argument1 != 0) {
        const std::string text(reinterpret_cast<const char*>(argument1));
        if (text.size() >= 24) {
          g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
        std::memcpy(packet.payload, text.c_str(), 24);
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_WEAR_EQUIP: {
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI) != nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_USE_WHEN_OPENED_EXCHANGEWND)), 255, 0, 0, 0);
        return 0;
      }
      if (this->IsMenuWindowOpen()) {
        return 0;
      }
      if (g_windowMgr.barterMarketWnd != nullptr || g_windowMgr.expandedBarterMarketWnd != nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LIMIT_HAVE_ITEM_CNT)), 255, 0, 0, 0);
        return 0;
      }
      if (this->waitingWearEquipAck + 2000U > timeGetTime() || this->isPlayerDead != 0) {
        return 0;
      }
      const EquipSlots slots = g_session.GetEquipSlots(argument2);
      const ITEM_INFO equipped = g_session.GetEquippedItemAt(slots.values[2]);
      if (equipped.damaged != 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DAMAGED_ITEM)), 255, 0, 0, 0);
        return 0;
      }
      if (equipped.identity[1] > 0 && equipped.quantity == 1) {
        g_session.GetEquipSlots(equipped.identity[3]);
      }
      const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(argument1));
      if (item.bindOnEquip == 1) {
        UIWindow* window = g_windowMgr.EnsureWindow(WID_YOURITEMWND);
        this->waitingWearEquipAck = timeGetTime();
        window->SendMsg(nullptr, UM_SET_BABY_MSG, argument1, argument2, 0, 0);
      } else {
        this->waitingWearEquipAck = timeGetTime();
        const GameModePacketWordDword packet{0x998, static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    }
    case MM_REQ_EXCHANGE_ITEM: {
      if (this->IsMenuWindowOpen()) {
        return 0;
      }
      if (g_serviceType == ServiceIndonesia && g_windowMgr.exchangeAcceptWnd != nullptr) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_DEALING)), 255, 0, 0, 0);
        return 0;
      }
      this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      g_session.ClearTradeItems();
      const std::string name = this->ActorName(argument1);
      if (!name.empty()) {
        this->exchangeName = name;
        const GameModePacketDword packet{0xE4, static_cast<std::uint32_t>(argument1)};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    }
    case MM_QUAKE:
      if (g_serviceOptions.Get(TT_QUAKE_ON_OFF)) {
        this->camera->Start(1, argument1 == 1 || argument1 >= 650 ? argument1 : 0, 1.0f, 0.24f, 0.2f);
      }
      return 0;
    case MM_SETQUAKEINFO:
      this->camera->SetAmplitudes(*reinterpret_cast<const float*>(argument1), *reinterpret_cast<const float*>(argument2), *reinterpret_cast<const float*>(argument3));
      return 0;
    case MM_PET_ACT: {
      if (argument1 == -1 || (argument1 % 10 < 2 && g_session.m_petFriendly / 10 < 90 && !g_session.IsAdminSubMode(3))) {
        return 0;
      }
      const GameModePacketDword packet{0x1A9, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_NPC_MARKET_PURCHASE_V2: {
      unsigned char packet[0x800]{};
      const int count = g_session.GetList2Size();
      const std::uint32_t length = static_cast<std::uint32_t>(count) * 10U + 4U;
      if (length > sizeof(packet)) {
        return 0;
      }
      const std::uint16_t header[2]{0xAAA, static_cast<std::uint16_t>(length)};
      std::memcpy(packet, header, sizeof(header));
      if (count > 0) {
        for (int index = 0; index < count; ++index) {
          const ITEM_INFO item = g_session.GetItem1720At(index);
          const std::int32_t itemId = std::atoi(item.itemId.c_str());
          const std::uint16_t identity = static_cast<std::uint16_t>(item.identity[1]);
          unsigned char* entry = packet + 4 + index * 10;
          std::memcpy(entry, &itemId, sizeof(itemId));
          std::memcpy(entry + 4, &item.quantity, sizeof(item.quantity));
          std::memcpy(entry + 8, &identity, sizeof(identity));
        }
        if (this->isPlayerDead == 0) {
          GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
        }
        if (UIWindow* window = g_windowMgr.LookupWindow(WID_PARAITEMPURCHASEWND)) {
          window->Show(0);
        }
        g_windowMgr.CloseWindow(WID_ITEMCOLLECTIONWND);
      } else {
        g_windowMgr.CloseWindow(WID_PARAITEMPURCHASEWND);
        if (g_modeMgr.gameMode != nullptr) {
          g_modeMgr.gameMode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        }
      }
      g_session.ClearTradeItems();
      g_windowMgr.CloseWindow(WID_PARAITEMSHOPWND);
      return 0;
    }
    case MM_REQ_CLICK_TO_BUYING_STORE: {
      if ((this->scene != nullptr && this->scene->player != nullptr && this->scene->player->render.state == 3) ||
          static_cast<std::uint32_t>(argument1) == g_session.m_aid || g_windowMgr.buyingShopItemsWnd != nullptr) {
        return 0;
      }
      std::uint32_t& lastRequest = this->merchantShopRequestTick;
      if (timeGetTime() <= lastRequest + 1000U) {
        return 0;
      }
      lastRequest = timeGetTime();
      const GameModePacketDword packet{0x817, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_PRISON_USER: {
      if (!IsAdminAccount(g_session.m_aid) && !IsMonsterJob(g_session.m_aid)) {
        return 0;
      }
      const char* name = reinterpret_cast<const char*>(argument1);
      const std::size_t length = strnlen(name, 24);
      if (length - 1 > 22) {
        return 0;
      }
      std::uint16_t packet[25]{};
      packet[0] = 0xB34;
      std::strncpy(reinterpret_cast<char*>(packet + 1), g_session.DecodePlayerName(), 24);
      std::strncpy(reinterpret_cast<char*>(packet + 13), name, 24);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet[0]), packet);
      return 0;
    }
    case MM_CHAT_ALLY: {
      if (std::strlen(g_windowMgr.chatInputText.c_str()) == 0 || g_guildInfo.m_guildId == 0 || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char filtered[0x16A];
      strncpy_s(filtered, sizeof(filtered), g_windowMgr.chatInputText.c_str(), static_cast<std::size_t>(-1));
      if (g_forbiddenWords.Contains(filtered)) {
        std::string masked;
        g_forbiddenWords.Mask(filtered, &masked);
        g_windowMgr.chatInputText.assign(filtered, std::strlen(filtered));
        return 0;
      }
      if (this->lastChat == filtered) {
        ++this->sameChatRepeatCount;
      } else {
        this->sameChatRepeatCount = 0;
      }
      if (this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
        return 0;
      }
      this->lastChat.assign(filtered, std::strlen(filtered));
      if (g_navigationLinkParser->HasUnownedItemLink(std::string(g_windowMgr.chatInputText.c_str()))) {
        return 0;
      }
      char text[0x169]{};
      const char* message = g_windowMgr.chatInputText.c_str();
      const char* prefix = g_serverLanguage->Prefix(false);
      const char* name = g_session.DecodePlayerName();
      const char separator[4]{' ', ':', ' ', 0};
      const char* parts[4]{name, separator, prefix, message};
      std::size_t length = 0;
      for (const char* part : parts) {
        const std::size_t available = 0x168 - length;
        const std::size_t partLength = std::strlen(part);
        const std::size_t copied = partLength < available ? partLength : available;
        std::memcpy(text + length, part, copied);
        length += copied;
      }
      text[length] = 0;
      g_windowMgr.ClearChatInputText();
      length = std::strlen(text);
      unsigned char packet[0x16C];
      const std::uint16_t header[2]{0xBDD, static_cast<std::uint16_t>(length + 4)};
      std::memcpy(packet, header, sizeof(header));
      std::memcpy(packet + 4, text, length);
      GetRagConnection().Send(header[1], packet);
      return 0;
    }
    case MM_WHISPERMSG: {
      if (std::strlen(g_windowMgr.chatInputText.c_str()) == 0 || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char text[0x152];
      std::strncpy(text, g_windowMgr.chatInputText.c_str(), 0x151);
      text[0x151] = 0;
      if (g_forbiddenWords.Contains(text)) {
        std::string replacement("");
        g_forbiddenWords.Mask(text, &replacement);
        g_windowMgr.chatInputText.assign(text, std::strlen(text));
      }
      if (g_pendingWhisperCount == 0) {
        if (this->lastChat == text) {
          ++this->sameChatRepeatCount;
        } else {
          this->sameChatRepeatCount = 0;
        }
      }
      if (g_session.m_autoReplyEnabled == 0 && this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
        return 0;
      }
      this->lastChat.assign(text, std::strlen(text));
      char target[25] = {};
      std::strcpy(target, reinterpret_cast<const char*>(argument1));
      this->lastWhisperName.assign(target, std::strlen(target));
      if (std::strcmp(this->lastWhisperName.c_str(), g_session.DecodePlayerName()) != 0) {
        char message[0x152];
        if (g_inlineCharset != 0) {
          std::strcpy(message, g_serverLanguage->Prefix(false));
          std::strncpy(message + 3, g_windowMgr.chatInputText.c_str(), 0x14E);
        } else {
          std::strncpy(message, g_windowMgr.chatInputText.c_str(), 0x151);
        }
        message[0x151] = 0;
        this->lastWhisper.assign(message, std::strlen(message));
        const std::size_t length = std::strlen(message);
        if (length - 1 <= 0x14F) {
          g_windowMgr.ClearChatInputText();
          RecordWhisper(target, reinterpret_cast<const unsigned char*>(message));
          unsigned char packet[0x16D];
          const std::uint16_t packetId = 0x96;
          const std::uint16_t packetLength = static_cast<std::uint16_t>(length + 0x1C);
          std::memcpy(packet, &packetId, sizeof(packetId));
          std::memcpy(packet + 2, &packetLength, sizeof(packetLength));
          std::memcpy(packet + 4, target, 24);
          std::memcpy(packet + 0x1C, message, length);
          GetRagConnection().Send(packetLength, packet);
          return 0;
        }
      }
      g_windowMgr.ClearChatInputText();
      return 0;
    }
    case MM_RELEASE_QUEST:
      this->isOnQuest = 0;
      if (argument1 == 0 && g_windowMgr.choose3Wnd != nullptr) {
        g_windowMgr.choose3Wnd->SendMsg(nullptr, UM_CANCEL_NPC_CHOICE, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_CHOOSEWND);
      }
      if (g_windowMgr.sayDialogWnd != nullptr) {
        if (CMode* mode = g_modeMgr.GetGameMode()) {
          mode->SendMsg(MM_CLOSE_DIALOG, 0, 0, 0, 0);
        }
      }
      g_windowMgr.CloseWindow(WID_CHOOSESELLBUYWND);
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
      g_windowMgr.CloseWindow(WID_ITEMSELLWND);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
      g_windowMgr.CloseWindow(WID_ITEMCOLLECTIONWND);
      g_windowMgr.CloseWindow(WID_ITEMPARAMCHANGEDISPLAYWND);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND_MODE_1);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND_MODE_1);
      g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_2);
      g_windowMgr.CloseWindow(WID_PARAITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_PARARESULTWND);
      g_windowMgr.CloseWindow(WID_PARAITEMPURCHASEWND);
      g_windowMgr.CloseWindow(WID_BARTERMARKETWND);
      g_windowMgr.CloseWindow(WID_BARTERITEMPURCHASEWND);
      g_windowMgr.CloseWindow(WID_EXPANDEDBARTERMARKETWND);
      g_windowMgr.CloseWindow(WID_EXPANDEDBARTERITEMPURCHASEWND);
      this->SendMsg(MM_SHOW_IMAGE, reinterpret_cast<int>(""), -1, 0, 0);
      return 0;
    case MM_CREATE_CHATROOM: {
      const ChatRoomSetting& setting = *reinterpret_cast<const ChatRoomSetting*>(argument1);
      this->chatRoom.title = setting.title;
      this->chatRoom.type = setting.isPublic;
      this->chatRoom.memberCount = setting.memberCount;
      this->chatRoom.memberLimit = setting.limit;
      this->chatRoom.id = setting.roomId;
      this->chatRoom.password = setting.password;
      unsigned char packet[0x800];
      const std::uint16_t packetId = 0xD5;
      const std::uint16_t packetLength = static_cast<std::uint16_t>(setting.title.size() + 15);
      const std::uint16_t limit = static_cast<std::uint16_t>(setting.limit);
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &packetLength, sizeof(packetLength));
      std::memcpy(packet + 4, &limit, sizeof(limit));
      packet[6] = static_cast<unsigned char>(setting.isPublic);
      std::memset(packet + 7, 0, 8);
      std::memcpy(packet + 7, setting.password.c_str(), setting.password.size());
      std::memcpy(packet + 15, setting.title.c_str(), setting.title.size());
      this->RecordChatRoomSettings();
      GetRagConnection().Send(static_cast<std::int16_t>(packetLength), packet);
      return 0;
    }
    case MM_DELETEPARTYPOS: {
      const auto found = this->partyPosList.find(static_cast<std::uint32_t>(argument1));
      if (found != this->partyPosList.end()) {
        this->partyPosList.erase(found);
      }
      return 0;
    }
    case MM_REQ_BAN_GUILD: {
      if (g_guildInfo.isMaster == 0 && g_guildInfo.canExpel == 0) {
        return 0;
      }
      unsigned char packet[54];
      const std::uint16_t packetId = 0x15B;
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, reinterpret_cast<const void*>(argument1), 12);
      char* text = reinterpret_cast<char*>(packet + 14);
      if (g_inlineCharset != 0) {
        std::strcpy(text, g_serverLanguage->Prefix(false));
        std::strncpy(text + 3, reinterpret_cast<const char*>(argument2), 0x24);
      } else {
        std::strncpy(text, reinterpret_cast<const char*>(argument2), 0x27);
      }
      text[0x27] = 0;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), packet);
      return 0;
    }
    case MM_JOIN_FRIEND: {
      if (argument3 != 0) {
        g_windowMgr.EnsureWindow(WID_MESSENGERGROUPWND)->SendMsg(nullptr, UM_COMMAND, ID_RADIOBTN, 1, 0, 0);
      }
      const GameModePacketDwordDwordDword packet{
        0x208, static_cast<std::uint32_t>(argument1), static_cast<std::uint32_t>(argument2),
        static_cast<std::uint32_t>(argument3)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_HOMUN_WAIT: {
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(
          g_session.m_homunAid));
      if (actor != nullptr && actor->HasActiveHomunculusAI()) {
        AiMessage message;
        message.type = 9;
        actor->SetMsg2AI(&message, 0);
      }
      return 0;
    }
    case MM_REQ_BUY_CASH_POINT: {
      if (g_session.GetList2Size() == 0 || this->isPlayerDead != 0) {
        g_session.ClearTradeItems();
        return 0;
      }
      const int count = g_session.GetList2Size();
      const std::uint32_t length = static_cast<std::uint32_t>(count) * 6U + 10U;
      if (length > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t packetId = 0x288;
      const std::uint16_t packetLength = static_cast<std::uint16_t>(length);
      const std::uint16_t itemCount = static_cast<std::uint16_t>(count);
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &packetLength, sizeof(packetLength));
      std::memcpy(packet + 4, &argument1, sizeof(argument1));
      std::memcpy(packet + 8, &itemCount, sizeof(itemCount));
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1720At(index);
        const std::uint16_t quantity = static_cast<std::uint16_t>(item.quantity);
        const int itemId = std::atoi(item.itemId.c_str());
        std::memcpy(packet + 10 + index * 6, &quantity, sizeof(quantity));
        std::memcpy(packet + 12 + index * 6, &itemId, sizeof(itemId));
      }
      if (count != 0 && this->isPlayerDead == 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(packetLength), packet);
      }
      g_session.ClearTradeItems();
      return 0;
    }
    case MM_NPC_EXPANDED_BARTER_MARKET_PURCHASE: {
      unsigned char packet[0x800] = {};
      const int count = g_expandedBarterMarketItemMgr->GetEntryCount();
      const std::uint32_t length = static_cast<std::uint32_t>(count) * 12U + 4U;
      if (length > sizeof(packet)) {
        return 0;
      }
      const std::uint16_t packetId = 0xB57;
      const std::uint16_t packetLength = static_cast<std::uint16_t>(length);
      std::memcpy(packet, &packetId, sizeof(packetId));
      std::memcpy(packet + 2, &packetLength, sizeof(packetLength));
      unsigned char* output = packet + 4;
      for (const BarterMarketEntry& entry : *g_expandedBarterMarketItemMgr->GetEntries()) {
        const int name = std::atoi(entry.name.c_str());
        std::memcpy(output, &name, sizeof(name));
        std::memcpy(output + 4, &entry.id, sizeof(entry.id));
        std::memcpy(output + 8, &entry.value, sizeof(entry.value));
        output += 12;
      }
      if (count > 0 && this->isPlayerDead == 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(packetLength), packet);
      }
      return 0;
    }
    case MM_USEITEM: {
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_USE_WHEN_OPENED_EXCHANGEWND)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (this->IsMenuWindowOpen() || this->waitingUseItemAck != 0 || this->waitingItemThrowAck != 0 || argument1 == 0 || this->isPlayerDead != 0) {
        return 0;
      }
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, argument1);
      const std::uint32_t state = this->scene->player->GetStateFlags();
      const int bodyState = this->scene->player->bodyState;
      if (StateFlag01(state) || ((bodyState == 4 || bodyState == 1) && item.GetItemId() != 0x31B5)) {
        return 0;
      }
      if (this->scene->player->render.state == 6 &&
          (item.GetItemId() == 0x259 || item.GetItemId() == 0x25A || item.GetItemId() == 0x263 ||
           item.GetItemId() == 0x25D || item.GetItemId() == 0x25E || item.GetItemId() == 0x5813)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_USE_WHEN_SITDOWN)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (Singleton<CSelectPackageItemMgr>::GetInstance()->OpenItem(argument1)) {
        return 0;
      }
      if (item.GetItemId() == 0x19295) {
        if (g_session.IsPlayerPerformerJob(g_session.m_job)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GENDER_CHANGE_MESSAGE_CONFIRM)), 0xFF, 0, 0, 0);
          return 0;
        }
        if (this->scene->player->isRiding != 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_REWARD_HEAVY_INVENTORY)), 0xFF, 0, 0, 0);
          return 0;
        }
        const unsigned baseJob = JobNames::BaseJob(g_session.GetDisplayJob());
        if (g_session.GetDisplayJob() != baseJob) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_REWARD_HEAVY_INVENTORY)), 0xFF, 0, 0, 0);
          return 0;
        }
        if (this->scene->player->GetGdid() > 0 || g_session.PartyCount() > 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GENDER_CHANGE_MESSAGE_OK)), 0xFF, 0, 0, 0);
          return 0;
        }
        if (g_session.m_genderChangeBlocked) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GENDER_CHANGE_MESSAGE_CANCEL)), 0xFF, 0, 0, 0);
          return 0;
        }
        for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
          if (effect.id == 0x26D) {
            g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GENDER_CHANGE_MESSAGE_COMPLETE)), 0xFF, 0, 0, 0);
            return 0;
          }
        }
        if (g_windowMgr.Confirm(LookupMessage(MSI_MACRO_CHECKER_MAP_NOT_GM), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_CHECKER_MAP_IS_COOLTIME)), 0xFF, 0, 0, 0);
          return 0;
        }
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_CHECKER_MAP_BANNED_MAP)), 0, 0, 0, 0);
      }
      this->waitingUseItemAck = 1;
      const GameModePacketWordDword packet{0x439, static_cast<std::uint16_t>(argument1), g_session.m_aid};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MAKE_GROUP: {
      if (g_session.partyBlocked != 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOPARTY)), 0x6E96FF, 0, 0, 0);
        return 0;
      }
      if (g_session.PartyCount() != 0) {
        return 0;
      }
      const char* name = reinterpret_cast<const char*>(argument1);
      if (_mbsstr(reinterpret_cast<const unsigned char*>(name), reinterpret_cast<const unsigned char*>("#"))) {
        g_windowMgr.Confirm(LookupMessage(MSI_INVENTORY_ITEM_COMPARE), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      if (std::strlen(name) >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
      g_windowMgr.EnsureWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
      g_session.partyRequestName = name;
      return 0;
    }
    case MM_REQ_EXPEL_GROUP_MEMBER: {
      if (g_session.partyLeader == 0) {
        return 0;
      }
      GameModePacketDwordPayload24 packet;
      packet.packetId = ID_CZ_REQ_EXPEL_GROUP_MEMBER;
      std::strncpy(reinterpret_cast<char*>(packet.payload), reinterpret_cast<const char*>(argument1), 24);
      const MessengerMember member = g_session.PartyByName(reinterpret_cast<const char*>(argument1));
      packet.argument = member.accountId;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_USE_SKILL_TOGROUND_WITH_TALKBOX: {
      const int* request = reinterpret_cast<const int*>(argument1);
      std::uint16_t packet[16];
      packet[0] = 0x367;
      packet[1] = static_cast<std::uint16_t>(request[3]);
      packet[2] = static_cast<std::uint16_t>(request[0]);
      packet[3] = static_cast<std::uint16_t>(request[1]);
      packet[4] = static_cast<std::uint16_t>(request[2]);
      if (this->ignoreSkillInterval == 0 && !g_session.IsRequestReady(packet[2])) {
        char text[1024];
        const char* skillName = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, packet[2]);
        std::sprintf(text, LookupMessage(MSI_SKILLINTERVAL2), skillName);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 11, 0, 0);
        return 0;
      }
      char* text = reinterpret_cast<char*>(packet + 5);
      _snprintf_s(text, 21, _TRUNCATE, "%s", reinterpret_cast<const char*>(request[4]));
      if (g_forbiddenWords.Contains(text)) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_SEND_BECAUSE_INSULT)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (packet[1] == 0) {
        packet[1] = static_cast<std::uint16_t>(g_session.m_skillBundle.Lookup(packet[2]).level);
      }
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet[0]), packet);
      return 0;
    }
    case MM_CHAT_BATTLEFIELD: {
      if (std::strlen(g_windowMgr.chatInputText.c_str()) == 0 || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char text[4096];
      std::strcpy(text, g_windowMgr.chatInputText.c_str());
      if (g_forbiddenWords.Contains(text)) {
        std::string masked;
        g_forbiddenWords.Mask(text, &masked);
        g_windowMgr.chatInputText.assign(text, std::strlen(text));
      }
      if (this->lastChat == text) {
        ++this->sameChatRepeatCount;
      } else {
        this->sameChatRepeatCount = 0;
      }
      if (this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
        return 0;
      }
      this->lastChat.assign(text, std::strlen(text));
      const char* message = g_windowMgr.chatInputText.c_str();
      const char* prefix = g_serverLanguage->Prefix(false);
      const char* name = g_session.DecodePlayerName();
      char formatted[16384];
      std::sprintf(formatted, "%s : %s%s", name, prefix, message);
      const std::size_t length = std::strlen(formatted);
      g_windowMgr.ClearChatInputText();
      std::uint16_t packet[16384];
      packet[0] = 0x2DB;
      packet[1] = static_cast<std::uint16_t>(length + 5);
      std::memcpy(packet + 2, formatted, length + 1);
      GetRagConnection().Send(packet[1], packet);
      return 0;
    }
    case MM_REMOVE_RODEX_ITEM: {
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen()) {
        return 0;
      }
      const GameModePacketWordWord packet{0xA06, static_cast<std::uint16_t>(argument1), static_cast<std::uint16_t>(argument2)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_SWAP_EQUIPMENT_BY_INDEX: {
      if (timeGetTime() - g_session.m_swapEquipmentTick < 10000U) {
        const char* suffix = LookupMessage(MSI_CONNECT_SNS_FAIL_CONNECT);
        const unsigned remaining = 10U - (timeGetTime() - g_session.m_swapEquipmentTick) / 1000U;
        const std::string text = FormatOwnedString("%d%s", remaining, suffix);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xC8FF00, 7, 0, 0);
        return 0;
      }
      const auto* request = reinterpret_cast<const std::uint16_t*>(argument1);
      const GameModePacketWord packet{0xACE, request[2]};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_SHORTCUT_KEY_CHANGE: {
      std::uint16_t packet[7];
      packet[0] = 0xB21;
      packet[1] = 0;
      const NormalItemSlot* slot = nullptr;
      if (argument1 >= 36) {
        if (static_cast<unsigned>(argument1 - 36) <= 1) {
          slot = &g_session.SkillHotkey(argument1 - 36);
        }
      } else if (argument2 == 0) {
        slot = &g_session.m_shortcutSlots[argument1];
      } else if (argument2 == 1) {
        slot = &g_session.m_secondaryShortcutSlots[argument1];
        packet[1] = 1;
      }
      if (slot) {
        std::memcpy(packet + 3, slot, 7);
      }
      packet[2] = static_cast<std::uint16_t>(argument1);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet[0]), packet);
      return 0;
    }
    case MM_CASH_EMOTION: {
      CCashEmotionAction_Play action;
      action.ExecuteNamed(reinterpret_cast<const char*>(argument1));
      return 0;
    }
    case MM_MENUSELECTED: {
      if (g_windowMgr.menuWnd) {
        g_windowMgr.CloseWindow(WID_MENUWND);
      }
      if (g_windowMgr.itemStoreWnd || static_cast<std::uint32_t>(argument1) >= this->actorMenuIds.size()) {
        return 0;
      }
      const int action = this->actorMenuIds[argument1];
      switch (action) {
        case MENUID_DEAL:
          if (this->isOnQuest == 1 || g_windowMgr.sayDialogWnd || g_windowMgr.choose3Wnd || g_windowMgr.sayDialogOverlayWnd) {
            g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUSY)), 0xFFFFFF, 0, 0, 0);
          } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->GetStateFlags() != 2) {
            this->SendMsg(MM_REQ_EXCHANGE_ITEM, this->menuTargetAid, 0, 0, 0);
          }
          return 0;
        case MENUID_INVITE_IN_PARTY:
        case 58: {
          const std::string name = this->ActorName(this->menuTargetAid);
          this->SendMsg(action == MENUID_INVITE_IN_PARTY ? MM_REQ_JOIN_GROUP : MM_OPEN_WRITE_RODEX, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
          return 0;
        }
        case MENUID_ADD_TO_FRIEND_CHAR_LIST:
          g_windowMgr.RequestAddFriend(this->ActorName(this->menuTargetAid).c_str());
          return 0;
        case MENUID_WRITE_CHAT_TO_FILE:
          g_windowMgr.SendMsgToNamedWindow(this->lastWhisperMenuCharacterName.c_str());
          return 0;
        case MENUID_REQ_EX_LIST:
        case MENUID_REQ_IN: {
          std::string name = this->ActorName(this->menuTargetAid);
          this->ExecuteTalkType(action == 12 ? 0x12 : 0x13, &name);
          return 0;
        }
        case MENUID_MAKE_WHISPER_WINDOW: {
          const std::string name = this->ActorName(this->menuTargetAid);
          const char* category = g_windowMgr.IsFriend(name.c_str()) ? LookupMessage(MSI_FRIEND) :
            (g_session.IsGuildMemberAccount(this->menuTargetAid) ? "GuildMember" : nullptr);
          std::string code;
          if (auto* actor = dynamic_cast<CPc*>(this->scene->GetGameActorByAID(this->menuTargetAid))) {
            code = ShortcutFilename(actor->GetPkState());
          }
          g_windowMgr.SendNotice(UIM_MAKE_WHISPER_WINDOW, reinterpret_cast<std::intptr_t>(name.c_str()), reinterpret_cast<std::intptr_t>(category),
            reinterpret_cast<std::intptr_t>(code.c_str()), 0, 0);
          return 0;
        }
        case MENUID_ADD_TO_FRIEND_CHAR_LIST_FROM_WINDOW:
          g_windowMgr.RequestAddFriend(this->lastWhisperMenuCharacterName.c_str());
          return 0;
        case MENUID_INVITE_IN_GUILD:
          this->SendMsg(MM_REQ_JOIN_GUILD, this->menuTargetAid, 0, 0, 0);
          return 0;
        case 23:
        case 24: {
          if (!(action == 23 ? IsMonsterJob(g_session.m_aid) : IsAdminAccount(g_session.m_aid))) {
            return 0;
          }
          auto* window = new UICntWnd;
          window->SetSize(0xB6, 0x2E);
          const int y = ScaleByScreenHeight(200);
          window->Move(UIWindowMgr::CenterOnViewportX(200), y);
          g_windowMgr.AddRendered(window);
          window->SendMsg(nullptr, UM_SET_TARGET, action == 23 ? 0x97 : 0x98, 0, 0, 0);
          window->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(action == 23 ? MSI_GIVE_PLUS_MANNER_POINT : MSI_GIVE_MINUS_MANNER_POINT)), 0, 0, 0);
          window->SendMsg(nullptr, UM_SET_COUNT, 1, 0, 0, 0);
          return 0;
        }
        case MENUID_ALLY_GUILD:
          this->SendMsg(MM_REQ_ALLY_GUILD, this->menuTargetAid, 0, 0, 0);
          return 0;
        case MENUID_HOSTILE_GUILD:
          this->SendMsg(MM_REQ_HOSTILE_GUILD, this->menuTargetAid, 0, 0, 0);
          return 0;
        case MENUID_DISCONNECT_CHARACTER:
          this->SendMsg(MM_DISCONNECT_CHARACTER, this->menuTargetAid, 0, 0, 0);
          return 0;
        case MENUID_PET_FEEDING:
        case MENUID_HOMUN_FEEDING: {
          if (action == 29 && g_windowMgr.rodexComposeWnd) {
            g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REFINING_FAIL_ITEM_DISAPPEAR)), 0xFF, 0, 0, 0);
            return 0;
          }
          auto* window = new UINormalMB;
          window->Create(nullptr, LookupMessage(MSI_SURE_TO_FEED_PET), 2, true, nullptr, 280, 120, nullptr);
          g_windowMgr.AddRendered(window);
          window->SetAction(0xBB, reinterpret_cast<void (*)()>(action == 29 ? ConfirmGameModeAction96 : ConfirmGameModeActionB8));
          return 0;
        }
        case MENUID_PET_PERFORMANCE:
        case MENUID_PET_RETURN_EGG:
        case MENUID_PET_ACC_OFF:
          this->SendMsg(MM_COMMAND_PET, action - 28, 0, 0, 0);
          return 0;
        case MENUID_PET_SHOWINFO:
          this->SendMsg(MM_COMMAND_PET, 0, 0, 0, 0);
          if (!g_windowMgr.CloseWindow(WID_PETINFOWND)) {
            g_windowMgr.EnsureWindow(WID_PETINFOWND);
          }
          return 0;
        case MENUID_COPY_GID: {
          char text[512];
          std::sprintf(text, "%s : %d (GID)", g_session.m_serverName.c_str(), this->menuTargetAid);
          CLanguageBase* language = g_serverLanguage;
          language->ResetInput();
          language->Insert(text);
          return 0;
        }
        case MENUID_SHOW_CHAT_PROHIBIT_LOG:
          g_windowMgr.EnsureWindow(WID_PROHIBITLISTWND)->SendMsg(nullptr, UM_SETINFO, this->menuTargetAid, 0, 0, 0);
          return 0;
        case MENUID_INVITE_IN_BABY:
          this->SendMsg(MM_REQ_JOIN_BABY, this->menuTargetAid, 0, 0, 0);
          return 0;
        case MENUID_HOMUN_SHOWINFO:
          this->SendMsg(MM_COMMAND_HOMUN, 0, 0, 0, 0);
          if (!g_windowMgr.CloseWindow(WID_HOMUNINFOWND)) {
            g_windowMgr.EnsureWindow(WID_HOMUNINFOWND);
          }
          return 0;
        case MENUID_HOMUN_FOLLOW: {
          auto* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(g_session.m_homunAid));
          if (actor && actor->HasActiveHomunculusAI()) {
            AiMessage value;
            value.type = 9;
            actor->SetMsg2AI(&value, 0);
          }
          return 0;
        }
        case MENUID_MER_SHOWINFO:
          this->SendMsg(MM_COMMAND_MER, 1, 0, 0, 0);
          if (!g_windowMgr.CloseWindow(WID_MERINFOWND)) {
            g_windowMgr.EnsureWindow(WID_MERINFOWND);
          }
          return 0;
        case MENUID_MER_FOLLOW: {
          auto* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(g_session.m_mercenaryAid));
          if (actor && actor->merAI && actor->merAI->state) {
            AiMessage value;
            value.type = 9;
            actor->SetMsg2AI(&value, 1);
          }
          return 0;
        }
        case MENUID_VIEW_OTHERUSER_EQUIPWND:
        case 44:
        case 65: {
          if (action == 44 && !IsAdminAccount(g_session.m_aid)) {
            return 0;
          }
          const GameModePacketDword packet{static_cast<std::uint16_t>(action == 42 ? 0x2D6 : action == 44 ? 0x7F5 : 0xAF9),
            static_cast<std::uint32_t>(this->menuTargetAid)};
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
          return 0;
        }
        case 57: {
          auto* actor = dynamic_cast<CPc*>(this->scene->GetGameActorByAID(this->menuTargetAid));
          if (!actor) {
            return 0;
          }
          const std::string code = ShortcutFilename(actor->GetPkState());
          HGLOBAL memory = GlobalAlloc(0x42, code.size() + 1);
          void* destination = GlobalLock(memory);
          std::memcpy(destination, code.data(), code.size());
          GlobalUnWire(memory);
          if (OpenClipboard(hWnd)) {
            EmptyClipboard();
            SetClipboardData(1, memory);
            CloseClipboard();
            g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
            g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("C-Code copied to clipboard"), 0x64FFFF, 0, 0, 0);
          }
          return 0;
        }
        case 63:
          g_windowMgr.EnsureWindow(WID_MACROREPORT);
          return 0;
        default:
          return 0;
      }
    }
    case MM_REQ_EMOTION: {
      auto& tick = this->emotionRequestTick;
      if (this->isEmotionRequested || timeGetTime() <= tick + 2500U) {
        return 0;
      }
      const bool doram = g_session.IsDoramJob(g_session.GetDisplayJob());
      const CSkillInfo* skill = g_session.m_skillBundle.Find(doram ? 0x139A : 1);
      if (!skill || skill->level <= (doram ? 0 : 1)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_USE_FAIL_SKILL_LEVEL")), 0xFF, 0, 0, 0);
        return 0;
      }
      if (g_cashEmotionMgr->BasePackId() != argument2 && !g_cashEmotionMgr->IsOwnedPack(argument2)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_UNPURCHASED")), 0xFF, 0, 0, 0);
        return 0;
      }
      if (!g_cashEmotionMgr->GetEmotionData(argument2, argument1)) {
        return 0;
      }
      const GameModePacketWordWord packet{0xBE9, static_cast<std::uint16_t>(argument2), static_cast<std::uint16_t>(argument1)};
      tick = timeGetTime();
      this->isEmotionRequested = 1;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_OPENSTORE: {
      const int count = g_session.GetList11Size();
      unsigned char packet[0x47BC];
      *reinterpret_cast<std::uint16_t*>(packet) = 0x1B2;
      *reinterpret_cast<std::uint16_t*>(packet + 2) = static_cast<std::uint16_t>(85 + count * 8);
      char* title = reinterpret_cast<char*>(packet + 4);
      if (argument2 == 1) {
        std::strncpy(title, reinterpret_cast<const char*>(argument1), 79);
        title[79] = 0;
        packet[84] = 1;
      } else {
        title[0] = 0;
        packet[84] = 0;
      }
      if (!g_replayAnimationScaling) {
        g_session.storeTitle = title;
      }
      unsigned char* entry = packet + 85;
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1768At(index);
        if (item.identity[1]) {
          *reinterpret_cast<std::uint16_t*>(entry) = static_cast<std::uint16_t>(item.identity[1]);
          *reinterpret_cast<std::uint16_t*>(entry + 2) = static_cast<std::uint16_t>(item.quantity);
          *reinterpret_cast<std::uint32_t*>(entry + 4) = item.prices[1];
          entry += 8;
        }
      }
      if (count == 0) {
        packet[84] = 0;
      }
      GetRagConnection().Send(*reinterpret_cast<std::int16_t*>(packet + 2), packet);
      g_session.ClearMerchantPurchaseSavedItems();
      return 0;
    }
    case MM_REQ_LEAVE_GUILD: {
      unsigned char packet[54];
      *reinterpret_cast<std::uint16_t*>(packet) = 0x159;
      *reinterpret_cast<std::uint32_t*>(packet + 2) = g_guildInfo.m_guildId;
      *reinterpret_cast<std::uint32_t*>(packet + 6) = g_session.m_aid;
      *reinterpret_cast<std::uint32_t*>(packet + 10) = g_session.m_gid;
      char* text = reinterpret_cast<char*>(packet + 14);
      if (g_inlineCharset) {
        std::strcpy(text, g_serverLanguage->Prefix(false));
        std::strncpy(text + 3, reinterpret_cast<const char*>(argument1), 36);
      } else {
        std::strncpy(text, reinterpret_cast<const char*>(argument1), 39);
      }
      text[39] = 0;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(ID_CZ_REQ_LEAVE_GUILD), packet);
      return 0;
    }
    case MM_REQ_MAKE_GUILD: {
      const char* name = reinterpret_cast<const char*>(argument1);
      const std::size_t length = std::strlen(name);
      char text[256];
      if ((static_cast<unsigned char>(name[0]) == 0xA1 && static_cast<unsigned char>(name[1]) == 0xA1) ||
          (static_cast<unsigned char>(name[length - 2]) == 0xA1 && static_cast<unsigned char>(name[length - 1]) == 0xA1) ||
          std::strchr(name, ' ')) {
        std::sprintf(text, LookupMessage(MSI_BILLING_CHN_3));
        if (std::strstr(text, "NO MSG")) {
          std::sprintf(text, "Can NOT use space in Guild Name !!");
        }
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x6E96FF, 0, 0, 0);
        return 0;
      }
      if (g_forbiddenWords.Contains(name)) {
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_SELECTCHARWND_BILLING_INFO));
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x6E96FF, 0, 0, 0);
        return 0;
      }
      if (g_serviceType == ServiceBrazil && std::strchr(name, 0x2323)) {
        g_windowMgr.Confirm(LookupMessage(MSI_BILLING_1), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      if (length >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->hasSp == 1) {
        std::sprintf(text, LookupMessage(MSI_TIME_LINE_MINUTE2));
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x6E96FF, 0, 0, 0);
        return 0;
      }
      GameModePacketDwordPayload24 packet;
      packet.packetId = ID_CZ_REQ_MAKE_GUILD;
      packet.argument = g_session.m_gid;
      std::strncpy(reinterpret_cast<char*>(packet.payload), name, 23);
      packet.payload[23] = 0;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_DELETEGUILDPOS: {
      const auto entry = this->guildPosList.find(static_cast<std::uint32_t>(argument1));
      if (entry != this->guildPosList.end()) {
        this->guildPosList.erase(entry);
      }
      return 0;
    }
    case MM_TAKE_OFF_SWAP_EQUIP: {
      int message = 0;
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
        message = 0xBB9;
      } else if (this->isPlayerDead) {
        message = 0xBB7;
      } else if (g_windowMgr.merchantItemMyShopWnd || g_windowMgr.merchantItemPurchaseWnd || g_windowMgr.merchantShopMakeWnd) {
        message = 0xBBA;
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->actorWindows[3]) {
        message = 0xBB8;
      } else if (g_windowMgr.sayDialogWnd || g_windowMgr.choose3Wnd) {
        message = 0xBBB;
      }
      if (message) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
        return 0;
      }
      ITEM_INFO item(0);
      item = argument2 ? g_session.GetSwapEquippedItem(argument1) : g_session.GetSwapCostumeItem(argument1);
      if (item.identity[1] && item.quantity > 0) {
        const GameModePacketWord packet{0xA99, static_cast<std::uint16_t>(item.identity[1])};
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      }
      return 0;
    }
    case MM_GET_ACCOUNT_LIMITED_SALE_LIST: {
      auto& tick = g_session.m_limitedSaleRequestTick;
      if (timeGetTime() - tick < 5000U) {
        const char* message = LookupMessage(MSI_CONNECT_SNS_FAIL_CONNECT);
        const std::string text = FormatOwnedString("%d%s", 5U - (timeGetTime() - tick) / 1000U, message);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xC8FF00, 0, 0, 0);
        return 0;
      }
      tick = timeGetTime();
      const std::uint16_t packet = 0xB4C;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
      return 0;
    }
    case MM_USE_PACKAGEITEM: {
      int message = 0;
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
        message = 0x6A3;
      } else if (this->IsMenuWindowOpen() || this->waitingUseItemAck || this->waitingItemThrowAck) {
        message = 0xF84;
      } else if (this->isPlayerDead) {
        message = 0xBB7;
      } else if (StateFlag01(this->scene->player->GetStateFlags())) {
        message = 0xF83;
      }
      if (message) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
        return 0;
      }
      const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, argument1);
      if (this->scene->player->bodyState == 4 || this->scene->player->bodyState == 1) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PACKAGEITEM_SETTINGFAIL)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (this->scene->player->render.state == 6) {
        const int itemId = item.GetItemId();
        if (itemId == 0x259 || itemId == 0x25A || itemId == 0x263 || itemId == 0x25D || itemId == 0x25E || itemId == 0x5813) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_USE_WHEN_SITDOWN)), 0xFF, 0, 0, 0);
          return 0;
        }
      }
      this->waitingUseItemAck = 1;
      unsigned char packet[16];
      *reinterpret_cast<std::uint16_t*>(packet) = 0xBAF;
      *reinterpret_cast<std::uint16_t*>(packet + 2) = static_cast<std::uint16_t>(argument1);
      *reinterpret_cast<std::uint32_t*>(packet + 4) = g_session.m_aid;
      *reinterpret_cast<std::uint32_t*>(packet + 8) = static_cast<std::uint32_t>(argument2);
      *reinterpret_cast<std::uint32_t*>(packet + 12) = static_cast<std::uint32_t>(argument3);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(ID_CZ_USE_PACKAGEITEM), packet);
      g_windowMgr.CloseWindow(WID_SELECTPACKAGEITEMBOX);
      return 0;
    }
    case MM_ADD_EXCHANGE_ITEM: {
      if (g_session.HasExchangeItem(static_cast<std::uint32_t>(argument1))) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CAN_DRAG_ITEM_OLNY_ONETIME)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (g_session.IsExchangeItemListFull() && argument1 != 0) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MAXEXCHANGE_IS_10_KIND)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (this->isWaitingAddExchangeItem != 0) {
        return 0;
      }
      this->isWaitingAddExchangeItem = 1;
      const GameModePacketWordDword packet{0xE8, static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)};
      this->exchangeItemCnt = argument2;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      if (Replay::Instance().state == 2) {
        std::uint16_t record[5] = {0x08AE, 0x0387, 10};
        std::memcpy(record + 3, &this->exchangeItemCnt, sizeof(int));
        Replay::Instance().Record(10, record, 0x2A, 0);
      }
      return 0;
    }
    case MM_PC_PURCHASE_ITEMLIST_FROMMC: {
      g_session.CopyItems1760To1770();
      const int count = g_session.GetList10Size();
      const std::uint32_t length = 12U + static_cast<std::uint32_t>(count) * 4U;
      if (length > 0x800U) {
        return 0;
      }
      unsigned char packet[0x800];
      *reinterpret_cast<std::uint16_t*>(packet) = 0x801;
      *reinterpret_cast<std::uint16_t*>(packet + 2) = static_cast<std::uint16_t>(length);
      std::memcpy(packet + 4, &argument1, sizeof(argument1));
      std::memcpy(packet + 8, &argument2, sizeof(argument2));
      for (int index = 0; index < count; ++index) {
        ITEM_INFO item = g_session.GetItem1760At(index);
        if (item.identity[1] != 0) {
          *reinterpret_cast<std::uint16_t*>(packet + 12 + index * 4) = static_cast<std::uint16_t>(item.quantity);
          *reinterpret_cast<std::uint16_t*>(packet + 14 + index * 4) = static_cast<std::uint16_t>(item.identity[1]);
        }
      }
      if (count != 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
      }
      return 0;
    }
    case MM_REQ_BUY_FROMMC: {
      if (this->scene != nullptr && this->scene->player != nullptr && this->scene->player->render.state == 3) {
        return 0;
      }
      if (static_cast<std::uint32_t>(argument1) == g_session.m_aid || g_windowMgr.merchantItemShopWnd != nullptr) {
        return 0;
      }
      auto& previousTick = this->merchantShopRequestTick;
      if (timeGetTime() <= previousTick + 1000U) {
        return 0;
      }
      previousTick = timeGetTime();
      const GameModePacketDword packet{0x130, static_cast<std::uint32_t>(argument1)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_SEND_MAIL: {
      const auto& request = *reinterpret_cast<const RodexSendMailRequest*>(argument1);
      if (g_forbiddenWords.Contains(request.title.c_str()) || g_forbiddenWords.Contains(request.body.c_str())) {
        g_windowMgr.Confirm(LookupMessage(MSI_BANK_SYSTEM_BUSY), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        return 0;
      }
      g_session.RestoreSelectionItems();
      g_session.ClearItemQuantities();
      unsigned char header[0x44];
      *reinterpret_cast<std::uint16_t*>(header) = 0xA6E;
      std::memcpy(header + 0x40, &request.recipientId, sizeof(request.recipientId));
      const std::int64_t zeny = request.zeny;
      std::memcpy(header + 0x34, &zeny, sizeof(zeny));
      strcpy_s(reinterpret_cast<char*>(header + 4), 24, request.recipient.c_str());
      strcpy_s(reinterpret_cast<char*>(header + 0x1C), 24, g_session.DecodePlayerName());
      const std::int16_t titleLength = static_cast<std::int16_t>(request.title.size() + 1);
      const std::int16_t bodyLength = static_cast<std::int16_t>(request.body.size() + 1);
      std::memcpy(header + 0x3C, &titleLength, sizeof(titleLength));
      std::memcpy(header + 0x3E, &bodyLength, sizeof(bodyLength));
      std::vector<unsigned char> packet;
      packet.insert(packet.end(), header, header + sizeof(header));
      packet.insert(packet.end(), request.title.c_str(), request.title.c_str() + titleLength);
      packet.insert(packet.end(), request.body.c_str(), request.body.c_str() + bodyLength);
      const std::int16_t length = static_cast<std::int16_t>(packet.size());
      std::memcpy(packet.data() + 2, &length, sizeof(length));
      GetRagConnection().Send(length, packet.data());
      g_windowMgr.CloseWindow(WID_RODEXCOMPOSEWND);
      return 0;
    }
    case MM_STOP_REPLAY_RECORDING: {
      if (Replay::Instance().state == 2) {
        Replay::Instance().SetState(1);
        Replay::Instance().UpdateDuration();
        Replay::Instance().Duration();
        Replay::Instance().CloseFile();
      }
      char notice[0x104] = {};
      char directory[0x104] = {};
      GetCurrentDirectoryA(0x104, directory);
      std::string text(directory);
      text += ReplayFileList::Instance().path;
      text += LookupMessage(MSI_REPLAY_RECORDEND);
      std::sprintf(notice, "%s", text.c_str());
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice), 0x62B0FF, 0, 0, 0);
      return 0;
    }
    case MM_CHAT_CLAN: {
      if (std::strlen(g_windowMgr.chatInputText.c_str()) == 0 || !g_clanInfo->m_hasClan || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char text[0x16A];
      strcpy_s(text, sizeof(text), g_windowMgr.chatInputText.c_str());
      if (g_forbiddenWords.Contains(text)) {
        std::string replacement("");
        g_forbiddenWords.Mask(text, &replacement);
        g_windowMgr.chatInputText.assign(text, std::strlen(text));
      }
      if (this->lastChat == text) {
        ++this->sameChatRepeatCount;
        if (this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
          return 0;
        }
      } else {
        this->sameChatRepeatCount = 0;
      }
      this->lastChat.assign(text, std::strlen(text));
      const char* message = g_windowMgr.chatInputText.c_str();
      const char* prefix = g_serverLanguage->Prefix(false);
      const char* playerName = g_session.DecodePlayerName();
      char formatted[0x400];
      std::sprintf(formatted, "%s : %s%s", playerName, prefix, message);
      const std::size_t textLength = std::strlen(formatted);
      g_windowMgr.ClearChatInputText();
      if (textLength > 0xDC) {
        return 0;
      }
      unsigned char packet[0x400];
      std::memset(packet + 4, 0, 0x3FC);
      const std::uint16_t length = static_cast<std::uint16_t>(textLength + 5);
      *reinterpret_cast<std::uint16_t*>(packet) = 0x98D;
      *reinterpret_cast<std::uint16_t*>(packet + 2) = length;
      std::memcpy(packet + 4, formatted, textLength + 1);
      GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
      return 0;
    }
    case MM_WEAR_SWAP_EQUIP: {
      if (this->IsMenuWindowOpen()) {
        return 0;
      }
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI) != nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_CHAT)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (this->isPlayerDead != 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_MERCHANTSHOP)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (g_windowMgr.merchantItemMyShopWnd != nullptr || g_windowMgr.merchantItemPurchaseWnd != nullptr || g_windowMgr.merchantShopMakeWnd != nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MDUNGEON_CREATE_FAIL_PARTY_ONLINE)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->actorWindows[3] != 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_USE_NPC)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (g_windowMgr.sayDialogWnd != nullptr || g_windowMgr.choose3Wnd != nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_USESKILL_FAIL_NEED_MORE_ARROW_CNT)), 0xFF, 0, 0, 0);
        return 0;
      }
      ITEM_INFO item = g_session.FindItemByIdentity(static_cast<std::uint32_t>(argument1), argument2);
      if (!item.identified || item.damaged != 0) {
        return 0;
      }
      const bool special = item.identity[1] > 0 &&
          (item.identity[0] == 10 || item.identity[0] == 16 || item.identity[0] == 17 || item.identity[0] == 19);
      if (special) {
        if (static_cast<std::uint32_t>(argument1) == g_session.swapArrowIndex) {
          return 0;
        }
      } else {
        const EquipSlots slots = g_session.GetEquipSlots(argument2);
        for (int index = 2; index >= 0; --index) {
          ITEM_INFO equipped;
          if (g_session.IsCostumeLocation(argument2)) {
            equipped = g_session.GetSwapEquippedItem(slots.values[index]);
          } else {
            equipped = g_session.GetSwapCostumeItem(slots.values[index]);
          }
          if (static_cast<std::uint32_t>(argument1) == equipped.identity[1]) {
            return 0;
          }
        }
      }
      this->waitingWearEquipAck = timeGetTime();
      const GameModePacketWordDword packet{0xA97, static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_MOVE_ITEM_TO_PERSONAL: {
      g_session.FlagCartItem(static_cast<std::uint32_t>(argument1));
      const GameModePacketDwordWordDword packet{
        0xC22, g_session.m_buyingStoreMode != 0 ? 1U : 0U, static_cast<std::uint16_t>(argument1), static_cast<std::uint32_t>(argument2)
      };
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_REQ_PURCHASE: {
      const int count = g_session.GetList2Size();
      const std::uint32_t length = 4U + static_cast<std::uint32_t>(count) * 6U;
      if (length > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t header[2] = {0xC8, static_cast<std::uint16_t>(length)};
      std::memcpy(packet, header, sizeof(header));
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1720At(index);
        const std::uint16_t quantity = static_cast<std::uint16_t>(item.quantity);
        const int itemId = std::atoi(item.itemId.c_str());
        std::memcpy(packet + 4 + index * 6, &quantity, sizeof(quantity));
        std::memcpy(packet + 6 + index * 6, &itemId, sizeof(itemId));
      }
      if (count != 0 && this->isPlayerDead == 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
      }
      g_session.ClearTradeItems();
      g_windowMgr.CloseWindow(WID_CHOOSESELLBUYWND);
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
      return 0;
    }
    case MM_CHANGE_CHATROOM: {
      if (g_windowMgr.chatRoomWnd == nullptr) {
        return 0;
      }
      const ChatRoomSetting& settings = *reinterpret_cast<const ChatRoomSetting*>(argument1);
      this->chatRoom.title = settings.title;
      this->chatRoom.type = settings.isPublic;
      this->chatRoom.memberCount = settings.memberCount;
      this->chatRoom.memberLimit = settings.limit;
      this->chatRoom.id = settings.roomId;
      this->chatRoom.password = settings.password;
      unsigned char packet[0x800];
      const std::uint16_t header[3] = {
        0xDE, static_cast<std::uint16_t>(settings.title.size() + 15), static_cast<std::uint16_t>(settings.limit)
      };
      std::memcpy(packet, header, sizeof(header));
      packet[6] = static_cast<unsigned char>(settings.isPublic);
      std::memset(packet + 7, 0, 8);
      std::memcpy(packet + 7, settings.password.c_str(), settings.password.size());
      std::memcpy(packet + 15, settings.title.c_str(), settings.title.size());
      this->RecordChatRoomSettings();
      GetRagConnection().Send(static_cast<std::int16_t>(header[1]), packet);
      return 0;
    }
    case MM_CHAT_PARTY: {
      if (g_windowMgr.chatInputText.empty() || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char text[0x800];
      std::strcpy(text, g_windowMgr.chatInputText.c_str());
      if (g_forbiddenWords.Contains(text)) {
        std::string masked;
        g_forbiddenWords.Mask(text, &masked);
        g_windowMgr.chatInputText.assign(text, std::strlen(text));
      }
      if (this->lastChat == text) {
        ++this->sameChatRepeatCount;
      } else {
        this->sameChatRepeatCount = 0;
      }
      if (this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
        return 0;
      }
      this->lastChat.assign(text, std::strlen(text));
      if (g_navigationLinkParser->HasUnownedItemLink(g_windowMgr.chatInputText)) {
        return 0;
      }
      char message[0x169] = {};
      const char* input = g_windowMgr.chatInputText.c_str();
      const char* prefix = g_serverLanguage->Prefix(false);
      const char* name = g_session.DecodePlayerName();
      StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), "%s : %s%s", name, prefix, input);
      g_windowMgr.ClearChatInputText();
      const std::size_t size = std::strlen(message);
      unsigned char packet[0x16D];
      const std::uint16_t header[2] = {0x108, static_cast<std::uint16_t>(size + 4)};
      std::memcpy(packet, header, sizeof(header));
      std::memcpy(packet + 4, message, size);
      GetRagConnection().Send(header[1], packet);
      return 0;
    }
    case MM_USESKILL_LEVEL_MINUS: {
      if (CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, this->skillId)) {
        const CSkillInfo skill = g_session.m_skillBundle.Lookup(this->skillId);
        if (skill.available != 0) {
          this->skillUseLevel -= argument1;
          if (this->skillUseLevel > skill.level) {
            this->skillUseLevel = skill.level;
          }
          if (this->skillUseLevel <= 0) {
            this->skillUseLevel = 1;
          }
        }
      }
      return 0;
    }
    case MM_SONG_TALK: {
      CFile file;
      const char* path = nullptr;
      if (argument1 == 0) {
        path = g_serviceType == ServiceGermany ? "english\\DC_scream.txt" : "DC_scream.txt";
      } else if (argument1 == 1) {
        path = g_serviceType == ServiceGermany ? "english\\BA_frostjoke.txt" : "BA_frostjoke.txt";
      }
      if (file.Open(path, 0, true, false)) {
        const int size = static_cast<int>(file.GetSize());
        char* data = static_cast<char*>(::operator new(static_cast<std::size_t>(size) + 1));
        file.Read(data, size);
        data[size] = 0;
        file.Close();
        int selected;
        if (argument1 == 0) {
          selected = std::rand() % 114 + 1;
        } else if (argument1 == 1) {
          selected = std::rand() % 117 + 1;
        }
        signed char line = 0;
        unsigned int length = 0;
        char text[0x100];
        for (int index = 0; index < size; ++index) {
          const char value = data[index];
          if (value == '\r') {
            line = static_cast<signed char>(static_cast<unsigned char>(line) + 1);
          }
          if (line == selected) {
            if (value != '\r' && value != '\n' && value != '\t') {
              text[length++] = value;
              if (length >= sizeof(text)) {
                __report_rangecheckfailure();
              }
              text[length] = 0;
            }
          } else if (line == selected + 1) {
            break;
          }
        }
        std::free(data);
        char message[0x200];
        const char* decoded = DecodeMessageRot(text);
        std::sprintf(message, "%s : %s", g_session.DecodePlayerName(), decoded);
        message[0x169] = 0;
        const std::size_t messageLength = std::strlen(message);
        g_windowMgr.ClearChatInputText();
        unsigned char packet[0x800];
        const std::uint16_t header[2] = {0xF3, static_cast<std::uint16_t>(messageLength + 5)};
        std::memcpy(packet, header, sizeof(header));
        std::memcpy(packet + 4, message, messageLength + 1);
        GetRagConnection().Send(header[1], packet);
      }
      return 0;
    }
    case MM_ITEMLISTWIN_RES: {
      const int count = g_session.GetList4Size();
      const std::uint32_t length = 12U + static_cast<std::uint32_t>(count) * 4U;
      if (length > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t header[2] = {0x7E4, static_cast<std::uint16_t>(length)};
      const int flag = 1;
      std::memcpy(packet, header, sizeof(header));
      std::memcpy(packet + 4, &argument1, sizeof(argument1));
      std::memcpy(packet + 8, &flag, sizeof(flag));
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1730At(index);
        const std::uint16_t entry[2] = {
          static_cast<std::uint16_t>(item.identity[1]), static_cast<std::uint16_t>(item.quantity)
        };
        std::memcpy(packet + 12 + index * 4, entry, sizeof(entry));
      }
      if (count != 0) {
        GetRagConnection().Send(static_cast<std::int16_t>(length), packet);
      }
      g_session.ClearTradeItems();
      return 0;
    }
    case MM_WHISPER_FILTER:
      if (g_session.partyBlocked != 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOPARTY)), 0x6E96FF, 0, 0, 0);
      } else if (g_session.PartyCount() == 0) {
        g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_HEIGHT_220);
        g_session.partyRequestName = reinterpret_cast<const char*>(argument1);
      }
      return 0;
    case MM_REFRESH_PLAYERGAGE: {
      if (this->itemWindow == nullptr) {
        return 0;
      }
      UIPlayerGage* gauge = static_cast<UIPlayerGage*>(this->itemWindow);
      gauge->SetFirstValues(
        [] { return static_cast<std::int64_t>(g_session.hp); },
        [] { return static_cast<std::int64_t>(g_session.maxHp); }
      );
      gauge->SetSecondValues(
        [] { return static_cast<std::int64_t>(g_session.m_sp); },
        [] { return static_cast<std::int64_t>(g_session.m_maxSp); }
      );
      if (g_session.IsTreeGroupThree(g_session.GetDisplayJob())) {
        gauge->SetFourthValues(
          [] { return static_cast<std::int64_t>(g_session.GetTraitStat(0xE8)); },
          [] { return static_cast<std::int64_t>(g_session.GetTraitStat(0xE9)); }
        );
        gauge->SetSize(60, 13);
      } else {
        gauge->SetFourthValues({}, {});
        gauge->SetSize(60, 9);
      }
      CPlayer* player = this->scene->player;
      const int offset = ScaleByScreenHeight(12);
      gauge->Move(player->render.screenX - 30,
        static_cast<int>(static_cast<float>(player->render.screenY) + static_cast<float>(offset) * player->render.distanceScale));
      return 0;
    }
    case MM_SOCKETERROR:
      this->noMove = 1;
      this->noMoveStartTick = timeGetTime();
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_UNABLE_TO_CONNECT_SERVER), 0, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        this->SendMsg(MM_QUIT_TO_IDENTRY, 0, 0, 0, 0);
      }
      return 0;
    case MM_BROADCASTMSG: {
      std::string text;
      if (argument1 == 0 || argument1 == 1) {
        text = std::string(g_session.DecodePlayerName()) + ": " + g_windowMgr.chatInputText.c_str();
      } else if (argument1 == 4 || argument1 == 5) {
        text = std::string("blue") + g_windowMgr.chatInputText.c_str();
      } else {
        text = g_windowMgr.chatInputText.c_str();
      }
      char message[0x16A];
      std::strcpy(message, text.c_str());
      message[0x169] = 0;
      const std::size_t length = std::strlen(message) + 1;
      if (std::strlen(g_windowMgr.chatInputText.c_str()) != 0) {
        std::uint16_t packetId;
        if (argument1 == 0 || argument1 == 2 || argument1 == 4) {
          packetId = 0x99;
        } else if (argument1 == 1 || argument1 == 3 || argument1 == 5) {
          packetId = 0x19C;
        }
        unsigned char packet[0x800];
        const std::uint16_t packetLength = static_cast<std::uint16_t>(length + 4);
        std::memcpy(packet, &packetId, 2);
        std::memcpy(packet + 2, &packetLength, 2);
        std::memcpy(packet + 4, message, length);
        GetRagConnection().Send(packetLength, packet);
      }
      g_windowMgr.ClearChatInputText();
      return 0;
    }
    case MM_CONTACTNPC: {
      if (this->scene->player != nullptr) {
        this->scene->player->SendMsg(nullptr, AM_FREEZE, 0, 0, 0, 0, 0, 0);
      }
      const auto* request = reinterpret_cast<const std::uint32_t*>(argument1);
      if (request[1] == 0x2D) {
        if (this->noMove == 1) {
          return 0;
        }
        this->noMove = 1;
        this->noMoveStartTick = timeGetTime();
      }
      const GameModePacketDwordByte packet{0x90, request[0], *reinterpret_cast<const std::uint8_t*>(request + 2)};
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetId), &packet);
      return 0;
    }
    case MM_USE_SKILL_TOGROUND: {
      if (this->scene->player->isRiding != 0 && !g_session.IsHomunculusSkill(argument1) && !g_session.IsMercenarySkill(argument1)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_USE_SKILL_IN_RIDING_STATE)), 0xFF, 0, 0, 0);
        return 0;
      }
      if (this->IsMenuWindowOpen()) {
        return 0;
      }
      const auto* request = reinterpret_cast<const int*>(argument1);
      const int skill = request[0];
      const int x = request[1];
      const int y = request[2];
      int level = request[3];
      if (this->ignoreSkillInterval == 0 && !g_session.IsRequestReady(skill)) {
        char message[0x800];
        const char* name = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, skill);
        std::sprintf(message, LookupMessage(MSI_SKILLINTERVAL2), name);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFF, 0xB, 0, 0);
        return 0;
      }
      if (level == 0) {
        level = g_session.m_skillBundle.Lookup(skill).level;
      }
      const std::uint16_t words[] = {0xAF4, static_cast<std::uint16_t>(level), static_cast<std::uint16_t>(skill), static_cast<std::uint16_t>(x), static_cast<std::uint16_t>(y)};
      unsigned char packet[11];
      std::memcpy(packet, words, sizeof(words));
      packet[10] = static_cast<unsigned char>(argument2);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(words[0]), packet);
      return 0;
    }
    case MM_SEND_GUILD_EMBLEM: {
      if (g_guildInfo.isMaster == 0 || static_cast<std::int32_t>(static_cast<std::uint32_t>(argument2) + 4) > 0x800) {
        return 0;
      }
      unsigned char packet[0x800];
      const std::uint16_t header[] = {0x153, static_cast<std::uint16_t>(argument2 + 4)};
      std::memcpy(packet, header, sizeof(header));
      std::memcpy(packet + 4, reinterpret_cast<const void*>(argument1), argument2);
      GetRagConnection().Send(static_cast<std::int16_t>(header[1]), packet);
      return 0;
    }
    case MM_CHAT_GUILD: {
      if (std::strlen(g_windowMgr.chatInputText.c_str()) == 0 || g_guildInfo.m_guildId == 0 || g_session.IsBerserkActive() == 1) {
        return 0;
      }
      char text[0x800];
      std::strcpy(text, g_windowMgr.chatInputText.c_str());
      if (g_forbiddenWords.Contains(text)) {
        std::string masked;
        g_forbiddenWords.Mask(text, &masked);
        g_windowMgr.chatInputText.assign(text, std::strlen(text));
      }
      if (this->lastChat == text) {
        ++this->sameChatRepeatCount;
      } else {
        this->sameChatRepeatCount = 0;
      }
      if (this->ShowRestrictedNotice(this->sameChatRepeatCount)) {
        return 0;
      }
      this->lastChat.assign(text, std::strlen(text));
      if (g_navigationLinkParser->HasUnownedItemLink(g_windowMgr.chatInputText.c_str())) {
        return 0;
      }
      char message[0x169] = {};
      const char* body = g_windowMgr.chatInputText.c_str();
      const char* prefix = g_serverLanguage->Prefix(false);
      StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), "%s : %s%s", g_session.DecodePlayerName(), prefix, body);
      g_windowMgr.ClearChatInputText();
      const std::size_t length = std::strlen(message);
      unsigned char packet[0x16D];
      const std::uint16_t header[] = {0x17E, static_cast<std::uint16_t>(length + 4)};
      std::memcpy(packet, header, sizeof(header));
      std::memcpy(packet + 4, message, length);
      GetRagConnection().Send(header[1], packet);
      return 0;
    }
    case MM_REQ_OPEN_BUYING_STORE: {
      const int count = g_session.GetList11Size();
      unsigned char packet[0x800];
      const std::uint16_t header[] = {0x811, static_cast<std::uint16_t>(0x59 + count * 10)};
      std::memcpy(packet, header, sizeof(header));
      if (argument2 == 1) {
        std::strncpy(reinterpret_cast<char*>(packet + 9), reinterpret_cast<const char*>(argument1), 0x4F);
        packet[0x58] = 0;
        std::memcpy(packet + 4, &argument3, 4);
        packet[8] = 1;
      } else {
        std::memset(packet + 4, 0, 6);
      }
      if (g_replayAnimationScaling == 0) {
        g_session.storeTitle = reinterpret_cast<const char*>(packet + 9);
      }
      unsigned char* output = packet + 0x59;
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1768At(index);
        if (item.identity[1] > 0) {
          const int id = std::atoi(item.itemId.c_str());
          const std::uint16_t quantity = static_cast<std::uint16_t>(item.quantity);
          std::memcpy(output, &id, 4);
          std::memcpy(output + 4, &quantity, 2);
          std::memcpy(output + 6, &item.prices[1], 4);
          output += 10;
        }
      }
      if (count == 0) {
        packet[8] = 0;
      }
      GetRagConnection().Send(static_cast<std::int16_t>(header[1]), packet);
      return 0;
    }
    case MM_OFFLINE_STORE_CREATE: {
      unsigned char packet[0x800] = {};
      const int count = g_session.GetList11Size();
      const std::uint16_t header[] = {0xA7F, static_cast<std::uint16_t>(0x58 + count * 8)};
      std::memcpy(packet, header, sizeof(header));
      std::strncpy(reinterpret_cast<char*>(packet + 4), reinterpret_cast<const char*>(argument1), 0x4F);
      packet[0x53] = 0;
      if (g_windowMgr.merchantShopMakeWnd != nullptr) {
        const auto* shop = static_cast<const UIMerchantShopMakeWnd*>(g_windowMgr.merchantShopMakeWnd);
        std::memcpy(packet + 0x54, &shop->selectedRow, 2);
        std::memcpy(packet + 0x56, &shop->selectedColumn, 2);
      }
      unsigned char* output = packet + 0x58;
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO item = g_session.GetItem1768At(index);
        if (item.identity[1] != 0) {
          const std::uint16_t id = static_cast<std::uint16_t>(item.identity[1]);
          const std::uint16_t quantity = static_cast<std::uint16_t>(item.quantity);
          std::memcpy(output, &id, 2);
          std::memcpy(output + 2, &quantity, 2);
          std::memcpy(output + 4, &item.prices[1], 4);
          output += 8;
        }
      }
      GetRagConnection().Send(static_cast<std::int16_t>(header[1]), packet);
      return 0;
    }
    default:
      return CMode::SendMsg(event, argument1, argument2, argument3, 0);
  }
}

// OFFSET: 0x00CCD880
// FUNCTION: fn_21217
CGameMode::CGameMode()
    : scene(nullptr), skillUseType(nullptr), skillId(0), skillAttackRange(0), skillUseLevel(0), ignoreSkillInterval(0), bossMonsterX(0), bossMonsterY(0),
      broadcastBalloon(nullptr), noticeBalloon(nullptr), recordItemPickups(0) {
  this->isInitializing[0] = 0;
  this->camera = nullptr;
  this->mousePointer = nullptr;
  this->replayPauseTick = 0;
  this->replayDragState[0] = 0;
  this->itemWindow = nullptr;
  this->sameMapMove[0] = 0;
}

// OFFSET: 0x00CEFE70
// FUNCTION: fn_21408
void CGameMode::RemoveNotificationWindow(int window) {
  UIWindow* target = reinterpret_cast<UIWindow*>(window);
  for (std::list<UIWindow*>::iterator it = this->notificationWindows.begin(); it != this->notificationWindows.end(); ++it) {
    if (*it == target) {
      this->notificationWindows.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00CEFB30
// FUNCTION: fn_21404
void CGameMode::AddNotificationWindow(UIWindow* window) {
  this->notificationWindows.push_back(window);
}

#pragma pack(push, 1)
struct AdminStatusRequestPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ unsigned char requestType;
  /* 0007 */ std::uint16_t requestValue;
};
#pragma pack(pop)
static_assert(sizeof(AdminStatusRequestPacket) == 9);

// OFFSET: 0x00CEFCA0
// FUNCTION: fn_21405
void CGameMode::RequestAdminStatus() {
  if (IsAdminAccount(g_session.m_aid)) {
    return;
  }

  if (g_serviceMode == 3) {
    return;
  }

  AdminStatusRequestPacket packet = {};
  packet.id = ID_CZ_REQ_GIVE_MANNER_POINT;
  packet.accountId = static_cast<std::uint32_t>(g_session.m_aid);
  packet.requestType = 2;
  packet.requestValue = 0xA;
  GetRagConnection().Send(GetRagConnection().GetPacketSize(ID_CZ_REQ_GIVE_MANNER_POINT), &packet);
}

// OFFSET: 0x00D06BD0
// FUNCTION: fn_21578
void SetPlayerName(std::uint32_t id, std::string name) {
  g_playerNameDictionary->Set(id, name);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CompleteGuildMemberStatusLookup(id, name);
  GuildInfo* guildInfo = &g_guildInfo;
  guildInfo->UpdateCharacterName(id, name.c_str());
  g_session.ResolvePendingActorName(id, name.c_str());
  g_session.UpdateFriendName(id, name.c_str());
}

// OFFSET: 0x00CDD350
// FUNCTION: fn_21316
void CGameMode::PlaceWindowAtActor(UIWindow* window, CRenderObject* actor) {
  const int width = window->geometry.width;
  const int height = window->geometry.height;
  const bool raised = g_serviceOptions.Get(TT_NAME_ABOVE_HEAD_ON_OFF) != 0;
  int offsetX = 0;
  int offsetY = 0;
  switch (actor->render.isCharacter != 0 ? raised : 0) {
  case NAME_BALLOON_BOTTOM:
    offsetX = -(width / 2);
    offsetY = static_cast<int>(static_cast<float>(g_renderer->viewport.height * 20 / 480) * actor->render.distanceScale);
    break;
  case NAME_BALLOON_LEFT:
    offsetX = static_cast<int>(
      static_cast<float>(g_renderer->viewport.width * -30 / 640) * actor->render.distanceScale - static_cast<float>(width / 2)
    );
    offsetY = static_cast<int>(
      static_cast<float>(g_renderer->viewport.height * -25 / 480) * actor->render.distanceScale - static_cast<float>(height / 2)
    );
    break;
  case NAME_BALLOON_RIGHT:
    offsetX = static_cast<int>(
      static_cast<float>(g_renderer->viewport.width * 30 / 640) * actor->render.distanceScale - static_cast<float>(width / 2)
    );
    offsetY = static_cast<int>(
      static_cast<float>(g_renderer->viewport.height * -25 / 480) * actor->render.distanceScale - static_cast<float>(height / 2)
    );
    break;
  case NAME_BALLOON_TOP:
    offsetX = -(width / 2);
    offsetY = static_cast<int>(static_cast<float>(g_renderer->viewport.height * -100 / 480) * actor->render.distanceScale);
    break;
  }

  int x = actor->render.screenX + offsetX;
  int y = actor->render.screenY + offsetY;
  if (x < -3) {
    x = -3;
  }
  const int maximumX = g_renderer->viewport.width - window->geometry.width + 3;
  if (maximumX < x) {
    x = maximumX;
  }
  if (y < -3) {
    y = -3;
  }
  const int maximumY = g_renderer->viewport.height - window->geometry.height + 3;
  if (maximumY < y) {
    y = maximumY;
  }
  window->Move(x, y);
}

// OFFSET: 0x00CDD790
// FUNCTION: fn_21318
void CGameMode::OnChangeState(int value) {
}

// OFFSET: 0x00CDFA00
// FUNCTION: fn_21324
std::uint64_t CGameMode::LaunchDummyEffect(const vector3d* position, int effectId, int lifeFrames, int value) {
  if (lifeFrames > 0) {
    CDummyEffectLauncher* launcher = new CDummyEffectLauncher;
    if (launcher != nullptr) {
      const int address = reinterpret_cast<int>(position);
      launcher->SendMsg(nullptr, AM_SETPOS, address, effectId, lifeFrames, value, 0, 0);
      this->scene->objects.push_back(launcher);
      return launcher->identity;
    }
  }
  return 0;
}

// OFFSET: 0x00CDFB00
// FUNCTION: fn_21326
void CGameMode::RequestPlayerAction(int target, int argument) {
  int value = this->skillId;
  if (value != 10000) {
    const int maxWeight = g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight;
    if (g_session.m_weight * 100 / maxWeight >= 90) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_ATTACK_OVERWEIGHT)), 0xFF, 0, 0, 0);
      value = this->skillId;
      if (value != 40000) {
        return;
      }
    }
  }

  switch (reinterpret_cast<std::intptr_t>(this->skillUseType)) {
  case SUT_NOSKILL:
    this->scene->player->SendMsg(nullptr, AM_PROCEEDATTACK, static_cast<std::uint32_t>(target), argument, 0, 0, 0, 0);
    break;
  case SUT_TO_CHARACTER:
  case SUT_TO_ALL:
    this->scene->player->SendMsg(nullptr, AM_PROCEEDSKILL, static_cast<std::uint32_t>(target), value, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, AM_SET_SKILL_USE_LEVEL, this->skillUseLevel, 0, 0, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00CE45B0
// FUNCTION: fn_21336
void CGameMode::UpdateLoopSounds() {
  for (std::vector<GameModeLoopSound>::iterator it = this->loopSounds.begin(); it != this->loopSounds.end(); ++it) {
    timeGetTime();
    if (timeGetTime() < it->nextTick) {
      continue;
    }

    if (it->npcId) {
      CRenderObject* actor = this->scene->GetGameActorByAID(it->npcId);
      if (actor) {
        CPlayer* player = this->scene->player;
        g_soundMgr->PlayWave(it->name.c_str(), player->render.position.x - actor->render.position.x, 0.0f,
          player->render.position.z - actor->render.position.z, 250, 40, 1.0f, 0);
      }
    } else {
      g_soundMgr->PlayWave(it->name.c_str(), 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    it->nextTick = timeGetTime() + it->term;
  }
}

// OFFSET: 0x00CE81C0
// FUNCTION: fn_21340
void CGameMode::AddSceneObject(CRenderObject* object) {
  if (this->scene && object) {
    this->scene->objects.push_back(object);
  }
}

// OFFSET: 0x00CED2E0
// FUNCTION: fn_21365
void CGameMode::SendMousePointerPosition(float x, float y, int value) {
  float position[2];
  position[0] = x;
  position[1] = y;
  const std::intptr_t address = reinterpret_cast<std::intptr_t>(position);
  this->mousePointer->SendMsg(nullptr, AM_SETPOSMOUSE, address, value, 0, 0, 0, 0);
}

// OFFSET: 0x00CED330
// FUNCTION: fn_21366
void ShowCurrentMapName() {
  if (g_serviceOptions.Get(TT_SHOW_MAP_NAME_ON_OFF)) {
    const std::string mapName = g_session.m_navigationPlayerMap + std::string(".rsw");
    g_mapInfoMgr->ShowMapName(mapName);
  }
}

// GLOBAL: gl_25129
char g_chatLogPath[260];

// OFFSET: 0x00CED760
// FUNCTION: fn_21369
void CGameMode::WriteChatLog(const char* text) {
  if (g_session.IsAdminSubMode(7) == 0) {
    return;
  }

  ++this->chatLogLines;
  CFile previous;
  previous.Open(g_chatLogPath, 0, true, false);
  const std::uint32_t size = previous.GetSize();
  char* contents = new char[size];
  previous.Read(contents, size);
  previous.Close();
  g_whisperFile.Open(g_chatLogPath, 0x1001, true, false);
  g_whisperFile.Write(contents, size);
  g_whisperFile.Write(text, static_cast<std::uint32_t>(std::strlen(text) + 1));
  g_whisperFile.Write("\r\n", 2);
  g_whisperFile.Close();
  delete[] contents;
}

// OFFSET: 0x00CF08D0
// FUNCTION: fn_21417
bool CGameMode::IsEffectId990Group(int id) {
  bool result = false;
  switch (id) {
  case ITEMID_BLOODY_RED:
  case ITEMID_CRYSTAL_BLUE:
  case ITEMID_WIND_OF_VERDURE:
  case ITEMID_YELLOW_LIVE:
    result = true;
    break;
  }
  return result;
}

// OFFSET: 0x00CF0910
// FUNCTION: fn_21418
bool CGameMode::IsEffectId994Group(int id) {
  bool result = false;
  switch (id) {
  case ITEMID_FLAME_HEART:
  case ITEMID_MISTIC_FROZEN:
  case ITEMID_ROUGH_WIND:
  case ITEMID_GREAT_NATURE:
    result = true;
    break;
  }
  return result;
}

// OFFSET: 0x00CF0950
// FUNCTION: fn_21419
bool CGameMode::ConfirmDraggedItemUse() {
  if (g_session.equippedArrowIndex == this->drag.id) {
    ITEM_INFO item = g_session.LookupClientItemByIdentity(this->drag.id);
    if (item.identity[1] > 0) {
      if (g_replayAnimationScaling) {
        this->replayDragState[0] = 0;
      }
      g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      g_windowMgr.Confirm(LookupMessage(MSI_UNEQUIP_ARROW), 0, true, false, 0, "Message", 280, 120, 0);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CFE480
// FUNCTION: fn_21432
void CGameMode::RefreshStatusStateEffect(int actorId) {
  if (g_windowMgr.windowVisibilityState != 2 && actorId == static_cast<int>(g_session.m_aid)) {
    this->scene->player->skillEffect->SendMsg(nullptr, AM_SETCOUNT, 0, 1, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CDBC30
// FUNCTION: fn_21312
void __stdcall LaunchBulletSpriteEffect(const vector3d* position, int actorId, float scale, int job) {
  if (actorId == 0) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CSprArrowEffect* effect = static_cast<CSprArrowEffect*>(effects->Spawn(0, 0x964, *position, actorId, -1, 0, 0));
  if (effect != nullptr) {
    effect->delayFrames = scale;
    effect->SetBulletSprite(job);
  }
}

// OFFSET: 0x00CE8F10
// FUNCTION: fn_21344
void __stdcall RecordWhisper(const void* info, const unsigned char* data) {
  if (Replay::Instance().state != 2) {
    return;
  }

  if (Replay::Instance().header.options.recordRestricted == 0) {
    return;
  }

  GameModeReplayPacket packet;
  packet.header = 0x038A08AE;
  packet.length = 0xFE;
  std::memcpy(&packet.info, info, sizeof(packet.info));
  std::memcpy(packet.data, data, sizeof(packet.data));

  Replay::Instance().Record(packet.length, &packet, 0x2A, 0);
}

// OFFSET: 0x00CDFAF0
// FUNCTION: fn_21325
void CGameMode::IgnorePacket() {
}

// OFFSET: 0x00CE8EC0
// FUNCTION: fn_21343
void CGameMode::RemovePendingItemPickup(int value) {
  for (std::list<int>::iterator it = this->pickupReqItemNaidList.begin(); it != this->pickupReqItemNaidList.end(); ++it) {
    if (*it == value) {
      this->pickupReqItemNaidList.erase(it);
      return;
    }
  }
}

#pragma pack(push, 1)
struct GameModeMessageRecord {
  /* 0000 */ std::uint32_t header;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int message;
  /* 000A */ int value;
  /* 000E */ int first;
  /* 0012 */ int second;
};
#pragma pack(pop)

static_assert(sizeof(GameModeMessageRecord) == 0x16);

// OFFSET: 0x00CE8FC0
// FUNCTION: fn_21345
bool CGameMode::RecordReplayMessage(int message, int value, int first, int second) {
  if (Replay::Instance().state != 2 || message != 0x45 || value != 10000) {
    return false;
  }

  GameModeMessageRecord record;
  record.header = 0x038908AE;
  record.length = sizeof(GameModeMessageRecord);
  record.message = 0x45;
  record.value = 10000;
  record.first = first;
  record.second = second;

  Replay::Instance().Record(record.length, &record, 0x2A, 0);
  return true;
}

// OFFSET: 0x00CED260
// FUNCTION: fn_21364
void CGameMode::UpdateReplayActionSpeeds() {
  this->scene->player->SetActionSpeed(Replay::Instance().AnimationScale());
  for (CRenderObject* actor : this->scene->actors) {
    const float speed = Replay::Instance().AnimationScale();
    actor->SetActionSpeed(speed);
  }
}

// OFFSET: 0x00CF1010
// FUNCTION: fn_45498
int ConfirmGameModeAction96() {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BARTERITEM_NAME_COUNT)), 0xFF, 0, 0, 0);
    return 0;
  }
  g_modeMgr.gameMode->SendMsg(MM_COMMAND_PET, 1, 0, 0, 0);
  return 0;
}

// OFFSET: 0x00CF1060
// FUNCTION: fn_21425
std::uint32_t CGameMode::StopReplayRecording() {
  if (Replay::Instance().state == 2) {
    Replay::Instance().SetState(1);
    Replay::Instance().UpdateDuration();
    const std::uint32_t duration = Replay::Instance().Duration();
    Replay::Instance().CloseFile();
    return duration;
  }

  return 0;
}

#pragma pack(push, 1)
struct GameModeStoredRecord00DA {
  /* 0000 */ std::uint16_t type;
  /* 0002 */ int aid;
  /* 0006 */ int job;
  /* 000A */ std::uint16_t cellX;
  /* 000C */ std::uint16_t cellY;
  /* 000E */ unsigned char sex;
  /* 000F */ std::uint16_t head;
  /* 0011 */ int weapon;
  /* 0015 */ int shield;
  /* 0019 */ std::uint16_t accessory;
  /* 001B */ std::uint16_t accessory2;
  /* 001D */ std::uint16_t accessory3;
  /* 001F */ std::uint16_t headPalette;
  /* 0021 */ std::uint16_t bodyPalette;
  /* 0023 */ std::uint16_t robe;
  /* 0025 */ char text[0x18];
  /* 003D */ std::uint16_t bodyCostume;
};
#pragma pack(pop)

static_assert(offsetof(GameModeStoredRecord00DA, text) == 0x25);
static_assert(offsetof(GameModeStoredRecord00DA, bodyCostume) == 0x3D);

static_assert(sizeof(GameModeTextRecord) == 0x190);
static_assert(offsetof(GameModeTextRecord, buffer) == 0x1E);

// OFFSET: 0x00CFE4D0
// FUNCTION: fn_21433
void CGameMode::RefreshNotificationWindows(int value) {
  for (std::list<UIWindow*>::iterator it = this->notificationWindows.begin(); it != this->notificationWindows.end(); ++it) {
    (*it)->SendMsg(nullptr, UM_REFRESH, value, 0, 0, 0);
  }
}

// OFFSET: 0x00CEBF30
// FUNCTION: fn_21354
unsigned CGameMode::ReadReplayGroundItems(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  unsigned offset = 6;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (15000 != header.type) {
    offset = 0xFFFFFFFFu;
  } else {
    while (offset < data.size()) {
      header = {};
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
      offset += 6;

      switch (header.type) {
      case 0xCF: {
        std::vector<unsigned char> record(0x13);
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveItemFallEntry(record.data());
        break;
      }
      case 15001:
        return offset;
      default:
        offset += header.size;
        break;
      }
    }
  }

  return offset;
}

// OFFSET: 0x00CEC100
// FUNCTION: fn_21355
unsigned CGameMode::ReadReplayActors(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  unsigned offset = 6;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (10000 != header.type) {
    offset = 0xFFFFFFFFu;
  } else {
    std::vector<unsigned char> record;
    while (offset < data.size()) {
      header = {};
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
      offset += 6;

      if (static_cast<int>(header.size) > 0) {
        record.resize(header.size);
      }

      if (header.type == 10001) {
        break;
      }

      switch (header.type) {
      case 0xC9:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorMessage32(record.data());
        break;
      case 0xCA:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorTitle(record.data());
        break;
      case 0xCB:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceivePlayerMessage82(record.data());
        break;
      case 0xCC:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorText(record.data());
        break;
      case 0xD0:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorMoveEntryNamed(record.data());
        break;
      case 0xD2:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorStateEntryNamedOld(record.data());
        break;
      case 0xD4:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorMoveEntryLegacy(record.data());
        break;
      case 0xD5:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorMoveEntry(record.data());
        break;
      case 0xD6:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorMoveEntryBody(record.data());
        break;
      case 0xD7:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorStateEntryLegacy(record.data());
        break;
      case 0xD8:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorStateEntryNamed(record.data());
        break;
      case 0xD9:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveActorStateEntryNamedBody(record.data());
        break;
      case 0xDA: {
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        const auto* stored = reinterpret_cast<const GameModeStoredRecord00DA*>(record.data());
        this->SpawnStandingPc2(
          stored->aid, stored->job, stored->cellX, stored->cellY, stored->sex, stored->head,
          stored->weapon, stored->shield, stored->accessory, stored->accessory2, stored->accessory3, stored->headPalette,
          stored->bodyPalette, stored->robe, stored->text, stored->bodyCostume
        );
        break;
      }
      case 0x191:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->ReceiveStatusChangeEntry(record.data());
        break;
      default:
        offset += header.size;
        break;
      }
    }
  }

  return offset;
}

// OFFSET: 0x00CCFD70
// FUNCTION: fn_21248
void CGameMode::UpdatePetFriendlyWindow(int previousFriendly) {
  if (!g_serviceOptions.Get(TT_PETINFO_ALERT_ONOFF)) {
    return;
  }

  if (g_session.GetPetFriendlyState(g_session.m_petFriendly) == 0 && g_session.m_petFriendly <= 100) {
    if (this->petFriendlyNoticeIds.find(g_session.petGid) == this->petFriendlyNoticeIds.end()) {
      if (g_windowMgr.petInfoWnd == nullptr) {
        g_windowMgr.EnsureWindow(WID_PETINFOWND);
      }

      this->UpdatePetFriendlyNoticeIds(g_session.petGid, true);
    }
  }

  if (previousFriendly < g_session.m_petFriendly && g_session.m_petFriendly > 99) {
    this->UpdatePetFriendlyNoticeIds(g_session.petGid, false);
  }
}

// OFFSET: 0x00CECE30
// FUNCTION: fn_21362
bool CGameMode::SaveEmblemFile(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return false;
  }

  const int nameLength = *reinterpret_cast<const int*>(data.data());
  std::string name(reinterpret_cast<const char*>(data.data() + 4), nameLength);

  char path[260];
  std::memset(path, 0, sizeof(path));
  std::sprintf(path, "%s\\_tmpEmblem\\%s", g_applicationDirectory, name.c_str());

  CFile file;
  if (file.Open(path, 0, true, false)) {
    file.Close();
  } else {
    file.Close();
    std::FILE* output = std::fopen(path, "wb");
    if (output != nullptr) {
      std::fwrite(data.data() + 4 + nameLength, data.size() - nameLength - 4, 1, output);
      std::fclose(output);
    }
  }

  return true;
}

// OFFSET: 0x00CF0BA0
// FUNCTION: fn_21423
void WriteMyLog(const char* file, int line, const char* format, ...) {
  char message[0x801];
  std::memset(message, 0, sizeof(message));

  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(message, 0x800, format, arguments);
  va_end(arguments);

  if (std::strrchr(file, '\\') != nullptr) {
    file = std::strrchr(file, '\\');
  }

  std::string path = FormatString("mylog(%s).txt", "");
  std::FILE* output = std::fopen(path.c_str(), "a");
  if (output != nullptr) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    std::fprintf(
      output, "%-30s(%04d) %02d-%02d-%02d %02d:%02d:%02d.%03d %s\n", file, line, time.wYear, time.wMonth, time.wDay, time.wHour,
      time.wMinute, time.wSecond, time.wMilliseconds, message
    );
    std::fclose(output);
  }
}

// OFFSET: 0x00CF0090
// FUNCTION: fn_21411
std::string CGameMode::HuntTargetText(const RenewQuestOngoingInfo& quest, int index) {
  std::string text;
  if (std::strlen(quest.m_huntName[index]) == 0) {
    const int maxLevel = quest.m_huntMaxLevel[index];
    const int minLevel = quest.m_huntMinLevel[index];
    CRenewQuestMgr* manager = g_renewQuestMgr;
    text = static_cast<const std::string&>(manager->GetMonsterConditionText(quest.m_huntConditionFlags[index], minLevel, maxLevel));
  } else {
    text = static_cast<const std::string&>(FormatOwnedString("%s", quest.m_huntName[index]));
  }

  if (quest.m_huntKillCount[index] == quest.m_huntMaxCount[index]) {
    text = static_cast<const std::string&>(FormatOwnedString("%s [ %s ]", text.c_str(), LookupMessage(MSI_COMPLETE_HUNTING_QUEST)));
  } else {
    text = static_cast<const std::string&>(FormatOwnedString("%s [ %d / %d ]", text.c_str(), quest.m_huntKillCount[index], quest.m_huntMaxCount[index]));
  }

  return text;
}

#pragma pack(push, 1)
struct GameModeStoredSkillEffect {
  /* 0000 */ GameModeChunkHeader header;
  /* 0006 */ ActiveSkillEffect effect;
};
#pragma pack(pop)

static_assert(sizeof(GameModeStoredSkillEffect) == 0x22);

#pragma pack(push, 1)
struct GameModeStoredValue {
  /* 0000 */ GameModeChunkHeader header;
  /* 0006 */ int value;
};
#pragma pack(pop)

static_assert(sizeof(GameModeStoredValue) == 0xA);

// OFFSET: 0x00CE9050
// FUNCTION: fn_21346
std::vector<unsigned char> CGameMode::SaveActiveSkillEffects() {
  std::vector<unsigned char> data;

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(19000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (ActiveSkillEffect effect : g_activeSkillEffects) {
    if (effect.expiration != 0 && effect.expiration != 999999) {
      effect.expiration -= timeGetTime();
    }

    const std::vector<unsigned char> chunk = MakeRecordChunk(0x4A43, effect);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(19001);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

// OFFSET: 0x00CE9DB0
// FUNCTION: fn_21348
std::vector<unsigned char> CGameMode::SaveQuestState() {
  std::vector<unsigned char> data;

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(14000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const std::vector<unsigned char> chunk = MakeRecordChunk(14011, this->isOnQuest);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(14001);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

// OFFSET: 0x00CF0E30
// FUNCTION: fn_21424
void CGameMode::SendNpcInputText(std::uint32_t npcId, const char* text, const char* replacement, int unused) {
  if (npcId != 0 && this->scene->GetGameActorByAID(npcId) == nullptr) {
    return;
  }

  char buffer[0x16A];
  std::memset(buffer, 0, sizeof(buffer));
  StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "%s", replacement != nullptr ? replacement : text);
  if (GetNavigationLinkParser()->HasUnownedItemLink(buffer) == true) {
    return;
  }

  const std::size_t length = std::strlen(buffer);
  const std::size_t size = length + 8;
  unsigned char* packet = static_cast<unsigned char*>(
    _MallocaComputeSize(size) != 0
      ? (_MallocaComputeSize(size) <= _ALLOCA_S_THRESHOLD
          ? _MarkAllocaS(_alloca(_MallocaComputeSize(size)), _ALLOCA_S_STACK_MARKER)
          : _MarkAllocaS(std::malloc(_MallocaComputeSize(size)), _ALLOCA_S_HEAP_MARKER))
      : nullptr
  );
  if (packet == nullptr) {
    return;
  }

  *reinterpret_cast<std::uint32_t*>(packet + 4) = npcId;
  *reinterpret_cast<std::uint16_t*>(packet) = 0x1D5;
  *reinterpret_cast<std::uint16_t*>(packet + 2) = static_cast<std::uint16_t>(length + 8);
  memcpy_s(packet + 8, length, buffer, length);
  GetRagConnection().Send(*reinterpret_cast<std::uint16_t*>(packet + 2), packet);
  _freea(packet);
}

// GLOBAL: gl_25200
float g_cameraMinDistance = 230.0f;

// GLOBAL: gl_24576
float g_cameraBaseDistance = 300.0f;

// GLOBAL: gl_25201
float g_cameraPitch = -45.0f;

// GLOBAL: gl_25202
float g_indoorCameraPitch = -45.0f;

// GLOBAL: gl_25203
float g_indoorCameraDistance = 300.0f;

// GLOBAL: gl_25204
float g_outdoorCameraPitch = -45.0f;

// GLOBAL: gl_25205
float g_outdoorCameraDistance = 300.0f;

// OFFSET: 0x00CECB90
// FUNCTION: fn_21359
void CGameMode::InitializeCamera() {
  {
    const float z = this->scene->player->render.position.z;
    const float y = this->scene->player->render.position.y;
    const float x = this->scene->player->render.position.x;
    CView* view = this->camera;
    view->camera.target[0] = x;
    view->camera.target[1] = y;
    view->camera.target[2] = z;
  }
  {
    const float z = this->scene->player->render.position.z;
    const float y = this->scene->player->render.position.y;
    const float x = this->scene->player->render.position.x;
    CView* view = this->camera;
    view->camera.desiredTarget[0] = x;
    view->camera.desiredTarget[1] = y;
    view->camera.desiredTarget[2] = z;
  }
  this->hasViewParameters = 0;

  int found;
  const std::vector<std::string>* parameters = g_session.GetMapViewParameters(this->mapName, &found);
  if (found != 0) {
    this->hasViewParameters = 1;
    for (int index = 0; index < 9; ++index) {
      this->viewParameters[index] = static_cast<std::int16_t>(std::atof((*parameters)[index].c_str()));
    }
  }

  const bool indoor = g_session.IsIndoorMapName(this->mapName);
  this->canRotateView = !indoor;
  float pitchRange = 10.0f;
  float maxDistance = g_cameraBaseDistance;
  float pitch;
  float distance;
  if (!indoor) {
    pitchRange = 20.0f;
    maxDistance = g_serviceOptionJitterScale;
    pitch = g_outdoorCameraPitch;
    distance = g_outdoorCameraDistance;
  } else {
    this->fixedLongitude = 45.0f;
    this->camera->camera.yaw = 45.0f;
    this->camera->camera.desiredYaw = this->fixedLongitude;
    pitch = g_indoorCameraPitch;
    distance = g_indoorCameraDistance;
  }

  g_cameraDistance = distance;
  g_cameraPitch = pitch;
  if (-45.0f - pitchRange > pitch) {
    pitch = -45.0f - pitchRange;
    g_cameraPitch = pitch;
  }
  if (pitch > pitchRange - 45.0f) {
    pitch = pitchRange - 45.0f;
    g_cameraPitch = pitch;
  }
  if (g_cameraMinDistance > distance) {
    distance = g_cameraMinDistance;
    g_cameraDistance = distance;
  }
  if (distance > maxDistance) {
    g_cameraDistance = maxDistance;
  }

  if (this->hasViewParameters != 0) {
    g_cameraDistance = static_cast<float>(this->viewParameters[2]);
    this->camera->camera.yaw = static_cast<float>(this->viewParameters[5]);
    this->camera->camera.desiredYaw = static_cast<float>(this->viewParameters[5]);
    pitch = static_cast<float>(this->viewParameters[8]);
    g_cameraPitch = pitch;
  }

  this->camera->camera.pitch = pitch;
  this->camera->camera.desiredPitch = g_cameraPitch;
  this->camera->camera.distance = g_cameraDistance;
  this->camera->camera.desiredDistance = g_cameraDistance;
}

#include "Contents/Schedule/Schedule.h"
#include "UI/Text/UINameBalloonText.h"
#include "UI/Text/UITransBalloonText.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Actors/ActorPickNode.h"
#include "Client/Application/Registry.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Render/Textures/TextureCache.h"
#include "Security/ReassemblyPacketMgr.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include <chrono>
#include <winsock2.h>
#include "UI/Dialogs/UIWordDisplayWnd.h"

class CPingLiveSchedule : public CSchedule {
public:
  // INLINED
  CPingLiveSchedule(const std::uint64_t& timestamp, const std::int64_t& interval)
      : CSchedule(timestamp, interval, 1, ++g_schedulerSerial, 0) {
  }

  void Process(const std::uint64_t& now) override;
};

// OFFSET: 0x00CDEDF0
// FUNCTION: fn_21321
void CPingLiveSchedule::Process(const std::uint64_t& now) {
  this->timestamp = now;
  if (GetRagConnection().NativeSocket() == INVALID_SOCKET) {
    this->active = false;
    return;
  }

  const DWORD tick = timeGetTime();
  if (g_session.m_lastReceiveTick + 60000 < tick) {
    this->active = false;
    GetRagConnection().Close();
    g_windowMgr.EnsureWindow(WID_DISCONNECTEDSERVERMSGWND);
    return;
  }

  if (g_session.m_lastSendTick + 10000 < tick) {
    GameModePacketHeader packet;
    packet.packetId = ID_CZ_PING_LIVE;
    GetRagConnection().Send(sizeof(packet), &packet);
  }
}

#include "Client/Mode/ModeFadeOut.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Resource/ResourceMgr.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Utility/Cipher/XorBytes.h"

namespace {

}

// OFFSET: 0x00CDEF00
// FUNCTION: fn_21322
int CGameMode::Run() {
  while (this->running) {
    if (g_modalImmediate) {
      break;
    }

    if (g_windowMgr.modalWindows.empty() || g_windowMgr.modalWindows.back() == nullptr) {
      if (!g_windowMgr.persistentWindows.empty()) {
        if (g_windowMgr.retainedPersistentWindow) {
          for (UIWindow* window : g_windowMgr.persistentWindows) {
            g_windowMgr.Schedule(window);
          }
          g_windowMgr.retainedPersistentWindow = false;
        }
        g_windowMgr.persistentWindows.clear();
      }

      if (this->pendingCursorType != -1) {
        this->cursorType = this->pendingCursorType;
        this->cursorFrame = 0;
        this->pendingCursorType = -1;
        this->OnChangeState(this->cursorType);
      }
    }

    if (g_nProtectGameGuardMgr->HasError()) {
      GetRagConnection().Close();
      g_modeMgr.Quit();
      std::string message = g_nProtectGameGuardMgr->ErrorMessage();
      if (!message.empty()) {
        MessageBoxA(hWnd, message.c_str(), OBFUSCATE_TEXT("nProtect GameGuard", 0xADC51BF9C98B413DULL), 0);
      }
    }

    this->Update();
    Sleep(1);
  }

  FadeOutMode(*g_renderer, *GetRenderSystemHolder()->system, ResourceMgr::Instance(), 0, false, 0.0f, 0.0f);
  return 1;
}

#include "Client/Input/Mouse.h"

// OFFSET: 0x00CDFC40
// FUNCTION: fn_21327
void CGameMode::TargetActorByAid(int aid) {
  if (this->isPlayerDead != 0) {
    return;
  }

  const unsigned char selfTargetOption = g_serviceOptions.Get(TT_NOSHIFT_ON_OFF);
  const unsigned char forceOption = g_serviceOptions.Get(TT_NOCTRL_ON_OFF);
  const int shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
  int force = shiftHeld;
  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
  const bool skillMode = mode == 2 || mode == 4;
  const bool allowed = aid != static_cast<int>(g_session.m_aid) || mode == 4;

  if (skillMode == true) {
    const bool support = IsSupportSkill(this->skillId);
    const bool optionalSelf = IsOptionalSelfTargetSkill(this->skillId);
    if (support == true) {
      force = 1;
    }
    if ((selfTargetOption != 0 || shiftHeld != 0) && optionalSelf == true) {
      force = 1;
    }

    CWorld* scene = this->scene;
    if (scene->isPKZone == 0 && scene->isEventPVPMode == 0) {
      if (scene->isSiegeMode != 0) {
        CGameActor* actor = static_cast<CGameActor*>(scene->GetGameActorByAID(aid));
        if (actor != nullptr && !g_session.IsLocalPlayerAid(actor->GetGdid())) {
          GuildInfo* guildInfo = &g_guildInfo;
          if (!guildInfo->HasRelatedGuild(actor->GetGdid())) {
            force = 1;
          }
        }
      }
    } else if (!g_session.IsPartyMember(aid, true)) {
      force = 1;
    }
  }

  if (allowed && g_mouse.buttons[0] == 1 && force != 0 && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
    this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
    this->isAutoMoveClickOn = 0;
    if (g_session.GetSiegeMode() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
      this->RequestPlayerAction(aid, 0);
      this->forcedAttack = 1;
      return;
    }
    this->RequestPlayerAction(aid, 1);
    this->forcedAttack = 0;
  }
}

#include "Contents/Actors/Skill.h"

#pragma pack(push, 1)
struct SkillEntryPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t creatorAid;
  /* 000A */ std::int16_t x;
  /* 000C */ std::int16_t y;
  /* 000E */ unsigned char job;
  /* 000F */ unsigned char isVisible;
};
#pragma pack(pop)

static_assert(sizeof(SkillEntryPacket) == 16);

#pragma pack(push, 1)
struct GameModeStoredSkillEntry {
  /* 0000 */ GameModeChunkHeader header;
  /* 0006 */ SkillEntryPacket packet;
};
#pragma pack(pop)

static_assert(sizeof(GameModeStoredSkillEntry) == 0x16);

// OFFSET: 0x00CEB4C0
// FUNCTION: fn_21351
std::vector<unsigned char> CGameMode::WriteReplaySkillState() {
  std::vector<unsigned char> data;

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(17000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (CRenderObject* object : this->scene->skills) {
    CSkill* skill = static_cast<CSkill*>(object);
    SkillEntryPacket packet;
    packet.packetType = ID_ZC_SKILL_ENTRY;
    packet.aid = skill->skillAid;
    packet.creatorAid = static_cast<std::uint32_t>(skill->sourceActorId);
    packet.job = static_cast<unsigned char>(skill->GetEffectiveJob(-1));
    packet.isVisible = 1;

    const C3dAttr* terrain = this->scene->terrain;
    const float column = static_cast<float>(std::floor(static_cast<float>(terrain->width / 2) + skill->render.position.x / static_cast<float>(terrain->cellSize)));
    const float row = static_cast<float>(std::floor(static_cast<float>(terrain->height / 2) + skill->render.position.z / static_cast<float>(terrain->cellSize)));
    packet.x = static_cast<std::int16_t>(static_cast<int>(column));
    packet.y = static_cast<std::int16_t>(static_cast<int>(row));

    const std::vector<unsigned char> chunk = MakeRecordChunk(0xCD, packet);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(17001);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

// OFFSET: 0x00CEBBB0
// FUNCTION: fn_21353
unsigned CGameMode::ReadReplayGuildInfo(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  std::vector<unsigned char> record;
  unsigned offset = 6;
  std::uint32_t guildId = 0;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (13000 != header.type) {
    offset = 0xFFFFFFFFu;
  } else {
    while (offset < data.size()) {
      header = {};
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
      offset += 6;

      if (static_cast<int>(header.size) > 0) {
        record.resize(header.size);
      }

      switch (header.type) {
      case 13001:
        goto finished;
      case 13002:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->SaveEmblemFile(record);
        break;
      case 13003:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->nameCard.Load(reinterpret_cast<const std::vector<char>&>(record));
        break;
      case 13004:
        std::memcpy(&guildId, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 13005: {
        int emblemVersion = 0;
        std::memcpy(&emblemVersion, data.data() + offset, header.size);
        offset += header.size;
        this->scene->player->SetGuildInfo(guildId, emblemVersion);
        g_guildInfo.m_guildId = guildId;
        break;
      }
      default:
        offset += header.size;
        break;
      }
    }
  }

finished:
  return offset;
}

#include "Contents/Actors/Item.h"

// OFFSET: 0x00CE93E0
// FUNCTION: fn_21347
std::vector<unsigned char> CGameMode::SaveNameCardState() {
  std::vector<unsigned char> data;

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(13000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  std::vector<char> buffer;
  std::vector<std::string> names;
  if (static_cast<int>(this->nameCard.Save(buffer)) > 0) {
    const std::vector<unsigned char> chunk = MakeDataChunk(13003, buffer);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    if (this->AppendGuildEmblem(buffer, names, this->scene->player->GetGdid(), g_guildMgr->GetEmblemVersion(this->scene->player->GetGdid()))) {
      const std::vector<unsigned char> chunk = MakeDataChunk(13002, buffer);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }

  {
    const std::vector<unsigned char> chunk = MakeRecordChunk(13004, this->scene->player->GetGdid());
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const std::vector<unsigned char> chunk = MakeRecordChunk(13005, g_guildMgr->GetEmblemVersion(this->scene->player->GetGdid()));
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (CRenderObject* object : this->scene->actors) {
    CGameActor* actor = static_cast<CGameActor*>(object);
    if (this->AppendGuildEmblem(buffer, names, actor->GetGdid(), g_guildMgr->GetEmblemVersion(actor->GetGdid()))) {
      const std::vector<unsigned char> chunk = MakeDataChunk(13002, buffer);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(13001);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

#include "UI/Chat/UIChatRoomTitle.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Text/UIStaticText.h"

#pragma pack(push, 1)
struct ChatRoomEntryPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t packetLength;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ std::uint32_t roomId;
  /* 000C */ std::uint16_t maxCount;
  /* 000E */ std::uint16_t curCount;
  /* 0010 */ unsigned char type;
};

struct ShopEntryPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ char name[0x50];
};

struct ProgressPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t color;
  /* 0006 */ std::uint32_t seconds;
};

struct GameModeStoredRecord0191 {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint16_t statusType;
  /* 0008 */ int total;
  /* 000C */ int remaining;
  /* 0010 */ int count;
  /* 0014 */ int value2;
  /* 0018 */ int value3;
};
#pragma pack(pop)

static_assert(sizeof(ChatRoomEntryPacket) == 0x11);
static_assert(sizeof(ShopEntryPacket) == 0x56);
static_assert(sizeof(ProgressPacket) == 0xA);
static_assert(sizeof(GameModeStoredRecord0191) == 0x1C);
static_assert(sizeof(GameModeStoredRecord00DA) == 0x3F);

// OFFSET: 0x00CEA510
// FUNCTION: fn_21350
std::vector<unsigned char> CGameMode::SaveActorStates() {
  std::vector<unsigned char> data;

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(10000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  std::vector<char> scratch;
  for (std::list<CRenderObject*>::iterator it = this->scene->actors.begin(); it != this->scene->actors.end(); ++it) {
    if (static_cast<CGameActor*>(*it)->storedActorEntry == 0) {
      if (static_cast<CGameActor*>(*it)->render.state == 1) {
        const std::vector<unsigned char> chunk = MakeDataChunk(0xD6, this->BuildActorEntryPacket(static_cast<CGameActor*>(*it), 0x9FD));
        data.insert(data.end(), chunk.begin(), chunk.end());
      } else {
        const std::vector<unsigned char> chunk = MakeDataChunk(0xD9, this->BuildActorEntryPacket(static_cast<CGameActor*>(*it), 0x9FF));
        data.insert(data.end(), chunk.begin(), chunk.end());
      }
    }

    UIChatRoomTitle* room = static_cast<UIChatRoomTitle*>(static_cast<CGameActor*>(*it)->actorWindows[1]);
    if (room != nullptr) {
      scratch.resize(sizeof(ChatRoomEntryPacket), 0);
      auto* packet = reinterpret_cast<ChatRoomEntryPacket*>(scratch.data());
      packet->type = 0xD7;
      packet->packetLength = static_cast<std::uint16_t>(room->title.size() + sizeof(ChatRoomEntryPacket));
      packet->aid = static_cast<CGameActor*>(*it)->render.accountId;
      packet->roomId = room->roomId;
      packet->maxCount = static_cast<std::uint16_t>(room->memberLimit);
      packet->curCount = static_cast<std::uint16_t>(room->memberCount);
      packet->type = static_cast<unsigned char>(room->roomType);
      scratch.insert(scratch.end(), room->title.begin(), room->title.end());

      const std::vector<unsigned char> chunk = MakeDataChunk(0xC9, scratch);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    CGameActor* actor = static_cast<CGameActor*>(*it);
    if (actor->storedActorEntry == 1) {
      CPc* pc = static_cast<CPc*>(actor);
      std::vector<char> record;
      record.resize(sizeof(GameModeStoredRecord00DA), 0);
      auto* stored = reinterpret_cast<GameModeStoredRecord00DA*>(record.data());
      stored->aid = actor->render.accountId;
      stored->job = actor->GetEffectiveJob(-1);
      stored->sex = static_cast<unsigned char>(actor->sex);
      stored->cellX = static_cast<std::uint16_t>(actor->movementState[5]);
      stored->cellY = static_cast<std::uint16_t>(actor->destCellY);
      stored->head = static_cast<std::uint16_t>(pc->head);
      const std::int64_t value = actor->GetWeapon();
      stored->weapon = static_cast<int>(value);
      stored->shield = static_cast<int>(value >> 32);
      stored->accessory = static_cast<std::uint16_t>(pc->accessory);
      stored->accessory2 = static_cast<std::uint16_t>(pc->accessory2);
      stored->accessory3 = static_cast<std::uint16_t>(pc->accessory3);
      stored->headPalette = static_cast<std::uint16_t>(pc->headPalette);
      stored->bodyPalette = static_cast<std::uint16_t>(actor->render.bodyPalette);
      stored->robe = static_cast<std::uint16_t>(pc->robe);
      stored->bodyCostume = static_cast<std::uint16_t>(actor->GetBodyCostume());
      {
        std::string name = this->nameCard.Name(actor->render.accountId);
        std::memcpy(stored->text, name.c_str(), sizeof(stored->text));
      }

      const std::vector<unsigned char> chunk = MakeDataChunk(0xDA, record);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    UIMerchantShopTitle* shop = static_cast<UIMerchantShopTitle*>(static_cast<CGameActor*>(*it)->actorWindows[2]);
    if (shop != nullptr) {
      scratch.resize(sizeof(ShopEntryPacket), 0);
      auto* packet = reinterpret_cast<ShopEntryPacket*>(scratch.data());
      packet->packetType = ID_ZC_STORE_ENTRY;
      packet->aid = static_cast<CGameActor*>(*it)->render.accountId;
      std::memcpy(packet->name, shop->label->tooltip.c_str(), sizeof(packet->name));

      const std::vector<unsigned char> chunk = MakeDataChunk(0xCA, scratch);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    if (static_cast<CGameActor*>(*it)->actorWindows[3] != 0) {
      scratch.resize(sizeof(ProgressPacket), 0);
      auto* packet = reinterpret_cast<ProgressPacket*>(scratch.data());
      packet->color = 0;
      packet->packetType = ID_ZC_PROGRESS;
      packet->seconds = (static_cast<CGameActor*>(*it)->skillRechargeEndTick - static_cast<CGameActor*>(*it)->skillRechargeStartTick) / 1000;

      const std::vector<unsigned char> chunk = MakeDataChunk(0xCB, scratch);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    UIMerchantShopTitle* buyingShop = static_cast<UIMerchantShopTitle*>(static_cast<CGameActor*>(*it)->actorWindows[4]);
    if (buyingShop != nullptr) {
      scratch.resize(sizeof(ShopEntryPacket), 0);
      auto* packet = reinterpret_cast<ShopEntryPacket*>(scratch.data());
      packet->packetType = ID_ZC_BUYING_STORE_ENTRY;
      packet->aid = static_cast<CGameActor*>(*it)->render.accountId;
      std::memcpy(packet->name, buyingShop->label->tooltip.c_str(), sizeof(packet->name));

      const std::vector<unsigned char> chunk = MakeDataChunk(0xCC, scratch);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    const int count = static_cast<CGameActor*>(*it)->GetMadogearType();
    if (count > 0) {
      GameModeStoredRecord0191 record;
      record.remaining = 9999;
      record.packetType = 0;
      record.count = count;
      record.value2 = 0;
      record.total = 9999;
      record.aid = static_cast<CGameActor*>(*it)->render.accountId;
      record.statusType = 0x47D;
      record.value3 = 0;

      std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader) + sizeof(record), 0);
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 0x191;
      header->size = sizeof(record);
      *reinterpret_cast<GameModeStoredRecord0191*>(chunk.data() + sizeof(GameModeChunkHeader)) = record;
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader), 0);
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 10001;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

#pragma pack(push, 1)
struct ItemEntryPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t itemId;
  /* 000A */ unsigned char identified;
  /* 000B */ std::int16_t x;
  /* 000D */ std::int16_t y;
  /* 000F */ std::int16_t count;
  /* 0011 */ unsigned char subX;
  /* 0012 */ unsigned char subY;
};

struct GameModeStoredItemEntry {
  /* 0000 */ GameModeChunkHeader header;
  /* 0006 */ ItemEntryPacket packet;
};
#pragma pack(pop)

static_assert(sizeof(ItemEntryPacket) == 0x13);
static_assert(sizeof(GameModeStoredItemEntry) == 0x19);

#include "Contents/Items/ItemNaming.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "Contents/Effects/Core/EffectActor.h"

#pragma pack(push, 1)
struct GameModeUiMessage {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int values[6];
  /* 001E */ char text[1];
};

struct GameModeUiWhisperMessage {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ char target[0x19];
  /* 001F */ char text[1];
};

struct GameModeUiKindTextMessage {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int kind;
  /* 000A */ int padding_000A;
  /* 000E */ char text[1];
};

struct GameModeUiNavigationMessage {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ char name[0x1E];
  /* 0024 */ int kind;
  /* 0028 */ unsigned char mode;
  /* 0029 */ unsigned char flag;
  /* 002A */ int x;
  /* 002E */ int y;
  /* 0032 */ int id;
};

struct GameModeUiDestinationMessage {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ char mapName[0x1E];
  /* 0024 */ int x;
  /* 0028 */ int y;
};

struct GameModeUiDragMessage {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int windowId;
  /* 000A */ int x;
  /* 000E */ int y;
  /* 0012 */ int source;
  /* 0016 */ std::uint32_t id;
  /* 001A */ std::uint32_t price;
  /* 001E */ std::uint32_t realPrice;
  /* 0022 */ int quantity;
  /* 0026 */ int slot;
  /* 002A */ std::uint32_t itemType;
  /* 002E */ std::uint32_t refine;
  /* 0032 */ unsigned char identified;
  /* 0033 */ char resource[100];
  /* 0097 */ char name[100];
  /* 00FB */ char skillName[100];
  /* 015F */ int skillLevel;
  /* 0163 */ unsigned char cards[16];
  /* 0173 */ unsigned char skill;
  /* 0174 */ int optionCount;
  /* 0178 */ unsigned char options[25];
};
#pragma pack(pop)

static_assert(offsetof(GameModeUiMessage, text) == 0x1E);
static_assert(offsetof(GameModeUiWhisperMessage, text) == 0x1F);
static_assert(offsetof(GameModeUiKindTextMessage, text) == 0x0E);
static_assert(offsetof(GameModeUiNavigationMessage, id) == 0x32);
static_assert(offsetof(GameModeUiDestinationMessage, y) == 0x28);
static_assert(offsetof(GameModeUiDragMessage, options) == 0x178);

// OFFSET: 0x00CCF680
// FUNCTION: fn_21244
void CGameMode::ProcessUiMessage(unsigned char* message) {
  GameModeUiMessage* values = reinterpret_cast<GameModeUiMessage*>(message);

  switch (values->type) {
  case 900: {
    if (values->values[1] == 0xB8) {
      if (values->values[0] == 0x29) {
        values->values[1] = 0xB9;
      } else if (values->values[0] == 0xAE) {
        break;
      }
    }
    UIWindow* window = g_windowMgr.LookupWindow(values->values[0]);
    if (window != nullptr) {
      window->SendMsg(nullptr, UM_COMMAND, values->values[1], values->values[2], values->values[3], values->values[4]);
    }
    break;
  }
  case 901:
    this->ReceiveUiDragMessage(message);
    break;
  case 902: {
    UIWindow* window = g_windowMgr.LookupWindow(values->values[0]);
    if (window != nullptr) {
      window->RestoreEditTexts(reinterpret_cast<const unsigned char*>(&values->values[1]));
    }
    break;
  }
  case 903:
    this->exchangeItemCnt = values->values[0];
    break;
  case 904: {
    char buffer[200];
    std::memset(buffer, 0, sizeof(buffer));
    std::memcpy(buffer, values->text, values->values[0]);
    this->chatRoom.title = buffer;
    std::memset(buffer, 0, sizeof(buffer));
    std::memcpy(buffer, values->text + values->values[0], values->values[1]);
    this->chatRoom.password = buffer;
    this->chatRoom.type = values->values[2];
    this->chatRoom.memberCount = values->values[3];
    this->chatRoom.memberLimit = values->values[4];
    this->chatRoom.id = values->values[5];
    break;
  }
  case 905:
    this->SendMsg(values->values[0], values->values[1], values->values[2], values->values[3], 0);
    this->SendMsg(MM_CANCEL_USE_SKILL, 0, 0, 0, 0);
    break;
  case 906: {
    const GameModeUiWhisperMessage* whisper = reinterpret_cast<const GameModeUiWhisperMessage*>(message);
    this->lastWhisperName = whisper->target;
    this->lastWhisper = whisper->text;
    break;
  }
  case 907: {
    const GameModeUiKindTextMessage* text = reinterpret_cast<const GameModeUiKindTextMessage*>(message);
    if (text->kind == 0x29) {
      g_session.storeTitle = text->text;
    } else if (text->kind == 0xAE) {
      g_session.storeTitle = text->text;
    }
    break;
  }
  case 908:
    if (values->values[0] == 0xCB && g_windowMgr.navigationWnd != nullptr) {
      g_windowMgr.navigationWnd->SendMsg(nullptr, values->values[1], values->values[2], reinterpret_cast<std::intptr_t>(&values->values[3]), 0, 0);
    }
    break;
  case 909:
    g_windowMgr.EnsureWindow(values->values[0]);
    break;
  case 910: {
    const GameModeUiNavigationMessage* navigation = reinterpret_cast<const GameModeUiNavigationMessage*>(message);
    g_navigationManager.StartNavigation(
      std::string(navigation->name), navigation->kind, navigation->mode, navigation->flag != 0, navigation->x, navigation->y, navigation->id
    );
    break;
  }
  case 911: {
    std::vector<char> states(values->length - 6, 0);
    std::memcpy(&states[0], values->values, states.size());
    g_windowMgr.CloseWindow(WID_COMBOBOXWND);
    g_navigationManager.LoadStates(states);
    if (g_windowMgr.navigationWnd != nullptr) {
      static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->FillRouteGroups();
    }
    break;
  }
  case 912: {
    const GameModeUiDestinationMessage* destination = reinterpret_cast<const GameModeUiDestinationMessage*>(message);
    g_navigationManager.SetDestinationMap(std::string(destination->mapName), destination->x, destination->y);
    break;
  }
  case 913: {
    if (values->values[1] == 0xB8) {
      if (values->values[0] == 0x29) {
        values->values[1] = 0xB9;
      } else if (values->values[0] == 0xAE) {
        break;
      }
    }
    UIWindow* window = g_windowMgr.LookupWindow(values->values[0]);
    if (window != nullptr) {
      window->SendMsg(nullptr, values->values[5], values->values[1], values->values[2], values->values[3], values->values[4]);
    }
    break;
  }
  case 914:
    g_windowMgr.CloseWindowExcept251(values->values[0]);
    break;
  }
}

// OFFSET: 0x00CE8400
// FUNCTION: fn_21341
void CGameMode::ReceiveUiDragMessage(const unsigned char* message) {
  const GameModeUiDragMessage* packet = reinterpret_cast<const GameModeUiDragMessage*>(message);

  ModeDragState drag;
  drag.source = packet->source;
  drag.id = packet->id;
  drag.itemValue = packet->price;
  drag.price = packet->realPrice;
  drag.quantity = packet->quantity;
  drag.slot = packet->slot;
  drag.type = packet->itemType;
  drag.refine = packet->refine;
  drag.identified = packet->identified;
  drag.resource = packet->resource;
  drag.name = packet->name;
  drag.skillName = packet->skillName;
  drag.skillLevel = packet->skillLevel;
  std::memcpy(drag.cards, packet->cards, sizeof(drag.cards));
  drag.skill = packet->skill;
  drag.optionCount = packet->optionCount;
  if (drag.optionCount > 5) {
    drag.optionCount = 0;
  }
  for (int index = 0; index < drag.optionCount; ++index) {
    std::memcpy(&drag.options[index], &packet->options[index * 5], sizeof(drag.options[index]));
  }

  UIWindow* window = g_windowMgr.LookupWindow(packet->windowId);
  this->drag = drag;

  if (this->drag.quantity != 1) {
    this->replayDragState[0] = this->IsAllowedPair(packet->windowId, packet->source);
  } else {
    const int kind = packet->windowId;
    if (kind == 0x17 || kind == 0x2C) {
      if (packet->itemType <= 3 || packet->itemType == 7) {
        this->replayDragState[0] = this->IsAllowedPair(kind, packet->source);
      }
    } else if (kind == 0x16) {
      if (packet->itemType <= 3 || packet->itemType == 7) {
        this->replayDragState[0] = this->IsAllowedPair(0x16, packet->source);
      }
    }
  }

  if (window != nullptr) {
    window->DragAndDrop(packet->x, packet->y, reinterpret_cast<std::intptr_t>(&drag));
  }

  const int kind = packet->windowId;
  if (kind == 0x24 || kind == 0x9F) {
    this->replayDragState[0] = 0;
  } else if (this->replayDragState[0] == 1 && g_windowMgr.itemDropCntWnd != nullptr) {
    if (drag.type != 6) {
      const ITEM_DESC description = FindItemDescription(std::atoi(drag.name.c_str()));
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETTEXT, 0, reinterpret_cast<std::intptr_t>(description.identifiedName), 0, 0);
    } else {
      const ItemCardName card = FindItemCardName(std::atoi(drag.name.c_str()));
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETTEXT, 0, reinterpret_cast<std::intptr_t>(card.name), 0, 0);
    }
  }

  this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00CEA0C0
// FUNCTION: fn_21349
std::vector<unsigned char> CGameMode::SaveGroundItems() {
  std::vector<unsigned char> data;

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(15000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (auto it = this->scene->items.begin(); it != this->scene->items.end(); ++it) {
    CItem* item = static_cast<CItem*>(*it);
    ItemEntryPacket packet;
    packet.packetType = ID_ZC_ITEM_ENTRY;
    packet.aid = item->itemAid;
    packet.itemId = item->itemId;
    packet.identified = item->identified;

    int column;
    int row;
    int subColumn;
    int subRow;
    this->WorldSubcell(item->render.position.x, item->render.position.z, column, row, subColumn, subRow);
    packet.x = static_cast<std::int16_t>(column);
    packet.y = static_cast<std::int16_t>(row);
    packet.subX = static_cast<unsigned char>(subColumn);

    const int length = static_cast<int>(item->name.size());
    std::size_t digits = 0;
    for (int index = length - 4; index >= 0; --index) {
      if (item->name[index] == ' ') {
        break;
      }
      ++digits;
    }
    char text[6] = {};
    std::memcpy(text, item->name.c_str() + (length - 3) - digits, digits);
    packet.count = static_cast<std::int16_t>(std::atoi(text));

    packet.subY = static_cast<unsigned char>(subRow);
    const std::vector<unsigned char> chunk = MakeRecordChunk(0xCF, packet);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const std::vector<unsigned char> chunk = MakeEmptyChunk(15001);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

// OFFSET: 0x00CEFFC0
// FUNCTION: fn_21409
UIBroadcastBalloon* CGameMode::EnsureBroadcastBalloon() {
  if (!this->broadcastBalloon) {
    this->broadcastBalloon = new UIBroadcastBalloon();
    this->broadcastBalloon->SetSize(g_renderer->viewport.width, 50);
    this->broadcastBalloon->Move(0, 51);
    g_windowMgr.AddRenderedFront(this->broadcastBalloon);
  }
  return this->broadcastBalloon;
}

// OFFSET: 0x00CDBB10
// FUNCTION: fn_21311
void CGameMode::EnsureNameBalloon(UINameBalloonText*& balloon) {
  if (balloon) {
    return;
  }

  switch (g_serviceOptions.Get(TT_NAME_ABOVE_HEAD_ON_OFF) != 0) {
  case NAME_BALLOON_BOTTOM:
    balloon = new UINameBalloonText();
    break;
  case NAME_BALLOON_TOP:
    balloon = new UINameBalloonText();
    break;
  case NAME_BALLOON_LEFT:
    balloon = new UIVerticalNameBalloonText();
    break;
  case NAME_BALLOON_RIGHT:
    balloon = new UIVerticalNameBalloonText();
    break;
  }

  balloon->SetSize(8, 8);
  g_windowMgr.AddRenderedFront(balloon);
}

// OFFSET: 0x00CDD240
// FUNCTION: fn_21315
void __stdcall OpenWordDisplay(CGameActor* actor) {
  if (actor->actorTitleWindow || IsAdminAccount(actor->render.accountId)) {
    return;
  }

  if (!IsAdminAccount(g_session.m_aid)) {
    if (StateFlag02(actor->GetStateFlags()) || StateFlag01(actor->GetStateFlags()) || StateFlag06(actor->GetStateFlags())) {
      return;
    }
  }

  UIWordDisplayWnd* window = new UIWordDisplayWnd();
  window->SendMsg(nullptr, AM_UPDATEGAGE, actor->render.accountId, 0, 0, 0);
  g_windowMgr.AddRendered(window);
  actor->actorTitleWindow = window;
}

// OFFSET: 0x00CFF070
// FUNCTION: fn_21435
void CGameMode::ShowNoticeBalloon(const std::vector<std::string>* lines) {
  if (!lines || lines->empty()) {
    return;
  }

  UIBroadcastBalloon* balloon = this->noticeBalloon;
  if (!balloon) {
    this->noticeBalloon = new UIBroadcastBalloon();
    this->noticeBalloon->SetSize(g_renderer->viewport.width, 50);
    this->noticeBalloon->Move(0, 130 * g_renderer->viewport.height / 480);
    g_windowMgr.AddRenderedFront(this->noticeBalloon);
    balloon = this->noticeBalloon;
    if (!balloon) {
      return;
    }
  }

  BroadcastBalloonMessage message;
  for (std::vector<std::string>::const_iterator it = lines->begin(); it != lines->end(); ++it) {
    message.lines.emplace_back(*it, 0xFFFF, 0);
  }
  balloon->AddMessage(2, message);
}

#pragma pack(push, 1)
struct ZoneEnterPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t gid;
  /* 000A */ int authCode;
  /* 000E */ unsigned char padding_000E[4];
  /* 0012 */ std::uint32_t clientTime;
  /* 0016 */ unsigned char sex;
};
#pragma pack(pop)

static_assert(sizeof(ZoneEnterPacket) == 0x17);
static_assert(offsetof(ZoneEnterPacket, clientTime) == 0x12);
static_assert(offsetof(ZoneEnterPacket, sex) == 0x16);

// OFFSET: 0x00D06CB0
// FUNCTION: fn_21579
void CGameMode::ChangeZoneServer(std::string mapName, int x, int y, int ip, int port, std::string host) {
  g_session.m_startTimeTicks = std::chrono::system_clock::now().time_since_epoch().count();
  g_session.m_loginTimeTicks = std::chrono::system_clock::now().time_since_epoch().count();
  g_session.SetOption(20, 1, 0);
  this->nameCard.Clear();
  g_session.ClearStoreSearchResults();
  g_session.ClearShortcutSlots();
  g_session.ClearSecondaryShortcutSlots();
  g_session.playerAppearanceState = 0;
  g_session.ResetInventoryItems();
  g_session.ClearPartyMembers();

  std::string name = mapName;
  name.resize(name.size() - 4);
  g_session.SetNavigationPlayerMap(name.c_str());
  g_session.m_playerX = x;
  g_session.m_playerY = y;

  in_addr address;
  address.s_addr = static_cast<ULONG>(ip);
  StringFormat(g_zoneServerEndpoint.address, sizeof(g_zoneServerEndpoint.address), static_cast<std::size_t>(-1), inet_ntoa(address));
  g_zoneServerEndpoint.port = port;
  StringFormat(g_zoneServerEndpoint.overrideAddress, sizeof(g_zoneServerEndpoint.overrideAddress), static_cast<std::size_t>(-1), "%s", host.c_str());
  GetRagConnection().Close();
  g_activeSkillEffects.clear();

  this->noMove = 1;
  this->noMoveStartTick = timeGetTime();
  this->scene->player->SendMsg(nullptr, AM_FREEZE, 0, 0, 0, 0, 0, 0);
  if (g_screenShakeDuration > 0) {
    g_screenShakeDuration = g_screenShakeMode == 2 ? 10001 : g_screenShakeDuration;
  }

  if (g_replayAnimationScaling == 0) {
    this->connectionReady = GetRagConnection().Connect(g_zoneServerEndpoint);
    if (!this->connectionReady) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 1, true, true, 0, "Message", 280, 120, 0) != 269) {
        this->SendMsg(MM_QUIT_TO_IDENTRY, 0, 0, 0, 0);
      }
      return;
    }

    g_nProtectGameGuardMgr->IncrementCounter();
    g_packetReassembler->Notify(1);
    GetRagConnection().SendDeadline() = timeGetTime() + 1000;

    ZoneEnterPacket packet;
    packet.packetType = ID_CZ_ENTER2;
    packet.clientTime = timeGetTime();
    packet.aid = g_session.m_aid;
    packet.gid = g_session.m_gid;
    packet.authCode = g_session.m_authCode;
    packet.sex = static_cast<unsigned char>(g_session.GetCurrentSex());
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
    g_session.SetOption(17, 1, 0);
    GetRagConnection().SetBlock(1);
  }

  static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
  g_windowMgr.modalResponse = 185;
}

// OFFSET: 0x00CE1FA0
// FUNCTION: fn_21333
void CGameMode::UpdateBalloons() {
  if (this->nameBalloon) {
    if (this->isNameBalloonPlaced == 0) {
      this->isNameBalloonPlaced = 1;
      if (this->balloonActorId == g_session.m_aid) {
        this->PlaceWindowAtActor(this->nameBalloon, this->scene->player);
      } else {
        CRenderObject* actor = this->scene->GetGameActorByAID(this->balloonActorId);
        if (actor) {
          this->PlaceWindowAtActor(this->nameBalloon, actor);
        } else {
          CItem* item = this->scene->GetItemByAID(this->balloonActorId);
          if (item) {
            this->PlaceWindowAtActor(this->nameBalloon, item);
          }
        }
      }
    } else {
      g_windowMgr.Schedule(this->nameBalloon);
      this->nameBalloon = nullptr;
    }
  }

  if (this->itemNameBalloon) {
    if (this->isItemNameBalloonPlaced == 0) {
      this->isItemNameBalloonPlaced = 1;
      if (this->balloonActorId == g_session.m_aid) {
        this->PlaceWindowAtActor(this->itemNameBalloon, this->scene->player);
      } else {
        CRenderObject* actor = this->scene->GetGameActorByAID(this->balloonActorId);
        if (actor) {
          this->PlaceWindowAtActor(this->itemNameBalloon, actor);
        } else {
          CItem* item = this->scene->GetItemByAID(this->balloonActorId);
          if (item) {
            this->PlaceWindowAtActor(this->itemNameBalloon, item);
          }
        }
      }
    } else {
      g_windowMgr.Schedule(this->itemNameBalloon);
      this->itemNameBalloon = nullptr;
    }
  }

  UIWindow*& skillBalloon = this->skillBalloon;
  UIWindow*& hintBalloon = this->skillHintBalloon;
  int& shownSkill = this->shownSkillId;
  if (this->skillUseType == nullptr) {
    if (skillBalloon) {
      g_windowMgr.Schedule(skillBalloon);
      skillBalloon = nullptr;
    }
    if (hintBalloon) {
      g_windowMgr.Schedule(hintBalloon);
      hintBalloon = nullptr;
    }
  } else {
    bool created = false;
    if (!skillBalloon) {
      skillBalloon = new UITransBalloonText();
      skillBalloon->SetSize(8, 8);
      g_windowMgr.AddRendered(skillBalloon);
      created = true;
    }

    if (this->skillId != shownSkill || created == true) {
      shownSkill = this->skillId;
      std::string text;
      if (shownSkill == 10000) {
        text.assign(OBFUSCATE_TEXT("Capture Monster", 0x3FA3AFA7035DE3FFULL));
      } else if (shownSkill == 20000) {
        text = OBFUSCATE_TEXT("Please Marry Me", 0xDB27CB557D038759ULL);
      } else if (shownSkill == 20001) {
        text = OBFUSCATE_TEXT("Be my Baby", 0x51BB39B763BBBD8DULL);
      } else if (shownSkill == 40000) {
        text = OBFUSCATE_TEXT("Capture Monster", 0x87E3D1C50D572DEFULL);
      } else if (g_session.IsState291To303(shownSkill)) {
        text = OBFUSCATE_TEXT("????", 0xB7AFBD2FD9FD7501ULL);
      } else {
        text = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, shownSkill);
      }

      UITransBalloonText* balloon = static_cast<UITransBalloonText*>(skillBalloon);
      balloon->SetText(text.c_str(), 35);
      balloon->Move((g_renderer->viewport.width - 640) / 2 - balloon->geometry.width / 2 + 320, 46 * g_renderer->viewport.height / 480);
      balloon->m_textColor = 0x32FF32;
      balloon->m_outlineColor = 0;
    }

    if (!hintBalloon) {
      UITransBalloonText* balloon = new UITransBalloonText();
      hintBalloon = balloon;
      balloon->SetSize(8, 8);
      g_windowMgr.AddRendered(hintBalloon);
      static_cast<UITransBalloonText*>(hintBalloon)->SetText(LookupMessage(MSI_SELECTTARGET), 35);
      hintBalloon->Move((g_renderer->viewport.width - 640) / 2 - hintBalloon->geometry.width / 2 + 320, 400 * g_renderer->viewport.height / 480);
      {
        UITransBalloonText* hint = static_cast<UITransBalloonText*>(hintBalloon);
        hint->m_textColor = 0x32FF32;
        hint->m_outlineColor = 0;
      }
    }
  }

  if (this->itemWindow) {
    CPlayer* player = this->scene->player;
    const int x = player->render.screenX - 30;
    const int y = player->render.screenY;
    const int offset = static_cast<int>(static_cast<float>(12 * g_renderer->viewport.height / 480) * player->render.distanceScale);
    if (IsScreenShakeActive()) {
      this->itemWindow->Move(x, y - offset);
    } else {
      this->itemWindow->Move(x, offset + y);
    }
  }

  if (this->achievementBalloon && timeGetTime() > this->achievementBalloonTick + 10000) {
    g_windowMgr.Schedule(this->achievementBalloon);
    this->achievementBalloon = nullptr;
  }
}

// OFFSET: 0x00CDD7A0
// FUNCTION: fn_21319
void CGameMode::End() {
  CSchedule* schedule = g_timeScheduler->FindSchedule(g_session.m_pingLiveSchedule);
  if (schedule) {
    schedule->active = false;
  }
  g_timeScheduler->RemoveSchedulesByKey(4);
  CGameMode::StopReplayRecording();
  g_soundMgr->EndSamples();

  if (this->scene) {
    this->scene->OnExitFrame();
    delete this->scene;
  }

  if (this->camera) {
    this->camera->OnExitFrame();
    delete this->camera;
  }

  g_actorPickNode.Clear();
  ResourceMgr::Instance().ClearUnused("rsm");
  ResourceMgr::Instance().ClearUnused("rsw");
  ResourceMgr::Instance().ClearUnused("gat");
  ResourceMgr::Instance().ClearExpired();
  g_texMgr.ClearUnused();
  g_renderer->ClearFrame();
  g_renderer->atlas.Clear();
  g_windowMgr.SaveShortcutItems();

  char subKey[512];
  std::strcpy(subKey, g_subKey);
  std::strcat(subKey, "\\NameBaloon");
  std::strcat(subKey, "\\");
  std::strcat(subKey, g_session.m_serverName.c_str());
  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
    DWORD value = g_serviceOptions.Get(TT_NAME_ABOVE_HEAD_ON_OFF) != 0;
    RegSetValueExA(key, "POS", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));
    RegCloseKey(key);
  }

  if (this->canRotateView) {
    g_outdoorCameraPitch = g_cameraPitch;
    g_outdoorCameraDistance = g_cameraDistance;
  } else {
    g_indoorCameraPitch = g_cameraPitch;
    g_indoorCameraDistance = g_cameraDistance;
  }

  if (g_session.m_whisperRecord != 0) {
    CloseWhisperRecord();
  }
  g_effectMgr->Clear();
  g_session.SaveMiniPartyInfo();
  g_session.SetPlayerSkillMode(0);
}

// OFFSET: 0x00CE8DE0
// FUNCTION: fn_21342
void CGameMode::ReloadScene() {
  this->scene->Clear();
  this->scene->CreatePlayer();
  this->scene->LoadMapEffects();
  g_weatherEffect.ResetEffect();
  this->mousePointer = new CMousePointer();
  this->scene->objects.push_back(this->mousePointer);
  const float z = this->scene->player->render.position.z;
  const float y = this->scene->player->render.position.y;
  const float x = this->scene->player->render.position.x;
  this->camera->camera.target[0] = x;
  this->camera->camera.target[1] = y;
  this->camera->camera.target[2] = z;
}

// OFFSET: 0x00CDBEB0
// FUNCTION: fn_21314
void CGameMode::PlayItemUseEffect(int itemId, std::uint32_t aid) {
  ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, itemId, 0);

  CRenderObject* actor;
  if (aid == g_session.m_aid) {
    CPlayer* player = this->scene->player;
    actor = player;
    if (actor == nullptr) {
      return;
    }

    if (itemId == 0x3113) {
      const int job = player->GetEffectiveJob(-1);
      char text[0x100];
      strcpy_s(text, sizeof(text), LookupMessage(job >= 4054 && job <= 4112 ? MSI_PLUSEXP50 : MSI_PLUSEXP75));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 8, 0, 0);
    }
  } else {
    actor = this->scene->GetGameActorByAID(aid);
    if (actor == nullptr) {
      return;
    }
  }

  int secondEffectId = -1;
  int effectId = -1;
  switch (itemId) {
  case ITEMID_RED_POTION:
  case Red_Herb:
  case ITEMID_RED_SLIM_POTION:
  case Novice_Potion:
  case Strawberry:
  case Light_Red_Pot:
  case Novice_Potion_B:
  case Red_Slim_Potion_B:
  case Red_Potion_B:
  case Tyr:
    effectId = 0xCC;
    break;
  case Orange_Potion:
  case Delicious_Fish:
  case Light_Orange_Pot:
  case Orange_Potion_B:
  case Mora_Berry:
    effectId = 0xCD;
    break;
  case ITEMID_YELLOW_POTION:
  case Yellow_Herb:
  case ITEMID_YELLOW_SLIM_POTION:
  case Light_Yellow_Pot:
  case Yellow_Potion_B:
  case Yellow_Slim_Potion_B:
    effectId = 0xCE;
    break;
  case ITEMID_WHITE_POTION:
  case White_Herb:
  case ITEMID_WHITE_SLIM_POTION:
  case Rice_Cake:
  case Long_Rice_Cake:
  case Hash_Rice_Cake:
  case Light_White_Pot:
  case Siege_White_Potion:
  case Woe_White_Potion:
  case TE_White_Potion:
  case TE_White_Slim_Potion:
  case White_Potion_B:
  case White_Slim_Potion_B:
    effectId = 0xCF;
    break;
  case ITEMID_BLUE_POTION:
  case Blue_Herb:
  case Light_Blue_Pot:
  case Siege_Blue_Potion:
  case Woe_Blue_Potion:
  case Blue_Potion_B:
    effectId = 0xD0;
    break;
  case Green_Potion:
  case Green_Herb:
  case Green_Potion_B:
    effectId = 0xD1;
    break;
  case ITEMID_APPLE:
  case Banana:
  case Carrot:
  case Sweet_Potato:
  case Apple_Juice:
  case Banana_Juice:
  case Carrot_Juice:
  case ITEMID_PUMPKIN:
  case Cheese:
  case Nice_Sweet_Potato:
  case Popped_Rice:
  case Orange:
  case Realgar_Wine:
    effectId = 0xD2;
    break;
  case Grape:
  case Grape_Juice:
  case Lemon:
  case Yggdrasilberry:
  case Magical_Carnation:
  case Exorcize_Herb:
  case ITEMID_ANCILLA:
  case Manuks_Courage:
  case Luciola:
  case Tiny_Waterbottle:
    effectId = 0xD3;
    break;
  case Meat:
  case Honey:
  case Milk:
  case Leaflet_Of_Hinal:
  case Leaflet_Of_Aloe:
  case Royal_Jelly:
  case Monster:
  case Candy:
  case Candy_Striper:
  case Ice_Cream:
  case ITEMID_PET_FOOD:
  case Well_Baked_Cookie:
  case Piece_Of_Cake:
  case Falcon:
  case Pecopeco:
  case Festive_Cookie:
  case Festive_Rainbow_Cake:
  case Fish_Slice:
  case Shusi:
  case KETUPAT:
  case Bun:
  case Pizza:
  case Pizza_01:
  case Rice_Ball:
  case Vita500_Bottle:
  case Tomyumkung:
  case Prawn:
  case Lucky_Candy:
  case Lucky_Candy_Cane:
  case Lucky_Cookie:
  case Piece_Of_Cake_:
  case Prickly_Fruit:
  case Grain:
  case Bread:
  case Mushroom:
  case KETUPAT_:
  case Fish_Ball_Soup:
  case Wurst:
  case Mother:
  case Korea_Rice_Cake:
  case Woe_Violet_Potion:
  case Lucky_Cookie_B:
  case Lucky_Candy_Cane_B:
  case Rice_Ball_B:
  case Candy_Striper_B:
  case Lucky_Candy_B:
  case Candy_B:
  case Piece_Of_Cake_B:
  case Well_Baked_Cookie_B:
    effectId = 7;
    break;
  case Chocolate:
  case White_Chocolate:
  case White_Chocolate_B:
  case Chocolate_B:
  case Choco_Lump:
    effectId = 0x16C;
    break;
  case HandMade_Chocolate:
  case HandMade_Chocolate_:
  case HandMade_Chocolate_B:
  case HandMade_Chocolate_B_:
  case Girl_Bunch_Of_Flower:
    effectId = 0x16B;
    break;
  case Center_Potion:
  case Light_Center_Pot:
  case Center_Potion_B:
    effectId = 0xDA;
    break;
  case Awakening_Potion:
  case Light_Awakening_Pot:
  case Awakening_Potion_B:
    effectId = 0xDB;
    break;
  case Berserk_Potion:
  case Light_Berserk_Pot:
  case Berserk_Potion_B:
    effectId = 0xDC;
    break;
  case Mojji:
  case Sesame_Pastry:
  case Honey_Pastry:
  case Rainbow_Cake:
  case Plain_Rice_Cake:
  case Hearty_Rice_Cake:
  case Salty_Rice_Cake:
  case Lucky_Rice_Cake:
  case New_Year_Rice_Cake_1:
  case New_Year_Rice_Cake_2:
  case Sesame_Pastry_B:
  case Honey_Pastry_B:
  case Rainbow_Cake_B:
    g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x1EB;
    break;
  case Inspector_Certificate:
  case Inspector_Certificate_:
    g_soundMgr->PlayWave("effect\\mapae.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x1FB;
    break;
  case Durian:
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_GUMGANG2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effectId = 0xDC;
    break;
  case RAMADAN:
    effectId = 0x1EC;
    break;
  case Fruit_Of_Mastela:
    g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case ITEMID_HOLY_WATER:
    g_soundMgr->PlayWave("effect\\EF_Blessing.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case Speed_Up_Potion:
  case Speed_Up_Potion_B:
    g_soundMgr->PlayWave("effect\\fast.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x207;
    break;
  case Slow_Down_Potion:
  case Slow_Down_Potion_B:
    effectId = 0x26;
    break;
  case Fire_Cracker:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x1FC;
    break;
  case Chocolate_Drink:
  case Cute_Strawberry_Choco:
  case Lovely_Choco_Tart:
  case Chocolate_Drink_B:
  case Cacao99:
    effectId = 0x1FD;
    break;
  case Giftbox_China:
    g_soundMgr->PlayWave("effect\\ro2effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x221;
    break;
  case Giggling_Box:
  case HALLOWEEN_G_BOX:
    g_soundMgr->PlayWave("wraith_die.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case Box_Of_Thunder:
    effectId = 0x240;
    break;
  case Gloomy_Box:
    effectId = 0x241;
    break;
  case Box_Of_Grudge:
    effectId = 0x242;
    break;
  case Sleepy_Box:
    effectId = 0x243;
    break;
  case Box_Of_Storm:
    effectId = 0x244;
    break;
  case Painting_Box:
    g_soundMgr->PlayWave("ork_warrior_breath.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case Box_Of_Sunlight:
    effectId = 0x245;
    break;
  case Resist_Fire:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x25B;
    break;
  case Resist_Water:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x33;
    break;
  case Resist_Earth:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x93;
    break;
  case Resist_Wind:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x34;
    break;
  case Lucky_Potion:
    effectId = 0x264;
    break;
  case Str_Dish10_M:
    effectId = 0x251;
    break;
  case Agi_Dish10_M:
    effectId = 0x254;
    break;
  case Int_Dish10_M:
    effectId = 0x252;
    break;
  case Dex_Dish10_M:
    effectId = 0x255;
    break;
  case Luk_Dish10_M:
    effectId = 0x256;
    break;
  case Vit_Dish10_M:
    effectId = 0x253;
    break;
  case Assumptio_5_Scroll:
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_ASSUMPTIO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_ASSUMPTIO2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case Spray_Of_Flowers:
  case Large_Spray_Of_Flowers:
    effectId = 0x2BB;
    break;
  case Goddess_Bless:
    effectId = 0x152;
    break;
  case Angel_Bless:
    effectId = 0x2C1;
    break;
  case Powder_Snow:
    g_soundMgr->PlayWave("effect\\storm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x2C2;
    break;
  case Little_Heart:
    effectId = 0x2C3;
    break;
  case J_Firecracker:
    g_soundMgr->PlayWave("effect\\pokjuk_jap.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x2C5;
    break;
  case Egg:
  case Manuk:
  case Pinguicula:
    effectId = 0xD2;
    break;
  case J_Aspersio_5_Scroll:
    effectId = 0x56;
    break;
  case RWC_Cele_Fire:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x3DB;
    break;
  case RWC_Cele_Fire2:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x3DC;
    break;
  case No100_Firecracker:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x3EE;
    break;
  case Fire_Cracker_Love:
    effectId = 0x2AA;
    break;
  case Fire_Cracker_Wday:
    effectId = 0x2AB;
    break;
  case Fire_Cracker_Vday:
    effectId = 0x2AC;
    break;
  case Fire_Cracker_Bday:
    effectId = 0x2AD;
    break;
  case Fire_Cracker_Xmas:
    effectId = 0x2AE;
    break;
  case Bath_Foam_A:
  case Bath_Foam_B:
  case Bath_Foam_C:
    effectId = 0x63D;
    secondEffectId = 0x63E;
    break;
  case Aroma_Oil:
    effectId = 0x63B;
    secondEffectId = 0x63C;
    break;
  case 0x19605:
    effectId = 0x982;
    break;
  }

  if ((itemId >= 0x2F09 && itemId <= 0x2F0D) || (itemId >= 0x2F27 && itemId <= 0x2F2B) || itemId == 0x2FAA) {
    effectId = 0x251;
  } else if ((itemId >= 0x2F0E && itemId <= 0x2F12) || (itemId >= 0x2F2C && itemId <= 0x2F30) || itemId == 0x2FAC) {
    effectId = 0x252;
  } else if ((itemId >= 0x2F13 && itemId <= 0x2F17) || (itemId >= 0x2F31 && itemId <= 0x2F35) || itemId == 0x2FAF) {
    effectId = 0x253;
  } else if ((itemId >= 0x2F18 && itemId <= 0x2F1C) || (itemId >= 0x2F36 && itemId <= 0x2F3A) || itemId == 0x2FAB) {
    effectId = 0x254;
  } else if ((itemId >= 0x2F1D && itemId <= 0x2F21) || (itemId >= 0x2F3B && itemId <= 0x2F3F) || itemId == 0x2FAD) {
    effectId = 0x255;
  } else if ((itemId >= 0x2F22 && itemId <= 0x2F26) || (itemId >= 0x2F40 && itemId <= 0x2F44) || itemId == 0x2FAE) {
    effectId = 0x256;
  }

  if (effectId > 0) {
    while (static_cast<EffectActor*>(actor)->HasId(effectId)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, effectId, 0, 0, 0, 0, 0);
    }
    if (g_effectMgr->HasAllocator(effectId)) {
      g_effectMgr->Spawn(actor->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      static_cast<EffectActor*>(actor)->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (secondEffectId > 0) {
    while (static_cast<EffectActor*>(actor)->HasId(secondEffectId)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, secondEffectId, 0, 0, 0, 0, 0);
    }
    if (g_effectMgr->HasAllocator(secondEffectId)) {
      g_effectMgr->Spawn(actor->render.accountId, secondEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      static_cast<EffectActor*>(actor)->LaunchEffect(secondEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
}

#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/SignBoard/SignBoard.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/Movement/TerrainPathSearch.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Network/Packets/GameTickPacket.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Macro/UIMacroUserInfoWnd.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/CashShop/UIMileageWnd.h"
#include "UI/Window/UIWindowFactory.h"
#include <list>
#include <windows.h>

// GLOBAL: gl_24776
int g_visibleMinX;

// GLOBAL: gl_24777
int g_visibleMinY;

// GLOBAL: gl_24778
int g_visibleMaxX;

// GLOBAL: gl_24779
int g_visibleMaxY;

// GLOBAL: gl_25330
std::uint32_t g_gameTickInterval = 12000;

// GLOBAL: gl_25332
int g_window315Enabled = 1;

// OFFSET: 0x00CD5E90
// FUNCTION: fn_21288
void CGameMode::InitializeState() {
  this->ClearTooltip();
  this->nameCard.Clear();
  this->actorPosList.clear();
  this->pickupReqItemNaidList.clear();
  this->actorMenuIds.clear();
  this->partyPosList.clear();
  this->compassPosList.clear();
  this->m_skillUnitIds.clear();
  this->spriteMirrorActorIds.clear();
  this->minimapMarkers.clear();

  if (g_windowMgr.miniMapWnd) {
    g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_CLEAR_QUEST_MARKS, 0, 0, 0, 0);
  }

  g_snapMgr.Clear();
  g_windowMgr.ClearMapWindows();
  this->broadcastBalloon = nullptr;
  this->noticeBalloon = nullptr;
  g_windowMgr.SetBackgroundSurface(nullptr);
  g_windowMgr.EnsureWindow(WID_BASICINFOWND);
  g_windowMgr.ShowWindowsWithId(WID_WHISPERWND, 1);
  g_windowMgr.EnsureWindow(WID_QUESTDISPLAY);

  const char* worldName = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(MM_QUERYRSWNAME, 0, 0, 0, 0));
  if (std::strcmp(worldName, "new_event.rsw") != 0) {
    g_windowMgr.EnsureWindow(WID_NCASH_CALLWND);
  }

  if (g_session.memorialWindowOpen) {
    g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND);
  }

  if (!g_replayAnimationScaling && g_serviceOptions.Get(TT_QUICK_SLOT_ON_OFF)) {
    g_windowMgr.EnsureWindow(WID_QUIKSLOTWND);
  }

  if (g_session.m_partyBooking.seekWindowOpen) {
    g_windowMgr.EnsureWindow(WID_SEEKPARTYWND);
  }

  if (g_session.m_searchStoreInfoCreated) {
    g_windowMgr.EnsureWindow(WID_SEARCH_STORE_INFO);
  }

  g_windowMgr.EnsureWindow(WID_OPENROULLETTEWND);
  if (g_session.showCameraInfo) {
    g_windowMgr.EnsureWindow(WID_VIEWCAMERAINFOWND);
  }

  this->running = 1;
  g_visibleMinX = 0;
  g_visibleMaxX = 0;
  g_visibleMinY = 0;
  g_visibleMaxY = 0;
  ActorPickBounds bounds;
  bounds.left = 0;
  bounds.top = 0;
  bounds.right = g_renderer->viewport.width;
  bounds.bottom = g_renderer->viewport.height;
  g_actorPickNode.MakeQuardTree(1, bounds);
  g_pathFinder.attr = this->scene->terrain;
  g_navigationManager.UpdateGround();
  g_overrideColor = 0;

  this->fadeInRequested = 1;
  this->cursorOffsetX = 0.0f;
  this->cursorOffsetY = 0.0f;
  this->leftBtnClickTick = 0;
  this->oldMouseX = g_mouse.x;
  this->oldMouseY = g_mouse.y;
  this->rBtnClickX = g_mouse.x;
  this->rBtnClickY = g_mouse.y;
  this->noMove = 0;
  this->isOnQuest = 0;
  this->isPlayerDead = 0;
  this->nameBalloon = nullptr;
  this->itemNameBalloon = nullptr;
  this->skillBalloon = nullptr;
  this->skillHintBalloon = nullptr;
  this->unknown_02C0 = 0;
  this->itemWindow = nullptr;
  this->statusIconTooltip = nullptr;
  this->hoveredStatusIconId = -1;
  this->balloonActorId = 0;
  this->nameBalloonWidth = 0;
  this->nameBalloonHeight = 0;
  this->isNameBalloonPlaced = 1;
  this->isItemNameBalloonPlaced = 1;
  this->waitingUseItemAck = 0;
  this->waitingItemThrowAck = 0;
  this->waitingReqStatusAck = 0;
  this->drag.Clear();
  this->lastNaid = 0;
  this->menuTargetAid = 0;
  this->lastPcGid = 0;
  this->lastMonGid = 0;
  this->lastLockOnMonGid = 0;
  this->isAutoMoveClickOn = 1;
  this->isWaitingEnterRoom = 0;
  this->isWaitingAddExchangeItem = 0;
  this->waitingWearEquipAck = timeGetTime() - 2000;
  this->waitingTakeoffEquipAck = timeGetTime() - 2000;
  this->isReqUpgradeSkillLevel = 0;
  this->exchangeItemCnt = 0;
  this->isWaitingCancelExchangeItem = 0;
  this->sameChatRepeatCount = 0;
  this->serverTimeSyncCount = 0;
  this->lastChat = "";
  this->skillUseType = nullptr;
  this->skillId = 0;
  g_pickInvert = 0;
  this->cardItemIndex = 0;
  this->miniMapTooltipId = -1;
  this->shownSkillId = -1;

  char subKey[512];
  std::strcpy(subKey, g_subKey);
  std::strcat(subKey, "\\NameBaloon");
  std::strcat(subKey, "\\");
  std::strcat(subKey, g_session.m_serverName.c_str());
  HKEY key;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
    RegCloseKey(key);
    HKEY valueKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_ALL_ACCESS, &valueKey) == ERROR_SUCCESS) {
      DWORD position = 0;
      DWORD type = REG_DWORD;
      DWORD size = sizeof(position);
      RegQueryValueExA(valueKey, "POS", nullptr, &type, reinterpret_cast<BYTE*>(&position), &size);
      g_serviceOptions.Set(92, position == 1);
      RegCloseKey(valueKey);
    }
  }

  g_mouse.Reset();
  this->syncAcknowledged = 1;
  this->isEmotionRequested = 0;
  this->syncRequestTick = timeGetTime();
  this->emotionRequestTick = timeGetTime();
  this->noMoveStartTick = timeGetTime();
  this->windowInvalidateTick = timeGetTime();
  this->broadcastBalloon = nullptr;
  this->chatRoomRequestTick = timeGetTime();
  this->merchantShopRequestTick = timeGetTime();
  this->inventoryChanged = 1;
  std::memset(&this->replayDragState[1], 0, 6);
  this->mouseRollCount = 0;
  this->mouseRollMax = 0;
  g_soundMgr->SetListenerYaw(this->camera->camera.desiredYaw);
  this->SendMsg(MM_TOGGLE_PLAYERGAGE, 0, 0, 0, 0);

  if (g_isStopByLogin) {
    g_isStopByLogin = 0;
    g_guildInfo.Init();
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LIST_COMMAND)), 0xFFE6E6, 0, 0, 0);
    inet_ntoa(*reinterpret_cast<const in_addr*>(&g_session.m_lastLoginIp));
    if (!g_accountLinkedUserDataMgr->m_loaded) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_OFFER_GUILD_EXP_MAX_RATE)), 255, 0, 0, 0);
    }
  }

  g_session.ResetStatBonuses();
  g_session.UpdateRandomStats();
  g_session.ClearCartItems();
  g_session.ClearTradeItems();
  g_session.ClearMerchantPurchaseSavedItems();
  g_session.ClearMerchantPurchaseItems();
  g_session.ClearMerchantMyShopItems();

  // GLOBAL: gl_25334
  static int s_window315Checked;
  if (!g_replayAnimationScaling && !s_window315Checked) {
    s_window315Checked = 1;
    if (g_window315Enabled) {
      g_windowMgr.EnsureWindow(WID_TIPBOXWND);
    }
  }

  if (g_replayAnimationScaling != 1) {
    GameModePacketHeader packet;
    packet.packetId = ID_CZ_NOTIFY_ACTORINIT;
    GetRagConnection().Send(2, &packet);
    GetRagConnection().ResetDeferred();
    g_session.SetOption(5, 1, 0);
  }

  SendGameTick(
    true, this->syncAcknowledged, this->syncRequestTick, g_gameTickInterval,
    GetRagConnection(), g_packetLengths, g_pingLogEnabled, g_pingLogFirstTick, g_pingLogTicks
  );
  g_weatherEffect.ResetEffect();
  g_weatherEffect.ApplyMapWeather(this->mapName);
  if (std::strcmp(this->mapName, "xmas_fild01.rsw") == 0 || std::strcmp(this->mapName, "xmas.rsw") == 0) {
    if (g_serviceOptions.Get(TT_EFFECT_ON_OFF)) {
      g_weatherEffect.Snow();
    } else {
      g_weatherEffect.ExtendEffect();
    }
  }

  if (!g_mapSkyMgr) {
    g_mapSkyMgr = new (std::nothrow) CMapSkyMgr;
  }

  g_mapSkyMgr->Apply(this->mapName);
  if ((g_serviceType == ServiceMalaysia || g_serviceType == ServiceSingapore) && std::strcmp(this->mapName, "comodo.rsw") == 0 && g_serviceOptions.Get(TT_EFFECT_ON_OFF)) {
    g_weatherEffect.Fireworks();
  }

  g_session.m_homunAid = -1;
  g_session.homunAlive = 0;
  g_session.ClearHomunStatus();
  g_session.m_mercenaryAid = -1;
  g_session.mercenaryAlive = 0;
  g_session.ResetMercenaryStats();
  g_windowMgr.FocusWindow01C8Edit();

  g_windowMgr.replayControlWnd = nullptr;
  const int replayState = Replay::Instance().state;
  if (g_replayAnimationScaling) {
    g_windowMgr.EnsureWindow(WID_REPLAYCONTROLWND);
    if (replayState == 8) {
      g_windowMgr.replayControlWnd->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
  }

  if (replayState == 3) {
    Replay::Instance().SetState(2);
    g_windowMgr.EnsureWindow(WID_REPLAYRECCONTROLWND);
    g_windowMgr.replayRecordControlWnd->Move(Replay::Instance().recordingState, Replay::Instance().playbackState);
    g_windowMgr.replayRecordControlWnd->SendMsg(nullptr, UM_REPLAY_FRAMED_DATA, 0, 0, 0, 0);
  } else if (replayState == 8) {
    Replay::Instance().SetState(7);
  }

  g_navigationManager.ResumeRoute();
  if (g_session.m_mileageWndActive) {
    UIWindow* mileage = g_windowMgr.EnsureWindow(WID_MILEAGEWND);
    if (mileage) {
      mileage->SendMsg(nullptr, UM_SETINFO, g_session.m_mileageWndActive, g_session.m_mileageWndPoint, g_session.m_mileageWndElapsed, g_session.m_mileageWndBonus);
    }
  }

  this->achievementBalloon = nullptr;
  this->achievementBalloonTick = 0;
  s_signBoardMgr->Initialize();
  g_statusRefreshSuppressed = 0;
  int& localMapMove = g_session.m_localMapMove;
  if (localMapMove == 1) {
    localMapMove = 0;
  }

  g_guildInfo.ClearAgitIds();
  this->menuTargetName = "";
}

// OFFSET: 0x00CE46B0
// FUNCTION: fn_21337
void CGameMode::ProcessRightButtonDrag() {
  switch (g_mouse.buttons[1]) {
  case 1: {
    if (g_windowMgr.menuWnd) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }

    this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    this->SendMsg(MM_CANCEL_USE_SKILL, 0, 0, 0, 0);
    this->rBtnClickX = g_mouse.x;
    this->rBtnClickY = g_mouse.y;
    if (!g_serviceOptions.Get(TT_QUICKSPELL_ON_OFF)) {
      return;
    }

    UIShortCutWnd* shortcuts = static_cast<UIShortCutWnd*>(g_windowMgr.shortCutWnd);
    if (!shortcuts) {
      return;
    }

    AutomaticSkills* automatic = &g_session.m_automaticSkills;
    if (shortcuts->m_slots[4]->enabled && automatic->Contains(static_cast<std::uint16_t>(shortcuts->m_slots[4]->itemId))) {
      g_session.SetOption(automatic->CancelOption(shortcuts->m_slots[4]->itemId), 1, 0);
    } else {
      automatic->DisableAll();
    }

    shortcuts->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 8, 0, 0, 0);
    return;
  }
  case 2:
    if (!HIBYTE(GetAsyncKeyState(VK_MENU)) && !HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      this->SetCursorAction(4);
      if (this->hasViewParameters) {
        float yaw = static_cast<float>(g_mouse.x) + this->camera->camera.desiredYaw - static_cast<float>(this->oldMouseX);
        const float low = static_cast<float>(this->viewParameters[3]);
        const float high = static_cast<float>(this->viewParameters[4]);
        if (yaw < low) {
          yaw = low;
        }
        if (yaw > high) {
          yaw = high;
        }
        this->camera->camera.desiredYaw = yaw;
      } else if (this->canRotateView == 0 && g_session.IsAdminSubMode(6) == 0) {
        float yaw = static_cast<float>(g_mouse.x) + this->camera->camera.desiredYaw - static_cast<float>(this->oldMouseX);
        const float low = this->fixedLongitude - 20.0f;
        const float high = this->fixedLongitude + 20.0f;
        if (yaw < low) {
          yaw = low;
        }
        if (yaw > high) {
          yaw = high;
        }
        this->camera->camera.desiredYaw = yaw;
      } else {
        this->camera->camera.AddYaw(static_cast<float>(g_mouse.x - this->oldMouseX));
      }

      g_soundMgr->SetListenerYaw(this->camera->camera.desiredYaw);
    }

    if (!HIBYTE(GetAsyncKeyState(VK_MENU)) && HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      const float range = this->canRotateView ? 42.0f : 10.0f;
      g_cameraPitch = this->camera->camera.desiredPitch - static_cast<float>((g_mouse.y - this->oldMouseY) / 2);
      float limit;
      if (this->hasViewParameters) {
        if (g_cameraPitch < static_cast<float>(this->viewParameters[7])) {
          g_cameraPitch = static_cast<float>(this->viewParameters[7]);
        }
        limit = static_cast<float>(this->viewParameters[6]);
      } else {
        if (g_cameraPitch < -45.0f - range) {
          g_cameraPitch = -45.0f - range;
        }
        if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) && this->camera->camera.desiredDistance <= g_serviceOptionJitterScale) {
          limit = range - 45.0f;
        } else {
          limit = range - 70.0f;
        }
      }

      if (g_cameraPitch > limit) {
        g_cameraPitch = limit;
      }
      this->camera->camera.desiredPitch = g_cameraPitch;
    }

    if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      g_cameraDistance = this->camera->camera.desiredDistance - static_cast<float>((g_mouse.y - this->oldMouseY) / 2);
      float maxDistance = this->canRotateView ? g_serviceOptionJitterScale : g_cameraBaseDistance;
      if (this->hasViewParameters) {
        if (g_cameraDistance < static_cast<float>(this->viewParameters[0])) {
          g_cameraDistance = static_cast<float>(this->viewParameters[0]);
        }
        maxDistance = static_cast<float>(this->viewParameters[1] + this->viewParameters[0]);
        if (g_cameraDistance > maxDistance) {
          g_cameraDistance = maxDistance;
        }
      } else if (g_session.IsAdminSubMode(5) == 0) {
        if (g_cameraDistance < g_cameraMinDistance) {
          g_cameraDistance = g_cameraMinDistance;
        }
        if (g_cameraDistance > maxDistance) {
          g_cameraDistance = maxDistance;
        }
      }

      this->camera->camera.desiredDistance = g_cameraDistance;
    }
    return;
  case 4:
    if (this->hasViewParameters) {
      return;
    }

    if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      this->camera->camera.desiredDistance = g_cameraBaseDistance;
    }
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      this->camera->camera.desiredPitch = -50.0f;
    }
    if (!g_serviceOptions.Get(TT_QUICKSPELL_ON_OFF) && this->canRotateView && !HIBYTE(GetAsyncKeyState(VK_SHIFT)) && !HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      this->camera->camera.desiredYaw = 0.0f;
    }
    return;
  }
}

// OFFSET: 0x00CE7CE0
// FUNCTION: fn_21339
void CGameMode::ProcessMouseWheel(int requireControl) {
  if (timeGetTime() - this->wheelTick < 16) {
    return;
  }

  this->wheelTick = timeGetTime();
  if (g_mouse.wheel == 0 || this->skillUseType != nullptr) {
    return;
  }

  CSkillInfo wheelSkill = g_session.m_skillBundle.Lookup(0x27F);
  if (wheelSkill.level > 0) {
    const int count = this->mouseRollCount;
    if (count > this->mouseRollMax) {
      this->mouseRollMax = count;
    }
    this->mouseRollCount = count + 4;
  }

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    const float step = g_mouse.wheel > 0 ? -5.0f : 5.0f;
    const float range = this->canRotateView ? 20.0f : 10.0f;
    g_cameraPitch = this->camera->camera.desiredPitch - step;
    float limit;
    if (this->hasViewParameters) {
      if (g_cameraPitch < static_cast<float>(this->viewParameters[7])) {
        g_cameraPitch = static_cast<float>(this->viewParameters[7]);
      }
      limit = static_cast<float>(this->viewParameters[6]);
    } else {
      if (g_cameraPitch < -45.0f - range) {
        g_cameraPitch = -45.0f - range;
      }
      if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) && this->camera->camera.desiredDistance <= g_serviceOptionJitterScale) {
        limit = range - 45.0f;
      } else {
        limit = range - 70.0f;
      }
    }

    if (g_cameraPitch > limit) {
      g_cameraPitch = limit;
    }
    this->camera->camera.desiredPitch = g_cameraPitch;
    return;
  }

  if (requireControl != 0 && !HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
    return;
  }

  if (g_serviceOptions.Get(TT_QUICKSPELL2_ON_OFF)) {
    UIShortCutWnd* shortcuts = static_cast<UIShortCutWnd*>(g_windowMgr.shortCutWnd);
    g_session.m_mapTransitionFlag = 1;
    AutomaticSkills* automatic = &g_session.m_automaticSkills;
    if (g_mouse.wheel > 0) {
      if (!shortcuts) {
        return;
      }

      if (shortcuts->m_slots[3]->enabled && automatic->Contains(shortcuts->m_slots[3]->itemId)) {
        g_session.SetOption(automatic->CancelOption(shortcuts->m_slots[3]->itemId), 1, 0);
      } else {
        automatic->DisableAll();
      }

      shortcuts->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 0, 0, 0);
    } else {
      if (!shortcuts) {
        return;
      }

      if (shortcuts->m_slots[2]->enabled && automatic->Contains(shortcuts->m_slots[2]->itemId)) {
        g_session.SetOption(automatic->CancelOption(shortcuts->m_slots[2]->itemId), 1, 0);
      } else {
        automatic->DisableAll();
      }

      shortcuts->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 0, 0, 0);
    }
    return;
  }

  const float step = g_mouse.wheel > 0 ? -24.3f : 24.3f;
  g_cameraDistance = this->camera->camera.desiredDistance - step;
  float maxDistance = this->canRotateView ? g_serviceOptionJitterScale : g_cameraBaseDistance;
  CSkillInfo zoomSkill = g_session.m_skillBundle.Lookup(0x1B9);
  bool extendedZoom = false;
  if (this->hasViewParameters) {
    if (g_cameraDistance < static_cast<float>(this->viewParameters[0])) {
      g_cameraDistance = static_cast<float>(this->viewParameters[0]);
    }
    maxDistance = static_cast<float>(this->viewParameters[1] + this->viewParameters[0]);
  } else if (zoomSkill.level <= 0 && this->scene->player->render.state == 6 && !this->scene->player->HasId(EF_BLIND)) {
    switch (g_session.m_job) {
    case JT_STAR:
    case JT_STAR_B:
    case JT_STAR_EMPEROR:
    case JT_STAR_EMPEROR_B:
    case JT_SKY_EMPEROR:
      extendedZoom = true;
      break;
    }
  }

  if (extendedZoom) {
    if (g_cameraDistance < g_cameraMinDistance) {
      g_cameraDistance = g_cameraMinDistance;
    }
    if (g_cameraDistance > maxDistance * 3.0f) {
      g_cameraDistance = maxDistance * 3.0f;
    }
    if (g_cameraDistance > maxDistance && this->camera->camera.desiredPitch > -50.0f) {
      this->camera->camera.desiredPitch = -50.0f;
    }
  } else {
    if (!this->hasViewParameters && g_cameraDistance < g_cameraMinDistance) {
      g_cameraDistance = g_cameraMinDistance;
    }
    if (g_cameraDistance > maxDistance) {
      g_cameraDistance = maxDistance;
    }
  }

  this->camera->camera.desiredDistance = g_cameraDistance;
}

#pragma pack(push, 1)
struct ContactNpcPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint32_t npcId;
  /* 0006 */ unsigned char type;
};
#pragma pack(pop)

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  virtual ~CEquipmentPropertiesMgr();
  /* 0004 */ unsigned char padding_0004[0x1F8];

  CEquipmentPropertiesMgr();
  void RefreshPretend();
  void RefreshShop();
};
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentProperties::CEquipmentPropertiesMgr) == 0x1FC);
#endif

// OFFSET: 0x00CDDA50
// FUNCTION: fn_21320
void CGameMode::Initialize(const char* name) {
  this->isInitializing[0] = 1;
  this->recordChatCount = 0;
  this->nameCard.SetName(g_session.m_aid, g_session.DecodePlayerName());
  g_playerNameDictionary->Set(g_session.m_gid, g_session.DecodePlayerName());
  this->bossMonsterX = -1;
  this->bossMonsterY = -1;
  this->bossAlarmAndCopyNameFlags[0] = 0;
  this->bossAlarmAndCopyNameFlags[1] = 0;

  if (Replay::Instance().state == 6) {
    const std::vector<unsigned char> data = Replay::Instance().ReadSection(0x12);
    if (data.begin() != data.end()) {
      GameModeChunkHeader header = {};
      g_activeSkillEffects.clear();
      unsigned offset = 6;
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
      if (header.type == 19000) {
        while (offset < data.size()) {
          header = {};
          header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
          offset += 6;
          if (header.type == 19001) {
            break;
          }

          if (header.type == 19011) {
            ActiveSkillEffect effect;
            std::memcpy(&effect, data.data() + offset, header.size);
            offset += header.size;
            if (effect.expiration != 999999) {
              g_cooldownOwner->Register(effect.id, effect.totalDuration, effect.expiration - effect.remainingDuration, effect.totalDuration - effect.remainingDuration);
            }
            g_activeSkillEffects.push_back(effect);
          } else {
            offset += header.size;
          }
        }
        g_statusRefreshSuppressed = 1;
      }
    }
  }

  g_session.ClearPairValues(1, 0, 0);
  this->scene = new CWorld();
  this->scene->gameMode = this;
  this->camera = new CView();
  this->camera->scene = this->scene;
  std::strcpy(this->mapName, name);

  std::string miniMap = "유저인터페이스\\map\\";
  miniMap.append(name, std::strlen(name));
  miniMap.resize(miniMap.size() - 4);
  miniMap.append(".bmp", 4);
  std::strcpy(reinterpret_cast<char*>(this->minimapBmpName), miniMap.c_str());

  g_renderer->ClearDepth(1);
  g_session.m_weight = 0;
  g_session.RefreshHpSpWindows();
  g_session.InvalidateWindowFrames();
  g_session.m_maxWeight = 0;
  g_session.RefreshHpSpWindows();
  g_session.InvalidateWindowFrames();
  g_windowMgr.SetBackgroundSurface(nullptr);
  g_windowMgr.ClearMapWindows();
  this->broadcastBalloon = nullptr;
  this->noticeBalloon = nullptr;
  g_windowMgr.HideChatWnd();
  g_windowMgr.ShowWindowsWithId(WID_WHISPERWND, 0);
  g_windowMgr.EnsureWindow(WID_LOADINGWND);
  g_windowMgr.SendNotice(UIM_LOADINGPERCENT, 0, 0, 0, 0, 0);
  g_windowMgr.DrawCenteredBackground();
  g_windowMgr.DrawWindows();
  if (g_renderer->Draw()) {
    g_renderer->Present(1);
  }

  this->camera->Reset();
  this->scene->InitWorld();
  this->checkGroundAlpha = 0;
  this->InitializeCamera();
  this->mousePointer = new CMousePointer();
  this->scene->objects.push_back(this->mousePointer);
  g_soundMgr->SetDefaultName(g_session.GetBgmFileName(std::string(this->mapName).c_str()));

  g_weatherEffect.enabled = g_serviceOptions.Get(TT_LIGHTMAP_ON_OFF) == 0;
  if (g_serviceOptions.Get(TT_FOG_ON_OFF)) {
    int found = 0;
    const std::vector<std::string>* fog = g_session.GetFogParameters(this->mapName, &found);
    if (found) {
      g_depthStart = static_cast<float>(std::atof((*fog)[0].c_str()));
      g_depthEnd = static_cast<float>(std::atof((*fog)[1].c_str()));
      g_worldOverlayColor = ParseHexString((*fog)[2].c_str());
      g_fogDensity = static_cast<float>(std::atof((*fog)[3].c_str()));
      g_renderer->SetFog(1);
    } else {
      g_renderer->SetFog(0);
    }
  } else {
    g_renderer->SetFog(0);
  }

  Replay::Instance().ResetPlayback();
  g_session.RestoreSelectionItems();
  g_session.ClearItemQuantities();
  this->InitializeState();
  ShowCurrentMapName();
  ResetTimer();
  this->forcedAttack = 0;
  this->countdownStartTick = 0;
  this->chatLogLines = 0;
  this->strikeCount = 0;
  this->periodicTicks.resize(6);

  if (g_session.m_whisperRecord && g_session.IsAdminSubMode(7)) {
    char message[256];
    std::sprintf(message, "Now Auto chat Save System is ENABLED.");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 0, 0, 0);
  }

  if (g_replayAnimationScaling == 0) {
    std::uint64_t now = _Xtime_get_ticks();
    std::int64_t interval = 60000;
    g_timeScheduler->AddSchedule(new CCheckGDISchedule(now, interval, 2));
    interval = 5000;
    CPingLiveSchedule* ping = new CPingLiveSchedule(now, interval);
    g_session.m_pingLiveSchedule = ping->serial;
    g_timeScheduler->AddSchedule(ping);
  }

  if (g_session.AreInventoryCategoriesEmpty()) {
    if (g_launchPassCommandLine) {
      this->SendMsg(MM_STOVE_AUTHENTICATION, 1, 0, 0, 0);
      g_stoveMgr->FetchAllProducts();
    } else {
      std::int16_t packet = 0x8C9;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet), &packet);
    }
  }

  if (Replay::Instance().state == 6) {
    Replay::Instance().speed = 1.0f;
    g_session.ReadReplayQuests(Replay::Instance().ReadSection(6));
    g_session.ReadReplayStats(Replay::Instance().ReadSection(4));
    g_session.ReadReplayItems(Replay::Instance().ReadSection(8));
    g_session.ReadReplayCompanions(Replay::Instance().ReadSection(9));
    g_session.ReadReplayMessenger(Replay::Instance().ReadSection(7));
    g_session.ReadReplayExtra(Replay::Instance().ReadSection(10));
    g_session.ReadReplayReserved(Replay::Instance().ReadSection(11));
    g_session.ReadReplayEntryQueue(Replay::Instance().ReadSection(12));
    g_session.ReadReplayShortcuts(Replay::Instance().ReadSection(13));
    g_session.ReadReplayShops(Replay::Instance().ReadSection(14));
    this->ReadReplayGuildInfo(Replay::Instance().ReadSection(0x16));
    this->ReadReplayActors(Replay::Instance().ReadSection(0xF));
    this->ReadReplayGroundItems(Replay::Instance().ReadSection(0x10));
    this->ReadReplayGroundSkills(Replay::Instance().ReadSection(0x11));

    const std::vector<unsigned char> data = Replay::Instance().ReadSection(0x17);
    if (data.begin() != data.end()) {
      GameModeChunkHeader header = {};
      unsigned offset = 6;
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
      if (header.type == 14000) {
        while (offset < data.size()) {
          header = {};
          header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
          offset += 6;
          if (header.type == 14001) {
            break;
          }

          if (header.type == 14011) {
            std::memcpy(&this->isOnQuest, data.data() + offset, header.size);
          }
          offset += header.size;
        }
      }
    }

    g_navigationManager.LoadReplayState(Replay::Instance().ReadSection(0x18));
    g_windowMgr.LoadReplaySettings(Replay::Instance().ReadSection(0x14));
    g_windowMgr.LoadReplayWindows(Replay::Instance().ReadSection(0x13));
    g_windowMgr.LoadReplayReserved(Replay::Instance().ReadSection(0x15));
    Replay::Instance().Rewind();
    this->replayPauseTick = 0;
    g_windowMgr.ShowWindowById(WID_CHATWND, Replay::Instance().header.options.recordRestricted);
  }

  if (g_screenShakeMode == 2) {
    this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, 0x22, g_screenShakeDuration <= 10000 && g_screenShakeDuration != 0, 0, 0, 0, 0);
  }

  if (g_session.GetOption(6) > 0) {
    g_session.SetOption(6, 0, 0);
    char message[256];
    std::memset(message, 0, sizeof(message));
    StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), LookupMessage(MSI_WEBSITE_LINK_ADDRESS), g_serviceOptions.Get(TT_SHOW_MERCHANT_SHOP_ON_OFF) ? "ON" : "OFF");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFF, 0, 0, 0);
  }

  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->RefreshShop();

  if (g_session.GetOption(0xB) == 1) {
    for (const std::string& blocked : g_whisperBlockMgr->Names()) {
      std::string arguments[1] = {blocked};
      this->ExecuteTalkType(0x12, arguments);
    }
    g_session.SetOption(0xB, 0, 0);
  }

  g_session.SetOption(0x14, 0, 0);
  this->isInitializing[0] = 0;
}

// OFFSET: 0x00D07030
// FUNCTION: fn_21580
void CGameMode::ReceiveSkillList(int packetType, const std::vector<SkillListEntry>& skills) {
  if (packetType == 0x10F || packetType == 0xB32) {
    g_session.m_skillBundle.skills.clear();
  } else if (packetType == 0x235) {
    g_session.m_homunSkills.clear();
  } else if (packetType == 0x29D) {
    g_session.m_mercSkills.clear();
  }

  for (auto entry = skills.begin(); entry != skills.end(); ++entry) {
    CSkillInfo skill;
    if ((entry->id >= 1 && entry->id < 7999) || g_session.IsHomunculusSkill(entry->id) || g_session.IsMercenarySkill(entry->id)) {
      skill.id = entry->id;
      skill.type = entry->type;
      skill.level = entry->level;
      skill.spCost = entry->spCost;
      skill.upgradable = entry->upgradable;
      skill.range = entry->range;
      skill.name = skill.IdName();
      skill.learnedLevel = entry->learnedLevel;
      skill.apCost = skill.UseApAmount(entry->level);
      g_session.m_skillBundle.ApplyLevelChange(skill);

      if (packetType == 0x10F || packetType == 0xB32) {
        g_session.UpdateSkill(skill, 0, 0);
      } else if (packetType == 0x235) {
        g_session.UpdateHomunSkill(skill, 0);
      } else if (packetType == 0x29D) {
        g_session.UpdateMercSkill(skill, 0);
      }
    }
  }

  if (packetType == 0x10F || packetType == 0xB32) {
    g_session.NotifySkillWindows();
  } else if (packetType == 0x235) {
    g_session.NotifyHomunSkillWindows();
  } else if (packetType == 0x29D) {
    g_session.NotifyMercSkillWindows();
  }

  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->RefreshPretend();
}

// OFFSET: 0x00D07250
// FUNCTION: fn_21581
void CGameMode::ReceiveSkillUpdate(
  std::uint16_t id, std::int16_t level, std::int16_t spCost, unsigned char upgradable, std::int16_t range, int type, std::int16_t learnedLevel
) {
  this->isReqUpgradeSkillLevel = 0;

  CSkillInfo skill = g_session.LookupSkill(id);
  skill.level = level;
  skill.spCost = spCost;
  skill.upgradable = upgradable;
  skill.range = range;
  skill.type = type;
  skill.learnedLevel = learnedLevel;
  skill.id = id;

  if (g_session.IsHomunculusSkill(id)) {
    g_session.UpdateHomunSkill(skill, 1);
  } else if (g_session.IsMercenarySkill(id)) {
    g_session.UpdateMercSkill(skill, 1);
  } else {
    g_session.UpdateSkill(skill, 1, 1);
  }

  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->RefreshPretend();
}

// OFFSET: 0x00CDB910
// FUNCTION: fn_21310
void CGameMode::ShowActorNameBalloon(std::uint32_t id) {
  if (!id) {
    return;
  }

  int& placed = this->isNameBalloonPlaced;
  if (this->balloonActorId != id && this->balloonActorId != 0) {
    this->balloonActorId = id;
    placed = 1;
    return;
  }

  this->balloonActorId = id;
  CRenderObject* actor = this->scene->GetGameActorByAID(id);
  if (actor) {
    CGameActor* gameActor = static_cast<CGameActor*>(actor);
    if (gameActor->GetEffectiveJob(-1) == JT_WARPNPC) {
      return;
    }

    placed = 0;
    this->UpdateNameBalloon(gameActor);
    if (this->bossAlarmAndCopyNameFlags[1] && g_session.IsPlayerJobId(gameActor->GetEffectiveJob(-1)) && IsAdminAccount(g_session.m_aid)) {
      g_windowMgr.SelectWhisperName(this->ActorName(id).c_str());
    }
    return;
  }

  if (id == g_session.m_aid) {
    placed = 0;
    this->UpdateNameBalloon(static_cast<CGameActor*>(this->scene->player));
    return;
  }

  CItem* item = this->scene->GetItemByAID(id);
  if (item) {
    std::string name(item->name.c_str());
    placed = 0;
    this->EnsureNameBalloon(this->nameBalloon);
    this->nameBalloon->AddNameLine(name.c_str());
    this->nameBalloon->SetIcon("", 0);
    this->nameBalloon->m_textColor = 0x95EFFF;
    this->nameBalloon->m_outlineColor = 0;
    this->nameBalloonWidth = this->nameBalloon->geometry.width;
    this->nameBalloonHeight = this->nameBalloon->geometry.height;
    this->PlaceWindowAtActor(this->nameBalloon, item);
  }
}

// OFFSET: 0x00CE18A0
// FUNCTION: fn_21332
void CGameMode::ProcessActorClick(int& handled, ActorPickInfo* pick, CGameActor* actor) {
  const unsigned char selfTargetOption = g_serviceOptions.Get(TT_NOSHIFT_ON_OFF);
  const unsigned char forceOption = g_serviceOptions.Get(TT_NOCTRL_ON_OFF);
  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);

  int attack;
  if (mode == 2 || mode == 4) {
    const int shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    attack = IsSupportSkill(this->skillId) ? shiftHeld : shiftHeld == 0;
    this->SetCursorAction(11);
  } else {
    attack = HIBYTE(GetAsyncKeyState(VK_SHIFT)) == 0;
    if (actor->objectType == NPC_PET_TYPE) {
      this->SetCursorAction(0);
    } else if (g_session.GetMapProperty08() != 0 || this->scene->isSiegeMode != 0) {
      if (g_session.IsLocalPlayerAid(actor->GetGdid())) {
        attack = 0;
        this->SetCursorAction(0);
      }
    } else {
      this->SetCursorAction(5);

    }
  }

  handled = 1;
  if (selfTargetOption) {
    attack = 1;
  }

  const std::uint32_t& homunAid = g_session.m_homunAid;
  const std::uint32_t& mercenaryAid = g_session.m_mercenaryAid;
  if (g_session.homunAlive != 0) {
    if (g_mouse.buttons[1] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (homun && homun->HasActiveHomunculusAI()) {
          AiMessage message = {};
          message.type = 3;
          message.target = actor->render.accountId;
          homun->SetResMsg2AI(&message, 0);
        }
      } else if (g_session.attackHomunEnemy != 0 && g_session.homunEnemyGid == actor->render.accountId) {
        CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (homun && homun->HasActiveHomunculusAI()) {
          AiMessage message = {};
          message.type = 3;
          message.target = g_session.homunEnemyGid;
          homun->SetMsg2AI(&message, 0);
        }
        g_session.attackHomunEnemy = 0;
      } else {
        this->SetCursorAction(11);
        g_session.homunEnemyGid = actor->render.accountId;
        g_session.homunEnemyX = 0;
        g_session.homunEnemyY = 0;
        g_session.moveHomunEnemy = 0;
        g_session.attackHomunEnemy = 1;
      }
      g_session.selectHomunEnemy = 1;
    }

    if (g_session.homunAlive != 0 && g_mouse.buttons[0] == 1 && g_session.IsHomunculusSkill(this->skillId)) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasActiveHomunculusAI()) {
        AiMessage message = {};
        message.type = 7;
        message.level = this->skillUseLevel;
        message.skill = this->skillId;
        message.target = actor->render.accountId;
        homun->SetMsg2AI(&message, 0);
      }
      return;
    }
  }

  if (g_session.mercenaryAlive != 0 && g_mouse.buttons[0] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasActiveMercenaryAI()) {
        AiMessage message = {};
        message.type = 3;
        message.target = actor->render.accountId;
        mercenary->SetResMsg2AI(&message, 1);
      }
    } else if (g_session.attackMercenaryEnemy != 0 && g_session.mercenaryEnemyGid == actor->render.accountId) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasActiveMercenaryAI()) {
        AiMessage message = {};
        message.type = 3;
        message.target = g_session.mercenaryEnemyGid;
        mercenary->SetMsg2AI(&message, 1);
      }
      g_session.attackMercenaryEnemy = 0;
    } else {
      this->SetCursorAction(11);
      g_session.mercenaryEnemyGid = actor->render.accountId;
      g_session.mercenaryEnemyX = 0;
      g_session.mercenaryEnemyY = 0;
      g_session.moveMercenaryEnemy = 0;
      g_session.attackMercenaryEnemy = 1;
    }
    g_session.selectMercenaryEnemy = 1;
  }

  int& targetAid = this->lastMonGid;
  int& forced = this->forcedAttack;
  if (attack != 0 && g_session.mercenaryAlive != 0 && g_mouse.buttons[0] == 1 && g_session.IsMercenarySkill(this->skillId)) {
    CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
    if (mercenary && mercenary->HasActiveMercenaryAI()) {
      AiMessage message = {};
      message.type = 7;
      message.level = this->skillUseLevel;
      message.skill = this->skillId;
      message.target = actor->render.accountId;
      mercenary->SetMsg2AI(&message, 1);
    }
    return;
  }

  if (g_mouse.buttons[0] == 1 && attack != 0 && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
    this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
    this->lastLockOnMonGid = targetAid;
    this->isAutoMoveClickOn = 0;
    if (g_session.GetSiegeMode() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
      this->RequestPlayerAction(targetAid, 0);
      forced = 1;
    } else {
      this->RequestPlayerAction(targetAid, 1);
      forced = 0;
    }
  } else if (forced == 0 && g_mouse.buttons[0] == 3) {
    targetAid = 0;
  }

  if ((this->oldMouseX != g_mouse.x || this->oldMouseY != g_mouse.y) && forced == 0 && g_mouse.buttons[0] == 0) {
    targetAid = 0;
  }

  this->ShowActorNameBalloon(targetAid);
  if (actor->render.state == 3) {
    targetAid = 0;
  }
}

// OFFSET: 0x00CDD6B0
// FUNCTION: fn_21317
CGameActor* CGameMode::NewActor(int job) {
  CGameActor* actor;
  if (IsPlayerJob(job) == 1) {
    actor = new CCharacterNpc;
  } else {
    const char* jobName = g_session.GetJobName(job, -1);
    if (std::strstr(jobName, ".gr2") != nullptr) {
      actor = new C3dGrannyGameActor(jobName);
    } else {
      actor = new CNpc;
    }
  }
  return actor;
}

// GLOBAL: gl_25393
int g_hoverActorGid;

// INLINED
__forceinline void CGameMode::ShowHoverActorName(CGameActor* actor) {
  this->EnsureNameBalloon(this->itemNameBalloon);
  CActorNameCard card = this->nameCard.Lookup(actor->render.accountId);
  if (card.received) {
    actor->hasHp == 1 && (this->scene->isPKZone == 1 || this->scene->isSiegeMode == 1)
        ? card.name = "??????"
        : card.name = static_cast<const std::string&>(this->nameCard.Name(actor->render.accountId));

    if (g_session.IsMonsterActor(actor) == 1) {
      UINameBalloonText* balloon = this->itemNameBalloon;
      unsigned char monsterType;
      if (actor->GetEffectiveJob(-1) == 20269) {
        monsterType = 0;
      } else {
        monsterType = actor->hpGaugeFlags;
      }
      balloon->SetName(card, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, monsterType);
    } else {
      this->itemNameBalloon->SetName(card, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, 0);
    }

    const unsigned char showEmblem = g_serviceOptions.Get(TT_SHOW_EMBLEM_ON_OFF);
    if ((g_session.IsNpcJob(actor->GetEffectiveJob(-1)) || g_session.IsMonsterActor(actor)) && card.groupIconId) {
      this->itemNameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.groupIconId).c_str(), 0);
    } else if (g_session.IsAllyActor(actor->render.accountId, actor->GetEffectiveJob(-1)) && card.groupIconId) {
      this->nameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.groupIconId).c_str(), 0);
    } else if (actor->GetGdid() && showEmblem) {
      std::string emblem = g_guildMgr->GetEmblemFilename(actor->GetGdid());
      const unsigned char framedEmblem = g_serviceOptions.Get(TT_EMBLEM_FRAME_ON_OFF);
      this->itemNameBalloon->SetIcon(emblem.c_str(), static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->isSiegeMode == 1 && framedEmblem ? 1 : 0);
    } else {
      this->itemNameBalloon->SetIcon("", 0);
    }

    UINameBalloonText* balloon = this->itemNameBalloon;
    if (balloon) {
      balloon->m_textColor = this->ActorNameColor(actor->GetEffectiveJob(-1), actor->render.accountId);
      balloon->m_outlineColor = 0;
    }
    this->PlaceWindowAtActor(this->itemNameBalloon, actor);
  }
}

// OFFSET: 0x00CDFE10
// FUNCTION: fn_21328
int CGameMode::ProcessMousePick(int result, ActorPickInfo* pick) {
  if (g_mouse.buttons[0] == 1 && pick) {
    UIMacroReporterWnd* reporter = static_cast<UIMacroReporterWnd*>(g_windowMgr.LookupWindow(WID_MACROREPORTERWND));
    if (reporter && reporter->activeOnly == true && reporter->m_mode == 0) {
      reporter->AddPickedActor(pick);
      return -1;
    }
  }

  if (g_windowMgr.LookupWindow(WID_COLLECTIONSYSTEMWND) || reinterpret_cast<std::intptr_t>(this->skillUseType) == 1) {
    return result;
  }

  int& lockedPick = this->forcedAttack;
  int& pickTarget = this->lastPcGid;
  int& clickTarget = this->lastMonGid;

  if (this->lockOnMouse == 0) {
    if (!pick) {
      if (lockedPick != 0 || g_mouse.buttons[0] == 1) {
        return result;
      }
      if (this->oldMouseX == g_mouse.x && this->oldMouseY == g_mouse.y) {
        return result;
      }
      pickTarget = 0;
      clickTarget = 0;
      return result;
    }
  } else {
    this->SetCursorAction(5);
    if (lockedPick != 0) {
      if (g_mouse.buttons[0] == 1) {
        this->SendMsg(MM_SETLOCKONMOUSE, 0, 0, 0, 0);
      } else {
        this->SetCursorAction(0);
        if (!pick) {
          return result;
        }
        result = 0;
      }
    } else {
      if (g_mouse.buttons[0] == 2) {
        return 1;
      }
      if (!pick && clickTarget == 0 && pickTarget == 0) {
        this->lockOnMouse = 0;
      }
    }
  }

  if (lockedPick == 0) {
    if (clickTarget != 0) {
      if (pick) {
        if (pick->kind == 3) {
          this->SetCursorAction(0);
          return 0;
        }
        if (pick->kind == 4) {
          this->SetCursorAction(0);
          this->ShowActorNameBalloon(pick->accountId);
          return this->HandleActorPick(pick, result);
        }
      }

      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(clickTarget));
      if (!actor) {
        clickTarget = 0;
        return result;
      }
      this->ProcessActorClick(result, pick, actor);
      return result;
    }

    if (pickTarget != 0) {
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pickTarget));
      if (!actor) {
        pickTarget = 0;
        return result;
      }
      if (lockedPick != 0 && std::bit_cast<int>(actor->render.motionFinishThreshold) == 3) {
        lockedPick = 0;
      }
      this->ProcessActorPick(result, pick, actor);
      return result;
    }
  }

  if (pick && result == 0) {
    this->ShowActorNameBalloon(pick->accountId);
    result = this->HandleActorPick(pick, 0);
  }
  return result;
}

#include "Contents/World/WorldModelView.h"
#include "Contents/World/WorldTerrainPicking.h"

// OFFSET: 0x00CE0210
// FUNCTION: fn_21329
int CGameMode::ProcessGroundClick(int result) {
  const auto drawCell = [](GroundGeometry* ground, const float* matrix, int column, int row, std::uint32_t color) {
    ground->DrawGrid(matrix, column, row, color);
  };

  int row;
  int column;
  UIMacroReporterWnd* reporter = static_cast<UIMacroReporterWnd*>(g_windowMgr.LookupWindow(WID_MACROREPORTERWND));
  if (reporter) {
    if (reporter->activeOnly == true && reporter->m_mode == 1) {
      column = -1;
      row = -1;
      this->PickScreenTerrain(column, row);
      const int radius = reporter->reportCount;
      for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
          drawCell(this->scene->ground, this->camera->frame.view.values, column + dx, row + dy, 0x660000FF);
        }
      }

      if (g_mouse.buttons[1] == 1) {
        reporter->SendMacroUserInfoPacket(static_cast<std::uint16_t>(column), static_cast<std::uint16_t>(row), static_cast<std::uint8_t>(radius));
        return -1;
      }
    }
  }

  UIMerchantShopMakeWnd* shop = static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.merchantShopMakeWnd);
  if (!shop && g_windowMgr.buyingShopMakeWnd) {
    shop = static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.buyingShopMakeWnd);
  }

  bool shopSuspended = false;
  if (shop) {
    if (shop->suspended == 1) {
      shopSuspended = true;
      column = -1;
      row = -1;
      this->PickScreenTerrain(column, row);
      drawCell(this->scene->ground, this->camera->frame.view.values, column, row, 0x66FF0000);
      if (g_mouse.buttons[0] == 1) {
        shop->SetSuspended(0);
        shop->SetSelection(static_cast<short>(column), static_cast<short>(row));
        if (g_windowMgr.Confirm(LookupMessage(MSI_BUTTON_PAY_BY_ZENY), 2, true, false, 0, "Message", 280, 120, 0) == 0xBB) {
          shop->SendShopRequest();
          return -1;
        }
        shop->SetSuspended(1);
      }
    } else if (shop->HasSelection() == true) {
      drawCell(this->scene->ground, this->camera->frame.view.values, shop->selectedRow, shop->selectedColumn, 0x66FF0000);
    }
  }

  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
  if (mode == 2 || mode == 3 || mode == 4) {
    return result;
  }

  column = -1;
  row = -1;
  if (!this->PickScreenTerrain(column, row)) {
    return result;
  }

  const std::uint32_t& homunAid = g_session.m_homunAid;
  const std::uint32_t& mercenaryAid = g_session.m_mercenaryAid;
  if (g_session.homunAlive != 0) {
    if (HIBYTE(GetAsyncKeyState(VK_MENU)) && g_mouse.buttons[1] == 1) {
      if (g_session.selectHomunEnemy != 0) {
        g_session.selectHomunEnemy = 0;
        return result;
      }

      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        if (homun && homun->HasActiveHomunculusAI()) {
          AiMessage message = {};
          message.type = 1;
          message.x = column;
          message.y = row;
          homun->SetResMsg2AI(&message, 0);
        }
        return result;
      }

      g_session.homunEnemyX = column;
      g_session.homunEnemyY = row;
      g_session.homunEnemyGid = 0;
      g_session.attackHomunEnemy = 0;
      if (homun && homun->HasActiveHomunculusAI()) {
        AiMessage message = {};
        message.type = 1;
        message.x = column;
        message.y = row;
        homun->SetMsg2AI(&message, 0);
      }
      g_session.moveHomunEnemy = 1;
      return result;
    }

    if (g_session.homunAlive != 0 && g_mouse.buttons[1] == 1) {
      g_session.ClearHomunStatus();
    }
  }

  if (g_session.mercenaryAlive != 0) {
    if (HIBYTE(GetAsyncKeyState(VK_MENU)) && g_mouse.buttons[0] == 1) {
      if (g_session.selectMercenaryEnemy != 0) {
        g_session.selectMercenaryEnemy = 0;
        return result;
      }

      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        if (mercenary && mercenary->HasActiveMercenaryAI()) {
          AiMessage message = {};
          message.type = 1;
          message.x = column;
          message.y = row;
          mercenary->SetResMsg2AI(&message, 1);
        }
        return result;
      }

      g_session.mercenaryEnemyX = column;
      g_session.mercenaryEnemyY = row;
      g_session.mercenaryEnemyGid = 0;
      g_session.attackMercenaryEnemy = 0;
      if (mercenary && mercenary->HasActiveMercenaryAI()) {
        AiMessage message = {};
        message.type = 1;
        message.x = column;
        message.y = row;
        mercenary->SetMsg2AI(&message, 1);
      }
      g_session.moveMercenaryEnemy = 1;
      return result;
    }

    if (g_session.mercenaryAlive != 0 && g_mouse.buttons[0] == 1) {
      g_session.ResetMercenaryStats();
      if (reinterpret_cast<std::intptr_t>(this->skillUseType) == 1 && g_session.IsMercenarySkill(this->skillId)) {
        CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
        if (mercenary && mercenary->HasActiveMercenaryAI()) {
          const std::uint32_t state = mercenary->render.state;
          if (state == 1 || state == 0 || state == 4) {
            AiMessage message;
            message.type = 8;
            message.x = column;
            message.y = row;
            message.level = this->skillUseLevel;
            message.skill = this->skillId;
            message.target = 0;
            mercenary->SetMsg2AI(&message, 1);
            return 1;
          }
        }
      }
    }
  }

  if (reinterpret_cast<std::intptr_t>(this->skillUseType) == 1 && g_mouse.buttons[0] == 1 && (result == 0 || result == 2)) {
    result = 1;
    if (g_session.IsHomunculusSkill(this->skillId) == 1 && g_session.homunAlive == 1) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasActiveHomunculusAI() == true) {
        const std::uint32_t state = homun->render.state;
        if (state == 0 || state == 1 || state == 4) {
          AiMessage message;
          message.type = 8;
          message.x = column;
          message.y = row;
          message.level = this->skillUseLevel;
          message.skill = this->skillId;
          message.target = 0;
          homun->SetMsg2AI(&message, 0);
          return 1;
        }
      }
    }

    if (this->scene->player) {
      const int skill = this->skillId;
      this->scene->player->SendMsg(nullptr, AM_PROCEEDGROUNDSKILL, skill, column, row, 0, 0, 0);
      const int level = this->skillUseLevel;
      this->scene->player->SendMsg(nullptr, AM_SET_SKILL_USE_LEVEL, level, 0, 0, 0, 0, 0);
    }
  }

  if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
    return result;
  }
  if (this->noMove != 0) {
    return result;
  }
  if (result != 0 && result != 2) {
    return result;
  }
  if (this->drag.source != 0) {
    return result;
  }

  if (g_session.m_showEffects != 0) {
    g_groundOverlayA = column;
    g_groundOverlayB = row;
    g_groundOverlaySkip = 0;
    int skip = 1;
    const TerrainHeightCell* cell = this->scene->terrain->CellAt(column, row);
    if (!cell || cell->type == 1) {
      g_groundOverlayC = 0xFEFF0000;
    } else if (cell->type == 5) {
      g_groundOverlayC = 0xFEFFFF00;
    } else {
      g_groundOverlayC = 0x6600FFAA;
      skip = 0;
    }
    g_groundOverlaySkip = shopSuspended == true ? 1 : skip;
  }

  std::uint32_t& clickTick = this->leftBtnClickTick;
  const bool elapsed = timeGetTime() > clickTick + 600;
  if (this->scene->player->proceedType == 0 && g_mouse.buttons[0] == 2) {
    if (!elapsed) {
      return result;
    }
    if (this->isAutoMoveClickOn == 0) {
      return result;
    }
  } else {
    if (g_mouse.buttons[0] != 1) {
      return result;
    }
    if (g_mouse.buttons[1] == 2) {
      return result;
    }
  }

  g_windowMgr.UpdateNavigationList();
  clickTick = timeGetTime();
  if (HIBYTE(GetAsyncKeyState(VK_SHIFT)) || g_windowMgr.chatRoomWnd || this->scene->player->render.state == 6) {
    this->scene->player->SendMsg(nullptr, AM_REQ_CHANGE_DIR, column, row, 0, 0, 0, 0);
    return result;
  }

  bool findClosest = true;
  CPlayer* player = this->scene->player;
  const TerrainHeightCell* cell = this->scene->terrain->CellAt(column, row);
  if (cell && cell->type != 1 && cell->type != 5 &&
      (column != player->destCellX || row != player->destCellZ)) {
    int startColumn;
    int startRow;
    int startSubColumn;
    int startSubRow;
    this->WorldSubcell(player->render.position.x, player->render.position.z, startColumn, startRow, startSubColumn, startSubRow);
    if (this->CanFindPath(startColumn, startRow, startSubColumn, startSubRow, column, row) && !this->IsRegisterCell(column, row)) {
      findClosest = false;
    }
  }

  if (findClosest && !this->GetClosestValidCell(&column, &row)) {
    return result;
  }

  if (g_windowMgr.menuWnd) {
    g_windowMgr.CloseWindow(WID_MENUWND);
  }
  if (this->scene->player) {
    this->scene->player->SendMsg(nullptr, AM_REQ_MOVE, column, row, 0, 0, 0, 0);
  }
  return result;
}

#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Window/InputFocus.h"

#include "Client/Application/ApplicationMessages.h"
#include "Client/Mode/ModeFade.h"
#include "Contents/Actors/ActorNameCard.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Client/Stove/StovePCSDK.h"
#include "Utility/Hash/GGHashMgr.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

#pragma pack(push, 1)
struct StatusPointRequestPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t statusId;
  /* 0004 */ std::uint8_t amount;
};
#pragma pack(pop)

static_assert(sizeof(StatusPointRequestPacket) == 5);

// GLOBAL: gl_25463
std::uint32_t g_spriteAtlasClearTick;

// OFFSET: 0x00CDF1E0
// FUNCTION: fn_21323
void CGameMode::Update() {
  PumpApplicationMessages();
  this->ProcessIncomingPackets();
  if (Replay::Instance().AdvanceFrame() == 0) {
    g_windowMgr.replayControlWnd->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
  }

  if (this->running != 0) {
    g_timeScheduler->Process();
    g_playerNameDictionary->Update();
    this->nameCard.ProcessRequests();
    g_guildMgr->UpdateEmblemRequests(
      *g_emblemDataMgr, g_session.m_aid, g_session.m_accountName, g_session.m_webAuthToken, GetRagConnection(),
      *g_asyncWorkMgr
    );
    g_asyncWorkMgr->Update(1000);
    this->ProcessInput();

    if (g_hoverActorGid != 0) {
      if (g_mouse.buttons[0] != 0) {
        g_hoverActorGid = 0;
        g_followLostTick = 0;
        g_followUpdateTick = 0;
      } else {
        CGameActor* actor = static_cast<CGameActor*>(this->scene->GetActorByGID(g_hoverActorGid));
        if (actor == nullptr) {
          if (g_followLostTick == 0) {
            g_followLostTick = timeGetTime();
          }
          if (timeGetTime() - g_followLostTick > 10000) {
            g_hoverActorGid = 0;
            g_followUpdateTick = 0;
            g_followLostTick = 0;
          }
        } else {
          g_followLostTick = 0;
          if (timeGetTime() - g_followUpdateTick > 1000) {
            g_followUpdateTick = timeGetTime();
            int x = actor->movementState[5];
            int y = actor->destCellY;
            int cellX = x;
            int cellY = y;
            if (g_windowMgr.chatRoomWnd == nullptr && this->scene->player->render.state != 6) {
              bool reachable = true;
              if (!this->IsValidCell(x, y)) {
                reachable = this->GetClosestValidCell(&cellX, &cellY);
                x = cellX;
                y = cellY;
              }
              if (reachable) {
                if (g_windowMgr.menuWnd != nullptr) {
                  g_windowMgr.CloseWindow(WID_MENUWND);
                }
                this->scene->player->SendMsg(nullptr, AM_REQ_MOVE, x, y, 0, 0, 0, 0);
              }
            } else {
              this->scene->player->SendMsg(nullptr, AM_REQ_CHANGE_DIR, x, y, 0, 0, 0, 0);
            }
          }
        }
      }
    }

    this->UpdateLoopSounds();

    g_soundMgr->Update();
    if (g_soundMgr->IsComplete() && g_soundMgr->secondaryEnabled != 0) {
      g_soundMgr->secondaryEnabled = 0;
      if (g_soundMgr->IsStopped()) {
        CSoundMgr::Pause(g_soundMgr, 0);
      } else {
        g_soundMgr->SetDefaultName(g_session.GetBgmFileName(this->MapName().c_str()));
      }
    }

    g_nProtectGameGuardMgr->Stop();
    g_ggHashMgr->VerifyPendingFiles();
    if (g_session.m_launchPassCommandLine) {
      Base_RunCallback();
    }

    if (g_updateTick > 0) {
      SendGameTick(
        false, this->syncAcknowledged, this->syncRequestTick, g_gameTickInterval,
        GetRagConnection(), g_packetLengths, g_pingLogEnabled, g_pingLogFirstTick, g_pingLogTicks
      );
      if (timeGetTime() > g_spriteAtlasClearTick) {
        g_spriteAtlasClearTick = timeGetTime() + 15000;
        g_renderer->atlas.ClearExpired(g_resourceReuseDelay);
      }
    }

    const int frames = GetSkipFrameCount();
    g_updateTick = frames;
    this->ProcessInput();
    ResourceMgr::Instance().UpdateTransient();
    if (frames > 0) {
      for (int frame = frames; frame != 0; --frame) {
        g_effectMgr->Update();
        this->scene->OnProcess();
        this->scene->ground->UpdateWater();
        this->camera->Update();
      }
    } else if (frames == 0) {
      this->scene->OnProcess();
      this->camera->Update();
    }
  }

  if (g_restoreSurfaces != 0) {
    g_restoreSurfaces = 0;
    GetRenderSystemHolder()->TestDevice();
    g_windowMgr.InvalidateAll();
  }

  g_weatherEffect.Process();
  if (g_isAppActive == 0 && GetRenderSystemHolder()->IsWindowed()) {
    return;
  }

  if (ShouldRenderFrame()) {
    g_renderer->ClearWorld();
    g_actorPickNode.Clear();
    this->camera->Render();
    this->DrawMiniMap();
    g_windowMgr.DrawCenteredBackground();
    g_windowMgr.DrawWindows();
    g_mapInfoMgr->DrawMapName();
    if (this->scene->isPKZone != 0 || g_session.m_isPvpZone != 0) {
      this->DrawMiniMapCoordinates();
      this->DrawTimeAttackTimer();
    }
    if (g_session.m_timeAttackVisible != 0) {
      this->DrawTimeAttackCountdown(g_session.m_timeAttackCountdown, g_session.m_timeAttackMode);
    }

    if (this->drag.source != 0 && (g_windowMgr.itemDropCntWnd == nullptr || g_windowMgr.itemDropCntWnd->geometry.visible == 0)) {
      if (this->drag.source == 0x1D) {
        if (g_windowMgr.Window4F250Ready()) {
          g_windowMgr.MoveTransient(g_mouse.x, g_mouse.y);
        }
      } else if (this->drag.resource.empty()) {
        this->drag.Clear();
      } else {
        const std::string resource = this->drag.resource;
        CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load((resource + ".spr").c_str()));
        CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load((resource + ".act").c_str()));
        this->DrawActionSprite(g_mouse.x, g_mouse.y, action, sprite, 0, 0, 1.0f, 0.0f, 0xFAFFFFFF, 0x201);
      }
    }

    this->DrawCursor();
    this->DrawMiniMapNumber();
    this->DrawFadeIn();
    if (g_renderer->Draw()) {
      g_renderer->DrawOverlays(*GetRenderSystemHolder()->system);
    }
    g_renderer->Present(1);
  }

  this->TakeScreenshot();

  for (int index = 0; index < 6; ++index) {
    unsigned char& pending = this->replayDragState[1 + index];
    if (pending == 0) {
      continue;
    }

    StatusPointRequestPacket packet;
    packet.packetType = ID_CZ_STATUS_CHANGE;
    if (index == 0) {
      packet.statusId = 13;
    } else if (index == 1) {
      packet.statusId = 14;
    } else if (index == 2) {
      packet.statusId = 18;
    } else if (index == 3) {
      packet.statusId = 16;
    } else if (index == 4) {
      packet.statusId = 15;
    } else if (index == 5) {
      packet.statusId = 17;
    }
    packet.amount = 1;
    if (this->isPlayerDead == 0) {
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
      --pending;
    }
  }
}

// OFFSET: 0x00CE0B80
// FUNCTION: fn_21330
void CGameMode::ProcessInput() {
  g_mouse.Update();
  int handled = g_windowMgr.ProcessMouse();
  g_windowMgr.UpdateFrameWindows();
  this->UpdateBalloons();
  this->ExpireTooltip();
  this->ProcessRightButtonDrag();
  this->ProcessMouseWheel(handled);

  if (timeGetTime() > this->windowInvalidateTick + 1000) {
    g_windowMgr.InvalidateRegisteredWindows();
    this->windowInvalidateTick = timeGetTime();
  }

  if (handled == 2) {
    handled = 0;
  }

  if (this->skillUseType != nullptr) {
    if (g_session.m_mapTransitionFlag == 0) {
      if (g_mouse.wheel > 0) {
        this->SendMsg(MM_USESKILL_LEVEL_PLUS, 1, 0, 0, 0);
      } else if (g_mouse.wheel < 0) {
        this->SendMsg(MM_USESKILL_LEVEL_MINUS, 1, 0, 0, 0);
      }
    }
    g_session.m_mapTransitionFlag = 0;
  }

  if (g_mouse.buttons[0] == 3) {
    if (this->scene->player) {
      this->scene->player->SendMsg(nullptr, AM_ATTACKCLICKUP, 0, 0, 0, 0, 0, 0);
    }
    this->SendMsg(MM_DROP, g_mouse.x, g_mouse.y, 0, 0);
    UIMenuWnd* menu = static_cast<UIMenuWnd*>(g_windowMgr.menuWnd);
    if (menu) {
      if (menu->m_menuFlags) {
        menu->m_menuFlags = 0;
      } else {
        g_windowMgr.CloseWindow(WID_MENUWND);
      }
    }
    this->isAutoMoveClickOn = 1;
  }

  g_pickModifierKey = HIBYTE(GetAsyncKeyState(VK_SHIFT));
  ActorPickInfo* pick = g_actorPickNode.Pick(static_cast<float>(g_mouse.x), static_cast<float>(g_mouse.y));
  UIWindow* blocker = g_windowMgr.LookupWindow(WID_EXCHANGEUI);
  if (!blocker) {
    this->OpenActorMenu(pick, handled);
  }

  this->cursorOffsetX = 0.0f;
  this->cursorOffsetY = 0.0f;
  if (handled == 0) {
    const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
    g_session.UpdateMonsterInfoEnabled(mode == 2 || mode == 4);
    if (pick) {
      bool offset;
      if (this->skillId == AM_POTIONPITCHER) {
        offset = g_session.IsCompanionJob(pick->job) && pick->kind == 4;
      } else {
        int eligible;
        if (g_session.monsterInfoEnabled && g_session.IsMonsterClass(pick->job)) {
          eligible = 1;
        } else if (g_serviceOptions.Get(TT_ITEM_SNAP_ON_OFF)) {
          eligible = pick->kind == 1;
        } else {
          eligible = 0;
        }
        offset = pick->kind != 3 && pick->kind != 4 && eligible;
      }
      if (offset) {
        this->cursorOffsetX = (pick->left - pick->right) * g_depthAverageHalf + pick->right - static_cast<float>(g_mouse.x);
        this->cursorOffsetY = (pick->top - pick->bottom) * g_depthAverageHalf + pick->bottom - static_cast<float>(g_mouse.y);
      }
    }
  }

  this->MarkWorldOcclusion();

  if (this->isPlayerDead == 0 && (this->isOnQuest == 0 || this->scene->player->actorWindows[3] != 0) && !blocker && g_windowMgr.itemStoreWnd == nullptr &&
      !g_windowMgr.LookupWindow(WID_RUNESYSTEMWND)) {
    handled = this->ProcessGroundClick(this->ProcessMousePick(handled, pick));
    this->UpdateHoverNameBalloon();
    if (handled != 1 && g_mouse.buttons[0] == 1) {
      g_windowMgr.SetFocus(nullptr);
    }
  } else if (pick && handled == 0) {
    this->ShowActorNameBalloon(pick->accountId);
  }

  if ((handled == 1 || g_mouse.buttons[0] == 1) && g_mouse.buttons[0] == 1) {
    g_moveRequestPending = false;
    this->SendMsg(MM_CANCEL_USE_SKILL, 0, 0, 0, 0);
    if (timeGetTime() > this->noMoveStartTick + 5000) {
      this->noMove = 0;
    }
    this->leftBtnClickTick = timeGetTime();
  }

  const bool statusIcons = g_serviceOptions.Get(TT_STATEINFO_ON_OFF) != 0;
  if (Replay::Instance().state != 6 && statusIcons) {
    this->ProcessStatusIconTooltip(g_mouse.x, g_mouse.y);
  }

  if (handled == 0) {
    if (this->skillUseType != nullptr) {
      this->SetCursorAction(10);
    } else if (g_mouse.buttons[1] != 2) {
      int column = -1;
      int row = -1;
      this->PickScreenTerrain(column, row);
      const TerrainHeightCell* cell = this->scene->terrain->CellAt(column, row);
      if (cell && cell->type != 1 && cell->type != 5) {
        this->SetCursorAction(0);
      } else {
        this->SetCursorAction(13);
      }
    }
  }

  this->oldMouseX = g_mouse.x;
  this->oldMouseY = g_mouse.y;
}

// OFFSET: 0x00CE1010
// FUNCTION: fn_21331
void CGameMode::UpdateHoverNameBalloon() {
  CGameActor* actor;
  if (g_hoverActorGid) {
    actor = static_cast<CGameActor*>(this->scene->GetActorByGID(g_hoverActorGid));
    if (!actor) {
      return;
    }

    if (actor->render.visible == 1 && actor->render.enabled == 1 && !StateFlag02(actor->GetStateFlags())) {
      this->ShowHoverActorName(actor);
    }
  } else {
    if (!this->lockOnMouse) {
      this->SendMsg(MM_HIDEMOUSEPOINTER, 0, 0, 0, 0);
      return;
    }

    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(this->lastLockOnMonGid));
    if (!actor) {
      return;
    }

    if (!actor->objectType && actor->render.visible == 1 && actor->render.enabled == 1 && !StateFlag02(actor->GetStateFlags())) {
      this->isItemNameBalloonPlaced = 0;
      this->ShowHoverActorName(actor);
    }
  }

  if (actor->GetEffectiveJob(-1) != JT_WARPNPC &&
      (actor->objectType || (actor->render.visible == 1 && actor->render.enabled == 1 && !StateFlag02(actor->GetStateFlags())))) {
    this->SendMsg(MM_SHOWMOUSEPOINTER, static_cast<int>(actor->render.position.x), static_cast<int>(actor->render.position.z), 0, 0);
    return;
  }

  this->SendMsg(MM_HIDEMOUSEPOINTER, 0, 0, 0, 0);
}

#include "Client/Browser/WebBrowser.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include <direct.h>
#include <format>
#include <io.h>

#pragma pack(push, 1)
struct TalkWhisperSettingPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[0x18];
  /* 001A */ unsigned char type;
};

struct TalkConfigPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char value;
};

struct TalkIntPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int value;
};

struct TalkSelectPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char enabled;
  /* 0003 */ int value;
  /* 0007 */ unsigned char option;
};

struct TalkCameraPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char action;
  /* 0003 */ float range;
  /* 0007 */ float rotation;
  /* 000B */ float latitude;
};

struct TalkNamePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[0x10];
};
#pragma pack(pop)

static_assert(sizeof(TalkWhisperSettingPacket) == 0x1B);
static_assert(sizeof(TalkCameraPacket) == 0xF);
static_assert(sizeof(TalkSelectPacket) == 8);

// OFFSET: 0x00CE4B90
// FUNCTION: fn_21338
void CGameMode::ExecuteTalkType(int talkType, std::string* arguments) {
  std::string defaults[3];
  std::string* parts = arguments ? arguments : defaults;
  if (g_serviceType == ServiceTaiwan && talkType == TT_REQ_NUM_USER) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_COMMAND)), 0xFFFF, 0, 0, 0);
    return;
  }

  switch (talkType) {
  case TT_GUILD_CHAT:
    g_windowMgr.SetChatInputText(parts[0].c_str());
    this->SendMsg(MM_CHAT_GUILD, 0, 0, 0, 0);
    break;
  case TT_REQ_WHISPER_PC_EX: {
    this->refuseWhisperName = parts[0];
    if (this->refuseWhisperName.size() > 0x18) {
      break;
    }
    g_whisperBlockMgr->Add(this->refuseWhisperName);
    TalkWhisperSettingPacket packet;
    packet.id = ID_CZ_SETTING_WHISPER_PC;
    StringFormat(packet.name, 0x18, static_cast<std::size_t>(-1), "%s", this->refuseWhisperName.c_str());
    packet.type = 0;
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_REQ_WHISPER_PC_IN: {
    this->refuseWhisperName = parts[0];
    if (this->refuseWhisperName.size() > 0x18) {
      break;
    }
    g_whisperBlockMgr->Remove(this->refuseWhisperName);
    TalkWhisperSettingPacket packet;
    packet.id = ID_CZ_SETTING_WHISPER_PC;
    StringFormat(packet.name, 0x18, static_cast<std::size_t>(-1), "%s", this->refuseWhisperName.c_str());
    packet.type = 1;
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_REQ_SAVE_CHAT_FROM_CHATROOM:
    g_windowMgr.SendNotice(UIM_SAVE_CHAT_TO_FILE_FROM_CHATROOM, 0, 0, 0, 0, 0);
    break;
  case TT_REQ_AUTO_SAVE_CHAT: {
    if (g_session.IsAdminSubMode(7) == 0) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_COMMAND)), 0xFFFF, 0, 0, 0);
      break;
    }
    char text[128];
    g_session.m_whisperRecord = 1 - g_session.m_whisperRecord;
    if (g_session.m_whisperRecord != 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (g_session.IsAdminSubMode(7) != 0) {
        char directory[3000];
        mode->chatLogLines = 0;
        std::sprintf(directory, "%s\\Chat", g_applicationDirectory);
        int index = 0;
        if (_access(directory, 0) == 0 || _mkdir(directory) == 0) {
          std::sprintf(g_chatLogPath, "%s\\Chat\\Chat.txt", g_applicationDirectory);
          if (_access(g_chatLogPath, 0) == 0) {
            do {
              ++index;
              std::sprintf(g_chatLogPath, "%s\\Chat\\Chat%03d.txt", g_applicationDirectory, index);
            } while (_access(g_chatLogPath, 0) == 0);
          }
        }
      }
      std::sprintf(text, "Chatting Auto Save Start..");
    } else {
      std::sprintf(text, "Chatting Auto Save END");
      g_modeMgr.GetGameMode();
      CloseWhisperRecord();
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    break;
  }
  case TT_UNKNOWN:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_COMMAND)), 0xFFFF, 0, 0, 0);
    break;
  case TT_NORMAL:
  case TT_NORMAL_FROM_CHATROOM: {
    char text[364];
    int party = 0;
    int alternate = 0;
    std::strcpy(text, g_windowMgr.chatInputText.c_str());
    if (text[0] == '%') {
      if (std::strlen(text) <= 1) {
        break;
      }
      g_windowMgr.SetChatInputText(text + 1);
      party = 1;
    } else if (text[0] == '$') {
      if (std::strlen(text) <= 1) {
        break;
      }
      g_windowMgr.SetChatInputText(text + 1);
      alternate = 1;
    }
    if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      party = 1;
    }
    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      alternate = 1;
    }
    bool clan = false;
    if (text[0] == '#') {
      if (std::strlen(text) <= 1) {
        break;
      }
      g_windowMgr.SetChatInputText(text + 1);
      clan = true;
    }
    int clanChat = clan;
    if (HIBYTE(GetAsyncKeyState(VK_CAPITAL))) {
      clanChat = 1;
    }
    if (talkType == TT_NORMAL_FROM_CHATROOM) {
      this->SendMsg(MM_CHATMSG, 0, 0, 0, 0);
      break;
    }
    if (g_session.battleChat != 0) {
      this->SendMsg(MM_CHAT_BATTLEFIELD, 0, 0, 0, 0);
      break;
    }
    const int sendMode = g_session.m_chatTarget;
    if (sendMode == 2 && g_guildInfo.m_guildId != 0) {
      this->SendMsg(alternate ? MM_CHATMSG : MM_CHAT_GUILD, 0, 0, 0, 0);
      break;
    }
    if (sendMode == 1) {
      if (party == 0 && g_session.PartyCount() != 0) {
        this->SendMsg(MM_CHAT_PARTY, 0, 0, 0, 0);
      } else {
        this->SendMsg(MM_CHATMSG, 0, 0, 0, 0);
      }
      break;
    }
    if (sendMode == 3 && g_clanInfo->m_hasClan == 1) {
      this->SendMsg(MM_CHAT_CLAN, 0, 0, 0, 0);
      break;
    }
    if (sendMode == 4 && g_guildInfo.m_guildId != 0) {
      this->SendMsg(static_cast<char>(clanChat) ? MM_CHATMSG : MM_CHAT_ALLY, 0, 0, 0, 0);
      break;
    }
    if (party != 0 && g_session.PartyCount() != 0) {
      this->SendMsg(MM_CHAT_PARTY, 0, 0, 0, 0);
    } else if (alternate != 0) {
      this->SendMsg(MM_CHAT_GUILD, 0, 0, 0, 0);
    } else if (static_cast<char>(clanChat)) {
      this->SendMsg(MM_CHAT_ALLY, 0, 0, 0, 0);
    } else {
      this->SendMsg(MM_CHATMSG, 0, 0, 0, 0);
    }
    break;
  }
  case TT_EX_HI: {
    char text[256];
    if (std::strcmp(parts[0].c_str(), "") == 0) {
      g_session.HiFriends(nullptr);
      std::sprintf(text, "[ To Friends ] : (Hi) *^_^*");
    } else {
      char* message = const_cast<char*>(g_windowMgr.chatInputText.c_str());
      std::string copy(message);
      g_session.HiFriends(message);
      if (g_serviceType == ServiceKorea) {
        std::sprintf(text, "[ To Friends ] : (Hi) %s", copy.c_str());
      } else {
        std::sprintf(text, "[ To Friends ] : (Hi) %s%s", g_serverLanguage->Prefix(false), copy.c_str());
      }
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x1EFFFF, 0, 0, 0);
    break;
  }
  case TT_LEVEL:
    for (int count = 32; count != 0; --count) {
      this->SendMsg(MM_ITEM_CREATE, reinterpret_cast<int>("LEVEL"), 0, 0, 0);
    }
    break;
  case TT_SP:
    for (int count = 5; count != 0; --count) {
      this->SendMsg(MM_ITEM_CREATE, reinterpret_cast<int>("SPPOINT"), 0, 0, 0);
      this->SendMsg(MM_ITEM_CREATE, reinterpret_cast<int>("JOBLEVEL"), 0, 0, 0);
    }
    break;
  case TT_RANDOM_LOADING_SCREEN_ON_OFF: {
    const char* text;
    if (g_session.m_randomLoadingScreen) {
      g_session.m_randomLoadingScreen = false;
      text = "RANDOM LOADING SCREEN ON";
    } else {
      g_session.m_randomLoadingScreen = true;
      text = "RANDOM LOADING SCREEN OFF";
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x6E96FF, 0, 0, 0);
    break;
  }
  case TT_SET1_ON_OFF:
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x71, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x6C, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x6D, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x8D, 0, 0, 0);
    break;
  case TT_SET2_ON_OFF:
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x71, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x6C, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x6D, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x78, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x79, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x8D, 0, 0, 0);
    break;
  case TT_SET3_ON_OFF:
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x71, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x6C, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x6D, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x8D, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0xA5, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x87, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0xA7, 0, 0, 0);
    break;
  case TT_QUICKSPELL3_ON_OFF:
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x78, 0, 0, 0);
    g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x79, 0, 0, 0);
    break;
  case TT_SKILLFAIL_ON_OFF:
    g_serviceOptions.Set(0x71, !g_serviceOptions.Get(TT_SKILLFAIL_ON_OFF));
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_serviceOptions.Get(TT_SKILLFAIL_ON_OFF) ? MSI_SKILLFAIL_ON : MSI_SKILLFAIL_OFF)), 0x64FFFF, 0, 0, 0);
    break;
  case TT_LOGINOUT_ON_OFF:
    g_serviceOptions.Set(0xA5, !g_serviceOptions.Get(TT_LOGINOUT_ON_OFF));
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_serviceOptions.Get(TT_LOGINOUT_ON_OFF) ? MSI_LOGINOUT_ON : MSI_LOGINOUT_OFF)), 0x64FFFF, 0, 0, 0);
    g_windowMgr.RefreshGameSettings();
    break;
  case TT_EMBLEM_FRAME_ON_OFF:
    g_serviceOptions.Set(0xF3, !g_serviceOptions.Get(TT_EMBLEM_FRAME_ON_OFF));
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_serviceOptions.Get(TT_EMBLEM_FRAME_ON_OFF) ? MSI_USESKILL_FAIL_EMPTY_SOUL_ENERGY : MSI_ADDSKILL_FAIL_INCLUDE_SKILL_YOU_CANNOT_LEARN)), 0x64FFFF, 0, 0, 0);
    g_windowMgr.RefreshGameSettings();
    break;
  case TT_BATTLE_ON_OFF:
    g_windowMgr.chatModeChanged = g_windowMgr.chatModeChanged == 0;
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    if (g_windowMgr.chatModeChanged) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLE_ON)), 0x64FFFF, 0, 0, 0);
    } else {
      if (!g_windowMgr.inputSuggestionsEnabled) {
        ChatWindow()->ToggleInput();
      }
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLE_OFF)), 0x64FFFF, 0, 0, 0);
    }
    break;
  case TT_BATTLECHAT_ON_OFF:
    g_session.battleChat = 1 - g_session.battleChat;
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_session.battleChat ? MSI_BATTLECHAT_ON : MSI_BATTLECHAT_OFF)), 0x64FFFF, 0, 0, 0);
    break;
  case TT_AUTO_MSG_ON_OFF:
    g_session.m_autoReplyEnabled = !g_session.m_autoReplyEnabled;
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    if (g_session.m_autoReplyEnabled) {
      g_windowMgr.EnsureWindow(WID_AUTOMESSAGEWND);
    } else {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_AUTO_MSG_OFF)), 0x64FFFF, 0, 0, 0);
    }
    break;
  case TT_TRACEAI_ON_OFF:
    g_session.m_traceAi = g_session.m_traceAi == 0;
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    if (g_session.m_traceAi != 0) {
      char path[256];
      GetCurrentDirectoryA(sizeof(path), path);
      char* data = std::strstr(path, "\\data");
      if (data) {
        std::memcpy(path, path, data - path);
      }
      std::strcat(path, "\\TraceAI.txt");
      CFile file;
      file.Open(path, 0x1001, true, false);
      file.Write("", 0);
      file.Close();
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TRACE_AI_ON)), 0x64FFFF, 0, 0, 0);
    } else {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TRACE_AI_OFF)), 0x64FFFF, 0, 0, 0);
    }
    break;
  case TT_VOLUME: {
    const int volume = std::atoi(parts[0].c_str());
    g_soundMgr->SetSampleVolume(volume);
    g_soundMgr->Set3DVolume(volume);
    char text[120];
    std::sprintf(text, "%s %d", LookupMessage(MSI_VOLUME_OF_SOUND), volume);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
    g_windowMgr.RefreshGameSettings();
    break;
  }
  case TT_BVOLUME: {
    const int volume = std::atoi(parts[0].c_str());
    CSoundMgr::SetVolume(g_soundMgr, volume);
    g_soundMgr->SetTargetVolume(CSoundMgr::Volume(g_soundMgr));
    char text[120];
    std::sprintf(text, "%s %d", LookupMessage(MSI_VOLUME_OF_BGM), volume);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
    g_windowMgr.RefreshGameSettings();
    break;
  }
  case TT_EFFECT_SOUND_ON_OFF:
    g_serviceOptions.Set(0xB, !g_serviceOptions.Get(TT_EFFECT_SOUND_ON_OFF));
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_serviceOptions.Get(TT_EFFECT_SOUND_ON_OFF) ? MSI_SOUND_ON : MSI_SOUND_OFF)), 0x64FFFF, 0, 0, 0);
    if (g_serviceOptions.Get(TT_EFFECT_SOUND_ON_OFF)) {
      g_soundMgr->tingOnly = 0;
    }
    g_windowMgr.RefreshGameSettings();
    break;
  case TT_TINGONLY_ON_OFF:
    g_soundMgr->tingOnly = g_soundMgr->tingOnly == 0;
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_soundMgr->tingOnly ? MSI_TINGONLY_ON : MSI_TINGONLY_OFF)), 0x64FFFF, 0, 0, 0);
    break;
  case TT_FRAMESKIP_ON_OFF:
    g_frameskip = 1 - g_frameskip;
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_frameskip ? MSI_FRAME_SKIP_ON : MSI_FRAME_SKIP_OFF)), 0x64FFFF, 0, 0, 0);
    break;
  case TT_MUSIC_ON_OFF:
    g_serviceOptions.Set(7, !g_serviceOptions.Get(TT_MUSIC_ON_OFF));
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(g_serviceOptions.Get(TT_MUSIC_ON_OFF) ? MSI_BGM_ON : MSI_BGM_OFF)), 0x64FFFF, 0, 0, 0);
    if (g_serviceOptions.Get(TT_MUSIC_ON_OFF)) {
      g_soundMgr->PlayStream(nullptr, 0, false);
    } else {
      g_soundMgr->StopStreams();
    }
    g_windowMgr.RefreshGameSettings();
    break;
  case TT_REQ_MOVETO_MAP: {
    const int third = std::atoi(parts[2].c_str());
    const int second = std::atoi(parts[1].c_str());
    this->SendMsg(MM_MOVETO_MAP, reinterpret_cast<int>(parts[0].c_str()), second, third, 0);
    break;
  }
  case TT_REQ_PK: {
    std::string value(parts[0].c_str());
    LowercaseText(value);
    if (value == "off" || value == "on") {
      this->SendMsg(0x55, value == "off" ? 0 : 1, 0, 0, 0);
    }
    break;
  }
  case TT_REQ_EXCHANGE_ITEM: {
    const std::uint32_t id = this->nameCard.FindId(parts[0].c_str());
    if (id != 0) {
      this->SendMsg(MM_REQ_EXCHANGE_ITEM, id, 0, 0, 0);
    }
    break;
  }
  case TT_OPEN_TIP:
    if (!g_windowMgr.CloseWindow(WID_TIPBOXWND)) {
      g_windowMgr.EnsureWindow(WID_TIPBOXWND);
    }
    break;
  case TT_REQ_EMOTION_SURPRISE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SURPRISE"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_QUESTION:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_QUESTION"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_DELIGHT:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_DELIGHT"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_THROB:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_THROB"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SWEAT:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SWEAT"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_AHA:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_AHA"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_FRET:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_FRET"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ANGER:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ANGER"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_MONEY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_MONEY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_THINK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_THINK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SCISSOR:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SCISSOR"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ROCK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ROCK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_WRAP:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_WRAP"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_BIGTHROB:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_BIGTHROB"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_THANKS:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_THANKS"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_KEK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_KEK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SORRY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SORRY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SMILE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SMILE"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_PROFUSELY_SWEAT:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_PROFUSELY_SWEAT"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SCRATCH:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SCRATCH"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_BEST:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_BEST"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_STARE_ABOUT:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_STARE_ABOUT"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HUK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HUK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_O:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_O"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_X:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_X"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HELP:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HELP"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_GO:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_GO"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_CRY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_CRY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_KIK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_KIK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_CHUP:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_CHUP"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_CHUPCHUP:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_CHUPCHUP"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HNG:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HNG"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_OK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_OK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_STARE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_STARE"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HUNGRY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HUNGRY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_COOL:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_COOL"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_MERONG:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_MERONG"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SHY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SHY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_GOODBOY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_GOODBOY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SPTIME:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SPTIME"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SEXY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SEXY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_COMEON:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_COMEON"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SLEEPY:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SLEEPY"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_CONGRATULATION:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_CONGRATULATION"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HPTIME:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HPTIME"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SPARK:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SPARK"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_CONFUSE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_CONFUSE"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_OHNO:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_OHNO"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HUM:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HUM"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_BLABLA:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_BLABLA"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_OTL:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_OTL"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_LUV:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_LUV"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_MOBILE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_MOBILE"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_MAIL:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_MAIL"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ANTENNA0:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ANTENNA0"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ANTENNA1:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ANTENNA1"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ANTENNA2:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ANTENNA2"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ANTENNA3:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ANTENNA3"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_HUM2:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_HUM2"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ABS:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ABS"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_OOPS:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_OOPS"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_SPIT:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SPIT"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_ENE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_ENE"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_PANIC:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_PANIC"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_WHISP:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_WHISP"), 0, 0, 0);
    break;
  case TT_REQ_EMOTION_DICE:
    this->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_DICE1"), 0, 0, 0);
    break;
  case TT_BROADCAST:
    this->SendMsg(MM_BROADCASTMSG, 0, 0, 0, 0);
    break;
  case TT_LOCALBROADCAST:
    this->SendMsg(MM_BROADCASTMSG, 1, 0, 0, 0);
    break;
  case TT_NAMELESS_BROADCAST:
    this->SendMsg(MM_BROADCASTMSG, 2, 0, 0, 0);
    break;
  case TT_NAMELESS_LOCALBROADCAST:
    this->SendMsg(MM_BROADCASTMSG, 3, 0, 0, 0);
    break;
  case TT_NAMELESS_BLUEBROADCAST:
    this->SendMsg(MM_BROADCASTMSG, 4, 0, 0, 0);
    break;
  case TT_NAMELESS_BLUELOCALBROADCAST:
    this->SendMsg(MM_BROADCASTMSG, 5, 0, 0, 0);
    break;
  case TT_REQ_NUM_USER:
    this->SendMsg(MM_REQ_NUM_USER, 0, 0, 0, 0);
    break;
  case TT_HOMUN_WAIT:
    this->SendMsg(MM_HOMUN_WAIT, 0, 0, 0, 0);
    break;
  case TT_DISCONNECT_CHARACTER:
    this->SendMsg(MM_DISCONNECT_CHARACTER, 0, 0, 0, 0);
    break;
  case TT_DISCONNECT_ALL_CHARACTER:
    this->SendMsg(MM_DISCONNECT_ALL_CHARACTER, 0, 0, 0, 0);
    break;
  case TT_REQ_BREAK_GUILD:
    this->SendMsg(MM_REQ_DISORGANIZE_GUILD, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_MAKE_GUILD:
    this->SendMsg(MM_REQ_MAKE_GUILD, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_MAKE_GROUP:
    this->SendMsg(MM_MAKE_GROUP, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_SEEK_PARTY:
    this->SendMsg(MM_OPEN_SEEKPARTY_MEMBER, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_RECRUIT:
    this->SendMsg(MM_OPEN_SEEKPARTY_LIST, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_ITEM_CREATE:
    this->SendMsg(MM_ITEM_CREATE, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REMOVE:
    this->SendMsg(MM_REMOVE_TO_AID, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_SHIFT:
    this->SendMsg(MM_SHIFT_TO_GID, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_RECALL_AID:
    this->SendMsg(MM_RECALL_AID, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_RECALL_GID:
    this->SendMsg(MM_RECALL_GID, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_CHECK_ID:
    this->SendMsg(MM_CHECK_ID, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_RC_ID:
    this->SendMsg(MM_RC_ID, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_BILLING_INFO:
    this->SendMsg(MM_BILLING_INFO, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_EXPEL_GROUP_MEMBER:
    this->SendMsg(MM_REQ_EXPEL_GROUP_MEMBER, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_RESETSTATE:
    this->SendMsg(MM_RESET_PARAMETER, 0, 0, 0, 0);
    break;
  case TT_REQ_INVITE_PARTY:
    this->SendMsg(MM_REQ_JOIN_GROUP, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_REQ_INVITE_GUILD:
    this->SendMsg(MM_REQ_JOIN_GUILD2, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_PRISON_USER:
    this->SendMsg(MM_PRISON_USER, reinterpret_cast<int>(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_OPEN_ACCOUNT_LIMITED_TOOL:
    this->SendMsg(MM_OPEN_ACCOUNT_LIMITED_TOOL, 0, 0, 0, 0);
    break;
  case TT_SHOW_EMOTIONLIST: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NEW_EMOTION_LIST)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_BIGTHROB)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_THANKS)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_KEK)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SORRY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SMILE)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_PROFUSELY_SWEAT)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SCRATCH)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_BEST)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_STARE_ABOUT)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_HUK)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_O)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_X)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_HELP)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_GO)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_CRY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_KIK)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_CHUP)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_CHUPCHUP)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_HNG)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_OK)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_STARE)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_HUNGRY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_COOL)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_MERONG)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SHY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_GOODBOY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SPTIME)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SEXY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_COMEON)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_SLEEPY)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_CONGRATULATION)), 0xFFFF64, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EMOTION_HPTIME)), 0xFFFF64, 0, 0, 0);
    break;
  }
  case TT_REQ_RESETSKILL:
    this->SendMsg(MM_RESET_SKILL, std::atoi(parts[0].c_str()), 0, 0, 0);
    break;
  case TT_CHANGE_MAPTYPE: {
    const int third = std::atoi(parts[2].c_str());
    const int second = std::atoi(parts[1].c_str());
    this->SendMsg(MM_REQ_CHANGE_MAPTYPE, std::atoi(parts[0].c_str()), second, third, 0);
    break;
  }
  case TT_HIDE: {
    std::uint32_t flags = g_session.m_jobFlags;
    if (StateFlag02(flags)) {
      ClearMemberFlags(&flags);
    } else {
      SetMemberFlags(&flags);
    }
    this->SendMsg(MM_CHANGE_EFFECTSTATE, 0x40, 0, 0, 0);
    break;
  }
  case TT_COPY_NAME:
    if (IsAdminAccount(g_session.m_aid)) {
      const unsigned char enabled = this->bossAlarmAndCopyNameFlags[1];
      this->bossAlarmAndCopyNameFlags[1] = enabled == 0;
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(enabled == 0 ? "Copy_Name_On!!" : "Copy_Name_Off!!"), 0x64FFFF, 0, 0, 0);
    }
    break;
  case TT_REQ_ACCEPT_INVITE_PARTY:
  case TT_REQ_REFUSE_INVITE_PARTY: {
    TalkConfigPacket packet;
    packet.value = talkType != TT_REQ_ACCEPT_INVITE_PARTY;
    packet.id = ID_CZ_PARTY_CONFIG;
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_REQ_OPEN_EQUIPEDITEM_ONOFF:
    this->SendMsg(MM_OPEN_EQUIPEDITEM_CHANGE, g_session.m_allowEquipView == 0, 0, 0, 0);
    break;
  case TT_SHOW_WEB_ADDRESSBAR: {
    CWebBrowser* browser = g_windowMgr.activeBrowser;
    if (browser) {
      browser->ShowAuxiliary(browser->auxiliaryVisible == 0);
    }
    break;
  }
  case TT_WEB_BROWSER: {
    CWebBrowser* browser = g_windowMgr.EnsureSpecialBrowser(g_renderer->viewport.width, g_renderer->viewport.height);
    if (browser) {
      browser->Navigate(parts[0].c_str());
    }
    break;
  }
  case TT_NAVIGATION: {
    const NaviCell cell = g_navigationManager.ParseCell(parts[1].c_str());
    if (cell.x == -1 || cell.y == -1) {
      g_navigationManager.ShowRouteResult(-100, 1);
      break;
    }
    g_navigationManager.StartNavigation(parts[0], 0, 5, true, cell.x, cell.y, 0x3EA);
    break;
  }
  case TT_NAVIGATION2: {
    const NaviCell cell = g_navigationManager.ParseCell(parts[1].c_str());
    bool first = false;
    bool second = false;
    bool third = false;
    if (cell.x == -1 || cell.y == -1 || !g_navigationManager.ParseFlags(parts[2].c_str(), &first, &second, &third)) {
      g_navigationManager.ShowRouteResult(-100, 1);
      break;
    }
    unsigned char mode = second ? 1 : 0;
    if (first) {
      mode += 2;
    }
    if (third) {
      mode += 4;
    }
    g_navigationManager.StartNavigation(parts[0], 0, mode, true, cell.x, cell.y, 0x3EA);
    break;
  }
  case TT_NAVIGATION3:
    g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
    break;
  case TT_SHOW_BUILD_INFO: {
    char text[1024];
    std::memset(text, 0, sizeof(text));
    sprintf_s(text, sizeof(text), "", "", "");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    sprintf_s(text, sizeof(text), "ver : %d.%d.%d", 14, 2, 3);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    sprintf_s(text, sizeof(text), "s/n : %s", "");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    _snprintf_s(text, _TRUNCATE, "verification : %s", "XXXXXXX");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    break;
  }
  case TT_CLAN_CHAT:
    g_windowMgr.SetChatInputText(parts[0].c_str());
    this->SendMsg(MM_CHAT_CLAN, 0, 0, 0, 0);
    break;
  case TT_SHOW_CASHSHOP_BTN_ON_OFF: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_NCASH_CALLWND);
    if (window) {
      g_serviceOptions.Set(0xDA, !g_serviceOptions.Get(TT_SHOW_CASHSHOP_BTN_ON_OFF));
      window->Show(g_serviceOptions.Get(TT_SHOW_CASHSHOP_BTN_ON_OFF));
    }
    break;
  }
  case TT_SHOW_GOLDPCCAFE_ON_OFF: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_MILEAGEWND);
    if (window) {
      g_serviceOptions.Set(0xDB, !g_serviceOptions.Get(TT_SHOW_GOLDPCCAFE_ON_OFF));
      window->Show(g_serviceOptions.Get(TT_SHOW_GOLDPCCAFE_ON_OFF));
    }
    break;
  }
  case TT_SHOW_ROULETTE_BTN_ON_OFF: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_OPENROULLETTEWND);
    if (window) {
      g_serviceOptions.Set(0xDC, !g_serviceOptions.Get(TT_SHOW_ROULETTE_BTN_ON_OFF));
      window->Show(g_serviceOptions.Get(TT_SHOW_ROULETTE_BTN_ON_OFF));
    }
    break;
  }
  case TT_MACRO_REGISTER:
  case TT_MACRO_DETECTOR: {
    TalkConfigPacket packet;
    packet.value = talkType == TT_MACRO_REGISTER ? 2 : 3;
    packet.id = ID_CZ_UI_OPEN;
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_MACRO_PREVIEW: {
    TalkIntPacket packet;
    packet.id = ID_CZ_REQ_PREVIEW_MACRO_DETECTOR;
    packet.value = std::atoi(parts[0].c_str());
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_VIEW_CAMERAINFO_ON_OFF: {
    TalkCameraPacket packet;
    packet.range = 0.0f;
    packet.rotation = 0.0f;
    packet.latitude = 0.0f;
    packet.id = ID_CZ_VIEW_CAMERAINFO;
    packet.action = 1;
    GetRagConnection().Send(sizeof(packet), &packet);
    break;
  }
  case TT_SET_CAMERA: {
    CView* camera = this->camera;
    if (camera == nullptr) {
      break;
    }
    TalkCameraPacket packet;
    packet.action = 0;
    packet.id = ID_CZ_VIEW_CAMERAINFO;
    packet.range = camera->camera.distance;
    packet.rotation = camera->camera.yaw;
    packet.latitude = camera->camera.pitch;
    if (parts[0].find("range", 0) != std::string::npos) {
      packet.range = static_cast<float>(std::atof(parts[1].c_str()));
    } else if (parts[0].find("rotation", 0) != std::string::npos) {
      packet.rotation = static_cast<float>(std::atof(parts[1].c_str()));
    } else if (parts[0].find("latitude", 0) != std::string::npos) {
      packet.latitude = static_cast<float>(std::atof(parts[1].c_str()));
    } else {
      packet.range = static_cast<float>(std::atof(parts[0].c_str()));
      packet.rotation = static_cast<float>(std::atof(parts[1].c_str()));
      packet.latitude = static_cast<float>(std::atof(parts[2].c_str()));
    }
    GetRagConnection().Send(sizeof(packet), &packet);
    break;
  }
  case TT_RESET_COOLTIME: {
    TalkConfigPacket packet;
    packet.id = ID_CZ_CMD_RESETCOOLTIME;
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_ADVEN_PARTY_BOARD:
    if (!g_windowMgr.CloseWindow(WID_ADVENPARTYBOARDWND)) {
      g_windowMgr.EnsureWindow(WID_ADVENPARTYBOARDWND);
    }
    break;
  case TT_REMOVE_ON:
  case TT_REMOVE_OFF: {
    TalkSelectPacket packet;
    packet.id = ID_CZ_MACRO_CHECK_REMOVE_CHEAT;
    if (talkType == TT_REMOVE_ON) {
      packet.enabled = 1;
      packet.value = std::atoi(parts[0].c_str());
      packet.option = static_cast<unsigned char>(std::atoi(parts[1].c_str()));
    } else {
      packet.enabled = 0;
      packet.value = 0;
    }
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_MACRO_CHECKER_MAP: {
    TalkNamePacket packet;
    packet.id = ID_CZ_GM_CHECKER;
    strncpy_s(packet.name, sizeof(packet.name), parts[0].c_str(), _TRUNCATE);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
    break;
  }
  case TT_PROBABILITY_TEST: {
    this->recordItemPickups = this->recordItemPickups == 0;
    std::string server("Dev");
    if (g_serviceMode == ServerPrimary && server.compare(0, 3, g_launchServerName, std::strlen(g_launchServerName)) != 0) {
      break;
    }
    SYSTEMTIME time;
    GetLocalTime(&time);
    if (this->recordItemPickups == 1) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("확률 로그 시작"), 0x64FFFF, 0, 0, 0);
      std::string name(FormatString("ProbabilityLog(%02d-%02d-%02d).txt", time.wYear, time.wMonth, time.wDay));
      FILE* file = std::fopen(name.c_str(), "a");
      if (file) {
        std::fprintf(file, "시작 : %02d-%02d-%02d %02d:%02d:%02d \n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
                     time.wSecond);
      }
      std::fclose(file);
    } else {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("확률 로그 끝"), 0x64FFFF, 0, 0, 0);
      std::string name(FormatString("ProbabilityLog(%02d-%02d-%02d).txt", time.wYear, time.wMonth, time.wDay));
      FILE* file = std::fopen(name.c_str(), "a");
      if (file) {
        for (const auto& entry : this->itemPickupCounts) {
          int itemId = entry.first;
          int count = entry.second;
          ITEM_INFO item;
          item.SetItemId(itemId);
          item.identified = 1;
          const std::string itemName = item.DisplayName(0);
          std::string line = std::format("ID : {}, {}, {}개 \n", itemId, itemName.c_str(), count);
          std::fprintf(file, line.c_str());
        }
        std::fprintf(file, "끝 : %02d-%02d-%02d %02d:%02d:%02d \n\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
                     time.wSecond);
        std::fclose(file);
      }
      this->itemPickupCounts.clear();
    }
    break;
  }
  case TT_STAT:
  case TT_IS_REDCELL:
  case TT_SIYEON_STATUS:
  case TT_REQ_HUNTING:
  case TT_MINIGAME_ON_OFF:
  case TT_PROCESS_AFFINITY:
  case TT_GPK_DYNCODE_RELOAD:
    break;
  default:
    Singleton<CGameSettingsMgr>::GetInstance()->ToggleOption(talkType);
    break;
  }
}

// OFFSET: 0x00CE3D30
// FUNCTION: fn_21335
void CGameMode::ProcessActorPick(int& handled, ActorPickInfo* pick, CGameActor* actor) {
  const unsigned char selfTargetOption = g_serviceOptions.Get(TT_NOSHIFT_ON_OFF);
  const unsigned char forceOption = g_serviceOptions.Get(TT_NOCTRL_ON_OFF);
  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);

  int skillMode;
  int shiftHeld;
  bool optionalSelf;
  int support;
  int attack;
  if (mode == 2 || mode == 4) {
    skillMode = 1;
    shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    optionalSelf = IsOptionalSelfTargetSkill(this->skillId);
    support = IsSupportSkill(this->skillId);
    attack = shiftHeld;
    if (support) {
      attack = shiftHeld == 0;
    }
  } else {
    skillMode = 0;
    shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    attack = shiftHeld;
    support = 0;
    optionalSelf = false;
  }

  bool resetCursor = false;
  if (pick) {
    if (this->scene->isBattleFieldMode == 0) {
      if (g_session.GetMapProperty07()) {
        if (!g_session.IsPartyMember(actor->render.accountId, true)) {
          attack = 1;
        }
      } else if (g_session.GetMapProperty08()) {
        if (!g_session.IsLocalPlayerAid(actor->GetGdid())) {
          attack = 1;
        }
      }

      CWorld* scene = this->scene;
      if (scene->isSiegeMode != 0) {
        if (actor) {
        GuildInfo* guildInfo = &g_guildInfo;
        bool friendly = true;
        bool known = true;
        switch (actor->objectType) {
        case NPC_HO_TYPE:
        case NPC_MERSOL_TYPE:
        case NPC_ELEMENTAL_TYPE:
        case NPC_ABR_TYPE:
        case NPC_BIONIC_TYPE: {
          CGameActor* summoner = static_cast<CGameActor*>(scene->GetGameActorByAID(actor->emblemVersion));
          if (summoner) {
            if (!g_session.IsLocalPlayerAid(summoner->GetGdid())) {
              friendly = guildInfo->HasRelatedGuild(summoner->GetGdid());
            }
          } else {
            known = this->scene->player->render.accountId == static_cast<std::uint32_t>(actor->emblemVersion);
          }
          break;
        }
        default:
          if (!g_session.IsLocalPlayerAid(actor->GetGdid())) {
            friendly = guildInfo->HasRelatedGuild(actor->GetGdid());
          }
          break;
        }

        if (!friendly) {
          attack = 1;
        } else if (known) {
          attack = 0;
          if (!skillMode) {
            resetCursor = true;
          } else {
            attack = support;
            if ((selfTargetOption || shiftHeld) && optionalSelf == true) {
              attack = 1;
            }
          }
        }
        }
      } else if (pick->healthState != 0 || scene->isPKZone != 0 || scene->isEventPVPMode != 0 ||
                 g_session.m_isSiegeZone != 0) {
        if (!g_session.IsPartyMember(actor->render.accountId, true)) {
          attack = 1;
        }
      }
    } else if (actor && g_session.playerTeam[0] != actor->render.battleTeam) {
      attack = 1;
    }
  }

  if (resetCursor) {
    this->SetCursorAction(0);
  } else if (attack && actor->render.state != 3) {
    this->SetCursorAction(skillMode ? 11 : 5);
    handled = 1;
  } else if (skillMode && actor->render.state != 3) {
    this->SetCursorAction(10);
  } else {
    this->SetCursorAction(0);
  }

  const std::uint32_t& homunAid = g_session.m_homunAid;
  const std::uint32_t& mercenaryAid = g_session.m_mercenaryAid;
  if ((attack || this->scene->isSiegeMode != 0) && actor->render.state != 3 && !skillMode && g_session.homunAlive != 0 && g_mouse.buttons[1] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasActiveHomunculusAI()) {
        AiMessage message = {};
        message.type = 3;
        message.target = actor->render.accountId;
        homun->SetResMsg2AI(&message, 0);
      }
    } else if (g_session.attackHomunEnemy != 0 && g_session.homunEnemyGid == actor->render.accountId) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasActiveHomunculusAI()) {
        AiMessage message = {};
        message.type = 3;
        message.target = g_session.homunEnemyGid;
        homun->SetMsg2AI(&message, 0);
      }
      g_session.attackHomunEnemy = 0;
    } else {
      this->SetCursorAction(11);
      g_session.homunEnemyGid = actor->render.accountId;
      g_session.homunEnemyX = 0;
      g_session.homunEnemyY = 0;
      g_session.moveHomunEnemy = 0;
      g_session.attackHomunEnemy = 1;
    }
    g_session.selectHomunEnemy = 1;
  }

  if (g_session.homunAlive != 0 && g_mouse.buttons[0] == 1 && g_session.IsHomunculusSkill(this->skillId)) {
    CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
    if (homun && homun->HasActiveHomunculusAI()) {
      AiMessage message = {};
      message.type = 7;
      message.level = this->skillUseLevel;
      message.skill = this->skillId;
      message.target = actor->render.accountId;
      homun->SetMsg2AI(&message, 0);
    }
    return;
  }

  if ((attack || this->scene->isSiegeMode != 0) && actor->render.state != 3 && !skillMode && g_session.mercenaryAlive != 0 && g_mouse.buttons[0] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasActiveMercenaryAI()) {
        AiMessage message = {};
        message.type = 3;
        message.target = actor->render.accountId;
        mercenary->SetResMsg2AI(&message, 1);
      }
    } else if (g_session.attackMercenaryEnemy != 0 && g_session.mercenaryEnemyGid == actor->render.accountId) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasActiveMercenaryAI()) {
        AiMessage message = {};
        message.type = 3;
        message.target = g_session.mercenaryEnemyGid;
        mercenary->SetMsg2AI(&message, 1);
      }
      g_session.attackMercenaryEnemy = 0;
    } else {
      this->SetCursorAction(11);
      g_session.mercenaryEnemyGid = actor->render.accountId;
      g_session.mercenaryEnemyX = 0;
      g_session.mercenaryEnemyY = 0;
      g_session.moveMercenaryEnemy = 0;
      g_session.attackMercenaryEnemy = 1;
    }
    g_session.selectMercenaryEnemy = 1;
  }

  if (attack != 0 && g_session.mercenaryAlive != 0 && g_mouse.buttons[0] == 1 && g_session.IsMercenarySkill(this->skillId)) {
    CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
    if (mercenary && mercenary->HasActiveMercenaryAI()) {
      AiMessage message = {};
      message.type = 7;
      message.level = this->skillUseLevel;
      message.skill = this->skillId;
      message.target = actor->render.accountId;
      mercenary->SetMsg2AI(&message, 1);
    }
    return;
  }

  int& forced = this->forcedAttack;
  if (g_mouse.buttons[0] == 1 && attack != 0 && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
    this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
    this->lastLockOnMonGid = actor->render.accountId;
    this->isAutoMoveClickOn = 0;
    if (g_session.GetSiegeMode() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
      this->RequestPlayerAction(actor->render.accountId, 0);
      if (!skillMode) {
        forced = 1;
      }
    } else {
      this->RequestPlayerAction(actor->render.accountId, 1);
      forced = 0;
    }
  }

  if (g_mouse.buttons[1] == 1 && shiftHeld) {
    if (this->IsOverWeight(true)) {
      return;
    }
    this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
    g_hoverActorGid = actor->GetPkState();
    this->isAutoMoveClickOn = 0;
  }

  int& balloonAid = this->lastPcGid;
  if (this->oldMouseX == g_mouse.x && this->oldMouseY == g_mouse.y) {
    this->ShowActorNameBalloon(balloonAid);
    return;
  }
  balloonAid = 0;
  this->ShowActorNameBalloon(0);
}

// OFFSET: 0x00CFE510
// FUNCTION: fn_21434
void CGameMode::ProcessStatusIconTooltip(int x, int y) {
  auto firstLine = [](UIWindow* window) -> std::string {
    const std::vector<std::string>& lines = static_cast<UIBalloonText*>(window)->m_lines;
    if (!lines.empty()) {
      return lines[0];
    }
    return std::string();
  };

  if (this->statusIconTooltip) {
    this->statusIconTooltip->Show(0);
  }

  if (g_activeSkillEffects.begin() == g_activeSkillEffects.end()) {
    return;
  }

  int& hoveredId = this->hoveredStatusIconId;
  const int count = static_cast<int>(g_activeSkillEffects.end() - g_activeSkillEffects.begin());
  const int rows = (g_renderer->viewport.height - 169) / 35;
  const int right = g_renderer->viewport.width - 18;
  const int columns = count / rows + (count % rows != 0 ? 1 : 0);
  if (right - columns * 45 + 15 > x || x > right || y < 168 || y > rows * 35 + 168) {
    hoveredId = -1;
    return;
  }

  const int dx = right - x;
  const int column = dx / 45;
  const int columnEdge = (column + (dx != column * 45 ? 1 : 0)) * 45;
  if (columnEdge - 15 < dx && dx < columnEdge) {
    return;
  }

  const int dy = y - 168;
  const int row = dy / 35;
  const int rowEdge = (row + (dy != row * 35 ? 1 : 0)) * 35;
  if (rowEdge - 5 < dy && dy < rowEdge) {
    return;
  }

  const int slot = column * rows + row;
  if (slot >= count) {
    return;
  }

  const int id = g_session.m_statusIconIds[slot];
  if (id == -1) {
    hoveredId = -1;
    if (this->statusIconTooltip) {
      static_cast<UIBalloonText*>(this->statusIconTooltip)->SetText(" ", 35);
    }
    return;
  }

  int hasLimit = 0;
  int limitLine = -1;
  g_session.m_hotkeyScript->Call("GetTimeLimitInfo", "d>dd", id, &hasLimit, &limitLine);

  std::string timeText;
  int activeId = id;
  if (hasLimit != 0) {
    timeText = this->GetActiveSkillEffectTimeText(id, &activeId);
  }

  if (hoveredId == id && this->statusIconTooltip && id != 0x175 && id != 0x2B7) {
    if (firstLine(this->statusIconTooltip).size() != 0) {
      this->statusIconTooltip->Show(1);
    }
  } else {
    if (!this->statusIconTooltip) {
      this->statusIconTooltip = new UITransBalloonText();
      this->statusIconTooltip->SetSize(50, 50);
      g_windowMgr.AddRendered(this->statusIconTooltip);
      static_cast<UITransBalloonText*>(this->statusIconTooltip)->SetFontHeight(11);
      static_cast<UITransBalloonText*>(this->statusIconTooltip)->m_borderColor = 0xC0C0C0;
    } else {
      static_cast<UIBalloonText*>(this->statusIconTooltip)->SetText(" ", 35);
    }

    UITransBalloonText* balloon = static_cast<UITransBalloonText*>(this->statusIconTooltip);
    std::string extra;
    int line = 1;
    std::uint32_t color = 0;
    if (id != 0x2B7) {
      for (;;) {
        const char* text;
        int red;
        int green;
        int blue;
        do {
          text = nullptr;
          red = 0;
          green = 0;
          blue = 0;
        } while (!g_session.m_hotkeyScript->Call("GetStateIconDescript", "dd>sddd", activeId, line, &text, &red, &green, &blue));

        std::string description(text);
        if (description.size() == 0) {
          break;
        }

        if (line == limitLine) {
          description = static_cast<const std::string&>(FormatOwnedString(description.c_str(), timeText.c_str()));
        }

        const std::uint32_t lineColor = RGB(red, green, blue);
        bool replace;
        if (line == 1) {
          replace = true;
        } else {
          const std::string first = firstLine(balloon);
          replace = first.size() == 1 && first[0] == ' ';
        }

        if (replace) {
          balloon->SetLine(description.c_str(), lineColor);
        } else {
          balloon->AddLine(description.c_str(), lineColor);
        }
        ++line;
      }

      if (id == 0x175) {
        if (timeGetTime() - g_session.nameValidationTick > 10000) {
          g_session.minimumNameLength = 0;
        } else if (g_session.minimumNameLength != 0) {
          extra = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_MUST_ID_EXCEED_CHAR), g_session.minimumNameLength));
        }

        if (extra.size() != 0) {
          balloon->AddLine(extra.c_str(), 0);
        }
      }
    } else {
      {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.equippedArrowIndex);
        if (item.identity[1] != 0) {
          extra = static_cast<const std::string&>(FormatOwnedString("%s : %d", item.DisplayName(0).c_str(), item.quantity));
          color = 0xFABEBE;
        }
      }

      if (extra.size() != 0) {
        balloon->SetLine(extra.c_str(), color);
      }
    }

    const std::string first = firstLine(this->statusIconTooltip);
    if (!(first.size() == 1 && first[0] == ' ')) {
      this->statusIconTooltip->Show(1);
      const int windowHeight = this->statusIconTooltip->geometry.height;
      const int left = right - 30 + (slot / rows) * -45 - this->statusIconTooltip->geometry.width;
      const int top = (slot % rows) * 35;
      int windowY = g_renderer->viewport.height - windowHeight;
      if (windowHeight + 171 + top <= g_renderer->viewport.height) {
        windowY = top + 171;
      }
      this->statusIconTooltip->Move(left, windowY);
    }

    hoveredId = id;
  }

  if (hasLimit != 0 && timeText.size() != 0 && limitLine != -1 && this->statusIconTooltip) {
    const char* text = nullptr;
    if (g_session.m_hotkeyScript->Call("GetStateIconDescript", "dd>s", activeId, limitLine, &text)) {
      std::string description(text);
      description = static_cast<const std::string&>(FormatOwnedString(description.c_str(), timeText.c_str()));
      static_cast<UITransBalloonText*>(this->statusIconTooltip)->ChangeText(description.c_str(), limitLine - 1);
    }
  }
}

// OFFSET: 0x00CE2880
// FUNCTION: fn_21334
int CGameMode::HandleActorPick(ActorPickInfo* pick, int result) {
  const unsigned char selfTargetOption = g_serviceOptions.Get(TT_NOSHIFT_ON_OFF);
  const unsigned char forceOption = g_serviceOptions.Get(TT_NOCTRL_ON_OFF);
  const int& siegeMode = g_session.m_isSiegeZone;
  const std::uint32_t& homunAid = g_session.m_homunAid;
  const std::uint32_t& mercenaryAid = g_session.m_mercenaryAid;
  GuildInfo* guildInfo = &g_guildInfo;

  if (pick->accountId != g_session.m_aid) {
    switch (pick->kind) {
    case 0:
      if (pick->job != JT_WARPNPC && g_session.IsAllyActor(pick->accountId, pick->job) == 1) {
        this->SetCursorAction(1);
        if (g_mouse.buttons[0] == 2) {
          result = 2;
        } else {
          result = 1;
        }
      } else if (pick->job != JT_WARPNPC) {
        result = 1;
        if (IsPlayerJob(pick->job)) {
          const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
          int skillMode;
          int shift;
          int support;
          int attack;
          if (mode == 2 || mode == 4) {
            skillMode = 1;
            shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
            support = IsSupportSkill(this->skillId);
            attack = shift;
            if (support) {
              skillMode = 1;
              attack = shift == 0;
            }
          } else {
            skillMode = 0;
            shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
            attack = shift;
            support = 0;
          }

          CWorld* scene = this->scene;
          CGameActor* actor = static_cast<CGameActor*>(scene->GetGameActorByAID(pick->accountId));
          if (scene->isBattleFieldMode == 0) {
            if (g_session.GetMapProperty07()) {
              if (!g_session.IsPartyMember(pick->accountId, true)) {
                attack = 1;
              }
            } else if (g_session.GetMapProperty08() && actor != nullptr) {
              if (!g_session.IsLocalPlayerAid(actor->GetGdid())) {
                attack = 1;
              }
            }

            if (this->scene->isSiegeMode != 0) {
              if (actor != nullptr) {
                if (!g_session.IsLocalPlayerAid(actor->GetGdid()) && !guildInfo->HasRelatedGuild(actor->GetGdid())) {
                  attack = 1;
                } else {
                  attack = 0;
                  if (skillMode) {
                    const bool optionalSelf = IsOptionalSelfTargetSkill(this->skillId);
                    attack = support;
                    if ((selfTargetOption != 0 || shift != 0) && optionalSelf == true) {
                      attack = 1;
                    }
                  }
                }
              }
            } else if (pick->healthState != 0 || this->scene->isPKZone != 0 || this->scene->isEventPVPMode != 0 || siegeMode != 0) {
              if (!g_session.IsPartyMember(pick->accountId, true)) {
                attack = 1;
              }
            }
          } else if (actor != nullptr && g_session.playerTeam[0] != actor->render.battleTeam) {
            attack = 1;
          }

          const int state = actor != nullptr ? actor->render.state : 3;
          if (attack && state != 3) {
            this->SetCursorAction(skillMode ? 11 : 5);
            result = 1;
            this->lastPcGid = pick->accountId;
          } else {
            this->SetCursorAction(skillMode && state != 3 ? 10 : 0);
            result = 0;
            if (g_mouse.buttons[0] == 0 && this->lockOnMouse == 0 && this->lastMonGid == 0) {
              this->lastPcGid = pick->accountId;
            }
          }
        } else if (g_session.IsNpcJob(pick->job) == true) {
          this->SetCursorAction(1);
          if (g_mouse.buttons[0] == 2) {
            result = 2;
          }
        } else if (g_session.IsMonsterClass(pick->job)) {
          const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
          int skillMode;
          int attack;
          if (mode == 2 || mode == 4) {
            skillMode = 1;
            const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
            attack = shift == 0;
            if (IsSupportSkill(this->skillId)) {
              attack = shift;
              skillMode = 1;
            }
          } else {
            skillMode = 0;
            attack = HIBYTE(GetAsyncKeyState(VK_SHIFT)) == 0;
          }
          if (selfTargetOption != 0) {
            attack = 1;
          }

          CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
          if (g_session.GetMapProperty08() && actor != nullptr && g_session.IsLocalPlayerAid(actor->GetGdid())) {
            attack = 0;
          }
          if (this->scene->isSiegeMode != 0 && actor != nullptr && g_session.IsLocalPlayerAid(actor->GetGdid())) {
            attack = 0;
          }

          if (this->isAutoMoveClickOn != 0) {
            if (attack) {
              if (skillMode) {
                this->SetCursorAction(11);
              } else if (actor != nullptr && actor->objectType == NPC_PET_TYPE) {
                this->SetCursorAction(0);
              } else {
                this->SetCursorAction(5);
              }
            } else {
              this->SetCursorAction(skillMode ? 10 : 0);
            }
          }

          if (g_mouse.buttons[0] == 0 && this->lockOnMouse == 0) {
            this->lastMonGid = pick->accountId;
          }
          if (g_mouse.buttons[0] == 2 || g_mouse.buttons[1] == 2) {
            result = 2;
          }
        }
      } else {
        this->SetCursorAction(7);
        result = 2;
      }
      break;
    case 2: {
      result = 1;
      int action;
      if (g_session.GetSiegeMode()) {
        switch (pick->job) {
        case UNT_ICEWALL:
        case UNT_BLASTMINE:
        case UNT_SKIDTRAP:
        case UNT_ANKLESNARE:
        case UNT_LANDMINE:
        case UNT_SHOCKWAVE:
        case UNT_SANDMAN:
        case UNT_FLASHER:
        case UNT_FREEZINGTRAP:
        case UNT_CLAYMORETRAP:
        case UNT_TALKIEBOX:
        case UNT_GROUNDDRIFT_WIND:
        case UNT_GROUNDDRIFT_DARK:
        case UNT_GROUNDDRIFT_POISON:
        case UNT_GROUNDDRIFT_WATER:
        case UNT_GROUNDDRIFT_FIRE:
        case UNT_EARTHSTRAIN:
        case UNT_MAGENTATRAP:
        case UNT_COBALTTRAP:
        case UNT_MAIZETRAP:
        case UNT_VERDURETRAP:
        case UNT_FIRINGTRAP:
        case UNT_ICEBOUNDTRAP:
        case UNT_ELECTRICSHOCKER:
        case UNT_CLUSTERBOMB:
        case UNT_REVERBERATION:
        case UNT_NETHERWORLD:
        case UNT_THORNS_TRAP:
        case UNT_WALLOFTHORN:
        case UNT_DEMONIC_FIRE:
        case UNT_FIRE_EXPANSION_SMOKE_POWDER:
        case UNT_FIRE_EXPANSION_TEAR_GAS:
        case UNT_POWER_OF_GAIA:
        case UNT_GROUNDDRIFT_NEUTRAL:
        case UNT_DEEPBLINDTRAP:
        case UNT_SOLIDTRAP:
        case UNT_SWIFTTRAP:
        case UNT_FLAMETRAP:
          action = reinterpret_cast<std::intptr_t>(this->skillUseType) == 5 ? 10 : 5;
          break;
        default:
          action = reinterpret_cast<std::intptr_t>(this->skillUseType) == 5 ? 10 : 0;
          break;
        }
      } else {
        switch (pick->job) {
        case UNT_ICEWALL:
        case UNT_BLASTMINE:
        case UNT_CLAYMORETRAP:
        case UNT_REVERBERATION:
        case UNT_NETHERWORLD:
        case UNT_WALLOFTHORN:
        case UNT_DEEPBLINDTRAP:
        case UNT_SOLIDTRAP:
        case UNT_SWIFTTRAP:
        case UNT_FLAMETRAP:
          action = reinterpret_cast<std::intptr_t>(this->skillUseType) == 5 ? 10 : 5;
          break;
        default:
          action = reinterpret_cast<std::intptr_t>(this->skillUseType) == 5 ? 10 : 0;
          break;
        }
      }
      this->SetCursorAction(action);
      if (g_mouse.buttons[0] == 2) {
        result = 2;
      }
      break;
    }
    case 1:
      result = 1;
      this->SetCursorAction(9);
      if (g_mouse.buttons[0] == 2) {
        result = 2;
      }
      break;
    case 3:
      result = 0;
      this->SetCursorAction(0);
      break;
    case 4: {
      result = 0;
      this->SetCursorAction(0);
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
      if (actor != nullptr && actor->render.state != 3) {
        if (g_session.m_aid == static_cast<std::uint32_t>(actor->emblemVersion)) {
          if (actor->objectType == NPC_HO_TYPE && this->skillId == AM_POTIONPITCHER) {
            result = 1;
            this->SetCursorAction(11);
            this->lastMonGid = pick->accountId;
          }
        } else {
          CWorld* scene = this->scene;
          if (scene->isPKZone == 1 || siegeMode == 1 || scene->isSiegeMode == 1) {
            if (scene->isSiegeMode == 1) {
              CGameActor* owner = static_cast<CGameActor*>(scene->GetGameActorByAID(actor->emblemVersion));
              if (owner == nullptr || g_session.IsLocalPlayerAid(owner->GetGdid()) == 1 || guildInfo->HasRelatedGuild(owner->GetGdid()) == true) {
                break;
              }
            }
            result = 1;
            this->SetCursorAction(5);
            this->lastMonGid = pick->accountId;
          }
        }
      }
      break;
    }
    }
  }

  if (g_mouse.buttons[0] != 1 && g_mouse.buttons[1] != 1) {
    return result;
  }

  switch (pick->kind) {
  case 0: {
    if (pick->job == JT_WARPNPC) {
      return result;
    }

    if (g_session.IsAllyActor(pick->accountId, pick->job) == 1) {
      const int state = this->scene->player->render.state;
      if (state != 0 && state != 6) {
        return result;
      }
      if (this->IsMenuWindowOpen() == true) {
        return result;
      }

      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
      int column;
      int row;
      this->WorldCell(actor->render.position.x, actor->render.position.z, column, row);
      this->scene->player->SendMsg(nullptr, AM_REQ_CHANGE_DIR, column, row, 0, 0, 0, 0);
      if (g_windowMgr.HasPendingDialog() == true) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_VIEW_CHANGE_EQUIPMENT_PROPERTIES_MSG)), 255, 25, 0, 0);
        return result;
      }

      ContactNpcPacket packet;
      packet.packetType = ID_CZ_CONTACTNPC;
      packet.npcId = pick->accountId;
      packet.type = 1;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
      return result;
    }

    const int job = pick->job;
    if (g_session.IsMonsterClass(job)) {
      const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
      int attack;
      if (mode == 2 || mode == 4) {
        const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
        const bool support = IsSupportSkill(this->skillId);
        attack = shift == 0;
        if (support) {
          attack = shift;
        }
      } else {
        attack = HIBYTE(GetAsyncKeyState(VK_SHIFT)) == 0;
      }
      if (selfTargetOption != 0) {
        attack = 1;
      }

      if (g_session.homunAlive && g_mouse.buttons[1] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
        if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
          CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
          if (homun != nullptr && homun->HasActiveHomunculusAI()) {
            AiMessage message = {};
            message.type = 3;
            message.target = pick->accountId;
            homun->SetResMsg2AI(&message, 0);
          }
        } else if (g_session.attackHomunEnemy != 0 && g_session.homunEnemyGid == pick->accountId) {
          CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
          if (homun != nullptr && homun->HasActiveHomunculusAI()) {
            AiMessage message = {};
            message.type = 3;
            message.target = g_session.homunEnemyGid;
            homun->SetMsg2AI(&message, 0);
          }
          g_session.attackHomunEnemy = 0;
        } else {
          this->SetCursorAction(11);
          g_session.homunEnemyGid = pick->accountId;
          g_session.homunEnemyX = 0;
          g_session.homunEnemyY = 0;
          g_session.moveHomunEnemy = 0;
          g_session.attackHomunEnemy = 1;
        }
        g_session.selectHomunEnemy = 1;
      }

      if (g_session.homunAlive && g_mouse.buttons[0] == 1 && g_session.IsHomunculusSkill(this->skillId)) {
        CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (homun != nullptr && homun->HasActiveHomunculusAI()) {
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->skillUseLevel;
          message.skill = this->skillId;
          message.target = pick->accountId;
          homun->SetMsg2AI(&message, 0);
        }
        return result;
      }

      if (g_session.mercenaryAlive && g_mouse.buttons[0] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
        if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
          CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
          if (mercenary != nullptr && mercenary->HasActiveMercenaryAI()) {
            AiMessage message = {};
            message.type = 3;
            message.target = pick->accountId;
            mercenary->SetResMsg2AI(&message, 1);
          }
        } else if (g_session.attackMercenaryEnemy != 0 && g_session.mercenaryEnemyGid == pick->accountId) {
          CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
          if (mercenary != nullptr && mercenary->HasActiveMercenaryAI()) {
            AiMessage message = {};
            message.type = 3;
            message.target = g_session.mercenaryEnemyGid;
            mercenary->SetMsg2AI(&message, 1);
          }
          g_session.attackMercenaryEnemy = 0;
        } else {
          this->SetCursorAction(11);
          g_session.mercenaryEnemyGid = pick->accountId;
          g_session.mercenaryEnemyX = 0;
          g_session.mercenaryEnemyY = 0;
          g_session.moveMercenaryEnemy = 0;
          g_session.attackMercenaryEnemy = 1;
        }
        g_session.selectMercenaryEnemy = 1;
      }

      if (attack && g_session.mercenaryAlive && g_mouse.buttons[0] == 1 && g_session.IsMercenarySkill(this->skillId)) {
        CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
        if (mercenary != nullptr && mercenary->HasActiveMercenaryAI()) {
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->skillUseLevel;
          message.skill = this->skillId;
          message.target = pick->accountId;
          mercenary->SetMsg2AI(&message, 1);
        }
        return result;
      }

      if (g_mouse.buttons[0] == 1 && attack && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
        this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
        this->lastLockOnMonGid = pick->accountId;
        this->lastMonGid = pick->accountId;
        this->isAutoMoveClickOn = 0;
        if (g_session.GetSiegeMode() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
          this->RequestPlayerAction(pick->accountId, 0);
          this->forcedAttack = 1;
        } else {
          this->RequestPlayerAction(pick->accountId, 1);
          this->forcedAttack = 0;
        }
        return result;
      }

      if (this->forcedAttack == 0 && g_mouse.buttons[0] == 3) {
        this->lastMonGid = 0;
      }
      return result;
    }

    if (!IsPlayerJob(job)) {
      break;
    }

    const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
    int attack;
    if (mode == 2 || mode == 4) {
      const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
      attack = shift;
      if (IsSupportSkill(this->skillId)) {
        attack = shift == 0;
      }
    } else {
      attack = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    }
    if (pick->healthState != 0 || this->scene->isPKZone != 0 || siegeMode != 0) {
      attack = 1;
    }
    if (selfTargetOption != 0) {
      attack = 1;
    }

    if (attack && g_mouse.buttons[0] == 1) {
      int index = 0;
      CGameActor* companion;
      if (g_session.IsHomunculusSkill(this->skillId) && g_session.homunAlive) {
        companion = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (companion != nullptr && companion->HasActiveHomunculusAI()) {
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->skillUseLevel;
          message.skill = this->skillId;
          message.target = pick->accountId;
          companion->SetMsg2AI(&message, index);
          return result;
        }
      } else if (g_session.IsMercenarySkill(this->skillId) && g_session.mercenaryAlive) {
        companion = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
        if (companion != nullptr && companion->HasActiveMercenaryAI()) {
          index = 1;
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->skillUseLevel;
          message.skill = this->skillId;
          message.target = pick->accountId;
          companion->SetMsg2AI(&message, index);
          return result;
        }
      }
    }

    const bool allowed = pick->accountId != g_session.m_aid || mode == 4;
    if (!allowed || !attack) {
      return result;
    }

    this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
    this->lastLockOnMonGid = pick->accountId;
    this->lastMonGid = pick->accountId;
    this->isAutoMoveClickOn = 0;
    if (g_session.GetSiegeMode() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
      this->RequestPlayerAction(pick->accountId, 0);
      this->forcedAttack = 1;
      return result;
    }
    this->RequestPlayerAction(pick->accountId, 1);
    this->forcedAttack = 0;
    return result;
  }
  case 2:
    switch (pick->job) {
    case UNT_ICEWALL:
    case UNT_BLASTMINE:
    case UNT_CLAYMORETRAP:
    case UNT_REVERBERATION:
    case UNT_NETHERWORLD:
    case UNT_WALLOFTHORN:
    case UNT_DEEPBLINDTRAP:
    case UNT_SOLIDTRAP:
    case UNT_SWIFTTRAP:
    case UNT_FLAMETRAP: {
      const int skill = this->skillId;
      if (skill == 0xD7 || skill == 0xD8 || skill == 0xD9 || skill == 0xDA) {
        return result;
      }

      this->SendMsg(MM_SETLOCKONMOUSE, 1, 0, 0, 0);
      this->isAutoMoveClickOn = 0;
      const int skillUse = reinterpret_cast<int>(&this->skillUseType);
      if (!HIBYTE(GetAsyncKeyState(VK_CONTROL)) && forceOption == 0) {
        this->scene->player->SendMsg(nullptr, AM_PROCEEDTOUCHSKILL, pick->accountId, skillUse, 1, 0, 0, 0);
        this->forcedAttack = 0;
      } else {
        this->scene->player->SendMsg(nullptr, AM_PROCEEDTOUCHSKILL, pick->accountId, skillUse, 0, 0, 0, 0);
        this->forcedAttack = 1;
      }
      return result;
    }
    default:
      if (reinterpret_cast<std::intptr_t>(this->skillUseType) == 5) {
        const long long skillUse = static_cast<long long>(reinterpret_cast<int>(this)) + offsetof(CGameMode, skillUseType);
        this->scene->player->SendMsg(nullptr, AM_PROCEEDTOUCHSKILL, pick->accountId, skillUse, 0, 0, 0, 0);
      }
      return result;
    }
  case 3:
    this->SetCursorAction(0);
    return 0;
  case 1:
    if (g_mouse.buttons[0] == 1 && reinterpret_cast<std::intptr_t>(this->skillUseType) == 0) {
      this->scene->player->SendMsg(nullptr, AM_PROCEEDPICKUPITEM, pick->accountId, 0, 0, 0, 0, 0);
    }
    break;
  case 4: {
    this->SetCursorAction(0);
    if (g_session.IsMercenaryJob(pick->job)) {
      CWorld* scene = this->scene;
      if (this->skillId == AM_POTIONPITCHER || ((scene->isPKZone != 0 || scene->isSiegeMode != 0 || siegeMode != 0) && !g_session.IsHomunAid(pick->accountId))) {
        if ((this->scene->isPKZone != 0 || this->scene->isSiegeMode != 0 || siegeMode != 0) && !g_session.IsHomunAid(pick->accountId)) {
          this->SetCursorAction(5);
        }
        this->RequestPlayerAction(pick->accountId, 1);
        this->forcedAttack = 0;
        return 0;
      }
      break;
    }

    const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->skillUseType);
    int attack;
    if (mode == 2 || mode == 4) {
      const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
      attack = shift;
      if (IsSupportSkill(this->skillId)) {
        attack = shift == 0;
      }
    } else {
      attack = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    }
    if (pick->healthState != 0 || this->scene->isPKZone != 0 || this->scene->isSiegeMode != 0 || siegeMode != 0) {
      attack = 1;
    }

    const bool allowed = pick->accountId != mercenaryAid || mode == 4;
    if (allowed && attack) {
      this->RequestPlayerAction(pick->accountId, 1);
    }
    return 0;
  }
  }

  return result;
}
