#include "Contents/ClientState.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/World/World.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstring>
#include <string>

namespace {
#pragma pack(push, 1)
  struct ReplayBlockHeader {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t bytes;
  };

  struct ReplayShopNameRecord {
    /* 0000 */ std::int16_t unknown;
    /* 0002 */ char name[16];
    /* 0012 */ int shopId;
  };
#pragma pack(pop)

  struct ReplayShopItemRecord {
    /* 0000 */ int shopId;
    /* 0004 */ unsigned int index;
    /* 0008 */ int itemId;
    /* 000C */ int first;
    /* 0010 */ int second;
  };

  struct ReplayShopFlagRecord {
    /* 0000 */ int shopId;
    /* 0004 */ char flag;
  };
}

// GLOBAL: gl_24492
bool g_homunculusAutoFeed;

// GLOBAL: gl_24493
bool g_petAutoFeed;

// OFFSET: 0x00E46040
// FUNCTION: fn_24210
std::uint32_t CSession::ReadReplayStats(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 2000) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    if (header.id == 2001) {
      break;
    }

    switch (header.id) {
    case 2010:
      std::memcpy(&this->unknown_166C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2011:
      std::memcpy(&this->unknown_1670, source, header.bytes);
      offset += header.bytes;
      break;
    case 2012:
      std::memcpy(&this->unknown_1674, source, header.bytes);
      offset += header.bytes;
      break;
    case 2013:
      std::memcpy(&this->unknown_1678, source, header.bytes);
      offset += header.bytes;
      break;
    case 2014:
      std::memcpy(&this->unknown_167C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2015:
      std::memcpy(&this->unknown_1680, source, header.bytes);
      offset += header.bytes;
      break;
    case 2016:
      std::memcpy(&this->unknown_0F4C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2017:
      std::memcpy(&this->m_weight, source, header.bytes);
      offset += header.bytes;
      break;
    case 2018:
      std::memcpy(&this->m_maxWeight, source, header.bytes);
      offset += header.bytes;
      break;
    case 2019:
      std::memcpy(&this->unknown_532C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2020:
      std::memcpy(&this->unknown_5330, source, header.bytes);
      offset += header.bytes;
      break;
    case 2021:
      std::memcpy(&this->unknown_5334, source, header.bytes);
      offset += header.bytes;
      break;
    case 2022:
      std::memcpy(&this->unknown_5338, source, header.bytes);
      offset += header.bytes;
      break;
    case 2023:
      std::memcpy(&this->unknown_533C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2024:
      std::memcpy(this->unknown_5340, source, header.bytes);
      offset += header.bytes;
      break;
    case 2025:
      std::memcpy(this->unknown_5370, source, header.bytes);
      offset += header.bytes;
      break;
    case 2026:
      std::memcpy(this->unknown_53A0, source, header.bytes);
      offset += header.bytes;
      break;
    case 2027:
      std::memcpy(this->unknown_53D0, source, header.bytes);
      offset += header.bytes;
      break;
    case 2028:
      std::memcpy(this->unknown_5400, source, header.bytes);
      offset += header.bytes;
      break;
    case 2029:
      std::memcpy(&this->unknown_5568, source, header.bytes);
      offset += header.bytes;
      break;
    case 2030:
      std::memcpy(&this->unknown_556C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2031:
      std::memcpy(&this->unknown_5570[0], source, header.bytes);
      offset += header.bytes;
      break;
    case 2032:
      std::memcpy(&this->unknown_5570[4], source, header.bytes);
      offset += header.bytes;
      break;
    case 2033:
      std::memcpy(&this->m_prohibitionTarget, source, header.bytes);
      offset += header.bytes;
      break;
    case 2034:
      std::memcpy(&this->unknown_5480, source, header.bytes);
      offset += header.bytes;
      break;
    case 2035:
      std::memcpy(&this->unknown_5484, source, header.bytes);
      offset += header.bytes;
      break;
    case 2036:
      std::memcpy(&this->unknown_5488, source, header.bytes);
      offset += header.bytes;
      break;
    case 2037:
      std::memcpy(&this->unknown_548C, source, header.bytes);
      offset += header.bytes;
      break;
    case 2038:
      this->m_accountName.resize(header.bytes);
      std::memcpy(this->m_accountName.data(), data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2052: {
      std::memcpy(&this->unknown_8074, source, header.bytes);
      offset += header.bytes;
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      if (player) {
        player->Slot57(this->unknown_8074);
        player->SendMsg(nullptr, this->unknown_8074 ? 105 : 106, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    default:
      offset += header.bytes;
      break;
    }
  }

  if (g_windowMgr.unknown_04FC == 0) {
    g_windowMgr.unknown_04FC = 1;
    for (UIWindow* window : g_windowMgr.rendered) {
      if (window->Slot03()) {
        window->InvalidateChildren();
      }
    }
  }

  return offset;
}

// OFFSET: 0x00E46620
// FUNCTION: fn_24211
std::uint32_t CSession::ReadReplayMessenger(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 4000) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 4501:
      return offset;
    case 4010:
      std::memcpy(&this->unknown_5938, source, header.bytes);
      offset += header.bytes;
      break;
    case 4011:
      std::memcpy(&this->unknown_5460, source, header.bytes);
      offset += header.bytes;
      break;
    case 4012:
      std::memcpy(&this->unknown_5464, source, header.bytes);
      offset += header.bytes;
      break;
    case 4013: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, source, header.bytes);
      offset += header.bytes;
      this->unknown_54AC = buffer;
      break;
    }
    case 4014: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, source, header.bytes);
      offset += header.bytes;
      this->unknown_54C4 = buffer;
      break;
    }
    case 4015:
      offset = ReadMessengerMembers(this->m_partyMembers, offset, header.bytes, data);
      break;
    case 4016:
      offset = ReadMessengerMembers(this->m_friends, offset, header.bytes, data);
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E46920
// FUNCTION: fn_24212
std::uint32_t CSession::ReadReplayItems(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 4500) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 4501:
    case 4703:
      return offset;
    case 4510:
      offset = ReadItemList(this->m_inventoryItems, offset, header.bytes, data);
      break;
    case 4511:
      offset = ReadItemList(this->unknown_1718, offset, header.bytes, data);
      break;
    case 4512:
      offset = ReadItemList(this->unknown_1720, offset, header.bytes, data);
      break;
    case 4513:
      offset = ReadItemList(this->unknown_1728, offset, header.bytes, data);
      break;
    case 4514:
      offset = ReadItemList(this->unknown_1730, offset, header.bytes, data);
      break;
    case 4515:
      offset = ReadItemList(this->unknown_1738, offset, header.bytes, data);
      break;
    case 4516:
      offset = ReadItemList(this->unknown_1740, offset, header.bytes, data);
      break;
    case 4517:
      offset = ReadItemList(this->unknown_1748, offset, header.bytes, data);
      break;
    case 4518:
      offset = ReadItemList(this->unknown_1750, offset, header.bytes, data);
      break;
    case 4519:
      offset = ReadItemList(this->unknown_1758, offset, header.bytes, data);
      break;
    case 4520:
      offset = ReadItemList(this->unknown_1760, offset, header.bytes, data);
      break;
    case 4521:
      offset = ReadItemList(this->unknown_1768, offset, header.bytes, data);
      break;
    case 4522:
      offset = ReadItemList(this->unknown_1770, offset, header.bytes, data);
      break;
    case 4601:
      offset = ReadItemArray(this->unknown_17F0, offset, header.bytes, data);
      break;
    case 4602:
      offset = ReadItemArray(this->unknown_21A0, offset, header.bytes, data);
      break;
    case 4603:
      offset = ReadItemArray(this->unknown_2B50, offset, header.bytes, data);
      break;
    case 4604:
      offset = ReadItemArray(this->unknown_3500, offset, header.bytes, data);
      break;
    case 4605:
      offset = ReadItemArray(this->m_exchangeItems, offset, header.bytes, data);
      break;
    case 4606:
      offset = ReadItemArray(this->m_itemSlots, offset, header.bytes, data);
      break;
    case 4701:
      std::memcpy(&this->unknown_5210, source, header.bytes);
      offset += header.bytes;
      break;
    case 4702:
      std::memcpy(&this->unknown_5214, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E46DC0
// FUNCTION: fn_24213
std::uint32_t CSession::ReadReplayShops(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 7500) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 7501:
      return offset;
    case 7502: {
      ReplayShopNameRecord record{};
      std::memcpy(&record, source, header.bytes);
      offset += header.bytes;
      std::string name(record.name);
      this->AddShopName(name, record.shopId);
      break;
    }
    case 7504: {
      ReplayShopItemRecord record{};
      std::memcpy(&record, source, header.bytes);
      offset += header.bytes;
      g_session.SetShopItem(record.shopId, record.index, record.itemId, record.first, record.second);
      break;
    }
    case 7505: {
      ReplayShopFlagRecord record{};
      std::memcpy(&record, source, header.bytes);
      offset += header.bytes;
      g_session.SetShopFlag(record.shopId, record.flag);
      break;
    }
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E470D0
// FUNCTION: fn_24214
std::uint32_t CSession::ReadReplayExtra(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 5500) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    bool done = false;
    switch (header.id) {
    case 5501:
      done = true;
      break;
    case 5510:
      std::memcpy(&this->unknown_593C, source, header.bytes);
      offset += header.bytes;
      break;
    case 5511:
      std::memcpy(&this->unknown_5940, source, header.bytes);
      offset += header.bytes;
      break;
    case 5512:
      std::memcpy(&this->unknown_5944, source, header.bytes);
      offset += header.bytes;
      break;
    case 5513:
      std::memcpy(&this->unknown_5948, source, header.bytes);
      offset += header.bytes;
      break;
    case 5514:
      std::memcpy(&this->unknown_594C, source, header.bytes);
      offset += header.bytes;
      break;
    case 5515: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, source, header.bytes);
      offset += header.bytes;
      this->unknown_5950 = buffer;
      break;
    }
    case 5516:
      std::memcpy(&this->unknown_5968, source, header.bytes);
      offset += header.bytes;
      break;
    case 5517:
      std::memcpy(&this->unknown_596C, source, header.bytes);
      offset += header.bytes;
      break;
    case 5518:
      std::memcpy(&this->unknown_5970, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }

    if (done) {
      break;
    }
  }

  if (this->unknown_593C == 1) {
    g_windowMgr.EnsureWindow(0x89);
  }

  return offset;
}

// OFFSET: 0x00E47410
// FUNCTION: fn_24215
std::uint32_t CSession::ReadReplayReserved(const std::vector<unsigned char>&) {
  return 0;
}

// OFFSET: 0x00E47420
// FUNCTION: fn_24216
std::uint32_t CSession::ReadReplayCompanions(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 5000) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 5001:
      return offset;
    case 5101:
      std::memcpy(&this->unknown_5570[8], source, header.bytes);
      offset += header.bytes;
      break;
    case 5102:
      std::memcpy(&this->unknown_5570[0xC], source, header.bytes);
      offset += header.bytes;
      break;
    case 5103:
      std::memcpy(&this->unknown_5570[0x2C], source, header.bytes);
      offset += header.bytes;
      break;
    case 5104:
      std::memcpy(&this->unknown_5570[0x30], source, header.bytes);
      offset += header.bytes;
      break;
    case 5105:
      std::memcpy(&this->unknown_5570[0x34], source, header.bytes);
      offset += header.bytes;
      break;
    case 5106:
      std::memcpy(&this->unknown_5570[0x38], source, header.bytes);
      offset += header.bytes;
      break;
    case 5107:
      std::memcpy(&this->unknown_5570[0x3C], source, header.bytes);
      offset += header.bytes;
      break;
    case 5108:
      std::memcpy(&this->unknown_5570[0x40], source, header.bytes);
      offset += header.bytes;
      break;
    case 5109:
      std::memcpy(&this->unknown_5570[0x44], source, header.bytes);
      offset += header.bytes;
      break;
    case 5110:
      std::memcpy(&this->unknown_5570[0x48], source, header.bytes);
      offset += header.bytes;
      break;
    case 5111:
      std::memcpy(&this->m_homunJob, source, header.bytes);
      offset += header.bytes;
      break;
    case 5112:
      std::memcpy(this->unknown_55C0, source, header.bytes);
      offset += header.bytes;
      break;
    case 5113:
      std::memcpy(&this->m_homunHp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5114:
      std::memcpy(this->unknown_55C8, source, header.bytes);
      offset += header.bytes;
      break;
    case 5115:
      std::memcpy(&this->m_homunSp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5116:
      std::memcpy(&this->unknown_55D0, source, header.bytes);
      offset += header.bytes;
      break;
    case 5117:
      std::memcpy(&this->unknown_55D4, source, header.bytes);
      offset += header.bytes;
      break;
    case 5118:
      std::memcpy(&this->unknown_55D8, source, header.bytes);
      offset += header.bytes;
      break;
    case 5119:
      std::memcpy(&this->m_homunExp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5120:
      std::memcpy(&this->m_homunMaxExp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5121:
      std::memcpy(&this->unknown_55F0, source, header.bytes);
      offset += header.bytes;
      break;
    case 5122:
      std::memcpy(&this->unknown_55F4, source, header.bytes);
      offset += header.bytes;
      break;
    case 5123:
      std::memcpy(&this->unknown_55F8, source, header.bytes);
      offset += header.bytes;
      break;
    case 5124:
      std::memcpy(&this->unknown_55FC, source, header.bytes);
      offset += header.bytes;
      break;
    case 5125:
      std::memcpy(&this->unknown_5600, source, header.bytes);
      offset += header.bytes;
      break;
    case 5126:
      std::memcpy(&this->unknown_5604, source, header.bytes);
      offset += header.bytes;
      break;
    case 5127:
      std::memcpy(&this->unknown_5608, source, header.bytes);
      offset += header.bytes;
      break;
    case 5128:
      std::memcpy(&this->unknown_560C, source, header.bytes);
      offset += header.bytes;
      break;
    case 5129:
      std::memcpy(&this->unknown_5610, source, header.bytes);
      offset += header.bytes;
      break;
    case 5130:
      std::memcpy(&this->unknown_5614, source, header.bytes);
      offset += header.bytes;
      break;
    case 5131:
      std::memcpy(&this->m_traceAi, source, header.bytes);
      offset += header.bytes;
      break;
    case 5132:
      std::memcpy(&this->m_homunActionState, source, header.bytes);
      offset += header.bytes;
      break;
    case 5133:
      std::memcpy(&this->unknown_5620[0], source, header.bytes);
      offset += header.bytes;
      break;
    case 5134:
      std::memcpy(&this->unknown_5620[4], source, header.bytes);
      offset += header.bytes;
      break;
    case 5201:
      std::memcpy(&this->unknown_5620[8], source, header.bytes);
      offset += header.bytes;
      break;
    case 5202:
      std::memcpy(this->m_mercenaryName, source, header.bytes);
      offset += header.bytes;
      break;
    case 5203:
      std::memcpy(&this->m_mercenaryStats[0], source, header.bytes);
      offset += header.bytes;
      break;
    case 5204:
      std::memcpy(&this->m_mercenaryStats[1], source, header.bytes);
      offset += header.bytes;
      break;
    case 5205:
      std::memcpy(&this->m_mercenaryStats[2], source, header.bytes);
      offset += header.bytes;
      break;
    case 5206:
      std::memcpy(&this->m_mercenaryStats[3], source, header.bytes);
      offset += header.bytes;
      break;
    case 5207:
      std::memcpy(&this->m_mercenaryStats[4], source, header.bytes);
      offset += header.bytes;
      break;
    case 5208:
      std::memcpy(&this->m_mercenaryStats[5], source, header.bytes);
      offset += header.bytes;
      break;
    case 5209:
      std::memcpy(&this->m_mercenaryStats[6], source, header.bytes);
      offset += header.bytes;
      break;
    case 5210:
      std::memcpy(&this->m_mercenaryStats[7], source, header.bytes);
      offset += header.bytes;
      break;
    case 5211:
      std::memcpy(&this->m_mercenaryHp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5212:
      std::memcpy(&this->m_mercenaryMaxHp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5213:
      std::memcpy(&this->m_mercenaryClass, source, header.bytes);
      offset += header.bytes;
      break;
    case 5214:
      std::memcpy(&this->m_mercenaryLevel, source, header.bytes);
      offset += header.bytes;
      break;
    case 5215:
      std::memcpy(&this->m_mercenarySp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5216:
      std::memcpy(&this->m_mercenaryMaxSp, source, header.bytes);
      offset += header.bytes;
      break;
    case 5217:
      std::memcpy(&this->m_mercenaryLoyalty, source, header.bytes);
      offset += header.bytes;
      break;
    case 5218:
      std::memcpy(&this->m_mercenarySummons, source, header.bytes);
      offset += header.bytes;
      break;
    case 5219:
      std::memcpy(&this->m_mercenaryKills, source, header.bytes);
      offset += header.bytes;
      break;
    case 5220:
      std::memcpy(this->unknown_5690, source, header.bytes);
      offset += header.bytes;
      break;
    case 5221:
      std::memcpy(&this->m_mercenaryExpiration, source, header.bytes);
      offset += header.bytes;
      break;
    case 5222:
      std::memcpy(&this->unknown_5698, source, header.bytes);
      offset += header.bytes;
      break;
    case 5223:
      std::memcpy(&this->unknown_569C, source, header.bytes);
      offset += header.bytes;
      break;
    case 5224:
      std::memcpy(&this->unknown_56A0, source, header.bytes);
      offset += header.bytes;
      break;
    case 5225:
      std::memcpy(&this->unknown_56A4, source, header.bytes);
      offset += header.bytes;
      break;
    case 5226:
      std::memcpy(&this->unknown_56A8, source, header.bytes);
      offset += header.bytes;
      break;
    case 5227:
      std::memcpy(&this->unknown_56AC, source, header.bytes);
      offset += header.bytes;
      break;
    case 5228:
      std::memcpy(&this->unknown_56B0, source, header.bytes);
      offset += header.bytes;
      break;
    case 5229:
      std::memcpy(&this->m_mercenaryActionState, source, header.bytes);
      offset += header.bytes;
      break;
    case 5301:
      std::memcpy(&this->unknown_1010, source, header.bytes);
      offset += header.bytes;
      break;
    case 5302:
      std::memcpy(this->unknown_1014, source, header.bytes);
      offset += header.bytes;
      break;
    case 5303:
      std::memcpy(this->unknown_1018, source, header.bytes);
      offset += header.bytes;
      break;
    case 5304:
      std::memcpy(&this->unknown_1038, source, header.bytes);
      offset += header.bytes;
      break;
    case 5305:
      std::memcpy(&this->unknown_103C, source, header.bytes);
      offset += header.bytes;
      break;
    case 5306:
      std::memcpy(&this->unknown_1040, source, header.bytes);
      offset += header.bytes;
      break;
    case 5307:
      std::memcpy(&this->unknown_1044, source, header.bytes);
      offset += header.bytes;
      break;
    case 5308:
      std::memcpy(&this->m_petFriendly, source, header.bytes);
      offset += header.bytes;
      break;
    case 5309:
      std::memcpy(&this->unknown_1050, source, header.bytes);
      offset += header.bytes;
      break;
    case 5310:
      std::memcpy(&this->unknown_1054, source, header.bytes);
      offset += header.bytes;
      break;
    case 5401:
      std::memcpy(&this->unknown_5BC0, source, header.bytes);
      offset += header.bytes;
      break;
    case 5402:
      std::memcpy(&this->unknown_5BC4, source, header.bytes);
      offset += header.bytes;
      break;
    case 5403:
      std::memcpy(&this->unknown_5BC8, source, header.bytes);
      offset += header.bytes;
      break;
    case 5404:
      std::memcpy(&this->unknown_5BCC, source, header.bytes);
      offset += header.bytes;
      break;
    case 5405:
      std::memcpy(&this->unknown_5BD0, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E488C0
// FUNCTION: fn_24221
void CSession::UpdateBodyStyle(int job, int style, int renderJob) {
  if (style >= 2 || this->IsSecondJob(renderJob)) {
    this->m_bodyStyle = style;
  } else if (static_cast<unsigned>(job - 4001) <= 21) {
    this->m_bodyStyle = job <= 4007;
  } else {
    this->m_bodyStyle = static_cast<unsigned>(job - 4001) <= 6;
  }
}

// OFFSET: 0x00E48930
// FUNCTION: fn_24222
void CSession::SetHomunculusAutoFeed(bool enabled) {
  int message;
  if (enabled == true) {
    this->m_homunculusAutoFeed = true;
    message = 3282;
  } else {
    g_homunculusAutoFeed = false;
    message = 3283;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E48980
// FUNCTION: fn_24223
void CSession::SetPetAutoFeed(bool enabled) {
  g_petAutoFeed = enabled == true;
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(enabled ? 2579 : 2580)), 0x64FFFF, 0, 0, 0);
}

extern "C" void __cdecl sub_6C2A80(int id, bool value);

struct UIWindowMgrHelper {
  void sub_AAB070(int id);
};

// OFFSET: 0x00E489C0
// FUNCTION: fn_24224
void __stdcall sub_E489C0(bool enabled) {
  std::string notice = LookupMessage(enabled ? 4357 : 4379);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(notice.c_str()), 0x64FFFF, 0, 0, 0);
  sub_6C2A80(0x128, enabled);
  reinterpret_cast<UIWindowMgrHelper*>(&g_windowMgr)->sub_AAB070(0x128);
}

// OFFSET: 0x00E48B00
// FUNCTION: fn_24225
void CSession::CollectCardItems() {
  this->m_cardItems.clear();
  for (const ITEM_INFO& item : this->m_inventoryItems) {
    if (item.unknown_0000[0] == 6) {
      this->m_cardItems.push_back(item);
    }
  }
}

// OFFSET: 0x00E48B70
// FUNCTION: fn_24226
void RequestBasicInfoRefresh() {
  if (g_windowMgr.unknown_01E0 != nullptr) {
    g_windowMgr.unknown_01E0->SendMsg(nullptr, 106, 575, 0, 0, 0);
  }
}

// OFFSET: 0x00E49290
// FUNCTION: fn_24232
void CSession::SetUnknown5BC0(int value) {
  this->unknown_5BC0 = value;
  UIWindow* window = g_windowMgr.LookupWindow(0x272E);
  if (window != nullptr) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E4A180
// FUNCTION: fn_24249
void AppendBytes(std::vector<unsigned char>& output, const std::vector<unsigned char>& bytes) {
  output.insert(output.end(), bytes.begin(), bytes.end());
}

// OFFSET: 0x00E44EE0
// FUNCTION: fn_24208
std::uint32_t CSession::ReadReplayEntryQueue(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  int waiting;
  int ranking;
  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 6500) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 6501:
      return offset;
    case 6511: {
      int queueId = 0;
      std::memcpy(&queueId, source, header.bytes);
      offset += header.bytes;
      for (const auto& entry : g_entryQueueList) {
        if (queueId == entry.second->id) {
          g_entryQueueMgr->current = entry.second;
        }
      }
      break;
    }
    case 6512: {
      unsigned char ready = 1;
      std::memcpy(&ready, source, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->ready = ready;
      break;
    }
    case 6513: {
      char queueName[0x30];
      std::memset(queueName, 0, sizeof(queueName));
      std::memcpy(queueName, source, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->SetQueueName(*reinterpret_cast<const EntryQueueName*>(queueName));
      break;
    }
    case 6514: {
      char characterName[0x30];
      std::memset(characterName, 0, sizeof(characterName));
      std::memcpy(characterName, source, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->SetCharacterName(*reinterpret_cast<const EntryQueueName*>(characterName));
      break;
    }
    case 6515: {
      int value;
      std::memcpy(&value, source, header.bytes);
      offset += header.bytes;
      UIEntryQueueWnd* window = dynamic_cast<UIEntryQueueWnd*>(g_windowMgr.LookupWindow(157));
      if (window) {
        window->SendMsg(nullptr, 135, 0, value, 0, 0);
      }
      break;
    }
    case 6516: {
      char queueText[100];
      std::memset(queueText, 0, sizeof(queueText));
      std::memcpy(queueText, source, header.bytes);
      offset += header.bytes;
      UIEntryQueueStandByWnd* window = dynamic_cast<UIEntryQueueStandByWnd*>(g_windowMgr.LookupWindow(209));
      if (window) {
        window->SetInfo(queueText, waiting, ranking);
      }
      break;
    }
    case 6517:
      std::memcpy(&waiting, source, header.bytes);
      offset += header.bytes;
      break;
    case 6518:
      std::memcpy(&ranking, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E452D0
// FUNCTION: fn_24209
std::uint32_t CSession::ReadReplayCharacter(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ReplayBlockHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 1000) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 1001:
      return offset;
    case 1010:
      std::memcpy(&this->m_aid, source, header.bytes);
      offset += header.bytes;
      break;
    case 1011:
      std::memcpy(&this->m_gid, source, header.bytes);
      offset += header.bytes;
      break;
    case 1012:
      std::memcpy(&this->m_navigationPlayerMap[0x14], source, header.bytes);
      offset += header.bytes;
      break;
    case 1013:
      std::memcpy(&this->m_navigationPlayerMap[0x18], source, header.bytes);
      offset += header.bytes;
      break;
    case 1014:
      std::memcpy(&this->m_job, source, header.bytes);
      offset += header.bytes;
      break;
    case 1015:
      std::memcpy(&this->unknown_162C[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1016:
      std::memcpy(&this->m_level, source, header.bytes);
      offset += header.bytes;
      break;
    case 1017:
      std::memcpy(this->unknown_1654, source, header.bytes);
      offset += header.bytes;
      break;
    case 1018:
      std::memcpy(&this->unknown_162C[0xC], source, header.bytes);
      offset += header.bytes;
      break;
    case 1019:
      std::memcpy(&this->m_jobLevel, source, header.bytes);
      offset += header.bytes;
      break;
    case 1020:
      std::memcpy(this->unknown_165C, source, header.bytes);
      offset += header.bytes;
      break;
    case 1021:
      std::memcpy(&this->m_guildSkillPoint, source, header.bytes);
      offset += header.bytes;
      break;
    case 1022:
      std::memcpy(this->unknown_1664, source, header.bytes);
      offset += header.bytes;
      break;
    case 1023:
      std::memcpy(&this->unknown_1664[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1024:
      std::memcpy(&this->m_standardStats[0], source, header.bytes);
      offset += header.bytes;
      break;
    case 1025:
      std::memcpy(&this->m_standardStats[1], source, header.bytes);
      offset += header.bytes;
      break;
    case 1026:
      std::memcpy(&this->m_standardStats[2], source, header.bytes);
      offset += header.bytes;
      break;
    case 1027:
      std::memcpy(&this->m_standardStats[3], source, header.bytes);
      offset += header.bytes;
      break;
    case 1028:
      std::memcpy(&this->m_standardStats[4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1029:
      std::memcpy(&this->m_standardStats[5], source, header.bytes);
      offset += header.bytes;
      break;
    case 1030:
      std::memcpy(this->unknown_169C, source, header.bytes);
      offset += header.bytes;
      break;
    case 1031:
      std::memcpy(&this->unknown_169C[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1032:
      std::memcpy(&this->unknown_169C[0x8], source, header.bytes);
      offset += header.bytes;
      break;
    case 1033:
      std::memcpy(&this->unknown_169C[0xC], source, header.bytes);
      offset += header.bytes;
      break;
    case 1034:
      std::memcpy(&this->unknown_169C[0x10], source, header.bytes);
      offset += header.bytes;
      break;
    case 1035:
      std::memcpy(&this->unknown_169C[0x14], source, header.bytes);
      offset += header.bytes;
      break;
    case 1036:
      std::memcpy(&this->unknown_169C[0x18], source, header.bytes);
      offset += header.bytes;
      break;
    case 1037:
      std::memcpy(&this->unknown_169C[0x1C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1038:
      std::memcpy(&this->unknown_169C[0x20], source, header.bytes);
      offset += header.bytes;
      break;
    case 1039:
      std::memcpy(&this->unknown_169C[0x24], source, header.bytes);
      offset += header.bytes;
      break;
    case 1040:
      std::memcpy(&this->unknown_169C[0x28], source, header.bytes);
      offset += header.bytes;
      break;
    case 1041:
      std::memcpy(&this->unknown_169C[0x2C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1042:
      std::memcpy(&this->unknown_169C[0x30], source, header.bytes);
      offset += header.bytes;
      break;
    case 1043:
      std::memcpy(&this->unknown_169C[0x34], source, header.bytes);
      offset += header.bytes;
      break;
    case 1044:
      std::memcpy(&this->unknown_169C[0x38], source, header.bytes);
      offset += header.bytes;
      break;
    case 1045:
      std::memcpy(&this->unknown_169C[0x3C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1046:
      std::memcpy(&this->unknown_169C[0x40], source, header.bytes);
      offset += header.bytes;
      break;
    case 1047:
      std::memcpy(&this->unknown_169C[0x44], source, header.bytes);
      offset += header.bytes;
      break;
    case 1048:
      std::memcpy(&this->unknown_169C[0x48], source, header.bytes);
      offset += header.bytes;
      break;
    case 1049:
      std::memcpy(&this->unknown_169C[0x4C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1050:
      std::memcpy(&this->unknown_16EC, source, header.bytes);
      offset += header.bytes;
      break;
    case 1051:
      std::memcpy(&this->m_zeny, source, header.bytes);
      offset += header.bytes;
      break;
    case 1052:
      std::memcpy(this->unknown_16F4, source, header.bytes);
      offset += header.bytes;
      break;
    case 1053:
      std::memcpy(&this->unknown_16F4[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1054:
      std::memcpy(&this->unknown_162C[0x14], source, header.bytes);
      offset += header.bytes;
      break;
    case 1055:
      std::memcpy(&this->unknown_162C[0x1C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1056:
      std::memcpy(this->unknown_1704, source, header.bytes);
      offset += header.bytes;
      break;
    case 1057:
      std::memcpy(&this->unknown_1708, source, header.bytes);
      offset += header.bytes;
      break;
    case 1058:
      std::memcpy(this->unknown_170C, source, header.bytes);
      offset += header.bytes;
      break;
    case 1059:
      std::memcpy(&this->unknown_0E9C[0x34], source, header.bytes);
      offset += header.bytes;
      break;
    case 1060:
      std::memcpy(&this->m_head, source, header.bytes);
      offset += header.bytes;
      break;
    case 1061:
      std::memcpy(&this->unknown_0EDC[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1062:
      std::memcpy(&this->unknown_0EDC[0xC], source, header.bytes);
      offset += header.bytes;
      break;
    case 1063:
      std::memcpy(&this->m_bodyStyle, source, header.bytes);
      offset += header.bytes;
      break;
    case 1064:
      std::memcpy(&this->m_headPalette, source, header.bytes);
      offset += header.bytes;
      break;
    case 1065:
      std::memcpy(this->unknown_0EF4, source, header.bytes);
      offset += header.bytes;
      break;
    case 1066:
      std::memcpy(&this->unknown_0EF4[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1067:
      std::memcpy(&this->unknown_0EF4[0x8], source, header.bytes);
      offset += header.bytes;
      break;
    case 1068:
      std::memcpy(this->unknown_0F0C, source, header.bytes);
      offset += header.bytes;
      break;
    case 1069:
      std::memcpy(&this->unknown_0F0C[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1070:
      std::memcpy(&this->m_jobFlags, source, header.bytes);
      offset += header.bytes;
      break;
    case 1071:
      std::memcpy(&this->m_robe, source, header.bytes);
      offset += header.bytes;
      break;
    case 1072:
      std::memcpy(&this->unknown_5974[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1073:
      std::memcpy(&this->unknown_5974[0x8], source, header.bytes);
      offset += header.bytes;
      break;
    case 1074:
      std::memcpy(&this->unknown_5974[0xC], source, header.bytes);
      offset += header.bytes;
      break;
    case 1075:
      std::memcpy(&this->unknown_5974[0x10], source, header.bytes);
      offset += header.bytes;
      break;
    case 1076:
      std::memcpy(&this->unknown_5974[0x14], source, header.bytes);
      offset += header.bytes;
      break;
    case 1077:
      std::memcpy(&this->unknown_5974[0x18], source, header.bytes);
      offset += header.bytes;
      break;
    case 1078:
      std::memcpy(&this->unknown_5974[0x1C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1079:
      std::memcpy(&this->unknown_5974[0x20], source, header.bytes);
      offset += header.bytes;
      break;
    case 1080:
      std::memcpy(&this->unknown_5974[0x24], source, header.bytes);
      offset += header.bytes;
      break;
    case 1081:
      std::memcpy(&this->unknown_5974[0x2C], source, header.bytes);
      offset += header.bytes;
      break;
    case 1082:
      std::memcpy(this->unknown_0F24, source, header.bytes);
      offset += header.bytes;
      break;
    case 1083:
      std::memcpy(&this->m_score1, source, header.bytes);
      offset += header.bytes;
      break;
    case 1084:
      std::memcpy(&this->m_score2, source, header.bytes);
      offset += header.bytes;
      break;
    case 1085:
      std::memcpy(this->unknown_0F30, source, header.bytes);
      offset += header.bytes;
      break;
    case 1086:
      std::memcpy(&this->cartCount, source, header.bytes);
      offset += header.bytes;
      break;
    case 1087:
      std::memcpy(&this->cartMaxCount, source, header.bytes);
      offset += header.bytes;
      break;
    case 1088:
      std::memcpy(&this->cartWeight, source, header.bytes);
      offset += header.bytes;
      break;
    case 1089:
      std::memcpy(&this->cartMaxWeight, source, header.bytes);
      offset += header.bytes;
      break;
    case 1090:
      std::memcpy(this->unknown_0F44, source, header.bytes);
      offset += header.bytes;
      break;
    case 1091:
      std::memcpy(&this->unknown_0F44[0x4], source, header.bytes);
      offset += header.bytes;
      break;
    case 1092:
      std::memcpy(&this->unknown_5490[0x8], source, header.bytes);
      offset += header.bytes;
      break;
    case 1093:
      std::memcpy(&this->unknown_5490[0xC], source, header.bytes);
      offset += header.bytes;
      break;
    case 1094:
      std::memcpy(&this->unknown_54A0, source, header.bytes);
      offset += header.bytes;
      break;
    case 1095:
      std::memcpy(&this->unknown_54A4, source, header.bytes);
      offset += header.bytes;
      break;
    case 1096:
      std::memcpy(&this->unknown_54A8, source, header.bytes);
      offset += header.bytes;
      break;
    case 1097:
      this->unknown_550C.resize(header.bytes);
      std::memcpy(this->unknown_550C.data(), data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1099:
      std::memcpy(&this->unknown_5468, source, header.bytes);
      offset += header.bytes;
      break;
    case 1100:
      std::memcpy(&this->m_bodyCostume, source, header.bytes);
      offset += header.bytes;
      break;
    case 1101:
      std::memcpy(&this->unknown_7FBC, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}
