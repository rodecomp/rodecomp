#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Actors/ActorEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Skills/SkillUnitId.h"
#include "Network/Packets/ModeResultEnums.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Client/Replay/Replay.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Clan/ClanInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Skills/GraffiSkill.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Skills/MonsterSkillEffects.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Rodex/RodexMailStore.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "File/FileCreation.h"
#include "Script/LuaScript.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Rodex/UIRodexInboxWnd.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Gauges/UIBarGraph.h"
#include "UI/Items/UIGradeEnchantWnd.h"
#include "UI/Status/UIHomunInfoWnd.h"
#include "UI/Party/UISeekPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <map>
#include <new>
#include <string>
#include <utility>
#include <vector>
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <timeapi.h>
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"

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
  /* 0004 */ unsigned char padding_0004;
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
struct PackageItemTestEntry {
  /* 0000 */ int itemId;
  /* 0004 */ std::uint16_t rate;
  /* 0006 */ int count;
};

struct Item1718BasicRecord {
  /* 0000 */ std::int32_t itemId;
  /* 0004 */ std::uint32_t price;
  /* 0008 */ std::uint32_t realPrice;
  /* 000C */ unsigned char type;
  /* 000D */ std::uint16_t sprite;
  /* 000F */ std::uint32_t location;
};

struct Item1718IndexRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ std::uint32_t price;
  /* 0006 */ std::uint32_t realPrice;
};

struct HomunculusInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char flags;
  /* 001B */ std::int16_t level;
  /* 001D */ std::int16_t hunger;
  /* 001F */ std::int16_t intimacy;
  /* 0021 */ int accessory;
  /* 0025 */ std::int16_t stats[8];
  /* 0035 */ std::int16_t hp;
  /* 0037 */ std::int16_t maxHp;
  /* 0039 */ std::int16_t sp;
  /* 003B */ std::int16_t maxSp;
  /* 003D */ int exp;
  /* 0041 */ int maxExp;
  /* 0045 */ std::int16_t skillPoints;
  /* 0047 */ std::int16_t attackRange;
};

struct HomunculusInfoPacket2 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char flags;
  /* 001B */ std::int16_t level;
  /* 001D */ std::int16_t hunger;
  /* 001F */ std::int16_t intimacy;
  /* 0021 */ std::int16_t stats[8];
  /* 0031 */ int hp;
  /* 0035 */ int maxHp;
  /* 0039 */ std::int16_t sp;
  /* 003B */ std::int16_t maxSp;
  /* 003D */ int exp;
  /* 0041 */ int maxExp;
  /* 0045 */ std::int16_t skillPoints;
  /* 0047 */ std::int16_t attackRange;
};

struct HomunculusInfoPacket3 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char flags;
  /* 001B */ std::int16_t level;
  /* 001D */ std::int16_t hunger;
  /* 001F */ std::int16_t intimacy;
  /* 0021 */ std::int16_t stats[8];
  /* 0031 */ int hp;
  /* 0035 */ int maxHp;
  /* 0039 */ int sp;
  /* 003D */ int maxSp;
  /* 0041 */ std::int64_t exp;
  /* 0049 */ std::int64_t maxExp;
  /* 0051 */ std::int16_t skillPoints;
  /* 0053 */ std::int16_t attackRange;
};

struct ItemRewardOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};

struct ItemRewardPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ std::int16_t index;
  /* 0005 */ std::int16_t quantity;
  /* 0007 */ int itemId;
  /* 000B */ unsigned char type;
  /* 000C */ unsigned char identified;
  /* 000D */ unsigned char damaged;
  /* 000E */ unsigned char refine;
  /* 000F */ std::uint32_t cards[4];
  /* 001F */ ItemRewardOption options[5];
  /* 0038 */ std::uint16_t value;
  /* 003A */ unsigned char bound;
  /* 003B */ int expireTime;
};

struct ItemRewardPacket2 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ std::int16_t index;
  /* 0005 */ std::int16_t quantity;
  /* 0007 */ int itemId;
  /* 000B */ unsigned char type;
  /* 000C */ unsigned char identified;
  /* 000D */ unsigned char damaged;
  /* 000E */ std::uint32_t cards[4];
  /* 001E */ ItemRewardOption options[5];
  /* 0037 */ std::uint16_t value;
  /* 0039 */ unsigned char bound;
  /* 003A */ int expireTime;
  /* 003E */ unsigned char refine;
  /* 003F */ unsigned char grade;
};

struct MinimapMarkerPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t key;
  /* 0006 */ std::int16_t x;
  /* 0008 */ std::int16_t y;
  /* 000A */ std::int16_t type;
  /* 000C */ std::int16_t color;
};

struct ActorSkillResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t source;
  /* 0006 */ std::uint32_t target;
  /* 000A */ std::uint16_t skill;
  /* 000C */ std::uint16_t x;
  /* 000E */ std::uint16_t y;
  /* 0010 */ int tick;
  /* 0014 */ int level;
  /* 0018 */ unsigned char flag;
  /* 0019 */ int result;
};

struct SimulationResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint32_t kind;
};

struct GroundSkillPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t owner;
  /* 000A */ std::int16_t x;
  /* 000C */ std::int16_t y;
  /* 000E */ unsigned char type;
  /* 000F */ unsigned char visible;
  /* 0010 */ unsigned char graffiti;
  /* 0011 */ char text[1];
};

struct GroundSkillPacket8 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ std::uint32_t owner;
  /* 000C */ std::int16_t x;
  /* 000E */ std::int16_t y;
  /* 0010 */ unsigned char type;
  /* 0011 */ signed char level;
  /* 0012 */ unsigned char visible;
  /* 0013 */ char text[1];
};

struct GroundSkillPacket32 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ std::uint32_t owner;
  /* 000C */ std::int16_t x;
  /* 000E */ std::int16_t y;
  /* 0010 */ int type;
  /* 0014 */ signed char level;
  /* 0015 */ unsigned char visible;
  /* 0016 */ char text[1];
};

struct GroundSkillPacketExtended {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ std::uint32_t owner;
  /* 000C */ std::int16_t x;
  /* 000E */ std::int16_t y;
  /* 0010 */ int type;
  /* 0014 */ signed char level;
  /* 0015 */ unsigned char visible;
  /* 0016 */ unsigned char flags;
  /* 0017 */ char text[1];
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(Item1718BasicRecord) == 0x13);
static_assert(sizeof(Item1718IndexRecord) == 0x0A);
static_assert(sizeof(HomunculusInfoPacket) == 0x49);
static_assert(sizeof(HomunculusInfoPacket2) == 0x49);
static_assert(sizeof(HomunculusInfoPacket3) == 0x55);
static_assert(sizeof(ItemRewardPacket) == 0x3F);
static_assert(sizeof(ItemRewardPacket2) == 0x40);
static_assert(sizeof(ActorSkillResultPacket) == 0x1D);
#endif

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  static CEquipmentPropertiesMgr* GetInstance();
  void RefreshExpansion(const int& kind, const int& itemId);
};
}

struct GroundSkillSpawnData {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ std::uint32_t owner;
  /* 0008 */ int type;
  /* 000C */ int x;
  /* 0010 */ int y;
  /* 0014 */ unsigned char visible;
  /* 0018 */ int level;
};

static_assert(sizeof(GroundSkillSpawnData) == 0x1C);

struct GroundSkillExtendedData {
  /* 0000 */ GroundSkillSpawnData skill;
  /* 001C */ unsigned flags;
};

struct GroundSkillArgumentData {
  /* 0000 */ GroundSkillExtendedData extended;
  /* 0020 */ int value;
};

static_assert(sizeof(GroundSkillExtendedData) == 0x20);
static_assert(sizeof(GroundSkillArgumentData) == 0x24);


// OFFSET: 0x00D66230
// FUNCTION: fn_22010
void CGameMode::ReceivePartyRoster(const unsigned char* packet) {
  std::vector<PartyRosterEntry> members;
  const unsigned count = (static_cast<unsigned>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 28u) / sizeof(PartyRosterEntry);
  const PartyRosterEntry* entry = reinterpret_cast<const PartyRosterEntry*>(packet + 28);
  for (unsigned i = 0; i < count; ++i) {
    members.push_back(*entry++);
  }
  this->SetPartyMemberList(reinterpret_cast<const char*>(packet + 4), &members);
}

