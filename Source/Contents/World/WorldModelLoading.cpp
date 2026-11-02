#include "Client/Application/ApplicationMessages.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/WorldModelLoading.h"
#include "Contents/World/WorldSpatialInitialization.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/WorldResource.h"

#include "Client/Application/ErrorDialog.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundWaterState.h"
#include "Contents/Movies/BinkMovies.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Resource/World/GroundResource.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <timeapi.h>
#include <windows.h>


// GLOBAL: gl_24754
float g_worldLightDirection[3];

// GLOBAL: gl_24755
float g_worldDiffuse[3];

// GLOBAL: gl_24756
float g_worldAmbient[3];

// GLOBAL: gl_24758
float g_worldLoadCenter[3];

namespace {
  // INLINED
  bool Before(const DeferredWorldModel& left, const DeferredWorldModel& right) {
    if (left.distance < right.distance) {
      return true;
    }

    if (right.distance < left.distance) {
      return false;
    }

    if (left.distance == right.distance) {
      return reinterpret_cast<std::uintptr_t>(left.placement) < reinterpret_cast<std::uintptr_t>(right.placement);
    }

    return false;
  }

  // INLINED
  C3dActor* CreatePlacedWorldModel(const WorldModelRecord& placement, C3dModelRes& resource, BinkMovies& movies, ModelLightingContext& lighting) {
    resource.RegisterMovies(movies, placement.position);

    if (!resource.FindNode(placement.node) && resource.nodes.empty()) {
      return nullptr;
    }

    C3dActor* actor = nullptr;

    if (resource.major == 1) {
      actor = new C3dActorV1;
    } else if (resource.major == 2) {
      actor = new C3dActorV2;
    }

    actor->CopyVolumes(resource.volumes);
    actor->SetSpeed(placement.animationSpeed);
    actor->playback = placement.animation;
    std::memcpy(actor->position, placement.position, sizeof(actor->position));
    actor->dirty = 1;
    std::memcpy(actor->rotation, placement.rotation, sizeof(actor->rotation));
    actor->dirty = 1;
    std::memcpy(actor->scale, placement.scale, sizeof(actor->scale));
    actor->dirty = 1;
    actor->commandMode = placement.flag;
    actor->color = static_cast<std::uint32_t>(placement.index);
    actor->Assign(resource);
    actor->occlusionEnabled = 0;
    actor->Prepare(lighting);

    return actor;
  }

  // INLINED
  C3dActor* CreateInitWorldModel(const WorldModelRecord& placement, C3dModelRes& resource, BinkMovies& movies) {
    resource.RegisterMovies(movies, placement.position);

    if (!resource.FindNode(placement.node) && resource.nodes.empty()) {
      return nullptr;
    }

    C3dActor* actor = nullptr;

    if (resource.major == 1) {
      actor = new C3dActorV1;
    } else if (resource.major == 2) {
      actor = new C3dActorV2;
    }

    actor->CopyVolumes(resource.volumes);
    actor->SetSpeed(placement.animationSpeed);
    actor->playback = placement.animation;
    std::memcpy(actor->position, placement.position, sizeof(actor->position));
    actor->dirty = 1;
    std::memcpy(actor->rotation, placement.rotation, sizeof(actor->rotation));
    actor->dirty = 1;
    std::memcpy(actor->scale, placement.scale, sizeof(actor->scale));
    actor->dirty = 1;
    actor->commandMode = placement.flag;
    actor->color = static_cast<std::uint32_t>(placement.index);
    actor->Assign(resource);
    actor->occlusionEnabled = 0;
    void** actorVtbl = *reinterpret_cast<void***>(actor);
    reinterpret_cast<void (__thiscall *)(C3dActor*, const float*, const float*, const float*)>(actorVtbl[4])(
      actor, g_worldLightDirection, g_worldDiffuse, g_worldAmbient
    );

    return actor;
  }
}

// INLINED
void WorldModelLoading::Schedule(const WorldModelRecord& placement, const TerrainHeight& terrain, int column, int row, float* center) {
  terrain.CellCenter(column, row, center[0], center[2]);
  const float y = placement.position[1] - center[1];
  const float x = placement.position[0] - center[0];
  const float z = placement.position[2] - center[2];
  const float squared = (y * y + x * x) + z * z;
  const float distance = std::sqrt(squared);

  if (distance > 200.0f) {
    this->deferred.push_back({distance, &placement});
    return;
  }

  auto* resource = static_cast<C3dModelRes*>(this->resources.Load(placement.model));

  if (resource) {
    if (C3dActor* model = CreatePlacedWorldModel(placement, *resource, this->movies, this->lighting)) {
      this->models.push_back(model);
    }
  }
}

