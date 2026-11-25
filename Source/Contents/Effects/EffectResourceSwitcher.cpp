#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Render/Primitives/RenderQueues.h"
#include "Resource/World/TerrainResource.h"
#include "Resource/Json/JsonCppLoader.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <new>
#include <string>
#include <vector>

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
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName));
  }

  if (!this->targetName.empty()) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->targetName));
  }
}

// OFFSET: 0x005A2110
// FUNCTION: fn_03083
void CEffect::SetDestinationBlend(int) {
}

// OFFSET: 0x005A2120
// FUNCTION: fn_03084
void CEffect::SetColor(short, short, short) {
}

// OFFSET: 0x005A2130
// FUNCTION: fn_03085
void CEffect::SetParticleTexture(std::string) {
}

// OFFSET: 0x005A2140
// FUNCTION: fn_03086
void CEffect::SetSourceBlend(int) {
}

// OFFSET: 0x005A2160
// FUNCTION: fn_03088
void CEffect::EnableTimeLimit() {
}

// OFFSET: 0x005A2170
// FUNCTION: fn_03089
vector3d CEffect::GetWindowOffset() {
  return vector3d{0.0f, 0.0f, 0.0f};
}

// OFFSET: 0x005A2190
// FUNCTION: fn_03090
int CEffect::GetFrame() {
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
void CEffect::Open() {
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

  EffectLists* effects = g_effectMgr;
  this->effect = static_cast<CEZ2STREffectEx*>(
      effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0));
  this->spawnPosition = actor->render.position;
}

// OFFSET: 0x005A22E0
// FUNCTION: fn_03094
void CEffect::InitEvents() {
}

// OFFSET: 0x005A22F0
// FUNCTION: fn_03095
void CEffect::ResetParticles() {
}

// OFFSET: 0x005A2300
// FUNCTION: fn_03096
bool EffectResourceSwitcher::IsAlive() {
  if (!this->effect) {
    return false;
  }

  if (this->effect->retained) {
    return true;
  }

  return !this->effect->IsFinished();
}

// OFFSET: 0x005A2330
// FUNCTION: fn_03097
void EffectResourceSwitcher::OnProcess() {
  if (!this->effect || !this->active || !this->effect->IsFinished()) {
    return;
  }

  CEZ2STREffectEx* previous = this->effect;
  this->sourceName = this->targetName;
  this->targetName = "";
  EffectLists* effectMgr = g_effectMgr;
  this->effect = static_cast<CEZ2STREffectEx*>(effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName), this->spawnPosition, 0, -1, 0, 0));
  this->active = 0;
  if (previous) {
    previous->EndLoop(0);
  }
}

// OFFSET: 0x005A2430
// FUNCTION: fn_03098
void CEffect::Render() {
}

// OFFSET: 0x005A2440
// FUNCTION: fn_03099
void CEffect::SetRenderFunc(ParticleSubmit) {
}

// OFFSET: 0x005A2450
// FUNCTION: fn_03100
void CEffect::SetAlpha(float) {
}

// OFFSET: 0x005A2460
// FUNCTION: fn_03101
void CEffect::SetWindowOffset(vector3d, int, int) {
}

// OFFSET: 0x005A2470
// FUNCTION: fn_03102
void CEffect::SetFlipX(bool) {
}

// OFFSET: 0x005A2480
// FUNCTION: fn_03103
void CEffect::SetFlipY(bool) {
}

// OFFSET: 0x005A2490
// FUNCTION: fn_03104
void CEffect::SetFlags(int) {
}

// OFFSET: 0x005A24A0
// FUNCTION: fn_03105
void CEffect::EnableGroundRender() {
}

// OFFSET: 0x005A24B0
// FUNCTION: fn_03106
void CEffect::SetRepeat(int) {
}

// OFFSET: 0x005A24C0
// FUNCTION: fn_03107
void CEffect::SetLifeTime(int) {
}

// OFFSET: 0x005A24D0
// FUNCTION: fn_03108
void CEffect::SetAngle(float) {
}

// OFFSET: 0x005A24E0
// FUNCTION: fn_03109
void CEffect::SetPitch(float) {
}

// OFFSET: 0x005A24F0
// FUNCTION: fn_03110
void CEffect::SetYaw(float) {
}

// OFFSET: 0x005A2500
// FUNCTION: fn_03111
void CEffect::SetScale(float) {
}

// OFFSET: 0x005A2510
// FUNCTION: fn_03112
void CEffect::AdvanceFrame(float) {
}

// OFFSET: 0x005A2520
// FUNCTION: fn_03113
void EffectResourceSwitcher::SetDelay(int argument) {
  if (this->effect) {
    this->effect->SetDelay(argument);
  }
}

// OFFSET: 0x005A2540
// FUNCTION: fn_03114
void CEffect::SetResource(int) {
}

