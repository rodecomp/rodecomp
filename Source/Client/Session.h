#ifndef RAGNAROK_SOURCE_CLIENT_SESSION_H
#define RAGNAROK_SOURCE_CLIENT_SESSION_H

#include "Contents/InstantVar.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/XmlNode.h"

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

class LuaScript;

struct SessionRecord6144 {
  /* 0000 */ std::uint16_t unknown_0000 = 0;
  /* 0004 */ std::uint32_t unknown_0004[10] = {};
  /* 002C */ std::uint32_t unknown_002C = 0;
  /* 0030 */ std::uint8_t unknown_0030 = 0;
  /* 0031 */ std::uint8_t unknown_0031[7];
};

struct RobeAlpha {
  /* 0000 */ int from;
  /* 0004 */ int to;
  /* 0008 */ int period;
};

struct SessionShop {
  /* 0000 */ unsigned char unknown_0000;
  /* 0004 */ std::vector<std::map<int, std::pair<int, int>>> unknown_0004;
};

class CSession {
public:
  /* 0000 */ std::uint32_t unknown_0000;
  /* 0004 */ std::list<CSkillInfo> m_additionalSkills;
  /* 000C */ CPlayerSkillBundle m_skillBundle;
  /* 0084 */ std::list<CSkillInfo> m_homunSkills;
  /* 008C */ std::list<CSkillInfo> m_mercSkills;
  /* 0094 */ unsigned char unknown_0094[0x28];
  /* 00BC */ unsigned char m_hotkeyPage;
  /* 00BD */ unsigned char unknown_00BD[3];
  /* 00C0 */ std::string m_hotkeyDescriptions[36];
  /* 0420 */ std::uint32_t unknown_0420[36];
  /* 04B0 */ NormalItemSlot m_shortcutSlots[36];
  /* 05AC */ NormalItemSlot m_secondaryShortcutSlots[36];
  /* 06A8 */ unsigned char unknown_06A8[0x43];
  /* 06EB */ NormalItemSlot unknown_06EB;
  /* 06F2 */ NormalItemSlot unknown_06F2;
  /* 06F9 */ unsigned char unknown_06F9[0x23];
  /* 071C */ std::vector<int> m_homunSkillLevels;
  /* 0728 */ std::vector<int> m_mercSkillLevels;
  /* 0734 */ unsigned char unknown_0734[0x44];
  /* 0778 */ std::vector<std::pair<int, int>> unknown_0778;
  /* 0784 */ std::map<std::int64_t, std::int64_t> unknown_0784;
  /* 078C */ std::map<std::int64_t, std::int64_t> unknown_078C;
  /* 0794 */ std::uint32_t m_serverTimeOffset;
  /* 0798 */ int m_serverTimeAdjustCount;
  /* 079C */ unsigned char unknown_079C[0x04];
  /* 07A0 */ std::uint32_t m_averagePing;
  /* 07A4 */ unsigned char unknown_07A4[0x6F4];
  /* 0E98 */ int unknown_0E98;
  /* 0E9C */ unsigned char unknown_0E9C[0x64];
  /* 0F00 */ int m_robe;
  /* 0F04 */ unsigned char unknown_0F04[0x14];
  /* 0F18 */ int m_playerX;
  /* 0F1C */ int m_playerY;
  /* 0F20 */ unsigned char unknown_0F20[0x08];
  /* 0F28 */ int m_score1;
  /* 0F2C */ int m_score2;
  /* 0F30 */ unsigned char unknown_0F30[0x78];
  /* 0FA8 */ const char* const* m_jobNamesBegin;
  /* 0FAC */ const char* const* m_jobNamesEnd;
  /* 0FB0 */ unsigned char unknown_0FB0[0x34];
  /* 0FE4 */ std::list<std::uint32_t> m_pingSamples;
  /* 0FEC */ unsigned char unknown_0FEC[0x6C];
  /* 1058 */ int m_petActionTable[5][5][7];
  /* 1314 */ unsigned char unknown_1314[0x258];
  /* 156C */ XmlDocument m_petTalkTable;
  /* 159C */ XmlDocument m_monsterSkillInfo;
  /* 15CC */ XmlDocument m_monsterTalkTable;
  /* 15FC */ unsigned char unknown_15FC[0x10];
  /* 160C */ char m_navigationPlayerMap[0x1C];
  /* 1628 */ int m_job;
  /* 162C */ unsigned char unknown_162C[0x24];
  /* 1650 */ int m_level;
  /* 1654 */ unsigned char unknown_1654[0x9C];
  /* 16F0 */ int m_zeny;
  /* 16F4 */ unsigned char unknown_16F4[0x1C];
  /* 1710 */ std::list<ItemInfo> m_inventoryItems;
  /* 1718 */ unsigned char unknown_1718[0x20];
  /* 1738 */ std::list<ItemInfo> unknown_1738;
  /* 1740 */ std::list<ItemInfo> unknown_1740;
  /* 1748 */ unsigned char unknown_1748[0x1408];
  /* 2B50 */ ItemInfo unknown_2B50[10];
  /* 3500 */ ItemInfo unknown_3500[10];
  /* 3EB0 */ unsigned char unknown_3EB0[0x9B0];
  /* 4860 */ ItemInfo m_itemSlots[10];
  /* 5210 */ unsigned char unknown_5210[0xA8];
  /* 52B8 */ std::vector<const char*> m_accessoryNames;
  /* 52C4 */ unsigned char unknown_52C4[0x294];
  /* 5558 */ int unknown_5558;
  /* 555C */ unsigned char unknown_555C[0xD0];
  /* 562C */ char m_mercenaryName[32];
  /* 564C */ int m_mercenaryStats[8];
  /* 566C */ int m_mercenaryHp;
  /* 5670 */ int m_mercenaryMaxHp;
  /* 5674 */ int m_mercenaryClass;
  /* 5678 */ int m_mercenaryLevel;
  /* 567C */ int m_mercenarySp;
  /* 5680 */ int m_mercenaryMaxSp;
  /* 5684 */ int m_mercenaryLoyalty;
  /* 5688 */ int m_mercenarySummons;
  /* 568C */ int m_mercenaryKills;
  /* 5690 */ unsigned char unknown_5690[4];
  /* 5694 */ __time32_t m_mercenaryExpiration;
  /* 5698 */ unsigned char unknown_5698[0x340];
  /* 59D8 */ LuaScript* m_hotkeyScript;
  /* 59DC */ unsigned char unknown_59DC[0x218];
  /* 5BF4 */ int m_pairValues[3];
  /* 5C00 */ unsigned char unknown_5C00[0x20];
  /* 5C20 */ long long m_bankBalance;
  /* 5C28 */ unsigned char unknown_5C28[0x40];
  /* 5C68 */ ItemInfo unknown_5C68[5];
  /* 6140 */ std::uint16_t unknown_6140;
  /* 6144 */ std::map<int, SessionRecord6144> unknown_6144;
  /* 614C */ unsigned char unknown_614C[0x20];
  /* 616C */ std::vector<int> unknown_616C;
  /* 6178 */ unsigned char unknown_6178[0x1D68];
  /* 7EE0 */ std::map<int, RobeAlpha> m_robeAlpha;
  /* 7EE8 */ unsigned char unknown_7EE8[0xC0];
  /* 7FA8 */ CInstantVar m_options;
  /* 7FB4 */ unsigned char unknown_7FB4[0x100];
  /* 80B4 */ std::map<std::string, int> m_nameToId;
  /* 80BC */ std::map<int, SessionShop> m_shops;
  /* 80C4 */ unsigned char unknown_80C4[0x58];
  /* 811C */ std::vector<ItemInfo> m_cardItems;
  /* 8128 */ unsigned char unknown_8128[0x0C];
  /* 8134 */ int unknown_8134;
  /* 8138 */ std::int64_t m_startTimeTicks;
  /* 8140 */ std::int64_t m_loginTimeTicks;
  /* 8148 */ int unknown_8148;