// OFFSET: 0x00AC8BF0
// FUNCTION: fn_17680
DWORD WINAPI WorldModelLoading::ThreadEntry(void* context) {
  static_cast<WorldModelLoading*>(context)->Run();
  return 0;
}

// INLINED
void WorldModelLoading::Start() {
  this->thread = CreateThread(nullptr, 0, &WorldModelLoading::ThreadEntry, this, 0, nullptr);

  if (!this->thread) {
    this->Run();
    this->Finish();
  }
}

// INLINED
void WorldModelLoading::Run() {
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
  std::sort(this->deferred.begin(), this->deferred.end(), Before);

  for (const DeferredWorldModel& entry : this->deferred) {
    const WorldModelRecord& placement = *entry.placement;
    auto* resource = static_cast<C3dModelRes*>(this->resources.Load(placement.model));

    if (!resource) {
      continue;
    }

    C3dActor* model = CreatePlacedWorldModel(placement, *resource, this->movies, this->lighting);

    if (!model) {
      continue;
    }

    this->models.push_back(model);
    std::atomic_ref<int>(this->pending).fetch_add(1, std::memory_order_seq_cst);
    Sleep(0);
  }
}

// INLINED
void WorldModelLoading::Publish(WorldSpatialTree& root) {
  C3dActor* const* data = this->models.data();
  PublishWorldModels(root, data, this->published, this->pending);
}

// OFFSET: 0x00ACB6C0
// FUNCTION: fn_17705
void WorldModelLoading::Finish() {
  if (this->thread) {
    WaitForSingleObject(this->thread, INFINITE);
    CloseHandle(this->thread);
    this->thread = nullptr;
  }

  this->deferred.clear();
  std::atomic_ref<int>(this->pending).store(0, std::memory_order_release);
}

