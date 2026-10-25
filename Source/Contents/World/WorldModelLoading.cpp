#include "Contents/Models/ModelActor.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/WorldModelLoading.h"
#include "Contents/World/WorldModelPlacement.h"
#include "Contents/World/WorldSpatialInitialization.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/WorldResource.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>

namespace {
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
}

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

DWORD WINAPI WorldModelLoading::ThreadEntry(void* context) {
  static_cast<WorldModelLoading*>(context)->Run();
  return 0;
}

void WorldModelLoading::Start() {
  this->thread = CreateThread(nullptr, 0, &WorldModelLoading::ThreadEntry, this, 0, nullptr);

  if (!this->thread) {
    this->Run();
    this->Finish();
  }
}

// OFFSET: 0x00AC8BF0
// FUNCTION: fn_17680
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
