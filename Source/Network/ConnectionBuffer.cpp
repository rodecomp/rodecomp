#include "Network/ConnectionBuffer.h"

#include <algorithm>
#include <bit>
#include <cstring>

namespace {
  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }
  int Subtract(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) - static_cast<unsigned>(right));
  }
}

// OFFSET: 0x00C7EF60
// FUNCTION: fn_20418
ConnectionBuffer::ConnectionBuffer() : m_data(nullptr) {
}
// OFFSET: 0x00C7EFC0
// FUNCTION: fn_20419
ConnectionBuffer::~ConnectionBuffer() = default;

// OFFSET: 0x00C7F060
// FUNCTION: fn_20422
void ConnectionBuffer::Reset(unsigned bytes) {
  this->m_read = 0;
  this->m_write = 0;
  this->m_storage.resize(bytes);
  this->m_data = this->m_storage.empty() ? nullptr : this->m_storage.data();
}

// OFFSET: 0x00C7F050
// FUNCTION: fn_20421
int ConnectionBuffer::Available() const {
  return Subtract(this->m_write, this->m_read);
}

// OFFSET: 0x00C7F0E0
// FUNCTION: fn_20423
void ConnectionBuffer::Append(int bytes, const void* source) {
  const int size = static_cast<int>(this->m_storage.size());
  if (Add(this->m_write, bytes) > size) {
    const int required = Add(this->Available(), bytes);
    if (required > size) {
      // Original vector resize, not reserve: newly appended storage is zeroed.
      this->m_storage.resize(static_cast<unsigned>(required) * 2u);
      this->m_data = this->m_storage.data();
    }
    // Both original thunks import vcruntime140 memcpy. Its x86 implementation
    // supports this overlapping left shift; use defined equivalent host code.
    std::memmove(this->m_data, this->m_data + this->m_read, this->Available());
    this->m_write = this->Available();
    this->m_read = 0;
  }
  std::memcpy(this->m_data + this->m_write, source, bytes);
  this->m_write = Add(this->m_write, bytes);
}

// OFFSET: 0x00C7F160
// FUNCTION: fn_20424
void ConnectionBuffer::Prepend(int bytes, const void* source) {
  const int available = this->Available();
  const int required = Add(bytes, available);
  std::vector<char> storage(static_cast<unsigned>(std::max(required, static_cast<int>(this->m_storage.size()))));
  std::memcpy(storage.data(), source, bytes);
  std::memcpy(storage.data() + bytes, this->m_data + this->m_read, available);
  this->m_write = required;
  this->m_read = 0;
  this->m_storage.swap(storage);
  this->m_data = this->m_storage.data();
}

// OFFSET: 0x00C7F010
// FUNCTION: fn_20420
bool ConnectionBuffer::Read(int bytes, void* destination) {
  if (this->Available() < bytes) {
    return false;
  }
  std::memcpy(destination, this->m_data + this->m_read, bytes);
  this->m_read = Add(this->m_read, bytes);
  return true;
}

// OFFSET: 0x00C7F2B0
// FUNCTION: fn_20425
bool ConnectionBuffer::Peek(int bytes, void* destination) const {
  if (this->Available() < bytes) {
    return false;
  }
  std::memcpy(destination, this->m_data + this->m_read, bytes);
  return true;
}

// OFFSET: 0x00C7F2E0
// FUNCTION: fn_20426
bool ConnectionBuffer::Consume(int bytes) {
  if (this->Available() < bytes) {
    return false;
  }
  this->m_read = Add(this->m_read, bytes);
  return true;
}
