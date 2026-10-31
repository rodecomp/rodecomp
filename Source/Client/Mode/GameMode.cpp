#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
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
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
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
void CGameMode::ClearUnknown04DC() {
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  if (g_windowMgr.unknown_0508 != 2 && actorId == *reinterpret_cast<const int*>(0x01733C3C)) {
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
