#include "Contents/Effects/MonsterSkillEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/GravityControlEffect.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Effects/EffectTypes.h"
#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>
#include <format>
// INLINED
static void SetScreenVertex(RenderVertex& vertex, float x, float y, std::uint32_t color, float u, float v) {
  vertex.x = x;
  vertex.y = y;
  vertex.z = 0.000001f;
  vertex.rhw = 0.999999f;
  vertex.diffuse = color;
  vertex.specular = 0xFF000000;
  vertex.u = u;
  vertex.v = v;
}

// INLINED
static CEZ2STRParticle* AsParticle(EZ2STREmitter* emitter) {
  return reinterpret_cast<CEZ2STRParticle*>(emitter);
}

// INLINED
static CSpriteParticle* AsSprite(EZ2STREmitter* emitter) {
  return reinterpret_cast<CSpriteParticle*>(emitter);
}

void __stdcall fn_07334(CParticle* particle);
void __stdcall fn_07337(CParticle* particle);
void __stdcall fn_07338(CParticle* particle);
void __stdcall fn_07339(CParticle* particle);

// OFFSET: 0x0070C070
// FUNCTION: fn_07123
bool CCrimsonArrowEffect::Slot06() {
  EZ2STREmitter* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed);
}

// OFFSET: 0x0070C150
// FUNCTION: fn_07126
CRootTwistEffect::CRootTwistEffect() : CEffect(1110) {
}

// OFFSET: 0x0070C0B0
// FUNCTION: fn_07124
void CCrimsonArrowEffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0] + this->emitter->offset.x;
  particle->position.y = this->position[1] + this->emitter->offset.y;
  particle->position.z = this->position[2] + this->emitter->offset.z;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0070C120
// FUNCTION: fn_07125
void CCrimsonArrowEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0070C3B0
// FUNCTION: fn_07130
void CRootTwistEffect::Slot12() {
  if (this->id == 1110) {
    this->AddEvent(0, "effect\\su_roottwist.wav", 0, 0);
  }
}

// OFFSET: 0x0070C3D0
// FUNCTION: fn_07131
bool CRootTwistEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070C3E0
// FUNCTION: fn_07132
void CRootTwistEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    ++emitter->elapsed;
    emitter->position.x = this->position[0] + 0.0f;
    emitter->position.y = this->position[1] + 3.0f;
    emitter->position.z = this->position[2] + 0.0f;
  }
}

// OFFSET: 0x0070C440
// FUNCTION: fn_07133
void CRootTwistEffect::Slot04() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->sub_B40F30(1);
  }
}

// OFFSET: 0x0070C7F0
// FUNCTION: fn_07137
bool CSprArrowEffect::Slot06() {
  return this->unknown_00FD;
}

// OFFSET: 0x0070C800
// FUNCTION: fn_07138
void CSprArrowEffect::Slot03() {
  const float ticks =
      static_cast<float>(static_cast<double>(timeGetTime() - static_cast<unsigned int>(this->unknown_00D0)) / 24.0);
  if (this->unknown_00F4 > ticks) {
    return;
  }

  CSpriteParticle* particle = reinterpret_cast<CSpriteParticle*>(this->unknown_00CC);
  if (!particle || !particle->action) {
    return;
  }

  if (!this->unknown_00FC) {
    this->unknown_00FC = 1;
    this->SetSourceActor(this->sourceActor);

    const float dx = this->sourcePosition[0] - particle->position.x;
    const float dy = this->sourcePosition[1] - particle->position.y;
    const float dz = this->sourcePosition[2] - particle->position.z;
    this->unknown_00E0 = dx * 0.125f;
    this->unknown_00E4 = dy * 0.125f;
    this->unknown_00E8 = dz * 0.125f;

    const float lengthSquared = dy * dy + dx * dx + dz * dz;
    const float length = SquareRoot(lengthSquared);
    if (length < 40.0f) {
      this->unknown_00EC = 40.0f / length;
      this->unknown_00E0 *= this->unknown_00EC;
      this->unknown_00E4 *= this->unknown_00EC;
      this->unknown_00E8 *= this->unknown_00EC;
    }

    float angle = 0.0f;
    if (dx != 0.0f || dz != 0.0f) {
      angle = static_cast<float>(std::atan2(static_cast<double>(-dx), static_cast<double>(-dz))) / 3.141592f * 180.0f;
      if (angle < 0.0f) {
        angle += 360.0f;
      }
    }
    this->unknown_00F0 = angle;

    const float inverse = 1.0f / SquareRoot(lengthSquared);
    particle->position.x += inverse * dx * 2.5f;
    particle->position.y += inverse * dy * 2.5f;
    particle->position.z += inverse * dz * 2.5f;
  }

  ++particle->unknown_00E8;
  const float time = ticks - this->unknown_00F4;
  if (this->frame > 8) {
    const int last = static_cast<int>(particle->action->FrameCount(0)) - 1;
    this->unknown_00F8 = std::min(this->unknown_00F8 + 1, last);
  }

  reinterpret_cast<float&>(particle->unknown_00C4) =
      this->unknown_00F0 - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
  particle->position.x = time * this->unknown_00E0 + this->unknown_00D4 + particle->velocity.x;
  particle->position.y = this->unknown_00E4 * time + this->unknown_00D8 + particle->velocity.y;
  particle->position.z = this->unknown_00E8 * time + this->unknown_00DC + particle->velocity.z;
  if (time > 8.0f / this->unknown_00EC) {
    this->unknown_00FD = false;
  }
}

// OFFSET: 0x0070CB80
// FUNCTION: fn_07139
void CSprArrowEffect::Slot04() {
  if (this->unknown_00CC != 0 && this->unknown_00FC) {
    AsSprite(reinterpret_cast<EZ2STREmitter*>(this->unknown_00CC))->sub_B40F30(1);
  }
}

// OFFSET: 0x0070CBA0
// FUNCTION: fn_07140
void CSprArrowEffect::SetBulletSprite(int job) {
  CSpriteParticle* particle = reinterpret_cast<CSpriteParticle*>(this->unknown_00CC);
  if (!particle) {
    return;
  }

  const char* name;
  switch (job) {
  case 20936:
  case 21052:
  case 22216:
  case 22218:
  case 22219:
  case 22232:
  case 22234:
  case 22235:
    name = "canon_bullet";
    break;
  case 22175:
  case 22176:
    name = "RIGEL_BULLET";
    break;
  case 22204:
    name = "phylla_bullet";
    break;
  case 22197:
  case 22198:
  case 22199:
    name = "mg_corruption_root_h_bullet";
    break;
  case 22214:
  case 22215:
  case 22226:
  case 22227:
    name = "mg_corruption_root_bullet";
    break;
  case 22245:
    name = "VR_MD_DREGG1_bullet";
    break;
  case 22246:
    name = "VR_MD_DREGG2_bullet";
    break;
  case 22322:
    name = "EP21_YORKER_Bullet";
    break;
  case 22326:
    name = "EP21_YOSTER_D_bullet";
    break;
  case 22327:
    name = "EP21_YOSTER_E_bullet";
    break;
  case 22354:
  case 22418:
    name = "EP21_YORMI_bullet";
    break;
  case 22355:
  case 22366:
  case 22416:
    name = "EP21_YORDOS_Bullet";
    break;
  case 22359:
  case 22420:
    name = "EP21_YOSCOPUS_Bullet";
    break;
  case 22383:
  case 22384:
    name = "violen_bullet";
    break;
  case 22464:
    name = "CH1_SHADOW_JAILER_bullet";
    break;
  case 22467:
    name = "CH1_SCABIOSA_bullet";
    break;
  case 22468:
    name = "CH1_LOBELIA_bullet";
    break;
  default:
    name = "skel_archer_arrow";
    break;
  }

  if (std::strlen(name) == 0) {
    this->unknown_00FD = false;
    return;
  }

  particle->SetSprite(std::format("몬스터\\{}.spr", name).c_str());
  reinterpret_cast<CSpriteParticle*>(this->unknown_00CC)->SetAction(std::format("몬스터\\{}.act", name).c_str());
}

// OFFSET: 0x0070CEA0
// FUNCTION: fn_07141
CSpriteMableEffect::CSpriteMableEffect() : CEffect(1096) {
}

// OFFSET: 0x0070D040
// FUNCTION: fn_07144
void CSpriteMableEffect::Slot01() {
  for (int i = 0; i < 3; ++i) {
    CSpriteParticle* particle =
        static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->scale = 0.3f;
    particle->alpha = 200;
    particle->unknown_0054 = static_cast<float>(i) * 2.0943947f;
    reinterpret_cast<float&>(particle->unknown_0058) = 0.0f;
    particle->loopMode = 3;
    switch (i) {
    case 0:
      particle->SetSprite("이팩트\\su_spritemable\\su_spritemable_green.spr");
      particle->SetAction("이팩트\\su_spritemable\\su_spritemable_green.act");
      break;
    case 1:
      particle->SetSprite("이팩트\\su_spritemable\\su_spritemable_red.spr");
      particle->SetAction("이팩트\\su_spritemable\\su_spritemable_red.act");
      break;
    case 2:
      particle->SetSprite("이팩트\\su_spritemable\\su_spritemable_blue.spr");
      particle->SetAction("이팩트\\su_spritemable\\su_spritemable_blue.act");
      break;
    }

    this->unknown_002C = 9999;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }
}

// OFFSET: 0x0070D150
// FUNCTION: fn_07145
bool CSpriteMableEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070D2C0
// FUNCTION: fn_07147
void CSpriteMableEffect::Slot04() {
  if (!g_serviceOptions.Get(240)) {
    return;
  }

  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->sub_B40F30(1);
  }
}

// OFFSET: 0x0070D300
// FUNCTION: fn_07148
CStemSpearEffect::CStemSpearEffect() : CEffect(1100) {
}

// OFFSET: 0x0070D4A0
// FUNCTION: fn_07151
void CStemSpearEffect::Slot01() {
  for (int i = 0; i < 4; ++i) {
    CSpriteParticle* particle =
        static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetSprite("이팩트\\sv_stemspear\\sv_stemspear.spr");
    particle->SetAction("이팩트\\sv_stemspear\\sv_stemspear.act");

    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    emitter->velocity.x = 0.0f;
    emitter->velocity.y = 0.0f;
    emitter->velocity.z = 0.0f;
    this->unknown_002C = 16;

    const float dx = this->sourcePosition[0] - this->position[0];
    const float dz = this->sourcePosition[2] - this->position[2];
    float angle =
        static_cast<float>(std::atan2(static_cast<double>(dx), static_cast<double>(-dz))) / 3.141592f * 180.0f;
    if (angle < 0.0f) {
      angle += 360.0f;
    }
    float direction = 180.0f - angle;
    if (direction < 0.0f) {
      direction += 360.0f;
    }
    emitter->baseAngle = static_cast<int>(direction);
    emitter->speed = SquareRoot(dx * dx + dz * dz) / static_cast<float>(this->unknown_002C);
    emitter->elapsed = -(this->unknown_002C / 4) * i;
    this->emitters.push_back(emitter);
  }
}

// OFFSET: 0x0070D650
// FUNCTION: fn_07152
void __stdcall Nullsub226(int) {
}

// OFFSET: 0x0070D660
// FUNCTION: fn_07153
bool CStemSpearEffect::Slot06() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed < this->unknown_002C) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0070D850
// FUNCTION: fn_07156
CSvgSpiritEffect::CSvgSpiritEffect() : CEffect(1128), emitter(nullptr) {
}

// OFFSET: 0x0070D690
// FUNCTION: fn_07154
void CStemSpearEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter->elapsed >= 0) {
      this->SetSourceActor(this->sourceActor);
      emitter->angle = static_cast<float>(emitter->baseAngle) - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;

      const float dx = this->sourcePosition[0] - this->position[0];
      const float dy = this->sourcePosition[1] - this->position[1];
      const float dz = this->sourcePosition[2] - this->position[2];
      const float inverse = 1.0f / SquareRoot(dx * dx + dy * dy + dz * dz);

      emitter->velocity.x += dx * inverse * emitter->speed;
      emitter->velocity.y += dy * inverse * emitter->speed;
      emitter->velocity.z += dz * inverse * emitter->speed;

      emitter->position.x = this->position[0] + emitter->velocity.x;
      emitter->position.y = this->position[1] - 10.0f;
      emitter->position.z = this->position[2] + emitter->velocity.z;

      if (this->frame >= 10) {
        emitter->alpha -= 40;
      }
    }

    ++emitter->elapsed;
  }
}

// OFFSET: 0x0070D810
// FUNCTION: fn_07155
void CStemSpearEffect::Slot04() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed > 0 && (*emitter)->elapsed < this->unknown_002C) {
      AsSprite(*emitter)->sub_B40F30(1);
    }
  }
}

// OFFSET: 0x0070DB30
// FUNCTION: fn_07160
bool CSvgSpiritEffect::Slot06() {
  return this->emitter->elapsed < this->unknown_002C;
}

