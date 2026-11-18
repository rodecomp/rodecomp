#pragma once

#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelActor.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/WorldRayQuery.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Render/Renderer.h"
#include <cstring>

template __declspec(noinline) std::list<WorldSpatialTree*>::~list() noexcept;
