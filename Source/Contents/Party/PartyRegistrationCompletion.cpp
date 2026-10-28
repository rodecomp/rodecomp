#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/RegisterPartyAsyncWork.h"
#include "Resource/ResourceFailures.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CCD20
// FUNCTION: fn_03666
void CAdventurerAgencyMgr::CompleteRegistration(AsyncDiagnostic diagnostic, std::string response, RegisterPartyContext& context) {
  --this->pending;

  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;
  const std::string document(response.c_str());

  if (!reader.parse(document, root, true)) {
    return;
  }

  const int type = root["Type"].asInt();

  if (type == 2 || type == 3) {
    context.messages.Message(LookupMessage(3417), 0xFF);
    return;
  }

  this->registered = true;
  context.messages.Message(LookupMessage(3418), 0xFFFF64);
  UIWindow* board = context.windows.LookupWindow(324);

  if (board) {
    board->SendMsg(nullptr, 6, 415, 0, 0, 0);
  } else {
    context.windows.EnsureWindow(324);
  }
}
