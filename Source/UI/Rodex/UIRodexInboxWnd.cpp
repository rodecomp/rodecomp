#include "UI/Rodex/UIRodexInboxWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Contents/Tables/MessageTable.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/NoticeEnums.h"
#include "UI/Window/UIWindowMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstring>
#include <list>

namespace {
#pragma pack(push, 1)
struct PACKET_CZ_OPEN_RODEXBOX {
  std::int16_t packetType;
  std::int64_t lastMailId0;
  std::int64_t lastMailId2;
  std::int64_t lastMailId1;
};

struct PACKET_CZ_REQ_REFRESH_RODEX {
  std::int16_t packetType;
  std::int64_t lastMailId0;
  std::int64_t lastMailId2;
  std::int64_t lastMailId1;
};

struct RodexMailRequestPacket {
  std::int16_t id;
  std::int64_t mailId;
  std::uint8_t type;
};

struct PACKET_CZ_RODEX_RETURN {
  std::int16_t packetType;
  std::uint32_t mailId;
};
#pragma pack(pop)
}

// OFFSET: 0x00804D50
// FUNCTION: fn_10038
UIRodexInboxWnd::UIRodexInboxWnd()
    : UIItemWnd(), mailTab(0), accountTab(0), returnTab(0), refreshButton(nullptr), writeButton(nullptr),
      receiveButton(nullptr), deleteButton(nullptr), previousPageButton(nullptr), nextPageButton(nullptr), page(0), pageCount(0),
      expired(0), searchEdit(nullptr), searchButton(0), searchTab(0), searchActive(0), searchResults(), mails0(), mails1(), mails2() {
  SecureZeroMemory(this->senderButtons, sizeof(this->senderButtons));
  SecureZeroMemory(this->subjectButtons, sizeof(this->subjectButtons));
  SecureZeroMemory(this->checks, sizeof(this->checks));
  SecureZeroMemory(this->returnButtons, sizeof(this->returnButtons));
  g_rodexMailStore->isPageComplete = false;
  PACKET_CZ_OPEN_RODEXBOX packet;
  packet.packetType = ID_CZ_OPEN_ALL_RODEX;
  packet.lastMailId0 = 0;
  if (g_rodexMailStore->m_mails0.size() > 0) {
    packet.lastMailId0 = g_rodexMailStore->m_mails0.rbegin()->first;
  }
  packet.lastMailId2 = 0;
  if (g_rodexMailStore->m_mails2.size() > 0) {
    packet.lastMailId2 = g_rodexMailStore->m_mails2.rbegin()->first;
  }
  packet.lastMailId1 = 0;
  if (g_rodexMailStore->m_mails1.size() > 0) {
    packet.lastMailId1 = g_rodexMailStore->m_mails1.rbegin()->first;
  }
  GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
}

// OFFSET: 0x00807640
// FUNCTION: fn_10049
void UIRodexInboxWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x00807660
// FUNCTION: fn_10050
void UIRodexInboxWnd::OnMouseHover(int, int) {}

// OFFSET: 0x00807670
// FUNCTION: fn_10051
void UIRodexInboxWnd::OnProcess() {}

// OFFSET: 0x00807680
// FUNCTION: fn_10052
void UIRodexInboxWnd::OnRBtnDown(int, int) {}

// OFFSET: 0x00805750
// FUNCTION: fn_10045
void UIRodexInboxWnd::DrawMailIcons(int row, unsigned char attachments, unsigned char read, unsigned char returned) {
  const int y = row * 0x2C;
  const char* status = returned == 1
    ? "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_status_mail_returned.bmp"
    : read != 0
      ? "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_status_mail_read.bmp"
      : "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_status_mail_received.bmp";
  CBitmapRes* statusIcon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(status)));
  this->DrawBitmap(0x24, y + 0x7C, statusIcon, true);

  if ((attachments & 6) == 6) {
    this->DrawBitmap(0xD8, y + 0x8B, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))), true);
    this->DrawBitmap(0xDB, y + 0x7F, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny_n_item.bmp"))), true);
  } else if ((attachments & 2) != 0) {
    this->DrawBitmap(0xD8, y + 0x8B, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))), true);
    this->DrawBitmap(0xDB, y + 0x7F, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny.bmp"))), true);
  } else if ((attachments & 4) != 0) {
    this->DrawBitmap(0xD8, y + 0x8B, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))), true);
    this->DrawBitmap(0xDB, y + 0x7F, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_item.bmp"))), true);
  }
}

