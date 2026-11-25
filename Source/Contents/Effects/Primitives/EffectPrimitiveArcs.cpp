#include "Contents/Effects/Primitives/EffectPrim.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Contents/Actors/GameActor.h"

// GLOBAL: gl_25993
float g_fireflyGroundOffset = 20.0f;

// GLOBAL: gl_26005
float g_fireflyArcStep33 = 33.0f;

// GLOBAL: gl_26006
float g_fireflyArcStep46 = 46.0f;

// GLOBAL: gl_26007
float g_fireflyArcStep59 = 59.0f;

// GLOBAL: gl_26008
float g_fireflyArcStep71 = 71.0f;

// GLOBAL: gl_26009
float g_fireflyArcStep84 = 84.0f;

// GLOBAL: gl_26010
float g_fireflyArcLength = 100.0f;

// GLOBAL: gl_25995
float g_fireflyModeFive = 5.0f;

// GLOBAL: gl_25998
float g_fireflyModeThirteen = 13.0f;

// GLOBAL: gl_25997
float g_fireflyModeTwelve = 12.0f;

// INLINED
static __forceinline void ZeroVector(vector3d& v) {
  v.x = 0.0f;
  v.y = 0.0f;
  v.z = 0.0f;
}

// INLINED
static __forceinline vector3d operator+(const vector3d& a, const vector3d& b) {
  vector3d result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return result;
}

// OFFSET: 0x00BEBC80
// FUNCTION: fn_19972
void CEffectPrim::RenderGroundArcGlow(matrix& wtm) {
  vector3d prevB;
  ZeroVector(prevB);
  vector3d curB;
  ZeroVector(curB);
  vector3d curT;
  ZeroVector(curT);
  vector3d prevT;
  ZeroVector(prevT);
  int rpCnt = 0;
  short alpha;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1) {
      continue;
    }

    const int step = this->state.slots[ec].fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= this->state.slots[ec].fullDisplayAngle; offset += step) {
      int angle = this->state.slots[ec].rotStart + offset;
      angle -= angle / 360 * 360;
      if (this->state.slots[ec].fullDisplayAngle == 360 && i == 20) {
        angle = this->state.slots[ec].rotStart;
      }

      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float radius = g_sinTable[this->state.slots[ec].rotStart] + this->state.slots[ec].distance;
      const float baseX = radius * cosine;
      const float baseZ = radius * sine;
      curB.x = baseX;
      curB.y = 0.0f;
      curB.z = baseZ;
      curT.x = this->state.slots[ec].height[i] * g_cosTable[this->state.slots[ec].riseAngle] * cosine + baseX;
      curT.y = -(this->state.slots[ec].height[i] * g_sinTable[this->state.slots[ec].riseAngle]);
      curT.z = this->state.slots[ec].height[i] * g_cosTable[this->state.slots[ec].riseAngle] * sine + baseZ;

      if (i > 0) {
        int texture;
        if (this->state.slots[ec].alphaT == 10) {
          texture = this->state.slots[ec].process / 3 % static_cast<int>(this->state.totalTexture);
        } else {
          texture = this->state.slots[ec].process % static_cast<int>(this->state.totalTexture);
          this->state.size = 0.0f;
          this->RenderTeiRect(&rpCnt, i - 1, this->state.slots[ec].alphaB, this->state.slots[ec].alphaB, prevB, curB, curT, prevT, wtm, static_cast<char>(texture));
        }

        if (this->state.slots[ec].alphaT <= 2) {
          this->state.size = 17.0f;
          alpha = this->state.slots[ec].alphaB * 3;
        } else if (this->state.slots[ec].alphaT == 3) {
          this->state.size = 18.0f;
          alpha = this->state.slots[ec].alphaB * 3;
        } else if (this->state.slots[ec].alphaT == 4) {
          this->state.size = 19.0f;
          alpha = this->state.slots[ec].alphaB * 2;
        } else if (this->state.slots[ec].alphaT == 5) {
          this->state.size = 20.0f;
          alpha = this->state.slots[ec].alphaB * 2;
        } else if (this->state.slots[ec].alphaT == 7) {
          this->state.size = 21.0f;
          alpha = this->state.slots[ec].alphaB * 2;
        } else if (this->state.slots[ec].alphaT == 8) {
          this->state.size = 22.0f;
          alpha = this->state.slots[ec].alphaB * 2;
        } else if (this->state.slots[ec].alphaT == 9) {
          this->state.size = 30.0f;
          if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
            alpha = this->state.slots[ec].alphaB;
          } else {
            alpha = this->state.slots[ec].alphaB * 2;
          }
        } else if (this->state.slots[ec].alphaT == 6) {
          this->state.size = 5.0f;
          alpha = this->state.slots[ec].alphaB * 2;
        } else if (this->state.slots[ec].alphaT == 10) {
          this->state.size = 6.0f;
          alpha = this->state.slots[ec].alphaB * 3;
        } else if (this->state.slots[ec].alphaT == 11) {
          this->state.size = 32.0f;
          if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
            alpha = this->state.slots[ec].alphaB;
          } else {
            alpha = this->state.slots[ec].alphaB * 2;
          }
        }

        if (alpha > 255) {
          alpha = 255;
        }
        this->RenderTeiRect(&rpCnt, i - 1, alpha, this->state.slots[ec].alphaB, prevB, curB, curT, prevT, wtm, static_cast<char>(texture));
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevB = curB;
      prevT = curT;
    }
  }
}

// OFFSET: 0x00BEC150
// FUNCTION: fn_19973
void CEffectPrim::RenderGroundArc(matrix& wtm) {
  vector3d prevB;
  ZeroVector(prevB);
  vector3d curB;
  ZeroVector(curB);
  vector3d curT;
  ZeroVector(curT);
  vector3d prevT;
  ZeroVector(prevT);
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].process <= 0) {
      continue;
    }

    const int step = this->state.slots[ec].fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= this->state.slots[ec].fullDisplayAngle; offset += step) {
      int angle = this->state.slots[ec].rotStart + offset;
      angle -= angle / 360 * 360;
      if (this->state.slots[ec].fullDisplayAngle == 360 && i == 20) {
        angle = this->state.slots[ec].rotStart;
      }

      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      curB.x = this->state.slots[ec].distance * cosine;
      curB.y = 5.0f;
      curB.z = this->state.slots[ec].distance * sine;
      curT.x = this->state.slots[ec].height[i] * g_cosTable[this->state.slots[ec].riseAngle] * cosine + curB.x;
      curT.z = this->state.slots[ec].height[i] * g_cosTable[this->state.slots[ec].riseAngle] * sine + curB.z;
      curT.y = g_fireflyModeFive - this->state.slots[ec].height[i] * g_sinTable[this->state.slots[ec].riseAngle];

      if (i > 0) {
        this->RenderTeiRect(
          &rpCnt, i - 1, this->state.slots[ec].alphaB, this->state.slots[ec].alphaB, prevB, curB, curT, prevT, wtm,
          static_cast<char>(this->state.slots[ec].process % static_cast<int>(this->state.totalTexture))
        );
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevB = curB;
      prevT = curT;
    }
  }
}

