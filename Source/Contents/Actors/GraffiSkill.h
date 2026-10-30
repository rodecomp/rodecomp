#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GRAFFISKILL_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GRAFFISKILL_H

#include "Contents/Actors/Skill.h"

#include <cstddef>

class CTexture;

class CGraffiSkill : public CSkill {
public:
  ~CGraffiSkill() override;
  bool OnProcess() override;

  virtual void BuildTexture();
  virtual void ReleaseTexture();

  /* 0440 */ unsigned char unknown_0440[0x0C];
  /* 044C */ CTexture* m_canvas;
  /* 0450 */ CTexture* m_font;
  /* 0454 */ char m_text[0x100];
  /* 0554 */ int m_penX;
  /* 0558 */ int m_penY;
  /* 055C */ float m_angle;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGraffiSkill) == 0x560);
static_assert(offsetof(CGraffiSkill, m_canvas) == 0x44C);
static_assert(offsetof(CGraffiSkill, m_text) == 0x454);
static_assert(offsetof(CGraffiSkill, m_angle) == 0x55C);
#endif

#endif
