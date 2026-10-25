#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEOBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEOBJECT_H

#include <cstdint>

struct CameraDirectionContext {
  const int& modeType;
  const float* const& cameraAngle;
};

class CGameObject {
public:
  CGameObject();
  virtual ~CGameObject();
  virtual bool Update();
  virtual void SendObjectMessage(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m);
  virtual void Draw(int argument);
  virtual int Direction(float angle, const CameraDirectionContext& camera);

  /* 0008 */ std::uint64_t identity;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGameObject) == 16);
#endif

#endif
