#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_ASYNCDIAGNOSTIC_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_ASYNCDIAGNOSTIC_H

#include <cstdint>
#include <string>
#include <tuple>

using AsyncDiagnostic = std::tuple<bool, std::string, std::string, std::uint32_t>;

#endif