// OFFSET: 0x00D40E40
// FUNCTION: fn_21822
void CGameMode::ReceiveGroundSkillExtended(const unsigned char* record) {
  const GroundSkillPacketExtended& packet = *reinterpret_cast<const GroundSkillPacketExtended*>(record);
  switch (packet.type) {
  case UNT_GRAFFITI: {
    if (packet.length <= 23) {
      return;
    }
    CGraffiSkill* skill = new CGraffiSkill;
    skill->SendMsg(nullptr, AM_SETMASTERGID, 0, packet.aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int address = reinterpret_cast<int>(packet.text);
    const int x = packet.x;
    const int y = packet.y;
    skill->SendMsg(nullptr, AM_SET_SKILL, 0, address, address >> 31, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    return;
  }
  case UNT_SHINKIROU:
    this->SpawnMirrorActor(packet.aid, packet.owner, packet.x, packet.y);
    return;
  }

  GroundSkillExtendedData data;
  data.skill.aid = packet.aid;
  data.skill.owner = packet.owner;
  data.skill.type = packet.type;
  data.skill.x = packet.x;
  data.skill.y = packet.y;
  data.skill.visible = packet.visible;
  data.skill.level = packet.level;
  data.flags = packet.flags;
  if (data.skill.owner != g_session.m_aid && !data.skill.visible) {
    const auto found = std::find(this->m_skillUnitIds.begin(), this->m_skillUnitIds.end(), data.skill.aid);
    if (found != this->m_skillUnitIds.end()) {
      this->m_skillUnitIds.erase(found);
      data.skill.visible = 0;
    }
  } else if (data.skill.visible) {
    this->m_skillUnitIds.push_back(packet.aid);
  }

  CSkill* skill = static_cast<CSkill*>(this->scene->GetSkillByAID(packet.aid));
  if (!skill) {
    const int type = packet.type;
    if (!((type >= 45 && type < 1000) || (type > 1000 && type < 3999) ||
          static_cast<unsigned>(type) - 20021u <= 11978u)) {
      return;
    }
    skill = new (std::nothrow) CSkill(packet.aid);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    if (!skill) {
      return;
    }
  }
  skill->sourceActorId = packet.owner;
  const int address = reinterpret_cast<int>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D413F0
// FUNCTION: fn_21824
void CGameMode::SpawnGroundSkill(std::uint32_t aid, std::uint32_t owner, int value, std::int16_t x, std::int16_t y, int type, signed char level, unsigned char visible, unsigned char flags) {
  if (type == 0x126) {
    this->SpawnMirrorActor(aid, owner, x, y);
    return;
  }
  GroundSkillArgumentData data;
  data.extended.skill.aid = aid;
  data.extended.skill.owner = owner;
  data.extended.skill.type = type;
  data.extended.skill.x = x;
  data.extended.skill.y = y;
  data.extended.skill.visible = visible;
  data.extended.skill.level = level;
  data.extended.flags = flags;
  data.value = value;
  if (owner != g_session.m_aid && !visible) {
    const auto found = std::find(this->m_skillUnitIds.begin(), this->m_skillUnitIds.end(), aid);
    if (found != this->m_skillUnitIds.end()) {
      this->m_skillUnitIds.erase(found);
      data.extended.skill.visible = 0;
    }
  } else if (visible) {
    this->m_skillUnitIds.push_back(aid);
  }
  CSkill* skill = static_cast<CSkill*>(this->scene->GetSkillByAID(aid));
  if (!skill) {
    if (!((type >= 45 && type < 1000) || (type > 1000 && type < 3999) ||
          static_cast<unsigned>(type) - 20021u <= 11978u)) {
      return;
    }
    skill = new (std::nothrow) CSkill(aid);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    if (!skill) {
      return;
    }
  }
  skill->sourceActorId = owner;
  const int address = reinterpret_cast<int>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}


int CountItemRandomOptions(const unsigned char* entries);

struct GameOptionEntry {
  /* 0000 */ int id;
  /* 0004 */ int value;
};

// GLOBAL: gl_24865
bool g_gameOptionFlag;

// GLOBAL: gl_24927
int g_pendingAid;

// GLOBAL: gl_24928
int g_pendingGid;

// GLOBAL: gl_24929
unsigned char g_toggleNoticeState;

// GLOBAL: gl_25094
bool g_playerMessage82Pending;

// OFFSET: 0x00D3B690
// FUNCTION: fn_21781
void CGameMode::ReceivePartyBookingRegisterResult(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 2:
    g_windowMgr.Confirm(LookupMessage(MSI_PARTY_AD_DELETE_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    g_windowMgr.Confirm(LookupMessage(MSI_PARTY_NOT_EXIST_AD), 0, true, false, 0, "Message", 280, 120, 0);
    [[fallthrough]];
  case 0:
  case 1:
    g_windowMgr.CloseWindow(WID_SEEKPARTYWND);
    break;
  }
}

// OFFSET: 0x00D3B730
// FUNCTION: fn_21782
void CGameMode::ReceivePartyBookingUpdateResult(const unsigned char* packet) {
  const std::int16_t result = *reinterpret_cast<const std::int16_t*>(packet + 2);
  if (result == 0) {
    g_session.m_partyBooking.registeredTick = GetTickCount();
    g_windowMgr.EnsureWindow(WID_SEEKPARTYWND);
    UISeekPartyWnd* seekWindow = static_cast<UISeekPartyWnd*>(g_windowMgr.LookupWindow(WID_SEEKPARTYWND));
    if (seekWindow) {
      UIWindow* jobWindow = g_windowMgr.LookupWindow(WID_SEEKPARTYMBWND);
      if (jobWindow) {
        seekWindow->SetJobs(reinterpret_cast<const std::int16_t*>(reinterpret_cast<const unsigned char*>(jobWindow) + 0x16D));
        g_windowMgr.CloseWindow(WID_SEEKPARTYMBWND);
      }
    }
  } else if (result == 2) {
    g_windowMgr.Confirm(LookupMessage(MSI_PARTY_FAILED_TIMEOUT), 0, true, false, 0, "Message", 280, 120, 0);
  } else {
    g_windowMgr.Confirm(LookupMessage(MSI_PARTY_REGISTRATION_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D3B7F0
// FUNCTION: fn_21783
void CGameMode::ReceivePartyBookingSearchResult(const unsigned char* packet) {
  const int size = *reinterpret_cast<const std::int16_t*>(packet + 2) - 5;
  const int count = size / sizeof(PARTY_BOOKING_AD_INFO);
  PARTY_BOOKING_AD_INFO* infos = new PARTY_BOOKING_AD_INFO[count];
  std::memcpy(infos, packet + 5, size);
  g_session.m_partyBooking.m_filterEnabled = true;
  UIWindow* window = g_windowMgr.LookupWindow(WID_SEEKPARTYLISTWND);
  if (!window) {
    return;
  }

  if (size == 0) {
    g_windowMgr.Confirm(LookupMessage(MSI_PARTY_SEARCH_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
    g_session.m_partyBooking.m_lastPage = false;
    window->Invalidate();
    return;
  }

  g_session.m_partyBooking.AddEntries(infos, count, *reinterpret_cast<const bool*>(packet + 4));
  window->Invalidate();
}

// OFFSET: 0x00D3B8D0
// FUNCTION: fn_21784
void CGameMode::ReceivePartyBookingDelete(const unsigned char* packet) {
  g_session.m_partyBooking.Remove(*reinterpret_cast<const int*>(packet + 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_SEEKPARTYLISTWND)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B910
// FUNCTION: fn_21785
void CGameMode::ReceivePartyBookingInsert(const unsigned char* packet) {
  g_session.m_partyBooking.OnAdvertisement(*reinterpret_cast<const PARTY_BOOKING_AD_INFO*>(packet + 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_SEEKPARTYLISTWND)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B960
// FUNCTION: fn_21786
void CGameMode::ReceivePartyBookingUpdate(const unsigned char* packet) {
  g_session.m_partyBooking.SetJobs(*reinterpret_cast<const int*>(packet + 2), reinterpret_cast<const short*>(packet + 6));
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_SEEKPARTYLISTWND)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B9A0
// FUNCTION: fn_21787
void CGameMode::ReceiveToggleNotice(const unsigned char* packet) {
  g_toggleNoticeState = packet[2];
  const char* message = LookupMessage(g_toggleNoticeState == 0 ? 1326 : 1325);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D3BF00
// FUNCTION: fn_21791
void CGameMode::ReceivePendingResult(const unsigned char* packet) {
  std::string text;
  std::uint32_t color = 0xFF;
  switch (*reinterpret_cast<const std::int16_t*>(packet + 10)) {
  case ERROR_TYPE_NONE:
  case 8:
    g_pendingAid = *reinterpret_cast<const int*>(packet + 2);
    g_pendingGid = *reinterpret_cast<const int*>(packet + 6);
    if (g_windowMgr.itemShopWnd) {
      g_windowMgr.itemShopWnd->Invalidate();
    }
    if (*reinterpret_cast<const std::int16_t*>(packet + 10) == 8) {
      text = LookupMessage(MSI_CASH_FAILED_BUY_SOME);
    } else {
      color = 0xFFFF00;
      text = LookupMessage(MSI_DEAL_SECCESS);
    }
    break;
  case ERROR_TYPE_NPC:
    text = LookupMessage(MSI_BUY_CASH_FAIL_NPC);
    break;
  case ERROR_TYPE_SYSTEM:
    text = LookupMessage(MSI_BUY_CASH_FAIL_SYSTEM);
    break;
  case ERROR_TYPE_INVENTORY_WEIGHT:
    text = LookupMessage(MSI_OVER_WEIGHT);
    break;
  case ERROR_TYPE_EXCHANGE:
    text = LookupMessage(MSI_BUY_CASH_FAIL_EXCHANGE);
    break;
  case ERROR_TYPE_ITEM_ID:
    text = LookupMessage(MSI_BUY_CASH_FAIL_ITEM_ID);
    break;
  case ERROR_TYPE_MONEY:
    text = LookupMessage(MSI_BUY_CASH_FAIL_MONEY);
    break;
  case ERROR_TYPE_AMOUNT:
    text = LookupMessage(MSI_CASH_FAILED_TOTAL_CNT);
    break;
  case 9:
    text = LookupMessage(MSI_SKILL_INVENTORY_KINDCNT_OVER);
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0, 0);
  }
}

// OFFSET: 0x00D3C1C0
// FUNCTION: fn_21792
void CGameMode::ReceiveItem1718BasicList(const unsigned char* packet) {
  g_session.ClearTradeItems();

  const std::int64_t count = (static_cast<std::int64_t>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 4) / 19;
  const Item1718BasicRecord* records = reinterpret_cast<const Item1718BasicRecord*>(packet + 4);

  for (std::int64_t index = 0; index < count; ++index) {
    Item1718BasicRecord record;
    memcpy_s(&record, sizeof(record), &records[index], sizeof(record));

    RawItemBasic item = {};
    _itoa(record.itemId, item.itemId, 10);
    item.price = record.price;
    item.realPrice = record.realPrice;
    item.type = record.type;
    item.location = record.location;
    item.sprite = record.sprite;
    item.id = -1;
    item.identified = 1;
    g_session.AddShopItem(item, 0);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND)) {
    window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMPURCHASEWND)) {
    window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D3C4F0
// FUNCTION: fn_21793
void CGameMode::ReceiveItem1718Result(const unsigned char* packet) {
  switch (packet[2]) {
  case PURCHASE_SUCCEED:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_SECCESS)), 0xFFFF00, 0, 0, 0);
    break;
  case PURCHASE_FAIL_MONEY:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_MONEY)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_FAIL_COUNT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_OVER_COUNT)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_FAIL_WEIGHT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_WEIGHT)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_FAIL_EACHITEM_COUNT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PURCHASE_FAIL_EACHITEM_COUNT)), 0xFF, 0, 0, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_ACCESS_BY_WEIGHTOVER_80)), 0xFFFF00, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_OUT_OF_STOCK)), 0xFF, 0, 0, 0);
    break;
  case 12:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAD)), 0xFFFF00, 0, 0, 0);
    break;
  case 13:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVENTORY_EXTEND_OF_BODYITEM)), 0xFF, 0, 0, 0);
    break;
  case 14:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PVP)), 0xFF, 0, 0, 0);
    break;
  }

  g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
  g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
}

// OFFSET: 0x00D3C710
// FUNCTION: fn_21794
void CGameMode::ReceiveItem1718IndexList(const unsigned char* packet) {
  g_session.ClearTradeItems();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(Item1718IndexRecord);
  const Item1718IndexRecord* records = reinterpret_cast<const Item1718IndexRecord*>(packet + 4);
  for (int i = 0; i < count; ++i) {
    const Item1718IndexRecord record = records[i];
    ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, record.index);
    if (owned.identity[1] != 0 && (owned.flags[0] == 0 || g_session.hotkeyAndSortOptions[10] == 0)) {
      RawItemBasic item = {};
      item.id = record.index;
      strcpy_s(item.itemId, sizeof(item.itemId), owned.itemId.c_str());
      item.price = record.price;
      item.realPrice = record.realPrice;
      item.type = owned.identity[0];
      item.identified = owned.identified;
      g_session.AddShopItem(item, 1);
    }
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND);
  window->SendMsg(nullptr, UM_SETTITLE, 1, 0, 0, 0);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  g_windowMgr.EnsureWindow(WID_ITEMSELLWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00D3CC10
// FUNCTION: fn_21796
void CGameMode::ReceivePetAct(unsigned char* packet) {
  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (!actor) {
    return;
  }

  int* data = reinterpret_cast<int*>(packet + 6);
  const int kind = *data % 10;
  const int value = *data / 10;
  if (kind == 2) {
    const std::int64_t emotion = value;
    const std::int64_t basePack = g_cashEmotionMgr->BasePackId();
    actor->SendMsg(
      nullptr, 160, 0, static_cast<int>(basePack), static_cast<int>(basePack >> 32), static_cast<int>(emotion), static_cast<int>(emotion >> 32), 0, 0, 0,
      0, 0, 0, 0, 0
    );
    return;
  }

  const int talkId = value / 10;
  *data = talkId;
  char talk[256];
  std::memset(talk, 0, sizeof(talk));
  g_session.GetPetTalkText(talkId / 10, value % 10, talkId % 10, talk);
  if (talk[0]) {
    char speech[256];
    g_session.FormatMonsterSpeech(talk, speech, *reinterpret_cast<const std::uint32_t*>(packet + 2), 1 - kind);
    if (std::strcmp(speech, "") != 0) {
      const std::int64_t text = reinterpret_cast<std::intptr_t>(speech);
      actor->SendMsg(nullptr, AM_CHAT, 0, static_cast<int>(text), static_cast<int>(text >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(speech), 0xFAFAFA, 0, 0, 0);
      }
    }
  }
}

// OFFSET: 0x00D3CDF0
// FUNCTION: fn_21797
void CGameMode::NotifyPetEvolutionResult(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_EVOLUTION_FAIL_RECIPE)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_EVOLUTION_FAIL_MATERIAL)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_EVOLUTION_FAIL_RG_FAMILIAR)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_AUTO_FEEDING)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_AUTO_FEEDING_REST)), 0xFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_AUTO_FEEDING_ON)), 0xFF, 0, 0, 0);
    break;
  case 7:
    g_session.ResetPetEggAndJob();
    break;
  }
}

// OFFSET: 0x00D3CEC0
// FUNCTION: fn_21798
void CGameMode::PlayBgm(unsigned char type, std::string name) {
  if (!g_serviceOptions.Get(7)) {
    return;
  }

  g_soundMgr->StopStreams();
  switch (type) {
  case 0:
    g_soundMgr->PlayStream(FormatOwnedString("bgm\\%s.mp3", name.c_str()).c_str(), 0, false);
    g_soundMgr->secondaryEnabled = 0;
    break;
  case 1:
    g_soundMgr->PlayStream(FormatOwnedString("bgm\\%s.mp3", name.c_str()).c_str(), 1, false);
    g_soundMgr->secondaryEnabled = 1;
    break;
  case 2:
    g_soundMgr->SetDefaultName(g_session.GetBgmFileName(std::string(this->mapName).c_str()));
    g_soundMgr->secondaryEnabled = 0;
    break;
  }
}

