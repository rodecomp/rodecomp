#include "Contents/Effects/Bash.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>

// OFFSET: 0x00B42270
// FUNCTION: fn_18869
void CBash::Close() {
  for (CParticle* particle : this->rays) {
    FreeParticle(particle);
  }

  this->rays.clear();

  for (CParticle* particle : this->sparks) {
    FreeParticle(particle);
  }

  this->sparks.clear();
}

// OFFSET: 0x00B422E0
// FUNCTION: fn_18870
void CBash::Slot01() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  RenderVertex screen;
  screen.x = 0.0f;
  screen.y = 0.0f;
  screen.z = 0.0f;
  screen.rhw = 0.0f;
  screen.diffuse = 0;
  screen.specular = 0xFF000000;
  screen.u = 0.0f;
  screen.v = 0.0f;
  g_renderer->viewport.Projection().Project(this->position, mode->camera->frame.view.values, screen);
  this->screenX = screen.x;
  this->screenY = screen.y - 40.0f;

  const int fadeFrame = this->unknown_002C - 10;
  int index = 0;
  for (int angle = 0; angle < 360; angle += 36) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
    particle->unknown_0004 = this->id;
    particle->unknown_000C = index;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
    particle->scale = 100.0f;
    particle->unknown_00C0 = 28.333334f;
    float* values = &particle->unknown_0054;
    values[0] = 28.333334f;
    values[1] = static_cast<float>(fadeFrame);
    values[2] = 170.0f;
    particle->alpha = 0;
    particle->SetTexture("effect\\alpha_down.tga");
    this->rays.push_back(particle);
    ++index;
  }

  int count = g_serviceOptions.Get(229) ? 10 : 20;
  do {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
    const int angle = rand() % 360;
    const float speed = static_cast<float>(std::max(rand() % 60, 0) + 10) / 10.0f;
    const float acceleration = -(speed / static_cast<float>(this->unknown_002C)) / 1.5f;
    const int width = rand() % 25;
    const int radius = rand() % 40;
    const int growth = rand() % 30;
    float* values = &particle->unknown_0054;
    values[0] = static_cast<float>(std::max(angle, 0));
    values[1] = speed;
    values[2] = acceleration;
    values[3] = static_cast<float>(std::max(width, 0) + 5) / 10.0f;
    values[4] = static_cast<float>(std::max(radius, 0) + 20);
    values[5] = static_cast<float>(std::max(growth, 0) + 20) / 10.0f;
    values[6] = 200.0f;
    values[7] = 20.0f;
    values[8] = static_cast<float>(this->unknown_002C - this->unknown_002C / 3);
    particle->alpha = 0;
    particle->SetTexture("EFFECT\\alpha_center.tga");
    this->sparks.push_back(particle);
  } while (--count);
}

// OFFSET: 0x00B42600
// FUNCTION: fn_18871
bool CBash::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B42720
// FUNCTION: fn_18872
void CBash::Slot03() {
  for (CParticle* particle : this->rays) {
    float* values = &particle->unknown_0054;
    const int fadeFrame = static_cast<int>(values[1]);
    const int limit = static_cast<int>(values[2]);
    float step = values[0];
    if (this->frame == fadeFrame) {
      step = static_cast<float>(-(particle->alpha / (this->unknown_002C - this->frame)));
    }

    values[0] = step;
    int alpha = static_cast<int>(static_cast<float>(particle->alpha) + step);
    if (alpha < 0.0f) {
      alpha = 0;
    }

    ++particle->unknown_00E8;
    particle->alpha = std::min(alpha, limit);
  }

  for (CParticle* particle : this->sparks) {
    float* values = &particle->unknown_0054;
    const int fadeFrame = static_cast<int>(values[8]);
    const int limit = static_cast<int>(values[6]);
    const float speed = values[2] + values[1];
    const float radius = values[5] + values[4];
    float step = values[7];
    const float angle = speed + values[0];
    if (this->frame == fadeFrame) {
      step = static_cast<float>(-(particle->alpha / (this->unknown_002C - this->frame)));
    }

    values[0] = angle;
    values[1] = speed;
    values[4] = radius;
    values[7] = step;
    int alpha = static_cast<int>(static_cast<float>(particle->alpha) + step);
    if (alpha < 0.0f) {
      alpha = 0;
    }

    ++particle->unknown_00E8;
    particle->alpha = std::min(alpha, limit);
  }
}

