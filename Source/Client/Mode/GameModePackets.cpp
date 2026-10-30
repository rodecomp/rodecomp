#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/World/World.h"
#include "Network/TextEncoding.h"
#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIBarGraph.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"

#include <direct.h>
#include <io.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

struct SessionMessengerContext {
  void UpdateRandomStats();
};

struct ClientItemLists {
  bool AddItem1738(const ITEM_INFO& item, unsigned char refresh);
};

namespace {
#pragma pack(push, 1)
  struct SimulationPacketHeader {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t length;
    /* 0004 */ std::uint32_t unknown_0004;
    /* 0008 */ int value;
    /* 000C */ unsigned char unknown_000C[0x18];
  };

  struct EnchantSimulationRecord {
    /* 0000 */ int itemId;
    /* 0004 */ int resultItemId;
    /* 0008 */ std::uint32_t unknown_0008[2];
    /* 0010 */ int rate;
    /* 0014 */ std::uint32_t unknown_0014;
    /* 0018 */ int count;
  };

  struct PackageSimulationRecord {
    /* 0000 */ int itemId;
    /* 0004 */ unsigned char refine;
    /* 0005 */ unsigned char packageType;
    /* 0006 */ int unknown_0006;
    /* 000A */ int unknown_000A;
    /* 000E */ char text[24];
    /* 0026 */ int unknown_0026;
    /* 002A */ int unknown_002A;
    /* 002E */ int count;
  };

  struct NamedProgressPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ unsigned char result;
    /* 0003 */ int first;
    /* 0007 */ int second;
    /* 000B */ std::uint16_t current;
    /* 000D */ std::uint16_t total;
    /* 000F */ unsigned char unknown_000F[4];
    /* 0013 */ char name[1];
  };

  struct ItemStoreListHeader {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t length;
    /* 0004 */ char title[24];
  };

  struct StoreItemOption {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t value;
    /* 0004 */ unsigned char param;
  };

  struct StoreEquipRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::uint32_t location;
    /* 000B */ std::uint32_t wearState;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ std::uint32_t expireTime;
    /* 0024 */ std::uint16_t bindOnEquip;
    /* 0026 */ std::uint16_t sprite;
    /* 0028 */ unsigned char flags;
  };

  struct StoreEquipOptionRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::uint32_t location;
    /* 000B */ std::uint32_t wearState;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ std::uint32_t expireTime;
    /* 0024 */ std::uint16_t bindOnEquip;
    /* 0026 */ std::uint16_t sprite;
    /* 0028 */ unsigned char optionCount;
    /* 0029 */ StoreItemOption options[5];
    /* 0042 */ unsigned char flags;
  };

  struct StoreNormalRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::int16_t amount;
    /* 0009 */ std::uint32_t location;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint32_t expireTime;
    /* 0021 */ unsigned char flags;
  };

  struct HomunculusProperty64Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t type;
    /* 0004 */ std::int64_t value;
  };

  struct HomunculusPropertyPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t type;
    /* 0004 */ int value;
  };
#pragma pack(pop)

  static_assert(sizeof(ItemStoreListHeader) == 0x1C);
  static_assert(sizeof(StoreEquipRecord) == 0x29);
  static_assert(sizeof(StoreEquipOptionRecord) == 0x43);
  static_assert(sizeof(StoreNormalRecord) == 0x22);

  // INLINED
  int CountStoreItemOptions(const StoreItemOption* options) {
    int count = 0;
    for (int index = 0; index < 5; ++index) {
      if (options[index].id != 0) {
        ++count;
      }
    }
    return count;
  }

  static_assert(sizeof(SimulationPacketHeader) == 0x24);
  static_assert(sizeof(EnchantSimulationRecord) == 0x1C);
  static_assert(sizeof(PackageSimulationRecord) == 0x32);
}

// OFFSET: 0x00D1F540
// FUNCTION: fn_21638
void CGameMode::IgnoreRemovedPacket3(const unsigned char*) {
}

