#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEOBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEOBJECT_H

#include <cstdint>

class CGameObject {
  static std::uint64_t s_nextIdentity;

public:
  CGameObject();
  virtual ~CGameObject();
  virtual bool OnProcess();
  virtual void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g);
  virtual void Render(int argument);
  virtual int Get8Dir(float angle);
  int Get8Dir2(float rot, int flag);
  int GetDir(float rot);

  /* 0008 */ std::uint64_t identity;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGameObject) == 16);
#endif

#endif
