#include "Contents/Effects/Skills/CaneOfEvilEyeEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <windows.h>
#include <timeapi.h>

#include <cmath>

namespace {
// INLINED
__forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x006F86B0
// FUNCTION: fn_06869
CCaneOfEvilEyeEffect::CCaneOfEvilEyeEffect() : CEffect(-1) {
  this->startTime = timeGetTime();
}

// OFFSET: 0x006F8800
// FUNCTION: fn_46161
__declspec(noinline) CCaneOfEvilEyeEffect::~CCaneOfEvilEyeEffect() {
}

// OFFSET: 0x006F8900
// FUNCTION: fn_06872
CEZ2STRParticle* CCaneOfEvilEyeEffect::CreateEye(int type, const float* position) {
  CEZ2STRParticle* eye = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (!eye) {
    return nullptr;
  }

  switch (type) {
  case 1:
    eye->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye\\npc_cane_of_evil_eye_start.str");
    eye->m_loop = false;
    break;
  case 2:
    eye->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye\\npc_cane_of_evil_eye.str");
    eye->m_loop = true;
    break;
  case 3:
    eye->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye\\npc_cane_of_evil_eye_end.str");
    eye->m_loop = false;
    break;
  }

  eye->offsetX = position[0];
  eye->offsetY = position[1];
  eye->offsetZ = position[2];
  eye->animationState = static_cast<signed char>(type);
  return eye;
}

// OFFSET: 0x006F8B40
// FUNCTION: fn_06873
CEZ2STRParticle* CCaneOfEvilEyeEffect::CreateLight(int type, const float* position) {
  CEZ2STRParticle* light = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (!light) {
    return nullptr;
  }

  switch (type) {
  case 1:
    light->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye_light\\npc_cane_of_evil_eye_light_03c_start.str");
    break;
  case 2:
    light->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye_light_loop\\npc_cane_of_evil_eye_light_03c_loop.str");
    break;
  case 3:
    light->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye_light\\npc_cane_of_evil_eye_light_03c_end.str");
    break;
  }

  light->originX = position[0];
  light->originY = position[1];
  light->originZ = position[2];
  light->animationState = static_cast<signed char>(type);
  return light;
}

// OFFSET: 0x006F8D40
// FUNCTION: fn_06874
void CCaneOfEvilEyeEffect::SpawnLights() {
  for (auto it = this->eyes.begin(); it != this->eyes.end(); ++it) {
    CEZ2STRParticle* eye = *it;
    if (eye) {
      float position[3];
      position[0] = eye->offsetX + 0.0f;
      position[1] = eye->offsetY + 3.5f;
      position[2] = eye->offsetZ + 0.0f;
      this->lights.push_back(this->CreateLight(1, position));
    }
  }
}

// OFFSET: 0x006F8DF0
// FUNCTION: fn_06875
void CCaneOfEvilEyeEffect::Close() {
  for (auto it = this->eyes.begin(); it != this->eyes.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->eyes.clear();

  for (auto it = this->lights.begin(); it != this->lights.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->lights.clear();
}

// OFFSET: 0x006F8E70
// FUNCTION: fn_06876
void CCaneOfEvilEyeEffect::RemoveNullEmitters() {
  for (auto it = this->eyes.begin(); it != this->eyes.end();) {
    if (*it == nullptr) {
      it = this->eyes.erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = this->lights.begin(); it != this->lights.end();) {
    if (*it == nullptr) {
      it = this->lights.erase(it);
    } else {
      ++it;
    }
  }
}

// OFFSET: 0x006F8F00
// FUNCTION: fn_06877
void CCaneOfEvilEyeEffect::Finish() {
  for (auto it = this->eyes.begin(); it != this->eyes.end(); ++it) {
    CEZ2STRParticle* eye = *it;
    if (eye) {
      *it = this->CreateEye(3, &eye->offsetX);
      FreeParticle(eye);
    }
  }

  for (auto it = this->lights.begin(); it != this->lights.end(); ++it) {
    CEZ2STRParticle* light = *it;
    if (!light) {
      continue;
    }
    const signed char state = light->animationState;
    if (state == 3) {
      continue;
    }
    *it = this->CreateLight(3, &light->originX);
    FreeParticle(light);
  }
}

// OFFSET: 0x006F9130
// FUNCTION: fn_06878
void CCaneOfEvilEyeEffect::Open() {
  this->retained = 1;

  std::vector<vector3d> offsets;
  offsets.resize(8);
  offsets[0] = {-20.0f, -12.0f, -20.0f};
  offsets[1] = {0.0f, -12.0f, -20.0f};
  offsets[2] = {20.0f, -12.0f, -20.0f};
  offsets[3] = {-20.0f, -12.0f, 0.0f};
  offsets[4] = {20.0f, -12.0f, 0.0f};
  offsets[5] = {-20.0f, -12.0f, 20.0f};
  offsets[6] = {0.0f, -12.0f, 20.0f};
  offsets[7] = {20.0f, -12.0f, 20.0f};

  for (unsigned int index = 0; index < 8; ++index) {
    this->eyes.push_back(this->CreateEye(1, &offsets[index].x));
  }

  this->SpawnLights();
}

// OFFSET: 0x006F9340
// FUNCTION: fn_06879
bool CCaneOfEvilEyeEffect::IsAlive() {
  if (this->ActorExists() && (!this->eyes.empty() || !this->lights.empty())) {
    return true;
  }

  return false;
}

// OFFSET: 0x006F9370
// FUNCTION: fn_06880
void CCaneOfEvilEyeEffect::UpdateEyes() {
  for (auto it = this->eyes.begin(); it != this->eyes.end(); ++it) {
    CEZ2STRParticle* eye = *it;
    if (IsParticleActive(eye) != true) {
      *it = eye->animationState == 1 ? this->CreateEye(2, &eye->offsetX) : nullptr;
      FreeParticle(eye);
    }
  }

  for (auto it = this->eyes.begin(); it != this->eyes.end(); ++it) {
    CEZ2STRParticle* eye = *it;
    if (eye) {
      vector3d position = {this->position[0], this->position[1], this->position[2]};
      position += eye->offset;

      const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
      const float viewX = position.y * view[3] + position.x * view[0] + position.z * view[6] + view[9];
      const float viewY = position.y * view[4] + position.x * view[1] + position.z * view[7] + view[10];
      const float viewZ = position.y * view[5] + position.x * view[2] + position.z * view[8] + view[11];

      const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
      const vector3d world = {viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9],
                              viewY * inverse[4] + viewX * inverse[1] + viewZ * inverse[7] + inverse[10],
                              viewY * inverse[5] + viewX * inverse[2] + viewZ * inverse[8] + inverse[11]};
      eye->position = world;
      eye->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x006F95D0
// FUNCTION: fn_06881
void CCaneOfEvilEyeEffect::OnProcess() {
  this->RemoveNullEmitters();
  this->UpdateEyes();

  if (this->startTime + 1000 <= timeGetTime()) {
    this->SpawnLights();
    this->startTime += 1000;
  }

  this->UpdateLights();
}

// OFFSET: 0x006F9690
// FUNCTION: fn_45459
void CCaneOfEvilEyeEffect::UpdateLights() {
  for (auto it = this->lights.begin(); it != this->lights.end(); ++it) {
    CEZ2STRParticle* light = *it;
    if (IsParticleActive(light) != true) {
      CEZ2STRParticle* next;
      if (light->animationState == 1) {
        next = this->CreateLight(3, &light->originX);
      } else if (light->animationState == 2) {
        next = this->CreateLight(3, &light->originX);
      } else {
        next = nullptr;
      }

      *it = next;
      FreeParticle(light);
    }
  }

  for (auto it = this->lights.begin(); it != this->lights.end(); ++it) {
    CEZ2STRParticle* light = *it;
    if (light) {
      vector3d from;
      from.x = this->position[0] + light->originX;
      from.y = light->originY + this->position[1];
      from.z = light->originZ + this->position[2];

      float fromReciprocalDepth = 0.0f;
      float toReciprocalDepth = 0.0f;
      float fromX;
      float fromY;
      float toX;
      float toY;
      g_renderer->ProjectViewport(&from.x, g_modeMgr.GetGameMode()->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
      g_renderer->ProjectViewport(this->position, g_modeMgr.GetGameMode()->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

      const float dx = toX - fromX;
      const float dy = toY - fromY;
      const float distance = dy * dy + dx * dx;
      double angle = static_cast<float>(std::acos(static_cast<double>(dx / SquareRoot(distance)))) / 3.141592f * 180.0f;
      if (fromY > toY) {
        angle = 360.0 - angle;
      }

      light->m_angle = static_cast<float>(angle);
      light->m_scale = SquareRoot(distance) * 0.0044f;

      vector3d position = {this->position[0], this->position[1], this->position[2]};
      position += light->offset;

      const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
      const float viewX = position.y * view[3] + position.x * view[0] + position.z * view[6] + view[9];
      const float viewY = position.y * view[4] + position.x * view[1] + position.z * view[7] + view[10];
      const float viewZ = position.y * view[5] + position.x * view[2] + position.z * view[8] + view[11];

      const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
      const vector3d world = {viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9],
                              viewY * inverse[4] + viewX * inverse[1] + viewZ * inverse[7] + inverse[10],
                              viewY * inverse[5] + viewX * inverse[2] + viewZ * inverse[8] + inverse[11]};
      light->position = world;
      light->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x006F9D40
// FUNCTION: fn_06882
void CCaneOfEvilEyeEffect::Render() {
  for (auto it = this->eyes.begin(); it != this->eyes.end(); ++it) {
    this->RenderParticle(*it);
  }

  for (auto it = this->lights.begin(); it != this->lights.end(); ++it) {
    this->RenderParticle(*it);
  }
}

// OFFSET: 0x006F9DD0
// FUNCTION: fn_46162
__forceinline void CCaneOfEvilEyeEffect::RenderParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x006F9BA0
// FUNCTION: fn_46281
void CCaneOfEvilEyeEffect::ProcessParticle(CEZ2STRParticle* particle) {
  if (particle) {
    vector3d position = {this->position[0], this->position[1], this->position[2]};
    position += particle->offset;

    const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
    const vector3d viewPos = {position.y * view[3] + position.x * view[0] + position.z * view[6] + view[9],
                              position.y * view[4] + position.x * view[1] + position.z * view[7] + view[10],
                              position.y * view[5] + position.x * view[2] + position.z * view[8] + view[11]};

    const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
    const vector3d world = {viewPos.y * inverse[3] + viewPos.x * inverse[0] + viewPos.z * inverse[6] + inverse[9],
                            viewPos.y * inverse[4] + viewPos.x * inverse[1] + viewPos.z * inverse[7] + inverse[10],
                            viewPos.y * inverse[5] + viewPos.x * inverse[2] + viewPos.z * inverse[8] + inverse[11]};
    particle->position = world;
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}
