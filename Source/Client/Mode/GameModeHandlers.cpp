#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/ClanInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/GuildMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/MessageTable.h"
#include "Contents/RodexMailStore.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "File/FileCreation.h"
#include "Script/LuaScript.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Rodex/UIRodexInboxWnd.h"
#include "UI/UIBarGraph.h"
#include "UI/UIGradeEnchantWnd.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "UI/Window/UISeekPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <timeapi.h>

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
struct PackageItemTestEntry {
  /* 0000 */ int itemId;
  /* 0004 */ std::uint16_t unknown_0004;
  /* 0006 */ int count;
};

struct Item1718BasicRecord {
  /* 0000 */ std::int32_t itemId;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ unsigned char type;
  /* 000D */ std::uint16_t unknown_000D;
  /* 000F */ std::uint32_t unknown_000F;
};

struct Item1718IndexRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ std::uint32_t unknown_0002;
  /* 0006 */ std::uint32_t unknown_0006;
};

struct HomunculusInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char unknown_001A;
  /* 001B */ std::int16_t unknown_001B;
  /* 001D */ std::int16_t unknown_001D;
  /* 001F */ std::int16_t unknown_001F;
  /* 0021 */ int unknown_0021;
  /* 0025 */ std::int16_t stats[8];
  /* 0035 */ std::int16_t hp;
  /* 0037 */ std::int16_t maxHp;
  /* 0039 */ std::int16_t sp;
  /* 003B */ std::int16_t maxSp;
  /* 003D */ int exp;
  /* 0041 */ int maxExp;
  /* 0045 */ std::int16_t unknown_0045;
  /* 0047 */ std::int16_t unknown_0047;
};

struct HomunculusInfoPacket2 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char unknown_001A;
  /* 001B */ std::int16_t unknown_001B;
  /* 001D */ std::int16_t unknown_001D;
  /* 001F */ std::int16_t unknown_001F;
  /* 0021 */ std::int16_t stats[8];
  /* 0031 */ int hp;
  /* 0035 */ int maxHp;
  /* 0039 */ std::int16_t sp;
  /* 003B */ std::int16_t maxSp;
  /* 003D */ int exp;
  /* 0041 */ int maxExp;
  /* 0045 */ std::int16_t unknown_0045;
  /* 0047 */ std::int16_t unknown_0047;
};

struct HomunculusInfoPacket3 {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char unknown_001A;
  /* 001B */ std::int16_t unknown_001B;
  /* 001D */ std::int16_t unknown_001D;
  /* 001F */ std::int16_t unknown_001F;
  /* 0021 */ std::int16_t stats[8];
  /* 0031 */ int hp;
  /* 0035 */ int maxHp;
  /* 0039 */ int sp;
  /* 003D */ int maxSp;
  /* 0041 */ std::int64_t exp;
  /* 0049 */ std::int64_t maxExp;
  /* 0051 */ std::int16_t unknown_0051;
  /* 0053 */ std::int16_t unknown_0053;
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

struct SessionItemLists {
  void ClearTradeItems();
};

struct ClientItemLists {
  void AddItem1718(const RawItemBasic& item, int mode);
  void RemoveItem1710(std::uint32_t id, int quantity);
};

int sub_D64A40(const unsigned char* entries);

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
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1828), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1829), 0, true, false, 0, "Message", 280, 120, 0);
    [[fallthrough]];
  case 0:
  case 1:
    g_windowMgr.CloseWindow(164);
    break;
  }
}

// OFFSET: 0x00D3B730
// FUNCTION: fn_21782
void CGameMode::ReceivePartyBookingUpdateResult(const unsigned char* packet) {
  const std::int16_t result = *reinterpret_cast<const std::int16_t*>(packet + 2);
  if (result != 0) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(result == 2 ? 1941 : 1826), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  }

  g_session.m_partyBooking.unknown_0034 = GetTickCount();
  g_windowMgr.EnsureWindow(164);
  UISeekPartyWnd* seekWindow = static_cast<UISeekPartyWnd*>(g_windowMgr.LookupWindow(164));
  if (!seekWindow) {
    return;
  }

  UIWindow* jobWindow = g_windowMgr.LookupWindow(168);
  if (!jobWindow) {
    return;
  }

  seekWindow->SetJobs(reinterpret_cast<const std::int16_t*>(reinterpret_cast<const unsigned char*>(jobWindow) + 0x16D));
  g_windowMgr.CloseWindow(168);
}

// OFFSET: 0x00D3B7F0
// FUNCTION: fn_21783
void CGameMode::ReceivePartyBookingSearchResult(const unsigned char* packet) {
  const int size = *reinterpret_cast<const std::int16_t*>(packet + 2) - 5;
  const int count = size / sizeof(PARTY_BOOKING_AD_INFO);
  PARTY_BOOKING_AD_INFO* infos = new PARTY_BOOKING_AD_INFO[count];
  std::memcpy(infos, packet + 5, size);
  g_session.m_partyBooking.m_filterEnabled = true;
  UIWindow* window = g_windowMgr.LookupWindow(169);
  if (!window) {
    return;
  }

  if (size == 0) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1827), 0, true, false, 0, "Message", 280, 120, 0);
    g_session.m_partyBooking.m_lastPage = false;
    window->Invalidate();
    return;
  }

  g_session.m_partyBooking.AddEntries(infos, count, packet[4]);
  window->Invalidate();
}

