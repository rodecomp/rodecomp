#ifndef RAGNAROK_SOURCE_CLIENT_SESSION_H
#define RAGNAROK_SOURCE_CLIENT_SESSION_H

#include <cstddef>
#include <cstdint>

class LuaScript;

class CSession {
public:
  /* 0000 */ unsigned char unknown_0000[0xF18];
  /* 0F18 */ int m_playerX;
  /* 0F1C */ int m_playerY;
  /* 0F20 */ unsigned char unknown_0F20[0x88];
  /* 0FA8 */ const char* const* m_jobNamesBegin;
  /* 0FAC */ const char* const* m_jobNamesEnd;
  /* 0FB0 */ unsigned char unknown_0FB0[0x678];
  /* 1628 */ int m_job;
  /* 162C */ unsigned char unknown_162C[0x43AC];
  /* 59D8 */ LuaScript* m_hotkeyScript;
  /* 59DC */ unsigned char unknown_59DC[0x275C];
  /* 8138 */ std::int64_t m_startTimeTicks;
  /* 8140 */ std::int64_t m_loginTimeTicks;
  /* 8148 */ int unknown_8148;
};

extern CSession g_session;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSession, m_playerX) == 0xF18);
static_assert(offsetof(CSession, m_playerY) == 0xF1C);
static_assert(offsetof(CSession, m_jobNamesBegin) == 0xFA8);
static_assert(offsetof(CSession, m_jobNamesEnd) == 0xFAC);
static_assert(offsetof(CSession, m_job) == 0x1628);
static_assert(offsetof(CSession, m_hotkeyScript) == 0x59D8);
static_assert(offsetof(CSession, m_startTimeTicks) == 0x8138);
static_assert(offsetof(CSession, m_loginTimeTicks) == 0x8140);
#endif

#endif