// OFFSET: 0x00BEC3D0
// FUNCTION: fn_19974
void CEffectPrim::RenderGroundArc2(matrix& wtm) {
  vector3d prevB;
  ZeroVector(prevB);
  vector3d curB;
  ZeroVector(curB);
  vector3d curT;
  ZeroVector(curT);
  vector3d prevT;
  ZeroVector(prevT);
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].process <= 0) {
      continue;
    }

    const int step = this->state.slots[ec].fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= this->state.slots[ec].fullDisplayAngle; offset += step) {
      int angle = this->state.slots[ec].rotStart + offset;
      angle -= angle / 360 * 360;
      if (this->state.slots[ec].fullDisplayAngle == 360 && i == 20) {
        angle = this->state.slots[ec].rotStart;
      }

      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      curB.x = this->state.slots[ec].distance * cosine;
      curB.y = 5.0f;
      curB.z = this->state.slots[ec].distance * sine;
      curT.x = this->state.slots[ec].height[i] * g_cosTable[this->state.slots[ec].riseAngle] * cosine + curB.x;
      curT.z = this->state.slots[ec].height[i] * g_cosTable[this->state.slots[ec].riseAngle] * sine + curB.z;
      curT.y = g_fireflyModeFive - this->state.slots[ec].height[i] * g_sinTable[this->state.slots[ec].riseAngle];

      if (i > 0) {
        this->RenderTeiRect(
          &rpCnt, i - 1, this->state.slots[ec].alphaB, this->state.slots[ec].alphaB, prevB, curB, curT, prevT, wtm,
          static_cast<char>(this->state.slots[ec].process % static_cast<int>(this->state.totalTexture))
        );
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevB = curB;
      prevT = curT;
    }
  }
}

// OFFSET: 0x00BEC650
// FUNCTION: fn_19975
void CEffectPrim::RenderGroundArcStep(matrix& wtm) {
  vector3d prevB;
  ZeroVector(prevB);
  vector3d curB;
  ZeroVector(curB);
  vector3d curT;
  ZeroVector(curT);
  vector3d prevT;
  ZeroVector(prevT);
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    auto& slotsRef = this->state.slots;
    if (slotsRef[ec].life != 1) {
      continue;
    }

    const int step = slotsRef[ec].fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= slotsRef[ec].fullDisplayAngle; offset += step) {
      int angle = slotsRef[ec].rotStart + offset;
      angle -= angle / 360 * 360;
      if (slotsRef[ec].fullDisplayAngle == 360 && i == 20) {
        angle = slotsRef[ec].rotStart;
      }

      curB.x = slotsRef[ec].distance * g_cosTable[angle];
      curB.z = slotsRef[ec].distance * g_sinTable[angle];
      if (slotsRef[ec].maxHeight == 6.001f) {
        curB.y = -2.0f;
      } else if (slotsRef[ec].maxHeight == 24.001f) {
        curB.y = -1.0f;
      } else {
        curB.y = 0.0f;
      }

      const float riseX = slotsRef[ec].height[i] * g_cosTable[slotsRef[ec].riseAngle];
      const float riseY = slotsRef[ec].height[i] * g_sinTable[slotsRef[ec].riseAngle];
      curT.x = riseX * g_cosTable[angle] + curB.x;
      curT.z = riseX * g_sinTable[angle] + curB.z;
      if (slotsRef[ec].maxHeight == 6.001f) {
        curT.y = -(2.0f + riseY);
      } else if (slotsRef[ec].maxHeight == 24.001f) {
        curT.y = -(1.0f + riseY);
      } else {
        curT.y = -riseY;
      }

      if (i > 0) {
        this->RenderTeiRect(
          &rpCnt, i - 1, slotsRef[ec].alphaB, slotsRef[ec].alphaB, prevB, curB, curT, prevT, wtm,
          static_cast<char>(slotsRef[ec].process % static_cast<int>(this->state.totalTexture))
        );
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevB = curB;
      prevT = curT;
    }
  }
}

// OFFSET: 0x00BEC960
// FUNCTION: fn_19976
__declspec(safebuffers) void CEffectPrim::RenderSegmentSprites(matrix& wtm) {
  CSprRes* sprite = this->GetSprite();
  CActRes* action = this->GetActRes();

  for (int ec = 0; ec < 4; ++ec) {
    for (int seg = 0; seg < this->state.numSegment; ++seg) {
      const EffectPrimitiveRecord& record = this->state.records[seg];
      vector3d position = record.position;
      position += this->state.slots[ec].vecBNow;
      const float depth = position.y * wtm.values[5] + position.x * wtm.values[2] + position.z * wtm.values[8] + wtm.values[11];

      float screenX;
      float screenY;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&position.x, wtm.values, &screenX, &screenY, &reciprocalDepth);
      const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

      const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);
      for (int index = 0; index < frame.cellCount; ++index) {
        const SpriteFrameCell& cell = frame.Cell(index);
        if (cell.image == -1) {
          continue;
        }

        const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
        const double scale = record.scale;
        const float top = static_cast<float>(cell.y * scale * cell.scaleY * distanceScale + screenY);
        const float bottom = static_cast<float>(((static_cast<short>(image->doubleHeight) + 1) * image->height + cell.y) * scale * cell.scaleY * distanceScale + screenY);
        if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f) {
          continue;
        }

        float left;
        float right;
        if ((cell.mirrored & 1) != 0) {
          left = static_cast<float>(((static_cast<short>(image->doubleWidth) + 1) * image->width + cell.x) * scale * cell.scaleX * distanceScale + screenX);
          right = static_cast<float>(cell.x * scale * cell.scaleX * distanceScale + screenX);
          if (right >= static_cast<float>(g_renderer->viewport.width) || left <= 0.0f) {
            continue;
          }
        } else {
          left = static_cast<float>(cell.x * scale * cell.scaleX * distanceScale + screenX);
          right = static_cast<float>(((static_cast<short>(image->doubleWidth) + 1) * image->width + cell.x) * scale * cell.scaleX * distanceScale + screenX);
          if (left >= static_cast<float>(g_renderer->viewport.width) || right <= 0.0f) {
            continue;
          }
        }

        SpriteQuad quad;
        quad.top = top;
        quad.left = left;
        quad.bottom = bottom;
        quad.right = right;
        short alpha = static_cast<short>((cell.color >> 24) * 0.85f);
        quad.bottomRhw = 1.0f / ((bottom - screenY) * g_tangent + depth) + 0.0001f;
        quad.topRhw = 1.0f / ((top - screenY) * g_tangent + depth) + 0.0001f;
        if ((this->state.pattern & 0x200) != 0) {
          if (this->state.slots[ec].flag1[0] == 0) {
            quad.color = (((((alpha << 8) + cell.colorChannels.red) << 8) + cell.colorChannels.green) << 8) + cell.colorChannels.blue;
          } else if (this->state.slots[ec].flag1[0] == 1) {
            quad.color = (alpha << 24) + 0x3737FF;
          }
        } else {
          quad.color = record.color;
        }
        quad.angle = static_cast<float>(cell.rotation) + this->state.roll;
        quad.image = image;
        this->DrawQuad(quad, sprite->palette, 5, 0.0f, 0.0f, 0);

        quad.top -= 4.0f;
        quad.bottom += 4.0f;
        quad.left -= 4.0f;
        quad.right += 4.0f;
        alpha = static_cast<short>((cell.color >> 24) * g_depthAverageHalf);
        quad.bottomRhw = 1.0f / ((bottom - screenY) * g_tangent + depth) + 0.0002f;
        quad.topRhw = 1.0f / ((top - screenY) * g_tangent + depth) + 0.0002f;
        if ((this->state.pattern & 0x200) != 0) {
          if (this->state.slots[ec].flag1[0] == 0) {
            quad.color = (((alpha << 8) + cell.colorChannels.red) << 16) + 0x3737;
          } else if (this->state.slots[ec].flag1[0] == 1) {
            quad.color = (alpha << 24) + 0x3737FF;
          }
        } else {
          quad.color = record.color;
        }
        quad.angle = static_cast<float>(cell.rotation) + this->state.roll;
        quad.image = image;
        this->DrawQuad(quad, sprite->palette, 5, 0.0f, 0.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BECEB0
// FUNCTION: fn_19977
void CEffectPrim::RenderGroundSquare(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1) {
      continue;
    }

    const int angle = this->state.slots[ec].rotStart;
    float cosA = g_cosTable[angle];
    float sinA = g_sinTable[angle];
    const double radius = (g_sinTable[this->state.slots[ec].riseAngle] + 1.0f) * (this->state.slots[ec].distance * 0.1f) + this->state.slots[ec].distance * 0.8;

    this->state.slots[ec].vecBPre.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecBPre.z = static_cast<float>(sinA * radius);
    this->state.slots[ec].vecBPre.y = this->state.slots[ec].height[0];
    int angle2 = angle + (angle < 270 ? 90 : -270);
    cosA = g_cosTable[angle2];
    sinA = g_sinTable[angle2];
    this->state.slots[ec].vecBNow.y = this->state.slots[ec].height[0];
    this->state.slots[ec].vecBNow.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecBNow.z = static_cast<float>(sinA * radius);
    angle2 += angle2 + 90 < 360 ? 90 : -270;
    cosA = g_cosTable[angle2];
    sinA = g_sinTable[angle2];
    this->state.slots[ec].vecTNow.y = this->state.slots[ec].height[0];
    this->state.slots[ec].vecTNow.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecTNow.z = static_cast<float>(sinA * radius);
    const int angle4 = angle2 + (angle2 + 90 < 360 ? 90 : -270);
    cosA = g_cosTable[angle4];
    sinA = g_sinTable[angle4];
    this->state.slots[ec].vecTPre.y = this->state.slots[ec].height[0];
    this->state.slots[ec].vecTPre.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecTPre.z = static_cast<float>(sinA * radius);

    const short alpha = this->state.slots[ec].alphaB;
    if (this->state.slots[ec].height[2] == 1.0f) {
      this->RenderTeiRect(
        &rpCnt, alpha, this->state.slots[ec].vecBPre, this->state.slots[ec].vecBNow, this->state.slots[ec].vecTNow, this->state.slots[ec].vecTPre, wtm, 1,
        static_cast<char>(this->state.slots[ec].height[1]), 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
    } else {
      this->RenderTeiRect(
        &rpCnt, alpha, this->state.slots[ec].vecBPre, this->state.slots[ec].vecBNow, this->state.slots[ec].vecTNow, this->state.slots[ec].vecTPre, wtm, 0,
        static_cast<char>(this->state.slots[ec].height[1]), 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
    }
  }
}

// OFFSET: 0x00BED140
// FUNCTION: fn_19978
void CEffectPrim::RenderGroundSquareBlend(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1) {
      continue;
    }

    const int angle = this->state.slots[ec].rotStart;
    float cosA = g_cosTable[angle];
    float sinA = g_sinTable[angle];
    const double radius = (g_sinTable[this->state.slots[ec].riseAngle] + 1.0f) * (this->state.slots[ec].distance * 0.1f) + this->state.slots[ec].distance * 0.8;

    this->state.slots[ec].vecBPre.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecBPre.z = static_cast<float>(sinA * radius);
    this->state.slots[ec].vecBPre.y = this->state.slots[ec].height[0];
    int angle2 = angle + (angle < 270 ? 90 : -270);
    cosA = g_cosTable[angle2];
    sinA = g_sinTable[angle2];
    this->state.slots[ec].vecBNow.y = this->state.slots[ec].height[0];
    this->state.slots[ec].vecBNow.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecBNow.z = static_cast<float>(sinA * radius);
    angle2 += angle2 + 90 < 360 ? 90 : -270;
    cosA = g_cosTable[angle2];
    sinA = g_sinTable[angle2];
    this->state.slots[ec].vecTNow.y = this->state.slots[ec].height[0];
    this->state.slots[ec].vecTNow.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecTNow.z = static_cast<float>(sinA * radius);
    const int angle4 = angle2 + (angle2 + 90 < 360 ? 90 : -270);
    cosA = g_cosTable[angle4];
    sinA = g_sinTable[angle4];
    this->state.slots[ec].vecTPre.y = this->state.slots[ec].height[0];
    this->state.slots[ec].vecTPre.x = static_cast<float>(cosA * radius);
    this->state.slots[ec].vecTPre.z = static_cast<float>(sinA * radius);

    short alpha = static_cast<short>(this->state.slots[ec].process % 256 * 2);
    if (alpha > 255) {
      alpha = 511 - alpha;
    }

    this->RenderTeiRect(
      &rpCnt, alpha, this->state.slots[ec].vecBPre, this->state.slots[ec].vecBNow, this->state.slots[ec].vecTNow, this->state.slots[ec].vecTPre, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f,
      1.0f, 0
    );
    this->RenderTeiRect(
      &rpCnt, 255 - alpha, this->state.slots[ec].vecBPre, this->state.slots[ec].vecBNow, this->state.slots[ec].vecTNow, this->state.slots[ec].vecTPre, wtm, 1, 1, 0, 255, 255, 255, 0.0f, 1.0f,
      0.0f, 1.0f, 0
    );
  }
}