// OFFSET: 0x00D3B8D0
// FUNCTION: fn_21784
void CGameMode::ReceivePartyBookingDelete(const unsigned char* packet) {
  g_session.m_partyBooking.Remove(*reinterpret_cast<const int*>(packet + 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(169)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B910
// FUNCTION: fn_21785
void CGameMode::ReceivePartyBookingInsert(const unsigned char* packet) {
  g_session.m_partyBooking.OnAdvertisement(*reinterpret_cast<const PARTY_BOOKING_AD_INFO*>(packet + 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(169)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B960
// FUNCTION: fn_21786
void CGameMode::ReceivePartyBookingUpdate(const unsigned char* packet) {
  g_session.m_partyBooking.SetJobs(*reinterpret_cast<const int*>(packet + 2), reinterpret_cast<const short*>(packet + 6));
  if (UIWindow* window = g_windowMgr.LookupWindow(169)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B9A0
// FUNCTION: fn_21787
void CGameMode::ReceiveToggleNotice(const unsigned char* packet) {
  g_toggleNoticeState = packet[2];
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(g_toggleNoticeState == 0 ? 1326 : 1325)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D3BF00
// FUNCTION: fn_21791
void CGameMode::ReceivePendingResult(const unsigned char* packet) {
  std::string text;
  std::uint32_t color = 0xFF;
  switch (*reinterpret_cast<const std::int16_t*>(packet + 10)) {
  case 0:
  case 8:
    g_pendingAid = *reinterpret_cast<const int*>(packet + 2);
    g_pendingGid = *reinterpret_cast<const int*>(packet + 6);
    if (g_windowMgr.unknown_0250) {
      g_windowMgr.unknown_0250->Invalidate();
    }
    if (*reinterpret_cast<const std::int16_t*>(packet + 10) == 8) {
      text = LookupMessage(1814);
    } else {
      color = 0xFFFF00;
      text = LookupMessage(54);
    }
    break;
  case 1:
    text = LookupMessage(1227);
    break;
  case 2:
    text = LookupMessage(1228);
    break;
  case 3:
    text = LookupMessage(56);
    break;
  case 4:
    text = LookupMessage(1229);
    break;
  case 5:
    text = LookupMessage(1230);
    break;
  case 6:
    text = LookupMessage(1254);
    break;
  case 7:
    text = LookupMessage(1813);
    break;
  case 9:
    text = LookupMessage(1572);
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0, 0);
  }
}

// OFFSET: 0x00D3C1C0
// FUNCTION: fn_21792
void CGameMode::ReceiveItem1718BasicList(const unsigned char* packet) {
  reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();

  const std::int64_t count = (static_cast<std::int64_t>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 4) / 19;
  const Item1718BasicRecord* records = reinterpret_cast<const Item1718BasicRecord*>(packet + 4);

  for (std::int64_t index = 0; index < count; ++index) {
    Item1718BasicRecord record;
    memcpy_s(&record, sizeof(record), &records[index], sizeof(record));

    RawItemBasic item = {};
    _itoa(record.itemId, item.itemId, 10);
    item.unknown_0004 = record.unknown_0004;
    item.unknown_0008 = record.unknown_0008;
    item.unknown_000C = record.type;
    item.unknown_0034 = record.unknown_000F;
    item.unknown_0038 = record.unknown_000D;
    item.id = -1;
    item.identified = 1;
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1718(item, 0);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(22)) {
    window->SendMsg(nullptr, 0x24, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(23)) {
    window->SendMsg(nullptr, 0x24, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D3C4F0
// FUNCTION: fn_21793
void CGameMode::ReceiveItem1718Result(const unsigned char* packet) {
  switch (packet[2]) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(54)), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(55)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(56)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(221)), 0xFF, 0, 0, 0);
    break;
  case 9:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1940)), 0xFF, 0, 0, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2917)), 0xFFFF00, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3554)), 0xFF, 0, 0, 0);
    break;
  case 12:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3555)), 0xFFFF00, 0, 0, 0);
    break;
  case 13:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3557)), 0xFF, 0, 0, 0);
    break;
  case 14:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3556)), 0xFF, 0, 0, 0);
    break;
  }

  g_windowMgr.CloseWindow(22);
  g_windowMgr.CloseWindow(23);
}

// OFFSET: 0x00D3C710
// FUNCTION: fn_21794
void CGameMode::ReceiveItem1718IndexList(const unsigned char* packet) {
  reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(Item1718IndexRecord);
  const Item1718IndexRecord* records = reinterpret_cast<const Item1718IndexRecord*>(packet + 4);
  for (int i = 0; i < count; ++i) {
    const Item1718IndexRecord record = records[i];
    ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, record.index);
    if (owned.unknown_0000[1] != 0 && (owned.unknown_0074[0] == 0 || g_session.unknown_61A8[10] == 0)) {
      RawItemBasic item = {};
      item.id = record.index;
      strcpy_s(item.itemId, sizeof(item.itemId), owned.itemId.c_str());
      item.unknown_0004 = record.unknown_0002;
      item.unknown_0008 = record.unknown_0006;
      item.unknown_000C = owned.unknown_0000[0];
      item.identified = owned.identified;
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1718(item, 1);
    }
  }

  UIWindow* window = g_windowMgr.EnsureWindow(22);
  window->SendMsg(nullptr, 0x24, 1, 0, 0, 0);
  window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  g_windowMgr.EnsureWindow(24)->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
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
      actor->SendMsg(nullptr, 7, 0, static_cast<int>(text), static_cast<int>(text >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(speech), 0xFAFAFA, 0, 0, 0);
      }
    }
  }
}

