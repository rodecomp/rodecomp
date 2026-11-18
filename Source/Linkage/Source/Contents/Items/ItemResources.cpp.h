#pragma once

#include <string>
#include "Client/Session/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemResources.h"
#include "Resource/ResourceMgr.h"
#include <cstdlib>

extern template std::string std::operator+(const std::string&, const char* const);
template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
