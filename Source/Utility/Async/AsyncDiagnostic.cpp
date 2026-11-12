#include "Utility/Async/AsyncDiagnostic.h"

#include <utility>

// OFFSET: 0x005B9CB0
// FUNCTION: fn_03438
AsyncDiagnostic::AsyncDiagnostic(const bool* success, std::string* message, const char* file, const std::uint32_t* line)
    : line(*line), file(file), message(std::move(*message)), success(*success) {
}

// OFFSET: 0x005B9D90
// FUNCTION: fn_03439
AsyncDiagnostic& AsyncDiagnostic::operator=(AsyncDiagnostic&& other) noexcept {
  this->success = other.success;
  this->message = std::move(other.message);
  this->file = std::move(other.file);

  this->line = other.line;
  return *this;
}

// OFFSET: 0x005BA220
// FUNCTION: fn_03441
AsyncDiagnostic::~AsyncDiagnostic() = default;
