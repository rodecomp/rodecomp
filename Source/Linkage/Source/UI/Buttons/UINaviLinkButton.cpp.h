#pragma once

#include <cstddef>
#include <string>
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Buttons/UINaviLinkButton.h"

extern template std::string::basic_string(const std::string&);
extern template std::size_t std::_Traits_find<std::char_traits<char>>(const char* const, const std::size_t, const std::size_t, const char* const, const std::size_t) noexcept;
template __declspec(noinline) std::size_t std::_Traits_find<std::char_traits<char>>(const char*, std::size_t, std::size_t, const char*, std::size_t) noexcept;
