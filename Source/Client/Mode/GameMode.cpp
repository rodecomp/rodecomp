#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Contents/Actors/Npc.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/DummyEffectLauncher.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Actors/MousePointer.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"
#include "UI/UIBroadcastBalloon.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Contents/Actors/GrannyGameActor.h"
#include "Utility/Text/FormatString.h"
#include "Client/Application/Launcher.h"
#include "UI/Text/NavigationLink.h"
#include "Contents/RenewQuestMgr.h"
#include "Client/Mode/ModeCursor.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "Contents/MapInfoMgr.h"
#include <timeapi.h>
#include "File/File.h"
#pragma pack(push, 1)
struct GameModeTextRecord {
  /* 0000 */ std::uint32_t header;
  /* 0004 */ std::int16_t length;
  /* 0006 */ int firstLength;
  /* 000A */ int secondLength;
  /* 000E */ int unknown_000E;
  /* 0012 */ int unknown_0012;
  /* 0016 */ int unknown_0016;
  /* 001A */ int unknown_001A;
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

static_assert(sizeof(GameModeReplayPacket) == 0xFE);

static_assert(sizeof(GameModeTextRecord) == 0x190);
static_assert(offsetof(GameModeTextRecord, buffer) == 0x1E);

// GLOBAL: gl_24661
std::vector<ActiveSkillEffect> g_activeSkillEffects;

// OFFSET: 0x00CCF480
// FUNCTION: fn_21242
void CGameMode::PushUnknown01A4(int value) {
  this->unknown_01A4.push_back(value);
}

// OFFSET: 0x00CCF530
// FUNCTION: fn_21243
bool CGameMode::IsOverWeight(bool notify) {
  int maxWeight = g_session.m_maxWeight;
  if (maxWeight < 1) {
    maxWeight = 1;
  }

  if (g_session.m_weight * 100 / maxWeight < 90) {
    return false;
  }

  if (notify) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(307)), 255, 0, 0, 0);
  }

  return true;
}

// OFFSET: 0x00CCFCF0
// FUNCTION: fn_21247
void CGameMode::CompleteUnknown0194(std::uint32_t id, const std::string& text) {
  auto it = this->unknown_0194.find(id);
  if (it != this->unknown_0194.end()) {
    this->SendMsg(140, reinterpret_cast<int>(text.c_str()), it->second, 0, 0);
    this->unknown_0194.erase(it);
  }
}

// OFFSET: 0x00CD52F0
// FUNCTION: fn_21279
int CGameMode::GetSkillAttackRange(int id, int level) {
  if (id == 10000) {
    return 5;
  }

  int range = 1;
  CSkillInfo skill = reinterpret_cast<const ClientSkills&>(g_session).Lookup(id);
  if (!skill.available) {
    skill = g_session.m_skillBundle.Pending(id);
    if (!skill.available) {
      return range;
    }

    level = skill.level;
  }

  int reduction = 0;
  g_luaScript->Call("GetSkillAttackRange", "ddd>dd", id, level, skill.level, &range, &reduction);
  range += skill.unknown_001C - reduction;
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
  const auto* player = reinterpret_cast<const CRenderObject*>(this->scene->player);
  this->WorldCell(player->render.position.x, player->render.position.z, column, row);

  std::string name = this->mapName;
  if (name.size() > 4) {
    name.resize(name.size() - 4);
  }

  std::sprintf(text, "Location: %s ( %d , %d )", name.c_str(), column, row);
  return text;
}

// OFFSET: 0x00CD5800
// FUNCTION: fn_21284
bool CGameMode::HasInventoryItem(int id) {
  const ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, id, 0);
  return item.unknown_0000[1] != 0 && item.quantity > 0;
}

// OFFSET: 0x00CD58B0
// FUNCTION: fn_21285
bool CGameMode::HasResurrectionItem() {
  ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, 7621, 0);
  const CWorld* world = this->scene;
  if (world->unknown_0048 || world->unknown_004C || world->unknown_0054) {
    return false;
  }

  if (item.unknown_0000[1] != 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 6833, 0);
  if (item.unknown_0000[1] != 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 6316, 0);
  if (item.unknown_0000[1] != 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 6293, 0);
  if (item.unknown_0000[1] != 0 && item.quantity > 0) {
    return true;
  }

  item = LookupClientItem(g_session.m_inventoryItems, 25310, 0);
  if (item.unknown_0000[1] != 0 && item.quantity > 0) {
    return true;
  }

  return false;
}

