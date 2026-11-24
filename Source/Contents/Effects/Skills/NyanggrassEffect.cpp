#include "Contents/Effects/Skills/NyanggrassEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/World/World.h"
#include "Render/Primitives/AuxiliarySubmission.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"

// OFFSET: 0x00707D70
// FUNCTION: fn_07062
CMultiTexParticle::CMultiTexParticle(vector3d position, int flags) : CParticle(position, flags) {
  this->Reset();
}

// OFFSET: 0x00707E80
// FUNCTION: fn_46098
__declspec(noinline) CMultiTexParticle::~CMultiTexParticle() {
}

// OFFSET: 0x00707F60
// FUNCTION: fn_07066
void CMultiTexParticle::ReleaseResource() {
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

struct MultiTexVertex : LightmappedVertex {
  MultiTexVertex();
  MultiTexVertex* Self();
};

// OFFSET: 0x00707E20
// FUNCTION: fn_07063
MultiTexVertex::MultiTexVertex() {
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->rhw = 0.0f;
  this->diffuse = 0xFFFFFFFFu;
  this->specular = 0xFFFFFFFFu;
  this->u = 0.0f;
  this->v = 0.0f;
  this->lightU = 0.0f;
  this->lightV = 0.0f;
}

// OFFSET: 0x00707E70
// FUNCTION: fn_07064
MultiTexVertex* MultiTexVertex::Self() {
  return this;
}

// INLINED
template <typename T>
static void SafeDeleteArray(T*& pointer) {
  delete[] pointer;
  pointer = nullptr;
}

// OFFSET: 0x00707FB0
// FUNCTION: fn_07068
void CMultiTexParticle::AllocateVertices(int count) {
  SafeDeleteArray(this->command.indices);
  SafeDeleteArray(this->command.vertices);
  this->command.indexCount = 0;
  this->command.vertexCount = count;
  this->command.vertices = new MultiTexVertex[count];
}

// INLINED
static void SetTexCoord(LightmappedVertex& vertex, float u, float v) {
  vertex.u = u;
  vertex.v = v;
}

// INLINED
static void SetLightCoord(LightmappedVertex& vertex, float u, float v) {
  vertex.lightU = u;
  vertex.lightV = v;
}

// OFFSET: 0x007080F0
// FUNCTION: fn_07069
void CMultiTexParticle::Render(int column, int row) {
  GroundGeometry* ground = g_modeMgr.GetGameMode()->scene->ground;
  if (!ground) {
    return;
  }

  EffectPoint corners[4];

  if (!ground->TerrainCellCorners(column, row, &corners[0].x)) {
    return;
  }

  this->command.texture = this->texture1;
  this->command.lightmap = this->texture2;

  CGameMode* mode = g_modeMgr.GetGameMode();
  const float* cameraMatrix = mode->camera->frame.view.values;

  EffectPoint* corner = corners;
  for (int i = 0; i < 4; ++i) {
    g_renderer->Project(&corner->x, cameraMatrix, this->command.vertices[i]);
    ++corner;
    this->command.vertices[i].z -= 0.000030517578125f;
    this->command.vertices[i].diffuse = 0xFFFFFFFF;
    const float u = this->uv1[i][0] + this->offsetU;
    const float v = this->uv1[i][1] + this->offsetV;
    SetTexCoord(this->command.vertices[i], u, v);
    SetLightCoord(this->command.vertices[i], this->uv2[i][0], this->uv2[i][1]);
  }

  g_renderer->SubmitAuxiliaryPrimitive(&this->command);
}

// OFFSET: 0x00708260
// FUNCTION: fn_07070
CNyanggrassEffect::CNyanggrassEffect() : CEffect(-1), gridSize(0) {
}

// OFFSET: 0x007082F0
// FUNCTION: fn_46099
__declspec(noinline) CNyanggrassEffect::~CNyanggrassEffect() {
}

// OFFSET: 0x007083D0
// FUNCTION: fn_46100
__forceinline void CNyanggrassEffect::Init() {
  for (int row = 0; row < this->gridSize; ++row) {
    for (int column = 0; column < this->gridSize; ++column) {
      CEZ2STRParticle* effect = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 1));
      if (effect) {
        effect->effectId = this->id;
        effect->columnIndex = 0;
        effect->rowIndex = 0;
        effect->groupIndex = 1;
        effect->m_loop = true;
        const vector3d origin = this->positionVector;
        const int half = this->gridSize / 2;
        const float x = static_cast<float>((column - half) * 5) + origin.x;
        const float z = static_cast<float>((row - half) * 5) + origin.z;
        effect->position.x = x;
        effect->position.y = origin.y;
        effect->position.z = z;
        effect->InitParticleQuad(effect->vertices);
        effect->Open("su_nyanggrass\\su_nyanggrass.str");
        this->particles.push_back(effect);
      }
    }
  }
}

// OFFSET: 0x007085B0
// FUNCTION: fn_07072
void CNyanggrassEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x007085F0
// FUNCTION: fn_07073
void CNyanggrassEffect::Open() {
  switch (this->effectLevel) {
  case 1:
  case 2:
    this->gridSize = 5;
    break;
  case 3:
  case 4:
    this->gridSize = 7;
    break;
  case 5:
    this->gridSize = 9;
    break;
  }

  this->Init();
}

// OFFSET: 0x00708820
// FUNCTION: fn_07074
bool CNyanggrassEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00708830
// FUNCTION: fn_07075
void CNyanggrassEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CEZ2STRParticle* particle = *it;
    this->UpdateParticle(particle);

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00708880
// FUNCTION: fn_46101
__forceinline void CNyanggrassEffect::UpdateParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}