// OFFSET: 0x00D3D0F0
// FUNCTION: fn_21799
void CGameMode::ReceiveNpcBgm(const unsigned char* packet) {
  if (g_serviceOptions.Get(7)) {
    const std::string path = std::string("bgm\\") + reinterpret_cast<const char*>(packet + 2) + std::string(".mp3");
    g_soundMgr->PlayStream(path.c_str(), 1, true);
    g_soundMgr->secondaryEnabled = 1;
  }
}

// OFFSET: 0x00D3D2E0
// FUNCTION: fn_21800
void CGameMode::IgnoreRemovedPacket1(const unsigned char*) {
}

// OFFSET: 0x00D3D2F0
// FUNCTION: fn_21801
void CGameMode::ReceivePlayerMessage82(const unsigned char* packet) {
  CPlayer* player = this->scene->player;
  if (!player) {
    return;
  }

  player->SendMsg(nullptr, AM_MAKE_SKILL_RECHARGEGAGE, 0, *reinterpret_cast<const int*>(packet + 6) * 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  g_session.playerStatusFlags[0x0A] = true;
}

// OFFSET: 0x00D3D390
// FUNCTION: fn_21802
void CGameMode::ReceiveHomunculusInfo(const unsigned char* data) {
  const HomunculusInfoPacket* packet = reinterpret_cast<const HomunculusInfoPacket*>(data);
  char* name = reinterpret_cast<char*>(&g_session.spAndHomunculusStats[0x0C]);
  int* stats = reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x2C]);
  if (g_serviceType == 10 || g_serviceType == 12 || g_serviceType == 15) {
    std::sprintf(name, "%s", packet->name);
  } else {
    std::sprintf(name, packet->name);
  }

  g_session.homunFriendly = packet->intimacy;
  g_session.homunAccessory = packet->accessory;
  g_session.homunNameModified = packet->flags;
  for (int i = 0; i < 8; ++i) {
    stats[i] = packet->stats[i];
  }
  *reinterpret_cast<int*>(g_session.homunLevel) = packet->level;
  g_session.m_homunHp = packet->hp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunSp = packet->sp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.homunMaxHp) = packet->maxHp;
  g_session.RefreshHomunculusInfo();
  g_session.homunMaxSp = packet->maxSp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunExp = packet->exp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunMaxExp = packet->maxExp;
  g_session.RefreshHomunculusInfo();
  g_session.homunHunger = packet->hunger;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.companionSkillPoints) = packet->skillPoints;
  g_session.InvalidateWindowFrames();
  *reinterpret_cast<int*>(&g_session.companionTargetState[4]) = packet->attackRange;
  if (g_windowMgr.homunculusSkillWnd) {
    g_windowMgr.homunculusSkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  int& notified = *reinterpret_cast<int*>(&g_session.companionTargetState[0]);
  const std::int16_t state = packet->hunger;
  if ((state == 1 || state == 5 || state == 10) && notified != state) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_HOMUN_HUNGRY)), 0xFF, 0, 0, 0);
    notified = packet->hunger;
  }
}

// OFFSET: 0x00D3D710
// FUNCTION: fn_21803
void CGameMode::ReceiveHomunculusInfo3(const unsigned char* data) {
  if (!data) {
    return;
  }

  const HomunculusInfoPacket3* packet = reinterpret_cast<const HomunculusInfoPacket3*>(data);
  int* stats = reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x2C]);
  StringFormat(reinterpret_cast<char*>(&g_session.spAndHomunculusStats[0x0C]), 32, static_cast<std::size_t>(-1), "%s", packet->name);
  g_session.homunNameModified = packet->flags;
  *reinterpret_cast<int*>(g_session.homunLevel) = packet->level;
  g_session.homunHunger = packet->hunger;
  g_session.RefreshHomunculusInfo();

  int& notified = *reinterpret_cast<int*>(&g_session.companionTargetState[0]);
  const int state = packet->hunger;
  switch (state) {
  case 1:
  case 5:
  case 10:
    if (notified != state) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_HOMUN_HUNGRY)), 0xFF, 0, 0, 0);
      notified = packet->hunger;
    }
  }

  g_session.homunFriendly = packet->intimacy;
  for (int i = 0; i < 8; ++i) {
    stats[i] = packet->stats[i];
  }
  g_session.m_homunHp = packet->hp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.homunMaxHp) = packet->maxHp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunSp = packet->sp;
  g_session.RefreshHomunculusInfo();
  g_session.homunMaxSp = packet->maxSp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunExp = packet->exp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunMaxExp = packet->maxExp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.companionSkillPoints) = packet->skillPoints;
  g_session.InvalidateWindowFrames();
  if (g_windowMgr.homunculusSkillWnd) {
    g_windowMgr.homunculusSkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  *reinterpret_cast<int*>(&g_session.companionTargetState[4]) = packet->attackRange;
  if (UIHomunInfoWnd* window = static_cast<UIHomunInfoWnd*>(g_windowMgr.homunInfoWnd)) {
    window->expBar->FillBackground(0xFFFFFF);
    window->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
    window->intimacyBar->SetValues(&g_session.homunHunger, &g_session.homunMaxHunger);
  }
}

// OFFSET: 0x00D3D910
// FUNCTION: fn_21804
void CGameMode::ReceiveHomunculusInfo2(const unsigned char* data) {
  const HomunculusInfoPacket2* packet = reinterpret_cast<const HomunculusInfoPacket2*>(data);
  char* name = reinterpret_cast<char*>(&g_session.spAndHomunculusStats[0x0C]);
  int* stats = reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x2C]);
  if (g_serviceType == 10 || g_serviceType == 12 || g_serviceType == 15) {
    std::sprintf(name, "%s", packet->name);
  } else {
    std::sprintf(name, packet->name);
  }

  g_session.homunFriendly = packet->intimacy;
  g_session.homunNameModified = packet->flags;
  for (int i = 0; i < 8; ++i) {
    stats[i] = packet->stats[i];
  }
  *reinterpret_cast<int*>(g_session.homunLevel) = packet->level;
  g_session.m_homunHp = packet->hp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunSp = packet->sp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.homunMaxHp) = packet->maxHp;
  g_session.RefreshHomunculusInfo();
  g_session.homunMaxSp = packet->maxSp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunExp = packet->exp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunMaxExp = packet->maxExp;
  g_session.RefreshHomunculusInfo();
  g_session.homunHunger = packet->hunger;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.companionSkillPoints) = packet->skillPoints;
  g_session.InvalidateWindowFrames();
  *reinterpret_cast<int*>(&g_session.companionTargetState[4]) = packet->attackRange;
  if (g_windowMgr.homunculusSkillWnd) {
    g_windowMgr.homunculusSkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  int& notified = *reinterpret_cast<int*>(&g_session.companionTargetState[0]);
  const std::int16_t state = packet->hunger;
  if ((state == 1 || state == 5 || state == 10) && notified != state) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_HOMUN_HUNGRY)), 0xFF, 0, 0, 0);
    notified = packet->hunger;
  }
}

// OFFSET: 0x00D3DC00
// FUNCTION: fn_21805
void CGameMode::ReceiveMinimapMarker(const unsigned char* data) {
  const MinimapMarkerPacket* packet = reinterpret_cast<const MinimapMarkerPacket*>(data);
  std::int16_t x = packet->x;
  std::int16_t y = packet->y;
  if (packet->type == 9999) {
    x = -1;
    y = -1;
  }

  this->minimapMarkers[packet->key].x = x;
  this->minimapMarkers[packet->key].y = y;
  this->minimapMarkers[packet->key].effectType = packet->type;
  this->minimapMarkers[packet->key].questIconId = packet->color;
  if (g_windowMgr.miniMapWnd) {
    for (auto it = this->minimapMarkers.begin(); it != this->minimapMarkers.end(); ++it) {
      if (0 != it->second.questIconId) {
        g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_ADD_QUEST_MARK, it->second.effectType, it->second.questIconId, it->second.x, it->second.y);
      }
    }
  }
}

// OFFSET: 0x00D3DD20
// FUNCTION: fn_21806
void CGameMode::ReceiveBookOpen(const unsigned char* packet) {
  char number[12];
  _itoa(*reinterpret_cast<const int*>(packet + 2), number, 10);

  char path[260];
  if (g_serviceType == 10) {
    std::sprintf(path, "%sbook\\%s.txt", g_serverLanguage->servicePrefix.c_str(), number);
  } else {
    std::sprintf(path, "book\\%s.txt", number);
  }

  CFile file;
  if (!file.Open(path, 0, true, false)) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("책이 없습니다"), 0xFF, 0, 0, 0);
  } else {
    g_windowMgr.EnsureWindow(WID_BOOKWND)->SendMsg(nullptr, UM_GETBOOKCONTENTS, reinterpret_cast<std::intptr_t>(number), 287, *reinterpret_cast<const int*>(packet + 6), 0);
    file.Close();
  }
}

// OFFSET: 0x00D3DF40
// FUNCTION: fn_21807
void CGameMode::ReceiveChatRoomEntryFailure(const unsigned char* packet) {
  this->isWaitingEnterRoom = 0;

  switch (packet[2]) {
  case ENTERROOM_FULL:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_MANY_PEOPLE_IN_ROOM)), 0xFF, 0, 0, 0);
    break;
  case ENTERROOM_WRONG_PASSWORD:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INCORRECT_PASSWORD)), 0xFF, 0, 0, 0);
    break;
  case ENTERROOM_KICKED:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_YOU_HAVE_BANNED_FROM_THE_ROOM)), 0xFF, 0, 0, 0);
    break;
  case REFUSETYPE_ZENY:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_MONEY)), 0xFF, 0, 0, 0);
    break;
  case REFUSETYPE_LOWLEVEL:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOT_ENOUGH_LEVEL)), 0xFF, 0, 0, 0);
    break;
  case REFUSETYPE_HIGHLEVEL:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_HIGH_LEVEL)), 0xFF, 0, 0, 0);
    break;
  case REFUSETYPE_JOB:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOT_ACCEPTABLE_JOB)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D3E0B0
// FUNCTION: fn_21808
void CGameMode::IgnoreRemovedPacket2(const unsigned char*) {
}

// OFFSET: 0x00D3E0C0
// FUNCTION: fn_21809
void CGameMode::ReceiveBillingInfo(const unsigned char* packet) {
  char text[512];
  std::memset(text, 0, sizeof(text));
  if (g_serviceType == 3) {
    switch (*reinterpret_cast<const int*>(packet + 2)) {
    case 1:
      std::sprintf(text, LookupMessage(MSI_BILLING_1), *reinterpret_cast<const int*>(packet + 6) / 86400 + 1);
      break;
    case 2: {
      const int seconds = *reinterpret_cast<const int*>(packet + 10);
      std::sprintf(text, LookupMessage(MSI_BILLING_2), seconds / 3600, seconds % 3600 / 60, seconds % 60);
      break;
    }
    }
  } else {
    __time32_t now;
    _time32(&now);
    _localtime32(&now);
    const __time32_t* expireTime = reinterpret_cast<const __time32_t*>(packet + 6);
    const tm* expire = _localtime32(expireTime);
    const int type = *reinterpret_cast<const int*>(packet + 2);
    if (type >= 0x8000 && type <= 0x80FF) {
      if (type & 0x20) {
        std::strcat(text, "(PC방-실버) ");
      }
      if (type & 0x40) {
        std::strcat(text, "(PC방-골드) ");
      }
      if (type & 0x80) {
        std::strcat(text, "(PC방-스페셜) ");
      }
      if (type & 0x01) {
        std::strcat(text, "프리미엄 정액 요금제");
        if (type & 0x10) {
          std::strcat(text, " + Advanced Package");
        } else {
          if (type & 0x02) {
            std::strcat(text, " + Exp UP");
          }
          if (type & 0x04) {
            std::strcat(text, " + Item UP");
          }
          if (type & 0x08) {
            std::strcat(text, " + Death Down");
          }

          char period[256];
          std::memset(period, 0, sizeof(period));
          const int remaining = *reinterpret_cast<const int*>(packet + 10);
          if (*expireTime > now) {
            if (remaining > 0) {
              std::sprintf(
                period, " (종료일시 : %04d/%02d/%02d , 남은시간 : %d 시간 %d 분 %d 초)", expire->tm_year + 1900, expire->tm_mon + 1,
                expire->tm_mday, remaining / 3600, remaining / 60 % 60, remaining % 60
              );
            } else if (remaining == 0) {
              std::sprintf(period, " (종료일시 : %04d/%02d/%02d )", expire->tm_year + 1900, expire->tm_mon + 1, expire->tm_mday);
            }
          } else if (remaining > 0) {
            std::sprintf(period, " (남은시간 : %d 시간 %d 분 %d 초)", remaining / 3600, remaining / 60 % 60, remaining % 60);
          }
          std::strcat(text, period);
        }
      }
    } else {
      switch (type) {
      case 100: {
        const int remaining = *reinterpret_cast<const int*>(packet + 10);
        if (*expireTime > now) {
          if (remaining > 0) {
            std::sprintf(
              text, "귀하는 혼합요금제 사용자 입니다. (종료일시 : %04d/%02d/%02d , 남은시간 : %d 시간 %d 분 %d 초)",
              expire->tm_year + 1900, expire->tm_mon + 1, expire->tm_mday, remaining / 3600, remaining / 60 % 60, remaining % 60
            );
          } else if (remaining == 0) {
            std::sprintf(
              text, "귀하는 정액요금제 사용자 입니다. (종료일시 : %04d 년 %02d 월 %02d 일 %02d 시 %02d 분 %02d 초)",
              expire->tm_year + 1900, expire->tm_mon + 1, expire->tm_mday, expire->tm_hour, expire->tm_min, expire->tm_sec
            );
          }
        } else if (remaining > 0) {
          std::sprintf(
            text, "귀하는 정량요금제 사용자 입니다. (남은시간 : %d 시간 %d 분 %d 초)", remaining / 3600, remaining / 60 % 60, remaining % 60
          );
        }
        break;
      }
      case 1000:
        std::sprintf(text, "PC방의 요금제 정보는 요청할 수 없습니다.");
        break;
      case 10000:
        std::sprintf(text, "귀하는 사크라이 2시간 무료요금제 사용자 입니다.");
        break;
      default:
        std::sprintf(text, "과금정보 요청에 실패 했습니다.");
        break;
      }
    }
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFFFF, 0, 0, 0);
}

