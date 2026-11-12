#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H

#include "Contents/Actors/GameActor.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

class C3dGrannyModelRes;
class CActRes;
class CRes;
class CSprRes;
class CTexture;
struct ActionFrame;
struct RenderCommand;
struct RenderVertex;
struct granny_control;
struct granny_model_instance;
struct CRagEffect;
class CMsgEffect;
class UIWindow;

int GetSlotLayer(int slot);
bool IsReadyOrAttackAction(int action);

class CPc : public CGameActor {
public:
  CPc();
  ~CPc() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  bool ProcessState() override;
  void SetState(int state, int resetPosition) override;
  void SetSprRobe(int first, int second) override;
  void SetSprActFull(
    int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh
  ) override;
  void SetSprAct2(
    int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh
  ) override;
  void SetSprJob(int job) override;
  void SetSprHead(int value) override;
  void SetSprWeapon(int first, int second) override;
  void SetSprWeapon2(int first, int second) override;
  void SetSprAccessory(int value) override;
  void SetSprAccessory2(int value) override;
  void SetSprAccessory3(int value) override;
  void SetSprShield(int value) override;
  void SetSprShoe(int value) override;
  void SetImfFileName() override;
  void SetHeadPaletteName() override;
  void SetBodyPaletteName() override;
  void SetBodyPalette(int value) override;
  void SetHeadPalette(int value) override;
  std::int64_t GetWeapon() override;
  void SetModifyFactorOfmotionSpeed(int value) override;
  void SetHonor(int value) override;
  void SetPropensityInfo(int first, int second) override;
  int GetPkState() override;
  void SetBodyCostume(int first, int second) override;
  int GetBodyCostume() override;
  void SetRiding(unsigned char value) override;
  int GetMonsterTransformJob() override;
  int GetMadogearType() override;
  void SetMadogearType(int value) override;
  bool IsArrowEffectSuppressed() override;
  void SetArrowEffectSuppressed(bool value) override;
  void RefreshAccessory3() override;
  void SetSex(int value) override;

  void AddMsgEffect(CGameObject* effect);
  void RenderBodyOutline(int layer, const float* position, float x, float y, float distanceScale, float scale, float depth, std::uint32_t color);
  bool IsDrawOnTop();
  const ActionFrame& SelectStanceFrame(int layer, CActRes* actRes, int* animation, int* frame, bool* replaced);
  void ApplyLayerEffects(
    SpriteQuad* quad, const SpriteFrameCell* cell, int layer, const int* box, float shiftX, float distance, float bottomRhw,
    float topRhw, float screenX, float screenY, const std::uint32_t* palette, const SpriteRleImage* image, int* blend, int offsetX,
    int offsetY
  );
  void LoadEquipAction(int slot, const char* name);
  void LoadEquipSprite(int slot, const char* name);
  void LoadEquipActions(const std::vector<std::string>& names);
  void LoadEquipSprites(const std::vector<std::string>& names);
  int MapMotion(int motion);
  std::vector<std::string> GetEquipActionNames() const;
  std::vector<std::string> GetEquipSpriteNames() const;
  CRes* GetEquipAction(unsigned slot) const;
  CRes* GetEquipSprite(int slot) const;
  int JobActionBase(int state);
  bool IsCostumeOutfit() const;
  int GetDualWieldWeaponType(int first, int second);
  bool HasAppearanceOverride();
  void DrawBody(int argument, int layer, const float* position, float x, float y, float distanceScale, float scale, float depth);
  void RenderOverlaySprite(const float* matrix, int);
  void UpdateGaugeWindow();
  void DrawLayers(const float* matrix, int padding_0030, char blendVariant);

  float* GetExtent(float* extent) const override;

