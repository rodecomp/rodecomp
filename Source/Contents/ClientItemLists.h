#ifndef RAGNAROK_SOURCE_CONTENTS_CLIENTITEMLISTS_H
#define RAGNAROK_SOURCE_CONTENTS_CLIENTITEMLISTS_H

#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Messenger/MessengerMember.h"

#include <cstdint>
#include <list>
#include <string>
#include <utility>

struct ClientItemLists {
  /* 0000 */ unsigned char unknown_0000[0xC0];
  /* 00C0 */ std::string hotkeyDescriptions[36];
  /* 0420 */ unsigned char unknown_0420[0x314];
  /* 0734 */ std::list<SessionRecord0734> records0734;
  /* 073C */ unsigned char unknown_073C[0xF30];
  /* 166C */ int unknown_166C;
  /* 1670 */ int unknown_1670;
  /* 1674 */ int unknown_1674;
  /* 1678 */ int unknown_1678;
  /* 167C */ int unknown_167C;
  /* 1680 */ int unknown_1680;
  /* 1684 */ unsigned char unknown_1684[0x8C];
  /* 1710 */ std::list<ITEM_INFO> inventory;
  /* 1718 */ std::list<ITEM_INFO> items_1718;
  /* 1720 */ std::list<ITEM_INFO> items_1720;
  /* 1728 */ std::list<ITEM_INFO> items_1728;
  /* 1730 */ std::list<ITEM_INFO> items_1730;
  /* 1738 */ std::list<ITEM_INFO> items_1738;
  /* 1740 */ std::list<ITEM_INFO> items_1740;
  /* 1748 */ std::list<ITEM_INFO> items_1748;
  /* 1750 */ std::list<ITEM_INFO> items_1750;
  /* 1758 */ std::list<ITEM_INFO> items_1758;
  /* 1760 */ std::list<ITEM_INFO> items_1760;
  /* 1768 */ std::list<ITEM_INFO> items_1768;
  /* 1770 */ std::list<ITEM_INFO> items_1770;
  /* 1778 */ unsigned char unknown_1778[0xC];
  /* 1784 */ std::list<ITEM_INFO> items_1784[11];
  /* 17DC */ std::list<MessengerMember> partyMembers;
  /* 17E4 */ std::list<MessengerMember> friends;
  /* 17EC */ unsigned char unknown_17EC[0x9B4];
  /* 21A0 */ ITEM_INFO equipped[10];
  /* 2B50 */ unsigned char unknown_2B50[0x1360];
  /* 3EB0 */ ITEM_INFO exchangeItems[10];
  /* 4860 */ ITEM_INFO itemSlots[10];
  /* 5210 */ int unknown_5210;
  /* 5214 */ int unknown_5214;
  /* 5218 */ unsigned char unknown_5218[0x24C];
  /* 5464 */ int unknown_5464;
  /* 5468 */ unsigned char unknown_5468[0x5C];
  /* 54C4 */ std::string unknown_54C4;
  /* 54DC */ unsigned char unknown_54DC[0xCDC];
  /* 61B8 */ ITEM_INFO items_61B8[10];
  /* 6B68 */ ITEM_INFO items_6B68[10];

  bool AddItem1710(const ITEM_INFO& item, unsigned char refresh);
  void AddItem1718(const RawItemBasic& item, int quantity, std::uint16_t unknown_006C);
  void AddItem1718(const RawItemBasic& item, int mode);
  bool AddItem1768(const ITEM_INFO& item, unsigned char alternate, unsigned char refresh);
  bool AddItem1740(const ITEM_INFO& item);
  bool AddItem1750(const ITEM_INFO& item, int type, unsigned char refresh);
  void AddItem1758(const ITEM_INFO& item);
  void AddItem1760(const ITEM_INFO& item);
  void AddItem1748(const ITEM_INFO& item);
  bool Equip(const ITEM_INFO& item);
  void AppendItem1718(const ITEM_INFO& item);
  void MergeItem1718ByName(const ITEM_INFO& item);
  void MergeItem1720ByName(const ITEM_INFO& item);
  void MergeItem1720(const ITEM_INFO& item);
  void AddItem1730(const ITEM_INFO& item);
  void MergeItem1718(const ITEM_INFO& item);
  bool AddItem1738(const ITEM_INFO& item, unsigned char refresh);
  void RemoveItem1758ByItemId(ITEM_INFO& item);
  __declspec(noinline) void RemoveItem1710(std::uint32_t id, int quantity);
  bool AddExchangeItem(ITEM_INFO item);
  bool RemoveItem1768(const ITEM_INFO& item, unsigned char alternate);
  bool RemoveItem1740(const ITEM_INFO& item);
  bool RemoveItem1750(const ITEM_INFO& item, int type, unsigned char refresh);
  void RemoveItem1758(ITEM_INFO& item);
  void RemoveItem1760(const ITEM_INFO& item);
  void RemoveItem1718ByItemId(const ITEM_INFO& item);
  void RemoveItem1718(const ITEM_INFO& item);
  void RemoveItem1720ByName(const ITEM_INFO& item);
  void RemoveItem1720(const ITEM_INFO& item);
  void RemoveItem1730(const ITEM_INFO& item);
  void ReduceItem1718(const ITEM_INFO& item);
  bool RemoveItem1738(const ITEM_INFO& item);
  void ClearInventory();
  void ClearItems1760And1748();
  void ClearItems1768();
  void ClearItems1740();
  void ClearItems1750();
  void SetItems1750(int count, const short* ids);
  void ClearItems1758();
  void ClearItems1760();
  void ClearItems1748();
  void ClearItems1784(int index);
  void ClearItemSlotQuantities();
  void ResetEquipped();
  void ClearItems1720();
  void ClearItems1738();
  void CopyItems1740To1750();
  void CopyItems1760To1770();
  std::pair<bool, ITEM_INFO> FindItem1750(const ITEM_INFO& item, unsigned char unknown);
  void ClearRecords0734();
  void ClearPartyMembers();
  void ResetUnknown166C();
  void ResetHotkeyDescriptions();
  int IsDifferentName(const char* name, const char* other);
  int IsGuildMemberName(const char* name);
  int IsFriendName(const char* name);
  bool EquipItem6B68(const ITEM_INFO& item);
  void ResetItems6B68();
  void ResetItems61B8();
};

#endif
