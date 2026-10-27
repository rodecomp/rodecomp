#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_ASYNCDIAGNOSTIC_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_ASYNCDIAGNOSTIC_H

#include <cstddef>
#include <cstdint>
#include <string>

struct AsyncDiagnostic {
  AsyncDiagnostic(const bool* success, std::string* message, const char* file, const std::uint32_t* line);
  ~AsyncDiagnostic();
  AsyncDiagnostic& MoveFrom(AsyncDiagnostic& other);

  /* 0000 */ std::uint32_t line;
  /* 0004 */ std::string file;
  /* 001C */ std::string message;
  /* 0034 */ bool success;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AsyncDiagnostic) == 56);
static_assert(offsetof(AsyncDiagnostic, file) == 4);
static_assert(offsetof(AsyncDiagnostic, message) == 0x001C);
static_assert(offsetof(AsyncDiagnostic, success) == 0x0034);
#endif

#endif
