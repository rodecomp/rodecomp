#ifndef RAGNAROK_SOURCE_UI_RODEX_UIRODEXINBOXWND_H
#define RAGNAROK_SOURCE_UI_RODEX_UIRODEXINBOXWND_H

#include "Contents/Rodex/RodexMailStore.h"
#include "UI/Items/UIItemWnd.h"

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <map>
#include <string>
#include <utility>
#include <vector>

class UIBasicButton;
class UIBitmapButton;
class UIBmpRadioBtn;
class UICheckBox;
class UIEditCtrl;
class UIRadioBtn;
class UITextButton;

using RodexMailEntry = std::pair<int, std::int64_t>;

// INLINED
inline RodexMailMap* RodexMailsOfType(int type) {
  if (type == 0) {
    return &g_rodexMailStore->m_mails0;
  }
  if (type == 1) {
    return &g_rodexMailStore->m_mails1;
  }
  if (type == 2) {
    return &g_rodexMailStore->m_mails2;
  }
  return nullptr;
}

struct RodexMailEntryLess {
  bool operator()(const RodexMailEntry& left, const RodexMailEntry& right) const;
};

class UIRodexInboxWnd : public UIItemWnd {
public:
  UIRodexInboxWnd();
  bool NeedsFrameRefresh() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnProcess() override;

  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void DrawMailIcons(int row, unsigned char attachments, unsigned char read, unsigned char returned);
  void SetButtonsDisabled(unsigned char disabled);
  void SetMailRow(int row, std::int64_t mailId, std::string name, std::string subject, unsigned char read, unsigned char returned, int noAttachment);
  void DrawExpireTime(int row, __time32_t sendTime, __time32_t expireTime, unsigned char returned);
  void ClearChecks();
  void RefreshMailLists();
  void RemoveInboxMail(int id);
  void SearchBySender(std::string keyword);
  void SearchByTitle(std::string keyword);
  void InitializeMailActions();
  void RequestAllMailActions();
  void RequestMailAttachments(int type, std::int64_t mailId);
  bool RequestMailAction(int type, std::int64_t mailId);

  /* 0130 */ UIBmpRadioBtn* mailTab;
  /* 0134 */ UIBmpRadioBtn* accountTab;
  /* 0138 */ UIBmpRadioBtn* returnTab;
  /* 013C */ UIBitmapButton* refreshButton;
  /* 0140 */ UIBitmapButton* writeButton;
  /* 0144 */ UITextButton* senderButtons[6];
  /* 015C */ UITextButton* subjectButtons[6];
  /* 0174 */ UICheckBox* checks[6];
  /* 018C */ UIBasicButton* receiveButton;
  /* 0190 */ UIBasicButton* deleteButton;
  /* 0194 */ UIBitmapButton* previousPageButton;
  /* 0198 */ UIBitmapButton* nextPageButton;
  /* 019C */ int page;
  /* 01A0 */ int pageCount;
  /* 01A4 */ unsigned char expired;
  /* 01A8 */ UIBitmapButton* returnButtons[6];
  /* 01C0 */ UIRadioBtn* searchModeButtons[2];
  /* 01C8 */ UIEditCtrl* searchEdit;
  /* 01CC */ UIBitmapButton* searchButton;
  /* 01D0 */ UIBmpRadioBtn* searchTab;
  /* 01D4 */ unsigned char searchActive;
  /* 01D8 */ std::vector<RodexMailEntry> searchResults;
  /* 01E4 */ std::vector<RodexMailEntry> mails0;
  /* 01F0 */ std::vector<RodexMailEntry> mails1;
  /* 01FC */ std::vector<RodexMailEntry> mails2;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RodexMailEntry) == 0x10);
static_assert(offsetof(UIRodexInboxWnd, filterButton) == 0x12C);
static_assert(offsetof(UIRodexInboxWnd, checks) == 0x174);
static_assert(offsetof(UIRodexInboxWnd, expired) == 0x1A4);
static_assert(offsetof(UIRodexInboxWnd, searchResults) == 0x1D8);
static_assert(offsetof(UIRodexInboxWnd, mails2) == 0x1FC);
#endif

#endif
