#include "Contents/Effects/MagnumBreak.h"

#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B49E60
// FUNCTION: fn_18942
void CMagnumBreak::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* ring = *it;
    float& growth = *reinterpret_cast<float*>(&ring->unknown_0048[0x0C]);
    float& maximum = *reinterpret_cast<float*>(&ring->unknown_0048[0x10]);
    float& spin = *reinterpret_cast<float*>(&ring->unknown_0048[0x14]);
    const float angle = spin;
    float speed = growth + ring->unknown_00C0;
    speed = speed > 0.0f ? speed : 0.0f;
    float scale = speed + ring->scale;
    scale = scale < maximum ? scale : maximum;
    matrix rotation;
    rotation.MakeXRotation(1.5707964f);
    ring->scale = scale;
    ring->unknown_00C0 = speed;
    spin = angle + speed;
    ++ring->unknown_00E8;
    ring->alpha = this->alpha;
    ring->transform = rotation;
  }

  for (std::vector<CParticle*>::iterator it = this->waves.begin(); it != this->waves.end(); ++it) {
    CParticle* wave = *it;
    const float speed = *reinterpret_cast<float*>(&wave->unknown_0048[0x0C]) + wave->unknown_00C0;
    wave->unknown_00C0 = speed;
    wave->scale = speed + wave->scale;
    *reinterpret_cast<float*>(&wave->unknown_0048[0x14]) =
        *reinterpret_cast<float*>(&wave->unknown_0048[0x10]) + *reinterpret_cast<float*>(&wave->unknown_0048[0x14]);
    ++wave->unknown_00E8;
    wave->alpha = this->alpha;
  }

  float step;
  if (this->frame == this->fadeFrame) {
    step = static_cast<float>(-(this->alpha / (this->unknown_002C - this->frame)));
    this->fadeStep = step;
  } else {
    step = this->fadeStep;
  }

  const int value = static_cast<int>(static_cast<float>(this->alpha) + step);
  this->alpha = value;
  if (static_cast<float>(value) < 0.0f) {
    this->alpha = 0;
  } else {
    this->alpha = value > 255 ? 255 : value;
  }
}

// OFFSET: 0x00B4A150
// FUNCTION: fn_18943
void CMagnumBreak::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* ring = *it;
    const int angle = static_cast<int>(*reinterpret_cast<float*>(&ring->unknown_00C4));
    const float outer = *reinterpret_cast<float*>(&ring->unknown_0048[0x14]);
    const float inner = outer - ring->scale;
    const float cos0 = g_cosTable[angle];
    const float sin0 = g_sinTable[angle];
    const float cos1 = g_cosTable[angle + 36];
    const float sin1 = g_sinTable[angle + 36];
    ring->vertices[0].z = 0.0f;
    ring->vertices[1].z = 0.0f;
    ring->vertices[0].x = cos0 * inner;
    ring->vertices[2].z = 0.0f;
    ring->vertices[0].y = sin0 * inner;
    ring->vertices[3].z = 0.0f;
    ring->vertices[1].x = cos1 * inner;
    ring->vertices[1].y = sin1 * inner;
    ring->vertices[2].x = cos0 * outer;
    ring->vertices[2].y = sin0 * outer;
    ring->vertices[3].x = cos1 * outer;
    ring->vertices[3].y = sin1 * outer;
    ParticleQuad quad;
    quad.billboard = false;
    quad.top = 1.0f;
    quad.bottom = 0.0f;
    quad.left = static_cast<float>(ring->unknown_000C % 4) * 0.25f;
    quad.right = quad.left + 0.25f;
    quad.unused = false;
    quad.depth = true;
    ring->Render(quad);
  }

  for (std::vector<CParticle*>::iterator it = this->waves.begin(); it != this->waves.end(); ++it) {
    CParticle* wave = *it;
    CMagnumBreak::UpdateWave(wave);
    ParticleQuad quad;
    quad.billboard = false;
    quad.left = static_cast<float>(wave->unknown_000C / 36) * 0.1f;
    quad.top = static_cast<float>(static_cast<int>((static_cast<float>(wave->unknown_0010) + 90.0f) / 36.0f)) * 0.2f;
    quad.right = quad.left + 0.1f;
    quad.bottom = quad.top + 0.2f;
    quad.unused = false;
    quad.depth = true;
    wave->Render(quad);
  }
}

// OFFSET: 0x00B4A590
// FUNCTION: fn_18944
void CMagnumBreak::UpdateWave(CParticle* particle) {
  const int spin = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_0048[0x14]));
  const int yaw = particle->unknown_000C;
  const int pitch = particle->unknown_0010;
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
