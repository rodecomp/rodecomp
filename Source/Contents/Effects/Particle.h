#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLE_H

#include "Render/Primitives/RenderPrimitive.h"
#include "Resource/EZeffectResource.h"
#include "UI/Window/UIControl.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CTexture;

struct vector2d {
  vector2d() : x(0), y(0) {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
};

struct ParticleQuad {
  /* 0000 */ bool billboard;
  /* 0004 */ float top;
  /* 0008 */ float bottom;
  /* 000C */ float left;
  /* 0010 */ float right;
  /* 0014 */ bool unused;
  /* 0015 */ bool depth;
};

struct EZeffectData {
  /* 0000 */ int maximumFrame;
  /* 0004 */ int fps;
  /* 0008 */ int clipCount;
  /* 000C */ int unknown_000C;
  /* 0010 */ EZeffectClip clips[60];
};

typedef void (*ParticleSubmit)(RenderCommand* primitive, int flags);

class CParticle {
public:
  CParticle(vector3d position, int flags);
  virtual ~CParticle();
  virtual void Slot01() {}

  void Reset();
  void SetQuad(RenderVertex* vertices, float top, float bottom, float left, float right);
  void Render(bool billboard, float top, float bottom, float left, float right, bool unused, bool depth);
  void Render(const ParticleQuad& quad);
  void SetColor(short redValue, short greenValue, short blueValue);
  void SetTexture(const char* name);
  void SetTexture(std::string name);
  void SetRenderFlag(unsigned char value);

  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ CTexture* texture;
  /* 001C */ int flags;
  /* 0020 */ float scale;
  /* 0024 */ vector3d position;
  /* 0030 */ vector3d unknown_0030 = {};
  /* 003C */ short red;
  /* 003E */ short green;
  /* 0040 */ short blue;
  /* 0042 */ unsigned char unknown_0042[2];
  /* 0044 */ int alpha;
  /* 0048 */ unsigned char unknown_0048[4];
  /* 004C */ int unknown_004C;
  /* 0050 */ unsigned char unknown_0050[4];
  /* 0054 */ float unknown_0054;
  /* 0058 */ unsigned char unknown_0058[0x50];
  /* 00A8 */ unsigned char unknown_00A8;
  /* 00A9 */ unsigned char unknown_00A9[0x17];
  /* 00C0 */ float unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ float life;
  /* 00D0 */ float unknown_00D0 = 0;
  /* 00D4 */ float unknown_00D4 = 0;
  /* 00D8 */ float unknown_00D8 = 0;
  /* 00DC */ int unknown_00DC = 0;
  /* 00E0 */ int unknown_00E0 = 0;
  /* 00E4 */ int unknown_00E4 = 0;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ int unknown_00EC;
  /* 00F0 */ RenderPrimitive primitive;
  /* 0198 */ unsigned char unknown_0198[0x30];
  /* 01C8 */ matrix transform;
  /* 01F8 */ RenderVertex vertices[4];
  /* 0278 */ vector3d unknown_0278[4];
  /* 02A8 */ int sourceBlend;
  /* 02AC */ int destinationBlend;
  /* 02B0 */ int unknown_02B0;
  /* 02B4 */ float depthOffset;
  /* 02B8 */ ParticleSubmit submit;
};

class CEZ2STRParticle : public CParticle {
public:
  CEZ2STRParticle(vector3d position, int flags);
  ~CEZ2STRParticle() override;
  void Slot01() override;

  void Clear();
  std::string ResolvePath(const std::string& name);
  void Open(std::string name);
  void Restart();
  void Load(const std::string& name);
  bool Process(float x, float y, float z);
  void UpdatePosition(float x, float y, float z);
  void DrawBillboards(const matrix& view);
  void DrawScreen(const matrix& view);
  void RenderBillboard(EZeffectClip* clip, EZeffectTransform* transform, const matrix& view, int index);
  void RenderScreen(EZeffectClip* clip, EZeffectTransform* transform, const matrix& view, int index);
  void RenderGround(EZeffectClip* clip, EZeffectTransform* transform, const matrix& view, int index);
  void DrawGround(const matrix& view);
  void Advance(float amount);

