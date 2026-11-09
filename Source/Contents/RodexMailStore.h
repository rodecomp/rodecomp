#ifndef RAGNAROK_SOURCE_CONTENTS_RODEXMAILSTORE_H
#define RAGNAROK_SOURCE_CONTENTS_RODEXMAILSTORE_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

struct RodexMail {
  /* 0000 */ unsigned char isRead;
  /* 0001 */ unsigned char type;
  /* 0002 */ std::array<std::byte, 2> unknown_0002;
  /* 0004 */ std::string sender;
  /* 001C */ int sentAt;
  /* 0020 */ int expiresAt;
  /* 0024 */ std::string title;
  /* 003C */ std::string body;
  /* 0054 */ unsigned char hasItem;
  /* 0055 */ std::array<std::byte, 0x133> attachments;
  /* 0188 */ std::int64_t zeny;
  /* 0190 */ std::uint8_t isReturned = 0;
  /* 0191 */ std::array<std::byte, 3> unknown_0191;
  /* 0194 */ int isKnownSender = -1;
};

typedef std::map<std::int64_t, RodexMail, std::greater<std::int64_t>> RodexMailMap;

class RodexMailStore {
public:
  // INLINED
  RodexMailStore() {
    this->m_isValid = false;
    this->m_mails0.clear();
    this->m_mails1.clear();
    this->m_mails2.clear();
    this->currentMailIdLow = 0;
    this->currentMailIdHigh = 0;
    this->inboxType = false;
    this->dailySendCount = 0;
    this->unreadCount = 0;
    this->isPageComplete = false;
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
      /* 0008 */ int currentMailIdLow;
      /* 000C */ int currentMailIdHigh;
    };
    /* 0008 */ std::int64_t currentMailId;
  };
  /* 0010 */ unsigned char inboxType;
  /* 0014 */ int dailySendCount;
  /* 0018 */ int unreadCount;
  /* 001C */ bool isPageComplete;
  /* 0020 */ RodexMailMap m_mails0;
  /* 0028 */ RodexMailMap m_mails1;
  /* 0030 */ RodexMailMap m_mails2;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RodexMail) == 0x198);
static_assert(offsetof(RodexMail, title) == 0x24);
static_assert(offsetof(RodexMail, zeny) == 0x188);
static_assert(offsetof(RodexMail, isReturned) == 0x190);
static_assert(offsetof(RodexMail, isKnownSender) == 0x194);
static_assert(sizeof(RodexMailStore) == 0x38);
static_assert(offsetof(RodexMailStore, m_mails0) == 0x20);
#endif

extern RodexMailStore* g_rodexMailStore;

#endif
