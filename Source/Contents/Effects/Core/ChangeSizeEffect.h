#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_CHANGESIZEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_CHANGESIZEEFFECT_H

#include "Contents/Effects/Core/Effect.h"

class CChangeSizeEffect : public CEffect {
public:
  CChangeSizeEffect();

  void OnProcess() override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CChangeSizeEffect) == 0xCC);
#endif

#endif
