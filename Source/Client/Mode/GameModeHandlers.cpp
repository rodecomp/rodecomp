#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/ClanInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/MessageTable.h"
#include "Contents/World/World.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

#pragma pack(push, 1)
struct ClanInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ int clanId;
  /* 0008 */ char clanName[24];
  /* 0020 */ char masterName[24];
  /* 0038 */ char mapName[16];
  /* 0048 */ unsigned char allyCount;
  /* 0049 */ unsigned char hostileCount;
  /* 004A */ char relationNames[1][24];
};

struct ActorClanNamePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ char name[1];
};

struct ClanMemberCountPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t connectedMembers;
  /* 0004 */ std::uint16_t totalMembers;
};

struct RuneSetGradeEntry {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint16_t grade;
  /* 0006 */ std::uint16_t gradeLevel;
};

struct RuneRewardStepEntry {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ unsigned char step;
};

struct RuneListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ unsigned char unknown_0004;
  /* 0005 */ std::uint16_t tagId;
  /* 0007 */ std::uint16_t count;
};

struct RuneActivationPacket : RuneListPacket {
  /* 0009 */ std::uint32_t ids[1];
};

struct RuneSetGradesPacket : RuneListPacket {
  /* 0009 */ RuneSetGradeEntry entries[1];
};

struct RuneRewardStepsPacket : RuneListPacket {
  /* 0009 */ RuneRewardStepEntry entries[1];
};
#pragma pack(pop)

struct GameOptionEntry {
  /* 0000 */ int id;
  /* 0004 */ int value;
};

// GLOBAL: gl_24865
bool g_gameOptionFlag;

// OFFSET: 0x00D65730
// FUNCTION: fn_22001
void CGameMode::ReceiveClanLeave(const unsigned char* packet) {
  g_clanInfo->Clear();
  this->nameCard.SetValue3(g_session.m_aid, g_clanInfo->m_clanName);
  if (this->scene->player) {
    this->scene->player->SetUnknown0305(0, 0);
  }

  g_windowMgr.RefreshClanWnd();
}

// OFFSET: 0x00D658C0
// FUNCTION: fn_22005
void CGameMode::ReceiveClanInfo(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ClanInfoPacket*>(data);

  g_clanInfo->Clear();
  g_clanInfo->m_hasClan = true;
  g_clanInfo->m_clanId = packet->clanId;

  char clanName[24] = {};
  strcpy_s(clanName, sizeof(clanName), packet->clanName);
  g_clanInfo->m_clanName = clanName;
  this->nameCard.SetValue3(g_session.m_aid, g_clanInfo->m_clanName);

  char masterName[24] = {};
  strcpy_s(masterName, sizeof(masterName), packet->masterName);
  g_clanInfo->m_masterName = masterName;

  char mapName[16] = {};
  strcpy_s(mapName, sizeof(mapName), packet->mapName);
  g_clanInfo->m_mapName = mapName;

  const char* relationName = packet->relationNames[0];
  for (int i = 0; i < packet->allyCount; ++i) {
    char name[24] = {};
    StringFormat(name, sizeof(name), static_cast<std::size_t>(-1), "%s", relationName);
    std::pair<int, std::string> relation(0, name);
    g_clanInfo->m_relations.push_back(relation);
    relationName += sizeof(name);
  }

  for (int i = 0; i < packet->hostileCount; ++i) {
    char name[24] = {};
    StringFormat(name, sizeof(name), static_cast<std::size_t>(-1), "%s", relationName);
    std::pair<int, std::string> relation(1, name);
    g_clanInfo->m_relations.push_back(relation);
    relationName += sizeof(name);
  }

  g_windowMgr.RefreshClanWnd();
}

// OFFSET: 0x00D66200
// FUNCTION: fn_22009
void CGameMode::ReceiveGameOptions(int count, const GameOptionEntry* entries) {
  for (int i = 0; i < count; ++i) {
    if (entries[i].id == 1) {
      g_gameOptionFlag = entries[i].value != 0;
    }
  }
}

// OFFSET: 0x00D66370
// FUNCTION: fn_22011
void CGameMode::ReceiveActorClanName(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorClanNamePacket*>(data);

  this->nameCard.SetValue3(packet->aid, std::string(packet->name, packet->name + (packet->length - 8)));
}

// OFFSET: 0x00D66A00
// FUNCTION: fn_22017
void CGameMode::ReceiveClanMemberCount(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ClanMemberCountPacket*>(data);

  g_clanInfo->m_connectedMembers = packet->connectedMembers;
  g_clanInfo->m_totalMembers = packet->totalMembers;
}

// OFFSET: 0x00D67040
// FUNCTION: fn_22023
void CGameMode::ReceiveRuneActivation(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RuneActivationPacket*>(data);
  if (!packet) {
    return;
  }

  unsigned int count = packet->count;
  const unsigned int size = packet->length - 9;
  if (size != count * sizeof(std::uint32_t)) {
    count = size / sizeof(std::uint32_t);
  }

  for (unsigned int i = 0; i < count; ++i) {
    g_runeSystemMgr->SetRuneGrade(packet->tagId, 1, packet->ids[i], 0, 0);
  }

  CTagData* tag = g_runeSystemMgr->GetTag(packet->tagId);
  if (!tag) {
    return;
  }

  tag->UpdateActiveCount(1);
  UIWindow* window = g_windowMgr.LookupWindow(361);
  if (window) {
    window->SendMsg(window, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D670F0
// FUNCTION: fn_22024
void CGameMode::ReceiveRuneSetGrades(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RuneSetGradesPacket*>(data);
  if (!packet) {
    return;
  }

  unsigned int count = packet->count;
  const unsigned int size = packet->length - 9;
  if (size != count * sizeof(RuneSetGradeEntry)) {
    count = size / sizeof(RuneSetGradeEntry);
  }

  for (unsigned int i = 0; i < count; ++i) {
    const RuneSetGradeEntry& entry = packet->entries[i];
    g_runeSystemMgr->SetRuneGrade(packet->tagId, 0, entry.id, entry.grade, entry.gradeLevel);
  }

  CTagData* tag = g_runeSystemMgr->GetTag(packet->tagId);
  if (!tag) {
    return;
  }

  tag->UpdateActiveCount(0);
  tag->m_locked = true;
  UIWindow* window = g_windowMgr.LookupWindow(361);
  if (window) {
    window->SendMsg(window, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D671A0
// FUNCTION: fn_22025
void CGameMode::ReceiveRuneRewardResult(int result, int tagId, std::int64_t id, int step) {
  switch (result) {
  case 0: {
    g_runeSystemMgr->SetRewardStep(tagId, id, step);
    UIWindow* window = g_windowMgr.LookupWindow(361);
    if (window) {
      if (step > 0) {
        window->SendMsg(window, 178, 224, step - 1, 0, 0);
      }

      window->SendMsg(window, 60, 0, 0, 0, 0);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4202)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4210)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4211)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4212)), 0x64FFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4213)), 0x64FFFF, 0, 0, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4214)), 0x64FFFF, 0, 0, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4215)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D672E0
// FUNCTION: fn_22026
void CGameMode::ReceiveRuneRewardSteps(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RuneRewardStepsPacket*>(data);
  if (!packet) {
    return;
  }

  for (int i = 0; i < packet->count; ++i) {
    const RuneRewardStepEntry& entry = packet->entries[i];
    g_runeSystemMgr->SetRewardStep(packet->tagId, entry.id, entry.step);
  }

  UIWindow* window = g_windowMgr.LookupWindow(361);
  if (window) {
    window->SendMsg(window, 60, 0, 0, 0, 0);
  }
}