// OFFSET: 0x00D208F0
// FUNCTION: fn_21649
void CGameMode::IgnoreRemovedPacket4(const unsigned char*) {
}

// OFFSET: 0x00D21210
// FUNCTION: fn_21653
void CGameMode::IgnoreRemovedPacket5(const unsigned char*) {
}

// OFFSET: 0x00D21D00
// FUNCTION: fn_21657
void CGameMode::ReceiveUnknown05C4Result(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const NamedProgressPacket*>(data);
  char message[256];
  char name[51] = {};
  std::sprintf(name, "%s", packet->name);
  this->unknown_05C4 = -1;
  this->unknown_05C8 = -1;

  switch (packet->result) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1223)), 0xFFFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(1223)), 0xFFFFFF, 0, 0, 0);
    return;
  case 1:
  case 2:
    this->unknown_05C4 = packet->first;
    this->unknown_05C8 = packet->second;
    if (packet->result != 2) {
      return;
    }
    std::sprintf(message, LookupMessage(1225), name);
    break;
  case 3:
    if (packet->current == 0 && packet->total == 1) {
      std::sprintf(message, LookupMessage(1261), name);
    } else {
      std::sprintf(message, LookupMessage(1224), name, packet->current, packet->total);
    }
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
}

// OFFSET: 0x00D230E0
// FUNCTION: fn_21660
void CGameMode::ReceiveColoredNotice(const unsigned char* packet) {
  std::string text;
  text.assign(reinterpret_cast<const char*>(packet + 9), *reinterpret_cast<const short*>(packet + 2) - 9);
  if (!text.empty()) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), *reinterpret_cast<const std::uint32_t*>(packet + 5), packet[4], 0, 0);
  }
}

