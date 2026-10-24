#include "Contents/AdventurerAgencyMgr.h"
#include "UI/UIAdvenPartyBoardWnd.h"
#include "UI/WindowManagerWindows.h"
#include "Utility/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CB810
// FUNCTION: fn_03660
void CAdventurerAgencyMgr::CompleteMyParty(
  AsyncDiagnostic diagnostic,
  std::string response,
  WindowManagerWindows& windows
) {
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

  if (root["data"].isNull()) {
    this->registered = false;
    auto* board = static_cast<UIAdvenPartyBoardWnd*>(windows.EnsureWindow(324));

    if (board) {
      board->Registration(false);
    }

    return;
  }

  const Json::Value data = root["data"];
  CAdvenPartyInfo info;
  info.characterName = data.get("CharName", "").asString();
  info.accountId = Id(data.get("AID", 0).asDouble());
  info.characterId = Id(data.get("GID", 0).asDouble());
  info.assist = data.get("Assist", 1).asInt() != 0;
  info.healer = data.get("Healer", 1).asInt() != 0;
  info.dealer = data.get("Dealer", 1).asInt() != 0;
  info.tanker = data.get("Tanker", 1).asInt() != 0;
  info.minimumLevel = data.get("MinLV", 0).asInt();
  info.maximumLevel = data.get("MaxLV", 999).asInt();
  info.type = data.get("Type", 1).asInt();
  info.memo = data.get("Memo", "").asString();
  auto* board = static_cast<UIAdvenPartyBoardWnd*>(windows.EnsureWindow(324));

  if (board) {
    board->SetOwnParty(info);
  }

  // The board update precedes this store in the original, including reentry.
  this->registered = true;
}