// OFFSET: 0x008058B0
// FUNCTION: fn_10046
bool UIRodexInboxWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x008058C0
// FUNCTION: fn_10047
void UIRodexInboxWnd::OnCreate(int width, int height) {
  g_rodexMailStore->PurgeFlagged();
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->returnTab = new UIBmpRadioBtn();
  this->returnTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->returnTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->returnTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  this->returnTab->SetLabel(LookupMessage(MSI_FAILED_HOMUN_AUTO_FEEDING_BY_ITEMACTION));
  {
    const int textX = static_cast<int>(static_cast<float>(this->returnTab->geometry.width - this->TextWidth(LookupMessage(MSI_FAILED_HOMUN_AUTO_FEEDING_BY_ITEMACTION), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    const int textY = static_cast<int>(static_cast<float>(this->returnTab->geometry.height - this->TextHeight(LookupMessage(MSI_FAILED_HOMUN_AUTO_FEEDING_BY_ITEMACTION), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    this->returnTab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->returnTab->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->returnTab->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->returnTab->Move(0xCF, 0x46);
  this->AddChild(this->returnTab);
  this->returnTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);

  this->accountTab = new UIBmpRadioBtn();
  this->accountTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->accountTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->accountTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  this->accountTab->SetLabel(LookupMessage(MSI_RODEX_TAB_SEARCH));
  {
    const int textX = static_cast<int>(static_cast<float>(this->accountTab->geometry.width - this->TextWidth(LookupMessage(MSI_RODEX_TAB_SEARCH), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    const int textY = static_cast<int>(static_cast<float>(this->accountTab->geometry.height - this->TextHeight(LookupMessage(MSI_RODEX_TAB_SEARCH), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    this->accountTab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->accountTab->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->accountTab->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->accountTab->Move(0x13, 0x46);
  this->AddChild(this->accountTab);
  this->accountTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);

  this->mailTab = new UIBmpRadioBtn();
  this->mailTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->mailTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->mailTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  this->mailTab->SetLabel(LookupMessage(MSI_FAILED_PET_AUTO_FEEDING_BY_ITEMACTION));
  {
    const int textX = static_cast<int>(static_cast<float>(this->mailTab->geometry.width - this->TextWidth(LookupMessage(MSI_FAILED_PET_AUTO_FEEDING_BY_ITEMACTION), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    const int textY = static_cast<int>(static_cast<float>(this->mailTab->geometry.height - this->TextHeight(LookupMessage(MSI_FAILED_PET_AUTO_FEEDING_BY_ITEMACTION), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    this->mailTab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->mailTab->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->mailTab->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->mailTab->Move(0x71, 0x46);
  this->AddChild(this->mailTab);
  this->mailTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);

  switch (g_rodexMailStore->inboxType) {
  case MAIL_INBOX_NORMAL:
    this->mailTab->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
    break;
  case MAIL_INBOX_ACCOUNT:
    this->accountTab->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
    break;
  case MAIL_INBOX_RETURNED:
    this->returnTab->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
    break;
  }

  this->refreshButton = new UIBitmapButton();
  this->refreshButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_refresh_out.bmp", 0, 0);
  this->refreshButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_refresh_over.bmp", 1, 0);
  this->refreshButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_refresh_press.bmp", 2, 0);
  this->refreshButton->SetTooltip(LookupMessage(MSI_RODEX_DELETE_TIME_DAY));
  this->refreshButton->SetSize(this->refreshButton->BitmapWidth(), this->refreshButton->BitmapHeight());
  this->refreshButton->Move(0xEF, 0x14);
  this->refreshButton->SetId(0x19F);
  this->AddChild(this->refreshButton);

  this->writeButton = new UIBitmapButton();
  this->writeButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_write_out.bmp", 0, 0);
  this->writeButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_write_over.bmp", 1, 0);
  this->writeButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_write_press.bmp", 2, 0);
  this->writeButton->SetTooltip(LookupMessage(MSI_MAIL_WRITE));
  this->writeButton->SetSize(this->writeButton->BitmapWidth(), this->writeButton->BitmapHeight());
  this->writeButton->Move(0x10C, 0x12);
  this->writeButton->SetId(0x131);
  this->AddChild(this->writeButton);

  for (int y = 0x66, row = 0; y < 0x16E; y += 0x2C, ++row) {
    this->checks[row] = new UICheckBox();
    this->checks[row]->SetSize(14, 14);
    this->checks[row]->SetBitmaps("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_on.bmp", "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_off.bmp");
    this->checks[row]->Move(0x11, y + 6);
    this->checks[row]->SetChecked(0);
    this->checks[row]->m_message = 0;
    this->checks[row]->Show(0);
    this->AddChild(this->checks[row]);

    this->subjectButtons[row] = new UITextButton();
    this->subjectButtons[row]->Configure("", 0, 0, 12, 0, 0);
    this->subjectButtons[row]->m_hoverColor = 0x51417E;
    this->subjectButtons[row]->Move(0x45, y);
    this->subjectButtons[row]->SetId(0x143);
    this->subjectButtons[row]->SetTooltip(LookupMessage(MSI_TAIWAN_CASH_MSG_2));
    this->subjectButtons[row]->Enable(0);
    this->AddChild(this->subjectButtons[row]);

    this->senderButtons[row] = new UITextButton();
    this->senderButtons[row]->Configure("", 0x844A42, 0, 11, 1, 0);
    this->senderButtons[row]->m_hoverColor = 0x51417E;
    this->senderButtons[row]->Move(0x47, y + 0x10);
    this->senderButtons[row]->SetId(0x142);
    this->senderButtons[row]->SetTooltip(LookupMessage(MSI_TAIWAN_CASH_MSG_1));
    this->senderButtons[row]->Enable(0);
    this->AddChild(this->senderButtons[row]);

    this->returnButtons[row] = new UIBitmapButton();
    this->returnButtons[row]->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_return_out.bmp", 0, 0);
    this->returnButtons[row]->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_return_over.bmp", 1, 0);
    this->returnButtons[row]->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_return_press.bmp", 2, 0);
    this->returnButtons[row]->SetSize(this->returnButtons[row]->BitmapWidth(), this->returnButtons[row]->BitmapHeight());
    this->returnButtons[row]->Move(0xBA, y + 0x1D);
    this->returnButtons[row]->SetTooltip(LookupMessage(MSI_RODEX_DELETE_TIME_HOUR));
    this->returnButtons[row]->SetId(0x136);
    this->returnButtons[row]->Enable(0);
    this->AddChild(this->returnButtons[row]);
  }

  UIBasicButton* button = new UIBasicButton();
  this->receiveButton = button;
  button->Configure(0x46, LookupMessage(MSI_RODEX_BUTTON_RECEIVE_ALL));
  this->receiveButton->SetId(0x222);
  this->receiveButton->Move(4, 0x178);
  this->AddChild(this->receiveButton);

  button = new UIBasicButton();
  this->deleteButton = button;
  button->Configure(0x46, LookupMessage(MSI_HOTKEYWND_SKILLBAR2));
  this->deleteButton->Move(0x54, 0x188);
  this->deleteButton->SetId(0x224);
  this->AddChild(this->deleteButton);

  this->previousPageButton = new UIBitmapButton();
  this->previousPageButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_prev_out.bmp", 0, 0);
  this->previousPageButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_prev_over.bmp", 1, 0);
  this->previousPageButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_prev_press.bmp", 2, 0);
  this->previousPageButton->SetTooltip(LookupMessage(MSI_BOOK_PREV));
  this->previousPageButton->SetSize(this->previousPageButton->BitmapWidth(), this->previousPageButton->BitmapHeight());
  this->previousPageButton->Move(0xF6, 0x176);
  this->previousPageButton->SetId(0x13C);
  this->AddChild(this->previousPageButton);

  this->nextPageButton = new UIBitmapButton();
  this->nextPageButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_next_out.bmp", 0, 0);
  this->nextPageButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_next_over.bmp", 1, 0);
  this->nextPageButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_next_press.bmp", 2, 0);
  this->nextPageButton->SetTooltip(LookupMessage(MSI_BOOK_NEXT));
  this->nextPageButton->SetSize(this->nextPageButton->BitmapWidth(), this->nextPageButton->BitmapHeight());
  this->nextPageButton->Move(0x10E, 0x176);
  this->nextPageButton->SetId(0x13D);
  this->AddChild(this->nextPageButton);

  const int searchTexts[2] = {0xC78, 0xC79};
  for (int x = 0x16, index = 0; x < 0x7C; x += 0x33, ++index) {
    this->searchModeButtons[index] = new UIRadioBtn();
    this->searchModeButtons[index]->SetSize(0x33, 12);
    this->searchModeButtons[index]->SetBitmaps("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_search_on.bmp", "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_search_off.bmp");
    this->searchModeButtons[index]->SetText(LookupMessage(searchTexts[index]));
    this->searchModeButtons[index]->textColor = 0x654939;
    this->searchModeButtons[index]->Move(x, 0x44);
    this->searchModeButtons[index]->value = 0x141;
    this->searchModeButtons[index]->SendMsg(this, UM_SETSTATE, 1 - index, 0, 0, 0);
    this->AddChild(this->searchModeButtons[index]);
  }

  this->searchEdit = new UIEditCtrl();
  this->searchEdit->SetSize(0x84, 0x12);
  this->searchEdit->Move(0x85, 0x41);
  this->searchEdit->maximum = 0x10;
  this->searchEdit->flat = 0;
  this->searchEdit->Border(0xAD, 0xAD, 0xAD);
  this->searchEdit->preserveInput = 1;
  this->AddChild(this->searchEdit);

  this->searchButton = new UIBitmapButton();
  this->searchButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_search_out.bmp", 0, 0);
  this->searchButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_search_over.bmp", 1, 0);
  this->searchButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_search_press.bmp", 2, 0);
  this->searchButton->SetSize(this->searchButton->BitmapWidth(), this->searchButton->BitmapHeight());
  this->searchButton->Move(0x108, 0x41);
  this->searchButton->SetId(0x141);
  this->AddChild(this->searchButton);

  this->searchTab = new UIBmpRadioBtn();
  this->searchTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->searchTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->searchTab->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  this->searchTab->SetLabel(LookupMessage(MSI_MSI_INVENTORY_TABNAME_4));
  {
    const int textX = static_cast<int>(static_cast<float>(this->searchTab->geometry.width - this->TextWidth(LookupMessage(MSI_MSI_INVENTORY_TABNAME_4), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    const int textY = static_cast<int>(static_cast<float>(this->searchTab->geometry.height - this->TextHeight(LookupMessage(MSI_MSI_INVENTORY_TABNAME_4), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    this->searchTab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->searchTab->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->searchTab->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->searchTab->Move(0xDF, 0x56);
  this->AddChild(this->searchTab);
  this->searchTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);

  this->accountTab->Move(0x13, 0x56);
  this->mailTab->Move(0x57, 0x56);
  this->returnTab->Move(0x9B, 0x56);
  this->refreshButton->Move(0xF8, 0x14);
  this->writeButton->Move(0x115, 0x12);
  for (int y = 0x79, row = 0; y < 0x181; y += 0x2C, ++row) {
    this->checks[row]->Move(0x11, y + 7);
    this->subjectButtons[row]->Move(0x45, y);
    this->senderButtons[row]->Move(0x47, y + 0x14);
  }
  this->receiveButton->Move(4, 0x188);
  this->previousPageButton->Move(0xFF, 0x186);
  this->nextPageButton->Move(0x117, 0x186);
  this->RefreshMailLists();
  this->Invalidate();
}

// OFFSET: 0x00806D40
// FUNCTION: fn_10048
void UIRodexInboxWnd::Paint() {
  this->ClearSurface(0xFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_rodex_list.bmp"))), false);
  for (int i = 0; i < 6; ++i) {
    this->returnButtons[i]->Enable(0);
  }

  if (!g_rodexMailStore->isPageComplete) {
    this->DrawBitmap(0x1F, 0x9A, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\img_notice.bmp"))), true);
    this->DrawWrappedFormattedText(0x1F, 0xE5, std::string(LookupMessage(MSI_RODEX_SENDER)), 0xF8, 3, 6, 0, 12, 0, 0);
    this->DrawCenteredText(0x1F, 0x10E, LookupMessage(MSI_CLOSE_SETUP), 0xF8, 0, 0x12, 0xC89482, 1);
    return;
  }

  unsigned int index = this->page * 6;
  if (this->searchActive == 1) {
    for (int row = 0; row < 6; ++row, ++index) {
      if (index < this->searchResults.size()) {
        RodexMailMap* mails = RodexMailsOfType(this->searchResults[index].first);
        if (mails) {
          RodexMailMap::iterator it = mails->find(this->searchResults[index].second);
          if (it != mails->end()) {
            this->DrawMailIcons(row, it->second.type, it->second.isRead, this->searchResults[index].first == 2);
            this->DrawExpireTime(row, it->second.sentAt, it->second.expiresAt, this->searchResults[index].first == 2);
            if (this->expired == 1) {
              this->expired = 0;
              mails->erase(it);
            } else {
              this->SetMailRow(row, this->searchResults[index].second, it->second.sender, it->second.title, it->second.isRead,
                               it->second.isReturned, it->second.isKnownSender);
            }
          }
        }
      } else {
        this->checks[row]->Show(0);
        this->checks[row]->SetChecked(0);
        this->checks[row]->m_message = 0;
        this->senderButtons[row]->Enable(0);
        this->subjectButtons[row]->Enable(0);
      }
    }
  } else {
    for (int row = 0; row < 6; ++row, ++index) {
      this->checks[row]->Show(0);
      this->senderButtons[row]->Enable(0);
      this->subjectButtons[row]->Enable(0);
      switch (g_rodexMailStore->inboxType) {
        case MAIL_INBOX_ACCOUNT:
          if (index < this->mails1.size()) {
            RodexMailMap::iterator it = g_rodexMailStore->m_mails1.find(this->mails1[index].second);
            if (it != g_rodexMailStore->m_mails1.end()) {
              this->DrawMailIcons(row, it->second.type, it->second.isRead, 0);
              this->DrawExpireTime(row, it->second.sentAt, it->second.expiresAt, 0);
              if (this->expired == 1) {
                this->expired = 0;
                g_rodexMailStore->m_mails1.erase(it);
              } else {
                this->checks[row]->Show(1);
                this->senderButtons[row]->Enable(1);
                this->subjectButtons[row]->Enable(1);
                this->SetMailRow(row, it->first, it->second.sender, it->second.title, it->second.isRead, it->second.isReturned, -1);
              }
            }
          }
          break;
        case MAIL_INBOX_RETURNED:
          if (index < this->mails2.size()) {
            RodexMailMap::iterator it = g_rodexMailStore->m_mails2.find(this->mails2[index].second);
            if (it != g_rodexMailStore->m_mails2.end()) {
              this->DrawMailIcons(row, it->second.type, it->second.isRead, 0);
              this->DrawExpireTime(row, it->second.sentAt, it->second.expiresAt, 0);
              if (this->expired == 1) {
                this->expired = 0;
                g_rodexMailStore->m_mails2.erase(it);
              } else {
                this->checks[row]->Show(1);
                this->senderButtons[row]->Enable(1);
                this->subjectButtons[row]->Enable(1);
                this->SetMailRow(row, it->first, it->second.sender, it->second.title, it->second.isRead, it->second.isReturned, -1);
              }
            }
          }
          break;
        default:
          this->senderButtons[row]->Enable(0);
          this->subjectButtons[row]->Enable(0);
          if (index < this->mails0.size()) {
            RodexMailMap::iterator it = g_rodexMailStore->m_mails0.find(this->mails0[index].second);
            if (it != g_rodexMailStore->m_mails0.end()) {
              this->DrawMailIcons(row, it->second.type, it->second.isRead, 0);
              this->DrawExpireTime(row, it->second.sentAt, it->second.expiresAt, 0);
              if (this->expired == 1) {
                this->expired = 0;
                g_rodexMailStore->m_mails0.erase(it);
              } else {
                this->SetMailRow(row, it->first, it->second.sender, it->second.title, it->second.isRead, it->second.isReturned,
                                 it->second.isKnownSender);
              }
            }
          }
      }
    }
  }

  this->DrawRightAlignedText(0x111, 0x2E, FormatOwnedString("%d", g_rodexMailStore->dailySendCount).c_str(), 0, 0, 11, 0xC89482, 1);
  this->DrawTextAt(0x111, 0x2E, LookupMessage(MSI_EMBLEM_CHANGE), 0, 0, 11, 0xC89482, 1, 0);
}

// OFFSET: 0x00808FD0
// FUNCTION: fn_10056
void UIRodexInboxWnd::SetButtonsDisabled(unsigned char disabled) {
  UIButton* button = this->previousPageButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->previousPageButton->Invalidate();

  button = this->nextPageButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->nextPageButton->Invalidate();

  button = this->refreshButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->refreshButton->Invalidate();

  button = this->receiveButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->receiveButton->Invalidate();

  button = this->writeButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->writeButton->Invalidate();

  button = this->deleteButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->deleteButton->Invalidate();

  this->Invalidate();
}

// OFFSET: 0x008090C0
// FUNCTION: fn_10057
void UIRodexInboxWnd::SetMailRow(int row, std::int64_t mailId, std::string name, std::string subject, unsigned char read, unsigned char returned, int noAttachment) {
  std::string nameText = name;
  std::string subjectText = subject;
  const std::pair<bool, std::string> stripped = GetNavigationLinkParser()->RemoveLineBreakTag(subjectText);
  subjectText = stripped.second;
  if (stripped.first == true) {
    this->senderButtons[row]->SetTooltip("");
  }

  nameText = GetNavigationLinkParser()->ExpandMessages(nameText);
  subjectText = GetNavigationLinkParser()->ExpandMessages(subjectText);

  this->checks[row]->Show(1);
  this->checks[row]->m_message = static_cast<int>(mailId);

  this->senderButtons[row]->SetLabel(nameText.c_str());
  this->senderButtons[row]->ResizeToTextClipped(0x8C, this->senderButtons[row]->geometry.height);
  this->senderButtons[row]->m_value = static_cast<int>(mailId);
  this->senderButtons[row]->Enable(1);

  std::uint32_t color = 0;
  if (noAttachment == 0) {
    this->returnButtons[row]->Enable(1);
    this->returnButtons[row]->m_argument = static_cast<int>(mailId);
    color = read == 1 ? 0xC8C8E6 : 0xFF;
  } else if (read == 1) {
    color = 0xC8C8C8;
  }

  this->subjectButtons[row]->m_color = color;
  this->subjectButtons[row]->SetLabel(subjectText.c_str());
  this->subjectButtons[row]->ResizeToTextClipped(0x8C, this->subjectButtons[row]->geometry.height);
  this->subjectButtons[row]->m_value = static_cast<int>(mailId);
  this->subjectButtons[row]->Enable(1);

  if (returned == 1) {
    this->checks[row]->m_message = -1;
    this->senderButtons[row]->Enable(0);
    this->subjectButtons[row]->m_color = 0xC8C8C8;
    this->subjectButtons[row]->SetLabel(LookupMessage(MSI_THAI_PERSONALINFO_DROP));
    this->subjectButtons[row]->ResizeToTextClipped(0x8C, this->subjectButtons[row]->geometry.height);
    this->subjectButtons[row]->m_value = -1;
    this->subjectButtons[row]->Enable(1);
  }
}

// OFFSET: 0x00809670
// FUNCTION: fn_10058
void UIRodexInboxWnd::DrawExpireTime(int row, __time32_t sendTime, __time32_t expireTime, unsigned char returned) {
  const __time32_t now = _time32(nullptr);
  _difftime32(now, sendTime);
  int remaining = static_cast<int>(_difftime32(expireTime, _time32(nullptr)));
  if (remaining <= 0) {
    remaining = 0;
  }
  if (remaining == 0) {
    this->expired = 1;
    return;
  }

  std::string text;
  if (remaining / 86400 > 0) {
    const std::string formatted = FormatOwnedString(LookupMessage(MSI_MINIMAP_TIP_POS_ON), remaining / 86400);
    text = formatted;
  } else if (remaining / 3600 > 0) {
    const std::string formatted = FormatOwnedString(LookupMessage(MSI_RODEX_DELETE_SELECT_MAIL), remaining / 3600);
    text = formatted;
  } else {
    const std::string formatted = FormatOwnedString(LookupMessage(MSI_RODEX_RETURNED), remaining / 60);
    text = formatted;
  }

  if (returned == 1) {
    this->DrawCenteredText(0x104, row * 0x2C + 0x7D, LookupMessage(MSI_MINIMAP_TIP_ZOOM_IN), 0, 0, 11, 0x666673, 0);
    this->DrawCenteredText(0x104, row * 0x2C + 0x8A, text.c_str(), 0, 0, 11, 0x666673, 0);
  } else {
    this->DrawCenteredText(0x104, row * 0x2C + 0x82, text.c_str(), 0, 0, 11, 0xA0AE9, 1);
  }
}

// OFFSET: 0x008098D0
// FUNCTION: fn_10059
void UIRodexInboxWnd::ClearChecks() {
  ++this->page;
  for (int i = 0; i < 6; ++i) {
    this->checks[i]->SetChecked(0);
    this->checks[i]->m_message = 0;
  }
  this->Invalidate();
}

// OFFSET: 0x00809990
// FUNCTION: fn_10061
void UIRodexInboxWnd::RefreshMailLists() {
  this->mails0.clear();
  this->mails2.clear();
  this->mails1.clear();

  for (RodexMailMap::iterator it = g_rodexMailStore->m_mails0.begin(); it != g_rodexMailStore->m_mails0.end(); ++it) {
    this->mails0.push_back(RodexMailEntry(0, it->first));
  }
  std::sort(this->mails0.begin(), this->mails0.end(), RodexMailEntryLess());

  for (RodexMailMap::iterator it = g_rodexMailStore->m_mails2.begin(); it != g_rodexMailStore->m_mails2.end(); ++it) {
    this->mails2.push_back(RodexMailEntry(2, it->first));
  }
  std::sort(this->mails2.begin(), this->mails2.end(), RodexMailEntryLess());

  for (RodexMailMap::iterator it = g_rodexMailStore->m_mails1.begin(); it != g_rodexMailStore->m_mails1.end(); ++it) {
    this->mails1.push_back(RodexMailEntry(1, it->first));
  }
  std::sort(this->mails1.begin(), this->mails1.end(), RodexMailEntryLess());
}

// OFFSET: 0x00809B60
// FUNCTION: fn_10062
void UIRodexInboxWnd::RemoveInboxMail(int id) {
  for (std::vector<RodexMailEntry>::iterator it = this->mails0.begin(); it != this->mails0.end(); ++it) {
    if (id == it->second) {
      this->mails0.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00809C80
// FUNCTION: fn_10067
void UIRodexInboxWnd::SearchBySender(const std::string keyword) {
  this->searchResults.clear();

  std::vector<RodexMailEntry>& results = this->searchResults;
  std::for_each(g_rodexMailStore->m_mails0.begin(), g_rodexMailStore->m_mails0.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.sender.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(0, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails1.begin(), g_rodexMailStore->m_mails1.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.sender.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(1, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails2.begin(), g_rodexMailStore->m_mails2.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.sender.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(2, mail.first));
    }
  });

  std::sort(this->searchResults.begin(), this->searchResults.end(), RodexMailEntryLess());
}

// OFFSET: 0x00809F70
// FUNCTION: fn_10068
void UIRodexInboxWnd::SearchByTitle(const std::string keyword) {
  this->searchResults.clear();

  std::vector<RodexMailEntry>& results = this->searchResults;
  std::for_each(g_rodexMailStore->m_mails0.begin(), g_rodexMailStore->m_mails0.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.title.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(0, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails1.begin(), g_rodexMailStore->m_mails1.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.title.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(1, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails2.begin(), g_rodexMailStore->m_mails2.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.title.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(2, mail.first));
    }
  });

  std::sort(this->searchResults.begin(), this->searchResults.end(), RodexMailEntryLess());
}

// OFFSET: 0x00809910
// FUNCTION: fn_10060
void UIBasicButton::SetText(std::string value) {
  this->text = value;
  this->Invalidate();
}

// OFFSET: 0x00807690
// FUNCTION: fn_10053
void UIRodexInboxWnd::RequestMailAttachments(int type, std::int64_t mailId) {
  if (g_modeMgr.GetGameMode() != nullptr && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen() == true) {
    return;
  }

  RodexMailMap* mails = RodexMailsOfType(type);
  unsigned char flags = 0;
  if (mails != nullptr) {
    RodexMailMap::iterator mail = mails->find(mailId);
    if (mail != mails->end()) {
      flags = mail->second.type;
    }
  }

  RodexMailRequestPacket packet;
  if ((flags & 4) != 0) {
    packet.id = ID_CZ_REQ_ITEM_FROM_RODEX;
    packet.mailId = mailId;
    packet.type = static_cast<std::uint8_t>(type);
    GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
  }
  if ((flags & 2) != 0) {
    packet.id = ID_CZ_REQ_ZENY_FROM_RODEX;
    packet.mailId = mailId;
    packet.type = static_cast<std::uint8_t>(type);
    GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x008078E0
// FUNCTION: fn_10054
void UIRodexInboxWnd::InitializeMailActions() {
  this->page = 0;
  for (int i = 0; i < 6; ++i) {
    this->checks[i]->SetChecked(0);
    this->checks[i]->m_message = 0;
  }

  this->receiveButton->SetText(LookupMessage(MSI_RODEX_BUTTON_RECEIVE_ALL));
  this->receiveButton->SetId(0x222);
  this->deleteButton->SetText(LookupMessage(MSI_HOTKEYWND_SKILLBAR2));
  this->deleteButton->SetId(0x224);
  this->Invalidate();
}

// OFFSET: 0x00805350
// FUNCTION: fn_10041
bool RodexMailEntryLess::operator()(const RodexMailEntry& left, const RodexMailEntry& right) const {
  RodexMailMap* leftMails = RodexMailsOfType(left.first);
  if (!leftMails) {
    return false;
  }
  RodexMailMap::iterator leftMail = leftMails->find(left.second);
  if (leftMail == leftMails->end()) {
    return false;
  }
  RodexMailMap* rightMails = RodexMailsOfType(right.first);
  if (!rightMails) {
    return false;
  }
  RodexMailMap::iterator rightMail = rightMails->find(right.second);
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
  return leftMail->second.sentAt > rightMail->second.sentAt;
}

// OFFSET: 0x008054D0
// FUNCTION: fn_10043
bool UIRodexInboxWnd::RequestMailAction(int type, std::int64_t mailId) {
  RodexMailMap* mails = RodexMailsOfType(type);
  if (mails == nullptr) {
    return false;
  }

  RodexMailMap::iterator mail = mails->find(mailId);
  if (mail == mails->end() || (mail->second.type & 6) != 0) {
    return false;
  }

  g_modeMgr.GetGameMode()->SendMsg(MM_DELETE_MAIL, static_cast<int>(mailId), type, 0, 0);
  if (g_windowMgr.rodexContentsWnd != 0) {
    g_windowMgr.CloseWindow(WID_RODEXCONTENTSWND);
  }
  return true;
}

// OFFSET: 0x00805570
// FUNCTION: fn_10044
void UIRodexInboxWnd::RequestAllMailActions() {
  int requested = 0;
  int skipped = 0;
  if (this->searchActive == 1) {
    for (std::vector<RodexMailEntry>::iterator entry = this->searchResults.begin(); entry != this->searchResults.end(); ++entry) {
      RodexMailMap* mails = RodexMailsOfType(entry->first);
      if (mails == nullptr) {
        continue;
      }

      RodexMailMap::iterator mail = mails->find(entry->second);
      if (mail == mails->end()) {
        continue;
      }

      if ((mail->second.type & 6) == 0) {
        g_modeMgr.GetGameMode()->SendMsg(MM_DELETE_MAIL, static_cast<int>(entry->second), entry->first, 0, 0);
        ++requested;
      } else {
        ++skipped;
      }
    }
  } else {
    int type = g_rodexMailStore->inboxType;
    RodexMailMap* mails = RodexMailsOfType(type);
    if (mails == nullptr) {
      return;
    }

    for (RodexMailMap::iterator mail = mails->begin(); mail != mails->end(); ++mail) {
      if ((mail->second.type & 6) == 0) {
        g_modeMgr.GetGameMode()->SendMsg(MM_DELETE_MAIL, static_cast<int>(mail->first), type, 0, 0);
        ++requested;
      } else {
        ++skipped;
      }
    }
  }

  if (requested > 0 && g_windowMgr.rodexContentsWnd != 0) {
    g_windowMgr.CloseWindow(WID_RODEXCONTENTSWND);
  }
  if (skipped > 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILE_MAIL_RECIEVER_INFO)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00807AA0
// FUNCTION: fn_10055
int UIRodexInboxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (g_windowMgr.rodexComposeWnd != nullptr || g_windowMgr.rodexContentsWnd != nullptr) {
    return 0;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (g_rodexMailStore->isPageComplete == false && !(message == UM_COMMAND && argument1 == ID_CLOSE)) {
    return 0;
  }

  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_RADIOBTN:
      if (argument2 == 0x141) {
        for (int i = 0; i < 2; ++i) {
          UIRadioBtn* radio = this->searchModeButtons[i];
          if (radio == sender) {
            radio->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
          } else {
            radio->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
          }
        }
        this->Invalidate();
      } else {
        this->mailTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        this->accountTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        this->returnTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        this->searchTab->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        sender->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        for (int i = 0; i < 6; ++i) {
          this->checks[i]->SetChecked(0);
          this->checks[i]->m_message = 0;
        }
        this->page = 0;

        if (this->mailTab == sender) {
          g_rodexMailStore->inboxType = 0;
        } else if (this->accountTab == sender) {
          g_rodexMailStore->inboxType = 1;
        } else if (this->returnTab == sender) {
          g_rodexMailStore->inboxType = 2;
        }
        this->searchActive = this->searchTab == sender;
        this->Invalidate();
      }
      break;

    case ID_PREV_PAGE:
      if (--this->page < 0) {
        this->page = 0;
      } else {
        for (int i = 0; i < 6; ++i) {
          this->checks[i]->SetChecked(0);
          this->checks[i]->m_message = 0;
        }
      }
      this->Invalidate();
      break;

    case ID_NEXT_PAGE: {
      if (this->searchActive == 1) {
        if (this->searchResults.size() > static_cast<std::size_t>((this->page + 1) * 6)) {
          this->ClearChecks();
        }
      } else if (RodexMailsOfType(g_rodexMailStore->inboxType)->size() > static_cast<std::size_t>((this->page + 1) * 6)) {
        this->ClearChecks();
      }
      break;
    }

    case 0x19F: {
      g_rodexMailStore->PurgeFlagged();
      for (int i = 0; i < 6; ++i) {
        this->checks[i]->SetChecked(0);
        this->checks[i]->m_message = 0;
      }

      PACKET_CZ_REQ_REFRESH_RODEX packet;
      packet.packetType = ID_CZ_UPDATE_ALL_RODEX;
      packet.lastMailId0 = 0;
      if (g_rodexMailStore->m_mails0.size() > 0) {
        packet.lastMailId0 = g_rodexMailStore->m_mails0.rbegin()->first;
      }
      packet.lastMailId2 = 0;
      if (g_rodexMailStore->m_mails2.size() > 0) {
        packet.lastMailId2 = g_rodexMailStore->m_mails2.rbegin()->first;
      }
      packet.lastMailId1 = 0;
      if (g_rodexMailStore->m_mails1.size() > 0) {
        packet.lastMailId1 = g_rodexMailStore->m_mails1.rbegin()->first;
      }
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      this->SetButtonsDisabled(1);
      break;
    }

    case ID_SEND_MAIL:
      if (g_windowMgr.HasNavigationBlocker() != true) {
        mode->SendMsg(MM_OPEN_WRITE_RODEX, 0, 0, 0, 0);
      }
      break;

    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_RODEXINBOXWND);
      break;

    case ID_DELETE:
      if (this->searchActive == 1) {
        std::vector<RodexMailEntry> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.state != 0 && check->m_message != -1) {
            const RodexMailEntry entry = this->searchResults[this->page * 6 + i];
            selected.push_back(entry);
          }
        }

        if (selected.size() == 0) {
          break;
        }
        if (g_windowMgr.Confirm(LookupMessage(MSI_MINIMAP_TIP_ZOOM_OUT), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
          break;
        }
        for (int i = 0; i < 6; ++i) {
          this->checks[i]->SetChecked(0);
          this->checks[i]->m_message = 0;
        }

        bool success = true;
        for (std::vector<RodexMailEntry>::iterator entry = selected.begin(); entry != selected.end(); ++entry) {
          success &= this->RequestMailAction(entry->first, entry->second);
        }
        if (!success) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILE_MAIL_RECIEVER_INFO)), 0x64FFFF, 0, 0, 0);
        }
      } else {
        std::list<int> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.state != 0) {
            const int id = check->m_message;
            if (id != -1) {
              selected.push_back(id);
            }
          }
        }

        if (selected.size() == 0) {
          break;
        }
        if (g_windowMgr.Confirm(LookupMessage(MSI_MINIMAP_TIP_ZOOM_OUT), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
          break;
        }
        for (int i = 0; i < 6; ++i) {
          this->checks[i]->SetChecked(0);
          this->checks[i]->m_message = 0;
        }

        bool success = true;
        for (std::list<int>::iterator id = selected.begin(); id != selected.end(); ++id) {
          success &= this->RequestMailAction(g_rodexMailStore->inboxType, *id);
        }
        if (!success) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILE_MAIL_RECIEVER_INFO)), 0x64FFFF, 0, 0, 0);
        }
      }
      break;

    case ID_SEARCH:
      if (this->searchModeButtons[0]->geometry.state == 1) {
        this->SearchByTitle(this->searchEdit->Text());
      } else if (this->searchModeButtons[1]->geometry.state == 1) {
        this->SearchBySender(this->searchEdit->Text());
      }
      this->SendMsg(this->searchTab, UM_COMMAND, ID_RADIOBTN, 0, 0, 0);
      this->Invalidate();
      break;

    case ID_CHECK: {
      int checked = 0;
      for (int i = 0; i < 6; ++i) {
        if (static_cast<int>(this->checks[i]->geometry.state) > 0) {
          ++checked;
        }
      }

      if (checked != 0) {
        this->receiveButton->SetText(LookupMessage(MSI_RODEX_BUTTON_RECEIVE_SELECTED));
        this->receiveButton->SetId(0xD3);
        this->deleteButton->SetText(LookupMessage(MSI_RODEX_MESSAGE_RECEIVE_ALL));
        this->deleteButton->SetId(0x223);
      } else {
        this->receiveButton->SetText(LookupMessage(MSI_RODEX_BUTTON_RECEIVE_ALL));
        this->receiveButton->SetId(0x222);
        this->deleteButton->SetText(LookupMessage(MSI_HOTKEYWND_SKILLBAR2));
        this->deleteButton->SetId(0x224);
      }
      break;
    }

    case 0x136:
      if (g_rodexMailStore->inboxType == MAIL_INBOX_NORMAL) {
        RodexMailMap::iterator mail = g_rodexMailStore->m_mails0.find(argument2);
        if (mail != g_rodexMailStore->m_mails0.end()) {
          PACKET_CZ_RODEX_RETURN packet;
          packet.packetType = ID_CZ_RODEX_RETURN;
          packet.mailId = static_cast<std::uint32_t>(mail->first);
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        }
      }
      break;

    case 0x222: {
      std::string type;
      if (this->searchActive == 1) {
        type = LookupMessage(MSI_MSI_INVENTORY_TABNAME_4);
      } else {
        switch (g_rodexMailStore->inboxType) {
        case MAIL_INBOX_NORMAL:
          type = LookupMessage(MSI_FAILED_PET_AUTO_FEEDING_BY_ITEMACTION);
          break;
        case MAIL_INBOX_ACCOUNT:
          type = LookupMessage(MSI_RODEX_TAB_SEARCH);
          break;
        case MAIL_INBOX_RETURNED:
          type = LookupMessage(MSI_FAILED_HOMUN_AUTO_FEEDING_BY_ITEMACTION);
          break;
        }
      }

      const std::string text = FormatOwnedString(LookupMessage(MSI_RODEX_MESSAGE_RECEIVE_SELECTED), type.c_str());
      if (g_windowMgr.Confirm(text.c_str(), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
        this->RequestAllMailActions();
      }
      break;
    }

    case 0x224: {
      std::string type;
      if (this->searchActive == 1) {
        type = LookupMessage(MSI_MSI_INVENTORY_TABNAME_4);
      } else {
        switch (g_rodexMailStore->inboxType) {
        case MAIL_INBOX_NORMAL:
          type = LookupMessage(MSI_FAILED_PET_AUTO_FEEDING_BY_ITEMACTION);
          break;
        case MAIL_INBOX_ACCOUNT:
          type = LookupMessage(MSI_RODEX_TAB_SEARCH);
          break;
        case MAIL_INBOX_RETURNED:
          type = LookupMessage(MSI_FAILED_HOMUN_AUTO_FEEDING_BY_ITEMACTION);
          break;
        }
      }

      const std::string text = FormatOwnedString(LookupMessage(MSI_HOTKEYGUIDEWND_LEFT), type.c_str());
      if (g_windowMgr.Confirm(text.c_str(), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
        if (this->searchActive == 1) {
          for (std::vector<RodexMailEntry>::iterator entry = this->searchResults.begin(); entry != this->searchResults.end(); ++entry) {
            if (entry->first == 0) {
              RodexMailMap::iterator mail = g_rodexMailStore->m_mails0.find(entry->second);
              if (mail != g_rodexMailStore->m_mails0.end() && mail->second.isKnownSender <= 0) {
                continue;
              }
            }
            this->RequestMailAttachments(entry->first, entry->second);
          }
        } else {
          const int mailType = g_rodexMailStore->inboxType;
          RodexMailMap* mails = RodexMailsOfType(mailType);
          if (mails != nullptr) {
            for (RodexMailMap::iterator mail = mails->begin(); mail != mails->end(); ++mail) {
              if (mail->second.isKnownSender > 0) {
                this->RequestMailAttachments(mailType, mail->first);
              }
            }
          }
        }
      }
      break;
    }

    case 0x223: {
      if (this->searchActive == 1) {
        std::vector<RodexMailEntry> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.state != 0 && check->m_message != -1) {
            const RodexMailEntry entry = this->searchResults[this->page * 6 + i];
            selected.push_back(entry);
          }
        }

        if (selected.size() != 0 &&
            g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_SKILLBER), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
          for (std::vector<RodexMailEntry>::iterator entry = selected.begin(); entry != selected.end(); ++entry) {
            this->RequestMailAttachments(entry->first, entry->second);
          }
          for (int i = 0; i < 6; ++i) {
            this->checks[i]->SetChecked(0);
            this->checks[i]->m_message = 0;
          }
        }
      } else {
        std::vector<int> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.state != 0) {
            const int id = check->m_message;
            if (id != -1) {
              selected.push_back(id);
            }
          }
        }

        if (selected.size() != 0 &&
            g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_SKILLBER), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
          for (std::vector<int>::iterator id = selected.begin(); id != selected.end(); ++id) {
            this->RequestMailAttachments(g_rodexMailStore->inboxType, *id);
          }
          for (int i = 0; i < 6; ++i) {
            this->checks[i]->SetChecked(0);
            this->checks[i]->m_message = 0;
          }
        }
      }
      break;
    }
    }
    break;

  case UM_REFRESH:
    this->Invalidate();
    break;

  case UM_TEXT_LBTN_UP:
    switch (argument1) {
    case 0x142:
      if (g_rodexMailStore->inboxType == MAIL_INBOX_NORMAL) {
        RodexMailMap::iterator mail = g_rodexMailStore->m_mails0.find(argument2);
        if (mail != g_rodexMailStore->m_mails0.end() && (mail->second.type & 8) == 0 && mail->second.sender.size() != 0) {
          const std::pair<bool, std::string> title = g_navigationLinkParser->RemoveLineBreakTag(mail->second.title);
          if (title.first != true) {
            mode->SendMsg(MM_OPEN_WRITE_RODEX, reinterpret_cast<int>(mail->second.sender.c_str()), 0, 0, 0);
          }
        }
      }
      break;
    case 0x143: {
      RodexMailMap::iterator mail = g_rodexMailStore->m_mails0.find(argument2);
      if (mail != g_rodexMailStore->m_mails0.end() && mail->second.isKnownSender == 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRADE_ENCHANT_SUCCESS_REFINING_LEVEL_DISAPPEAR)), 0xFF, 0, 0, 0);
        if (g_windowMgr.Confirm(LookupMessage(MSI_GRADE_ENCHANT_SUCCESS_REFINING_LEVEL_DISAPPEAR), 2, false, false, 0, "Message", 280, 120, 0) == 0xBC) {
          break;
        }
      }
      mode->SendMsg(MM_OPEN_MAIL, argument2, g_rodexMailStore->inboxType, 0, 0);
      break;
    }
    }
    break;

  case UM_REFRESHITEM:
    this->RefreshMailLists();
    this->InitializeMailActions();
    this->SetButtonsDisabled(0);
    this->Invalidate();
    break;

  case UM_DEFPUSH:
    if (this->searchEdit == sender) {
      this->SendMsg(this->searchButton, UM_COMMAND, ID_SEARCH, 0, 0, 0);
    }
    break;

  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}
