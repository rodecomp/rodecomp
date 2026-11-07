#include "Contents/Effects/RagEffect.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Render/Primitives/RenderQueues.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Contents/Effects/RagEffectState.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Render/Textures/TextureCache.h"
#include "Contents/MapSkyMgr.h"
#include "Client/Replay.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "Render/SpriteGeometry.h"
#include "Render/SpriteQuad.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24499
int g_overrideColor = 0;

// INLINED
static __forceinline void ZeroVector(vector3d& v) {
  v.x = 0.0f;
  v.y = 0.0f;
  v.z = 0.0f;
}

// INLINED
static __forceinline void TransformPrimitiveVertex(const float* matrix, const vector3d& in, vector3d& out) {
  out.x = in.z * matrix[6] + (in.y * matrix[3] + in.x * matrix[0]) + matrix[9];
  out.y = in.z * matrix[7] + (in.y * matrix[4] + in.x * matrix[1]) + matrix[10];
  out.z = in.z * matrix[8] + (in.y * matrix[5] + in.x * matrix[2]) + matrix[11];
}

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

// OFFSET: 0x00BCD960
// FUNCTION: fn_19865
void CEffectPrim::DrawEffectPrimitive15(matrix& wtm) {
  vector3d vert[4];
  for (vector3d& element : vert) {
    ZeroVector(element);
  }
  vector3d tvert[4];
  for (vector3d& element : tvert) {
    ZeroVector(element);
  }

  vert[0].z = this->state.widthSize;
  vert[2].y = this->state.heightSize;
  vert[1].z = -vert[0].z;
  vert[0].y = -vert[2].y;

  if ((this->state.pattern & 0x80u) != 0) {
    vert[0].y = vert[0].y + vert[0].y;
    vert[2].y = 0.0f;
  }

  if ((this->state.pattern & 8u) != 0) {
    vert[2].y = 0.0f;
  }

  vert[1].y = vert[0].y;
  vert[2].z = vert[0].z;
  vert[3].y = vert[2].y;
  vert[3].z = vert[1].z;

  const float* matrix = this->state.rotTransform;
  TransformPrimitiveVertex(matrix, vert[0], tvert[0]);
  TransformPrimitiveVertex(matrix, vert[1], tvert[1]);
  TransformPrimitiveVertex(matrix, vert[2], tvert[2]);
  TransformPrimitiveVertex(matrix, vert[3], tvert[3]);

  {
    RenderTriangle& triangle = this->state.triangles[0];
    const float firstPosition[3] = {tvert[0].x + this->render.position.x, tvert[0].y + this->render.position.y, tvert[0].z + this->render.position.z};
    RenderVertex first{};
    first.specular = 0xFF000000;
    g_renderer->viewport.Project(firstPosition, wtm.values, first);
    triangle.vertices[0] = first;
    triangle.vertices[0].diffuse = this->state.argb;
    triangle.vertices[0].u = 0.0f;
    triangle.vertices[0].v = this->state.v1;
    const float secondPosition[3] = {tvert[1].x + this->render.position.x, tvert[1].y + this->render.position.y, tvert[1].z + this->render.position.z};
    RenderVertex second{};
    second.specular = 0xFF000000;
    g_renderer->viewport.Project(secondPosition, wtm.values, second);
    triangle.vertices[1] = second;
    triangle.vertices[1].diffuse = this->state.argb;
    triangle.vertices[1].u = 1.0f;
    triangle.vertices[1].v = this->state.v1;
    const float thirdPosition[3] = {tvert[2].x + this->render.position.x, tvert[2].y + this->render.position.y, tvert[2].z + this->render.position.z};
    RenderVertex third{};
    third.specular = 0xFF000000;
    g_renderer->viewport.Project(thirdPosition, wtm.values, third);
    triangle.vertices[2] = third;
    triangle.vertices[2].diffuse = this->state.argb;
    triangle.vertices[2].u = 0.0f;
    triangle.vertices[2].v = this->state.v2;
    triangle.texture = this->state.textures[this->render.curMotion];
  }

  {
    RenderTriangle& triangle = this->state.triangles[1];
    const float firstPosition[3] = {tvert[3].x + this->render.position.x, tvert[3].y + this->render.position.y, tvert[3].z + this->render.position.z};
    RenderVertex first{};
    first.specular = 0xFF000000;
    g_renderer->viewport.Project(firstPosition, wtm.values, first);
    triangle.vertices[0] = first;
    triangle.vertices[0].diffuse = this->state.argb;
    triangle.vertices[0].u = 1.0f;
    triangle.vertices[0].v = this->state.v2;
    const float secondPosition[3] = {tvert[2].x + this->render.position.x, tvert[2].y + this->render.position.y, tvert[2].z + this->render.position.z};
    RenderVertex second{};
    second.specular = 0xFF000000;
    g_renderer->viewport.Project(secondPosition, wtm.values, second);
    triangle.vertices[1] = second;
    triangle.vertices[1].diffuse = this->state.argb;
    triangle.vertices[1].u = 0.0f;
    triangle.vertices[1].v = this->state.v2;
    const float thirdPosition[3] = {tvert[1].x + this->render.position.x, tvert[1].y + this->render.position.y, tvert[1].z + this->render.position.z};
    RenderVertex third{};
    third.specular = 0xFF000000;
    g_renderer->viewport.Project(thirdPosition, wtm.values, third);
    triangle.vertices[2] = third;
    triangle.vertices[2].diffuse = this->state.argb;
    triangle.vertices[2].u = 1.0f;
    triangle.vertices[2].v = this->state.v1;
    triangle.texture = this->state.textures[this->render.curMotion];
  }

  for (int index = 0; index < 2; ++index) {
    RenderTriangle& triangle = this->state.triangles[index];

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
    }
  }
}

