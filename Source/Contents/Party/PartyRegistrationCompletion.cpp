#include "Contents/Party/AdventurerAgencyMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CCD20
// FUNCTION: fn_03666
void CAdventurerAgencyMgr::CompleteRegistration(AsyncDiagnostic diagnostic, std::string response) {
  --this->pending;

  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;

  if (!reader.parse(response.c_str(), root, true)) {
    return;
  }

  const int type = root["Type"].asInt();

  switch (type) {
    case 2:
    case 3:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ADVEN_TANKER)), 0xFF, 0, 0, 0);
      return;
  }

  this->registered = true;
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BASIC_EXP_ZERO)), 0xFFFF64, 0, 0, 0);
  UIWindow* board = g_windowMgr.LookupWindow(WID_ADVENPARTYBOARDWND);

  if (board == nullptr) {
    g_windowMgr.EnsureWindow(WID_ADVENPARTYBOARDWND);
  } else {
    board->SendMsg(nullptr, UM_COMMAND, ID_PARTY_RELOAD_FIRST_PAGE, 0, 0, 0);
  }
}
