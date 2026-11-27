#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PRIMITIVES_EFFECTPRIMITIVESTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PRIMITIVES_EFFECTPRIMITIVESTATE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"
struct CRagEffect;
class CTexture;

extern float g_fullCircleAngle;
extern float g_fireflyNormalDrift;
extern float g_fireflyModeFive;

enum TeiRectPalette : int {
  TEI_RECT_WHITE_ALPHA_BLEND = 0,
  TEI_RECT_PALE_RED = 1,
  TEI_RECT_PALE_BLUE_ALPHA_BLEND = 2,
  TEI_RECT_FAINT_WHITE_ALPHA_BLEND = 3,
  TEI_RECT_SOFT_BLUE = 4,
  TEI_RECT_WHITE_ADDITIVE = 5,
  TEI_RECT_SOFT_RED = 6,
  TEI_RECT_SOFT_GREEN = 7,
  TEI_RECT_PALE_YELLOW = 8,
  TEI_RECT_MUTED_GREEN = 9,
  TEI_RECT_ROSE_PINK = 10,
  TEI_RECT_LAVENDER_BLUE = 11,
  TEI_RECT_DARK_GRAY_ALPHA_BLEND = 12,
  TEI_RECT_MAGENTA_ALPHA_BLEND = 13,
  TEI_RECT_BLUE_ALPHA_BLEND = 14,
  TEI_RECT_BRIGHT_GREEN = 15,
  TEI_RECT_LIME_GREEN = 16,
  TEI_RECT_BLUE = 17,
  TEI_RECT_RED = 18,
  TEI_RECT_GREEN = 19,
  TEI_RECT_YELLOW = 20,
  TEI_RECT_MAGENTA = 21,
  TEI_RECT_CYAN = 22,
  TEI_RECT_STEEL_BLUE_ALPHA_BLEND = 23,
  TEI_RECT_DARK_BLUE = 24,
  TEI_RECT_CERULEAN = 25,
  TEI_RECT_LIGHT_BLUE = 26,
  TEI_RECT_SKY_BLUE = 27,
  TEI_RECT_BLUE_VIOLET = 28,
  TEI_RECT_YELLOW_GREEN = 29,
  TEI_RECT_DEEP_VIOLET = 30,
  TEI_RECT_PEACH = 31,
  TEI_RECT_BLACK_ALPHA_BLEND = 32,
  TEI_RECT_VIVID_BLUE = 40,
  TEI_RECT_DEEP_BLUE = 41,
  TEI_RECT_CHARCOAL_ALPHA_BLEND = 42,
  TEI_RECT_SEA_GREEN_ALPHA_BLEND = 43,
  TEI_RECT_BRICK_RED_ALPHA_BLEND = 44,
  TEI_RECT_AZURE = 45,
  TEI_RECT_RUST_RED = 46,
  TEI_RECT_DARK_GOLD = 47,
  TEI_RECT_MAROON = 48,
  TEI_RECT_COPPER = 49,
  TEI_RECT_AMBER = 50,
  TEI_RECT_ICE_WHITE_CROPPED = 100,
};

enum PolygonFrameMode : char {
  POLYGON_FRAME_UNKNOWN_0 = 0,
  POLYGON_FRAME_UNKNOWN_2 = 2,
  POLYGON_FRAME_UNKNOWN_3 = 3,
  POLYGON_FRAME_GREEN = 19,
  POLYGON_FRAME_RED_GLOW = 20,
  POLYGON_FRAME_PALE_GREEN = 21,
};

enum SpiralBladeMode : char {
  SPIRAL_BLADE_UNKNOWN_0 = 0,
  SPIRAL_BLADE_UNKNOWN_3 = 3,
  SPIRAL_BLADE_TORCH_RED = 4,
};

enum FieldPanelMode : char {
  FIELD_PANEL_WHITE_ALPHA_BLEND = 0,
  FIELD_PANEL_PALE_BLUE = 1,
  FIELD_PANEL_REVERSED_BLUE = 2,
  FIELD_PANEL_MUTED_YELLOW = 3,
  FIELD_PANEL_REVERSED_WHITE_ALPHA_BLEND = 4,
  FIELD_PANEL_REVERSED_WHITE = 7,
  FIELD_PANEL_PALE_GREEN = 8,
  FIELD_PANEL_SELECTED_TINT = 9,
  FIELD_PANEL_PALE_YELLOW = 10,
  FIELD_PANEL_PALE_MINT = 11,
  FIELD_PANEL_PEACH = 12,
  FIELD_PANEL_SOFT_RED = 13,
  FIELD_PANEL_DARK_RED = 14,
  FIELD_PANEL_PALE_MAGENTA = 15,
  FIELD_PANEL_BLACK = 16,
  FIELD_PANEL_PALE_RED = 17,
  FIELD_PANEL_HALF_ALPHA_WHITE = 18,
  FIELD_PANEL_ANIMATED_WHITE = 23,
  FIELD_PANEL_INDIGO = 24,
  FIELD_PANEL_LOWERED_WHITE = 25,
};

