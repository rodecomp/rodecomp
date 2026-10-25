#include "Client/Mode/ModeEvents.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
#include "Contents/MessageTable.h"
#include "Resource/ResourceFailures.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CC3A0
// FUNCTION: fn_03664
void CAdventurerAgencyMgr::CompletePartyInfo(AsyncDiagnostic diagnostic, std::string response, PartyInfoContext& context) {
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

  if (root["Type"].asInt() == 2 || root["Type"].asInt() == 3) {
    return;
  }

  const auto notify = [&](int id) { context.messages.Message(LookupMessage(id, context.languagePrefix, context.service, context.language), 0xFF); };
  auto entry = root["data"].begin();

  if (entry == root["data"].end()) {
    notify(3443);
    context.windows.CloseWindow(345);
    return;
  }

  CAdvenPartyInfo info;
  info.characterName = (*entry)["CharName"].asString();
  info.accountId = Id((*entry).get("AID", 0).asDouble());
  info.characterId = Id((*entry).get("GID", 0).asDouble());
  info.assist = (*entry).get("Assist", 1).asInt() != 0;
  info.healer = (*entry).get("Healer", 1).asInt() != 0;
  info.dealer = (*entry).get("Dealer", 1).asInt() != 0;
  info.tanker = (*entry).get("Tanker", 1).asInt() != 0;
  info.minimumLevel = (*entry).get("MinLV", 0).asInt();
  info.maximumLevel = (*entry).get("MaxLV", 999).asInt();
  info.type = (*entry).get("Type", 1).asInt();
  info.memo = (*entry).get("Memo", "").asString();
  UIWindow* board = context.windows.LookupWindow(324);

  if (board) {
    for (auto& value : this->entries) {
      if (value.accountId == info.accountId) {
        value = info;
        break;
      }
    }

    board->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }

  auto* application = static_cast<UIApplyForPartyWnd*>(context.windows.LookupWindow(345));

  if (application) {
    application->SetInfo(info);
    context.ordering.Promote(application);
  }

  if (info.minimumLevel > context.playerLevel || info.maximumLevel < context.playerLevel) {
    notify(3416);
  } else if (context.partyCount > 0) {
    notify(79);
  } else {
    context.mode->SendModeMessage(265, reinterpret_cast<std::intptr_t>(&info.accountId), reinterpret_cast<std::intptr_t>(&info.characterId), 0, 0);
    context.windows.CloseWindow(345);
  }
}
