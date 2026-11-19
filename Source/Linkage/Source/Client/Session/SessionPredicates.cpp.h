#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <xtree>
#include "Client/Mode/CGameMode.h"
#include "Contents/Skills/SkillId.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include <algorithm>

using SummonHpTree = std::_Tree<std::_Tmap_traits<
  std::int64_t, std::int64_t, std::less<std::int64_t>, std::allocator<std::pair<const std::int64_t, std::int64_t>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::int64_t, std::int64_t>, void*>*>
SummonHpTree::_Find_lower_bound<std::int64_t>(const std::int64_t&) const;
extern template std::string::basic_string(const std::string&);
extern template SessionRecord6178Entry* std::vector<SessionRecord6178Entry>::_Ucopy<SessionRecord6178Entry*>(SessionRecord6178Entry*, SessionRecord6178Entry*, SessionRecord6178Entry*);
template __declspec(noinline) SessionRecord6178Entry* std::allocator<SessionRecord6178Entry>::allocate(std::size_t);
template __declspec(noinline) std::vector<SessionRecord6178Entry>::~vector() noexcept;
template __declspec(noinline)
std::_Tree_find_result<std::map<std::int64_t, std::int64_t>::_Mybase::_Nodeptr>
std::map<std::int64_t, std::int64_t>::_Mybase::_Find_lower_bound<std::int64_t>(const std::int64_t&) const;