struct ActorMessage32 {
  /* 0000 */ std::string text;
  /* 0018 */ std::string extra;
  /* 0030 */ int flags;
  /* 0034 */ int userCount;
  /* 0038 */ int maxUsers;
  /* 003C */ int chatId;
};

// OFFSET: 0x00D3E8B0
// FUNCTION: fn_21810
void CGameMode::ReceiveServerRateNotice(const unsigned char* packet) {
  g_session.partyExpShare = *reinterpret_cast<const int*>(packet + 2);
  g_session.partyItemCollect = packet[6];
  g_session.partyItemDistribute = packet[7];
  if (g_windowMgr.partySettingWnd) {
    reinterpret_cast<UIWindow*>(g_windowMgr.partySettingWnd)->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }

  char text[128];
  std::memset(text, 0, sizeof(text));
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  switch (g_session.partyExpShare) {
  case EXP_NOTSHARED:
    std::sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWEXPDIV), LookupMessage(MSI_EXPDIV1));
    break;
  case EXP_SHARED:
    std::sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWEXPDIV), LookupMessage(MSI_EXPDIV2));
    break;
  case EXP_CHANGE_IMPOSSIBLE:
    std::sprintf(text, "%s - %s : %s(%s)", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWEXPDIV), LookupMessage(MSI_EXPDIV1), LookupMessage(MSI_FAILED_CHANGE));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }

  switch (g_session.partyItemCollect) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWITEMCOLLECT), LookupMessage(MSI_EXPDIV1));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWITEMCOLLECT), LookupMessage(MSI_ITEMCOLLECT2));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }

  switch (g_session.partyItemDistribute) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWITEMDIV), LookupMessage(MSI_EXPDIV1));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWITEMDIV), LookupMessage(MSI_EXPDIV2));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }
}

// OFFSET: 0x00D3F130
// FUNCTION: fn_21811
void CGameMode::ReceiveActorMessage32(const unsigned char* packet) {
  this->chatRoomActorIds[*reinterpret_cast<const std::uint32_t*>(packet + 8)] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4));
  if (!actor) {
    return;
  }

  ActorMessage32 message;
  message.text.assign(reinterpret_cast<const char*>(packet + 17), *reinterpret_cast<const std::int16_t*>(packet + 2) - 17);
  message.flags = packet[16];
  message.userCount = *reinterpret_cast<const std::int16_t*>(packet + 14);
  message.maxUsers = *reinterpret_cast<const std::int16_t*>(packet + 12);
  message.chatId = *reinterpret_cast<const int*>(packet + 8);
  if (message.text.size() > 60) {
    message.text.resize(60);
  }

  const std::int64_t argument = reinterpret_cast<std::intptr_t>(&message);
  actor->SendMsg(nullptr, AM_MAKE_CHAT_TITLE, 0, static_cast<int>(argument), static_cast<int>(argument >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D3F3E0
// FUNCTION: fn_21812
void CGameMode::ReceiveRequestResult(const unsigned char* packet) {
  std::string text("");
  switch (*reinterpret_cast<const std::int16_t*>(packet + 6)) {
  case 0:
  case 8: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND);
    if (window) {
      window->SendMsg(nullptr, UM_UPDATE_CASHSHOP_BALANCE, *reinterpret_cast<const int*>(packet + 8), *reinterpret_cast<const int*>(packet + 12), 0, 0);
    }
    if (8 == *reinterpret_cast<const std::int16_t*>(packet + 6)) {
      text = LookupMessage(MSI_CASH_FAILED_BUY_SOME);
    }
    break;
  }
  case 9:
    text = LookupMessage(MSI_CASH_FAILED_RUNE_OVERCOUNT);
    break;
  case 10:
    text = LookupMessage(MSI_CASH_FAILED_EACHITEM_OVERCOUNT);
    break;
  case 11:
    text = LookupMessage(MSI_CASH_FAILED_UNKOWN);
    break;
  case 12:
    text = LookupMessage(MSI_CASH_FAILED_BUSY);
    break;
  case 303:
    text = "- " + IntToString(303, 0) + " : " + LookupMessage(MSI_ZEROCHATWND_LOCK);
    g_windowMgr.Confirm(text.c_str(), 0, false, false, 0, "Error", 280, 120, 0);
    break;
  case 304:
    text = "- " + IntToString(304, 0) + " : " + LookupMessage(MSI_ZEROCHATWND_UNLOCK);
    g_windowMgr.Confirm(text.c_str(), 0, false, false, 0, "Error", 280, 120, 0);
    break;
  default:
    text = LookupMessage(MSI_DEAL_FAIL);
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D3F760
// FUNCTION: fn_21813
void CGameMode::ReceiveStatusNotice(const unsigned char* packet) {
  const char* message;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 21:
    message = LookupMessage(MSI_GOSPEL1);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 22:
    message = LookupMessage(MSI_GOSPEL2);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 23:
    message = LookupMessage(MSI_GOSPEL3);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 24:
    message = LookupMessage(MSI_GOSPEL4);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 25:
    message = LookupMessage(MSI_GOSPEL5);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 28:
    message = LookupMessage(MSI_GOSPEL6);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 29:
    message = LookupMessage(MSI_GOSPEL7);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 30:
    message = LookupMessage(MSI_GOSPEL8);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 31:
    message = LookupMessage(MSI_GOSPEL9);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 32:
    message = LookupMessage(MSI_GOSPEL10);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  case 40:
    message = LookupMessage(MSI_FULLSTRIP);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x9B9BFF, 8, 0, 0);
    break;
  }
}

// OFFSET: 0x00D40080
// FUNCTION: fn_21816
void CGameMode::ReceiveWindow181Message(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_SEARCH_STORE_INFO);
  if (window) {
    window->SendMsg(nullptr, UM_REFRESH, 1, packet[2], 0, 0);
  }
}

// OFFSET: 0x00D40120
// FUNCTION: fn_21817
void CGameMode::ReceiveNamedResultNotice(const unsigned char* packet) {
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  std::string name(reinterpret_cast<const char*>(packet + 2));
  switch (packet[26]) {
  case 0:
    switch (packet[27]) {
    case 0:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_TO_UNBLOCK_WHISPER)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_TO_UNBLOCK_WHISPER_FAILED1)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_TO_UNBLOCK_WHISPER_FAILED2)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  case 1:
    switch (packet[27]) {
    case 0:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_NO_BLOCK_LIST)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_BLOCK_LIST)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_RENEW_QUEST_TAB_PROCESS)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D40360
