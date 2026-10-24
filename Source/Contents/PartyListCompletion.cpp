#include "Contents/AdventurerAgencyMgr.h"
#include "UI/UIAdvenPartyBoardWnd.h"
#include "UI/WindowManagerWindows.h"
#include "Utility/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CC000
// FUNCTION: fn_03662
void CAdventurerAgencyMgr::CompleteList(
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

  this->LoadList(root["data"]);
  auto* board = static_cast<UIAdvenPartyBoardWnd*>(windows.EnsureWindow(324));

  if (board) {
    const int pages = root.get("totalPage", 0).asInt();

    if (pages != 0) {
      board->totalPages = pages;
    }

    board->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}