  /* 0410 */ unsigned char padding_0410[4];
  /* 0414 */ CMsgEffect* msgEffect;
  /* 0418 */ std::string imfName;
  /* 0430 */ int honor;
  /* 0434 */ int virtue;
  /* 0438 */ int appearanceFlags;
  /* 043C */ int headDir;
  /* 0440 */ int head;
  /* 0444 */ int headPalette;
  /* 0448 */ int weapon;
  /* 044C */ int secondaryWeapon;
  /* 0450 */ int accessory;
  /* 0454 */ int accessory2;
  /* 0458 */ int accessory3;
  /* 045C */ int shield;
  /* 0460 */ int shoe[1];
  /* 0464 */ int footprintCounter;
  /* 0468 */ vector3d footprintPosition;
  /* 0474 */ int renderWithoutLayer;
  /* 0478 */ std::string headPaletteName;
  /* 0490 */ UIWindow* gaugeWindow;
  /* 0494 */ int pkRank;
  /* 0498 */ int pkTotal;
  /* 049C */ int robe;
  /* 04A0 */ int blend;
  /* 04A4 */ int layerCellX;
  /* 04A8 */ int layerCellY;
  /* 04AC */ std::map<int, std::pair<int, int>> padding_04AC;
  /* 04B4 */ std::vector<CSprRes*> equipSprites;
  /* 04C0 */ std::vector<CActRes*> equipActions;
  /* 04CC */ int pkState;
  /* 04D0 */ int bodyStyle;
  /* 04D4 */ int appearanceJob;
  /* 04D8 */ unsigned char alternateDoramAppearance[1];
  /* 04D9 */ bool isHelmRobe;
  /* 04DA */ unsigned char padding_04DA[2];
  /* 04DC */ int madogearType;
  /* 04E0 */ bool arrowEffectSuppressed;
  /* 04E1 */ unsigned char padding_04E1[0x03];
  /* 04E4 */ float extentLeft;
  /* 04E8 */ float extentTop;
  /* 04EC */ float extentRight;
  /* 04F0 */ float extentBottom;
  /* 04F4 */ int outlineColor;
  /* 04F8 */ float destCellX;
  /* 04FC */ int destCellZ;
  /* 0500 */ int attackReqTime;
  /* 0504 */ std::uint32_t preMoveStartTick;
  /* 0508 */ int proceedType;
  union { /* 050C */ int preMoveOn; CPc* blurSource; };
};

class CBlurPC : public CPc {
public:
  CBlurPC(CPc* source);
  ~CBlurPC() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;
};

class CHalluPC : public CPc {
public:
  CHalluPC(CPc* source);
  ~CHalluPC() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0510 */ CPc* sourcePc;
  /* 0514 */ int padding_0514;
};

class CHalluPC2 : public CPc {
public:
  CHalluPC2(CPc* source);
  ~CHalluPC2() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0510 */ CPc* sourcePc;
  /* 0514 */ int padding_0514;
};

class CPlayer : public CPc {
public:
  CPlayer();
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  static int AngleToDirection(float angle);

  bool IsSpecialState(int state) const;
  void ReapplySkillEffects();
  void RegisterPos() override;
  void UnRegisterPos() override;
  void RefreshEffect164();
  void UpdateFootprints(int kind);
  void UpdateMsgEffect(bool show);
  void ApplyFollowOffset(CGameActor* target, float x, float z, int* row, int* column);
  float AdjustFollowDistance(CGameActor* target, float distance);
  void UpdatePendingAction();
  void Initialize();
  void SetState(int state, int resetPosition) override;

  /* 0510 */ int attackMode;
  /* 0514 */ int isAttackRequest;
  /* 0518 */ int isPreengageStateOfMove;
  /* 051C */ int proceedTargetGid;
  /* 0520 */ int totalAttackReqCnt;
  /* 0524 */ std::uint32_t tickOfMoveForAttack;
  /* 0528 */ std::uint32_t standTick;
  /* 052C */ int skillId;
  /* 0530 */ int skillUseLevel;
  /* 0534 */ int gSkillDx;
  /* 0538 */ int gSkillDy;
  /* 053C */ int preengageXOfMove;
  /* 0540 */ int preengageYOfMove;
  /* 0544 */ CRagEffect* skillEffect;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPlayer) == 0x548);
