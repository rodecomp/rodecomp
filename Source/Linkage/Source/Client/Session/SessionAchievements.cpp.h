#pragma once

#include <cstddef>
#include <string>
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <vector>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::string& std::string::assign(const char*, const std::size_t);
template __declspec(noinline) SessionRecord6178Entry* std::_Uninitialized_copy<SessionRecord6178Entry*, std::allocator<SessionRecord6178Entry>>(SessionRecord6178Entry*, SessionRecord6178Entry*, SessionRecord6178Entry*, std::allocator<SessionRecord6178Entry>&);
template __declspec(noinline) std::pair<std::_Tree_node<std::pair<const int, SessionRecord6144>, void*>*, bool> std::map<int, SessionRecord6144>::_Mybase::_Emplace<std::pair<int, SessionRecord6144>>(std::pair<int, SessionRecord6144>&&);
