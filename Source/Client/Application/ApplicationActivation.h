#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONACTIVATION_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONACTIVATION_H

#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/Configuration/ServiceOptions.h"

struct ApplicationActivation {
  HWND& window;
  const DWORD& priority;
  int& active;
  ServiceOptions& service;
  AutomaticSkills& automaticSkills;

  void Set(int value);
};

#endif
