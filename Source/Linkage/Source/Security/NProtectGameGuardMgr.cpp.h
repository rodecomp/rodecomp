#pragma once

#include <string>
#include <vector>

#include "Security/NProtectGameGuardMgr.h"

extern template std::string::basic_string(const std::string&);
// OFFSET: 0x0076C320
// FUNCTION: fn_08202
template __declspec(noinline) std::vector<CNProtectGameGuardMgr::lcCSAuthToken>::~vector() noexcept;