// OFFSET: 0x00D3CDF0
// FUNCTION: fn_21797
void CGameMode::NotifyResult2571(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2571)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2572)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2573)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2574)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2575)), 0xFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2576)), 0xFF, 0, 0, 0);
    break;
  case 7:
    g_session.ResetUnknown1050();
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

  player->SendMsg(nullptr, 0x52, 0, *reinterpret_cast<const int*>(packet + 6) * 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  g_playerMessage82Pending = true;
}

// OFFSET: 0x00D3D390
// FUNCTION: fn_21802
void CGameMode::ReceiveHomunculusInfo(const unsigned char* data) {
  const HomunculusInfoPacket* packet = reinterpret_cast<const HomunculusInfoPacket*>(data);
  char* name = reinterpret_cast<char*>(&g_session.unknown_5570[0x0C]);
  int* stats = reinterpret_cast<int*>(&g_session.unknown_5570[0x2C]);
  if (g_serviceType == 10 || g_serviceType == 12 || g_serviceType == 15) {
    std::sprintf(name, "%s", packet->name);
  } else {
    std::sprintf(name, packet->name);
  }

  g_session.unknown_55D4 = packet->unknown_001F;
  g_session.unknown_55D8 = packet->unknown_0021;
  g_session.unknown_55F8 = packet->unknown_001A;
  for (int i = 0; i < 8; ++i) {
    stats[i] = packet->stats[i];
  }
  *reinterpret_cast<int*>(g_session.unknown_55C0) = packet->unknown_001B;
  g_session.m_homunHp = packet->hp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunSp = packet->sp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.unknown_55C8) = packet->maxHp;
  g_session.RefreshHomunculusInfo();
  g_session.unknown_55D0 = packet->maxSp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunExp = packet->exp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunMaxExp = packet->maxExp;
  g_session.RefreshHomunculusInfo();
  g_session.unknown_55F0 = packet->unknown_001D;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.unknown_1664) = packet->unknown_0045;
  g_session.InvalidateWindowFrames();
  *reinterpret_cast<int*>(&g_session.unknown_5620[4]) = packet->unknown_0047;
  if (g_windowMgr.window_03C4) {
    g_windowMgr.window_03C4->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }

  int& notified = *reinterpret_cast<int*>(&g_session.unknown_5620[0]);
  const std::int16_t state = packet->unknown_001D;
  if ((state == 1 || state == 5 || state == 10) && notified != state) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1018)), 0xFF, 0, 0, 0);
    notified = packet->unknown_001D;
  }
}

// OFFSET: 0x00D3D710
// FUNCTION: fn_21803
void CGameMode::ReceiveHomunculusInfo3(const unsigned char* data) {
  if (!data) {
    return;
  }

  const HomunculusInfoPacket3* packet = reinterpret_cast<const HomunculusInfoPacket3*>(data);
  int* stats = reinterpret_cast<int*>(&g_session.unknown_5570[0x2C]);
  StringFormat(reinterpret_cast<char*>(&g_session.unknown_5570[0x0C]), 32, static_cast<std::size_t>(-1), "%s", packet->name);
  g_session.unknown_55F8 = packet->unknown_001A;
  *reinterpret_cast<int*>(g_session.unknown_55C0) = packet->unknown_001B;
  g_session.unknown_55F0 = packet->unknown_001D;
  g_session.RefreshHomunculusInfo();

  int& notified = *reinterpret_cast<int*>(&g_session.unknown_5620[0]);
  const int state = packet->unknown_001D;
  if ((state == 1 || state == 5 || state == 10) && notified != state) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1018)), 0xFF, 0, 0, 0);
    notified = packet->unknown_001D;
  }

  g_session.unknown_55D4 = packet->unknown_001F;
  for (int i = 0; i < 8; ++i) {
    stats[i] = packet->stats[i];
  }
  g_session.m_homunHp = packet->hp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.unknown_55C8) = packet->maxHp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunSp = packet->sp;
  g_session.RefreshHomunculusInfo();
  g_session.unknown_55D0 = packet->maxSp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunExp = packet->exp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunMaxExp = packet->maxExp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.unknown_1664) = packet->unknown_0051;
  g_session.InvalidateWindowFrames();
  if (g_windowMgr.window_03C4) {
    g_windowMgr.window_03C4->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }

  *reinterpret_cast<int*>(&g_session.unknown_5620[4]) = packet->unknown_0053;
  if (UIHomunInfoWnd* window = static_cast<UIHomunInfoWnd*>(g_windowMgr.window_03C0)) {
    window->expBar->FillBackground(0xFFFFFF);
    window->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
    window->intimacyBar->SetValues(&g_session.unknown_55F0, &g_session.unknown_55F4);
  }
}

