#include "Contents/ClientState.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
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
