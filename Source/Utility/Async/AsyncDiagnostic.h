#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_ASYNCDIAGNOSTIC_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_ASYNCDIAGNOSTIC_H

#include <cstdint>
#include <string>

struct AsyncDiagnostic {
  AsyncDiagnostic(const bool* success, std::string* message, const char* file, const std::uint32_t* line);
  ~AsyncDiagnostic();
  AsyncDiagnostic& MoveFrom(AsyncDiagnostic& other);

  std::uint32_t line;
  std::string file;
  std::string message;
  bool success;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AsyncDiagnostic) == 56);
#endif

#endif
