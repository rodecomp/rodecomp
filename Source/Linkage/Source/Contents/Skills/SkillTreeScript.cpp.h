#pragma once

#include "Client/Session/Session.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/Skills/SkillTreeScript.h"
extern "C" {
#include <lauxlib.h>
}
#include <cstdint>
#include <cstring>

template std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, std::list<CSkillInfo>>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, std::list<CSkillInfo>>>>, std::_Iterator_base0>::operator++() noexcept;
