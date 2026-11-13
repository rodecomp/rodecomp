#include "Contents/ClientState.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"

#include "Client/Mode/CGameMode.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Quest/RenewQuestMgr.h"
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

    // INLINED
    ReplayBlockHeader() : id(0), bytes(0) {
    }

    // INLINED
    ~ReplayBlockHeader() {
      this->id = 0;
      this->bytes = 0;
    }

    // INLINED
    void Read(const unsigned char* source) {
      this->id = 0;
      this->bytes = 0;
      std::memcpy(this, source, sizeof(*this));
    }
  };

  struct ReplayShopNameRecord {
    /* 0000 */ std::int16_t padding_0000;
    /* 0002 */ char name[16];
    /* 0012 */ int shopId;
  };
#pragma pack(pop)

  struct ReplayShopItemRecord {
    /* 0000 */ int shopId = 0;
    /* 0004 */ unsigned int index = 0;
    /* 0008 */ int itemId = 0;
    /* 000C */ int first = 0;
    /* 0010 */ int second = 0;
  };

  struct ReplayShopFlagRecord {
    /* 0000 */ int shopId = 0;
    /* 0004 */ char flag = 0;
  };
}


// OFFSET: 0x00E46040
// FUNCTION: fn_24210
std::uint32_t CSession::ReadReplayStats(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (2000 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 2001:
      this->InvalidateWindowFrames();
      return offset;
    case 2010:
      std::memcpy(&this->strengthBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2011:
      std::memcpy(&this->agilityBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2012:
      std::memcpy(&this->vitalityBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2013:
      std::memcpy(&this->intelligenceBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2014:
      std::memcpy(&this->dexterityBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2015:
      std::memcpy(&this->luckBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2016:
      std::memcpy(&this->storeItemLimit, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2017:
      std::memcpy(&this->m_weight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2018:
      std::memcpy(&this->m_maxWeight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2019:
      std::memcpy(&this->randomStatIndexA, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2020:
      std::memcpy(&this->randomStatIndexB, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2021:
      std::memcpy(&this->randomStatIndexC, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2022:
      std::memcpy(&this->randomStatIndexD, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2023:
      std::memcpy(&this->randomStatIndexE, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2024:
      std::memcpy(this->randomStatValues, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2025:
      std::memcpy(this->replayStatValuesA, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2026:
      std::memcpy(this->replayStatValuesB, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2027:
      std::memcpy(this->replayStatValuesC, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2028:
      std::memcpy(this->replayStatValuesD, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2029:
      std::memcpy(&this->hp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2030:
      std::memcpy(&this->maxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2031:
      std::memcpy(&this->m_sp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2032:
      std::memcpy(&this->m_maxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2033:
      std::memcpy(&this->m_prohibitionTarget, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2034:
      std::memcpy(&this->quitBlocked, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2035:
      std::memcpy(&this->battleChat, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2036:
      std::memcpy(&this->quitAllowed, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2037:
      std::memcpy(&this->sessionFlag, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2038:
      this->m_accountName.resize(header.bytes);
      std::memcpy(this->m_accountName.data(), data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 2052: {
      std::memcpy(&this->madogearType, data.data() + offset, header.bytes);
      offset += header.bytes;
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      if (player) {
        player->SetMadogearType(this->madogearType);
        if (this->madogearType) {
          player->SendMsg(nullptr, 105, 0, 0, 0, 0, 0, 0);
        } else {
          player->SendMsg(nullptr, 106, 0, 0, 0, 0, 0, 0);
        }
      }
      break;
    }
    default:
      offset += header.bytes;
      break;
    }
  }

  this->InvalidateWindowFrames();

  return offset;
}

// OFFSET: 0x00E46620
// FUNCTION: fn_24211
std::uint32_t CSession::ReadReplayMessenger(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (4000 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 4501:
      return offset;
    case 4010:
      std::memcpy(&this->replaySessionFlag, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 4011:
      std::memcpy(&this->partyBlocked, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 4012:
      std::memcpy(&this->partyLeader, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 4013: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->partyRequestName = buffer;
      break;
    }
    case 4014: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->partyName = buffer;
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

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (4500 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 4501:
    case 4703:
      return offset;
    case 4701:
      std::memcpy(&this->exchangeMoney, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 4702:
      std::memcpy(&this->otherExchangeMoney, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 4510:
      offset = ReadItemList(this->m_inventoryItems, offset, header.bytes, data);
      break;
    case 4511:
      offset = ReadItemList(this->shopItems, offset, header.bytes, data);
      break;
    case 4512:
      offset = ReadItemList(this->purchaseItems, offset, header.bytes, data);
      break;
    case 4513:
      offset = ReadItemList(this->sellItems, offset, header.bytes, data);
      break;
    case 4514:
      offset = ReadItemList(this->storageItems, offset, header.bytes, data);
      break;
    case 4515:
      offset = ReadItemList(this->cartItems, offset, header.bytes, data);
      break;
    case 4516:
      offset = ReadItemList(this->merchantShopItems, offset, header.bytes, data);
      break;
    case 4517:
      offset = ReadItemList(this->merchantMirrorItems, offset, header.bytes, data);
      break;
    case 4518:
      offset = ReadItemList(this->merchantMyShopItems, offset, header.bytes, data);
      break;
    case 4519:
      offset = ReadItemList(this->merchantPurchaseItems, offset, header.bytes, data);
      break;
    case 4520:
      offset = ReadItemList(this->merchantHopeToSellItems, offset, header.bytes, data);
      break;
    case 4521:
      offset = ReadItemList(this->merchantPurchaseSavedItems, offset, header.bytes, data);
      break;
    case 4522:
      offset = ReadItemList(this->filteredInventoryItems, offset, header.bytes, data);
      break;
    case 4601:
      offset = ReadItemArray(this->equippedItems, offset, header.bytes, data);
      break;
    case 4602:
      offset = ReadItemArray(this->alternateEquippedItems, offset, header.bytes, data);
      break;
    case 4603:
      offset = ReadItemArray(this->costumeItems, offset, header.bytes, data);
      break;
    case 4604:
      offset = ReadItemArray(this->alternateCostumeItems, offset, header.bytes, data);
      break;
    case 4605:
      offset = ReadItemArray(this->m_exchangeItems, offset, header.bytes, data);
      break;
    case 4606:
      offset = ReadItemArray(this->m_itemSlots, offset, header.bytes, data);
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

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (7500 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 7501:
      return offset;
    case 7502: {
      ReplayShopNameRecord record{};
      std::memcpy(&record, data.data() + offset, header.bytes);
      offset += header.bytes;
      std::string name(record.name);
      this->AddShopName(name, record.shopId);
      break;
    }
    case 7504: {
      ReplayShopItemRecord record;
      std::memcpy(&record, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_session.SetShopItem(record.shopId, record.index, record.itemId, record.first, record.second);
      break;
    }
    case 7505: {
      ReplayShopFlagRecord record;
      std::memcpy(&record, data.data() + offset, header.bytes);
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

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (5500 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    bool done = false;
    switch (header.id) {
    case 5501:
      done = true;
      break;
    case 5510:
      std::memcpy(&this->memorialWindowOpen, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5511:
      std::memcpy(&this->memorialWindowX, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5512:
      std::memcpy(&this->memorialWindowY, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5513:
      std::memcpy(&this->memorialDungeonState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5514:
      std::memcpy(&this->memorialReplayValue, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5515: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->memorialDungeonName = buffer;
      break;
    }
    case 5516:
      std::memcpy(&this->memorialQueuePosition, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5517:
      std::memcpy(&this->memorialCreationTime, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5518:
      std::memcpy(&this->memorialExpirationTime, data.data() + offset, header.bytes);
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

  if (this->memorialWindowOpen == 1) {
    g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND);
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

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (5000 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 5001:
      return offset;
    case 5101:
      std::memcpy(&this->m_homunAid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5102:
      std::memcpy(this->m_homunName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5103:
      std::memcpy(&this->m_homunStats[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5104:
      std::memcpy(&this->m_homunStats[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5105:
      std::memcpy(&this->m_homunStats[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5106:
      std::memcpy(&this->m_homunStats[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5107:
      std::memcpy(&this->m_homunStats[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5108:
      std::memcpy(&this->m_homunStats[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5109:
      std::memcpy(&this->m_homunStats[6], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5110:
      std::memcpy(&this->m_homunStats[7], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5111:
      std::memcpy(&this->m_homunJob, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5112:
      std::memcpy(&this->m_homunLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5113:
      std::memcpy(&this->m_homunHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5114:
      std::memcpy(&this->m_homunMaxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5115:
      std::memcpy(&this->m_homunSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5116:
      std::memcpy(&this->homunMaxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5117:
      std::memcpy(&this->homunFriendly, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5118:
      std::memcpy(&this->homunAccessory, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5119:
      std::memcpy(&this->m_homunExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5120:
      std::memcpy(&this->m_homunMaxExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5121:
      std::memcpy(&this->homunHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5122:
      std::memcpy(&this->homunMaxHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5123:
      std::memcpy(&this->homunNameModified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5124:
      std::memcpy(&this->homunEnemyGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5125:
      std::memcpy(&this->homunEnemyX, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5126:
      std::memcpy(&this->homunEnemyY, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5127:
      std::memcpy(&this->moveHomunEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5128:
      std::memcpy(&this->selectHomunEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5129:
      std::memcpy(&this->attackHomunEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5130:
      std::memcpy(&this->homunAlive, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5131:
      std::memcpy(&this->m_traceAi, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5132:
      std::memcpy(&this->m_homunActionState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5133:
      std::memcpy(&this->m_homunHungerNotified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5134:
      std::memcpy(&this->m_homunAttackRange, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5201:
      std::memcpy(&this->m_mercenaryAid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5202:
      std::memcpy(this->m_mercenaryName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5203:
      std::memcpy(&this->m_mercenaryStats[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5204:
      std::memcpy(&this->m_mercenaryStats[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5205:
      std::memcpy(&this->m_mercenaryStats[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5206:
      std::memcpy(&this->m_mercenaryStats[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5207:
      std::memcpy(&this->m_mercenaryStats[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5208:
      std::memcpy(&this->m_mercenaryStats[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5209:
      std::memcpy(&this->m_mercenaryStats[6], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5210:
      std::memcpy(&this->m_mercenaryStats[7], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5211:
      std::memcpy(&this->m_mercenaryHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5212:
      std::memcpy(&this->m_mercenaryMaxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5213:
      std::memcpy(&this->m_mercenaryClass, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5214:
      std::memcpy(&this->m_mercenaryLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5215:
      std::memcpy(&this->m_mercenarySp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5216:
      std::memcpy(&this->m_mercenaryMaxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5217:
      std::memcpy(&this->m_mercenaryLoyalty, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5218:
      std::memcpy(&this->m_mercenarySummons, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5219:
      std::memcpy(&this->m_mercenaryKills, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5220:
      std::memcpy(&this->m_mercenaryAttackRange, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5221:
      std::memcpy(&this->m_mercenaryExpiration, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5222:
      std::memcpy(&this->mercenaryEnemyGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5223:
      std::memcpy(&this->mercenaryEnemyX, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5224:
      std::memcpy(&this->mercenaryEnemyY, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5225:
      std::memcpy(&this->moveMercenaryEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5226:
      std::memcpy(&this->selectMercenaryEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5227:
      std::memcpy(&this->attackMercenaryEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5228:
      std::memcpy(&this->mercenaryAlive, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5229:
      std::memcpy(&this->m_mercenaryActionState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5301:
      std::memcpy(&this->petGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5302:
      std::memcpy(&this->m_petNameModified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5303:
      std::memcpy(this->petName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5304:
      std::memcpy(&this->petAccessory, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5305:
      std::memcpy(&this->petJob, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5306:
      std::memcpy(&this->petLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5307:
      std::memcpy(&this->petHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5308:
      std::memcpy(&this->m_petFriendly, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5309:
      std::memcpy(&this->petEggIndex, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5310:
      std::memcpy(&this->petPreviousHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5401:
      std::memcpy(&this->elementalGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5402:
      std::memcpy(&this->elementalHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5403:
      std::memcpy(&this->elementalSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5404:
      std::memcpy(&this->elementalMaxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5405:
      std::memcpy(&this->elementalMaxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E48100
// FUNCTION: fn_24217
std::uint32_t CSession::ReadReplayQuests(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (3500 != header.id) {
    return 0xFFFFFFFF;
  }

  g_renewQuestMgr->m_ongoingQuests.clear();
  g_renewQuestMgr->SetParameter(3502, 0x18);
  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 3501:
      return offset;
    case 3502: {
      int parameter;
      std::memcpy(&parameter, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_renewQuestMgr->SetParameter(3502, parameter);
      break;
    }
    case 3510: {
      const std::uint32_t bytes = header.bytes;
      char* buffer = new char[bytes];
      std::memcpy(buffer, data.data() + offset, bytes);
      offset += bytes;
      g_renewQuestMgr->ReadOngoingQuests(buffer, bytes);
      delete buffer;
      break;
    }
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00E482B0
// FUNCTION: fn_24218
std::uint32_t CSession::ReadReplayShortcuts(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (7000 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 7001:
      return offset;
    case 7100:
      g_session.ClearShortcutSlots();
      std::memcpy(this->m_shortcutSlots, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 7101:
      g_session.ClearQuickSlots();
      std::memcpy(&this->quickSlot, data.data() + offset, header.bytes);
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
    g_session.m_homunculusAutoFeed = false;
    message = 3283;
  }

  const char* text = LookupMessage(message);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E48980
// FUNCTION: fn_24223
void CSession::SetPetAutoFeed(bool enabled) {
  g_session.m_petAutoFeed = enabled == true;
  const char* text = LookupMessage(enabled ? 2579 : 2580);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E489C0
// FUNCTION: fn_24224
void CSession::SetPetInfoAlert(bool enabled) {
  std::string notice = LookupMessage(enabled ? 4357 : 4379);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice.c_str()), 0x64FFFF, 0, 0, 0);
  g_serviceOptions.Set(0x128, enabled);
  g_windowMgr.RefreshGameSettingOption(0x128);
}

// OFFSET: 0x00E48B00
// FUNCTION: fn_24225
void CSession::CollectCardItems() {
  this->m_cardItems.clear();
  for (std::list<ITEM_INFO>::iterator it = this->m_inventoryItems.begin(); it != this->m_inventoryItems.end(); ++it) {
    if (it->identity[0] == 6) {
      this->m_cardItems.push_back(*it);
    }
  }
}

// OFFSET: 0x00E48B70
// FUNCTION: fn_24226
void RequestBasicInfoRefresh() {
  if (g_windowMgr.equipWnd != nullptr) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_SET_DISABLE, 575, 0, 0, 0);
  }
}

// OFFSET: 0x00E49290
// FUNCTION: fn_24232
void CSession::SetElementalGid(int value) {
  this->elementalGid = value;
  UIWindow* window = g_windowMgr.LookupWindow(WID_SKILLDELAYINFO);
  if (window != nullptr) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
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
  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (6500 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 6501:
      return offset;
    case 6511: {
      int queueId = 0;
      std::memcpy(&queueId, data.data() + offset, header.bytes);
      offset += header.bytes;
      const std::map<unsigned int, EntryQueueInfo*>::iterator end = g_entryQueueList.end();
      for (std::map<unsigned int, EntryQueueInfo*>::iterator it = g_entryQueueList.begin(); it != end; ++it) {
        if (queueId == it->second->id) {
          g_entryQueueMgr->current = it->second;
        }
      }
      break;
    }
    case 6512: {
      unsigned char ready = 1;
      std::memcpy(&ready, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->ready = ready;
      break;
    }
    case 6513: {
      char queueName[0x30];
      std::memset(queueName, 0, sizeof(queueName));
      std::memcpy(queueName, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->SetQueueName(*reinterpret_cast<const EntryQueueName*>(queueName));
      break;
    }
    case 6514: {
      char characterName[0x30];
      std::memset(characterName, 0, sizeof(characterName));
      std::memcpy(characterName, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->SetCharacterName(*reinterpret_cast<const EntryQueueName*>(characterName));
      break;
    }
    case 6515: {
      int value;
      std::memcpy(&value, data.data() + offset, header.bytes);
      offset += header.bytes;
      UIEntryQueueWnd* window = dynamic_cast<UIEntryQueueWnd*>(g_windowMgr.LookupWindow(WID_JOINTO_BATTLEFIELDWND));
      if (window) {
        window->SendMsg(nullptr, UM_LIST_SELECTION, 0, value, 0, 0);
      }
      break;
    }
    case 6517:
      std::memcpy(&waiting, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 6518:
      std::memcpy(&ranking, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 6516: {
      char queueText[100];
      std::memset(queueText, 0, sizeof(queueText));
      std::memcpy(queueText, data.data() + offset, header.bytes);
      offset += header.bytes;
      UIEntryQueueStandByWnd* window = dynamic_cast<UIEntryQueueStandByWnd*>(g_windowMgr.LookupWindow(WID_ENTRYQUEUESTANDBYWND));
      if (window) {
        window->SetInfo(queueText, waiting, ranking);
      }
      break;
    }
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

  ReplayBlockHeader header;
  header.Read(data.data());
  std::uint32_t offset = 6;
  if (1000 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 1001:
      return offset;
    case 1010:
      std::memcpy(&this->m_aid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1011:
      std::memcpy(&this->m_gid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1012:
      std::memcpy(&this->m_navigationPlayerMap[0x14], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1013:
      std::memcpy(&this->m_navigationPlayerMap[0x18], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1014:
      std::memcpy(&this->m_job, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1015:
      std::memcpy(&this->m_exp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1016:
      std::memcpy(&this->m_level, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1017:
      std::memcpy(&this->m_statusPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1018:
      std::memcpy(&this->m_nextExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1019:
      std::memcpy(&this->m_jobLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1020:
      std::memcpy(&this->m_skillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1021:
      std::memcpy(&this->m_guildSkillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1022:
      std::memcpy(&this->m_homunSkillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1023:
      std::memcpy(&this->m_mercenarySkillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1024:
      std::memcpy(&this->m_standardStats[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1025:
      std::memcpy(&this->m_standardStats[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1026:
      std::memcpy(&this->m_standardStats[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1027:
      std::memcpy(&this->m_standardStats[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1028:
      std::memcpy(&this->m_standardStats[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1029:
      std::memcpy(&this->m_standardStats[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1030:
      std::memcpy(&this->m_statusPointCost[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1031:
      std::memcpy(&this->m_statusPointCost[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1032:
      std::memcpy(&this->m_statusPointCost[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1033:
      std::memcpy(&this->m_statusPointCost[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1034:
      std::memcpy(&this->m_statusPointCost[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1035:
      std::memcpy(&this->m_statusPointCost[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1036:
      std::memcpy(&this->m_aspd, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1037:
      std::memcpy(&this->m_atk1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1038:
      std::memcpy(&this->m_mdef1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1039:
      std::memcpy(&this->m_sp36, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1040:
      std::memcpy(&this->m_def1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1041:
      std::memcpy(&this->m_def2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1042:
      std::memcpy(&this->m_atk2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1043:
      std::memcpy(&this->m_matk1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1044:
      std::memcpy(&this->m_matk2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1045:
      std::memcpy(&this->m_mdef2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1046:
      std::memcpy(&this->m_hit, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1047:
      std::memcpy(&this->m_flee1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1048:
      std::memcpy(&this->m_critical, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1049:
      std::memcpy(&this->m_flee2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1050:
      std::memcpy(&this->equippedArrowIndex, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1051:
      std::memcpy(&this->m_zeny, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1052:
      std::memcpy(&this->m_moveSpeed, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1053:
      std::memcpy(&this->m_manner, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1054:
      std::memcpy(&this->m_nextJobExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1055:
      std::memcpy(&this->m_jobExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1056:
      std::memcpy(&this->m_karma, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1057:
      std::memcpy(&this->monsterInfoEnabled, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1058:
      std::memcpy(&this->m_clientTimeOffset, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1059:
      std::memcpy(&this->m_characterFlags, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1060:
      std::memcpy(&this->m_head, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1061:
      std::memcpy(&this->m_weaponLook, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1062:
      std::memcpy(&this->m_shield, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1063:
      std::memcpy(&this->m_bodyStyle, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1064:
      std::memcpy(&this->m_headPalette, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1065:
      std::memcpy(&this->m_accessory, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1066:
      std::memcpy(&this->m_accessory2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1067:
      std::memcpy(&this->m_accessory3, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1068:
      std::memcpy(&this->m_bodyState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1069:
      std::memcpy(&this->m_effectState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1070:
      std::memcpy(&this->m_jobFlags, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1071:
      std::memcpy(&this->m_robe, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1072:
      std::memcpy(&this->m_equipViewLook[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1073:
      std::memcpy(&this->m_equipViewLook[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1074:
      std::memcpy(&this->m_equipViewLook[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1075:
      std::memcpy(&this->m_equipViewLook[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1076:
      std::memcpy(&this->m_equipViewLook[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1077:
      std::memcpy(&this->m_equipViewLook[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1078:
      std::memcpy(&this->m_equipViewLook[6], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1079:
      std::memcpy(&this->m_equipViewLook[7], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1080:
      std::memcpy(&this->m_equipViewLook[8], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1081:
      std::memcpy(this->m_equipViewName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1082:
      std::memcpy(this->playerTeam, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1083:
      std::memcpy(&this->m_score1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1084:
      std::memcpy(&this->m_score2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1085:
      std::memcpy(&this->chatFont, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1086:
      std::memcpy(&this->cartCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1087:
      std::memcpy(&this->cartMaxCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1088:
      std::memcpy(&this->cartWeight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1089:
      std::memcpy(&this->cartMaxWeight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1090:
      std::memcpy(&this->m_vendingSlotCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1091:
      std::memcpy(&this->m_buyingStoreSlotCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1092:
      std::memcpy(&this->m_chatTarget, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1093:
      std::memcpy(&this->unknown_549C, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1094:
      std::memcpy(&this->partyExpShare, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1095:
      std::memcpy(&this->partyItemCollect, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1096:
      std::memcpy(&this->partyItemDistribute, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1097:
      this->storeTitle.resize(header.bytes);
      std::memcpy(this->storeTitle.data(), data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1099:
      std::memcpy(&this->adoptionSkillAvailable, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1100:
      std::memcpy(&this->m_bodyCostume, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1101:
      std::memcpy(&this->extraInventorySlots, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00DE93F0
// FUNCTION: fn_23500
void CSession::ResetMercenaryStats() {
  this->moveMercenaryEnemy = 0;
  this->selectMercenaryEnemy = 0;
  this->attackMercenaryEnemy = 0;
  this->mercenaryEnemyGid = 0;
  this->mercenaryEnemyX = 0;
  this->mercenaryEnemyY = 0;
}
