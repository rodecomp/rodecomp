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
#include "Client/Application/Launcher.h"
#include "UI/Text/NavigationLink.h"
#include "Contents/RenewQuestMgr.h"
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