// OFFSET: 0x0070DB50
// FUNCTION: fn_07161
void CSvgSpiritEffect::Slot03() {
  EZ2STREmitter* emitter = this->emitter;
  if (emitter->elapsed >= 0) {
    this->SetSourceActor(this->sourceActor);
    emitter->angle = static_cast<float>(emitter->baseAngle) - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;

    const float dx = this->sourcePosition[0] - this->position[0];
    const float dy = this->sourcePosition[1] - this->position[1];
    const float dz = this->sourcePosition[2] - this->position[2];
    const float inverse = 1.0f / SquareRoot(dx * dx + dy * dy + dz * dz);

    emitter->velocity.x += dx * inverse * emitter->speed;
    emitter->velocity.y += dy * inverse * emitter->speed;
    emitter->velocity.z += dz * inverse * emitter->speed;

    emitter->position.x = this->position[0] + emitter->velocity.x;
    emitter->position.y = this->position[1] - 10.0f;
    emitter->position.z = this->position[2] + emitter->velocity.z;

    if (this->frame >= 10) {
      emitter->alpha -= 40;
    }
  }

  ++emitter->elapsed;
}

// OFFSET: 0x0070DCD0
// FUNCTION: fn_07162
void CSvgSpiritEffect::Slot04() {
  EZ2STREmitter* emitter = this->emitter;
  if (emitter->elapsed > 0 && emitter->elapsed < this->unknown_002C) {
    AsSprite(emitter)->sub_B40F30(1);
  }
}

// OFFSET: 0x0070DCF0
// FUNCTION: fn_07163
CTunaPartyEffect::CTunaPartyEffect() : CEffect(1097) {
}

// OFFSET: 0x0070DF30
// FUNCTION: fn_07167
bool CTunaPartyEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070E090
// FUNCTION: fn_07169
void CTunaPartyEffect::Slot04() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->sub_B40F30(1);
  }
}

// OFFSET: 0x0070E0C0
// FUNCTION: fn_07170
CCurseOfCubeEffect::CCurseOfCubeEffect()
    : CEffect(-1), unknown_00D8(0), unknown_00DC(0), unknown_00E0(-10.0f), unknown_00E4(0) {
}

// OFFSET: 0x0070EB60
// FUNCTION: fn_07178
bool CCurseOfCubeEffect::Slot06() {
  if (!this->ActorExists() || this->emitters.empty()) {
    return false;
  }

  return true;
}

// OFFSET: 0x0070F080
// FUNCTION: fn_07180
void CCurseOfCubeEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0070F0D0
// FUNCTION: fn_07181
CWarningPlaneEffect::CWarningPlaneEffect() : CEffect(1114), fadingIn(0), expireTime(0) {
}

// OFFSET: 0x0070F240
// FUNCTION: fn_07183
void CWarningPlaneEffect::AddPlane(vector3d position) {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (particle) {
    particle->SetTexture("warning_plane.tga");
    particle->unknown_0004 = this->id;
    particle->unknown_000C = 0;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->scale = 2.5f;
    particle->alpha = 150;
    particle->SetColor(255, 0, 0);
    particle->position = position;
    InitParticleQuad(particle->vertices);
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }
}

// OFFSET: 0x0070F350
// FUNCTION: fn_07185
void CWarningPlaneEffect::Slot01() {
}

// OFFSET: 0x0070F360
// FUNCTION: fn_07186
bool CWarningPlaneEffect::Slot06() {
  if (this->expireTime != 0 && this->expireTime < timeGetTime()) {
    return false;
  }

  return this->ActorExists();
}

// OFFSET: 0x0070FC60
// FUNCTION: fn_07194
bool CCylinderEffect::Slot06() {
  return this->startTime + this->unknown_002C >= timeGetTime();
}

// OFFSET: 0x0070F720
// FUNCTION: fn_07189
CCylinderEffect::CCylinderEffect() : CEffect(-1) {
}

// OFFSET: 0x0070FB50
// FUNCTION: fn_07193
void CCylinderEffect::Slot01() {
  this->startTime = timeGetTime();
  for (long long i = 0; i < 21; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = static_cast<int>(i);
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    InitParticleQuad(particle->vertices);
    particle->unknown_00C4 = 0;
    if (this->id == 2077) {
      particle->SetTexture("effect\\nipelheim\\nipelheim_cast\\nip_cast.tga");
      particle->scale = 4.5f;
      particle->unknown_004C = 70;
      particle->unknown_0054 = 25.0f;
      particle->unknown_00C0 = 0.052359872f;
      particle->sourceBlend = 5;
      particle->destinationBlend = 6;
    }
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }
}

// OFFSET: 0x0070F390
// FUNCTION: fn_07187
void CWarningPlaneEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (this->expireTime != 0 && emitter) {
      CEZ2STRParticle* particle = AsParticle(emitter);

      matrix rotation;
      rotation.Identity();
      rotation.MakeXRotation(1.5707963f);
      rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);

      matrix scale;
      scale.Identity();
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix transform;
      transform.Compose(scale, rotation);
      particle->transform = transform;

      if (this->fadingIn == 1) {
        particle->alpha += 2;
        if (particle->alpha >= 250) {
          this->fadingIn = 0;
        }
      } else {
        particle->alpha -= 2;
        if (particle->alpha <= 50) {
          this->fadingIn = 1;
        }
      }
    }

    ++emitter->elapsed;
  }
}

// OFFSET: 0x0070F670
// FUNCTION: fn_07188
void CWarningPlaneEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (this->expireTime != 0 && *it) {
      AsParticle(*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x0070FFA0
// FUNCTION: fn_07196
void CCylinderEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart / 21.0),
                               static_cast<float>((texPart + 1) / 21.0), false, true};
    particle->Render(quad);
  }
}

// OFFSET: 0x007100D0
// FUNCTION: fn_07197
CDancingKnifeEffect::CDancingKnifeEffect() : CEffect(1829) {
}

// OFFSET: 0x00710830
// FUNCTION: fn_07203
bool CDancingKnifeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00710A20
// FUNCTION: fn_07205
void CDancingKnifeEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }

  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00710AE0
// FUNCTION: fn_07206
CEternalSlashAttackEffect::CEternalSlashAttackEffect() : CEffect(-1) {
}

// OFFSET: 0x00710C80
// FUNCTION: fn_07209
void CEternalSlashAttackEffect::Slot01() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (bottom) {
    bottom->Load(std::string("eternal_slash\\eternal_slash_bottom\\eternal_slash_bottom.str"));
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(bottom);
    emitter->offset.x = 0.0f;
    emitter->offset.y = -13.0f;
    emitter->offset.z = 0.0f;
    this->emitters.push_back(emitter);
  }

  CEZ2STRParticle* slash = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (slash) {
    slash->Load(std::string("eternal_slash\\eternal_slash\\eternal_slash.str"));
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(slash);
    emitter->offset.x = 0.0f;
    emitter->offset.y = -13.0f;
    emitter->offset.z = 0.0f;
    this->emitters.push_back(emitter);
  }
}

// OFFSET: 0x00710F50
// FUNCTION: fn_07210
bool CEternalSlashAttackEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00710FB0
// FUNCTION: fn_07211
void CEternalSlashAttackEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    particle->position.x = this->position[0] + emitter->offset.x;
    particle->position.y = this->position[1] + emitter->offset.y;
    particle->position.z = this->position[2] + emitter->offset.z;
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x007110A0
// FUNCTION: fn_07212
void CEternalSlashAttackEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00711120
// FUNCTION: fn_07213
CEternalSlashCountEffect::CEternalSlashCountEffect() : CEffect(1913), emitter(nullptr) {
}

// OFFSET: 0x00711280
// FUNCTION: fn_07216
void CEternalSlashCountEffect::Slot01() {
  if (static_cast<unsigned int>(this->unknown_00B0) - 1 > 4) {
    return;
  }

  CEZ2STRParticle* particle =
      static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (!particle) {
    return;
  }

  particle->Load(FormatOwnedString("eternal_slash\\eternal_slash_count\\eternal_slash_count_%02d.str", this->unknown_00B0));
  particle->unknown_00D0 = 0.0f;
  particle->unknown_00D4 = -25.0f;
  particle->unknown_00D8 = 0.0f;
  this->emitter = reinterpret_cast<EZ2STREmitter*>(particle);
}

// OFFSET: 0x00711340
// FUNCTION: fn_07217
bool CEternalSlashCountEffect::Slot06() {
  EZ2STREmitter* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f;
}

// OFFSET: 0x00711380
// FUNCTION: fn_07218
void CEternalSlashCountEffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0] + this->emitter->offset.x;
  particle->position.y = this->position[1] + this->emitter->offset.y;
  particle->position.z = this->position[2] + this->emitter->offset.z;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x007113F0
// FUNCTION: fn_07219
void CEternalSlashCountEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00711420
// FUNCTION: fn_07220
CFallenAngelEffect::CFallenAngelEffect() : CEffect(1140), emitter(nullptr), unknown_00D0(0), unknown_00D4(0), unknown_00D8(0) {
}

// OFFSET: 0x00711580
// FUNCTION: fn_07223
void CFallenAngelEffect::Slot01() {
  if (!this->emitter) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    this->emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    particle->Open("RL_fallen_angel\\Penne.str");
  } else {
    AsParticle(this->emitter)->Restart();
  }

  InitParticleQuad(AsParticle(this->emitter)->vertices);
  this->unknown_00D0 = this->position[0];
  this->unknown_00D4 = this->position[1];
  this->unknown_00D8 = this->position[2];
}

// OFFSET: 0x00711690
// FUNCTION: fn_07224
bool CFallenAngelEffect::Slot06() {
  if (this->emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007116D0
// FUNCTION: fn_07225
void CFallenAngelEffect::Slot03() {
  EZ2STREmitter* emitter = this->emitter;
  const vector3d start = emitter->position;
  const float dx = this->unknown_00D0 - this->position[0];
  const float dz = this->unknown_00D8 - this->position[2];
  const float speed = 1.0f / SquareRoot(dx * dx + 0.0f + dz * dz);

  emitter->position.x = start.x + dx * speed * 0.09f;
  emitter->position.y = start.y + speed * 0.0f;
  emitter->position.z = start.z + dz * speed * 0.09f;

  AsParticle(emitter)->Process(0.0f, -80.0f, 0.0f);
  emitter->elapsed++;
}

// OFFSET: 0x007117F0
// FUNCTION: fn_07226
void CFallenAngelEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00711850
// FUNCTION: fn_07227
void CFallenAngelEffect::SetPosition(vector3d position) {
  vector3d target = position;
  target.y += (g_cameraDistance - 230.0f) * 0.0625f + 10.0f;

  this->position[0] = target.x;
  this->position[1] = target.y;
  this->position[2] = target.z;
  this->emitter->position = target;
}

// OFFSET: 0x007118C0
// FUNCTION: fn_07228
CFloralFlareRoad::CFloralFlareRoad() : CEffect(1804) {
}

// OFFSET: 0x00711A70
// FUNCTION: fn_07231
void CFloralFlareRoad::Slot01() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  this->unknown_00CC = reinterpret_cast<EZ2STREmitter*>(bottom);
  bottom->Load(std::string("froral_flareroad\\froral_flareroad_bottom\\froral_flareroad_bottom.str"));

  for (int delay = 0; delay < 4500; delay += 300) {
    vector3d position = {this->position[0], this->position[1], this->position[2]};
    const int distanceX = std::max(rand() % 20, 0);
    const int angleX = std::max(rand() % 360, 0);
    position.x = ClientSine(angleX) * static_cast<float>(distanceX) + position.x;
    const int distanceZ = std::max(rand() % 20, 0);
    const int angleZ = std::max(rand() % 360, 0);
    position.z = ClientCosine(angleZ) * static_cast<float>(distanceZ) + position.z;

    CEZ2STRParticle* flare = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
    flare->Load(std::string("froral_flareroad\\froral_flareroad\\froral_flareroad.str"));
    flare->m_delay = delay;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(flare));
    this->AddEvent(0, "effect\\AG_ALL_BLOOM.wav", delay / 16, 0);
  }
}

// OFFSET: 0x00711DA0
// FUNCTION: fn_07232
bool CFloralFlareRoad::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00711DB0
// FUNCTION: fn_07233
void CFloralFlareRoad::Slot03() {
  if (this->unknown_00CC) {
    AsParticle(this->unknown_00CC)->Process(0.0f, -80.0f, 0.0f);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime == 0.0f || emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed)) {
      if (emitter) {
        AsParticle(emitter)->Process(0.0f, -80.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
    }
  }
}

