#pragma once

#include "Contents/Actors/ActorEnums.h"
#include "Contents/Skills/SkillId.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/CombatTypes.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"
#include <algorithm>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

template __declspec(noinline) std::vector<int>::~vector() noexcept;
