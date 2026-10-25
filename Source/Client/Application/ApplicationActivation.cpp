#include "Client/Application/ApplicationActivation.h"
#include "Client/Application/ApplicationWindow.h"
#include "Contents/Configuration/ServiceOptions.h"

// OFFSET: 0x00E5A190
// FUNCTION: fn_24369
void ApplicationActivation::Set(int value) {
  this->active = value;
  if (value != 0) {
    SetPriorityClass(GetCurrentProcess(), this->priority);
    if (this->service.Get(ServiceOption::ConfineCursor) != 0) {
      ClipApplicationCursor(this->window);
    }
  } else {
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    ClipCursor(nullptr);
    this->automaticSkills.DisableAll();
  }
}
