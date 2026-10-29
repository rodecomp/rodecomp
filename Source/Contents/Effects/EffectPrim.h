#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIM_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIM_H

#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/AffineMatrix.h"

#include <list>

class ResourceMgr;
struct CRagEffect;

struct CEffectPrim : CRenderObject {
  CEffectPrim();

  bool UpdateFireWall();
  bool UpdateStoneCurse();
  void Prim3DCircle();
  bool Prim3DParticle();
  bool Prim3DParticleGravity();
  bool Prim3DParticleNoMaster();
  bool Prim3DParticleOrbit();
  bool Prim3DParticleSpline();
  bool Prim3DTextureNoMaster();
  void PrimBottom2();
  void PrimBottom();
  void PrimBottomOut();
  void PrimBubbleDrop();
  void PrimChemical2();
  void PrimChemical3();
  void PrimChemical();
  void PrimEffectTexture2();
  void PrimEffectTexture_Ani();
  void PrimFirstAid();
  void PrimIntimidate();
  void PrimPoison();
  void PrimGuard();
  bool PrimLineLink();
  void PrimSMA();
  void PrimShieldBoomerang3();
  bool PrimSuperAngel();
  void PrimTanji2();
  void PrimTeiHit2();
  bool PrimSakura();
  bool PrimKouenka();
  void PrimShieldBoomerang();
  bool PrimGhost();
  void PrimThrowItem();
  bool PrimPeong();
  void PrimGI3();
  void PrimWaterFall();
  bool PrimAcidDemon();
  bool PrimAsuraCasting();
  void PrimElectric2();
  bool PrimFlowerCast();
  void PrimForest();
  bool PrimMapParticle();
  void PrimParticleSpread();
  void PrimParticleUp();
  bool PrimPeongUp();
  void PrimPokJuk();
  void PrimSTIN4();
  void PrimSaintWing();
  bool PrimSoulParticle();
  void PrimSphereWind2();
  void PrimGI4();
  void PrimGI1();
  bool PrimCHOOKGI();
  bool PrimTwilight();
  bool PrimWind();
  void Tei_TowardScreen(vector3d& vec1, vector3d& vec2, vector3d& vec3, vector3d& vec4, short rot0, short rot1);

  bool AnimMotion();
  bool FollowMasterAndAdvance();
  void FollowMasterAndTickSlots();
  void FollowMasterAndTickSlots2();
  void ShiftSegment();
  void InitSegment();
  void RenderTeiRect(int* rpCnt, short alpha, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, short pw, char tn, char f1, unsigned char r, unsigned char g, unsigned char b, float ty1, float ty2, float tx, float tx2, char extra);
  void RenderTeiRect(int* rpCnt, int texPart, short alphaB, short alphaT, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, char pw);
  void RenderTeiRect(int* rpCnt, int texPart, short alphaB, short alphaT, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, char pw, unsigned char r, unsigned char g, unsigned char b);
  void RenderHalfSphere(matrix& wtm);
  void RenderHalfSphere2(matrix& wtm);
  void RenderHermode(matrix& wtm);
  void RenderHitLine3(matrix& wtm);
  void RenderHitLine(matrix& wtm);
  void RenderMagnum2(matrix& wtm);
  void RenderPeongUp(matrix& wtm);
  void RenderRipple(matrix& wtm);
  void RenderRainbow(matrix& wtm);
  void RenderRing(matrix& wtm);
  void RenderPentagon(matrix& wtm);
  void RenderGrid(matrix& wtm);
  void RenderRectUp2(matrix& wtm);
  void RenderRectUp(matrix& wtm);
  void RenderSMA2(matrix& wtm);
  void RenderSaintWing(matrix& wtm);
  void RenderSlash1(matrix& wtm);
  void RenderSoulLink(matrix& wtm);
  void RenderSoulParticle(matrix& wtm);
  void RenderSphereWind(matrix& wtm);

  /* 0138 */ EffectPrimitiveState state;
};

CEffectPrim* CreateEffectParticle(
  CRagEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z,
  std::list<CRenderObject*>& worldObjects
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEffectPrim) == 0xFA18);
static_assert(offsetof(CEffectPrim, state) == 0x138);
#endif

#endif