// OFFSET: 0x00BCE350
// FUNCTION: fn_19866
void CEffectPrim::Render(int argument) {
  matrix& wtm = *reinterpret_cast<matrix*>(argument);
  if (!g_session.IsEffectRenderEnabled() || !this->IsPartyVisible()) {
    return;
  }

  switch (this->state.count) {
    case 0:
    case 1:
      this->Render2DPrimitive();
      break;
    case 2:
      this->Render2DDiamond();
      break;
    case 3: {
      const float firstCos = ClientCosine(static_cast<int>(this->state.longitude - this->state.arcAngle));
      const float firstSin = ClientSine(static_cast<int>(this->state.longitude - this->state.arcAngle));
      const float secondCos = ClientCosine(static_cast<int>(this->state.longitude + this->state.arcAngle));
      const float secondSin = ClientSine(static_cast<int>(this->state.longitude + this->state.arcAngle));
      RenderTriangle& triangle = this->state.triangles[0];
      const float x = this->render.position.x;
      triangle.vertices[0].x = x;
      const float y = this->render.position.y;
      triangle.vertices[0].y = y;
      triangle.vertices[0].z = 0.000001f;
      triangle.vertices[0].rhw = 0.999999f;
      triangle.vertices[0].diffuse = this->state.argb;
      triangle.vertices[0].u = 0.5f;
      triangle.vertices[0].v = 0.0f;
      const float radius = this->state.heightSize;
      triangle.vertices[1].x = radius * firstCos + x;
      triangle.vertices[1].y = y - radius * firstSin;
      triangle.vertices[2].x = radius * secondCos + x;
      triangle.vertices[2].y = y - radius * secondSin;
      triangle.vertices[1].v = 1.0f;
      triangle.vertices[1].u = 1.0f;
      triangle.vertices[1].diffuse = this->state.argb;
      triangle.vertices[1].rhw = 0.999999f;
      triangle.vertices[1].z = 0.000001f;
      triangle.vertices[2].z = 0.000001f;
      triangle.vertices[2].rhw = 0.999999f;
      triangle.vertices[2].diffuse = this->state.argb;
      triangle.vertices[2].u = 0.0f;
      triangle.vertices[2].v = 1.0f;
      triangle.texture = this->state.textures[0];
      g_renderer->queues.SubmitTriangle(triangle, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      break;
    }
    case 4:
      this->Render2DCircle();
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 21:
    case 40:
    case 46:
    case 51:
    case 84:
    case 85:
    case 124:
      this->RenderSegmentSprites2(wtm);
      break;
    case 167:
      this->RenderSegmentSprites(wtm);
      break;
    case 152:
      this->RenderSegmentSpriteTrail(wtm);
      break;
    case 153:
      this->RenderSegmentSpriteGlow(wtm);
      break;
    case 168:
      this->RenderGroundSprite(wtm);
      break;
    case 9:
    case 117:
      this->RenderPyramid(wtm);
      break;
    case 15:
      this->DrawEffectPrimitive15(wtm);
      break;
    case 22: {
      float x;
      float y;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&this->render.position.x, wtm.values, &x, &y, &reciprocalDepth);
      const float scale = g_renderer->ScaleDistance(reciprocalDepth);
      const float bottom = y - scale * this->state.heightSize;
      const float right = scale * this->state.widthSize + x;
      const std::uint32_t color = this->state.argb;
      RenderTriangle& first = this->state.triangles[0];
      first.vertices[0].x = x;
      first.vertices[0].y = y;
      first.vertices[0].z = 0.8f;
      first.vertices[0].rhw = 1.0f;
      first.vertices[0].diffuse = color;
      first.vertices[0].u = 0.0f;
      first.vertices[0].v = 0.0f;
      first.vertices[1].x = right;
      first.vertices[1].y = y;
      first.vertices[1].z = 0.8f;
      first.vertices[1].rhw = 1.0f;
      first.vertices[1].diffuse = color;
      first.vertices[1].u = 1.0f;
      first.vertices[1].v = 0.0f;
      first.vertices[2].x = x;
      first.vertices[2].y = bottom;
      first.vertices[2].z = 0.8f;
      first.vertices[2].rhw = 1.0f;
      first.vertices[2].diffuse = color;
      first.vertices[2].u = 0.0f;
      first.vertices[2].v = 1.0f;
      first.texture = this->state.textures[this->render.curMotion];
      RenderTriangle& second = this->state.triangles[1];
      second.vertices[0].x = right;
      second.vertices[0].y = y;
      second.vertices[0].z = 0.8f;
      second.vertices[0].rhw = 1.0f;
      second.vertices[0].diffuse = color;
      second.vertices[0].u = 1.0f;
      second.vertices[0].v = 0.0f;
      second.vertices[1].x = x;
      second.vertices[1].y = bottom;
      second.vertices[1].z = 0.8f;
      second.vertices[1].rhw = 1.0f;
      second.vertices[1].diffuse = color;
      second.vertices[1].u = 0.0f;
      second.vertices[1].v = 1.0f;
      second.vertices[2].x = right;
      second.vertices[2].y = bottom;
      second.vertices[2].z = 0.8f;
      second.vertices[2].rhw = 1.0f;
      second.vertices[2].diffuse = color;
      second.vertices[2].u = 1.0f;
      second.vertices[2].v = 1.0f;
      second.texture = this->state.textures[this->render.curMotion];
      g_renderer->queues.SubmitTriangle(first, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      g_renderer->queues.SubmitTriangle(second, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      break;
    }
    case 16:
      this->RenderCrossPlanes(wtm);
      break;
    case 12:
      this->RenderRecordRings(wtm);
      break;
    case 13:
      this->RenderArcBand(wtm);
      break;
    case 104:
      this->RenderHalfSphere(wtm);
      break;
    case 146:
      this->RenderTrailingArc(wtm);
      break;
    case 144:
      this->RenderGroundArcStep(wtm);
      break;
    case 149:
      this->RenderGroundArcGlow(wtm);
      break;
    case 158:
      this->RenderGroundArc(wtm);
      break;
    case 173:
      this->RenderGroundArc2(wtm);
      break;
    case 137:
      this->RenderArcWalls(wtm);
      break;
    case 121:
      this->RenderSMA2(wtm);
      break;
    case 73:
      this->RenderMagnum2(wtm);
      break;
    case 61:
      this->RenderSphereCage(wtm);
      break;
    case 105:
      this->RenderCone(wtm);
      break;
    case 106:
      this->RenderAttackEnergy2(wtm);
      break;
    case 76:
      this->RenderSaintWing(wtm);
      break;
    case 77:
    case 91:
      this->RenderSphereWind(wtm);
      break;
    case 78:
      this->RenderColorPaper(wtm);
      break;
    case 62:
      this->RenderRiseArc(wtm);
      break;
    case 26:
    case 28:
    case 39:
    case 67:
    case 88:
    case 161:
      this->RenderSizedSquare(wtm);
      break;
    case 143:
      this->RenderGroundSquare(wtm);
      break;
    case 148:
      this->RenderGroundSquareBlend(wtm);
      break;
    case 114:
      this->RenderHermode(wtm);
      break;
    case 68:
    case 147:
      this->RenderHaloLayers(wtm);
      break;
    case 131:
      this->RenderRipple(wtm);
      break;
    case 70:
      this->RenderHitLine(wtm);
      break;
    case 100:
      this->RenderHitLine3(wtm);
      break;
    case 103:
      this->RenderStackedRings(wtm);
      break;
    case 101:
      this->RenderLightningArc(wtm);
      break;
    case 135:
      this->RenderJitterSweeps(wtm);
      break;
    case 102:
      this->RenderLightningTube(wtm);
      break;
    case 27:
      this->RenderGroundBubbles(wtm);
      break;
    case 29:
    case 80:
      this->RenderGroundFog(wtm);
      break;
    case 150:
      this->RenderHoverSquares(wtm);
      break;
    case 130:
      this->RenderFacingQuads(wtm);
      break;
    case 111:
    case 115:
      this->RenderSoulParticle(wtm);
      break;
    case 155:
    case 170:
      this->RenderTwinGlowSquares(wtm);
      break;
    case 157:
      this->RenderGlowSquare(wtm);
      break;
    case 109:
      this->RenderSoulLink(wtm);
      break;
    case 151:
      this->RenderLaunchEffects(wtm);
      break;
    case 96:
      this->RenderPeongUp(wtm);
      break;
    case 94:
      this->RenderPeong(wtm);
      break;
    case 35: {
      this->RenderOrbitIcons(wtm, 1.0f, 100, 100, 255);
      break;
    }
    case 99:
      this->RenderDoubleGumGang(wtm);
      break;
    case 23:
    case 24:
    case 25:
    case 30:
    case 31:
    case 41:
    case 50:
    case 57:
    case 60:
    case 63:
    case 65:
    case 107:
    case 108:
    case 113:
    case 118:
    case 139:
      this->Render3DCasting(wtm);
      break;
    case 36:
    case 110:
    case 169:
      this->Render3DCasting_LowPolygon(wtm);
      break;
    case 34:
      this->Render3DCasting2(wtm);
      break;
    case 37:
      this->RenderGrandCross2(wtm);
      break;
    case 75:
      this->RenderSlash1(wtm);
      break;
    case 64:
      this->RenderRectUp(wtm);
      break;
    case 90:
      this->RenderRectUp2(wtm);
      break;
    case 132:
      this->RenderColorPanel(wtm);
      break;
    case 43:
      this->RenderOrbitSquare(wtm);
      break;
    case 44:
      this->RenderRotatedPlate(wtm);
      break;
    case 86:
    case 92:
      this->RenderFloorSquare(wtm);
      break;
    case 119:
    case 122:
    case 125:
      this->RenderFloorSquare2(wtm);
      break;
    case 126:
      this->RenderRotatedRing(wtm);
      break;
    case 120:
      this->RenderSMA3(wtm);
      break;
    case 98:
      this->RenderRing(wtm);
      break;
    case 32:
      this->RenderLayeredGlow(wtm);
      break;
    case 145:
      this->RenderPillarSquares(wtm);
      break;
    case 138:
      this->RenderTwinFacingSquares(wtm);
      break;
    case 33:
    case 42:
    case 45:
    case 47:
    case 49:
    case 53:
    case 58:
    case 59:
    case 87:
    case 97:
    case 116:
    case 123:
    case 127:
      this->RenderFieldPanel(wtm);
      break;
    case 156:
      this->RenderJitterQuads(wtm);
      break;
    case 136:
      this->RenderSpinSquares(wtm);
      break;
    case 82:
    case 95:
    case 128:
    case 140:
      this->RenderTeiFacingSlots(wtm);
      break;
    case 160:
      this->RenderPolygonFrames(wtm);
      break;
    case 141:
      this->RenderTaperedQuad(wtm);
      break;
    case 133:
      this->RenderSpiralBlades(wtm);
      break;
    case 134:
      this->RenderBladeTrail(wtm);
      break;
    case 54:
      this->RenderChemical(wtm);
      break;
    case 89:
      this->RenderFanBeam(wtm);
      break;
    case 159:
      this->RenderBladeQuads(wtm);
      break;
    case 162:
      this->RenderFanSlice(wtm);
      break;
    case 163:
      this->RenderFanWedge(wtm);
      break;
    case 164:
      this->RenderFanTrail(wtm);
      break;
    case 165:
      this->RenderCurvedFanTrail(wtm);
      break;
    case 166:
      this->RenderFoldingRibbon(wtm);
      break;
    case 55:
      this->RenderFloorRect(wtm);
      break;
    case 56:
      this->RenderGroundBar(wtm);
      break;
    case 154:
      this->RenderBladeFins(wtm);
      break;
    case 112:
      this->RenderMagicTarget(wtm);
      break;
    case 74:
      this->RenderHalfSphere2(wtm);
      break;
    case 71:
      this->RenderBlind(wtm);
      break;
    case 72:
      this->RenderGrid(wtm);
      break;
    case 172: {
      int rpCnt = 0;
      EffectPrimitiveSlot& slot = this->state.slots[0];
      if (slot.life == 1 && slot.alphaB > 0) {
        const short alpha = slot.alphaB;
        const unsigned char red = static_cast<unsigned char>(this->state.argb);
        const unsigned char green = static_cast<unsigned char>(this->state.argb >> 8);
        const unsigned char blue = static_cast<unsigned char>(this->state.argb >> 16);
        vector3d corners[4];
        for (int index = 0; index < 4; ++index) {
          corners[index].x = 0.0f;
          corners[index].y = 0.0f;
          corners[index].z = 0.0f;
        }
        corners[0].x = -200.0f;
        corners[0].y = -10.0f;
        corners[0].z = -200.0f;
        corners[1].x = 200.0f;
        corners[1].y = -10.0f;
        corners[1].z = -200.0f;
        corners[2].x = 200.0f;
        corners[2].y = -10.0f;
        corners[2].z = 200.0f;
        corners[3].x = -200.0f;
        corners[3].y = -10.0f;
        corners[3].z = 200.0f;
        this->Tei_TowardScreen(corners[0], corners[1], corners[2], corners[3], slot.alphaT, this->state.slots[1].alphaT);
        this->RenderTeiRect(&rpCnt, alpha, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
      break;
    }
    case 52:
      this->RenderTrailSquares(wtm);
      break;
    case 38:
      this->RenderTube(wtm);
      break;
    case 69:
      this->RenderPentaRing(wtm);
      break;
    case 66:
      this->RenderPentagon(wtm);
      break;
    case 129: {
      vector3d prevB{};
      vector3d curB{};
      vector3d curT{};
      vector3d prevT{};
      int rpCnt = 0;
      for (int ec = 0; ec < 4; ++ec) {
        EffectPrimitiveSlot& slot = this->state.slots[ec];
        if (slot.life != 1 || slot.alphaB <= 0) {
          continue;
        }
        const float radius = slot.maxHeight;
        const float outerRadius = radius * 4.0f;
        for (int i = 0; i <= 10; ++i) {
          std::int16_t angle = static_cast<std::int16_t>(slot.rotStart + 36 * i);
          if (angle >= 360) {
            angle -= 360;
          }
          curB.x = g_cosTable[angle] * outerRadius + slot.vecTNow.x;
          curB.y = slot.vecTNow.y;
          curB.z = outerRadius * g_sinTable[angle] + slot.vecTNow.z;
          curT.x = g_cosTable[angle] * radius + slot.vecTPre.x;
          curT.y = slot.vecTPre.y;
          curT.z = radius * g_sinTable[angle] + slot.vecTPre.z;
          if (i != 0) {
            this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 0, 0, 1, 230, 255, 230, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          }
          prevB = curB;
          prevT = curT;
        }
      }
      break;
    }
    case 79:
      this->RenderScrollColumn(wtm);
      break;
    case 142:
      this->RenderStandingPanels(wtm);
      break;
    case 93:
      this->RenderRainbow(wtm);
      break;
    case 81:
      this->RenderBeam(wtm);
      break;
    case 48:
      this->RenderColorPlane(wtm);
      break;
    case 10:
    case 11:
      this->RenderRecordQuads(wtm);
      break;
    case 14:
      this->RenderSphere(wtm);
      break;
    case 19: {
      const float firstCos = ClientCosine(static_cast<int>(this->state.longitude - this->state.arcAngle));
      const float firstSin = ClientSine(static_cast<int>(this->state.longitude - this->state.arcAngle));
      const float secondCos = ClientCosine(static_cast<int>(this->state.longitude + this->state.arcAngle));
      const float secondSin = ClientSine(static_cast<int>(this->state.longitude + this->state.arcAngle));
      RenderTriangle& triangle = this->state.triangles[0];
      const float x = this->render.position.x;
      triangle.vertices[0].x = x;
      const float y = this->render.position.y;
      triangle.vertices[0].y = y;
      triangle.vertices[0].z = 0.000001f;
      triangle.vertices[0].rhw = 0.999999f;
      triangle.vertices[0].diffuse = this->state.argb;
      triangle.vertices[0].u = 0.5f;
      triangle.vertices[0].v = 0.0f;
      const float radius = this->state.heightSize;
      triangle.vertices[1].x = radius * firstCos + x;
      triangle.vertices[1].y = y - radius * firstSin;
      triangle.vertices[2].x = radius * secondCos + x;
      triangle.vertices[2].y = y - radius * secondSin;
      triangle.vertices[1].v = 1.0f;
      triangle.vertices[1].u = 1.0f;
      triangle.vertices[1].diffuse = this->state.argb;
      triangle.vertices[1].rhw = 0.999999f;
      triangle.vertices[1].z = 0.000001f;
      triangle.vertices[2].z = 0.000001f;
      triangle.vertices[2].rhw = 0.999999f;
      triangle.vertices[2].diffuse = this->state.argb;
      triangle.vertices[2].u = 0.0f;
      triangle.vertices[2].v = 1.0f;
      triangle.texture = this->state.textures[0];
      g_renderer->queues.SubmitTriangle(triangle, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      break;
    }
    case 20: {
      float position[3] = {this->render.position.x, this->render.position.y, this->render.position.z};
      RenderVertex top{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(position, wtm.values, top);
      position[1] -= 10.0f;
      RenderVertex bottom{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(position, wtm.values, bottom);
      RenderTriangle& triangle = this->state.triangles[0];
      triangle.vertices[0].x = top.x;
      triangle.vertices[2].x = bottom.x;
      triangle.vertices[2].y = bottom.y;
      triangle.vertices[2].z = bottom.z;
      triangle.vertices[0].y = top.y;
      triangle.vertices[0].z = top.z;
      triangle.vertices[0].rhw = 0.999999f;
      triangle.vertices[0].diffuse = 0x9CFFFFFFu;
      triangle.vertices[0].u = 0.0f;
      triangle.vertices[0].v = 0.0f;
      triangle.vertices[1].x = top.x + 1.0f;
      triangle.vertices[1].y = top.y;
      triangle.vertices[1].z = top.z;
      triangle.vertices[1].rhw = 0.999999f;
      triangle.vertices[1].diffuse = 0x9BFFFFFFu;
      triangle.vertices[1].u = 0.0f;
      triangle.vertices[1].v = 0.0f;
      triangle.vertices[2].rhw = 0.999999f;
      triangle.vertices[2].diffuse = 0x9BFFFFFFu;
      triangle.vertices[2].u = 0.0f;
      triangle.vertices[2].v = 0.0f;
      triangle.texture = nullptr;
      g_renderer->queues.SubmitTriangle(triangle, 5, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      break;
    }
    case 171:
      this->RenderGlowDisc(wtm);
      break;
  }
}

// OFFSET: 0x00BE4210
// FUNCTION: fn_19923
__declspec(safebuffers) void CEffectPrim::RenderTeiRect(
  int* rpCnt, short alpha, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, short pw, char tn, char f1, unsigned char r,
  unsigned char g, unsigned char b, float ty1, float ty2, float tx, float tx2, char extra
) {
  const RagEffectState& master = this->state.master->state;
  const int blend = pw != 0 ? 6 : 2;
  int red = r;
  if (master.unknown_11C88[0] != -1) {
    red = static_cast<unsigned char>(master.unknown_11C88[0]);
  }
  int green = g;
  if (master.unknown_11C88[1] != -1) {
    green = static_cast<unsigned char>(master.unknown_11C88[1]);
  }
  int blue = b;
  if (master.unknown_11C88[2] != -1) {
    blue = static_cast<unsigned char>(master.unknown_11C88[2]);
  }
  int destBlend = master.unknown_11C88[3];
  if (destBlend == -1) {
    destBlend = blend;
  }

  if (*rpCnt >= 510) {
    return;
  }

  const std::uint32_t color = static_cast<std::uint32_t>(((((alpha << 8) + static_cast<unsigned char>(red)) << 8) + static_cast<unsigned char>(green)) << 8) +
                              static_cast<unsigned char>(blue);

  if (f1 == 0) {
    const vector3d& position = this->render.position;
    vec1.x += position.x;
    vec1.y += position.y;
    vec1.z += position.z;
    vec2.x += position.x;
    vec2.y += position.y;
    vec2.z += position.z;
    vec3.x += position.x;
    vec3.y += position.y;
    vec3.z += position.z;
    vec4.x += position.x;
    vec4.y += position.y;
    vec4.z += position.z;
  }

  {
    RenderTriangle& triangle = this->state.triangles[*rpCnt];

    if (pw == 10) {
      triangle.unknown_0064 = 2;
      triangle.unknown_0068 = 1;
    } else {
      triangle.unknown_0064 = 5;
      triangle.unknown_0068 = pw == 99 ? 6 : destBlend;
    }

    RenderVertex first{};
    first.specular = 0xFF000000;
    g_renderer->viewport.Project(&vec1.x, wtm.values, first);
    triangle.vertices[0] = first;
    triangle.vertices[0].diffuse = color;
    triangle.vertices[0].u = tx;
    triangle.vertices[0].v = ty2;
    RenderVertex second{};
    second.specular = 0xFF000000;
    g_renderer->viewport.Project(&vec2.x, wtm.values, second);
    triangle.vertices[1] = second;
    triangle.vertices[1].diffuse = color;
    triangle.vertices[1].u = tx2;
    triangle.vertices[1].v = ty2;
    RenderVertex third{};
    third.specular = 0xFF000000;
    g_renderer->viewport.Project(&vec3.x, wtm.values, third);
    triangle.vertices[2] = third;
    triangle.vertices[2].diffuse = color;
    triangle.vertices[2].u = tx2;
    triangle.vertices[2].v = ty1;
    triangle.texture = this->state.textures[tn];

    if (extra == 2) {
      for (int index = 0; index < 3; ++index) {
        RenderVertex& vertex = triangle.vertices[index];
        vertex.rhw += 0.000045f;
        vertex.z = RhwToDepth(vertex.rhw);
      }
    } else if (extra == 1 || extra == 3) {
      if (extra == 3) {
        const RenderVertex center = TransformVert(wtm, this->render.position);
        const float width = std::fabs(triangle.vertices[0].x - triangle.vertices[1].x) * 0.5f;
        triangle.vertices[0].x = center.x + width;
        triangle.vertices[1].x = center.x - width;
        triangle.vertices[2].x = center.x - width;

        for (int index = 0; index < 3; ++index) {
          triangle.vertices[index].y += static_cast<float>(-30 * g_renderer->viewport.height / 480) * this->render.unknown_005C;
        }
      }

      for (int index = 0; index < 3; ++index) {
        triangle.vertices[index].z = 1e-09f;
        triangle.vertices[index].rhw = 1.0f;
      }
    }

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }

  {
    RenderTriangle& triangle = this->state.triangles[*rpCnt];

    if (pw == 10) {
      triangle.unknown_0064 = 2;
      triangle.unknown_0068 = 1;
    } else {
      triangle.unknown_0064 = 5;
      triangle.unknown_0068 = pw == 99 ? 6 : destBlend;
    }

    RenderVertex first{};
    first.specular = 0xFF000000;
    g_renderer->viewport.Project(&vec4.x, wtm.values, first);
    triangle.vertices[0] = first;
    triangle.vertices[0].diffuse = color;
    triangle.vertices[0].u = tx;
    triangle.vertices[0].v = ty1;
    RenderVertex second{};
    second.specular = 0xFF000000;
    g_renderer->viewport.Project(&vec3.x, wtm.values, second);
    triangle.vertices[1] = second;
    triangle.vertices[1].diffuse = color;
    triangle.vertices[1].u = tx2;
    triangle.vertices[1].v = ty1;
    RenderVertex third{};
    third.specular = 0xFF000000;
    g_renderer->viewport.Project(&vec1.x, wtm.values, third);
    triangle.vertices[2] = third;
    triangle.vertices[2].diffuse = color;
    triangle.vertices[2].u = tx;
    triangle.vertices[2].v = ty2;
    triangle.texture = this->state.textures[tn];

    if (extra == 2) {
      for (int index = 0; index < 3; ++index) {
        RenderVertex& vertex = triangle.vertices[index];
        vertex.rhw += 0.000045f;
        vertex.z = RhwToDepth(vertex.rhw);
      }
    } else if (extra == 1 || extra == 3) {
      if (extra == 3) {
        const RenderVertex center = TransformVert(wtm, this->render.position);
        const float width = std::fabs(triangle.vertices[0].x - triangle.vertices[1].x) * 0.5f;
        triangle.vertices[0].x = center.x + width;
        triangle.vertices[1].x = center.x - width;
        triangle.vertices[2].x = center.x + width;

        for (int index = 0; index < 3; ++index) {
          triangle.vertices[index].y += static_cast<float>(-30 * g_renderer->viewport.height / 480) * this->render.unknown_005C;
        }
      }

      for (int index = 0; index < 3; ++index) {
        triangle.vertices[index].z = 1e-09f;
        triangle.vertices[index].rhw = 1.0f;
      }
    }

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }
}

// OFFSET: 0x00BE4C60
// FUNCTION: fn_19924
void CEffectPrim::RenderTeiRect(
  int* rpCnt, int texPart, short alphaB, short alphaT, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, char pw
) {
  if (*rpCnt >= 510) {
    return;
  }

  int alpha = alphaB;
  int blend = 2;
  int red = 255;
  int green = 255;
  int blue = 255;
  float textureV = 1.0f;

  switch (static_cast<int>(this->state.size)) {
  case 0:
    blend = 6;
    break;
  case 1:
    green = 175;
    blue = 175;
    break;
  case 2:
    red = 195;
    green = 195;
    blend = 6;
    break;
  case 3:
    alpha = 80;
    blend = 6;
    break;
  case 4:
    red = 100;
    green = 100;
    break;
  case 6:
    green = 100;
    blue = 100;
    break;
  case 7:
    red = 100;
    blue = 100;
    break;
  case 8:
    blue = 170;
    break;
  case 9:
    red = 140;
    green = 210;
    blue = 140;
    break;
  case 10:
    green = 89;
    blue = 182;
    break;
  case 11:
    red = 170;
    green = 170;
    break;
  case 12:
    red = 50;
    green = 50;
    blue = 50;
    blend = 6;
    break;
  case 13:
    green = 50;
    blend = 6;
    break;
  case 14:
    red = 120;
    green = 120;
    blend = 6;
    break;
  case 15:
    red = 10;
    blue = 10;
    break;
  case 16:
    red = 20;
    blue = 20;
    break;
  case 17:
    red = 0;
    green = 0;
    break;
  case 18:
    green = 0;
    blue = 0;
    break;
  case 19:
    red = 0;
    blue = 0;
    break;
  case 20:
    blue = 0;
    break;
  case 21:
    green = 0;
    break;
  case 22:
    red = 0;
    break;
  case 23:
    red = 41;
    green = 94;
    blue = 188;
    blend = 6;
    break;
  case 24:
    red = 10;
    green = 66;
    blue = 114;
    break;
  case 25:
    red = 35;
    green = 146;
    blue = 185;
    break;
  case 26:
    red = 164;
    green = 205;
    blue = 254;
    break;
  case 27:
    red = 84;
    green = 169;
    blue = 205;
    break;
  case 28:
    red = 108;
    green = 26;
    break;
  case 29:
    red = 177;
    blue = 58;
    break;
  case 30:
    red = 85;
    green = 30;
    blue = 185;
    break;
  case 31:
    red = 244;
    green = 187;
    blue = 124;
    break;
  case 32:
    red = 0;
    green = 0;
    blue = 0;
    blend = 6;
    break;
  case 40:
    red = 70;
    green = 70;
    break;
  case 41:
    red = 30;
    green = 30;
    break;
  case 42:
    red = 43;
    green = 40;
    blue = 46;
    blend = 6;
    break;
  case 43:
    red = 125;
    green = 182;
    blue = 160;
    blend = 6;
    break;
  case 44:
    red = 180;
    green = 55;
    blue = 30;
    blend = 6;
    break;
  case 45:
    red = 65;
    green = 141;
    blue = 231;
    break;
  case 46:
    red = 164;
    green = 47;
    blue = 29;
    break;
  case 47:
    red = 172;
    green = 154;
    blue = 56;
    break;
  case 48:
    red = 80;
    green = 20;
    blue = 20;
    break;
  case 49:
    red = 200;
    green = 130;
    blue = 84;
    break;
  case 50:
    green = 198;
    blue = 91;
    break;
  case 100:
    red = 250;
    green = 250;
    textureV = 0.9f;
    blend = 6;
    break;
  default:
    break;
  }

  const RagEffectState& master = this->state.master->state;
  if (master.unknown_11C88[3] != -1) {
    blend = master.unknown_11C88[3];
  }
  if (master.unknown_11C88[0] != -1) {
    red = master.unknown_11C88[0];
  }
  if (master.unknown_11C88[1] != -1) {
    green = master.unknown_11C88[1];
  }
  if (master.unknown_11C88[2] != -1) {
    blue = master.unknown_11C88[2];
  }
  const std::uint32_t color = static_cast<std::uint32_t>(((((alpha << 8) + red) << 8) + green) << 8) + blue;

  float left;
  float right;
  if (texPart < 100) {
    left = static_cast<float>(texPart) / 21.0f;
    if (texPart == 0) {
      left = 0.0f;
    }
    right = static_cast<float>(texPart + 1) / 21.0f;
  } else if (texPart == 100) {
    left = 0.35f;
    right = 0.45f;
  } else if (texPart == 101) {
    left = 0.45f;
    right = 0.55f;
  } else {
    left = 0.55f;
    right = 0.65f;
  }

  const vector3d& position = this->render.position;
  vec1.x += position.x;
  vec1.y += position.y;
  vec1.z += position.z;
  vec2.x += position.x;
  vec2.y += position.y;
  vec2.z += position.z;
  vec3.x += position.x;
  vec3.y += position.y;
  vec3.z += position.z;
  vec4.x += position.x;
  vec4.y += position.y;
  vec4.z += position.z;

  const vector3d* const corners[2][3] = {{&vec1, &vec2, &vec3}, {&vec4, &vec3, &vec1}};
  const float us[2][3] = {{left, right, right}, {left, right, left}};
  const float vs[2][3] = {{textureV, textureV, 0.0f}, {0.0f, 0.0f, textureV}};

  for (int half = 0; half < 2; ++half) {
    RenderTriangle& triangle = this->state.triangles[*rpCnt];
    triangle.unknown_0064 = 5;
    triangle.unknown_0068 = blend;

    for (int index = 0; index < 3; ++index) {
      RenderVertex& vertex = triangle.vertices[index];
      vertex = TransformVert(wtm, *corners[half][index]);
      vertex.diffuse = color;
      vertex.u = us[half][index];
      vertex.v = vs[half][index];
    }

    triangle.texture = this->state.textures[pw];

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }
}

// OFFSET: 0x00BE5640
// FUNCTION: fn_19925
__declspec(safebuffers) void CEffectPrim::RenderTeiRect(
  int* rpCnt, int texPart, short alphaB, short alphaT, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, char pw, unsigned char r,
  unsigned char g, unsigned char b
) {
  const RagEffectState& master = this->state.master->state;
  const int blend = pw != 0 ? 6 : 2;
  const int red = master.unknown_11C88[0] != -1 ? static_cast<unsigned char>(master.unknown_11C88[0]) : r;
  const int green = master.unknown_11C88[1] != -1 ? static_cast<unsigned char>(master.unknown_11C88[1]) : g;
  const int blue = master.unknown_11C88[2] != -1 ? static_cast<unsigned char>(master.unknown_11C88[2]) : b;
  const int destBlend = master.unknown_11C88[3] != -1 ? master.unknown_11C88[3] : blend;

  if (*rpCnt >= 510) {
    return;
  }

  const std::uint32_t color = static_cast<std::uint32_t>(((((alphaB << 8) + static_cast<unsigned char>(red)) << 8) + static_cast<unsigned char>(green)) << 8) +
                              static_cast<unsigned char>(blue);
  float top = static_cast<float>(texPart) / 21.0f;
  if (texPart == 0) {
    top = 0.0f;
  }
  const float bottom = static_cast<float>(texPart + 1) / 21.0f;

  const vector3d& position = this->render.position;
  vec1.x += position.x;
  vec1.y += position.y;
  vec1.z += position.z;
  vec2.x += position.x;
  vec2.y += position.y;
  vec2.z += position.z;
  vec3.x += position.x;
  vec3.y += position.y;
  vec3.z += position.z;
  vec4.x += position.x;
  vec4.y += position.y;
  vec4.z += position.z;

  alignas(16) RenderVertex vertex;
  this->state.triangles[*rpCnt].unknown_0064 = 5;
  this->state.triangles[*rpCnt].unknown_0068 = destBlend;
  vertex = {0.0f, 0.0f, 0.0f, 0.0f, 0, 0xFF000000, 0.0f, 0.0f};
  g_renderer->viewport.Project(&vec1.x, wtm.values, vertex);
  this->state.triangles[*rpCnt].vertices[0] = vertex;
  this->state.triangles[*rpCnt].vertices[0].diffuse = color;
  this->state.triangles[*rpCnt].vertices[0].u = 1.0f;
  this->state.triangles[*rpCnt].vertices[0].v = top;
  vertex = {0.0f, 0.0f, 0.0f, 0.0f, 0, 0xFF000000, 0.0f, 0.0f};
  g_renderer->viewport.Project(&vec2.x, wtm.values, vertex);
  this->state.triangles[*rpCnt].vertices[1] = vertex;
  this->state.triangles[*rpCnt].vertices[1].diffuse = color;
  this->state.triangles[*rpCnt].vertices[1].u = 1.0f;
  this->state.triangles[*rpCnt].vertices[1].v = bottom;
  vertex = {0.0f, 0.0f, 0.0f, 0.0f, 0, 0xFF000000, 0.0f, 0.0f};
  g_renderer->viewport.Project(&vec3.x, wtm.values, vertex);
  this->state.triangles[*rpCnt].vertices[2] = vertex;
  this->state.triangles[*rpCnt].vertices[2].diffuse = color;
  this->state.triangles[*rpCnt].vertices[2].u = 0.0f;
  this->state.triangles[*rpCnt].vertices[2].v = bottom;
  this->state.triangles[*rpCnt].texture = this->state.textures[0];

  if (this->state.triangles[*rpCnt].Visible()) {
    g_renderer->queues.SubmitTriangle(
      this->state.triangles[*rpCnt], static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
    ++*rpCnt;
  }

  this->state.triangles[*rpCnt].unknown_0064 = 5;
  this->state.triangles[*rpCnt].unknown_0068 = destBlend;
  vertex = {0.0f, 0.0f, 0.0f, 0.0f, 0, 0xFF000000, 0.0f, 0.0f};
  g_renderer->viewport.Project(&vec4.x, wtm.values, vertex);
  this->state.triangles[*rpCnt].vertices[0] = vertex;
  this->state.triangles[*rpCnt].vertices[0].diffuse = color;
  this->state.triangles[*rpCnt].vertices[0].u = 0.0f;
  this->state.triangles[*rpCnt].vertices[0].v = top;
  vertex = {0.0f, 0.0f, 0.0f, 0.0f, 0, 0xFF000000, 0.0f, 0.0f};
  g_renderer->viewport.Project(&vec3.x, wtm.values, vertex);
  this->state.triangles[*rpCnt].vertices[1] = vertex;
  this->state.triangles[*rpCnt].vertices[1].diffuse = color;
  this->state.triangles[*rpCnt].vertices[1].u = 0.0f;
  this->state.triangles[*rpCnt].vertices[1].v = bottom;
  vertex = {0.0f, 0.0f, 0.0f, 0.0f, 0, 0xFF000000, 0.0f, 0.0f};
  g_renderer->viewport.Project(&vec1.x, wtm.values, vertex);
  this->state.triangles[*rpCnt].vertices[2] = vertex;
  this->state.triangles[*rpCnt].vertices[2].diffuse = color;
  this->state.triangles[*rpCnt].vertices[2].u = 1.0f;
  this->state.triangles[*rpCnt].vertices[2].v = top;
  this->state.triangles[*rpCnt].texture = this->state.textures[0];

  if (this->state.triangles[*rpCnt].Visible()) {
    g_renderer->queues.SubmitTriangle(
      this->state.triangles[*rpCnt], static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
    ++*rpCnt;
  }
}

// OFFSET: 0x00BE5BB0
// FUNCTION: fn_19926
void CEffectPrim::RenderPentaRing(matrix& wtm) {
  vector3d prevB;
  ZeroVector(prevB);
  vector3d prevT;
  ZeroVector(prevT);
  vector3d curB;
  ZeroVector(curB);
  vector3d curT;
  ZeroVector(curT);
  int rpCnt = 0;
  float radius;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const short direction = static_cast<short>((slot.rotStart + 90) % 360);
    for (int pass = 0; pass < 2; ++pass) {
      int i = 0;
      for (int angle = 0; angle <= 360; angle += 72) {
        if (pass == 0) {
          radius = slot.maxHeight;
        } else if (pass == 1) {
          if (slot.height[4] == 3.0f) {
            radius = slot.maxHeight + slot.maxHeight;
          } else {
            radius = slot.maxHeight + 0.25f;
          }
        }

        const float across = radius * g_cosTable[angle];
        const float offsetX = across * g_cosTable[direction];
        const float offsetZ = across * g_sinTable[direction];
        const float offsetY = radius * g_sinTable[angle] - slot.height[0];
        curB.x = offsetX + slot.vecBNow.x;
        curT.x = offsetX + slot.vecTNow.x;
        curB.y = offsetY + slot.vecBNow.y;
        curT.y = offsetY + slot.vecTNow.y;
        curB.z = offsetZ + slot.vecBNow.z;
        curT.z = offsetZ + slot.vecTNow.z;

        if (i > 0) {
          if (pass == 0) {
            if (slot.height[4] == 2.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 1, 0, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.height[4] == 3.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 1, 0, 1, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.height[4] == 4.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 1, 0, 1, 55, 55, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
            } else if (slot.height[4] == 5.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 1, 0, 1, 55, 55, 55, 0.0f, 1.0f, 0.0f, 1.0f, 1);
            } else {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 1, 0, 1, 255, 255, 55, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          } else if (pass == 1) {
            if (slot.height[4] == 2.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 0, 0, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.height[4] == 3.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 0, 0, 1, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.height[4] == 4.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 0, 0, 1, 55, 55, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
            } else if (slot.height[4] == 5.0f) {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 0, 0, 1, 55, 55, 55, 0.0f, 1.0f, 0.0f, 1.0f, 1);
            } else {
              this->RenderTeiRect(&rpCnt, slot.alphaB, prevB, curB, curT, prevT, wtm, 0, 0, 1, 255, 255, 200, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }
        }

        prevB = curB;
        prevT = curT;
        ++i;
      }
    }
  }
}

// OFFSET: 0x00BE60C0
// FUNCTION: fn_19927
void CEffectPrim::RenderScrollColumn(matrix& wtm) {
  ZeroVector3d vec4;
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    int frames = static_cast<int>(slot.maxHeight);
    int cycle;
    float scroll;
    if (frames == 0) {
      cycle = 0;
      scroll = 40.0f;
      frames = 1;
    } else {
      cycle = slot.process % (frames * 3);
      scroll = static_cast<float>(slot.process % frames) * 40.0f / static_cast<float>(frames);
    }

    if (slot.flag1[1] == 1 || slot.flag1[1] == 10) {
      scroll = 40.0f - scroll;
    }

    for (int i = 0; i < 5; ++i) {
      if (frames == 1 && i > 0) {
        break;
      }

      float top = scroll - slot.height[i];
      float bottom = scroll - slot.height[i + 1];
      const float left = -slot.distance * g_depthAverageHalf;
      const float right = slot.distance * g_depthAverageHalf;
      const float depth = slot.vecBNow.z;
      vec4.Set(left, top, depth);
      vec1.Set(left, bottom, depth);
      vec2.Set(right, bottom, depth);
      vec3.Set(right, top, depth);

      if (slot.flag1[2] > 10) {
        top = top * 10.0f / static_cast<float>(slot.flag1[2]);
        bottom = bottom * 10.0f / static_cast<float>(slot.flag1[2]);
        vec4.y = top;
        vec1.y = bottom;
        vec2.y = bottom;
        vec3.y = top;
      }

      if (slot.flag1[0] == 1) {
        vec4.x = depth;
        vec4.z = left;
        vec1.x = depth;
        vec1.z = left;
        vec3.x = depth;
        vec3.z = right;
        vec2.x = depth;
        vec2.z = right;
      }

      int texture = static_cast<unsigned>(i) % 3;
      const int shift = cycle / frames;
      const char direction = slot.flag1[1];
      if (direction == 1 || direction == 10) {
        texture -= shift;
        while (texture < 0) {
          texture += 3;
        }
      } else {
        texture += shift;
        if (texture > 2) {
          texture -= 3;
        }
      }

      if (i == 0) {
        const float cut = scroll * 0.025f;
        top -= (top - bottom) * cut;
        vec4.y = top;
        vec3.y = top;
        if (direction == 1) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(texture), 0, 55, 55, 255, cut, 1.0f, 0.0f, 1.0f, 0);
        } else if (direction == 10) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(texture), 0, 255, 55, 255, cut, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, static_cast<char>(texture), 0, 255, 255, 255, cut, 1.0f, 0.0f, 1.0f, 0);
        }
      } else if (i == 4) {
        const float cut = scroll * 0.025f;
        bottom = (top - bottom) * (1.0f - cut) + bottom;
        vec1.y = bottom;
        vec2.y = bottom;
        if (direction == 1) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(texture), 0, 55, 55, 255, 0.0f, cut, 0.0f, 1.0f, 0);
        } else if (direction == 10) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(texture), 0, 255, 55, 255, 0.0f, cut, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, static_cast<char>(texture), 0, 255, 255, 255, 0.0f, cut, 0.0f, 1.0f, 0);
        }
      } else {
        if (direction == 1) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(texture), 0, 55, 55, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (direction == 10) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(texture), 0, 255, 55, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, static_cast<char>(texture), 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00BE65A0
// FUNCTION: fn_19928
void CEffectPrim::RenderRiseArc(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 21;
    int i = 0;
    for (int offset = 0; offset < slot.fullDisplayAngle; offset += step) {
      int angle = slot.rotStart + offset;
      angle %= 360;
      if (slot.fullDisplayAngle == 360 && i == 20) {
        angle = slot.rotStart;
      }

      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      float* v = &slot.vecBNow.z;
      v[-1] = -slot.maxHeight;
      v[-2] = slot.distance * cosine;
      v[0] = slot.distance * sine;

      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      v[2] = -(slot.maxHeight + rise * g_sinTable[slot.riseAngle]);
      v[3] = riseX * sine + v[0];
      v[1] = riseX * cosine + v[-2];

      if (i > 0) {
        this->RenderTeiRect(&rpCnt, i - 1, slot.alphaB, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0);
      }

      ++i;
      slot.vecBPre = slot.vecBNow;
      slot.vecTPre = slot.vecTNow;
      if (i >= 21) {
        i = 20;
      }
    }
  }
}

// INLINED
static __forceinline void RenderFacingQuad(
  CEffectPrim& prim, int* rpCnt, const EffectPrimitiveSlot& slot, const vector3d& vec1, const vector3d& vec2, const vector3d& vec3, const vector3d& vec4,
  const vector3d& position, char alphaValue, matrix& wtm
) {
  const vector3d corner1{position.x + vec1.x, position.y + vec1.y, position.z + vec1.z};
  const vector3d corner2{position.x + vec2.x, position.y + vec2.y, position.z + vec2.z};
  const vector3d corner3{position.x + vec3.x, position.y + vec3.y, position.z + vec3.z};
  const vector3d corner4{position.x + vec4.x, position.y + vec4.y, position.z + vec4.z};
  const int alpha = std::max(alphaValue * 2, 0);

  if (slot.flag1[5] == 7) {
    prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 250, 70, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[5] == 8) {
    prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 250, 250, 10, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[5] == 10) {
    prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 250, 250, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 70, 120, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE67E0
// FUNCTION: fn_19929
void CEffectPrim::RenderFacingQuads(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float size = slot.distance;
    vec1 = {size * g_cosTable[45], 0.0f, size * g_sinTable[45]};
    vec2 = {size * g_cosTable[135], 0.0f, size * g_sinTable[135]};
    vec3 = {size * g_cosTable[225], 0.0f, size * g_sinTable[225]};
    vec4 = {size * g_cosTable[315], 0.0f, size * g_sinTable[315]};
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    RenderFacingQuad(*this, &rpCnt, slot, vec1, vec2, vec3, vec4, slot.vecBPre, slot.flag1[11], wtm);
    RenderFacingQuad(*this, &rpCnt, slot, vec1, vec2, vec3, vec4, slot.vecBNow, slot.flag1[12], wtm);
    RenderFacingQuad(*this, &rpCnt, slot, vec1, vec2, vec3, vec4, slot.vecTPre, slot.flag1[13], wtm);
    RenderFacingQuad(*this, &rpCnt, slot, vec1, vec2, vec3, vec4, slot.vecTNow, slot.flag1[14], wtm);
  }
}

// OFFSET: 0x00BD2AB0
// FUNCTION: fn_19875
void CEffectPrim::RenderHalfSphere2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 10;
    int level = 0;

    for (int ring = 10; ring != 0; --ring) {
      for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
        const int angle = (slot.rotStart + count) % 360;
        const float cs = g_cosTable[angle];
        const float sn = g_sinTable[angle];
        const float dist = slot.distance;
        slot.vecBNow = {dist * cs, 0.0f, dist * sn};

        const float radius = slot.height[level];
        const float spread = radius * g_cosTable[slot.riseAngle];
        slot.vecTNow = {spread * cs + slot.vecBNow.x, -(radius * g_sinTable[slot.riseAngle]), spread * sn + slot.vecBNow.z};

        if (level > 0) {
          this->RenderTeiRect(&rpCnt, level - 1, slot.alphaB, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0);
        }

        ++level;
        if (level >= 21) {
          level = 20;
        }

        slot.vecBPre = slot.vecBNow;
        slot.vecTPre = slot.vecTNow;
      }
    }
  }
}

// OFFSET: 0x00BDAC40
// FUNCTION: fn_19894
void CEffectPrim::RenderHalfSphere(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 10;

    for (int angle2 = 0; angle2 < 180; angle2 += 18) {
      for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
        int angle = slot.rotStart + count;

        if (angle >= 360) {
          angle -= 360;
        }

        if (slot.fullDisplayAngle == 360 && count == 360) {
          angle = slot.rotStart;
        }

        const float cs1 = g_cosTable[angle];
        const float sn1 = g_sinTable[angle];
        const float dist = slot.distance;

        float ring = g_sinTable[angle2] * dist;
        vec2.Set(cs1 * ring + slot.vecTNow.x, g_cosTable[angle2] * dist - slot.maxHeight, sn1 * ring + slot.vecTNow.z);
        ring = g_sinTable[angle2 + 18] * dist;
        vec4.Set(cs1 * ring + slot.vecTNow.x, g_cosTable[angle2 + 18] * dist - slot.maxHeight, sn1 * ring + slot.vecTNow.z);

        if (count > 0) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 1, 0, 0, 250, 250, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }

        vec1 = vec2;
        vec3 = vec4;
      }
    }
  }
}

// OFFSET: 0x00BDAEF0
// FUNCTION: fn_19895
void CEffectPrim::RenderHermode(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    int angle = slot.rotStart;
    const float snA = g_sinTable[angle] * 2.0f;
    const float rx = slot.distance * 0.1f * (g_sinTable[slot.riseAngle] + 1.0f);
    const double radius = slot.distance * 0.8 + rx;
    const float y = slot.height[0] + snA;

    slot.vecBPre = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    angle += angle >= 270 ? -270 : 90;
    slot.vecBNow = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    angle += angle + 90 >= 360 ? -270 : 90;
    slot.vecTNow = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    angle += angle + 90 >= 360 ? -270 : 90;
    slot.vecTPre = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};

    vector3d vec1 = slot.vecBPre;
    vec1.y -= slot.distance;
    vector3d vec2 = slot.vecBNow;
    vec2.y -= slot.distance;
    vector3d vec3 = slot.vecTPre;
    vec3.y -= slot.distance;
    vector3d vec4 = slot.vecTNow;
    vec4.y -= slot.distance;

    const unsigned char shade = static_cast<unsigned char>(slot.flag1[0]);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, shade, shade, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, shade, shade, 220, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, shade, shade, 190, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, shade, shade, 130, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, shade, shade, 160, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0, 0, 0, shade, shade, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BDB530