// OFFSET: 0x006FFA20
// FUNCTION: fn_06968
void CEZ2STREffectEx::Open() {
  CEZ2STREffectMgr* manager = CEZ2STREffectMgr::s_instance;
  if (!manager) {
    manager = new (std::nothrow) CEZ2STREffectMgr;
    CEZ2STREffectMgr::s_instance = manager;
  }

  EZ2STREffectInfo* info = manager->FindById(this->id);
  if (!info) {
    return;
  }

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (!particle) {
    return;
  }

  particle->InitParticleQuad(particle->vertices);
  particle->m_actorId = this->actor;
  particle->Load(info->name);

  CEZ2STRParticle* emitter = static_cast<CEZ2STRParticle*>(particle);
  const vector3d offset = {info->offsetX, info->offsetY, 0.0f};
  emitter->offset = offset;
  if (info->flags & 1) {
    this->retained = 1;
    emitter->m_loop = 1;
  }

  emitter->m_delay = info->interval;
  this->frameStep = info->interval / 16;
  if (!info->soundPath.empty()) {
    this->AddEvent(EFFECT_EVENT_SOUND, info->soundPath.c_str(), this->frameStep + 1, 0);
  }

  if (info->delay >= 0) {
    this->AddEvent(EFFECT_EVENT_QUAKE, nullptr, info->delay, 0);
  }

  if (info->flags & 2) {
    particle->flags = RSF_ALPHA | RSF_CUSTOM_BLEND | RSF_TOPMOST;
  }

  if (info->flags & 4) {
    particle->flags = RSF_ALPHA | RSF_CUSTOM_BLEND | RSF_NO_DEPTH;
  }

  emitter->m_repeatFrame = (info->flags >> 3) & 1;
  emitter->m_followGround = (info->flags >> 4) & 1;
  this->emitter = emitter;
}

// OFFSET: 0x006FFBF0
// FUNCTION: fn_06969
bool CEZ2STREffectEx::IsAlive() {
  CEZ2STRParticle* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (this->timed == 1) {
    return emitter->m_alpha > 0.0f;
  }

  if (emitter->m_loop == 1) {
    return this->ActorExists();
  }

  if (0.0f == emitter->life) {
    return true;
  }

  return static_cast<float>(emitter->elapsedFrames) < emitter->life * 60.0f;
}

// OFFSET: 0x006FFC60
// FUNCTION: fn_06970
void CEZ2STREffectEx::OnProcess() {
  if (!this->emitter) {
    return;
  }

  if (this->timed == 1) {
    this->emitter->m_alpha -= 0.05f;
  }

  if (this->emitter->m_followGround) {
    vector3d position = this->positionVector;
    CGameMode* mode = g_modeMgr.GetGameMode();
    position.y = mode->scene->terrain->Height(position.x, position.z);
    this->positionVector = position;
  }

  if (CEZ2STRParticle* emitter = this->emitter) {
    const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
    const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;

    const vector3d position = this->positionVector;
    const float viewZ = position.x * view.values[2] + position.y * view.values[5] + position.z * view.values[8] + view.values[11];
    const float viewX = position.x * view.values[0] + position.y * view.values[3] + position.z * view.values[6] + view.values[9] + emitter->offsetX;
    const float viewY = position.x * view.values[1] + position.y * view.values[4] + position.z * view.values[7] + view.values[10] - emitter->offsetY;

    vector3d world;
    world.x = viewY * inverse.values[3];
    world.x += viewX * inverse.values[0];
    world.x += viewZ * inverse.values[6];
    world.x += inverse.values[9];
    world.y = viewX * inverse.values[1];
    world.y += viewY * inverse.values[4];
    world.y += viewZ * inverse.values[7];
    world.y += inverse.values[10];
    world.z = viewX * inverse.values[2];
    world.z += viewY * inverse.values[5];
    world.z += viewZ * inverse.values[8];
    world.z += inverse.values[11];
    emitter->position = world;
    static_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);
  }

  if (this->emitter->m_repeatFrame == 1 && this->emitter->m_frame == 1) {
    this->frame = this->frameStep;
  }

  ++this->emitter->elapsedFrames;
}