// OFFSET: 0x00BED490
// FUNCTION: fn_19979
__declspec(safebuffers) void CEffectPrim::RenderJitterQuads(matrix& wtm) {
  vector3d vec1;
  ZeroVector(vec1);
  vector3d vec2;
  ZeroVector(vec2);
  vector3d vec3;
  ZeroVector(vec3);
  vector3d vec4;
  ZeroVector(vec4);
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    if (this->state.slots[ec].height[8] >= 360.0f || this->state.slots[ec].height[8] < 0.0f) {
      this->state.slots[ec].height[8] = 0.0f;
    }

    int angle = this->state.slots[ec].fullDisplayAngle;
    const float radius = this->state.slots[ec].distance * g_sinTable[static_cast<int>(this->state.slots[ec].height[8])] * 0.05f + this->state.slots[ec].distance;
    vec1.x = radius * g_cosTable[angle];
    vec1.y = 0.0f;
    vec1.z = radius * g_sinTable[angle];
    angle += rand() % 16 + 85;
    if (angle >= 360) {
      angle -= 360;
    }
    if (angle < 0) {
      angle += 360;
    }
    vec2.x = radius * g_cosTable[angle];
    vec2.y = 0.0f;
    vec2.z = radius * g_sinTable[angle];
    angle += rand() % 16 + 85;
    if (angle >= 360) {
      angle -= 360;
    }
    if (angle < 0) {
      angle += 360;
    }
    vec3.x = radius * g_cosTable[angle];
    vec3.y = 0.0f;
    vec3.z = radius * g_sinTable[angle];
    angle += rand() % 16 + 85;
    if (angle >= 360) {
      angle -= 360;
    }
    if (angle < 0) {
      angle += 360;
    }
    vec4.x = radius * g_cosTable[angle];
    vec4.y = 0.0f;
    vec4.z = radius * g_sinTable[angle];
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    vec1.x = this->state.slots[ec].vecBNow.x + vec1.x;
    vec1.y = this->state.slots[ec].vecBNow.y + vec1.y;
    vec1.z = this->state.slots[ec].vecBNow.z + vec1.z;
    vec2.x = this->state.slots[ec].vecBNow.x + vec2.x;
    vec2.y = this->state.slots[ec].vecBNow.y + vec2.y;
    vec2.z = this->state.slots[ec].vecBNow.z + vec2.z;
    vec3.x = this->state.slots[ec].vecBNow.x + vec3.x;
    vec3.y = this->state.slots[ec].vecBNow.y + vec3.y;
    vec3.z = this->state.slots[ec].vecBNow.z + vec3.z;
    vec4.x = this->state.slots[ec].vecBNow.x + vec4.x;
    vec4.y = this->state.slots[ec].vecBNow.y + vec4.y;
    vec4.z = this->state.slots[ec].vecBNow.z + vec4.z;

    switch (this->state.slots[0].flag1[1]) {
    case 1:
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, vec2, vec1, vec4, vec3, wtm, 1, this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x + 0.2f, vec2.y + 0.0f, vec2.z + 0.0f}, vector3d{vec1.x + 0.2f, vec1.y + 0.0f, vec1.z + 0.0f},
        vector3d{vec4.x + 0.2f, vec4.y + 0.0f, vec4.z + 0.0f}, vector3d{vec3.x + 0.2f, vec3.y + 0.0f, vec3.z + 0.0f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x - 0.2f, vec2.y - 0.0f, vec2.z - 0.0f}, vector3d{vec1.x - 0.2f, vec1.y - 0.0f, vec1.z - 0.0f},
        vector3d{vec4.x - 0.2f, vec4.y - 0.0f, vec4.z - 0.0f}, vector3d{vec3.x - 0.2f, vec3.y - 0.0f, vec3.z - 0.0f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x + 0.0f, vec2.y + 0.0f, vec2.z + 0.2f}, vector3d{vec1.x + 0.0f, vec1.y + 0.0f, vec1.z + 0.2f},
        vector3d{vec4.x + 0.0f, vec4.y + 0.0f, vec4.z + 0.2f}, vector3d{vec3.x + 0.0f, vec3.y + 0.0f, vec3.z + 0.2f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x - 0.0f, vec2.y - 0.0f, vec2.z - 0.2f}, vector3d{vec1.x - 0.0f, vec1.y - 0.0f, vec1.z - 0.2f},
        vector3d{vec4.x - 0.0f, vec4.y - 0.0f, vec4.z - 0.2f}, vector3d{vec3.x - 0.0f, vec3.y - 0.0f, vec3.z - 0.2f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, vec2, vec1, vec4, vec3, wtm, 0, this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x + 0.2f, vec2.y + 0.0f, vec2.z + 0.0f}, vector3d{vec1.x + 0.2f, vec1.y + 0.0f, vec1.z + 0.0f},
        vector3d{vec4.x + 0.2f, vec4.y + 0.0f, vec4.z + 0.0f}, vector3d{vec3.x + 0.2f, vec3.y + 0.0f, vec3.z + 0.0f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x - 0.2f, vec2.y - 0.0f, vec2.z - 0.0f}, vector3d{vec1.x - 0.2f, vec1.y - 0.0f, vec1.z - 0.0f},
        vector3d{vec4.x - 0.2f, vec4.y - 0.0f, vec4.z - 0.0f}, vector3d{vec3.x - 0.2f, vec3.y - 0.0f, vec3.z - 0.0f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x + 0.0f, vec2.y + 0.0f, vec2.z + 0.2f}, vector3d{vec1.x + 0.0f, vec1.y + 0.0f, vec1.z + 0.2f},
        vector3d{vec4.x + 0.0f, vec4.y + 0.0f, vec4.z + 0.2f}, vector3d{vec3.x + 0.0f, vec3.y + 0.0f, vec3.z + 0.2f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x - 0.0f, vec2.y - 0.0f, vec2.z - 0.2f}, vector3d{vec1.x - 0.0f, vec1.y - 0.0f, vec1.z - 0.2f},
        vector3d{vec4.x - 0.0f, vec4.y - 0.0f, vec4.z - 0.2f}, vector3d{vec3.x - 0.0f, vec3.y - 0.0f, vec3.z - 0.2f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      break;
    default:
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, vec2, vec1, vec4, vec3, wtm, 1, this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x + 0.2f, vec2.y + 0.0f, vec2.z + 0.0f}, vector3d{vec1.x + 0.2f, vec1.y + 0.0f, vec1.z + 0.0f},
        vector3d{vec4.x + 0.2f, vec4.y + 0.0f, vec4.z + 0.0f}, vector3d{vec3.x + 0.2f, vec3.y + 0.0f, vec3.z + 0.0f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x - 0.2f, vec2.y - 0.0f, vec2.z - 0.0f}, vector3d{vec1.x - 0.2f, vec1.y - 0.0f, vec1.z - 0.0f},
        vector3d{vec4.x - 0.2f, vec4.y - 0.0f, vec4.z - 0.0f}, vector3d{vec3.x - 0.2f, vec3.y - 0.0f, vec3.z - 0.0f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x + 0.0f, vec2.y + 0.0f, vec2.z + 0.2f}, vector3d{vec1.x + 0.0f, vec1.y + 0.0f, vec1.z + 0.2f},
        vector3d{vec4.x + 0.0f, vec4.y + 0.0f, vec4.z + 0.2f}, vector3d{vec3.x + 0.0f, vec3.y + 0.0f, vec3.z + 0.2f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].alphaB / 2, vector3d{vec2.x - 0.0f, vec2.y - 0.0f, vec2.z - 0.2f}, vector3d{vec1.x - 0.0f, vec1.y - 0.0f, vec1.z - 0.2f},
        vector3d{vec4.x - 0.0f, vec4.y - 0.0f, vec4.z - 0.2f}, vector3d{vec3.x - 0.0f, vec3.y - 0.0f, vec3.z - 0.2f}, wtm, 0,
        this->state.slots[ec].flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
      );
      break;
    }
  }
}

