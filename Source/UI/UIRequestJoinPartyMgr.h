#ifndef RAGNAROK_SOURCE_UI_UIREQUESTJOINPARTYMGR_H
#define RAGNAROK_SOURCE_UI_UIREQUESTJOINPARTYMGR_H

#include <cstdint>
#include <list>

class WindowManagerWindows;

struct PartyJoinRequest {
  char name[24];
  std::uint32_t characterId;
  std::uint32_t accountId;
  std::int16_t level;
  std::int16_t job;
};
static_assert(sizeof(PartyJoinRequest) == 36);

// RTTI UIRequestJoinPartyMgr, original x86 size 24. Manager/factory view is
// borrowed source-only state after the original owning lists.
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

// STATIC: 0x01450CEC
// GLOBAL: gl_24363
extern UIRequestJoinPartyMgr* g_requestJoinPartyMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRequestJoinPartyMgr) == 28);
#endif

#endif
