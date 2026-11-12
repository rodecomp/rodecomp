#include "Client/Mode/ModeMessage.h"
#include "UI/Login/UIChangeNameWnd.h"
#include "UI/Window/UIEnums.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Navigation/NavigationLink.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstdio>
#include <cstring>
#include <string>

template __declspec(noinline) std::string::size_type std::string::find(const char, const std::string::size_type) const noexcept;

#pragma pack(push, 1)
struct CharacterRenamePacket {
  /* 0000 */ short id;
  /* 0002 */ std::uint32_t gid;
  /* 0006 */ char name[24];
};
#pragma pack(pop)

static_assert(sizeof(CharacterRenamePacket) == 30);

// GLOBAL: gl_25092
std::uint32_t g_renameGid;

// GLOBAL: gl_25093
char g_renameName[24];

// OFFSET: 0x0092BCA0
// FUNCTION: fn_12660
void UIChangeNameWnd::Paint() {
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\login_interface\\change_name.bmp"))), false);
}

// OFFSET: 0x0091EDA0
// FUNCTION: fn_12637
void UIChangeNameWnd::OnCreate(int, int) {
  {
    const std::string prefix("유저인터페이스\\");
    this->m_okButton = new UIBitmapButton();
    this->m_okButton->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->m_okButton->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->m_okButton->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->m_okButton->SetSize(this->m_okButton->BitmapWidth(), this->m_okButton->BitmapHeight());
    this->m_okButton->Move(this->geometry.width - 93, this->geometry.height - 24);
    this->m_okButton->SetId(184);
    this->AddChild(this->m_okButton);
  }
  {
    const std::string prefix("유저인터페이스\\");
    this->m_cancelButton = new UIBitmapButton();
    this->m_cancelButton->SetBitmap((prefix + "btn_cancel" + ".bmp").c_str(), 0, 0);
    this->m_cancelButton->SetBitmap((prefix + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
    this->m_cancelButton->SetBitmap((prefix + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
    this->m_cancelButton->SetSize(this->m_cancelButton->BitmapWidth(), this->m_cancelButton->BitmapHeight());
    this->m_cancelButton->Move(this->geometry.width - 46, this->geometry.height - 24);
    this->m_cancelButton->SetId(185);
    this->AddChild(this->m_cancelButton);
  }

  g_modeMgr.gameMode->SendMsg(LMM_GET_MAKING_CHAR_NAME, 0, 0, 0, 0);
  this->m_edit = new UIBitmapEditCtrl;
  this->m_edit->SetBitmapName("유저인터페이스\\login_interface\\name-edit.bmp");
  this->m_edit->SetSize(this->m_edit->geometry.width, this->m_edit->geometry.height);
  this->m_edit->Move(60, 44);
  this->m_edit->maximum = 24;
  this->m_edit->SetId(184);
  this->AddChild(this->m_edit);
  g_windowMgr.SetFocus(this->m_edit);
  this->m_message0 = 184;
}

// OFFSET: 0x00911190
// FUNCTION: fn_12511
UIChangeNameWnd::UIChangeNameWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x00934A10
// FUNCTION: fn_12743
int UIChangeNameWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  switch (message) {
  case UM_DEFPUSH:
    this->SendMsg(nullptr, UM_COMMAND, this->m_message0, 0, 0, 0);
    break;
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      if (GetNavigationLinkParser()->HasLink(this->m_edit->Text()) == true) {
        g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON), 0, true, false, 0, "Message", 280, 120, 0);
        this->m_edit->SetText("");
        return 0;
      }

      std::string name(this->m_edit->Text());
      if (name.find('\n', 0) != std::string::npos) {
        g_windowMgr.Confirm(LookupMessage(MSI_TOKENAGENCY_SERVER_CONNECTION_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
        this->m_edit->SetText("");
        break;
      }

      if (g_serviceType == 1 && !ValidateCharacterName(this->m_edit->Text())) {
        g_windowMgr.Confirm(LookupMessage(MSI_CHARACTERNAME_ERROR), 0, true, false, 0, "Message", 280, 120, 0);
        this->m_edit->SetText("");
        return 0;
      }

      if (this->m_edit->Length() < 4) {
        g_windowMgr.Confirm(LookupMessage(MSI_ENTER_NAME_MORE_THAN_4_CHAR), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }

      const int allowed = g_forbiddenWords.Allows(this->m_edit->Text());
      const int blank = g_session.IsBlankString(this->m_edit->Text());
      if (!allowed || blank) {
        if (!allowed) {
          g_windowMgr.Confirm(LookupMessage(MSI_BAD_NAME), 0, true, false, 0, "Message", 280, 120, 0);
        }
        if (blank) {
          g_windowMgr.Confirm(LookupMessage(MSI_NO_SPACE_IN_NAME), 0, true, false, 0, "Message", 280, 120, 0);
        }
        break;
      }

      if (this->m_edit->Length() >= 24 || !this->IsNameWithinLimit(this->m_edit->Text(), 24)) {
        g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
        this->m_edit->SetText("");
        break;
      }

      char newName[24] = {};
      std::strcpy(newName, this->m_edit->Text());
      memcpy_s(&this->m_info, sizeof(this->m_info), this->m_character, sizeof(CharacterInfo));
      std::memcpy(this->m_info.name, newName, sizeof(newName));
      if (std::strstr(this->m_info.name, "그라비티") != nullptr) {
        return 0;
      }

      _strlwr(newName);
      if (std::strstr(newName, "gravity") != nullptr) {
        return 0;
      }

      if ((newName[0] == '[' && newName[1] == 'g' && newName[2] == 'm') || (newName[1] == '[' && newName[2] == 'g' && newName[3] == 'm') ||
          (newName[2] == '[' && newName[3] == 'g' && newName[4] == 'm')) {
        return 0;
      }

      char text[300];
      std::sprintf(text, LookupMessage(MSI_CONFIRM_TO_CHANGE_NAME), this->m_info.name);
      if (g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) == 187) {
        CharacterRenamePacket packet;
        packet.id = ID_CH_REQ_CHANGE_CHARACTERNAME;
        packet.gid = this->m_character->gid;
        g_renameGid = packet.gid;
        std::memcpy(packet.name, this->m_info.name, sizeof(packet.name));
        std::memcpy(g_renameName, this->m_info.name, sizeof(g_renameName));
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
        g_windowMgr.Schedule(this);
      }
      break;
    }
    case ID_CANCEL:
      g_windowMgr.Schedule(this);
      break;
    }
    break;
  case UM_SETINFO:
    this->m_character = reinterpret_cast<CharacterInfo*>(a);
    this->m_edit->SetText(this->m_character->name);
    break;
  }

  return 0;
}