// OFFSET: 0x00BEFD80
// FUNCTION: fn_19983
void CEffectPrim::RenderFanWedge(matrix& wtm) {
  vector3d tip{};
  vector3d left{};
  vector3d right{};
  vector3d back{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    const int angle = this->state.slots[ec].riseAngle;
    const float dist = this->state.slots[ec].distance;
    const float base = this->state.slots[ec].height[0];
    int leftAngle = angle - 7;
    if (leftAngle < 0) {
      leftAngle += 360;
    }
    int rightAngle = angle + 7;
    if (rightAngle >= 360) {
      rightAngle -= 360;
    }
    int backAngle = angle - 270;
    if (backAngle < 0) {
      backAngle += 360;
    }
    const float leftSpread = dist * g_cosTable[leftAngle];
    left.z = dist * g_sinTable[leftAngle];
    const float rotSin = g_sinTable[this->state.slots[ec].rotStart];
    const float rotCos = g_cosTable[this->state.slots[ec].rotStart];
    left.x = leftSpread * rotCos;
    left.y = leftSpread * rotSin + base;
    const float rightSpread = dist * g_cosTable[rightAngle];
    right.z = dist * g_sinTable[rightAngle];
    right.x = rightSpread * rotCos;
    right.y = rightSpread * rotSin + base;
    const float backSpread = 7.0f * g_cosTable[backAngle];
    back.z = 7.0f * g_sinTable[backAngle];
    back.x = backSpread * rotCos;
    back.y = backSpread * rotSin + base;
    int tipAngle = angle - 90;
    if (tipAngle < 0) {
      tipAngle += 360;
    }
    const float tipSpread = 7.0f * g_cosTable[tipAngle];
    tip.z = 7.0f * g_sinTable[tipAngle];
    tip.x = tipSpread * rotCos;
    tip.y = tipSpread * rotSin + base;

    this->RenderTeiRect(
      &rpCnt, this->state.slots[ec].alphaB, tip, left, right, back, wtm, 0, static_cast<unsigned char>(this->state.slots[ec].height[1]), 0, 105,
      105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
    );
  }
}

// OFFSET: 0x00BF1210
// FUNCTION: fn_19986
void CEffectPrim::RenderFanSlice(matrix& wtm) {
  vector3d center2{};
  vector3d left{};
  vector3d right{};
  vector3d center{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    const int angle = this->state.slots[ec].riseAngle;
    const float dist = this->state.slots[ec].distance;
    const float base = this->state.slots[ec].height[0];
    int leftAngle = angle - 5;
    if (leftAngle < 0) {
      leftAngle += 360;
    }
    int rightAngle = angle + 5;
    if (rightAngle >= 360) {
      rightAngle -= 360;
    }
    const float leftSpread = dist * g_cosTable[leftAngle];
    left.z = dist * g_sinTable[leftAngle];
    const float rotSin = g_sinTable[this->state.slots[ec].rotStart];
    const float rotCos = g_cosTable[this->state.slots[ec].rotStart];
    left.x = leftSpread * rotCos;
    left.y = leftSpread * rotSin + base;
    const float rightSpread = dist * g_cosTable[rightAngle];
    right.z = dist * g_sinTable[rightAngle];
    right.x = rightSpread * rotCos;
    right.y = rightSpread * rotSin + base;
    center.x = 0.0f;
    center.y = base;
    center.z = 0.0f;
    center2.x = 0.0f;
    center2.y = base;
    center2.z = 0.0f;

    this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, left, right, center, center2, wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
  }
}