// OFFSET: 0x00CD5CD0
// FUNCTION: fn_21287
void CGameMode::SendMousePointerState(int value) {
  if (this->mousePointer) {
    this->mousePointer->SendMsg(nullptr, 2, 0, value, value >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CECE00
// FUNCTION: fn_21360
void CGameMode::ClearUnknown0458() {
  *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0x458) = 0;
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
void CGameMode::UpdateUnknown0628(int id, bool add) {
  if (add == true) {
    this->unknown_0628.insert(id);
  } else {
    this->unknown_0628.erase(id);
  }
}

// OFFSET: 0x00CEFFC0

// OFFSET: 0x00CF0070
// FUNCTION: fn_21410
const char* CGameMode::Unknown04DCText() {
  return reinterpret_cast<std::string*>(reinterpret_cast<char*>(this) + 0x4DC)->c_str();
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
std::string CGameMode::GetUnknown0084(unsigned int index) {
  if (!this->unknown_0084.empty() && index <= this->unknown_0084.size()) {
    return this->unknown_0084[index];
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
    const std::uint32_t expiration = effect.unknown_0004;
    if (expiration == 999999 || expiration == 0) {
      return output;
    }

    std::uint32_t remainingSeconds = 0;
    if (g_replayAnimationScaling == 0) {
      remainingSeconds = (expiration - timeGetTime()) / 1000;
    } else {
      const int remainingMilliseconds = static_cast<int>(expiration) - static_cast<int>(Replay::Instance().elapsed);
      if (remainingMilliseconds >= 1000) {
        remainingSeconds = static_cast<std::uint32_t>(remainingMilliseconds) / 1000;
      }
    }

    const int signedSeconds = static_cast<int>(remainingSeconds);
    const int hours = signedSeconds / 3600;
    if (hours != 0) {
      output.append(FormatOwnedString("%d %s", hours, LookupMessage(0x6A1)));
    }

    const int remainder = signedSeconds - hours * 3600;
    const int minutes = remainder / 60;
    const int seconds = remainder % 60;
    if (minutes != 0) {
      output.append(FormatOwnedString(" %d %s", minutes, LookupMessage(0x70F)));
    }
    if (seconds != 0) {
      output.append(FormatOwnedString(" %d %s", seconds, LookupMessage(0x710)));
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
bool CGameMode::IsExcludedSkillEffectId(std::uint32_t id) {
  return id == 0x28 || id == 0x2A;
}

// OFFSET: 0x00D07740
// FUNCTION: fn_21583
int CGameMode::SubD07740(int id, int defaultResult) {
  if (id < 0xFB5) {
    if (id == 0xFB4 || id == 0x13) {
      return 1;
    }

    if (id == 0x14) {
      return 0;
    }
  } else {
    switch (id) {
    case 0xFB5:
    case 0xFCB:
    case 0xFE5:
    case 0xFEC:
    case 0x1009:
    case 0x10A8:
      return 0;
    case 0xFCA:
    case 0xFE4:
    case 0xFEB:
    case 0x1008:
    case 0x10A7:
      return 1;
    }
  }

  return defaultResult;
}

// OFFSET: 0x00CEB8E0
// FUNCTION: fn_21352
void CGameMode::sub_CEB8E0() {
  if (Replay::Instance().state != 2) {
    return;
  }

  GameModeTextRecord record = {};
  record.header = 0x038808AE;
  record.firstLength = static_cast<int>(this->unknown_03C8.size());
  record.secondLength = static_cast<int>(this->unknown_03E0.size());
  record.unknown_000E = this->unknown_03F8;
  record.unknown_0012 = this->unknown_03FC;
  record.unknown_0016 = this->unknown_0400;
  record.unknown_001A = this->unknown_0404;

  std::memcpy(record.buffer, this->unknown_03C8.data(), record.firstLength);
  std::memcpy(record.buffer + record.firstLength, this->unknown_03E0.data(), record.secondLength);
  record.length = static_cast<std::int16_t>(offsetof(GameModeTextRecord, buffer) + record.firstLength + record.secondLength);

  Replay& replay = Replay::Instance();
  ReplayPacketRecorder recorder = {
    g_packetLengths,
    replay.baseFrame,
    replay.started,
    replay.frame,
    replay.header.key0,
    replay.header.key1,
    replay.header.options.recordRestricted,
    replay.file,
    replay.fileOffset,
    replay.records,
    replay.cipherIndex,
    replay.cipherBase,
    replay.cipherKey,
  };
  recorder.Record(static_cast<std::uint16_t>(record.length), &record, 0x2A, 0);
}

// OFFSET: 0x00CEBF30

// OFFSET: 0x00CEC100

// OFFSET: 0x00CEC6A0
// FUNCTION: fn_21356
unsigned CGameMode::sub_CEC6A0(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  unsigned offset = 6;
  if (*reinterpret_cast<const std::int16_t*>(data.data()) == 17000) {
    while (offset < data.size()) {
      const short type = *reinterpret_cast<const short*>(data.data() + offset);
      const std::uint32_t size = *reinterpret_cast<const std::uint32_t*>(data.data() + offset + 2);
      offset += 6;

      if (type == 0xCD) {
        unsigned char* record = new unsigned char[0x10]();
        std::memcpy(record, data.data() + offset, size);
        offset += size;
        this->sub_D41250(record);
        delete[] record;
      } else if (type == 0xCE) {
        unsigned char* record = new unsigned char[0x61]();
        std::memcpy(record, data.data() + offset, size);
        offset += size;
        this->sub_D40730(record);
        delete[] record;
      } else if (type == 0x4269) {
        break;
      } else {
        offset += size;
      }
    }
  } else {
    offset = 0xFFFFFFFFu;
  }

  return offset;
}

// OFFSET: 0x00CEFE50
// FUNCTION: fn_21407
void UIWindowMgr::ClearUnknown04DC() {
  this->unknown_04DC.assign("", 0);
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
  return g_windowMgr.LookupWindow(0x122) != nullptr || g_windowMgr.LookupWindow(0x12E) != nullptr
      || g_windowMgr.LookupWindow(0x15C) != nullptr || g_windowMgr.LookupWindow(0x2716) != nullptr
      || g_windowMgr.LookupWindow(0x169) != nullptr;
}

// OFFSET: 0x00CF0B30
// FUNCTION: fn_21422
bool CGameMode::ShowRestrictedNotice(int grade) {
  if (g_serviceMode == 3 || g_serviceType == 10 || grade < 2) {
    return false;
  }

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x31)), 255, 0, 0, 0);
  g_windowMgr.unknown_04DC.assign("", 0);
  return true;
}

// OFFSET: 0x00CCD880
// FUNCTION: fn_21217
CGameMode::CGameMode()
    : scene(nullptr), unknown_0408(nullptr), unknown_040C(0), unknown_0410(0), unknown_0414(0), unknown_0418(0), unknown_05C4(0), unknown_05C8(0),
      broadcastBalloon(nullptr), noticeBalloon(nullptr), unknown_0648(0) {
  this->unknown_0624[0] = 0;
  this->camera = nullptr;
  this->mousePointer = nullptr;
  this->replayPauseTick = 0;
  this->unknown_05EC[0] = 0;
  this->itemWindow = nullptr;
  this->unknown_060C[0] = 0;
}

// OFFSET: 0x00CEFE70
// FUNCTION: fn_21408
void CGameMode::RemoveUnknown0604(int window) {
  UIWindow* target = reinterpret_cast<UIWindow*>(window);
  for (std::list<UIWindow*>::iterator it = this->unknown_0604.begin(); it != this->unknown_0604.end(); ++it) {
    if (*it == target) {
      this->unknown_0604.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00CEFB30
// FUNCTION: fn_21404
void CGameMode::AddUnknown0604(UIWindow* window) {
  this->unknown_0604.push_back(window);
}

#pragma pack(push, 1)
struct AdminStatusRequestPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ unsigned char unknown_0006;
  /* 0007 */ std::uint16_t unknown_0007;
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
  packet.id = 0x149;
  packet.accountId = static_cast<std::uint32_t>(g_session.m_aid);
  packet.unknown_0006 = 2;
  packet.unknown_0007 = 0xA;
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00D06BD0
// FUNCTION: fn_21578
void SetPlayerName(std::uint32_t id, std::string name) {
  g_playerNameDictionary->Set(id, name);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CompleteUnknown0194(id, name);
  GuildInfo* guildInfo = &g_guildInfo;
  guildInfo->UpdateCharacterName(id, name.c_str());
  g_session.NotifyUnknown7F94Name(id, name.c_str());
  g_session.UpdateFriendName(id, name.c_str());
}

// OFFSET: 0x00CDD350
// FUNCTION: fn_21316
void CGameMode::PlaceWindowAtActor(UIWindow* window, CGameActor* actor) {
  const int width = window->geometry.width;
  const int height = window->geometry.height;
  const bool raised = g_serviceOptions.Get(0x5C) != 0;
  int offsetX = 0;
  int offsetY = 0;
  switch (actor->render.unknown_00A8 != 0 ? raised : 0) {
  case 0:
    offsetX = -(width / 2);
    offsetY = static_cast<int>(static_cast<float>(g_renderer->viewport.height * 20 / 480) * actor->render.unknown_005C);
    break;
  case 1:
    offsetX = -(width / 2);
    offsetY = static_cast<int>(static_cast<float>(g_renderer->viewport.height * -100 / 480) * actor->render.unknown_005C);
    break;
  case 2:
    offsetX = static_cast<int>(
      static_cast<float>(g_renderer->viewport.width * -30 / 640) * actor->render.unknown_005C - static_cast<float>(width / 2)
    );
    offsetY = static_cast<int>(
      static_cast<float>(g_renderer->viewport.height * -25 / 480) * actor->render.unknown_005C - static_cast<float>(height / 2)
    );
    break;
  case 3:
    offsetX = static_cast<int>(
      static_cast<float>(g_renderer->viewport.width * 30 / 640) * actor->render.unknown_005C - static_cast<float>(width / 2)
    );
    offsetY = static_cast<int>(
      static_cast<float>(g_renderer->viewport.height * -25 / 480) * actor->render.unknown_005C - static_cast<float>(height / 2)
    );
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
void CGameMode::Slot7(int value) {
}

// OFFSET: 0x00CDFA00
// FUNCTION: fn_21324
std::uint64_t CGameMode::LaunchDummyEffect(const vector3d* position, int effectId, int lifeFrames, int value) {
  if (lifeFrames > 0) {
    CDummyEffectLauncher* launcher = new CDummyEffectLauncher;
    if (launcher != nullptr) {
      const int address = reinterpret_cast<int>(position);
      launcher->SendMsg(nullptr, 14, 0, address, address >> 31, effectId, effectId >> 31, lifeFrames, lifeFrames >> 31, value, value >> 31, 0, 0, 0, 0);
      this->scene->objects.push_back(launcher);
      return launcher->identity;
    }
  }
  return 0;
}

// OFFSET: 0x00CDFB00
// FUNCTION: fn_21326
void CGameMode::RequestPlayerAction(int target, int argument) {
  int value = this->unknown_040C;
  if (value != 10000) {
    const int maxWeight = g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight;
    if (g_session.m_weight * 100 / maxWeight >= 90) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(307)), 0xFF, 0, 0, 0);
      value = this->unknown_040C;
      if (value != 40000) {
        return;
      }
    }
  }

  switch (reinterpret_cast<std::intptr_t>(this->unknown_0408)) {
  case 0:
    this->scene->player->SendMsg(nullptr, 10, 0, target, 0, argument, argument >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 2:
  case 4:
    this->scene->player->SendMsg(nullptr, 41, 0, target, 0, value, value >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, 90, 0, this->unknown_0414, this->unknown_0414 >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
  this->mousePointer->SendMsg(nullptr, 1, 0, address, address >> 31, value, value >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00CED330
// FUNCTION: fn_21366
void ShowCurrentMapName() {
  if (g_serviceOptions.Get(0x10C)) {
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
  case 990:
  case 991:
  case 992:
  case 993:
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
  case 994:
  case 995:
  case 996:
  case 997:
    result = true;
    break;
  }
  return result;
}

// OFFSET: 0x00CF0950
// FUNCTION: fn_21419
bool CGameMode::ConfirmDraggedItemUse() {
  if (g_session.unknown_16EC != this->drag.id) {
    return false;
  }

  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, this->drag.id);
  if (item.unknown_0000[1] > 0) {
    if (g_replayAnimationScaling) {
      this->unknown_05EC[0] = 0;
    }
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(657), 0, true, false, 0, "Message", 280, 120, 0);
    return true;
  }

  return false;
}

// OFFSET: 0x00CFE480
// FUNCTION: fn_21432
void CGameMode::sub_CFE480(int actorId) {
  if (g_windowMgr.unknown_0508 != 2 && actorId == static_cast<int>(g_session.m_aid)) {
    this->scene->player->unknown_0544->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CDBC30
// FUNCTION: fn_21312
void __stdcall sub_CDBC30(const vector3d* position, int actorId, float scale, int job) {
  if (actorId == 0) {
    return;
  }

  CSprArrowEffect* effect = static_cast<CSprArrowEffect*>(
    g_effectMgr->Spawn(0, 0x964, position->x, position->y, position->z, actorId, -1, 0, 0)
  );
  if (effect != nullptr) {
    effect->unknown_00F4 = scale;
    effect->SetBulletSprite(job);
  }
}

// OFFSET: 0x00CE8F10
// FUNCTION: fn_21344
void __stdcall sub_CE8F10(const void* info, const unsigned char* data) {
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

  Replay& replay = Replay::Instance();
  ReplayPacketRecorder recorder = {
    g_packetLengths,
    replay.baseFrame,
    replay.started,
    replay.frame,
    replay.header.key0,
    replay.header.key1,
    replay.header.options.recordRestricted,
    replay.file,
    replay.fileOffset,
    replay.records,
    replay.cipherIndex,
    replay.cipherBase,
    replay.cipherKey,
  };
  recorder.Record(packet.length, &packet, 0x2A, 0);
}

// OFFSET: 0x00CDFAF0
// FUNCTION: fn_21325
void CGameMode::IgnorePacket() {
}

// OFFSET: 0x00CE8EC0
// FUNCTION: fn_21343
void CGameMode::RemoveUnknown01A4(int value) {
  for (std::list<int>::iterator it = this->unknown_01A4.begin(); it != this->unknown_01A4.end(); ++it) {
    if (*it == value) {
      this->unknown_01A4.erase(it);
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

  Replay& replay = Replay::Instance();
  ReplayPacketRecorder recorder = {
    g_packetLengths,
    replay.baseFrame,
    replay.started,
    replay.frame,
    replay.header.key0,
    replay.header.key1,
    replay.header.options.recordRestricted,
    replay.file,
    replay.fileOffset,
    replay.records,
    replay.cipherIndex,
    replay.cipherBase,
    replay.cipherKey,
  };
  recorder.Record(record.length, &record, 0x2A, 0);
  return true;
}

// OFFSET: 0x00CED260
// FUNCTION: fn_21364
void CGameMode::UpdateReplayActionSpeeds() {
  CPlayer* player = this->scene->player;
  player->SetActionSpeed(Replay::Instance().AnimationScale());
  for (CRenderObject* actor : this->scene->actors) {
    actor->SetActionSpeed(Replay::Instance().AnimationScale());
  }
}

// OFFSET: 0x00CF1060
// FUNCTION: fn_21425
ReplayTime CGameMode::StopReplayRecording() {
  if (Replay::Instance().state != 2) {
    return ReplayTime();
  }

  Replay::Instance().SetState(1);
  Replay::Instance().UpdateDuration();
  const ReplayTime duration = Replay::Instance().Duration();
  Replay::Instance().CloseFile();
  return duration;
}

#pragma pack(push, 1)
struct GameModeChunkHeader {
  /* 0000 */ std::int16_t type;
  /* 0002 */ std::uint32_t size;
};
#pragma pack(pop)

static_assert(sizeof(GameModeChunkHeader) == 6);

#pragma pack(push, 1)
struct GameModeStoredRecord00DA {
  /* 0000 */ std::uint16_t type;
  /* 0002 */ int value0002;
  /* 0006 */ int value0006;
  /* 000A */ std::uint16_t value000A;
  /* 000C */ std::uint16_t value000C;
  /* 000E */ unsigned char value000E;
  /* 000F */ std::uint16_t value000F;
  /* 0011 */ int value0011;
  /* 0015 */ int value0015;
  /* 0019 */ std::uint16_t value0019;
  /* 001B */ std::uint16_t value001B;
  /* 001D */ std::uint16_t value001D;
  /* 001F */ std::uint16_t value001F;
  /* 0021 */ std::uint16_t value0021;
  /* 0023 */ std::uint16_t value0023;
  /* 0025 */ char text[0x18];
  /* 003D */ std::uint16_t value003D;
};
#pragma pack(pop)

static_assert(offsetof(GameModeStoredRecord00DA, text) == 0x25);
static_assert(offsetof(GameModeStoredRecord00DA, value003D) == 0x3D);

static_assert(sizeof(GameModeTextRecord) == 0x190);
static_assert(offsetof(GameModeTextRecord, buffer) == 0x1E);

// OFFSET: 0x00CFE4D0
// FUNCTION: fn_21433
void CGameMode::NotifyUnknown0604(int value) {
  for (std::list<UIWindow*>::iterator it = this->unknown_0604.begin(); it != this->unknown_0604.end(); ++it) {
    (*it)->SendMsg(nullptr, 60, value, 0, 0, 0);
  }
}

// OFFSET: 0x00CEBF30
// FUNCTION: fn_21354
unsigned CGameMode::sub_CEBF30(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  unsigned offset = 6;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (header.type == 15000) {
    while (offset < data.size()) {
      header = {};
      header = *reinterpret_cast<const GameModeChunkHeader*>(data.data() + offset);
      offset += 6;

      if (header.type == 0xCF) {
        std::vector<unsigned char> record(0x13);
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D2DB80(record.data());
      } else if (header.type == 15001) {
        break;
      } else {
        offset += header.size;
      }
    }
  } else {
    offset = 0xFFFFFFFFu;
  }

  return offset;
}

// OFFSET: 0x00CEC100
// FUNCTION: fn_21355
unsigned CGameMode::sub_CEC100(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  unsigned offset = 6;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (header.type == 10000) {
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
        this->sub_D3F130(record.data());
        break;
      case 0xCA:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D51A90(record.data());
        break;
      case 0xCB:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D3D2F0(record.data());
        break;
      case 0xCC:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D24430(record.data());
        break;
      case 0xD0:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D350A0(record.data());
        break;
      case 0xD2:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D37A10(record.data());
        break;
      case 0xD4:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D619E0(record.data());
        break;
      case 0xD5:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D61390(record.data());
        break;
      case 0xD6:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D616B0(record.data());
        break;
      case 0xD7:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D63D70(record.data());
        break;
      case 0xD8:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D63720(record.data());
        break;
      case 0xD9:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D63A40(record.data());
        break;
      case 0xDA: {
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        const auto* stored = reinterpret_cast<const GameModeStoredRecord00DA*>(record.data());
        this->sub_D7B170(
          stored->value0002, stored->value0006, stored->value000A, stored->value000C, stored->value000E, stored->value000F,
          stored->value0011, stored->value0015, stored->value0019, stored->value001B, stored->value001D, stored->value001F,
          stored->value0021, stored->value0023, stored->text, stored->value003D
        );
        break;
      }
      case 0x191:
        std::memcpy(record.data(), data.data() + offset, header.size);
        offset += header.size;
        this->sub_D65FC0(record.data());
        break;
      default:
        offset += header.size;
        break;
      }
    }
  } else {
    offset = 0xFFFFFFFFu;
  }

  return offset;
}

// OFFSET: 0x00CCFD70
// FUNCTION: fn_21248
void CGameMode::UpdatePetFriendlyWindow(int previousFriendly) {
  if (!g_serviceOptions.Get(0x128)) {
    return;
  }

  if (g_session.GetPetFriendlyState(g_session.m_petFriendly) == 0 && g_session.m_petFriendly <= 100) {
    if (this->unknown_0628.find(g_session.unknown_1010) == this->unknown_0628.end()) {
      if (g_windowMgr.window_038C == nullptr) {
        g_windowMgr.EnsureWindow(0x58);
      }

      this->UpdateUnknown0628(g_session.unknown_1010, true);
    }
  }

  if (previousFriendly < g_session.m_petFriendly && g_session.m_petFriendly > 99) {
    auto it = this->unknown_0628.find(g_session.unknown_1010);
    if (it != this->unknown_0628.end()) {
      this->unknown_0628.erase(it);
    }
  }
}

// OFFSET: 0x00CECE30
// FUNCTION: fn_21362
bool CGameMode::SaveEmblemFile(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return false;
  }

  const int nameLength = *reinterpret_cast<const int*>(data.data());
  std::string name;
  name.assign(reinterpret_cast<const char*>(data.data() + 4), nameLength);

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

// OFFSET: 0x00CF0E30

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
    text.assign(g_renewQuestMgr->GetMonsterConditionText(quest.m_huntUnknown01A0[index], quest.m_huntUnknown01AC[index], quest.m_huntUnknown01B2[index]));
  } else {
    text.assign(FormatOwnedString("%s", quest.m_huntName[index]));
  }

  if (quest.m_huntUnknown00E0[index] == quest.m_huntUnknown0180[index]) {
    text.assign(FormatOwnedString("%s [ %s ]", text.c_str(), LookupMessage(0x7EE)));
  } else {
    text.assign(FormatOwnedString("%s [ %d / %d ]", text.c_str(), quest.m_huntUnknown00E0[index], quest.m_huntUnknown0180[index]));
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
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 19000;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (ActiveSkillEffect effect : g_activeSkillEffects) {
    if (effect.unknown_0004 != 0 && effect.unknown_0004 != 999999) {
      effect.unknown_0004 -= timeGetTime();
    }

    std::vector<unsigned char> chunk(sizeof(GameModeStoredSkillEffect));
    auto* record = reinterpret_cast<GameModeStoredSkillEffect*>(chunk.data());
    record->header.type = 0x4A43;
    record->header.size = sizeof(ActiveSkillEffect);
    record->effect = effect;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 19001;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

// OFFSET: 0x00CE9DB0
// FUNCTION: fn_21348
std::vector<unsigned char> CGameMode::SaveUnknown024C() {
  std::vector<unsigned char> data;

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 14000;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    std::vector<unsigned char> chunk(sizeof(GameModeStoredValue));
    auto* record = reinterpret_cast<GameModeStoredValue*>(chunk.data());
    record->header.type = 14011;
    record->header.size = sizeof(int);
    record->value = this->unknown_024C;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 14001;
    header->size = 0;
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
  if (g_navigationLinkParser->HasUnownedItemLink(buffer) == true) {
    return;
  }

  const std::size_t length = std::strlen(buffer);
  unsigned char* packet = static_cast<unsigned char*>(
    _MallocaComputeSize(length + 8) != 0
      ? (_MallocaComputeSize(length + 8) <= _ALLOCA_S_THRESHOLD
          ? _MarkAllocaS(_alloca(_MallocaComputeSize(length + 8)), _ALLOCA_S_STACK_MARKER)
          : _MarkAllocaS(std::malloc(_MallocaComputeSize(length + 8)), _ALLOCA_S_HEAP_MARKER))
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
  this->camera->camera.target[0] = this->scene->player->render.position.x;
  this->camera->camera.target[1] = this->scene->player->render.position.y;
  this->camera->camera.target[2] = this->scene->player->render.position.z;
  this->camera->camera.desiredTarget[0] = this->scene->player->render.position.x;
  this->camera->camera.desiredTarget[1] = this->scene->player->render.position.y;
  this->camera->camera.desiredTarget[2] = this->scene->player->render.position.z;
  this->hasViewParameters = 0;

  int found;
  const std::vector<std::string>* parameters = g_session.GetMapViewParameters(this->mapName, &found);
  if (found != 0) {
    this->hasViewParameters = 1;
    for (int index = 0; index < 9; ++index) {
      this->viewParameters[index] = static_cast<std::int16_t>(std::atof((*parameters)[index].c_str()));
    }
  }

  const bool indoor = g_session.IsUnknownName5248(this->mapName);
  this->unknown_0254 = !indoor;
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
    this->unknown_0288 = 45.0f;
    this->camera->camera.yaw = 45.0f;
    this->camera->camera.desiredYaw = this->unknown_0288;
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
#include "ActorPickNode.h"
#include "Client/Registry.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Render/DX9/RendererFrameDX9.h"
#include "Render/Textures/TextureCache.h"
#include "Contents/ClientItemLists.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Network/Packets/PacketId.h"
#include "Security/ReassemblyPacketMgr.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include <chrono>
#include <winsock2.h>
#include "UI/Window/UIWordDisplayWnd.h"
#include "Contents/Actors/ActorStateFlags.h"

class CPingLiveSchedule : public CSchedule {
public:
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
    g_windowMgr.EnsureWindow(332);
    return;
  }

  if (g_session.m_lastSendTick + 10000 < tick) {
    std::uint16_t packet = 0xB1C;
    GetRagConnection().Send(sizeof(packet), &packet);
  }
}

#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/ModeFadeOut.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Resource/ResourceMgr.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Utility/Cipher/XorBytes.h"

namespace {

template <std::size_t Length, std::uint64_t Key>
class GameModeEncryptedName {
public:
  explicit GameModeEncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~GameModeEncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

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
        this->Slot7(this->cursorType);
      }
    }

    if (g_nProtectGameGuardMgr->HasError()) {
      GetRagConnection().Close();
      g_modeMgr.Quit();
      std::string message = g_nProtectGameGuardMgr->ErrorMessage();
      if (!message.empty()) {
        const std::uint8_t source[] = {0x53, 0x11, 0xF9, 0xA6, 0x8D, 0x7E, 0xA6, 0xD9, 0x1D, 0x06,
                                       0xEA, 0xA4, 0x9C, 0x5C, 0xB0, 0xCC, 0x4F, 0x25, 0x8B};
        thread_local GameModeEncryptedName<sizeof(source), 0xADC51BF9C98B413DULL> name(source);
        MessageBoxA(hWnd, message.c_str(), name.Text(), 0);
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
  if (this->unknown_0250 != 0) {
    return;
  }

  const unsigned char selfTargetOption = g_serviceOptions.Get(0x69);
  const unsigned char forceOption = g_serviceOptions.Get(0x6D);
  const int shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
  int force = shiftHeld;
  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
  const bool skillMode = mode == 2 || mode == 4;
  const bool allowed = aid != static_cast<int>(g_session.m_aid) || mode == 4;

  if (skillMode == true) {
    const bool support = IsSupportSkill(this->unknown_040C);
    const bool optionalSelf = IsOptionalSelfTargetSkill(this->unknown_040C);
    if (support == true) {
      force = 1;
    }
    if ((selfTargetOption != 0 || shiftHeld != 0) && optionalSelf == true) {
      force = 1;
    }

    CWorld* scene = this->scene;
    if (scene->unknown_0048 == 0 && scene->unknown_0054 == 0) {
      if (scene->unknown_004C != 0) {
        CGameActor* actor = static_cast<CGameActor*>(scene->GetGameActorByAID(aid));
        if (actor != nullptr && !g_session.IsLocalPlayerAid(actor->Slot50())) {
          GuildInfo* guildInfo = &g_guildInfo;
          if (!guildInfo->HasRelatedGuild(actor->Slot50())) {
            force = 1;
          }
        }
      }
    } else if (!g_session.IsPartyMember(aid, true)) {
      force = 1;
    }
  }

  if (allowed && g_mouse.buttons[0] == 1 && force != 0 && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
    this->SendMsg(0x1A, 1, 0, 0, 0);
    *reinterpret_cast<int*>(&this->unknown_00D8[0x20]) = 0;
    if (g_session.GetSceneUnknown004C() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
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

// OFFSET: 0x00CEB4C0
// FUNCTION: fn_21351
std::vector<unsigned char> CGameMode::WriteReplaySkillState() {
  std::vector<unsigned char> output;
  {
    const auto record = BuildReplayTaggedRecord(17000, nullptr, 0);
    output.insert(output.end(), record.begin(), record.end());
  }

  for (CRenderObject* object : this->scene->unknown_0020) {
    CSkill* skill = static_cast<CSkill*>(object);
    SkillEntryPacket packet;
    packet.packetType = 0x11F;
    packet.aid = skill->skillAid;
    packet.creatorAid = static_cast<std::uint32_t>(skill->unknown_0410);
    packet.job = static_cast<unsigned char>(skill->GetEffectiveJob(-1));
    packet.isVisible = 1;

    const C3dAttr* terrain = this->scene->terrain;
    const float column = static_cast<float>(std::floor(static_cast<float>(terrain->width / 2) + skill->render.position.x / static_cast<float>(terrain->cellSize)));
    const float row = static_cast<float>(std::floor(static_cast<float>(terrain->height / 2) + skill->render.position.z / static_cast<float>(terrain->cellSize)));
    packet.x = static_cast<std::int16_t>(static_cast<int>(column));
    packet.y = static_cast<std::int16_t>(static_cast<int>(row));

    const auto record = BuildReplayTaggedRecord(0xCD, &packet, sizeof(packet));
    output.insert(output.end(), record.begin(), record.end());
  }

  {
    const auto record = BuildReplayTaggedRecord(17001, nullptr, 0);
    output.insert(output.end(), record.begin(), record.end());
  }
  return output;
}

// OFFSET: 0x00CEBBB0
// FUNCTION: fn_21353
unsigned CGameMode::sub_CEBBB0(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFFu;
  }

  GameModeChunkHeader header = {};
  std::vector<unsigned char> record;
  unsigned offset = 6;
  std::uint32_t guildId = 0;
  header = *reinterpret_cast<const GameModeChunkHeader*>(data.data());
  if (header.type == 13000) {
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
        this->scene->player->Slot49(guildId, emblemVersion);
        g_guildInfo.m_guildId = guildId;
        break;
      }
      default:
        offset += header.size;
        break;
      }
    }
  } else {
    offset = 0xFFFFFFFFu;
  }

finished:
  return offset;
}

#include "Contents/Actors/Item.h"
#include <cstdlib>

// OFFSET: 0x00CE93E0
// FUNCTION: fn_21347
std::vector<unsigned char> CGameMode::SaveNameCardState() {
  std::vector<unsigned char> data;

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 13000;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  std::vector<char> buffer;
  std::vector<std::string> names;
  if (static_cast<int>(this->nameCard.Save(buffer)) > 0) {
    std::vector<unsigned char> chunk(buffer.size() + sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 13003;
    header->size = buffer.size();
    if (buffer.size() != 0) {
      std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), buffer.data(), buffer.size());
    }
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const int emblemVersion = g_guildMgr->GetEmblemVersion(this->scene->player->Slot50());
    if (this->AppendGuildEmblem(buffer, names, this->scene->player->Slot50(), emblemVersion)) {
      std::vector<unsigned char> chunk(buffer.size() + sizeof(GameModeChunkHeader));
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 13002;
      header->size = buffer.size();
      if (buffer.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), buffer.data(), buffer.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }

  {
    const int guildId = this->scene->player->Slot50();
    std::vector<unsigned char> chunk(sizeof(GameModeStoredValue));
    auto* record = reinterpret_cast<GameModeStoredValue*>(chunk.data());
    record->header.type = 13004;
    record->header.size = sizeof(int);
    record->value = guildId;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    const int emblemVersion = g_guildMgr->GetEmblemVersion(this->scene->player->Slot50());
    std::vector<unsigned char> chunk(sizeof(GameModeStoredValue));
    auto* record = reinterpret_cast<GameModeStoredValue*>(chunk.data());
    record->header.type = 13005;
    record->header.size = sizeof(int);
    record->value = emblemVersion;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (CRenderObject* object : this->scene->actors) {
    CGameActor* actor = static_cast<CGameActor*>(object);
    const int emblemVersion = g_guildMgr->GetEmblemVersion(actor->Slot50());
    if (this->AppendGuildEmblem(buffer, names, actor->Slot50(), emblemVersion)) {
      std::vector<unsigned char> chunk(buffer.size() + sizeof(GameModeChunkHeader));
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 13002;
      header->size = buffer.size();
      if (buffer.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), buffer.data(), buffer.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 13001;
    header->size = 0;
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
  /* 0000 */ std::uint16_t value0000;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint16_t value0006;
  /* 0008 */ int value0008;
  /* 000C */ int value000C;
  /* 0010 */ int count;
  /* 0014 */ int value0014;
  /* 0018 */ int value0018;
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
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 10000;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  std::vector<unsigned char> scratch;
  for (std::list<CRenderObject*>::iterator it = this->scene->actors.begin(); it != this->scene->actors.end(); ++it) {
    CGameActor* actor = static_cast<CGameActor*>(*it);
    CPc* pc = static_cast<CPc*>(actor);

    if (actor->unknown_03E0Low == 0) {
      if (actor->render.unknown_0070 == 1) {
        std::vector<char> entry = this->BuildActorEntryPacket(actor, 0x9FD);
        std::vector<unsigned char> chunk(entry.size() + sizeof(GameModeChunkHeader), 0);
        auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
        header->type = 0xD6;
        header->size = entry.size();
        if (entry.size() != 0) {
          std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), entry.data(), entry.size());
        }
        data.insert(data.end(), chunk.begin(), chunk.end());
      } else {
        std::vector<char> entry = this->BuildActorEntryPacket(actor, 0x9FF);
        std::vector<unsigned char> chunk(entry.size() + sizeof(GameModeChunkHeader), 0);
        auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
        header->type = 0xD9;
        header->size = entry.size();
        if (entry.size() != 0) {
          std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), entry.data(), entry.size());
        }
        data.insert(data.end(), chunk.begin(), chunk.end());
      }
    }

    UIChatRoomTitle* room = reinterpret_cast<UIChatRoomTitle*>(actor->unknown_0264[1]);
    if (room != nullptr) {
      scratch.resize(sizeof(ChatRoomEntryPacket), 0);
      auto* packet = reinterpret_cast<ChatRoomEntryPacket*>(scratch.data());
      packet->type = 0xD7;
      packet->packetLength = static_cast<std::uint16_t>(room->title.size() + sizeof(ChatRoomEntryPacket));
      packet->aid = actor->render.accountId;
      packet->roomId = room->roomId;
      packet->maxCount = static_cast<std::uint16_t>(room->memberLimit);
      packet->curCount = static_cast<std::uint16_t>(room->memberCount);
      packet->type = static_cast<unsigned char>(room->roomType);
      scratch.insert(scratch.end(), room->title.begin(), room->title.end());

      std::vector<unsigned char> chunk(scratch.size() + sizeof(GameModeChunkHeader));
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 0xC9;
      header->size = scratch.size();
      if (scratch.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), scratch.data(), scratch.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    if (actor->unknown_03E0Low == 1) {
      std::vector<unsigned char> record;
      record.resize(sizeof(GameModeStoredRecord00DA), 0);
      auto* stored = reinterpret_cast<GameModeStoredRecord00DA*>(record.data());
      stored->value0002 = actor->render.accountId;
      stored->value0006 = actor->GetEffectiveJob(-1);
      stored->value000E = static_cast<unsigned char>(actor->unknown_0260);
      stored->value000A = static_cast<std::uint16_t>(actor->unknown_0158[5]);
      stored->value000C = static_cast<std::uint16_t>(*reinterpret_cast<const int*>(actor->unknown_0170));
      stored->value000F = static_cast<std::uint16_t>(pc->unknown_0440);
      const std::int64_t value = actor->Slot35();
      stored->value0011 = static_cast<int>(value);
      stored->value0015 = static_cast<int>(value >> 32);
      stored->value0019 = static_cast<std::uint16_t>(pc->unknown_0450);
      stored->value001B = static_cast<std::uint16_t>(pc->unknown_0454);
      stored->value001D = static_cast<std::uint16_t>(pc->unknown_0458);
      stored->value001F = static_cast<std::uint16_t>(pc->unknown_0444);
      stored->value0021 = static_cast<std::uint16_t>(actor->render.unknown_0048);
      stored->value0023 = static_cast<std::uint16_t>(pc->unknown_049C);
      stored->value003D = static_cast<std::uint16_t>(actor->Slot46());
      {
        std::string name = this->nameCard.Name(actor->render.accountId);
        std::memcpy(stored->text, name.c_str(), sizeof(stored->text));
      }

      std::vector<unsigned char> chunk(record.size() + sizeof(GameModeChunkHeader));
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 0xDA;
      header->size = record.size();
      if (record.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), record.data(), record.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    UIMerchantShopTitle* shop = reinterpret_cast<UIMerchantShopTitle*>(actor->unknown_0264[2]);
    if (shop != nullptr) {
      scratch.resize(sizeof(ShopEntryPacket), 0);
      auto* packet = reinterpret_cast<ShopEntryPacket*>(scratch.data());
      packet->packetType = 0x131;
      packet->aid = actor->render.accountId;
      std::memcpy(packet->name, shop->label->tooltip.c_str(), sizeof(packet->name));

      std::vector<unsigned char> chunk(scratch.size() + sizeof(GameModeChunkHeader));
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 0xCA;
      header->size = scratch.size();
      if (scratch.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), scratch.data(), scratch.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    if (actor->unknown_0264[3] != 0) {
      scratch.resize(sizeof(ProgressPacket), 0);
      auto* packet = reinterpret_cast<ProgressPacket*>(scratch.data());
      packet->color = 0;
      packet->packetType = 0x2F0;
      packet->seconds = (actor->unknown_0280 - actor->unknown_0284) / 1000;

      std::vector<unsigned char> chunk(scratch.size() + sizeof(GameModeChunkHeader));
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 0xCB;
      header->size = scratch.size();
      if (scratch.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), scratch.data(), scratch.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    UIMerchantShopTitle* buyingShop = reinterpret_cast<UIMerchantShopTitle*>(actor->unknown_0264[4]);
    if (buyingShop != nullptr) {
      scratch.resize(sizeof(ShopEntryPacket), 0);
      auto* packet = reinterpret_cast<ShopEntryPacket*>(scratch.data());
      packet->packetType = 0x814;
      packet->aid = actor->render.accountId;
      std::memcpy(packet->name, buyingShop->label->tooltip.c_str(), sizeof(packet->name));

      std::vector<unsigned char> chunk(scratch.size() + sizeof(GameModeChunkHeader), 0);
      auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
      header->type = 0xCC;
      header->size = scratch.size();
      if (scratch.size() != 0) {
        std::memcpy(chunk.data() + sizeof(GameModeChunkHeader), scratch.data(), scratch.size());
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }

    const int count = actor->Slot56();
    if (count > 0) {
      GameModeStoredRecord0191 record;
      record.value000C = 9999;
      record.value0000 = 0;
      record.count = count;
      record.value0014 = 0;
      record.value0008 = 9999;
      record.aid = actor->render.accountId;
      record.value0006 = 0x47D;
      record.value0018 = 0;

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
#include "Contents/NavigationManager.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UINavigationWnd.h"
#include "Contents/Effects/EffectActor.h"

#pragma pack(push, 1)
struct GameModeUiMessage {
  /* 0000 */ std::uint16_t unknown_0000;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int values[6];
  /* 001E */ char text[1];
};

struct GameModeUiWhisperMessage {
  /* 0000 */ std::uint16_t unknown_0000;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ char target[0x19];
  /* 001F */ char text[1];
};

struct GameModeUiKindTextMessage {
  /* 0000 */ std::uint16_t unknown_0000;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int kind;
  /* 000A */ int unknown_000A;
  /* 000E */ char text[1];
};

struct GameModeUiNavigationMessage {
  /* 0000 */ std::uint16_t unknown_0000;
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
  /* 0000 */ std::uint16_t unknown_0000;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ char mapName[0x1E];
  /* 0024 */ int x;
  /* 0028 */ int y;
};

struct GameModeUiDragMessage {
  /* 0000 */ std::uint16_t unknown_0000;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::uint16_t length;
  /* 0006 */ int windowId;
  /* 000A */ int x;
  /* 000E */ int y;
  /* 0012 */ int source;
  /* 0016 */ std::uint32_t id;
  /* 001A */ std::uint32_t unknown_001A;
  /* 001E */ std::uint32_t unknown_001E;
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
      window->SendMsg(nullptr, 6, values->values[1], values->values[2], values->values[3], values->values[4]);
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
    *reinterpret_cast<int*>(&this->unknown_0100[0x10]) = values->values[0];
    break;
  case 904: {
    char buffer[200];
    std::memset(buffer, 0, sizeof(buffer));
    std::memcpy(buffer, values->text, values->values[0]);
    this->unknown_03C8 = buffer;
    std::memset(buffer, 0, sizeof(buffer));
    std::memcpy(buffer, values->text + values->values[0], values->values[1]);
    this->unknown_03E0 = buffer;
    this->unknown_03F8 = values->values[2];
    this->unknown_03FC = values->values[3];
    this->unknown_0400 = values->values[4];
    this->unknown_0404 = values->values[5];
    break;
  }
  case 905:
    this->SendMsg(values->values[0], values->values[1], values->values[2], values->values[3], 0);
    this->SendMsg(0x47, 0, 0, 0, 0);
    break;
  case 906: {
    const GameModeUiWhisperMessage* whisper = reinterpret_cast<const GameModeUiWhisperMessage*>(message);
    this->unknown_022C = whisper->target;
    this->unknown_0214 = whisper->text;
    break;
  }
  case 907: {
    const GameModeUiKindTextMessage* text = reinterpret_cast<const GameModeUiKindTextMessage*>(message);
    if (text->kind == 0x29) {
      g_session.unknown_550C = text->text;
    } else if (text->kind == 0xAE) {
      g_session.unknown_550C = text->text;
    }
    break;
  }
  case 908:
    if (values->values[0] == 0xCB && g_navigationWindow != nullptr) {
      g_navigationWindow->SendMsg(nullptr, values->values[1], values->values[2], reinterpret_cast<std::intptr_t>(&values->values[3]), 0, 0);
    }
    break;
  case 909:
    g_windowMgr.EnsureWindow(values->values[0]);
    break;
  case 910: {
    const GameModeUiNavigationMessage* navigation = reinterpret_cast<const GameModeUiNavigationMessage*>(message);
    g_navigationManager.sub_B99030(
      std::string(navigation->name), navigation->kind, navigation->mode, navigation->flag != 0, navigation->x, navigation->y, navigation->id
    );
    break;
  }
  case 911: {
    std::vector<char> states(values->length - 6, 0);
    std::memcpy(&states[0], values->values, states.size());
    g_windowMgr.CloseWindow(0x1A);
    g_navigationManager.LoadStates(states);
    if (g_navigationWindow != nullptr) {
      static_cast<UINavigationV4Wnd*>(g_navigationWindow)->FillRouteGroups();
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
  drag.unknown_0310 = packet->unknown_001A;
  drag.unknown_0314 = packet->unknown_001E;
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
    std::memcpy(&drag.options[index * 5], &packet->options[index * 5], 5);
  }

  UIWindow* window = g_windowMgr.LookupWindow(packet->windowId);
  this->drag = drag;

  if (this->drag.quantity == 1) {
    const int kind = packet->windowId;
    if (kind == 0x17 || kind == 0x2C) {
      if (packet->itemType <= 3 || packet->itemType == 7) {
        this->unknown_05EC[0] = this->IsAllowedPair(kind, packet->source);
      }
    } else if (kind == 0x16) {
      if (packet->itemType <= 3 || packet->itemType == 7) {
        this->unknown_05EC[0] = this->IsAllowedPair(0x16, packet->source);
      }
    }
  } else {
    this->unknown_05EC[0] = this->IsAllowedPair(packet->windowId, packet->source);
  }

  if (window != nullptr) {
    window->Slot10(packet->x, packet->y, reinterpret_cast<std::intptr_t>(&drag));
  }

  const int kind = packet->windowId;
  if (kind == 0x24 || kind == 0x9F) {
    this->unknown_05EC[0] = 0;
  } else if (this->unknown_05EC[0] == 1 && g_windowMgr.window_01EC != nullptr) {
    if (drag.type == 6) {
      const ItemCardName card = FindItemCardName(std::atoi(drag.name.c_str()));
      g_windowMgr.window_01EC->SendMsg(nullptr, 0x1D, 0, reinterpret_cast<std::intptr_t>(card.name), 0, 0);
    } else {
      const ITEM_DESC description = FindItemDescription(std::atoi(drag.name.c_str()));
      g_windowMgr.window_01EC->SendMsg(nullptr, 0x1D, 0, reinterpret_cast<std::intptr_t>(description.identifiedName), 0, 0);
    }
  }

  this->SendMsg(0x12, 0, 0, 0, 0);
}

// OFFSET: 0x00CDBEB0

// OFFSET: 0x00CEA0C0
// FUNCTION: fn_21349
std::vector<unsigned char> CGameMode::SaveGroundItems() {
  std::vector<unsigned char> data;

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 15000;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  for (CRenderObject* object : this->scene->unknown_0018) {
    CItem* item = static_cast<CItem*>(object);
    ItemEntryPacket packet;
    packet.packetType = 0x9D;
    packet.aid = item->itemAid;
    packet.itemId = item->itemId;
    packet.identified = item->identified;

    int column;
    int row;
    int subColumn;
    int subRow;
    this->scene->terrain->Grid().WorldSubcell(item->render.position.x, item->render.position.z, column, row, subColumn, subRow);
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

    std::vector<unsigned char> chunk(sizeof(GameModeStoredItemEntry));
    auto* record = reinterpret_cast<GameModeStoredItemEntry*>(chunk.data());
    record->header.type = 0xCF;
    record->header.size = sizeof(ItemEntryPacket);
    packet.subY = static_cast<unsigned char>(subRow);
    record->packet = packet;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  {
    std::vector<unsigned char> chunk(sizeof(GameModeChunkHeader));
    auto* header = reinterpret_cast<GameModeChunkHeader*>(chunk.data());
    header->type = 15001;
    header->size = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }

  return data;
}

// OFFSET: 0x00CEFFC0
// FUNCTION: fn_21409
UIBroadcastBalloon* CGameMode::EnsureBroadcastBalloon() {
  if (!this->broadcastBalloon) {
    this->broadcastBalloon = new UIBroadcastBalloon(*reinterpret_cast<UIWindowContext*>(nullptr));
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

  g_serviceOptions.Get(0x5C);
  balloon = new UINameBalloonText(*reinterpret_cast<UIWindowContext*>(nullptr));
  balloon->SetSize(8, 8);
  g_windowMgr.AddRenderedFront(balloon);
}

// OFFSET: 0x00CDD240
// FUNCTION: fn_21315
void __stdcall OpenWordDisplay(CGameActor* actor) {
  if (actor->unknown_018C || IsAdminAccount(actor->render.accountId)) {
    return;
  }

  if (!IsAdminAccount(g_session.m_aid)) {
    if (StateFlag02(actor->Slot13()) || StateFlag01(actor->Slot13()) || StateFlag06(actor->Slot13())) {
      return;
    }
  }

  UIWordDisplayWnd* window = new UIWordDisplayWnd(*reinterpret_cast<UIWindowContext*>(nullptr));
  window->SendMsg(nullptr, 34, actor->render.accountId, 0, 0, 0);
  g_windowMgr.AddRendered(window);
  actor->unknown_018C = reinterpret_cast<int>(window);
}

// OFFSET: 0x00CFF070
// FUNCTION: fn_21435
void CGameMode::ShowNoticeBalloon(const std::vector<std::string>* lines) {
  if (!lines || lines->empty()) {
    return;
  }

  UIBroadcastBalloon* balloon = this->noticeBalloon;
  if (!balloon) {
    this->noticeBalloon = new UIBroadcastBalloon(*reinterpret_cast<UIWindowContext*>(nullptr));
    this->noticeBalloon->SetSize(g_renderer->viewport.width, 50);
    this->noticeBalloon->Move(0, 130 * g_renderer->viewport.height / 480);
    g_windowMgr.AddRenderedFront(this->noticeBalloon);
    balloon = this->noticeBalloon;
    if (!balloon) {
      return;
    }
  }

  BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
  message.start = std::chrono::system_clock::now();
  for (std::vector<std::string>::const_iterator it = lines->begin(); it != lines->end(); ++it) {
    message.lines.push_back(BroadcastBalloonLine{*it, 0xFFFF, 0});
  }
  balloon->AddMessage(2, message);
}

#pragma pack(push, 1)
struct ZoneEnterPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t gid;
  /* 000A */ int authCode;
  /* 000E */ unsigned char unknown_000E[4];
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
  g_session.ClearEntryList5BD4();
  g_session.ClearShortcutSlots();
  g_session.ClearSecondaryShortcutSlots();
  g_session.unknown_6184 = 0;
  g_session.ResetInventoryItems();
  g_session.ClearPartyMembers();

  std::string name = mapName;
  name.resize(name.size() - 4);
  g_session.SetNavigationPlayerMap(name.c_str());
  g_session.m_playerX = x;
  g_session.m_playerY = y;

  in_addr address;
  address.s_addr = static_cast<ULONG>(ip);
  StringFormat(g_zoneServerIp, sizeof(g_zoneServerIp), static_cast<std::size_t>(-1), inet_ntoa(address));
  g_zoneServerPort = port;
  StringFormat(g_zoneServerHost, sizeof(g_zoneServerHost), static_cast<std::size_t>(-1), "%s", host.c_str());
  GetRagConnection().Close();
  g_activeSkillEffects.clear();

  this->unknown_0244 = 1;
  this->unknown_0248 = timeGetTime();
  this->scene->player->SendMsg(nullptr, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  if (g_screenShakeDuration > 0) {
    g_screenShakeDuration = g_screenShakeMode == 2 ? 10001 : g_screenShakeDuration;
  }

  if (g_replayAnimationScaling == 0) {
    this->unknown_0018 = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(g_zoneServerIp));
    if (!this->unknown_0018) {
      GetRagConnection().Close();
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1), 1, true, true, 0, "Message", 280, 120, 0) != 269) {
        this->SendMsg(141, 0, 0, 0, 0);
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
    packet.authCode = reinterpret_cast<const int*>(g_session.unknown_0E9C)[2];
    packet.sex = static_cast<unsigned char>(g_session.SyncUnknown0E98());
    GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
    g_session.SetOption(17, 1, 0);
    GetRagConnection().SetBlock(1);
  }

  static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(122), 16, 1);
  g_windowMgr.unknown_0474 = 185;
}

// OFFSET: 0x00CE1FA0
// FUNCTION: fn_21333
void CGameMode::UpdateBalloons() {
  if (this->nameBalloon) {
    if (*reinterpret_cast<int*>(&this->unknown_02B8[0x0C])) {
      g_windowMgr.Schedule(this->nameBalloon);
      this->nameBalloon = nullptr;
    } else {
      *reinterpret_cast<int*>(&this->unknown_02B8[0x0C]) = 1;
      if (this->balloonActorId == g_session.m_aid) {
        this->PlaceWindowAtActor(this->nameBalloon, reinterpret_cast<CGameActor*>(this->scene->player));
      } else if (CRenderObject* actor = this->scene->GetGameActorByAID(this->balloonActorId)) {
        this->PlaceWindowAtActor(this->nameBalloon, reinterpret_cast<CGameActor*>(actor));
      } else if (CItem* item = this->scene->GetItemByAID(this->balloonActorId)) {
        this->PlaceWindowAtActor(this->nameBalloon, reinterpret_cast<CGameActor*>(item));
      }
    }
  }

  if (this->itemNameBalloon) {
    if (*reinterpret_cast<int*>(&this->unknown_02B8[0x10])) {
      g_windowMgr.Schedule(this->itemNameBalloon);
      this->itemNameBalloon = nullptr;
    } else {
      *reinterpret_cast<int*>(&this->unknown_02B8[0x10]) = 1;
      if (this->balloonActorId == g_session.m_aid) {
        this->PlaceWindowAtActor(this->itemNameBalloon, reinterpret_cast<CGameActor*>(this->scene->player));
      } else if (CRenderObject* actor = this->scene->GetGameActorByAID(this->balloonActorId)) {
        this->PlaceWindowAtActor(this->itemNameBalloon, reinterpret_cast<CGameActor*>(actor));
      } else if (CItem* item = this->scene->GetItemByAID(this->balloonActorId)) {
        this->PlaceWindowAtActor(this->itemNameBalloon, reinterpret_cast<CGameActor*>(item));
      }
    }
  }

  UIWindow*& skillBalloon = *reinterpret_cast<UIWindow**>(&this->unknown_02B8[0x00]);
  UIWindow*& hintBalloon = *reinterpret_cast<UIWindow**>(&this->unknown_02B8[0x04]);
  int& shownSkill = *reinterpret_cast<int*>(&this->unknown_045C[0x04]);
  if (this->unknown_0408 == nullptr) {
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

    if (shownSkill != this->unknown_040C || created == true) {
      std::string text;
      shownSkill = this->unknown_040C;
      switch (shownSkill) {
      case 10000: {
        const std::uint8_t source[] = {0xBC, 0x82, 0x2D, 0x77, 0xD2, 0xDD, 0xC6, 0x1F, 0xB2, 0x8C, 0x33, 0x70, 0xD3, 0xCA, 0xD1, 0x3F};
        thread_local GameModeEncryptedName<sizeof(source), 0x3FA3AFA7035DE3FFULL> name(source);
        text.assign(name.Text());
        break;
      }
      case 20000: {
        const std::uint8_t source[] = {0x09, 0xEB, 0x66, 0x1C, 0x26, 0xAE, 0x07, 0x96, 0x38, 0xF5, 0x71, 0x04, 0x75, 0x86, 0x42, 0xDB};
        thread_local GameModeEncryptedName<sizeof(source), 0xDB27CB557D038759ULL> name(source);
        text = name.Text();
        break;
      }
      case 20001: {
        const std::uint8_t source[] = {0xCF, 0xD8, 0x9B, 0x0E, 0xCE, 0x19, 0xF9, 0x30, 0xEF, 0xC4, 0xBB};
        thread_local GameModeEncryptedName<sizeof(source), 0x51BB39B763BBBD8DULL> name(source);
        text = name.Text();
        break;
      }
      case 40000: {
        const std::uint8_t source[] = {0xAC, 0x4C, 0x27, 0x79, 0xB0, 0xA3, 0x86, 0xA7, 0xA2, 0x42, 0x39, 0x7E, 0xB1, 0xB4, 0x91, 0x87};
        thread_local GameModeEncryptedName<sizeof(source), 0x87E3D1C50D572DEFULL> name(source);
        text = name.Text();
        break;
      }
      default:
        if (g_session.IsState291To303(shownSkill)) {
          const std::uint8_t source[] = {0x3E, 0x4A, 0xC2, 0xE6, 0x2F};
          thread_local GameModeEncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
          text = name.Text();
        } else {
          text = CSkillInfo::DisplayName(*g_luaScript, shownSkill);
        }
        break;
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
      static_cast<UITransBalloonText*>(hintBalloon)->SetText(LookupMessage(234), 35);
      hintBalloon->Move((g_renderer->viewport.width - 640) / 2 - hintBalloon->geometry.width / 2 + 320, 400 * g_renderer->viewport.height / 480);
      static_cast<UITransBalloonText*>(hintBalloon)->m_textColor = 0x32FF32;
      static_cast<UITransBalloonText*>(hintBalloon)->m_outlineColor = 0;
    }
  }

  if (this->itemWindow) {
    CPlayer* player = this->scene->player;
    const int x = player->render.screenX - 30;
    const int y = player->render.screenY;
    const int offset = static_cast<int>(static_cast<float>(12 * g_renderer->viewport.height / 480) * player->render.unknown_005C);
    if (IsScreenShakeActive()) {
      this->itemWindow->Move(x, y - offset);
    } else {
      this->itemWindow->Move(x, offset + y);
    }
  }

  if (this->unknown_05FC && timeGetTime() > this->unknown_0600 + 10000) {
    g_windowMgr.Schedule(this->unknown_05FC);
    this->unknown_05FC = nullptr;
  }
}

// OFFSET: 0x00CDD7A0
// FUNCTION: fn_21319
void CGameMode::End() {
  CSchedule* schedule = reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->FindSchedule(g_session.m_pingLiveSchedule);
  if (schedule) {
    schedule->active = false;
  }
  reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->sub_BA95F0(4);
  this->StopReplayRecording();
  g_soundMgr->EndSamples();

  if (this->scene) {
    this->scene->OnExitFrame();
    delete this->scene;
  }

  if (this->camera) {
    this->camera->Slot2();
    delete this->camera;
  }

  g_actorPickNode.Clear();
  ResourceMgr::Instance().ClearUnused("rsm");
  ResourceMgr::Instance().ClearUnused("rsw");
  ResourceMgr::Instance().ClearUnused("gat");
  ResourceMgr::Instance().ClearExpired();
  g_texMgr.ClearUnused();
  RendererFrameDX9 frame{
    *reinterpret_cast<DeviceOwnerDX9*>(nullptr), *reinterpret_cast<RendererDX9Operations*>(nullptr), g_renderer->commands,
    g_renderer->primitives, g_renderer->lightmapped, g_renderer->vertices, g_renderer->queues, *reinterpret_cast<EffectRetirement*>(nullptr)
  };
  frame.Clear();
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
    DWORD value = g_serviceOptions.Get(92) != 0;
    RegSetValueExA(key, "POS", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));
    RegCloseKey(key);
  }

  if (this->unknown_0254) {
    g_outdoorCameraPitch = g_cameraPitch;
    g_outdoorCameraDistance = g_cameraDistance;
  } else {
    g_indoorCameraPitch = g_cameraPitch;
    g_indoorCameraDistance = g_cameraDistance;
  }

  if (g_whisperRecord != 0) {
    sub_CCFF10();
  }
  g_effectMgr->Clear();
  g_session.SaveMiniPartyInfo();
  g_session.SetUnknown8134(0);
}

#include "Contents/MapSkyMgr.h"

// OFFSET: 0x00CE8DE0
// FUNCTION: fn_21342
void CGameMode::ReloadScene() {
  this->scene->Clear();
  this->scene->CreatePlayer();
  this->scene->LoadMapEffects();
  g_weatherEffect.ResetEffect();
  this->mousePointer = new CMousePointer();
  this->scene->objects.push_back(this->mousePointer);
  this->camera->camera.target[0] = this->scene->player->render.position.x;
  this->camera->camera.target[1] = this->scene->player->render.position.y;
  this->camera->camera.target[2] = this->scene->player->render.position.z;
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
      strcpy_s(text, sizeof(text), LookupMessage(job >= 4054 && job <= 4112 ? 0x704 : 0x703));
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 8, 0, 0);
    }
  } else {
    actor = this->scene->GetGameActorByAID(aid);
    if (actor == nullptr) {
      return;
    }
  }

  int effectId = -1;
  int secondEffectId = -1;
  switch (itemId) {
  case 0x1F5:
  case 0x1FB:
  case 0x221:
  case 0x239:
  case 0x242:
  case 0x256:
  case 0x2D2F:
  case 0x2D30:
  case 0x2D32:
  case 0x3909:
    effectId = 0xCC;
    break;
  case 0x1F6:
  case 0x243:
  case 0x257:
  case 0x2D31:
  case 0x311E:
    effectId = 0xCD;
    break;
  case 0x1F7:
  case 0x1FC:
  case 0x222:
  case 0x2CEC:
  case 0x2D2E:
  case 0x2D36:
    effectId = 0xCE;
    break;
  case 0x1F8:
  case 0x1FD:
  case 0x223:
  case 0x22B:
  case 0x22C:
  case 0x22D:
  case 0x2CED:
  case 0x2CEF:
  case 0x2D1C:
  case 0x2D25:
  case 0x2D26:
  case 0x2D2D:
  case 0x2D35:
    effectId = 0xCF;
    break;
  case 0x1F9:
  case 0x1FE:
  case 0x2CEE:
  case 0x2CF0:
  case 0x2D1D:
  case 0x2D34:
    effectId = 0xD0;
    break;
  case 0x1FA:
  case 0x1FF:
  case 0x2D33:
    effectId = 0xD1;
    break;
  case 0x200:
  case 0x201:
  case 0x203:
  case 0x204:
  case 0x213:
  case 0x214:
  case 0x216:
  case 0x217:
  case 0x224:
  case 0x225:
  case 0x226:
  case 0x246:
  case 0x2AA:
    effectId = 0xD2;
    break;
  case 0x202:
  case 0x215:
  case 0x238:
  case 0x25F:
  case 0x2A8:
  case 0x2AB:
  case 0x302D:
  case 0x3037:
  case 0x3039:
  case 0x3041:
    effectId = 0xD3;
    break;
  case 0x205:
  case 0x206:
  case 0x207:
  case 0x208:
  case 0x209:
  case 0x20E:
  case 0x210:
  case 0x211:
  case 0x212:
  case 0x218:
  case 0x219:
  case 0x21A:
  case 0x21B:
  case 0x21C:
  case 0x21D:
  case 0x21E:
  case 0x21F:
  case 0x220:
  case 0x227:
  case 0x228:
  case 0x229:
  case 0x232:
  case 0x233:
  case 0x234:
  case 0x235:
  case 0x236:
  case 0x237:
  case 0x23A:
  case 0x23B:
  case 0x23C:
  case 0x23F:
  case 0x240:
  case 0x241:
  case 0x244:
  case 0x245:
  case 0x247:
  case 0x248:
  case 0x249:
  case 0x24A:
  case 0x297:
  case 0x2D1B:
  case 0x2D37:
  case 0x2D38:
  case 0x2D39:
  case 0x2D3A:
  case 0x2D3B:
  case 0x2D3C:
  case 0x2D3D:
  case 0x2D3E:
    effectId = 7;
    break;
  case 0x20A:
    g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x20B:
    g_soundMgr->PlayWave("effect\\EF_Blessing.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x22A:
  case 0x2F5A:
  case 0x2F5B:
  case 0x2F5C:
  case 0x2FA3:
  case 0x2FA4:
  case 0x2FA5:
  case 0x2FA6:
  case 0x2FCE:
  case 0x2FCF:
  case 0x5817:
  case 0x5818:
  case 0x5819:
    g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x1EB;
    break;
  case 0x22E:
  case 0x231:
  case 0x2D40:
  case 0x2D43:
  case 0x2FCD:
    effectId = 0x16C;
    break;
  case 0x22F:
  case 0x230:
  case 0x2D41:
  case 0x2D42:
  case 0x2DB5:
    effectId = 0x16B;
    break;
  case 0x23D:
  case 0x254:
  case 0x255:
  case 0x2D3F:
  case 0x2FCA:
    effectId = 0x1FD;
    break;
  case 0x23E:
  case 0x3036:
  case 0x3038:
    effectId = 0xD2;
    break;
  case 0x285:
  case 0x38AD:
  case 0x580E:
    effectId = 0xDA;
    break;
  case 0x290:
  case 0x38AE:
  case 0x5810:
    effectId = 0xDB;
    break;
  case 0x291:
  case 0x38AF:
  case 0x580F:
    effectId = 0xDC;
    break;
  case 0x296:
  case 0x2FE6:
    g_soundMgr->PlayWave("effect\\mapae.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x1FB;
    break;
  case 0x2AC:
    static_cast<EffectActor*>(actor)->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effectId = 0xDC;
    break;
  case 0x2AD:
    effectId = 0x1EC;
    break;
  case 0x2EF0:
  case 0x5811:
    g_soundMgr->PlayWave("effect\\fast.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x207;
    break;
  case 0x2EF1:
  case 0x5812:
    effectId = 0x26;
    break;
  case 0x2EF2:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x1FC;
    break;
  case 0x2EF7:
    g_soundMgr->PlayWave("effect\\ro2effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x221;
    break;
  case 0x2EFB:
  case 0x588D:
    g_soundMgr->PlayWave("wraith_die.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x2EFC:
    effectId = 0x240;
    break;
  case 0x2EFD:
    effectId = 0x241;
    break;
  case 0x2EFE:
    effectId = 0x242;
    break;
  case 0x2EFF:
    effectId = 0x243;
    break;
  case 0x2F00:
    effectId = 0x244;
    break;
  case 0x2F01:
    effectId = 0x245;
    break;
  case 0x2F02:
    g_soundMgr->PlayWave("ork_warrior_breath.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x2F56:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x25B;
    break;
  case 0x2F57:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x33;
    break;
  case 0x2F58:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x93;
    break;
  case 0x2F59:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x34;
    break;
  case 0x2F63:
    effectId = 0x264;
    break;
  case 0x2FBA:
    static_cast<EffectActor*>(actor)->LaunchEffect(0x177, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(actor)->LaunchEffect(0x1B8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2FDA:
    effectId = 0x251;
    break;
  case 0x2FDB:
    effectId = 0x254;
    break;
  case 0x2FDC:
    effectId = 0x252;
    break;
  case 0x2FDD:
    effectId = 0x255;
    break;
  case 0x2FDE:
    effectId = 0x256;
    break;
  case 0x2FDF:
    effectId = 0x253;
    break;
  case 0x3016:
  case 0x3017:
    effectId = 0x2BB;
    break;
  case 0x301B:
    effectId = 0x152;
    break;
  case 0x301C:
    effectId = 0x2C1;
    break;
  case 0x301D:
    g_soundMgr->PlayWave("effect\\storm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x2C2;
    break;
  case 0x301E:
    effectId = 0x2C3;
    break;
  case 0x3026:
    g_soundMgr->PlayWave("effect\\pokjuk_jap.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x2C5;
    break;
  case 0x3198:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x3DB;
    break;
  case 0x3199:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x3DC;
    break;
  case 0x31F4:
    g_soundMgr->PlayWave("effect\\itempokjuk.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = 0x3EE;
    break;
  case 0x3280:
    effectId = 0x56;
    break;
  case 0x38D2:
    effectId = 0x2AA;
    break;
  case 0x38D3:
    effectId = 0x2AB;
    break;
  case 0x38D4:
    effectId = 0x2AC;
    break;
  case 0x38D5:
    effectId = 0x2AD;
    break;
  case 0x38D6:
    effectId = 0x2AE;
    break;
  case 0x18733:
  case 0x18734:
  case 0x18735:
    effectId = 0x63D;
    secondEffectId = 0x63E;
    break;
  case 0x18736:
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
    while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(effectId)) {
      actor->SendMsg(nullptr, 109, 0, effectId, effectId >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_effectMgr->HasAllocator(effectId)) {
      g_effectMgr->Spawn(actor->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      static_cast<EffectActor*>(actor)->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (secondEffectId > 0) {
    while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(secondEffectId)) {
      actor->SendMsg(nullptr, 109, 0, secondEffectId, secondEffectId >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_effectMgr->HasAllocator(secondEffectId)) {
      g_effectMgr->Spawn(actor->render.accountId, secondEffectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      static_cast<EffectActor*>(actor)->LaunchEffect(secondEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
}

#include "ActorPickNode.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Registry.h"
#include "Contents/ClientItemLists.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/NavigationManager.h"
#include "Contents/SignBoard/SignBoard.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/TerrainPathSearch.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Network/GameTickPacket.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/UIMacroUserInfoWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIMileageWnd.h"
#include "UI/Window/UIWindowFactory.h"
#include <list>
#include <new>
#include <winsock2.h>
#include <windows.h>

struct SessionMessengerContext {
  void UpdateRandomStats();
};

struct SessionItemLists {
  void ClearTradeItems();
};

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
  this->unknown_019C.clear();
  this->unknown_01A4.clear();
  this->actorMenuIds.clear();
  this->unknown_01B4.clear();
  this->unknown_01C4.clear();
  this->m_skillUnitIds.clear();
  this->spriteMirrorActorIds.clear();
  this->minimapMarkers.clear();

  if (g_windowMgr.window_4F1CC) {
    g_windowMgr.window_4F1CC->SendMsg(nullptr, 486, 0, 0, 0, 0);
  }

  g_snapMgr.Clear();
  g_windowMgr.sub_AA6B80();
  this->broadcastBalloon = nullptr;
  this->noticeBalloon = nullptr;
  g_windowMgr.SetSurface0478(nullptr);
  g_windowMgr.EnsureWindow(0);
  g_windowMgr.ShowWindowsWithId(67, 1);
  g_windowMgr.EnsureWindow(204);

  const char* worldName = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(9, 0, 0, 0, 0));
  if (std::strcmp(worldName, "new_event.rsw") != 0) {
    g_windowMgr.EnsureWindow(190);
  }

  if (g_closeWindow137State) {
    g_windowMgr.EnsureWindow(137);
  }

  if (!g_replayAnimationScaling && g_serviceOptions.Get(270)) {
    g_windowMgr.EnsureWindow(159);
  }

  if (g_closeWindow164State) {
    g_windowMgr.EnsureWindow(164);
  }

  if (g_searchStoreInfoCreated) {
    g_windowMgr.EnsureWindow(181);
  }

  g_windowMgr.EnsureWindow(268);
  if (g_refreshWindow289) {
    g_windowMgr.EnsureWindow(289);
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
  *reinterpret_cast<int*>(this->unknown_00D8) = 0;
  *reinterpret_cast<int*>(&this->unknown_00D8[4]) = g_screenWidth;
  *reinterpret_cast<int*>(&this->unknown_00D8[8]) = g_screenHeight;
  this->unknown_00E4 = g_screenWidth;
  this->unknown_00E8 = g_screenHeight;
  this->unknown_0244 = 0;
  this->unknown_024C = 0;
  this->unknown_0250 = 0;
  *reinterpret_cast<int*>(&this->unknown_0294[0x18]) = 0;
  *reinterpret_cast<int*>(&this->unknown_0294[0x1C]) = 0;
  *reinterpret_cast<int*>(this->unknown_02B8) = 0;
  *reinterpret_cast<int*>(&this->unknown_02B8[4]) = 0;
  *reinterpret_cast<int*>(&this->unknown_02B8[8]) = 0;
  this->itemWindow = nullptr;
  this->unknown_05D0 = nullptr;
  this->unknown_01F8 = -1;
  this->balloonActorId = 0;
  *reinterpret_cast<int*>(this->unknown_02E4) = 0;
  *reinterpret_cast<int*>(&this->unknown_02E4[4]) = 0;
  *reinterpret_cast<int*>(&this->unknown_02B8[0x0C]) = 1;
  *reinterpret_cast<int*>(&this->unknown_02B8[0x10]) = 1;
  this->unknown_02CC = 0;
  *reinterpret_cast<int*>(this->unknown_02D0) = 0;
  this->unknown_02D4 = 0;
  this->drag.Clear();
  this->unknown_02DC = 0;
  this->unknown_02E0 = 0;
  *reinterpret_cast<int*>(this->unknown_00EC) = 0;
  *reinterpret_cast<int*>(&this->unknown_00EC[4]) = 0;
  *reinterpret_cast<int*>(&this->unknown_00EC[8]) = 0;
  this->unknown_00F8 = 1;
  this->unknown_00FC = 0;
  *reinterpret_cast<int*>(this->unknown_0100) = 0;
  *reinterpret_cast<std::uint32_t*>(&this->unknown_0100[0x04]) = timeGetTime() - 2000;
  *reinterpret_cast<std::uint32_t*>(&this->unknown_0100[0x08]) = timeGetTime() - 2000;
  *reinterpret_cast<int*>(&this->unknown_0100[0x0C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_0100[0x10]) = 0;
  *reinterpret_cast<int*>(&this->unknown_0100[0x14]) = 0;
  this->unknown_0450 = 0;
  this->unknown_0454 = 0;
  this->unknown_0438 = "";
  this->unknown_0408 = nullptr;
  this->unknown_040C = 0;
  g_pickInvert = 0;
  *reinterpret_cast<int*>(this->unknown_045C) = 0;
  this->miniMapTooltipId = -1;
  *reinterpret_cast<int*>(&this->unknown_045C[4]) = -1;

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
  *reinterpret_cast<int*>(&this->unknown_026E[0x02]) = 1;
  this->unknown_026E[0x16] = 0;
  *reinterpret_cast<std::uint32_t*>(&this->unknown_026E[0x06]) = timeGetTime();
  *reinterpret_cast<std::uint32_t*>(&this->unknown_026E[0x0A]) = timeGetTime();
  this->unknown_0248 = timeGetTime();
  *reinterpret_cast<std::uint32_t*>(this->unknown_01E0) = timeGetTime();
  this->broadcastBalloon = nullptr;
  *reinterpret_cast<std::uint32_t*>(&this->unknown_026E[0x0E]) = timeGetTime();
  *reinterpret_cast<std::uint32_t*>(&this->unknown_026E[0x12]) = timeGetTime();
  this->unknown_05CE = 1;
  std::memset(&this->unknown_05EC[1], 0, 6);
  *reinterpret_cast<int*>(&this->unknown_05EC[0x08]) = 0;
  *reinterpret_cast<int*>(&this->unknown_05EC[0x0C]) = 0;
  g_soundMgr->SetListenerYaw(this->camera->camera.desiredYaw);
  this->SendMsg(41, 0, 0, 0, 0);

  if (g_isStopByLogin) {
    g_isStopByLogin = 0;
    g_guildInfo.Init();
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(22)), 0xFFE6E6, 0, 0, 0);
    inet_ntoa(*reinterpret_cast<const in_addr*>(&g_session.unknown_0E9C[0x10]));
    if (!g_accountLinkedUserDataMgr->m_loaded) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3457)), 255, 0, 0, 0);
    }
  }

  reinterpret_cast<ClientItemLists*>(&g_session)->ResetUnknown166C();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1738();
  reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1758();
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1750();

  // GLOBAL: gl_25334
  static int s_window315Checked;
  if (!g_replayAnimationScaling && !s_window315Checked) {
    s_window315Checked = 1;
    if (g_window315Enabled) {
      g_windowMgr.EnsureWindow(315);
    }
  }

  if (g_replayAnimationScaling != 1) {
    std::int16_t packet = 0x7D;
    GetRagConnection().Send(2, &packet);
    GetRagConnection().ResetDeferred();
    g_session.SetOption(5, 1, 0);
  }

  SendGameTick(
    true, *reinterpret_cast<std::uint32_t*>(&this->unknown_026E[0x02]), *reinterpret_cast<std::uint32_t*>(&this->unknown_026E[0x06]), g_gameTickInterval,
    GetRagConnection(), g_packetLengths, g_pingLogEnabled, g_pingLogFirstTick, g_pingLogTicks
  );
  g_weatherEffect.ResetEffect();
  g_weatherEffect.ApplyMapWeather(this->mapName);
  if (std::strcmp(this->mapName, "xmas_fild01.rsw") == 0 || std::strcmp(this->mapName, "xmas.rsw") == 0) {
    if (g_serviceOptions.Get(8)) {
      g_weatherEffect.Snow();
    } else {
      g_weatherEffect.ExtendEffect();
    }
  }

  if (!g_mapSkyMgr) {
    g_mapSkyMgr = new (std::nothrow) CMapSkyMgr;
  }

  g_mapSkyMgr->Apply(this->mapName);
  if ((g_serviceType == 8 || g_serviceType == 9) && std::strcmp(this->mapName, "comodo.rsw") == 0 && g_serviceOptions.Get(8)) {
    g_weatherEffect.Fireworks();
  }

  *reinterpret_cast<int*>(&g_session.unknown_5570[0x08]) = -1;
  g_groundMarkEnabled = 0;
  g_session.ClearHomunStatus();
  *reinterpret_cast<int*>(&g_session.unknown_5620[0x08]) = -1;
  g_groundMarkAltEnabled = 0;
  g_session.ResetMercenaryStats();
  g_windowMgr.FocusWindow01C8Edit();

  g_windowMgr.window_4F168 = nullptr;
  const int replayState = Replay::Instance().state;
  if (g_replayAnimationScaling) {
    g_windowMgr.EnsureWindow(187);
    if (replayState == 8) {
      g_windowMgr.window_4F168->SendMsg(nullptr, 35, 0, 0, 0, 0);
    }
  }

  if (replayState == 3) {
    Replay::Instance().SetState(2);
    g_windowMgr.EnsureWindow(198);
    g_windowMgr.window_4F164->Move(Replay::Instance().unknown_01F4, Replay::Instance().unknown_01F8);
    g_windowMgr.window_4F164->SendMsg(nullptr, 123, 0, 0, 0, 0);
  } else if (replayState == 8) {
    Replay::Instance().SetState(7);
  }

  g_navigationManager.ResumeRoute();
  if (g_mileageWndActive) {
    UIWindow* mileage = g_windowMgr.EnsureWindow(267);
    if (mileage) {
      mileage->SendMsg(nullptr, 34, g_mileageWndActive, g_mileageWndPoint, g_mileageWndElapsed, g_mileageWndBonus);
    }
  }

  this->unknown_05FC = nullptr;
  *reinterpret_cast<int*>(this->unknown_0600) = 0;
  s_signBoardMgr->Initialize();
  g_statusRefreshSuppressed = 0;
  int& localMapMove = g_session.m_localMapMove;
  if (localMapMove == 1) {
    localMapMove = 0;
  }

  g_guildInfo.ClearUnknown0114();
  this->unknown_02EC = "";
}

// OFFSET: 0x00CE46B0
// FUNCTION: fn_21337
void CGameMode::ProcessRightButtonDrag() {
  switch (g_mouse.buttons[1]) {
  case 1: {
    if (g_windowMgr.unknown_0210) {
      g_windowMgr.CloseWindow(0x12);
    }

    this->SendMsg(0x12, 0, 0, 0, 0);
    this->SendMsg(0x47, 0, 0, 0, 0);
    this->unknown_00E4 = g_screenWidth;
    this->unknown_00E8 = g_screenHeight;
    if (!g_serviceOptions.Get(0x78)) {
      return;
    }

    UIShortCutWnd* shortcuts = static_cast<UIShortCutWnd*>(g_windowMgr.window_01E8);
    if (!shortcuts) {
      return;
    }

    AutomaticSkills* automatic = reinterpret_cast<AutomaticSkills*>(g_session.unknown_7FB4);
    if (shortcuts->m_slots[4]->unknown_0000 && automatic->Contains(static_cast<std::uint16_t>(shortcuts->m_slots[4]->itemId))) {
      g_session.SetOption(AutomaticSkills::CancelOption(static_cast<std::int16_t>(shortcuts->m_slots[4]->itemId)), 1, 0);
    } else {
      automatic->DisableAll();
    }

    shortcuts->SendMsg(nullptr, 0x29, 8, 0, 0, 0);
    return;
  }
  case 2:
    if (!HIBYTE(GetAsyncKeyState(VK_MENU)) && !HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      reinterpret_cast<ModeCursor*>(&this->cursorAction)->Set(4, this->cursorTick);
      if (this->hasViewParameters) {
        float yaw = static_cast<float>(g_screenWidth) + this->camera->camera.desiredYaw - static_cast<float>(this->unknown_00DC);
        const float low = static_cast<float>(this->viewParameters[3]);
        const float high = static_cast<float>(this->viewParameters[4]);
        if (yaw < low) {
          yaw = low;
        }
        if (yaw > high) {
          yaw = high;
        }
        this->camera->camera.desiredYaw = yaw;
      } else if (this->unknown_0254 == 0 && g_session.IsAdminSubMode(6) == 0) {
        float yaw = static_cast<float>(g_screenWidth) + this->camera->camera.desiredYaw - static_cast<float>(this->unknown_00DC);
        const float low = this->unknown_0288 - 20.0f;
        const float high = this->unknown_0288 + 20.0f;
        if (yaw < low) {
          yaw = low;
        }
        if (yaw > high) {
          yaw = high;
        }
        this->camera->camera.desiredYaw = yaw;
      } else {
        this->camera->camera.AddYaw(static_cast<float>(g_screenWidth - this->unknown_00DC));
      }

      g_soundMgr->SetListenerYaw(this->camera->camera.desiredYaw);
    }

    if (!HIBYTE(GetAsyncKeyState(VK_MENU)) && HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      const float range = this->unknown_0254 ? 42.0f : 10.0f;
      g_cameraPitch = this->camera->camera.desiredPitch - static_cast<float>((g_screenHeight - this->unknown_00E0) / 2);
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
      g_cameraDistance = this->camera->camera.desiredDistance - static_cast<float>((g_screenHeight - this->unknown_00E0) / 2);
      float maxDistance = this->unknown_0254 ? g_serviceOptionJitterScale : g_cameraBaseDistance;
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
    if (!g_serviceOptions.Get(0x78) && this->unknown_0254 && !HIBYTE(GetAsyncKeyState(VK_SHIFT)) && !HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
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
  if (g_mouse.wheel == 0 || this->unknown_0408 != nullptr) {
    return;
  }

  ClientSkills skills{
    g_session.m_skillBundle,
    g_session.m_homunSkills,
    g_session.m_mercSkills,
    g_session.m_additionalSkills,
    reinterpret_cast<const int&>(g_session.m_jobFlags),
    g_activeSkillEffects
  };
  CSkillInfo wheelSkill = skills.bundle.Lookup(0x27F, skills.classMode, skills.HasEffect(1717), skills.HasEffect(1716));
  if (wheelSkill.level > 0) {
    const int count = this->unknown_05F4;
    if (count > this->unknown_05F8) {
      this->unknown_05F8 = count;
    }
    this->unknown_05F4 = count + 4;
  }

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    const float step = g_mouse.wheel > 0 ? -5.0f : 5.0f;
    const float range = this->unknown_0254 ? 20.0f : 10.0f;
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

  if (g_serviceOptions.Get(0x79)) {
    UIShortCutWnd* shortcuts = static_cast<UIShortCutWnd*>(g_windowMgr.window_01E8);
    *reinterpret_cast<int*>(&g_session.unknown_544C[0x00]) = 1;
    AutomaticSkills* automatic = reinterpret_cast<AutomaticSkills*>(g_session.unknown_7FB4);
    if (g_mouse.wheel > 0) {
      if (!shortcuts) {
        return;
      }

      if (shortcuts->m_slots[3]->unknown_0000 && automatic->Contains(shortcuts->m_slots[3]->itemId)) {
        g_session.SetOption(AutomaticSkills::CancelOption(static_cast<std::int16_t>(shortcuts->m_slots[3]->itemId)), 1, 0);
      } else {
        automatic->DisableAll();
      }

      shortcuts->SendMsg(nullptr, 0x29, 7, 0, 0, 0);
    } else {
      if (!shortcuts) {
        return;
      }

      if (shortcuts->m_slots[2]->unknown_0000 && automatic->Contains(shortcuts->m_slots[2]->itemId)) {
        g_session.SetOption(AutomaticSkills::CancelOption(static_cast<std::int16_t>(shortcuts->m_slots[2]->itemId)), 1, 0);
      } else {
        automatic->DisableAll();
      }

      shortcuts->SendMsg(nullptr, 0x29, 6, 0, 0, 0);
    }
    return;
  }

  const float step = g_mouse.wheel > 0 ? -24.3f : 24.3f;
  g_cameraDistance = this->camera->camera.desiredDistance - step;
  float maxDistance = this->unknown_0254 ? g_serviceOptionJitterScale : g_cameraBaseDistance;
  CSkillInfo zoomSkill = skills.bundle.Lookup(0x1B9, skills.classMode, skills.HasEffect(1717), skills.HasEffect(1716));
  bool extendedZoom = false;
  if (this->hasViewParameters) {
    if (g_cameraDistance < static_cast<float>(this->viewParameters[0])) {
      g_cameraDistance = static_cast<float>(this->viewParameters[0]);
    }
    maxDistance = static_cast<float>(this->viewParameters[1] + this->viewParameters[0]);
  } else if (zoomSkill.level <= 0 && this->scene->player->render.unknown_0070 == 6 && !this->scene->player->state.Attachments().HasId(0x14E)) {
    switch (g_session.m_job) {
    case 0xFCF:
    case 0x1082:
    case 0x108F:
    case 0x1091:
    case 0x10CE:
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

struct AiMessage {
  /* 0000 */ int type;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ int level;
  /* 0010 */ int skill;
  /* 0014 */ int target;
};

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
  /* 0004 */ unsigned char unknown_0004[0x1F8];

  CEquipmentPropertiesMgr();
  void RefreshPretend();
};
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentProperties::CEquipmentPropertiesMgr) == 0x1FC);
#endif

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

  SkillWindows windows{g_windowMgr.window_02C4, g_windowMgr.window_01E8, g_windowMgr};
  for (const SkillListEntry& entry : skills) {
    CSkillInfo skill;
    if ((entry.id != 0 && entry.id < 7999) || g_session.IsHomunculusSkill(entry.id) || g_session.IsMercenarySkill(entry.id)) {
      skill.id = entry.id;
      skill.unknown_000C = entry.type;
      skill.level = entry.level;
      skill.unknown_0014 = entry.spCost;
      skill.unknown_0018 = entry.upgradable;
      skill.unknown_001C = entry.range;
      skill.name = skill.IdName(*g_session.m_hotkeyScript);
      skill.unknown_0030 = entry.unknown_000D;
      skill.unknown_0034 = skill.UseApAmount(*g_session.m_hotkeyScript, entry.level);
      g_session.m_skillBundle.ApplyLevelChange(skill);

      if (packetType == 0x10F || packetType == 0xB32) {
        ClientSkills client{
          g_session.m_skillBundle,
          g_session.m_homunSkills,
          g_session.m_mercSkills,
          g_session.m_additionalSkills,
          reinterpret_cast<const int&>(g_session.m_jobFlags),
          g_activeSkillEffects
        };
        const SkillUseLevels levels{
          g_session.m_skillBundle.unknown_0064, *reinterpret_cast<const std::vector<int>*>(g_session.unknown_0710), g_session.m_homunSkillLevels
        };
        client.Update(skill, 0, 0, *g_session.m_hotkeyScript, levels, windows);
      } else if (packetType == 0x235) {
        g_session.UpdateHomunSkill(skill, 0);
      } else if (packetType == 0x29D) {
        g_session.UpdateMercSkill(skill, 0);
      }
    }
  }

  if (packetType == 0x10F || packetType == 0xB32) {
    windows.Notify();
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
  std::uint16_t id, std::int16_t level, std::int16_t spCost, unsigned char upgradable, std::int16_t range, int type, std::int16_t unknown
) {
  *reinterpret_cast<int*>(&this->unknown_0100[0x0C]) = 0;

  ClientSkills client{
    g_session.m_skillBundle,
    g_session.m_homunSkills,
    g_session.m_mercSkills,
    g_session.m_additionalSkills,
    reinterpret_cast<const int&>(g_session.m_jobFlags),
    g_activeSkillEffects
  };
  CSkillInfo skill = client.Lookup(id);
  skill.level = level;
  skill.unknown_0014 = spCost;
  skill.unknown_0018 = upgradable;
  skill.unknown_001C = range;
  skill.unknown_000C = type;
  skill.unknown_0030 = unknown;
  skill.id = id;

  if (g_session.IsHomunculusSkill(id)) {
    g_session.UpdateHomunSkill(skill, 1);
  } else if (g_session.IsMercenarySkill(id)) {
    g_session.UpdateMercSkill(skill, 1);
  } else {
    const SkillUseLevels levels{
      g_session.m_skillBundle.unknown_0064, *reinterpret_cast<const std::vector<int>*>(g_session.unknown_0710), g_session.m_homunSkillLevels
    };
    SkillWindows windows{g_windowMgr.window_02C4, g_windowMgr.window_01E8, g_windowMgr};
    client.Update(skill, 1, 1, *g_session.m_hotkeyScript, levels, windows);
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

  int& placed = *reinterpret_cast<int*>(&this->unknown_02B8[0x0C]);
  if (this->balloonActorId != id && this->balloonActorId != 0) {
    this->balloonActorId = id;
    placed = 1;
    return;
  }

  this->balloonActorId = id;
  CRenderObject* actor = this->scene->GetGameActorByAID(id);
  if (actor) {
    CGameActor* gameActor = static_cast<CGameActor*>(actor);
    if (gameActor->GetEffectiveJob(-1) == 45) {
      return;
    }

    placed = 0;
    this->UpdateNameBalloon(gameActor);
    if (this->unknown_05CC[1] && g_session.IsPlayerJobId(gameActor->GetEffectiveJob(-1)) && IsAdminAccount(g_session.m_aid)) {
      g_windowMgr.SelectWhisperName(this->ActorName(id).c_str());
    }
    return;
  }

  if (id == g_session.m_aid) {
    placed = 0;
    this->UpdateNameBalloon(reinterpret_cast<CGameActor*>(this->scene->player));
    return;
  }

  CItem* item = this->scene->GetItemByAID(id);
  if (!item) {
    return;
  }

  std::string name(item->name.c_str());
  placed = 0;
  this->EnsureNameBalloon(this->nameBalloon);
  this->nameBalloon->AddNameLine(name.c_str());
  this->nameBalloon->SetIcon("", 0);
  this->nameBalloon->m_textColor = 0x95EFFF;
  this->nameBalloon->m_outlineColor = 0;
  *reinterpret_cast<int*>(&this->unknown_02E4[0]) = this->nameBalloon->geometry.width;
  *reinterpret_cast<int*>(&this->unknown_02E4[4]) = this->nameBalloon->geometry.height;
  this->PlaceWindowAtActor(this->nameBalloon, reinterpret_cast<CGameActor*>(item));
}


// OFFSET: 0x00CE18A0
// FUNCTION: fn_21332
void CGameMode::ProcessActorClick(int& handled, ActorPickInfo* pick, CGameActor* actor) {
  const unsigned char selfTargetOption = g_serviceOptions.Get(0x69);
  const unsigned char forceOption = g_serviceOptions.Get(0x6D);
  ModeCursor& cursor = *reinterpret_cast<ModeCursor*>(&this->cursorAction);
  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);

  int attack;
  if (mode == 2 || mode == 4) {
    const int shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    attack = IsSupportSkill(this->unknown_040C) ? shiftHeld : shiftHeld == 0;
    cursor.Set(11, this->cursorTick);
  } else {
    attack = HIBYTE(GetAsyncKeyState(VK_SHIFT)) == 0;
    if (actor->unknown_0314 == 7) {
      cursor.Set(0, this->cursorTick);
    } else if (g_session.GetMapProperty08() == 0 && this->scene->unknown_004C == 0) {
      cursor.Set(5, this->cursorTick);
    } else if (g_session.IsLocalPlayerAid(actor->Slot50())) {
      attack = 0;
      cursor.Set(0, this->cursorTick);
    }
  }

  handled = 1;
  if (selfTargetOption) {
    attack = 1;
  }

  const std::uint32_t homunAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
  const std::uint32_t mercenaryAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
  if (g_groundMarkEnabled != 0) {
    if (g_mouse.buttons[1] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (homun && homun->HasUnknown030CEntry()) {
          AiMessage message = {};
          message.type = 3;
          message.target = actor->render.accountId;
          homun->SetUnknown0330(reinterpret_cast<const float*>(&message), 0);
        }
      } else if (g_session.unknown_5610 != 0 && g_groundMarkAid == actor->render.accountId) {
        CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (homun && homun->HasUnknown030CEntry()) {
          AiMessage message = {};
          message.type = 3;
          message.target = g_groundMarkAid;
          homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
        }
        g_session.unknown_5610 = 0;
      } else {
        cursor.Set(11, this->cursorTick);
        g_groundMarkAid = actor->render.accountId;
        g_groundMarkA = 0;
        g_groundMarkB = 0;
        g_groundMarkSet = 0;
        g_session.unknown_5610 = 1;
      }
      g_session.unknown_560C = 1;
    }

    if (g_groundMarkEnabled != 0 && g_mouse.buttons[0] == 1 && g_session.IsHomunculusSkill(this->unknown_040C)) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasUnknown030CEntry()) {
        AiMessage message = {};
        message.type = 7;
        message.level = this->unknown_0414;
        message.skill = this->unknown_040C;
        message.target = actor->render.accountId;
        homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
      }
      return;
    }
  }

  if (g_groundMarkAltEnabled != 0 && g_mouse.buttons[0] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasUnknown0310Entry()) {
        AiMessage message = {};
        message.type = 3;
        message.target = actor->render.accountId;
        mercenary->SetUnknown0330(reinterpret_cast<const float*>(&message), 1);
      }
    } else if (g_session.unknown_56AC != 0 && g_groundMarkAltAid == actor->render.accountId) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasUnknown0310Entry()) {
        AiMessage message = {};
        message.type = 3;
        message.target = g_groundMarkAltAid;
        mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
      }
      g_session.unknown_56AC = 0;
    } else {
      cursor.Set(11, this->cursorTick);
      g_groundMarkAltAid = actor->render.accountId;
      g_groundMarkAltA = 0;
      g_groundMarkAltB = 0;
      g_groundMarkAltSet = 0;
      g_session.unknown_56AC = 1;
    }
    g_session.unknown_56A8 = 1;
  }

  int& targetAid = *reinterpret_cast<int*>(&this->unknown_00EC[0x04]);
  int& forced = this->forcedAttack;
  if (attack != 0 && g_groundMarkAltEnabled != 0 && g_mouse.buttons[0] == 1 && g_session.IsMercenarySkill(this->unknown_040C)) {
    CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
    if (mercenary && mercenary->HasUnknown0310Entry()) {
      AiMessage message = {};
      message.type = 7;
      message.level = this->unknown_0414;
      message.skill = this->unknown_040C;
      message.target = actor->render.accountId;
      mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
    }
    return;
  }

  if (g_mouse.buttons[0] == 1 && attack != 0 && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
    this->SendMsg(0x1A, 1, 0, 0, 0);
    *reinterpret_cast<int*>(&this->unknown_00EC[0x08]) = targetAid;
    this->unknown_00F8 = 0;
    if (g_session.GetSceneUnknown004C() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
      this->RequestPlayerAction(targetAid, 0);
      forced = 1;
    } else {
      this->RequestPlayerAction(targetAid, 1);
      forced = 0;
    }
  } else if (forced == 0 && g_mouse.buttons[0] == 3) {
    targetAid = 0;
  }

  if ((this->unknown_00DC != g_screenWidth || this->unknown_00E0 != g_screenHeight) && forced == 0 && g_mouse.buttons[0] == 0) {
    targetAid = 0;
  }

  this->ShowActorNameBalloon(targetAid);
  if (actor->render.unknown_0070 == 3) {
    targetAid = 0;
  }
}

// OFFSET: 0x00CDD6B0
// FUNCTION: fn_21317
CGameActor* CGameMode::NewActor(int job) {
  CGameActor* actor;
  if (IsPlayerJob(job)) {
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
  if (!card.received) {
    return;
  }

  if (actor->unknown_02F8 == 1 && (this->scene->unknown_0048 == 1 || this->scene->unknown_004C == 1)) {
    card.name = "??????";
  } else {
    const std::string name = this->nameCard.Name(actor->render.accountId);
    card.name = name;
  }

  if (g_session.IsMonsterActor(actor) == 1) {
    UINameBalloonText* balloon = this->itemNameBalloon;
    unsigned char monsterType;
    if (actor->GetEffectiveJob(-1) == 20269) {
      monsterType = 0;
    } else {
      monsterType = actor->unknown_0304;
    }
    balloon->SetName(card, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, monsterType);
  } else {
    this->itemNameBalloon->SetName(card, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, 0);
  }

  const unsigned char showEmblem = g_serviceOptions.Get(0x40);
  if ((g_session.IsNpcJob(actor->GetEffectiveJob(-1)) || g_session.IsMonsterActor(actor)) && card.unknown_0094) {
    this->itemNameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.unknown_0094).c_str(), 0);
  } else if (g_session.IsAllyActor(actor->render.accountId, actor->GetEffectiveJob(-1)) && card.unknown_0094) {
    this->nameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.unknown_0094).c_str(), 0);
  } else if (actor->Slot50() && showEmblem) {
    std::string emblem = g_guildMgr->GetEmblemFilename(actor->Slot50());
    const unsigned char framedEmblem = g_serviceOptions.Get(0xF3);
    this->itemNameBalloon->SetIcon(emblem.c_str(), static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->unknown_004C == 1 && framedEmblem ? 1 : 0);
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

// OFFSET: 0x00CDFE10
// FUNCTION: fn_21328
int CGameMode::ProcessMousePick(int result, ActorPickInfo* pick) {
  if (g_sub_1319eb4 == 1 && pick) {
    UIMacroReporterWnd* reporter = static_cast<UIMacroReporterWnd*>(g_windowMgr.LookupWindow(286));
    if (reporter && reporter->unknown_010C == true && reporter->m_mode == 0) {
      reporter->sub_7EF2C0(reinterpret_cast<MacroReporterEntry*>(pick));
      return -1;
    }
  }

  if (g_windowMgr.LookupWindow(196) || reinterpret_cast<std::intptr_t>(this->unknown_0408) == 1) {
    return result;
  }

  ModeCursor& cursor = *reinterpret_cast<ModeCursor*>(&this->cursorAction);
  int& lockedPick = this->forcedAttack;
  int& pickTarget = *reinterpret_cast<int*>(&this->unknown_00EC[0x00]);
  int& clickTarget = *reinterpret_cast<int*>(&this->unknown_00EC[0x04]);

  if (this->unknown_0028 == 0) {
    if (!pick) {
      if (lockedPick != 0 || g_sub_1319eb4 == 1) {
        return result;
      }
      if (this->unknown_00DC == g_screenWidth && this->unknown_00E0 == g_screenHeight) {
        return result;
      }
      pickTarget = 0;
      clickTarget = 0;
      return result;
    }
  } else {
    cursor.Set(5, this->cursorTick);
    if (lockedPick != 0) {
      if (g_sub_1319eb4 == 1) {
        this->SendMsg(26, 0, 0, 0, 0);
      } else {
        cursor.Set(0, this->cursorTick);
        if (!pick) {
          return result;
        }
        result = 0;
      }
    } else {
      if (g_sub_1319eb4 == 2) {
        return 1;
      }
      if (!pick && clickTarget == 0 && pickTarget == 0) {
        this->unknown_0028 = 0;
      }
    }
  }

  if (lockedPick == 0) {
    if (clickTarget != 0) {
      if (pick) {
        if (pick->kind == 3) {
          cursor.Set(0, this->cursorTick);
          return 0;
        }
        if (pick->kind == 4) {
          cursor.Set(0, this->cursorTick);
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
      if (lockedPick != 0 && *reinterpret_cast<const int*>(&actor->render.unknown_0060) == 3) {
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
#include "UI/Shop/UIMerchantShopMakeWnd.h"

void __stdcall SendMacroUserInfoPacket(std::uint16_t first, std::uint16_t second, std::uint8_t third);

// OFFSET: 0x00CE0210
// FUNCTION: fn_21329
int CGameMode::ProcessGroundClick(int result) {
  const RenderProjection projection = g_renderer->viewport.Projection();
  const auto drawCell = [](GroundGeometry* ground, const float* matrix, int column, int row, std::uint32_t color) {
    void** groundVtable = *reinterpret_cast<void***>(ground);
    reinterpret_cast<void(__thiscall*)(GroundGeometry*, const float*, int, int, std::uint32_t)>(groundVtable[14])(ground, matrix, column, row, color);
  };

  UIWindow* macroArea = g_windowMgr.LookupWindow(0x11E);
  if (macroArea) {
    const unsigned char* macroFields = reinterpret_cast<const unsigned char*>(macroArea);
    if (macroFields[0x10C] == 1 && *reinterpret_cast<const int*>(macroFields + 0xEC) == 1) {
      int column = -1;
      int row = -1;
      PickScreenTerrain(projection, g_screenWidth, g_screenHeight, this->camera->frame.eye, this->camera->frame.inverse.values, this->scene->spatial, column, row);
      const int radius = *reinterpret_cast<const int*>(macroFields + 0xF4);
      for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
          drawCell(this->scene->ground, this->camera->frame.view.values, column + dx, row + dy, 0x660000FF);
        }
      }

      if (g_mouse.buttons[1] == 1) {
        SendMacroUserInfoPacket(static_cast<std::uint16_t>(column), static_cast<std::uint16_t>(row), static_cast<std::uint8_t>(radius));
        return -1;
      }
    }
  }

  UIMerchantShopMakeWnd* shop = reinterpret_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_02FC);
  if (!shop && g_windowMgr.unknown_040C) {
    shop = reinterpret_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_040C);
  }

  bool shopSuspended = false;
  if (shop) {
    if (shop->suspended == 1) {
      shopSuspended = true;
      int column = -1;
      int row = -1;
      PickScreenTerrain(projection, g_screenWidth, g_screenHeight, this->camera->frame.eye, this->camera->frame.inverse.values, this->scene->spatial, column, row);
      drawCell(this->scene->ground, this->camera->frame.view.values, column, row, 0x66FF0000);
      if (g_mouse.buttons[0] == 1) {
        shop->SetSuspended(0);
        shop->SetSelection(static_cast<short>(column), static_cast<short>(row));
        if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xB63), 2, true, false, 0, "Message", 280, 120, 0) == 0xBB) {
          shop->SendShopRequest();
          return -1;
        }
        shop->SetSuspended(1);
      }
    } else if (shop->HasSelection() == true) {
      drawCell(this->scene->ground, this->camera->frame.view.values, shop->unknown_018A, shop->unknown_018C, 0x66FF0000);
    }
  }

  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
  if (mode == 2 || mode == 3 || mode == 4) {
    return result;
  }

  int column = -1;
  int row = -1;
  if (!PickScreenTerrain(projection, g_screenWidth, g_screenHeight, this->camera->frame.eye, this->camera->frame.inverse.values, this->scene->spatial, column, row)) {
    return result;
  }

  const std::uint32_t homunAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
  const std::uint32_t mercenaryAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
  if (g_groundMarkEnabled != 0) {
    if (HIBYTE(GetAsyncKeyState(VK_MENU)) && g_mouse.buttons[1] == 1) {
      if (g_session.unknown_560C != 0) {
        g_session.unknown_560C = 0;
        return result;
      }

      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        if (homun && homun->HasUnknown030CEntry()) {
          AiMessage message = {};
          message.type = 1;
          message.x = column;
          message.y = row;
          homun->SetUnknown0330(reinterpret_cast<const float*>(&message), 0);
        }
        return result;
      }

      g_groundMarkA = column;
      g_groundMarkB = row;
      g_groundMarkAid = 0;
      g_session.unknown_5610 = 0;
      if (homun && homun->HasUnknown030CEntry()) {
        AiMessage message = {};
        message.type = 1;
        message.x = column;
        message.y = row;
        homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
      }
      g_groundMarkSet = 1;
      return result;
    }

    if (g_groundMarkEnabled != 0 && g_mouse.buttons[1] == 1) {
      g_session.ClearHomunStatus();
    }
  }

  if (g_groundMarkAltEnabled != 0) {
    if (HIBYTE(GetAsyncKeyState(VK_MENU)) && g_mouse.buttons[0] == 1) {
      if (g_session.unknown_56A8 != 0) {
        g_session.unknown_56A8 = 0;
        return result;
      }

      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        if (mercenary && mercenary->HasUnknown0310Entry()) {
          AiMessage message = {};
          message.type = 1;
          message.x = column;
          message.y = row;
          mercenary->SetUnknown0330(reinterpret_cast<const float*>(&message), 1);
        }
        return result;
      }

      g_groundMarkAltA = column;
      g_groundMarkAltB = row;
      g_groundMarkAltAid = 0;
      g_session.unknown_56AC = 0;
      if (mercenary && mercenary->HasUnknown0310Entry()) {
        AiMessage message = {};
        message.type = 1;
        message.x = column;
        message.y = row;
        mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
      }
      g_groundMarkAltSet = 1;
      return result;
    }

    if (g_groundMarkAltEnabled != 0 && g_mouse.buttons[0] == 1) {
      g_session.ResetMercenaryStats();
      if (reinterpret_cast<std::intptr_t>(this->unknown_0408) == 1 && g_session.IsMercenarySkill(this->unknown_040C)) {
        CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
        if (mercenary && mercenary->HasUnknown0310Entry()) {
          const std::uint32_t state = mercenary->render.unknown_0070;
          if (state == 1 || state == 0 || state == 4) {
            AiMessage message;
            message.type = 8;
            message.x = column;
            message.y = row;
            message.level = this->unknown_0414;
            message.skill = this->unknown_040C;
            message.target = 0;
            mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
            return 1;
          }
        }
      }
    }
  }

  if (reinterpret_cast<std::intptr_t>(this->unknown_0408) == 1 && g_mouse.buttons[0] == 1 && (result == 0 || result == 2)) {
    result = 1;
    if (g_session.IsHomunculusSkill(this->unknown_040C) == 1 && g_groundMarkEnabled == 1) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasUnknown030CEntry() == true) {
        const std::uint32_t state = homun->render.unknown_0070;
        if (state == 0 || state == 1 || state == 4) {
          AiMessage message;
          message.type = 8;
          message.x = column;
          message.y = row;
          message.level = this->unknown_0414;
          message.skill = this->unknown_040C;
          message.target = 0;
          homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
          return 1;
        }
      }
    }

    if (this->scene->player) {
      const int skill = this->unknown_040C;
      this->scene->player->SendMsg(nullptr, 0x41, 0, skill, skill >> 31, column, column >> 31, row, row >> 31, 0, 0, 0, 0, 0, 0);
      const int level = this->unknown_0414;
      this->scene->player->SendMsg(nullptr, 0x5A, 0, level, level >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
    return result;
  }
  if (this->unknown_0244 != 0) {
    return result;
  }
  if (result != 0 && result != 2) {
    return result;
  }
  if (this->drag.source != 0) {
    return result;
  }

  if (*reinterpret_cast<const int*>(g_session.unknown_546C) != 0) {
    g_groundOverlayA = column;
    g_groundOverlayB = row;
    g_groundOverlaySkip = 0;
    int skip = 1;
    const TerrainHeightCell* cell = this->scene->terrain->Grid().CellAt(column, row);
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

  std::uint32_t& clickTick = *reinterpret_cast<std::uint32_t*>(&this->unknown_00D8[0]);
  const bool elapsed = timeGetTime() > clickTick + 600;
  if (this->scene->player->unknown_0508 == 0 && g_mouse.buttons[0] == 2) {
    if (!elapsed) {
      return result;
    }
    if (this->unknown_00F8 == 0) {
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
  if (HIBYTE(GetAsyncKeyState(VK_SHIFT)) || g_windowMgr.window_027C || this->scene->player->render.unknown_0070 == 6) {
    this->scene->player->SendMsg(nullptr, 0x10, 0, column, column >> 31, row, row >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    return result;
  }

  bool findClosest = true;
  CPlayer* player = this->scene->player;
  const TerrainHeightCell* cell = this->scene->terrain->Grid().CellAt(column, row);
  if (cell && cell->type != 1 && cell->type != 5 &&
      (column != *reinterpret_cast<const int*>(&player->unknown_04F8) || row != player->unknown_04FC)) {
    int startColumn;
    int startRow;
    int startSubColumn;
    int startSubRow;
    this->scene->terrain->Grid().WorldSubcell(player->render.position.x, player->render.position.z, startColumn, startRow, startSubColumn, startSubRow);
    if (this->CanFindPath(startColumn, startRow, startSubColumn, startSubRow, column, row) && !this->IsRegisterCell(column, row)) {
      findClosest = false;
    }
  }

  if (findClosest && !this->GetClosestValidCell(&column, &row)) {
    return result;
  }

  if (g_windowMgr.unknown_0210) {
    g_windowMgr.CloseWindow(0x12);
  }
  if (this->scene->player) {
    this->scene->player->SendMsg(nullptr, 0x11, 0, column, column >> 31, row, row >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  return result;
}

#include "Client/Mode/ModeTooltip.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/World/WorldOcclusion.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Window/InputFocus.h"

#include "Client/Mode/ModeTooltip.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/World/WorldOcclusion.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Window/InputFocus.h"

#include "Client/Application/ApplicationMessages.h"
#include "Client/ModeFade.h"
#include "Contents/ActorNameCard.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/GuildMgr.h"
#include "Contents/MapInfoMgr.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Sound/SoundMgr.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Stove/StovePCSDK.h"
#include "Client/Timer.h"
#include "Network/GameTickPacket.h"
#include "Render/ActionSpriteDrawing.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Utility/GGHashMgr.h"
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

int sub_C7F5A0();

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

  if (timeGetTime() > *reinterpret_cast<std::uint32_t*>(this->unknown_01E0) + 1000) {
    g_windowMgr.InvalidateRegisteredWindows();
    *reinterpret_cast<std::uint32_t*>(this->unknown_01E0) = timeGetTime();
  }

  if (handled == 2) {
    handled = 0;
  }

  if (this->unknown_0408 != nullptr) {
    if (*reinterpret_cast<int*>(&g_session.unknown_544C[0x00]) == 0) {
      if (g_mouse.wheel > 0) {
        this->SendMsg(0x5D, 1, 0, 0, 0);
      } else if (g_mouse.wheel < 0) {
        this->SendMsg(0x5E, 1, 0, 0, 0);
      }
    }
    *reinterpret_cast<int*>(&g_session.unknown_544C[0x00]) = 0;
  }

  if (g_mouse.buttons[0] == 3) {
    if (this->scene->player) {
      this->scene->player->SendMsg(nullptr, 0xF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    this->SendMsg(0x11, g_mouse.x, g_mouse.y, 0, 0);
    UIMenuWnd* menu = static_cast<UIMenuWnd*>(g_windowMgr.unknown_0210);
    if (menu) {
      if (menu->unknown_00DC) {
        menu->unknown_00DC = 0;
      } else {
        g_windowMgr.CloseWindow(0x12);
      }
    }
    this->unknown_00F8 = 1;
  }

  g_pickModifierKey = HIBYTE(GetAsyncKeyState(VK_SHIFT));
  ActorPickInfo* pick = g_actorPickNode.Pick(static_cast<float>(g_mouse.x), static_cast<float>(g_mouse.y));
  UIWindow* blocker = g_windowMgr.LookupWindow(0x271B);
  if (!blocker) {
    this->OpenActorMenu(pick, handled);
  }

  this->cursorOffsetX = 0.0f;
  this->cursorOffsetY = 0.0f;
  if (handled == 0) {
    const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
    g_session.UpdateUnknown1708(mode == 2 || mode == 4);
    if (pick) {
      bool offset;
      if (this->unknown_040C == 0xE7) {
        offset = g_session.IsSkillIdInRange6001(pick->job) && pick->kind == 4;
      } else {
        int eligible;
        if (g_session.unknown_1708 && reinterpret_cast<CGameActor*>(&g_session)->IsMonsterClass(pick->job)) {
          eligible = 1;
        } else if (g_serviceOptions.Get(0x2F)) {
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

  if (this->scene && this->scene->player && g_renderer) {
    MarkScreenWorldOcclusion(
      g_renderer->viewport.Projection(), this->scene->player->render.screenX, this->scene->player->render.screenY, g_renderer->viewport.uiScaleY, this->camera->frame.eye,
      this->camera->frame.inverse.values, this->scene->player->render.position.y, this->camera->camera.distance, this->scene->spatial, *this->scene->ground,
      this->unknown_0458 != 0
    );
  }

  if (this->unknown_0250 == 0 && (this->unknown_024C == 0 || this->scene->player->unknown_0264[3] != 0) && !blocker && g_windowMgr.window_0288 == nullptr &&
      !g_windowMgr.LookupWindow(0x169)) {
    handled = this->ProcessGroundClick(this->ProcessMousePick(handled, pick));
    this->UpdateHoverNameBalloon();
    if (handled != 1 && g_mouse.buttons[0] == 1) {
      g_windowMgr.SetFocus(nullptr);
    }
  } else if (pick && handled == 0) {
    this->ShowActorNameBalloon(pick->accountId);
  }

  if ((handled == 1 || g_mouse.buttons[0] == 1) && g_mouse.buttons[0] == 1) {
    g_unknown173AD3F = false;
    this->SendMsg(0x47, 0, 0, 0, 0);
    if (timeGetTime() > this->unknown_0248 + 5000) {
      this->unknown_0244 = 0;
    }
    *reinterpret_cast<std::uint32_t*>(this->unknown_00D8) = timeGetTime();
  }

  const bool statusIcons = g_serviceOptions.Get(0xC9) != 0;
  if (Replay::Instance().state != 6 && statusIcons) {
    this->ProcessStatusIconTooltip(g_mouse.x, g_mouse.y);
  }

  if (handled == 0) {
    ModeCursor& cursor = *reinterpret_cast<ModeCursor*>(&this->cursorAction);
    if (this->unknown_0408 != nullptr) {
      cursor.Set(10, this->cursorTick);
    } else if (g_mouse.buttons[1] != 2) {
      int column = -1;
      int row = -1;
      PickScreenTerrain(
        g_renderer->viewport.Projection(), g_mouse.x, g_mouse.y, this->camera->frame.eye, this->camera->frame.inverse.values, this->scene->spatial,
        column, row
      );
      const TerrainHeightCell* cell = this->scene->terrain->Grid().CellAt(column, row);
      if (cell && cell->type != 1 && cell->type != 5) {
        cursor.Set(0, this->cursorTick);
      } else {
        cursor.Set(13, this->cursorTick);
      }
    }
  }

  this->unknown_00DC = g_mouse.x;
  this->unknown_00E0 = g_mouse.y;
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

    if (actor->render.visible == 1 && actor->render.enabled == 1 && !StateFlag02(actor->Slot13())) {
      this->ShowHoverActorName(actor);
    }
  } else {
    if (!this->unknown_0028) {
      this->SendMsg(5, 0, 0, 0, 0);
      return;
    }

    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<std::uint32_t*>(&this->unknown_00EC[8])));
    if (!actor) {
      return;
    }

    if (!actor->unknown_0314 && actor->render.visible == 1 && actor->render.enabled == 1 && !StateFlag02(actor->Slot13())) {
      *reinterpret_cast<int*>(&this->unknown_02B8[0x10]) = 0;
      this->ShowHoverActorName(actor);
    }
  }

  if (actor->GetEffectiveJob(-1) != 45 &&
      (actor->unknown_0314 || (actor->render.visible == 1 && actor->render.enabled == 1 && !StateFlag02(actor->Slot13())))) {
    this->SendMsg(4, static_cast<int>(actor->render.position.x), static_cast<int>(actor->render.position.z), 0, 0);
    return;
  }

  this->SendMsg(5, 0, 0, 0, 0);
}

#include "Client/Browser/WebBrowser.h"
#include "Client/Input/InputCharset.h"
#include "Client/Timer.h"
#include "Contents/ClanInfo.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/NavigationManager.h"
#include "Network/TextEncoding.h"
#include "UI/Chat/ChatSuggestions.h"
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

// OFFSET: 0x00CE3D30
// FUNCTION: fn_21335
void CGameMode::ProcessActorPick(int& handled, ActorPickInfo* pick, CGameActor* actor) {
  const unsigned char selfTargetOption = g_serviceOptions.Get(0x69);
  const unsigned char forceOption = g_serviceOptions.Get(0x6D);
  ModeCursor& cursor = *reinterpret_cast<ModeCursor*>(&this->cursorAction);
  const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);

  int skillMode;
  int shiftHeld;
  bool optionalSelf;
  int support;
  int attack;
  if (mode == 2 || mode == 4) {
    skillMode = 1;
    shiftHeld = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    optionalSelf = IsOptionalSelfTargetSkill(this->unknown_040C);
    support = IsSupportSkill(this->unknown_040C);
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
    if (this->scene->unknown_0050 != 0) {
      if (actor && g_session.unknown_0F24[0] != actor->render.unknown_00FB) {
        attack = 1;
      }
    } else {
      if (g_session.GetMapProperty07()) {
        if (!g_session.IsPartyMember(actor->render.accountId, true)) {
          attack = 1;
        }
      } else if (g_session.GetMapProperty08()) {
        if (!g_session.IsLocalPlayerAid(actor->Slot50())) {
          attack = 1;
        }
      }

      CWorld* scene = this->scene;
      if (scene->unknown_004C == 0) {
        if (pick->unknown_0024 != 0 || scene->unknown_0048 != 0 || scene->unknown_0054 != 0 ||
            *reinterpret_cast<const int*>(&g_session.unknown_5454[4]) != 0) {
          if (!g_session.IsPartyMember(actor->render.accountId, true)) {
            attack = 1;
          }
        }
      } else if (actor) {
        GuildInfo* guildInfo = &g_guildInfo;
        bool friendly = true;
        bool known = true;
        switch (actor->unknown_0314) {
        case 8:
        case 9:
        case 10:
        case 13:
        case 14: {
          CGameActor* summoner = static_cast<CGameActor*>(scene->GetGameActorByAID(actor->unknown_02EC));
          if (!summoner) {
            known = this->scene->player->render.accountId == static_cast<std::uint32_t>(actor->unknown_02EC);
          } else if (!g_session.IsLocalPlayerAid(summoner->Slot50())) {
            friendly = guildInfo->HasRelatedGuild(summoner->Slot50());
          }
          break;
        }
        default:
          if (!g_session.IsLocalPlayerAid(actor->Slot50())) {
            friendly = guildInfo->HasRelatedGuild(actor->Slot50());
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
    }
  }

  if (resetCursor) {
    cursor.Set(0, this->cursorTick);
  } else if (attack && actor->render.unknown_0070 != 3) {
    cursor.Set(skillMode ? 11 : 5, this->cursorTick);
    handled = 1;
  } else if (skillMode && actor->render.unknown_0070 != 3) {
    cursor.Set(10, this->cursorTick);
  } else {
    cursor.Set(0, this->cursorTick);
  }

  const std::uint32_t homunAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
  const std::uint32_t mercenaryAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
  const bool engaged = (attack || this->scene->unknown_004C != 0) && actor->render.unknown_0070 != 3 && !skillMode;
  if (engaged && g_groundMarkEnabled != 0 && g_mouse.buttons[1] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasUnknown030CEntry()) {
        AiMessage message = {};
        message.type = 3;
        message.target = actor->render.accountId;
        homun->SetUnknown0330(reinterpret_cast<const float*>(&message), 0);
      }
    } else if (g_session.unknown_5610 != 0 && g_groundMarkAid == actor->render.accountId) {
      CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
      if (homun && homun->HasUnknown030CEntry()) {
        AiMessage message = {};
        message.type = 3;
        message.target = g_groundMarkAid;
        homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
      }
      g_session.unknown_5610 = 0;
    } else {
      cursor.Set(11, this->cursorTick);
      g_groundMarkAid = actor->render.accountId;
      g_groundMarkA = 0;
      g_groundMarkB = 0;
      g_groundMarkSet = 0;
      g_session.unknown_5610 = 1;
    }
    g_session.unknown_560C = 1;
  }

  if (g_groundMarkEnabled != 0 && g_mouse.buttons[0] == 1 && g_session.IsHomunculusSkill(this->unknown_040C)) {
    CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
    if (homun && homun->HasUnknown030CEntry()) {
      AiMessage message = {};
      message.type = 7;
      message.level = this->unknown_0414;
      message.skill = this->unknown_040C;
      message.target = actor->render.accountId;
      homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
    }
    return;
  }

  if (engaged && g_groundMarkAltEnabled != 0 && g_mouse.buttons[0] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasUnknown0310Entry()) {
        AiMessage message = {};
        message.type = 3;
        message.target = actor->render.accountId;
        mercenary->SetUnknown0330(reinterpret_cast<const float*>(&message), 1);
      }
    } else if (g_session.unknown_56AC != 0 && g_groundMarkAltAid == actor->render.accountId) {
      CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
      if (mercenary && mercenary->HasUnknown0310Entry()) {
        AiMessage message = {};
        message.type = 3;
        message.target = g_groundMarkAltAid;
        mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
      }
      g_session.unknown_56AC = 0;
    } else {
      cursor.Set(11, this->cursorTick);
      g_groundMarkAltAid = actor->render.accountId;
      g_groundMarkAltA = 0;
      g_groundMarkAltB = 0;
      g_groundMarkAltSet = 0;
      g_session.unknown_56AC = 1;
    }
    g_session.unknown_56A8 = 1;
  }

  if (attack != 0 && g_groundMarkAltEnabled != 0 && g_mouse.buttons[0] == 1 && g_session.IsMercenarySkill(this->unknown_040C)) {
    CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
    if (mercenary && mercenary->HasUnknown0310Entry()) {
      AiMessage message = {};
      message.type = 7;
      message.level = this->unknown_0414;
      message.skill = this->unknown_040C;
      message.target = actor->render.accountId;
      mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
    }
    return;
  }

  int& forced = this->forcedAttack;
  if (g_mouse.buttons[0] == 1 && attack != 0 && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
    this->SendMsg(0x1A, 1, 0, 0, 0);
    *reinterpret_cast<std::uint32_t*>(&this->unknown_00EC[0x08]) = actor->render.accountId;
    *reinterpret_cast<int*>(&this->unknown_00EC[0x0C]) = 0;
    if (g_session.GetSceneUnknown004C() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
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
    this->SendMsg(0x1A, 1, 0, 0, 0);
    g_hoverActorGid = actor->Slot42();
    *reinterpret_cast<int*>(&this->unknown_00EC[0x0C]) = 0;
  }

  int& balloonAid = *reinterpret_cast<int*>(&this->unknown_00EC[0x00]);
  if (this->unknown_00DC == g_screenWidth && this->unknown_00E0 == g_screenHeight) {
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

  if (this->unknown_05D0) {
    this->unknown_05D0->Show(0);
  }

  if (g_activeSkillEffects.begin() == g_activeSkillEffects.end()) {
    return;
  }

  int& hoveredId = this->unknown_01F8;
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

  const int id = g_statusIconIds[slot];
  if (id == -1) {
    hoveredId = -1;
    if (this->unknown_05D0) {
      static_cast<UIBalloonText*>(this->unknown_05D0)->SetText(" ", 35);
    }
    return;
  }

  int hasLimit = 0;
  int limitLine = -1;
  g_luaScript->Call("GetTimeLimitInfo", "d>dd", id, &hasLimit, &limitLine);

  std::string timeText;
  int activeId = id;
  if (hasLimit != 0) {
    timeText = this->GetActiveSkillEffectTimeText(id, &activeId);
  }

  if (hoveredId == id && this->unknown_05D0 && id != 0x175 && id != 0x2B7) {
    if (firstLine(this->unknown_05D0).size() != 0) {
      this->unknown_05D0->Show(1);
    }
  } else {
    if (!this->unknown_05D0) {
      this->unknown_05D0 = new UITransBalloonText();
      this->unknown_05D0->SetSize(50, 50);
      g_windowMgr.AddRendered(this->unknown_05D0);
      static_cast<UITransBalloonText*>(this->unknown_05D0)->SetFontHeight(11);
      static_cast<UITransBalloonText*>(this->unknown_05D0)->m_borderColor = 0xC0C0C0;
    } else {
      static_cast<UIBalloonText*>(this->unknown_05D0)->SetText(" ", 35);
    }

    UITransBalloonText* balloon = static_cast<UITransBalloonText*>(this->unknown_05D0);
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
        } while (!g_luaScript->Call("GetStateIconDescript", "dd>sddd", activeId, line, &text, &red, &green, &blue));

        std::string description(text);
        if (description.size() == 0) {
          break;
        }

        if (line == limitLine) {
          description = FormatOwnedString(description.c_str(), timeText.c_str());
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
        if (timeGetTime() - g_session.unknown_7ED4 > 10000) {
          g_session.unknown_7ED0 = 0;
        } else if (g_session.unknown_7ED0 != 0) {
          extra = FormatOwnedString(LookupMessage(0xBD2), g_session.unknown_7ED0);
        }

        if (extra.size() != 0) {
          balloon->AddLine(extra.c_str(), 0);
        }
      }
    } else {
      {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.unknown_16EC);
        if (item.unknown_0000[1] != 0) {
          extra = FormatOwnedString("%s : %d", item.DisplayName(0).c_str(), item.quantity);
          color = 0xFABEBE;
        }
      }

      if (extra.size() != 0) {
        balloon->SetLine(extra.c_str(), color);
      }
    }

    const std::string first = firstLine(this->unknown_05D0);
    if (!(first.size() == 1 && first[0] == ' ')) {
      this->unknown_05D0->Show(1);
      const int windowHeight = this->unknown_05D0->geometry.height;
      const int left = right - 30 + (slot / rows) * -45 - this->unknown_05D0->geometry.width;
      const int top = (slot % rows) * 35;
      int windowY = g_renderer->viewport.height - windowHeight;
      if (windowHeight + 171 + top <= g_renderer->viewport.height) {
        windowY = top + 171;
      }
      this->unknown_05D0->Move(left, windowY);
    }

    hoveredId = id;
  }

  if (hasLimit != 0 && timeText.size() != 0 && limitLine != -1 && this->unknown_05D0) {
    const char* text = nullptr;
    if (g_luaScript->Call("GetStateIconDescript", "dd>s", activeId, limitLine, &text)) {
      std::string description(text);
      description = FormatOwnedString(description.c_str(), timeText.c_str());
      static_cast<UITransBalloonText*>(this->unknown_05D0)->ChangeText(description.c_str(), limitLine - 1);
    }
  }
}

// OFFSET: 0x00CE2880
// FUNCTION: fn_21334
int CGameMode::HandleActorPick(ActorPickInfo* pick, int result) {
  const unsigned char selfTargetOption = g_serviceOptions.Get(0x69);
  const unsigned char forceOption = g_serviceOptions.Get(0x6D);
  ModeCursor& cursor = *reinterpret_cast<ModeCursor*>(&this->cursorAction);
  const int& siegeMode = *reinterpret_cast<const int*>(&g_session.unknown_5454[4]);
  const std::uint32_t& homunAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
  const std::uint32_t& mercenaryAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
  GuildInfo* guildInfo = &g_guildInfo;

  if (pick->accountId != g_session.m_aid) {
    switch (pick->kind) {
    case 0:
      if (pick->job != 45 && g_session.IsAllyActor(pick->accountId, pick->job) == 1) {
        cursor.Set(1, this->cursorTick);
        if (g_mouse.buttons[0] == 2) {
          result = 2;
        } else {
          result = 1;
        }
      } else if (pick->job == 45) {
        cursor.Set(7, this->cursorTick);
        result = 2;
      } else {
        result = 1;
        if (IsPlayerJob(pick->job)) {
          const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
          int skillMode;
          int shift;
          int support;
          int attack;
          if (mode == 2 || mode == 4) {
            skillMode = 1;
            shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
            support = IsSupportSkill(this->unknown_040C);
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
          if (scene->unknown_0050 == 0) {
            if (g_session.GetMapProperty07()) {
              if (!g_session.IsPartyMember(pick->accountId, true)) {
                attack = 1;
              }
            } else if (g_session.GetMapProperty08() && actor != nullptr) {
              if (!g_session.IsLocalPlayerAid(actor->Slot50())) {
                attack = 1;
              }
            }

            if (this->scene->unknown_004C != 0) {
              if (actor != nullptr) {
                if (!g_session.IsLocalPlayerAid(actor->Slot50()) && !guildInfo->HasRelatedGuild(actor->Slot50())) {
                  attack = 1;
                } else {
                  attack = 0;
                  if (skillMode) {
                    const bool optionalSelf = IsOptionalSelfTargetSkill(this->unknown_040C);
                    attack = support;
                    if ((selfTargetOption != 0 || shift != 0) && optionalSelf == true) {
                      attack = 1;
                    }
                  }
                }
              }
            } else if (pick->unknown_0024 != 0 || this->scene->unknown_0048 != 0 || this->scene->unknown_0054 != 0 || siegeMode != 0) {
              if (!g_session.IsPartyMember(pick->accountId, true)) {
                attack = 1;
              }
            }
          } else if (actor != nullptr && g_session.unknown_0F24[0] != actor->render.unknown_00FB) {
            attack = 1;
          }

          const int state = actor != nullptr ? actor->render.unknown_0070 : 3;
          if (attack && state != 3) {
            cursor.Set(skillMode ? 11 : 5, this->cursorTick);
            result = 1;
            *reinterpret_cast<int*>(&this->unknown_00EC[0]) = pick->accountId;
          } else {
            cursor.Set(skillMode && state != 3 ? 10 : 0, this->cursorTick);
            result = 0;
            if (g_mouse.buttons[0] == 0 && this->unknown_0028 == 0 && *reinterpret_cast<int*>(&this->unknown_00EC[4]) == 0) {
              *reinterpret_cast<int*>(&this->unknown_00EC[0]) = pick->accountId;
            }
          }
        } else if (g_session.IsNpcJob(pick->job) == true) {
          cursor.Set(1, this->cursorTick);
          if (g_mouse.buttons[0] == 2) {
            result = 2;
          }
        } else if (reinterpret_cast<CGameActor*>(&g_session)->IsMonsterClass(pick->job)) {
          const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
          int skillMode;
          int attack;
          if (mode == 2 || mode == 4) {
            skillMode = 1;
            const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
            attack = shift == 0;
            if (IsSupportSkill(this->unknown_040C)) {
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
          if (g_session.GetMapProperty08() && actor != nullptr && g_session.IsLocalPlayerAid(actor->Slot50())) {
            attack = 0;
          }
          if (this->scene->unknown_004C != 0 && actor != nullptr && g_session.IsLocalPlayerAid(actor->Slot50())) {
            attack = 0;
          }

          if (this->unknown_00F8 != 0) {
            if (attack) {
              if (skillMode) {
                cursor.Set(11, this->cursorTick);
              } else if (actor != nullptr && actor->unknown_0314 == 7) {
                cursor.Set(0, this->cursorTick);
              } else {
                cursor.Set(5, this->cursorTick);
              }
            } else {
              cursor.Set(skillMode ? 10 : 0, this->cursorTick);
            }
          }

          if (g_mouse.buttons[0] == 0 && this->unknown_0028 == 0) {
            *reinterpret_cast<int*>(&this->unknown_00EC[4]) = pick->accountId;
          }
          if (g_mouse.buttons[0] == 2 || g_mouse.buttons[1] == 2) {
            result = 2;
          }
        }
      }
      break;
    case 2: {
      result = 1;
      int action;
      if (g_session.GetSceneUnknown004C()) {
        switch (pick->job) {
        case 0x8D:
        case 0x8F:
        case 0x90:
        case 0x91:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0xBE:
        case 0xBF:
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xCB:
        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD6:
        case 0xD7:
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDE:
        case 0xE5:
        case 0xE6:
        case 0xE7:
        case 0xE8:
        case 0xE9:
        case 0xF0:
        case 0x108:
        case 0x5174:
        case 0x5175:
        case 0x5176:
        case 0x5177:
          action = reinterpret_cast<std::intptr_t>(this->unknown_0408) == 5 ? 10 : 5;
          break;
        default:
          action = reinterpret_cast<std::intptr_t>(this->unknown_0408) == 5 ? 10 : 0;
          break;
        }
      } else {
        switch (pick->job) {
        case 0x8D:
        case 0x8F:
        case 0x98:
        case 0xDA:
        case 0xDE:
        case 0xE6:
        case 0x5174:
        case 0x5175:
        case 0x5176:
        case 0x5177:
          action = reinterpret_cast<std::intptr_t>(this->unknown_0408) == 5 ? 10 : 5;
          break;
        default:
          action = reinterpret_cast<std::intptr_t>(this->unknown_0408) == 5 ? 10 : 0;
          break;
        }
      }
      cursor.Set(action, this->cursorTick);
      if (g_mouse.buttons[0] == 2) {
        result = 2;
      }
      break;
    }
    case 1:
      result = 1;
      cursor.Set(9, this->cursorTick);
      if (g_mouse.buttons[0] == 2) {
        result = 2;
      }
      break;
    case 3:
      result = 0;
      cursor.Set(0, this->cursorTick);
      break;
    case 4: {
      result = 0;
      cursor.Set(0, this->cursorTick);
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
      if (actor != nullptr && actor->render.unknown_0070 != 3) {
        if (g_session.m_aid == static_cast<std::uint32_t>(actor->unknown_02EC)) {
          if (actor->unknown_0314 == 8 && this->unknown_040C == 0xE7) {
            result = 1;
            cursor.Set(11, this->cursorTick);
            *reinterpret_cast<int*>(&this->unknown_00EC[4]) = pick->accountId;
          }
        } else {
          CWorld* scene = this->scene;
          if (scene->unknown_0048 == 1 || siegeMode == 1 || scene->unknown_004C == 1) {
            if (scene->unknown_004C == 1) {
              CGameActor* owner = static_cast<CGameActor*>(scene->GetGameActorByAID(actor->unknown_02EC));
              if (owner == nullptr || g_session.IsLocalPlayerAid(owner->Slot50()) == 1 || guildInfo->HasRelatedGuild(owner->Slot50()) == true) {
                break;
              }
            }
            result = 1;
            cursor.Set(5, this->cursorTick);
            *reinterpret_cast<int*>(&this->unknown_00EC[4]) = pick->accountId;
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
    if (pick->job == 45) {
      return result;
    }

    if (g_session.IsAllyActor(pick->accountId, pick->job) == 1) {
      const int state = this->scene->player->render.unknown_0070;
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
      this->scene->player->SendMsg(nullptr, 16, 0, column, column >> 31, row, row >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      if (g_windowMgr.HasPendingDialog() == true) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFBB)), 255, 25, 0, 0);
        return result;
      }

      ContactNpcPacket packet;
      packet.packetType = ID_CZ_CONTACTNPC;
      packet.npcId = pick->accountId;
      packet.type = 1;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      return result;
    }

    const int job = pick->job;
    if (reinterpret_cast<CGameActor*>(&g_session)->IsMonsterClass(job)) {
      const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
      int attack;
      if (mode == 2 || mode == 4) {
        const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
        const bool support = IsSupportSkill(this->unknown_040C);
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

      if (g_groundMarkEnabled && g_mouse.buttons[1] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
        if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
          CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
          if (homun != nullptr && homun->HasUnknown030CEntry()) {
            AiMessage message = {};
            message.type = 3;
            message.target = pick->accountId;
            homun->SetUnknown0330(reinterpret_cast<const float*>(&message), 0);
          }
        } else if (g_session.unknown_5610 != 0 && g_groundMarkAid == pick->accountId) {
          CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
          if (homun != nullptr && homun->HasUnknown030CEntry()) {
            AiMessage message = {};
            message.type = 3;
            message.target = g_groundMarkAid;
            homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
          }
          g_session.unknown_5610 = 0;
        } else {
          cursor.Set(11, this->cursorTick);
          g_groundMarkAid = pick->accountId;
          g_groundMarkA = 0;
          g_groundMarkB = 0;
          g_groundMarkSet = 0;
          g_session.unknown_5610 = 1;
        }
        g_session.unknown_560C = 1;
      }

      if (g_groundMarkEnabled && g_mouse.buttons[0] == 1 && g_session.IsHomunculusSkill(this->unknown_040C)) {
        CGameActor* homun = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (homun != nullptr && homun->HasUnknown030CEntry()) {
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->unknown_0414;
          message.skill = this->unknown_040C;
          message.target = pick->accountId;
          homun->SetUnknown0318(reinterpret_cast<const float*>(&message), 0);
        }
        return result;
      }

      if (g_groundMarkAltEnabled && g_mouse.buttons[0] == 1 && HIBYTE(GetAsyncKeyState(VK_MENU))) {
        if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
          CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
          if (mercenary != nullptr && mercenary->HasUnknown0310Entry()) {
            AiMessage message = {};
            message.type = 3;
            message.target = pick->accountId;
            mercenary->SetUnknown0330(reinterpret_cast<const float*>(&message), 1);
          }
        } else if (g_session.unknown_56AC != 0 && g_groundMarkAltAid == pick->accountId) {
          CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
          if (mercenary != nullptr && mercenary->HasUnknown0310Entry()) {
            AiMessage message = {};
            message.type = 3;
            message.target = g_groundMarkAltAid;
            mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
          }
          g_session.unknown_56AC = 0;
        } else {
          cursor.Set(11, this->cursorTick);
          g_groundMarkAltAid = pick->accountId;
          g_groundMarkAltA = 0;
          g_groundMarkAltB = 0;
          g_groundMarkAltSet = 0;
          g_session.unknown_56AC = 1;
        }
        g_session.unknown_56A8 = 1;
      }

      if (attack && g_groundMarkAltEnabled && g_mouse.buttons[0] == 1 && g_session.IsMercenarySkill(this->unknown_040C)) {
        CGameActor* mercenary = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
        if (mercenary != nullptr && mercenary->HasUnknown0310Entry()) {
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->unknown_0414;
          message.skill = this->unknown_040C;
          message.target = pick->accountId;
          mercenary->SetUnknown0318(reinterpret_cast<const float*>(&message), 1);
        }
        return result;
      }

      if (g_mouse.buttons[0] == 1 && attack && !HIBYTE(GetAsyncKeyState(VK_MENU))) {
        this->SendMsg(0x1A, 1, 0, 0, 0);
        *reinterpret_cast<int*>(&this->unknown_00EC[8]) = pick->accountId;
        *reinterpret_cast<int*>(&this->unknown_00EC[4]) = pick->accountId;
        this->unknown_00F8 = 0;
        if (g_session.GetSceneUnknown004C() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
          this->RequestPlayerAction(pick->accountId, 0);
          this->forcedAttack = 1;
        } else {
          this->RequestPlayerAction(pick->accountId, 1);
          this->forcedAttack = 0;
        }
        return result;
      }

      if (this->forcedAttack == 0 && g_mouse.buttons[0] == 3) {
        *reinterpret_cast<int*>(&this->unknown_00EC[4]) = 0;
      }
      return result;
    }

    if (!IsPlayerJob(job)) {
      break;
    }

    const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
    int attack;
    if (mode == 2 || mode == 4) {
      const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
      attack = shift;
      if (IsSupportSkill(this->unknown_040C)) {
        attack = shift == 0;
      }
    } else {
      attack = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    }
    if (pick->unknown_0024 != 0 || this->scene->unknown_0048 != 0 || siegeMode != 0) {
      attack = 1;
    }
    if (selfTargetOption != 0) {
      attack = 1;
    }

    if (attack && g_mouse.buttons[0] == 1) {
      int index = 0;
      CGameActor* companion;
      if (g_session.IsHomunculusSkill(this->unknown_040C) && g_groundMarkEnabled) {
        companion = static_cast<CGameActor*>(this->scene->GetGameActorByAID(homunAid));
        if (companion != nullptr && companion->HasUnknown030CEntry()) {
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->unknown_0414;
          message.skill = this->unknown_040C;
          message.target = pick->accountId;
          companion->SetUnknown0318(reinterpret_cast<const float*>(&message), index);
          return result;
        }
      } else if (g_session.IsMercenarySkill(this->unknown_040C) && g_groundMarkAltEnabled) {
        companion = static_cast<CGameActor*>(this->scene->GetGameActorByAID(mercenaryAid));
        if (companion != nullptr && companion->HasUnknown0310Entry()) {
          index = 1;
          AiMessage message;
          message.type = 7;
          message.x = 0;
          message.y = 0;
          message.level = this->unknown_0414;
          message.skill = this->unknown_040C;
          message.target = pick->accountId;
          companion->SetUnknown0318(reinterpret_cast<const float*>(&message), index);
          return result;
        }
      }
    }

    const bool allowed = pick->accountId != g_session.m_aid || mode == 4;
    if (!allowed || !attack) {
      return result;
    }

    this->SendMsg(0x1A, 1, 0, 0, 0);
    *reinterpret_cast<int*>(&this->unknown_00EC[8]) = pick->accountId;
    *reinterpret_cast<int*>(&this->unknown_00EC[4]) = pick->accountId;
    this->unknown_00F8 = 0;
    if (g_session.GetSceneUnknown004C() == 0 && (HIBYTE(GetAsyncKeyState(VK_CONTROL)) || forceOption != 0)) {
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
    case 0x8D:
    case 0x8F:
    case 0x98:
    case 0xDA:
    case 0xDE:
    case 0xE6:
    case 0x5174:
    case 0x5175:
    case 0x5176:
    case 0x5177: {
      const int skill = this->unknown_040C;
      if (skill == 0xD7 || skill == 0xD8 || skill == 0xD9 || skill == 0xDA) {
        return result;
      }

      this->SendMsg(0x1A, 1, 0, 0, 0);
      this->unknown_00F8 = 0;
      const int skillUse = reinterpret_cast<int>(&this->unknown_0408);
      if (!HIBYTE(GetAsyncKeyState(VK_CONTROL)) && forceOption == 0) {
        this->scene->player->SendMsg(nullptr, 103, 0, pick->accountId, 0, skillUse, skillUse >> 31, 1, 0, 0, 0, 0, 0, 0, 0);
        this->forcedAttack = 0;
      } else {
        this->scene->player->SendMsg(nullptr, 103, 0, pick->accountId, 0, skillUse, skillUse >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
        this->forcedAttack = 1;
      }
      return result;
    }
    default:
      if (reinterpret_cast<std::intptr_t>(this->unknown_0408) == 5) {
        const long long skillUse = static_cast<long long>(reinterpret_cast<int>(this)) + offsetof(CGameMode, unknown_0408);
        this->scene->player->SendMsg(nullptr, 103, 0, pick->accountId, 0, static_cast<int>(skillUse), static_cast<int>(skillUse >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
      }
      return result;
    }
  case 3:
    cursor.Set(0, this->cursorTick);
    return 0;
  case 1:
    if (g_mouse.buttons[0] == 1 && reinterpret_cast<std::intptr_t>(this->unknown_0408) == 0) {
      this->scene->player->SendMsg(nullptr, 18, 0, pick->accountId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 4: {
    cursor.Set(0, this->cursorTick);
    if (g_session.IsMercenaryJob(pick->job)) {
      CWorld* scene = this->scene;
      if (this->unknown_040C == 0xE7 || ((scene->unknown_0048 != 0 || scene->unknown_004C != 0 || siegeMode != 0) && !g_session.IsHomunAid(pick->accountId))) {
        if ((this->scene->unknown_0048 != 0 || this->scene->unknown_004C != 0 || siegeMode != 0) && !g_session.IsHomunAid(pick->accountId)) {
          cursor.Set(5, this->cursorTick);
        }
        this->RequestPlayerAction(pick->accountId, 1);
        this->forcedAttack = 0;
        return 0;
      }
      break;
    }

    const std::intptr_t mode = reinterpret_cast<std::intptr_t>(this->unknown_0408);
    int attack;
    if (mode == 2 || mode == 4) {
      const int shift = HIBYTE(GetAsyncKeyState(VK_SHIFT));
      attack = shift;
      if (IsSupportSkill(this->unknown_040C)) {
        attack = shift == 0;
      }
    } else {
      attack = HIBYTE(GetAsyncKeyState(VK_SHIFT));
    }
    if (pick->unknown_0024 != 0 || this->scene->unknown_0048 != 0 || this->scene->unknown_004C != 0 || siegeMode != 0) {
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
