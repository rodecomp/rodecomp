#ifndef RAGNAROK_SOURCE_CONTENTS_NAVIGATIONMANAGER_H
#define RAGNAROK_SOURCE_CONTENTS_NAVIGATIONMANAGER_H

#include <cstddef>
#include <string>

class NavigationObject;

struct NavigationTarget {
  /* 0000 */ int type;
  /* 0004 */ NavigationObject* object;
  /* 0008 */ int x;
  /* 000C */ int y;
  /* 0010 */ std::string name;
};

class NavigationManager {
public:
  NavigationTarget GetCurrentTarget() const;
  NavigationTarget GetSavedTarget() const;
  int GetCurrentTargetType() const;
  int SaveCurrentTarget();

  /* 0000 */ unsigned char unknown_0000[0x10FC];
  /* 10FC */ NavigationTarget currentTarget;
  /* 1124 */ NavigationTarget savedTarget;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(NavigationManager, savedTarget) == 0x1124);
#endif

#endif
