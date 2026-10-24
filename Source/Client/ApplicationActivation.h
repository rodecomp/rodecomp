#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATIONACTIVATION_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATIONACTIVATION_H

#include "Contents/AutomaticSkills.h"
#include "Contents/ServiceOptions.h"

namespace client {
  // Borrowed original main-window/priority/active globals and concrete owners.
  struct ApplicationActivation {
    HWND& window;
    const DWORD& priority;
    int& active;
    ServiceOptions& service;
    AutomaticSkills& automaticSkills;

    // Publishes value verbatim. Nonzero restores configured process priority
    // and clips only when option 271 is enabled. Zero selects idle priority,
    // releases clipping, then cancels automatic skills through their connection.
    void Set(int value);
  };
}

#endif