// OFFSET: 0x00711ED0
// FUNCTION: fn_07234
void CFloralFlareRoad::Slot04() {
  if (this->unknown_00CC) {
    AsParticle(this->unknown_00CC)
      ->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00711F80
// FUNCTION: fn_07235
CFootprintEffect::CFootprintEffect() : CEffect(2411), stepDistance(50.0), unknown_0110(1), lastPosition{}, unknown_0120(-1000.0f) {
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
}

// OFFSET: 0x007123C0
// FUNCTION: fn_07238
bool CFootprintEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x007127A0
// FUNCTION: fn_07240
bool CFootprintEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00712C50
// FUNCTION: fn_07244
void CFootprintEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->Render(true, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00712CA0
// FUNCTION: fn_07245
CFootprintStrEffect::CFootprintStrEffect()
    : CEffect(2412), unknown_0140(1.0), unknown_0148(1.0), unknown_0150(0.0), unknown_0158(50.0), unknown_0160(2.0),
      unknown_0168(0), unknown_0169(1), unknown_016C{} {
}

// OFFSET: 0x007130E0
// FUNCTION: fn_07247
void CFootprintStrEffect::Close() {
  for (EZ2STREmitter* emitter : this->emitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->emitters.clear();

  for (EZ2STREmitter* emitter : this->unknown_00D4) {
    FreeParticle(AsParticle(emitter));
  }
  this->unknown_00D4.clear();
}

// OFFSET: 0x00713370
// FUNCTION: fn_07248
bool CFootprintStrEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x00713830
// FUNCTION: fn_07250
bool CFootprintStrEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00713F70
// FUNCTION: fn_07253
void CFootprintStrEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->DrawGround(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    AsParticle(*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00714250

// OFFSET: 0x00715160
// FUNCTION: fn_07265
void CFromTheAbyssEffect::Close() {
  for (auto it = this->unknown_00E8.begin(); it != this->unknown_00E8.end(); ++it) {
    if (it->second.emitter) {
      FreeParticle(AsParticle(it->second.emitter));
    }
  }
  this->unknown_00E8.clear();
}

// OFFSET: 0x00715400
// FUNCTION: fn_07266
void CFromTheAbyssEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00715410
// FUNCTION: fn_07267
bool CFromTheAbyssEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00715420
// FUNCTION: fn_07268
void CFromTheAbyssEffect::Slot03() {
  for (auto it = this->unknown_00E8.begin(); it != this->unknown_00E8.end(); ++it) {
    this->UpdateParticle(it->first, it->second);
  }
}

// OFFSET: 0x007154A0
// FUNCTION: fn_07269
void CFromTheAbyssEffect::UpdateParticle(long long key, AbyssParticle particle) {
  EZ2STREmitter* emitter = particle.emitter;
  if (!emitter) {
    return;
  }

  switch (particle.kind) {
  case 1:
    if (emitter->lifetime != 0.0f && !(emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed))) {
      const double scale = AsParticle(emitter)->m_scale;
      const vector3d offset = emitter->offset;
      FreeParticle(AsParticle(emitter));
      EZ2STREmitter* dying =
          reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
      if (dying) {
        AsParticle(dying)->Load("from_the_abyss\\from_the_abyss_shadowball\\from_the_abyss_shadowball.str");
        dying->looping = 1;
        AsParticle(dying)->m_scale = static_cast<float>(scale);
        dying->offset = offset;
      }

      AbyssParticle& entry = this->unknown_00E8[key];
      entry.kind = 2;
      entry.emitter = dying;
      return;
    }
    break;
  case 2:
    break;
  case 3:
    if (emitter->lifetime != 0.0f && !(emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed))) {
      FreeParticle(AsParticle(emitter));
      AbyssParticle& entry = this->unknown_00E8[key];
      entry.kind = 0;
      entry.emitter = nullptr;
      return;
    }
    break;
  default:
    return;
  }

  const vector3d origin = *reinterpret_cast<const vector3d*>(this->position);
  const matrix view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
  const float viewX = origin.x * view.values[0] + origin.y * view.values[3] + origin.z * view.values[6] + view.values[9] + emitter->offset.x;
  const float viewY = origin.x * view.values[1] + origin.y * view.values[4] + origin.z * view.values[7] + view.values[10] + emitter->offset.y;
  const float viewZ = origin.x * view.values[2] + origin.y * view.values[5] + origin.z * view.values[8] + view.values[11] + emitter->offset.z;
  emitter->position.x = viewX * inverse.values[0] + viewY * inverse.values[3] + viewZ * inverse.values[6] + inverse.values[9];
  emitter->position.y = viewX * inverse.values[1] + viewY * inverse.values[4] + viewZ * inverse.values[7] + inverse.values[10];
  emitter->position.z = viewX * inverse.values[2] + viewY * inverse.values[5] + viewZ * inverse.values[8] + inverse.values[11];
  AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00715C80
// FUNCTION: fn_07270
void CFromTheAbyssEffect::Slot04() {
  for (auto it = this->unknown_00E8.begin(); it != this->unknown_00E8.end(); ++it) {
    EZ2STREmitter* emitter = it->second.emitter;
    if (emitter) {
      AsParticle(emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00715F30
// FUNCTION: fn_07271
void CFromTheAbyssEffect::SetLevel(long long level) {
  if (level < 0 || level > this->maxLevel) {
    return;
  }

  if (level > this->level) {
    for (long long i = this->level; i < level; ++i) {
      auto it = this->unknown_00E8.find(i);
      if (it != this->unknown_00E8.end() && it->second.emitter) {
        FreeParticle(AsParticle(it->second.emitter));
      }

      EZ2STREmitter* emitter =
          reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
      if (emitter) {
        AsParticle(emitter)->Load("from_the_abyss\\from_the_abyss_shadowball_create\\from_the_abyss_shadowball_create.str");
        AsParticle(emitter)->m_scale = this->unknown_00E0[i].scale;
        emitter->offset = this->unknown_00E0[i].offset;
        AbyssParticle& entry = this->unknown_00E8[i];
        entry.emitter = emitter;
        entry.kind = 1;
      }
    }
  } else if (level < this->level) {
    for (long long i = this->level - 1; i > level - 1; --i) {
      auto it = this->unknown_00E8.find(i);
      if (it != this->unknown_00E8.end() && it->second.emitter) {
        EZ2STREmitter* previous = it->second.emitter;
        const double scale = AsParticle(previous)->m_scale;
        const vector3d offset = previous->offset;
        FreeParticle(AsParticle(previous));
        EZ2STREmitter* emitter =
            reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
        if (emitter) {
          AsParticle(emitter)->Load("from_the_abyss\\from_the_abyss_shadowball_die\\from_the_abyss_shadowball_die.str");
          AsParticle(emitter)->m_scale = static_cast<float>(scale);
          emitter->offset = offset;
        }

        AbyssParticle& entry = this->unknown_00E8[i];
        entry.kind = 3;
        entry.emitter = emitter;
      }
    }
  }

  this->level = level;
}

// OFFSET: 0x00716800
// FUNCTION: fn_07274
CFullScreenEffect::CFullScreenEffect() : CEffect(-1), loaded(1) {
}

// OFFSET: 0x007169A0
// FUNCTION: fn_07277
void CFullScreenEffect::Slot01() {
  std::string texture;
  if (this->id == 2272) {
    texture = "effect\\state_effect\\frostbite\\frostbite_screen\\frostbite_screen.tga";
    this->retained = 1;
    for (long long i = 0; i < 4; ++i) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x201);
      if (!particle) {
        this->loaded = 0;
        return;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = static_cast<int>(i);
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->SetTexture(texture.c_str());
      particle->sourceBlend = 5;
      particle->destinationBlend = 6;
      particle->alpha = 0xFF;
      this->particles.push_back(particle);
    }
    return;
  }

  this->loaded = 0;
}

// OFFSET: 0x00716BD0
// FUNCTION: fn_07278
bool CFullScreenEffect::Slot06() {
  if (this->loaded == 1 && this->ActorExists()) {
    return true;
  }

  return false;
}

// OFFSET: 0x00716BF0
// FUNCTION: fn_07279
void CFullScreenEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    RenderPrimitive& primitive = particle->primitive;
    primitive.texture = particle->texture;
    const float left = g_textureOffsetX / primitive.texture->TextureWidth();
    const float right = (primitive.texture->Width() + g_textureOffsetX) / primitive.texture->TextureWidth();
    const float top = g_textureOffsetY / primitive.texture->TextureHeight();
    const float bottom = (primitive.texture->Height() + g_textureOffsetY) / primitive.texture->TextureHeight();
    const std::uint32_t color = ((particle->alpha << 8 | particle->red) << 8 | particle->green) << 8 | particle->blue;

    RenderVertex* vertices = primitive.quad;
    switch (particle->unknown_000C) {
    case 0:
      SetScreenVertex(vertices[0], 0.0f, 0.0f, color, right, top);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.centerX), 0.0f, color, left, top);
      SetScreenVertex(vertices[2], 0.0f, static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      break;
    case 1:
      SetScreenVertex(vertices[0], static_cast<float>(g_renderer->viewport.centerX), 0.0f, color, left, top);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.width), 0.0f, color, right, top);
      SetScreenVertex(vertices[2], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      break;
    case 2:
      SetScreenVertex(vertices[0], 0.0f, static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      SetScreenVertex(vertices[2], 0.0f, static_cast<float>(g_renderer->viewport.height), color, right, top);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.height), color, left, top);
      break;
    case 3:
      SetScreenVertex(vertices[0], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      SetScreenVertex(vertices[2], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.height), color, left, top);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.height), color, right, top);
      break;
    }

    primitive.unknown_0018 = particle->sourceBlend;
    primitive.unknown_001C = particle->destinationBlend;
    g_renderer->queues.Submit(&primitive, static_cast<std::uint16_t>(particle->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}

// OFFSET: 0x007176E0
// FUNCTION: fn_07280
CGaleStorm::CGaleStorm() : CEffect(1973), unknown_00D8(5), unknown_00DC(0) {
}

// OFFSET: 0x00717DE0
// FUNCTION: fn_07285
void CGaleStorm::Slot01() {
  struct LevelData {
    int count;
    int unknown;
  };
  LevelData table[10] = {{5}, {5}, {5}, {5}, {7}, {7}, {7}, {9}, {9}, {11}};

  unsigned int index = this->unknown_00B0;
  if (index > 10) {
    index = 10;
  }

  this->unknown_00D8 = table[index - 1].count;
  this->unknown_00DC = table[index - 1].unknown;
}

// OFFSET: 0x00717EB0
// FUNCTION: fn_07286
bool CGaleStorm::Slot06() {
  if (this->frame > this->unknown_002C && this->emitters.empty()) {
    return false;
  }

  return this->ActorExists();
}

// OFFSET: 0x00718040
// FUNCTION: fn_07288
void CGaleStorm::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x007183F0
// FUNCTION: fn_07292
void CGravityControlEffect::Slot04() {
  if (!this->target) {
    return;
  }

  AsParticle(reinterpret_cast<EZ2STREmitter*>(this->target))
      ->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x007184D0
CHawkVumerangEffect::CHawkVumerangEffect()
    : CEffect(1968),
      unknown_00E0(10),
      unknown_00E4(0),
      unknown_00E8(6.283185307179586),
      unknown_00F0(0),
      unknown_00F4(-17.0f),
      unknown_00F8(0),
      unknown_0100{},
      unknown_010C{},
      unknown_0118{} {
}

// OFFSET: 0x007187F0
// FUNCTION: fn_07297
void CHawkVumerangEffect::Close() {
  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D4.clear();
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00718B90
// FUNCTION: fn_07299
bool CHawkVumerangEffect::Slot06() {
  return this->emitters.size() != 0;
}

// OFFSET: 0x00718BA0

// OFFSET: 0x00718E70
// FUNCTION: fn_07301
void CHawkVumerangEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter) {
      AsParticle(emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00719030
CDropItemEffect::CDropItemEffect()
    : CEffect(-1), emitterCount(2), soundPath("wav\\effect\\"), position{} {
  this->emitters.resize(this->emitterCount);
}

// OFFSET: 0x00719400
// FUNCTION: fn_07307
void CDropItemEffect::Slot01() {
  if (!static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene) {
    return;
  }

  for (int i = 0; i < this->emitterCount; ++i) {
    EZ2STREmitter* emitter =
        reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->CEffect::position), 5));
    if (!emitter) {
      return;
    }

    InitParticleQuad(AsParticle(emitter)->vertices);
    this->emitters.push_back(emitter);
  }

  this->OpenEmitters();
  this->Slot12();
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetItemByAID(this->actor);
  this->position.x = this->CEffect::position[0];
  this->position.y = this->CEffect::position[1];
  this->position.z = this->CEffect::position[2];
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    EZ2STREmitter* emitter = this->emitters[i];
    if (emitter) {
      emitter->position = this->position;
    }
  }
}

// OFFSET: 0x00719520

// OFFSET: 0x00719660
// FUNCTION: fn_07309
bool CDropItemEffect::Slot06() {
  if (this->actor == 0) {
    return false;
  }

  if (!static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetItemByAID(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00719690

// OFFSET: 0x00719C30
// FUNCTION: fn_07311
void CDropItemEffect::Slot03() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      AsParticle(this->emitters[i])->Process(0.0f, 0.0f, 0.0f);
      this->emitters[i]->elapsed++;
    }
  }
}

// OFFSET: 0x00719CA0
// FUNCTION: fn_07312
void CDropItemEffect::Slot04() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      AsParticle(this->emitters[i])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0071B8C0
// FUNCTION: fn_07318
CJupitelThunderHit::CJupitelThunderHit() : CEffect(-1) {
  this->unknown_002C = 250;
}

// OFFSET: 0x0071BAB0

// OFFSET: 0x0071C270

// OFFSET: 0x0071C520
// FUNCTION: fn_07323
void CJupitelThunderHit::Slot01() {
  if (this->id != 1200) {
    this->SpawnSpark();
  }

  this->SpawnHit();
}

// OFFSET: 0x0071C540
// FUNCTION: fn_07324
bool CJupitelThunderHit::Slot06() {
  if (this->id == 1200) {
    return this->ActorExists();
  }

  if (this->actor == 0) {
    return true;
  }

  return this->frame < this->unknown_002C;
}

// OFFSET: 0x0071C570

// OFFSET: 0x0071CB70
// FUNCTION: fn_07326
void CJupitelThunderHit::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && emitter->elapsed >= 0) {
      AsParticle(emitter)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && emitter->elapsed >= 0) {
      AsParticle(emitter)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x0071CCB0
// FUNCTION: fn_07327
CNewCloudKillEffect::CNewCloudKillEffect() : CEffect(1196), unknown_00F4(0), unknown_00F8(0) {
}

// OFFSET: 0x0071D460
// FUNCTION: fn_07333
void CNewCloudKillEffect::Close() {
  for (auto it = this->unknown_00CC.begin(); it != this->unknown_00CC.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00CC.clear();
  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D4.clear();
  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00DC.clear();
  for (auto it = this->unknown_00E4.begin(); it != this->unknown_00E4.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00E4.clear();
  for (auto it = this->unknown_00EC.begin(); it != this->unknown_00EC.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00EC.clear();
}

// OFFSET: 0x0071DEC0
// FUNCTION: fn_07340
bool CNewCloudKillEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0071E080
// FUNCTION: fn_07341
void CNewCloudKillEffect::Slot03() {
  const unsigned int now = timeGetTime();

  for (auto it = this->unknown_00CC.begin(); it != this->unknown_00CC.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (now - static_cast<unsigned int>(this->unknown_00F4) > 5000) {
      particle->alpha = 0xFF;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = 0.0f;
      particle->unknown_00E8 = 60;
      particle->unknown_00A8 = 0;
      InitParticleQuad(particle->vertices);
    }

    if (particle != nullptr) {
      if (particle->unknown_00E8 == 0) {
        particle->unknown_00A8 = 1;
      } else {
        const int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4) + 1.0f) % 360;
        particle->alpha = static_cast<int>(static_cast<float>(particle->unknown_00E8) * 4.25f);
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);

        matrix rotation;
        rotation.MakeZRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
        rotation.AppendXRotation(1.5707964f);

        matrix scale;
        scale.SetScale(particle->scale, particle->scale, 1.0f);

        matrix transform;
        transform.Compose(scale, rotation);
        particle->transform = transform;

        particle->position.x = this->position[0];
        particle->position.y = this->position[1];
        particle->position.z = this->position[2];
      }
    }

    --particle->unknown_00E8;
  }

  if (now - static_cast<unsigned int>(this->unknown_00F4) > 5000) {
    this->unknown_00F4 = static_cast<int>(timeGetTime());
  }

  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (now - static_cast<unsigned int>(this->unknown_00F8) > 5000) {
      fn_07337(particle);
    }

    if (particle != nullptr) {
      int alpha = particle->alpha - 3;
      if (alpha < 0) {
        alpha = 0;
      }
      particle->alpha = alpha;
      if (alpha == 0) {
        particle->unknown_00A8 = 1;
      } else {
        const float fall = particle->unknown_00C0 * 1.0f;
        particle->scale -= particle->scale / static_cast<float>(alpha);
        particle->unknown_00D4 -= fall;
        particle->unknown_00D0 += 0.0f;
        particle->unknown_00D8 += 0.0f;

        particle->position.x = particle->unknown_00D0 + this->position[0];
        particle->position.y = particle->unknown_00D4 + this->position[1];
        particle->position.z = particle->unknown_00D8 + this->position[2];

        float angle = *reinterpret_cast<float*>(&particle->unknown_00C4) + 1.0f;
        angle = static_cast<float>(static_cast<int>(angle) % 360);
        *reinterpret_cast<float*>(&particle->unknown_00C4) = angle;

        matrix rotation;
        rotation.MakeZRotation(angle / 180.0f * 3.141592f);
        rotation.AppendXRotation(
            static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f
        );

        matrix scale;
        scale.SetScale(particle->scale, particle->scale, 1.0f);

        matrix transform;
        transform.Compose(scale, rotation);
        particle->transform = transform;
      }
    }
  }

  if (now - static_cast<unsigned int>(this->unknown_00F8) > 5000) {
    this->unknown_00F8 = static_cast<int>(timeGetTime());
  }

  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    ++particle->unknown_00E8;
    if (particle->unknown_00E8 <= 0 || particle == nullptr) {
      continue;
    }

    const int maximum = *reinterpret_cast<int*>(particle->unknown_0048);
    if (particle->unknown_00A8 == 0) {
      int alpha = particle->alpha + 2;
      if (alpha > maximum) {
        alpha = maximum;
      }
      particle->alpha = alpha;
      if (alpha == maximum) {
        particle->unknown_00A8 = 1;
      }
    } else if (particle->unknown_00A8 == 1) {
      int alpha = particle->alpha - 1;
      if (alpha < 0) {
        alpha = 0;
      }
      particle->alpha = alpha;
      if (alpha == 0) {
        fn_07339(particle);
        continue;
      }
      particle->scale -= particle->scale / static_cast<float>(alpha);
    }

    const float fall = particle->unknown_00C0 * 1.0f;
    particle->unknown_00D4 -= fall;
    particle->unknown_00D0 += 0.0f;
    particle->unknown_00D8 += 0.0f;

    particle->position.x = particle->unknown_00D0 + this->position[0];
    particle->position.y = particle->unknown_00D4 + this->position[1];
    particle->position.z = particle->unknown_00D8 + this->position[2];

    matrix scale;
    scale.SetScale(particle->scale, particle->scale, 1.0f);

    matrix rotation;
    rotation.MakeXRotation(
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f
    );

    matrix transform;
    transform.Compose(scale, rotation);
    particle->transform = transform;
  }

  for (auto it = this->unknown_00E4.begin(); it != this->unknown_00E4.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->unknown_00E8 < particle->unknown_004C) {
      int state = static_cast<signed char>(particle->unknown_00A8) - 1;
      if (state < 0) {
        state = 0;
      }
      particle->unknown_00A8 = static_cast<unsigned char>(state);
      particle->alpha = state * 25;

      matrix scale;
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix rotation;
      rotation.MakeZRotation(*reinterpret_cast<float*>(&particle->unknown_00C4) / 180.0f * 3.141592f);
      rotation.AppendXRotation(
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f
      );

      matrix transform;
      transform.Compose(scale, rotation);
      particle->transform = transform;

      matrix yaw;
      yaw.MakeYRotation(
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / 180.0f * 3.141592f
      );

      particle->position.x = yaw.values[0] * particle->unknown_00D0 + yaw.values[3] * particle->unknown_00D4 +
                             yaw.values[6] * particle->unknown_00D8 + yaw.values[9] + this->position[0];
      particle->position.y = yaw.values[1] * particle->unknown_00D0 + yaw.values[4] * particle->unknown_00D4 +
                             yaw.values[7] * particle->unknown_00D8 + yaw.values[10] + this->position[1];
      particle->position.z = yaw.values[2] * particle->unknown_00D0 + yaw.values[5] * particle->unknown_00D4 +
                             yaw.values[8] * particle->unknown_00D8 + yaw.values[11] + this->position[2];
    } else {
      fn_07338(particle);
    }

    ++particle->unknown_00E8;
  }

  for (auto it = this->unknown_00EC.begin(); it != this->unknown_00EC.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    ++particle->unknown_00E8;
    if (particle->unknown_00E8 <= 0 || particle == nullptr) {
      continue;
    }

    const int maximum = *reinterpret_cast<int*>(particle->unknown_0048);
    if (particle->unknown_00A8 == 0) {
      int alpha = particle->alpha + 5;
      if (alpha > maximum) {
        alpha = maximum;
      }
      particle->alpha = alpha;
      if (alpha == maximum) {
        particle->unknown_00A8 = 1;
      }
    } else if (particle->unknown_00A8 == 1) {
      int alpha = particle->alpha - 2;
      if (alpha < 0) {
        alpha = 0;
      }
      particle->alpha = alpha;
      if (alpha == 0) {
        fn_07334(particle);
        continue;
      }
      particle->scale = (particle->scale / static_cast<float>(maximum)) * static_cast<float>(alpha);
    }

    const float fall = particle->unknown_00C0 * 1.0f;
    particle->unknown_00D4 -= fall;
    particle->unknown_00D0 += 0.0f;
    particle->unknown_00D8 += 0.0f;

    particle->position.x = particle->unknown_00D0 + this->position[0];
    particle->position.y = particle->unknown_00D4 + this->position[1];
    particle->position.z = particle->unknown_00D8 + this->position[2];

    matrix scale;
    scale.SetScale(particle->scale, particle->scale, 1.0f);

    matrix rotation;
    rotation.MakeXRotation(
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f
    );

    matrix transform;
    transform.Compose(scale, rotation);
    particle->transform = transform;
  }
}

