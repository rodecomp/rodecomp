#include "UI/Rodex/UIRodexInboxWnd.h"

#include "Contents/MessageTable.h"
#include "Client/Mode/ModeMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstring>
#include <list>

namespace {
#pragma pack(push, 1)
struct PACKET_CZ_OPEN_RODEXBOX {
  std::uint16_t packetType;
  std::int64_t lastMailId0;
  std::int64_t lastMailId2;
  std::int64_t lastMailId1;
};

struct RodexMailRequestPacket {
  std::uint16_t id;
  std::int64_t mailId;
  std::uint8_t type;
};

struct PACKET_CZ_RODEX_RETURN {
  std::uint16_t packetType;
  std::uint32_t mailId;
};
#pragma pack(pop)
}

// OFFSET: 0x00804D50
// FUNCTION: fn_10038
UIRodexInboxWnd::UIRodexInboxWnd()
    : UIItemWnd(), unknown_0130(0), unknown_0134(0), unknown_0138(0), unknown_013C(nullptr), unknown_0140(nullptr),
      unknown_018C(nullptr), unknown_0190(nullptr), unknown_0194(nullptr), unknown_0198(nullptr), unknown_019C(0), unknown_01A0(0),
      expired(0), unknown_01C8(nullptr), unknown_01CC(0), unknown_01D0(0), unknown_01D4(0), searchResults(), mails0(), mails1(), mails2() {
  std::memset(this->unknown_0144, 0, sizeof(this->unknown_0144));
  std::memset(this->unknown_015C, 0, sizeof(this->unknown_015C));
  std::memset(this->checks, 0, sizeof(this->checks));
  std::memset(this->unknown_01A8, 0, sizeof(this->unknown_01A8));
  g_rodexMailStore->unknown_001C = false;
  PACKET_CZ_OPEN_RODEXBOX packet;
  packet.packetType = ID_CZ_OPEN_RODEXBOX;
  packet.lastMailId0 = 0;
  if (g_rodexMailStore->m_mails0.size() > 0) {
    packet.lastMailId0 = (--g_rodexMailStore->m_mails0.end())->first;
  }
  packet.lastMailId2 = 0;
  if (g_rodexMailStore->m_mails2.size() > 0) {
    packet.lastMailId2 = (--g_rodexMailStore->m_mails2.end())->first;
  }
  packet.lastMailId1 = 0;
  if (g_rodexMailStore->m_mails1.size() > 0) {
    packet.lastMailId1 = (--g_rodexMailStore->m_mails1.end())->first;
  }
  GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
}

// OFFSET: 0x00807640
// FUNCTION: fn_10049
void UIRodexInboxWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x00807660
// FUNCTION: fn_10050
void UIRodexInboxWnd::Slot26(int, int) {}

// OFFSET: 0x00807670
// FUNCTION: fn_10051
void UIRodexInboxWnd::Nullsub257() {}

// OFFSET: 0x00807680
// FUNCTION: fn_10052
void UIRodexInboxWnd::Slot28(int, int) {}

// OFFSET: 0x00805750
// FUNCTION: fn_10045
void UIRodexInboxWnd::DrawMailIcons(int row, unsigned char attachments, unsigned char read, unsigned char returned) {
  const int y = row * 0x2C;
  const char* status = returned == 1
    ? "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_status_mail_returned.bmp"
    : read == 0
      ? "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_status_mail_received.bmp"
      : "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_status_mail_read.bmp";
  this->DrawBitmap(0x24, y + 0x7C, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(status))), true);

  const char* attachmentIcon;
  if ((attachments & 6) == 6) {
    this->DrawBitmap(0xD8, y + 0x8B, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))), true);
    attachmentIcon = "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny_n_item.bmp";
  } else if ((attachments & 2) == 0) {
    if ((attachments & 4) == 0) {
      return;
    }
    this->DrawBitmap(0xD8, y + 0x8B, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))), true);
    attachmentIcon = "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_item.bmp";
  } else {
    this->DrawBitmap(0xD8, y + 0x8B, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))), true);
    attachmentIcon = "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny.bmp";
  }

  this->DrawBitmap(0xDB, y + 0x7F, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(attachmentIcon))), true);
}

// OFFSET: 0x008058B0
// FUNCTION: fn_10046
bool UIRodexInboxWnd::Slot03() {
  return true;
}

