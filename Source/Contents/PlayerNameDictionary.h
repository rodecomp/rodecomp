#ifndef RAGNAROK_SOURCE_CONTENTS_PLAYERNAMEDICTIONARY_H
#define RAGNAROK_SOURCE_CONTENTS_PLAYERNAMEDICTIONARY_H

#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <string>
#include <windows.h>

class CConnection;

struct PlayerNameContext {
  CConnection& connection;
  const bool& languagePrefix;
  const int& service;
  const WORD& language;
};

class CPlayerNameDictionary {
public:
  CPlayerNameDictionary();
  explicit CPlayerNameDictionary(PlayerNameContext& context);
  virtual ~CPlayerNameDictionary();
  std::string Lookup(std::uint32_t id);
  void Set(std::uint32_t id, std::string name);
  void Clear();
  bool Contains(std::uint32_t id) const;
  void Update();

private:
  struct Entry {
    /* 0000 */ std::string name;
    /* 0018 */ std::uint32_t tick;

    bool IsExpired() const;
  };
  struct Request {
    /* 0000 */ std::uint32_t id;
    /* 0004 */ std::uint32_t tick;
  };
#if defined(_WIN32) && !defined(_WIN64)
  static_assert(sizeof(Entry) == 0x1C);
  static_assert(offsetof(Entry, tick) == 0x18);
  static_assert(sizeof(Request) == 0x08);
  static_assert(offsetof(Request, tick) == 0x04);
#endif
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::map<std::uint32_t, Entry> m_names;
  /* 0010 */ std::deque<std::uint32_t> m_pending;
  /* 0024 */ std::deque<Request> m_requests;
  PlayerNameContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPlayerNameDictionary) == 60);
#endif

extern CPlayerNameDictionary* g_playerNameDictionary;

bool CreatePlayerNameDictionary();
void DestroyPlayerNameDictionary();
std::string LookupPlayerName(std::uint32_t id);

#endif
