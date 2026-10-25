#include "Utility/Async/AsyncDiagnostic.h"

#include <utility>

// OFFSET: 0x005B9CB0
// FUNCTION: fn_03438
AsyncDiagnostic::AsyncDiagnostic(const bool* success, std::string* message, const char* file, const std::uint32_t* line)
    : line(*line), file(file), message(std::move(*message)), success(*success) {
}

// OFFSET: 0x005B9D90
// FUNCTION: fn_03439
AsyncDiagnostic& AsyncDiagnostic::MoveFrom(AsyncDiagnostic& other) {
  this->success = other.success;
  if (&this->message != &other.message) {
    this->message = std::move(other.message);
  }

  if (&this->file != &other.file) {
    this->file = std::move(other.file);
  }

  this->line = other.line;
  return *this;
}

// OFFSET: 0x005BA220
// FUNCTION: fn_03441
AsyncDiagnostic::~AsyncDiagnostic() = default;