// OFFSET: 0x0071F5F0
// FUNCTION: fn_07344
CPetitioEffect::CPetitioEffect() : CEffect(1911) {
}

// OFFSET: 0x0071F750
// FUNCTION: fn_07346
void CPetitioEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    FreeParticle(AsParticle(*it));
    it = this->emitters.erase(it);
  }
}

// OFFSET: 0x0071F7A0
// FUNCTION: fn_07347
void CPetitioEffect::Slot01() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  bottom->Load(std::string("petitio\\petitio_bottom\\petitio_bottom.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));

  CEZ2STRParticle* petitio = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  petitio->Load(std::string("petitio\\petitio\\petitio.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(petitio));

  this->AddEvent(1, nullptr, 6, 0);
  this->AddEvent(0, "effect\\CD_PETITIO.wav", 0, 0);
}

// OFFSET: 0x0071FA30
// FUNCTION: fn_07348
bool CPetitioEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0071FA90
// FUNCTION: fn_07349
void CPetitioEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    particle->position.x = this->position[0];
    particle->position.y = this->position[1] - 12.0f;
    particle->position.z = this->position[2];
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0071FB70
// FUNCTION: fn_07350
void CPetitioEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0071FBF0
// FUNCTION: fn_07351
CPneumaticusProcellaEffect::CPneumaticusProcellaEffect() : CEffect(1957) {
}

// OFFSET: 0x0071FE40
// FUNCTION: fn_07354
void CPneumaticusProcellaEffect::Slot01() {
  double scale;
  switch (this->unknown_00B0) {
  case 1:
  case 2:
  case 3:
    scale = 0.6f;
    break;
  case 4:
  case 5:
  case 6:
    scale = 0.8f;
    break;
  case 10:
    scale = 1.2f;
    break;
  default:
    scale = 1.0;
    break;
  }

  this->emitter =
      reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (this->emitter) {
    AsParticle(this->emitter)
        ->Load(std::string("pneumaticusprocella\\pneumaticusprocella_target\\pneumaticusprocella_target.str"));
    AsParticle(this->emitter)->m_loop = true;
    AsParticle(this->emitter)->m_scale = static_cast<float>(scale);
  }

  scale += 0.6f;
  const double offsetsX[6] = {-12.0, -6.0, 0.0, 0.0, 6.0, 12.0};
  const double offsetsZ[9] = {-12.0, -10.0, -6.0, -4.0, 0.0, 4.0, 6.0, 10.0, 12.0};
  for (int i = 0; i < 40; ++i) {
    const double x = scale * offsetsX[std::max(0, rand() % 6)];
    const double z = scale * offsetsZ[std::max(0, rand() % 9)];
    const int delay = std::max(0, rand() % 200) + (i + i / 10 * 20) * 100;

    EZ2STREmitter* emitter =
        reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (emitter) {
      char name[260] = {};
      StringFormat(name, sizeof(name), static_cast<std::size_t>(-1),
                   "pneumaticusprocella\\pneumaticusprocella\\pneumaticusprocella_%d.str", std::max(1, rand() % 4));
      AsParticle(emitter)->Load(std::string(name));
      AsParticle(emitter)->m_delay = delay;
      emitter->offset.y = -25.0f;
      emitter->offset.x = static_cast<float>(x);
      emitter->offset.z = static_cast<float>(z);
      this->emitters.push_back(emitter);
    }
  }

  this->AddEvent(0, "effect\\CD_PNEUMATICUS_PROCELLA.wav", 0, 0);
}

// OFFSET: 0x00720410
// FUNCTION: fn_07356
void CPneumaticusProcellaEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00720310
// FUNCTION: fn_07355
void CPneumaticusProcellaEffect::Slot03() {
  if (this->emitter) {
    AsParticle(this->emitter)->Process(0.0f, -80.0f, 0.0f);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    if (emitter->lifetime != 0.0f && emitter->lifetime * 60.0f <= static_cast<float>(emitter->elapsed)) {
      FreeParticle(AsParticle(emitter));
      *it = nullptr;
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    particle->position.x = this->position[0] + emitter->offset.x;
    particle->position.y = this->position[1] + emitter->offset.y;
    particle->position.z = this->position[2] + emitter->offset.z;
    particle->Process(0.0f, -80.0f, 0.0f);
  }
}

// OFFSET: 0x00720480
// FUNCTION: fn_07357
CRandomHitEffect::CRandomHitEffect() : CEffect(-1), active(true) {
}

// OFFSET: 0x007208A0
// FUNCTION: fn_07362
void CRandomHitEffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0] + this->emitter->offset.x;
  particle->position.y = this->position[1] + this->emitter->offset.y;
  particle->position.z = this->position[2] + this->emitter->offset.z;
  particle->Process(0.0f, 0.0f, 0.0f);
  this->active = particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00720940
// FUNCTION: fn_07363
void CRandomHitEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00720970
// FUNCTION: fn_07364
CRandomOptEffect::CRandomOptEffect() : CEffect(-1), offsets{} {
}

// OFFSET: 0x00720AF0
// FUNCTION: fn_07366
void CRandomOptEffect::Slot08(int red, int green, int blue) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00720B30
// FUNCTION: fn_07367
void CRandomOptEffect::Slot11(std::string name) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetTexture(name);
  }
}

// OFFSET: 0x00720C80
// FUNCTION: fn_07368
void CRandomOptEffect::Init() {
  int ring = 0;
  int baseAngle = 0;
  for (int alpha = 50; alpha > 20; alpha -= 10) {
    for (int i = 0; i <= 21; ++i) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\alpha_down.tga");
      particle->unknown_0004 = this->id;
      particle->unknown_0010 = ring;
      particle->unknown_000C = i;
      particle->unknown_0008 = 3;
      particle->sourceBlend = 5;
      particle->destinationBlend = 6;
      particle->alpha = alpha;
      particle->SetColor(100, 100, 255);
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(baseAngle);
      const float level = static_cast<float>(ring);
      particle->unknown_0050 = 360;
      particle->unknown_00C0 = 100.0f - level * 10.0f;
      particle->scale = level * 0.2f + 0.5f;
      particle->unknown_004C = static_cast<int>(level * 0.5f + 90.0f);
      particle->unknown_0054 = 0.0f;

      switch (this->id) {
      case 1186:
        particle->SetColor(255, 200, 255);
        particle->unknown_00C0 = 120.0f - level * 12.0f;
        particle->scale = level * 0.24f + 0.6f;
        break;
      case 1187:
        particle->SetColor(192, 192, 192);
        break;
      case 1188:
        particle->SetColor(0, 100, 150);
        break;
      case 1189:
        particle->SetColor(255, 255, 128);
        break;
      case 1190:
        particle->SetColor(128, 0, 255);
        break;
      case 1191:
        particle->SetColor(255, 128, 0);
        break;
      }

      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);

      if (particle) {
        const int segment = particle->unknown_000C;
        const float height = g_sinTable[segment * 9] * particle->unknown_00C0;
        const int step = static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f);
        const int angle =
            static_cast<int>(static_cast<float>(step * segment) + *reinterpret_cast<const float*>(&particle->unknown_00C4)) %
            360;
        const float spread = height * g_cosTable[particle->unknown_004C];
        const float lift = height * g_sinTable[particle->unknown_004C];
        const float cosine = g_cosTable[angle];
        const float sine = g_sinTable[angle];
        const float innerX = particle->scale * cosine;
        const float innerZ = particle->scale * sine;

        particle->vertices[3].x = innerX;
        particle->vertices[3].y = 0.0f;
        particle->vertices[3].z = innerZ;
        particle->vertices[1].x = innerX + spread * cosine;
        particle->vertices[1].y = 0.0f - lift;
        particle->vertices[1].z = innerZ + spread * sine;

        if (segment != 0) {
          particle->vertices[0].x = this->offsets[1].x;
          particle->vertices[0].y = this->offsets[1].y;
          particle->vertices[0].z = this->offsets[1].z;
          particle->vertices[2].x = this->offsets[0].x;
          particle->vertices[2].y = this->offsets[0].y;
          particle->vertices[2].z = this->offsets[0].z;
        }

        this->offsets[0].x = innerX;
        this->offsets[0].y = 0.0f;
        this->offsets[0].z = innerZ;
        this->offsets[1].x = particle->vertices[1].x;
        this->offsets[1].y = particle->vertices[1].y;
        this->offsets[1].z = particle->vertices[1].z;
      }
    }

    ++ring;
    baseAngle += 120;
  }
}