enum FieldPanelTint : char {
  FIELD_PANEL_TINT_RED = 1,
  FIELD_PANEL_TINT_ORANGE = 2,
  FIELD_PANEL_TINT_YELLOW = 3,
  FIELD_PANEL_TINT_WHITE_ALPHA_BLEND = 4,
  FIELD_PANEL_TINT_BLUE = 5,
  FIELD_PANEL_TINT_WHITE_ADDITIVE = 6,
};

enum StackedRingMode : char {
  STACKED_RING_BLUE_ADDITIVE = 0,
  STACKED_RING_GOLD = 1,
  STACKED_RING_GREEN = 2,
  STACKED_RING_BLUE_ALPHA_BLEND = 3,
  STACKED_RING_RED = 4,
  STACKED_RING_NEAR_BLACK = 5,
  STACKED_RING_WHITE = 6,
  STACKED_RING_MAGENTA = 7,
  STACKED_RING_FAINT_PALE_BLUE = 8,
  STACKED_RING_PALE_BLUE_ADDITIVE = 10,
  STACKED_RING_RED_WITH_WHITE_GLOW = 11,
};

enum TeiFacingMode : char {
  TEI_FACING_WHITE_ALPHA_BLEND = 0,
  TEI_FACING_PALE_BLUE = 2,
  TEI_FACING_OFFSET_WHITE_ALPHA_BLEND = 3,
  TEI_FACING_WHITE_ADDITIVE = 4,
  TEI_FACING_SOFT_GREEN = 5,
  TEI_FACING_GRAY = 10,
  TEI_FACING_DARK_GRAY = 12,
  TEI_FACING_TEAL = 15,
  TEI_FACING_OVERHEAD_WHITE = 16,
  TEI_FACING_RAISED_WHITE = 17,
  TEI_FACING_RAISED_BLUE_GLOW = 18,
  TEI_FACING_RAISED_GREEN_GLOW = 19,
  TEI_FACING_RAISED_RED_GLOW = 20,
  TEI_FACING_UNKNOWN_31 = 31,
  TEI_FACING_RED_ADDITIVE = 32,
};

enum BladeQuadMode : char {
  BLADE_QUAD_CYAN_AND_RED = 0,
  BLADE_QUAD_WHITE_AND_YELLOW = 1,
  BLADE_QUAD_RED_AND_PINK = 2,
  BLADE_QUAD_PALE_BLUE_AND_BLUE = 3,
};

enum JitterQuadMode : char {
  JITTER_QUAD_WHITE_WITH_ADDITIVE_GLOW = 0,
  JITTER_QUAD_BLUE_GLOW = 1,
  JITTER_QUAD_WHITE_ADDITIVE = 2,
};

enum DriftingOrbMotion : std::uint32_t {
  DRIFTING_ORB_IDLE = 0,
  DRIFTING_ORB_DISPERSING = 1,
};

enum ElementalOrbitKind : char {
  ELEMENTAL_ORBIT_SPIRIT_SPHERE = 0,
  ELEMENTAL_ORBIT_SPIRIT_SPHERE2 = 1,
  ELEMENTAL_ORBIT_SPIRIT_SPHERE3 = 3,
  ELEMENTAL_ORBIT_RICH_COIN = 4,
  ELEMENTAL_ORBIT_FIRE = 10,
  ELEMENTAL_ORBIT_LIGHTNING = 11,
  ELEMENTAL_ORBIT_WATER = 12,
  ELEMENTAL_ORBIT_STONE = 13,
};

