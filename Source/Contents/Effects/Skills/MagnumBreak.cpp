#include "Contents/Effects/Skills/MagnumBreak.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B49910
// FUNCTION: fn_45967
CMagnumBreak::CMagnumBreak() : CEffect(EF_MAGNUMBREAK), fadeStep(16.933333f), fadeFrame(15), alpha(0) {
  this->effectLifetime = 30;
}

// OFFSET: 0x00B499D0
// FUNCTION: fn_45968
__declspec(noinline) CMagnumBreak::~CMagnumBreak() {
}

// OFFSET: 0x00B49AD0
// FUNCTION: fn_18938
void CMagnumBreak::AddRings() {
  const float growth = -(1.75f / static_cast<float>(this->effectLifetime)) * 0.5f;

  for (int index = 0; index < 10; ++index) {
    CParticle* ring = CreateParticle(&this->positionVector, 0x201);
    ring->effectId = this->id;
    ring->columnIndex = index;
    ring->rowIndex = 0;
    ring->groupIndex = 1;
    ring->angle = static_cast<float>(index) * 36.0f;
    ring->scale = 0.0f;
    ring->speedParameter = 1.75f;
    ring->animationParameter = growth;
    ring->animationParameters[0] = 12.0f;
    ring->animationParameters[1] = 0.0f;
    ring->SetTexture("effect\\ring_yellow.tga");
    this->rings.push_back(ring);
  }
}

// OFFSET: 0x00B49BD0
// FUNCTION: fn_45969
__forceinline void CMagnumBreak::AddWaves() {
  const float growth = -(1.15f / static_cast<float>(this->effectLifetime)) * 0.5f;

  for (int tilt = -90; tilt <= 54; tilt += 36) {
    for (int angle = 0; angle <= 324; angle += 36) {
      CParticle* wave = CreateParticle(&this->positionVector, 1);
      wave->effectId = this->id;
      wave->columnIndex = angle;
      wave->rowIndex = tilt;
      wave->groupIndex = 1;
      wave->scale = 0.0f;
      wave->speedParameter = 1.15f;
      wave->animationParameter = growth;
      wave->animationParameters[0] = 3.0f;
      wave->animationParameters[1] = 0.0f;
      wave->SetTexture("effect\\대폭발.tga");
      this->waves.push_back(wave);
    }
  }
}

// OFFSET: 0x00B49CD0
// FUNCTION: fn_18939
void CMagnumBreak::Close() {
  for (auto it = this->rings.begin(); it != this->rings.end(); ++it) {
    FreeParticle(*it);
  }
  this->rings.clear();

  for (auto it = this->waves.begin(); it != this->waves.end(); ++it) {
    FreeParticle(*it);
  }
  this->waves.clear();
}

// OFFSET: 0x00B49D40
// FUNCTION: fn_18940
void CMagnumBreak::Open() {
  this->AddRings();
  this->AddRings();
  this->AddWaves();
}

// OFFSET: 0x00B49E50
// FUNCTION: fn_18941
bool CMagnumBreak::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B4A030
// FUNCTION: fn_45970
__forceinline void CMagnumBreak::AdvanceRing(CParticle* ring) {
  const float angle = ring->animationParameters[1];
  float speed = ring->animationParameter + ring->speedParameter;
  speed = 0.0f > speed ? 0.0f : speed;
  float scale = speed + ring->scale;
  scale = ring->animationParameters[0] < scale ? ring->animationParameters[0] : scale;
  matrix rotation;
  rotation.MakeXRotation(1.570796f);
  ring->scale = scale;
  ring->speedParameter = speed;
  ring->animationParameters[1] = angle + speed;
  ring->alpha = this->alpha;
  ring->transform = rotation;
}

// OFFSET: 0x00B4A100
// FUNCTION: fn_45971
void CMagnumBreak::AdvanceWave(CParticle* wave) {
  const float speed = wave->animationParameter + wave->speedParameter;
  wave->speedParameter = speed;
  wave->scale = speed + wave->scale;
  wave->animationParameters[1] =
      wave->animationParameters[0] + wave->animationParameters[1];
  wave->alpha = this->alpha;
}

// OFFSET: 0x00B4A380
// FUNCTION: fn_45972
void CMagnumBreak::RenderRing(CParticle* ring) {
  const int angle = static_cast<int>(ring->angle);
  const float outer = ring->animationParameters[1];
  const float inner = outer - ring->scale;
  const float cos0 = g_cosTable[angle];
  const float sin0 = g_sinTable[angle];
  const float cos1 = g_cosTable[angle + 36];
  const float sin1 = g_sinTable[angle + 36];
  RenderVertex* vertices = ring->vertices;
  const vector3d innerStart = {cos0 * inner, sin0 * inner, 0.0f};
  vertices[0].x = innerStart.x;
  vertices[0].y = innerStart.y;
  vertices[0].z = innerStart.z;
  const vector3d innerEnd = {cos1 * inner, sin1 * inner, 0.0f};
  vertices[1].x = innerEnd.x;
  vertices[1].y = innerEnd.y;
  vertices[1].z = innerEnd.z;
  const vector3d outerStart = {cos0 * outer, sin0 * outer, 0.0f};
  vertices[2].x = outerStart.x;
  vertices[2].y = outerStart.y;
  vertices[2].z = outerStart.z;
  const vector3d outerEnd = {cos1 * outer, sin1 * outer, 0.0f};
  vertices[3].x = outerEnd.x;
  vertices[3].y = outerEnd.y;
  vertices[3].z = outerEnd.z;
  const float left = static_cast<float>(ring->columnIndex % 4) * 0.25f;
  ParticleQuad quad;
  quad.billboard = false;
  quad.unused = false;
  quad.depth = true;
  quad.top = 1.0f;
  quad.bottom = 0.0f;
  quad.left = left;
  quad.right = left + 0.25f;
  ring->Render(quad);
}