// OFFSET: 0x00AC94C0
// FUNCTION: fn_17684
void CWorld::InitWorld() {
  const auto ErrorDialog = reinterpret_cast<int (*)(const char*, UINT)>(
    static_cast<int (*)(CRenderSystemBase*, const char*, UINT)>(&ShowErrorDialog)
  );

  const char* rawMapName = reinterpret_cast<const char*>(this->gameMode->SendMsg(9, 0, 0, 0, 0));
  std::string mapName(rawMapName ? rawMapName : "");

  auto* worldRes = static_cast<C3dWorldRes*>(ResourceMgr::Instance().Load(mapName.c_str()));
  if (!worldRes) {
    ErrorDialog("world\xbf\xa1 NULL\xb0\xaa\xc0\xcc \xb5\xe9\xbe\xee\xbf\xd4\xbd\xc0\xb4\xcf\xb4\xd9", 0);
    return;
  }

  this->movies = new BinkMovies();
  this->serializedSpatial = worldRes->spatial;

  int* modeBounds = reinterpret_cast<int*>(this->gameMode->unknown_0058);
  modeBounds[0] = worldRes->bounds[2];
  modeBounds[1] = worldRes->bounds[3];
  modeBounds[2] = worldRes->bounds[0];
  modeBounds[3] = worldRes->bounds[1];

  g_worldLightDirection[0] = worldRes->lightDirection[0];
  g_worldLightDirection[1] = worldRes->lightDirection[1];
  g_worldLightDirection[2] = worldRes->lightDirection[2];
  g_worldDiffuse[0] = worldRes->diffuse[0];
  g_worldDiffuse[1] = worldRes->diffuse[1];
  g_worldDiffuse[2] = worldRes->diffuse[2];
  g_worldAmbient[0] = worldRes->ambient[0];
  g_worldAmbient[1] = worldRes->ambient[1];
  g_worldAmbient[2] = worldRes->ambient[2];

  g_renderer->twoPass = worldRes->flag;
  g_renderer->SetLighting(g_worldLightDirection, g_worldDiffuse, g_worldAmbient);

  if (worldRes->terrainName.empty()) {
    ErrorDialog("attr \xc6\xc4\xc0\xcf\xc0\xcc rsw \xb3\xbb\xbf\xa1 \xc1\xf6\xc1\xa4\xb5\xc7\xbe\xee \xc0\xd6\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9", 0);
    return;
  }

  this->terrain = static_cast<C3dAttr*>(ResourceMgr::Instance().Load(worldRes->terrainName.c_str()));
  if (!this->terrain) {
    ErrorDialog("attr \xc6\xc4\xc0\xcf \xc0\xd0\xb1\xe2\xbf\xa1 \xbd\xc7\xc6\xd0", 0);
    return;
  }

  this->terrain->cellSize = 5;
  this->nearbyRange = this->terrain->cellSize * this->nearbyRange;

  if (worldRes->groundName.empty()) {
    ErrorDialog("gnd \xc6\xc4\xc0\xcf\xc0\xcc rsw \xb3\xbb\xbf\xa1 \xc1\xf6\xc1\xa4\xb5\xc7\xbe\xee \xc0\xd6\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9", 0);
    return;
  }

  auto* groundRes = static_cast<CGndRes*>(ResourceMgr::Instance().Load(worldRes->groundName.c_str()));
  if (!groundRes) {
    ErrorDialog("ground\xbf\xa1 NULL\xc0\xcc \xb5\xe9\xbe\xee\xbf\xd4\xbd\xc0\xb4\xcf\xb4\xd9.", 0);
    return;
  }

  if (groundRes->loaded == 0) {
    ErrorDialog("\xb8\xca\xc0\xcc \xbf\xbe \xb9\xf6\xc1\xaf\xc0\xd4\xb4\xcf\xb4\xd9", 0);
    return;
  }

  this->ground = new GroundGeometry();
  void** groundVtbl = *reinterpret_cast<void***>(this->ground);
  reinterpret_cast<void (__thiscall *)(GroundGeometry*, CGndRes*, const float*, const float*, const float*)>(groundVtbl[2])(
    this->ground, groundRes, g_worldLightDirection, g_worldDiffuse, g_worldAmbient
  );
  *reinterpret_cast<C3dAttr**>(reinterpret_cast<char*>(this->ground) + 4) = this->terrain;

  reinterpret_cast<GroundWaterState*>(this->ground)->Set(
    worldRes->waterLevel,
    worldRes->waterType,
    worldRes->waterAnimation,
    static_cast<int>(worldRes->wavePitch),
    static_cast<int>(worldRes->waveSpeed),
    worldRes->waveHeight
  );

  reinterpret_cast<void (__thiscall *)(GroundGeometry*, CGndRes*)>(groundVtbl[13])(this->ground, groundRes);

  ResourceMgr::Instance().Remove(groundRes);

  const std::size_t modelCount = worldRes->models.size();
  this->models.clear();
  if (this->models.capacity() < modelCount) {
    this->models.reserve(modelCount);
  }

  DWORD lastNoticeTime = 0;
  int lastPercent = -1;
  int progress = 0;

  for (const WorldModelRecord* placement : worldRes->models) {
    progress += 100;
    DWORD now = timeGetTime();
    int percent = static_cast<int>(progress / modelCount);
    if ((percent != lastPercent && now - lastNoticeTime > 15) || percent == 100) {
      lastPercent = percent;
      lastNoticeTime = now;
      g_windowMgr.SendNotice(4, percent, 0, 0, 0, 0);
      g_windowMgr.CenterSurface0478();
      g_windowMgr.DrawWindows();
      if (g_renderer->Draw()) {
        g_renderer->Present(0);
      }
      PumpApplicationMessages();
    }

    reinterpret_cast<const ::TerrainHeight*>(g_modeMgr.GetGameMode())
      ->CellCenter(g_session.m_playerX, g_session.m_playerY, g_worldLoadCenter[0], g_worldLoadCenter[2]);

    const float dx = placement->position[0] - g_worldLoadCenter[0];
    const float dy = placement->position[1] - g_worldLoadCenter[1];
    const float dz = placement->position[2] - g_worldLoadCenter[2];
    const float squared = dy * dy + dx * dx + dz * dz;
    const float distance = std::sqrt(squared);

    if (distance > 200.0f) {
      g_deferredWorldModels.push_back({distance, placement});
    } else {
      auto* modelRes = static_cast<C3dModelRes*>(ResourceMgr::Instance().Load(placement->model));
      if (modelRes) {
        if (C3dActor* actor = CreateInitWorldModel(*placement, *modelRes, *this->movies)) {
          this->models.push_back(actor);
        }
      }
    }
  }

  InitializeWorldSpatial(this->spatial, reinterpret_cast<GroundSpatial&>(*this->ground), *this->terrain, this->models, this->serializedSpatial);
  this->LoadMapEffects();

  reinterpret_cast<void (__thiscall *)(GroundGeometry*)>(groundVtbl[11])(this->ground);

  this->published = static_cast<int>(this->models.size());

  DWORD threadId = 0;
  hObject = CreateThread(nullptr, 0, &WorldModelLoading::ThreadEntry, this, 0, &threadId);
  if (!hObject) {
    WorldModelLoading::ThreadEntry(this);
    reinterpret_cast<WorldModelLoading*>(this)->Finish();
  }

  this->player = new CPlayer();
  this->objects.push_back(this->player);
  g_partyEffectNoticePending = 0;
}