// OFFSET: 0x00D3D910
// FUNCTION: fn_21804
void CGameMode::ReceiveHomunculusInfo2(const unsigned char* data) {
  const HomunculusInfoPacket2* packet = reinterpret_cast<const HomunculusInfoPacket2*>(data);
  char* name = reinterpret_cast<char*>(&g_session.unknown_5570[0x0C]);
  int* stats = reinterpret_cast<int*>(&g_session.unknown_5570[0x2C]);
  if (g_serviceType == 10 || g_serviceType == 12 || g_serviceType == 15) {
    std::sprintf(name, "%s", packet->name);
  } else {
    std::sprintf(name, packet->name);
  }

  g_session.unknown_55D4 = packet->unknown_001F;
  g_session.unknown_55F8 = packet->unknown_001A;
  for (int i = 0; i < 8; ++i) {
    stats[i] = packet->stats[i];
  }
  *reinterpret_cast<int*>(g_session.unknown_55C0) = packet->unknown_001B;
  g_session.m_homunHp = packet->hp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunSp = packet->sp;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.unknown_55C8) = packet->maxHp;
  g_session.RefreshHomunculusInfo();
  g_session.unknown_55D0 = packet->maxSp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunExp = packet->exp;
  g_session.RefreshHomunculusInfo();
  g_session.m_homunMaxExp = packet->maxExp;
  g_session.RefreshHomunculusInfo();
  g_session.unknown_55F0 = packet->unknown_001D;
  g_session.RefreshHomunculusInfo();
  *reinterpret_cast<int*>(g_session.unknown_1664) = packet->unknown_0045;
  g_session.InvalidateWindowFrames();
  *reinterpret_cast<int*>(&g_session.unknown_5620[4]) = packet->unknown_0047;
  if (g_windowMgr.window_03C4) {
    g_windowMgr.window_03C4->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }

  int& notified = *reinterpret_cast<int*>(&g_session.unknown_5620[0]);
  const std::int16_t state = packet->unknown_001D;
  if ((state == 1 || state == 5 || state == 10) && notified != state) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1018)), 0xFF, 0, 0, 0);
    notified = packet->unknown_001D;
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

  this->minimapMarkers[packet->key].unknown_0000 = x;
  this->minimapMarkers[packet->key].unknown_0002 = y;
  this->minimapMarkers[packet->key].unknown_0004 = packet->type;
  this->minimapMarkers[packet->key].unknown_0006 = packet->color;
  if (g_windowMgr.window_4F1CC) {
    for (auto it = this->minimapMarkers.begin(); it != this->minimapMarkers.end(); ++it) {
      if (it->second.unknown_0006 != 0) {
        g_windowMgr.window_4F1CC->SendMsg(nullptr, 485, it->second.unknown_0004, it->second.unknown_0006, it->second.unknown_0000, it->second.unknown_0002);
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
  if (file.Open(path, 0, true, false)) {
    g_windowMgr.EnsureWindow(106)->SendMsg(nullptr, 0x5E, reinterpret_cast<std::intptr_t>(number), 287, *reinterpret_cast<const int*>(packet + 6), 0);
    file.Close();
  } else {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("책이 없습니다"), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D3DF40
// FUNCTION: fn_21807
void CGameMode::ReceiveResultNotice00FC(const unsigned char* packet) {
  this->unknown_00FC = 0;

  int messageId;
  switch (packet[2]) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x43;
    break;
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 7;
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x44;
    break;
  case 4:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x37;
    break;
  case 5:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x1B0;
    break;
  case 6:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x1B1;
    break;
  case 7:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x1B2;
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(messageId)), 0xFF, 0, 0, 0);
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
      std::sprintf(text, LookupMessage(1124), *reinterpret_cast<const int*>(packet + 6) / 86400 + 1);
      break;
    case 2: {
      const int seconds = *reinterpret_cast<const int*>(packet + 10);
      std::sprintf(text, LookupMessage(1125), seconds / 3600, seconds % 3600 / 60, seconds % 60);
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

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFFFF, 0, 0, 0);
}

struct ActorMessage32 {
  /* 0000 */ std::string text;
  /* 0018 */ std::string extra;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int unknown_0038;
  /* 003C */ int unknown_003C;
};

// OFFSET: 0x00D3E8B0
// FUNCTION: fn_21810
void CGameMode::ReceiveServerRateNotice(const unsigned char* packet) {
  g_session.unknown_54A0 = *reinterpret_cast<const int*>(packet + 2);
  g_session.unknown_54A4 = packet[6];
  g_session.unknown_54A8 = packet[7];
  if (g_windowMgr.unknown_030C) {
    reinterpret_cast<UIWindow*>(g_windowMgr.unknown_030C)->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }

  char text[128];
  std::memset(text, 0, sizeof(text));
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  switch (g_session.unknown_54A0) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(292), LookupMessage(287));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(292), LookupMessage(288));
    break;
  case 2:
    std::sprintf(text, "%s - %s : %s(%s)", LookupMessage(291), LookupMessage(292), LookupMessage(287), LookupMessage(1535));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }

  switch (g_session.unknown_54A4) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(293), LookupMessage(287));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(293), LookupMessage(290));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }

  switch (g_session.unknown_54A8) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(738), LookupMessage(287));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(738), LookupMessage(288));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }
}