enum DriftingOrbStyle : char {
  DRIFTING_ORB_STYLE_DEFAULT = 0,
  DRIFTING_ORB_STYLE_BROWN_GLOW = 1,
  DRIFTING_ORB_STYLE_FIRE = ELEMENTAL_ORBIT_FIRE,
  DRIFTING_ORB_STYLE_LIGHTNING = ELEMENTAL_ORBIT_LIGHTNING,
  DRIFTING_ORB_STYLE_WATER = ELEMENTAL_ORBIT_WATER,
  DRIFTING_ORB_STYLE_STONE = ELEMENTAL_ORBIT_STONE,
  DRIFTING_ORB_STYLE_ONE_EXTRA_GLOW = 20,
  DRIFTING_ORB_STYLE_TWO_EXTRA_GLOWS = 21,
};

enum RippleMode : char {
  RIPPLE_FOLLOW_MASTER = 1,
  RIPPLE_TIGHT_RAISED_LAYERS = 2,
};

enum ColorPlaneTint : int {
  COLOR_PLANE_PALE_RED_ALPHA_BLEND = 1,
  COLOR_PLANE_PALE_GREEN_ALPHA_BLEND = 2,
  COLOR_PLANE_YELLOW = 3,
  COLOR_PLANE_PALE_RED_ADDITIVE = 4,
};

enum ColorPanelMode : int {
  COLOR_PANEL_FOREGROUND_GREEN = 2,
  COLOR_PANEL_FOREGROUND_BLUE = 3,
  COLOR_PANEL_FOREGROUND_YELLOW = 4,
  COLOR_PANEL_FOREGROUND_RED = 5,
  COLOR_PANEL_FOREGROUND_MAGENTA = 6,
  COLOR_PANEL_FOREGROUND_CYAN = 7,
  COLOR_PANEL_FOREGROUND_GRAYSCALE = 8,
};

enum BladeTrailTint : char {
  BLADE_TRAIL_BLUE = 0,
  BLADE_TRAIL_PALEST_BLUE = 1,
  BLADE_TRAIL_SOFT_BLUE = 2,
  BLADE_TRAIL_PALE_BLUE = 3,
};

enum FanBeamMode : int {
  FAN_BEAM_NARROW_BLUE_AND_YELLOW = 2,
  FAN_BEAM_WHITE = 3,
  FAN_BEAM_BLUE_WITH_ALPHA_BLENDED_YELLOW = 4,
  FAN_BEAM_GRAY_WITH_ALPHA_BLENDED_WHITE = 5,
  FAN_BEAM_FOREGROUND_BLUE = 6,
};

enum OrbitSquareMode : char {
  ORBIT_SQUARE_LIGHT_BLUE = 0,
  ORBIT_SQUARE_PALE_YELLOW = 1,
  ORBIT_SQUARE_FACING_PALE_RED = 2,
  ORBIT_SQUARE_BLUE = 3,
  ORBIT_SQUARE_RED = 4,
  ORBIT_SQUARE_GREEN = 5,
  ORBIT_SQUARE_FOREGROUND_PALE_RED = 6,
  ORBIT_SQUARE_FOREGROUND_BLUE = 7,
  ORBIT_SQUARE_FACING_BLUE = 8,
  ORBIT_SQUARE_FACING_GRAY = 9,
};

enum HaloLayerMode : char {
  HALO_LAYER_BLACK_ALPHA_BLEND = 0,
  HALO_LAYER_FOLLOW_MASTER = 1,
  HALO_LAYER_TIGHT_RAISED = 2,
  HALO_LAYER_DARK_GRAY_WITH_OUTER_PASS = 3,
  HALO_LAYER_TIGHT_RAISED_QUICK_FADE = 4,
  HALO_LAYER_TIGHT_WHITE_WITH_BLUE_GLOW = 5,
  HALO_LAYER_WIDE_WHITE_WITH_BLUE_GLOW = 6,
  HALO_LAYER_WIDE_WHITE_WITH_RED_GLOW = 7,
};

enum SizedSquareMode : int {
  SIZED_SQUARE_ROTATING_WHITE_ALPHA_BLEND = 0,
  SIZED_SQUARE_CONDITIONAL_ROTATION = 4,
  SIZED_SQUARE_ROTATING_WHITE = 10,
  SIZED_SQUARE_GREEN = 15,
  SIZED_SQUARE_PALE_BLUE = 17,
  SIZED_SQUARE_BLUE = 18,
  SIZED_SQUARE_DOUBLE_WHITE = 100,
  SIZED_SQUARE_DOUBLE_RED = 101,
  SIZED_SQUARE_CYAN = 102,
  SIZED_SQUARE_WHITE_ALPHA_BLEND = 110,
  SIZED_SQUARE_ROTATING_BLUE = 111,
  SIZED_SQUARE_ROTATING_SOFT_GREEN = 112,
  SIZED_SQUARE_FOREGROUND_BLUE = 113,
  SIZED_SQUARE_FOREGROUND_RED = 114,
};

