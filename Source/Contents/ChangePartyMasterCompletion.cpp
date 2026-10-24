#include "Contents/AdventurerAgencyMgr.h"
#include "UI/UIWindow.h"
#include "UI/WindowManagerWindows.h"
#include "Utility/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005CB520
// FUNCTION: fn_03658
void CAdventurerAgencyMgr::CompleteChangeMaster(
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

  const int type = root["Type"].asInt();

  if (type != 2 && type != 3) {
    if (UIWindow* window = windows.LookupWindow(324)) {
      window->SendMsg(nullptr, 6, 415, 0, 0, 0);
    }
  }
}
