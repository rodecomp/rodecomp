#include "Client/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Contents/MessageTable.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/UIWindowMgr.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CD0A0
// FUNCTION: fn_03668
void CAdventurerAgencyMgr::CompleteStopRecruiting(AsyncDiagnostic diagnostic, std::string response) {
  --this->pending;

  if (!diagnostic.success) {
    if (this->unknown_0008 == g_session.m_aid) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3507)), 0xFF, 0, 0, 0);
    }

    return;
  }

  Json::Value root;
  Json::Reader reader;

  if (!reader.parse(response.c_str(), root, true)) {
    return;
  }

  switch (root["Type"].asInt()) {
  case 2:
  case 3:
    if (this->unknown_0008 == g_session.m_aid) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3507)), 0xFF, 0, 0, 0);
    }

    return;
  }

  if (this->unknown_0008 == g_session.m_aid) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3459)), 0xFFFF64, 0, 0, 0);
  }

  this->registered = false;
  auto* board = static_cast<UIAdvenPartyBoardWnd*>(g_windowMgr.LookupWindow(324));

  if (board) {
    board->stop->Enable(0);
    board->SendMsg(nullptr, 6, 415, 0, 0, 0);
  }
}
