#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHANGESIZEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHANGESIZEEFFECT_H

#include "Contents/Effects/Effect.h"

class CChangeSizeEffect : public CEffect {
public:
  CChangeSizeEffect();

  void Slot03() override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CChangeSizeEffect) == 0xCC);
#endif

#endif
