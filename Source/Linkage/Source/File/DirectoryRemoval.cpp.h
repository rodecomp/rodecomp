#pragma once

#include <cstddef>
#include <string>
#include "File/DirectoryRemoval.h"
#include <cstring>
#include <direct.h>
#include <io.h>
#include <list>
#include <sys/stat.h>
#include <windows.h>

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string::~basic_string() noexcept;
