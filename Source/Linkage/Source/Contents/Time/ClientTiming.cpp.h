#pragma once

#include <cstddef>
#include <string>
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session/Session.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Navigation/NavigationManager.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <timeapi.h>
#include <windows.h>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::pair<std::string, std::vector<std::string>>::pair(std::string&, std::vector<std::string>&);
template __declspec(noinline) std::_Tree_find_result<std::map<int, RobeAlpha>::_Mybase::_Nodeptr> std::map<int, RobeAlpha>::_Mybase::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) std::vector<CNaviInfo*>::size_type std::vector<CNaviInfo*>::size() const noexcept;
extern template SessionRecord6178Entry* std::vector<SessionRecord6178Entry>::_Ucopy<SessionRecord6178Entry*>(SessionRecord6178Entry*, SessionRecord6178Entry*, SessionRecord6178Entry*);
template __declspec(noinline) SessionRecord6178Entry* std::allocator<SessionRecord6178Entry>::allocate(std::size_t);