static_assert(offsetof(CPlayer, proceedTargetGid) == 0x51C);
static_assert(offsetof(CPlayer, skillEffect) == 0x544);
#endif

extern bool g_refreshWindow289;

struct GrannyPcLight {
  /* 0000 */ vector3d direction;
  /* 000C */ int intensity;
  /* 0010 */ int red;
  /* 0014 */ int green;
  /* 0018 */ int blue;
};

class CGrannyPc : public CGameActor {
public:
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;
  void SetAction(unsigned animation, int motion, int repeat) override;
  void ProcessMotion() override;
  bool ProcessState() override;
  void SetState(int first, int second) override;
  void SetSprActFull(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) override;
  void SetSprAct2(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) override;
  void SetSprJob(int value) override;
  void SetSprHead(int value) override;
  void SetSprWeapon(int first, int second) override;
  void SetSprWeapon2(int first, int second) override;
  void SetSprAccessory(int value) override;
  void SetSprAccessory2(int value) override;
  void SetSprAccessory3(int value) override;
  void SetSprShield(int value) override;
  void SetImfFileName() override;
  void SetHeadPaletteName() override;
  std::int64_t GetWeapon() override;
  void SetModifyFactorOfmotionSpeed(int value) override;
  void SetHonor(int value) override;
  void SetPropensityInfo(int first, int second) override;

  void PlayMotion(int repeat);
  void ReleaseMeshes();
  void UpdateGaugeWindow();
  granny_world_pose* UpdateAnimation(C3dGrannyModelRes* model);
  void RenderModel(const matrix& view, const GrannyPcLight& light, C3dGrannyModelRes* model, int layer, C3dGrannyModelRes* animation);
  void RenderCached(const matrix& view, int layer, C3dGrannyModelRes* model);
  void RenderShadow(int argument, vector3d lightDirection, bool cached);

  /* 0410 */ std::string imfName;
  /* 0428 */ int honor;
  /* 042C */ int virtue;
  /* 0430 */ int headDir;
  /* 0434 */ int head;
  /* 0438 */ int headPalette;
  /* 043C */ unsigned char padding_043C[4];
  /* 0440 */ int weapon;
  /* 0444 */ int secondaryWeapon;
  /* 0448 */ int accessory;
  /* 044C */ int accessory2;
  /* 0450 */ int accessory3;
  /* 0454 */ int shield;
  /* 0458 */ unsigned char padding_0458[8];
  /* 0460 */ std::string headPaletteName;
  /* 0478 */ UIWindow* gaugeWindow;
  /* 047C */ int pkRank;
  /* 0480 */ int pkTotal;
  /* 0484 */ C3dGrannyModelRes* model;
  /* 0488 */ C3dGrannyModelRes* layerModels[10];
  /* 04B0 */ granny_model_instance* instance;
  /* 04B4 */ RenderCommand* commands[30];
  /* 052C */ RenderCommand* layerCommands[10][30];
  /* 09DC */ RenderVertex* vertices[30];
  /* 0A54 */ RenderVertex* layerVertices[10][30];
  /* 0F04 */ granny_system_clock clock;
  /* 0F14 */ granny_system_clock lastClock;
  /* 0F24 */ float animationTime;
  /* 0F28 */ int directionalAnimation;
  /* 0F2C */ int baseAnimation;
  /* 0F30 */ float angle;
  /* 0F34 */ unsigned char alpha;
  /* 0F35 */ unsigned char padding_0F35[0x03];
  /* 0F38 */ unsigned brightness;
  /* 0F3C */ int motion;
  /* 0F40 */ unsigned char currentFrame[4];
  /* 0F44 */ char modelName[0x104];
  /* 1048 */ unsigned char padding_1048[8];
  /* 1050 */ granny_control* controls[20];
  /* 10A0 */ unsigned char snapFacing;
  /* 10A1 */ unsigned char padding_10A1[3];
  /* 10A4 */ int renderFrame;
  /* 10A8 */ CTexture* meshTextures[10][30];
  /* 1558 */ std::uint16_t* meshIndices[10][30];
  /* 1A08 */ int meshTriangleCounts[10][30];
  /* 1EB8 */ matrix meshTransforms[30];
  /* 2458 */ unsigned char padding_2458[0xF8];
  /* 2550 */ granny_world_pose* worldPose;
  /* 2554 */ CTexture* shadowTexture;
  /* 2558 */ std::vector<vector3d> attachmentPositions;
  /* 2564 */ int attachLayer;
  /* 2568 */ int padding_2568;
  /* 256C */ matrix attachTransform;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPc, honor) == 0x430);