// OFFSET: 0x00BF0010
// FUNCTION: fn_19984
void CEffectPrim::RenderFanTrail(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    const int angle = this->state.slots[ec].riseAngle;
    const float size = this->state.slots[ec].maxHeight;
    int side[2];
    side[0] = angle - 20;
    side[1] = angle + 20;
    const int leftAngle = side[0] < 0 ? side[0] + 360 : side[0];

    vector3d left;
    const float leftX = size * g_cosTable[leftAngle];
    left.z = size * g_sinTable[leftAngle];
    const float rotCos = g_cosTable[this->state.slots[ec].rotStart];
    const float rotSin = g_sinTable[this->state.slots[ec].rotStart];
    left.x = leftX * rotCos;
    left.y = leftX * rotSin + this->state.slots[ec].height[0];

    const int rightAngle = side[1] >= 360 ? side[1] - 360 : side[1];
    vector3d right;
    right.z = size * g_sinTable[rightAngle];
    const float rightX = size * g_cosTable[rightAngle];
    right.x = rightX * rotCos;
    right.y = rightX * rotSin + this->state.slots[ec].height[0];

    const int backRightAngle = side[1] - 180 < 0 ? side[1] - 180 + 360 : side[1] - 180;
    vector3d backRight;
    backRight.z = size * g_sinTable[backRightAngle];
    const float backRightX = size * g_cosTable[backRightAngle];
    backRight.x = backRightX * rotCos;
    backRight.y = backRightX * rotSin + this->state.slots[ec].height[0];

    const int backLeftAngle = side[0] + 180 >= 360 ? side[0] + 180 - 360 : side[0] + 180;
    vector3d backLeft;
    const float backLeftX = size * g_cosTable[backLeftAngle];
    backLeft.z = size * g_sinTable[backLeftAngle];
    backLeft.x = backLeftX * rotCos;
    backLeft.y = backLeftX * rotSin + this->state.slots[ec].height[0];

    const float dirCos = g_cosTable[angle];
    const float dirSin = g_sinTable[angle];

    for (short step = 0; step < 6; ++step) {
      const float dist = step * this->state.slots[ec].maxHeight + this->state.slots[ec].distance;
      vector3d offset;
      offset.x = dist * dirCos;
      offset.y = 0.0f;
      offset.z = dist * dirSin;
      if (dist > g_fireflyArcLength) {
        break;
      }

      if (dist > 0.0f) {
        this->RenderTeiRect(
          &rpCnt, this->state.slots[ec].alphaB, backRight + offset, left + offset, right + offset, backLeft + offset, wtm, 0,
          static_cast<unsigned char>(this->state.slots[ec].height[1]), 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
        );
      }
    }
  }
}

