#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CD0A0
// FUNCTION: fn_03668
void CAdventurerAgencyMgr::CompleteStopRecruiting(AsyncDiagnostic diagnostic, std::string response) {
  --this->pending;

  if (!diagnostic.success) {
    if (this->masterAccountId == g_session.m_aid) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXPAND_SIGHT_HELP)), 0xFF, 0, 0, 0);
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
    if (this->masterAccountId == g_session.m_aid) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXPAND_SIGHT_HELP)), 0xFF, 0, 0, 0);
    }

    return;
  }

  if (this->masterAccountId == g_session.m_aid) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FORCED_WEB_LOGIN)), 0xFFFF64, 0, 0, 0);
  }

  this->registered = false;
  auto* board = static_cast<UIAdvenPartyBoardWnd*>(g_windowMgr.LookupWindow(WID_ADVENPARTYBOARDWND));

  if (board) {
    board->stop->Enable(0);
    board->SendMsg(nullptr, UM_COMMAND, ID_PARTY_RELOAD_FIRST_PAGE, 0, 0, 0);
  }
}
