#include "Client/Application/ApplicationActivation.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "UI/GameSettings/GameSettingsUI.h"

// OFFSET: 0x00E5A190
// FUNCTION: fn_24369
void SetApplicationActive(int value) {
  g_isAppActive = value;
  if (value != 0) {
    SetPriorityClass(GetCurrentProcess(), dwPriorityClass);
    if (g_serviceOptions.Get(ServiceOption::ConfineCursor) != 0) {
      ClipApplicationCursor(hWnd);
    }
  } else {
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    ClipCursor(nullptr);
    reinterpret_cast<AutomaticSkills*>(g_session.unknown_7FB4)->DisableAll();
  }
}