// OFFSET: 0x00D24430
// FUNCTION: fn_21666
void CGameMode::ReceiveActorText(const unsigned char* packet) {
  char text[81];
  std::strncpy(text, reinterpret_cast<const char*>(packet + 6), 80);
  text[80] = '\0';
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
    actor->SendMsg(nullptr, 141, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D25920
// FUNCTION: fn_21674
void CGameMode::ReceiveRentalItemTime(const unsigned char* packet) {
  char number[12];
  char text[260];
  _itoa(*reinterpret_cast<const int*>(packet + 2), number, 10);
  const ITEM_DESC& description = FindItemDescription(std::atoi(number));
  const std::uint32_t seconds = *reinterpret_cast<const std::uint32_t*>(packet + 6);
  const char* name = description.identifiedName;
  const int minutes = static_cast<int>(seconds / 60);
  if (minutes > 0) {
    std::sprintf(text, LookupMessage(1256), name, minutes);
  } else {
    std::sprintf(text, LookupMessage(1270), name, seconds);
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  if (minutes == 1) {
    std::sprintf(text, LookupMessage(1257), name);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D26600
// FUNCTION: fn_21680
void CGameMode::ReceiveActorSequenceDeadline(const unsigned char* packet) {
  g_session.InsertPair(*reinterpret_cast<const int*>(packet + 2), *reinterpret_cast<const int*>(packet + 6));
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  if (aid == g_session.m_aid) {
    this->scene->player->SetSequenceDeadline(*reinterpret_cast<const std::uint32_t*>(packet + 6));
    return;
  }
  if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
    actor->SetSequenceDeadline(*reinterpret_cast<const std::uint32_t*>(packet + 6));
  }
}

// OFFSET: 0x00D27060
// FUNCTION: fn_21686
void CGameMode::ReceiveActorSignal(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    actor->SendMsg(nullptr, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D2AF90
// FUNCTION: fn_21707
void CGameMode::IgnorePacket(const unsigned char*) {
}

// OFFSET: 0x00D2B710
// FUNCTION: fn_21710
void CGameMode::ReceiveItem1738EquipList(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(33));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreEquipRecord);
  const StoreEquipRecord* record = reinterpret_cast<const StoreEquipRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i, ++record) {
    StoreItemOption options[5] = {};
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    *reinterpret_cast<std::uint32_t*>(item.unknown_0074) = record->sprite;
    *reinterpret_cast<unsigned char*>(item.unknown_0078) = (record->flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2BA50
// FUNCTION: fn_21711
void CGameMode::ReceiveItem1738EquipListOptions(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(33));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreEquipOptionRecord);
  const StoreEquipOptionRecord* record = reinterpret_cast<const StoreEquipOptionRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    *reinterpret_cast<std::uint32_t*>(item.unknown_0074) = record->sprite;
    *reinterpret_cast<unsigned char*>(item.unknown_0078) = (record->flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2BDF0
// FUNCTION: fn_21712
void CGameMode::ReceiveItem1738NormalList(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(33));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreNormalRecord);
  const StoreNormalRecord* record = reinterpret_cast<const StoreNormalRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[3] = record->location;
    item.unknown_0000[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = 0;
    item.unknown_0060 = 0;
    item.unknown_0068 = record->expireTime;
    item.unknown_0074[0] = (record->flags >> 1) & 1;
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2C190
// FUNCTION: fn_21713
void CGameMode::ReceiveHomunculusProperty64(const unsigned char* data) {
  if (!data) {
    return;
  }

  const HomunculusProperty64Packet* packet = reinterpret_cast<const HomunculusProperty64Packet*>(data);
  switch (packet->type) {
  case 1:
    g_session.m_homunExp = packet->value;
    g_session.RefreshHomunculusInfo();
    if (UIHomunInfoWnd* window = static_cast<UIHomunInfoWnd*>(g_windowMgr.window_03C0)) {
      window->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
    }
    break;
  case 5:
    g_session.m_homunHp = static_cast<int>(packet->value);
    g_session.RefreshHomunculusInfo();
    break;
  case 7:
    g_session.m_homunSp = static_cast<int>(packet->value);
    g_session.RefreshHomunculusInfo();
    break;
  }

  g_session.RefreshHomunculusInfo();
}

// OFFSET: 0x00D2C250
// FUNCTION: fn_21714
void CGameMode::ReceiveHomunculusProperty(const unsigned char* data) {
  const HomunculusPropertyPacket* packet = reinterpret_cast<const HomunculusPropertyPacket*>(data);
  switch (packet->type) {
  case 5:
    g_session.m_homunHp = packet->value;
    g_session.RefreshHomunculusInfo();
    break;
  case 7:
    g_session.m_homunSp = packet->value;
    g_session.RefreshHomunculusInfo();
    break;
  case 1:
    g_session.m_homunExp = packet->value;
    g_session.RefreshHomunculusInfo();
    break;
  }

  g_session.RefreshHomunculusInfo();
}

// OFFSET: 0x00D2C2B0
// FUNCTION: fn_21715
void CGameMode::ReceiveHuntingList(const unsigned char* packet) {
  char text[256];
  int size = *reinterpret_cast<const short*>(packet + 2) - 4;
  if (size > 5000) {
    size = 5000;
  }
  char* list = new char[size + 1];
  std::memcpy(list, packet + 4, size);
  list[size] = '\0';
  std::sprintf(text, "=========== HUNTING LIST =============");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(list), 0xCDFFCD, 0, 0, 0);
  std::sprintf(text, "========================================");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
}

// OFFSET: 0x00D2F150
// FUNCTION: fn_21728
bool CGameMode::NotifyItemResult(int result) {
  int message;
  switch (result) {
  case 1:
  case 6:
    message = 53;
    break;
  case 2:
    message = 52;
    break;
  case 4:
    message = 220;
    break;
  case 5:
    message = 279;
    break;
  case 7:
    message = 1418;
    break;
  case 9:
    message = 3984;
    break;
  default:
    return false;
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
  return true;
}

// OFFSET: 0x00D4B2C0
// FUNCTION: fn_21864
void CGameMode::ReceiveEquipAck(const unsigned char* packet) {
  switch (*reinterpret_cast<const short*>(packet + 2)) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(242)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(243)), 0xFFFF64, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(244)), 0xFFFF64, 0, 0, 0);
    break;
  case 3:
    switch (g_session.GetDisplayJob()) {
    case 12:
    case 4013:
    case 4035:
    case 4059:
    case 4065:
    case 4101:
    case 4254:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1040)), 0xFFFF64, 0, 0, 0);
      break;
    case 24:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1175)), 0xFFFF64, 0, 0, 0);
      break;
    case 25:
    case 4222:
      break;
    default:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(245)), 0xFFFF64, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D58970
