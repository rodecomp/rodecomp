#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GRANNYGAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GRANNYGAMEACTOR_H

#include "Contents/Actors/GameActor.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <cstdint>
#include <vector>

class CTexture;
struct RenderCommand;
struct RenderVertex;

struct GrannyActorLight {
  /* 0000 */ vector3d direction;
  /* 000C */ int intensity;
  /* 0010 */ int red;
  /* 0014 */ int green;
  /* 0018 */ int blue;
};

class C3dGrannyGameActor : public CGameActor {
public:
  explicit C3dGrannyGameActor(const char* name);
  void SetAction(unsigned animation, int motion, int repeat) override;
  void ProcessMotion() override;
  void ProcessMotionWithDist() override;
  int GetAttackMotion() override;
  void Unused6860(int value);
  void Unused7950(int value);
  void SetSprAct(int job, int value, int unused) override;
  void Render(int argument) override;
  void RenderLit(int argument, const GrannyActorLight& light);
  void RenderShadowed(int argument);
  void MakeCorpse() override;
  bool OnProcess() override;

  /* 0410 */ CTexture* emblemTexture;
  /* 0414 */ C3dGrannyModelRes* model;
  /* 0418 */ granny_model_instance* instance;
  /* 041C */ RenderCommand* commands[30];
  /* 0494 */ RenderVertex* vertices[30];
  /* 050C */ granny_system_clock clock;
  /* 051C */ granny_system_clock lastClock;
  /* 052C */ float motionProgress;
  /* 0530 */ unsigned char padding_0530[0x08];
  /* 0538 */ float angle;
  /* 053C */ unsigned char alpha;
  /* 053D */ unsigned char padding_053D[0x03];
  /* 0540 */ unsigned brightness;
  /* 0544 */ int motion;
  /* 0548 */ unsigned char currentFrame[0x04];
  /* 054C */ char name[0x104];
  /* 0650 */ int renderState;
  /* 0654 */ int cellTexture;
  /* 0658 */ granny_control* controls[20];
  /* 06A8 */ unsigned char initializeFacing;
  /* 06A9 */ unsigned char padding_06A9[0x03];
  /* 06AC */ CTexture* meshTextures[30];
  /* 0724 */ std::uint16_t* meshIndices[30];
  /* 079C */ int meshTriangleCounts[30];
  /* 0814 */ matrix meshTransforms[30];
  /* 0DB4 */ unsigned char worldPose[0x04];
  /* 0DB8 */ CTexture* texture;
  /* 0DBC */ std::vector<vector3d> litPositions;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(C3dGrannyGameActor) == 0xDC8);
#endif

#endif