  int GetOption(int id) const;
  void SetOption(int id, int value, int extra);
  std::uint32_t GetServerTime();
  std::uint32_t AddPingSample(std::uint32_t sample);
  std::uint32_t SetServerTime(std::uint32_t serverTime);
  void SetHotkeyDescription(const char* description, int index);
  void AddItemSlot(ItemInfo item);
  SessionShop* GetShop(int shopId);
  void SetShopFlag(int shopId, char flag);
  void SetShopItem(int shopId, unsigned int index, int itemId, int first, int second);
  bool AddShopName(std::string name, int shopId);
  int SetUnknown8134(int value);
  void ClearUnknown1710(int id);
  void ClearUnknown1740(int id);
  void ClearUnknown1738(int id);
  std::uint32_t SyncServerTime(std::uint32_t serverTime);
  std::pair<int, int>* InsertPair(int key, int value);
  int SetPairValue(int value, int slot);
  int SetHotkeyEntry(char enabled, int id, int value, int slot);
  int SetFlag(char value);
  // INLINED
  NormalItemSlot& SkillHotkey(int index) {
    return reinterpret_cast<NormalItemSlot*>(reinterpret_cast<unsigned char*>(this) + offsetof(CSession, unknown_06EB))[index];
  }
  void ResetItems2B50();
  int GetPetFood(int job);
  std::string GetMonsterActName(int job);
  std::string GetMonsterSprName(int job);
  void GetPetIllustName(int job, int, char* buffer);
  int GetPetJobByEggItem(int eggItemId);
  const char* GetPetString(int id);
  const char* GetPetTalkName(int job);
  const char* GetPetTalkActionName(int action);
  const char* GetMonsterTalkEventName(int event);
  bool UpdateHomunSkill(const CSkillInfo& value, unsigned char notify);
  bool UpdateMercSkill(const CSkillInfo& value, unsigned char notify);
  void AddUnknownId(int id);
  void AddStackedItem(ItemInfo item);
  void AddItemToFreeSlot(ItemInfo item);
  bool IsListedId(int id, int variant) const;
  void ClearItemQuantities();
  void ResetItems3500();
  void ClearSlots06EB();
  void ClearShortcutSlots();
  void ClearSecondaryShortcutSlots();
  void ClearUnknown0420();
  void EraseUnknownKey(std::int64_t key);
  std::int64_t FindUnknown0784(std::int64_t key) const;
  std::int64_t FindUnknown078C(std::int64_t key) const;
  SessionRecord6144 FindRecord6144(int key) const;
  int GetBeginEffectId(int skillId) const;
  ItemInfo GetItem2B50(int index) const;
  int CountItems5C68() const;
  void SplitDate(__time32_t time, std::uint16_t& year, std::uint16_t& month, std::uint16_t& day) const;
  int GetDefaultOptionValue(const char* name, int fallback) const;
  int FindUnknown0778(int key) const;
  CSkillInfo AdditionalSkillAt(int index) const;
  CSkillInfo HomunSkillAt(int index) const;
  void NotifyHomunSkillWindows();
  void NotifyMercSkillWindows();
  int GetPetActionNumber(int hungerLevel, int friendlyLevel, int action);
  int GetPetFriendlyState(int friendly);
  void GetPetFriendlyStateString(int friendly, char* buffer);
  int GetPetHungryState(int hunger);
  void GetPetHungryStateString(int hunger, char* buffer);
  int GetPetEmotionNumber(int job, int action, int hungerLevel);
  void ClearItemSelection();
  bool IsReducedSkill(int skillId) const;
  char* ConvertTooltipText(const char* source, char* output) const;
  int MercSkillLevel(int skillId) const;
  int HomunSkillLevel(int skillId) const;
  int SyncUnknown0E98();
  void LoadXmlTables();
  std::string GetAccessoryName(int id);
  void CollectCardItems();
  int GetRobeAlpha(int robeId);
  bool IsTopLayerRobe(int robe);
  int IsSkillIdInRange6018(unsigned id) const;
  int IsSkillIdInRange6001(unsigned id) const;
};

