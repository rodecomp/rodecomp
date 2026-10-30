#ifndef RAGNAROK_SOURCE_CONTENTS_CASHEMOTIONMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CASHEMOTIONMGR_H

#include "Utility/Timestamp.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class CCashEmotionMgr;
class ICashEmotionAction;
struct lua_State;

struct CashEmotionKeyHash {
  // INLINED
  std::size_t operator()(int key) const {
    return static_cast<std::size_t>(key);
  }
};

class CashEmotionCommand {
public:
  CashEmotionCommand(const std::string& name, int packId, int emotionId);

  void Add(int packId, int emotionId);
  std::pair<int, int> Get(int index) const;
  // INLINED
  int Count() const {
    return static_cast<int>(this->entries.size());
  }

  /* 0000 */ std::string name;
  /* 0018 */ std::vector<std::pair<int, int>> entries;
};

struct CashEmotionData {
  CashEmotionData(int id, int subId, const char* name, int unknown20, int unknown24, int actId, int sprId);

  /* 0000 */ int id;
  /* 0004 */ int subId;
  /* 0008 */ std::string name;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ int actId;
  /* 002C */ int sprId;
};

struct CashEmotionPack {
  // INLINED
  CashEmotionPack(const char* name, int unknown18, unsigned int startTime, unsigned int endTime, int unknown24, char unknown28)
      : name(name), unknown_0018(unknown18), startTime(startTime), endTime(endTime), unknown_0024(unknown24), unknown_0028(unknown28),
        originalStartTime(startTime), originalEndTime(endTime) {
  }

  // INLINED
  bool IsAvailable(unsigned int now) const {
    if (this->startTime == 0 && this->endTime == 0) {
      return true;
    }
    if (now >= this->startTime && this->endTime == 0) {
      return true;
    }
    if (this->startTime == 0 && now < this->endTime) {
      return true;
    }
    return now >= this->startTime && now < this->endTime;
  }

  /* 0000 */ std::string name;
  /* 0018 */ int unknown_0018;
  /* 001C */ unsigned int startTime;
  /* 0020 */ unsigned int endTime;
  /* 0024 */ int unknown_0024;
  /* 0028 */ bool unknown_0028;
  /* 002C */ unsigned int originalStartTime;
  /* 0030 */ unsigned int originalEndTime;
};

enum class CashEmotionSpecial : char {
  ChatProhibit,
  Yut,
  Dice,
  Sweat,
  Chup,
};

struct CashEmotionSpecialHash {
  // INLINED
  std::size_t operator()(CashEmotionSpecial key) const {
    return static_cast<std::size_t>(key);
  }
};

struct CashEmotionPackRefresh {
  /* 0000 */ bool changed;
  /* 0004 */ int nextTime;
};

using CashEmotionTable = std::unordered_map<int, CashEmotionData, CashEmotionKeyHash>;
using CashEmotionFileNames = std::unordered_map<int, std::string, CashEmotionKeyHash>;

struct CashEmotionPackOrder {
  bool operator()(const int& left, const int& right) const;

  /* 0000 */ CCashEmotionMgr* mgr;
  /* 0004 */ const bool& ascending;
};

class CCashEmotionMgr {
  friend class UICashEmotionListWnd;

public:
  CCashEmotionMgr();
  virtual ~CCashEmotionMgr();

  static bool Initialize();
  static void Shutdown();
  static int InsertEmotionGroup(lua_State* state);
  static int InsertEmotionMsgListTable(lua_State* state);
  static int InsertEmotionMsgListTableForCustom(lua_State* state);
  static int InsertPackInfo(lua_State* state);

