#ifndef RAGNAROK_SOURCE_CONTENTS_NAVIGATIONMANAGER_H
#define RAGNAROK_SOURCE_CONTENTS_NAVIGATIONMANAGER_H

#include <cstddef>
#include <string>
#include <vector>

class NavigationObject {
public:
  bool HasName(const char* name) const;

  /* 0000 */ unsigned char unknown_0000[8];
  /* 0008 */ std::string name;
};

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
  NavigationObject* FindObject(const char* name) const;
  float FitToSquare(int* offsetX, int* offsetY, int width, int height, float size) const;
  void LoadSettings();

  /* 0000 */ std::vector<NavigationObject*> objects;
  /* 000C */ unsigned char unknown_000C[0x10F0];
  /* 10FC */ NavigationTarget currentTarget;
  /* 1124 */ NavigationTarget savedTarget;
  /* 114C */ unsigned char unknown_114C[0x1F8];
  /* 1344 */ int viewIcon;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(NavigationObject, name) == 0x8);
static_assert(offsetof(NavigationManager, savedTarget) == 0x1124);
static_assert(offsetof(NavigationManager, viewIcon) == 0x1344);
#endif

#endif
