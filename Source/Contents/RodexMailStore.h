#ifndef RAGNAROK_SOURCE_CONTENTS_RODEXMAILSTORE_H
#define RAGNAROK_SOURCE_CONTENTS_RODEXMAILSTORE_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

struct RodexMail {
  /* 0000 */ unsigned char isRead;
  /* 0001 */ unsigned char unknown_0001;
  /* 0002 */ std::array<std::byte, 2> unknown_0002;
  /* 0004 */ std::string unknown_0004;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ std::string unknown_0024;
  /* 003C */ std::string unknown_003C;
  /* 0054 */ unsigned char hasItem;
  /* 0055 */ std::array<std::byte, 0x133> unknown_0055;
  /* 0188 */ std::int64_t zeny;
  /* 0190 */ std::uint8_t unknown_0190 = 0;
  /* 0191 */ std::array<std::byte, 3> unknown_0191;
  /* 0194 */ int unknown_0194 = -1;
};

class RodexMailStore {
public:
  // INLINED
  RodexMailStore() {
    this->m_isValid = false;
    this->m_mails0.clear();
    this->m_mails1.clear();
    this->m_mails2.clear();
    this->unknown_0008 = 0;
    this->unknown_000C = 0;
    this->unknown_0010 = false;
    this->unknown_0014 = 0;
    this->unknown_0018 = 0;
    this->unknown_001C = false;
    this->m_isValid = true;
  }

  static bool Create();
  static void Destroy();

  void Clear();
  void PurgeFlagged();

  /* 0000 */ bool m_isValid;
  /* 0004 */ int unknown_0004;
  union {
    struct {
      /* 0008 */ int unknown_0008;
      /* 000C */ int unknown_000C;
    };
    /* 0008 */ std::int64_t currentMailId;
  };
  /* 0010 */ unsigned char unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ bool unknown_001C;
  /* 0020 */ std::map<std::int64_t, RodexMail> m_mails0;
  /* 0028 */ std::map<std::int64_t, RodexMail> m_mails1;
  /* 0030 */ std::map<std::int64_t, RodexMail> m_mails2;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RodexMail) == 0x198);
static_assert(offsetof(RodexMail, unknown_0024) == 0x24);
static_assert(offsetof(RodexMail, zeny) == 0x188);
static_assert(offsetof(RodexMail, unknown_0190) == 0x190);
static_assert(offsetof(RodexMail, unknown_0194) == 0x194);
static_assert(sizeof(RodexMailStore) == 0x38);
static_assert(offsetof(RodexMailStore, m_mails0) == 0x20);
#endif

extern RodexMailStore* g_rodexMailStore;

#endif
