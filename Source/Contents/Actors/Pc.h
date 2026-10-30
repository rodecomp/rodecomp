#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H

#include "Contents/Actors/GameActor.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class C3dGrannyModelRes;
class CActRes;
class CRes;
class CTexture;
struct ActionFrame;
struct RenderCommand;
struct RenderVertex;
struct granny_control;
struct granny_model_instance;

int GetSlotLayer(int slot);
bool IsReadyOrAttackAction(int action);

class CPc : public CGameActor {
public:
  ~CPc() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;

  void Slot15(int first, int second) override;
  void Slot19(
    int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh
  ) override;
  void Slot20(
    int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh
  ) override;
  void Slot22(int value) override;
  void Slot23(int first, int second) override;
  void Slot24(int first, int second) override;
  void Slot25(int value) override;
  void Slot26(int value) override;
  void Slot27(int value) override;
  void Slot30() override;
  void Slot31() override;
  void Slot33(int value) override;
  void Slot34(int value) override;
  std::int64_t Slot35() override;
  void Slot39(int value) override;
  void Slot40(int value) override;
  void Slot41(int first, int second) override;
  int Slot42() override;
  void Slot44(int first, int second) override;
  int Slot46() override;
  void Slot47(unsigned char value) override;
  int Slot55() override;
  int Slot56() override;
  void Slot57(int value) override;
  bool Slot58() override;
  void Slot59(bool value) override;
  void Slot60() override;
  void Slot62(int value) override;

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

  float* GetExtent(float* extent) const override;

  /* 0410 */ unsigned char unknown_0410[8];
  /* 0418 */ std::string unknown_0418;
  /* 0430 */ int unknown_0430;
  /* 0434 */ int unknown_0434;
  /* 0438 */ int unknown_0438;
  /* 043C */ int unknown_043C;
  /* 0440 */ int unknown_0440;
  /* 0444 */ int unknown_0444;
  /* 0448 */ int weapon;
  /* 044C */ int unknown_044C;
  /* 0450 */ int unknown_0450;
  /* 0454 */ int unknown_0454;
  /* 0458 */ int unknown_0458;
  /* 045C */ int unknown_045C;
  /* 0460 */ unsigned char unknown_0460[0x18];
  /* 0478 */ std::string unknown_0478;
  /* 0490 */ unsigned char unknown_0490[0x0C];
  /* 049C */ int unknown_049C;
  /* 04A0 */ int blend;
  /* 04A4 */ unsigned char unknown_04A4[0x10];
  /* 04B4 */ std::vector<CRes*> equipSprites;
  /* 04C0 */ std::vector<CRes*> equipActions;
  /* 04CC */ int unknown_04CC;
  /* 04D0 */ int unknown_04D0;
  /* 04D4 */ int unknown_04D4;
  /* 04D8 */ unsigned char unknown_04D8[1];
  /* 04D9 */ bool unknown_04D9;
  /* 04DA */ unsigned char unknown_04DA[2];
  /* 04DC */ int unknown_04DC;
  /* 04E0 */ bool unknown_04E0;
  /* 04E1 */ unsigned char unknown_04E1[0x03];
  /* 04E4 */ float unknown_04E4;
  /* 04E8 */ float unknown_04E8;
  /* 04EC */ float unknown_04EC;
  /* 04F0 */ float unknown_04F0;
  /* 04F4 */ int unknown_04F4;
  /* 04F8 */ float unknown_04F8;
  /* 04FC */ int unknown_04FC;
  /* 0500 */ unsigned char unknown_0500[0x0C];
  /* 050C */ int unknown_050C;
};

class CBlurPC : public CPc {
public:
  ~CBlurPC() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;
};

class CHalluPC : public CPc {
public:
  ~CHalluPC() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0510 */ int unknown_0510;
  /* 0514 */ int unknown_0514;
};

class CHalluPC2 : public CPc {
public:
  ~CHalluPC2() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0510 */ int unknown_0510;
  /* 0514 */ int unknown_0514;
};

class CPlayer : public CPc {
public:
  static int AngleToDirection(float angle);

  bool IsSpecialState(int state) const;
  void Slot16() override;
  void Slot17() override;
};

class CGrannyPc : public CGameActor {
public:
  void SetAction(unsigned animation, int motion, int repeat) override;
  void Slot08() override;
  void Slot20(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) override;
  void Slot21(int value) override;
  void Slot22(int value) override;
  void Slot23(int first, int second) override;
  void Slot24(int first, int second) override;
  void Slot25(int value) override;
  void Slot26(int value) override;
  void Slot27(int value) override;
  void Slot28(int value) override;
  void Slot30() override;
  void Slot31() override;
  std::int64_t Slot35() override;
  void Slot39(int value) override;
  void Slot40(int value) override;
  void Slot41(int first, int second) override;

  void PlayMotion(int repeat);
  void ReleaseMeshes();
  void UpdateGaugeWindow();
  void UpdateAnimation(C3dGrannyModelRes* model);
  void RenderCached(const matrix& view, int layer, C3dGrannyModelRes* model);