// FUNCTION: fn_19896
void CEffectPrim::RenderHitLine3(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0) {
      continue;
    }

    for (int j = 0; static_cast<float>(j) < slot.vecTNow.y; ++j) {
      float speed = 0.0f;

      if (slot.height[j] > 0.0f) {
        speed += slot.height[j] / 90.0f * slot.distance;
      }

      vector3d vecA{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j])] * slot.vecTNow.x,
        speed * g_sinTable[slot.riseAngle]
      };

      speed = 0.0f;

      if (slot.height[j + 1] > 0.0f) {
        speed += slot.height[j + 1] / 90.0f * slot.distance;
      }

      vector3d vecB{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j + 1])] * slot.vecTNow.x,
        speed * g_sinTable[slot.riseAngle]
      };

      std::int16_t angle;
      Get_Angle(vecA.x - vecB.x, vecA.z - vecB.z, &angle);
      vecA.x += slot.vecBNow.x;
      vecA.y += slot.vecBNow.y;
      vecA.z += slot.vecBNow.z;
      vecB.x += slot.vecBNow.x;
      vecB.y += slot.vecBNow.y;
      vecB.z += slot.vecBNow.z;
      angle += 90;

      if (angle >= 360) {
        angle -= 360;
      }

      for (int i = 0; i <= 3; ++i) {
        const float radius = slot.maxHeight;
        const float tx = radius * g_cosTable[i * 120];
        const float ry = radius * g_sinTable[i * 120] - 5.0f;
        const float rx = tx * g_cosTable[angle];
        const float rz = tx * g_sinTable[angle];
        vec2.Set(vecA.x + rx, vecA.y + ry, vecA.z + rz);
        vec4.Set(vecB.x + rx, vecB.y + ry, vecB.z + rz);

        if (i > 0) {
          const int alpha = slot.alphaB - j * 10;

          if (alpha > 0) {
            if (slot.alphaT == 0) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 255, 200, 150 - (5 - j) * 30, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT == 1) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 120 - (5 - j) * 22, 120 - (5 - j) * 22, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT == 2) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 255, 200, 150 - (3 - j) * 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT >= 3) {
              const unsigned char grey = static_cast<unsigned char>(255 - (5 - j) * 30);
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, grey, grey, grey, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }
        }

        vec1 = vec2;
        vec3 = vec4;
      }

      if (slot.alphaT >= 3) {
        for (int i = 0; i <= 3; ++i) {
          const float radius = slot.maxHeight * 4.0f;
          const float tx = radius * g_cosTable[i * 120];
          const float ry = radius * g_sinTable[i * 120] - 5.0f;
          const float rx = tx * g_cosTable[angle];
          const float rz = tx * g_sinTable[angle];
          vec2.Set(vecA.x + rx, vecA.y + ry, vecA.z + rz);
          vec4.Set(vecB.x + rx, vecB.y + ry, vecB.z + rz);

          if (i > 0) {
            const int alpha = static_cast<int>(static_cast<float>(slot.alphaB - j * 10) * 0.5f);

            if (alpha > 0 && slot.alphaT == 3) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }

          vec1 = vec2;
          vec3 = vec4;
        }
      }
    }
  }
}

// OFFSET: 0x00BDBC50
// FUNCTION: fn_19897
void CEffectPrim::RenderHitLine(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0) {
      continue;
    }

    for (int j = 0; j < 20; ++j) {
      float speed = 0.0f;
      float radius = 1.0f;

      for (int i = 0; i < slot.flag1[j]; ++i) {
        speed += slot.distance * 2.0f * radius;
        radius *= 0.7f;
      }

      if (slot.height[j] > 0.0f) {
        speed += slot.distance * radius * (slot.height[j] / 90.0f);
      }

      radius = 1.0f;

      for (int i = 0; i < slot.flag1[j]; ++i) {
        radius *= 0.7f;
      }

      vector3d vecA{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j])] * 1.5f * radius,
        speed * g_sinTable[slot.riseAngle]
      };

      speed = 0.0f;
      radius = 1.0f;

      for (int i = 0; i < slot.flag1[j + 1]; ++i) {
        speed += slot.distance * 2.0f * radius;
        radius *= 0.7f;
      }

      if (slot.height[j + 1] > 0.0f) {
        speed += slot.distance * radius * (slot.height[j + 1] / 90.0f);
      }

      radius = 1.0f;

      for (int i = 0; i < slot.flag1[j + 1]; ++i) {
        radius *= 0.7f;
      }

      vector3d vecB{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j + 1])] * 1.5f * radius,
        speed * g_sinTable[slot.riseAngle]
      };

      std::int16_t angle;
      Get_Angle(vecA.x - vecB.x, vecA.z - vecB.z, &angle);
      vecA.x += slot.vecBNow.x;
      vecA.y += slot.vecBNow.y;
      vecA.z += slot.vecBNow.z;
      vecB.x += slot.vecBNow.x;
      vecB.y += slot.vecBNow.y;
      vecB.z += slot.vecBNow.z;
      angle = static_cast<std::int16_t>((angle + 90) % 360);

      for (int i = 0; i <= 3; ++i) {
        const float tx = slot.maxHeight * g_cosTable[i * 120];
        const float ry = slot.maxHeight * g_sinTable[i * 120];
        const float rx = tx * g_cosTable[angle];
        const float rz = tx * g_sinTable[angle];
        vec2.Set(vecA.x + rx, vecA.y + ry, vecA.z + rz);
        vec4.Set(vecB.x + rx, vecB.y + ry, vecB.z + rz);

        if (i > 0) {
          const int alpha = slot.alphaB - j * 5;

          if (alpha > 0) {
            if (slot.alphaT == 2) {
              const unsigned char shade = static_cast<unsigned char>(200 - (20 - j) * 7);
              this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, 255, shade, shade, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT == 8) {
              const unsigned char shade = static_cast<unsigned char>(150 - (20 - j) * 7);
              this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, shade, shade, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else {
              this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, 255, 200, 150 - (20 - j) * 7, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }
        }

        vec1 = vec2;
        vec3 = vec4;
      }
    }
  }
}

// OFFSET: 0x00BDD980
// FUNCTION: fn_19902
void CEffectPrim::RenderMagnum2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (rpCnt >= 256 || slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = slot.rotStart + count;

      if (angle >= 360) {
        angle -= 360;
      }

      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      slot.vecBNow.x = cs1 * slot.distance;
      slot.vecBNow.y = -3.0f;
      slot.vecBNow.z = sn1 * slot.distance;

      const float rx = g_cosTable[slot.riseAngle] * slot.maxHeight;
      const float ry = g_sinTable[slot.riseAngle] * slot.maxHeight;
      slot.vecTNow.x = cs1 * rx + slot.vecBNow.x;
      slot.vecTNow.y = -(ry + 3.0f);
      slot.vecTNow.z = sn1 * rx + slot.vecBNow.z;

      if (order > 0 && slot.alphaB > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0);
      }

      ++order;

      if (order >= 21) {
        order = 20;
      }

      slot.vecBPre = slot.vecBNow;
      slot.vecTPre = slot.vecTNow;
    }
  }
}