enum LandProtectorMotion : int {
  LAND_PROTECTOR_SLOW_PULSE = 2,
  LAND_PROTECTOR_FAST_SPIN = 4,
  LAND_PROTECTOR_SPINNING_FLARE = 10,
  LAND_PROTECTOR_SPINNING_OUTER_RING = 11,
  LAND_PROTECTOR_SPINNING_INNER_RING = 12,
};

enum DriftingSquareMotion : char {
  DRIFTING_SQUARE_SINGLE_VERTICAL = 1,
};

enum PulseRingMode : int {
  PULSE_RING_WHITE_ADDITIVE = 0,
  PULSE_RING_PALE_GREEN_ALPHA_BLEND = 1,
  PULSE_RING_CROSS_BRACED_GRAY_ALPHA_BLEND = 2,
  PULSE_RING_OSCILLATING_BLUE_CHANNEL = 4,
};

enum GlowFollowMotion : char {
  GLOW_FOLLOW_BRIGHT_PALE_GREEN = 31,
};

enum SpiralFadeMotion : char {
  SPIRAL_FADE_REVERSED_EARLY_FADE = 1,
};

enum FieldFireflyMode : int {
  FIELD_FIREFLY_ALTITUDE_GATED_CLOUD = 0,
  FIELD_FIREFLY_GROUND_LEVEL_CLOUD = 2,
  FIELD_FIREFLY_TERRAIN_FOG = 3,
  FIELD_FIREFLY_LOW_SLOW_DARK_RED_CLOUD = 5,
  FIELD_FIREFLY_DARK_RED_CLOUD = 11,
  FIELD_FIREFLY_BLACK_CLOUD = 12,
  FIELD_FIREFLY_PALE_RED_CLOUD = 13,
  FIELD_FIREFLY_INDIGO_CLOUD = 14,
  FIELD_FIREFLY_MAP_CLOUD = 99,
};

enum PulsingColumnMode : char {
  PULSING_COLUMN_WAVE = 0,
  PULSING_COLUMN_FAST_SPIN = 1,
  PULSING_COLUMN_SHORT_PULSE = 2,
  PULSING_COLUMN_SLOWER_SPIN = 3,
  PULSING_COLUMN_CAMERA_ALIGNED_WAVE = 33,
};

enum FadingColumnMode : int {
  FADING_COLUMN_TALL_PALE_GREEN = 0,
  FADING_COLUMN_TIMED_BLUE = 10,
  FADING_COLUMN_MOVE_SENSITIVE_BLUE = 20,
};

enum AuraGlowMotion : char {
  AURA_GLOW_PALE_BLUE_SLOW_FADE_IN = 0,
  AURA_GLOW_WHITE_PULSE = 1,
  AURA_GLOW_EXPANDING_PALE_BLUE = 2,
  AURA_GLOW_OFFSET_WHITE_PULSE = 3,
  AURA_GLOW_GRAY_FORWARD_SPIN = 10,
  AURA_GLOW_GRAY_REVERSE_SPIN = 11,
  AURA_GLOW_DARK_GRAY_REVERSE_SPIN = 12,
  AURA_GLOW_LARGE_DARK_GRAY = 13,
  AURA_GLOW_FAINT_WHITE = 14,
  AURA_GLOW_SOFT_GREEN_REVERSE_SPIN = 16,
  AURA_GLOW_SMALL_SOFT_GREEN_REVERSE_SPIN = 17,
  AURA_GLOW_STATIC_SOFT_GREEN = 18,
  AURA_GLOW_SMALL_WHITE = 20,
  AURA_GLOW_OVERHEAD_WHITE = 22,
  AURA_GLOW_STATIC_RAISED_GREEN = 30,
};

enum GroundFogMode : char {
  GROUND_FOG_DOUBLE_WHITE = 0,
  GROUND_FOG_DOUBLE_BLUE = 1,
  GROUND_FOG_YELLOW_GHOST = 3,
  GROUND_FOG_WHITE = 10,
  GROUND_FOG_GRAY_WANDERING_GHOST = 11,
  GROUND_FOG_RISING_WHITE = 14,
  GROUND_FOG_HORIZONTAL_DRIFT = 15,
  GROUND_FOG_RISING_SCATTERED = 16,
};

