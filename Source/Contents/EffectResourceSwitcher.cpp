#include "Contents/EffectResourceSwitcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Network/TextEncoding.h"
#include "Resource/World/TerrainResource.h"

#include <algorithm>
#include <cctype>
#include <new>
#include <string>

// GLOBAL: gl_24485
template <>
CEZ2STREffectMgr* Singleton<CEZ2STREffectMgr>::s_instance = nullptr;

// OFFSET: 0x005A1B70
// FUNCTION: fn_03077
EffectResourceSwitcher::EffectResourceSwitcher() : CEffect(-1), active(0), effect(nullptr), spawnPosition{0.0f, 0.0f, 0.0f} {
}

// OFFSET: 0x005A2090
// FUNCTION: fn_03082
void EffectResourceSwitcher::RetireEffects() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  if (!this->sourceName.empty()) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName));
  }

  if (!this->targetName.empty()) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->targetName));
  }
}

// OFFSET: 0x005A2110
// FUNCTION: fn_03083
void CEffect::Slot10(int) {
}

// OFFSET: 0x005A2120
// FUNCTION: fn_03084
void CEffect::Slot08(int, int, int) {
}

// OFFSET: 0x005A2130
// FUNCTION: fn_03085
void CEffect::Slot11(std::string) {
}

// OFFSET: 0x005A2140
// FUNCTION: fn_03086
void CEffect::Slot09(int) {
}

// OFFSET: 0x005A2160
// FUNCTION: fn_03088
void CEffect::Slot31() {
}

// OFFSET: 0x005A2170
// FUNCTION: fn_03089
vector3d CEffect::Slot25() {
  return vector3d{0.0f, 0.0f, 0.0f};
}

// OFFSET: 0x005A2190
// FUNCTION: fn_03090
int CEffect::Slot28() {
  return 0;
}

// OFFSET: 0x005A21A0
// FUNCTION: fn_03091
void EffectResourceSwitcher::Start() {
  if (this->effect) {
    this->active = 1;
    this->effect->EndLoop(1);
  }
}

// OFFSET: 0x005A21C0
// FUNCTION: fn_03092
void CEffect::Slot01() {
}

// OFFSET: 0x005A21D0
// FUNCTION: fn_03093
void EffectResourceSwitcher::SetNames(const std::string& source, const std::string& target) {
  if (source.empty() || target.empty()) {
    return;
  }

  this->sourceName = source;
  this->targetName = target;

  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  this->effect = static_cast<CEZ2STREffect*>(
      g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
  this->spawnPosition = actor->render.position;
}

// OFFSET: 0x005A22E0
// FUNCTION: fn_03094
void CEffect::Slot12() {
}

// OFFSET: 0x005A22F0
// FUNCTION: fn_03095
void CEffect::Slot07() {
}

// OFFSET: 0x005A2300
// FUNCTION: fn_03096
bool EffectResourceSwitcher::Slot06() {
  if (!this->effect) {
    return false;
  }

  if (!this->effect->retained) {
    return !this->effect->IsFinished();
  }

  return true;
}

// OFFSET: 0x005A2330
// FUNCTION: fn_03097
void EffectResourceSwitcher::Slot03() {
  if (!this->effect || !this->active || !this->effect->IsFinished()) {
    return;
  }

  CEZ2STREffect* previous = this->effect;
  this->sourceName = this->targetName;
  this->targetName = "";
  this->effect = static_cast<CEZ2STREffect*>(g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName), this->spawnPosition.x,
                                                                this->spawnPosition.y, this->spawnPosition.z, 0, -1, 0, 0));
  this->active = 0;
  if (previous) {
    previous->EndLoop(0);
  }
}

// OFFSET: 0x005A2430
// FUNCTION: fn_03098
void CEffect::Slot04() {
}

// OFFSET: 0x005A2440
// FUNCTION: fn_03099
void CEffect::Slot00(int) {
}

// OFFSET: 0x005A2450
// FUNCTION: fn_03100
void CEffect::Slot23(float) {
}

// OFFSET: 0x005A2460
// FUNCTION: fn_03101
void CEffect::Slot24(int, int, int, int, int) {
}

// OFFSET: 0x005A2470
// FUNCTION: fn_03102
void CEffect::Slot26(int) {
}

// OFFSET: 0x005A2480
// FUNCTION: fn_03103
void CEffect::Slot27(int) {
}

// OFFSET: 0x005A2490
// FUNCTION: fn_03104
void CEffect::Slot21(int) {
}

// OFFSET: 0x005A24A0
// FUNCTION: fn_03105
void CEffect::Slot32() {
}

// OFFSET: 0x005A24B0
// FUNCTION: fn_03106
void CEffect::Slot15(int) {
}

// OFFSET: 0x005A24C0
// FUNCTION: fn_03107
void CEffect::Slot14(int) {
}