// FUNCTION: fn_21818
void CGameMode::ReceiveWhisperFilterResult(const unsigned char* packet) {
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  switch (packet[2]) {
  case WT_EXCLUDE_WHISPER:
    switch (packet[3]) {
    case WHIPSER_SETTING_FAILED:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((this->refuseWhisperName + LookupMessage(MSI_WHISPERIGNOREFAILED1)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case WHISPER_SETTING_FAILED_OVERCOUNT:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((this->refuseWhisperName + LookupMessage(MSI_WHISPERIGNOREFAILED2)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  case WT_INCLUDE_WHISPER:
    switch (packet[3]) {
    case WHISPER_SETTING_SUCCEED:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((this->refuseWhisperName + LookupMessage(MSI_WHISPERNOIGNORESUCCESS)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case WHIPSER_SETTING_FAILED:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((this->refuseWhisperName + LookupMessage(MSI_WHISPERNOIGNOREFAILED1)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case WHISPER_SETTING_FAILED_OVERCOUNT:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((this->refuseWhisperName + LookupMessage(MSI_WHISPERNOIGNOREFAILED2)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D652E0
// FUNCTION: fn_21998
void CGameMode::ReceivePackageItemTest(const unsigned char* packet) {
  const PackageItemTestEntry* entry = reinterpret_cast<const PackageItemTestEntry*>(packet + 8);
  char packageName[260];
  std::memset(packageName, 0, sizeof(packageName));
  ITEM_INFO item;
  item.SetItemId(*reinterpret_cast<const int*>(packet + 4));
  int capacity = sizeof(packageName);
  item.GetIdDisplayName(packageName, capacity, 0);

  char path[2048];
  std::memset(path, 0, sizeof(path));
  int suffix = 0;
  std::sprintf(path, "%s\\PackageItemTest", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  SYSTEMTIME time;
  GetLocalTime(&time);
  std::sprintf(path, "%s\\PackageItemTest\\%s(%d_%d_%d).txt", g_applicationDirectory, packageName, time.wYear, time.wMonth, time.wDay);
  while (_access(path, 0) == 0) {
    ++suffix;
    std::sprintf(path, "%s\\PackageItemTest\\%s(%d_%d_%d)_%d.txt", g_applicationDirectory, packageName, time.wYear, time.wMonth, time.wDay, suffix);
  }

  std::FILE* file = std::fopen(path, "w");
  if (!file) {
    return;
  }

  int total = 0;
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(PackageItemTestEntry);
  std::fprintf(file, "============================START============================\n");
  for (int i = 0; i < count; ++i) {
    char itemName[260];
    std::memset(itemName, 0, sizeof(itemName));
    const PackageItemTestEntry current = *entry;
    item.SetItemId(current.itemId);
    int nameCapacity = sizeof(itemName);
    item.GetIdDisplayName(itemName, nameCapacity, 0);
    std::fprintf(file, "%s\t\t\t%d\t%c%d%c\n", itemName, current.count, '(', current.rate, ')');
    total += current.count;
    ++entry;
  }

  std::fprintf(file, "============================END============================\n");
  std::fprintf(file, "Unpacked %d Packages\n", total);
  std::fclose(file);
}

#pragma pack(push, 1)
struct SummonHpInitPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int32_t summonId;
  /* 0006 */ std::int32_t hp;
  /* 000A */ std::int32_t maxHp;
};

struct SummonHpUpdatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int32_t summonId;
  /* 0006 */ std::uint16_t varId;
  /* 0008 */ std::int32_t value;
};
#pragma pack(pop)

// OFFSET: 0x00D655E0
// FUNCTION: fn_21999
void CGameMode::ReceiveSummonHpInit(const unsigned char* buffer) {
  const SummonHpInitPacket* packet = reinterpret_cast<const SummonHpInitPacket*>(buffer);
  g_session.summonHp[packet->summonId] = packet->hp;
  g_session.summonMaxHp[packet->summonId] = packet->maxHp;

  CRenderObject* actor = this->scene->GetGameActorByAID(packet->summonId);
  if (actor != nullptr) {
    const std::int64_t id = packet->summonId;
    actor->SendMsg(nullptr, AM_UPDATEGAGE, 0, static_cast<int>(id), static_cast<int>(id >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D65680
// FUNCTION: fn_22000
void CGameMode::ReceiveSummonHpUpdate(const unsigned char* buffer) {
  const SummonHpUpdatePacket* packet = reinterpret_cast<const SummonHpUpdatePacket*>(buffer);
  if (g_session.FindSummonMaxHp(packet->summonId) == -1) {
    return;
  }

  switch (packet->varId) {
  case 5:
    g_session.summonHp[packet->summonId] = packet->value;
    break;
  case 6:
    g_session.summonMaxHp[packet->summonId] = packet->value;
    break;
  }

  CRenderObject* actor = this->scene->GetGameActorByAID(packet->summonId);
  if (actor != nullptr) {
    const std::int64_t id = packet->summonId;
    actor->SendMsg(nullptr, AM_UPDATEGAGE, 0, static_cast<int>(id), static_cast<int>(id >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D65730
// FUNCTION: fn_22001
void CGameMode::ReceiveClanLeave(const unsigned char* packet) {
  g_clanInfo->Clear();
  this->nameCard.SetValue3(g_session.m_aid, g_clanInfo->m_clanName);
  if (this->scene->player) {
    this->scene->player->SetSpGauge(0, 0);
  }

  g_windowMgr.RefreshClanWnd();
}

// OFFSET: 0x00D657D0
// FUNCTION: fn_22002
void CGameMode::ReceiveActorNameCard(const unsigned char* packet) {
  char name[25] = {0};

  memcpy_s(name, 24, packet + 6, 24);
  name[24] = '\0';
  this->SetActorNameCard(
    *reinterpret_cast<const std::uint32_t*>(packet + 2), name, reinterpret_cast<const char*>(packet + 0x1E), reinterpret_cast<const char*>(packet + 0x36),
    reinterpret_cast<const char*>(packet + 0x4E), *reinterpret_cast<const int*>(packet + 0x66)
  );
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

// OFFSET: 0x00D65FF0
// FUNCTION: fn_22008
void CGameMode::SpawnSkillWarningPlanes(std::uint32_t aid, std::uint16_t skillId, int level, std::int16_t x, std::int16_t y, int duration) {
  CRenderObject* actor = g_session.FindGameActor(aid);
  if (!actor) {
    return;
  }

  int width = 0;
  int height = 0;
  if (!g_session.m_hotkeyScript->Call("GetSkillScale", "dd>dd", skillId, level, &width, &height)) {
    return;
  }

  CWarningPlaneEffect* effect = static_cast<CWarningPlaneEffect*>(g_effectMgr->Spawn(actor->render.accountId, 1114, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
  if (!effect) {
    return;
  }

  for (int column = 0; column < width; ++column) {
    for (int row = 0; row < height; ++row) {
      vector3d position = {0.0f, 0.0f, 0.0f};
      this->CellCenter(x - width / 2 + column, y - height / 2 + row, position.x, position.z);
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      position.y = mode->scene->terrain->Height(position.x, position.z);
      effect->AddPlane(position);
      effect->expireTime = timeGetTime() + duration;
    }
  }
}

// OFFSET: 0x00D66200
// FUNCTION: fn_22009
void CGameMode::ReceiveGameOptions(int count, const GameOptionEntry* entries) {
  for (int i = 0; i < count; ++i) {
    switch (entries[i].id) {
    case 1:
      g_gameOptionFlag = entries[i].value != 0;
      break;
    }
  }
}

// OFFSET: 0x00D66370
// FUNCTION: fn_22011
void CGameMode::ReceiveActorClanName(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorClanNamePacket*>(data);

  this->nameCard.SetValue3(packet->aid, std::string(packet->name, packet->name + (packet->length - 8)));
}

// OFFSET: 0x00D66470
// FUNCTION: fn_22013
void CGameMode::ReceiveRuneRewardPacket(const unsigned char* packet) {
  if (!packet) {
    return;
  }

  switch (packet[0x32]) {
  case 0: {
    CEffect* effect = g_effectMgr->Spawn(g_session.m_aid, 2399, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const vector3d offset = {598.0f, 219.0f, 0.0f};
    const int* words = reinterpret_cast<const int*>(&offset);
    effect->SetWindowOffset(words[0], words[1], words[2], 1, 361);
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND)) {
      window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
    }
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_RUNESYSTEM_DECOMRESULTWND)) {
      window->SendMsg(nullptr, UM_SETSTATE, reinterpret_cast<std::intptr_t>(packet), 0, 0, 0);
      window->Invalidate();
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_LACK_OF_ITEM)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_ALREADY_EQUIPED)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_ALREADY_EQUIPED), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_NOTHING_EQUIPED)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_NOTHING_EQUIPED), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_INVALID_ID)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_INVALID_ID), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_MAX_LEVEL)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_MAX_LEVEL), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_INVALID_OWNER)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_INVALID_OWNER), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_TABLE_NOT_EXIT)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_TABLE_NOT_EXIT), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 8:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_DB_LOAD_NOT_YET)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_DB_LOAD_NOT_YET), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }

  g_runeSystemMgr->SetRequestPending(false);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND)) {
    window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x69, 1000, 0, 0, 0);
  }
}

// OFFSET: 0x00D66840
// FUNCTION: fn_22015
void CGameMode::ReceiveDeferredReset(const unsigned char*) {
  g_session.m_options.Set(5, 0);
  GetRagConnection().ResetDeferred();
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

  int count = packet->count;
  const int size = packet->length - 9;
  if (size != count * sizeof(std::uint32_t)) {
    count = size / sizeof(std::uint32_t);
  }

  for (int i = 0; i < count; ++i) {
    g_runeSystemMgr->SetRuneGrade(packet->tagId, 1, packet->ids[i], 0, 0);
  }

  CTagData* tag = g_runeSystemMgr->GetTag(packet->tagId);
  if (!tag) {
    return;
  }

  tag->UpdateActiveCount(1);
  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (window) {
    window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
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
  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (window) {
    window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D671A0
// FUNCTION: fn_22025
void CGameMode::ReceiveRuneRewardResult(int result, int tagId, std::int64_t id, int step) {
  switch (result) {
  case 0: {
    g_runeSystemMgr->SetRewardStep(tagId, id, step);
    UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
    if (window) {
      if (step > 0) {
        window->SendMsg(window, UM_RUNE_REWARD_EFFECT, 224, step - 1, 0, 0);
      }

      window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SECOND_PASSWORD_ONOFF_TIP_MSG)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_REWARD_RUNE_UI_NOT_OPENED)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_REWARD_TABLE_NOT_EXIT)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_MOVE_INDOOR_MAP)), 0x64FFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_LOGINOUT)), 0x64FFFF, 0, 0, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_TAB_ETC)), 0x64FFFF, 0, 0, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_FOG_ON)), 0x64FFFF, 0, 0, 0);
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

  const RuneRewardStepEntry* entry = packet->entries;
  for (int i = 0; i < packet->count; ++i) {
    g_runeSystemMgr->SetRewardStep(packet->tagId, entry->id, entry->step);
    ++entry;
  }


  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (window) {
    window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
  }
}
// OFFSET: 0x00D66B10
// FUNCTION: fn_22021
void CGameMode::ReceiveRodexMailDeleteResult(int mailId, int result) {
  int message;
  if (result == 0) {
    if (g_windowMgr.rodexContentsWnd) {
      reinterpret_cast<UIWindow*>(g_windowMgr.rodexContentsWnd)->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    }

    RodexMailMap::iterator mail = g_rodexMailStore->m_mails0.find(mailId);
    if (mail != g_rodexMailStore->m_mails0.end()) {
      if (!mail->second.isRead) {
        --g_rodexMailStore->unreadCount;
        if (g_rodexMailStore->unreadCount == 0) {
          if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
            window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 476, 0, 0);
          }
        }
      }

      g_rodexMailStore->m_mails0.erase(mail);
      if (g_windowMgr.rodexInboxWnd) {
        reinterpret_cast<UIRodexInboxWnd*>(g_windowMgr.rodexInboxWnd)->RemoveInboxMail(mailId);
      }
    }
    message = 3865;
  } else {
    message = 3866;
  }

  g_windowMgr.Confirm(LookupMessage(message), 0, true, false, 0, "Message", 280, 120, false);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D67360
// FUNCTION: fn_22027
void CGameMode::ReceiveRuneGradeResult(int result, int tagId, int type, std::int64_t id, int grade, int gradeLevel) {
  if (this->HandleRuneResult(result) == 0) {
    const int previousSlots = g_runeSystemMgr->GetCurSlotCount();
    const int state = g_runeSystemMgr->SetRuneGrade(tagId, type, id, grade, gradeLevel);
    if (state == 2) {
      CEffect* effect = g_effectMgr->Spawn(g_session.m_aid, 2397, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      const vector3d offset = {784.0f, 161.0f, 0.0f};
      const int* words = reinterpret_cast<const int*>(&offset);
      effect->SetWindowOffset(words[0], words[1], words[2], 1, 361);
    } else if (state == 1) {
      CEffect* effect = g_effectMgr->Spawn(g_session.m_aid, 2398, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      const vector3d offset = {784.0f, 161.0f, 0.0f};
      const int* words = reinterpret_cast<const int*>(&offset);
      effect->SetWindowOffset(words[0], words[1], words[2], 1, 361);
    } else {
      CEffect* effect = g_effectMgr->Spawn(g_session.m_aid, type != 0 ? 2395 : 2396, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      vector3d offset = {0.0f, 0.0f, 0.0f};
      if (type == 0) {
        offset.x = 473.0f;
        offset.y = 286.0f;
      } else {
        offset.x = 598.0f;
        offset.y = 271.0f;
      }
      const int* words = reinterpret_cast<const int*>(&offset);
      effect->SetWindowOffset(words[0], words[1], words[2], 1, 361);
    }

    if (RuneSetData* runeSet = g_runeSystemMgr->GetCurRuneSet()) {
      const int slots = runeSet->CountActiveSlots();
      if (previousSlots != slots) {
        g_runeSystemMgr->SetCurSlotCount(slots);
        const int itemId = -1;
        const int kind = 1;
        EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RefreshExpansion(kind, itemId);
      }
    }
  }

  g_runeSystemMgr->SetRequestPending(false);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND)) {
    window->SendMsg(window, UM_REFRESH, type, 0, 0, 0);
    window->SendMsg(nullptr, 0x69, 1000, 0, 0, 0);
  }
}

// OFFSET: 0x00D66CE0
// FUNCTION: fn_22022
int CGameMode::HandleRuneResult(int result) {
  switch (result) {
  case 0:
    return 0;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_RUNE_UI_NOT_OPENED)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_RUNESYSTEM_RUNE_UI_NOT_OPENED), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_FAIL)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_FAIL), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_CANNOT_FIND_ITEM)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_CANNOT_FIND_ITEM), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_NOT_ENOUGH_INVENTORY)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_NOT_ENOUGH_INVENTORY), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_HEAVY_INVENTORY)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_HEAVY_INVENTORY), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_OVER_ITEM_COUNT)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_OVER_ITEM_COUNT), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_RUNE_UI_NOT_OPENED)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_RUNE_UI_NOT_OPENED), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 8:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_TABLE_NOT_EXIT)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_TABLE_NOT_EXIT), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_USER_REPORT_NOT_USE_AREA)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_DECOM_NOT_ENOUGH_ITEM_COUNT), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_USER_REPORT_NOT_USE_AREA)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_USER_REPORT_NOT_USE_AREA), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 12:
  case 13:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRAPHIC_SETTING_RENDER_SYSTEM)), 0xFF, 0, 0, 0);
    g_windowMgr.Confirm(LookupMessage(MSI_GRAPHIC_SETTING_RENDER_SYSTEM), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }

  return result;
}

