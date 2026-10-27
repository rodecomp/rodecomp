#ifndef RAGNAROK_SOURCE_CONTENTS_CASHEMOTIONMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CASHEMOTIONMGR_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

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
  void HideSuggestions();
  int ProcessChatCommand(const char* command);
  const char* GetPackName(int packId);
  CashEmotionData* GetEmotionData(int packId, int emotionId);
  const char* GetEmotionName(int packId, int emotionId);

protected:
  /* 0004 */ bool m_enabled;
  /* 0005 */ std::uint8_t unknown_0005[3];
  /* 0008 */ void* m_action;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint8_t unknown_0010[32];
  /* 0030 */ std::unordered_map<int, std::unordered_map<int, CashEmotionData>> m_emotionsByPack;
  /* 0050 */ std::uint8_t unknown_0050[64];
  /* 0090 */ std::unordered_map<int, std::string> m_packNames;
  /* 00B0 */ std::uint8_t unknown_00B0[200];
  /* 0178 */ std::unordered_map<std::string, int> m_commands;
  /* 0198 */ std::uint8_t unknown_0198[64];
};

extern CCashEmotionMgr* g_cashEmotionMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CashEmotionData, name) == 0x08);
static_assert(sizeof(CCashEmotionMgr) == 472);
#endif

#endif