// OFFSET: 0x00B49E60
// FUNCTION: fn_18942
void CMagnumBreak::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* ring = *it;
    this->AdvanceRing(ring);
    ++ring->elapsedFrames;
  }

  for (std::vector<CParticle*>::iterator it = this->waves.begin(); it != this->waves.end(); ++it) {
    CParticle* wave = *it;
    this->AdvanceWave(wave);
    ++wave->elapsedFrames;
  }

  float step;
  if (this->frame == this->fadeFrame) {
    step = static_cast<float>(-(this->alpha / (this->effectLifetime - this->frame)));
    this->fadeStep = step;
  } else {
    step = this->fadeStep;
  }

  this->alpha = static_cast<int>(static_cast<float>(this->alpha) + step);
  if (static_cast<float>(this->alpha) < 0.0f) {
    this->alpha = 0;
  } else if (this->alpha > 255) {
    this->alpha = 255;
  }
}

// OFFSET: 0x00B4A4D0
// FUNCTION: fn_45973
__forceinline void CMagnumBreak::RenderWave(CParticle* wave) {
  CMagnumBreak::UpdateWave(wave);
  const float column = static_cast<float>(wave->columnIndex / 36);
  const float row = static_cast<float>(static_cast<int>((static_cast<float>(wave->rowIndex) + 90.0f) / 36.0f));
  ParticleQuad quad;
  quad.billboard = false;
  quad.unused = false;
  quad.depth = true;
  quad.left = column * 0.1f;
  quad.top = row * 0.2f;
  quad.right = quad.left + 0.1f;
  quad.bottom = quad.top + 0.2f;
  wave->Render(quad);
}

// OFFSET: 0x00B4A150
// FUNCTION: fn_18943
void CMagnumBreak::Render() {
  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* ring = *it;
    this->RenderRing(ring);
  }

  for (std::vector<CParticle*>::iterator it = this->waves.begin(); it != this->waves.end(); ++it) {
    CParticle* wave = *it;
    this->RenderWave(wave);
  }
}

// INLINED
static void MatrixMult(vector3d& result, const vector3d& v, const float* m) {
  result.x = v.x * m[0] + v.y * m[3] + v.z * m[6] + m[9];
  result.y = v.x * m[1] + v.y * m[4] + v.z * m[7] + m[10];
  result.z = v.x * m[2] + v.y * m[5] + v.z * m[8] + m[11];
}

// OFFSET: 0x00B4A590
// FUNCTION: fn_18944
void CMagnumBreak::UpdateWave(CParticle* particle) {
  const int yaw = particle->columnIndex;
  const int pitch = particle->rowIndex;
  const int spin = static_cast<int>(particle->animationParameters[1]);
  const float yaw0 = GetRadian(spin + yaw);
  const float pitch0 = GetRadian(pitch);
  const float yaw1 = GetRadian(static_cast<int>(static_cast<float>(yaw + 36) + 0.25f + static_cast<float>(spin)));
  const float pitch1 = GetRadian(pitch + 36);
  const float scale = particle->scale;

  matrix first;
  first.MakeXRotation(pitch0);
  first.AppendYRotation(yaw0);
  matrix second;
  second.MakeXRotation(pitch1);
  second.AppendYRotation(yaw0);
  matrix third;
  third.MakeXRotation(pitch0);
  third.AppendYRotation(yaw1);
  matrix fourth;
  fourth.MakeXRotation(pitch1);
  fourth.AppendYRotation(yaw1);

  const vector3d offset{0.0f, 0.0f, scale};
  vector3d corner0;
  vector3d corner2;
  vector3d corner1;
  vector3d corner3;
  MatrixMult(corner0, offset, first.values);
  MatrixMult(corner2, offset, second.values);
  MatrixMult(corner1, offset, third.values);
  MatrixMult(corner3, offset, fourth.values);
  particle->vertices[0].x = corner0.x;
  particle->vertices[0].y = corner0.y;
  particle->vertices[0].z = corner0.z;
  particle->vertices[2].x = corner2.x;
  particle->vertices[2].y = corner2.y;
  particle->vertices[2].z = corner2.z;
  particle->vertices[1].x = corner1.x;
  particle->vertices[1].y = corner1.y;
  particle->vertices[1].z = corner1.z;
  particle->vertices[3].x = corner3.x;
  particle->vertices[3].y = corner3.y;
  particle->vertices[3].z = corner3.z;
}