// OFFSET: 0x00B42C40
// FUNCTION: fn_18873
void CBash::Slot04() {
  for (CParticle* particle : this->rays) {
    const int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4));
    const float scale = particle->scale;
    const float cosine = g_cosTable[angle];
    const float sine = g_sinTable[angle];
    const float nextCosine = g_cosTable[angle + 36];
    const float nextSine = g_sinTable[angle + 36];
    const float left = static_cast<float>(particle->unknown_000C % 4) * 0.25f;
    const std::uint32_t color = ((particle->alpha << 8 | particle->red) << 8 | particle->green) << 8 | particle->blue;
    const float inner = 0.0f;

    RenderPrimitive& primitive = particle->primitive;
    RenderVertex* vertices = primitive.quad;
    vertices[0].x = scale * cosine + this->screenX;
    vertices[0].y = this->screenY - scale * sine;
    vertices[0].z = 0.000001f;
    vertices[0].rhw = 0.999999f;
    vertices[0].diffuse = color;
    vertices[0].u = left;
    vertices[0].v = 0.0f;
    vertices[1].x = scale * nextCosine + this->screenX;
    vertices[1].y = this->screenY - nextSine * scale;
    vertices[1].z = 0.000001f;
    vertices[1].rhw = 0.999999f;
    vertices[1].diffuse = color;
    vertices[1].u = left + 0.25f;
    vertices[1].v = 0.0f;
    vertices[2].x = cosine * inner + this->screenX;
    vertices[2].y = this->screenY - sine * inner;
    vertices[2].z = 0.000001f;
    vertices[2].rhw = 0.999999f;
    vertices[2].diffuse = color;
    vertices[2].u = left;
    vertices[2].v = 1.0f;
    vertices[3].x = nextCosine * inner + this->screenX;
    vertices[3].y = nextSine * inner + this->screenY;
    vertices[3].z = 0.000001f;
    vertices[3].rhw = 0.999999f;
    vertices[3].diffuse = color;
    vertices[3].u = left;
    vertices[3].v = 1.0f;
    primitive.texture = particle->texture;
    primitive.unknown_0018 = particle->sourceBlend;
    primitive.unknown_001C = particle->destinationBlend;
    g_renderer->queues.Submit(&primitive, static_cast<std::uint16_t>(particle->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }

  for (CParticle* particle : this->sparks) {
    const float* values = &particle->unknown_0054;
    const float angle = values[0];
    const float width = values[3];
    const float radius = values[4];
    const int first = static_cast<int>(angle - width);
    const float firstCosine = ClientCosine(first);
    const float firstSine = ClientSine(first);
    const int second = static_cast<int>(width + angle);
    const float secondCosine = ClientCosine(second);
    const float secondSine = ClientSine(second);
    const std::uint32_t color = ((particle->alpha << 8 | particle->red) << 8 | particle->green) << 8 | particle->blue;

    RenderPrimitive& primitive = particle->primitive;
    RenderVertex* vertices = primitive.quad;
    vertices[0].x = this->screenX;
    vertices[0].y = this->screenY;
    vertices[0].z = 0.000001f;
    vertices[0].rhw = 0.999999f;
    vertices[0].diffuse = color;
    vertices[0].u = 0.5f;
    vertices[0].v = 0.0f;
    vertices[1].x = firstCosine * radius + this->screenX;
    vertices[1].y = this->screenY - firstSine * radius;
    vertices[1].z = 0.000001f;
    vertices[1].rhw = 0.999999f;
    vertices[1].diffuse = color;
    vertices[1].u = 1.0f;
    vertices[1].v = 1.0f;
    vertices[2].x = secondCosine * radius + this->screenX;
    vertices[2].y = this->screenY - secondSine * radius;
    vertices[2].z = 0.000001f;
    vertices[2].rhw = 0.999999f;
    vertices[2].diffuse = color;
    vertices[2].u = 0.0f;
    vertices[2].v = 1.0f;
    vertices[3].x = this->screenX;
    vertices[3].y = this->screenY;
    vertices[3].z = 0.000001f;
    vertices[3].rhw = 0.999999f;
    vertices[3].diffuse = color;
    vertices[3].u = 0.5f;
    vertices[3].v = 0.0f;
    primitive.texture = particle->texture;
    primitive.unknown_0018 = particle->sourceBlend;
    primitive.unknown_001C = particle->destinationBlend;
    g_renderer->queues.Submit(&primitive, static_cast<std::uint16_t>(particle->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}