// OFFSET: 0x00721220
// FUNCTION: fn_07370
void CRandomOptEffect::Slot01() {
  this->Init();
}

// OFFSET: 0x00721230
// FUNCTION: fn_07371
bool CRandomOptEffect::Slot06() {
  if (this->actor == 0) {
    return false;
  }

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!gameMode || !gameMode->scene) {
    return false;
  }

  if (!gameMode->scene->GetItemByAID(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00721270
// FUNCTION: fn_07372
void CRandomOptEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      const int angle = static_cast<int>(
          static_cast<float>(particle->unknown_0010) + *reinterpret_cast<const float*>(&particle->unknown_00C4) + 3.0f);
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
      particle->transform = rotation;
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00721430
// FUNCTION: fn_07373
void CRandomOptEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    if (texPart <= 0) {
      continue;
    }

    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f,
                               static_cast<float>(texPart) / 21.0f, false, true};
    particle->Render(quad);
  }
}

// OFFSET: 0x00721560
// FUNCTION: fn_07374
CRichsCoinEffect::CRichsCoinEffect() : CEffect(1135), unknown_00E4(0) {
}

// OFFSET: 0x007218D0
// FUNCTION: fn_07377
void CRichsCoinEffect::Slot01() {
  int count = this->unknown_00E4;
  if (count > 5) {
    count = 5;
  } else if (count <= 0) {
    return;
  }

  for (int i = 0; i < count; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->SetTexture("effect\\rl_richs_coin_b\\rcoin1.bmp");
      particle->unknown_0054 = static_cast<float>(360 / count * i) / 180.0f * 3.141592f - 10.0f;
      reinterpret_cast<float&>(particle->unknown_0058) = 0.0f;
      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }

  if (this->unknown_00E4 > 5) {
    const int lights = this->unknown_00E4 - 5;
    for (int i = 0; i < lights; ++i) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->SetTexture("effect\\rl_richs_coin_b\\light.bmp");
        particle->unknown_0054 = static_cast<float>(i * 72) / 180.0f * 3.141592f - 10.0f;
        reinterpret_cast<float&>(particle->unknown_0058) = 0.0f;
        particle->scale = 1.5f;
        matrix scale;
        std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
        scale.SetScale(1.5f, 1.5f, 1.0f);
        particle->transform = scale;
        particle->alpha = 70;
        InitParticleQuad(particle->vertices);
        this->coins.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00721AE0
// FUNCTION: fn_07378
bool CRichsCoinEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00721F80
// FUNCTION: fn_07380
void CRichsCoinEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->coins.begin(); it != this->coins.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x00722070
// FUNCTION: fn_07381
CSavageImpactEffect::CSavageImpactEffect() : CEffect(1837), offset{} {
}

// OFFSET: 0x00722460
// FUNCTION: fn_07385
bool CSavageImpactEffect::Slot06() {
  if (this->emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007224A0
// FUNCTION: fn_07386
void CSavageImpactEffect::Slot03() {
  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0];
  particle->position.y = this->position[1] - 12.0f;
  particle->position.z = this->position[2];
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00722560
// FUNCTION: fn_07387
void CSavageImpactEffect::Slot04() {
  AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x007226A0
// FUNCTION: fn_07389
CServantWeaponEffect::CServantWeaponEffect() : CEffect(-1), offsets{} {
}

// OFFSET: 0x00722730
// FUNCTION: fn_07390
CServantWeaponNewEffect::CServantWeaponNewEffect() : CEffect(2460) {
}

// OFFSET: 0x00722A70
// FUNCTION: fn_07394
void CServantWeaponNewEffect::AddWeapon() {
  if (this->emitters.size() < 5) {
    float angle = static_cast<float>(this->emitters.size()) * 72.0f + 90.0f;
    if (angle > 360.0f) {
      angle -= 360.0f;
    }

    CEZ2STRParticle* particle =
        static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (particle) {
      particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANTWEAPON\\servantweapon_cast\\servantweapon_cast.str"));
      particle->m_scale = 0.12f;
      EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
      emitter->angle = angle;
      this->emitters.push_back(emitter);
      this->AddEvent(0, "effect\\DK_SERVANTWEAPON.wav", this->frame, 0);
    }
  }
}

// OFFSET: 0x00722F20
// FUNCTION: fn_07395
void CServantWeaponEffect::SetShoot(int column) {
  EZ2STREmitter* emitter =
      reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  AsParticle(emitter)->Load(std::string("servantweapon\\servantweapon_shoot\\servantweapon_shoot.str"));
  if (this->emitters[2][column] != nullptr) {
    FreeParticle(AsParticle(this->emitters[2][column]));
  }
  this->emitters[2][column] = emitter;
}

// OFFSET: 0x00723080
// FUNCTION: fn_07396
void CServantWeaponEffect::SetPhantomShot(long long column) {
  if (column < 5) {
    EZ2STREmitter* emitter =
        reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    if (emitter) {
      AsParticle(emitter)->Load(std::string("servant_phantom\\servant_phantom_shot\\servant_phantom_shot.str"));
      if (this->emitters[2][column] != nullptr) {
        FreeParticle(AsParticle(this->emitters[2][column]));
      }
      this->emitters[2][column] = emitter;
    }
  }
}

// OFFSET: 0x00723210
// FUNCTION: fn_07397
void CServantWeaponEffect::SetSignShot(int column) {
  EZ2STREmitter* emitter =
      reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  AsParticle(emitter)->Load(std::string("servant_sign\\servant_sign_shot\\servant_sign_shot.str"));
  if (this->emitters[2][column] != nullptr) {
    FreeParticle(AsParticle(this->emitters[2][column]));
  }
  this->emitters[2][column] = emitter;
}

// OFFSET: 0x007233D0
// FUNCTION: fn_07399
void CServantWeaponNewEffect::Close() {
  for (EZ2STREmitter* emitter : this->emitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->emitters.clear();
  for (EZ2STREmitter* trail : this->trails) {
    FreeParticle(AsParticle(trail));
  }
  this->trails.clear();
}

// OFFSET: 0x007237E0
// FUNCTION: fn_07400
void CServantWeaponEffect::Slot01() {
  this->retained = 1;

  for (int column = 0; column < 5; ++column) {
    this->emitters[0][column] = nullptr;
    this->emitters[1][column] = nullptr;
    this->emitters[2][column] = nullptr;
  }

  this->offsets[0] = {-7.0f, -2.0f, 0.0f};
  this->offsets[1] = {-10.0f, -12.0f, 0.0f};
  this->offsets[2] = {0.0f, -19.0f, 0.0f};
  this->offsets[3] = {10.0f, -12.0f, 0.0f};
  this->offsets[4] = {7.0f, -2.0f, 0.0f};

  for (int attempt = 0; attempt < 5; ++attempt) {
    for (int column = 0; column < 5; ++column) {
      if (this->emitters[0][column] != nullptr || this->emitters[1][column] != nullptr) {
        continue;
      }

      EZ2STREmitter* emitter =
          reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
      AsParticle(emitter)->Load(std::string("servantweapon\\servantweapon_cast\\servantweapon_cast.str"));
      this->emitters[1][column] = emitter;
    }
  }
}

// OFFSET: 0x00723A90
// FUNCTION: fn_07401
void CServantWeaponNewEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00723AA0
// FUNCTION: fn_07402
bool CServantWeaponEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00723AB0
// FUNCTION: fn_07403
bool CServantWeaponNewEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00723D90
// FUNCTION: fn_07404
EZ2STREmitter* CServantWeaponNewEffect::CreateShot(float angle, int skill) {
  CEZ2STRParticle* particle =
      static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (!particle) {
    return nullptr;
  }

  switch (skill) {
  case 5202:
    particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANTWEAPON\\servantweapon_shoot\\servantweapon_shoot.str"));
    break;
  case 5203:
    particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANT_W_SIGN\\servant_sign_shot\\servant_sign_shot.str"));
    break;
  case 5204:
    particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANT_W_PHANTOM\\servant_phantom_shot\\servant_phantom_shot.str"));
    break;
  case 5205:
    particle->Load(
        std::string("DRAGON_KNIGHT\\DK_SERVANT_W_DEMOL\\servant_demolition_shot\\servant_demolition_shot.str"));
    break;
  }

  particle->m_scale = 0.12f;
  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
  emitter->angle = angle;
  return emitter;
}

// INLINED
static __forceinline bool IsServantEmitterAlive(EZ2STREmitter* emitter) {
  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed);
}

// INLINED
static __forceinline void PlaceServantEmitter(CServantWeaponEffect* effect, EZ2STREmitter* emitter, unsigned int column) {
  if (column >= 5) {
    return;
  }

  const vector3d position = {effect->position[0], effect->position[1], effect->position[2]};
  const matrix view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
  const vector3d& offset = effect->offsets[column];
  const float viewX =
      position.x * view.values[0] + position.y * view.values[3] + position.z * view.values[6] + view.values[9] + offset.x;
  const float viewY =
      position.x * view.values[1] + position.y * view.values[4] + position.z * view.values[7] + view.values[10] + offset.y;
  const float viewZ =
      position.x * view.values[2] + position.y * view.values[5] + position.z * view.values[8] + view.values[11] + offset.z;
  emitter->position.x = viewX * inverse.values[0] + viewY * inverse.values[3] + viewZ * inverse.values[6] + inverse.values[9];
  emitter->position.y = viewX * inverse.values[1] + viewY * inverse.values[4] + viewZ * inverse.values[7] + inverse.values[10];
  emitter->position.z = viewX * inverse.values[2] + viewY * inverse.values[5] + viewZ * inverse.values[8] + inverse.values[11];
  AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
  ++emitter->elapsed;
}

// OFFSET: 0x00724100
// FUNCTION: fn_07405
void CServantWeaponEffect::Slot03() {
  for (int column = 0; column < 5; ++column) {
    EZ2STREmitter* emitter = this->emitters[0][column];
    if (emitter != nullptr) {
      PlaceServantEmitter(this, emitter, column);
    }

    emitter = this->emitters[1][column];
    if (emitter != nullptr) {
      if (IsServantEmitterAlive(emitter)) {
        PlaceServantEmitter(this, emitter, column);
      } else {
        FreeParticle(AsParticle(emitter));
        this->emitters[1][column] = nullptr;
        if (this->emitters[0][column] == nullptr) {
          CEZ2STRParticle* particle =
              static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
          particle->Load(std::string("servantweapon\\servantweapon\\servantweapon.str"));
          particle->m_loop = true;
          this->emitters[0][column] = reinterpret_cast<EZ2STREmitter*>(particle);
        }
      }
    }

    emitter = this->emitters[2][column];
    if (emitter != nullptr) {
      if (IsServantEmitterAlive(emitter)) {
        PlaceServantEmitter(this, emitter, column);
      } else {
        FreeParticle(AsParticle(emitter));
        this->emitters[2][column] = nullptr;
      }
    }
  }
}

// INLINED
static __forceinline void OrbitServantEmitter(const CServantWeaponNewEffect* effect, EZ2STREmitter* emitter) {
  const float radians = emitter->angle / 180.0f * 3.141592f;
  const float sine = std::sin(radians) * 13.0f;
  const float cosine = std::cos(radians) * 13.0f;
  emitter->position.z = sine + effect->position[2];
  emitter->position.y = effect->position[1] - 10.0f;
  emitter->position.x = cosine + effect->position[0];
}

// OFFSET: 0x00724900
// FUNCTION: fn_07406
void CServantWeaponNewEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    if (!AsParticle(*it)->Process(0.0f, 0.0f, 0.0f)) {
      const float angle = (*it)->angle;
      CEZ2STRParticle* particle =
          static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
      if (!particle) {
        FreeParticle(AsParticle(*it));
        it = this->emitters.erase(it);
        continue;
      }

      particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANTWEAPON\\servantweapon\\servantweapon.str"));
      particle->m_loop = true;
      particle->m_scale = 0.12f;
      reinterpret_cast<EZ2STREmitter*>(particle)->angle = angle;
      FreeParticle(AsParticle(*it));
      *it = reinterpret_cast<EZ2STREmitter*>(particle);
    }

    OrbitServantEmitter(this, *it);
    ++it;
  }

  for (auto it = this->trails.begin(); it != this->trails.end();) {
    if (!AsParticle(*it)->Process(0.0f, 0.0f, 0.0f)) {
      FreeParticle(AsParticle(*it));
      it = this->trails.erase(it);
      continue;
    }

    OrbitServantEmitter(this, *it);
    ++it;
  }
}

// OFFSET: 0x00724EB0
// FUNCTION: fn_07407
void CServantWeaponEffect::Slot04() {
  for (int column = 0; column < 5; ++column) {
    if (this->emitters[0][column] != nullptr) {
      AsParticle(this->emitters[0][column])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
    if (this->emitters[1][column] != nullptr) {
      AsParticle(this->emitters[1][column])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
    if (this->emitters[2][column] != nullptr) {
      AsParticle(this->emitters[2][column])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00724F40
// FUNCTION: fn_07408
void CServantWeaponNewEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->trails.begin(); it != this->trails.end(); ++it) {
    AsParticle(*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00725090
// FUNCTION: fn_07409
void CServantWeaponNewEffect::Shoot(int skill) {
  if (!this->emitters.empty()) {
    EZ2STREmitter* weapon = this->emitters.back();
    EZ2STREmitter* shot = this->CreateShot(weapon->angle, skill);
    if (shot) {
      FreeParticle(AsParticle(weapon));
      this->emitters.pop_back();
      this->trails.push_back(shot);
      this->AddEvent(0, "effect\\DK_SERVANTWEAPON.wav", this->frame, 0);
    }
  }
}

// OFFSET: 0x00725130
// FUNCTION: fn_07410
CSevereRainStorm::CSevereRainStorm() : CEffect(-1) {
  this->unknown_0050 = 0;
}

// OFFSET: 0x0071D650
// FUNCTION: fn_07334
void __stdcall fn_07334(CParticle* particle) {
  particle->alpha = 0;
  *reinterpret_cast<int*>(particle->unknown_0048) = rand() % 50 + 150;
  particle->unknown_00A8 = 0;
  particle->unknown_00E8 = 0;
  InitParticleQuad(particle->vertices);

  const int spread = rand() % 15 + 2;
  const int angle = rand() % 360;
  particle->unknown_00D4 = 0.0f;
  particle->unknown_00D8 = static_cast<float>(spread) * g_sinTable[angle];
  particle->unknown_00D0 = static_cast<float>(spread) * g_cosTable[angle];
  particle->scale = static_cast<float>(rand() % 10) * 0.1f + 0.5f;
  particle->unknown_00C0 = static_cast<float>(rand() % 30) * 0.01f + 0.05f;
}

// OFFSET: 0x0071D760
// FUNCTION: fn_07335
void CNewCloudKillEffect::Slot01() {
  this->unknown_00F4 = static_cast<int>(timeGetTime());
  this->unknown_00F8 = static_cast<int>(timeGetTime());

  CParticle* circle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (circle) {
    circle->SetTexture("effect\\new_cloud_kill\\ef_cloud_circle.tga");
    circle->unknown_0004 = this->id;
    circle->unknown_000C = 1;
    circle->unknown_0010 = 0;
    circle->unknown_0008 = 1;
    circle->scale = 20.0f;
    circle->alpha = 0xFF;
    circle->unknown_00C4 = 0;
    circle->unknown_00E8 = 60;
    circle->unknown_00A8 = 0;
    InitParticleQuad(circle->vertices);
    this->unknown_00CC.push_back(reinterpret_cast<EZ2STREmitter*>(circle));
  }

  for (int i = 0; i < 100; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->unknown_0004 = this->id;
      particle->unknown_000C = i;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->SetTexture("effect\\new_cloud_kill\\ef_cloud_kill.tga");
      fn_07337(particle);
      this->unknown_00D4.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }

  for (int i = 0; i < 30; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->unknown_0004 = this->id;
      particle->unknown_000C = i;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->SetTexture("effect\\new_cloud_kill\\ef_cloud_kill.tga");
      fn_07339(particle);
      particle->unknown_00E8 = -std::max(0, rand() % 71);
      this->unknown_00DC.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }

  CParticle* lightning = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (lightning) {
    lightning->unknown_0004 = this->id;
    lightning->unknown_000C = 1;
    lightning->unknown_0010 = 0;
    lightning->unknown_0008 = 1;
    lightning->SetTexture("effect\\new_cloud_kill\\ef_comm_lightning_cloud.tga");
    lightning->alpha = 0xFF;
    lightning->depthOffset = 0.035f;
    fn_07338(lightning);
    lightning->unknown_004C = 50;
    this->unknown_00E4.push_back(reinterpret_cast<EZ2STREmitter*>(lightning));
  }

  for (int i = 0; i < 30; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->unknown_0004 = this->id;
      particle->unknown_000C = i;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->SetTexture("effect\\new_cloud_kill\\ef_cloud_kill.tga");
      fn_07334(particle);
      particle->unknown_00E8 = -std::max(0, rand() % 80);
      this->unknown_00EC.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }
}

// OFFSET: 0x0071DB70
// FUNCTION: fn_07337
void __stdcall fn_07337(CParticle* particle) {
  particle->unknown_00A9[0] = rand() % 2;

  const int angle = rand() % 360;
  particle->unknown_00D4 = 0.0f;
  particle->alpha = 0xFF;
  particle->unknown_00D0 = g_cosTable[angle] * 18.0f;
  particle->unknown_00D8 = g_sinTable[angle] * 18.0f;
  particle->scale = static_cast<float>(rand() % 31) * 0.1f + 2.0f;
  particle->unknown_00A8 = 0;
  particle->unknown_00C0 = static_cast<float>(rand() % 31) * 0.01f + 0.05f;
  InitParticleQuad(particle->vertices);
}

// OFFSET: 0x0071DC70
// FUNCTION: fn_07338
void __stdcall fn_07338(CParticle* particle) {
  particle->scale = static_cast<float>(rand() % 5) + 10.0f;
  particle->unknown_00A8 = 10;
  particle->unknown_00A9[0] = rand() % 4;
  particle->unknown_004C = rand() % 50 + 10;
  *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(rand() % 360);
  particle->unknown_00D8 = static_cast<float>(rand() % 10) - 5.0f;
  particle->unknown_00D0 = static_cast<float>(rand() % 10) - 5.0f;
  particle->unknown_00D4 = -10.0f;
  particle->unknown_00E8 = 0;
  InitParticleQuad(particle->vertices);
}

// OFFSET: 0x0071DD80
// FUNCTION: fn_07339
void __stdcall fn_07339(CParticle* particle) {
  particle->alpha = 0;
  particle->unknown_00A8 = 0;
  particle->unknown_00E8 = 0;
  InitParticleQuad(particle->vertices);

  particle->scale = static_cast<float>(rand() % 51) * 0.1f + 7.0f;
  particle->unknown_00D4 = particle->scale * -0.5f;

  const int spread = rand() % 13 + 2;
  const int angle = rand() % 360;
  particle->unknown_00D8 = static_cast<float>(spread) * g_sinTable[angle];
  particle->unknown_00D0 = static_cast<float>(spread) * g_cosTable[angle];

  particle->unknown_00A9[0] = rand() % 4;
  *reinterpret_cast<int*>(particle->unknown_0048) = rand() % 151 + 50;
  particle->unknown_00C0 = static_cast<float>(rand() % 10) * 0.01f + 0.05f;
}

// OFFSET: 0x0070C2B0
// FUNCTION: fn_07128
void CRootTwistEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070D000
// FUNCTION: fn_07143
void CSpriteMableEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070DE50
// FUNCTION: fn_07165
void CTunaPartyEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070EAD0
// FUNCTION: fn_07175
void CCurseOfCubeEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070F310
// FUNCTION: fn_07184
void CWarningPlaneEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00710C40
// FUNCTION: fn_07208
void CEternalSlashAttackEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00716960
// FUNCTION: fn_07276
void CFullScreenEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x00717DA0
// FUNCTION: fn_07284
void CGaleStorm::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x007211E0
// FUNCTION: fn_07369
void CRandomOptEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0070FB10
// FUNCTION: fn_07192
void CCylinderEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070C520
// FUNCTION: fn_07135
void CSprArrowEffect::Close() {
  if (this->unknown_00CC != 0) {
    FreeParticle(reinterpret_cast<CParticle*>(this->unknown_00CC));
    this->unknown_00CC = 0;
  }
}

// OFFSET: 0x0070D460
// FUNCTION: fn_07150
void CStemSpearEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070D980
// FUNCTION: fn_07158
void CSvgSpiritEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x00711250
// FUNCTION: fn_07215
void CEternalSlashCountEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x007205A0
// FUNCTION: fn_07359
void CRandomHitEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x00711570
// FUNCTION: fn_07222
void CFallenAngelEffect::Close() {
  FreeParticle(AsParticle(this->emitter));
}

// OFFSET: 0x007221B0
// FUNCTION: fn_07383
void CSavageImpactEffect::Close() {
  FreeParticle(AsParticle(this->emitter));
}

// OFFSET: 0x007193B0
// FUNCTION: fn_07306
void CDropItemEffect::Close() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i] != nullptr) {
      FreeParticle(AsParticle(this->emitters[i]));
    }
  }
}

// OFFSET: 0x00711A20
// FUNCTION: fn_07230
void CFloralFlareRoad::Close() {
  FreeParticle(AsParticle(this->unknown_00CC));
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0071FD50
// FUNCTION: fn_07353
void CPneumaticusProcellaEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
  for (std::vector<EZ2STREmitter*>::iterator it = this->emitters.begin(); it < this->emitters.end(); ++it) {
    if (*it != nullptr) {
      FreeParticle(AsParticle(*it));
      *it = nullptr;
    }
  }
  this->emitters.clear();
}

// OFFSET: 0x00723370
// FUNCTION: fn_07398
void CServantWeaponEffect::Close() {
  for (int column = 0; column < 5; ++column) {
    if (this->emitters[0][column] != nullptr) {
      FreeParticle(AsParticle(this->emitters[0][column]));
      this->emitters[0][column] = nullptr;
    }
    if (this->emitters[1][column] != nullptr) {
      FreeParticle(AsParticle(this->emitters[1][column]));
      this->emitters[1][column] = nullptr;
    }
    if (this->emitters[2][column] != nullptr) {
      FreeParticle(AsParticle(this->emitters[2][column]));
      this->emitters[2][column] = nullptr;
    }
  }
}

// OFFSET: 0x00712350
// FUNCTION: fn_07237
void CFootprintEffect::Close() {
  for (EZ2STREmitter* emitter : this->emitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->emitters.clear();
}

// OFFSET: 0x00710790
// FUNCTION: fn_07201
void CDancingKnifeEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D8.clear();
}

// OFFSET: 0x0071C4B0
// FUNCTION: fn_07322
void CJupitelThunderHit::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D8.clear();
}

// OFFSET: 0x00721860
// FUNCTION: fn_07376
void CRichsCoinEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
  for (auto it = this->coins.begin(); it != this->coins.end(); ++it) {
    FreeParticle(*it);
  }
  this->coins.clear();
}

// OFFSET: 0x00725490
// FUNCTION: fn_07413
void CSevereRainStorm::SpawnParticles() {
  switch (this->id) {
  case 1752:
    if (this->unknown_00D4 % (4 - (this->unknown_00B0 + 1) / 2) == 0) {
      this->SpawnCrystalRain();
    }
    break;
  case 1276:
    if (this->unknown_00D4 % 3 == 0) {
      this->SpawnSevereRain();
    }
    break;
  case 2026:
    if (this->unknown_00D4 % 5 == 0) {
      vector3d position = {this->position[0], this->position[1], this->position[2]};
      position.x = (static_cast<float>(std::max(0, rand() % 451)) - 225.0f) * 0.1f + position.x;
      position.z = (static_cast<float>(std::max(0, rand() % 451)) - 225.0f) * 0.1f + position.z;

      CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
      particle->Open(std::string("diamond_storm\\diamond_storm\\diamond_storm.str"));
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
    break;
  case 2165:
    if (this->unknown_00D4 % (9 - this->unknown_00B0 / 3) == 0) {
      const long long ranges[10] = {3, 3, 3, 5, 5, 5, 7, 7, 7, 9};
      const long long spread = ranges[std::min(static_cast<unsigned int>(this->unknown_00B0), 10u) - 1] * 50;
      vector3d position = {this->position[0], this->position[1], this->position[2]};
      const float half = static_cast<float>(spread) * 0.5f;
      position.x = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + position.x;
      position.z = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + position.z;

      CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
      particle->Load(std::string("crossrain\\cross_rain\\cross_rain.str"));
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
    break;
  case 2348:
    if (this->ActorExists() && this->unknown_00D4 % 15 == 0) {
      this->SpawnTwinklingGalaxy(this->unknown_00D4 / 15 % 4);
    }
    break;
  case 2349:
    if (this->ActorExists() && this->unknown_00D4 % 18 == 0) {
      this->SpawnGalaxyStar();
    }
    break;
  case 2350:
    if (this->ActorExists() && this->unknown_00D4 % 18 == 0) {
      this->SpawnStarCannon();
    }
    break;
  case 2351:
    if (this->ActorExists() && this->unknown_00D4 % 15 == 0) {
      this->SpawnGrenades();
    }
    break;
  case 2354:
    if (this->ActorExists() && this->unknown_00D4 % 15 == 0) {
      this->SpawnBombardHits();
    }
    break;
  case 2462:
    if (this->unknown_00D4 % (9 - this->unknown_00B0 / 3) == 0) {
      this->SpawnCrossRain();
    }
    break;
  }
}

// OFFSET: 0x00725D60
// FUNCTION: fn_07414
void CSevereRainStorm::SpawnCrossRain() {
  const int ranges[10] = {3, 3, 3, 5, 5, 5, 7, 7, 7, 9};
  const int index = std::max(0, std::min(this->unknown_00B0 - 1, 9));
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const int spread = ranges[index] * 50;

  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(std::max(0, rand() % (spread + 1))) - half) * 0.1f + position.x;
  position.z = (static_cast<float>(std::max(0, rand() % (spread + 1))) - half) * 0.1f + position.z;
  position.y -= 10.0f;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
  particle->Load(std::string("IMPERIAL_GUARD\\IG_CROSS_RAIN\\new_crossrain\\new_cross_rain\\new_cross_rain.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
}

// OFFSET: 0x00726110
// FUNCTION: fn_07415
void CSevereRainStorm::SpawnGrenades() {
  const long long ranges[5] = {11, 11, 11, 9, 9};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->unknown_00B0), 5u) - 1] * 50;
  for (int count = 0; count < (this->unknown_00B0 + 1) / 2; ++count) {
    const float half = static_cast<float>(spread) * 0.5f;
    const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
    vector3d position = base;
    position.x = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + base.x;
    position.z = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + base.z;

    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    if (particle) {
      particle->Load(std::string("NIGHT_WATCH\\NW_GRENADES_DROPPING\\grenades_dropping_bottom\\grenades_dropping_bottom.str"));
      particle->position = position;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }

    particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (particle) {
      particle->Load(std::string("NIGHT_WATCH\\NW_GRENADES_DROPPING\\grenades_dropping\\grenades_dropping.str"));
      particle->position = position;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }
}

// OFFSET: 0x007265B0
// FUNCTION: fn_07416
void CSevereRainStorm::SpawnBombardHits() {
  for (int count = 0; count < 3; ++count) {
    const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
    vector3d position = base;
    position.x = (static_cast<float>(std::max(0, rand() % 451)) - 225.0f) * 0.1f + base.x;
    position.z = (static_cast<float>(std::max(0, rand() % 451)) - 225.0f) * 0.1f + base.z;

    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (particle) {
      particle->Load(std::string("NIGHT_WATCH\\NW_MISSION_BOMBARD\\mission_bombard\\mission_bombard_hit.str"));
      particle->position = position;
      particle->m_scale = static_cast<float>(std::max(0, rand() % 51) + 50) * 0.01f;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }
}

// OFFSET: 0x00726890
// FUNCTION: fn_07417
void CSevereRainStorm::SpawnCrystalRain() {
  const long long ranges[5] = {11, 13, 15, 17, 19};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->unknown_00B0), 5u) - 1] * 50;
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + position.x;
  position.z = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + position.z;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 13));
  particle->Load(std::string("rain_of_crystal\\rain_of_crystal_attack_bottom\\rain_of_crystal_attack_bottom.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
  particle->Load(std::string("rain_of_crystal\\rain_of_crystal_attack\\rain_of_crystal_attack.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));

  if (rand() % 7 == 0) {
    this->AddEvent(0, "effect\\AG_RAIN_OF_CRYSTAL.wav", this->frame + 1, 0);
  }
}

// OFFSET: 0x00726BE0
// FUNCTION: fn_07418
void CSevereRainStorm::SpawnSevereRain() {
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  position.z += 10.0f;
  if (std::max(0, rand() % 10) >= 5) {
    position.x = static_cast<float>(std::max(0, rand() % 251)) * 0.1f + position.x;
  } else {
    position.x = position.x - static_cast<float>(std::max(0, rand() % 251)) * 0.1f;
  }
  if (std::max(0, rand() % 10) >= 5) {
    position.z = static_cast<float>(std::max(0, rand() % 251)) * 0.1f + position.z;
  } else {
    position.z = position.z - static_cast<float>(std::max(0, rand() % 251)) * 0.1f;
  }

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 1));
  if (g_serviceOptions.Get(229)) {
    particle->Open(std::string("mineffect\\new_severerainstorm\\new_severerainstorm\\new_severerainstorm.str"));
  } else {
    particle->Open(std::string("new_severerainstorm\\new_severerainstorm\\new_severerainstorm.str"));
  }
  particle->flags = 5;
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
}

// OFFSET: 0x00726E30
// FUNCTION: fn_07419
void CSevereRainStorm::SpawnTwinklingGalaxy(int index) {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (particle) {
    particle->Load(std::string("SKY_EMPEROR\\SKE_TWINKLING_GALAXY\\twinkling_galaxy_bottom\\ske_twinkling_galaxy_bottom.str"));
    particle->unknown_00D0 = 0.2f;
    particle->unknown_00D4 = -13.5f;
    particle->unknown_00D8 = 0.0f;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    particle->Load(FormatOwnedString("SKY_EMPEROR\\SKE_TWINKLING_GALAXY\\twinkling_galaxy\\ske_twinkling_galaxy%02d.str", index));
    particle->unknown_00D0 = 0.2f;
    particle->unknown_00D4 = -13.5f;
    particle->unknown_00D8 = 0.0f;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  this->AddEvent(0, "effect\\SKY_EMPEROR\\ske_star_cannon.wav", this->frame + 1, 0);
}

// OFFSET: 0x00727050
// FUNCTION: fn_07420
void CSevereRainStorm::SpawnStarCannon() {
  const long long ranges[5] = {13, 13, 11, 11, 9};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->unknown_00B0), 5u) - 1] * 50;
  const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
  vector3d position = base;
  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + base.x;
  position.z = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + base.z;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (particle) {
    particle->Load(std::string("SKY_EMPEROR\\SKE_STAR_CANNON\\star_cannon_bottom\\ske_star_cannon_bottom.str"));
    particle->position = position;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    particle->Load(std::string("SKY_EMPEROR\\SKE_STAR_CANNON\\star_cannon\\ske_star_cannon.str"));
    particle->position = position;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  this->AddEvent(0, "effect\\SKY_EMPEROR\\ske_star_cannon.wav", this->frame + 1, 0);
}

// OFFSET: 0x00727450
// FUNCTION: fn_07421
void CSevereRainStorm::SpawnGalaxyStar() {
  const long long ranges[5] = {13, 13, 11, 11, 9};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->unknown_00B0), 5u) - 1] * 50;
  const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
  vector3d position = base;
  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + base.x;
  position.z = (static_cast<float>(std::max(0, rand() % (static_cast<int>(spread) + 1))) - half) * 0.1f + base.z;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (particle) {
    particle->Load(std::string("SKY_EMPEROR\\SKE_TWINKLING_GALAXY\\twinkling_galaxy_bottom\\ske_twinkling_galaxy_bottom.str"));
    particle->position = position;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    particle->Load(std::string("SKY_EMPEROR\\SKE_TWINKLING_GALAXY\\twinkling_galaxy\\ske_twinkling_galaxy03.str"));
    particle->position = position;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  this->AddEvent(0, "effect\\SKY_EMPEROR\\ske_star_cannon.wav", this->frame + 1, 0);
}