// OFFSET: 0x00D67540
// FUNCTION: fn_22028
void CGameMode::ApplyRuneSetResult(int result, int tagId, int runeSetId) {
  if (this->HandleRuneResult(result) == 0) {
    g_runeSystemMgr->SetCurRuneSet(tagId, runeSetId, -1, -1);
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (window) {
    window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D67590
// FUNCTION: fn_22029
void CGameMode::ApplyRuneSetResultWithRequest(int result, int tagId, int runeSetId, int grade, int slotCount) {
  if (this->HandleRuneResult(result) == 0) {
    g_runeSystemMgr->SetCurRuneSet(tagId, runeSetId, grade, slotCount);
  }

  g_runeSystemMgr->SendRequest(0x0BCB, nullptr, tagId);

  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (window) {
    window->SendMsg(window, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D677C0
// FUNCTION: fn_22033
void CGameMode::ReceiveGameOptionResult(int command, int result, int enabled) {
  switch (result) {
  case 1:
    if (command == 1) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RODEX_SPAMOPTION_USERBLOCK)), 0x64FFFF, 0, 0, 0);
    }

    g_gameOptionFlag = enabled != 0;
    g_windowMgr.RefreshGameSettings();
    if (!g_gameOptionFlag) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RODEX_SPAMOPTION_RETURN_SUCCESS)), 0x64FFFF, 0, 0, 0);
    } else {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RODEX_SPAMOPTION_RETURN_FAIL)), 0x64FFFF, 0, 0, 0);
    }

    break;
  }
}


// OFFSET: 0x00D678E0
// FUNCTION: fn_22035
void CGameMode::ReceiveServiceRequest(unsigned char type, int value) {
  switch (type) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    this->OpenServiceWindow(type);
    break;
  case OUT_UI_TIP:
    if (UITipboxWnd* window = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(WID_TIPBOXWND))) {
      window->ShowTip(value);
    }
    break;
  case OUT_UI_QUEST:
    g_renewQuestMgr->ShowQuestInfo(value);
    break;
  case OUT_UI_ATTENDANCE:
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_CHECKATTENDANCEWND)) {
      window->SendMsg(nullptr, UM_SETINFO, value, 0, 0, 0);
    }
    break;
  case OUT_UI_ENCHANTGRADE:
    g_windowMgr.EnsureWindow(WID_GRADEENCHANTWND);
    break;
  }
}

// OFFSET: 0x00D679E0
// FUNCTION: fn_22037
void CGameMode::ReceiveGradeEnchantResult(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t*>(packet + 2));
  if (item.identity[1]) {
    const int result = *reinterpret_cast<const int*>(packet + 6);
    if (result == ENCHANTGRADE_UPGRADE_SUCCESS || result == ENCHANTGRADE_UPGRADE_DOWNGRADE) {
      if (result == ENCHANTGRADE_UPGRADE_SUCCESS) {
        item.refiningLevel = 0;
      }
      item.grade = *reinterpret_cast<const std::int16_t*>(packet + 4);
      g_session.UpdateInventoryItem(item, 1);
    }
  }

  auto* window = static_cast<UIGradeEnchantWnd*>(g_windowMgr.LookupWindow(WID_GRADEENCHANTWND));
  if (window) {
    const int result = *reinterpret_cast<const int*>(packet + 6);
    switch (result) {
    case ENCHANTGRADE_UPGRADE_SUCCESS:
      window->SetResult(2, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1607, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1608, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)->SetDelay(2800);
      break;
    case ENCHANTGRADE_UPGRADE_FAILED:
    case ENCHANTGRADE_UPGRADE_DOWNGRADE:
    case ENCHANTGRADE_UPGRADE_BREAK:
    case ENCHANTGRADE_UPGRADE_PROTECTED:
      window->SetResult(2, result);
      g_effectMgr->Spawn(g_session.m_aid, 1607, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1609, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)->SetDelay(2800);
      break;
    }
  }
}

// OFFSET: 0x00D67C20
// FUNCTION: fn_22038
void CGameMode::ReceiveAchievementUpdate(
  int score, short achievementLevel, int completed, int total, int id, unsigned char achieved, const std::uint32_t* objectives,
  __time32_t completedAt, unsigned char rewarded
) {
  const SessionRecord6144 previous = g_session.FindAchievementRecord(id);
  if (reinterpret_cast<const unsigned char*>(&previous.completionFlags)[1] == 0 && achieved == 1) {
    this->achievementBalloonTick = timeGetTime();
    if (!this->achievementBalloon) {
      UIAchBalloonText* balloon =
        new UIAchBalloonText();
      this->achievementBalloon = balloon;
      balloon->SetSize(g_renderer->viewport.width, 50);
      g_windowMgr.AddRenderedFront(this->achievementBalloon);
    }

    AchievementInfo info;
    info.rewardItemId = -1;
    info.rewardTitleId = -1;
    info.rewardBuffId = -1;
    g_achievementMgr->GetAchievement(info, id);
    const std::string text = FormatOwnedString(LookupMessage(MSI_NOTICE_NEXT_ROULLETE_STAGE), info.title.c_str());
    static_cast<UIBalloonText*>(this->achievementBalloon)->WrapText(text.c_str(), 72);
    static_cast<UIBalloonText*>(this->achievementBalloon)->m_fontHeight = 20;
    static_cast<UIBalloonText*>(this->achievementBalloon)->m_textColor = 0xFFFFFF;
    static_cast<UIBalloonText*>(this->achievementBalloon)->m_outlineColor = 0;
    this->achievementBalloon->Move(0, 50);
    g_effectMgr->Spawn(g_session.m_aid, 0x446, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0x1D9, 1, 0);
    }
  }

  g_session.achievementScore = score;
  g_session.achievementLevel = achievementLevel;
  g_session.achievementCompleted = completed;
  g_session.achievementTotal = total;

  AchievementRecordPacket record;
  record.id = id;
  record.completed = achieved;
  std::memcpy(record.objectives, objectives, sizeof(record.objectives));
  record.completedAt = completedAt;
  record.rewarded = rewarded;
  g_session.UpdateAchievementRecord(record);

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_ACHIEVEMENTWND)) {
    window->SendMsg(nullptr, UM_REFRESH, id, 0, 0, 0);
  }

  g_session.RefreshTrackedAchievement(id);
  if (g_session.HasTrackedAchievements() == true) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ACHTRACINGWND)) {
      window->Invalidate();
    }
  }
}

// OFFSET: 0x00D681A0
// FUNCTION: fn_22039
void CGameMode::ReceiveAchievementResult(unsigned char result, int id) {
  if (result != 0) {
    return;
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_ACHIEVEMENTWND)) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }

  const int index = g_session.AddAchievementJobTitle(id);
  if (index != -1) {
    g_session.AddAchievementId(index);
  }
}

// OFFSET: 0x00D68200
// FUNCTION: fn_22040
void CGameMode::ReceiveItemReward2(const unsigned char* data) {
  const ItemRewardPacket2* packet = reinterpret_cast<const ItemRewardPacket2*>(data);
  ITEM_INFO item;
  if (packet->result == 0) {
    item.identity[1] = packet->index;
    item.quantity = packet->quantity;
    item.SetItemId(packet->itemId);
    item.identity[0] = packet->type;
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));

    int count = 0;
    for (int i = 0; i < 5; ++i) {
      if (packet->options[i].id != 0) {
        ++count;
      }
    }
    item.identity[2] = packet->expireTime;
    item.optionCount = count;
    for (std::int64_t index = 0; index < count; ++index) {
      reinterpret_cast<ItemRewardOption*>(item.options)[index] = packet->options[index];
    }
    item.flags[0] = packet->bound;
    item.grade = packet->grade;
  }

  const std::uint16_t value = packet->value;
  ITEM_INFO reward = item;
  switch (packet->result) {
  case 0:
    g_session.AddStackedItem(reward);
    g_session.RemoveInventoryItem(reward.identity[1], reward.quantity);
    g_session.achievementResult = value;
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_SEND_MAIL)), 0x64FFFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_CLOSE_ROULLETTE)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_OPEN_ROULLETTE)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RESULT_ALREADY_LOSING_IDX)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RODEX_READ)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D684C0
// FUNCTION: fn_22041
void CGameMode::ReceiveItemReward(const unsigned char* data) {
  const ItemRewardPacket* packet = reinterpret_cast<const ItemRewardPacket*>(data);
  switch (packet->result) {
  case 0: {
    ITEM_INFO item;
    item.identity[1] = packet->index;
    item.quantity = packet->quantity;
    item.SetItemId(packet->itemId);
    item.identity[0] = packet->type;
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.optionCount = CountItemRandomOptions(reinterpret_cast<const unsigned char*>(packet->options));
    item.identity[2] = packet->expireTime;
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<ItemRewardOption*>(item.options)[index] = packet->options[index];
    }
    item.flags[0] = packet->bound;
    g_session.AddStackedItem(item);
    g_session.RemoveInventoryItem(packet->index, packet->quantity);
    g_session.achievementResult = packet->value;
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_SEND_MAIL)), 0x64FFFF, 0, 0, 0);
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_CLOSE_ROULLETTE)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_OPEN_ROULLETTE)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RESULT_ALREADY_LOSING_IDX)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RODEX_READ)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D686F0
// FUNCTION: fn_22042
void CGameMode::ReceiveGuildEmblemVersion(std::uint32_t aid, int guildId, int emblemVersion) {
  if (this->scene) {
    CRenderObject* actor = this->scene->GetGameActorByAID(aid);
    if (actor) {
      static_cast<CGameActor*>(actor)->SetGdid(guildId);
    }
  }

  if (guildId) {
    g_guildMgr->SetEmblemVersion(guildId, emblemVersion);
  }
}

// OFFSET: 0x00D658B0
// FUNCTION: fn_22004
int CGameMode::RejectRequest(int, int, int, int, int) {
  return -1;
}

// OFFSET: 0x00D65FA0
// FUNCTION: fn_22006
void CGameMode::EnsureWindow259(const unsigned char*) {
  g_windowMgr.EnsureWindow(WID_SECONDCOSTUMEWND);
}

// OFFSET: 0x00D66A50
// FUNCTION: fn_22019
void CGameMode::IgnoreRemovedPacket12(const unsigned char*, int, int, int, int) {
}

// OFFSET: 0x00D67670
// FUNCTION: fn_22031
void CGameMode::ToggleWindow169(unsigned char enable) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (enable != 0) {
    if (!window) {
      g_windowMgr.EnsureWindow(WID_RUNESYSTEMWND);
    }
  } else if (window) {
    g_windowMgr.CloseWindow(WID_RUNESYSTEMWND);
  }
}

#pragma pack(push, 1)
struct RequestWindow169Packet {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char flags;
};
#pragma pack(pop)

// OFFSET: 0x00D67600
// FUNCTION: fn_22030
void CGameMode::RequestWindow169(unsigned char enable) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND);
  if (enable != 0) {
    if (!window) {
      RequestWindow169Packet packet;
      packet.id = 0x0BE0;
      packet.flags = 0;
      GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
    }
  } else if (window) {
    g_windowMgr.CloseWindow(WID_RUNESYSTEMWND);
  }
}

// OFFSET: 0x00D66A60
// FUNCTION: fn_22020
void CGameMode::ShowNoticeText(std::string text) {
  g_windowMgr.SendNotice(
    UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_RODEX_SPAMOPTION_WARNING), text.c_str()).c_str()), 0x64FFFF, 0, 0, 0
  );
}

struct DdukddakBoxTestEntry {
  /* 0000 */ int itemId;
  /* 0004 */ int count;
};

// OFFSET: 0x00D64A90
// FUNCTION: fn_21997
void CGameMode::ReceiveDdukddakBoxTest(const unsigned char* packet) {
  std::map<int, int> counts;
  const DdukddakBoxTestEntry* source = reinterpret_cast<const DdukddakBoxTestEntry*>(packet + 8);
  int total = 0;
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(DdukddakBoxTestEntry);
  for (int i = 0; i < count; ++i) {
    DdukddakBoxTestEntry entry;
    memcpy_s(&entry, sizeof(entry), source, sizeof(entry));
    if (counts.find(entry.itemId) == counts.end()) {
      counts.insert(std::make_pair(entry.itemId, 0));
    }
    counts[entry.itemId] += entry.count;
    total += entry.count;
    ++source;
  }

  std::vector<std::pair<int, int>> items(counts.begin(), counts.end());
  std::sort(items.begin(), items.end(), [](const std::pair<int, int>& left, const std::pair<int, int>& right) {
    if (left.second != right.second) {
      return left.second > right.second;
    }
    return left.first < right.first;
  });

  char name[64];
  std::memset(name, 0, sizeof(name));
  ITEM_INFO box;
  box.SetItemId(*reinterpret_cast<const int*>(packet + 4));
  int capacity = sizeof(name);
  box.GetIdDisplayName(name, capacity, 0);

  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string folder = "DdukddakBoxTest";
  std::string fileName = FormatOwnedString("%d%02d%02d_(%s).txt", time.wYear, time.wMonth, time.wDay, name);
  std::string path = FormatOwnedString("%s\\%s\\%s", g_applicationDirectory, folder.c_str(), fileName.c_str());
  path = static_cast<const std::string&>(CreateUniqueFileName(path));

  std::ofstream output(path.c_str());
  output << FormatOwnedString("%s\t%s\t%s\t%s\n\n", "ID", "name", "count", "property");
  for (auto item = items.begin(); item != items.end(); ++item) {
    ITEM_INFO info;
    info.SetItemId(item->first);
    info.GetIdDisplayName(name, capacity, 0);
    std::string amount = FormatOwnedString("%d", item->second);
    std::string ratio = FormatOwnedString("%.5f%%", static_cast<float>(item->second) / static_cast<float>(total) * 100.0f);
    output << FormatOwnedString("%d\t%s\t%s\t%s\n", item->first, name, amount.c_str(), ratio.c_str());
  }

  output << FormatOwnedString("\nresult item count:%d\n", total);
  output.close();
}