// OFFSET: 0x00BD8870
// FUNCTION: fn_19889
void CEffectPrim::RenderRipple(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    for (int ring = 10; ring >= 1; --ring) {
      const char kind = slot.flag1[4];
      int spread = 10 - ring;
      if (kind != 2) {
        spread *= 3;
      }

      const float rx = static_cast<float>(spread) + slot.distance;
      slot.vecBPre = {rx * g_cosTable[45], 0.0f, rx * g_sinTable[45]};
      slot.vecBNow = {rx * g_cosTable[135], 0.0f, rx * g_sinTable[135]};
      slot.vecTNow = {rx * g_cosTable[225], 0.0f, rx * g_sinTable[225]};
      slot.vecTPre = {rx * g_cosTable[315], 0.0f, rx * g_sinTable[315]};

      const float base = slot.height[0];
      vec1.Set(slot.vecBPre.x + base, slot.vecBPre.y, slot.vecBPre.z);
      vec2.Set(slot.vecBNow.x + base, slot.vecBNow.y, slot.vecBNow.z);
      vec3.Set(slot.vecTNow.x + base, slot.vecTNow.y, slot.vecTNow.z);
      vec4.Set(slot.vecTPre.x + base, slot.vecTPre.y, slot.vecTPre.z);
      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

      const float drop = kind == 2 ? 40.0f : 30.0f;
      vec1.y -= drop;
      vec2.y -= drop;
      vec3.y -= drop;
      vec4.y -= drop;

      const int alpha = static_cast<int>(slot.height[ring]);
      if (alpha > 0) {
        const unsigned char shade = ring == 10 ? 55 : 5;
        this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 0, shade, shade, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BDE720
// FUNCTION: fn_19905
void CEffectPrim::RenderPeongUp(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vector3d vec1{g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
    vector3d vec2{g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
    vector3d vec3{g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
    vector3d vec4{g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d& base = slot.vecBPre;
    vec1 = {vec1.x + base.x, vec1.y + base.y, vec1.z + base.z};
    vec2 = {vec2.x + base.x, vec2.y + base.y, vec2.z + base.z};
    vec3 = {vec3.x + base.x, vec3.y + base.y, vec3.z + base.z};
    vec4 = {vec4.x + base.x, vec4.y + base.y, vec4.z + base.z};

    if (vec1.y <= 0.0f) {
      if (slot.flag1[0] == 2) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 105, 105, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      } else if (slot.flag1[0] == 3) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 55, 55, 55, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      } else {
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BDEA00
// FUNCTION: fn_19906
__declspec(safebuffers) void CEffectPrim::RenderGrid(matrix& wtm) {
  vector3d vec1 = {0.0f, 0.0f, 0.0f};
  vector3d vec2 = {0.0f, 0.0f, 0.0f};
  vector3d vec3 = {0.0f, 0.0f, 0.0f};
  vector3d vec4 = {0.0f, 0.0f, 0.0f};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    for (int x = -150; x < 150; x += 30) {
      for (int z = -90; z < 90; z += 30) {
        const float size = slot.distance;
        const float base = slot.maxHeight;
        vec1.x = base + static_cast<float>(x);
        vec1.y = -10.0f;
        vec1.z = base + static_cast<float>(z);
        vec2.x = size + static_cast<float>(x) + base;
        vec2.y = -10.0f;
        vec2.z = vec1.z;
        vec3.x = vec2.x;
        vec3.y = -10.0f;
        vec3.z = size + static_cast<float>(z) + base;
        vec4.x = vec1.x;
        vec4.y = -10.0f;
        vec4.z = vec3.z;
        this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

        const char color = slot.flag1[10];
        if (color == 0) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (color == 1) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (color == 3) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00BCFC00
// FUNCTION: fn_19867
void CEffectPrim::RenderRing(matrix& wtm) {
  vector3d corners[4];
  for (vector3d& element : corners) {
    ZeroVector(element);
  }
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const float cs = g_cosTable[slot.riseAngle];
    const float sn = g_sinTable[slot.riseAngle];
    const vector3d& center = slot.vecBNow;
    const int angles[4] = {90, 180, 270, 0};

    for (int index = 0; index < 4; ++index) {
      const float radial = dist * g_cosTable[angles[index]];
      corners[index] = {cs * radial + center.x, dist * g_sinTable[angles[index]] + center.y, sn * radial + center.z};
    }

    this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 60, 60, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD85F0
// FUNCTION: fn_19888
void CEffectPrim::RenderPentagon(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec4;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const vector3d top = slot.vecTPre;
    const float radius = slot.maxHeight;

    for (int point = 0; point <= 5; ++point) {
      short angle = static_cast<short>(slot.rotStart + point * 72);
      if (angle >= 360) {
        angle -= 360;
      }

      const float dx = radius * g_cosTable[angle];
      const float dz = radius * g_sinTable[angle];
      vec2.Set(dx + slot.vecTNow.x, slot.vecTNow.y, dz + slot.vecTNow.z);
      vec3.Set(top.x + dx, top.y, top.z + dz);

      if (point != 0) {
        if (slot.flag1[0] == 13) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 230, 255, 230, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }

      vec1 = vec2;
      vec4 = vec3;
    }
  }
}

// OFFSET: 0x00BE0990
// FUNCTION: fn_19912
void CEffectPrim::RenderSMA3(matrix&) {
  const EffectPrimitiveSlot& slot = this->state.slots[0];
  if (slot.life != 1) {
    return;
  }

  int effectId;
  if (slot.flag1[0] == 0) {
    if (slot.process % 8 != 0) {
      return;
    }
    effectId = 552;
  } else if (slot.flag1[0] == 1) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    CRagEffect* effect = gameMode->scene->player->LaunchEffect(556, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      vector3d position = slot.vecBNow;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    return;
  } else if (slot.flag1[0] == 99) {
    if (slot.process % 8 != 0) {
      return;
    }
    effectId = 1205;
  } else {
    return;
  }

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CRagEffect* effect = gameMode->scene->player->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    vector3d position = slot.vecBNow;
    float extra[3] = {static_cast<float>(slot.process), slot.process % 16 != 0 ? 1.0f : 0.0f, 0.0f};
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), reinterpret_cast<int>(extra), reinterpret_cast<int>(extra) >> 31, 0, 0);
  }
}
// OFFSET: 0x00BDD0D0
// FUNCTION: fn_19900
void CEffectPrim::RenderBeam(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float lift = slot.height[3];
    const float length = slot.distance * g_sinTable[slot.fullDisplayAngle] + slot.distance;
    const float reach = length * g_cosTable[slot.riseAngle];
    const vector3d tip{reach * g_cosTable[slot.rotStart], length * g_sinTable[slot.riseAngle] - lift, reach * g_sinTable[slot.rotStart]};
    const unsigned char red = static_cast<unsigned char>(static_cast<int>(slot.height[0]));
    const unsigned char green = static_cast<unsigned char>(static_cast<int>(slot.height[1]));
    const unsigned char blue = static_cast<unsigned char>(static_cast<int>(slot.height[2]));

    this->RenderTeiRect(&rpCnt, slot.alphaB, vector3d{0.5f, -lift, 0.0f}, tip, tip, vector3d{-0.5f, -lift, 0.0f}, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vector3d{0.0f, -lift, 0.5f}, tip, tip, vector3d{0.0f, -lift, -0.5f}, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD0A20
// FUNCTION: fn_19870
void CEffectPrim::RenderCone(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec4;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.fullDisplayAngle < 0) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int texPart = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = slot.riseAngle + count;
      if (angle >= 360) {
        angle -= 360;
      }

      if (slot.fullDisplayAngle == 360 && count == 360) {
        angle = slot.riseAngle;
      }

      const float scale = slot.height[0];
      const float cs = g_cosTable[angle];
      const float inner = slot.distance * g_sinTable[angle];
      const float outerRadius = scale * slot.distance * 0.1f + slot.distance;
      const float outer = outerRadius * g_sinTable[angle];
      vec2.Set(slot.vecTNow.x + inner * g_cosTable[slot.rotStart], slot.distance * cs - slot.maxHeight, slot.vecTNow.z + inner * g_sinTable[slot.rotStart]);
      vec3.Set(
        outer * g_cosTable[slot.rotStart] - slot.vecTNow.x * scale, outerRadius * cs - slot.maxHeight, outer * g_sinTable[slot.rotStart] - slot.vecTNow.z * scale
      );

      if (count > 0) {
        this->RenderTeiRect(&rpCnt, texPart - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0);
      }

      if (++texPart > 20) {
        texPart = 20;
      }

      vec1 = vec2;
      vec4 = vec3;
    }
  }
}

// OFFSET: 0x00BDE460
// FUNCTION: fn_19904
void CEffectPrim::RenderPeong(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vec1.Set(g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx);
    vec2.Set(g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx);
    vec3.Set(g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx);
    vec4.Set(g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d& base = slot.vecBPre;
    vec1.Set(vec1.x + base.x, vec1.y + base.y, vec1.z + base.z);
    vec2.Set(vec2.x + base.x, vec2.y + base.y, vec2.z + base.z);
    vec3.Set(vec3.x + base.x, vec3.y + base.y, vec3.z + base.z);
    vec4.Set(vec4.x + base.x, vec4.y + base.y, vec4.z + base.z);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD2CF0
// FUNCTION: fn_19876
void CEffectPrim::RenderFloorRect(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int rot = slot.riseAngle;
    const float back = slot.maxHeight;
    const float width = slot.height[0];
    const float length = slot.distance;
    const float baseX = width * g_cosTable[rot >= 90 ? rot - 90 : rot + 270] + back * g_cosTable[rot >= 90 ? rot - 90 : rot + 270];
    const float baseZ = width * g_sinTable[rot >= 90 ? rot - 90 : rot + 270] + back * g_sinTable[rot >= 90 ? rot - 90 : rot + 270];
    const float sideX = back * g_cosTable[rot < 270 ? rot + 90 : rot - 270] + width * g_cosTable[rot >= 90 ? rot - 90 : rot + 270];
    const float sideZ = back * g_sinTable[rot < 270 ? rot + 90 : rot - 270] + width * g_sinTable[rot >= 90 ? rot - 90 : rot + 270];

    vec3.Set(length * g_cosTable[rot < 0 ? rot + 360 : rot] + baseX, 0.0f, length * g_sinTable[rot < 0 ? rot + 360 : rot] + baseZ);
    vec2.Set(length * g_cosTable[rot >= 180 ? rot - 180 : rot + 180] + baseX, 0.0f, length * g_sinTable[rot >= 180 ? rot - 180 : rot + 180] + baseZ);
    vec1.Set(length * g_cosTable[rot < 0 ? rot + 360 : rot] + sideX, 0.0f, length * g_sinTable[rot < 0 ? rot + 360 : rot] + sideZ);
    vec4.Set(length * g_cosTable[rot >= 180 ? rot - 180 : rot + 180] + sideX, 0.0f, length * g_sinTable[rot >= 180 ? rot - 180 : rot + 180] + sideZ);

    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec4, vec2, vec3, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE0B40
// FUNCTION: fn_19913
void CEffectPrim::RenderRotatedRing(matrix& wtm) {
  vector3d corners[4];
  for (vector3d& element : corners) {
    ZeroVector(element);
  }
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const float cs = g_cosTable[slot.rotStart];
    const float sn = g_sinTable[slot.rotStart];
    const vector3d& center = slot.vecBNow;
    int angle = slot.riseAngle;

    for (int index = 0; index < 4; ++index) {
      angle = angle < 270 ? angle + 90 : angle - 270;
      const float radial = dist * g_cosTable[angle];
      corners[index] = {cs * radial + center.x, dist * g_sinTable[angle] + center.y, sn * radial + center.z};
    }

    this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 80, 255, 80, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE1A80
// FUNCTION: fn_19917
void CEffectPrim::RenderFloorSquare(matrix& wtm) {
  vector3d corners[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const vector3d& center = slot.vecBNow;
    const float y = center.y + 0.0f;
    int angle = slot.riseAngle + 45 < 360 ? slot.riseAngle + 45 : slot.riseAngle - 315;

    for (int index = 0; index < 4; ++index) {
      corners[index] = {dist * g_cosTable[angle] + center.x, y, dist * g_sinTable[angle] + center.z};
      angle = angle + 90 < 360 ? angle + 90 : angle - 270;
    }

    if (slot.flag1[1] == 1) {
      const char frame = slot.process > 0 ? static_cast<char>(slot.process / 6 % 4) : 0;
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, frame, 1, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[1] == 2) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 0, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 200, 200, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE0E30
// FUNCTION: fn_19914
void CEffectPrim::RenderFloorSquare2(matrix& wtm) {
  vector3d corners[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const vector3d& center = slot.vecBNow;
    const float y = center.y + 0.0f;
    int angle = slot.riseAngle < 270 ? slot.riseAngle + 90 : slot.riseAngle - 270;

    for (int index = 0; index < 4; ++index) {
      corners[index] = {dist * g_cosTable[angle] + center.x, y, dist * g_sinTable[angle] + center.z};
      angle = angle + 90 < 360 ? angle + 90 : angle - 270;
    }

    switch (slot.flag1[3]) {
    case 0:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 1:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 80, 255, 80, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 70, 70, 70, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }
  }
}

// OFFSET: 0x00BD1CB0
// FUNCTION: fn_19873
void CEffectPrim::RenderColorPlane(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float height = -slot.maxHeight;
    const vector3d vec1{slot.vecBNow.x, 0.0f, slot.vecBNow.z};
    const vector3d vec2{slot.vecBPre.x, 0.0f, slot.vecBPre.z};
    const vector3d vec3{slot.vecBPre.x, height, slot.vecBPre.z};
    const vector3d vec4{slot.vecBNow.x, height, slot.vecBNow.z};
    const float kind = slot.height[0];

    if (kind == 1.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 2.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 155, 255, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 3.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 4.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 150, 150, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE37A0
// FUNCTION: fn_19922
void CEffectPrim::RenderRotatedPlate(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int angle = slot.fullDisplayAngle;
    const int rise = slot.riseAngle;
    const float size = slot.height[1];
    const float shift = static_cast<float>(slot.flag1[1]) * size;
    const float outer = shift + slot.distance;
    const float inner = slot.distance - shift;
    const float back = size * -1.0f;

    float across = g_cosTable[angle] * outer - g_sinTable[angle] * size;
    vec2.Set(across * g_cosTable[rise] - g_sinTable[rise] * 0.0f, g_sinTable[angle] * outer + g_cosTable[angle] * size, across * g_sinTable[rise] + g_cosTable[rise] * 0.0f);
    across = g_cosTable[angle] * inner - g_sinTable[angle] * size;
    vec3.Set(across * g_cosTable[rise] - g_sinTable[rise] * 0.0f, g_sinTable[angle] * inner + g_cosTable[angle] * size, across * g_sinTable[rise] + g_cosTable[rise] * 0.0f);
    across = g_cosTable[angle] * inner - g_sinTable[angle] * back;
    vec4.Set(across * g_cosTable[rise] - g_sinTable[rise] * 0.0f, g_cosTable[angle] * back + g_sinTable[angle] * inner, across * g_sinTable[rise] + g_cosTable[rise] * 0.0f);
    across = g_cosTable[angle] * outer - g_sinTable[angle] * back;
    vec1.Set(across * g_cosTable[rise] - g_sinTable[rise] * 0.0f, g_cosTable[angle] * back + g_sinTable[angle] * outer, across * g_sinTable[rise] + g_cosTable[rise] * 0.0f);

    switch (slot.flag1[0]) {
    case 0:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 225, 225, 80, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 1:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 65, 184, 240, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    default:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 200, 200, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }
  }
}

// OFFSET: 0x00BDEC50
// FUNCTION: fn_19907
void CEffectPrim::RenderTrailSquares(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    float shrink = 0.0f;
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    switch (slot.flag1[0]) {
    case 0:
      red = 70;
      green = 70;
      blue = 255;
      break;
    case 1:
      red = 255;
      green = 70;
      blue = 70;
      break;
    case 2:
      red = 70;
      green = 255;
      blue = 70;
      break;
    case 3:
      red = 255;
      green = 255;
      blue = 90;
      break;
    default:
      red = 255;
      green = 70;
      blue = 255;
      break;
    }

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    int index = 0;
    do {
      const int angle1 = slot.rotStart;
      const float radius = slot.maxHeight - shrink;
      const int angle2 = angle1 < 270 ? angle1 + 90 : angle1 - 270;
      const int angle3 = angle2 + 90 < 360 ? angle2 + 90 : angle2 - 270;
      const int angle4 = angle3 + 90 < 360 ? angle3 + 90 : angle3 - 270;
      vec1.Set(radius * g_cosTable[angle1], 0.0f, radius * g_sinTable[angle1]);
      vec2.Set(radius * g_cosTable[angle2], 0.0f, radius * g_sinTable[angle2]);
      vec3.Set(radius * g_cosTable[angle3], 0.0f, radius * g_sinTable[angle3]);
      vec4.Set(radius * g_cosTable[angle4], 0.0f, radius * g_sinTable[angle4]);
      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

      const int step = index;
      const vector3d& offset = index == 0 ? slot.vecBNow : index == 1 ? slot.vecBPre : slot.vecTPre;
      ++index;
      const int alpha = slot.alphaB / index;
      vec1.Set(offset.x + vec1.x, offset.y + vec1.y, offset.z + vec1.z);
      vec2.Set(offset.x + vec2.x, offset.y + vec2.y, offset.z + vec2.z);
      vec3.Set(offset.x + vec3.x, offset.y + vec3.y, offset.z + vec3.z);
      vec4.Set(offset.x + vec4.x, offset.y + vec4.y, offset.z + vec4.z);

      if (slot.flag1[3] != 0) {
        this->RenderTeiRect(&rpCnt, static_cast<short>(alpha / 4), vec1, vec2, vec3, vec4, wtm, 0, slot.flag1[3], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      } else {
        this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 1, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }

      shrink += step != 0 ? 0.25f : 0.5f;
    } while (index < 2);
  }
}

// OFFSET: 0x00BD3D90
// FUNCTION: fn_19879
void CEffectPrim::RenderColorPanel(matrix& wtm) {
  ZeroVector3d vec4;
  ZeroVector3d vec3;
  int rpCnt = 0;
  unsigned char red;
  unsigned char green;
  unsigned char blue;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0) {
      continue;
    }

    const int angle = slot.rotStart;
    const float size = slot.height[2];
    const float half = size * 0.5f;
    const int opposite = angle < 180 ? angle + 180 : angle - 180;
    const float top = half - slot.height[4];
    slot.vecBPre = {half * g_cosTable[angle] - g_sinTable[angle] * 0.0f, top, half * g_sinTable[angle] + g_cosTable[angle] * 0.0f};
    slot.vecBNow = {half * g_cosTable[opposite] - g_sinTable[opposite] * 0.0f, top, half * g_sinTable[opposite] + g_cosTable[opposite] * 0.0f};

    const float mode = slot.height[5];
    const float level = slot.height[3];
    char extra = 1;
    if (mode == 2.0f) {
      if (level == 0.0f) {
        red = 105;
        green = 255;
        blue = 105;
      } else if (level == 1.0f) {
        red = 5;
        green = 255;
        blue = 5;
      } else if (level == 2.0f) {
        red = 55;
        green = 255;
        blue = 55;
      } else if (level == 3.0f) {
        red = 155;
        green = 255;
        blue = 155;
      }
    } else if (mode == 3.0f) {
      if (level == 0.0f) {
        red = 105;
        green = 105;
        blue = 255;
      } else if (level == 1.0f) {
        red = 5;
        green = 5;
        blue = 255;
      } else if (level == 2.0f) {
        red = 55;
        green = 55;
        blue = 255;
      } else if (level == 3.0f) {
        red = 155;
        green = 155;
        blue = 255;
      }
    } else if (mode == 4.0f) {
      if (level == 0.0f) {
        red = 255;
        green = 255;
        blue = 105;
      } else if (level == 1.0f) {
        red = 255;
        green = 255;
        blue = 5;
      } else if (level == 2.0f) {
        red = 255;
        green = 255;
        blue = 55;
      } else if (level == 3.0f) {
        red = 255;
        green = 255;
        blue = 155;
      }
    } else if (mode == 5.0f) {
      if (level == 0.0f) {
        red = 255;
        green = 105;
        blue = 105;
      } else if (level == 1.0f) {
        red = 255;
        green = 5;
        blue = 5;
      } else if (level == 2.0f) {
        red = 255;
        green = 55;
        blue = 55;
      } else if (level == 3.0f) {
        red = 255;
        green = 155;
        blue = 155;
      }
    } else if (mode == 6.0f) {
      if (level == 0.0f) {
        red = 255;
        green = 105;
        blue = 255;
      } else if (level == 1.0f) {
        red = 255;
        green = 5;
        blue = 255;
      } else if (level == 2.0f) {
        red = 255;
        green = 55;
        blue = 255;
      } else if (level == 3.0f) {
        red = 255;
        green = 155;
        blue = 255;
      }
    } else if (mode == 7.0f) {
      if (level == 0.0f) {
        red = 105;
        green = 255;
        blue = 255;
      } else if (level == 1.0f) {
        red = 5;
        green = 255;
        blue = 255;
      } else if (level == 2.0f) {
        red = 55;
        green = 255;
        blue = 255;
      } else if (level == 3.0f) {
        red = 155;
        green = 255;
        blue = 255;
      }
    } else if (mode == 8.0f) {
      if (level == 0.0f) {
        red = 105;
        green = 105;
        blue = 105;
      } else if (level == 1.0f) {
        red = 5;
        green = 5;
        blue = 5;
      } else if (level == 2.0f) {
        red = 55;
        green = 55;
        blue = 55;
      } else if (level == 3.0f) {
        red = 255;
        green = 255;
        blue = 255;
      }
    } else {
      extra = 0;
      if (level == 0.0f) {
        red = 105;
        green = 105;
        blue = 255;
      } else if (level == 1.0f) {
        red = 5;
        green = 5;
        blue = 255;
      } else if (level == 2.0f) {
        red = 55;
        green = 55;
        blue = 255;
      } else if (level == 3.0f) {
        red = 155;
        green = 155;
        blue = 255;
      }
    }

    if (size > 0.0f) {
      vec3.Set(slot.vecBNow.x, top - size, slot.vecBNow.z);
      vec4.Set(slot.vecBPre.x, top - size, slot.vecBPre.z);
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec3, vec4, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, extra);
    }
  }
}

// OFFSET: 0x00BDCB70
// FUNCTION: fn_19899
void CEffectPrim::RenderGlowDisc(matrix& wtm) {
  const EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1) {
    return;
  }

  int rpCnt = 0;
  float rx = (static_cast<float>(slot.alphaB) / 255.0f * 0.2f + 1.0f) * slot.distance
    + slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f;
  vector3d vec1{g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
  vector3d vec2{g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
  vector3d vec3{g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
  vector3d vec4{g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);
  vec1 = {slot.vecBNow.x + vec1.x, slot.vecBNow.y + vec1.y, slot.vecBNow.z + vec1.z};
  vec2 = {slot.vecBNow.x + vec2.x, slot.vecBNow.y + vec2.y, slot.vecBNow.z + vec2.z};
  vec3 = {slot.vecBNow.x + vec3.x, slot.vecBNow.y + vec3.y, slot.vecBNow.z + vec3.z};
  vec4 = {slot.vecBNow.x + vec4.x, slot.vecBNow.y + vec4.y, slot.vecBNow.z + vec4.z};
  this->state.renderFlag = 5;
  const float offset[3] = {0.0f, 0.0f, 0.01f};
  matrix glowTm = wtm;
  glowTm.AppendTranslate(offset);
  this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, glowTm, 0, 0, 0, 180, 180, 180, 0.0f, 1.0f, 0.0f, 1.0f, 0);

  rx = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
  vec1 = {g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
  vec2 = {g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
  vec3 = {g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
  vec4 = {g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);
  vec1 = {slot.vecBNow.x + vec1.x, slot.vecBNow.y + vec1.y, slot.vecBNow.z + vec1.z};
  vec2 = {slot.vecBNow.x + vec2.x, slot.vecBNow.y + vec2.y, slot.vecBNow.z + vec2.z};
  vec3 = {slot.vecBNow.x + vec3.x, slot.vecBNow.y + vec3.y, slot.vecBNow.z + vec3.z};
  vec4 = {slot.vecBNow.x + vec4.x, slot.vecBNow.y + vec4.y, slot.vecBNow.z + vec4.z};
  this->state.renderFlag = 257;
  this->RenderTeiRect(&rpCnt, 255, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
}

// OFFSET: 0x00BD8080
// FUNCTION: fn_19887
void CEffectPrim::RenderSpiralBlades(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    for (int i = 4; i < 18; ++i) {
      const float step = static_cast<float>(i);
      const float rx = slot.distance * step / 14.0f * static_cast<float>(i + 46) / 50.0f
        + slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f;
      const float shape = slot.height[10];
      int angle = slot.fullDisplayAngle;
      vec1.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

      if (shape == 0.0f) {
        angle += angle < 90 ? 270 : -90;
      } else if (shape == 1.0f) {
        angle += angle < 60 ? 300 : -60;
      } else if (shape == 2.0f) {
        angle -= 130;
        if (angle < 0) {
          angle += 360;
        }
      }
      vec2.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

      if (shape == 0.0f) {
        angle -= 90;
        if (angle < 0) {
          angle += 360;
        }
      } else if (shape == 1.0f) {
        angle -= 120;
        if (angle < 0) {
          angle += 360;
        }
      } else if (shape == 2.0f) {
        angle -= 50;
        if (angle < 0) {
          angle += 360;
        }
      }
      vec3.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

      if (shape == 0.0f) {
        angle -= 90;
        if (angle < 0) {
          angle += 360;
        }
      } else if (shape == 1.0f) {
        angle -= 60;
        if (angle < 0) {
          angle += 360;
        }
      } else if (shape == 2.0f) {
        angle -= 130;
        if (angle < 0) {
          angle += 360;
        }
      }
      vec4.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

      const vector3d& position = this->render.position;
      const float deltaX = slot.vecBNow.x - position.x;
      const float deltaZ = slot.vecBNow.z - position.z;
      const float along = SquareRoot(deltaX * deltaX + deltaZ * deltaZ) / 12.0f * step;
      const float x = along * g_cosTable[slot.riseAngle] + position.x;
      const float y = position.y - 12.0f + 0.0f;
      const float z = along * g_sinTable[slot.riseAngle] + position.z;
      const vector3d p1{x + vec1.x, y + vec1.y, z + vec1.z};
      const vector3d p2{x + vec2.x, y + vec2.y, z + vec2.z};
      const vector3d p3{x + vec3.x, y + vec3.y, z + vec3.z};
      const vector3d p4{x + vec4.x, y + vec4.y, z + vec4.z};

      switch (this->state.slots[0].flag1[2]) {
      case 0:
      case 3:
        this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p2, p3, p4, wtm, 1, static_cast<unsigned char>(slot.flag1[0]), 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 4:
        this->RenderTeiRect(
          &rpCnt, slot.alphaB, p2, p1, p4, p3, wtm, 0, static_cast<short>(i + slot.flag1[0]) % static_cast<int>(this->state.totalTexture), 1, 255, 150, 150, 0.0f, 1.0f, 0.0f, 1.0f, 0
        );
        break;
      }
    }
  }
}

// OFFSET: 0x00BDD370
// FUNCTION: fn_19901
void CEffectPrim::RenderTube(matrix& wtm) {
  const EffectPrimitiveSlot& slot = this->state.slots[0];
  int rpCnt = 0;

  if (slot.life != 1 || slot.alphaB <= 0) {
    return;
  }

  const float deltaX = slot.vecTNow.x - slot.vecTPre.x;
  const float deltaZ = slot.vecTNow.z - slot.vecTPre.z;
  const float length = SquareRoot(deltaX * deltaX + deltaZ * deltaZ);
  short heading = 0;
  while (deltaX / length < g_cosTable[heading]) {
    if (++heading > 180) {
      break;
    }
  }
  if (heading > 180) {
    heading = 180;
  } else if (deltaZ / length < 0.0f) {
    heading = static_cast<short>(360 - heading);
  }
  short side = static_cast<short>(heading + 90);
  if (side >= 360) {
    side = static_cast<short>(side - 360);
  }

  vector3d end;
  if (slot.process >= 15) {
    end = slot.vecTPre;
  } else {
    const float t = g_sinTable[6 * slot.process];
    end = {
      (slot.vecTPre.x - slot.vecTNow.x) * t + slot.vecTNow.x, (slot.vecTPre.y - slot.vecTNow.y) * t + slot.vecTNow.y,
      (slot.vecTPre.z - slot.vecTNow.z) * t + slot.vecTNow.z
    };
  }

  vector3d prevStart{};
  vector3d prevEnd{};
  short alpha = slot.alphaB;

  for (int pass = 0; pass < 2; ++pass) {
    const float radius = pass == 0 ? slot.maxHeight : slot.maxHeight + slot.maxHeight;

    for (int step = 0; step <= 10; ++step) {
      const int angle = step * 36;
      const float across = radius * g_cosTable[angle];
      const float up = radius * g_sinTable[angle];
      const float offsetX = across * g_cosTable[side];
      const float offsetZ = across * g_sinTable[side];
      const vector3d start{offsetX + slot.vecTNow.x, up + slot.vecTNow.y, offsetZ + slot.vecTNow.z};
      const vector3d finish{end.x + offsetX, end.y + up, end.z + offsetZ};

      if (step > 0) {
        const float mode = slot.height[4];
        if (pass == 0) {
          alpha = slot.alphaB;
          if (mode == 2.0f) {
            this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 0, 0, 1, 0, 100, 150, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          } else if (mode == 3.0f) {
            this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 0, 0, 1, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          } else if (mode == 4.0f) {
            alpha = static_cast<short>(slot.alphaB / 2);
            this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 1, 0, 1, 50, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          } else {
            this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 0, 0, 1, 255, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          }
        } else if (mode == 2.0f) {
          alpha = static_cast<short>(2 * slot.alphaB);
          this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 0, 0, 1, 255, 50, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (mode == 3.0f) {
          alpha = static_cast<short>(2 * slot.alphaB);
          this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 0, 0, 1, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (mode == 4.0f) {
          this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 1, 0, 1, 50, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          alpha = static_cast<short>(2 * slot.alphaB);
          this->RenderTeiRect(&rpCnt, alpha, prevStart, start, finish, prevEnd, wtm, 0, 0, 1, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }

      prevStart = start;
      prevEnd = finish;
    }
  }
}

// OFFSET: 0x00BD4360
// FUNCTION: fn_19880
void CEffectPrim::RenderFieldPanel(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const bool field = std::string(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->mapName) == "ecl_fild01.rsw";
    if (field && slot.alphaB > 100) {
      slot.alphaB = 100;
    }

    if (slot.height[8] >= 360.0f || slot.height[8] < 0.0f) {
      slot.height[8] = 0.0f;
    }

    const float rx = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
    const int angle1 = slot.fullDisplayAngle;
    const int angle2 = angle1 + (angle1 < 270 ? 90 : -270);
    const int angle3 = angle2 + (angle2 + 90 < 360 ? 90 : -270);
    const int angle4 = angle3 + (angle3 + 90 < 360 ? 90 : -270);
    vec1 = {rx * g_cosTable[angle1], 0.0f, rx * g_sinTable[angle1]};
    vec2 = {rx * g_cosTable[angle2], 0.0f, rx * g_sinTable[angle2]};
    vec3 = {rx * g_cosTable[angle3], 0.0f, rx * g_sinTable[angle3]};
    vec4 = {rx * g_cosTable[angle4], 0.0f, rx * g_sinTable[angle4]};
    if (slot.height[9] == 10.0f) {
      vec1.x += 4.0f;
      vec2.x += 4.0f;
      vec3.x += 4.0f;
      vec4.x += 4.0f;
    }
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    vec1 = {slot.vecBNow.x + vec1.x, slot.vecBNow.y + vec1.y, slot.vecBNow.z + vec1.z};
    vec2 = {slot.vecBNow.x + vec2.x, slot.vecBNow.y + vec2.y, slot.vecBNow.z + vec2.z};
    vec3 = {slot.vecBNow.x + vec3.x, slot.vecBNow.y + vec3.y, slot.vecBNow.z + vec3.z};
    vec4 = {slot.vecBNow.x + vec4.x, slot.vecBNow.y + vec4.y, slot.vecBNow.z + vec4.z};

    const unsigned char texture = static_cast<unsigned char>(slot.flag1[0]);
    switch (this->state.slots[0].flag1[2]) {
      case 0:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 1, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 1:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, texture, 1, 150, 150, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 2:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 130, 130, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 3:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 200, 200, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 4:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 7:
      case 23:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 8:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 135, 255, 135, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 10:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 250, 250, 150, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 11:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 220, 250, 220, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 12:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 252, 171, 143, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 13:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 250, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 14:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 94, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 15:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 1, texture, 1, 255, 200, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 16:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 0, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 17:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 255, 180, 180, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 18:
        this->RenderTeiRect(&rpCnt, static_cast<short>(slot.alphaB / 2), vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 24:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 63, 72, 204, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 9:
        switch (this->state.slots[0].flag1[3]) {
        case 1:
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 255, 135, 135, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          break;
        case 2:
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 255, 205, 135, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          break;
        case 3:
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 255, 255, 135, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          break;
        case 4:
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          break;
        case 5:
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, texture, 1, 135, 135, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          break;
        case 6:
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          break;
        default:
          break;
        }
        break;
      case 25:
        vec1.y -= 110.0f;
        vec2.y -= 110.0f;
        vec3.y -= 110.0f;
        vec4.y -= 110.0f;
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, texture, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      default:
        break;
    }
  }
}

// OFFSET: 0x00BD4E40
// FUNCTION: fn_19881
void CEffectPrim::RenderBladeTrail(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float rx = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
    const float shape = slot.height[10];
    int angle = slot.fullDisplayAngle;
    vec1.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

    if (shape == 0.0f) {
      angle += angle < 90 ? 270 : -90;
    } else if (shape == 1.0f) {
      angle += angle < 60 ? 300 : -60;
    } else if (shape == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    vec2.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

    if (shape == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 1.0f) {
      angle -= 120;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 2.0f) {
      angle -= 50;
      if (angle < 0) {
        angle += 360;
      }
    }
    vec3.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

    if (shape == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    vec4.Set(rx * g_cosTable[angle], 0.0f, rx * g_sinTable[angle]);

    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    int trail = 0;
    for (int i = 0; i < 10; i += 2) {
      const vector3d& position = this->render.position;
      const float deltaX = slot.vecBNow.x - position.x;
      const float deltaZ = slot.vecBNow.z - position.z;
      const float length = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
      short alpha = slot.alphaB;
      const float along = std::max(0.0f, length / static_cast<float>(this->state.slots[0].flag1[5]) * static_cast<float>(slot.process) - static_cast<float>(i));
      const float z = along * g_sinTable[slot.riseAngle] + position.z;
      const float x = along * g_cosTable[slot.riseAngle] + position.x;
      const float y = position.y - 12.0f + 0.0f;
      const vector3d p2{x + vec2.x, y + vec2.y, z + vec2.z};
      const vector3d p1{x + vec1.x, y + vec1.y, z + vec1.z};
      const vector3d p4{x + vec4.x, y + vec4.y, z + vec4.z};
      const vector3d p3{x + vec3.x, y + vec3.y, z + vec3.z};

      if (trail > 0) {
        alpha = static_cast<short>(alpha + 20 * (-5 - trail));
        if (alpha < 0) {
          alpha = 0;
        }
      }

      unsigned char tint;
      switch (this->state.slots[0].flag1[2]) {
      case 0:
        tint = 55;
        break;
      case 1:
        tint = 105;
        break;
      case 2:
        tint = 75;
        break;
      case 3:
        tint = 85;
        break;
      default:
        ++trail;
        continue;
      }

      this->RenderTeiRect(&rpCnt, alpha, p2, p1, p4, p3, wtm, 0, static_cast<unsigned char>(slot.flag1[0]), 1, tint, tint, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      ++trail;
    }
  }
}

// OFFSET: 0x00BD0CC0
// FUNCTION: fn_19871
void CEffectPrim::RenderFanBeam(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float mode = slot.height[4];
    const float length = slot.distance;
    const float lift = slot.height[0];
    const float rotCos = g_cosTable[slot.rotStart];
    const float rotSin = g_sinTable[slot.rotStart];
    const vector3d center{0.0f, lift, 0.0f};

    int spread = mode == 2.0f ? 1 : 2;
    int left = slot.riseAngle - spread;
    if (left < 0) {
      left += 360;
    }
    int right = slot.riseAngle + spread;
    if (right >= 360) {
      right -= 360;
    }
    vector3d vec1{rotCos * (length * g_cosTable[left]), rotSin * (length * g_cosTable[left]) + lift, length * g_sinTable[left]};
    vector3d vec2{length * g_cosTable[right] * rotCos, length * g_cosTable[right] * rotSin + lift, length * g_sinTable[right]};

    if (mode == 2.0f || mode == 4.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 0, 0, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 3.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 250, 250, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 5.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 50, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 6.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 0, 250, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }

    spread = mode == 2.0f ? 2 : 5;
    left = slot.riseAngle - spread;
    if (left < 0) {
      left += 360;
    }
    right = slot.riseAngle + spread;
    if (right >= 360) {
      right -= 360;
    }
    vec1 = {rotCos * (length * g_cosTable[left]), rotSin * (length * g_cosTable[left]) + lift, length * g_sinTable[left]};
    vec2 = {length * g_cosTable[right] * rotCos, length * g_cosTable[right] * rotSin + lift, length * g_sinTable[right]};

    if (mode == 2.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 250, 250, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 3.0f || mode == 5.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 1, 0, 0, 250, 250, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 4.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 1, 0, 0, 250, 250, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 6.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, center, center, wtm, 0, 0, 0, 250, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE2DB0
// FUNCTION: fn_19921
void CEffectPrim::RenderStackedRings(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    float y = -slot.height[0];
    float radius = slot.maxHeight;

    for (int i = 0; i < slot.flag1[0]; ++i) {
      if (i > 0) {
        y += slot.height[1];
        radius -= (2.2f - static_cast<float>(i) * 0.2f) * slot.distance;
      }

      short angle1 = static_cast<short>(slot.rotStart + 45 * i);
      if (angle1 >= 360) {
        angle1 = static_cast<short>(static_cast<unsigned short>(angle1) % 360u);
      }
      short angle2 = static_cast<short>(angle1 + 90);
      if (angle2 >= 360) {
        angle2 = static_cast<short>(angle2 - 360);
      }
      const short angle3 = static_cast<short>(angle2 + 90 < 360 ? angle2 + 90 : angle2 - 270);
      const short angle4 = static_cast<short>(angle3 + 90 < 360 ? angle3 + 90 : angle3 - 270);
      const vector3d vec1{radius * g_cosTable[angle1], y - 1.0f, radius * g_sinTable[angle1]};
      const vector3d vec2{radius * g_cosTable[angle2], y - 1.0f, radius * g_sinTable[angle2]};
      const vector3d vec3{radius * g_cosTable[angle3], y, radius * g_sinTable[angle3]};
      const vector3d vec4{radius * g_cosTable[angle4], y, radius * g_sinTable[angle4]};

      switch (slot.flag1[4]) {
      case 0:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 1:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 164, 136, 55, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 2:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 155, 255, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 3:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 130, 130, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 4:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 5:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 5, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 6:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 7:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 125, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 8:
        this->RenderTeiRect(&rpCnt, static_cast<short>(slot.alphaB / 10), vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 225, 225, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 10:
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 125, 125, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      case 11:
        this->RenderTeiRect(&rpCnt, static_cast<short>(slot.alphaB / 3), vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 55, 55, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        break;
      default:
        break;
      }
    }
  }
}

// OFFSET: 0x00BD71B0
// FUNCTION: fn_19885
void CEffectPrim::RenderLightningTube(matrix& wtm) {
  vector3d prevStart{};
  vector3d prevEnd{};
  int rpCnt = 0;
  float jitterZ[21];
  float jitterX[21];
  float jitterY[21];

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    const float count = slot.vecTNow.y;

    if (count > 0.0f) {
      int i = 0;
      do {
        const int y = std::rand() % 11;
        jitterY[i] = static_cast<float>(y >= 0 ? y : 0) * 0.1f - 0.5f;
        const int x = std::rand() % 11;
        jitterX[i] = static_cast<float>(x >= 0 ? x : 0) * 0.1f - 0.5f;
        const int z = std::rand() % 11;
        jitterZ[i] = static_cast<float>(z >= 0 ? z : 0) * 0.1f - 0.5f;
        ++i;
      } while (slot.vecTNow.y > static_cast<float>(i));
    }

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0 || slot.vecTNow.y <= 0.0f) {
      continue;
    }

    int fade = 150;
    int rise = 0;
    int segment = 0;
    do {
      const int next = segment + 1;
      const float sn = g_sinTable[slot.riseAngle];
      const float cs = g_cosTable[slot.riseAngle];
      const float t0 = segment > 0 ? static_cast<float>(segment) / 5.0f * slot.distance + 0.0f : 0.0f;
      const float s0 = sn * t0 + jitterZ[segment];
      const float y0 = jitterY[segment] - 8.0f - g_sinTable[static_cast<short>(static_cast<int>(slot.height[segment]))] * 15.0f * slot.vecTNow.x;
      const float c0 = cs * t0 + jitterX[segment];
      const float t1 = static_cast<float>(next) / 5.0f * slot.distance + 0.0f;
      const float s1 = sn * t1 + jitterZ[next];
      const float c1 = cs * t1 + jitterX[next];
      const float y1 = jitterY[next] - 8.0f - g_sinTable[static_cast<short>(static_cast<int>(slot.height[next]))] * 15.0f * slot.vecTNow.x;
      const float deltaC = c0 - c1;
      const float deltaS = s0 - s1;
      const float length = SquareRoot(deltaC * deltaC + deltaS * deltaS);
      short heading = 0;
      while (deltaC / length < g_cosTable[heading]) {
        if (++heading > 180) {
          break;
        }
      }
      if (heading > 180) {
        heading = 180;
      } else if (deltaS / length < 0.0f) {
        heading = static_cast<short>(360 - heading);
      }
      short side = static_cast<short>(heading + 90);
      if (side >= 360) {
        side = static_cast<short>(side - 360);
      }

      const vector3d start{slot.vecBNow.x + c0, slot.vecBNow.y + y0, slot.vecBNow.z + s0};
      const vector3d end{slot.vecBNow.x + c1, slot.vecBNow.y + y1, slot.vecBNow.z + s1};

      if (static_cast<unsigned>(next - 2) <= 17) {
        int ring = 0;
        for (int angle = 0; angle <= 360; angle += 120) {
          const float across = slot.maxHeight * g_cosTable[angle];
          const float up = slot.maxHeight * g_sinTable[angle];
          const float offsetZ = across * g_sinTable[side];
          const float offsetX = across * g_cosTable[side];
          const vector3d ringEnd{offsetX + end.x, end.y + up, offsetZ + end.z};
          const vector3d ringStart{offsetX + start.x, up + start.y, offsetZ + start.z};

          if (ring > 0) {
            const int alpha = (fade > 0 ? rise - 135 : fade) + slot.alphaB;
            if (alpha > 0) {
              const int green = std::rand() % 121;
              const int red = std::rand() % 121;
              this->RenderTeiRect(
                &rpCnt, static_cast<short>(alpha), ringStart, ringEnd, prevEnd, prevStart, wtm, 0, 0, 1, static_cast<unsigned char>(red >= 0 ? red : 0),
                static_cast<unsigned char>(green >= 0 ? green : 0), 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
              );
            }
          }

          ++ring;
          prevStart = ringStart;
          prevEnd = ringEnd;
        }
      }

      segment = next;
      fade -= 15;
      rise += 15;
    } while (slot.vecTNow.y > static_cast<float>(segment));
  }
}

// OFFSET: 0x00BDC3E0
// FUNCTION: fn_19898
void CEffectPrim::RenderOrbitSquare(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float distance = slot.distance;
    const float baseX = g_cosTable[slot.fullDisplayAngle] * distance - g_sinTable[slot.fullDisplayAngle] * 0.0f;
    const float offsetY = g_sinTable[slot.fullDisplayAngle] * distance + g_cosTable[slot.fullDisplayAngle] * 0.0f;
    const float offsetX = baseX * g_cosTable[slot.riseAngle] - g_sinTable[slot.riseAngle] * 0.0f;
    const float offsetZ = baseX * g_sinTable[slot.riseAngle] + g_cosTable[slot.riseAngle] * 0.0f;

    const float radius = slot.height[1];
    const int angle1 = slot.rotStart;
    const int tilt = static_cast<int>(slot.height[2]);
    const float sn = g_sinTable[tilt];
    const float cs = g_cosTable[tilt];
    const int angle2 = angle1 + 90 < 360 ? angle1 + 90 : angle1 - 270;
    const int angle3 = angle2 + 90 < 360 ? angle2 + 90 : angle2 - 270;
    const int angle4 = angle3 + 90 < 360 ? angle3 + 90 : angle3 - 270;
    const char kind = slot.flag1[0];

    if (kind == 2 || kind == 8 || kind == 9) {
      const int face1 = angle1 + 90 < 360 ? angle1 + 90 : angle1 - 270;
      const int face2 = face1 + 90 < 360 ? face1 + 90 : face1 - 270;
      const int face3 = face2 + 90 < 360 ? face2 + 90 : face2 - 270;
      vec1.Set(radius * g_cosTable[angle1], 0.0f, radius * g_sinTable[angle1]);
      vec2.Set(radius * g_cosTable[face1], 0.0f, radius * g_sinTable[face1]);
      vec3.Set(radius * g_cosTable[face2], 0.0f, radius * g_sinTable[face2]);
      vec4.Set(radius * g_cosTable[face3], 0.0f, radius * g_sinTable[face3]);
      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    } else {
      vec1.Set(cs * (radius * g_cosTable[angle1]) - sn * 0.0f, radius * g_sinTable[angle1], sn * (radius * g_cosTable[angle1]) + cs * 0.0f);
      vec2.Set(cs * (radius * g_cosTable[angle2]) - sn * 0.0f, radius * g_sinTable[angle2], sn * (radius * g_cosTable[angle2]) + cs * 0.0f);
      vec3.Set(cs * (radius * g_cosTable[angle3]) - sn * 0.0f, radius * g_sinTable[angle3], sn * (radius * g_cosTable[angle3]) + cs * 0.0f);
      vec4.Set(cs * (radius * g_cosTable[angle4]) - sn * 0.0f, radius * g_sinTable[angle4], sn * (radius * g_cosTable[angle4]) + cs * 0.0f);
    }

    const vector3d& position = slot.vecBNow;
    vec1.Set(offsetX + vec1.x + position.x, offsetY + vec1.y + position.y, offsetZ + vec1.z + position.z);
    vec2.Set(offsetX + vec2.x + position.x, offsetY + vec2.y + position.y, offsetZ + vec2.z + position.z);
    vec3.Set(offsetX + vec3.x + position.x, offsetY + vec3.y + position.y, offsetZ + vec3.z + position.z);
    vec4.Set(offsetX + vec4.x + position.x, offsetY + vec4.y + position.y, offsetZ + vec4.z + position.z);

    switch (kind) {
    case 0:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 100, 150, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 1:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 250, 250, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 250, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 3:
    case 8:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 4:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 255, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 5:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 50, 255, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 6:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 250, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 7:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 9:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 70, 70, 70, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    default:
      break;
    }
  }
}

// OFFSET: 0x00BD2FC0
// FUNCTION: fn_19877
void CEffectPrim::RenderGroundBar(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int angle = slot.riseAngle;
    const float width = slot.maxHeight;
    const int back = angle + (angle >= 90 ? -90 : 270);
    const float baseX = slot.height[0] * g_cosTable[back];
    const float baseZ = slot.height[0] * g_sinTable[back];
    const float farX = width * g_cosTable[back] + baseX;
    const float farZ = width * g_sinTable[back] + baseZ;

    const float start = slot.height[1];
    float startX;
    float startZ;
    if (100.0f <= start) {
      const float reach = start - slot.distance;
      startX = reach * g_cosTable[angle];
      startZ = reach * g_sinTable[angle];
    } else {
      const int opposite = angle < 180 ? angle + 180 : angle - 180;
      const float reach = slot.distance - start;
      startX = reach * g_cosTable[opposite];
      startZ = reach * g_sinTable[opposite];
    }

    const float end = start + slot.height[2];
    float endX;
    float endZ;
    if (100.0f <= end) {
      const float reach = end - slot.distance;
      endX = reach * g_cosTable[angle];
      endZ = reach * g_sinTable[angle];
    } else {
      const int opposite = angle + (angle >= 180 ? -180 : 180);
      const float reach = slot.distance - end;
      endX = reach * g_cosTable[opposite];
      endZ = reach * g_sinTable[opposite];
    }

    const int front = angle + (angle < 270 ? 90 : -270);
    const float nearX = width * g_cosTable[front] + baseX;
    const float nearZ = width * g_sinTable[front] + baseZ;
    const vector3d vec1{nearX + endX, -6.0f, nearZ + endZ};
    const vector3d vec2{nearX + startX, -6.0f, nearZ + startZ};
    const vector3d vec3{startX + farX, -6.0f, startZ + farZ};
    const vector3d vec4{endX + farX, -6.0f, endZ + farZ};

    switch (slot.flag1[0]) {
    case 0:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 225, 225, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 1:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 185, 185, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 10:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 11:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 205, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 12:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 185, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 20:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 225, 255, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 21:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 205, 255, 205, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 22:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 185, 255, 185, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 30:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 31:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 65, 65, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 32:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 25, 25, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 33:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 103, 33, 177, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 34:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 66, 1, 104, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 35:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 178, 136, 236, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 36:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 241, 225, 178, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 37:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 202, 77, 65, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 38:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 148, 79, 73, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      break;
    case 125:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 126:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 215, 215, 215, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 127:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 185, 185, 185, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    default:
      break;
    }
  }
}

// OFFSET: 0x00BD7910
// FUNCTION: fn_19886
void CEffectPrim::RenderLightningArc(matrix& wtm) {
  vector3d prevStart{};
  vector3d prevEnd{};
  int rpCnt = 0;
  float jitterZ[21];
  float jitterX[21];
  float jitterY[21];

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.vecTNow.y > 0.0f) {
      int i = 0;
      do {
        const int y = std::rand() % 7;
        jitterY[i] = static_cast<float>(y >= 0 ? y : 0) * 0.1f - 0.3f;
        const int x = std::rand() % 7;
        jitterX[i] = static_cast<float>(x >= 0 ? x : 0) * 0.1f - 0.3f;
        const int z = std::rand() % 7;
        jitterZ[i] = static_cast<float>(z >= 0 ? z : 0) * 0.1f - 0.3f;
        ++i;
      } while (slot.vecTNow.y > static_cast<float>(i));
    }

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0 || slot.vecTNow.y <= 0.0f) {
      continue;
    }

    int fade = 0;
    int segment = 0;
    do {
      const float cs = g_cosTable[slot.riseAngle];
      const float sn = g_sinTable[slot.riseAngle];
      const float a0 = slot.height[segment];
      const float r0 = a0 > 0.0f ? a0 / 90.0f * slot.distance + 0.0f : 0.0f;
      const float c0 = cs * r0 + jitterX[segment];
      const float y0 = -slot.distance * g_sinTable[static_cast<short>(static_cast<int>(a0))] * slot.vecTNow.x + (jitterY[segment] - 2.0f);
      const float s0 = sn * r0 + jitterZ[segment];
      const float a1 = slot.height[segment + 1];
      const float r1 = a1 > 0.0f ? a1 / 90.0f * slot.distance + 0.0f : 0.0f;
      const float s1 = sn * r1 + jitterZ[segment + 1];
      const float y1 = -slot.distance * g_sinTable[static_cast<short>(static_cast<int>(a1))] * slot.vecTNow.x + (jitterY[segment + 1] - 2.0f);
      const float c1 = cs * r1 + jitterX[segment + 1];
      const float deltaS = s0 - s1;
      const float deltaC = c0 - c1;
      const float length = SquareRoot(deltaC * deltaC + deltaS * deltaS);
      short heading = 0;
      while (deltaC / length < g_cosTable[heading]) {
        if (++heading > 180) {
          break;
        }
      }
      if (heading > 180) {
        heading = 180;
      } else if (deltaS / length < 0.0f) {
        heading = static_cast<short>(360 - heading);
      }
      short side = static_cast<short>(heading + 90);
      if (side >= 360) {
        side = static_cast<short>(side - 360);
      }

      const vector3d start{slot.vecBNow.x + c0, slot.vecBNow.y + y0, slot.vecBNow.z + s0};
      const vector3d end{slot.vecBNow.x + c1, slot.vecBNow.y + y1, slot.vecBNow.z + s1};

      if (static_cast<unsigned>(segment - 1) <= 17) {
        int ring = 0;
        for (int angle = 0; angle <= 360; angle += 120) {
          const float across = slot.maxHeight * g_cosTable[angle];
          const float up = slot.maxHeight * g_sinTable[angle];
          const float offsetX = across * g_cosTable[side];
          const float offsetZ = across * g_sinTable[side];
          const vector3d ringStart{offsetX + start.x, up + start.y, offsetZ + start.z};
          const vector3d ringEnd{offsetX + end.x, up + end.y, offsetZ + end.z};

          if (ring > 0) {
            const int alpha = slot.alphaB - fade;
            if (alpha > 0) {
              unsigned char red;
              unsigned char green;
              if (slot.alphaT == 3) {
                const int first = std::rand() % 100;
                const int second = std::rand() % 50;
                green = static_cast<unsigned char>(first >= 0 ? first : 0);
                red = static_cast<unsigned char>((second >= 0 ? second : 0) - 56);
              } else {
                red = static_cast<unsigned char>(6 * segment);
                green = red;
              }
              this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), ringStart, ringEnd, prevEnd, prevStart, wtm, 0, 0, 1, red, green, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }

          ++ring;
          prevStart = ringStart;
          prevEnd = ringEnd;
        }
      }

      ++segment;
      fade += 5;
    } while (slot.vecTNow.y > static_cast<float>(segment));
  }
}

// OFFSET: 0x00BCFE80
// FUNCTION: fn_19868
void CEffectPrim::RenderHaloLayers(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    for (int i = 10; i >= 1; --i) {
      const char mode = slot.flag1[4];
      const bool narrow = mode == 6 || mode == 7;
      const int angle1 = narrow ? 35 : 45;
      const int angle2 = narrow ? 145 : 135;
      int step = 10 - i;
      if (mode != 2 && mode != 4 && mode != 5) {
        step *= 3;
      }
      const float radius = static_cast<float>(step) + slot.distance;
      slot.vecBPre = {radius * g_cosTable[angle1], 0.0f, g_sinTable[angle1] * radius};
      slot.vecBNow = {radius * g_cosTable[angle2], 0.0f, g_sinTable[angle2] * radius};
      unsigned angle3 = static_cast<unsigned>(angle2 + (narrow ? 70 : 90));
      if (angle3 >= 360) {
        angle3 -= 360;
      }
      slot.vecTNow = {radius * g_cosTable[angle3], 0.0f, g_sinTable[angle3] * radius};
      int angle4 = static_cast<int>(angle3) + (narrow ? 110 : 90);
      if (angle4 >= 360) {
        angle4 -= 360;
      }
      slot.vecTPre = {radius * g_cosTable[angle4], 0.0f, g_sinTable[angle4] * radius};

      vector3d vec1{slot.height[0] + slot.vecBPre.x, 0.0f, slot.vecBPre.z};
      vector3d vec2{slot.height[0] + slot.vecBNow.x, 0.0f, slot.vecBNow.z};
      vector3d vec3{slot.height[0] + slot.vecTNow.x, 0.0f, slot.vecTNow.z};
      vector3d vec4{slot.height[0] + slot.vecTPre.x, 0.0f, slot.vecTPre.z};
      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
      const float lift = mode == 2 || mode == 4 || mode == 6 || mode == 7 ? 40.0f : 30.0f;
      vec1.y -= lift;
      vec2.y -= lift;
      vec4.y -= lift;
      vec3.y -= lift;

      const int alpha = static_cast<int>(slot.height[i]);
      if (alpha > 0) {
        const unsigned char texture = static_cast<unsigned char>(slot.flag1[0]);
        if (mode == 0) {
          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec2, vec1, vec4, vec3, wtm, 1, texture, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (mode == 3) {
          if (i == 10) {
            this->RenderTeiRect(&rpCnt, static_cast<short>(alpha / 2), vec2, vec1, vec4, vec3, wtm, 1, texture, 0, 10, 10, 10, 0.0f, 1.0f, 0.0f, 1.0f, 0);
          }
          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec2, vec1, vec4, vec3, wtm, 1, texture, 0, 10, 10, 10, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec2, vec1, vec4, vec3, wtm, narrow ? 0 : 1, texture, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }

      if (mode == 5 || mode == 6 || mode == 7) {
        slot.vecBPre = {slot.vecBPre.x * 1.1f, slot.vecBPre.y * 1.1f, slot.vecBPre.z * 1.1f};
        slot.vecBNow = {slot.vecBNow.x * 1.1f, slot.vecBNow.y * 1.1f, slot.vecBNow.z * 1.1f};
        slot.vecTPre = {slot.vecTPre.x * 1.1f, slot.vecTPre.y * 1.1f, slot.vecTPre.z * 1.1f};
        slot.vecTNow = {slot.vecTNow.x * 1.1f, slot.vecTNow.y * 1.1f, slot.vecTNow.z * 1.1f};
        vec1 = {slot.vecBPre.x + slot.height[0], slot.vecBPre.y, slot.vecBPre.z};
        vec2 = {slot.vecBNow.x + slot.height[0], slot.vecBNow.y, slot.vecBNow.z};
        vec3 = {slot.vecTNow.x + slot.height[0], slot.vecTNow.y, slot.vecTNow.z};
        vec4 = {slot.vecTPre.x + slot.height[0], slot.vecTPre.y, slot.vecTPre.z};
        this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
        const float outerLift = mode == 5 ? 30.0f : 40.0f;
        vec1.y -= outerLift;
        vec2.y -= outerLift;
        vec3.y -= outerLift;
        vec4.y -= outerLift;

        const int outerAlpha = static_cast<int>(slot.height[i]);
        if (outerAlpha > 0) {
          const unsigned char red = mode == 7 ? 255 : 5;
          const unsigned char blue = mode == 7 ? 5 : 255;
          this->RenderTeiRect(
            &rpCnt, static_cast<short>(outerAlpha), vec2, vec1, vec4, vec3, wtm, 0, static_cast<unsigned char>(slot.flag1[0]), 0, red, 5, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0
          );
        }
      }
    }
  }
}

// INLINED
static __forceinline void RenderGroundBubbleQuad(
  CEffectPrim& prim, int* rpCnt, const EffectPrimitiveSlot& slot, const vector3d& c1, const vector3d& c2, const vector3d& c3, const vector3d& c4,
  ZeroVector3d& vec1, ZeroVector3d& vec2, ZeroVector3d& vec3, ZeroVector3d& vec4, const vector3d& center, matrix& wtm
) {
  vec1.Set(c1.x + center.x, c1.y + center.y, c1.z + center.z);
  vec2.Set(c2.x + center.x, c2.y + center.y, c2.z + center.z);
  vec3.Set(c3.x + center.x, c3.y + center.y, c3.z + center.z);
  vec4.Set(c4.x + center.x, c4.y + center.y, c4.z + center.z);
  int alpha = 150;
  if (slot.flag1[1] != 2) {
    alpha = 130;
  }
  if (-1.0f > vec1.y) {
    alpha = static_cast<int>((vec1.y + 1.0f) * 4.0f + 130.0f);
  }
  if (alpha < 0) {
    alpha = 0;
  }
  if (vec1.y <= 0.0f) {
    if (slot.flag1[1] == 2) {
      prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 155, 255, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BDDBC0
// FUNCTION: fn_19903
__declspec(safebuffers) void CEffectPrim::RenderGroundBubbles(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vec1.Set(rx * g_cosTable[45], 0.0f, rx * g_sinTable[45]);
    vec2.Set(rx * g_cosTable[135], 0.0f, rx * g_sinTable[135]);
    vec3.Set(rx * g_cosTable[225], 0.0f, rx * g_sinTable[225]);
    vec4.Set(rx * g_cosTable[315], 0.0f, rx * g_sinTable[315]);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d c1{vec1.x, vec1.y, vec1.z};
    const vector3d c2{vec2.x, vec2.y, vec2.z};
    const vector3d c3{vec3.x, vec3.y, vec3.z};
    const vector3d c4{vec4.x, vec4.y, vec4.z};
    RenderGroundBubbleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecBPre, wtm);
    RenderGroundBubbleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecBNow, wtm);
    RenderGroundBubbleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecTPre, wtm);
    RenderGroundBubbleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecTNow, wtm);
  }
}

// INLINED
static __forceinline void BuildSphereCageQuad(const EffectPrimitiveSlot& slot, int latitude, int turn, float radius, float drop, vector3d* out) {
  int pitch = latitude + 360;
  if (pitch >= 360) {
    pitch -= 360;
  }
  if (pitch < 0) {
    pitch += 360;
  }
  int tilt = latitude + 450;
  if (tilt < 0) {
    tilt += 360;
  }
  int yaw = slot.rotStart + turn - 90;
  if (yaw >= 360) {
    yaw -= 360;
  }
  if (yaw < 0) {
    yaw += 360;
  }
  int side = slot.rotStart + turn - 180;
  if (side >= 360) {
    side -= 360;
  }
  if (side < 0) {
    side += 360;
  }

  const float reach = radius * g_cosTable[pitch];
  const vector3d offset{reach * g_cosTable[yaw], radius * g_sinTable[pitch] - drop, reach * g_sinTable[yaw]};
  const float inside = static_cast<float>(static_cast<int>(slot.distance));
  const float outside = static_cast<float>(static_cast<int>(-slot.distance));
  const float lateral[4] = {inside, outside, outside, inside};
  const float depth[4] = {inside, inside, outside, outside};
  for (int corner = 0; corner < 4; ++corner) {
    const float along = depth[corner] * g_cosTable[tilt];
    out[corner] = {
      lateral[corner] * g_cosTable[side] - along * g_sinTable[side] + offset.x, depth[corner] * g_sinTable[tilt] + offset.y,
      along * g_cosTable[side] + lateral[corner] * g_sinTable[side] + offset.z
    };
  }
}

// OFFSET: 0x00BD95A0
// FUNCTION: fn_19892
void CEffectPrim::RenderSphereCage(matrix& wtm) {
  vector3d quad[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    int row = 0;
    for (int latitude = -405; latitude < -270; latitude += 45, ++row) {
      const float level = static_cast<float>(row - 1);

      for (int turn = 0; turn < 225; turn += 45) {
        slot.distance = 1.5f - (level < 0.0f ? level * -1.0f : level) + 1.5f;

        BuildSphereCageQuad(slot, latitude, turn, slot.maxHeight, slot.maxHeight + 2.0f, quad);
        if (slot.flag1[0] == 3) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 100, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 2) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 155, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 4) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 150, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 5) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 100, 100, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 205, 255, 205, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }

        BuildSphereCageQuad(slot, latitude, turn, slot.maxHeight + 0.2f, slot.maxHeight, quad);
        if (slot.flag1[0] == 3) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 100, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 2) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 155, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 4) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 150, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 5) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 100, 100, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 155, 255, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }

        BuildSphereCageQuad(slot, latitude, turn, slot.maxHeight + 0.4f + slot.height[0], slot.maxHeight, quad);
        if (slot.flag1[0] == 3) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 1, 1, 0, 255, 100, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 2) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 1, 1, 0, 255, 155, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 4) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 1, 1, 0, 255, 150, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else if (slot.flag1[0] == 5) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 100, 100, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 1, 1, 0, 225, 255, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }
    }
  }
}

// INLINED
static __forceinline void BuildLayeredGlowSquare(CEffectPrim& prim, const EffectPrimitiveSlot& slot, float scale, vector3d* corners) {
  const float rx = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance * scale;
  corners[0] = {rx * g_cosTable[0], 0.0f, rx * g_sinTable[0]};
  corners[1] = {rx * g_cosTable[90], 0.0f, rx * g_sinTable[90]};
  corners[2] = {rx * g_cosTable[180], 0.0f, rx * g_sinTable[180]};
  corners[3] = {rx * g_cosTable[270], 0.0f, rx * g_sinTable[270]};
  prim.Tei_TowardScreen(corners[0], corners[1], corners[2], corners[3], slot.alphaT, prim.state.slots[1].alphaT);
  for (int corner = 0; corner < 4; ++corner) {
    corners[corner] = {slot.vecBNow.x + corners[corner].x, slot.vecBNow.y + corners[corner].y, slot.vecBNow.z + corners[corner].z};
  }
}

// OFFSET: 0x00BD1F40
// FUNCTION: fn_19874
void CEffectPrim::RenderLayeredGlow(matrix& wtm) {
  const EffectPrimitiveSlot& slot = this->state.slots[0];
  int rpCnt = 0;

  if (slot.life != 1 || slot.alphaB <= 0) {
    return;
  }

  vector3d quad[4];
  const char kind = slot.flag1[0];
  if (kind == 21) {
    BuildLayeredGlowSquare(*this, slot, 3.0f, quad);
    this->RenderTeiRect(&rpCnt, static_cast<short>(slot.alphaB / 2), quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    BuildLayeredGlowSquare(*this, slot, 1.7f, quad);
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (kind == 20) {
    BuildLayeredGlowSquare(*this, slot, 1.3f, quad);
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }

  BuildLayeredGlowSquare(*this, slot, 1.0f, quad);
  const short pw = this->state.isChangeProcessed == 1 ? 0 : 10;
  if (slot.flag1[0] == 1) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 68, 42, 30, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 10) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, pw, 0, 0, 255, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 11) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, pw, 0, 0, 200, 200, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 12) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, pw, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 13) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, pw, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 20) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 40, 40, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 21) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 40, 40, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }

  BuildLayeredGlowSquare(*this, slot, 0.5f, quad);
  if (slot.flag1[0] == 1) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 10) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 11) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 200, 200, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 12) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 13) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 100, 150, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 20) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 150, 150, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 21) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 250, 250, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    this->RenderTeiRect(&rpCnt, slot.alphaB, quad[0], quad[1], quad[2], quad[3], wtm, 0, 0, 0, 255, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD9FC0
// FUNCTION: fn_19893
void CEffectPrim::RenderOrbitIcons(matrix& wtm, float scale, unsigned char red, unsigned char green, unsigned char blue) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    if (slot.flag1[4] == 1) {
      red = 255;
      green = 20;
      blue = 20;
    }

    const int angle0 = static_cast<int>(slot.height[3]);
    slot.vecBPre.x = g_cosTable[angle0] * 4.0f;
    slot.vecBPre.z = g_sinTable[angle0] * 4.0f;
    const int angle1 = static_cast<int>(slot.height[7]);
    slot.vecBNow.x = g_cosTable[angle1] * 4.0f;
    slot.vecBNow.z = g_sinTable[angle1] * 4.0f;
    const int angle2 = static_cast<int>(slot.height[11]);
    slot.vecTPre.x = g_cosTable[angle2] * 4.0f;
    slot.vecTPre.z = g_sinTable[angle2] * 4.0f;
    const int angle3 = static_cast<int>(slot.height[15]);
    slot.vecTNow.x = g_cosTable[angle3] * 4.0f;
    slot.vecTNow.z = g_sinTable[angle3] * 4.0f;

    const float rx = scale * slot.distance;
    vec1 = {rx * g_cosTable[45], 0.0f, rx * g_sinTable[45]};
    vec2 = {rx * g_cosTable[135], 0.0f, rx * g_sinTable[135]};
    vec3 = {rx * g_cosTable[225], 0.0f, rx * g_sinTable[225]};
    vec4 = {rx * g_cosTable[315], 0.0f, rx * g_sinTable[315]};
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    const vector3d quad[4] = {vec1, vec2, vec3, vec4};

    const int delta0 = static_cast<int>(slot.height[3] - static_cast<float>(this->state.slots[0].alphaT));
    const bool back0 = (delta0 >= 70 || delta0 <= -80) && static_cast<unsigned>(delta0 + 270) <= 540;
    const float mode0 = slot.height[2];
    const bool flipped0 = mode0 == 0.0f || mode0 == 2.0f ? back0 : !back0;
    const vector3d corner00{(flipped0 ? vec2.x : vec1.x) + slot.vecBPre.x,
                            (flipped0 ? vec2.y : vec1.y) + slot.vecBPre.y - slot.height[20],
                            (flipped0 ? vec2.z : vec1.z) + slot.vecBPre.z};
    const vector3d corner01{(flipped0 ? vec1.x : vec2.x) + slot.vecBPre.x,
                            (flipped0 ? vec1.y : vec2.y) + slot.vecBPre.y - slot.height[20],
                            (flipped0 ? vec1.z : vec2.z) + slot.vecBPre.z};
    const vector3d corner02{(flipped0 ? vec4.x : vec3.x) + slot.vecBPre.x,
                            (flipped0 ? vec4.y : vec3.y) + slot.vecBPre.y - slot.height[20],
                            (flipped0 ? vec4.z : vec3.z) + slot.vecBPre.z};
    const vector3d corner03{(flipped0 ? vec3.x : vec4.x) + slot.vecBPre.x,
                            (flipped0 ? vec3.y : vec4.y) + slot.vecBPre.y - slot.height[20],
                            (flipped0 ? vec3.z : vec4.z) + slot.vecBPre.z};
    this->RenderTeiRect(&rpCnt, static_cast<short>(static_cast<int>(slot.height[1])), corner00, corner01, corner02, corner03, wtm, 0,
                        static_cast<unsigned char>(static_cast<int>(mode0)), 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 2);

    const int delta1 = static_cast<int>(slot.height[7] - static_cast<float>(this->state.slots[0].alphaT));
    const bool back1 = (delta1 >= 70 || delta1 <= -80) && static_cast<unsigned>(delta1 + 270) <= 540;
    const float mode1 = slot.height[6];
    const bool flipped1 = mode1 == 0.0f || mode1 == 2.0f ? back1 : !back1;
    const vector3d corner10{(flipped1 ? vec2.x : vec1.x) + slot.vecBNow.x,
                            (flipped1 ? vec2.y : vec1.y) + slot.vecBNow.y - slot.height[20],
                            (flipped1 ? vec2.z : vec1.z) + slot.vecBNow.z};
    const vector3d corner11{(flipped1 ? vec1.x : vec2.x) + slot.vecBNow.x,
                            (flipped1 ? vec1.y : vec2.y) + slot.vecBNow.y - slot.height[20],
                            (flipped1 ? vec1.z : vec2.z) + slot.vecBNow.z};
    const vector3d corner12{(flipped1 ? vec4.x : vec3.x) + slot.vecBNow.x,
                            (flipped1 ? vec4.y : vec3.y) + slot.vecBNow.y - slot.height[20],
                            (flipped1 ? vec4.z : vec3.z) + slot.vecBNow.z};
    const vector3d corner13{(flipped1 ? vec3.x : vec4.x) + slot.vecBNow.x,
                            (flipped1 ? vec3.y : vec4.y) + slot.vecBNow.y - slot.height[20],
                            (flipped1 ? vec3.z : vec4.z) + slot.vecBNow.z};
    this->RenderTeiRect(&rpCnt, static_cast<short>(static_cast<int>(slot.height[5])), corner10, corner11, corner12, corner13, wtm, 0,
                        static_cast<unsigned char>(static_cast<int>(mode1)), 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 2);

    const int delta2 = static_cast<int>(slot.height[11] - static_cast<float>(this->state.slots[0].alphaT));
    const bool back2 = (delta2 >= 70 || delta2 <= -80) && static_cast<unsigned>(delta2 + 270) <= 540;
    const float mode2 = slot.height[10];
    const bool flipped2 = mode2 == 0.0f || mode2 == 2.0f ? back2 : !back2;
    const vector3d corner20{(flipped2 ? vec2.x : vec1.x) + slot.vecTPre.x,
                            (flipped2 ? vec2.y : vec1.y) + slot.vecTPre.y - slot.height[20],
                            (flipped2 ? vec2.z : vec1.z) + slot.vecTPre.z};
    const vector3d corner21{(flipped2 ? vec1.x : vec2.x) + slot.vecTPre.x,
                            (flipped2 ? vec1.y : vec2.y) + slot.vecTPre.y - slot.height[20],
                            (flipped2 ? vec1.z : vec2.z) + slot.vecTPre.z};
    const vector3d corner22{(flipped2 ? vec4.x : vec3.x) + slot.vecTPre.x,
                            (flipped2 ? vec4.y : vec3.y) + slot.vecTPre.y - slot.height[20],
                            (flipped2 ? vec4.z : vec3.z) + slot.vecTPre.z};
    const vector3d corner23{(flipped2 ? vec3.x : vec4.x) + slot.vecTPre.x,
                            (flipped2 ? vec3.y : vec4.y) + slot.vecTPre.y - slot.height[20],
                            (flipped2 ? vec3.z : vec4.z) + slot.vecTPre.z};
    this->RenderTeiRect(&rpCnt, static_cast<short>(static_cast<int>(slot.height[9])), corner20, corner21, corner22, corner23, wtm, 0,
                        static_cast<unsigned char>(static_cast<int>(mode2)), 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 2);

    const int delta3 = static_cast<int>(slot.height[15] - static_cast<float>(this->state.slots[0].alphaT));
    const bool back3 = (delta3 >= 70 || delta3 <= -80) && static_cast<unsigned>(delta3 + 270) <= 540;
    const float mode3 = slot.height[14];
    const bool flipped3 = mode3 == 0.0f || mode3 == 2.0f ? back3 : !back3;
    const vector3d corner30{(flipped3 ? vec2.x : vec1.x) + slot.vecTNow.x,
                            (flipped3 ? vec2.y : vec1.y) + slot.vecTNow.y - slot.height[20],
                            (flipped3 ? vec2.z : vec1.z) + slot.vecTNow.z};
    const vector3d corner31{(flipped3 ? vec1.x : vec2.x) + slot.vecTNow.x,
                            (flipped3 ? vec1.y : vec2.y) + slot.vecTNow.y - slot.height[20],
                            (flipped3 ? vec1.z : vec2.z) + slot.vecTNow.z};
    const vector3d corner32{(flipped3 ? vec4.x : vec3.x) + slot.vecTNow.x,
                            (flipped3 ? vec4.y : vec3.y) + slot.vecTNow.y - slot.height[20],
                            (flipped3 ? vec4.z : vec3.z) + slot.vecTNow.z};
    const vector3d corner33{(flipped3 ? vec3.x : vec4.x) + slot.vecTNow.x,
                            (flipped3 ? vec3.y : vec4.y) + slot.vecTNow.y - slot.height[20],
                            (flipped3 ? vec3.z : vec4.z) + slot.vecTNow.z};
    this->RenderTeiRect(&rpCnt, static_cast<short>(static_cast<int>(slot.height[13])), corner30, corner31, corner32, corner33, wtm, 0,
                        static_cast<unsigned char>(static_cast<int>(mode3)), 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 2);
  }
}

