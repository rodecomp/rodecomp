#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/StopRecruitingAsyncWork.h"
#include "Resource/ResourceFailures.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/WindowManagerWindows.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CD0A0
// FUNCTION: fn_03668
void CAdventurerAgencyMgr::CompleteStopRecruiting(AsyncDiagnostic diagnostic, std::string response, StopRecruitingContext& context) {
  --this->pending;
  const auto notify = [&](int id, unsigned color) {
    context.messages.Message(LookupMessage(id, context.languagePrefix, context.service, context.language), color);
  };

  if (!diagnostic.success) {
    if (this->unknown_0008 == context.accountId) {
      notify(3507, 0xFF);
    }

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
    if (this->unknown_0008 == context.accountId) {
      notify(3507, 0xFF);
    }

    return;
  }

  if (this->unknown_0008 == context.accountId) {
    notify(3459, 0xFFFF64);
  }

  this->registered = false;
  auto* board = static_cast<UIAdvenPartyBoardWnd*>(context.windows.LookupWindow(324));

  if (board) {
    board->stop->Enable(0);
    board->SendMsg(nullptr, 6, 415, 0, 0, 0);
  }
}