static_assert(offsetof(CPc, headPalette) == 0x444);
static_assert(offsetof(CPc, accessory3) == 0x458);
static_assert(offsetof(CGrannyPc, honor) == 0x428);
static_assert(offsetof(CGrannyPc, weapon) == 0x440);
static_assert(offsetof(CGrannyPc, headPaletteName) == 0x460);
static_assert(offsetof(CGrannyPc, model) == 0x484);
static_assert(offsetof(CGrannyPc, instance) == 0x4B0);
static_assert(offsetof(CGrannyPc, clock) == 0xF04);
static_assert(offsetof(CGrannyPc, lastClock) == 0xF14);
static_assert(offsetof(CGrannyPc, animationTime) == 0xF24);
static_assert(offsetof(CGrannyPc, motion) == 0xF3C);
static_assert(offsetof(CGrannyPc, controls) == 0x1050);
static_assert(offsetof(CGrannyPc, modelName) == 0xF44);
static_assert(offsetof(CGrannyPc, layerModels) == 0x488);
static_assert(offsetof(CGrannyPc, commands) == 0x4B4);
static_assert(offsetof(CGrannyPc, layerCommands) == 0x52C);
static_assert(offsetof(CGrannyPc, vertices) == 0x9DC);
static_assert(offsetof(CGrannyPc, layerVertices) == 0xA54);
static_assert(offsetof(CGrannyPc, worldPose) == 0x2550);
static_assert(offsetof(CGrannyPc, angle) == 0xF30);
static_assert(offsetof(CGrannyPc, brightness) == 0xF38);
static_assert(offsetof(CGrannyPc, meshTextures) == 0x10A8);
static_assert(offsetof(CGrannyPc, meshIndices) == 0x1558);
static_assert(offsetof(CGrannyPc, meshTriangleCounts) == 0x1A08);
static_assert(offsetof(CGrannyPc, meshTransforms) == 0x1EB8);
static_assert(offsetof(CGrannyPc, attachLayer) == 0x2564);
static_assert(offsetof(CGrannyPc, attachTransform) == 0x256C);
static_assert(sizeof(CBlurPC) == 0x510);
static_assert(sizeof(CHalluPC) == 0x518);
static_assert(sizeof(CHalluPC2) == 0x518);
static_assert(offsetof(CPc, msgEffect) == 0x414);
static_assert(offsetof(CPc, imfName) == 0x418);
static_assert(offsetof(CPc, extentLeft) == 0x4E4);
static_assert(offsetof(CPc, extentBottom) == 0x4F0);
static_assert(offsetof(CPc, destCellX) == 0x4F8);
static_assert(offsetof(CPc, destCellZ) == 0x4FC);
static_assert(offsetof(CPc, preMoveOn) == 0x50C);
static_assert(offsetof(CHalluPC, sourcePc) == 0x510);
static_assert(offsetof(CHalluPC2, sourcePc) == 0x510);
static_assert(offsetof(CPc, weapon) == 0x448);
static_assert(offsetof(CPc, secondaryWeapon) == 0x44C);
static_assert(offsetof(CPc, pkState) == 0x4CC);
static_assert(offsetof(CPc, madogearType) == 0x4DC);
static_assert(offsetof(CPc, arrowEffectSuppressed) == 0x4E0);
#endif

extern unsigned char g_statusRefreshSuppressed;
extern bool g_moveRequestPending;

#endif
