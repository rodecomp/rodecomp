#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPOINT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPOINT_H

struct EffectPoint {
  EffectPoint() : x(0), y(0), z(0) {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

static_assert(sizeof(EffectPoint) == 12);

#endif