// OFFSET: 0x00BDF0A0
// FUNCTION: fn_19908
void CEffectPrim::RenderRainbow(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;

    for (int rcolor = 0; rcolor < 7; ++rcolor) {
      int order = 0;
      const float inner = static_cast<float>(1.0 - rcolor * 0.05f);
      const float outer = static_cast<float>(1.0 - (rcolor + 1) * 0.05f);

      for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
        int angle = slot.rotStart + count;

        if (angle >= 360) {
          angle -= 360;
        }

        const float cs = g_cosTable[slot.riseAngle];
        const float sn = g_sinTable[slot.riseAngle];
        const float x1 = g_cosTable[angle] * slot.distance;
        const float y1 = g_sinTable[angle] * slot.distance;
        const vector3d vec1{cs * (x1 * inner), y1 * inner * 2.0f, sn * (x1 * inner)};
        const vector3d vec4{cs * (x1 * outer), y1 * outer * 2.0f, sn * (x1 * outer)};

        int next = angle + step;

        if (next >= 360) {
          next -= 360;
        }

        const float x2 = g_cosTable[next] * slot.distance;
        const float y2 = g_sinTable[next] * slot.distance;
        const vector3d vec2{cs * (x2 * inner), y2 * inner * 2.0f, sn * (x2 * inner)};
        const vector3d vec3{cs * (x2 * outer), y2 * outer * 2.0f, sn * (x2 * outer)};

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        switch (rcolor) {
        case 0:
          red = 255;
          green = 0;
          blue = 0;
          break;
        case 1:
          red = 255;
          green = 126;
          blue = 0;
          break;
        case 2:
          red = 255;
          green = 255;
          blue = 0;
          break;
        case 3:
          red = 0;
          green = 255;
          blue = 0;
          break;
        case 4:
          red = 0;
          green = 0;
          blue = 255;
          break;
        case 5:
          red = 115;
          green = 50;
          blue = 200;
          break;
        case 6:
          red = 255;
          green = 0;
          blue = 255;
          break;
        }

        this->RenderTeiRect(&rpCnt, order, slot.alphaB, slot.alphaB, vec2, vec3, vec4, vec1, wtm, 0, red, green, blue);

        ++order;

        if (order >= 21) {
          order = 20;
        }
      }
    }
  }
}

