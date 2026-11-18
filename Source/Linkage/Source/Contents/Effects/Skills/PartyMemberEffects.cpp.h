#pragma once

#include <list>
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Skills/PartyMemberEffects.h"
#include "Client/Application/LaunchInfo.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"
#include <cstring>

class CGameObject;

extern template void std::list<CGameObject*>::push_back(CGameObject*&&);
template __declspec(noinline) std::_Tree_find_result<std::map<int, std::int64_t>::_Mybase::_Nodeptr> std::map<int, std::int64_t>::_Mybase::_Find_lower_bound<int>(const int&) const;