// OFFSET: 0x00BF0370
// FUNCTION: fn_19985
void CEffectPrim::RenderCurvedFanTrail(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }
    for (short step = 0; step < 7; ++step) {
      const float size = this->state.slots[ec].maxHeight;
      const float dist = step * size + this->state.slots[ec].distance;
      short angle = this->state.slots[ec].riseAngle;
      if (dist > 20.0f) {
        angle += 5;
      }
      if (dist > 33.0f) {
        angle += 5;
      }
      if (dist > 46.0f) {
        angle += 5;
      }
      if (dist > 59.0f) {
        angle += 5;
      }
      if (dist > 71.0f) {
        angle += 5;
      }
      if (dist > 84.0f) {
        angle += 5;
      }
      if (angle >= 360) {
        angle -= 360;
      }

      int leftAngle = angle - 15;
      if (leftAngle < 0) {
        leftAngle += 360;
      }
      int rightAngle = angle + 15;
      if (rightAngle >= 360) {
        rightAngle -= 360;
      }
      const int high = angle + 15;
      const int low = angle - 15;
      const int backRightAngle = high < 180 ? high + 180 : high - 180;
      const int backLeftAngle = low + 180 >= 360 ? low + 180 - 360 : low + 180;
      const float base = this->state.slots[ec].height[0];
      vector3d left;
      left.z = size * g_sinTable[leftAngle];
      const float rotCos = g_cosTable[this->state.slots[ec].rotStart];
      const float rotSin = g_sinTable[this->state.slots[ec].rotStart];
      left.x = size * g_cosTable[leftAngle] * rotCos;
      left.y = size * g_cosTable[leftAngle] * rotSin + base;
      vector3d right;
      right.z = size * g_sinTable[rightAngle];
      right.x = size * g_cosTable[rightAngle] * rotCos;
      right.y = size * g_cosTable[rightAngle] * rotSin + base;
      vector3d backRight;
      backRight.z = size * g_sinTable[backRightAngle];
      backRight.x = size * g_cosTable[backRightAngle] * rotCos;
      backRight.y = size * g_cosTable[backRightAngle] * rotSin + base;
      vector3d backLeft;
      backLeft.z = size * g_sinTable[backLeftAngle];
      backLeft.x = size * g_cosTable[backLeftAngle] * rotCos;
      backLeft.y = size * g_cosTable[backLeftAngle] * rotSin + base;

      float remain = dist;
      int dir = this->state.slots[ec].riseAngle;
      vector3d bend0{};
      if (dist > 20.0f) {
        remain = dist - g_fireflyGroundOffset;
        bend0.x = g_cosTable[dir] * g_fireflyGroundOffset;
        bend0.z = g_sinTable[dir] * g_fireflyGroundOffset;
      }
      vector3d bend1{};
      if (dist > 33.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep33;
        bend1.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend1.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      vector3d bend2{};
      if (dist > 46.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep46;
        bend2.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend2.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      vector3d bend3{};
      if (dist > 59.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep59;
        bend3.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend3.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      vector3d bend4{};
      if (dist > 71.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep71;
        bend4.x = g_cosTable[dir] * g_fireflyModeTwelve;
        bend4.z = g_sinTable[dir] * g_fireflyModeTwelve;
      }
      vector3d bend5{};
      if (dist > 84.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep84;
        bend5.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend5.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      if (dist > 20.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
      }
      vector3d tail;
      tail.x = remain * g_cosTable[dir];
      tail.y = 0.0f;
      tail.z = remain * g_sinTable[dir];

      vec1.x = backRight.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec1.y = backRight.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec1.z = backRight.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;
      vec2.x = left.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec2.y = left.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec2.z = left.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;
      vec3.x = right.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec3.y = right.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec3.z = right.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;
      vec4.x = backLeft.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec4.y = backLeft.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec4.z = backLeft.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;

      if (dist > g_fireflyArcLength) {
        break;
      }

      if (dist > 0.0f) {
        this->RenderTeiRect(
          &rpCnt, this->state.slots[ec].alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<unsigned char>(this->state.slots[ec].height[1]), 0,
          65, 65, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
        );
      }
    }

    for (short step = 0; step < 7; ++step) {
      const float size = this->state.slots[ec].maxHeight;
      const float dist = step * size + this->state.slots[ec].height[10];
      short angle = this->state.slots[ec].riseAngle;
      if (dist > 20.0f) {
        angle += 5;
      }
      if (dist > 33.0f) {
        angle += 5;
      }
      if (dist > 46.0f) {
        angle += 5;
      }
      if (dist > 59.0f) {
        angle += 5;
      }
      if (dist > 71.0f) {
        angle += 5;
      }
      if (dist > 84.0f) {
        angle += 5;
      }
      if (angle >= 360) {
        angle -= 360;
      }

      int leftAngle = angle - 25;
      if (leftAngle < 0) {
        leftAngle += 360;
      }
      int rightAngle = angle + 25;
      if (rightAngle >= 360) {
        rightAngle -= 360;
      }
      const int high = angle + 25;
      const int low = angle - 25;
      const int backRightAngle = high < 180 ? high + 180 : high - 180;
      const int backLeftAngle = low + 180 >= 360 ? low + 180 - 360 : low + 180;
      const float base = this->state.slots[ec].height[0];
      vector3d left;
      left.z = size * g_sinTable[leftAngle];
      const float rotCos = g_cosTable[this->state.slots[ec].rotStart];
      const float rotSin = g_sinTable[this->state.slots[ec].rotStart];
      left.x = size * g_cosTable[leftAngle] * rotCos;
      left.y = size * g_cosTable[leftAngle] * rotSin + base;
      vector3d right;
      right.z = size * g_sinTable[rightAngle];
      right.x = size * g_cosTable[rightAngle] * rotCos;
      right.y = size * g_cosTable[rightAngle] * rotSin + base;
      vector3d backRight;
      backRight.z = size * g_sinTable[backRightAngle];
      backRight.x = size * g_cosTable[backRightAngle] * rotCos;
      backRight.y = size * g_cosTable[backRightAngle] * rotSin + base;
      vector3d backLeft;
      backLeft.z = size * g_sinTable[backLeftAngle];
      backLeft.x = size * g_cosTable[backLeftAngle] * rotCos;
      backLeft.y = size * g_cosTable[backLeftAngle] * rotSin + base;

      float remain = dist;
      int dir = this->state.slots[ec].riseAngle;
      vector3d bend0{};
      if (dist > 20.0f) {
        remain = dist - g_fireflyGroundOffset;
        bend0.x = g_cosTable[dir] * g_fireflyGroundOffset;
        bend0.z = g_sinTable[dir] * g_fireflyGroundOffset;
      }
      vector3d bend1{};
      if (dist > 33.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep33;
        bend1.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend1.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      vector3d bend2{};
      if (dist > 46.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep46;
        bend2.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend2.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      vector3d bend3{};
      if (dist > 59.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep59;
        bend3.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend3.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      vector3d bend4{};
      if (dist > 71.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep71;
        bend4.x = g_cosTable[dir] * g_fireflyModeTwelve;
        bend4.z = g_sinTable[dir] * g_fireflyModeTwelve;
      }
      vector3d bend5{};
      if (dist > 84.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
        remain = dist - g_fireflyArcStep84;
        bend5.x = g_cosTable[dir] * g_fireflyModeThirteen;
        bend5.z = g_sinTable[dir] * g_fireflyModeThirteen;
      }
      if (dist > 20.0f) {
        dir += 5;
        if (dir >= 360) {
          dir -= 360;
        }
      }
      vector3d tail;
      tail.x = remain * g_cosTable[dir];
      tail.y = 0.0f;
      tail.z = remain * g_sinTable[dir];

      vec1.x = backRight.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec1.y = backRight.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec1.z = backRight.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;
      vec2.x = left.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec2.y = left.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec2.z = left.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;
      vec3.x = right.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec3.y = right.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec3.z = right.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;
      vec4.x = backLeft.x + bend0.x + bend1.x + bend2.x + bend3.x + bend4.x + bend5.x + tail.x;
      vec4.y = backLeft.y + bend0.y + bend1.y + bend2.y + bend3.y + bend4.y + bend5.y + tail.y;
      vec4.z = backLeft.z + bend0.z + bend1.z + bend2.z + bend3.z + bend4.z + bend5.z + tail.z;

      if (dist > g_fireflyArcLength) {
        break;
      }

      if (dist > 0.0f) {
        this->RenderTeiRect(
          &rpCnt, this->state.slots[ec].alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<unsigned char>(this->state.slots[ec].height[1]), 0,
          35, 35, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
        );
      }
    }
  }
}

// OFFSET: 0x00BEE900
// FUNCTION: fn_19980
__declspec(safebuffers) void CEffectPrim::RenderTwinFacingSquares(matrix& wtm) {
  int rpCnt = 0;
  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1 || slot.alphaB <= 0) {
    return;
  }

  vector3d vec1;
  vector3d vec2;
  vector3d vec3;
  vector3d vec4;
  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;

  float radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * g_fireflyNormalDrift + slot.distance;
  int angle = slot.process * 2;
  angle = angle % 360;
  vec1.x = radius * g_cosTable[angle];
  vec1.z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  vec2.x = radius * g_cosTable[angle];
  vec2.z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  vec3.x = radius * g_cosTable[angle];
  vec3.z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  vec4.x = radius * g_cosTable[angle];
  vec4.z = radius * g_sinTable[angle];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = slot.vecBNow.x + vec1.x;
  vec1.y = slot.vecBNow.y + vec1.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = slot.vecBNow.x + vec2.x;
  vec2.y = slot.vecBNow.y + vec2.y;
  vec2.z = slot.vecBNow.z + vec2.z;
  vec3.x = slot.vecBNow.x + vec3.x;
  vec3.y = slot.vecBNow.y + vec3.y;
  vec3.z = slot.vecBNow.z + vec3.z;
  vec4.x = slot.vecBNow.x + vec4.x;
  vec4.y = slot.vecBNow.y + vec4.y;
  vec4.z = slot.vecBNow.z + vec4.z;

  if (slot.flag1[0] == 10) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 11) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 200, 200, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 12) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 13) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 100, 150, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }

  vec1.y = 0.0f;
  int pulse = slot.process * 5;
  pulse = pulse % 90;
  radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * g_fireflyNormalDrift +
           (g_sinTable[pulse] * 2.0f + g_projectionDepthNumerator) * slot.distance;
  angle = slot.process * 2;
  angle = angle % 360;
  vec1.x = radius * g_cosTable[angle];
  vec1.z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  vec2.x = radius * g_cosTable[angle];
  vec2.y = 0.0f;
  vec2.z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  vec3.x = radius * g_cosTable[angle];
  vec3.y = 0.0f;
  vec3.z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  vec4.x = radius * g_cosTable[angle];
  vec4.y = 0.0f;
  vec4.z = radius * g_sinTable[angle];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x += slot.vecBNow.x;
  vec1.y += slot.vecBNow.y;
  vec1.z += slot.vecBNow.z;
  vec2.x += slot.vecBNow.x;
  vec2.y += slot.vecBNow.y;
  vec2.z += slot.vecBNow.z;
  vec3.x += slot.vecBNow.x;
  vec3.y += slot.vecBNow.y;
  vec3.z += slot.vecBNow.z;
  vec4.x += slot.vecBNow.x;
  vec4.y += slot.vecBNow.y;
  vec4.z += slot.vecBNow.z;

  if (slot.flag1[0] == 10) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 1, 0, 255, 20, 20, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 11) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 200, 200, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 12) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 13) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 100, 150, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BEEF80