// OFFSET: 0x00BDF440
// FUNCTION: fn_19909
void CEffectPrim::RenderRectUp2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 2; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const bool distant = 10.0f <= slot.distance;
    const unsigned char r = distant ? 100 : 200;
    const unsigned char g = distant ? 100 : 250;
    const unsigned char b = distant ? 250 : 200;

    slot.vecBPre = {slot.height[0], -1.0f, slot.height[1]};
    slot.vecBNow = {slot.height[0], -1.0f, -slot.height[1]};
    slot.vecTNow = {-slot.height[0], -1.0f, -slot.height[1]};
    slot.vecTPre = {-slot.height[0], -1.0f, slot.height[1]};

    const float y = -1.0f - slot.height[2];
    const vector3d vec1{slot.vecBPre.x, y, slot.vecBPre.z};
    const vector3d vec2{slot.vecBNow.x, y, slot.vecBNow.z};
    const vector3d vec3{slot.vecTPre.x, y, slot.vecTPre.z};
    const vector3d vec4{slot.vecTNow.x, y, slot.vecTNow.z};

    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }

  for (int ec = 2; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    float left = 0.0f;

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const bool distant = 10.0f <= slot.distance;
    const unsigned char r = distant ? 50 : 150;
    const unsigned char g = distant ? 50 : 250;
    const unsigned char b = distant ? 250 : 150;
    const float w = slot.height[0];
    const float h = slot.height[1];
    const float dist = slot.distance;

    slot.vecBPre = {w, -1.0f, h};
    slot.vecBNow = {w, -1.0f, -h};
    slot.vecTNow = {-w, -1.0f, -h};
    slot.vecTPre = {-w, -1.0f, h};

    if (dist < w * 2.0f) {
      slot.vecBPre.z = h - dist;
      slot.vecBNow.z = slot.vecBPre.z - slot.height[3];

      if (slot.vecBNow.z < -h) {
        left = -h - slot.vecBNow.z;
        slot.vecBNow.z = -h;
        slot.vecTNow.x = slot.vecBNow.x - left;
      }
    } else if (dist < w * 4.0f) {
      slot.vecBNow.x = w - (dist - w * 2.0f);
      slot.vecTNow.x = slot.vecBNow.x - slot.height[3];

      if (slot.vecTNow.x < -w) {
        left = -w - slot.vecTNow.x;
        slot.vecTNow.x = -w;
        slot.vecTPre.z = slot.vecTNow.z + left;
      }
    } else if (dist < w * 6.0f) {
      slot.vecTNow.z = dist - w * 4.0f - h;
      slot.vecTPre.z = slot.vecTNow.z + slot.height[3];

      if (slot.vecTPre.z > h) {
        left = slot.vecTPre.z - h;
        slot.vecTPre.z = h;
        slot.vecBPre.x = slot.vecTPre.x + left;
      }
    } else {
      slot.vecTPre.x = dist - w * 6.0f - w;
      slot.vecBPre.x = slot.vecTPre.x + slot.height[3];

      if (slot.vecBPre.x > w) {
        left = slot.vecBPre.x - w;
        slot.vecBPre.x = w;
        slot.vecBNow.z = slot.vecBPre.z - left;
      }
    }

    const float y = -1.0f - slot.height[2];
    const vector3d vec1{slot.vecBPre.x, y, slot.vecBPre.z};
    const vector3d vec2{slot.vecBNow.x, y, slot.vecBNow.z};
    const vector3d vec3{slot.vecTPre.x, y, slot.vecTPre.z};
    const vector3d vec4{slot.vecTNow.x, y, slot.vecTNow.z};

    if (dist < w * 2.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else if (dist < w * 4.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else if (dist < w * 6.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BE0010
// FUNCTION: fn_19910
void CEffectPrim::RenderRectUp(matrix& wtm) {
  int rpCnt = 0;
  unsigned char r;
  unsigned char g;
  unsigned char b;
  short p;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float cs1 = g_cosTable[slot.rotStart];
    const float sn1 = g_sinTable[slot.rotStart];
    float rx = slot.height[0];
    float rz = slot.height[1];
    slot.vecBPre = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};
    rz = -slot.height[1];
    slot.vecBNow = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};
    rx = -slot.height[0];
    slot.vecTNow = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};
    rz = slot.height[1];
    slot.vecTPre = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};

    const float y = -1.0f - slot.height[2];
    const vector3d vec1{slot.vecBPre.x, y, slot.vecBPre.z};
    const vector3d vec2{slot.vecBNow.x, y, slot.vecBNow.z};
    const vector3d vec3{slot.vecTPre.x, y, slot.vecTPre.z};
    const vector3d vec4{slot.vecTNow.x, y, slot.vecTNow.z};

    if (slot.height[3] == 0.0f) {
      r = 255;
      g = 255;
      b = 255;
      p = 0;
    } else if (slot.height[3] == 1.0f) {
      r = 235;
      g = 255;
      b = 235;
      p = 1;
    } else if (slot.height[3] == 2.0f) {
      r = 80;
      g = 80;
      b = 80;
      p = 1;
    } else if (slot.height[3] == 4.0f) {
      r = 255;
      g = 255;
      b = static_cast<unsigned char>(static_cast<int>(slot.height[4] + 0.0f + slot.height[5]));
      p = 1;
    }

    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    if (slot.height[3] == 2.0f) {
      int alpha = slot.alphaB * 2;

      if (alpha > 255) {
        alpha = 255;
      }

      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), slot.vecBPre, slot.vecTNow, vec4, vec1, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), slot.vecBNow, slot.vecTPre, vec3, vec2, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE0680
// FUNCTION: fn_19911
void CEffectPrim::RenderSMA2(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = (slot.rotStart + count) % 360;

      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      const float rx = g_cosTable[slot.riseAngle] * slot.height[order];
      const float ry = g_sinTable[slot.riseAngle] * slot.height[order];
      const vector3d& delta = this->state.deltaPos;
      vec2.Set(cs1 * slot.distance + delta.x, delta.y + 0.0f, sn1 * slot.distance + delta.z);
      vec4.Set(cs1 * rx + cs1 * slot.distance + delta.x, delta.y - ry, sn1 * rx + sn1 * slot.distance + delta.z);

      if (order > 0) {
        const float tx1 = static_cast<float>(order - 1) / 21.0f;
        const float tx2 = static_cast<float>(order) / 21.0f;

        if (slot.alphaT == 99) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 99, 0, 1, 70, 70, 70, 0.0f, 1.0f, tx1, tx2, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, 90, 90, 255, 0.0f, 1.0f, tx1, tx2, 0);
        }
      }

      ++order;

      if (order >= 21) {
        order = 20;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}