// FUNCTION: fn_21955
std::string CGameMode::MakeSimulatorFolder(const std::string& folder) {
  std::string path = g_applicationDirectory + folder;
  if (_access(path.c_str(), 0) != 0 && _mkdir(path.c_str()) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("Create Folder Fail!!! %s", path.c_str()).c_str()), 0xFF, 0, 0, 0);
    return std::string();
  }
  return path;
}

// OFFSET: 0x00D58B20
// FUNCTION: fn_21956
std::FILE* CGameMode::OpenPackageSimulatorFile(const std::string& folder, int itemId, std::string& itemName) {
  std::string directory = this->MakeSimulatorFolder(folder);
  if (directory.size() == 0) {
    return nullptr;
  }

  itemName = FindItemDescription(itemId).identifiedName;
  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%s_%d(%d_%d_%d)", itemName.c_str(), itemId, time.wYear, time.wMonth, time.wDay);
  std::string path = FormatOwnedString("%s\\%s.txt", directory.c_str(), name.c_str());
  int index = 0;
  while (_access(path.c_str(), 0) == 0) {
    ++index;
    path = FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index);
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("Simulator Result File Create %s %d!!!!!!!!!!!!", file ? "success" : "Fail", itemId).c_str()),
                         file ? 0xFFFF : 0xFF, 0, 0, 0);
  return file;
}

// OFFSET: 0x00D58D80
// FUNCTION: fn_21957
std::FILE* CGameMode::OpenSimulatorFile(const std::string& folder, const std::string& name) {
  std::string directory = this->MakeSimulatorFolder(folder);
  if (directory.size() == 0) {
    return nullptr;
  }

  std::string path = FormatOwnedString("%s\\%s.txt", directory.c_str(), name.c_str());
  int index = 0;
  while (_access(path.c_str(), 0) == 0) {
    ++index;
    path = FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index);
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s Simulator File Create %s !!!!!!!!!!!!", name.c_str(), file ? "success" : "Fail").c_str()),
                         file ? 0xFFFF : 0xFF, 0, 0, 0);
  return file;
}