// FUNCTION: fn_19981
__declspec(safebuffers) void CEffectPrim::RenderPillarSquares(matrix& wtm) {
  int rpCnt = 0;
  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1 || slot.alphaB <= 0) {
    return;
  }

  vector3d vec1;
  vector3d vec2;
  vector3d vec3;
  vector3d vec4;
  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;

  float radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
  vec1.x = radius * g_cosTable[0];
  vec1.z = radius * g_sinTable[0];
  vec2.x = radius * g_cosTable[90];
  vec2.z = radius * g_sinTable[90];
  vec3.x = radius * g_cosTable[180];
  vec3.z = radius * g_sinTable[180];
  vec4.x = radius * g_cosTable[270];
  vec4.z = radius * g_sinTable[270];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = slot.vecBNow.x + vec1.x;
  vec1.y = slot.vecBNow.y + vec1.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = slot.vecBNow.x + vec2.x;
  vec2.y = slot.vecBNow.y + vec2.y;
  vec2.z = slot.vecBNow.z + vec2.z;
  vec3.x = slot.vecBNow.x + vec3.x;
  vec3.y = slot.vecBNow.y + vec3.y;
  vec3.z = slot.vecBNow.z + vec3.z;
  vec4.x = slot.vecBNow.x + vec4.x;
  vec4.y = slot.vecBNow.y + vec4.y;
  vec4.z = slot.vecBNow.z + vec4.z;
  this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 68, 42, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;
  radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance * 0.5f;
  vec1.x = radius * g_cosTable[0];
  vec1.z = radius * g_sinTable[0];
  vec2.x = radius * g_cosTable[90];
  vec2.z = radius * g_sinTable[90];
  vec3.x = radius * g_cosTable[180];
  vec3.z = radius * g_sinTable[180];
  vec4.x = radius * g_cosTable[270];
  vec4.z = radius * g_sinTable[270];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = slot.vecBNow.x + vec1.x;
  vec1.y = slot.vecBNow.y + vec1.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = slot.vecBNow.x + vec2.x;
  vec2.y = slot.vecBNow.y + vec2.y;
  vec2.z = slot.vecBNow.z + vec2.z;
  vec3.x = slot.vecBNow.x + vec3.x;
  vec3.y = slot.vecBNow.y + vec3.y;
  vec3.z = slot.vecBNow.z + vec3.z;
  vec4.x = slot.vecBNow.x + vec4.x;
  vec4.y = slot.vecBNow.y + vec4.y;
  vec4.z = slot.vecBNow.z + vec4.z;
  this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 155, 155, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

  for (int ec = 0; ec < 4; ++ec) {
    const int angle = this->state.slots[ec].rotStart;
    const int opposite = angle + (angle < 180 ? 180 : -180);
    const float height = this->state.slots[ec].height[12] * 0.5f;

    const float cosA = g_cosTable[angle];
    const float sinA = g_sinTable[angle];
    this->state.slots[ec].vecTPre.y = height;
    this->state.slots[ec].vecTPre.z = sinA * height + cosA * 0.0f;
    this->state.slots[ec].vecTPre.x = cosA * height - sinA * 0.0f;
    const float cosB = g_cosTable[opposite];
    const float sinB = g_sinTable[opposite];
    this->state.slots[ec].vecTNow.y = height;
    this->state.slots[ec].vecTNow.x = cosB * height - sinB * 0.0f;
    this->state.slots[ec].vecTNow.z = sinB * height + cosB * 0.0f;

    this->state.slots[ec].vecTPre += slot.vecBNow;
    this->state.slots[ec].vecTNow += slot.vecBNow;

    vec2 = this->state.slots[ec].vecTNow;
    vec1 = this->state.slots[ec].vecTPre;
    vec2.y -= this->state.slots[ec].height[12];
    vec1.y -= this->state.slots[ec].height[12];

    unsigned char shade;
    if (ec == 0) {
      shade = 105;
    } else if (ec == 1) {
      shade = 35;
    } else {
      shade = ec == 2 ? 75 : 5;
    }

    if (this->state.slots[ec].height[12] > 0.0f) {
      this->RenderTeiRect(
        &rpCnt, this->state.slots[ec].riseAngle, this->state.slots[ec].vecTPre, this->state.slots[ec].vecTNow, vec2, vec1, wtm, 0, 1, 0, shade, shade, 255, 0.0f, 1.0f, 0.0f,
        1.0f, 0
      );
    }
  }
}