// OFFSET: 0x00727850
// FUNCTION: fn_07422
void CSevereRainStorm::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x007278C0
// FUNCTION: fn_07423
void CSevereRainStorm::Slot01() {
  this->unknown_00D4 = 0;
  if (this->id == 2348) {
    this->retained = 1;
  }
}

// OFFSET: 0x007278E0
// FUNCTION: fn_07424
bool CSevereRainStorm::Slot06() {
  switch (this->id) {
  case 1276:
    return this->unknown_00D4 <= 240;
  case 2026:
    return EffectLifetime(2026) >= this->unknown_00D4;
  case 1752:
  case 2165:
  case 2462:
    return this->ActorExists();
  case 2348:
  case 2349:
  case 2350:
  case 2351:
  case 2354:
    if (this->ActorExists() != true && this->emitters.size() == 0) {
      return false;
    }
    break;
  }
  return true;
}

// OFFSET: 0x00727A10
// FUNCTION: fn_07425
void CSevereRainStorm::Slot03() {
  this->SpawnParticles();
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime < 0.0f) {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
      continue;
    }

    if (this->id == 2348 && emitter) {
      const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
      const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
      const vector3d position = {this->position[0], this->position[1], this->position[2]};

      const float vx = position.y * view.values[3] + position.x * view.values[0] + position.z * view.values[6] + view.values[9] +
                       emitter->offset.x;
      const float vy = position.x * view.values[1] + position.y * view.values[4] + position.z * view.values[7] + view.values[10] +
                       emitter->offset.y;
      const float vz = position.x * view.values[2] + position.y * view.values[5] + position.z * view.values[8] + view.values[11] +
                       emitter->offset.z;

      emitter->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
      emitter->position.y = vx * inverse.values[1] + vy * inverse.values[4] + vz * inverse.values[7] + inverse.values[10];
      emitter->position.z = vx * inverse.values[2] + vy * inverse.values[5] + vz * inverse.values[8] + inverse.values[11];
      AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
    }

    emitter = *it;
    if (emitter) {
      AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
    }
    ++it;
  }
  ++this->unknown_00D4;
}

