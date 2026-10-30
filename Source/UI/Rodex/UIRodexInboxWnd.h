#ifndef RAGNAROK_SOURCE_UI_RODEX_UIRODEXINBOXWND_H
#define RAGNAROK_SOURCE_UI_RODEX_UIRODEXINBOXWND_H

#include "Contents/RodexMailStore.h"
#include "UI/Item/UIItemWnd.h"

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <map>
#include <string>
#include <utility>
#include <vector>

class UIBasicButton;
class UIBitmapButton;
class UICheckBox;
class UITextButton;

using RodexMailEntry = std::pair<int, std::int64_t>;

// INLINED
inline std::map<std::int64_t, RodexMail>* RodexMailsOfType(int type) {
  switch (type) {
  case 0:
    return &g_rodexMailStore->m_mails0;
  case 1:
    return &g_rodexMailStore->m_mails1;
  case 2:
    return &g_rodexMailStore->m_mails2;
  default:
    return nullptr;
  }
}

struct RodexMailEntryLess {
  // INLINED
  bool operator()(const RodexMailEntry& left, const RodexMailEntry& right) const {
    std::map<std::int64_t, RodexMail>* leftMails = RodexMailsOfType(left.first);
    if (!leftMails) {
      return false;
    }
    std::map<std::int64_t, RodexMail>::iterator leftMail = leftMails->find(left.second);
    if (leftMail == leftMails->end()) {
      return false;
    }
    std::map<std::int64_t, RodexMail>* rightMails = RodexMailsOfType(right.first);
    if (!rightMails) {
      return false;
    }
    std::map<std::int64_t, RodexMail>::iterator rightMail = rightMails->find(right.second);
    if (rightMail == rightMails->end()) {
      return false;
    }
    if (leftMail->second.isRead == 0) {
      if (rightMail->second.isRead == 1) {
        return true;
      }
    } else if (rightMail->second.isRead == 0) {
      return false;
    }
    return leftMail->second.unknown_001C > rightMail->second.unknown_001C;
  }
};

class UIRodexInboxWnd : public UIItemWnd {
public:
  bool Slot03() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Nullsub257();

  void Slot28(int x, int y) override;

  void DrawMailIcons(int row, unsigned char attachments, unsigned char read, unsigned char returned);
  void SetButtonsDisabled(unsigned char disabled);
  void DrawExpireTime(int row, __time32_t sendTime, __time32_t expireTime, unsigned char returned);
  void ClearChecks();
  void RefreshMailLists();
  void RemoveInboxMail(int id);
  void SearchBySender(std::string keyword);
  void SearchByTitle(std::string keyword);
  void InitializeMailActions();

  /* 012C */ unsigned char unknown_012C[0x4];
  /* 0130 */ int unknown_0130;
  /* 0134 */ int unknown_0134;
  /* 0138 */ int unknown_0138;
  /* 013C */ UIBitmapButton* unknown_013C;
  /* 0140 */ UIBitmapButton* unknown_0140;
  /* 0144 */ UITextButton* unknown_0144[6];
  /* 015C */ UITextButton* unknown_015C[6];
  /* 0174 */ UICheckBox* checks[6];
  /* 018C */ UIBasicButton* unknown_018C;
  /* 0190 */ UIBasicButton* unknown_0190;
  /* 0194 */ UIBitmapButton* unknown_0194;
  /* 0198 */ UIBitmapButton* unknown_0198;
  /* 019C */ int unknown_019C;
  /* 01A0 */ int unknown_01A0;
  /* 01A4 */ unsigned char expired;
  /* 01A8 */ UIBitmapButton* unknown_01A8[6];
  /* 01C0 */ UIWindow* unknown_01C0;
  /* 01C4 */ UIWindow* unknown_01C4;
  /* 01C8 */ UIWindow* unknown_01C8;
  /* 01CC */ int unknown_01CC;
  /* 01D0 */ int unknown_01D0;
  /* 01D4 */ unsigned char unknown_01D4;
  /* 01D8 */ std::vector<RodexMailEntry> searchResults;
  /* 01E4 */ std::vector<RodexMailEntry> mails0;
  /* 01F0 */ std::vector<RodexMailEntry> mails1;
  /* 01FC */ std::vector<RodexMailEntry> mails2;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RodexMailEntry) == 0x10);
static_assert(offsetof(UIRodexInboxWnd, unknown_012C) == 0x12C);
static_assert(offsetof(UIRodexInboxWnd, checks) == 0x174);
static_assert(offsetof(UIRodexInboxWnd, expired) == 0x1A4);
static_assert(offsetof(UIRodexInboxWnd, searchResults) == 0x1D8);
static_assert(offsetof(UIRodexInboxWnd, mails2) == 0x1FC);
#endif

#endif
