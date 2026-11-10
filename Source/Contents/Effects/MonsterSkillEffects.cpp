#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/GravityControlEffect.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"
#include "Utility/Text/StringUtil.h"
#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>
#include <format>

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

namespace {
// INLINED
__forceinline int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}
}

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

// INLINED
static bool IsEmitterAlive(const EZ2STREmitter* emitter) {
  if (0.0f == emitter->lifetime) {
    return true;
  }

  return emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed);
}

// OFFSET: 0x0070C070
// FUNCTION: fn_07123
bool CCrimsonArrowEffect::IsAlive() {
  EZ2STREmitter* emitter = this->emitter;
  if (emitter) {
    if (0.0f == emitter->lifetime) {
      return true;
    }

    return emitter->lifetime * 60.0f > static_cast<float>(emitter->elapsed);
  }

  return false;
}

// OFFSET: 0x0070C150
// FUNCTION: fn_07126
CRootTwistEffect::CRootTwistEffect() : CEffect(1110) {
}

// OFFSET: 0x0070C0B0
// FUNCTION: fn_07124
void CCrimsonArrowEffect::OnProcess() {
  if (!this->emitter) {
    return;
  }

  EZ2STREmitter* emitter = this->emitter;
  const vector3d base = {this->position[0], this->position[1], this->position[2]};
  const float x = emitter->offset.x + base.x;
  const float y = emitter->offset.y + base.y;
  const float z = emitter->offset.z + base.z;
  emitter->position.x = x;
  emitter->position.y = y;
  emitter->position.z = z;
  AsParticle(this->emitter)->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0070C120
// FUNCTION: fn_07125
void CCrimsonArrowEffect::Render() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0070C3B0
// FUNCTION: fn_07130
void CRootTwistEffect::InitEvents() {
  if (this->id == 1110) {
    this->AddEvent(0, "effect\\su_roottwist.wav", 0, 0);
  }
}

// OFFSET: 0x0070C3D0
// FUNCTION: fn_07131
bool CRootTwistEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0070C3E0
// FUNCTION: fn_07132
void CRootTwistEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    const vector3d position = {this->position[0] + 0.0f, this->position[1] + 3.0f, this->position[2] + 0.0f};
    EZ2STREmitter* emitter = *it;
    ++emitter->elapsed;
    emitter->position = position;
  }
}

// OFFSET: 0x0070C440
// FUNCTION: fn_07133
void CRootTwistEffect::Render() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->Draw(1);
  }
}

// OFFSET: 0x0070C7F0
// FUNCTION: fn_07137
bool CSprArrowEffect::IsAlive() {
  return this->alive;
}

// OFFSET: 0x0070C800
// FUNCTION: fn_07138
void CSprArrowEffect::OnProcess() {
  const float ticks =
      static_cast<float>(static_cast<double>(timeGetTime() - static_cast<unsigned int>(this->startTick)) / 24.0);
  if (this->delayFrames > ticks) {
    return;
  }

  CSpriteParticle* particle = reinterpret_cast<CSpriteParticle*>(this->spriteParticle);
  if (!particle || !particle->action) {
    return;
  }

  if (!this->launched) {
    this->launched = 1;
    this->SetSourceActor(this->sourceActor);

    const float dx = this->sourcePosition[0] - particle->position.x;
    const float dy = this->sourcePosition[1] - particle->position.y;
    const float dz = this->sourcePosition[2] - particle->position.z;
    this->velocityX = dx * 0.125f;
    this->velocityY = dy * 0.125f;
    this->velocityZ = dz * 0.125f;

    const float lengthSquared = dy * dy + dx * dx + dz * dz;
    const float length = SquareRoot(lengthSquared);
    if (length < 40.0f) {
      this->travelSpeedScale = 40.0f / length;
      this->velocityX *= this->travelSpeedScale;
      this->velocityY *= this->travelSpeedScale;
      this->velocityZ *= this->travelSpeedScale;
    }

    float angle = 0.0f;
    if (dx != 0.0f || dz != 0.0f) {
      angle = static_cast<float>(std::atan2(static_cast<double>(-dx), static_cast<double>(-dz))) / 3.141592f * 180.0f;
      if (angle < 0.0f) {
        angle += 360.0f;
      }
    }
    this->heading = angle;

    const float inverse = 1.0f / SquareRoot(lengthSquared);
    particle->position.x += inverse * dx * 2.5f;
    particle->position.y += inverse * dy * 2.5f;
    particle->position.z += inverse * dz * 2.5f;
  }

  ++particle->elapsedFrames;
  const float time = ticks - this->delayFrames;
  if (this->frame > 8) {
    const int last = static_cast<int>(particle->action->FrameCount(0)) - 1;
    this->spriteFrame = std::min(this->spriteFrame + 1, last);
  }

  reinterpret_cast<float&>(particle->angleBits) =
      this->heading - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
  particle->position.x = time * this->velocityX + this->startX + particle->velocity.x;
  particle->position.y = this->velocityY * time + this->startY + particle->velocity.y;
  particle->position.z = this->velocityZ * time + this->startZ + particle->velocity.z;
  if (time > 8.0f / this->travelSpeedScale) {
    this->alive = false;
  }
}

// OFFSET: 0x0070CB80
// FUNCTION: fn_07139
void CSprArrowEffect::Render() {
  if (this->spriteParticle != 0 && this->launched) {
    AsSprite(reinterpret_cast<EZ2STREmitter*>(this->spriteParticle))->Draw(1);
  }
}

// OFFSET: 0x0070CBA0
// FUNCTION: fn_07140
void CSprArrowEffect::SetBulletSprite(int job) {
  CSpriteParticle* particle = reinterpret_cast<CSpriteParticle*>(this->spriteParticle);
  if (!particle) {
    return;
  }

  const char* name;
  switch (job) {
  case JT_DISGUISER:
  case JT_G_DISGUISER:
  case JT_SPIRIT_H_WATER_S:
  case JT_SPIRIT_H_WATER_L:
  case JT_SPIRIT_H_WATER_SL:
  case JT_SPIRIT_C_WATER_S:
  case JT_SPIRIT_C_WATER_L:
  case JT_SPIRIT_C_WATER_SL:
    name = "canon_bullet";
    break;
  case JT_MD_PRI_RIGEL_AC:
  case JT_MD_PRI_RIGEL_ACS:
    name = "RIGEL_BULLET";
    break;
  case JT_SPIRIT_I_WATER_S:
    name = "phylla_bullet";
    break;
  case JT_SPIRIT_B_FLAME_M:
  case JT_SPIRIT_B_FLAME_L:
  case JT_SPIRIT_B_FLAME_SL:
    name = "mg_corruption_root_h_bullet";
    break;
  case JT_SPIRIT_C_FLAME_L:
  case JT_SPIRIT_C_FLAME_SL:
  case JT_SPIRIT_R_FLAME_L:
  case JT_SPIRIT_R_FLAME_SL:
    name = "mg_corruption_root_bullet";
    break;
  case JT_VR_MD_DREGG1:
    name = "VR_MD_DREGG1_bullet";
    break;
  case JT_VR_MD_DREGG2:
    name = "VR_MD_DREGG2_bullet";
    break;
  case JT_EP21_YORKER_B:
    name = "EP21_YORKER_Bullet";
    break;
  case JT_EP21_YOSTER_D:
    name = "EP21_YOSTER_D_bullet";
    break;
  case JT_EP21_YOSTER_E:
    name = "EP21_YOSTER_E_bullet";
    break;
  case JT_EP21_YORMI_B:
  case JT_EP21_YORMI_B_H:
    name = "EP21_YORMI_bullet";
    break;
  case JT_EP21_YORDOS_A:
  case JT_G_EP21_YORDOS_A:
  case JT_EP21_YORDOS_A_H:
    name = "EP21_YORDOS_Bullet";
    break;
  case JT_EP21_YOSCOPUS_B:
  case JT_EP21_YOSCOPUS_B_H:
    name = "EP21_YOSCOPUS_Bullet";
    break;
  case JT_VIOLEN:
  case JT_CP_VIOLEN:
    name = "violen_bullet";
    break;
  case JT_CH1_SHADOW_JAILER:
    name = "CH1_SHADOW_JAILER_bullet";
    break;
  case JT_CH1_SCABIOSA:
    name = "CH1_SCABIOSA_bullet";
    break;
  case JT_CH1_LOBELIA:
    name = "CH1_LOBELIA_bullet";
    break;
  default:
    name = "skel_archer_arrow";
    break;
  }

  if (std::strlen(name) == 0) {
    this->alive = false;
    return;
  }

  particle->SetSprite(std::format("몬스터\\{}.spr", name).c_str());
  reinterpret_cast<CSpriteParticle*>(this->spriteParticle)->SetAction(std::format("몬스터\\{}.act", name).c_str());
}

// OFFSET: 0x0070CEA0
// FUNCTION: fn_07141
CSpriteMableEffect::CSpriteMableEffect() : CEffect(1096) {
}

// OFFSET: 0x0070D040
// FUNCTION: fn_07144
void CSpriteMableEffect::Open() {
  for (int i = 0; i < 3; ++i) {
    EZ2STREmitter* emitter =
        reinterpret_cast<EZ2STREmitter*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    if (!emitter) {
      continue;
    }

    CSpriteParticle* particle = AsSprite(emitter);

    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->scale = 0.3f;
    particle->alpha = 200;
    particle->animationParameter = static_cast<float>(i) * 2.0943947f;
    reinterpret_cast<float&>(particle->animationParameters) = 0.0f;
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

    this->effectLifetime = 9999;
    this->emitters.push_back(emitter);
  }
}

// OFFSET: 0x0070D150
// FUNCTION: fn_07145
bool CSpriteMableEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0070D2C0
// FUNCTION: fn_07147
void CSpriteMableEffect::Render() {
  if (!g_serviceOptions.Get(240)) {
    return;
  }

  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->Draw(1);
  }
}

// OFFSET: 0x0070D300
// FUNCTION: fn_07148
CStemSpearEffect::CStemSpearEffect() : CEffect(1100) {
}

// OFFSET: 0x0070D4A0
// FUNCTION: fn_07151
void CStemSpearEffect::Open() {
  for (int i = 0; i < 4; ++i) {
    EZ2STREmitter* emitter =
        reinterpret_cast<EZ2STREmitter*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
    if (!emitter) {
      continue;
    }

    CSpriteParticle* particle = reinterpret_cast<CSpriteParticle*>(emitter);

    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->SetSprite("이팩트\\sv_stemspear\\sv_stemspear.spr");
    particle->SetAction("이팩트\\sv_stemspear\\sv_stemspear.act");

    emitter->velocity.x = 0.0f;
    emitter->velocity.y = 0.0f;
    emitter->velocity.z = 0.0f;
    this->effectLifetime = 16;

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
    const float distance = SquareRoot(dx * dx + dz * dz);
    emitter->speed = distance / static_cast<float>(this->effectLifetime);
    emitter->elapsed = -(this->effectLifetime / 4) * i;
    this->emitters.push_back(emitter);
  }
}

// OFFSET: 0x0070D650
// FUNCTION: fn_07152
void __stdcall Nullsub226(int) {
}

// OFFSET: 0x0070D660
// FUNCTION: fn_07153
bool CStemSpearEffect::IsAlive() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed < this->effectLifetime) {
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
void CStemSpearEffect::OnProcess() {
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
void CStemSpearEffect::Render() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed > 0 && (*emitter)->elapsed < this->effectLifetime) {
      AsSprite(*emitter)->Draw(1);
    }
  }
}

// OFFSET: 0x0070DB30
// FUNCTION: fn_07160
bool CSvgSpiritEffect::IsAlive() {
  return this->emitter->elapsed < this->effectLifetime;
}

// OFFSET: 0x0070DB50
// FUNCTION: fn_07161
void CSvgSpiritEffect::OnProcess() {
  if (this->emitter->elapsed >= 0) {
    this->SetSourceActor(this->sourceActor);
    this->emitter->angle = static_cast<float>(this->emitter->baseAngle) - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;

    const float dx = this->sourcePosition[0] - this->position[0];
    const float dy = this->sourcePosition[1] - this->position[1];
    const float dz = this->sourcePosition[2] - this->position[2];
    const float inverse = 1.0f / SquareRoot(dx * dx + dy * dy + dz * dz);

    EZ2STREmitter* emitter = this->emitter;
    emitter->velocity.x += dx * inverse * emitter->speed;
    emitter->velocity.y += dy * inverse * emitter->speed;
    emitter->velocity.z += dz * inverse * emitter->speed;

    const vector3d position = {this->position[0] + emitter->velocity.x, this->position[1] - 10.0f, this->position[2] + emitter->velocity.z};
    this->emitter->position = position;

    if (this->frame >= 10) {
      this->emitter->alpha -= 40;
    }
  }

  ++this->emitter->elapsed;
}

// OFFSET: 0x0070DCD0
// FUNCTION: fn_07162
void CSvgSpiritEffect::Render() {
  EZ2STREmitter* emitter = this->emitter;
  if (emitter->elapsed > 0 && emitter->elapsed < this->effectLifetime) {
    AsSprite(emitter)->Draw(1);
  }
}

// OFFSET: 0x0070DCF0
// FUNCTION: fn_07163
CTunaPartyEffect::CTunaPartyEffect() : CEffect(1097) {
}

// OFFSET: 0x0070DF30
// FUNCTION: fn_07167
bool CTunaPartyEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0070E090
// FUNCTION: fn_07169
void CTunaPartyEffect::Render() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->Draw(1);
  }
}

// OFFSET: 0x0070E0C0
// FUNCTION: fn_07170
CCurseOfCubeEffect::CCurseOfCubeEffect()
    : CEffect(-1), spawnPhase(0), viewOffsetX(0), viewOffsetY(-10.0f), viewOffsetZ(0) {
}