  /* 02BC */ CResPtr<CEZeffectRes> m_res;
  /* 02C0 */ EZeffectData* m_data;
  /* 02C4 */ EZeffectTransform m_transforms[128];
  /* 3CC4 */ int m_finishedClips;
  /* 3CC8 */ int m_clipLimit;
  /* 3CCC */ bool m_active[128];
  /* 3D4C */ int m_cursors[128];
  /* 3F4C */ vector3d m_screen;
  /* 3F58 */ int m_frame;
  /* 3F5C */ bool m_loop;
  /* 3F5D */ bool m_hold = false;
  /* 3F60 */ std::string m_name;
  /* 3F78 */ std::string m_texturePrefix;
  /* 3F90 */ int m_endFrame;
  /* 3F94 */ bool m_counting;
  /* 3F98 */ int m_lifeTime;
  /* 3F9C */ int m_repeat;
  /* 3FA0 */ int m_delay;
  /* 3FA4 */ float m_angle;
  /* 3FA8 */ float m_pitch = 0;
  /* 3FAC */ float m_yaw = 0;
  /* 3FB0 */ float m_scale;
  /* 3FB4 */ float m_alpha;
  /* 3FB8 */ int m_windowId;
  /* 3FBC */ bool m_attachToWindow;
  /* 3FBD */ bool m_visible;
  /* 3FC0 */ vector3d m_windowOffset = {};
  /* 3FCC */ bool m_followActor;
  /* 3FD0 */ std::uint32_t m_actorId;
  /* 3FD4 */ float m_groundHeight = 0;
  /* 3FD8 */ float m_shiftX;
  /* 3FDC */ float m_shiftY;
  /* 3FE0 */ long long m_lift;
  /* 3FE8 */ bool m_flipX;
  /* 3FE9 */ bool m_flipY;
  /* 3FEA */ short unknown_3FEA = 0;
  /* 3FEC */ float m_screenShiftX = 0;
  /* 3FF0 */ float m_screenShiftY = 0;
  /* 3FF4 */ int unknown_3FF4;
};

void InitParticleQuad(RenderVertex* vertices);
float RandomRange(float minimum, float maximum);
vector2d RandomDiscPoint(float x, float y, long long radius);
std::string EmitterKey(std::string effect, std::string emitter);
std::string EmitterInfoKey(std::string effect);
void SubmitParticlePrimitive(RenderCommand* primitive, int flags);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ParticleQuad) == 0x18);
static_assert(offsetof(ParticleQuad, unused) == 0x14);
static_assert(offsetof(EZeffectData, clips) == 0x10);
static_assert(offsetof(CParticle, texture) == 0x18);
static_assert(offsetof(CParticle, red) == 0x3C);
static_assert(offsetof(CParticle, alpha) == 0x44);
static_assert(offsetof(CParticle, life) == 0xCC);
static_assert(offsetof(CParticle, primitive) == 0xF0);
static_assert(offsetof(CParticle, transform) == 0x1C8);
static_assert(offsetof(CParticle, vertices) == 0x1F8);
static_assert(offsetof(CParticle, sourceBlend) == 0x2A8);
static_assert(offsetof(CParticle, submit) == 0x2B8);
static_assert(sizeof(CParticle) == 0x2BC);
static_assert(offsetof(CEZ2STRParticle, m_transforms) == 0x2C4);
static_assert(offsetof(CEZ2STRParticle, m_finishedClips) == 0x3CC4);
static_assert(offsetof(CEZ2STRParticle, m_cursors) == 0x3D4C);
static_assert(offsetof(CEZ2STRParticle, m_screen) == 0x3F4C);
static_assert(offsetof(CEZ2STRParticle, m_name) == 0x3F60);
static_assert(offsetof(CEZ2STRParticle, m_texturePrefix) == 0x3F78);
static_assert(offsetof(CEZ2STRParticle, m_endFrame) == 0x3F90);
static_assert(offsetof(CEZ2STRParticle, m_angle) == 0x3FA4);
static_assert(offsetof(CEZ2STRParticle, m_windowId) == 0x3FB8);
static_assert(offsetof(CEZ2STRParticle, m_actorId) == 0x3FD0);
static_assert(offsetof(CEZ2STRParticle, m_lift) == 0x3FE0);
static_assert(offsetof(CEZ2STRParticle, m_flipX) == 0x3FE8);
static_assert(offsetof(CEZ2STRParticle, m_screenShiftY) == 0x3FF0);
static_assert(sizeof(CEZ2STRParticle) == 0x3FF8);
#endif

#endif
