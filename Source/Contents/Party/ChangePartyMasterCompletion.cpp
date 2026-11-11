#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CB520
// FUNCTION: fn_03658
void CAdventurerAgencyMgr::CompleteChangeMaster(AsyncDiagnostic diagnostic, std::string response) {
  --this->pending;

  if (!diagnostic.success) {
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
    break;
  default:
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_ADVENPARTYBOARDWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_PARTY_RELOAD_FIRST_PAGE, 0, 0, 0);
    }
    break;
  }
}