// OFFSET: 0x005A24D0
// FUNCTION: fn_03108
void CEffect::Slot17(float) {
}

// OFFSET: 0x005A24E0
// FUNCTION: fn_03109
void CEffect::Slot18(float) {
}

// OFFSET: 0x005A24F0
// FUNCTION: fn_03110
void CEffect::Slot19(float) {
}

// OFFSET: 0x005A2500
// FUNCTION: fn_03111
void CEffect::Slot22(float) {
}

// OFFSET: 0x005A2510
// FUNCTION: fn_03112
void CEffect::Slot13(int) {
}

// OFFSET: 0x005A2520
// FUNCTION: fn_03113
void EffectResourceSwitcher::Slot16(int argument) {
  if (this->effect) {
    this->effect->Slot16(argument);
  }
}

// OFFSET: 0x005A2540
// FUNCTION: fn_03114
void CEffect::Slot20(int) {
}

// OFFSET: 0x005A2630
// FUNCTION: fn_03115
template <>
CEZ2STREffectMgr* Singleton<CEZ2STREffectMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CEZ2STREffectMgr;
  }

  return s_instance;
}

// OFFSET: 0x006FFA00
// FUNCTION: fn_06967
bool CEZ2STREffect::IsFinished() {
  if (!this->emitter) {
    return false;
  }

  return this->emitter->currentFrame >= this->emitter->frameCount;
}

// OFFSET: 0x006FFA20
// FUNCTION: fn_06968
void CEZ2STREffect::Slot01() {
  EZ2STREffectInfo* info = CEZ2STREffectMgr::GetInstance()->FindById(this->id);
  if (!info) {
    return;
  }

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (!particle) {
    return;
  }

  InitParticleQuad(particle->vertices);
  particle->m_actorId = this->actor;
  particle->Load(info->name);

  EZ2STREmitter* emitter = reinterpret_cast<EZ2STREmitter*>(particle);
  emitter->offset.x = info->offsetX;
  emitter->offset.y = info->offsetY;
  emitter->offset.z = 0.0f;
  if (info->unknown_0024 & 1) {
    this->retained = 1;
    emitter->looping = 1;
  }

  emitter->duration = info->unknown_0040[1];
  this->frameStep = info->unknown_0040[1] / 16;
  if (!info->unknown_0028.empty()) {
    this->AddEvent(0, info->unknown_0028.c_str(), this->frameStep + 1, 0);
  }

  if (info->unknown_0040[0] >= 0) {
    this->AddEvent(1, nullptr, info->unknown_0040[0], 0);
  }

  if (info->unknown_0024 & 2) {
    particle->flags = 0xD;
  }

  if (info->unknown_0024 & 4) {
    particle->flags = 0x205;
  }

  emitter->unknown_3FEA = (info->unknown_0024 >> 3) & 1;
  emitter->unknown_3FEB = (info->unknown_0024 >> 4) & 1;
  this->emitter = emitter;
}

// OFFSET: 0x006FFBF0
// FUNCTION: fn_06969
bool CEZ2STREffect::Slot06() {
  EZ2STREmitter* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (this->timed == 1) {
    return emitter->remainingTime > 0.0f;
  }

  if (emitter->looping == 1) {
    return CEffect::Slot06();
  }

  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f;
}

// OFFSET: 0x006FFC60
// FUNCTION: fn_06970
void CEZ2STREffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  if (this->timed == 1) {
    this->emitter->remainingTime -= 0.05f;
  }

  if (this->emitter->unknown_3FEB) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->position[1] = mode->scene->terrain->Grid().Height(this->position[0], this->position[2]);
  }

  if (EZ2STREmitter* emitter = this->emitter) {
    const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
    const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;

    const float x = this->position[0];
    const float y = this->position[1];
    const float z = this->position[2];
    const float viewX = x * view[0] + y * view[3] + z * view[6] + view[9] + emitter->offset.x;
    const float viewY = x * view[1] + y * view[4] + z * view[7] + view[10] - emitter->offset.y;
    const float viewZ = x * view[2] + y * view[5] + z * view[8] + view[11];

    emitter->position.x = viewX * inverse[0] + viewY * inverse[3] + viewZ * inverse[6] + inverse[9];
    emitter->position.y = viewX * inverse[1] + viewY * inverse[4] + viewZ * inverse[7] + inverse[10];
    emitter->position.z = viewX * inverse[2] + viewY * inverse[5] + viewZ * inverse[8] + inverse[11];
    reinterpret_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);
  }

  if (this->emitter->unknown_3FEA == 1 && this->emitter->unknown_3F58 == 1) {
    this->frame = this->frameStep;
  }

  ++this->emitter->elapsed;
}

