#ifndef RAGNAROK_SOURCE_CLIENT_CLIENTOPTIONS_H
#define RAGNAROK_SOURCE_CLIENT_CLIENTOPTIONS_H

#include "Contents/InstantVar.h"

// Low/high-word client operations on its CInstantVar at +0x7FA8.
struct ClientOptions {
  CInstantVar& values;
  int Get(int id) const;
  void Set(int id, int value, int extra);
};

#endif
