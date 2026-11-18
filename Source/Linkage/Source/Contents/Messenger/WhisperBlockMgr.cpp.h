#pragma once

#include <cstddef>
#include <string>
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Network/TextEncoding.h"
#include <new>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