// OFFSET: 0x00D3F130
// FUNCTION: fn_21811
void CGameMode::ReceiveActorMessage32(const unsigned char* packet) {
  this->unknown_01AC[*reinterpret_cast<const std::uint32_t*>(packet + 8)] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4));
  if (!actor) {
    return;
  }

  ActorMessage32 message;
  message.text.assign(reinterpret_cast<const char*>(packet + 17), *reinterpret_cast<const std::int16_t*>(packet + 2) - 17);
  message.unknown_0030 = packet[16];
  message.unknown_0034 = *reinterpret_cast<const std::int16_t*>(packet + 14);
  message.unknown_0038 = *reinterpret_cast<const std::int16_t*>(packet + 12);
  message.unknown_003C = *reinterpret_cast<const int*>(packet + 8);
  if (message.text.size() > 60) {
    message.text.resize(60);
  }

  const std::int64_t argument = reinterpret_cast<std::intptr_t>(&message);
  actor->SendMsg(nullptr, 32, 0, static_cast<int>(argument), static_cast<int>(argument >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D3F3E0
// FUNCTION: fn_21812
void CGameMode::ReceiveRequestResult(const unsigned char* packet) {
  std::string text;
  switch (*reinterpret_cast<const std::int16_t*>(packet + 6)) {
  case 0:
  case 8: {
    UIWindow* window = g_windowMgr.LookupWindow(318);
    if (window) {
      window->SendMsg(nullptr, 122, *reinterpret_cast<const int*>(packet + 8), *reinterpret_cast<const int*>(packet + 12), 0, 0);
    }
    if (*reinterpret_cast<const std::int16_t*>(packet + 6) == 8) {
      text = LookupMessage(1814);
    }
    break;
  }
  case 9:
    text = LookupMessage(1944);
    break;
  case 10:
    text = LookupMessage(1945);
    break;
  case 11:
    text = LookupMessage(1946);
    break;
  case 12:
    text = LookupMessage(1947);
    break;
  case 303:
    text = "- " + IntToString(303, 0) + " : " + LookupMessage(3940);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, false, false, 0, "Error", 280, 120, 0);
    break;
  case 304:
    text = "- " + IntToString(304, 0) + " : " + LookupMessage(3941);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, false, false, 0, "Error", 280, 120, 0);
    break;
  default:
    text = LookupMessage(57);
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D3F760
// FUNCTION: fn_21813
void CGameMode::ReceiveStatusNotice(const unsigned char* packet) {
  int message;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 21:
    message = 889;
    break;
  case 22:
    message = 890;
    break;
  case 23:
    message = 891;
    break;
  case 24:
    message = 892;
    break;
  case 25:
    message = 893;
    break;
  case 28:
    message = 894;
    break;
  case 29:
    message = 895;
    break;
  case 30:
    message = 896;
    break;
  case 31:
    message = 897;
    break;
  case 32:
    message = 898;
    break;
  case 40:
    message = 915;
    break;
  default:
    return;
  }
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0x9B9BFF, 8, 0, 0);
}

// OFFSET: 0x00D40080
// FUNCTION: fn_21816
void CGameMode::ReceiveWindow181Message(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.LookupWindow(181);
  if (window) {
    window->SendMsg(nullptr, 60, 1, packet[2], 0, 0);
  }
}

// OFFSET: 0x00D40120
// FUNCTION: fn_21817
void CGameMode::ReceiveNamedResultNotice(const unsigned char* packet) {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  std::string name(reinterpret_cast<const char*>(packet + 2));
  switch (packet[26]) {
  case 0:
    switch (packet[27]) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3389)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3390)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3391)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  case 1:
    switch (packet[27]) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3392)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3393)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3394)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D40360
