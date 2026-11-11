#pragma once

#include "Contents/Skills/PlayerSkillBundle.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Skills/SkillId.h"
#include "Client/Session/Session.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include <algorithm>

using SkillGroupTreeValue = std::_Tree_val<std::_Tree_simple_types<std::pair<const int, std::list<CSkillInfo>>>>;
extern template std::_Tree_unchecked_const_iterator<SkillGroupTreeValue, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<SkillGroupTreeValue, std::_Iterator_base0>::operator++() noexcept;
