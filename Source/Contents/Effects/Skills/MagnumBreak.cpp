#include "Contents/Effects/Skills/MagnumBreak.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B49AD0
// FUNCTION: fn_18938
void CMagnumBreak::AddRings() {
  const float growth = -(1.75f / static_cast<float>(this->effectLifetime)) * 0.5f;

  for (int index = 0; index < 10; ++index) {
    CParticle* ring = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x201);
    ring->effectId = this->id;
    ring->columnIndex = index;
    ring->rowIndex = 0;
    ring->groupIndex = 1;
    *reinterpret_cast<float*>(&ring->angleBits) = static_cast<float>(index) * 36.0f;
    ring->scale = 0.0f;
    ring->speedParameter = 1.75f;
    *reinterpret_cast<float*>(&ring->effectParameters[0x0C]) = growth;
    *reinterpret_cast<float*>(&ring->effectParameters[0x10]) = 12.0f;
    *reinterpret_cast<float*>(&ring->effectParameters[0x14]) = 0.0f;
    ring->SetTexture("effect\\ring_yellow.tga");
    this->rings.push_back(ring);
  }
}

// INLINED
void CMagnumBreak::AddWaves() {
  const float growth = -(1.15f / static_cast<float>(this->effectLifetime)) * 0.5f;

  for (int tilt = -90; tilt <= 54; tilt += 36) {
    for (int angle = 0; angle <= 324; angle += 36) {
      CParticle* wave = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
      wave->effectId = this->id;
      wave->columnIndex = angle;
      wave->rowIndex = tilt;
      wave->groupIndex = 1;
      wave->scale = 0.0f;
      wave->speedParameter = 1.15f;
      *reinterpret_cast<float*>(&wave->effectParameters[0x0C]) = growth;
      *reinterpret_cast<float*>(&wave->effectParameters[0x10]) = 3.0f;
      *reinterpret_cast<float*>(&wave->effectParameters[0x14]) = 0.0f;
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

// OFFSET: 0x00B49E60
// FUNCTION: fn_18942
void CMagnumBreak::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* ring = *it;
    float& growth = *reinterpret_cast<float*>(&ring->effectParameters[0x0C]);
    float& maximum = *reinterpret_cast<float*>(&ring->effectParameters[0x10]);
    float& spin = *reinterpret_cast<float*>(&ring->effectParameters[0x14]);
    const float angle = spin;
    float speed = growth + ring->speedParameter;
    speed = 0.0f > speed ? 0.0f : speed;
    float scale = speed + ring->scale;
    scale = maximum < scale ? maximum : scale;
    matrix rotation;
    rotation.MakeXRotation(1.570796f);
    ring->scale = scale;
    ring->speedParameter = speed;
    spin = angle + speed;
    ++ring->elapsedFrames;
    ring->alpha = this->alpha;
    ring->transform = rotation;
  }

  for (std::vector<CParticle*>::iterator it = this->waves.begin(); it != this->waves.end(); ++it) {
    CParticle* wave = *it;
    const float speed = *reinterpret_cast<float*>(&wave->effectParameters[0x0C]) + wave->speedParameter;
    wave->speedParameter = speed;
    wave->scale = speed + wave->scale;
    *reinterpret_cast<float*>(&wave->effectParameters[0x14]) =
        *reinterpret_cast<float*>(&wave->effectParameters[0x10]) + *reinterpret_cast<float*>(&wave->effectParameters[0x14]);
    ++wave->elapsedFrames;
    wave->alpha = this->alpha;
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

// OFFSET: 0x00B4A150
// FUNCTION: fn_18943
void CMagnumBreak::Render() {
  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* ring = *it;
    const int angle = static_cast<int>(*reinterpret_cast<float*>(&ring->angleBits));
    const float outer = *reinterpret_cast<float*>(&ring->effectParameters[0x14]);
    const float inner = outer - ring->scale;
    const float cos0 = g_cosTable[angle];
    const float sin0 = g_sinTable[angle];
    const float cos1 = g_cosTable[angle + 36];
    const float sin1 = g_sinTable[angle + 36];
    const vector3d innerStart = {cos0 * inner, sin0 * inner, 0.0f};
    ring->vertices[0].x = innerStart.x;
    ring->vertices[0].y = innerStart.y;
    ring->vertices[0].z = innerStart.z;
    const vector3d innerEnd = {cos1 * inner, sin1 * inner, 0.0f};
    ring->vertices[1].x = innerEnd.x;
    ring->vertices[1].y = innerEnd.y;
    ring->vertices[1].z = innerEnd.z;
    const vector3d outerStart = {cos0 * outer, sin0 * outer, 0.0f};
    ring->vertices[2].x = outerStart.x;
    ring->vertices[2].y = outerStart.y;
    ring->vertices[2].z = outerStart.z;
    const vector3d outerEnd = {cos1 * outer, sin1 * outer, 0.0f};
    ring->vertices[3].x = outerEnd.x;
    ring->vertices[3].y = outerEnd.y;
    ring->vertices[3].z = outerEnd.z;
    const float column = static_cast<float>(ring->columnIndex % 4);
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 1.0f;
    quad.bottom = 0.0f;
    quad.left = column * 0.25f;
    quad.right = quad.left + 0.25f;
    ring->Render(quad);
  }

  for (std::vector<CParticle*>::iterator it = this->waves.begin(); it != this->waves.end(); ++it) {
    CParticle* wave = *it;
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
}

// OFFSET: 0x00B4A590
// FUNCTION: fn_18944
void CMagnumBreak::UpdateWave(CParticle* particle) {
  const int spin = static_cast<int>(*reinterpret_cast<float*>(&particle->effectParameters[0x14]));
  const int yaw = particle->columnIndex;
  const int pitch = particle->rowIndex;
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

  particle->vertices[0].x = first.values[3] * 0.0f + first.values[0] * 0.0f + first.values[6] * scale + first.values[9];
  particle->vertices[0].y = first.values[4] * 0.0f + first.values[1] * 0.0f + first.values[7] * scale + first.values[10];
  particle->vertices[0].z = first.values[5] * 0.0f + first.values[2] * 0.0f + first.values[8] * scale + first.values[11];
  particle->vertices[2].x = second.values[3] * 0.0f + second.values[0] * 0.0f + second.values[6] * scale + second.values[9];
  particle->vertices[2].y = second.values[4] * 0.0f + second.values[1] * 0.0f + second.values[7] * scale + second.values[10];
  particle->vertices[2].z = second.values[5] * 0.0f + second.values[2] * 0.0f + second.values[8] * scale + second.values[11];
  particle->vertices[1].x = third.values[3] * 0.0f + third.values[0] * 0.0f + third.values[6] * scale + third.values[9];
  particle->vertices[1].y = third.values[4] * 0.0f + third.values[1] * 0.0f + third.values[7] * scale + third.values[10];
  particle->vertices[1].z = third.values[5] * 0.0f + third.values[2] * 0.0f + third.values[8] * scale + third.values[11];
  particle->vertices[3].x = fourth.values[3] * 0.0f + fourth.values[0] * 0.0f + fourth.values[6] * scale + fourth.values[9];
  particle->vertices[3].y = fourth.values[4] * 0.0f + fourth.values[1] * 0.0f + fourth.values[7] * scale + fourth.values[10];
  particle->vertices[3].z = fourth.values[5] * 0.0f + fourth.values[2] * 0.0f + fourth.values[8] * scale + fourth.values[11];
}