// OFFSET: 0x00727C70
// FUNCTION: fn_07426
void CSevereRainStorm::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter) {
      AsParticle(emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0070B820
// FUNCTION: fn_07119
CCrimsonArrowEffect::CCrimsonArrowEffect() : CEffect(-1), emitter(nullptr) {
}

// OFFSET: 0x0070B940
// FUNCTION: fn_07121
void CCrimsonArrowEffect::Close() {
  if (this->emitter) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x0070B970
// FUNCTION: fn_07122
void CCrimsonArrowEffect::Slot01() {
  CEZ2STRParticle* arrow = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (!arrow) {
    return;
  }

  vector3d to{this->sourcePosition[0], this->sourcePosition[1], this->sourcePosition[2]};
  vector3d from{this->position[0], this->position[1], this->position[2]};
  float fromReciprocalDepth = 0.0f;
  float toReciprocalDepth = 0.0f;
  float fromX;
  float fromY;
  float toX;
  float toY;
  g_renderer->ProjectViewport(&from.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
  g_renderer->ProjectViewport(&to.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

  const float dx = toX - fromX;
  const float dy = toY - fromY;
  const float length = SquareRoot(dy * dy + dx * dx);
  double angle = static_cast<float>(std::acos(static_cast<double>(dx / length))) / 3.141592f * 180.0f;
  if (fromY > toY) {
    angle = 360.0 - angle;
  }

  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(arrow);
  switch (this->id) {
  case 1756:
    arrow->Load("crimsonarrow\\crimsonarrow\\crimsonarrow.str");
    emitter->offset.x = 0.0f;
    emitter->offset.y = -13.0f;
    emitter->offset.z = 0.0f;
    arrow->m_angle = static_cast<float>(angle);
    this->AddEvent(0, "effect\\AG_CRIMSON_ARROW1.wav", 0, 0);
    break;
  case 2074:
    arrow->Load("rhythmshooting\\rhythmshooting_shot\\rhythmshooting_shot.str");
    emitter->offset.x = 0.0f;
    emitter->offset.y = -8.0f;
    emitter->offset.z = 0.0f;
    arrow->m_angle = static_cast<float>(angle + 180.0);
    this->AddEvent(0, "effect\\TR_RHYTHMSHOOTING.wav", 0, 0);
    break;
  case 2114:
    arrow->Load("frenzy_shot\\frenzy_shot\\frenzy_shot.str");
    emitter->offset.x = 0.0f;
    emitter->offset.y = -4.0f;
    emitter->offset.z = 0.0f;
    arrow->m_angle = static_cast<float>(angle);
    this->AddEvent(0, "effect\\ABC_FRENZY_SHOT.wav", 0, 0);
    break;
  case 2154:
    arrow->Load("unlucky_rush\\unlucky_rush\\unlucky_rush.str");
    arrow->m_angle = static_cast<float>(angle);
    break;
  case 2195:
    arrow->Load("judgement_cross\\judgement_cross_shot\\judgement_cross_shot.str");
    arrow->m_angle = static_cast<float>(angle + 180.0);
    break;
  }

  this->emitter = emitter;
}

// OFFSET: 0x0070DE90
// FUNCTION: fn_07166
void CTunaPartyEffect::Slot01() {
  CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (!particle) {
    return;
  }

  particle->unknown_0004 = this->id;
  particle->unknown_000C = 1;
  particle->unknown_0010 = 0;
  particle->unknown_0008 = 1;
  particle->SetSprite("이팩트\\su_tunaparty\\su_tunaparty.spr");
  particle->SetAction("이팩트\\su_tunaparty\\su_tunaparty.act");
  particle->unknown_0054 = 0.0f;
  particle->loopMode = 3;
  this->unknown_002C = 9999;
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
}

// OFFSET: 0x0070DF40
// FUNCTION: fn_07168
void CTunaPartyEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CSpriteParticle* particle = AsSprite(*it);
    const float phase = particle->unknown_0054 + 0.05f;
    particle->unknown_0054 = phase;
    const float cosine = std::cos(phase) * 10.0f;
    const float sine = std::sin(phase) * 10.0f;
    particle->position.y = this->position[1] - 10.0f;
    particle->position.x = this->position[0] + sine;
    particle->position.z = this->position[2] + cosine;
    float angle = phase / 3.141592f * 180.0f;
    if (angle > 360.0f) {
      angle = std::fmod(angle, 360.0f);
    }
    angle -= static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
    if (angle < 0.0f) {
      angle += 360.0f;
    }
    int direction = static_cast<int>((angle - 157.5) / 45.0);
    if (direction < 0) {
      direction += 8;
    }
    ++particle->unknown_00E8;
    particle->frame = direction;
  }
}

// OFFSET: 0x0070C2F0
// FUNCTION: fn_07129
void CRootTwistEffect::Slot01() {
  CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (particle) {
    particle->unknown_0004 = this->id;
    particle->unknown_000C = 0;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetSprite("이팩트\\su_sv_roottwist\\su_sv_roottwist.spr");
    particle->SetAction("이팩트\\su_sv_roottwist\\su_sv_roottwist.act");
    particle->unknown_00EC |= 0x200;
    particle->frameDelay = 8.0f;
    particle->depthOffset = 0.00015f;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }
  this->unknown_002C = 9999;
}

// OFFSET: 0x0070D9B0
// FUNCTION: fn_07159
void CSvgSpiritEffect::Slot01() {
  this->emitter = reinterpret_cast<EZ2STREmitter*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (!this->emitter) {
    return;
  }

  CSpriteParticle* sprite = AsSprite(this->emitter);
  sprite->unknown_0004 = this->id;
  sprite->unknown_000C = 0;
  sprite->unknown_0010 = 0;
  sprite->unknown_0008 = 1;
  sprite->SetSprite("몬스터\\svg_spirit.spr");
  sprite->SetAction("몬스터\\svg_spirit.act");
  this->emitter->velocity.x = 0.0f;
  this->emitter->velocity.y = 0.0f;
  this->emitter->velocity.z = 0.0f;
  this->unknown_002C = 16;
  const float deltaX = this->sourcePosition[0] - this->position[0];
  const float deltaZ = this->sourcePosition[2] - this->position[2];
  float angle = (static_cast<float>(std::atan2(static_cast<double>(deltaX), static_cast<double>(-deltaZ))) / 3.141592f) * 180.0f;
  if (angle < 0.0f) {
    angle += 360.0f;
  }
  angle = 180.0f - angle;
  if (angle < 0.0f) {
    angle += 360.0f;
  }
  this->emitter->baseAngle = static_cast<int>(angle);
  this->emitter->speed = SquareRoot(deltaX * deltaX + deltaZ * deltaZ) / static_cast<float>(this->unknown_002C);
  this->emitter->elapsed = 0;
}

// OFFSET: 0x0070E240
// FUNCTION: fn_07172
void CCurseOfCubeEffect::SpawnShock() {
  CEZ2STRParticle* particle;
  if (this->id == 0x901) {
    particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (!particle) {
      return;
    }
    particle->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_shock\\npc_cur_r_shock.str");
  } else if (this->id == 0x902) {
    particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (!particle) {
      return;
    }
    particle->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_shock\\npc_cur_b_shock.str");
  } else {
    return;
  }

  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
}

// OFFSET: 0x0071F0C0
// FUNCTION: fn_07342
void CNewCloudKillEffect::Slot04() {
  for (auto it = this->unknown_00CC.begin(); it != this->unknown_00CC.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->unknown_00A8 != 0 || !particle) {
      continue;
    }

    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }

  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->unknown_00A8 != 0 || !particle) {
      continue;
    }

    const int frame = static_cast<signed char>(particle->unknown_00A9[0]);
    const ParticleQuad quad = {false, 0.5f, 0.75f, static_cast<float>(frame + 1) * 0.25f, static_cast<float>(frame + 2) * 0.25f, false, true};
    particle->Render(quad);
  }

  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->unknown_00E8 <= 0 || !particle) {
      continue;
    }

    const int frame = static_cast<signed char>(particle->unknown_00A9[0]);
    const ParticleQuad quad = {false, 0.0f, 0.25f, static_cast<float>(frame) * 0.25f, static_cast<float>(frame + 1) * 0.25f, false, true};
    particle->Render(quad);
  }

  for (auto it = this->unknown_00E4.begin(); it != this->unknown_00E4.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (static_cast<signed char>(particle->unknown_00A8) <= 0 || !particle) {
      continue;
    }

    const int frame = static_cast<signed char>(particle->unknown_00A9[0]);
    const ParticleQuad quad = {false,
                               static_cast<float>(frame / 2) * 0.5f,
                               static_cast<float>(frame / 2 + 1) * 0.5f,
                               static_cast<float>(frame % 2) * 0.5f,
                               static_cast<float>(frame % 2 + 1) * 0.5f,
                               false,
                               true};
    particle->Render(quad);
  }

  for (auto it = this->unknown_00EC.begin(); it != this->unknown_00EC.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->unknown_00E8 <= 0 || !particle) {
      continue;
    }

    const ParticleQuad quad = {false, 0.5f, 0.75f, 0.0f, 0.25f, false, true};
    particle->Render(quad);
  }
}

