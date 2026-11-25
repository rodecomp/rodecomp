#include "Contents/ClientState.h"
#include "Contents/Jobs/JobId.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/ReplayChunkTag.h"
#include "Contents/Actors/ActorEnums.h"

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
#include "UI/Items/UIEquipWnd.h"

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
  if (REPLAY_CHUNK_STATS_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_STATS_END:
      this->InvalidateWindowFrames();
      return offset;
    case REPLAY_CHUNK_STATS_STRENGTH_BONUS:
      std::memcpy(&this->strengthBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_AGILITY_BONUS:
      std::memcpy(&this->agilityBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_VITALITY_BONUS:
      std::memcpy(&this->vitalityBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_INTELLIGENCE_BONUS:
      std::memcpy(&this->intelligenceBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_DEXTERITY_BONUS:
      std::memcpy(&this->dexterityBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_LUCK_BONUS:
      std::memcpy(&this->luckBonus, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_STORE_ITEM_LIMIT:
      std::memcpy(&this->storeItemLimit, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_WEIGHT:
      std::memcpy(&this->m_weight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_MAX_WEIGHT:
      std::memcpy(&this->m_maxWeight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_A:
      std::memcpy(&this->randomStatIndexA, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_B:
      std::memcpy(&this->randomStatIndexB, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_C:
      std::memcpy(&this->randomStatIndexC, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_D:
      std::memcpy(&this->randomStatIndexD, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_E:
      std::memcpy(&this->randomStatIndexE, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_RANDOM_STAT_VALUES:
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
    case REPLAY_CHUNK_STATS_HP:
      std::memcpy(&this->hp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_MAX_HP:
      std::memcpy(&this->maxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_SP:
      std::memcpy(&this->m_sp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_MAX_SP:
      std::memcpy(&this->m_maxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_PROHIBITION_TARGET:
      std::memcpy(&this->m_prohibitionTarget, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_QUIT_BLOCKED:
      std::memcpy(&this->quitBlocked, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_BATTLE_CHAT:
      std::memcpy(&this->battleChat, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_QUIT_ALLOWED:
      std::memcpy(&this->quitAllowed, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_SESSION_FLAG:
      std::memcpy(&this->sessionFlag, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_ACCOUNT_NAME:
      this->m_accountName.resize(header.bytes);
      std::memcpy(this->m_accountName.data(), data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_STATS_MADOGEAR_TYPE: {
      std::memcpy(&this->madogearType, data.data() + offset, header.bytes);
      offset += header.bytes;
      CPlayer* player = g_modeMgr.GetGameMode()->scene->player;
      if (player) {
        player->SetMadogearType(this->madogearType);
        if (this->madogearType) {
          player->SendMsg(nullptr, AM_MAKE_CHICKEN, 0, 0, 0, 0, 0, 0);
        } else {
          player->SendMsg(nullptr, AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
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
  if (REPLAY_CHUNK_MESSENGER_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_ITEMS_END:
      return offset;
    case REPLAY_CHUNK_MESSENGER_SESSION_FLAG:
      std::memcpy(&this->replaySessionFlag, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MESSENGER_PARTY_BLOCKED:
      std::memcpy(&this->partyBlocked, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MESSENGER_PARTY_LEADER:
      std::memcpy(&this->partyLeader, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MESSENGER_PARTY_REQUEST_NAME: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->partyRequestName = buffer;
      break;
    }
    case REPLAY_CHUNK_MESSENGER_PARTY_NAME: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->partyName = buffer;
      break;
    }
    case REPLAY_CHUNK_MESSENGER_PARTY_MEMBERS:
      offset = ReadMessengerMembers(this->m_partyMembers, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_MESSENGER_FRIENDS:
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
  if (REPLAY_CHUNK_ITEMS_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_ITEMS_END:
    case 4703:
      return offset;
    case REPLAY_CHUNK_ITEMS_EXCHANGE_MONEY:
      std::memcpy(&this->exchangeMoney, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_ITEMS_OTHER_EXCHANGE_MONEY:
      std::memcpy(&this->otherExchangeMoney, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_ITEMS_INVENTORY:
      offset = ReadItemList(this->m_inventoryItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_SHOP:
      offset = ReadItemList(this->m_shopItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_PURCHASE:
      offset = ReadItemList(this->m_purchaseItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_SELL:
      offset = ReadItemList(this->m_sellItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_STORAGE:
      offset = ReadItemList(this->m_storageItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_CART:
      offset = ReadItemList(this->m_cartItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_MERCHANT_SHOP:
      offset = ReadItemList(this->m_merchantShopItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_MERCHANT_MIRROR:
      offset = ReadItemList(this->m_merchantMirrorItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_MERCHANT_MY_SHOP:
      offset = ReadItemList(this->m_merchantMyShopItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_MERCHANT_PURCHASE:
      offset = ReadItemList(this->m_merchantPurchaseItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_MERCHANT_HOPE_TO_SELL:
      offset = ReadItemList(this->m_merchantHopeToSellItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_MERCHANT_PURCHASE_SAVED:
      offset = ReadItemList(this->m_merchantPurchaseSavedItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_FILTERED_INVENTORY:
      offset = ReadItemList(this->m_filteredInventoryItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_EQUIPPED:
      offset = ReadItemArray(this->equippedItems.data(), offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_ALTERNATE_EQUIPPED:
      offset = ReadItemArray(this->alternateEquippedItems.data(), offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_COSTUME:
      offset = ReadItemArray(this->costumeItems.data(), offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_ALTERNATE_COSTUME:
      offset = ReadItemArray(this->alternateCostumeItems.data(), offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_EXCHANGE:
      offset = ReadItemArray(this->m_exchangeItems, offset, header.bytes, data);
      break;
    case REPLAY_CHUNK_ITEMS_SLOTS:
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
  if (REPLAY_CHUNK_SHOPS_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_SHOPS_END:
      return offset;
    case REPLAY_CHUNK_SHOPS_NAME: {
      ReplayShopNameRecord record{};
      std::memcpy(&record, data.data() + offset, header.bytes);
      offset += header.bytes;
      std::string name(record.name);
      this->AddShopName(name, record.shopId);
      break;
    }
    case REPLAY_CHUNK_SHOPS_ITEM: {
      ReplayShopItemRecord record;
      std::memcpy(&record, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_session.SetShopItem(record.shopId, record.index, record.itemId, record.first, record.second);
      break;
    }
    case REPLAY_CHUNK_SHOPS_FLAG: {
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
  if (REPLAY_CHUNK_MEMORIAL_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    bool done = false;
    switch (header.id) {
    case REPLAY_CHUNK_MEMORIAL_END:
      done = true;
      break;
    case REPLAY_CHUNK_MEMORIAL_WINDOW_OPEN:
      std::memcpy(&this->memorialWindowOpen, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MEMORIAL_WINDOW_X:
      std::memcpy(&this->memorialWindowX, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MEMORIAL_WINDOW_Y:
      std::memcpy(&this->memorialWindowY, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MEMORIAL_DUNGEON_STATE:
      std::memcpy(&this->memorialDungeonState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 5514:
      std::memcpy(&this->memorialReplayValue, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MEMORIAL_DUNGEON_NAME: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->memorialDungeonName = buffer;
      break;
    }
    case REPLAY_CHUNK_MEMORIAL_QUEUE_POSITION:
      std::memcpy(&this->memorialQueuePosition, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MEMORIAL_CREATION_TIME:
      std::memcpy(&this->memorialCreationTime, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_MEMORIAL_EXPIRATION_TIME:
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
  if (REPLAY_CHUNK_COMPANIONS_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_COMPANIONS_END:
      return offset;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_AID:
      std::memcpy(&this->m_homunAid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_NAME:
      std::memcpy(this->m_homunName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_0:
      std::memcpy(&this->m_homunStats[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_1:
      std::memcpy(&this->m_homunStats[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_2:
      std::memcpy(&this->m_homunStats[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_3:
      std::memcpy(&this->m_homunStats[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_4:
      std::memcpy(&this->m_homunStats[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_5:
      std::memcpy(&this->m_homunStats[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_6:
      std::memcpy(&this->m_homunStats[6], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_7:
      std::memcpy(&this->m_homunStats[7], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_JOB:
      std::memcpy(&this->m_homunJob, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_LEVEL:
      std::memcpy(&this->m_homunLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_HP:
      std::memcpy(&this->m_homunHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_HP:
      std::memcpy(&this->m_homunMaxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_SP:
      std::memcpy(&this->m_homunSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_SP:
      std::memcpy(&this->homunMaxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_FRIENDLY:
      std::memcpy(&this->homunFriendly, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ACCESSORY:
      std::memcpy(&this->homunAccessory, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_EXP:
      std::memcpy(&this->m_homunExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_EXP:
      std::memcpy(&this->m_homunMaxExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_HUNGER:
      std::memcpy(&this->homunHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_HUNGER:
      std::memcpy(&this->homunMaxHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_NAME_MODIFIED:
      std::memcpy(&this->homunNameModified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ENEMY_GID:
      std::memcpy(&this->homunEnemyGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ENEMY_X:
      std::memcpy(&this->homunEnemyX, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ENEMY_Y:
      std::memcpy(&this->homunEnemyY, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_MOVE_ENEMY:
      std::memcpy(&this->moveHomunEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_SELECT_ENEMY:
      std::memcpy(&this->selectHomunEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ATTACK_ENEMY:
      std::memcpy(&this->attackHomunEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ALIVE:
      std::memcpy(&this->homunAlive, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_TRACE_AI:
      std::memcpy(&this->m_traceAi, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ACTION_STATE:
      std::memcpy(&this->m_homunActionState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_HUNGER_NOTIFIED:
      std::memcpy(&this->m_homunHungerNotified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_HOMUN_ATTACK_RANGE:
      std::memcpy(&this->m_homunAttackRange, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_AID:
      std::memcpy(&this->m_mercenaryAid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_NAME:
      std::memcpy(this->m_mercenaryName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_0:
      std::memcpy(&this->m_mercenaryStats[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_1:
      std::memcpy(&this->m_mercenaryStats[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_2:
      std::memcpy(&this->m_mercenaryStats[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_3:
      std::memcpy(&this->m_mercenaryStats[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_4:
      std::memcpy(&this->m_mercenaryStats[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_5:
      std::memcpy(&this->m_mercenaryStats[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_6:
      std::memcpy(&this->m_mercenaryStats[6], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_7:
      std::memcpy(&this->m_mercenaryStats[7], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_HP:
      std::memcpy(&this->m_mercenaryHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_MAX_HP:
      std::memcpy(&this->m_mercenaryMaxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_CLASS:
      std::memcpy(&this->m_mercenaryClass, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_LEVEL:
      std::memcpy(&this->m_mercenaryLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_SP:
      std::memcpy(&this->m_mercenarySp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_MAX_SP:
      std::memcpy(&this->m_mercenaryMaxSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_LOYALTY:
      std::memcpy(&this->m_mercenaryLoyalty, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_SUMMONS:
      std::memcpy(&this->m_mercenarySummons, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_KILLS:
      std::memcpy(&this->m_mercenaryKills, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ATTACK_RANGE:
      std::memcpy(&this->m_mercenaryAttackRange, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_EXPIRATION:
      std::memcpy(&this->m_mercenaryExpiration, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ENEMY_GID:
      std::memcpy(&this->mercenaryEnemyGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ENEMY_X:
      std::memcpy(&this->mercenaryEnemyX, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ENEMY_Y:
      std::memcpy(&this->mercenaryEnemyY, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_MOVE_ENEMY:
      std::memcpy(&this->moveMercenaryEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_SELECT_ENEMY:
      std::memcpy(&this->selectMercenaryEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ATTACK_ENEMY:
      std::memcpy(&this->attackMercenaryEnemy, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ALIVE:
      std::memcpy(&this->mercenaryAlive, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_MERCENARY_ACTION_STATE:
      std::memcpy(&this->m_mercenaryActionState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_GID:
      std::memcpy(&this->petGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_NAME_MODIFIED:
      std::memcpy(&this->m_petNameModified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_NAME:
      std::memcpy(this->petName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_ACCESSORY:
      std::memcpy(&this->petAccessory, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_JOB:
      std::memcpy(&this->petJob, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_LEVEL:
      std::memcpy(&this->petLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_HUNGER:
      std::memcpy(&this->petHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_FRIENDLY:
      std::memcpy(&this->m_petFriendly, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_EGG_INDEX:
      std::memcpy(&this->petEggIndex, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_PET_PREVIOUS_HUNGER:
      std::memcpy(&this->petPreviousHunger, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_ELEMENTAL_GID:
      std::memcpy(&this->elementalGid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_ELEMENTAL_HP:
      std::memcpy(&this->elementalHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_ELEMENTAL_SP:
      std::memcpy(&this->elementalSp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_ELEMENTAL_MAX_HP:
      std::memcpy(&this->elementalMaxHp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_COMPANIONS_ELEMENTAL_MAX_SP:
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
  if (REPLAY_CHUNK_QUESTS_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  g_renewQuestMgr->m_ongoingQuests.clear();
  g_renewQuestMgr->SetParameter(REPLAY_CHUNK_QUESTS_PARAMETER, 0x18);
  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_QUESTS_END:
      return offset;
    case REPLAY_CHUNK_QUESTS_PARAMETER: {
      int parameter;
      std::memcpy(&parameter, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_renewQuestMgr->SetParameter(REPLAY_CHUNK_QUESTS_PARAMETER, parameter);
      break;
    }
    case REPLAY_CHUNK_QUESTS_ONGOING: {
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
  if (REPLAY_CHUNK_SHORTCUTS_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_SHORTCUTS_END:
      return offset;
    case REPLAY_CHUNK_SHORTCUTS_SLOTS:
      g_session.ClearShortcutSlots();
      std::memcpy(this->m_shortcutSlots, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_SHORTCUTS_QUICK_SLOT:
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

// INLINED
static inline bool IsSecondJobId(int job) {
  return job >= JT_RUNE_KNIGHT_2ND && job < JT_SECOND_JOB_END;
}

// OFFSET: 0x00E488C0
// FUNCTION: fn_24221
void CSession::UpdateBodyStyle(int job, int style, int renderJob) {
  const bool upperJob = job >= JT_NOVICE_H && job <= JT_CHICKEN2_H;
  if (style >= 2 || IsSecondJobId(renderJob)) {
    this->m_bodyStyle = style;
  } else if (upperJob) {
    this->m_bodyStyle = job <= JT_THIEF_H;
  } else {
    const int bodyJob = g_session.GetBodyStyleJob(job);
    this->m_bodyStyle = bodyJob >= JT_NOVICE_H && bodyJob <= JT_THIEF_H;
  }
}

// OFFSET: 0x00E48930
// FUNCTION: fn_24222
void CSession::SetHomunculusAutoFeed(bool enabled) {
  int message;
  if (enabled == true) {
    this->m_homunculusAutoFeed = true;
    message = MSI_HOMUN_SELF_FEEDING_ON;
  } else {
    g_session.m_homunculusAutoFeed = false;
    message = MSI_HOMUN_SELF_FEEDING_OFF;
  }

  const char* text = LookupMessage(message);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E48980
// FUNCTION: fn_24223
void CSession::SetPetAutoFeed(bool enabled) {
  int message;
  if (enabled == true) {
    g_session.m_petAutoFeed = true;
    message = MSI_PET_AUTO_FEEDING_ON;
  } else {
    g_session.m_petAutoFeed = false;
    message = MSI_PET_AUTO_FEEDING_OFF;
  }

  const char* text = LookupMessage(message);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E489C0
// FUNCTION: fn_24224
void CSession::SetPetInfoAlert(bool enabled) {
  std::string notice = LookupMessage(enabled ? 4357 : 4379);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice.c_str()), 0x64FFFF, 0, 0, 0);
  g_serviceOptions.Set(TT_PETINFO_ALERT_ONOFF, enabled);
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
void CSession::RequestBasicInfoRefresh() {
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
  if (REPLAY_CHUNK_ENTRY_QUEUE_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_ENTRY_QUEUE_END:
      return offset;
    case REPLAY_CHUNK_ENTRY_QUEUE_ID: {
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
    case REPLAY_CHUNK_ENTRY_QUEUE_READY: {
      unsigned char ready = 1;
      std::memcpy(&ready, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->ready = ready;
      break;
    }
    case REPLAY_CHUNK_ENTRY_QUEUE_NAME: {
      char queueName[0x30];
      std::memset(queueName, 0, sizeof(queueName));
      std::memcpy(queueName, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->SetQueueName(queueName);
      break;
    }
    case REPLAY_CHUNK_ENTRY_QUEUE_CHARACTER_NAME: {
      char characterName[0x30];
      std::memset(characterName, 0, sizeof(characterName));
      std::memcpy(characterName, data.data() + offset, header.bytes);
      offset += header.bytes;
      g_entryQueueMgr->SetCharacterName(characterName);
      break;
    }
    case REPLAY_CHUNK_ENTRY_QUEUE_SELECTION: {
      int value;
      std::memcpy(&value, data.data() + offset, header.bytes);
      offset += header.bytes;
      UIEntryQueueWnd* window = dynamic_cast<UIEntryQueueWnd*>(g_windowMgr.LookupWindow(WID_JOINTO_BATTLEFIELDWND));
      if (window) {
        window->SendMsg(nullptr, UM_LIST_SELECTION, 0, value, 0, 0);
      }
      break;
    }
    case REPLAY_CHUNK_ENTRY_QUEUE_WAITING:
      std::memcpy(&waiting, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_ENTRY_QUEUE_RANKING:
      std::memcpy(&ranking, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_ENTRY_QUEUE_STAND_BY_NAME: {
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
  if (REPLAY_CHUNK_CHARACTER_BEGIN != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case REPLAY_CHUNK_CHARACTER_END:
      return offset;
    case REPLAY_CHUNK_CHARACTER_AID:
      std::memcpy(&this->m_aid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_GID:
      std::memcpy(&this->m_gid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1012:
      std::memcpy(&this->unknown_1620[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1013:
      std::memcpy(&this->unknown_1620[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_JOB:
      std::memcpy(&this->m_job, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EXP:
      std::memcpy(&this->m_exp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_LEVEL:
      std::memcpy(&this->m_level, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT:
      std::memcpy(&this->m_statusPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_NEXT_EXP:
      std::memcpy(&this->m_nextExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_JOB_LEVEL:
      std::memcpy(&this->m_jobLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_SKILL_POINT:
      std::memcpy(&this->m_skillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_GUILD_SKILL_POINT:
      std::memcpy(&this->m_guildSkillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_HOMUN_SKILL_POINT:
      std::memcpy(&this->m_homunSkillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MERCENARY_SKILL_POINT:
      std::memcpy(&this->m_mercenarySkillPoint, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STANDARD_STAT_0:
      std::memcpy(&this->m_standardStats[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STANDARD_STAT_1:
      std::memcpy(&this->m_standardStats[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STANDARD_STAT_2:
      std::memcpy(&this->m_standardStats[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STANDARD_STAT_3:
      std::memcpy(&this->m_standardStats[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STANDARD_STAT_4:
      std::memcpy(&this->m_standardStats[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STANDARD_STAT_5:
      std::memcpy(&this->m_standardStats[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_0:
      std::memcpy(&this->m_statusPointCost[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_1:
      std::memcpy(&this->m_statusPointCost[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_2:
      std::memcpy(&this->m_statusPointCost[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_3:
      std::memcpy(&this->m_statusPointCost[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_4:
      std::memcpy(&this->m_statusPointCost[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_5:
      std::memcpy(&this->m_statusPointCost[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ASPD:
      std::memcpy(&this->m_aspd, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ATK1:
      std::memcpy(&this->m_atk1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MDEF1:
      std::memcpy(&this->m_mdef1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1039:
      std::memcpy(&this->m_sp36, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_DEF1:
      std::memcpy(&this->m_def1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_DEF2:
      std::memcpy(&this->m_def2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ATK2:
      std::memcpy(&this->m_atk2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MATK1:
      std::memcpy(&this->m_matk1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MATK2:
      std::memcpy(&this->m_matk2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MDEF2:
      std::memcpy(&this->m_mdef2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_HIT:
      std::memcpy(&this->m_hit, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_FLEE1:
      std::memcpy(&this->m_flee1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CRITICAL:
      std::memcpy(&this->m_critical, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_FLEE2:
      std::memcpy(&this->m_flee2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIPPED_ARROW_INDEX:
      std::memcpy(&this->equippedArrowIndex, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ZENY:
      std::memcpy(&this->m_zeny, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MOVE_SPEED:
      std::memcpy(&this->m_moveSpeed, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MANNER:
      std::memcpy(&this->m_manner, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_NEXT_JOB_EXP:
      std::memcpy(&this->m_nextJobExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_JOB_EXP:
      std::memcpy(&this->m_jobExp, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_KARMA:
      std::memcpy(&this->m_karma, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_MONSTER_INFO_ENABLED:
      std::memcpy(&this->monsterInfoEnabled, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CLIENT_TIME_OFFSET:
      std::memcpy(&this->m_clientTimeOffset, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CHARACTER_FLAGS:
      std::memcpy(&this->m_characterFlags, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_HEAD:
      std::memcpy(&this->m_head, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_WEAPON_LOOK:
      std::memcpy(&this->m_weaponLook, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_SHIELD:
      std::memcpy(&this->m_shield, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_BODY_STYLE:
      std::memcpy(&this->m_bodyStyle, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_HEAD_PALETTE:
      std::memcpy(&this->m_headPalette, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ACCESSORY:
      std::memcpy(&this->m_accessory, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ACCESSORY2:
      std::memcpy(&this->m_accessory2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ACCESSORY3:
      std::memcpy(&this->m_accessory3, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_BODY_STATE:
      std::memcpy(&this->m_bodyState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EFFECT_STATE:
      std::memcpy(&this->m_effectState, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_JOB_FLAGS:
      std::memcpy(&this->m_jobFlags, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ROBE:
      std::memcpy(&this->m_robe, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_0:
      std::memcpy(&this->m_equipViewLook[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_1:
      std::memcpy(&this->m_equipViewLook[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_2:
      std::memcpy(&this->m_equipViewLook[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_3:
      std::memcpy(&this->m_equipViewLook[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_4:
      std::memcpy(&this->m_equipViewLook[4], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_5:
      std::memcpy(&this->m_equipViewLook[5], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_6:
      std::memcpy(&this->m_equipViewLook[6], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_7:
      std::memcpy(&this->m_equipViewLook[7], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_8:
      std::memcpy(&this->m_equipViewLook[8], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_NAME:
      std::memcpy(this->m_equipViewName, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_PLAYER_TEAM:
      std::memcpy(&this->playerTeam, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_SCORE1:
      std::memcpy(&this->m_score1, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_SCORE2:
      std::memcpy(&this->m_score2, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CHAT_FONT:
      std::memcpy(&this->chatFont, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CART_COUNT:
      std::memcpy(&this->cartCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CART_MAX_COUNT:
      std::memcpy(&this->cartMaxCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CART_WEIGHT:
      std::memcpy(&this->cartWeight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CART_MAX_WEIGHT:
      std::memcpy(&this->cartMaxWeight, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_VENDING_SLOT_COUNT:
      std::memcpy(&this->m_vendingSlotCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_BUYING_STORE_SLOT_COUNT:
      std::memcpy(&this->m_buyingStoreSlotCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_CHAT_TARGET:
      std::memcpy(&this->m_chatTarget, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 1093:
      std::memcpy(&this->m_replayValue1093, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_PARTY_EXP_SHARE:
      std::memcpy(&this->partyExpShare, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_PARTY_ITEM_COLLECT:
      std::memcpy(&this->partyItemCollect, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_PARTY_ITEM_DISTRIBUTE:
      std::memcpy(&this->partyItemDistribute, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_STORE_TITLE:
      this->storeTitle.resize(header.bytes);
      std::memcpy(this->storeTitle.data(), data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_ADOPTION_SKILL_AVAILABLE:
      std::memcpy(&this->adoptionSkillAvailable, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_BODY_COSTUME:
      std::memcpy(&this->m_bodyCostume, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case REPLAY_CHUNK_CHARACTER_EXTRA_INVENTORY_SLOTS:
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
