#include "Client/Session.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Contents/MessageTable.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"

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

  if (!reader.parse(response.c_str(), root, true)) {
    return;
  }

  if (root["Type"].asInt() == 2 || root["Type"].asInt() == 3) {
    return;
  }

  auto entry = root["data"].begin();

  if (entry == root["data"].end()) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3443)), 0xFF, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
    return;
  }

  CAdvenPartyInfo info;
  info.characterName = (*entry)["CharName"].asString();
  info.accountId = static_cast<std::uint32_t>((*entry).get("AID", 0).asDouble());
  info.characterId = static_cast<std::uint32_t>((*entry).get("GID", 0).asDouble());
  info.assist = (*entry).get("Assist", 1).asInt() != 0;
  info.healer = (*entry).get("Healer", 1).asInt() != 0;
  info.dealer = (*entry).get("Dealer", 1).asInt() != 0;
  info.tanker = (*entry).get("Tanker", 1).asInt() != 0;
  info.minimumLevel = (*entry).get("MinLV", 0).asInt();
  info.maximumLevel = (*entry).get("MaxLV", 999).asInt();
  info.type = (*entry).get("Type", 1).asInt();
  info.memo = (*entry).get("Memo", "").asString();
  UIWindow* board = g_windowMgr.LookupWindow(WID_ADVENPARTYBOARDWND);

  if (board) {
    for (auto& value : this->entries) {
      if (value.accountId == info.accountId) {
        value = info;
        break;
      }
    }

    board->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }

  auto* application = static_cast<UIApplyForPartyWnd*>(g_windowMgr.LookupWindow(WID_APPLYFORPARTYWND));

  if (application) {
    application->SetInfo(info);
    g_windowMgr.Promote(application);
  }

  if (info.minimumLevel > g_session.m_level || info.maximumLevel < g_session.m_level) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3416)), 0xFF, 0, 0, 0);
  } else if (g_session.PartyCount() > 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(79)), 0xFF, 0, 0, 0);
  } else {
    g_modeMgr.gameMode->SendMsg(MM_PARTY_REQ_MASTER_TO_JOIN, reinterpret_cast<int>(&info.accountId), reinterpret_cast<int>(&info.characterId), 0, 0);
    g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
  }
}
