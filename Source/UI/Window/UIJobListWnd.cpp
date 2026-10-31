#include "UI/Window/UIJobListWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <windows.h>

#include <cstdio>
#include <string>

// GLOBAL: gl_25023
std::map<int, bool> g_partyBookingJobs;

struct PartyBookingUpdatePacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t jobs[6];
};

static_assert(sizeof(PartyBookingUpdatePacket) == 14);

// OFFSET: 0x00920D60
// FUNCTION: fn_12640
void UIJobListWnd::Slot15(int width, int height) {
  char title[9] = {};
  std::sprintf(title, "Job List");
  this->m_title = new UIStaticText(*this->m_context, *this->m_controls);
  this->m_title->SetSize(this->geometry.width - 32, 15);
  this->m_title->SetTextFitted(title);
  this->m_title->Move(16, 2);
  this->m_title->shadow = 1;
  this->AddChild(this->m_title);
  this->AddCloseButton(*this->m_controls, "", 0);

  const std::string path = "유저인터페이스\\";
  this->m_okButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_okButton->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
  this->m_okButton->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  this->m_okButton->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  this->m_okButton->SetSize(this->m_okButton->BitmapWidth(), this->m_okButton->BitmapHeight());
  this->m_okButton->Move(this->geometry.width - 92, this->geometry.height - 24);
  this->m_okButton->SetId(184);
  this->AddChild(this->m_okButton);

  for (int index = 0; index < 6; ++index) {
    this->m_jobChecks[index] = nullptr;
  }
}

// OFFSET: 0x0092C640
// FUNCTION: fn_12664
void UIJobListWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, "", 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x00940DE0
// FUNCTION: fn_12780
void UIJobListWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00935610
// FUNCTION: fn_12746
int UIJobListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    if (message != 34) {
      return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }

    this->m_saved = reinterpret_cast<JobListWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    this->SendMsg(nullptr, 14, 3, this->m_saved->width, this->m_saved->height, 0);
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    return 0;
  }

  if (argument1 == 184) {
    PartyBookingUpdatePacket packet = {ID_CZ_PARTY_BOOKING_REQ_UPDATE};
    for (int i = 0; i < 6; ++i) {
      packet.jobs[i] = -1;
    }

    int selected = 0;
    int index = 0;
    for (auto it = g_partyBookingJobs.begin(); it != g_partyBookingJobs.end(); ++it, ++index) {
      UIWindow* check = this->m_jobChecks[index];
      std::int16_t job = -1;
      bool enabled;
      if (check == nullptr) {
        enabled = false;
      } else if (check->geometry.unknown_0030 == 0) {
        enabled = false;
      } else {
        ++selected;
        enabled = true;
        job = static_cast<std::int16_t>(it->first);
      }
      packet.jobs[index] = job;
      it->second = enabled;
    }

    if (selected == 0) {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1767), 0, true, false, 0, "Message", 280, 120, 0);
      return 0;
    }

    GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
  } else if (argument1 != 201) {
    return 0;
  }

  g_windowMgr.CloseWindow(170);
  return 0;
}

// OFFSET: 0x00944C90
// FUNCTION: fn_12834
void UIJobListWnd::RefreshJobs() {
  const std::string path = "유저인터페이스\\";
  for (int index = 0; index < 6; ++index) {
    if (this->m_jobChecks[index]) {
      this->RemoveChild(this->m_jobChecks[index], 1);
      this->m_jobChecks[index] = nullptr;
    }
  }

  int index = 0;
  for (auto it = g_partyBookingJobs.begin(); it != g_partyBookingJobs.end(); ++it, ++index) {
    this->m_jobChecks[index] = new UICheckBox(*this->m_context, *this->m_controls);
    this->m_jobChecks[index]->SetSize(120, 15);
    const int job = it->first;
    if (job == -1) {
      continue;
    }

    if (job - 1 < 4) {
      this->m_jobChecks[index]->SetImages(
        (path + "basic_interface\\" + "grp_online" + ".bmp").c_str(), (path + "basic_interface\\" + "grp_offline" + ".bmp").c_str(),
        LookupMessage(job + 0x6AF)
      );
    } else {
      const char* text = LookupMessage(job + 0x658);
      const std::string unchecked = path + "basic_interface\\" + "grp_offline" + ".bmp";
      const std::string checked = path + "basic_interface\\" + "grp_online" + ".bmp";
      this->m_jobChecks[index]->SetImages(checked.c_str(), unchecked.c_str(), text);
    }

    this->m_jobChecks[index]->Move(10, (index + 1) * 25);
    this->m_jobChecks[index]->SetChecked(it->second);
    this->AddChild(this->m_jobChecks[index]);
  }
}