enum FacingQuadTint : char {
  FACING_QUAD_PINK_RED = 7,
  FACING_QUAD_YELLOW = 8,
  FACING_QUAD_WHITE = 10,
};

enum TeiQuadShape : int {
  TEI_QUAD_SQUARE = 0,
  TEI_QUAD_RECTANGLE_60_120 = 1,
  TEI_QUAD_RECTANGLE_130_50 = 2,
};

struct ZeroVector3d : vector3d {
  // INLINED
  ZeroVector3d() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  }

  using vector3d::operator=;

  // INLINED
  void Set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

struct EffectPrimitiveRecord {
  EffectPrimitiveRecord();

  /* 0000 */ ZeroVector3d position;
  /* 000C */ ZeroVector3d corners[4];
  /* 003C */ float radius;
  /* 0040 */ float scale;
  /* 0044 */ float angle;
  /* 0048 */ float matrix[12];
  /* 0078 */ std::int32_t red;
  /* 007C */ std::int32_t green;
  /* 0080 */ std::int32_t blue;
  /* 0084 */ float alpha;
  /* 0088 */ std::uint32_t color;
};

struct EffectPrimitiveSlot {
  EffectPrimitiveSlot();
  /* 0000 */ char life;
  /* 0001 */ unsigned char padding_0001;
  /* 0002 */ std::int16_t alphaB;
  /* 0004 */ std::int16_t alphaT;
  /* 0006 */ std::int16_t fullDisplayAngle;
  /* 0008 */ float maxHeight;
  /* 000C */ int process;
  /* 0010 */ std::int16_t rotStart;
  /* 0012 */ unsigned char padding_0012[2];
  /* 0014 */ float height[21];
  /* 0068 */ char flag1[21];
  /* 007D */ unsigned char padding_007D;
  /* 007E */ std::int16_t riseAngle;
  /* 0080 */ float distance;
  union {
    /* 0084 */ vector3d vectors[4];
    struct {
      /* 0084 */ vector3d vecBNow;
      /* 0090 */ vector3d vecTNow;
      /* 009C */ vector3d vecBPre;
      /* 00A8 */ vector3d vecTPre;
    };
  };
};

struct EffectPrimitiveState {
  // INLINED
  __forceinline EffectPrimitiveState() {}

