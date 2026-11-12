#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_GRAFFISKILL_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_GRAFFISKILL_H

#include "Contents/Actors/Skill.h"

#include <cstddef>

class CTexture;

class CGraffiSkill : public CSkill {
public:
  CGraffiSkill();
  ~CGraffiSkill() override;
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;
  void Render(int argument) override;

  virtual void BuildTexture();
  virtual void ReleaseTexture();

  /* 0440 */ unsigned char m_masterGid[0x04];
  /* 0444 */ int m_cellX;
  /* 0448 */ int m_cellY;
  /* 044C */ CTexture* m_canvas;
  /* 0450 */ CTexture* m_font;
  /* 0454 */ char m_text[0x100];
  /* 0554 */ int m_penX;
  /* 0558 */ int m_penY;
  /* 055C */ float m_angle;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGraffiSkill) == 0x560);
static_assert(offsetof(CGraffiSkill, m_cellX) == 0x444);
static_assert(offsetof(CGraffiSkill, m_canvas) == 0x44C);
static_assert(offsetof(CGraffiSkill, m_text) == 0x454);
static_assert(offsetof(CGraffiSkill, m_angle) == 0x55C);
#endif

#endif