// FUNCTION: fn_21818
void CGameMode::ReceiveResultNotice0118(const unsigned char* packet) {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  switch (packet[2]) {
  case 0:
    switch (packet[3]) {
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(194)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(195)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  case 1:
    switch (packet[3]) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(196)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(197)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(198)).c_str()), 0xFFFF, 0, 0, 0);
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
  ItemDescriptions{g_itemDescriptions}.Name(item, packageName, capacity, 0);

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
    ItemDescriptions{g_itemDescriptions}.Name(item, itemName, nameCapacity, 0);
    std::fprintf(file, "%s\t\t\t%d\t%c%d%c\n", itemName, current.count, '(', current.unknown_0004, ')');
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
  g_session.unknown_0784[packet->summonId] = packet->hp;
  g_session.unknown_078C[packet->summonId] = packet->maxHp;

  CRenderObject* actor = this->scene->GetGameActorByAID(packet->summonId);
  if (actor != nullptr) {
    const std::int64_t id = packet->summonId;
    actor->SendMsg(nullptr, 34, 0, static_cast<int>(id), static_cast<int>(id >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D65680
// FUNCTION: fn_22000
void CGameMode::ReceiveSummonHpUpdate(const unsigned char* buffer) {
  const SummonHpUpdatePacket* packet = reinterpret_cast<const SummonHpUpdatePacket*>(buffer);
  if (g_session.FindUnknown078C(packet->summonId) == -1) {
    return;
  }

  switch (packet->varId) {
  case 5:
    g_session.unknown_0784[packet->summonId] = packet->value;
    break;
  case 6:
    g_session.unknown_078C[packet->summonId] = packet->value;
    break;
  }

  CRenderObject* actor = this->scene->GetGameActorByAID(packet->summonId);
  if (actor != nullptr) {
    const std::int64_t id = packet->summonId;
    actor->SendMsg(nullptr, 34, 0, static_cast<int>(id), static_cast<int>(id >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

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

// OFFSET: 0x00D657D0
// FUNCTION: fn_22002
void CGameMode::ReceiveActorNameCard(const unsigned char* packet) {
  char name[25] = {};
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
  if (!g_luaScript->Call("GetSkillScale", "dd>dd", skillId, level, &width, &height)) {
    return;
  }

  CWarningPlaneEffect* effect = static_cast<CWarningPlaneEffect*>(g_effectMgr->Spawn(actor->render.accountId, 1114, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
  if (!effect) {
    return;
  }

  for (int column = 0; column < width; ++column) {
    for (int row = 0; row < height; ++row) {
      vector3d position = {0.0f, 0.0f, 0.0f};
      reinterpret_cast<const ::TerrainHeight*>(this)->CellCenter(x - width / 2 + column, y - height / 2 + row, position.x, position.z);
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      position.y = mode->scene->terrain->Grid().Height(position.x, position.z);
      effect->AddPlane(position);
      effect->expireTime = timeGetTime() + duration;
    }
  }
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
    effect->Slot24(words[0], words[1], words[2], 1, 361);
    if (UIWindow* window = g_windowMgr.LookupWindow(361)) {
      window->SendMsg(window, 0x3C, 0, 0, 0, 0);
    }
    if (UIWindow* window = g_windowMgr.EnsureWindow(362)) {
      window->SendMsg(nullptr, 0x0D, reinterpret_cast<std::intptr_t>(packet), 0, 0, 0);
      window->Invalidate();
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4075)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4076)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4076), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4077)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4077), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4078)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4078), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4079)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4079), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4080)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4080), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4081)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4081), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 8:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4082)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4082), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }

  g_runeSystemMgr->SetRequestPending(false);
  if (UIWindow* window = g_windowMgr.LookupWindow(361)) {
    window->SendMsg(window, 0x3C, 0, 0, 0, 0);
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
// OFFSET: 0x00D66B10
// FUNCTION: fn_22021
void CGameMode::ReceiveRodexMailDeleteResult(int mailId, int result) {
  int message;
  if (result == 0) {
    if (g_windowMgr.unknown_045C) {
      reinterpret_cast<UIWindow*>(g_windowMgr.unknown_045C)->SendMsg(nullptr, 6, 201, 0, 0, 0);
    }

    std::map<std::int64_t, RodexMail>::iterator mail = g_rodexMailStore->m_mails0.find(mailId);
    if (mail != g_rodexMailStore->m_mails0.end()) {
      if (!mail->second.isRead) {
        --g_rodexMailStore->unknown_0018;
        if (g_rodexMailStore->unknown_0018 == 0) {
          if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
            window->SendMsg(nullptr, 6, 286, 476, 0, 0);
          }
        }
      }

      g_rodexMailStore->m_mails0.erase(mail);
      if (g_windowMgr.unknown_0454) {
        reinterpret_cast<UIRodexInboxWnd*>(g_windowMgr.unknown_0454)->RemoveInboxMail(mailId);
      }
    }
    message = 3865;
  } else {
    message = 3866;
  }

  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(message), 0, true, false, 0, "Message", 280, 120, false);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0x64FFFF, 0, 0, 0);
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
      effect->Slot24(words[0], words[1], words[2], 1, 361);
    } else if (state == 1) {
      CEffect* effect = g_effectMgr->Spawn(g_session.m_aid, 2398, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      const vector3d offset = {784.0f, 161.0f, 0.0f};
      const int* words = reinterpret_cast<const int*>(&offset);
      effect->Slot24(words[0], words[1], words[2], 1, 361);
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
      effect->Slot24(words[0], words[1], words[2], 1, 361);
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
  if (UIWindow* window = g_windowMgr.LookupWindow(361)) {
    window->SendMsg(window, 0x3C, type, 0, 0, 0);
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
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF4)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF4), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF5)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF5), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF6)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF6), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF7)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF7), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF8)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF8), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF9)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF9), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFA)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFA), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 8:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFB)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFB), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFD)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFC), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFD)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFD), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 12:
  case 13:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFE)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFE), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  default:
    return result;
  }

  return result;
}