extern CSession g_session;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SessionRecord6144) == 0x38);
static_assert(offsetof(SessionRecord6144, unknown_002C) == 0x2C);
static_assert(offsetof(SessionRecord6144, unknown_0030) == 0x30);
static_assert(offsetof(CSession, m_additionalSkills) == 0x4);
static_assert(offsetof(CSession, m_options) == 0x7FA8);
static_assert(offsetof(CSession, m_nameToId) == 0x80B4);
static_assert(offsetof(CSession, m_level) == 0x1650);
static_assert(offsetof(CSession, m_hotkeyPage) == 0xBC);
static_assert(offsetof(CSession, m_pairValues) == 0x5BF4);
static_assert(offsetof(CSession, m_zeny) == 0x16F0);
static_assert(offsetof(CSession, m_bankBalance) == 0x5C20);
static_assert(offsetof(CSession, m_mercenaryStats) == 0x564C);
static_assert(offsetof(CSession, m_mercenaryExpiration) == 0x5694);
static_assert(offsetof(CSession, m_hotkeyDescriptions) == 0xC0);
static_assert(offsetof(CSession, m_serverTimeOffset) == 0x794);
static_assert(offsetof(CSession, m_serverTimeAdjustCount) == 0x798);
static_assert(offsetof(CSession, m_averagePing) == 0x7A0);
static_assert(offsetof(CSession, m_pingSamples) == 0xFE4);
static_assert(offsetof(CSession, m_petTalkTable) == 0x156C);
static_assert(offsetof(CSession, m_monsterTalkTable) == 0x15CC);
static_assert(offsetof(CSession, m_navigationPlayerMap) == 0x160C);
static_assert(offsetof(CSession, unknown_1738) == 0x1738);
static_assert(offsetof(CSession, unknown_1740) == 0x1740);
static_assert(offsetof(CSession, m_itemSlots) == 0x4860);
static_assert(offsetof(CSession, m_shops) == 0x80BC);
static_assert(offsetof(CSession, unknown_8134) == 0x8134);
static_assert(offsetof(CSession, m_skillBundle) == 0xC);
static_assert(offsetof(CSession, m_homunSkills) == 0x84);
static_assert(offsetof(CSession, m_mercSkills) == 0x8C);
static_assert(offsetof(CSession, unknown_0420) == 0x420);
static_assert(offsetof(CSession, m_shortcutSlots) == 0x4B0);
static_assert(offsetof(CSession, m_secondaryShortcutSlots) == 0x5AC);
static_assert(offsetof(CSession, unknown_06EB) == 0x6EB);
static_assert(offsetof(CSession, unknown_06F2) == 0x6F2);
static_assert(offsetof(CSession, m_homunSkillLevels) == 0x71C);
static_assert(offsetof(CSession, m_mercSkillLevels) == 0x728);
static_assert(offsetof(CSession, unknown_0778) == 0x778);
static_assert(offsetof(CSession, unknown_0784) == 0x784);
static_assert(offsetof(CSession, unknown_078C) == 0x78C);
static_assert(offsetof(CSession, m_robe) == 0xF00);
static_assert(offsetof(CSession, m_playerX) == 0xF18);
static_assert(offsetof(CSession, m_playerY) == 0xF1C);
static_assert(offsetof(CSession, m_score1) == 0xF28);
static_assert(offsetof(CSession, m_score2) == 0xF2C);
static_assert(offsetof(CSession, m_jobNamesBegin) == 0xFA8);
static_assert(offsetof(CSession, m_jobNamesEnd) == 0xFAC);
static_assert(offsetof(CSession, unknown_0E98) == 0xE98);
static_assert(offsetof(CSession, m_petActionTable) == 0x1058);
static_assert(offsetof(CSession, m_job) == 0x1628);
static_assert(offsetof(CSession, m_inventoryItems) == 0x1710);
static_assert(offsetof(CSession, unknown_2B50) == 0x2B50);
static_assert(offsetof(CSession, unknown_3500) == 0x3500);
static_assert(offsetof(CSession, m_accessoryNames) == 0x52B8);
static_assert(offsetof(CSession, unknown_5558) == 0x5558);
static_assert(offsetof(CSession, m_hotkeyScript) == 0x59D8);
static_assert(offsetof(CSession, unknown_5C68) == 0x5C68);
static_assert(offsetof(CSession, unknown_6140) == 0x6140);
static_assert(offsetof(CSession, unknown_6144) == 0x6144);
static_assert(offsetof(CSession, unknown_616C) == 0x616C);
static_assert(offsetof(CSession, m_robeAlpha) == 0x7EE0);
static_assert(offsetof(CSession, m_cardItems) == 0x811C);
static_assert(offsetof(CSession, m_startTimeTicks) == 0x8138);
static_assert(offsetof(CSession, m_loginTimeTicks) == 0x8140);
#endif

#endif