// OFFSET: 0x0070EB10
// FUNCTION: fn_07176
void CCurseOfCubeEffect::Restart() {
  this->Close();
  this->SpawnShock();
  this->spawnPhase = 3;
}

// OFFSET: 0x0070EB30
// FUNCTION: fn_07177
void CCurseOfCubeEffect::Open() {
  this->retained = 1;
  this->SpawnStart();
  if (!this->emitters.empty()) {
    this->spawnPhase = 1;
  }
}

// OFFSET: 0x0070EB60
// FUNCTION: fn_07178
bool CCurseOfCubeEffect::IsAlive() {
  if (!this->ActorExists() || this->emitters.empty()) {
    return false;
  }

  return true;
}

// OFFSET: 0x0070F080
// FUNCTION: fn_07180
void CCurseOfCubeEffect::Render() {
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
  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
  if (particle) {
    particle->SetTexture("warning_plane.tga");
    particle->effectId = this->id;
    particle->columnIndex = 0;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->scale = 2.5f;
    particle->alpha = 150;
    particle->SetColor(255, 0, 0);
    particle->position = position;
    InitParticleQuad(particle->vertices);
    this->emitters.push_back(emitter);
  }
}

// OFFSET: 0x0070F350
// FUNCTION: fn_07185
void CWarningPlaneEffect::Open() {
}

// OFFSET: 0x0070F360
// FUNCTION: fn_07186
bool CWarningPlaneEffect::IsAlive() {
  if (this->expireTime != 0 && this->expireTime < timeGetTime()) {
    return false;
  }

  return this->ActorExists();
}

// OFFSET: 0x0070FC60
// FUNCTION: fn_07194
bool CCylinderEffect::IsAlive() {
  return this->startTime + this->effectLifetime >= timeGetTime();
}

// OFFSET: 0x0070F720
// FUNCTION: fn_07189
CCylinderEffect::CCylinderEffect() : CEffect(-1) {
}

// OFFSET: 0x0070FB50
// FUNCTION: fn_07193
void CCylinderEffect::Open() {
  this->startTime = timeGetTime();
  for (long long i = 0; i < 21; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = static_cast<int>(i);
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    InitParticleQuad(particle->vertices);
    particle->angleBits = 0;
    if (this->id == 2077) {
      particle->SetTexture("effect\\nipelheim\\nipelheim_cast\\nip_cast.tga");
      particle->scale = 4.5f;
      particle->phaseParameter = 70;
      particle->animationParameter = 25.0f;
      particle->speedParameter = 0.052359872f;
      particle->sourceBlend = 5;
      particle->destinationBlend = 6;
    }
    this->emitters.push_back(emitter);
  }
}

// OFFSET: 0x0070F390
// FUNCTION: fn_07187
void CWarningPlaneEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (this->expireTime != 0 && emitter) {
      CEZ2STRParticle* particle = AsParticle(emitter);

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(1.5707963f);
      rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix transform;
      transform.Compose(scale, rotation);
      particle->transform = transform;

      int alpha = particle->alpha;
      if (this->fadingIn == 1) {
        alpha += 2;
        if (alpha >= 250) {
          this->fadingIn = 0;
        }
      } else {
        alpha -= 2;
        if (alpha <= 50) {
          this->fadingIn = 1;
        }
      }
      particle->alpha = alpha;
    }

    ++emitter->elapsed;
  }
}

// OFFSET: 0x0070F670
// FUNCTION: fn_07188
void CWarningPlaneEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (this->expireTime != 0 && *it) {
      AsParticle(*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x0070FFA0
// FUNCTION: fn_07196
void CCylinderEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (!particle) {
      continue;
    }

    const int texPart = particle->columnIndex;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.left = static_cast<float>(texPart / 21.0);
    quad.right = static_cast<float>((texPart + 1) / 21.0);
    particle->Render(quad);
  }
}

// OFFSET: 0x007100D0
// FUNCTION: fn_07197
CDancingKnifeEffect::CDancingKnifeEffect() : CEffect(1829) {
}

// OFFSET: 0x00710830
// FUNCTION: fn_07203
bool CDancingKnifeEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00710A20
// FUNCTION: fn_07205
void CDancingKnifeEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }

  for (auto it = this->secondStrikeEmitters.begin(); it != this->secondStrikeEmitters.end(); ++it) {
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
void CEternalSlashAttackEffect::Open() {
  EZ2STREmitter* bottom =
      reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (bottom) {
    AsParticle(bottom)->Load(std::string("eternal_slash\\eternal_slash_bottom\\eternal_slash_bottom.str"));
    bottom->offset.x = 0.0f;
    bottom->offset.y = -13.0f;
    bottom->offset.z = 0.0f;
    this->emitters.push_back(bottom);
  }

  EZ2STREmitter* slash =
      reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (slash) {
    AsParticle(slash)->Load(std::string("eternal_slash\\eternal_slash\\eternal_slash.str"));
    slash->offset.x = 0.0f;
    slash->offset.y = -13.0f;
    slash->offset.z = 0.0f;
    this->emitters.push_back(slash);
  }
}

// OFFSET: 0x00710F50
// FUNCTION: fn_07210
bool CEternalSlashAttackEffect::IsAlive() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && IsEmitterAlive(emitter) == true) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00710FB0
// FUNCTION: fn_07211
void CEternalSlashAttackEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    const vector3d base = {this->position[0], this->position[1], this->position[2]};
    emitter->position.x = base.x + emitter->offset.x;
    emitter->position.y = base.y + emitter->offset.y;
    emitter->position.z = base.z + emitter->offset.z;
    AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x007110A0
