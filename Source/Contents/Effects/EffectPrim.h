#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIM_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIM_H

#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/AffineMatrix.h"

#include <list>

class ResourceMgr;
struct CRagEffect;
struct EffectVisibilityContext;

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
  bool PrimTextureSpin();
  void PrimScrollTexture();
  void PrimFrameFade();
  void PrimRiseFollow();
  void PrimFadeInFollow();
  void PrimAnimatedFlash();
  void PrimRandomSpin();
  void PrimHeightRamp();
  void PrimBobbing();
  bool PrimTwinSpin();
  void PrimFlashFade();
  bool PrimFadeAlpha();
  void PrimFrameScroll();
  void PrimFramePhase();
  void PrimTrailShift();
  void PrimGrowFollow();
  void PrimSpinFade();
  void PrimExpandPulse();
  void ScaleByMaster(float& scale) const;
  void PrimPulseRing();
  void PrimHoverFollow();
  void PrimGlowFollow();
  bool PrimOrbitFade();
  void PrimWaveHeights();
  void PrimWaveHeightsSlow();
  void PrimTextureFrames();
  void PrimWaveSpin();
  void PrimBreathingWave();
  bool PrimDriftingRings();
  bool PrimSpiralFade();
  bool Prim3DSlotParticle();
  bool PrimFieldFireflies();
  void Render2DCircle();
  void RenderGroundFog(matrix& wtm);
  void RenderCrossPlanes(matrix& wtm);
  void RenderPyramid(matrix& wtm);
  void RenderArcBand(matrix& wtm);
  void RenderSizedSquare(matrix& wtm);
  void RenderRecordRings(matrix& wtm);
  void Render2DDiamond();
  void Render3DCasting2(matrix& wtm);
  void Render3DCasting(matrix& wtm);
  void Render3DCasting_LowPolygon(matrix& wtm);
  void RenderAttackEnergy2(matrix& wtm);
  void RenderBlind(matrix& wtm);
  void RenderChemical(matrix& wtm);
  void RenderColorPaper(matrix& wtm);
  void RenderDoubleGumGang(matrix& wtm);
  void RenderGrandCross2(matrix& wtm);
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
  bool IsPartyVisible(const EffectVisibilityContext& context);
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
  void RenderOrbitIcons(matrix& wtm, float scale, unsigned char red, unsigned char green, unsigned char blue);
  void RenderLayeredGlow(matrix& wtm);
  void RenderSphereCage(matrix& wtm);
  void RenderGroundBubbles(matrix& wtm);
  void RenderHaloLayers(matrix& wtm);
  void RenderLightningArc(matrix& wtm);
  void RenderGroundBar(matrix& wtm);
  void RenderOrbitSquare(matrix& wtm);
  void RenderLightningTube(matrix& wtm);
  void RenderStackedRings(matrix& wtm);
  void RenderRiseArc(matrix& wtm);
  void RenderFacingQuads(matrix& wtm);
  void RenderPentaRing(matrix& wtm);
  void RenderScrollColumn(matrix& wtm);
  void RenderFanBeam(matrix& wtm);
  void RenderBladeTrail(matrix& wtm);
  void RenderFieldPanel(matrix& wtm);
  void RenderTube(matrix& wtm);
  void RenderSpinSquares(matrix& wtm);
  void RenderSpinSquare(matrix& wtm, int* rpCnt, float scale, short slotIndex, char pw, unsigned char red, unsigned char green, unsigned char blue);
  void RenderTrailingArc(matrix& wtm);
  void RenderRecordQuads(matrix& wtm);
  void RenderSpiralBlades(matrix& wtm);
  void RenderGlowDisc(matrix& wtm);
  void RenderColorPanel(matrix& wtm);
  void RenderTrailSquares(matrix& wtm);
  void RenderRotatedPlate(matrix& wtm);
  void RenderColorPlane(matrix& wtm);
  void RenderSMA3(matrix& wtm);
  void RenderBeam(matrix& wtm);
  void RenderCone(matrix& wtm);
  void RenderPeong(matrix& wtm);
  void RenderFloorRect(matrix& wtm);
  void RenderRotatedRing(matrix& wtm);
  void RenderFloorSquare(matrix& wtm);
  void RenderFloorSquare2(matrix& wtm);
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
  void RenderMagicTarget(matrix& wtm);
  void RenderGroundArcGlow(matrix& wtm);
  void RenderGroundArc(matrix& wtm);
  void RenderGroundArc2(matrix& wtm);
  void RenderGroundArcStep(matrix& wtm);
  void RenderSegmentSprites(matrix& wtm);
  void RenderGroundSquare(matrix& wtm);
  void RenderGroundSquareBlend(matrix& wtm);
  void RenderJitterQuads(matrix& wtm);
  void RenderFanWedge(matrix& wtm);
  void RenderFanSlice(matrix& wtm);
  void RenderFanTrail(matrix& wtm);
  void RenderCurvedFanTrail(matrix& wtm);
  void RenderTwinFacingSquares(matrix& wtm);
  void RenderPillarSquares(matrix& wtm);
  void RenderBladeFins(matrix& wtm);
  void RenderPolygonFrames(matrix& wtm);

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