#include "UI/Text/UIBroadcastBalloon.h"

extern template decltype(auto) std::list<BroadcastBalloonLine>::emplace_back<std::string&, std::uint32_t&>(std::string&, std::uint32_t&);

#include <chrono>

// OFFSET: 0x00D3C9B0
// FUNCTION: fn_21795
void CGameMode::ReceiveBroadcastBalloonNotice(const unsigned char* packet) {
  std::uint32_t color = 0x62B0FF;
  std::string text = FormatOwnedString(
    LookupMessage(MSI_PCBANG_EVENT), *reinterpret_cast<const int*>(packet + 2), *reinterpret_cast<const int*>(packet + 6),
    *reinterpret_cast<const int*>(packet + 10)
  );
  if (g_serviceType == 12) {
    text = LookupMessage(MSI_BILLING_2);
  }

  if (!text.empty()) {
    if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
      BroadcastBalloonMessage message;
      message.lines.emplace_back(text, color);
      balloon->AddMessage(0, message);
    }
    if (g_serviceType == 0) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x62B0FF, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D65870
// FUNCTION: fn_22003
void CGameMode::ReceivePartyMemberInfoWithCharacterId(const unsigned char* packet) {
  this->UpdatePartyMemberInfo(
    reinterpret_cast<const char*>(packet + 0x2F), *reinterpret_cast<const int*>(packet + 2), reinterpret_cast<const char*>(packet + 0x47),
    *reinterpret_cast<const int*>(packet + 10), packet[0x16], *reinterpret_cast<const std::uint16_t*>(packet + 0x0E),
    *reinterpret_cast<const std::uint16_t*>(packet + 0x10), reinterpret_cast<const char*>(packet + 0x17), packet[0x57], packet[0x58],
    *reinterpret_cast<const int*>(packet + 6)
  );
}

// OFFSET: 0x00D65FC0
// FUNCTION: fn_22007
void CGameMode::ReceiveStatusChangeEntry(const unsigned char* record) {
  this->ApplyStatusChange(
    *reinterpret_cast<const std::uint32_t*>(record + 2), 1, *reinterpret_cast<const std::int16_t*>(record + 6),
    *reinterpret_cast<const std::uint32_t*>(record + 0x0C), *reinterpret_cast<const int*>(record + 0x10),
    *reinterpret_cast<const int*>(record + 0x14), *reinterpret_cast<const int*>(record + 0x18), 1,
    *reinterpret_cast<const std::uint32_t*>(record + 8)
  );
}

// OFFSET: 0x00D66810
// FUNCTION: fn_22014
void CGameMode::ReceiveStatusChangeEffect2(const unsigned char* packet) {
  this->ApplyStatusChange(
    *reinterpret_cast<const std::uint32_t*>(packet + 4), packet[8], *reinterpret_cast<const std::int16_t*>(packet + 2),
    *reinterpret_cast<const std::uint32_t*>(packet + 0x0D), *reinterpret_cast<const int*>(packet + 0x11),
    *reinterpret_cast<const int*>(packet + 0x15), *reinterpret_cast<const int*>(packet + 0x19), 0,
    *reinterpret_cast<const std::uint32_t*>(packet + 9)
  );
}

// OFFSET: 0x00D66A30
// FUNCTION: fn_22018
void CGameMode::ReceiveActorStatusValuePacket2(const unsigned char* packet) {
  this->ReceiveActorStatusValue(
    *reinterpret_cast<const std::uint32_t*>(packet + 2), *reinterpret_cast<const int*>(packet + 6), *reinterpret_cast<const int*>(packet + 10),
    *reinterpret_cast<const int*>(packet + 14)
  );
}

// OFFSET: 0x00D676C0
// FUNCTION: fn_22032
void CGameMode::ReceiveWindow95Data(const unsigned char* packet) {
  const int size = *reinterpret_cast<const std::int16_t*>(packet + 2) - 8;
  if (size == 0) {
    return;
  }

  std::vector<unsigned char> data;
  data.insert(data.end(), packet + 8, packet + 8 + size);
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_SELECTCARTWND)) {
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(data.data()), size, *reinterpret_cast<const int*>(packet + 4), 0);
  }
}

// OFFSET: 0x00D679A0
// FUNCTION: fn_22036
void CGameMode::ReceiveActorSkillResult(const unsigned char* packet) {
  const ActorSkillResultPacket& value = *reinterpret_cast<const ActorSkillResultPacket*>(packet);
  this->NotifySkillCasting(
    value.source, value.target, value.skill, value.x, value.y, value.tick, value.level, value.flag, value.result
  );
}

// OFFSET: 0x00D66400
// FUNCTION: fn_22012
void CGameMode::ReceiveSimulationResult(const unsigned char* packet) {
  const SimulationResultPacket& value = *reinterpret_cast<const SimulationResultPacket*>(packet);
  switch (value.kind) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("Simulator Start Fail!!!!!!!!!!!!"), 255, 0, 0, 0);
    break;
  case 1:
    this->ReceivePackageSimulation(packet);
    break;
  case 2:
    this->ReceiveRandomOptionSimulation(packet);
    break;
  case 3:
    this->ReceiveEnchantSimulation(packet);
    break;
  case 4:
    this->ReceiveRandomUpgradeSimulation(packet);
    break;
  }
}

// OFFSET: 0x00D415C0
// FUNCTION: fn_21825
void CGameMode::UpdateSkillCooldown(std::uint16_t skill, std::uint32_t remaining, std::uint32_t duration) {
  ShortcutCooldownRecord value;
  value.id = skill;
  const std::uint32_t now = g_replayAnimationScaling == 1 ? Replay::Instance().elapsed : timeGetTime();
  value.end = now + remaining;
  value.duration = duration;

  for (const ShortcutCooldownIcon& icon : g_session.m_mainCooldownIcons) {
    if (icon.id == skill) {
      value.main.push_back(CooldownState(icon.slot, icon.x, icon.y, duration, value.end - remaining, duration - remaining, 0));
    }
  }

  for (const ShortcutCooldownIcon& icon : g_session.m_alternateCooldownIcons) {
    if (icon.id == skill) {
      value.alternate.push_back(CooldownState(icon.slot, icon.x, icon.y, duration, value.end - remaining, duration - remaining, 0));
    }
  }

  const auto found = std::find_if(g_session.m_requestTimers.begin(), g_session.m_requestTimers.end(), [skill](const ShortcutCooldownRecord& timer) {
    return timer.id == skill;
  });
  if (found == g_session.m_requestTimers.end()) {
    g_session.m_requestTimers.push_back(value);
  }
}

