#ifndef RAGNAROK_SOURCE_CLIENT_CLIENTOPTIONS_H
#define RAGNAROK_SOURCE_CLIENT_CLIENTOPTIONS_H

#include "Contents/InstantVar.h"

struct ClientOptions {
  CInstantVar& values;
  int Get(int id) const;
  void Set(int id, int value, int extra);
};

#endif