// OFFSET: 0x00BE1150
// FUNCTION: fn_19915
void CEffectPrim::RenderSaintWing(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    for (int i = 0; i < 10; ++i) {
      const float rx = slot.distance;
      int angle = slot.rotStart - 65;

      if (angle >= 360) {
        angle -= 360;
      }

      if (angle < 0) {
        angle += 360;
      }

      const vector3d vec1{rx * g_cosTable[angle], slot.height[0], rx * g_sinTable[angle]};
      int angle2 = slot.rotStart + 65;

      if (angle2 >= 360) {
        angle2 -= 360;
      }

      if (angle2 < 0) {
        angle2 += 360;
      }

      const vector3d vec4{rx * g_cosTable[angle2], slot.height[0], rx * g_sinTable[angle2]};
      const float rz = i < 5 ? slot.maxHeight : slot.maxHeight + (0.5f * g_sinTable[slot.riseAngle] + 0.5f);

      angle = slot.rotStart;

      if (angle >= 360) {
        angle -= 360;
      }

      if (angle < 0) {
        angle += 360;
      }

      const float sn1 = g_sinTable[angle];
      const float cs1 = g_cosTable[angle];
      const int add = i < 5 ? (i - i % 5) * 9 + i % 5 * 3 + slot.fullDisplayAngle : (i - i % 5) * 9 + i % 5 * 3;
      const float lx = g_cosTable[add] * rz;
      const float ly = g_sinTable[add] * rz;

      slot.vecBNow.z = sn1 * lx;
      slot.vecBNow.x = cs1 * lx;
      const vector3d vec2{vec1.x + slot.vecBNow.x, vec1.y - ly, vec1.z + slot.vecBNow.z};
      const vector3d vec3{vec4.x + slot.vecBNow.x, vec4.y - ly, vec4.z + slot.vecBNow.z};

      if (ec == 1) {
        if (i % 5 < 4) {
          int alpha = slot.alphaB;

          for (int b = 3; b >= i % 5; --b) {
            alpha = static_cast<int>(static_cast<float>(alpha) * 0.7f);
          }

          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      } else if (ec == 0) {
        if (i % 5 < 4) {
          int alpha = slot.alphaB;

          for (int b = 3; b >= i % 5; --b) {
            alpha = static_cast<int>(static_cast<float>(alpha) * 0.7f);
          }

          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec4, vec3, vec2, vec1, wtm, 0, 0, 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec4, vec3, vec2, vec1, wtm, 1, 0, 0, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00BE1820
// FUNCTION: fn_19916
void CEffectPrim::RenderSlash1(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    for (int i = 0; i <= 2; ++i) {
      int angle = slot.rotStart - 1 + i;

      if (angle < 0) {
        angle += 360;
      } else if (angle >= 360) {
        angle -= 360;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      float rx = slot.height[0];
      const vector3d vec1{cs1 * rx, -0.0f, sn1 * rx};
      const vector3d vec2{vec1.x, i == 1 ? vec1.y - slot.maxHeight * 0.2f : vec1.y - slot.maxHeight * 0.1f, vec1.z};
      rx = slot.height[0] + slot.distance;
      const vector3d vec3{cs1 * rx, -0.0f, sn1 * rx};
      const vector3d vec4{vec3.x, i == 1 ? vec3.y - slot.maxHeight : vec3.y - slot.maxHeight * 0.4f, vec3.z};
      const int alpha = i == 1 ? slot.alphaB : slot.alphaB / 3;

      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec3, vec4, vec2, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE1D70
// FUNCTION: fn_19918
void CEffectPrim::RenderSoulLink(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vector3d vec1{g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
    vector3d vec2{g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
    vector3d vec3{g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
    vector3d vec4{g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
    const std::int16_t rot0 = this->state.slots[0].alphaT;
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, rot0, this->state.slots[1].alphaT);

    const float length = slot.vecBPre.x;
    const vector3d vecA{length * g_cosTable[rot0], slot.vecBPre.y, length * g_sinTable[rot0]};
    vec1 = {vec1.x + vecA.x, vec1.y + vecA.y, vec1.z + vecA.z};
    vec2 = {vec2.x + vecA.x, vec2.y + vecA.y, vec2.z + vecA.z};
    vec3 = {vec3.x + vecA.x, vec3.y + vecA.y, vec3.z + vecA.z};
    vec4 = {vec4.x + vecA.x, vec4.y + vecA.y, vec4.z + vecA.z};
    const int alpha = static_cast<int>(slot.height[13]);

    if (vec1.y > 0.0f) {
      continue;
    }

    if (slot.process < 180) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      CRagEffect* effect = nullptr;

      if (static_cast<float>(g_session.m_aid) == slot.height[10]) {
        effect = gameMode->scene->player->LaunchEffect(506, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (auto* actor = static_cast<CGameActor*>(gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(slot.height[10])))) {
        effect = actor->LaunchEffect(506, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }

      if (effect) {
        vector3d position = vecA;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      }
    }

    this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 125, 125, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// INLINED
static __forceinline void RenderSoulParticleQuad(
  CEffectPrim& prim, int* rpCnt, const EffectPrimitiveSlot& slot, const vector3d& c1, const vector3d& c2, const vector3d& c3, const vector3d& c4, ZeroVector3d& vec1, ZeroVector3d& vec2, ZeroVector3d& vec3, ZeroVector3d& vec4, const vector3d& base, float countValue, unsigned char extraShade, matrix& wtm
) {
  int count = static_cast<int>(countValue);

  if (count < 0) {
    count = 0;
  }

  vec1.Set(c1.x + base.x, c1.y + base.y, c1.z + base.z);
  vec2.Set(c2.x + base.x, c2.y + base.y, c2.z + base.z);
  vec3.Set(c3.x + base.x, c3.y + base.y, c3.z + base.z);
  vec4.Set(c4.x + base.x, c4.y + base.y, c4.z + base.z);

  if (vec1.y > 0.0f) {
    return;
  }

  const short alpha = static_cast<short>(count);

  if (slot.height[15] == 0.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 1.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 200, 200, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 2.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 160, 160, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 3.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 120, 120, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 4.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, extraShade, extraShade, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE2190
// FUNCTION: fn_19919
__declspec(safebuffers) void CEffectPrim::RenderSoulParticle(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vec1.Set(g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx);
    vec2.Set(g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx);
    vec3.Set(g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx);
    vec4.Set(g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d c1{vec1.x, vec1.y, vec1.z};
    const vector3d c2{vec2.x, vec2.y, vec2.z};
    const vector3d c3{vec3.x, vec3.y, vec3.z};
    const vector3d c4{vec4.x, vec4.y, vec4.z};
    RenderSoulParticleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecBPre, slot.height[12], 40, wtm);
    RenderSoulParticleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecBNow, slot.height[13], 10, wtm);
    RenderSoulParticleQuad(*this, &rpCnt, slot, c1, c2, c3, c4, vec1, vec2, vec3, vec4, slot.vecTPre, slot.height[14], 70, wtm);
  }
}

// OFFSET: 0x00BE2B10
// FUNCTION: fn_19920
void CEffectPrim::RenderSphereWind(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = slot.rotStart + count;

      if (angle >= 360) {
        angle -= 360;
      } else if (angle < 0) {
        angle += 360;
      }

      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      const vector3d vecM{cs1 * (g_cosTable[slot.riseAngle] * slot.distance), cs1 * (g_sinTable[slot.riseAngle] * slot.distance), sn1 * slot.distance};
      const vector3d vecA{-(slot.maxHeight * g_sinTable[slot.riseAngle]), slot.maxHeight * g_cosTable[slot.riseAngle], 0.0f};
      vec2.Set(vecM.x + vecA.x, vecM.y + vecA.y + slot.height[0], vecM.z + vecA.z);
      vec4.Set(vecM.x - vecA.x, vecM.y - vecA.y + slot.height[0], vecM.z - vecA.z);

      if (order > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0);
      }

      ++order;

      if (order >= 21) {
        order = 20;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}

// OFFSET: 0x00BC60E0
// FUNCTION: fn_19850
void CEffectPrim::Render2DCircle() {
  float uInc = 0.0f;
  int rpCnt = 0;
  const float temp = 360.0f - this->state.arcAngle;
  const float innerSize = this->state.radius - this->state.size;

  for (float i = 0.0f; i <= temp; i += this->state.arcAngle) {
    const float cs1 = g_cosTable[static_cast<int>(i)];
    const float sn1 = g_sinTable[static_cast<int>(i)];
    const float cs2 = g_cosTable[static_cast<int>(i + this->state.arcAngle)];
    const float sn2 = g_sinTable[static_cast<int>(i + this->state.arcAngle)];

    RenderTriangle& first = this->state.triangles[rpCnt];
    first.vertices[0].x = this->render.position.x + innerSize * cs1;
    first.vertices[0].y = this->render.position.y - innerSize * sn1;
    first.vertices[0].z = 0.000001f;
    first.vertices[0].rhw = 0.999999f;
    first.vertices[0].diffuse = this->state.argb;
    first.vertices[0].u = uInc;
    first.vertices[0].v = 1.0f;
    first.vertices[1].x = this->render.position.x + this->state.radius * cs2;
    first.vertices[1].y = this->render.position.y - this->state.radius * sn2;
    first.vertices[1].z = 0.000001f;
    first.vertices[1].rhw = 0.999999f;
    first.vertices[1].diffuse = this->state.argb;
    first.vertices[1].u = uInc + 0.25f;
    first.vertices[1].v = 0.0f;
    first.vertices[2].x = this->render.position.x + this->state.radius * cs1;
    first.vertices[2].y = this->render.position.y - this->state.radius * sn1;
    first.vertices[2].z = 0.000001f;
    first.vertices[2].rhw = 0.999999f;
    first.vertices[2].diffuse = this->state.argb;
    first.vertices[2].u = uInc;
    first.vertices[2].v = 0.0f;
    first.texture = this->state.textures[0];
    g_renderer->queues.SubmitTriangle(first, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    ++rpCnt;

    RenderTriangle& second = this->state.triangles[rpCnt];
    second.vertices[0].x = this->render.position.x + innerSize * cs1;
    second.vertices[0].y = this->render.position.y - innerSize * sn1;
    second.vertices[0].z = 0.000001f;
    second.vertices[0].rhw = 0.999999f;
    second.vertices[0].diffuse = this->state.argb;
    second.vertices[0].u = uInc;
    second.vertices[0].v = 1.0f;
    second.vertices[1].x = this->render.position.x + innerSize * cs2;
    second.vertices[1].y = this->render.position.y - innerSize * sn2;
    second.vertices[1].z = 0.000001f;
    second.vertices[1].rhw = 0.999999f;
    second.vertices[1].diffuse = this->state.argb;
    second.vertices[1].u = uInc + 0.25f;
    second.vertices[1].v = 1.0f;
    second.vertices[2].x = this->render.position.x + this->state.radius * cs2;
    second.vertices[2].y = this->render.position.y - this->state.radius * sn2;
    second.vertices[2].z = 0.000001f;
    second.vertices[2].rhw = 0.999999f;
    second.vertices[2].diffuse = this->state.argb;
    second.vertices[2].u = uInc + 0.25f;
    second.vertices[2].v = 0.0f;
    second.texture = this->state.textures[0];
    g_renderer->queues.SubmitTriangle(second, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    ++rpCnt;

    uInc += 0.25f;
    if (uInc >= 1.0f) {
      uInc = 0.0f;
    }
  }
}

// OFFSET: 0x00BC65D0
// FUNCTION: fn_19851
void CEffectPrim::Render2DDiamond() {
  const float sn = ClientSine(static_cast<int>(this->state.roll));
  const float cs = ClientCosine(static_cast<int>(this->state.roll));
  const float halfHeight = this->state.heightSize * 0.5f;
  const float halfWidth = this->state.widthSize * 0.5f;
  const float notch = halfHeight / 1.2f;
  const float leftX = -halfWidth;
  const float leftY = notch - halfHeight;
  const float bottom = -halfHeight;
  const float topX = cs * 0.0f - halfHeight * sn;
  const float topY = halfHeight * cs + sn * 0.0f;
  const float bottomX = cs * 0.0f - bottom * sn;
  const float bottomY = bottom * cs + sn * 0.0f;

  RenderTriangle& first = this->state.triangles[0];
  first.vertices[0].z = 0.000001f;
  first.vertices[0].rhw = 0.999999f;
  first.vertices[0].diffuse = this->state.argb;
  first.vertices[0].u = 0.0f;
  first.vertices[0].v = 0.5f;
  first.vertices[1].z = 0.000001f;
  first.vertices[1].rhw = 0.999999f;
  first.vertices[1].diffuse = 0xFFFFFFFFu;
  first.vertices[1].u = 0.5f;
  first.vertices[1].v = 1.0f;
  first.vertices[2].z = 0.000001f;
  first.vertices[2].rhw = 0.999999f;
  first.vertices[2].diffuse = this->state.argb;
  first.vertices[2].u = 0.5f;
  first.vertices[2].v = 0.0f;
  first.texture = this->state.textures[this->render.curMotion];
  first.vertices[0].x = leftX * cs - leftY * sn + this->render.position.x;
  first.vertices[0].y = leftY * cs + leftX * sn + this->render.position.y;
  first.vertices[1].x = this->render.position.x + topX;
  first.vertices[1].y = this->render.position.y + topY;
  first.vertices[2].x = this->render.position.x + bottomX;
  first.vertices[2].y = this->render.position.y + bottomY;
  g_renderer->queues.SubmitTriangle(first, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);

  RenderTriangle& second = this->state.triangles[1];
  const float rightY = halfHeight - notch;
  second.vertices[0].z = 0.000001f;
  second.vertices[0].rhw = 0.999999f;
  second.vertices[0].diffuse = this->state.argb;
  second.vertices[0].u = 1.0f;
  second.vertices[0].v = 0.5f;
  second.vertices[1].z = 0.000001f;
  second.vertices[1].rhw = 0.999999f;
  second.vertices[1].diffuse = 0xFFFFFFFFu;
  second.vertices[1].u = 0.5f;
  second.vertices[1].v = 1.0f;
  second.vertices[2].z = 0.000001f;
  second.vertices[2].rhw = 0.999999f;
  second.vertices[2].diffuse = this->state.argb;
  second.vertices[2].u = 0.5f;
  second.vertices[2].v = 0.0f;
  second.texture = this->state.textures[this->render.curMotion];
  second.vertices[0].x = halfWidth * cs - rightY * sn + this->render.position.x;
  second.vertices[0].y = rightY * cs + halfWidth * sn + this->render.position.y;
  second.vertices[1].x = this->render.position.x + topX;
  second.vertices[1].y = this->render.position.y + topY;
  second.vertices[2].x = this->render.position.x + bottomX;
  second.vertices[2].y = this->render.position.y + bottomY;
  g_renderer->queues.SubmitTriangle(second, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x00BCC960
// FUNCTION: fn_19863
void CEffectPrim::RenderRecordQuads(matrix& wtm) {
  RenderTriangle* triangle = this->state.triangles;

  for (int index = 0; index < this->state.numSegment; ++index) {
    const EffectPrimitiveRecord& record = this->state.records[index];
    RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};

    g_renderer->viewport.Project(&record.corners[0].x, wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = record.color;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    triangle->vertices[0].u = 0.0f;
    triangle->vertices[0].v = 1.0f;
    g_renderer->viewport.Project(&record.corners[1].x, wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = record.color;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    triangle->vertices[1].u = 1.0f;
    triangle->vertices[1].v = 1.0f;
    g_renderer->viewport.Project(&record.corners[2].x, wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = record.color;
    triangle->vertices[2].u = 0.0f;
    triangle->vertices[2].v = 0.0f;
    triangle->texture = this->state.textures[0];
    if (triangle->Visible()) {
      g_renderer->queues.SubmitTriangle(*triangle++, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }

    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(&record.corners[3].x, wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = record.color;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    triangle->vertices[0].u = 1.0f;
    triangle->vertices[0].v = 0.0f;
    g_renderer->viewport.Project(&record.corners[2].x, wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = record.color;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    triangle->vertices[1].u = 0.0f;
    triangle->vertices[1].v = 0.0f;
    g_renderer->viewport.Project(&record.corners[1].x, wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = record.color;
    triangle->vertices[2].u = 1.0f;
    triangle->vertices[2].v = 1.0f;
    triangle->texture = this->state.textures[0];
    if (triangle->Visible()) {
      g_renderer->queues.SubmitTriangle(*triangle++, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }
  }
}

// OFFSET: 0x00BC97E0
// FUNCTION: fn_19858
void CEffectPrim::RenderRecordRings(matrix& wtm) {
  const float* tm = this->state.transform;
  RenderTriangle* triangle = this->state.triangles;
  const float last = 360.0f - this->state.arcAngle;
  float uInc = 0.0f;

  for (int index = 0; index < this->state.numSegment; ++index) {
    const EffectPrimitiveRecord& record = this->state.records[index];
    const float inner = record.unknown_003C - this->state.size;
    const float outer = record.unknown_003C;

    for (float i = 0.0f; last >= i; i += this->state.arcAngle) {
      const int angle1 = static_cast<int>(i);
      const int angle2 = static_cast<int>(this->state.arcAngle + i);
      const float local[4][2] = {
        {inner * g_cosTable[angle1], inner * g_sinTable[angle1]},
        {inner * g_cosTable[angle2], inner * g_sinTable[angle2]},
        {outer * g_cosTable[angle1], outer * g_sinTable[angle1]},
        {outer * g_cosTable[angle2], outer * g_sinTable[angle2]},
      };
      float world[4][3];
      for (int corner = 0; corner < 4; ++corner) {
        const float x = local[corner][0];
        const float y = local[corner][1];
        world[corner][0] = x * tm[0] + y * tm[3] + tm[6] * 0.0f + tm[9] + record.position.x;
        world[corner][1] = x * tm[1] + y * tm[4] + tm[7] * 0.0f + tm[10] + record.position.y;
        world[corner][2] = x * tm[2] + y * tm[5] + tm[8] * 0.0f + tm[11] + record.position.z;
      }

      const float uNext = uInc + 0.25f;
      RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};

      g_renderer->viewport.Project(world[0], wtm.values, projected);
      triangle->vertices[0] = projected;
      triangle->vertices[0].diffuse = record.color;
      triangle->vertices[0].u = uInc;
      triangle->vertices[0].v = 1.0f;
      projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(world[1], wtm.values, projected);
      triangle->vertices[1] = projected;
      triangle->vertices[1].diffuse = record.color;
      triangle->vertices[1].u = uNext;
      triangle->vertices[1].v = 1.0f;
      projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(world[2], wtm.values, projected);
      triangle->vertices[2] = projected;
      triangle->vertices[2].diffuse = record.color;
      triangle->vertices[2].u = uInc;
      triangle->vertices[2].v = 0.0f;
      triangle->texture = this->state.textures[0];
      if (triangle->Visible()) {
        g_renderer->queues.SubmitTriangle(
          *triangle++, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
        );
      }

      projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(world[3], wtm.values, projected);
      triangle->vertices[0] = projected;
      triangle->vertices[0].diffuse = record.color;
      triangle->vertices[0].u = uNext;
      triangle->vertices[0].v = 0.0f;
      projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(world[2], wtm.values, projected);
      triangle->vertices[1] = projected;
      triangle->vertices[1].diffuse = record.color;
      triangle->vertices[1].u = uInc;
      triangle->vertices[1].v = 0.0f;
      projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(world[1], wtm.values, projected);
      triangle->vertices[2] = projected;
      triangle->vertices[2].diffuse = record.color;
      triangle->vertices[2].u = uNext;
      triangle->vertices[2].v = 1.0f;
      triangle->texture = this->state.textures[0];
      if (triangle->Visible()) {
        g_renderer->queues.SubmitTriangle(
          *triangle++, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
        );
      }

      uInc = uNext;
      if (uInc >= 1.0f) {
        uInc = 0.0f;
      }
    }
  }
}

// OFFSET: 0x00BC6E60
// FUNCTION: fn_19853
void CEffectPrim::RenderSizedSquare(matrix& wtm) {
  int rpCnt = 0;
  int index = 0;

  for (int ec = 0; ec < 4; ++ec, index += 23) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float size = this->state.size;
    int angle = index;
    if (size == 100.0f || size >= 110.0f || (size == 4.0f && slot.height[4] == 4.0f) || size == 10.0f || size == 0.0f) {
      angle = slot.rotStart;
    }

    const double radius = (g_sinTable[slot.riseAngle] + 1.0f) * (slot.distance * 0.1f) + slot.distance * 0.8;
    const int angle2 = angle + 90 < 360 ? angle + 90 : angle - 270;
    const int angle3 = angle2 + 90 < 360 ? angle2 + 90 : angle2 - 270;
    const int angle4 = angle3 + 90 < 360 ? angle3 + 90 : angle3 - 270;
    const float y = slot.height[0];
    slot.vecBPre = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    slot.vecBNow = {static_cast<float>(g_cosTable[angle2] * radius), y, static_cast<float>(g_sinTable[angle2] * radius)};
    slot.vecTNow = {static_cast<float>(g_cosTable[angle3] * radius), y, static_cast<float>(g_sinTable[angle3] * radius)};
    slot.vecTPre = {static_cast<float>(g_cosTable[angle4] * radius), y, static_cast<float>(g_sinTable[angle4] * radius)};

    const float mode = this->state.size;
    if (mode == 101.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 102.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 111.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 55, 55, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 112.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 105, 225, 105, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 100.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 110.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 113.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (mode == 114.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 155, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (mode == 15.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 35, 255, 35, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 17.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 18.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 35, 35, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (mode == 0.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BCAAC0
// FUNCTION: fn_19860
void CEffectPrim::RenderArcBand(matrix& wtm) {
  const float* tm = this->state.transform;
  const vector3d& position = this->render.position;
  float uInc = 0.0f;
  RenderTriangle* triangle = this->state.triangles;
  const float last = 360.0f - this->state.arcAngle;

  for (float i = 0.0f; last >= i; i += this->state.arcAngle) {
    const int angle1 = static_cast<int>(i);
    const int angle2 = static_cast<int>(this->state.arcAngle + i);
    const float inner = this->state.innerSize;
    const float outer = this->state.outerSize;
    const float depth = -this->state.heightSize;
    const float local[4][3] = {
      {inner * g_cosTable[angle1], 0.0f, inner * g_sinTable[angle1]},
      {inner * g_cosTable[angle2], 0.0f, inner * g_sinTable[angle2]},
      {outer * g_cosTable[angle1], depth, outer * g_sinTable[angle1]},
      {outer * g_cosTable[angle2], depth, outer * g_sinTable[angle2]},
    };
    float world[4][3];
    for (int corner = 0; corner < 4; ++corner) {
      const float x = local[corner][0];
      const float y = local[corner][1];
      const float z = local[corner][2];
      world[corner][0] = x * tm[0] + y * tm[3] + z * tm[6] + tm[9] + position.x;
      world[corner][1] = x * tm[1] + y * tm[4] + z * tm[7] + tm[10] + position.y;
      world[corner][2] = x * tm[2] + y * tm[5] + z * tm[8] + tm[11] + position.z;
    }

    const float uNext = uInc + 0.25f;
    RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};

    g_renderer->viewport.Project(world[0], wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = this->state.argb;
    triangle->vertices[0].u = uInc;
    triangle->vertices[0].v = 1.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[1], wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = this->state.argb;
    triangle->vertices[2].u = uNext;
    triangle->vertices[2].v = 1.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[2], wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = this->state.argb;
    triangle->vertices[1].u = uInc;
    triangle->vertices[1].v = 0.0f;
    triangle->texture = this->state.textures[this->render.curMotion];
    if (triangle->Visible()) {
      g_renderer->queues.SubmitTriangle(*triangle++, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }

    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[3], wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = this->state.argb;
    triangle->vertices[2].u = uNext;
    triangle->vertices[2].v = 0.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[2], wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = this->state.argb;
    triangle->vertices[1].u = uInc;
    triangle->vertices[1].v = 0.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[1], wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = this->state.argb;
    triangle->vertices[0].u = uNext;
    triangle->vertices[0].v = 1.0f;
    triangle->texture = this->state.textures[this->render.curMotion];
    if (triangle->Visible()) {
      g_renderer->queues.SubmitTriangle(*triangle++, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }

    uInc = uNext;
    if (uInc >= 1.0f) {
      uInc = 0.0f;
    }
  }
}

// OFFSET: 0x00BCB9F0
// FUNCTION: fn_19862
void CEffectPrim::RenderPyramid(matrix& wtm) {
  vector3d corners[12]{};
  const float size = this->state.size;
  const float height = this->state.heightSize;
  const vector3d table[12] = {
    {-size, -size, 0.0f}, {0.0f, 0.0f, height}, {size, -size, 0.0f},
    {-size, -size, 0.0f}, {0.0f, 0.0f, height}, {-size, size, 0.0f},
    {-size, size, 0.0f}, {0.0f, 0.0f, height}, {size, size, 0.0f},
    {size, size, 0.0f}, {0.0f, 0.0f, height}, {size, -size, 0.0f},
  };
  float uInc = 0.0f;

  if (this->state.slots[0].flag1[0] == 3) {
    const int first = std::rand() % 11;
    const int second = std::rand() % 11;
    this->state.argb = static_cast<std::uint32_t>((((first << 8) + second) << 8) - 0xD7D701);
  }

  RenderTriangle* triangle = this->state.triangles;
  const float* tm = this->state.rotTransform;
  for (int index = 0; index < 12; ++index) {
    const vector3d& local = table[index];
    corners[index] = {
      local.y * tm[3] + local.x * tm[0] + local.z * tm[6] + tm[9], local.y * tm[4] + local.x * tm[1] + local.z * tm[7] + tm[10],
      local.y * tm[5] + local.x * tm[2] + local.z * tm[8] + tm[11]
    };
  }

  for (int face = 0; face < 4; ++face) {
    RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    const vector3d& corner0 = corners[face * 3 + 0];
    const float point0[3] = {corner0.x + this->render.position.x, this->render.position.y + corner0.y, this->render.position.z + corner0.z};
    g_renderer->viewport.Project(point0, wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = this->state.argb;
    triangle->vertices[0].u = uInc;
    triangle->vertices[0].v = 1.0f;
    const vector3d& corner1 = corners[face * 3 + 1];
    const float point1[3] = {corner1.x + this->render.position.x, corner1.y + this->render.position.y, this->render.position.z + corner1.z};
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(point1, wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = this->state.argb;
    triangle->vertices[1].u = uInc;
    triangle->vertices[1].v = 0.0f;
    const vector3d& corner2 = corners[face * 3 + 2];
    const float point2[3] = {corner2.x + this->render.position.x, corner2.y + this->render.position.y, this->render.position.z + corner2.z};
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(point2, wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = this->state.argb;
    uInc += 0.2f;
    triangle->vertices[2].u = uInc;
    triangle->vertices[2].v = 1.0f;
    triangle->texture = this->state.textures[0];

    if (triangle->Visible()) {
      RenderTriangle& submitted = *triangle++;
      if (this->state.slots[0].flag1[0] == 3) {
        submitted.unknown_0064 = 5;
        submitted.unknown_0068 = 2;
        g_renderer->queues.SubmitTriangle(submitted, 5, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      } else {
        g_renderer->queues.SubmitTriangle(submitted, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      }
    }
  }
}

// OFFSET: 0x00BCA060
// FUNCTION: fn_19859
void CEffectPrim::RenderCrossPlanes(matrix& wtm) {
  const float width = this->state.widthSize;
  const float height = this->state.heightSize;
  const vector3d planes[2][4] = {
    {{-height, 0.0f, width}, {-height, 0.0f, -width}, {height, 0.0f, width}, {height, 0.0f, -width}},
    {{0.0f, -height, width}, {0.0f, -height, -width}, {0.0f, height, width}, {0.0f, height, -width}},
  };
  const float* tm = this->state.transform;
  const vector3d& position = this->render.position;
  RenderTriangle* triangle = this->state.triangles;

  for (const auto& plane : planes) {
    float world[4][3];
    for (int corner = 0; corner < 4; ++corner) {
      const vector3d& local = plane[corner];
      world[corner][0] = local.x * tm[0] + local.y * tm[3] + local.z * tm[6] + tm[9] + position.x;
      world[corner][1] = local.x * tm[1] + local.y * tm[4] + local.z * tm[7] + tm[10] + position.y;
      world[corner][2] = local.x * tm[2] + local.y * tm[5] + local.z * tm[8] + tm[11] + position.z;
    }

    RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[0], wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = this->state.argb;
    triangle->vertices[0].u = 0.0f;
    triangle->vertices[0].v = 1.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[1], wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = this->state.argb;
    triangle->vertices[1].u = 1.0f;
    triangle->vertices[1].v = 1.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[2], wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = this->state.argb;
    triangle->vertices[2].u = 0.0f;
    triangle->vertices[2].v = 0.0f;
    triangle->texture = this->state.textures[this->render.curMotion];
    if (triangle->Visible()) {
      g_renderer->queues.SubmitTriangle(*triangle++, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }

    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[3], wtm.values, projected);
    triangle->vertices[0] = projected;
    triangle->vertices[0].diffuse = this->state.argb;
    triangle->vertices[0].u = 1.0f;
    triangle->vertices[0].v = 0.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[2], wtm.values, projected);
    triangle->vertices[1] = projected;
    triangle->vertices[1].diffuse = this->state.argb;
    triangle->vertices[1].u = 0.0f;
    triangle->vertices[1].v = 0.0f;
    projected = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    g_renderer->viewport.Project(world[1], wtm.values, projected);
    triangle->vertices[2] = projected;
    triangle->vertices[2].diffuse = this->state.argb;
    triangle->vertices[2].u = 1.0f;
    triangle->vertices[2].v = 1.0f;
    triangle->texture = this->state.textures[this->render.curMotion];
    if (triangle->Visible()) {
      g_renderer->queues.SubmitTriangle(*triangle++, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }
  }
}

// INLINED
static __forceinline void RenderGroundFogQuad(CEffectPrim& prim, int* rpCnt, const EffectPrimitiveSlot& slot, const vector3d* corners, const vector3d& point, int copy, vector3d& vec1, vector3d& vec2, vector3d& vec3, vector3d& vec4, matrix& wtm) {
  vec1 = {point.x + corners[0].x, point.y + corners[0].y, point.z + corners[0].z};
  vec2 = {point.x + corners[1].x, point.y + corners[1].y, point.z + corners[1].z};
  vec3 = {point.x + corners[2].x, point.y + corners[2].y, point.z + corners[2].z};
  vec4 = {point.x + corners[3].x, point.y + corners[3].y, point.z + corners[3].z};
  const float y = vec1.y;
  const char kind = slot.flag1[0];
  int alpha;

  if (kind <= 1) {
    alpha = 250;
    if (-20.0f > y) {
      alpha = static_cast<int>((y + 20.0f) * 25.0f + 250.0f);
    }
  } else if (kind == 14 || kind == 16) {
    alpha = 100;
    if (y > -10.0f) {
      alpha = static_cast<int>(150.0f - (y + 10.0f) * 15.0f);
    } else if (-40.0f > y) {
      alpha = 0;
    }
  } else if (kind == 11) {
    alpha = 250;
    if (-40.0f > y) {
      alpha = static_cast<int>((y + 40.0f) * 25.0f + 250.0f);
    }
  } else if (kind == 15) {
    alpha = 255 - 2 * slot.flag1[1 + copy];
  } else {
    alpha = 50;
    if (-4.0f > y) {
      alpha = static_cast<int>((y + 4.0f) * 15.0f + 50.0f);
    }
    if (y > -0.5f) {
      alpha = static_cast<int>(y * -100.0f);
    }
  }
  if (alpha < 0) {
    alpha = 0;
  }

  if (kind == 15) {
    if (slot.flag1[5] == 7) {
      prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 250, 70, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[5] == 8) {
      prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 250, 250, 10, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      prim.RenderTeiRect(rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 70, 120, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
    return;
  }

  if (y <= 0.0f) {
    const short shade = static_cast<short>(alpha);
    if (kind == 0) {
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 1) {
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 3) {
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 55, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 10 || kind == 14) {
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 11) {
      if (copy == 0) {
        prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 155, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else {
      prim.RenderTeiRect(rpCnt, shade, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 70, 120, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BC7580
// FUNCTION: fn_19854
void CEffectPrim::RenderGroundFog(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vec1 = {rx * g_cosTable[45], 0.0f, rx * g_sinTable[45]};
    vec2 = {rx * g_cosTable[135], 0.0f, rx * g_sinTable[135]};
    vec3 = {rx * g_cosTable[225], 0.0f, rx * g_sinTable[225]};
    vec4 = {rx * g_cosTable[315], 0.0f, rx * g_sinTable[315]};
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    const vector3d corners[4] = {vec1, vec2, vec3, vec4};

    RenderGroundFogQuad(*this, &rpCnt, slot, corners, slot.vecBPre, 0, vec1, vec2, vec3, vec4, wtm);
    RenderGroundFogQuad(*this, &rpCnt, slot, corners, slot.vecBNow, 1, vec1, vec2, vec3, vec4, wtm);
    RenderGroundFogQuad(*this, &rpCnt, slot, corners, slot.vecTPre, 2, vec1, vec2, vec3, vec4, wtm);
    RenderGroundFogQuad(*this, &rpCnt, slot, corners, slot.vecTNow, 3, vec1, vec2, vec3, vec4, wtm);
  }
}

// OFFSET: 0x00BC8E00
// FUNCTION: fn_19855
void CEffectPrim::Render3DCasting2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    const int basicAngle = slot.fullDisplayAngle / 9;
    int order = 0;

    for (int count = 0; count < slot.fullDisplayAngle; count += basicAngle) {
      int angle = (count + slot.rotStart) % 360;
      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      float cs1 = g_cosTable[angle];
      float sn1 = g_sinTable[angle];
      slot.vecBNow.x = cs1 * slot.distance;
      slot.vecBNow.z = sn1 * slot.distance;
      slot.vecBNow.y = 0.0f;

      float rx = g_cosTable[slot.riseAngle] * slot.height[order];
      const float ry = g_sinTable[slot.riseAngle] * slot.height[order];

      if (ec == 2) {
        angle = 90 - angle;
      } else if (ec == 1) {
        angle = 270 - angle;
      } else if (ec == 0) {
        angle = 450 - angle;
      } else {
        angle = 630 - angle;
      }
      if (angle >= 360) {
        angle -= 360;
      }

      sn1 = g_sinTable[angle];
      cs1 = g_cosTable[angle];
      const float rz = sn1 * rx;
      rx = cs1 * rx;

      slot.vecTNow.x = slot.vecBNow.x + rx;
      slot.vecTNow.z = slot.vecBNow.z + rz;
      slot.vecTNow.y = -ry;

      const float offsetX = slot.flag1[0];
      const float offsetZ = slot.flag1[1];
      vector3d vec1 = slot.vecTNow;
      vec1.x += offsetX;
      vec1.z += offsetZ;
      vector3d vec2 = slot.vecBNow;
      vec2.x += offsetX;
      vec2.z += offsetZ;
      vector3d vec3 = slot.vecTPre;
      vec3.x += offsetX;
      vec3.z += offsetZ;
      vector3d vec4 = slot.vecBPre;
      vec4.x += offsetX;
      vec4.z += offsetZ;

      if (order > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec4, vec2, vec1, vec3, wtm, 0);
      }

      ++order;
      if (order > 20) {
        order = 20;
      }

      slot.vecTPre = slot.vecTNow;
      slot.vecBPre = slot.vecBNow;
    }
  }
}

// OFFSET: 0x00BC91D0
// FUNCTION: fn_19856
void CEffectPrim::Render3DCasting(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    const int basicAngle = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += basicAngle) {
      int angle = (count + slot.rotStart) % 360;
      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];

      vec2.x = cs1 * slot.distance;
      vec2.z = sn1 * slot.distance;
      if (slot.maxHeight == 6.001f) {
        vec2.y = -2.0f;
      } else if (slot.maxHeight == 24.001f) {
        vec2.y = -1.0f;
      } else {
        vec2.y = 0.0f;
      }

      float rx = g_cosTable[slot.riseAngle] * slot.height[order];
      float ry = g_sinTable[slot.riseAngle] * slot.height[order];
      const float rz = sn1 * rx;
      rx = cs1 * rx;

      vec4.x = vec2.x + rx;
      vec4.z = vec2.z + rz;
      if (slot.maxHeight == 6.001f) {
        ry += 2.0f;
      } else if (slot.maxHeight == 24.001f) {
        ry += 1.0f;
      }
      vec4.y = -ry;

      if (order > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0);
      }

      ++order;
      if (order > 20) {
        order = 20;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}

// OFFSET: 0x00BC94C0
// FUNCTION: fn_19857
void CEffectPrim::Render3DCasting_LowPolygon(matrix& wtm) {
  ZeroVector3d vec1;
  ZeroVector3d vec3;
  ZeroVector3d vec2;
  ZeroVector3d vec4;
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    const int basicAngle = slot.fullDisplayAngle / 10;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += basicAngle) {
      int angle = (count + slot.rotStart) % 360;
      if (slot.fullDisplayAngle == 360 && order == 10) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];

      vec2.x = cs1 * slot.distance;
      vec2.z = sn1 * slot.distance;
      vec2.y = 0.0f;

      float rx = g_cosTable[slot.riseAngle] * slot.height[order];
      const float ry = g_sinTable[slot.riseAngle] * slot.height[order];
      const float rz = sn1 * rx;
      rx = cs1 * rx;

      vec4.x = vec2.x + rx;
      vec4.z = vec2.z + rz;
      vec4.y = -ry;

      if (slot.vecBNow.y == 1234.5f) {
        const int heart = this->state.slots[2].alphaT;
        const float lx = slot.vecBNow.x;
        const float lz = slot.vecBNow.z;
        const float x = g_cosTable[heart] * lx - g_sinTable[heart] * lz;
        const float z = g_sinTable[heart] * lx + g_cosTable[heart] * lz;
        vec2.x += x;
        vec4.x += x;
        vec2.z += z;
        vec4.z += z;
      }

      if (order > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0);
      }

      ++order;
      if (order > 10) {
        order = 10;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}

// OFFSET: 0x00BD0680
// FUNCTION: fn_19869
void CEffectPrim::RenderAttackEnergy2(matrix& wtm) {
  vector3d vec1;
  ZeroVector(vec1);
  vector3d vec3;
  ZeroVector(vec3);
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int basicAngle = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += basicAngle) {
      int angle = count + slot.riseAngle;
      if (angle >= 360) {
        angle -= 360;
      }
      if (slot.fullDisplayAngle == 360 && count == 360) {
        angle = slot.riseAngle;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      const float cs2 = g_cosTable[slot.rotStart];
      const float sn2 = g_sinTable[slot.rotStart];
      const int angle2 = static_cast<int>(slot.height[1]);
      float cs3 = g_cosTable[angle2];
      float sn3 = g_sinTable[angle2];

      float dist = slot.distance * sn3;
      vector3d vec2;
      vec2.y = cs1 * dist;
      dist = sn1 * dist;
      vec2.x = cs2 * dist;
      vec2.z = sn2 * dist;
      vec2.y -= slot.maxHeight;
      vec2.x += slot.vecTNow.x * 3.0f;
      vec2.z += slot.vecTNow.z * 3.0f;
      vec2.x += slot.vecTNow.x * cs3 * slot.maxHeight;
      vec2.z += slot.vecTNow.z * cs3 * slot.maxHeight;

      int angle3 = static_cast<int>(slot.height[1] + 10.0f);
      if (angle3 > 90) {
        angle3 = 90;
      }
      cs3 = g_cosTable[angle3];
      sn3 = g_sinTable[angle3];

      dist = slot.distance * sn3;
      vector3d vec4;
      vec4.y = cs1 * dist;
      dist = sn1 * dist;
      vec4.z = sn2 * dist;
      vec4.x = cs2 * dist;
      vec4.y -= slot.maxHeight;
      vec4.x += slot.vecTNow.x * 3.0f;
      vec4.z += slot.vecTNow.z * 3.0f;
      vec4.x += slot.vecTNow.x * cs3 * slot.maxHeight;
      vec4.z += slot.vecTNow.z * cs3 * slot.maxHeight;

      if (count > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0);
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0);
      }

      ++order;
      if (order > 20) {
        order = 20;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}

// OFFSET: 0x00BD12D0
// FUNCTION: fn_19872
__declspec(safebuffers) void CEffectPrim::RenderBlind(matrix& wtm) {
  int rpCnt = 0;
  int r = 10;
  int g = 10;
  int b = 10;
  ZeroVector3d vec1;
  ZeroVector3d vec2;
  ZeroVector3d vec3;
  ZeroVector3d vec4;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    if (slot.flag1[0] == 1) {
      r = 30;
      g = 30;
      b = 30;
    }

    const float distance = slot.distance;
    const float outer = distance + 100.0f;
    vec1.Set(0.0f, 0.0f, 0.0f);
    vec2.Set(0.0f, 0.0f, distance);
    vec3.Set(distance, 0.0f, distance);
    vec4.Set(distance, 0.0f, 0.0f);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(0.0f, 0.0f, 0.0f);
    vec2.Set(0.0f, 0.0f, -slot.distance);
    vec3.Set(slot.distance, 0.0f, -slot.distance);
    vec4.Set(slot.distance, 0.0f, 0.0f);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(0.0f, 0.0f, 0.0f);
    vec2.Set(0.0f, 0.0f, slot.distance);
    vec3.Set(-slot.distance, 0.0f, slot.distance);
    vec4.Set(-slot.distance, 0.0f, 0.0f);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(0.0f, 0.0f, 0.0f);
    vec2.Set(0.0f, 0.0f, -slot.distance);
    vec3.Set(-slot.distance, 0.0f, -slot.distance);
    vec4.Set(-slot.distance, 0.0f, 0.0f);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(slot.distance, 0.0f, -slot.distance);
    vec2.Set(slot.distance + 100.0f, 0.0f, -slot.distance);
    vec3.Set(slot.distance + 100.0f, 0.0f, slot.distance + 100.0f);
    vec4.Set(slot.distance, 0.0f, slot.distance + 100.0f);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 1, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(slot.distance, 0.0f, slot.distance);
    vec2.Set(slot.distance, 0.0f, slot.distance + 100.0f);
    vec3.Set(-(slot.distance + 100.0f), 0.0f, slot.distance + 100.0f);
    vec4.Set(-(slot.distance + 100.0f), 0.0f, slot.distance);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 1, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(-slot.distance, 0.0f, slot.distance);
    vec2.Set(-(slot.distance + 100.0f), 0.0f, slot.distance);
    vec3.Set(-(slot.distance + 100.0f), 0.0f, -(slot.distance + 100.0f));
    vec4.Set(-slot.distance, 0.0f, -(slot.distance + 100.0f));
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 1, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec1.Set(-slot.distance, 0.0f, -slot.distance);
    vec2.Set(-slot.distance, 0.0f, -(slot.distance + 100.0f));
    vec3.Set(slot.distance + 100.0f, 0.0f, -(slot.distance + 100.0f));
    vec4.Set(slot.distance + 100.0f, 0.0f, -slot.distance);
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 1, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD37B0
// FUNCTION: fn_19878
void CEffectPrim::RenderChemical(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    vector3d vec1 = {0.0f, 0.0f, 0.0f};
    vector3d vec4 = {0.0f, 0.0f, 0.0f};

    int angle = slot.riseAngle - 3;
    if (angle < 0) {
      angle += 360;
    }
    vector3d vec2 = {g_cosTable[angle] * slot.distance, 0.0f, g_sinTable[angle] * slot.distance};

    angle = slot.riseAngle + 3;
    if (angle >= 360) {
      angle -= 360;
    }
    vector3d vec3 = {g_cosTable[angle] * slot.distance, 0.0f, g_sinTable[angle] * slot.distance};

    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    angle = static_cast<int>(slot.alphaB * 0.5f);
    this->RenderTeiRect(&rpCnt, static_cast<short>(angle), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    vec4 = {0.0f, 0.0f, 0.0f};
    vec1 = {0.0f, 0.0f, 0.0f};

    angle = slot.riseAngle - 5;
    if (angle < 0) {
      angle += 360;
    }
    vec2 = {g_cosTable[angle] * slot.distance, 0.0f, g_sinTable[angle] * slot.distance};

    angle = slot.riseAngle + 5;
    if (angle >= 360) {
      angle -= 360;
    }
    vec3 = {g_cosTable[angle] * slot.distance, 0.0f, g_sinTable[angle] * slot.distance};

    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 130, 130, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD53E0
// FUNCTION: fn_19882
__declspec(safebuffers) void CEffectPrim::RenderColorPaper(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.maxHeight;
    const float cs2 = g_cosTable[slot.riseAngle];
    const float sn2 = g_sinTable[slot.riseAngle];
    vector3d corners[4];

    for (int index = 0; index < 4; ++index) {
      int angle = slot.rotStart + index * 90;
      if (angle >= 360) {
        angle -= 360;
      }
      if (angle < 0) {
        angle += 360;
      }

      const float ry = rx * g_sinTable[angle];
      corners[index].x = rx * g_cosTable[angle];
      corners[index].y = ry * cs2;
      corners[index].z = ry * sn2;
    }

    vector3d vec1 = corners[0];
    vector3d vec2 = corners[1];
    vector3d vec3 = corners[2];
    vector3d vec4 = corners[3];
    vec3.x += slot.vecBNow.x;
    vec3.y += slot.vecBNow.y;
    vec3.z += slot.vecBNow.z;
    vec4.x += slot.vecBNow.x;
    vec4.y += slot.vecBNow.y;
    vec4.z += slot.vecBNow.z;
    vec1.x += slot.vecBNow.x;
    vec1.y += slot.vecBNow.y;
    vec1.z += slot.vecBNow.z;
    vec2.x += slot.vecBNow.x;
    vec2.y += slot.vecBNow.y;
    vec2.z += slot.vecBNow.z;

    this->RenderTeiRect(
      &rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, static_cast<unsigned char>(static_cast<int>(slot.height[0])),
      static_cast<unsigned char>(static_cast<int>(slot.height[1])), static_cast<unsigned char>(static_cast<int>(slot.height[2])), 0.0f, 1.0f, 0.0f,
      1.0f, 0
    );
  }
}

// OFFSET: 0x00BD5730
// FUNCTION: fn_19883
void CEffectPrim::RenderDoubleGumGang(matrix& wtm) {
  int rpCnt = 0;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  float rdist;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    for (int i = 0; i < 3; ++i) {
      if (slot.flag1[4] == 0) {
        if (i == 0) {
          red = 100;
          green = 100;
          blue = 255;
          rdist = 1.0f;
        } else if (i == 1) {
          red = 0;
          green = 0;
          blue = 120;
          rdist = 1.2f;
        } else if (i == 2) {
          red = 0;
          green = 0;
          blue = 120;
          rdist = 2.0f;
        }
      } else if (slot.flag1[4] == 1) {
        if (i == 0) {
          red = 90;
          green = 90;
          blue = 80;
          rdist = 1.0f;
        } else if (i == 1) {
          red = 60;
          green = 60;
          blue = 50;
          rdist = 1.2f;
        } else if (i == 2) {
          red = 30;
          green = 30;
          blue = 20;
          rdist = 2.0f;
        }
      } else {
        if (i == 0) {
          red = 255;
          green = 100;
          blue = 100;
          rdist = 1.0f;
        } else if (i == 1) {
          red = 120;
          green = 0;
          blue = 0;
          rdist = 1.2f;
        } else if (i == 2) {
          red = 120;
          green = 0;
          blue = 0;
          rdist = 2.0f;
        }
      }

      const float rx = slot.distance * rdist;
      const float length = 4.0f;

      slot.vecBPre.x = g_cosTable[static_cast<int>(slot.height[3])] * length;
      slot.vecBPre.z = g_sinTable[static_cast<int>(slot.height[3])] * length;
      slot.vecBNow.x = g_cosTable[static_cast<int>(slot.height[7])] * length;
      slot.vecBNow.z = g_sinTable[static_cast<int>(slot.height[7])] * length;
      slot.vecTPre.x = g_cosTable[static_cast<int>(slot.height[11])] * length;
      slot.vecTPre.z = g_sinTable[static_cast<int>(slot.height[11])] * length;
      slot.vecTNow.x = g_cosTable[static_cast<int>(slot.height[15])] * length;
      slot.vecTNow.z = g_sinTable[static_cast<int>(slot.height[15])] * length;

      vector3d vec1 = {g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
      vector3d vec2 = {g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
      vector3d vec3 = {g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
      vector3d vec4 = {g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};

      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

      const vector3d vec5 = vec1;
      const vector3d vec6 = vec2;
      const vector3d vec7 = vec3;
      const vector3d vec8 = vec4;

      {
        const vector3d& center = slot.vecBPre;
        const float* const height = &slot.height[0];
        const int dif = static_cast<int>(height[3] - this->state.slots[0].alphaT);
        bool front = (dif < 70 && dif > -80) || dif < -270 || dif > 270;
        if (height[2] != 0.0f && height[2] != 2.0f) {
          front = !front;
        }
        if (front) {
          vec1 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec2 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec3 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
          vec4 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
        } else {
          vec1 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec2 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec3 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
          vec4 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
        }
        vec1.y -= slot.height[20];
        vec2.y -= slot.height[20];
        vec3.y -= slot.height[20];
        vec4.y -= slot.height[20];
        int count = static_cast<int>(height[1]);
        if (count > slot.process) {
          count = slot.process;
        }
        this->RenderTeiRect(
          &rpCnt, static_cast<short>(count), vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(height[2]), 0, red, green, blue, 0.0f, 1.0f, 0.0f,
          1.0f, 2
        );
      }

      {
        const vector3d& center = slot.vecBNow;
        const float* const height = &slot.height[4];
        const int dif = static_cast<int>(height[3] - this->state.slots[0].alphaT);
        bool front = (dif < 70 && dif > -80) || dif < -270 || dif > 270;
        if (height[2] != 0.0f && height[2] != 2.0f) {
          front = !front;
        }
        if (front) {
          vec1 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec2 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec3 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
          vec4 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
        } else {
          vec1 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec2 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec3 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
          vec4 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
        }
        vec1.y -= slot.height[20];
        vec2.y -= slot.height[20];
        vec3.y -= slot.height[20];
        vec4.y -= slot.height[20];
        int count = static_cast<int>(height[1]);
        if (count > slot.process) {
          count = slot.process;
        }
        this->RenderTeiRect(
          &rpCnt, static_cast<short>(count), vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(height[2]), 0, red, green, blue, 0.0f, 1.0f, 0.0f,
          1.0f, 2
        );
      }

      {
        const vector3d& center = slot.vecTPre;
        const float* const height = &slot.height[8];
        const int dif = static_cast<int>(height[3] - this->state.slots[0].alphaT);
        bool front = (dif < 70 && dif > -80) || dif < -270 || dif > 270;
        if (height[2] != 0.0f && height[2] != 2.0f) {
          front = !front;
        }
        if (front) {
          vec1 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec2 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec3 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
          vec4 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
        } else {
          vec1 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec2 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec3 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
          vec4 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
        }
        vec1.y -= slot.height[20];
        vec2.y -= slot.height[20];
        vec3.y -= slot.height[20];
        vec4.y -= slot.height[20];
        int count = static_cast<int>(height[1]);
        if (count > slot.process) {
          count = slot.process;
        }
        this->RenderTeiRect(
          &rpCnt, static_cast<short>(count), vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(height[2]), 0, red, green, blue, 0.0f, 1.0f, 0.0f,
          1.0f, 2
        );
      }

      {
        const vector3d& center = slot.vecTNow;
        const float* const height = &slot.height[12];
        const int dif = static_cast<int>(height[3] - this->state.slots[0].alphaT);
        bool front = (dif < 70 && dif > -80) || dif < -270 || dif > 270;
        if (height[2] != 0.0f && height[2] != 2.0f) {
          front = !front;
        }
        if (front) {
          vec1 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec2 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec3 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
          vec4 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
        } else {
          vec1 = {vec6.x + center.x, vec6.y + center.y, vec6.z + center.z};
          vec2 = {vec5.x + center.x, vec5.y + center.y, vec5.z + center.z};
          vec3 = {vec8.x + center.x, vec8.y + center.y, vec8.z + center.z};
          vec4 = {vec7.x + center.x, vec7.y + center.y, vec7.z + center.z};
        }
        vec1.y -= slot.height[20];
        vec2.y -= slot.height[20];
        vec3.y -= slot.height[20];
        vec4.y -= slot.height[20];
        int count = static_cast<int>(height[1]);
        if (count > slot.process) {
          count = slot.process;
        }
        this->RenderTeiRect(
          &rpCnt, static_cast<short>(count), vec1, vec2, vec3, vec4, wtm, 0, static_cast<char>(height[2]), 0, red, green, blue, 0.0f, 1.0f, 0.0f,
          1.0f, 2
        );
      }
    }
  }
}

// OFFSET: 0x00BD8B80
// FUNCTION: fn_19890
void CEffectPrim::RenderGrandCross2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float cs1 = g_cosTable[slot.rotStart];
    const float sn1 = g_sinTable[slot.rotStart];
    float rx = slot.height[0];
    float rz = slot.height[1];
    slot.vecBPre.x = cs1 * rx - sn1 * rz;
    slot.vecBPre.z = sn1 * rx + cs1 * rz;
    slot.vecBPre.y = -1.0f;

    rx = slot.height[0];
    rz = -slot.height[1];
    slot.vecBNow.x = cs1 * rx - sn1 * rz;
    slot.vecBNow.z = sn1 * rx + cs1 * rz;
    slot.vecBNow.y = -1.0f;

    rx = -slot.height[0];
    rz = -slot.height[1];
    slot.vecTNow.x = cs1 * rx - sn1 * rz;
    slot.vecTNow.z = sn1 * rx + cs1 * rz;
    slot.vecTNow.y = -1.0f;

    rx = -slot.height[0];
    rz = slot.height[1];
    slot.vecTPre.x = cs1 * rx - sn1 * rz;
    slot.vecTPre.z = sn1 * rx + cs1 * rz;
    slot.vecTPre.y = -1.0f;

    vector3d vec1 = slot.vecBPre;
    vec1.y -= slot.height[2];
    vector3d vec2 = slot.vecBNow;
    vec2.y -= slot.height[2];
    vector3d vec3 = slot.vecTPre;
    vec3.y -= slot.height[2];
    vector3d vec4 = slot.vecTNow;
    vec4.y -= slot.height[2];

    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    const short alpha = static_cast<short>(slot.alphaB / 2);
    this->RenderTeiRect(&rpCnt, alpha, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, alpha, slot.vecTPre, slot.vecTNow, slot.vecBNow, slot.vecBPre, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD9180
// FUNCTION: fn_19891
void CEffectPrim::RenderMagicTarget(matrix& wtm) {

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const GroundGeometry* ground = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->ground;
    int column;
    int row;
    gameMode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    column -= 3;
    row -= 3;

    const float sn = g_sinTable[slot.rotStart];
    const float cs = g_cosTable[slot.rotStart];

    for (int z = 0; z < 7; ++z) {
      for (int x = 0; x < 7; ++x) {
        const float x0 = static_cast<float>(x) - 3.5f;
        const float x1 = static_cast<float>(x + 1) - 3.5f;
        const float z0 = static_cast<float>(z) - 3.5f;
        const float z1 = static_cast<float>(z + 1) - 3.5f;
        const float u00 = z0 * sn + x0 * cs;
        const float v00 = z0 * cs - x0 * sn;
        const float u10 = x1 * cs + z0 * sn;
        const float v10 = z0 * cs - x1 * sn;
        const float u01 = z1 * sn + x0 * cs;
        const float v01 = z1 * cs - x0 * sn;
        const float u11 = z1 * sn + x1 * cs;
        const float v11 = z1 * cs - x1 * sn;

        float scale = slot.maxHeight;
        float uv[8] = {
          (u00 * scale + 3.5f) / 7.0f, (v00 * scale + 3.5f) / 7.0f, (u10 * scale + 3.5f) / 7.0f, (v10 * scale + 3.5f) / 7.0f,
          (u01 * scale + 3.5f) / 7.0f, (v01 * scale + 3.5f) / 7.0f, (u11 * scale + 3.5f) / 7.0f, (v11 * scale + 3.5f) / 7.0f,
        };
        ground->DrawGridDecal(
          wtm.values, column + x, row + z, 0xAA9999FF, *g_texMgr.Load("effect\\magic_target.tga", 0, false, true, false), uv);

        scale = slot.maxHeight + 0.1f;
        uv[0] = (scale * u00 + 3.5f) / 7.0f;
        uv[1] = (scale * v00 + 3.5f) / 7.0f;
        uv[2] = (scale * u10 + 3.5f) / 7.0f;
        uv[3] = (scale * v10 + 3.5f) / 7.0f;
        uv[4] = (scale * u01 + 3.5f) / 7.0f;
        uv[5] = (scale * v01 + 3.5f) / 7.0f;
        uv[6] = (scale * u11 + 3.5f) / 7.0f;
        uv[7] = (scale * v11 + 3.5f) / 7.0f;
        ground->DrawGridDecal(
          wtm.values, column + x, row + z, 0x559999FF, *g_texMgr.Load("effect\\magic_target.tga", 0, false, true, false), uv);
      }
    }
  }
}

// OFFSET: 0x00BC69A0
// FUNCTION: fn_19852
void CEffectPrim::Render2DPrimitive() {
  if (this->state.slots[0].life == 'b') {
    this->render.curMotion = 0;
  }

  const float sinRoll = ClientSine(static_cast<int>(this->state.roll));
  const float cosRoll = ClientCosine(static_cast<int>(this->state.roll));
  const float width = this->state.widthSize;
  const float height = this->state.heightSize;

  float extra = 0.0f;
  float negHeight = -height;
  if ((this->state.pattern & 1) == 0) {
    extra = height;
  } else {
    negHeight += negHeight;
  }

  const float negWidth = -width;
  const float x0 = negWidth * cosRoll;
  const float y0 = negWidth * sinRoll;
  const float p1_x = x0 - negHeight * sinRoll;
  const float p1_y = y0 + negHeight * cosRoll;

  const float p3_x = width * sinRoll + extra * cosRoll;
  const float p3_y = width * cosRoll - extra * sinRoll;

  const float posX = this->render.position.x;
  const float posY = this->render.position.y;
  const float depthAvgHalf = g_depthAverageHalf;

  RenderTriangle& tri0 = this->state.triangles[0];
  tri0.vertices[0].x = (x0 - extra * sinRoll) + posX - depthAvgHalf;
  tri0.vertices[0].y = (y0 + extra * cosRoll) + posY - depthAvgHalf;
  tri0.vertices[0].z = 0.000001f;
  tri0.vertices[0].rhw = 0.999999f;
  tri0.vertices[0].diffuse = this->state.argb;
  tri0.vertices[0].u = 0.0f;
  tri0.vertices[0].v = 1.0f;

  tri0.vertices[1].x = (p1_x + posX) - depthAvgHalf;
  tri0.vertices[1].y = (posY + p1_y) - depthAvgHalf;
  tri0.vertices[1].z = 0.000001f;
  tri0.vertices[1].rhw = 0.999999f;
  tri0.vertices[1].diffuse = this->state.argb;
  tri0.vertices[1].u = 0.0f;
  tri0.vertices[1].v = 0.0f;

  tri0.vertices[2].x = (p3_y + posX) - depthAvgHalf;
  tri0.vertices[2].y = (posY + p3_x) - depthAvgHalf;
  tri0.vertices[2].z = 0.000001f;
  tri0.vertices[2].rhw = 0.999999f;
  tri0.vertices[2].diffuse = this->state.argb;
  tri0.vertices[2].u = 1.0f;
  tri0.vertices[2].v = 1.0f;

  tri0.texture = this->state.textures[this->render.curMotion];

  const std::uint16_t flag1 = (this->state.unknown_01C8[0] == 0) ? 1 : static_cast<std::uint16_t>(this->state.renderFlag);
  g_renderer->queues.SubmitTriangle(tri0, flag1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);

  RenderTriangle& tri1 = this->state.triangles[1];
  tri1.vertices[0].x = (p3_y + posX) - depthAvgHalf;
  tri1.vertices[0].y = (p3_x + posY) - depthAvgHalf;
  tri1.vertices[0].z = 0.000001f;
  tri1.vertices[0].rhw = 0.999999f;
  tri1.vertices[0].diffuse = this->state.argb;
  tri1.vertices[0].u = 1.0f;
  tri1.vertices[0].v = 1.0f;

  tri1.vertices[1].x = (posX + p1_x) - depthAvgHalf;
  tri1.vertices[1].y = (posY + p1_y) - depthAvgHalf;
  tri1.vertices[1].z = 0.000001f;
  tri1.vertices[1].rhw = 0.999999f;
  tri1.vertices[1].diffuse = this->state.argb;
  tri1.vertices[1].u = 0.0f;
  tri1.vertices[1].v = 0.0f;

  tri1.vertices[2].x = (posX + (width * cosRoll - negHeight * sinRoll)) - depthAvgHalf;
  tri1.vertices[2].y = (posY + width * sinRoll + negHeight * cosRoll) - depthAvgHalf;
  tri1.vertices[2].z = 0.000001f;
  tri1.vertices[2].rhw = 0.999999f;
  tri1.vertices[2].diffuse = this->state.argb;
  tri1.vertices[2].u = 1.0f;
  tri1.vertices[2].v = 0.0f;

  tri1.texture = this->state.textures[this->render.curMotion];

  const std::uint16_t flag2 = (this->state.unknown_01C8[0] == 0) ? 1 : static_cast<std::uint16_t>(this->state.renderFlag);
  g_renderer->queues.SubmitTriangle(tri1, flag2, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);

  if (this->state.slots[0].life == 'b') {
    for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
      if (effect.id == static_cast<std::uint32_t>(this->state.slots[0].rotStart)) {
        const std::uint32_t expire = effect.unknown_0004;
        bool match = false;
        if (timeGetTime() < expire && expire != 999999 && expire != 0) {
          match = true;
        } else if (g_replayAnimationScaling != 0) {
          if (Replay::Instance().elapsed < expire && expire != 999999 && expire != 0) {
            match = true;
          }
        }
        if (match) {
          CooldownState* cdState = g_cooldownOwner->Find(effect.id);
          if (cdState) {
            cdState->x = static_cast<int>(this->render.position.x);
            cdState->y = static_cast<int>(this->render.position.y);
            g_cooldownOwner->Drawing().Draw(0, 0, 16, 16, *cdState, 0, 0, 0);
          }
        }
      }
    }
    if (g_overrideColor == 1) {
      g_cooldownOwner->ClearReverseCommands();
    } else {
      g_cooldownOwner->Drawing().SubmitReverse(g_renderer->queues, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }
  }
}

// OFFSET: 0x00BCB660
// FUNCTION: fn_19861
void CEffectPrim::RenderSegmentSprites2(matrix& wtm) {
  CSprRes* sprite = this->render.sprite;
  CActRes* action = this->render.action;
  if (this->state.numSegment <= 0) {
    return;
  }

  for (int seg = 0; seg < this->state.numSegment; ++seg) {
    EffectPrimitiveRecord& record = this->state.records[seg];
    const float depth = record.position.y * wtm.values[5] + record.position.x * wtm.values[2] + record.position.z * wtm.values[8] + wtm.values[11];
    float screenX;
    float screenY;
    float reciprocalDepth;
    g_renderer->ProjectViewport(&record.position.x, wtm.values, &screenX, &screenY, &reciprocalDepth);
    float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);
    this->ScaleByMaster(distanceScale);

    const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);
    for (int index = 0; index < frame.cellCount; ++index) {
      const SpriteFrameCell& cell = frame.Cell(index);
      if (cell.image == -1) {
        continue;
      }

      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
      const double scale = record.scale;
      const float top = static_cast<float>(cell.y * scale * cell.scaleY * distanceScale + screenY);
      const float bottom = static_cast<float>(((static_cast<short>(image->unknown_0006) + 1) * image->height + cell.y) * scale * cell.scaleY * distanceScale + screenY);
      if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f) {
        continue;
      }

      float left;
      float right;
      if ((cell.mirrored & 1) == 0) {
        left = static_cast<float>(cell.x * scale * cell.scaleX * distanceScale + screenX);
        right = static_cast<float>(((static_cast<short>(image->unknown_0004) + 1) * image->width + cell.x) * scale * cell.scaleX * distanceScale + screenX);
        if (left >= static_cast<float>(g_renderer->viewport.width)) {
          continue;
        }
      } else {
        left = static_cast<float>(((static_cast<short>(image->unknown_0004) + 1) * image->width + cell.x) * scale * cell.scaleX * distanceScale + screenX);
        right = static_cast<float>(cell.x * scale * cell.scaleX * distanceScale + screenX);
        if (right >= static_cast<float>(g_renderer->viewport.width)) {
          continue;
        }
      }

      if ((cell.mirrored & 1) == 0 ? right <= 0.0f : left <= 0.0f) {
        continue;
      }

      SpriteQuad quad;
      quad.top = top;
      quad.left = left;
      quad.bottom = bottom;
      quad.right = right;
      quad.bottomRhw = 1.0f / ((bottom - screenY) * g_tangent + depth) + 0.0001f;
      quad.topRhw = 1.0f / ((top - screenY) * g_tangent + depth) + 0.0001f;
      if ((this->state.pattern & 0x200) == 0) {
        quad.color = record.color;
      } else {
        quad.color = (cell.color & 0xFF00FF00) | ((cell.color & 0xFF) << 16) | ((cell.color >> 16) & 0xFF);
      }
      quad.angle = static_cast<float>(cell.unknown_001C) + this->state.roll;
      quad.image = image;
      this->DrawQuad(quad, sprite->palette, this->state.renderFlag, 0.0f, 0.0f, 0);
    }
  }
}

// OFFSET: 0x00BCCD50
// FUNCTION: fn_19864
void CEffectPrim::RenderSphere(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  const float longitudeEnd = 360.0f - this->state.arcAngle;
  const float uStep = 1.0f / (360.0f / this->state.arcAngle);
  const float vStep = 1.0f / (180.0f / this->state.arcAngle);
  const float latitudeEnd = 90.0f - this->state.arcAngle;
  float v = 0.0f;

  for (float latitude = -90.0f; latitude <= latitudeEnd; latitude += this->state.arcAngle) {
    float u = 0.0f;
    for (float longitude = 0.0f; longitude <= longitudeEnd; longitude += this->state.arcAngle) {
      const float yaw1 = GetRadian(static_cast<int>(longitude + this->state.longitude));
      const float pitch1 = GetRadian(static_cast<int>(latitude));
      const float yaw2 = GetRadian(static_cast<int>(longitude + this->state.arcAngle + 0.25f + this->state.longitude));
      const float pitch2 = GetRadian(static_cast<int>(latitude + this->state.arcAngle));

      matrix rotation;
      rotation.MakeXRotation(pitch1);
      rotation.AppendYRotation(yaw1);
      const float* m1 = rotation.values;
      vec1.x = m1[3] * 0.0f + m1[0] * 0.0f + this->state.radius * m1[6] + m1[9];
      vec1.y = m1[4] * 0.0f + m1[1] * 0.0f + this->state.radius * m1[7] + m1[10];
      vec1.z = m1[5] * 0.0f + m1[2] * 0.0f + this->state.radius * m1[8] + m1[11];

      rotation.MakeXRotation(pitch2);
      rotation.AppendYRotation(yaw1);
      const float* m2 = rotation.values;
      vec2.x = m2[3] * 0.0f + m2[0] * 0.0f + this->state.radius * m2[6] + m2[9];
      vec2.y = m2[4] * 0.0f + m2[1] * 0.0f + this->state.radius * m2[7] + m2[10];
      vec2.z = m2[5] * 0.0f + m2[2] * 0.0f + this->state.radius * m2[8] + m2[11];

      rotation.MakeXRotation(pitch1);
      rotation.AppendYRotation(yaw2);
      const float* m3 = rotation.values;
      vec3.x = m3[3] * 0.0f + m3[0] * 0.0f + this->state.radius * m3[6] + m3[9];
      vec3.y = m3[4] * 0.0f + m3[1] * 0.0f + this->state.radius * m3[7] + m3[10];
      vec3.z = m3[5] * 0.0f + m3[2] * 0.0f + this->state.radius * m3[8] + m3[11];

      rotation.MakeXRotation(pitch2);
      rotation.AppendYRotation(yaw2);
      const float* m4 = rotation.values;
      vec4.x = m4[3] * 0.0f + m4[0] * 0.0f + this->state.radius * m4[6] + m4[9];
      vec4.y = m4[4] * 0.0f + m4[1] * 0.0f + this->state.radius * m4[7] + m4[10];
      vec4.z = m4[5] * 0.0f + m4[2] * 0.0f + this->state.radius * m4[8] + m4[11];

      const vector3d& position = this->render.position;
      vec1.x += position.x;
      vec1.y += position.y;
      vec1.z += position.z;
      vec2.x += position.x;
      vec2.y += position.y;
      vec2.z += position.z;
      vec3.x += position.x;
      vec3.y += position.y;
      vec3.z += position.z;
      vec4.x += position.x;
      vec4.y += position.y;
      vec4.z += position.z;

      const float nextU = u + uStep;
      const float nextV = v + vStep;

      RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};

      RenderTriangle& triangle1 = this->state.triangles[rpCnt];
      projected = RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(&vec1.x, wtm.values, projected);
      projected.diffuse = this->state.argb;
      projected.u = u;
      projected.v = v;
      triangle1.vertices[0] = projected;

      projected = RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(&vec2.x, wtm.values, projected);
      projected.diffuse = this->state.argb;
      projected.u = u;
      projected.v = nextV;
      triangle1.vertices[1] = projected;

      projected = RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(&vec3.x, wtm.values, projected);
      projected.diffuse = this->state.argb;
      projected.u = nextU;
      projected.v = v;
      triangle1.vertices[2] = projected;

      triangle1.texture = this->state.textures[0];
      if (triangle1.Visible()) {
        ++rpCnt;
        g_renderer->queues.SubmitTriangle(triangle1, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      }

      RenderTriangle& triangle2 = this->state.triangles[rpCnt];
      projected = RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(&vec4.x, wtm.values, projected);
      projected.diffuse = this->state.argb;
      projected.u = nextU;
      projected.v = nextV;
      triangle2.vertices[0] = projected;

      projected = RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(&vec3.x, wtm.values, projected);
      projected.diffuse = this->state.argb;
      projected.u = nextU;
      projected.v = v;
      triangle2.vertices[1] = projected;

      projected = RenderVertex{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      g_renderer->viewport.Project(&vec2.x, wtm.values, projected);
      projected.diffuse = this->state.argb;
      projected.u = u;
      projected.v = nextV;
      triangle2.vertices[2] = projected;

      triangle2.texture = this->state.textures[0];
      if (triangle2.Visible()) {
        ++rpCnt;
        g_renderer->queues.SubmitTriangle(triangle2, 1, g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      }

      u = nextU;
    }

    v += vStep;
  }
}

// OFFSET: 0x00BD6510
// FUNCTION: fn_19884
__declspec(safebuffers) void CEffectPrim::RenderTeiFacingSlots(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec4{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
    const float shape = slot.height[10];

    int angle = slot.fullDisplayAngle;
    vec1 = {dist * g_cosTable[angle], 0.0f, dist * g_sinTable[angle]};

    if (shape == 0.0f) {
      angle = angle >= 90 ? angle - 90 : angle + 270;
    } else if (shape == 1.0f) {
      angle = angle >= 60 ? angle - 60 : angle + 300;
    } else if (shape == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    vec2 = {dist * g_cosTable[angle], 0.0f, dist * g_sinTable[angle]};

    if (shape == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 1.0f) {
      angle -= 120;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 2.0f) {
      angle -= 50;
      if (angle < 0) {
        angle += 360;
      }
    }
    vec3 = {dist * g_cosTable[angle], 0.0f, dist * g_sinTable[angle]};

    if (shape == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (shape == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    vec4 = {dist * g_cosTable[angle], 0.0f, dist * g_sinTable[angle]};

    const char kind = slot.flag1[2];
    if (kind == 3) {
      vec1.x += 3.1f;
      vec2.x += 3.1f;
      vec3.x += 3.1f;
      vec4.x += 3.1f;
      vec4.z += 6.0f;
      vec3.z += 6.0f;
      vec2.z += 6.0f;
      vec1.z += 6.0f;
    } else if (kind >= 17 && kind <= 31) {
      vec4.z += 6.0f;
      vec3.z += 6.0f;
      vec2.z += 6.0f;
      vec1.z += 6.0f;
    }

    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d& base = slot.vecBNow;
    vec1.x += base.x;
    vec1.y += base.y;
    vec1.z += base.z;
    vec2.x += base.x;
    vec2.y += base.y;
    vec2.z += base.z;
    vec3.x += base.x;
    vec3.y += base.y;
    vec3.z += base.z;
    vec4.x += base.x;
    vec4.y += base.y;
    vec4.z += base.z;

    switch (kind) {
    case 0:
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 130, 130, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    case 4:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    case 5:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 125, 255, 125, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    case 10:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 150, 150, 150, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    case 12:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 50, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    case 15:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 51, 153, 153, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 16:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 1, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 3);
      break;
    case 17:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 18:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 155, 155, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 19:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 5, 255, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 155, 255, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 20:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 255, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 32:
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 1, 255, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }
  }
}

