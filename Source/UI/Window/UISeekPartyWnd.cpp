#include "UI/Window/UISeekPartyWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIJobListWnd.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>


// OFFSET: 0x00912010
// FUNCTION: fn_12525
UISeekPartyWnd::UISeekPartyWnd() : UIFrameWnd(0) {
  this->m_label = nullptr;
  this->m_cancelButton = nullptr;
  this->m_level = 1;
  this->m_type = 0;
  this->m_frameDuration = 1000;
  this->m_elapsed = 0;
  this->m_frameElapsed = 0;
  this->m_frameTick = 0;
  this->expandedHeight = 0;
  this->m_minimized = 0;
  this->m_elapsed = GetTickCount() - g_session.m_partyBooking.unknown_0034;
}

// OFFSET: 0x009281A0
// FUNCTION: fn_12651
void UISeekPartyWnd::OnCreate(int, int) {
  char text[0x80] = {};
  const int minimum = g_session.m_partyBookingLevelLimit;
  int maximum = g_session.m_maxLevel;
  if (minimum + 0xF <= maximum) {
    maximum = minimum + 0xF;
  }
  std::sprintf(text, "%s - %d~%d", LookupMessage(MSI_PARTY_BOOKING_REQURT), minimum, maximum);

  this->m_label = new UIStaticText();
  this->m_label->SetSize(this->geometry.width - 0x20, 0xF);
  this->m_label->SetTextFitted(text);
  this->m_label->Move(0x10, 2);
  this->m_label->shadow = 1;
  this->AddChild(this->m_label);

  this->AddCloseButton("", 0);
  this->AddMinimizeButton("", 0);

  {
    const std::string path = "유저인터페이스\\";
    this->m_cancelButton = new UIBitmapButton();
    this->m_cancelButton->SetBitmap((path + "btn_cancel" + ".bmp").c_str(), 0, 0);
    this->m_cancelButton->SetBitmap((path + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
    this->m_cancelButton->SetBitmap((path + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
    this->m_cancelButton->SetSize(this->m_cancelButton->BitmapWidth(), this->m_cancelButton->BitmapHeight());
    this->m_cancelButton->Move(this->geometry.width - 0x2E, this->geometry.height - 0x18);
    this->m_cancelButton->SetId(0xC9);
    this->AddChild(this->m_cancelButton);
  }

  {
    const std::string path = "유저인터페이스\\";
    this->m_rewriteButton = new UIBitmapButton();
    this->m_rewriteButton->SetBitmap((path + "btn_rewrite" + ".bmp").c_str(), 0, 0);
    this->m_rewriteButton->SetBitmap((path + "btn_rewrite_a" + ".bmp").c_str(), 1, 0);
    this->m_rewriteButton->SetBitmap((path + "btn_rewrite_b" + ".bmp").c_str(), 2, 0);
    this->m_rewriteButton->SetSize(this->m_rewriteButton->BitmapWidth(), this->m_rewriteButton->BitmapHeight());
    this->m_rewriteButton->Move(this->geometry.width - 0x5C, this->geometry.height - 0x18);
    this->m_rewriteButton->SetId(0x11C);
    this->AddChild(this->m_rewriteButton);
  }

  this->m_type = std::rand() % 5;
  this->m_frameDuration = 1000;
}

// OFFSET: 0x00944B20
// FUNCTION: fn_12833
void UISeekPartyWnd::SetJobs(const std::int16_t* jobs) {
  g_partyBookingJobs.clear();
  for (int index = 0; index < 6; ++index) {
    if (jobs[index] < 0) {
      break;
    }
    g_partyBookingJobs.insert(std::pair<const int, bool>(jobs[index], true));
  }
}