// OFFSET: 0x00D5F830
// FUNCTION: fn_21971
void CGameMode::ReceiveRandomUpgradeSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%d_EnchantUpgradeTest_Slot_%d(%d_%d_%d)", header->value / 10, header->value % 10, time.wYear, time.wMonth, time.wDay);
  std::FILE* file = this->OpenSimulatorFile("\\PackageItemTest", name);
  if (!file) {
    return;
  }

  const EnchantSimulationRecord* record = reinterpret_cast<const EnchantSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  int total = 0;
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(EnchantSimulationRecord));
  std::fprintf(file, "\n- %s\n\n", ToUtf8(name, g_textCodePage).c_str());
  std::fprintf(file, "============================\tSTART\t============================\n");
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.SetItemId(record->itemId);
    item.identified = 1;
    item.unknown_0000[0] = 0x15;
    ITEM_INFO result;
    result.SetItemId(record->resultItemId);
    result.identified = 1;
    result.unknown_0000[0] = 0x15;
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    std::fprintf(file, "\t\t%s\t%s\t%d\t\t%.5f%%\n", ToUtf8(naming.DisplayName(item, 0), g_textCodePage).c_str(),
                 ToUtf8(naming.DisplayName(result, 0), g_textCodePage).c_str(), record->count, static_cast<float>(record->rate) / 1000.0f);
    total += record->count;
    ++record;
  }
  std::fprintf(file, "Try\t%d\tRandomUpgrade\n", total);
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D5FC60
// FUNCTION: fn_21972
void CGameMode::ReceiveEnchantSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%d_EnchantTest_Slot_%d(%d_%d_%d)", header->value / 100, header->value / 10 % 10, time.wYear, time.wMonth, time.wDay);
  std::FILE* file = this->OpenSimulatorFile("\\PackageItemTest", name);
  if (!file) {
    return;
  }

  const int grade = header->value % 10;
  const EnchantSimulationRecord* record = reinterpret_cast<const EnchantSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  int total = 0;
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(EnchantSimulationRecord));
  std::fprintf(file, "\n- %s\n\n", ToUtf8(name, g_textCodePage).c_str());
  std::fprintf(file, "============================\tSTART\t============================\n");
  std::fprintf(file, "\nGrade %d\n", grade);
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.SetItemId(record->itemId);
    item.identified = 1;
    item.unknown_0000[0] = 6;
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    std::fprintf(file, "\t\t%s\t%d\t%.5f%%\n", ToUtf8(naming.DisplayName(item, 0), g_textCodePage).c_str(), record->count,
                 static_cast<float>(record->rate) / 1000.0f);
    total += record->count;
    ++record;
  }
  std::fprintf(file, "Try\t%d\tSetEnchant\n", total);
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D60030
// FUNCTION: fn_21973
void CGameMode::ReceivePackageSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  std::FILE* file = this->OpenPackageSimulatorFile("\\PackageItemTest", header->value, itemName);
  if (!file) {
    return;
  }

  const char* typeNames[22] = {
    "PACKAGETYPE_NONE",      "PACKAGETYPE_MUST",      "PACKAGETYPE_RANDOM_0",  "PACKAGETYPE_RANDOM_1",  "PACKAGETYPE_RANDOM_2",  "PACKAGETYPE_RANDOM_3",
    "PACKAGETYPE_RANDOM_4",  "PACKAGETYPE_RANDOM_5",  "PACKAGETYPE_RANDOM_6",  "PACKAGETYPE_RANDOM_7",  "PACKAGETYPE_RANDOM_8",  "PACKAGETYPE_RANDOM_9",
    "PACKAGETYPE_NATURAL_0", "PACKAGETYPE_NATURAL_1", "PACKAGETYPE_NATURAL_2", "PACKAGETYPE_NATURAL_3", "PACKAGETYPE_NATURAL_4", "PACKAGETYPE_NATURAL_5",
    "PACKAGETYPE_NATURAL_6", "PACKAGETYPE_NATURAL_7", "PACKAGETYPE_NATURAL_8", "PACKAGETYPE_NATURAL_9",
  };
  const PackageSimulationRecord* record = reinterpret_cast<const PackageSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(PackageSimulationRecord));
  std::vector<const PackageSimulationRecord*> packages[22];
  for (int i = 0; i < count; ++i) {
    packages[record->packageType].push_back(record);
    ++record;
  }

  std::fprintf(file, "\n--Item Name : %s (%d)\n\n", ToUtf8(itemName, g_textCodePage).c_str(), header->value);
  std::fprintf(file, "============================\tSTART\t============================\n");
  for (int type = 0; type < 22; ++type) {
    if (packages[type].empty()) {
      continue;
    }

    std::fprintf(file, "\n--\t%s\n", typeNames[type]);
    int unpacked = 0;
    for (const PackageSimulationRecord* package : packages[type]) {
      ITEM_INFO item;
      item.SetItemId(package->itemId);
      item.unknown_0088 = package->refine;
      item.identified = 1;
      item.unknown_0060 = package->unknown_0006;
      if (package->refine != 0 || package->unknown_0006 != 0) {
        item.unknown_0000[0] = 0x15;
      }
      ItemDescriptions descriptions{g_itemDescriptions};
      bool languagePrefix = false;
      WORD language = g_serverLanguage->language;
      ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
      std::fprintf(file, "\t%-24s\t\t\t\t\t\t\t%d\t(%d)\t%d/%d\t%s\n", ToUtf8(naming.DisplayName(item, 0), g_textCodePage).c_str(), package->count,
                   package->unknown_000A, package->unknown_0026, package->unknown_002A, ToUtf8(std::string(package->text), g_textCodePage).c_str());
      unpacked += package->count;
    }
    std::fprintf(file, "\n\t\t\t\t\tUnpacked\t%d\tPackages\n", unpacked);
  }
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D8A300
// FUNCTION: fn_22218
void CGameMode::ReceiveHpSpRecovery(const unsigned char* packet, int variant) {
  CPlayer* player = this->scene->player;
  int amount;
  std::int16_t type;
  switch (variant) {
  case 0:
    amount = *reinterpret_cast<const std::int16_t*>(packet + 4);
    type = *reinterpret_cast<const std::int16_t*>(packet + 2);
    break;
  case 1:
    amount = *reinterpret_cast<const int*>(packet + 4);
    type = *reinterpret_cast<const std::int16_t*>(packet + 2);
    break;
  }

  switch (type) {
  case 5: {
    reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
    const int hp = g_session.unknown_5568 + amount;
    if (g_session.unknown_5568 != hp) {
      g_session.unknown_5568 = hp;
      g_session.RefreshHpSpWindows();
    }
    player->SendMsg(nullptr, 88, 0, amount, amount >> 31, static_cast<int>(0xFF00FF00), 0, 114, 0, 0, 0, 0, 0, 0, 0);
    player->LaunchEffect(331, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  case 7: {
    reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
    int& sp = *reinterpret_cast<int*>(&g_session.unknown_5570[0]);
    const int value = sp + amount;
    if (sp != value) {
      sp = value;
      g_session.RefreshHpSpWindows();
    }
    player->SendMsg(nullptr, 88, 0, amount, amount >> 31, static_cast<int>(0xFF0000FF), 0, 114, 0, 0, 0, 0, 0, 0, 0);
    player->LaunchEffect(332, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  }
}

// GLOBAL: gl_24965
int g_unknown173A5BC;

// OFFSET: 0x00D50580
// FUNCTION: fn_21884
void CGameMode::IgnoreRemovedPacket7(const unsigned char*) {
}

// OFFSET: 0x00D79420
// FUNCTION: fn_22167
void CGameMode::ReceiveUnknown173A5BC(const unsigned char* packet) {
  const int value = *reinterpret_cast<const int*>(packet + 2);
  if (value > 0) {
    g_unknown173A5BC = value;
  }
}

// GLOBAL: gl_24967
int g_unknown16FC588;

// OFFSET: 0x00D7A0B0
// FUNCTION: fn_22186
void CGameMode::ReceiveUnknown16FC588(const unsigned char* packet) {
  const signed char value = *reinterpret_cast<const signed char*>(packet + 2);
  if (value != 1 && value != 2) {
    g_unknown16FC588 = 0;
  }
}

// OFFSET: 0x00D08370
// FUNCTION: fn_21586
void CGameMode::IgnoreRemovedPacket8() {
}

// OFFSET: 0x00D08380
// FUNCTION: fn_21587
void CGameMode::IgnoreRemovedPacket9() {
}

// OFFSET: 0x00D088F0
// FUNCTION: fn_21593
void CGameMode::IgnoreRemovedPacket10(const unsigned char*) {
}

// OFFSET: 0x00D08900
// FUNCTION: fn_21594
void CGameMode::IgnoreRemovedPacket11(const unsigned char*) {
}

// OFFSET: 0x00D89CA0
// FUNCTION: fn_22213
void CGameMode::ResetItemLists(const unsigned char*) {
  g_session.ResetItems61B8();
  g_session.ResetItems6B68();
}

// OFFSET: 0x00D8A1F0
// FUNCTION: fn_22216
void CGameMode::ClearItemLists(const unsigned char* packet) {
  const std::uint16_t slot = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  g_session.ClearUnknown1710(slot);
  this->unknown_05CE = 1;
  RefreshItemWindows(&g_windowMgr);
  g_session.ClearUnknown1740(slot);
  g_session.RefreshItem1740Window();
  g_session.ClearUnknown1738(slot);
  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D31E50
// FUNCTION: fn_21736
void CGameMode::ReceiveMemorialDungeonInfo(const unsigned char* packet) {
  g_session.unknown_5950 = reinterpret_cast<const char*>(packet + 2);
  g_session.unknown_596C = *reinterpret_cast<const int*>(packet + 63);
  g_session.unknown_5970 = *reinterpret_cast<const int*>(packet + 67);
  const int previous = g_session.unknown_5948;
  const int state = *reinterpret_cast<const int*>(packet + 67) != 0 ? 1 : 2;
  g_session.unknown_5948 = state;
  if (previous != state || state != 2 || g_session.unknown_593C != 0) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(137)) {
      window->SendMsg(nullptr, 60, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D31EE0
// FUNCTION: fn_21737
void CGameMode::ReceiveMemorialDungeonNotify(const unsigned char* packet) {
  const int previous = g_session.unknown_5948;
  g_session.unknown_5970 = *reinterpret_cast<const int*>(packet + 6);
  const int time = *reinterpret_cast<const int*>(packet + 6);
  g_session.unknown_5948 = time != 0 ? 1 : 2;
  if (*reinterpret_cast<const int*>(packet + 2) == 0) {
    if (previous != (time != 0 ? 1 : 2) || time != 0 || g_session.unknown_593C != 0) {
      if (UIWindow* window = g_windowMgr.EnsureWindow(137)) {
        window->SendMsg(nullptr, 60, 0, 0, 0, 0);
      }
    }
    return;
  }

  g_windowMgr.CloseWindow(137);
  const int type = *reinterpret_cast<const int*>(packet + 2);
  if (type == 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1346)), 0x64FFFF, 0, 0, 0);
  } else if (type == 2) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1347)), 0x64FFFF, 0, 0, 0);
  } else if (type == 3) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1348)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D31FD0
