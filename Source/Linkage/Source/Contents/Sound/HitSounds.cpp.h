#pragma once

#include "Client/Session/Session.h"
#include "Contents/Jobs/JobInheritance.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

template __declspec(noinline) void std::vector<std::string>::resize(size_type, const std::string&);
