#include "UI/Rodex/UIRodexInboxWnd.h"

#include "Contents/MessageTable.h"
#include "Client/Mode/ModeMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>

namespace {
#pragma pack(push, 1)
struct RodexMailRequestPacket {
  std::uint16_t id;
  std::int64_t mailId;
  std::uint8_t type;
};
#pragma pack(pop)
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
bool RequestRodexMailAction(int type, std::uint64_t mailId) {
  std::map<std::int64_t, RodexMail>* mails = RodexMailsOfType(type);
  if (mails == nullptr) {
    return false;
  }

  std::map<std::int64_t, RodexMail>::iterator mail = mails->find(static_cast<std::int64_t>(mailId));
  if (mail == mails->end() || (mail->second.unknown_0001 & 6) != 0) {
    return false;
  }

  g_modeMgr.GetGameMode()->SendMsg(0xC4, static_cast<int>(mailId), type, 0, 0);
  return true;
}