  /* 0138 */ std::uint32_t changePointReached;
  /* 013C */ std::uint32_t isChangeProcessed;
  /* 0140 */ CRagEffect* master;
  /* 0144 */ CTexture** textures = nullptr;
  /* 0148 */ std::uint32_t playWaveName;
  /* 014C */ std::uint32_t masterReached;
  /* 0150 */ std::uint32_t alphaSpeedCycle;
  /* 0154 */ std::uint32_t alphaCycle;
  /* 0158 */ std::uint32_t chRadiSpeed;
  /* 015C */ std::uint32_t isDisappear;
  /* 0160 */ matrix transform;
  /* 0190 */ matrix rotTransform;
  /* 01C0 */ std::uint32_t argb;
  /* 01C4 */ std::uint32_t renderFlag;
  /* 01C8 */ unsigned char useRenderFlag[4];
  /* 01CC */ std::uint32_t type;
  /* 01D0 */ int count;
  /* 01D4 */ std::uint32_t chCnt;
  /* 01D8 */ int colorCycleIndex;
  /* 01DC */ std::uint32_t pattern;
  /* 01E0 */ int stateCnt;
  /* 01E4 */ int duration;
  /* 01E8 */ int animSpeed;
  /* 01EC */ int fadeInCnt;
  /* 01F0 */ int fadeOutCnt;
  /* 01F4 */ int numSegment;
  /* 01F8 */ int validAngle;
  /* 01FC */ std::uint32_t RGBCycleCnt;
  /* 0200 */ int changePoint;
  /* 0204 */ unsigned totalTexture;
  /* 0208 */ std::uint32_t playWaveType;
  /* 020C */ unsigned char notifyCounts[40];
  /* 0234 */ std::uint32_t RGBCycleDelay;
  /* 0238 */ std::uint32_t commonPattern;
  /* 023C */ int red;
  /* 0240 */ int green;
  /* 0244 */ int blue;
  /* 0248 */ int redSpeed;
  /* 024C */ int greenSpeed;
  /* 0250 */ int blueSpeed;
  /* 0254 */ unsigned char repeatAnim;
  /* 0255 */ unsigned char padding_0255[3];
  /* 0258 */ std::vector<float> chPoint;
  /* 0264 */ std::vector<float> chVal1;
  /* 0270 */ std::vector<float> chVal2;
  /* 027C */ std::vector<float> chVal3;
  /* 0288 */ float arcAngle;
  /* 028C */ std::uint32_t minRadius;
  /* 0290 */ float rotAxis[3];
  /* 029C */ float changeSpeed;
  /* 02A0 */ float changeAccel;
  /* 02A4 */ vector3d changeSpeed3d;
  /* 02B0 */ vector3d changeAccel3d;
  /* 02BC */ float speed;
  /* 02C0 */ float accel;
  /* 02C4 */ float changeWidthAccel;
  /* 02C8 */ float changeWidthSpeed;
  /* 02CC */ float changeHeightSpeed;
  /* 02D0 */ float changeHeightAccel;
  /* 02D4 */ float maxAlpha;
  /* 02D8 */ float minAlpha;
  /* 02DC */ std::uint32_t u1Bits;
  /* 02E0 */ float u2;
  /* 02E4 */ float v1;
  /* 02E8 */ float v2;
  /* 02EC */ float gravSpeed;
  /* 02F0 */ float gravAccel;
  /* 02F4 */ float innerSize;
  /* 02F8 */ float outerSize;
  /* 02FC */ float innerSpeed;
  /* 0300 */ float outerSpeed;
  /* 0304 */ float innerAccel;
  /* 0308 */ float outerAccel;
  /* 030C */ float longitude;
  /* 0310 */ float latitude;
  /* 0314 */ float roll;
  /* 0318 */ float size;
  /* 031C */ float sizeSpeed;
  /* 0320 */ float sizeAccel;
  /* 0324 */ float alpha;
  /* 0328 */ float alphaSpeed;
  /* 032C */ float alphaDelta;
  /* 0330 */ float longSpeed;
  /* 0334 */ float latiSpeed;
  /* 0338 */ float rollSpeed;
  /* 033C */ float longAccel;
  /* 0340 */ float latiAccel;
  /* 0344 */ float rollAccel;
  /* 0348 */ float radius;
  /* 034C */ float radiusSpeed;
  /* 0350 */ float radiusAccel;
  /* 0354 */ float widthSize;
  /* 0358 */ float widthSpeed;
  /* 035C */ float widthAccel;
  /* 0360 */ float heightSize;
  /* 0364 */ float heightSpeed;
  /* 0368 */ float heightAccel;
  /* 036C */ float maxHeightSize;
  /* 0370 */ ZeroVector3d speed3d;
  /* 037C */ ZeroVector3d orgPos;
  /* 0388 */ vector3d deltaPos;
  /* 0394 */ vector3d deltaPos2;
  /* 03A0 */ ZeroVector3d deltaPos3;
  /* 03AC */ ZeroVector3d deltaPosAccel;
  /* 03B8 */ ZeroVector3d segPos[4];
  /* 03E8 */ EffectPrimitiveSlot slots[4];
  /* 06B8 */ EffectPrimitiveRecord records[50];
  /* 2210 */ RenderTriangle triangles[512];
  union {
    /* FA10 */ std::int64_t spriteLift;
    struct {
      /* FA10 */ std::uint32_t spriteLiftLow;
      /* FA14 */ std::uint32_t spriteLiftHigh;
    };
  };
};

class CActRes;

struct EffectPrimitiveObject {
  /* 0000 */ RenderObjectState render;
  /* 0138 */ EffectPrimitiveState state;

  CActRes* ActionResource();
  bool AdvanceFrame();
};

static_assert(sizeof(EffectPrimitiveRecord) == 140);
static_assert(sizeof(EffectPrimitiveSlot) == 0xB4);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(EffectPrimitiveRecord, corners) == 0xC);
static_assert(offsetof(EffectPrimitiveRecord, matrix) == 0x48);
static_assert(offsetof(EffectPrimitiveRecord, color) == 0x88);
static_assert(offsetof(EffectPrimitiveSlot, height) == 0x14);
static_assert(offsetof(EffectPrimitiveSlot, flag1) == 0x68);
static_assert(offsetof(EffectPrimitiveSlot, vecBNow) == 0x84);
static_assert(offsetof(EffectPrimitiveState, masterReached) == 0x14);
static_assert(offsetof(EffectPrimitiveState, alphaCycle) == 0x1C);
static_assert(sizeof(EffectPrimitiveState) == 0xF8E0);
#endif

#endif
