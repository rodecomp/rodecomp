#include "Contents/Effects/NyanggrassEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/World/World.h"
#include "Render/AuxiliarySubmission.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"

#include <cstdlib>

// OFFSET: 0x00707D70

// INLINED
CMultiTexParticle::~CMultiTexParticle() {
  if (this->command.vertices) {
    std::free(this->command.vertices);
    this->command.vertices = nullptr;
  }
  if (this->command.indices) {
    std::free(this->command.indices);
    this->command.indices = nullptr;
  }
}

// OFFSET: 0x00707F60
// FUNCTION: fn_07066
void CMultiTexParticle::Slot01() {
}

// OFFSET: 0x00707F70
// FUNCTION: fn_07067
void CMultiTexParticle::Reset() {
  CParticle::Reset();
  this->texture1 = nullptr;
  this->texture2 = nullptr;
  this->offsetU = 0.0f;
  this->offsetV = 0.0f;
}

// OFFSET: 0x007080F0
// FUNCTION: fn_07069
void CMultiTexParticle::Render(int column, int row) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!gameMode->scene->ground) {
    return;
  }

  float corners[4][3];
  for (int i = 0; i < 4; ++i) {
    corners[i][0] = 0.0f;
    corners[i][1] = 0.0f;
    corners[i][2] = 0.0f;
  }

  if (!gameMode->scene->ground->TerrainCellCorners(*gameMode->scene->terrain, column, row, &corners[0][0])) {
    return;
  }

  this->command.texture = this->texture1;
  this->command.lightmap = this->texture2;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float* cameraMatrix = mode->camera->frame.view.values;

  for (int i = 0; i < 4; ++i) {
    LightmappedVertex& vertex = this->command.vertices[i];
    reinterpret_cast<const RenderProjection*>(g_renderer)->Project(corners[i], cameraMatrix, vertex);
    vertex.z -= 0.000030517578125f;
    vertex.diffuse = 0xFFFFFFFF;
    vertex.u = this->uv1[i][0] + this->offsetU;
    vertex.v = this->uv1[i][1] + this->offsetV;
    vertex.lightU = this->uv2[i][0];
    vertex.lightV = this->uv2[i][1];
  }

  const float depth = 0.0f;
  SubmitAuxiliaryPrimitive(*g_renderer, reinterpret_cast<LightmappedPrimitive*>(&this->command), depth, depth);
}

// OFFSET: 0x00708260
// FUNCTION: fn_07070
CNyanggrassEffect::CNyanggrassEffect() : CEffect(-1), unknown_00D8(0) {
}

// OFFSET: 0x007085B0
// FUNCTION: fn_07072
void CNyanggrassEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x007085F0

// OFFSET: 0x00708820
// FUNCTION: fn_07074
bool CNyanggrassEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00708830
// FUNCTION: fn_07075
void CNyanggrassEffect::Slot03() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      static_cast<CEZ2STRParticle*>(particle)->Process(0.0f, 0.0f, 0.0f);
    }

    ++particle->unknown_00E8;
  }
}