// OFFSET: 0x00BEF6C0
// FUNCTION: fn_19982
void CEffectPrim::RenderBladeFins(matrix& wtm) {
  vector3d base{};
  vector3d outer{};
  vector3d inner{};
  vector3d base2{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    base2.x = 0.0f;
    base2.y = -6.0f;
    base2.z = 0.0f;
    base.x = 0.0f;
    base.y = -6.0f;
    base.z = 0.0f;
    outer.y = -6.0f;
    inner.y = -6.0f;

    int angle = this->state.slots[ec].riseAngle < 25 ? this->state.slots[ec].riseAngle + 335 : this->state.slots[ec].riseAngle - 25;
    inner.x = this->state.slots[ec].height[0] * g_cosTable[angle];
    inner.z = this->state.slots[ec].height[0] * g_sinTable[angle];
    angle -= 90;
    if (angle < 0) {
      angle += 360;
    }
    outer.x = this->state.slots[ec].maxHeight * g_cosTable[angle] + inner.x;
    outer.z = this->state.slots[ec].maxHeight * g_sinTable[angle] + inner.z;

    if (this->state.slots[ec].flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, base, outer, inner, base2, wtm, 0, 0, 0, 255, 25, 25, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (this->state.slots[ec].flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, base, outer, inner, base2, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else {
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, base, outer, inner, base2, wtm, 0, 0, 0, 25, 25, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    }

    angle = this->state.slots[ec].riseAngle + 25;
    if (angle >= 360) {
      angle -= 360;
    }
    inner.x = this->state.slots[ec].height[0] * g_cosTable[angle];
    inner.z = this->state.slots[ec].height[0] * g_sinTable[angle];
    angle -= 90;
    if (angle < 0) {
      angle += 360;
    }
    outer.x = this->state.slots[ec].maxHeight * g_cosTable[angle] + inner.x;
    outer.z = this->state.slots[ec].maxHeight * g_sinTable[angle] + inner.z;

    if (this->state.slots[ec].flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, base, outer, inner, base2, wtm, 0, 0, 0, 255, 25, 25, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (this->state.slots[ec].flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, base, outer, inner, base2, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else {
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, base, outer, inner, base2, wtm, 0, 0, 0, 25, 25, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    }
  }
}

// OFFSET: 0x00BF1400
// FUNCTION: fn_19987
void CEffectPrim::RenderPolygonFrames(matrix& wtm) {
  vector3d p0;
  ZeroVector(p0);
  vector3d p1;
  ZeroVector(p1);
  vector3d p2;
  ZeroVector(p2);
  vector3d p3;
  ZeroVector(p3);
  int rpCnt = 0;
  float radius;
  float top;
  int angle;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    radius = slot.distance;
    angle = slot.fullDisplayAngle;
    p0.x = radius * g_cosTable[angle];
    p0.y = 0.0f;
    p0.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p1.x = radius * g_cosTable[angle];
    p1.y = 0.0f;
    p1.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 120;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 50;
      if (angle < 0) {
        angle += 360;
      }
    }
    p2.x = radius * g_cosTable[angle];
    p2.y = 0.0f;
    p2.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p3.x = radius * g_cosTable[angle];
    p3.y = 0.0f;
    p3.z = radius * g_sinTable[angle];

    top = p1.z > p0.z ? p1.z : p0.z;
    top = p2.z > top ? p2.z : top;
    top = p3.z > top ? p3.z : top;
    p0.z -= top;
    p1.z -= top;
    p2.z -= top;
    p3.z -= top;
    this->Tei_TowardScreen(p0, p1, p2, p3, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    p0 += slot.vecBNow;
    p1 += slot.vecBNow;
    p2 += slot.vecBNow;
    p3 += slot.vecBNow;

    switch (slot.flag1[2]) {
    case 0:
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p0, p1, p2, p3, wtm, 1, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 19:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 205, 255, 205, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 20:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 21:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 1, slot.flag1[0], 1, 225, 255, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }

    radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
    angle = slot.fullDisplayAngle;
    p0.x = radius * g_cosTable[angle];
    p0.y = 0.0f;
    p0.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p1.x = radius * g_cosTable[angle];
    p1.y = 0.0f;
    p1.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 120;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 50;
      if (angle < 0) {
        angle += 360;
      }
    }
    p2.x = radius * g_cosTable[angle];
    p2.y = 0.0f;
    p2.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p3.x = radius * g_cosTable[angle];
    p3.y = 0.0f;
    p3.z = radius * g_sinTable[angle];

    top = p1.z > p0.z ? p1.z : p0.z;
    top = p2.z > top ? p2.z : top;
    top = p3.z > top ? p3.z : top;
    p0.z -= top;
    p1.z -= top;
    p2.z -= top;
    p3.z -= top;
    this->Tei_TowardScreen(p0, p1, p2, p3, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    p0 += slot.vecBNow;
    p1 += slot.vecBNow;
    p2 += slot.vecBNow;
    p3 += slot.vecBNow;

    switch (slot.flag1[2]) {
    case 0:
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p0, p1, p2, p3, wtm, 1, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 19:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 105, 255, 105, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 20:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }
  }
}

// OFFSET: 0x00BF6AD0
// FUNCTION: fn_20000
__declspec(safebuffers) void CEffectPrim::RenderSegmentSpriteGlow(matrix& wtm) {
  CSprRes* sprite = this->GetSprite();
  CActRes* action = this->GetActRes();
  const int opacity = static_cast<int>(this->state.alpha);
  const std::int16_t alpha = static_cast<std::int16_t>(opacity);

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life <= 0 || !(this->state.alpha > 0.0f)) {
      continue;
    }

    for (int seg = 0; seg < this->state.numSegment; ++seg) {
      const vector3d& position = this->state.records[seg].position;
      float xDepth = position.x;
      float yDepth = position.y;
      yDepth *= wtm.values[5];
      xDepth *= wtm.values[2];
      const float depth = yDepth + xDepth + position.z * wtm.values[8] + wtm.values[11];

      float screenX;
      float screenY;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&position.x, wtm.values, &screenX, &screenY, &reciprocalDepth);
      const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

      const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);
      const auto drawGlow = [&](const RenderBounds& bounds, const SpriteFrameCell& cell, const SpriteRleImage* image, std::uint32_t color) {
        SpriteQuad glow;
        float topY = bounds.top;
        glow.top = topY - 1.0f;
        glow.bottom = bounds.bottom + 1.0f;
        glow.left = bounds.left - 1.0f;
        glow.right = bounds.right + 1.0f;
        const float centerY = screenY;
        topY -= centerY;
        glow.bottomRhw = 1.0f / ((bounds.bottom - centerY) * g_tangent + depth) + 0.0001f;
        glow.image = image;
        glow.topRhw = 1.0f / (topY * g_tangent + depth) + 0.0001f;
        glow.color = color;
        glow.angle = static_cast<float>(cell.rotation) + this->state.roll;
        this->DrawQuad(glow, sprite->palette, 5, 0.0f, 0.0f, 0);
      };

      for (int index = 0; index < frame.cellCount; ++index) {
        const SpriteFrameCell& cell = frame.Cell(index);
        if (cell.image == -1) {
          continue;
        }

        const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
        const double distance = distanceScale;
        const float scale = this->state.records[seg].scale;
        const double scaledTop = static_cast<double>(cell.y) * scale * cell.scaleY;
        RenderBounds bounds;
        bounds.top = static_cast<float>(scaledTop * distance + screenY);
        const int imageHeight = (image->doubleHeight + 1) * image->height;
        const auto cellBottom = static_cast<std::int32_t>(static_cast<std::uint32_t>(cell.y) + imageHeight);
        const double scaledBottom = static_cast<double>(cellBottom) * scale * cell.scaleY;
        bounds.bottom = static_cast<float>(scaledBottom * distance + screenY);
        if (bounds.top >= static_cast<float>(g_renderer->viewport.height) || bounds.bottom <= 0.0f) {
          continue;
        }

        const double horizontalScale = scale;
        const double scaleX = cell.scaleX;
        if ((cell.mirrored & 1) != 0) {
          const auto cellRight = static_cast<std::int32_t>((image->doubleWidth + 1) * image->width + static_cast<std::uint32_t>(cell.x));
          bounds.left = static_cast<float>(cellRight * horizontalScale * scaleX * distance + screenX);
          bounds.right = static_cast<float>(cell.x * horizontalScale * scaleX * distance + screenX);
          if (bounds.right >= static_cast<float>(g_renderer->viewport.width) || bounds.left <= 0.0f) {
            continue;
          }
        } else {
          bounds.left = static_cast<float>(cell.x * horizontalScale * scaleX * distance + screenX);
          const auto cellRight = static_cast<std::int32_t>((image->doubleWidth + 1) * image->width + static_cast<std::uint32_t>(cell.x));
          bounds.right = static_cast<float>(cellRight * horizontalScale * scaleX * distance + screenX);
          if (bounds.left >= static_cast<float>(g_renderer->viewport.width) || bounds.right <= 0.0f) {
            continue;
          }
        }

        SpriteQuad quad;
        quad.top = bounds.top;
        quad.bottom = bounds.bottom;
        quad.left = bounds.left;
        quad.right = bounds.right;
        quad.image = image;
        quad.bottomRhw = 1.0f / ((bounds.bottom - screenY) * g_tangent + depth);
        quad.topRhw = 1.0f / ((bounds.top - screenY) * g_tangent + depth);
        quad.angle = static_cast<float>(cell.rotation) + this->state.roll;

        unsigned char red = cell.colorChannels.red;
        unsigned char green = cell.colorChannels.green;
        unsigned char blue = cell.colorChannels.blue;
        this->GetColorOverride(&red, &green, &blue);
        const std::uint32_t alphaChannel = alpha;
        const std::uint32_t color = (((((alphaChannel << 8) + red) << 8) + green) << 8) + blue;
        quad.color = color;
        this->DrawQuad(quad, sprite->palette, this->state.slots[ec].flag1[2] != 1 ? 0x101 : 5, 0.0f, 0.0f, 0);

        drawGlow(bounds, cell, image, color);
      }
    }
  }
}

// OFFSET: 0x00BF6FD0
// FUNCTION: fn_20001
__declspec(safebuffers) void CEffectPrim::RenderTaperedQuad(matrix& wtm) {
  vector3d vec1;
  ZeroVector(vec1);
  vector3d vec2;
  ZeroVector(vec2);
  vector3d vec3;
  ZeroVector(vec3);
  vector3d vec4;
  ZeroVector(vec4);
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    const float dist = this->state.slots[ec].distance;
    const int angle = this->state.slots[ec].fullDisplayAngle;
    const float inner = dist - this->state.slots[ec].rotStart * dist / 50.0f;
    const float cosA = g_cosTable[angle];
    const float sinA = g_sinTable[angle];
    int angle1 = angle - 90;
    if (0 > angle1) {
      angle1 += 360;
    }
    int angle2 = angle1 - 180;
    if (angle2 < 0) {
      angle2 += 360;
    }

    const float innerX = cosA * inner;
    const float innerZ = sinA * inner;
    vec1.x = dist * g_cosTable[angle1] * 0.5f + innerX;
    vec1.y = 0.0f;
    vec1.z = dist * g_sinTable[angle1] * 0.5f + innerZ;
    vec2.x = dist * g_cosTable[angle2] * 0.5f + innerX;
    vec2.y = 0.0f;
    vec2.z = dist * g_sinTable[angle2] * 0.5f + innerZ;

    int angle4 = angle - 90;
    if (angle < 90) {
      angle4 = angle + 270;
    }
    int angle3 = angle4 - 180;
    if (angle3 < 0) {
      angle3 += 360;
    }
    vec4.x = dist * g_cosTable[angle4] * 0.5f + cosA * dist;
    vec4.y = 0.0f;
    vec4.z = dist * g_sinTable[angle4] * 0.5f + sinA * dist;
    vec3.x = dist * g_cosTable[angle3] * 0.5f + cosA * dist;
    vec3.y = 0.0f;
    vec3.z = dist * g_sinTable[angle3] * 0.5f + sinA * dist;
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    vec1 += this->state.slots[ec].vecBNow;
    vec2 += this->state.slots[ec].vecBNow;
    vec3 += this->state.slots[ec].vecBNow;
    vec4 += this->state.slots[ec].vecBNow;
    this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB, vec1, vec2, vec3, vec4, wtm, 0, this->state.slots[ec].flag1[0], 1, 100, 100, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
  }
}