// OFFSET: 0x006FFEA0
// FUNCTION: fn_06971
void CEZ2STREffectEx::Render() {
  if (!this->emitter) {
    return;
  }

  if (this->alternateRender) {
    static_cast<CEZ2STRParticle*>(this->emitter)->DrawGround(g_modeMgr.GetGameMode()->camera->frame.view);
  } else {
    static_cast<CEZ2STRParticle*>(this->emitter)->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x006FFEF0
// FUNCTION: fn_06972
void CEZ2STREffectEx::SetAlpha(float argument) {
  if (this->emitter) {
    this->emitter->m_alpha = argument;
  }
}

// OFFSET: 0x006FFF10
// FUNCTION: fn_06973
void CEZ2STREffectEx::SetFlipX(bool argument) {
  if (this->emitter) {
    this->emitter->m_flipX = argument;
  }
}

// OFFSET: 0x006FFF30
// FUNCTION: fn_06974
void CEZ2STREffectEx::SetFlipY(bool argument) {
  if (this->emitter) {
    this->emitter->m_flipY = argument;
  }
}

// OFFSET: 0x006FFF50
// FUNCTION: fn_06975
__declspec(noinline) void CEZ2STREffectEx::EndLoop(unsigned char fadeOut) {
  if (this->emitter) {
    this->emitter->m_loop = 0;
    this->emitter->m_hold = fadeOut;
  }
}

// OFFSET: 0x006FFF80
// FUNCTION: fn_06976
void CEZ2STREffectEx::EnableGroundRender() {
  this->alternateRender = 1;
}

// OFFSET: 0x006FFF90
// FUNCTION: fn_06977
void CEZ2STREffectEx::SetLifeTime(int argument) {
  if (this->emitter) {
    this->emitter->m_lifeTime = argument;
  }
}

// OFFSET: 0x006FFFB0
// FUNCTION: fn_06978
void CEZ2STREffectEx::SetAngle(float argument) {
  if (this->emitter) {
    this->emitter->m_angle = argument;
  }
}

// OFFSET: 0x006FFFD0
// FUNCTION: fn_06979
void CEZ2STREffectEx::SetPitch(float argument) {
  if (this->emitter) {
    this->emitter->m_pitch = argument;
  }
}

// OFFSET: 0x006FFFF0
// FUNCTION: fn_06980
void CEZ2STREffectEx::SetYaw(float argument) {
  if (this->emitter) {
    this->emitter->m_yaw = argument;
  }
}

// OFFSET: 0x00700010
// FUNCTION: fn_06981
void CEZ2STREffectEx::SetScale(float argument) {
  if (this->emitter) {
    this->emitter->m_scale = argument;
  }
}

// OFFSET: 0x00700030
// FUNCTION: fn_06982
void CEZ2STREffectEx::SetDelay(int argument) {
  if (this->emitter) {
    this->emitter->m_delay = argument;
  }

  this->frameStep = argument / 16;
  this->SetEventFrame(EFFECT_EVENT_SOUND, this->frameStep + 1);
}

// INLINED
static void SetOffset(EZ2STREffectInfo& info, float x, float y) {
  info.offsetX = x;
  info.offsetY = y;
}

// OFFSET: 0x00700070
// FUNCTION: fn_06983
void CEZ2STREffectMgr::InfoInserter::operator()(const std::pair<const std::string, EZ2STREffectRecord>& entry) const {
  if (entry.second.name.empty()) {
    return;
  }

  EZ2STREffectInfo info;
  info.id = static_cast<int>(this->manager->effects.size());
  info.name = static_cast<const std::string&>(FromUtf8(entry.second.name, g_textCodePage));
  SetOffset(info, static_cast<float>(entry.second.offsetX), static_cast<float>(entry.second.offsetY));
  info.flags = entry.second.infinite ? info.flags | 1 : info.flags;
  info.flags = entry.second.floor ? info.flags | 2 : info.flags;
  info.flags = entry.second.aboveUi ? info.flags | 4 : info.flags;
  info.soundPath = static_cast<const std::string&>(FromUtf8(entry.second.soundPath, g_textCodePage));
  info.flags = entry.second.soundLoop ? info.flags | 8 : info.flags;
  info.delay = entry.second.quakeDelay;
  info.interval = entry.second.startDelayTime;
  info.flags = entry.second.toGround ? info.flags | 16 : info.flags;

  std::string key = FromUtf8(entry.first, g_textCodePage);
  std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  this->manager->effects.insert(std::pair<const std::string, EZ2STREffectInfo>(key, info));
}

// OFFSET: 0x00700A10
// FUNCTION: fn_06988
EZ2STREffectInfo* CEZ2STREffectMgr::FindById(int id) {
  auto it = this->effects.cbegin();
  const auto end = this->effects.cend();
  while (it != end && id != it->second.id + 10000) {
    ++it;
  }

  return it != end ? const_cast<EZ2STREffectInfo*>(&it->second) : nullptr;
}

// OFFSET: 0x00700A90
// FUNCTION: fn_06989
int CEZ2STREffectMgr::EffectId(const std::string& name) {
  std::string key = name;
  std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

  const auto found = this->effects.find(key);
  if (found != this->effects.end()) {
    return found->second.id + 10000;
  }

  return -1;
}

// OFFSET: 0x00700BA0
// FUNCTION: fn_06990
std::vector<int> CEZ2STREffectMgr::EffectIds() {
  std::vector<int> ids;
  const auto end = this->effects.end();
  for (auto it = this->effects.begin(); it != end; it++) {
    ids.push_back(it->second.id + 10000);
  }
  return ids;
}

// OFFSET: 0x00700E30
// FUNCTION: fn_06991
bool CEZ2STREffectMgr::Load() {
  CFile file;
  if (!file.Open("ContentData\\EffectData\\EZ2STREffect.bson", 0, true, false)) {
    return false;
  }

  JsonCppLoader loader;
  bool failed;
  {
    const auto* data = static_cast<const unsigned char*>(file.GetData());
    std::vector<unsigned char> buffer(data, data + file.GetSize());
    failed = !loader.Load(buffer, true);
  }
  if (failed) {
    return false;
  }

  std::map<std::string, EZ2STREffectRecord> records;
  if (!loader.GetEZ2STREffect(&records)) {
    return false;
  }

  this->effects.clear();
  std::for_each(records.begin(), records.end(), InfoInserter{this});
  return true;
}

