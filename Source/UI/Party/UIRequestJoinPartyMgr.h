#ifndef RAGNAROK_SOURCE_UI_PARTY_UIREQUESTJOINPARTYMGR_H
#define RAGNAROK_SOURCE_UI_PARTY_UIREQUESTJOINPARTYMGR_H

#include <cstddef>
#include <cstdint>
#include <list>

class WindowManagerWindows;

struct PartyJoinRequest {
  /* 0000 */ char name[24];
  /* 0018 */ std::uint32_t characterId;
  /* 001C */ std::uint32_t accountId;
  /* 0020 */ std::int16_t level;
  /* 0022 */ std::int16_t job;
};
static_assert(sizeof(PartyJoinRequest) == 36);

class UIRequestJoinPartyMgr {
public:
  explicit UIRequestJoinPartyMgr(WindowManagerWindows& windows);
  virtual ~UIRequestJoinPartyMgr();
  static bool Initialize(WindowManagerWindows& windows);
  static void Shutdown();
  void Add(std::uint32_t accountId, std::uint32_t characterId, const char* name, std::int16_t job, std::int16_t level);
  void Advance();
  void Block(std::uint32_t accountId);

private:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::list<PartyJoinRequest> m_pending;
  /* 0010 */ std::list<std::uint32_t> m_blocked;
  WindowManagerWindows* m_windows;
};

extern UIRequestJoinPartyMgr* g_requestJoinPartyMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(PartyJoinRequest, characterId) == 0x18);
static_assert(offsetof(PartyJoinRequest, accountId) == 0x1C);
static_assert(offsetof(PartyJoinRequest, level) == 0x20);
static_assert(offsetof(PartyJoinRequest, job) == 0x22);
static_assert(sizeof(UIRequestJoinPartyMgr) == 28);
#endif

#endif