  /* 0410 */ std::string unknown_0410;
  /* 0428 */ int unknown_0428;
  /* 042C */ int unknown_042C;
  /* 0430 */ unsigned char unknown_0430[4];
  /* 0434 */ int unknown_0434;
  /* 0438 */ int unknown_0438;
  /* 043C */ unsigned char unknown_043C[4];
  /* 0440 */ int unknown_0440;
  /* 0444 */ int unknown_0444;
  /* 0448 */ int unknown_0448;
  /* 044C */ int unknown_044C;
  /* 0450 */ int unknown_0450;
  /* 0454 */ int unknown_0454;
  /* 0458 */ unsigned char unknown_0458[8];
  /* 0460 */ std::string unknown_0460;
  /* 0478 */ unsigned char unknown_0478[0x0C];
  /* 0484 */ C3dGrannyModelRes* model;
  /* 0488 */ unsigned char unknown_0488[0x28];
  /* 04B0 */ granny_model_instance* instance;
  /* 04B4 */ RenderCommand* commands[30];
  /* 052C */ RenderCommand* layerCommands[10][30];
  /* 09DC */ RenderVertex* vertices[30];
  /* 0A54 */ RenderVertex* layerVertices[10][30];
  /* 0F04 */ granny_system_clock clock;
  /* 0F14 */ granny_system_clock lastClock;
  /* 0F24 */ float unknown_0F24;
  /* 0F28 */ unsigned char unknown_0F28[0x08];
  /* 0F30 */ float angle;
  /* 0F34 */ unsigned char alpha;
  /* 0F35 */ unsigned char unknown_0F35[0x03];
  /* 0F38 */ unsigned brightness;
  /* 0F3C */ int motion;
  /* 0F40 */ unsigned char unknown_0F40[0x110];
  /* 1050 */ granny_control* controls[20];
  /* 10A0 */ int unknown_10A0;
  /* 10A4 */ int renderFrame;
  /* 10A8 */ CTexture* meshTextures[10][30];
  /* 1558 */ std::uint16_t* meshIndices[10][30];
  /* 1A08 */ int meshTriangleCounts[10][30];
  /* 1EB8 */ matrix meshTransforms[30];
  /* 2458 */ unsigned char unknown_2458[0xF8];
  /* 2550 */ int unknown_2550;
  /* 2554 */ unsigned char unknown_2554[0x10];
  /* 2564 */ int attachLayer;
  /* 2568 */ int unknown_2568;
  /* 256C */ matrix attachTransform;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPc, unknown_0430) == 0x430);
static_assert(offsetof(CPc, unknown_0444) == 0x444);
static_assert(offsetof(CPc, unknown_0458) == 0x458);
static_assert(offsetof(CGrannyPc, unknown_0428) == 0x428);
static_assert(offsetof(CGrannyPc, unknown_0440) == 0x440);
static_assert(offsetof(CGrannyPc, unknown_0460) == 0x460);
static_assert(offsetof(CGrannyPc, model) == 0x484);
static_assert(offsetof(CGrannyPc, instance) == 0x4B0);
static_assert(offsetof(CGrannyPc, clock) == 0xF04);
static_assert(offsetof(CGrannyPc, lastClock) == 0xF14);
static_assert(offsetof(CGrannyPc, unknown_0F24) == 0xF24);
static_assert(offsetof(CGrannyPc, motion) == 0xF3C);
static_assert(offsetof(CGrannyPc, controls) == 0x1050);
static_assert(offsetof(CGrannyPc, commands) == 0x4B4);
static_assert(offsetof(CGrannyPc, layerCommands) == 0x52C);
static_assert(offsetof(CGrannyPc, vertices) == 0x9DC);
static_assert(offsetof(CGrannyPc, layerVertices) == 0xA54);
static_assert(offsetof(CGrannyPc, unknown_2550) == 0x2550);
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
static_assert(offsetof(CPc, unknown_0418) == 0x418);
static_assert(offsetof(CPc, unknown_04E4) == 0x4E4);
static_assert(offsetof(CPc, unknown_04F0) == 0x4F0);
static_assert(offsetof(CPc, unknown_04F8) == 0x4F8);
static_assert(offsetof(CPc, unknown_04FC) == 0x4FC);
static_assert(offsetof(CPc, unknown_050C) == 0x50C);
static_assert(offsetof(CHalluPC, unknown_0510) == 0x510);
static_assert(offsetof(CHalluPC2, unknown_0510) == 0x510);
static_assert(offsetof(CPc, weapon) == 0x448);
static_assert(offsetof(CPc, unknown_044C) == 0x44C);
static_assert(offsetof(CPc, unknown_04CC) == 0x4CC);
static_assert(offsetof(CPc, unknown_04DC) == 0x4DC);
static_assert(offsetof(CPc, unknown_04E0) == 0x4E0);
#endif

#endif