  void Reset();
  void ClearOwnedPacks();
  void ClearCommands();
  void HideSuggestions();
  int FindEmotionId(const char* name);
  int ProcessChatCommand(const char* command);
  const char* GetPackName(int packId);
  std::vector<int> FindBaseEmotions(const char* filter);
  CashEmotionTable* GetBasePackEmotions();
  int GetSpecialEmotion(CashEmotionSpecial type);
  CashEmotionData* GetEmotionData(int packId, int emotionId);
  const char* GetEmotionName(int packId, int emotionId);
  std::vector<int> GetPackEmotions(int packId);
  int GetEarliestExpiry();
  CashEmotionTable* GetPackEmotionTable(int packId);
  CashEmotionPack* GetPack(int packId);
  const char* GetPackTitle(int packId);
  int GetPackUnknown18(int packId);
  Timestamp* FindExpiry(int packId);
  std::vector<int> SortOwnedPacks(const char* filter);
  unsigned int GetPackEndTime(int packId);
  unsigned int GetPackStartTime(int packId);
  void Search(
    const char* text, std::vector<std::pair<int, int>>& basePack, std::vector<std::pair<int, int>>& ownedPacks, std::vector<std::pair<int, int>>& otherPacks
  );
  void AddSearchResult(std::vector<std::pair<int, int>>& results, const std::pair<int, int>& entry);
  std::vector<int> SortPacks(const std::vector<int>& packs, const char* filter);
  const char* GetSprFileName(int sprId);
  std::vector<int> SortUnownedPacks(const char* filter);
  int GetPackUnknown24(int packId);
  void SetOwnedPack(int packId, unsigned int expiry);
  int LookupSpecialEmotion(CashEmotionSpecial type);
  int LookupSpecialEmotion(CashEmotionSpecial type, const char* name);
  void AddShortcut(const std::string& name, int packId, int emotionId);
  bool PlaySearchedEmotion(const std::string& name);
  bool IsChatProhibitEmotion(int emotionId);
  bool IsPackAvailable(unsigned int now, int packId);
  bool IsOwnedPack(int packId);
  void RemoveOwnedPack(int packId, bool refresh, bool clearExpiry);
  CashEmotionPackRefresh RefreshUnownedPacks();
  void sub_59CDE0(int param);
  void SetServerTimeZone(short serverBias);
  bool sub_59C5A0();
  bool sub_59D2C0();
  void ShowErrorMessage(const char* key, int value);
  void Unknown59D040(int argument1, int argument2);
  int sub_59FC50();
  // INLINED
  int BasePackId() const {
    return this->unknown_000C;
  }

protected:
  /* 0004 */ bool m_enabled;
  /* 0005 */ std::uint8_t unknown_0005[3];
  /* 0008 */ ICashEmotionAction* m_action;
  /* 000C */ int unknown_000C;
  /* 0010 */ std::unordered_map<int, CashEmotionPack, CashEmotionKeyHash> unknown_0010;
  /* 0030 */ std::unordered_map<int, CashEmotionTable> m_emotionsByPack;
  /* 0050 */ std::unordered_map<int, std::vector<int>, CashEmotionKeyHash> unknown_0050;
  /* 0070 */ std::unordered_map<std::string, std::pair<int, int>> m_searchIndex;
  /* 0090 */ CashEmotionFileNames m_packNames;
  /* 00B0 */ std::unordered_map<std::string, int> unknown_00B0;
  /* 00D0 */ int m_lastActId = 0;
  /* 00D4 */ CashEmotionFileNames m_sprNames;
  /* 00F4 */ std::unordered_map<std::string, int> m_sprIds;
  /* 0114 */ int m_lastSprId = 0;
  /* 0118 */ std::unordered_map<std::string, int> m_emotionIds;
  /* 0138 */ std::unordered_map<CashEmotionSpecial, int, CashEmotionSpecialHash> m_specials;
  /* 0158 */ std::unordered_map<std::string, CashEmotionCommand*> m_shortcuts;
  /* 0178 */ std::unordered_map<std::string, int> m_commands;
  /* 0198 */ std::vector<int> unknown_0198;
  /* 01A4 */ std::vector<int> m_ownedPacks;
  /* 01B0 */ std::list<std::pair<int, Timestamp>> m_expiry;
  /* 01B8 */ std::vector<int> m_unownedPacks;
  /* 01C4 */ int unknown_01C4 = -1;
  /* 01C8 */ int unknown_01C8 = 6909;
  /* 01CC */ int unknown_01CC = 0;
  /* 01D0 */ int unknown_01D0 = 0;
  /* 01D4 */ int unknown_01D4 = 0;
};

extern CCashEmotionMgr* g_cashEmotionMgr;
class UICashEmotionListWnd;
extern UICashEmotionListWnd* g_cashEmotionListWnd;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CashEmotionData, name) == 0x08);
static_assert(sizeof(CashEmotionData) == 0x30);
static_assert(sizeof(CashEmotionPack) == 0x34);
static_assert(sizeof(CashEmotionCommand) == 0x24);
static_assert(sizeof(CashEmotionPackOrder) == 0x08);
static_assert(sizeof(CCashEmotionMgr) == 472);
#endif

#endif