// OFFSET: 0x006FFEA0
// FUNCTION: fn_06971
void CEZ2STREffect::Slot04() {
  if (!this->emitter) {
    return;
  }

  if (this->alternateRender) {
    reinterpret_cast<CEZ2STRParticle*>(this->emitter)->DrawGround(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  } else {
    reinterpret_cast<CEZ2STRParticle*>(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x006FFEF0
// FUNCTION: fn_06972
void CEZ2STREffect::Slot23(float argument) {
  if (this->emitter) {
    this->emitter->remainingTime = argument;
  }
}

// OFFSET: 0x006FFF10
// FUNCTION: fn_06973
void CEZ2STREffect::Slot26(int argument) {
  if (this->emitter) {
    this->emitter->unknown_3FE8 = static_cast<unsigned char>(argument);
  }
}

// OFFSET: 0x006FFF30
// FUNCTION: fn_06974
void CEZ2STREffect::Slot27(int argument) {
  if (this->emitter) {
    this->emitter->unknown_3FE9 = static_cast<unsigned char>(argument);
  }
}

// OFFSET: 0x006FFF50
// FUNCTION: fn_06975
__declspec(noinline) void CEZ2STREffect::EndLoop(unsigned char fadeOut) {
  if (this->emitter) {
    this->emitter->looping = 0;
    this->emitter->fadeOut = fadeOut;
  }
}

// OFFSET: 0x006FFF80
// FUNCTION: fn_06976
void CEZ2STREffect::Slot32() {
  this->alternateRender = 1;
}

// OFFSET: 0x006FFF90
// FUNCTION: fn_06977
void CEZ2STREffect::Slot14(int argument) {
  if (this->emitter) {
    this->emitter->unknown_3F98 = argument;
  }
}

// OFFSET: 0x006FFFB0
// FUNCTION: fn_06978
void CEZ2STREffect::Slot17(float argument) {
  if (this->emitter) {
    this->emitter->unknown_3FA4 = argument;
  }
}

// OFFSET: 0x006FFFD0
// FUNCTION: fn_06979
void CEZ2STREffect::Slot18(float argument) {
  if (this->emitter) {
    this->emitter->unknown_3FA8 = argument;
  }
}

// OFFSET: 0x006FFFF0
// FUNCTION: fn_06980
void CEZ2STREffect::Slot19(float argument) {
  if (this->emitter) {
    this->emitter->unknown_3FAC = argument;
  }
}

// OFFSET: 0x00700010
// FUNCTION: fn_06981
void CEZ2STREffect::Slot22(float argument) {
  if (this->emitter) {
    this->emitter->unknown_3FB0 = argument;
  }
}

// OFFSET: 0x00700030
// FUNCTION: fn_06982
void CEZ2STREffect::Slot16(int argument) {
  if (this->emitter) {
    this->emitter->duration = argument;
  }

  this->frameStep = argument / 16;
  this->SetEventFrame(0, this->frameStep + 1);
}

// OFFSET: 0x00700070
// FUNCTION: fn_06983
void CEZ2STREffectMgr::InfoInserter::operator()(const std::pair<const std::string, EZ2STREffectRecord>& entry) const {
  const EZ2STREffectRecord& record = entry.second;
  if (record.name.empty()) {
    return;
  }

  EZ2STREffectInfo info;
  info.id = static_cast<int>(this->manager->effects.size());
  info.name = static_cast<const std::string&>(FromUtf8(record.name, g_textCodePage));
  info.offsetX = static_cast<float>(record.offsetX);
  if (record.unknown_0028) {
    info.unknown_0024 |= 1;
  }
  if (record.unknown_0029) {
    info.unknown_0024 |= 2;
  }
  info.offsetY = static_cast<float>(record.offsetY);
  if (record.unknown_002A) {
    info.unknown_0024 |= 4;
  }
  info.unknown_0028 = static_cast<const std::string&>(FromUtf8(record.unknown_002C, g_textCodePage));
  if (record.unknown_0044) {
    info.unknown_0024 |= 8;
  }
  info.unknown_0040[0] = record.unknown_0048[0];
  info.unknown_0040[1] = record.unknown_0048[1];
  if (record.unknown_0050) {
    info.unknown_0024 |= 16;
  }

  std::string key = FromUtf8(entry.first, g_textCodePage);
  std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  this->manager->effects.insert(std::make_pair(key, info));
}

// OFFSET: 0x00700A10
// FUNCTION: fn_06988
EZ2STREffectInfo* CEZ2STREffectMgr::FindById(int id) {
  auto it = this->effects.begin();
  while (it != this->effects.end() && id != it->second.id + 10000) {
    ++it;
  }

  return it != this->effects.end() ? &it->second : nullptr;
}

// OFFSET: 0x00700A90
// FUNCTION: fn_06989
int CEZ2STREffectMgr::EffectId(const std::string& name) {
  std::string key = name;
  for (char& c : key) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }

  const auto found = this->effects.lower_bound(key);
  if (found != this->effects.end() && key.compare(found->first) >= 0) {
    return found->second.id + 10000;
  }

  return -1;
}