// FUNCTION: fn_21738
void CGameMode::ReceiveMemorialDungeonSubscriptionInfo(const unsigned char* packet) {
  g_session.unknown_5950 = reinterpret_cast<const char*>(packet + 2);
  g_session.unknown_5968 = *reinterpret_cast<const short*>(packet + 63);
  g_session.unknown_5948 = 0;
  if (UIWindow* window = g_windowMgr.EnsureWindow(137)) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D32040
// FUNCTION: fn_21739
void CGameMode::ReceiveMemorialDungeonSubscriptionNotify(const unsigned char* packet) {
  g_session.unknown_5968 = *reinterpret_cast<const short*>(packet + 2);
  UIWindow* window = g_windowMgr.EnsureWindow(137);
  if (!window) {
    return;
  }

  if (*reinterpret_cast<const short*>(packet + 2) >= 0) {
    g_session.unknown_5948 = 0;
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.CloseWindow(137);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1345)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D320D0
// FUNCTION: fn_21740
void CGameMode::ReceiveWindow17Entries(const unsigned char* packet) {
  this->unknown_024C = 1;
  g_windowMgr.CloseWindow(17);
  if (UIWindow* window = g_windowMgr.EnsureWindow(17)) {
    std::string text;
    text.assign(reinterpret_cast<const char*>(packet + 8), *reinterpret_cast<const unsigned short*>(packet + 2) - 9);
    const std::vector<std::string> entries = Split(text, ":");
    for (const std::string& entry : entries) {
      if (!entry.empty()) {
        window->SendMsg(nullptr, 27, reinterpret_cast<std::intptr_t>(entry.c_str()), 0, 0, 0);
      }
    }
    window->SendMsg(nullptr, 28, *reinterpret_cast<const int*>(packet + 4), 0, 0, 0);
  }

  if (g_windowMgr.unknown_01F0) {
    g_windowMgr.unknown_01F0->SendMsg(nullptr, 32, 0, 0, 0, 0);
  }
}
