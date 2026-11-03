#include "Client/Session.h"
#include "Client/Mode/CMode.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Contents/MessageTable.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowOrdering.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CC3A0
// FUNCTION: fn_03664
void CAdventurerAgencyMgr::CompletePartyInfo(AsyncDiagnostic diagnostic, std::string response) {
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

  const auto notify = [&](int id) { g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(id)), 0xFF, 0, 0, 0); };
  auto entry = root["data"].begin();

  if (entry == root["data"].end()) {
    notify(3443);
    g_windowMgr.CloseWindow(345);
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
  UIWindow* board = g_windowMgr.LookupWindow(324);

  if (board) {
    for (auto& value : this->entries) {
      if (value.accountId == info.accountId) {
        value = info;
        break;
      }
    }

    board->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }

  auto* application = static_cast<UIApplyForPartyWnd*>(g_windowMgr.LookupWindow(345));

  if (application) {
    application->SetInfo(info);
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(application);
  }

  if (info.minimumLevel > g_session.m_level || info.maximumLevel < g_session.m_level) {
    notify(3416);
  } else if (g_session.PartyCount() > 0) {
    notify(79);
  } else {
    static_cast<CMode*>(g_charViewMode)->SendMsg(265, reinterpret_cast<int>(&info.accountId), reinterpret_cast<int>(&info.characterId), 0, 0);
    g_windowMgr.CloseWindow(345);
  }
}
