#ifndef RAGNAROK_SOURCE_CONTENTS_CASHEMOTIONMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CASHEMOTIONMGR_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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

  /* 0000 */ std::string name;
  /* 0018 */ std::vector<std::pair<int, int>> entries;
};

struct CashEmotionData {
  int id;
  int subId;
  /* 0008 */ std::string name;
};

class CCashEmotionMgr {
public:
  CCashEmotionMgr();
  virtual ~CCashEmotionMgr();

  static bool Initialize();
  static void Shutdown();

  void Reset();
  void ClearCommands();
  void HideSuggestions();
  int ProcessChatCommand(const char* command);
  const char* GetPackName(int packId);
  CashEmotionData* GetEmotionData(int packId, int emotionId);
  const char* GetEmotionName(int packId, int emotionId);
  const char* GetSprFileName(int sprId);
  bool IsOwnedPack(int packId);

protected:
  /* 0004 */ bool m_enabled;
  /* 0005 */ std::uint8_t unknown_0005[3];
  /* 0008 */ void* m_action;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint8_t unknown_0010[32];
  /* 0030 */ std::unordered_map<int, std::unordered_map<int, CashEmotionData>> m_emotionsByPack;
  /* 0050 */ std::uint8_t unknown_0050[64];
  /* 0090 */ std::unordered_map<int, std::string> m_packNames;
  /* 00B0 */ std::uint8_t unknown_00B0[36];
  /* 00D4 */ std::unordered_map<int, std::string, CashEmotionKeyHash> m_sprNames;
  /* 00F4 */ std::uint8_t unknown_00F4[132]{};
  /* 0178 */ std::unordered_map<std::string, int> m_commands;
  /* 0198 */ std::uint8_t unknown_0198[12];
  /* 01A4 */ std::vector<int> m_ownedPacks;
  /* 01B0 */ std::uint8_t unknown_01B0[40]{};
};

extern CCashEmotionMgr* g_cashEmotionMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CashEmotionData, name) == 0x08);
static_assert(sizeof(CashEmotionCommand) == 0x24);
static_assert(sizeof(CCashEmotionMgr) == 472);
#endif

#endif