// FUNCTION: fn_07212
void CEternalSlashAttackEffect::Render() {
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
void CEternalSlashCountEffect::Open() {
  if (static_cast<unsigned int>(this->effectLevel) - 1 > 4) {
    return;
  }

  CEZ2STRParticle* particle =
      static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (!particle) {
    return;
  }

  particle->Load(FormatOwnedString("eternal_slash\\eternal_slash_count\\eternal_slash_count_%02d.str", this->effectLevel));
  particle->offsetX = 0.0f;
  particle->offsetY = -25.0f;
  particle->offsetZ = 0.0f;
  this->emitter = reinterpret_cast<EZ2STREmitter*>(particle);
}

// OFFSET: 0x00711340
// FUNCTION: fn_07217
bool CEternalSlashCountEffect::IsAlive() {
  EZ2STREmitter* emitter = this->emitter;
  if (emitter) {
    if (0.0f == emitter->lifetime) {
      return true;
    }

    return static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f;
  }

  return false;
}

// OFFSET: 0x00711380
// FUNCTION: fn_07218
void CEternalSlashCountEffect::OnProcess() {
  if (!this->emitter) {
    return;
  }

  EZ2STREmitter* emitter = this->emitter;
  const vector3d base = {this->position[0], this->position[1], this->position[2]};
  const float x = emitter->offset.x + base.x;
  const float y = emitter->offset.y + base.y;
  const float z = emitter->offset.z + base.z;
  emitter->position.x = x;
  emitter->position.y = y;
  emitter->position.z = z;
  AsParticle(this->emitter)->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x007113F0
// FUNCTION: fn_07219
void CEternalSlashCountEffect::Render() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00711420
// FUNCTION: fn_07220
CFallenAngelEffect::CFallenAngelEffect() : CEffect(1140), emitter(nullptr), startX(0), startY(0), startZ(0) {
}

// OFFSET: 0x00711580
// FUNCTION: fn_07223
void CFallenAngelEffect::Open() {
  if (!this->emitter) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    this->emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    particle->Open("RL_fallen_angel\\Penne.str");
  } else {
    AsParticle(this->emitter)->Restart();
  }

  InitParticleQuad(AsParticle(this->emitter)->vertices);
  this->startX = this->position[0];
  this->startY = this->position[1];
  this->startZ = this->position[2];
}

// OFFSET: 0x00711690
// FUNCTION: fn_07224
bool CFallenAngelEffect::IsAlive() {
  if (0.0f == this->emitter->lifetime) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007116D0
// FUNCTION: fn_07225
void CFallenAngelEffect::OnProcess() {
  const vector3d start = this->emitter->position;
  const float dx = this->startX - this->position[0];
  const float dz = this->startZ - this->position[2];
  const float speed = 1.0f / SquareRoot(dx * dx + 0.0f + dz * dz);

  this->emitter->position.x = start.x + dx * speed * 0.09f;
  this->emitter->position.y = start.y + speed * 0.0f;
  this->emitter->position.z = start.z + dz * speed * 0.09f;

  AsParticle(this->emitter)->Process(0.0f, -80.0f, 0.0f);
  this->emitter->elapsed++;
}

// OFFSET: 0x007117F0
// FUNCTION: fn_07226
void CFallenAngelEffect::Render() {
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
void CFloralFlareRoad::Open() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  this->emitter = reinterpret_cast<EZ2STREmitter*>(bottom);
  bottom->Load(std::string("froral_flareroad\\froral_flareroad_bottom\\froral_flareroad_bottom.str"));

  for (int delay = 0; delay < 4500; delay += 300) {
    vector3d position = {this->position[0], this->position[1], this->position[2]};
    const int distanceX = ClampedRandom(20);
    const int angleX = ClampedRandom(360);
    position.x = ClientSine(angleX) * static_cast<float>(distanceX) + position.x;
    const int distanceZ = ClampedRandom(20);
    const int angleZ = ClampedRandom(360);
    position.z = ClientCosine(angleZ) * static_cast<float>(distanceZ) + position.z;

    CEZ2STRParticle* flare = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
    EZ2STREmitter* flareEmitter = reinterpret_cast<EZ2STREmitter*>(flare);
    flare->Load(std::string("froral_flareroad\\froral_flareroad\\froral_flareroad.str"));
    flare->m_delay = delay;
    this->emitters.push_back(flareEmitter);
    this->AddEvent(0, "effect\\AG_ALL_BLOOM.wav", delay / 16, 0);
  }
}

// OFFSET: 0x00711DA0
// FUNCTION: fn_07232
bool CFloralFlareRoad::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00711DB0
// FUNCTION: fn_07233
void CFloralFlareRoad::OnProcess() {
  if (this->emitter) {
    AsParticle(this->emitter)->Process(0.0f, -80.0f, 0.0f);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (IsEmitterAlive(emitter) == true) {
      if (*it) {
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
void CFloralFlareRoad::Render() {
  if (this->emitter) {
    AsParticle(this->emitter)
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
CFootprintEffect::CFootprintEffect() : CEffect(2411), stepDistance(50.0), leftFoot(1), lastPosition{}, lastGroundHeight(-1000.0f) {
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
}

// OFFSET: 0x007123C0
// FUNCTION: fn_07238
bool CFootprintEffect::IsVisible() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.isCharacter == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x00712410
// FUNCTION: fn_07239
void CFootprintEffect::Open() {
  char* leftFile = nullptr;
  char* rightFile = nullptr;
  g_session.m_hotkeyScript->Call("GetFootprintPngFile", "d>ss", this->instanceId, &leftFile, &rightFile);
  if (!leftFile || std::strlen(leftFile) == 0 || !rightFile || std::strlen(rightFile) == 0) {
    return;
  }

  this->leftTexture = leftFile;
  this->rightTexture = rightFile;

  double scale = 5.0;
  g_session.m_hotkeyScript->Call("GetFootprintPngScale", "d>l", this->instanceId, &scale);
  *reinterpret_cast<float*>(&this->scaleParameter) = static_cast<float>(scale);

  int alpha = 250;
  g_session.m_hotkeyScript->Call("GetFootprintPngAplha", "d>d", this->instanceId, &alpha);
  *reinterpret_cast<int*>(this->alphaParameters) = alpha;

  int duration = 10;
  g_session.m_hotkeyScript->Call("GetFootprintPngDuration", "d>d", this->instanceId, &duration);
  this->effectLifetime = duration;

  g_session.m_hotkeyScript->Call("GetFootprintStride", "d>l", this->instanceId, &this->stepDistance);
  this->lastGroundHeight = 0.0f;
}

// OFFSET: 0x007127A0
// FUNCTION: fn_07240
bool CFootprintEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x007127B0
// FUNCTION: fn_07241
CParticle* CFootprintEffect::SpawnFootprint() {
  const std::string& texture = this->leftFoot ? this->leftTexture : this->rightTexture;
  if (texture.size() == 0) {
    return nullptr;
  }

  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
  if (!particle) {
    return particle;
  }

  particle->alpha = *reinterpret_cast<int*>(this->alphaParameters);
  particle->life = static_cast<float>(this->effectLifetime);
  particle->sourceBlend = 5;
  particle->destinationBlend = 6;
  particle->speedParameter = static_cast<float>(*reinterpret_cast<int*>(this->alphaParameters)) / static_cast<float>(this->effectLifetime * 60);
  particle->SetTexture(texture);
  InitParticleQuad(particle->vertices);

  const float angle =
    ScreenAngle(&this->lastPosition.x, this->position, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values) + 90.0f;
  const float scale = *reinterpret_cast<float*>(&this->scaleParameter);
  matrix world;
  world.SetScale(scale, scale, 0.0f);
  world.AppendXRotation(1.5707964f);
  world.AppendYRotation((angle + static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw) / 180.0f * 3.141592f);
  world.AppendTranslate(this->position);

  for (int i = 0; i < 4; ++i) {
    RenderVertex& vertex = particle->vertices[i];
    const float x = vertex.x * world.values[0] + vertex.y * world.values[3] + 0.0f * world.values[6] + world.values[9];
    const float z = vertex.x * world.values[2] + vertex.y * world.values[5] + 0.0f * world.values[8] + world.values[11];
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    float y = mode->scene->terrain->PlaneHeight(x, z) - 2.0f;
    if (this->lastGroundHeight != -1000.0f && y - this->lastGroundHeight > 5.0f) {
      y = this->lastGroundHeight;
    }
    this->lastGroundHeight = y;
    vertex.x = x;
    vertex.y = y;
    vertex.z = z;
  }

  this->leftFoot = this->leftFoot == 0;
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
  return particle;
}

// OFFSET: 0x00712A90
// FUNCTION: fn_07242
void CFootprintEffect::OnProcess() {
  const float dx = this->position[0] - this->lastPosition.x;
  const float dy = this->position[1] - this->lastPosition.y;
  const float dz = this->position[2] - this->lastPosition.z;
  if (static_cast<double>(dx * dx + dy * dy + dz * dz) > this->stepDistance) {
    CParticle* particle = this->SpawnFootprint();
    if (particle) {
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    CParticle* particle = AsParticle(emitter);
    if (IsEmitterAlive(emitter)) {
      particle->alpha = static_cast<int>(std::max(static_cast<float>(particle->alpha) - particle->speedParameter, 0.0f));
      ++particle->elapsedFrames;
      ++it;
    } else {
      FreeParticle(particle);
      it = this->emitters.erase(it);
    }
  }
}

// OFFSET: 0x00712C50
// FUNCTION: fn_07244
void CFootprintEffect::Render() {
  for (EZ2STREmitter* emitter : this->emitters) {
    AsParticle(emitter)->Render(true, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00712CA0
// FUNCTION: fn_07245
CFootprintStrEffect::CFootprintStrEffect()
    : CEffect(2412), bottomScale(1.0), topScale(1.0), topHeight(0.0), stepDistance(50.0), footGap(2.0),
      adjustAngle(0), leftFoot(1), lastPosition{} {
}

// OFFSET: 0x007130E0
// FUNCTION: fn_07247
void CFootprintStrEffect::Close() {
  for (EZ2STREmitter* emitter : this->emitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->emitters.clear();

  for (EZ2STREmitter* emitter : this->topEmitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->topEmitters.clear();
}

// OFFSET: 0x00713370
// FUNCTION: fn_07248
bool CFootprintStrEffect::IsVisible() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.isCharacter == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x007133C0
// FUNCTION: fn_07249
void CFootprintStrEffect::Open() {
  char* leftFile = nullptr;
  char* rightFile = nullptr;
  g_session.m_hotkeyScript->Call("GetFootprintStrFileBottom", "d>ss", this->instanceId, &leftFile, &rightFile);
  if (!IsNullOrEmpty(leftFile) && !IsNullOrEmpty(rightFile)) {
    this->leftBottomFile = leftFile;
    this->rightBottomFile = rightFile;
  }

  g_session.m_hotkeyScript->Call("GetFootprintStrFileTop", "d>ss", this->instanceId, &leftFile, &rightFile);
  if (!IsNullOrEmpty(leftFile) && !IsNullOrEmpty(rightFile)) {
    this->leftTopFile = leftFile;
    this->rightTopFile = rightFile;
  }

  if (this->leftBottomFile.size() == 0 && this->rightBottomFile.size() == 0 && this->leftTopFile.size() == 0 &&
      this->rightTopFile.size() == 0) {
    return;
  }

  g_session.m_hotkeyScript->Call("GetFootprintStrScale", "d>ll", this->instanceId, &this->bottomScale, &this->topScale);
  g_session.m_hotkeyScript->Call("GetFootprintStrTopHeight", "d>l", this->instanceId, &this->topHeight);
  g_session.m_hotkeyScript->Call("GetFootprintStride", "d>l", this->instanceId, &this->stepDistance);
  g_session.m_hotkeyScript->Call("GetFootprintStrGap", "d>l", this->instanceId, &this->footGap);
  g_session.m_hotkeyScript->Call("IsFootprintStrAdjustAngle", "d>b", this->instanceId, &this->adjustAngle);
}

// OFFSET: 0x00713830
// FUNCTION: fn_07250
bool CFootprintStrEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00713960
// FUNCTION: fn_07251
void CFootprintStrEffect::SpawnFootprint() {
  vector3d direction;
  direction.x = this->position[0] - this->lastPosition.x;
  direction.y = 0.0f;
  direction.z = this->position[2] - this->lastPosition.z;
  const float inverse = 1.0f / SquareRoot(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
  direction.x = inverse * direction.x;
  direction.y = inverse * direction.y;
  direction.z = inverse * direction.z;

  matrix rotation;
  rotation.MakeYRotation(1.5707964f);
  const float side = static_cast<float>((this->leftFoot != 0) * 2 - 1);
  const float gap = static_cast<float>(this->footGap);
  vector3d point;
  point.x =
    (direction.x * rotation.values[0] + direction.y * rotation.values[3] + direction.z * rotation.values[6] + rotation.values[9]) * gap * side +
    this->position[0];
  point.z =
    (direction.x * rotation.values[2] + direction.y * rotation.values[5] + direction.z * rotation.values[8] + rotation.values[11]) * gap * side +
    this->position[2];
  point.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(point.x, point.z);
  vector3d spawn = point;

  const std::string& bottom = this->leftFoot ? this->leftBottomFile : this->rightBottomFile;
  if (bottom.size() != 0) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&spawn, 5));
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    if (particle) {
      float angle;
      if (this->adjustAngle) {
        angle = ScreenAngle(&this->lastPosition.x, &spawn.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values) + 90.0f;
      } else {
        angle = 0.0f;
      }
      particle->Open(bottom);
      particle->m_scale = static_cast<float>(this->bottomScale);
      particle->m_angle = angle;
      this->emitters.push_back(emitter);
    }
  }

  const std::string& top = this->leftFoot ? this->leftTopFile : this->rightTopFile;
  if (top.size() != 0) {
    spawn.x = point.x + 0.0f;
    spawn.y = point.y + static_cast<float>(-this->topHeight);
    spawn.z = point.z + 0.0f;
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&spawn, 5));
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    if (particle) {
      particle->Open(top);
      particle->depthOffset = 0.000035f;
      particle->m_scale = static_cast<float>(this->topScale);
      this->topEmitters.push_back(emitter);
    }
  }

  this->leftFoot = this->leftFoot == 0;
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
}

// OFFSET: 0x00713E00
// FUNCTION: fn_07252
void CFootprintStrEffect::OnProcess() {
  const float dx = this->position[0] - this->lastPosition.x;
  const float dy = this->position[1] - this->lastPosition.y;
  const float dz = this->position[2] - this->lastPosition.z;
  if (static_cast<double>(dx * dx + dy * dy + dz * dz) > this->stepDistance) {
    this->SpawnFootprint();
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->Process(0.0f, 0.0f, 0.0f)) {
      ++it;
    } else {
      FreeParticle(particle);
      it = this->emitters.erase(it);
    }
  }

  for (auto it = this->topEmitters.begin(); it != this->topEmitters.end();) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->Process(0.0f, 0.0f, 0.0f)) {
      ++it;
    } else {
      FreeParticle(particle);
      it = this->topEmitters.erase(it);
    }
  }
}

// OFFSET: 0x00713F70
// FUNCTION: fn_07253
void CFootprintStrEffect::Render() {
  for (EZ2STREmitter* emitter : this->emitters) {
    AsParticle(emitter)->DrawGround(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (EZ2STREmitter* emitter : this->topEmitters) {
    AsParticle(emitter)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00714250
// FUNCTION: fn_07259
CFromTheAbyssEffect::CFromTheAbyssEffect() : CEffect(-1), maxLevel(5), level(0) {
  this->waves[0] = AbyssWave(0.8f, vector3d{-4.8f, -9.1f, 0.0f});
  this->waves[1] = AbyssWave(0.6f, vector3d{-3.4f, -12.8f, 0.0f});
  this->waves[2] = AbyssWave(0.5f, vector3d{-0.7f, -14.0f, 0.0f});
  this->waves[3] = AbyssWave(0.7f, vector3d{2.7f, -14.0f, 0.0f});
  this->waves[4] = AbyssWave(0.4f, vector3d{4.3f, -10.0f, 0.0f});
}

// OFFSET: 0x00715160
// FUNCTION: fn_07265
void CFromTheAbyssEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (it->second.emitter) {
      FreeParticle(AsParticle(it->second.emitter));
    }
  }
  this->particles.clear();
}

// OFFSET: 0x00715400
// FUNCTION: fn_07266
void CFromTheAbyssEffect::Open() {
  this->retained = 1;
}

// OFFSET: 0x00715410
// FUNCTION: fn_07267
bool CFromTheAbyssEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00715420
// FUNCTION: fn_07268
void CFromTheAbyssEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->UpdateParticle(it->first, it->second);
  }
}

// INLINED
static __forceinline void FollowAbyssOwner(const CFromTheAbyssEffect* effect, EZ2STREmitter* emitter) {
  const vector3d origin = *reinterpret_cast<const vector3d*>(effect->position);
  const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const vector3d viewPosition = {origin.x * view.values[0] + origin.y * view.values[3] + origin.z * view.values[6] + view.values[9],
                                 origin.x * view.values[1] + origin.y * view.values[4] + origin.z * view.values[7] + view.values[10],
                                 origin.x * view.values[2] + origin.y * view.values[5] + origin.z * view.values[8] + view.values[11]};
  const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
  const float viewX = viewPosition.x + emitter->offset.x;
  const float viewY = viewPosition.y + emitter->offset.y;
  const float viewZ = viewPosition.z + emitter->offset.z;
  const float worldX = viewX * inverse.values[0] + viewY * inverse.values[3] + viewZ * inverse.values[6] + inverse.values[9];
  const float worldY = viewX * inverse.values[1] + viewY * inverse.values[4] + viewZ * inverse.values[7] + inverse.values[10];
  const float worldZ = viewX * inverse.values[2] + viewY * inverse.values[5] + viewZ * inverse.values[8] + inverse.values[11];
  emitter->position.x = worldX;
  emitter->position.y = worldY;
  emitter->position.z = worldZ;
}

// OFFSET: 0x007154A0
// FUNCTION: fn_07269
void CFromTheAbyssEffect::UpdateParticle(long long key, AbyssParticle particle) {
  EZ2STREmitter* emitter = particle.emitter;
  if (!emitter) {
    return;
  }

  switch (particle.kind) {
  case 3:
    if (IsEmitterAlive(emitter) != true) {
      FreeParticle(AsParticle(emitter));
      AbyssParticle& entry = this->particles[key];
      entry.kind = 0;
      entry.emitter = nullptr;
      return;
    }
    FollowAbyssOwner(this, emitter);
    break;
  case 2:
    FollowAbyssOwner(this, emitter);
    break;
  case 1:
    if (IsEmitterAlive(emitter) != true) {
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

      AbyssParticle& entry = this->particles[key];
      entry.kind = 2;
      entry.emitter = dying;
      return;
    }
    FollowAbyssOwner(this, emitter);
    break;
  default:
    return;
  }

  AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00715C80
// FUNCTION: fn_07270
void CFromTheAbyssEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
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
      auto it = this->particles.find(i);
      if (it != this->particles.end() && it->second.emitter) {
        FreeParticle(AsParticle(it->second.emitter));
      }

      EZ2STREmitter* emitter =
          reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
      if (emitter) {
        AsParticle(emitter)->Load("from_the_abyss\\from_the_abyss_shadowball_create\\from_the_abyss_shadowball_create.str");
        AsParticle(emitter)->m_scale = this->waves[i].scale;
        emitter->offset = this->waves[i].offset;
        AbyssParticle& entry = this->particles[i];
        entry.emitter = emitter;
        entry.kind = 1;
      }
    }
  } else if (level < this->level) {
    for (long long i = this->level - 1; i > level - 1; --i) {
      auto it = this->particles.find(i);
      if (it != this->particles.end() && it->second.emitter) {
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

        AbyssParticle& entry = this->particles[i];
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
void CFullScreenEffect::Open() {
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

      particle->effectId = this->id;
      particle->columnIndex = static_cast<int>(i);
      particle->rowIndex = 0;
      particle->groupIndex = 1;
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
bool CFullScreenEffect::IsAlive() {
  if (this->loaded == 1 && this->ActorExists()) {
    return true;
  }

  return false;
}

// OFFSET: 0x00716BF0
// FUNCTION: fn_07279
void CFullScreenEffect::Render() {
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
    switch (particle->columnIndex) {
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

    primitive.sourceBlend = particle->sourceBlend;
    primitive.destinationBlend = particle->destinationBlend;
    g_renderer->queues.Submit(&primitive, static_cast<std::uint16_t>(particle->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}

// OFFSET: 0x007176E0
// FUNCTION: fn_07280
CGaleStorm::CGaleStorm() : CEffect(1973), countLow(5), countHigh(0) {
}

// OFFSET: 0x00717DE0
// FUNCTION: fn_07285
void CGaleStorm::Open() {
  long long table[10] = {5, 5, 5, 5, 7, 7, 7, 9, 9, 11};

  unsigned int index = this->effectLevel;
  if (index > 10) {
    index = 10;
  }

  this->count = table[index - 1];
}

// OFFSET: 0x00717EB0
// FUNCTION: fn_07286
bool CGaleStorm::IsAlive() {
  if (this->frame > this->effectLifetime && this->emitters.empty()) {
    return false;
  }

  return this->ActorExists();
}

// OFFSET: 0x00718040
// FUNCTION: fn_07288
void CGaleStorm::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x007183F0
// FUNCTION: fn_07292
void CGravityControlEffect::Render() {
  if (!this->target) {
    return;
  }

  AsParticle(reinterpret_cast<EZ2STREmitter*>(this->target))
      ->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x007184D0
// FUNCTION: fn_07294
CHawkVumerangEffect::CHawkVumerangEffect()
    : CEffect(1968),
      countLow(10),
      countHigh(0),
      angularRange(6.283184051513672),
      orbitOffsetX(0),
      orbitOffsetY(-17.0f),
      orbitOffsetZ(0),
      targetPosition{},
      originPosition{},
      orbitCenter{} {
}

// OFFSET: 0x00718620
// FUNCTION: fn_07295
CHawkVumerangEffect::~CHawkVumerangEffect() {
}

// OFFSET: 0x007187F0
// FUNCTION: fn_07297
void CHawkVumerangEffect::Close() {
  for (auto it = this->finishedEmitters.begin(); it != this->finishedEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->finishedEmitters.clear();
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00718B90
// FUNCTION: fn_07299
bool CHawkVumerangEffect::IsAlive() {
  return this->emitters.size() != 0;
}

// OFFSET: 0x00718BA0
// FUNCTION: fn_07300
void CHawkVumerangEffect::OnProcess() {
  if (this->emitters.size() == 0) {
    return;
  }

  EZ2STREmitter* emitter = this->emitters.back();
  this->emitters.pop_back();

  const DWORD elapsed = timeGetTime() - this->startTime;
  double angle = static_cast<double>(elapsed) / static_cast<double>(this->effectLifetime * 2) * this->angularRange;
  if (angle < this->angularRange) {
    angle += this->startAngle;
    if (angle > 6.283184f) {
      angle -= 6.283184f;
    }

    AsParticle(emitter)->m_angle = 360.0f - static_cast<float>(angle) / 3.141592f * 180.0f;
    const float sine = static_cast<float>(std::sin(static_cast<float>(angle)) * this->orbitRadius);
    const float cosine = static_cast<float>(std::cos(static_cast<float>(angle)) * this->orbitRadius);
    emitter->position.z = this->orbitCenter.z + sine + this->orbitOffsetZ;
    emitter->position.x = cosine + this->orbitCenter.x + this->orbitOffsetX;
    emitter->position.y = this->orbitCenter.y + 0.0f + this->orbitOffsetY;
    this->emitters.emplace_front(emitter);
  } else {
    this->finishedEmitters.push_back(emitter);
  }

  long long index = this->count - this->emitters.size();
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it, ++index) {
    EZ2STREmitter* current = *it;
    if (current) {
      const double alpha = 1.0f - 1.0f / static_cast<float>(this->count) * static_cast<float>(index);
      AsParticle(current)->m_scale = static_cast<float>(alpha);
      AsParticle(current)->m_alpha = static_cast<float>(alpha);
      AsParticle(current)->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x00718E70
// FUNCTION: fn_07301
void CHawkVumerangEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter) {
      AsParticle(emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00719030
// FUNCTION: fn_07303
CDropItemEffect::CDropItemEffect()
    : CEffect(-1), emitterCount(2), soundPath("wav\\effect\\"), position{} {
  this->emitters.reserve(this->emitterCount);
}

// OFFSET: 0x00719400
// FUNCTION: fn_07307
void CDropItemEffect::Open() {
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
  this->InitEvents();
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
// FUNCTION: fn_07308
void CDropItemEffect::InitEvents() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      this->emitters[i]->looping = 1;
      AsParticle(this->emitters[i])->flags = 13;
    }
  }

  this->AddEvent(0, (this->soundPath + this->soundName).c_str(), 0, 0);
}

// OFFSET: 0x00719660
// FUNCTION: fn_07309
bool CDropItemEffect::IsAlive() {
  if (this->actor == 0) {
    return false;
  }

  if (!static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetItemByAID(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00719690
// FUNCTION: fn_07310
void CDropItemEffect::OpenEmitters() {
  switch (this->id) {
  case EF_DROPITEM_PURPLE:
    AsParticle(this->emitters[0])->Open("new_dropitem\\dropitem_purple\\dropitem_purple\\dropitem_purple.str");
    AsParticle(this->emitters[1])->Open("new_dropitem\\dropitem_purple\\dropitem_purple_bottom\\dropitem_purple_bottom.str");
    this->soundName = "drop_purple.wav";
    break;
  case EF_DROPITEM_YELLOW:
    AsParticle(this->emitters[0])->Open("new_dropitem\\dropitem_yellow\\dropitem_yellow\\dropitem_yellow.str");
    AsParticle(this->emitters[1])->Open("new_dropitem\\dropitem_yellow\\dropitem_yellow_bottom\\dropitem_yellow_bottom.str");
    this->soundName = "drop_yellow.wav";
    break;
  case EF_DROPITEM_BLUE:
    AsParticle(this->emitters[0])->Open("new_dropitem\\dropitem_blue\\dropitem_blue\\dropitem_blue.str");
    AsParticle(this->emitters[1])->Open("new_dropitem\\dropitem_blue\\dropitem_blue_bottom\\dropitem_blue_bottom.str");
    this->soundName = "drop_blue.wav";
    break;
  case EF_DROPITEM_GREEN:
    AsParticle(this->emitters[0])->Open("new_dropitem\\dropitem_green\\dropitem_green\\dropitem_green.str");
    AsParticle(this->emitters[1])->Open("new_dropitem\\dropitem_green\\dropitem_green_bottom\\dropitem_green_bottom.str");
    this->soundName = "drop_green.wav";
    break;
  case EF_DROPITEM_RED:
    AsParticle(this->emitters[0])->Open("new_dropitem\\dropitem_red\\dropitem_red\\dropitem_red.str");
    AsParticle(this->emitters[1])->Open("new_dropitem\\dropitem_red\\dropitem_red_bottom\\dropitem_red_bottom.str");
    this->soundName = "drop_red.wav";
    break;
  default:
    AsParticle(this->emitters[0])->Open("new_dropitem\\dropitem_pink\\dropitem_pink\\dropitem_pink.str");
    AsParticle(this->emitters[1])->Open("new_dropitem\\dropitem_pink\\dropitem_pink_bottom\\dropitem_pink_bottom.str");
    this->soundName = "drop_pink.wav";
    break;
  }
}

// OFFSET: 0x00719C30
// FUNCTION: fn_07311
void CDropItemEffect::OnProcess() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      AsParticle(this->emitters[i])->Process(0.0f, 0.0f, 0.0f);
      this->emitters[i]->elapsed++;
    }
  }
}

// OFFSET: 0x00719CA0
// FUNCTION: fn_07312
void CDropItemEffect::Render() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      AsParticle(this->emitters[i])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0071B8C0
// FUNCTION: fn_07318
CJupitelThunderHit::CJupitelThunderHit() : CEffect(-1) {
  this->effectLifetime = 250;
}

// OFFSET: 0x0071BAB0
// FUNCTION: fn_07320
void CJupitelThunderHit::SpawnHit() {
  switch (this->id) {
  case EF_YUFITELHIT:
  case EF_THUNDER_PLAZMA_BLAST:
    this->textures.push_back("effect\\thunder_plazma_blast_a.bmp");
    this->textures.push_back("effect\\thunder_plazma_blast_b.bmp");
    this->textures.push_back("effect\\thunder_ball_d.bmp");
    this->textures.push_back("effect\\thunder_ball_e.bmp");
    this->textures.push_back("effect\\thunder_ball_f.bmp");
    break;
  case EF_YUFITEL2:
    this->textures.push_back("effect\\twirl_soft.bmp");
    this->textures.push_back("effect\\thunder_ball_b.bmp");
    this->textures.push_back("effect\\twirl_soft.bmp");
    this->textures.push_back("effect\\thunder_ball_c.bmp");
    this->textures.push_back("effect\\twirl_soft.bmp");
    break;
  }

  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x203);
  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
  particle->elapsedFrames = -10;
  particle->scale = 7.5f;
  particle->animationState = 0;
  particle->SetTexture("effect\\thunder_plazma_blast_a.bmp");
  this->hitEmitters.push_back(emitter);
}

// OFFSET: 0x0071C270
// FUNCTION: fn_07321
void CJupitelThunderHit::SpawnSpark() {
  std::string texture = "effect\\thunder_pang.bmp";
  switch (this->id) {
  case EF_YUFITELHIT:
    texture = "effect\\thunder_pang.bmp";
    break;
  case EF_YUFITEL2:
    texture = "effect\\pokjuk_d.bmp";
    break;
  }

  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x203);
  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
  particle->SetTexture(texture);
  particle->speedParameter = 2.5f;
  this->emitters.push_back(emitter);
}

// OFFSET: 0x0071C520
// FUNCTION: fn_07323
void CJupitelThunderHit::Open() {
  if (this->id != 1200) {
    this->SpawnSpark();
  }

  this->SpawnHit();
}

// OFFSET: 0x0071C540
// FUNCTION: fn_07324
bool CJupitelThunderHit::IsAlive() {
  if (this->id != 1200) {
    if (this->actor == 0) {
      return true;
    }

    return !this->IsFinished();
  }

  return this->ActorExists();
}

// OFFSET: 0x0071C570
// FUNCTION: fn_07325
void CJupitelThunderHit::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    ++(*it)->elapsed;
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle && particle->elapsedFrames >= 0) {
      if (particle->elapsedFrames == 10) {
        particle->elapsedFrames = -10;
      } else {
        const float scale = static_cast<float>(particle->elapsedFrames) * particle->speedParameter;
        matrix rotation;
        rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
        matrix scaling;
        scaling.SetScale(scale, scale, 1.0f);
        matrix transform;
        transform.Compose(scaling, rotation);
        particle->transform = transform;
        particle->scale = scale;
        particle->position.x = this->position[0] + 0.0f;
        particle->position.y = this->position[1] - 5.0f;
        particle->position.z = this->position[2] + 0.0f;
      }
    }
  }

  for (auto it = this->hitEmitters.begin(); it != this->hitEmitters.end(); ++it) {
    ++(*it)->elapsed;
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle && particle->elapsedFrames >= 0) {
      std::size_t index = static_cast<signed char>(particle->animationState) + 1;
      if (index >= this->textures.size()) {
        index = 0;
      }

      particle->SetTexture(this->textures[index]);
      particle->animationState = static_cast<unsigned char>(index);
      matrix rotation;
      rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
      matrix scaling;
      scaling.SetScale(particle->scale, particle->scale, 1.0f);
      matrix transform;
      transform.Compose(scaling, rotation);
      particle->transform = transform;
      particle->position.x = this->position[0] + 0.0f;
      particle->position.y = this->position[1] - 5.0f;
      particle->position.z = this->position[2] + 0.0f;
    }
  }
}

// OFFSET: 0x0071CB70
// FUNCTION: fn_07326
void CJupitelThunderHit::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && emitter->elapsed >= 0) {
      AsParticle(emitter)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->hitEmitters.begin(); it != this->hitEmitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && emitter->elapsed >= 0) {
      AsParticle(emitter)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x0071CCB0
// FUNCTION: fn_07327
CNewCloudKillEffect::CNewCloudKillEffect() : CEffect(1196), circleResetTime(0), cloudResetTime(0) {
}

// OFFSET: 0x0071CE00
// FUNCTION: fn_07328
CNewCloudKillEffect::~CNewCloudKillEffect() {
}

// OFFSET: 0x0071D460
// FUNCTION: fn_07333
void CNewCloudKillEffect::Close() {
  for (auto it = this->circleEmitters.begin(); it != this->circleEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->circleEmitters.clear();
  for (auto it = this->cloudEmitters.begin(); it != this->cloudEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->cloudEmitters.clear();
  for (auto it = this->smokeEmitters.begin(); it != this->smokeEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->smokeEmitters.clear();
  for (auto it = this->lightningEmitters.begin(); it != this->lightningEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->lightningEmitters.clear();
  for (auto it = this->mistEmitters.begin(); it != this->mistEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->mistEmitters.clear();
}

// OFFSET: 0x0071DEC0
// FUNCTION: fn_07340
bool CNewCloudKillEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0071E080
// FUNCTION: fn_07341
void CNewCloudKillEffect::OnProcess() {
  const unsigned int now = timeGetTime();

  for (auto it = this->circleEmitters.begin(); it != this->circleEmitters.end(); ++it) {
    if (now - static_cast<unsigned int>(this->circleResetTime) > 5000) {
      CEZ2STRParticle* particle = AsParticle(*it);
      particle->alpha = 0xFF;
      *reinterpret_cast<float*>(&particle->angleBits) = 0.0f;
      particle->elapsedFrames = 60;
      particle->animationState = 0;
      InitParticleQuad(particle->vertices);
    }

    if (CEZ2STRParticle* particle = AsParticle(*it)) {
      if (particle->elapsedFrames == 0) {
        particle->animationState = 1;
      } else {
        int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->angleBits) + 1.0f);
        angle %= 360;
        particle->alpha = static_cast<int>(static_cast<float>(particle->elapsedFrames) * 4.25f);
        *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(angle);

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

    --AsParticle(*it)->elapsedFrames;
  }

  if (now - static_cast<unsigned int>(this->circleResetTime) > 5000) {
    this->circleResetTime = static_cast<int>(timeGetTime());
  }

  for (auto it = this->cloudEmitters.begin(); it != this->cloudEmitters.end(); ++it) {
    if (now - static_cast<unsigned int>(this->cloudResetTime) > 5000) {
      ResetCloudParticle(AsParticle(*it));
    }

    if (CEZ2STRParticle* particle = AsParticle(*it)) {
      int alpha = std::max(particle->alpha - 3, 0);
      particle->alpha = alpha;
      if (alpha == 0) {
        particle->animationState = 1;
      } else {
        const float fall = particle->speedParameter * 1.0f;
        particle->scale -= particle->scale / static_cast<float>(alpha);
        particle->offsetY -= fall;
        particle->offsetX += 0.0f;
        particle->offsetZ += 0.0f;

        particle->position.x = particle->offsetX + this->position[0];
        particle->position.y = particle->offsetY + this->position[1];
        particle->position.z = particle->offsetZ + this->position[2];

        int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->angleBits) + 1.0f);
        angle %= 360;
        *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(angle);

        matrix rotation;
        rotation.MakeZRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
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

  if (now - static_cast<unsigned int>(this->cloudResetTime) > 5000) {
    this->cloudResetTime = static_cast<int>(timeGetTime());
  }

  for (auto it = this->smokeEmitters.begin(); it != this->smokeEmitters.end(); ++it) {
    ++AsParticle(*it)->elapsedFrames;
    CEZ2STRParticle* particle = AsParticle(*it);
    if (AsParticle(*it)->elapsedFrames <= 0 || particle == nullptr) {
      continue;
    }

    const int maximum = *reinterpret_cast<int*>(particle->effectParameters);
    if (particle->animationState == 0) {
      int alpha = std::min(particle->alpha + 2, maximum);
      particle->alpha = alpha;
      if (alpha == maximum) {
        particle->animationState = 1;
      }
    } else if (particle->animationState == 1) {
      int alpha = std::max(particle->alpha - 1, 0);
      particle->alpha = alpha;
      if (alpha == 0) {
        ResetSmokeParticle(particle);
        continue;
      }
      particle->scale -= particle->scale / static_cast<float>(alpha);
    }

    const float fall = particle->speedParameter * 1.0f;
    particle->offsetY -= fall;
    particle->offsetX += 0.0f;
    particle->offsetZ += 0.0f;

    particle->position.x = particle->offsetX + this->position[0];
    particle->position.y = particle->offsetY + this->position[1];
    particle->position.z = particle->offsetZ + this->position[2];

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

  for (auto it = this->lightningEmitters.begin(); it != this->lightningEmitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->elapsedFrames >= particle->phaseParameter) {
      ResetLightning(particle);
    } else {
      int state = std::max(static_cast<signed char>(particle->animationState) - 1, 0);
      particle->animationState = static_cast<unsigned char>(state);
      particle->alpha = state * 25;

      matrix scale;
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix rotation;
      rotation.MakeZRotation(*reinterpret_cast<float*>(&particle->angleBits) / 180.0f * 3.141592f);
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

      particle->position.x = yaw.values[0] * particle->offsetX + yaw.values[3] * particle->offsetY +
                             yaw.values[6] * particle->offsetZ + yaw.values[9] + this->position[0];
      particle->position.y = yaw.values[1] * particle->offsetX + yaw.values[4] * particle->offsetY +
                             yaw.values[7] * particle->offsetZ + yaw.values[10] + this->position[1];
      particle->position.z = yaw.values[2] * particle->offsetX + yaw.values[5] * particle->offsetY +
                             yaw.values[8] * particle->offsetZ + yaw.values[11] + this->position[2];
    }

    ++AsParticle(*it)->elapsedFrames;
  }

  for (auto it = this->mistEmitters.begin(); it != this->mistEmitters.end(); ++it) {
    ++AsParticle(*it)->elapsedFrames;
    CEZ2STRParticle* particle = AsParticle(*it);
    if (AsParticle(*it)->elapsedFrames <= 0 || particle == nullptr) {
      continue;
    }

    const int maximum = *reinterpret_cast<int*>(particle->effectParameters);
    if (particle->animationState == 0) {
      int alpha = std::min(particle->alpha + 5, maximum);
      particle->alpha = alpha;
      if (alpha == maximum) {
        particle->animationState = 1;
      }
    } else if (particle->animationState == 1) {
      int alpha = std::max(particle->alpha - 2, 0);
      particle->alpha = alpha;
      if (alpha == 0) {
        ResetMistParticle(particle);
        continue;
      }
      particle->scale = (particle->scale / static_cast<float>(maximum)) * static_cast<float>(alpha);
    }

    const float fall = particle->speedParameter * 1.0f;
    particle->offsetY -= fall;
    particle->offsetX += 0.0f;
    particle->offsetZ += 0.0f;

    particle->position.x = particle->offsetX + this->position[0];
    particle->position.y = particle->offsetY + this->position[1];
    particle->position.z = particle->offsetZ + this->position[2];

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
void CPetitioEffect::Open() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  EZ2STREmitter* bottomEmitter = reinterpret_cast<EZ2STREmitter*>(bottom);
  bottom->Load(std::string("petitio\\petitio_bottom\\petitio_bottom.str"));
  this->emitters.push_back(bottomEmitter);

  CEZ2STRParticle* petitio = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  EZ2STREmitter* petitioEmitter = reinterpret_cast<EZ2STREmitter*>(petitio);
  petitio->Load(std::string("petitio\\petitio\\petitio.str"));
  this->emitters.push_back(petitioEmitter);

  this->AddEvent(1, nullptr, 6, 0);
  this->AddEvent(0, "effect\\CD_PETITIO.wav", 0, 0);
}

// OFFSET: 0x0071FA30
// FUNCTION: fn_07348
bool CPetitioEffect::IsAlive() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (IsEmitterAlive(emitter) == true) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0071FA90
// FUNCTION: fn_07349
void CPetitioEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    const float x = this->position[0] + 0.0f;
    const float y = this->position[1] - 12.0f;
    const float z = this->position[2] + 0.0f;
    particle->position.x = x;
    particle->position.y = y;
    particle->position.z = z;
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0071FB70
// FUNCTION: fn_07350
void CPetitioEffect::Render() {
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
void CPneumaticusProcellaEffect::Open() {
  double scale;
  switch (this->effectLevel) {
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
    const double x = scale * offsetsX[ClampedRandom(6)];
    const double z = scale * offsetsZ[ClampedRandom(9)];
    const int delay = ClampedRandom(200) + (i + i / 10 * 20) * 100;

    EZ2STREmitter* emitter =
        reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (emitter) {
      char name[260] = {};
      StringFormat(name, sizeof(name), static_cast<std::size_t>(-1),
                   "pneumaticusprocella\\pneumaticusprocella\\pneumaticusprocella_%d.str", std::max(rand() % 4, 1));
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
void CPneumaticusProcellaEffect::Render() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->emitters.begin(); it < this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00720310
// FUNCTION: fn_07355
void CPneumaticusProcellaEffect::OnProcess() {
  if (this->emitter) {
    AsParticle(this->emitter)->Process(0.0f, -80.0f, 0.0f);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    if (IsEmitterAlive(emitter) != true) {
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

// GLOBAL: gl_24963
static long long s_randomHitIndex;

// OFFSET: 0x007205D0
// FUNCTION: fn_07360
void CRandomHitEffect::Open() {
  const double offsetsX[3] = {0.0, 4.0, -4.0};
  const double offsetsY[3] = {-7.0, -6.0, -5.0};
  const double angles[3] = {0.0, 320.0, 40.0};

  this->emitter =
      reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (this->emitter) {
    if (this->id == 2080) {
      AsParticle(this->emitter)->Load(std::string("4ig_secondfalth\\secondfalth_hit\\secondfalth_hit.str"));
      const int index = static_cast<int>(s_randomHitIndex);
      AsParticle(this->emitter)->m_angle = static_cast<float>(angles[index]);
      this->emitter->offset.x = static_cast<float>(offsetsX[index]);
      this->emitter->offset.y = static_cast<float>(offsetsY[index]);
      this->emitter->offset.z = 0.0f;
      AsParticle(this->emitter)->m_delay = index * 200;
    } else if (this->id == 2082) {
      AsParticle(this->emitter)->Load(std::string("4iq_secondjudgement\\secondjudgement\\secondjudgement.str"));
      const int index = static_cast<int>(s_randomHitIndex);
      AsParticle(this->emitter)->m_angle = static_cast<float>(angles[index]);
      this->emitter->offset.x = static_cast<float>(offsetsX[index]);
      this->emitter->offset.y = static_cast<float>(offsetsY[index]);
      this->emitter->offset.z = 0.0f;
      AsParticle(this->emitter)->m_delay = index * 200;
    }
  }

  if (s_randomHitIndex < 2) {
    ++s_randomHitIndex;
  } else {
    s_randomHitIndex = 0;
  }
}

// OFFSET: 0x007208A0
// FUNCTION: fn_07362
void CRandomHitEffect::OnProcess() {
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
void CRandomHitEffect::Render() {
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
void CRandomOptEffect::SetColor(int red, int green, int blue) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00720B30
// FUNCTION: fn_07367
void CRandomOptEffect::SetParticleTexture(std::string name) {
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
      particle->effectId = this->id;
      particle->rowIndex = ring;
      particle->columnIndex = i;
      particle->groupIndex = 3;
      particle->sourceBlend = 5;
      particle->destinationBlend = 6;
      particle->alpha = alpha;
      particle->SetColor(100, 100, 255);
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(baseAngle);
      const float level = static_cast<float>(ring);
      particle->angleParameter = 360;
      particle->speedParameter = 100.0f - level * 10.0f;
      particle->scale = level * 0.2f + 0.5f;
      particle->phaseParameter = static_cast<int>(level * 0.5f + 90.0f);
      particle->animationParameter = 0.0f;

      switch (this->id) {
      case EF_UNK_1186:
        particle->SetColor(255, 200, 255);
        particle->speedParameter = 120.0f - level * 12.0f;
        particle->scale = level * 0.24f + 0.6f;
        break;
      case EF_UNK_1187:
        particle->SetColor(192, 192, 192);
        break;
      case EF_UNK_1188:
        particle->SetColor(0, 100, 150);
        break;
      case EF_DROPITEM_YELLOW:
        particle->SetColor(255, 255, 128);
        break;
      case EF_DROPITEM_PURPLE:
        particle->SetColor(128, 0, 255);
        break;
      case EF_UNK_1191:
        particle->SetColor(255, 128, 0);
        break;
      }

      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);

      if (particle) {
        const int segment = particle->columnIndex;
        const float height = g_sinTable[segment * 9] * particle->speedParameter;
        const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
        const int angle =
            static_cast<int>(static_cast<float>(step * segment) + *reinterpret_cast<const float*>(&particle->angleBits)) %
            360;
        const float spread = height * g_cosTable[particle->phaseParameter];
        const float lift = height * g_sinTable[particle->phaseParameter];
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
void CRandomOptEffect::Open() {
  this->Init();
}

// OFFSET: 0x00721230
// FUNCTION: fn_07371
bool CRandomOptEffect::IsAlive() {
  if (this->actor == 0) {
    return false;
  }

  if (!g_modeMgr.GetGameMode()) {
    return false;
  }

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (!world) {
    return false;
  }

  if (!world->GetItemByAID(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00721270
// FUNCTION: fn_07372
void CRandomOptEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (*it) {
      const int angle = static_cast<int>(
          static_cast<float>(particle->rowIndex) + *reinterpret_cast<const float*>(&particle->angleBits) + 3.0f);
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(angle);

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
      particle->transform = rotation;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00721430
// FUNCTION: fn_07373
void CRandomOptEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->columnIndex;
    if (texPart <= 0) {
      continue;
    }

    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.left = static_cast<float>(texPart - 1) / 21.0f;
    quad.right = static_cast<float>(texPart) / 21.0f;
    particle->Render(quad);
  }
}

// OFFSET: 0x00721560
// FUNCTION: fn_07374
CRichsCoinEffect::CRichsCoinEffect() : CEffect(1135), coinCount(0) {
}

// OFFSET: 0x007218D0
// FUNCTION: fn_07377
void CRichsCoinEffect::Open() {
  int count = this->coinCount;
  if (count > 5) {
    count = 5;
  }

  for (int i = 0; i < count; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->SetTexture("effect\\rl_richs_coin_b\\rcoin1.bmp");
      particle->animationParameter = static_cast<float>(360 / count * i) / 180.0f * 3.141592f - 10.0f;
      reinterpret_cast<float&>(particle->animationParameters) = 0.0f;
      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }

  if (this->coinCount > 5) {
    const int lights = this->coinCount - 5;
    for (int i = 0; i < lights; ++i) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->SetTexture("effect\\rl_richs_coin_b\\light.bmp");
        particle->animationParameter = static_cast<float>(i * 72) / 180.0f * 3.141592f - 10.0f;
        reinterpret_cast<float&>(particle->animationParameters) = 0.0f;
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
bool CRichsCoinEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00721F80
// FUNCTION: fn_07380
void CRichsCoinEffect::Render() {
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
CSavageImpactEffect::CSavageImpactEffect() : CEffect(1837) {
  this->offset.x = 0.0f;
  this->offset.y = 0.0f;
  this->offset.z = 0.0f;
}

// OFFSET: 0x00722460
// FUNCTION: fn_07385
bool CSavageImpactEffect::IsAlive() {
  if (0.0f == this->emitter->lifetime) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007224A0
// FUNCTION: fn_07386
void CSavageImpactEffect::OnProcess() {
  CEZ2STRParticle* particle = AsParticle(this->emitter);
  const float x = this->position[0] + 0.0f;
  const float y = this->position[1] - 12.0f;
  const float z = this->position[2] + 0.0f;
  particle->position.x = x;
  particle->position.y = y;
  particle->position.z = z;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00722560
// FUNCTION: fn_07387
void CSavageImpactEffect::Render() {
  AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x007226A0
// FUNCTION: fn_07389
CServantWeaponEffect::CServantWeaponEffect() : CEffect(-1) {
  for (vector3d& offset : this->offsets) {
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
  }
}

// OFFSET: 0x00722730
// FUNCTION: fn_07390
CServantWeaponNewEffect::CServantWeaponNewEffect() : CEffect(2460) {
}

// OFFSET: 0x00722830
// FUNCTION: fn_07391
CServantWeaponNewEffect::~CServantWeaponNewEffect() {
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
      reinterpret_cast<EZ2STREmitter*>(particle)->angle = angle;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
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
void CServantWeaponEffect::Open() {
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
void CServantWeaponNewEffect::Open() {
  this->retained = 1;
}

// OFFSET: 0x00723AA0
// FUNCTION: fn_07402
bool CServantWeaponEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00723AB0
// FUNCTION: fn_07403
bool CServantWeaponNewEffect::IsAlive() {
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
  case DK_SERVANTWEAPON_ATK:
    particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANTWEAPON\\servantweapon_shoot\\servantweapon_shoot.str"));
    break;
  case DK_SERVANT_W_SIGN:
    particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANT_W_SIGN\\servant_sign_shot\\servant_sign_shot.str"));
    break;
  case DK_SERVANT_W_PHANTOM:
    particle->Load(std::string("DRAGON_KNIGHT\\DK_SERVANT_W_PHANTOM\\servant_phantom_shot\\servant_phantom_shot.str"));
    break;
  case DK_SERVANT_W_DEMOL:
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

  const vector3d position = *reinterpret_cast<const vector3d*>(effect->position);
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
void CServantWeaponEffect::OnProcess() {
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
  const float sine = ClientSin(radians) * 13.0f;
  const float cosine = ClientCos(radians) * 13.0f;
  emitter->position.z = sine + effect->position[2];
  emitter->position.y = effect->position[1] - 10.0f;
  emitter->position.x = cosine + effect->position[0];
}

// OFFSET: 0x00724900
// FUNCTION: fn_07406
void CServantWeaponNewEffect::OnProcess() {
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
void CServantWeaponEffect::Render() {
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
void CServantWeaponNewEffect::Render() {
  for (EZ2STREmitter* emitter : this->emitters) {
    AsParticle(emitter)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (EZ2STREmitter* trail : this->trails) {
    AsParticle(trail)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
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
  this->rotationParameter = 0;
}

// OFFSET: 0x007251C0
// FUNCTION: fn_07411
CSevereRainStorm::~CSevereRainStorm() {
}

// OFFSET: 0x0071D650
// FUNCTION: fn_07334
void CNewCloudKillEffect::ResetMistParticle(CParticle* particle) {
  particle->alpha = 0;
  *reinterpret_cast<int*>(particle->effectParameters) = ClampedRandom(50) + 150;
  particle->animationState = 0;
  particle->elapsedFrames = 0;
  InitParticleQuad(particle->vertices);

  const int spread = ClampedRandom(15) + 2;
  const int angle = ClampedRandom(360);
  particle->offsetY = 0.0f;
  particle->offsetZ = static_cast<float>(spread) * g_sinTable[angle];
  particle->offsetX = static_cast<float>(spread) * g_cosTable[angle];
  particle->scale = static_cast<float>(ClampedRandom(10)) * 0.1f + 0.5f;
  particle->speedParameter = static_cast<float>(ClampedRandom(30)) * 0.01f + 0.05f;
}

// OFFSET: 0x0071D760
// FUNCTION: fn_07335
void CNewCloudKillEffect::Open() {
  this->circleResetTime = static_cast<int>(timeGetTime());
  this->cloudResetTime = static_cast<int>(timeGetTime());

  CParticle* circle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  EZ2STREmitter* circleEmitter = reinterpret_cast<EZ2STREmitter*>(circle);
  if (circle) {
    circle->SetTexture("effect\\new_cloud_kill\\ef_cloud_circle.tga");
    circle->effectId = this->id;
    circle->columnIndex = 1;
    circle->rowIndex = 0;
    circle->groupIndex = 1;
    circle->scale = 20.0f;
    circle->alpha = 0xFF;
    circle->angleBits = 0;
    circle->elapsedFrames = 60;
    circle->animationState = 0;
    InitParticleQuad(circle->vertices);
    this->circleEmitters.push_back(circleEmitter);
  }

  for (int i = 0; i < 100; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    if (particle) {
      particle->effectId = this->id;
      particle->columnIndex = i;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->SetTexture("effect\\new_cloud_kill\\ef_cloud_kill.tga");
      ResetCloudParticle(particle);
      this->cloudEmitters.push_back(emitter);
    }
  }

  for (int i = 0; i < 30; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    if (particle) {
      particle->effectId = this->id;
      particle->columnIndex = i;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->SetTexture("effect\\new_cloud_kill\\ef_cloud_kill.tga");
      ResetSmokeParticle(particle);
      particle->elapsedFrames = -ClampedRandom(71);
      this->smokeEmitters.push_back(emitter);
    }
  }

  CParticle* lightning = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  EZ2STREmitter* lightningEmitter = reinterpret_cast<EZ2STREmitter*>(lightning);
  if (lightning) {
    lightning->effectId = this->id;
    lightning->columnIndex = 1;
    lightning->rowIndex = 0;
    lightning->groupIndex = 1;
    lightning->SetTexture("effect\\new_cloud_kill\\ef_comm_lightning_cloud.tga");
    lightning->alpha = 0xFF;
    lightning->depthOffset = 0.035f;
    ResetLightning(lightning);
    lightning->phaseParameter = 50;
    this->lightningEmitters.push_back(lightningEmitter);
  }

  for (int i = 0; i < 30; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
    if (particle) {
      particle->effectId = this->id;
      particle->columnIndex = i;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->SetTexture("effect\\new_cloud_kill\\ef_cloud_kill.tga");
      ResetMistParticle(particle);
      particle->elapsedFrames = -ClampedRandom(80);
      this->mistEmitters.push_back(emitter);
    }
  }
}

// OFFSET: 0x0071DB70
// FUNCTION: fn_07337
void CNewCloudKillEffect::ResetCloudParticle(CParticle* particle) {
  particle->frameParameters[0] = ClampedRandom(2);

  const int angle = ClampedRandom(360);
  particle->offsetY = 0.0f;
  particle->alpha = 0xFF;
  particle->offsetX = g_cosTable[angle] * 18.0f;
  particle->offsetZ = g_sinTable[angle] * 18.0f;
  particle->scale = static_cast<float>(ClampedRandom(31)) * 0.1f + 2.0f;
  particle->animationState = 0;
  particle->speedParameter = static_cast<float>(ClampedRandom(31)) * 0.01f + 0.05f;
  InitParticleQuad(particle->vertices);
}

// OFFSET: 0x0071DC70
// FUNCTION: fn_07338
void CNewCloudKillEffect::ResetLightning(CParticle* particle) {
  particle->scale = static_cast<float>(ClampedRandom(5)) + 10.0f;
  particle->animationState = 10;
  particle->frameParameters[0] = ClampedRandom(4);
  particle->phaseParameter = ClampedRandom(50) + 10;
  *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(ClampedRandom(360));
  particle->offsetZ = static_cast<float>(ClampedRandom(10)) - 5.0f;
  particle->offsetX = static_cast<float>(ClampedRandom(10)) - 5.0f;
  particle->offsetY = -10.0f;
  particle->elapsedFrames = 0;
  InitParticleQuad(particle->vertices);
}

// OFFSET: 0x0071DD80
// FUNCTION: fn_07339
void CNewCloudKillEffect::ResetSmokeParticle(CParticle* particle) {
  particle->alpha = 0;
  particle->animationState = 0;
  particle->elapsedFrames = 0;
  InitParticleQuad(particle->vertices);

  particle->scale = static_cast<float>(ClampedRandom(51)) * 0.1f + 7.0f;
  particle->offsetY = particle->scale * -0.5f;

  const int spread = ClampedRandom(13) + 2;
  const int angle = ClampedRandom(360);
  particle->offsetZ = static_cast<float>(spread) * g_sinTable[angle];
  particle->offsetX = static_cast<float>(spread) * g_cosTable[angle];

  particle->frameParameters[0] = ClampedRandom(4);
  *reinterpret_cast<int*>(particle->effectParameters) = ClampedRandom(151) + 50;
  particle->speedParameter = static_cast<float>(ClampedRandom(10)) * 0.01f + 0.05f;
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
  if (this->spriteParticle != 0) {
    FreeParticle(reinterpret_cast<CParticle*>(this->spriteParticle));
    this->spriteParticle = 0;
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
  FreeParticle(AsParticle(this->emitter));
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
  for (auto it = this->secondStrikeEmitters.begin(); it != this->secondStrikeEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->secondStrikeEmitters.clear();
}

// OFFSET: 0x0071C4B0
// FUNCTION: fn_07322
void CJupitelThunderHit::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
  for (auto it = this->hitEmitters.begin(); it != this->hitEmitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->hitEmitters.clear();
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
  case EF_UNK_1752:
    if (this->elapsedFrames % (4 - (this->effectLevel + 1) / 2) == 0) {
      this->SpawnCrystalRain();
    }
    break;
  case EF_UNK_1276:
    if (this->elapsedFrames % 3 == 0) {
      this->SpawnSevereRain();
    }
    break;
  case EF_DIAMOND_STORM:
    if (this->elapsedFrames % 5 == 0) {
      vector3d position = {this->position[0], this->position[1], this->position[2]};
      position.x = (static_cast<float>(ClampedRandom(451)) - 225.0f) * 0.1f + position.x;
      position.z = (static_cast<float>(ClampedRandom(451)) - 225.0f) * 0.1f + position.z;

      CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
      EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
      particle->Open(std::string("diamond_storm\\diamond_storm\\diamond_storm.str"));
      this->emitters.push_back(emitter);
    }
    break;
  case EF_CROSS_RAIN:
    if (this->elapsedFrames % (9 - this->effectLevel / 3) == 0) {
      const long long ranges[10] = {3, 3, 3, 5, 5, 5, 7, 7, 7, 9};
      const long long spread = ranges[std::min(static_cast<unsigned int>(this->effectLevel), 10u) - 1] * 50;
      vector3d position = {this->position[0], this->position[1], this->position[2]};
      const float half = static_cast<float>(spread) * 0.5f;
      position.x = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + position.x;
      position.z = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + position.z;

      CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
      EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
      particle->Load(std::string("crossrain\\cross_rain\\cross_rain.str"));
      this->emitters.push_back(emitter);
    }
    break;
  case EF_UNK_2348:
    if (this->ActorExists() && this->elapsedFrames % 15 == 0) {
      this->SpawnTwinklingGalaxy(this->elapsedFrames / 15 % 4);
    }
    break;
  case EF_UNK_2349:
    if (this->ActorExists() && this->elapsedFrames % 18 == 0) {
      this->SpawnGalaxyStar();
    }
    break;
  case EF_UNK_2350:
    if (this->ActorExists() && this->elapsedFrames % 18 == 0) {
      this->SpawnStarCannon();
    }
    break;
  case EF_UNK_2351:
    if (this->ActorExists() && this->elapsedFrames % 15 == 0) {
      this->SpawnGrenades();
    }
    break;
  case EF_UNK_2354:
    if (this->ActorExists() && this->elapsedFrames % 15 == 0) {
      this->SpawnBombardHits();
    }
    break;
  case EF_UNK_2462:
    if (this->elapsedFrames % (9 - this->effectLevel / 3) == 0) {
      this->SpawnCrossRain();
    }
    break;
  }
}

// OFFSET: 0x00725D60
// FUNCTION: fn_07414
void CSevereRainStorm::SpawnCrossRain() {
  const int ranges[10] = {3, 3, 3, 5, 5, 5, 7, 7, 7, 9};
  const int index = std::max(0, std::min(this->effectLevel - 1, 9));
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const int spread = ranges[index] * 50;

  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(ClampedRandom(spread + 1)) - half) * 0.1f + position.x;
  position.z = (static_cast<float>(ClampedRandom(spread + 1)) - half) * 0.1f + position.z;
  position.y -= 10.0f;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
  particle->Load(std::string("IMPERIAL_GUARD\\IG_CROSS_RAIN\\new_crossrain\\new_cross_rain\\new_cross_rain.str"));
  this->emitters.push_back(emitter);
}

// OFFSET: 0x00726110
// FUNCTION: fn_07415
void CSevereRainStorm::SpawnGrenades() {
  const long long ranges[5] = {11, 11, 11, 9, 9};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->effectLevel), 5u) - 1] * 50;
  for (int count = 0; count < (this->effectLevel + 1) / 2; ++count) {
    const float half = static_cast<float>(spread) * 0.5f;
    const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
    vector3d position = base;
    position.x = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + base.x;
    position.z = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + base.z;

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
    position.x = (static_cast<float>(ClampedRandom(451)) - 225.0f) * 0.1f + base.x;
    position.z = (static_cast<float>(ClampedRandom(451)) - 225.0f) * 0.1f + base.z;

    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (particle) {
      particle->Load(std::string("NIGHT_WATCH\\NW_MISSION_BOMBARD\\mission_bombard\\mission_bombard_hit.str"));
      particle->position = position;
      particle->m_scale = static_cast<float>(ClampedRandom(51) + 50) * 0.01f;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
    }
  }
}

// OFFSET: 0x00726890
// FUNCTION: fn_07417
void CSevereRainStorm::SpawnCrystalRain() {
  const long long ranges[5] = {11, 13, 15, 17, 19};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->effectLevel), 5u) - 1] * 50;
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + position.x;
  position.z = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + position.z;

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
  if (ClampedRandom(10) >= 5) {
    position.x = static_cast<float>(ClampedRandom(251)) * 0.1f + position.x;
  } else {
    position.x = position.x - static_cast<float>(ClampedRandom(251)) * 0.1f;
  }
  if (ClampedRandom(10) >= 5) {
    position.z = static_cast<float>(ClampedRandom(251)) * 0.1f + position.z;
  } else {
    position.z = position.z - static_cast<float>(ClampedRandom(251)) * 0.1f;
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
  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (emitter) {
    AsParticle(emitter)->Load(std::string("SKY_EMPEROR\\SKE_TWINKLING_GALAXY\\twinkling_galaxy_bottom\\ske_twinkling_galaxy_bottom.str"));
    emitter->offset.x = 0.2f;
    emitter->offset.y = -13.5f;
    emitter->offset.z = 0.0f;
    this->emitters.push_back(emitter);
  }

  emitter = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (emitter) {
    AsParticle(emitter)->Load(FormatOwnedString("SKY_EMPEROR\\SKE_TWINKLING_GALAXY\\twinkling_galaxy\\ske_twinkling_galaxy%02d.str", index));
    emitter->offset.x = 0.2f;
    emitter->offset.y = -13.5f;
    emitter->offset.z = 0.0f;
    this->emitters.push_back(emitter);
  }

  this->AddEvent(0, "effect\\SKY_EMPEROR\\ske_star_cannon.wav", this->frame + 1, 0);
}

// OFFSET: 0x00727050
// FUNCTION: fn_07420
void CSevereRainStorm::SpawnStarCannon() {
  const long long ranges[5] = {13, 13, 11, 11, 9};
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->effectLevel), 5u) - 1] * 50;
  const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
  vector3d position = base;
  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + base.x;
  position.z = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + base.z;

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
  const long long spread = ranges[std::min(static_cast<unsigned int>(this->effectLevel), 5u) - 1] * 50;
  const vector3d base = {this->position[0] + 0.0f, this->position[1] - 15.0f, this->position[2] + 0.0f};
  vector3d position = base;
  const float half = static_cast<float>(spread) * 0.5f;
  position.x = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + base.x;
  position.z = (static_cast<float>(ClampedRandom((static_cast<int>(spread) + 1))) - half) * 0.1f + base.z;

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
void CSevereRainStorm::Open() {
  this->elapsedFrames = 0;
  if (this->id == 2348) {
    this->retained = 1;
  }
}

// OFFSET: 0x007278E0
// FUNCTION: fn_07424
bool CSevereRainStorm::IsAlive() {
  switch (this->id) {
  case EF_UNK_1752:
  case EF_CROSS_RAIN:
  case EF_UNK_2462:
    return this->ActorExists();
  case EF_UNK_1276:
    if (this->elapsedFrames > 240) {
      return false;
    }
    break;
  case EF_DIAMOND_STORM:
    if (EffectLifetime(2026) < this->elapsedFrames) {
      return false;
    }
    break;
  case EF_UNK_2348:
  case EF_UNK_2349:
  case EF_UNK_2350:
    if (this->ActorExists() != true && this->emitters.size() == 0) {
      return false;
    }
    break;
  case EF_UNK_2351:
  case EF_UNK_2354:
    if (this->ActorExists() != true && this->emitters.size() == 0) {
      return false;
    }
    break;
  }
  return true;
}

// OFFSET: 0x00727A10
// FUNCTION: fn_07425
void CSevereRainStorm::OnProcess() {
  this->SpawnParticles();
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime >= 0.0f) {
      if (this->id == 2348 && emitter) {
        const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
        const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
        const vector3d position = *reinterpret_cast<const vector3d*>(this->position);

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
    } else {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
    }
  }
  ++this->elapsedFrames;
}

// OFFSET: 0x00727C70
// FUNCTION: fn_07426
void CSevereRainStorm::Render() {
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
void CCrimsonArrowEffect::Open() {
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
  case EF_CRIMSONARROW:
    arrow->Load("crimsonarrow\\crimsonarrow\\crimsonarrow.str");
    emitter->offset.x = 0.0f;
    emitter->offset.y = -13.0f;
    emitter->offset.z = 0.0f;
    arrow->m_angle = static_cast<float>(angle);
    this->AddEvent(0, "effect\\AG_CRIMSON_ARROW1.wav", 0, 0);
    break;
  case EF_RHYTHMSHOOTING_SHOT:
    arrow->Load("rhythmshooting\\rhythmshooting_shot\\rhythmshooting_shot.str");
    emitter->offset.x = 0.0f;
    emitter->offset.y = -8.0f;
    emitter->offset.z = 0.0f;
    arrow->m_angle = static_cast<float>(angle + 180.0);
    this->AddEvent(0, "effect\\TR_RHYTHMSHOOTING.wav", 0, 0);
    break;
  case EF_FRENZY_SHOT:
    arrow->Load("frenzy_shot\\frenzy_shot\\frenzy_shot.str");
    emitter->offset.x = 0.0f;
    emitter->offset.y = -4.0f;
    emitter->offset.z = 0.0f;
    arrow->m_angle = static_cast<float>(angle);
    this->AddEvent(0, "effect\\ABC_FRENZY_SHOT.wav", 0, 0);
    break;
  case EF_UNLUCKY_RUSH:
    arrow->Load("unlucky_rush\\unlucky_rush\\unlucky_rush.str");
    arrow->m_angle = static_cast<float>(angle);
    break;
  case EF_JUDGEMENT_CROSS_SHOT:
    arrow->Load("judgement_cross\\judgement_cross_shot\\judgement_cross_shot.str");
    arrow->m_angle = static_cast<float>(angle + 180.0);
    break;
  }

  this->emitter = emitter;
}

// OFFSET: 0x0070DE90
// FUNCTION: fn_07166
void CTunaPartyEffect::Open() {
  EZ2STREmitter* emitter =
      reinterpret_cast<EZ2STREmitter*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (!emitter) {
    return;
  }

  CSpriteParticle* particle = AsSprite(emitter);

  particle->effectId = this->id;
  particle->columnIndex = 1;
  particle->rowIndex = 0;
  particle->groupIndex = 1;
  particle->SetSprite("이팩트\\su_tunaparty\\su_tunaparty.spr");
  particle->SetAction("이팩트\\su_tunaparty\\su_tunaparty.act");
  particle->animationParameter = 0.0f;
  particle->loopMode = 3;
  this->effectLifetime = 9999;
  this->emitters.push_back(emitter);
}

// OFFSET: 0x0070DF40
// FUNCTION: fn_07168
void CTunaPartyEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CSpriteParticle* particle = AsSprite(*it);
    const float phase = particle->animationParameter + 0.05f;
    particle->animationParameter = phase;
    const float cosine = ClientCos(phase) * 10.0f;
    const float sine = ClientSin(phase) * 10.0f;
    particle->position.y = this->position[1] - 10.0f;
    particle->position.x = this->position[0] + sine;
    particle->position.z = this->position[2] + cosine;
    float angle = phase / 3.141592f * 180.0f;
    if (angle > 360.0f) {
      angle -= static_cast<float>(static_cast<int>(angle / 360.0f) * 360);
    }
    angle -= static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
    if (angle < 0.0f) {
      angle += 360.0f;
    }
    int direction = static_cast<int>((angle - 157.5) / 45.0);
    if (direction < 0) {
      direction += 8;
    }
    ++particle->elapsedFrames;
    particle->frame = direction;
  }
}

// OFFSET: 0x0070C2F0
// FUNCTION: fn_07129
void CRootTwistEffect::Open() {
  EZ2STREmitter* emitter =
      reinterpret_cast<EZ2STREmitter*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (emitter) {
    CSpriteParticle* particle = AsSprite(emitter);
    particle->effectId = this->id;
    particle->columnIndex = 0;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->SetSprite("이팩트\\su_sv_roottwist\\su_sv_roottwist.spr");
    particle->SetAction("이팩트\\su_sv_roottwist\\su_sv_roottwist.act");
    particle->renderOptions |= 0x200;
    particle->frameDelay = 8.0f;
    particle->depthOffset = 0.00015f;
    this->emitters.push_back(emitter);
  }
  this->effectLifetime = 9999;
}

// OFFSET: 0x0070D9B0
// FUNCTION: fn_07159
void CSvgSpiritEffect::Open() {
  this->emitter = reinterpret_cast<EZ2STREmitter*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (!this->emitter) {
    return;
  }

  CSpriteParticle* sprite = AsSprite(this->emitter);
  sprite->effectId = this->id;
  sprite->columnIndex = 0;
  sprite->rowIndex = 0;
  sprite->groupIndex = 1;
  AsSprite(this->emitter)->SetSprite("몬스터\\svg_spirit.spr");
  AsSprite(this->emitter)->SetAction("몬스터\\svg_spirit.act");
  this->emitter->velocity = {0.0f, 0.0f, 0.0f};
  this->effectLifetime = 16;
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
  const float distance = SquareRoot(deltaX * deltaX + deltaZ * deltaZ);
  this->emitter->speed = distance / static_cast<float>(this->effectLifetime);
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
void CNewCloudKillEffect::Render() {
  for (auto it = this->circleEmitters.begin(); it != this->circleEmitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->animationState != 0 || !*it) {
      continue;
    }

    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }

  for (auto it = this->cloudEmitters.begin(); it != this->cloudEmitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->animationState != 0 || !*it) {
      continue;
    }

    const int frame = static_cast<signed char>(particle->frameParameters[0]);
    const ParticleQuad quad = {false, 0.5f, 0.75f, static_cast<float>(frame + 1) * 0.25f, static_cast<float>(frame + 2) * 0.25f, false, true};
    particle->Render(quad);
  }

  for (auto it = this->smokeEmitters.begin(); it != this->smokeEmitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->elapsedFrames <= 0 || !*it) {
      continue;
    }

    const int frame = static_cast<signed char>(particle->frameParameters[0]);
    const ParticleQuad quad = {false, 0.0f, 0.25f, static_cast<float>(frame) * 0.25f, static_cast<float>(frame + 1) * 0.25f, false, true};
    particle->Render(quad);
  }

  for (auto it = this->lightningEmitters.begin(); it != this->lightningEmitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (static_cast<signed char>(particle->animationState) <= 0 || !*it) {
      continue;
    }

    const int frame = static_cast<signed char>(particle->frameParameters[0]);
    const int column = frame % 2;
    const int row = frame / 2;
    const ParticleQuad quad = {false,
                               static_cast<float>(row) * 0.5f,
                               static_cast<float>(row + 1) * 0.5f,
                               static_cast<float>(column) * 0.5f,
                               static_cast<float>(column + 1) * 0.5f,
                               false,
                               true};
    particle->Render(quad);
  }

  for (auto it = this->mistEmitters.begin(); it != this->mistEmitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (particle->elapsedFrames <= 0 || !*it) {
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
  this->secondStrikeEmitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));

  CEZ2STRParticle* knife = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  knife->Load(std::string("dancing_knife\\dancing_knife\\dancing_knife_skill_02.str"));
  knife->m_loop = true;
  knife->m_actorId = this->actor;
  this->secondStrikeEmitters.push_back(reinterpret_cast<EZ2STREmitter*>(knife));
}

// OFFSET: 0x00710800
// FUNCTION: fn_07202
void CDancingKnifeEffect::Open() {
  this->secondStrike = 0;
  this->SpawnFirstStrike();
  this->AddEvent(0, "effect\\SHC_DANCING_KNIFE.wav", 0, 0);
}

// OFFSET: 0x00710840
// FUNCTION: fn_07204
void CDancingKnifeEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (IsEmitterAlive(emitter) == true) {
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

  for (auto it = this->secondStrikeEmitters.begin(); it != this->secondStrikeEmitters.end(); ++it) {
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
// FUNCTION: fn_07173
void CCurseOfCubeEffect::SpawnLoop() {
  if (this->id == 0x901) {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    if (bottom) {
      bottom->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_cube\\npc_cur_r_cube_bottom_loop.str");
      bottom->m_loop = true;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));
    }
    CEZ2STRParticle* cube = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (cube) {
      cube->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_cube\\npc_cur_r_cube_loop.str");
      cube->m_loop = true;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(cube));
    }
  } else if (this->id == 0x902) {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    if (bottom) {
      bottom->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_cube\\npc_cur_b_cube_bottom_loop.str");
      bottom->m_loop = true;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(bottom));
    }
    CEZ2STRParticle* cube = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (cube) {
      cube->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_cube\\npc_cur_b_cube_loop.str");
      cube->m_loop = true;
      this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(cube));
    }
  }
}

// OFFSET: 0x0070E770
// FUNCTION: fn_07174
void CCurseOfCubeEffect::SpawnStart() {
  if (this->id == 0x901) {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    EZ2STREmitter* bottomEmitter = reinterpret_cast<EZ2STREmitter*>(bottom);
    if (bottom) {
      bottom->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_cube\\npc_cur_r_cube_bottom_start.str");
      this->emitters.push_back(bottomEmitter);
    }
    CEZ2STRParticle* cube = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    EZ2STREmitter* cubeEmitter = reinterpret_cast<EZ2STREmitter*>(cube);
    if (cube) {
      cube->Open("NPC_CURSE_OF_RED_CUBE\\npc_cur_r_cube\\npc_cur_r_cube_start.str");
      this->emitters.push_back(cubeEmitter);
    }
  } else if (this->id == 0x902) {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    EZ2STREmitter* bottomEmitter = reinterpret_cast<EZ2STREmitter*>(bottom);
    if (bottom) {
      bottom->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_cube\\npc_cur_b_cube_bottom_start.str");
      this->emitters.push_back(bottomEmitter);
    }
    CEZ2STRParticle* cube = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    EZ2STREmitter* cubeEmitter = reinterpret_cast<EZ2STREmitter*>(cube);
    if (cube) {
      cube->Open("NPC_CURSE_OF_BLUE_CUBE\\npc_cur_b_cube\\npc_cur_b_cube_start.str");
      this->emitters.push_back(cubeEmitter);
    }
  }
}

// OFFSET: 0x0070EB90
// FUNCTION: fn_07179
void CCurseOfCubeEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (IsEmitterAlive(emitter)) {
      if (emitter) {
        const vector3d position = *reinterpret_cast<const vector3d*>(this->position);
        const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
        const vector3d viewPosition = {
            position.x * view.values[0] + position.y * view.values[3] + position.z * view.values[6] + view.values[9],
            position.x * view.values[1] + position.y * view.values[4] + position.z * view.values[7] + view.values[10],
            position.x * view.values[2] + position.y * view.values[5] + position.z * view.values[8] + view.values[11]};
        const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

        const float vx = viewPosition.x + this->viewOffsetX;
        const float vy = viewPosition.y + this->viewOffsetY;
        const float vz = viewPosition.z + this->viewOffsetZ;

        const float worldX = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
        const float worldY = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
        const float worldZ = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];
        emitter->position.x = worldX;
        emitter->position.y = worldY;
        emitter->position.z = worldZ;
        AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
    }
  }

  if (this->spawnPhase == 1 && this->emitters.empty()) {
    this->SpawnLoop();
    if (!this->emitters.empty()) {
      this->spawnPhase = 2;
    }
  }
}

// OFFSET: 0x0070C750
// FUNCTION: fn_07136
void CSprArrowEffect::Open() {
  this->spriteParticle = reinterpret_cast<int>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x101));
  if (!this->spriteParticle) {
    return;
  }

  CSpriteParticle* particle = reinterpret_cast<CSpriteParticle*>(this->spriteParticle);
  particle->effectId = this->id;
  particle->columnIndex = 0;
  particle->rowIndex = 0;
  particle->groupIndex = 1;
  reinterpret_cast<CSpriteParticle*>(this->spriteParticle)->velocity = {0.0f, -10.0f, 0.0f};
  particle = reinterpret_cast<CSpriteParticle*>(this->spriteParticle);
  particle->position.x = this->position[0];
  particle->position.y = this->position[1];
  particle->position.z = this->position[2];
  this->startTick = static_cast<int>(timeGetTime());
  this->startX = this->position[0];
  this->startY = this->position[1];
  this->startZ = this->position[2];
}

struct CylinderSlice {
  /* 0000 */ double radius;
  /* 0008 */ double pitch;
  /* 0010 */ double height;

  vector3d* ComputePoints(vector3d points[2], double angle);
};

// OFFSET: 0x0070F800
// FUNCTION: fn_07190
__declspec(noinline) vector3d* CylinderSlice::ComputePoints(vector3d points[2], double angle) {
  const float baseZ = static_cast<float>(ClientSin(static_cast<float>(angle)) * this->radius);
  const float baseX = static_cast<float>(ClientCos(static_cast<float>(angle)) * this->radius);
  vector3d result[2];
  result[0].x = baseX;
  result[0].y = 0.0f;
  result[0].z = baseZ;
  const float offsetZ =
      static_cast<float>(ClientSin(static_cast<float>(angle)) * (this->height * ClientCos(static_cast<float>(this->pitch))));
  const float offsetY = static_cast<float>(this->height * -ClientSin(static_cast<float>(this->pitch)));
  const float offsetX =
      static_cast<float>(ClientCos(static_cast<float>(this->pitch)) * this->height * ClientCos(static_cast<float>(angle)));
  result[1].x = offsetX + baseX;
  result[1].y = offsetY + 0.0f;
  result[1].z = offsetZ + baseZ;
  points[0] = result[0];
  points[1] = result[1];
  return points;
}

// OFFSET: 0x0070FC80
// FUNCTION: fn_07195
void CCylinderEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    emitter->position.x = this->position[0];
    emitter->position.y = this->position[1];
    emitter->position.z = this->position[2];
    if (!*it) {
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    float& angle = reinterpret_cast<float&>(particle->angleBits);
    const double baseAngle = angle;

    CylinderSlice slice;
    slice.radius = particle->scale;
    slice.pitch = static_cast<float>(particle->phaseParameter) / 180.0f * 3.141592f;
    slice.height = particle->animationParameter;

    vector3d front[2];
    vector3d back[2];
    const double offset = static_cast<double>(particle->columnIndex) * 0.29919925332069397 + baseAngle;
    slice.ComputePoints(front, offset);
    slice.ComputePoints(back, offset + 0.29919925332069397);

    particle->vertices[0].x = front[1].x;
    particle->vertices[0].y = front[1].y;
    particle->vertices[0].z = front[1].z;
    particle->vertices[2].x = front[0].x;
    particle->vertices[2].y = front[0].y;
    particle->vertices[2].z = front[0].z;
    particle->vertices[1].x = back[1].x;
    particle->vertices[1].y = back[1].y;
    particle->vertices[1].z = back[1].z;
    particle->vertices[3].x = back[0].x;
    particle->vertices[3].y = back[0].y;
    particle->vertices[3].z = back[0].z;

    angle = static_cast<float>(static_cast<double>(particle->speedParameter) + baseAngle);
  }
}

// OFFSET: 0x0070D160
// FUNCTION: fn_07146
void CSpriteMableEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CSpriteParticle* particle = AsSprite(*it);
    const float yaw = particle->animationParameter += 0.02f;
    const float bob = reinterpret_cast<float&>(particle->animationParameters) += 0.05f;

    const float offsetX = ClientSin(yaw) * 6.0f;
    const float offsetY = ClientSin(bob) * 3.0f - 6.0f;
    const float offsetZ = ClientCos(yaw) * 6.0f;
    particle->position.x = offsetX + this->position[0];
    particle->position.y = offsetY + this->position[1];
    particle->position.z = offsetZ + this->position[2];

    int angle = static_cast<int>(static_cast<float>(static_cast<int>(yaw / 3.141592f * 180.0f) % 360) -
                                 static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw);
    if (angle < 0) {
      angle += 360;
    }

    const int frame = (angle - 75) / 30;
    ++particle->elapsedFrames;
    particle->frame = frame < 0 ? frame + 12 : frame;
  }
}

// OFFSET: 0x007180C0
// FUNCTION: fn_07289
void CGravityControlEffect::Close() {
  if (this->target) {
    FreeParticle(reinterpret_cast<CParticle*>(this->target));
    this->target = 0;
  }

  CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
  if (!actor) {
    return;
  }

  actor->render.position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(actor->render.position.x, actor->render.position.z);
  actor->positionDelta.x = 0.0f;
  actor->positionDelta.y = 0.0f;
  actor->positionDelta.z = 0.0f;

  if (this->id == 0x4B1) {
    CRenderObject* player = reinterpret_cast<CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
    const vector3d playerPosition = player->render.position;
    g_soundMgr->PlayWave("effect\\t_공격력.wav", this->position[0] - playerPosition.x, 0.0f, this->position[2] - playerPosition.z, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x007182E0
// FUNCTION: fn_07291
void CGravityControlEffect::OnProcess() {
  CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
  if (!actor) {
    return;
  }

  this->heightOffset = std::max(this->heightOffset - g_depthAverageHalf, this->minimumHeightOffset);

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float height = mode->scene->terrain->Height(actor->render.position.x, actor->render.position.z);
  const int offset = static_cast<int>(this->heightOffset);
  actor->positionDelta.x = 0.0f;
  actor->positionDelta.z = 0.0f;
  actor->positionDelta.y = static_cast<float>(offset);
  actor->render.position.y = height + this->heightOffset;

  if (this->target) {
    EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(this->target);
    AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
    emitter->position.x = this->position[0];
    emitter->position.y = this->position[1];
    emitter->position.z = this->position[2];
    ++emitter->elapsed;
  }
}

// OFFSET: 0x00717850
// FUNCTION: fn_07282
void CGaleStorm::SpawnArrow() {
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const int spanX = static_cast<int>(this->count) * 5 + 1;
  const int offsetX = spanX == 0 ? 0 : ClampedRandom(spanX);
  position.x = static_cast<float>(offsetX) - static_cast<float>(this->count * 5) * 0.5f + position.x;
  const int spanZ = static_cast<int>(this->count) * 5 + 1;
  const int offsetZ = spanZ == 0 ? 0 : ClampedRandom(spanZ);
  position.z = static_cast<float>(offsetZ) - static_cast<float>(this->count * 5) * 0.5f + position.z;

  CEZ2STRParticle* arrow = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
  EZ2STREmitter* arrowEmitter = reinterpret_cast<EZ2STREmitter*>(arrow);
  if (arrow) {
    arrow->Load(std::string("galestorm\\galestorm_arrow\\galestorm_arrow.str"));
    this->emitters.push_back(arrowEmitter);
  }

  if (rand() % 7 == 0) {
    this->AddEvent(0, "effect\\WH_GALESTORM.wav", this->frame + 1, 0);
  }
}

// OFFSET: 0x00717B60
// FUNCTION: fn_07283
void CGaleStorm::SpawnStorm() {
  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const int spanX = static_cast<int>(this->count) * 5 + 1;
  const int offsetX = spanX == 0 ? 0 : ClampedRandom(spanX);
  position.x = static_cast<float>(offsetX) - static_cast<float>(this->count * 5) * 0.5f + position.x;
  const int spanZ = static_cast<int>(this->count) * 5 + 1;
  const int offsetZ = spanZ == 0 ? 0 : ClampedRandom(spanZ);
  position.z = static_cast<float>(offsetZ) - static_cast<float>(this->count * 5) * 0.5f + position.z;

  CEZ2STRParticle* storm = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
  if (storm) {
    storm->Load(std::string("galestorm\\galestorm\\galestorm.str"));
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(storm));
  }
}

// OFFSET: 0x00717ED0
// FUNCTION: fn_07287
void CGaleStorm::OnProcess() {
  const int frame = this->frame;
  if (frame <= this->effectLifetime) {
    if (0 == frame % (100 / this->count)) {
      this->SpawnStorm();
    }
    if (0 == frame % (30 / this->count)) {
      this->SpawnArrow();
    }
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    EZ2STREmitter* emitter = *it;
    if (IsEmitterAlive(emitter) == true) {
      if (*it) {
        AsParticle(emitter)->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(AsParticle(emitter));
      it = this->emitters.erase(it);
    }
  }
}

// OFFSET: 0x007181D0
// FUNCTION: fn_07290
void CGravityControlEffect::Open() {
  switch (this->id) {
  case 0x4B1: {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (particle) {
      particle->m_loop = true;
      particle->Open("WL_TELEKINESIS_INTENSE.str");
      InitParticleQuad(particle->vertices);
      this->target = reinterpret_cast<int>(particle);
    }
    break;
  }
  case 0x960:
    this->retained = 1;
    break;
  }
}

// OFFSET: 0x007188C0
// FUNCTION: fn_07298
void CHawkVumerangEffect::Open() {
  this->startTime = timeGetTime();
  this->targetPosition.x = this->position[0];
  this->targetPosition.y = this->position[1];
  this->targetPosition.z = this->position[2];
  this->originPosition.x = this->sourcePosition[0];
  this->originPosition.y = this->sourcePosition[1];
  this->originPosition.z = this->sourcePosition[2];
  this->orbitCenter.x = (this->originPosition.x + this->targetPosition.x) * 0.5f;
  this->orbitCenter.y = (this->originPosition.y + this->targetPosition.y) * 0.5f;
  this->orbitCenter.z = (this->originPosition.z + this->targetPosition.z) * 0.5f;

  const float dx = this->originPosition.x - this->targetPosition.x;
  const float dy = this->originPosition.y - this->targetPosition.y;
  const float dz = this->originPosition.z - this->targetPosition.z;
  this->orbitRadius = SquareRoot(dy * dy + dx * dx + dz * dz) * 0.5f;
  this->startAngle = std::acos(static_cast<double>(this->targetPosition.x - this->orbitCenter.x) / this->orbitRadius);
  if (this->orbitCenter.z > this->targetPosition.z) {
    this->startAngle = 6.283184f - this->startAngle;
  }

  for (long long i = 0; i < this->count; ++i) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    particle->Open("hawkvumerang\\hawkvumerang\\hawkvumerang.str");
    particle->m_angle = 360.0f - static_cast<float>(this->startAngle) / 3.141592f * 180.0f;
    particle->m_loop = true;
    this->emitters.push_back(reinterpret_cast<EZ2STREmitter*>(particle));
  }

  this->AddEvent(0, "effect\\WH_HAWKBOOMERANG.wav", 0, 0);
}

// OFFSET: 0x00721AF0
// FUNCTION: fn_07379
void CRichsCoinEffect::OnProcess() {
  char name[256];

  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      const float angle = particle->animationParameter + 0.015f;
      particle->animationParameter = angle;
      const float cosine = ClientCos(angle) * 7.0f;
      const float sine = ClientSin(angle) * 7.0f;
      particle->position.z = cosine + this->position[2];
      particle->position.x = sine + this->position[0];
      particle->position.y = this->position[1] - 10.0f;

      float degrees = angle / 3.141592f * 180.0f - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
      if (degrees > 360.0f) {
        degrees = static_cast<float>(static_cast<int>(degrees) % 360);
      }
      while (degrees < 0.0f) {
        degrees += 360.0f;
      }

      const int frame = static_cast<int>(degrees / 36.0f);
      std::sprintf(name, "effect\\rl_richs_coin_b\\rcoin%d.bmp", frame % 5 + 1);
      particle->SetTexture(name);
    }
    ++particle->elapsedFrames;
  }

  for (auto it = this->coins.begin(); it != this->coins.end(); ++it) {
    CParticle* coin = *it;
    coin->position.x = this->position[0];
    coin->position.y = this->position[1];
    coin->position.z = this->position[2];
    if (coin) {
      const float angle = coin->animationParameter + 0.015f;
      coin->animationParameter = angle;
      const float cosine = ClientCos(angle) * 7.0f;
      const float sine = ClientSin(angle) * 7.0f;
      coin->position.z = cosine + this->position[2];
      coin->position.y = this->position[1] - 10.0f;
      coin->position.x = sine + this->position[0];
    }
    ++coin->elapsedFrames;
  }
}

// OFFSET: 0x007221C0
// FUNCTION: fn_07384
void CSavageImpactEffect::Open() {
  this->emitter = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  float toReciprocalDepth = 0.0f;
  float fromReciprocalDepth = 0.0f;
  this->offset.x = this->sourcePosition[0] - this->position[0];
  this->offset.y = this->sourcePosition[1] - this->position[1];
  this->offset.z = this->sourcePosition[2] - this->position[2];

  float fromX;
  float fromY;
  float toX;
  float toY;
  g_renderer->ProjectViewport(this->position, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
  g_renderer->ProjectViewport(this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

  const float dx = toX - fromX;
  const float dy = toY - fromY;
  double angle = static_cast<float>(std::acos(static_cast<double>(dx / SquareRoot(dy * dy + dx * dx)))) / 3.141592f * 180.0f;
  if (fromY > toY) {
    angle = 360.0 - angle;
  }

  AsParticle(this->emitter)->Load(std::string("savage_impact\\savage_impact\\savage_impact.str"));
  AsParticle(this->emitter)->m_angle = static_cast<float>(angle);
  this->AddEvent(0, "effect\\SHC_SAVAGE_IMPACT.wav", 0, 0);
}