// OFFSET: 0x00D41250
// FUNCTION: fn_21823
void CGameMode::Zc_Skill_Entry(const unsigned char* record) {
  GroundSkillPacket& packet = *reinterpret_cast<GroundSkillPacket*>(const_cast<unsigned char*>(record));
  GroundSkillSpawnData data;
  data.aid = packet.aid;
  data.owner = packet.owner;
  data.type = packet.type;
  data.x = packet.x;
  data.y = packet.y;
  data.visible = true;
  data.level = 0;
  packet.visible = 1;


  CSkill* skill = static_cast<CSkill*>(this->scene->GetSkillByAID(data.aid));
  if (data.owner != g_session.m_aid && !data.visible) {

    const auto found = std::find(this->m_skillUnitIds.begin(), this->m_skillUnitIds.end(), packet.aid);
    if (found != this->m_skillUnitIds.end()) {
      this->m_skillUnitIds.erase(found);
      packet.visible = 0;
      data.visible = false;
    }
  } else if (data.visible) {
    this->m_skillUnitIds.push_back(packet.aid);
  }

  if (!skill) {
    if (packet.type < 45) {
      return;
    }
    skill = new CSkill(packet.aid);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
  }

  skill->sourceActorId = packet.owner;
  const int address = reinterpret_cast<int>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D40730
// FUNCTION: fn_21819
void CGameMode::Zc_Skill_Entry2(const unsigned char* record) {
  GroundSkillPacket& packet = *reinterpret_cast<GroundSkillPacket*>(const_cast<unsigned char*>(record));
  GroundSkillSpawnData data;
  data.aid = packet.aid;
  data.owner = packet.owner;
  data.type = packet.type;
  data.x = packet.x;
  data.y = packet.y;
  data.visible = true;
  data.level = 0;
  packet.visible = 1;

  if (packet.graffiti == 1) {
    CGraffiSkill* skill = new CGraffiSkill;
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    skill->SendMsg(nullptr, AM_SETMASTERGID, 0, packet.aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int address = reinterpret_cast<int>(packet.text);
    skill->SendMsg(nullptr, AM_SET_SKILL, 0, address, address >> 31, data.x, data.x >> 31, data.y, data.y >> 31, 0, 0, 0, 0, 0, 0);
    return;
  }

  CSkill* skill = static_cast<CSkill*>(this->scene->GetSkillByAID(data.aid));
  if (data.owner != g_session.m_aid && !data.visible) {
    const auto found = std::find(this->m_skillUnitIds.begin(), this->m_skillUnitIds.end(), packet.aid);
    if (found != this->m_skillUnitIds.end()) {
      this->m_skillUnitIds.erase(found);
      packet.visible = 0;
      data.visible = false;
    }
  } else if (data.visible) {
    this->m_skillUnitIds.push_back(packet.aid);
  }

  if (!skill) {
    if (packet.type < 45) {
      return;
    }
    skill = new CSkill(0);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
  }

  const int address = reinterpret_cast<int>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D40970
// FUNCTION: fn_21820
void CGameMode::ReceiveGroundSkill8(const unsigned char* record) {
  const GroundSkillPacket8& packet = *reinterpret_cast<const GroundSkillPacket8*>(record);
  if (packet.type == 176) {
    if (packet.length <= 19) {
      return;
    }
    CGraffiSkill* skill = new CGraffiSkill;
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    skill->SendMsg(nullptr, AM_SETMASTERGID, 0, packet.aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int address = reinterpret_cast<int>(packet.text);
    const int x = packet.x;
    const int y = packet.y;
    skill->SendMsg(nullptr, AM_SET_SKILL, 0, address, address >> 31, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0);
    return;
  }

  GroundSkillSpawnData data;
  data.aid = packet.aid;
  data.owner = packet.owner;
  data.type = packet.type;
  data.x = packet.x;
  data.y = packet.y;
  data.visible = packet.visible;
  data.level = packet.level;
  if (data.owner == g_session.m_aid) {
    if (data.visible) {
      this->m_skillUnitIds.push_back(packet.aid);
    }
  } else if (!data.visible) {
    const auto found = std::find(this->m_skillUnitIds.begin(), this->m_skillUnitIds.end(), data.aid);
    if (found != this->m_skillUnitIds.end()) {
      this->m_skillUnitIds.erase(found);
      data.visible = 0;
    }
  } else {
    this->m_skillUnitIds.push_back(packet.aid);
  }

  CSkill* skill = static_cast<CSkill*>(this->scene->GetSkillByAID(packet.aid));
  if (!skill) {
    if (packet.type < 45) {
      return;
    }
    skill = new CSkill(packet.aid);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    if (!skill) {
      return;
    }
  }
  skill->sourceActorId = packet.owner;
  const int address = reinterpret_cast<int>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D40BC0
// FUNCTION: fn_21821
void CGameMode::ReceiveGroundSkill32(const unsigned char* record) {
  const GroundSkillPacket32& packet = *reinterpret_cast<const GroundSkillPacket32*>(record);
  if (packet.type == 176) {
    if (packet.length <= 22) {
      return;
    }
    CGraffiSkill* skill = new CGraffiSkill;
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    skill->SendMsg(nullptr, AM_SETMASTERGID, 0, packet.aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int address = reinterpret_cast<int>(packet.text);
    const int x = packet.x;
    const int y = packet.y;
    skill->SendMsg(nullptr, AM_SET_SKILL, 0, address, address >> 31, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0);
    return;
  }

  GroundSkillSpawnData data;
  data.aid = packet.aid;
  data.owner = packet.owner;
  data.type = packet.type;
  data.x = packet.x;
  data.y = packet.y;
  data.visible = packet.visible;
  data.level = packet.level;
  if (data.owner == g_session.m_aid) {
    if (data.visible) {
      this->m_skillUnitIds.push_back(packet.aid);
    }
  } else if (!data.visible) {
    const auto found = std::find(this->m_skillUnitIds.begin(), this->m_skillUnitIds.end(), data.aid);
    if (found != this->m_skillUnitIds.end()) {
      this->m_skillUnitIds.erase(found);
      data.visible = 0;
    }
  } else {
    this->m_skillUnitIds.push_back(packet.aid);
  }

  CSkill* skill = static_cast<CSkill*>(this->scene->GetSkillByAID(packet.aid));
  if (!skill) {
    const int type = packet.type;
    if (!((type >= 45 && type < 1000) || (type > 1000 && type < 3999) ||
          static_cast<unsigned>(type) - 20021u <= 11978u)) {
      return;
    }
    skill = new (std::nothrow) CSkill(packet.aid);
    this->scene->objects.push_back(skill);
    this->scene->skills.push_back(skill);
    if (!skill) {
      return;
    }
  }
  skill->sourceActorId = packet.owner;
  const int address = reinterpret_cast<int>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

#pragma pack(push, 1)
struct SpriteChange2RelayPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::int64_t value;
};
#pragma pack(pop)

// OFFSET: 0x00D67850
// FUNCTION: fn_22034
void CGameMode::ReceiveSpriteChange2Mirrored(const unsigned char* packet) {
  this->ReceiveSpriteChange2(packet);
  if (*reinterpret_cast<const std::uint32_t*>(packet + 2) == g_session.m_aid) {
    for (std::uint32_t aid : this->spriteMirrorActorIds) {
      SpriteChange2RelayPacket relay;
      relay.id = ID_ZC_SPRITE_CHANGE2;
      relay.aid = aid;
      relay.type = packet[6];
      relay.value = *reinterpret_cast<const std::int64_t*>(packet + 7);
      this->ReceiveSpriteChange2(reinterpret_cast<const unsigned char*>(&relay));
    }
  }
}

#pragma pack(push, 1)
struct Item1718PendingRecord {
  /* 0000 */ std::uint32_t price;
  /* 0004 */ std::uint32_t realPrice;
  /* 0008 */ unsigned char type;
  /* 0009 */ std::int32_t itemId;
  /* 000D */ std::uint16_t sprite;
  /* 000F */ std::uint32_t location;
  /* 0013 */ unsigned char padding_0013[6];
};
#pragma pack(pop)

static_assert(sizeof(Item1718PendingRecord) == 0x19);

struct Unknown015D4818Values {
  /* 0000 */ std::uint32_t padding_0000[2];
  /* 0008 */ std::uint32_t location;
  /* 000C */ std::uint32_t padding_000C[25];
  /* 0070 */ std::uint32_t sprite;
};

struct Unknown015D4818Entry {
  /* 0000 */ std::uint32_t padding_0000[2];
  /* 0008 */ Unknown015D4818Values values;
};

struct Unknown015D4818 {
  /* 0000 */ std::uint32_t padding_0000;
  /* 0004 */ Unknown015D4818Entry* entry;
};

// GLOBAL: gl_25368
Unknown015D4818* g_pendingCashItemList;

// OFFSET: 0x00D3BD40
// FUNCTION: fn_21790
void CGameMode::ReceivePendingItem1718List(const unsigned char* packet) {
  if (g_windowMgr.nCashWnd || g_windowMgr.LookupWindow(WID_CASHSHOP_WND)) {
    return;
  }

  g_session.ClearTradeItems();
  if (g_session.m_channelCopId == 1) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("한게임 유저는 12월 3일부로 유료아이템 구매가 불가능합니다."), 0xFF, 0, 0, 0);
    return;
  }

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 12) / sizeof(Item1718PendingRecord);
  const Item1718PendingRecord* record = reinterpret_cast<const Item1718PendingRecord*>(packet + 12);
  for (int i = 0; i < count; ++i, ++record) {
    RawItemBasic item = {};
    _itoa(record->itemId, item.itemId, 10);
    item.price = record->price;
    item.realPrice = record->realPrice;
    item.type = record->type;
    item.id = -1;
    item.identified = 1;
    g_session.AddShopItem(item, 0);
    if (g_pendingCashItemList && g_pendingCashItemList->entry) {
      Unknown015D4818Values& values = g_pendingCashItemList->entry->values;
      values.sprite = record->sprite;
      values.location = record->location;
    }
  }

  g_pendingAid = *reinterpret_cast<const int*>(packet + 4);
  g_pendingGid = *reinterpret_cast<const int*>(packet + 8);
  this->isOnQuest = 1;

  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND);
  window->SendMsg(nullptr, UM_SETTITLE, 0, 1, 0, 0);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  window = g_windowMgr.EnsureWindow(WID_ITEMPURCHASEWND);
  window->SendMsg(nullptr, UM_SETTITLE, 1, 0, 0, 0);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00D66860
// FUNCTION: fn_22016
void CGameMode::ReceiveClanChat(const unsigned char* packet) {
  const int messageLength = *reinterpret_cast<const std::int16_t*>(packet + 2) - 28;

  if (static_cast<unsigned int>(messageLength) > 220) {
    return;
  }

  char message[220];

  std::memset(message, 0, sizeof(message));
  strcpy_s(message, sizeof(message), reinterpret_cast<const char*>(packet + 28));

  if (g_forbiddenWords.Contains(message) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(message, &masked);
  }

  if (!g_serviceOptions.Get(148)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xB4B4FF, 21, 0, 0);
  }
}

#pragma pack(push, 1)
struct SearchStorePacketHeader {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ unsigned char reset;
  /* 0005 */ unsigned char hasNext;
  /* 0006 */ unsigned char count;
};

struct SearchStoreOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};

struct SearchStoreItemPacket {
  /* 0000 */ unsigned char storeIdentity[8];
  /* 0008 */ char storeName[80];
  /* 0058 */ int itemId;
  /* 005C */ unsigned char kind;
  /* 005D */ int price;
  /* 0061 */ std::uint16_t quantity;
  /* 0063 */ std::uint32_t cards[4];
  /* 0073 */ SearchStoreOption options[5];
  /* 008C */ unsigned char refine;
  /* 008D */ unsigned char grade;
};

struct SearchStoreItemPacketOld {
  SearchStoreItemPacketOld() {
    SecureZeroMemory(this->storeName, sizeof(this->storeName));
    SecureZeroMemory(this->cards, sizeof(this->cards));
    SecureZeroMemory(this->options, sizeof(this->options));
  }

  /* 0000 */ unsigned char storeIdentity[8];
  /* 0008 */ char storeName[80];
  /* 0058 */ int itemId;
  /* 005C */ unsigned char kind;
  /* 005D */ int price;
  /* 0061 */ std::uint16_t quantity;
  /* 0063 */ unsigned char refine;
  /* 0064 */ unsigned char cards[16];
  /* 0074 */ unsigned char options[25];
};
#pragma pack(pop)

static_assert(sizeof(SearchStoreItemPacket) == 0x8E);
static_assert(sizeof(SearchStoreItemPacketOld) == 0x8D);

// OFFSET: 0x00D3FA20
// FUNCTION: fn_21814
void CGameMode::ReceiveSearchStoreItems(const unsigned char* packet) {
  const SearchStorePacketHeader* header = reinterpret_cast<const SearchStorePacketHeader*>(packet);
  const SearchStoreItemPacket* items = reinterpret_cast<const SearchStoreItemPacket*>(packet + 7);
  UIWindow* window = g_windowMgr.LookupWindow(WID_SEARCH_STORE_INFO);
  if (header->reset) {
    if (window) {
      window->SendMsg(nullptr, UM_REFRESH, 1, 387, 0, 0);
    }
    g_session.storeSearchResults.clear();
  }
  reinterpret_cast<unsigned char*>(&g_session.storeSearchPageFlags)[1] = header->hasNext;
  g_session.storeSearchCount = header->count;
  const unsigned int count = (header->length - 7) / sizeof(SearchStoreItemPacket);
  for (long long i = 0; i < count; ++i) {
    const SearchStoreItemPacket record = items[i];
    SessionEntry5BD4 entry;
    std::memcpy(entry.storeIdentity, record.storeIdentity, sizeof(entry.storeIdentity));
    std::memcpy(entry.storeName, record.storeName, sizeof(entry.storeName));
    entry.price = record.price;
    entry.quantity = record.quantity;
    entry.item.SetItemId(record.itemId);
    entry.item.refiningLevel = record.refine;
    entry.item.identity[0] = record.kind;
    entry.item.identified = 1;
    int optionCount = 0;
    for (int j = 0; j < 5; ++j) {
      if (record.options[j].id != 0) {
        ++optionCount;
      }
    }
    entry.item.optionCount = optionCount;
    for (long long j = 0; j < optionCount; ++j) {
      reinterpret_cast<SearchStoreOption*>(entry.item.options)[j] = record.options[j];
    }
    for (long long j = 0; j < 4; ++j) {
      reinterpret_cast<std::uint32_t*>(entry.item.cards)[j] = record.cards[j];
    }
    entry.item.grade = record.grade;
    g_session.storeSearchResults.push_back(entry);
  }
  if (window) {
    window->SendMsg(nullptr, UM_REFRESH, 1, 184, 0, 0);
  }
}

// OFFSET: 0x00D3FD70
// FUNCTION: fn_21815
void CGameMode::ReceiveSearchStoreItemsOld(const unsigned char* packet) {
  const SearchStorePacketHeader* header = reinterpret_cast<const SearchStorePacketHeader*>(packet);
  const SearchStoreItemPacketOld* next = reinterpret_cast<const SearchStoreItemPacketOld*>(packet + 7);
  UIWindow* window = g_windowMgr.LookupWindow(WID_SEARCH_STORE_INFO);
  if (header->reset) {
    if (window) {
      window->SendMsg(nullptr, UM_REFRESH, 1, 387, 0, 0);
    }
    g_session.storeSearchResults.clear();
  }
  const int count = (header->length - 7) / sizeof(SearchStoreItemPacketOld);
  reinterpret_cast<unsigned char*>(&g_session.storeSearchPageFlags)[1] = header->hasNext;
  g_session.storeSearchCount = header->count;
  for (int i = 0; i < count; ++i, ++next) {
    SearchStoreItemPacketOld record;
    std::memcpy(&record, next, sizeof(record));
    SessionEntry5BD4 entry;
    std::memcpy(entry.storeIdentity, record.storeIdentity, sizeof(entry.storeIdentity));
    std::memcpy(entry.storeName, record.storeName, sizeof(entry.storeName));
    entry.price = record.price;
    entry.quantity = record.quantity;
    entry.item.SetItemId(record.itemId);
    entry.item.refiningLevel = record.refine;
    entry.item.identity[0] = record.kind;
    entry.item.identified = 1;
    const SearchStoreOption* options = reinterpret_cast<const SearchStoreOption*>(record.options);
    int optionCount = 0;
    for (int j = 0; j < 5; ++j) {
      if (0 != options[j].id) {
        ++optionCount;
      }
    }
    entry.item.optionCount = optionCount;
    for (int j = 0; j < entry.item.optionCount; ++j) {
      reinterpret_cast<SearchStoreOption*>(entry.item.options)[j] = options[j];
    }
    std::memcpy(entry.item.cards, record.cards, sizeof(record.cards));
    g_session.storeSearchResults.push_back(entry);
  }
  if (window) {
    window->SendMsg(nullptr, UM_REFRESH, 1, 184, 0, 0);
  }
}