// OFFSET: 0x008058C0
// FUNCTION: fn_10047
void UIRodexInboxWnd::Slot15(int width, int height) {
  g_rodexMailStore->PurgeFlagged();
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "", 0);

  this->unknown_0138 = new UIBmpRadioBtn(*this->m_context, *this->m_controls);
  this->unknown_0138->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->unknown_0138->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->unknown_0138->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  {
    UIBmpRadioBtn* tab = this->unknown_0138;
    const std::string label = LookupMessage(0xDDC);
    tab->m_label = label;
    tab->Invalidate();
  }
  {
    const int textX = static_cast<int>(static_cast<float>(this->unknown_0138->geometry.width - this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDC), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    UIBmpRadioBtn* tab = this->unknown_0138;
    const int textY = static_cast<int>(static_cast<float>(tab->geometry.height - this->TextHeight(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDC), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    tab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->unknown_0138->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->unknown_0138->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->unknown_0138->Move(0xCF, 0x46);
  this->AddChild(this->unknown_0138);
  this->unknown_0138->SendMsg(this, 13, 0, 0, 0, 0);

  this->unknown_0134 = new UIBmpRadioBtn(*this->m_context, *this->m_controls);
  this->unknown_0134->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->unknown_0134->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->unknown_0134->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  {
    UIBmpRadioBtn* tab = this->unknown_0134;
    const std::string label = LookupMessage(0xDDA);
    tab->m_label = label;
    tab->Invalidate();
  }
  {
    const int textX = static_cast<int>(static_cast<float>(this->unknown_0134->geometry.width - this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDA), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    UIBmpRadioBtn* tab = this->unknown_0134;
    const int textY = static_cast<int>(static_cast<float>(tab->geometry.height - this->TextHeight(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDA), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    tab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->unknown_0134->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->unknown_0134->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->unknown_0134->Move(0x13, 0x46);
  this->AddChild(this->unknown_0134);
  this->unknown_0134->SendMsg(this, 13, 0, 0, 0, 0);

  this->unknown_0130 = new UIBmpRadioBtn(*this->m_context, *this->m_controls);
  this->unknown_0130->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->unknown_0130->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->unknown_0130->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  {
    UIBmpRadioBtn* tab = this->unknown_0130;
    const std::string label = LookupMessage(0xDDB);
    tab->m_label = label;
    tab->Invalidate();
  }
  {
    const int textX = static_cast<int>(static_cast<float>(this->unknown_0130->geometry.width - this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDB), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    UIBmpRadioBtn* tab = this->unknown_0130;
    const int textY = static_cast<int>(static_cast<float>(tab->geometry.height - this->TextHeight(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDB), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    tab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->unknown_0130->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->unknown_0130->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->unknown_0130->Move(0x71, 0x46);
  this->AddChild(this->unknown_0130);
  this->unknown_0130->SendMsg(this, 13, 0, 0, 0, 0);

  switch (g_rodexMailStore->unknown_0010) {
  case 0:
    this->unknown_0130->SendMsg(this, 13, 1, 0, 0, 0);
    break;
  case 1:
    this->unknown_0134->SendMsg(this, 13, 1, 0, 0, 0);
    break;
  case 2:
    this->unknown_0138->SendMsg(this, 13, 1, 0, 0, 0);
    break;
  }

  this->unknown_013C = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_013C->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_refresh_out.bmp", 0, 0);
  this->unknown_013C->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_refresh_over.bmp", 1, 0);
  this->unknown_013C->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_refresh_press.bmp", 2, 0);
  this->unknown_013C->SetTooltip(LookupMessage(0xB21));
  this->unknown_013C->SetSize(this->unknown_013C->BitmapWidth(), this->unknown_013C->BitmapHeight());
  this->unknown_013C->Move(0xEF, 0x14);
  this->unknown_013C->SetId(0x19F);
  this->AddChild(this->unknown_013C);

  this->unknown_0140 = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_0140->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_write_out.bmp", 0, 0);
  this->unknown_0140->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_write_over.bmp", 1, 0);
  this->unknown_0140->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_write_press.bmp", 2, 0);
  this->unknown_0140->SetTooltip(LookupMessage(0x402));
  this->unknown_0140->SetSize(this->unknown_0140->BitmapWidth(), this->unknown_0140->BitmapHeight());
  this->unknown_0140->Move(0x10C, 0x12);
  this->unknown_0140->SetId(0x131);
  this->AddChild(this->unknown_0140);

  for (int y = 0x66, row = 0; y < 0x16E; y += 0x2C, ++row) {
    this->checks[row] = new UICheckBox(*this->m_context, *this->m_controls);
    this->checks[row]->SetSize(14, 14);
    this->checks[row]->SetBitmaps("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_on.bmp", "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_off.bmp");
    this->checks[row]->Move(0x11, y + 6);
    this->checks[row]->SetChecked(0);
    this->checks[row]->m_message = 0;
    this->checks[row]->Show(0);
    this->AddChild(this->checks[row]);

    this->unknown_015C[row] = new UITextButton(*this->m_context, *this->m_controls);
    this->unknown_015C[row]->Configure("", 0, 0, 12, 0, 0);
    this->unknown_015C[row]->m_hoverColor = 0x51417E;
    this->unknown_015C[row]->Move(0x45, y);
    this->unknown_015C[row]->SetId(0x143);
    this->unknown_015C[row]->SetTooltip(LookupMessage(0xA8E));
    this->unknown_015C[row]->Enable(0);
    this->AddChild(this->unknown_015C[row]);

    this->unknown_0144[row] = new UITextButton(*this->m_context, *this->m_controls);
    this->unknown_0144[row]->Configure("", 0x844A42, 0, 11, 1, 0);
    this->unknown_0144[row]->m_hoverColor = 0x51417E;
    this->unknown_0144[row]->Move(0x47, y + 0x10);
    this->unknown_0144[row]->SetId(0x142);
    this->unknown_0144[row]->SetTooltip(LookupMessage(0xA8D));
    this->unknown_0144[row]->Enable(0);
    this->AddChild(this->unknown_0144[row]);

    this->unknown_01A8[row] = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->unknown_01A8[row]->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_return_out.bmp", 0, 0);
    this->unknown_01A8[row]->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_return_over.bmp", 1, 0);
    this->unknown_01A8[row]->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_return_press.bmp", 2, 0);
    this->unknown_01A8[row]->SetSize(this->unknown_01A8[row]->BitmapWidth(), this->unknown_01A8[row]->BitmapHeight());
    this->unknown_01A8[row]->Move(0xBA, y + 0x1D);
    this->unknown_01A8[row]->SetTooltip(LookupMessage(0xB20));
    this->unknown_01A8[row]->SetId(0x136);
    this->unknown_01A8[row]->Enable(0);
    this->AddChild(this->unknown_01A8[row]);
  }

  UIBasicButton* button = new UIBasicButton(*this->m_context, *this->m_controls);
  this->unknown_018C = button;
  button->Configure(0x46, LookupMessage(0xE05));
  this->unknown_018C->SetId(0x222);
  this->unknown_018C->Move(4, 0x178);
  this->AddChild(this->unknown_018C);

  button = new UIBasicButton(*this->m_context, *this->m_controls);
  this->unknown_0190 = button;
  button->Configure(0x46, LookupMessage(0xE08));
  this->unknown_0190->Move(0x54, 0x188);
  this->unknown_0190->SetId(0x224);
  this->AddChild(this->unknown_0190);

  this->unknown_0194 = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_0194->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_prev_out.bmp", 0, 0);
  this->unknown_0194->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_prev_over.bmp", 1, 0);
  this->unknown_0194->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_prev_press.bmp", 2, 0);
  this->unknown_0194->SetTooltip(LookupMessage(0x511));
  this->unknown_0194->SetSize(this->unknown_0194->BitmapWidth(), this->unknown_0194->BitmapHeight());
  this->unknown_0194->Move(0xF6, 0x176);
  this->unknown_0194->SetId(0x13C);
  this->AddChild(this->unknown_0194);

  this->unknown_0198 = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_0198->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_next_out.bmp", 0, 0);
  this->unknown_0198->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_next_over.bmp", 1, 0);
  this->unknown_0198->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_next_press.bmp", 2, 0);
  this->unknown_0198->SetTooltip(LookupMessage(0x512));
  this->unknown_0198->SetSize(this->unknown_0198->BitmapWidth(), this->unknown_0198->BitmapHeight());
  this->unknown_0198->Move(0x10E, 0x176);
  this->unknown_0198->SetId(0x13D);
  this->AddChild(this->unknown_0198);

  const int searchTexts[2] = {0xC78, 0xC79};
  for (int x = 0x16, index = 0; x < 0x7C; x += 0x33, ++index) {
    this->unknown_01C0[index] = new UIRadioBtn(*this->m_context, *this->m_controls);
    this->unknown_01C0[index]->SetSize(0x33, 12);
    {
      UIRadioBtn* radio = this->unknown_01C0[index];
      const std::string unchecked = "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_search_off.bmp";
      const std::string checked = "유저인터페이스\\basic_interface\\rodexsystem\\renewal\\checkbox_search_on.bmp";
      radio->checkedBitmap = checked;
      radio->uncheckedBitmap = unchecked;
      radio->Invalidate();
    }
    this->unknown_01C0[index]->SetText(LookupMessage(searchTexts[index]));
    this->unknown_01C0[index]->textColor = 0x654939;
    this->unknown_01C0[index]->Move(x, 0x44);
    this->unknown_01C0[index]->value = 0x141;
    this->unknown_01C0[index]->SendMsg(this, 13, 1 - index, 0, 0, 0);
    this->AddChild(this->unknown_01C0[index]);
  }

  this->unknown_01C8 = new UIEditCtrl();
  this->unknown_01C8->SetSize(0x84, 0x12);
  this->unknown_01C8->Move(0x85, 0x41);
  this->unknown_01C8->maximum = 0x10;
  this->unknown_01C8->flat = 0;
  this->unknown_01C8->Border(0xAD, 0xAD, 0xAD);
  this->unknown_01C8->preserveInput = 1;
  this->AddChild(this->unknown_01C8);

  this->unknown_01CC = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_01CC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_search_out.bmp", 0, 0);
  this->unknown_01CC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_search_over.bmp", 1, 0);
  this->unknown_01CC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_search_press.bmp", 2, 0);
  this->unknown_01CC->SetSize(this->unknown_01CC->BitmapWidth(), this->unknown_01CC->BitmapHeight());
  this->unknown_01CC->Move(0x108, 0x41);
  this->unknown_01CC->SetId(0x141);
  this->AddChild(this->unknown_01CC);

  this->unknown_01D0 = new UIBmpRadioBtn(*this->m_context, *this->m_controls);
  this->unknown_01D0->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_out.bmp", 0);
  this->unknown_01D0->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_over.bmp", 1);
  this->unknown_01D0->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_listtab_empty_press.bmp", 2);
  {
    UIBmpRadioBtn* tab = this->unknown_01D0;
    const std::string label = LookupMessage(0xDDD);
    tab->m_label = label;
    tab->Invalidate();
  }
  {
    const int textX = static_cast<int>(static_cast<float>(this->unknown_01D0->geometry.width - this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDD), 0, 0, 12, 0, 0)) * g_depthAverageHalf);
    UIBmpRadioBtn* tab = this->unknown_01D0;
    const int textY = static_cast<int>(static_cast<float>(tab->geometry.height - this->TextHeight(this->m_context->fonts, this->m_context->locale, LookupMessage(0xDDD), 0, 0, 12, 0, 0) - 3) * g_depthAverageHalf);
    tab->AddText(0, 12, 0xCCB5AD, true, false, textX, textY);
    this->unknown_01D0->AddText(1, 12, 0x9C3E1F, true, false, textX, textY);
    this->unknown_01D0->AddText(2, 12, 0x9C3E1F, true, false, textX, textY);
  }
  this->unknown_01D0->Move(0xDF, 0x56);
  this->AddChild(this->unknown_01D0);
  this->unknown_01D0->SendMsg(this, 13, 0, 0, 0, 0);

  this->unknown_0134->Move(0x13, 0x56);
  this->unknown_0130->Move(0x57, 0x56);
  this->unknown_0138->Move(0x9B, 0x56);
  this->unknown_013C->Move(0xF8, 0x14);
  this->unknown_0140->Move(0x115, 0x12);
  for (int y = 0x79, row = 0; y < 0x181; y += 0x2C, ++row) {
    this->checks[row]->Move(0x11, y + 7);
    this->unknown_015C[row]->Move(0x45, y);
    this->unknown_0144[row]->Move(0x47, y + 0x14);
  }
  this->unknown_018C->Move(4, 0x188);
  this->unknown_0194->Move(0xFF, 0x186);
  this->unknown_0198->Move(0x117, 0x186);
  this->RefreshMailLists();
  this->Invalidate();
}

// OFFSET: 0x00806D40
// FUNCTION: fn_10048
void UIRodexInboxWnd::Paint() {
  this->ClearSurface(0xFF00FF);
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_rodex_list.bmp"))), false);
  for (int i = 0; i < 6; ++i) {
    this->unknown_01A8[i]->Enable(0);
  }

  if (!g_rodexMailStore->unknown_001C) {
    this->DrawBitmap(0x1F, 0x9A, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\img_notice.bmp"))), true);
    this->sub_A84400(0x1F, 0xE5, std::string(LookupMessage(0xC76)), 0xF8, 3, 6, 0, 12, 0, 0);
    this->DrawCenteredText(0x1F, 0x10E, LookupMessage(0xC77), 0xF8, 0, 0x12, 0xC89482, 1);
    return;
  }

  unsigned int index = this->unknown_019C * 6;
  if (this->unknown_01D4 == 1) {
    for (int row = 0; row < 6; ++row, ++index) {
      if (index < this->searchResults.size()) {
        std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(this->searchResults[index].first);
        if (mails) {
          std::map<std::int64_t, RodexMail>::iterator it = mails->find(this->searchResults[index].second);
          if (it != mails->end()) {
            this->DrawMailIcons(row, it->second.unknown_0001, it->second.isRead, this->searchResults[index].first == 2);
            this->DrawExpireTime(row, it->second.unknown_001C, it->second.unknown_0020, this->searchResults[index].first == 2);
            if (this->expired == 1) {
              this->expired = 0;
              mails->erase(it);
            } else {
              this->SetMailRow(
                row, this->searchResults[index].second, it->second.unknown_0004, it->second.unknown_0024, it->second.isRead, it->second.unknown_0190,
                it->second.unknown_0194
              );
            }
          }
        }
      } else {
        this->checks[row]->Show(0);
        this->checks[row]->SetChecked(0);
        this->checks[row]->m_message = 0;
        this->unknown_0144[row]->Enable(0);
        this->unknown_015C[row]->Enable(0);
      }
    }
  } else {
    for (int row = 0; row < 6; ++row, ++index) {
      this->checks[row]->Show(0);
      this->unknown_0144[row]->Enable(0);
      this->unknown_015C[row]->Enable(0);
      if (g_rodexMailStore->unknown_0010 == 1) {
        if (index < this->mails1.size()) {
          std::map<std::int64_t, RodexMail>::iterator it = g_rodexMailStore->m_mails1.find(this->mails1[index].second);
          if (it != g_rodexMailStore->m_mails1.end()) {
            this->DrawMailIcons(row, it->second.unknown_0001, it->second.isRead, 0);
            this->DrawExpireTime(row, it->second.unknown_001C, it->second.unknown_0020, 0);
            if (this->expired == 1) {
              this->expired = 0;
              g_rodexMailStore->m_mails1.erase(it);
            } else {
              this->checks[row]->Show(1);
              this->unknown_0144[row]->Enable(1);
              this->unknown_015C[row]->Enable(1);
              this->SetMailRow(row, it->first, it->second.unknown_0004, it->second.unknown_0024, it->second.isRead, it->second.unknown_0190, -1);
            }
          }
        }
      } else if (g_rodexMailStore->unknown_0010 == 2) {
        if (index < this->mails2.size()) {
          std::map<std::int64_t, RodexMail>::iterator it = g_rodexMailStore->m_mails2.find(this->mails2[index].second);
          if (it != g_rodexMailStore->m_mails2.end()) {
            this->DrawMailIcons(row, it->second.unknown_0001, it->second.isRead, 0);
            this->DrawExpireTime(row, it->second.unknown_001C, it->second.unknown_0020, 0);
            if (this->expired == 1) {
              this->expired = 0;
              g_rodexMailStore->m_mails2.erase(it);
            } else {
              this->checks[row]->Show(1);
              this->unknown_0144[row]->Enable(1);
              this->unknown_015C[row]->Enable(1);
              this->SetMailRow(row, it->first, it->second.unknown_0004, it->second.unknown_0024, it->second.isRead, it->second.unknown_0190, -1);
            }
          }
        }
      } else {
        this->unknown_0144[row]->Enable(0);
        this->unknown_015C[row]->Enable(0);
        if (index < this->mails0.size()) {
          std::map<std::int64_t, RodexMail>::iterator it = g_rodexMailStore->m_mails0.find(this->mails0[index].second);
          if (it != g_rodexMailStore->m_mails0.end()) {
            this->DrawMailIcons(row, it->second.unknown_0001, it->second.isRead, 0);
            this->DrawExpireTime(row, it->second.unknown_001C, it->second.unknown_0020, 0);
            if (this->expired == 1) {
              this->expired = 0;
              g_rodexMailStore->m_mails0.erase(it);
            } else {
              this->SetMailRow(
                row, it->first, it->second.unknown_0004, it->second.unknown_0024, it->second.isRead, it->second.unknown_0190, it->second.unknown_0194
              );
            }
          }
        }
      }
    }
  }

  this->DrawRightAlignedText(0x111, 0x2E, FormatOwnedString("%d", g_rodexMailStore->unknown_0014).c_str(), 0, 0, 11, 0xC89482, 1);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 0x111, 0x2E, LookupMessage(0xBDA), 0, 0, 11, 0xC89482, 1, 0);
}

// OFFSET: 0x00808FD0
// FUNCTION: fn_10056
void UIRodexInboxWnd::SetButtonsDisabled(unsigned char disabled) {
  UIButton* button = this->unknown_0194;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->unknown_0194->Invalidate();

  button = this->unknown_0198;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->unknown_0198->Invalidate();

  button = this->unknown_013C;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->unknown_013C->Invalidate();

  button = this->unknown_018C;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->unknown_018C->Invalidate();

  button = this->unknown_0140;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->unknown_0140->Invalidate();

  button = this->unknown_0190;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->unknown_0190->Invalidate();

  this->Invalidate();
}

// OFFSET: 0x008090C0
// FUNCTION: fn_10057
void UIRodexInboxWnd::SetMailRow(int row, std::int64_t mailId, std::string name, std::string subject, unsigned char read, unsigned char returned, int noAttachment) {
  std::string nameText = name;
  std::string subjectText = subject;
  const std::pair<bool, std::string> stripped = RemoveLineBreakTag(subjectText);
  subjectText = stripped.second;
  if (stripped.first == true) {
    this->unknown_0144[row]->SetTooltip("");
  }

  nameText = g_navigationLinkParser->ExpandMessages(nameText);
  subjectText = g_navigationLinkParser->ExpandMessages(subjectText);

  this->checks[row]->Show(1);
  this->checks[row]->m_message = static_cast<int>(mailId);

  UITextButton* button = this->unknown_0144[row];
  {
    const std::string label = nameText.c_str();
    button->label = label;
  }
  this->unknown_0144[row]->ResizeToTextClipped(0x8C, this->unknown_0144[row]->geometry.height);
  this->unknown_0144[row]->m_value = static_cast<int>(mailId);
  this->unknown_0144[row]->Enable(1);

  std::uint32_t color;
  if (noAttachment == 0) {
    this->unknown_01A8[row]->Enable(1);
    this->unknown_01A8[row]->m_argument = static_cast<int>(mailId);
    color = read == 1 ? 0xC8C8E6 : 0xFF;
  } else {
    color = read == 1 ? 0xC8C8C8 : 0;
  }

  this->unknown_015C[row]->m_color = color;
  button = this->unknown_015C[row];
  {
    const std::string label = subjectText.c_str();
    button->label = label;
  }
  this->unknown_015C[row]->ResizeToTextClipped(0x8C, this->unknown_015C[row]->geometry.height);
  this->unknown_015C[row]->m_value = static_cast<int>(mailId);
  this->unknown_015C[row]->Enable(1);

  if (returned == 1) {
    this->checks[row]->m_message = -1;
    this->unknown_0144[row]->Enable(0);
    this->unknown_015C[row]->m_color = 0xC8C8C8;
    button = this->unknown_015C[row];
    {
      const std::string label = LookupMessage(0xB5B);
      button->label = label;
    }
    this->unknown_015C[row]->ResizeToTextClipped(0x8C, this->unknown_015C[row]->geometry.height);
    this->unknown_015C[row]->m_value = -1;
    this->unknown_015C[row]->Enable(1);
  }
}

// OFFSET: 0x00809670
// FUNCTION: fn_10058
void UIRodexInboxWnd::DrawExpireTime(int row, __time32_t sendTime, __time32_t expireTime, unsigned char returned) {
  _difftime32(_time32(nullptr), sendTime);
  int remaining = static_cast<int>(_difftime32(expireTime, _time32(nullptr)));
  if (remaining <= 0) {
    remaining = 0;
  }
  if (remaining == 0) {
    this->expired = 1;
    return;
  }

  std::string text;
  text.assign(
    remaining / 86400 > 0   ? FormatOwnedString(LookupMessage(0xB24), remaining / 86400)
    : remaining / 3600 > 0 ? FormatOwnedString(LookupMessage(0xB23), remaining / 3600)
                           : FormatOwnedString(LookupMessage(0xB22), remaining / 60)
  );

  if (returned == 1) {
    this->DrawCenteredText(0x104, row * 0x2C + 0x7D, LookupMessage(0xB25), 0, 0, 11, 0x666673, 0);
    this->DrawCenteredText(0x104, row * 0x2C + 0x8A, text.c_str(), 0, 0, 11, 0x666673, 0);
  } else {
    this->DrawCenteredText(0x104, row * 0x2C + 0x82, text.c_str(), 0, 0, 11, 0xA0AE9, 1);
  }
}

// OFFSET: 0x008098D0
// FUNCTION: fn_10059
void UIRodexInboxWnd::ClearChecks() {
  ++this->unknown_019C;
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

  for (std::map<std::int64_t, RodexMail>::iterator it = g_rodexMailStore->m_mails0.begin(); it != g_rodexMailStore->m_mails0.end(); ++it) {
    this->mails0.push_back(RodexMailEntry(0, it->first));
  }
  std::sort(this->mails0.begin(), this->mails0.end(), RodexMailEntryLess());

  for (std::map<std::int64_t, RodexMail>::iterator it = g_rodexMailStore->m_mails2.begin(); it != g_rodexMailStore->m_mails2.end(); ++it) {
    this->mails2.push_back(RodexMailEntry(2, it->first));
  }
  std::sort(this->mails2.begin(), this->mails2.end(), RodexMailEntryLess());

  for (std::map<std::int64_t, RodexMail>::iterator it = g_rodexMailStore->m_mails1.begin(); it != g_rodexMailStore->m_mails1.end(); ++it) {
    this->mails1.push_back(RodexMailEntry(1, it->first));
  }
  std::sort(this->mails1.begin(), this->mails1.end(), RodexMailEntryLess());
}

// OFFSET: 0x00809B60
// FUNCTION: fn_10062
void UIRodexInboxWnd::RemoveInboxMail(int id) {
  for (std::vector<RodexMailEntry>::iterator it = this->mails0.begin(); it != this->mails0.end(); ++it) {
    if (it->second == id) {
      this->mails0.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00809C80
// FUNCTION: fn_10067
void UIRodexInboxWnd::SearchBySender(std::string keyword) {
  this->searchResults.clear();

  std::vector<RodexMailEntry>& results = this->searchResults;
  std::for_each(g_rodexMailStore->m_mails0.begin(), g_rodexMailStore->m_mails0.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.unknown_0004.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(0, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails1.begin(), g_rodexMailStore->m_mails1.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.unknown_0004.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(1, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails2.begin(), g_rodexMailStore->m_mails2.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.unknown_0004.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(2, mail.first));
    }
  });

  std::sort(this->searchResults.begin(), this->searchResults.end(), RodexMailEntryLess());
}

// OFFSET: 0x00809F70
// FUNCTION: fn_10068
void UIRodexInboxWnd::SearchByTitle(std::string keyword) {
  this->searchResults.clear();

  std::vector<RodexMailEntry>& results = this->searchResults;
  std::for_each(g_rodexMailStore->m_mails0.begin(), g_rodexMailStore->m_mails0.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.unknown_0024.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(0, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails1.begin(), g_rodexMailStore->m_mails1.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.unknown_0024.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(1, mail.first));
    }
  });
  std::for_each(g_rodexMailStore->m_mails2.begin(), g_rodexMailStore->m_mails2.end(), [&results, keyword](const std::pair<const std::int64_t, RodexMail>& mail) {
    if (mail.second.unknown_0024.find(keyword, 0) != std::string::npos) {
      results.push_back(RodexMailEntry(2, mail.first));
    }
  });

  std::sort(this->searchResults.begin(), this->searchResults.end(), RodexMailEntryLess());
}

// OFFSET: 0x00807690
// FUNCTION: fn_10053
void __stdcall RequestRodexMailAttachments(int type, std::int64_t mailId) {
  if (g_modeMgr.GetGameMode() != nullptr) {
    g_modeMgr.GetGameMode();
    if (g_windowMgr.LookupWindow(0x122) != nullptr || g_windowMgr.LookupWindow(0x12E) != nullptr || g_windowMgr.LookupWindow(0x15C) != nullptr || g_windowMgr.LookupWindow(0x2716) != nullptr || g_windowMgr.LookupWindow(0x169) != nullptr) {
      return;
    }
  }

  unsigned char flags = 0;
  std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(type);
  if (mails != nullptr) {
    std::map<std::int64_t, RodexMail>::iterator mail = mails->find(mailId);
    if (mail != mails->end()) {
      flags = reinterpret_cast<const std::uint8_t*>(&mail->second)[0x19];
    }
  }

  if ((flags & 4) != 0) {
    RodexMailRequestPacket packet;
    packet.id = ID_CZ_REQ_ITEM_FROM_RODEX;
    packet.mailId = mailId;
    packet.type = static_cast<std::uint8_t>(type);
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
  if ((flags & 2) != 0) {
    RodexMailRequestPacket packet;
    packet.id = ID_CZ_REQ_ZENY_FROM_RODEX;
    packet.mailId = mailId;
    packet.type = static_cast<std::uint8_t>(type);
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x008078E0
// FUNCTION: fn_10054
void UIRodexInboxWnd::InitializeMailActions() {
  this->unknown_019C = 0;
  for (int i = 0; i < 6; ++i) {
    this->checks[i]->SetChecked(0);
    this->checks[i]->m_message = 0;
  }

  this->unknown_018C->SetText(LookupMessage(0xE05));
  this->unknown_018C->SetId(0x222);
  this->unknown_0190->SetText(LookupMessage(0xE08));
  this->unknown_0190->SetId(0x224);
  this->Invalidate();
}

// OFFSET: 0x00805350
// FUNCTION: fn_10041
bool RodexMailEntryLess::operator()(const RodexMailEntry& left, const RodexMailEntry& right) const {
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

// OFFSET: 0x008054D0
// FUNCTION: fn_10043
bool __stdcall RequestRodexMailAction(int type, std::uint64_t mailId) {
  std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(type);
  if (mails == nullptr) {
    return false;
  }

  std::map<std::int64_t, RodexMail>::iterator mail = mails->find(static_cast<std::int64_t>(mailId));
  if (mail == mails->end() || (mail->second.unknown_0001 & 6) != 0) {
    return false;
  }

  g_modeMgr.GetGameMode()->SendMsg(0xC4, static_cast<int>(mailId), type, 0, 0);
  if (g_windowMgr.unknown_045C != 0) {
    g_windowMgr.CloseWindow(265);
  }
  return true;
}

// OFFSET: 0x00805570
// FUNCTION: fn_10044
void UIRodexInboxWnd::RequestAllMailActions() {
  int requested = 0;
  int skipped = 0;
  if (this->unknown_01D4 == 1) {
    for (std::vector<RodexMailEntry>::iterator entry = this->searchResults.begin(); entry != this->searchResults.end(); ++entry) {
      std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(entry->first);
      if (mails == nullptr) {
        continue;
      }

      std::map<std::int64_t, RodexMail>::iterator mail = mails->find(entry->second);
      if (mail == mails->end()) {
        continue;
      }

      if ((mail->second.unknown_0001 & 6) == 0) {
        g_modeMgr.GetGameMode()->SendMsg(0xC4, static_cast<int>(entry->second), entry->first, 0, 0);
        ++requested;
      } else {
        ++skipped;
      }
    }
  } else {
    int type = g_rodexMailStore->unknown_0010;
    std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(type);
    if (mails == nullptr) {
      return;
    }

    for (std::map<std::int64_t, RodexMail>::iterator mail = mails->begin(); mail != mails->end(); ++mail) {
      if ((mail->second.unknown_0001 & 6) == 0) {
        g_modeMgr.GetGameMode()->SendMsg(0xC4, static_cast<int>(mail->first), type, 0, 0);
        ++requested;
      } else {
        ++skipped;
      }
    }
  }

  if (requested > 0 && g_windowMgr.unknown_045C != 0) {
    g_windowMgr.CloseWindow(265);
  }
  if (skipped > 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA34)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00807AA0
// FUNCTION: fn_10055
int UIRodexInboxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (g_windowMgr.unknown_0458 != nullptr || g_windowMgr.unknown_045C != nullptr) {
    return 0;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (g_rodexMailStore->unknown_001C == false && !(message == 6 && argument1 == 0xC9)) {
    return 0;
  }

  switch (message) {
  case 0:
    if (this->unknown_01C8 == sender) {
      this->SendMsg(this->unknown_01CC, 6, 0x141, 0, 0, 0);
    }
    break;

  case 6:
    switch (argument1) {
    case 0xC9:
      g_windowMgr.CloseWindow(0x107);
      break;

    case 0xD3:
      if (this->unknown_01D4 == 1) {
        std::vector<RodexMailEntry> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.unknown_0030 != 0 && check->m_message != -1) {
            selected.push_back(this->searchResults[this->unknown_019C * 6 + i]);
          }
        }

        if (selected.size() != 0) {
          if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xB26), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
            for (int i = 0; i < 6; ++i) {
              this->checks[i]->SetChecked(0);
              this->checks[i]->m_message = 0;
            }

            bool success = true;
            for (std::vector<RodexMailEntry>::iterator entry = selected.begin(); entry != selected.end(); ++entry) {
              success &= RequestRodexMailAction(entry->first, entry->second);
            }
            if (!success) {
              g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA34)), 0x64FFFF, 0, 0, 0);
            }
          }
        }
      } else {
        std::list<int> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.unknown_0030 != 0 && check->m_message != -1) {
            selected.push_back(check->m_message);
          }
        }

        if (selected.size() != 0) {
          if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xB26), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
            for (int i = 0; i < 6; ++i) {
              this->checks[i]->SetChecked(0);
              this->checks[i]->m_message = 0;
            }

            bool success = true;
            for (std::list<int>::iterator id = selected.begin(); id != selected.end(); ++id) {
              success &= RequestRodexMailAction(g_rodexMailStore->unknown_0010, *id);
            }
            if (!success) {
              g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA34)), 0x64FFFF, 0, 0, 0);
            }
          }
        }
      }
      break;

    case 0xD5: {
      int checked = 0;
      for (int i = 0; i < 6; ++i) {
        if (this->checks[i]->geometry.unknown_0030 > 0) {
          ++checked;
        }
      }

      if (checked != 0) {
        this->unknown_018C->SetText(LookupMessage(0xE04));
        this->unknown_018C->SetId(0xD3);
        this->unknown_0190->SetText(LookupMessage(0xE07));
        this->unknown_0190->SetId(0x223);
      } else {
        this->unknown_018C->SetText(LookupMessage(0xE05));
        this->unknown_018C->SetId(0x222);
        this->unknown_0190->SetText(LookupMessage(0xE08));
        this->unknown_0190->SetId(0x224);
      }
      break;
    }

    case 0xD7:
      if (argument2 == 0x141) {
        for (int i = 0; i < 2; ++i) {
          UIRadioBtn* radio = this->unknown_01C0[i];
          radio->SendMsg(this, 13, radio == sender, 0, 0, 0);
        }
        this->Invalidate();
      } else {
        this->unknown_0130->SendMsg(this, 13, 0, 0, 0, 0);
        this->unknown_0134->SendMsg(this, 13, 0, 0, 0, 0);
        this->unknown_0138->SendMsg(this, 13, 0, 0, 0, 0);
        this->unknown_01D0->SendMsg(this, 13, 0, 0, 0, 0);
        sender->SendMsg(this, 13, 1, 0, 0, 0);
        for (int i = 0; i < 6; ++i) {
          this->checks[i]->SetChecked(0);
          this->checks[i]->m_message = 0;
        }
        this->unknown_019C = 0;

        if (this->unknown_0130 == sender) {
          g_rodexMailStore->unknown_0010 = 0;
        } else if (this->unknown_0134 == sender) {
          g_rodexMailStore->unknown_0010 = 1;
        } else if (this->unknown_0138 == sender) {
          g_rodexMailStore->unknown_0010 = 2;
        }
        this->unknown_01D4 = this->unknown_01D0 == sender;
        this->Invalidate();
      }
      break;

    case 0x131:
      if (g_windowMgr.HasNavigationBlocker() != true) {
        mode->SendMsg(0x10C, 0, 0, 0, 0);
      }
      break;

    case 0x136:
      if (g_rodexMailStore->unknown_0010 == 0) {
        std::map<std::int64_t, RodexMail>::iterator mail = g_rodexMailStore->m_mails0.find(argument2);
        if (mail != g_rodexMailStore->m_mails0.end()) {
          PACKET_CZ_RODEX_RETURN packet;
          packet.packetType = ID_CZ_RODEX_RETURN;
          packet.mailId = static_cast<std::uint32_t>(mail->first);
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        }
      }
      break;

    case 0x13C:
      if (--this->unknown_019C < 0) {
        this->unknown_019C = 0;
      } else {
        for (int i = 0; i < 6; ++i) {
          this->checks[i]->SetChecked(0);
          this->checks[i]->m_message = 0;
        }
      }
      this->Invalidate();
      break;

    case 0x13D: {
      std::size_t count;
      if (this->unknown_01D4 == 1) {
        count = this->searchResults.size();
      } else {
        count = RodexMailsOfType(g_rodexMailStore->unknown_0010)->size();
      }
      if (count > static_cast<std::size_t>((this->unknown_019C + 1) * 6)) {
        this->ClearChecks();
      }
      break;
    }

    case 0x141:
      if (this->unknown_01C0[0]->geometry.unknown_0030 == 1) {
        this->SearchByTitle(this->unknown_01C8->Text());
      } else if (this->unknown_01C0[1]->geometry.unknown_0030 == 1) {
        this->SearchBySender(this->unknown_01C8->Text());
      }
      this->SendMsg(this->unknown_01D0, 6, 0xD7, 0, 0, 0);
      this->Invalidate();
      break;

    case 0x19F: {
      g_rodexMailStore->PurgeFlagged();
      for (int i = 0; i < 6; ++i) {
        this->checks[i]->SetChecked(0);
        this->checks[i]->m_message = 0;
      }

      PACKET_CZ_OPEN_RODEXBOX packet;
      packet.packetType = ID_CZ_OPEN_RODEXBOX;
      packet.lastMailId0 = 0;
      if (g_rodexMailStore->m_mails0.size() > 0) {
        packet.lastMailId0 = (--g_rodexMailStore->m_mails0.end())->first;
      }
      packet.lastMailId2 = 0;
      if (g_rodexMailStore->m_mails2.size() > 0) {
        packet.lastMailId2 = (--g_rodexMailStore->m_mails2.end())->first;
      }
      packet.lastMailId1 = 0;
      if (g_rodexMailStore->m_mails1.size() > 0) {
        packet.lastMailId1 = (--g_rodexMailStore->m_mails1.end())->first;
      }
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      this->SetButtonsDisabled(1);
      break;
    }

    case 0x222: {
      std::string type;
      if (this->unknown_01D4 == 1) {
        type = LookupMessage(0xDDD);
      } else {
        switch (g_rodexMailStore->unknown_0010) {
        case 0:
          type = LookupMessage(0xDDB);
          break;
        case 1:
          type = LookupMessage(0xDDA);
          break;
        case 2:
          type = LookupMessage(0xDDC);
          break;
        }
      }

      const std::string text = FormatOwnedString(LookupMessage(0xE06), type.c_str());
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
        this->RequestAllMailActions();
      }
      break;
    }

    case 0x223: {
      if (this->unknown_01D4 == 1) {
        std::vector<RodexMailEntry> selected;
        for (int i = 0; i < 6; ++i) {
          UICheckBox* check = this->checks[i];
          if (check->geometry.visible == 1 && check->geometry.unknown_0030 != 0 && check->m_message != -1) {
            selected.push_back(this->searchResults[this->unknown_019C * 6 + i]);
          }
        }

        if (selected.size() != 0 &&
            reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xE09), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
          for (std::vector<RodexMailEntry>::iterator entry = selected.begin(); entry != selected.end(); ++entry) {
            RequestRodexMailAttachments(entry->first, entry->second);
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
          if (check->geometry.visible == 1 && check->geometry.unknown_0030 != 0 && check->m_message != -1) {
            selected.push_back(check->m_message);
          }
        }

        if (selected.size() != 0 &&
            reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xE09), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
          for (std::vector<int>::iterator id = selected.begin(); id != selected.end(); ++id) {
            RequestRodexMailAttachments(g_rodexMailStore->unknown_0010, *id);
          }
          for (int i = 0; i < 6; ++i) {
            this->checks[i]->SetChecked(0);
            this->checks[i]->m_message = 0;
          }
        }
      }
      break;
    }

    case 0x224: {
      std::string type;
      if (this->unknown_01D4 == 1) {
        type = LookupMessage(0xDDD);
      } else {
        switch (g_rodexMailStore->unknown_0010) {
        case 0:
          type = LookupMessage(0xDDB);
          break;
        case 1:
          type = LookupMessage(0xDDA);
          break;
        case 2:
          type = LookupMessage(0xDDC);
          break;
        }
      }

      const std::string text = FormatOwnedString(LookupMessage(0xE0A), type.c_str());
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
        if (this->unknown_01D4 == 1) {
          for (std::vector<RodexMailEntry>::iterator entry = this->searchResults.begin(); entry != this->searchResults.end(); ++entry) {
            if (entry->first == 0) {
              std::map<std::int64_t, RodexMail>::iterator mail = g_rodexMailStore->m_mails0.find(entry->second);
              if (mail != g_rodexMailStore->m_mails0.end() && mail->second.unknown_0194 <= 0) {
                continue;
              }
            }
            RequestRodexMailAttachments(entry->first, entry->second);
          }
        } else {
          const int mailType = g_rodexMailStore->unknown_0010;
          std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(mailType);
          if (mails != nullptr) {
            for (std::map<std::int64_t, RodexMail>::iterator mail = mails->begin(); mail != mails->end(); ++mail) {
              if (mail->second.unknown_0194 > 0) {
                RequestRodexMailAttachments(mailType, mail->first);
              }
            }
          }
        }
      }
      break;
    }
    }
    break;

  case 0x17:
    this->RefreshMailLists();
    this->InitializeMailActions();
    this->SetButtonsDisabled(0);
    this->Invalidate();
    break;

  case 0x3C:
    this->Invalidate();
    break;

  case 0x62:
    if (argument1 == 0x142) {
      if (g_rodexMailStore->unknown_0010 == 0) {
        std::map<std::int64_t, RodexMail>::iterator mail = g_rodexMailStore->m_mails0.find(argument2);
        if (mail != g_rodexMailStore->m_mails0.end() && (mail->second.unknown_0001 & 8) == 0 && mail->second.unknown_0004.size() != 0) {
          const std::pair<bool, std::string> title = RemoveLineBreakTag(mail->second.unknown_0024);
          if (title.first != true) {
            mode->SendMsg(0x10C, reinterpret_cast<int>(mail->second.unknown_0004.c_str()), 0, 0, 0);
          }
        }
      }
    } else if (argument1 == 0x143) {
      std::map<std::int64_t, RodexMail>::iterator mail = g_rodexMailStore->m_mails0.find(argument2);
      if (mail != g_rodexMailStore->m_mails0.end() && mail->second.unknown_0194 == 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xF1B)), 0xFF, 0, 0, 0);
        if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xF1B), 2, false, false, 0, "Message", 280, 120, 0) == 0xBC) {
          break;
        }
      }
      mode->SendMsg(0xC2, argument2, g_rodexMailStore->unknown_0010, 0, 0);
    }
    break;

  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}