// OFFSET: 0x00D67540
// FUNCTION: fn_22028
void CGameMode::ApplyRuneSetResult(int result, int tagId, int runeSetId) {
  if (this->HandleRuneResult(result) == 0) {
    g_runeSystemMgr->SetCurRuneSet(tagId, runeSetId, -1, -1);
  }

  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (window) {
    window->SendMsg(window, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D67590
// FUNCTION: fn_22029
void CGameMode::ApplyRuneSetResultWithRequest(int result, int tagId, int runeSetId, int grade, int slotCount) {
  if (this->HandleRuneResult(result) == 0) {
    g_runeSystemMgr->SetCurRuneSet(tagId, runeSetId, grade, slotCount);
  }

  g_runeSystemMgr->SendRequest(0x0BCB, nullptr, tagId);

  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (window) {
    window->SendMsg(window, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D677C0
// FUNCTION: fn_22033
void CGameMode::ReceiveGameOptionResult(int command, int result, int enabled) {
  if (result != 1) {
    return;
  }

  if (command == 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xF15)), 0x64FFFF, 0, 0, 0);
  }

  g_gameOptionFlag = enabled != 0;
  g_windowMgr.sub_AAB090();
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(g_gameOptionFlag ? 0xF17 : 0xF16)), 0x64FFFF, 0, 0, 0);
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
  case 5:
    if (UITipboxWnd* window = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(315))) {
      window->ShowTip(value);
    }
    break;
  case 6:
    g_renewQuestMgr->sub_67B650(value);
    break;
  case 7:
    if (UIWindow* window = g_windowMgr.EnsureWindow(328)) {
      window->SendMsg(nullptr, 0x22, value, 0, 0, 0);
    }
    break;
  case 8:
    g_windowMgr.EnsureWindow(343);
    break;
  }
}

// OFFSET: 0x00D679E0
// FUNCTION: fn_22037
void CGameMode::ReceiveGradeEnchantResult(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t*>(packet + 2));
  if (item.unknown_0000[1]) {
    const int result = *reinterpret_cast<const int*>(packet + 6);
    if (result == 0 || result == 2) {
      if (result == 0) {
        item.unknown_0060 = 0;
      }
      item.unknown_0088 = *reinterpret_cast<const std::int16_t*>(packet + 4);
      g_session.UpdateInventoryItem(item, 1);
    }
  }

  auto* window = static_cast<UIGradeEnchantWnd*>(g_windowMgr.LookupWindow(343));
  if (window) {
    const int result = *reinterpret_cast<const int*>(packet + 6);
    switch (result) {
    case 0:
      window->SetResult(2, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1607, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1608, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)->Slot16(2800);
      break;
    case 1:
    case 2:
    case 3:
    case 4:
      window->SetResult(2, result);
      g_effectMgr->Spawn(g_session.m_aid, 1607, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1609, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)->Slot16(2800);
      break;
    }
  }
}

// OFFSET: 0x00D681A0
// FUNCTION: fn_22039
void CGameMode::ReceiveAchievementResult(unsigned char result, int id) {
  if (result != 0) {
    return;
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(270)) {
    window->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }

  const int index = g_session.AddUnknown6160(id);
  if (index != -1) {
    g_session.AddUnknownId(index);
  }
}