// OFFSET: 0x00710270
// FUNCTION: fn_07199
void CDancingKnifeEffect::SpawnFirstStrike() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  bottom->Load(std::string("dancing_knife\\dancing_knife_bottom\\dancing_knife_skill_bottom_01.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));

  CEZ2STRParticle* knife = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  knife->Load(std::string("dancing_knife\\dancing_knife\\dancing_knife_skill_01.str"));
  this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(knife));
}

// OFFSET: 0x007104F0
// FUNCTION: fn_07200
void CDancingKnifeEffect::SpawnSecondStrike() {
  this->secondStrike = 1;

  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  bottom->Load(std::string("dancing_knife\\dancing_knife_bottom\\dancing_knife_skill_bottom_02.str"));
  bottom->m_loop = true;
  bottom->m_actorId = this->actor;
  this->unknown_00D8.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));

  CEZ2STRParticle* knife = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  knife->Load(std::string("dancing_knife\\dancing_knife\\dancing_knife_skill_02.str"));
  knife->m_loop = true;
  knife->m_actorId = this->actor;
  this->unknown_00D8.push_back(reinterpret_cast<EZ2STREmitter*>(knife));
}

// OFFSET: 0x00710800
// FUNCTION: fn_07202
void CDancingKnifeEffect::Slot01() {
  this->secondStrike = 0;
  this->SpawnFirstStrike();
  this->AddEvent(0, "effect\\SHC_DANCING_KNIFE.wav", 0, 0);
}

// OFFSET: 0x00710840
// FUNCTION: fn_07204
void CDancingKnifeEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime == 0.0f || emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed)) {
      if (emitter) {
        emitter->position.x = this->position[0] + 0.0f;
        emitter->position.y = this->position[1] - 12.0f;
        emitter->position.z = this->position[2] + 0.0f;
        AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
    }
  }

  if (this->emitters.empty() && !this->secondStrike) {
    this->SpawnSecondStrike();
  }

  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter) {
      emitter->position.x = this->position[0] + 0.0f;
      emitter->position.y = this->position[1] - 12.0f;
      emitter->position.z = this->position[2] + 0.0f;
      AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x0070E400

// OFFSET: 0x0070E770
// FUNCTION: fn_07174
void CCurseOfCubeEffect::SpawnStart() {
  if (this->id == 0x901) {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    if (bottom) {
      bottom->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_cube\\npc_cur_r_cube_bottom_start.str");
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));
    }
    CEZ2STRParticle* cube = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (cube) {
      cube->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_cube\\npc_cur_r_cube_start.str");
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(cube));
    }
  } else if (this->id == 0x902) {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    if (bottom) {
      bottom->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_cube\\npc_cur_b_cube_bottom_start.str");
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));
    }
    CEZ2STRParticle* cube = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (cube) {
      cube->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_cube\\npc_cur_b_cube_start.str");
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(cube));
    }
  }
}

// OFFSET: 0x0070EB90
// FUNCTION: fn_07179
void CCurseOfCubeEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime == 0.0f || emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed)) {
      if (emitter) {
        const vector3d position = {this->position[0], this->position[1], this->position[2]};
        const matrix view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
        const matrix inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

        const float vx = position.x * view.values[0] + position.y * view.values[3] + position.z * view.values[6] + view.values[9] +
                         this->unknown_00DC;
        const float vy = position.x * view.values[1] + position.y * view.values[4] + position.z * view.values[7] + view.values[10] +
                         this->unknown_00E0;
        const float vz = position.y * view.values[5] + position.x * view.values[2] + position.z * view.values[8] + view.values[11] +
                         this->unknown_00E4;

        emitter->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
        emitter->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
        emitter->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];
        AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
    }
  }

  if (this->unknown_00D8 == 1 && this->emitters.empty()) {
    this->SpawnLoop();
    if (!this->emitters.empty()) {
      this->unknown_00D8 = 2;
    }
  }
}