// OFFSET: 0x00D68200
// FUNCTION: fn_22040
void CGameMode::ReceiveItemReward2(const unsigned char* data) {
  const ItemRewardPacket2* packet = reinterpret_cast<const ItemRewardPacket2*>(data);
  ITEM_INFO item;
  if (packet->result == 0) {
    item.unknown_0000[1] = packet->index;
    item.quantity = packet->quantity;
    item.SetItemId(packet->itemId);
    item.unknown_0000[0] = packet->type;
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));

    int count = 0;
    for (int i = 0; i < 5; ++i) {
      if (packet->options[i].id != 0) {
        ++count;
      }
    }
    item.unknown_0000[2] = packet->expireTime;
    item.unknown_0098 = count;
    for (std::int64_t index = 0; index < count; ++index) {
      reinterpret_cast<ItemRewardOption*>(item.unknown_009C)[index] = packet->options[index];
    }
    item.unknown_0074[0] = packet->bound;
    item.unknown_0088 = packet->grade;
  }

  const std::uint16_t value = packet->value;
  ITEM_INFO reward = item;
  switch (packet->result) {
  case 0:
    g_session.AddStackedItem(reward);
    reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1710(reward.unknown_0000[1], reward.quantity);
    g_session.unknown_6140 = value;
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2594)), 0x64FFFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2630)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2629)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2697)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2699)), 0x64FFFF, 0, 0, 0);
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
    item.unknown_0000[1] = packet->index;
    item.quantity = packet->quantity;
    item.SetItemId(packet->itemId);
    item.unknown_0000[0] = packet->type;
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.unknown_0098 = sub_D64A40(reinterpret_cast<const unsigned char*>(packet->options));
    item.unknown_0000[2] = packet->expireTime;
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<ItemRewardOption*>(item.unknown_009C)[index] = packet->options[index];
    }
    item.unknown_0074[0] = packet->bound;
    g_session.AddStackedItem(item);
    reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1710(packet->index, packet->quantity);
    g_session.unknown_6140 = packet->value;
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2594)), 0x64FFFF, 0, 0, 0);
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2630)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2629)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2697)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2699)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D686F0
// FUNCTION: fn_22042
void CGameMode::ReceiveGuildEmblemVersion(std::uint32_t aid, int guildId, int emblemVersion) {
  if (this->scene) {
    CRenderObject* actor = this->scene->GetGameActorByAID(aid);
    if (actor) {
      static_cast<CGameActor*>(actor)->Slot48(guildId);
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
  g_windowMgr.EnsureWindow(0x103);
}

// OFFSET: 0x00D66A50
// FUNCTION: fn_22019
void CGameMode::IgnoreRemovedPacket12(const unsigned char*, int, int, int, int) {
}

// OFFSET: 0x00D67670
// FUNCTION: fn_22031
void CGameMode::ToggleWindow169(unsigned char enable) {
  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (enable == 0) {
    if (window) {
      g_windowMgr.CloseWindow(0x169);
    }
  } else if (!window) {
    g_windowMgr.EnsureWindow(0x169);
  }
}

#pragma pack(push, 1)
struct RequestWindow169Packet {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ unsigned char unknown_0002;
};
#pragma pack(pop)

// OFFSET: 0x00D67600
// FUNCTION: fn_22030
void CGameMode::RequestWindow169(unsigned char enable) {
  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (enable == 0) {
    if (window) {
      g_windowMgr.CloseWindow(0x169);
    }
    return;
  }

  if (window) {
    return;
  }

  RequestWindow169Packet packet;
  packet.id = 0x0BE0;
  packet.unknown_0002 = 0;
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00D66A60
// FUNCTION: fn_22020
void CGameMode::ShowNoticeText(std::string text) {
  const std::string message = FormatOwnedString(LookupMessage(0xF18), text.c_str());
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0x64FFFF, 0, 0, 0);
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
  ItemDescriptions{g_itemDescriptions}.Name(box, name, capacity, 0);

  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string folder = "DdukddakBoxTest";
  std::string fileName = FormatOwnedString("%d%02d%02d_(%s).txt", time.wYear, time.wMonth, time.wDay, name);
  std::string path = FormatOwnedString("%s\\%s\\%s", g_applicationDirectory, folder.c_str(), fileName.c_str());
  path = CreateUniqueFileName(path);

  std::ofstream output(path.c_str());
  output << FormatOwnedString("%s\t%s\t%s\t%s\n\n", "ID", "name", "count", "property");
  for (auto item = items.begin(); item != items.end(); ++item) {
    ITEM_INFO info;
    info.SetItemId(item->first);
    ItemDescriptions{g_itemDescriptions}.Name(info, name, capacity, 0);
    std::string amount = FormatOwnedString("%d", item->second);
    std::string ratio = FormatOwnedString("%.5f%%", static_cast<float>(item->second) / static_cast<float>(total) * 100.0f);
    output << FormatOwnedString("%d\t%s\t%s\t%s\n", item->first, name, amount.c_str(), ratio.c_str());
  }

  output << FormatOwnedString("\nresult item count:%d\n", total);
  output.close();
}

#include "UI/UIBroadcastBalloon.h"

#include <chrono>

// OFFSET: 0x00D3C9B0
// FUNCTION: fn_21795
void CGameMode::ReceiveBroadcastBalloonNotice(const unsigned char* packet) {
  std::uint32_t color = 0x62B0FF;
  std::string text = FormatOwnedString(
    LookupMessage(1271), *reinterpret_cast<const int*>(packet + 2), *reinterpret_cast<const int*>(packet + 6),
    *reinterpret_cast<const int*>(packet + 10)
  );
  if (g_serviceType == 12) {
    text = LookupMessage(1125);
  }

  if (!text.empty()) {
    if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
      BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
      message.start = std::chrono::system_clock::now();
      message.lines.push_back(BroadcastBalloonLine{text, color, 0});
      balloon->AddMessage(0, message);
    }
    if (g_serviceType == 0) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0x62B0FF, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D65870
// FUNCTION: fn_22003
void CGameMode::ReceiveUnknownD65870(const unsigned char* packet) {
  this->sub_D5EEF0(
    reinterpret_cast<const char*>(packet + 0x2F), *reinterpret_cast<const int*>(packet + 2), reinterpret_cast<const char*>(packet + 0x47),
    *reinterpret_cast<const int*>(packet + 10), packet[0x16], *reinterpret_cast<const std::uint16_t*>(packet + 0x0E),
    *reinterpret_cast<const std::uint16_t*>(packet + 0x10), reinterpret_cast<const char*>(packet + 0x17), packet[0x57], packet[0x58],
    *reinterpret_cast<const int*>(packet + 6)
  );
}

// OFFSET: 0x00D65FC0
// FUNCTION: fn_22007
void CGameMode::sub_D65FC0(const unsigned char* record) {
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
  if (UIWindow* window = g_windowMgr.EnsureWindow(95)) {
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(data.data()), size, *reinterpret_cast<const int*>(packet + 4), 0);
  }
}

// OFFSET: 0x00D679A0
// FUNCTION: fn_22036
void CGameMode::ReceiveActorSkillResult(const unsigned char* packet) {
  const ActorSkillResultPacket& value = *reinterpret_cast<const ActorSkillResultPacket*>(packet);
  this->sub_D5B030(
    value.source, value.target, value.skill, value.x, value.y, value.tick, value.level, value.flag, value.result
  );
}

// OFFSET: 0x00D66400
// FUNCTION: fn_22012
void CGameMode::ReceiveSimulationResult(const unsigned char* packet) {
  const SimulationResultPacket& value = *reinterpret_cast<const SimulationResultPacket*>(packet);
  switch (value.kind) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("Simulator Start Fail!!!!!!!!!!!!"), 255, 0, 0, 0);
    break;
  case 1:
    this->ReceivePackageSimulation(packet);
    break;
  case 2:
    this->sub_D606B0(packet);
    break;
  case 3:
    this->ReceiveEnchantSimulation(packet);
    break;
  case 4:
    this->ReceiveRandomUpgradeSimulation(packet);
    break;
  }
}
