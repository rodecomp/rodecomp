#include "Network/Connection/PacketQueue.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00C7EF60
// FUNCTION: fn_20418
CPacketQueue::CPacketQueue() : m_data(nullptr) {
}

// OFFSET: 0x00C7EFC0
// FUNCTION: fn_20419
CPacketQueue::~CPacketQueue() = default;

// OFFSET: 0x00C7F060
// FUNCTION: fn_20422
void CPacketQueue::Init(unsigned bytes) {
  this->m_read = 0;
  this->m_write = 0;
  this->m_storage.resize(bytes);
  this->m_data = this->m_storage.empty() ? nullptr : this->m_storage.data();
}

// OFFSET: 0x00C7F050
// FUNCTION: fn_20421
int CPacketQueue::GetSize() const {
  return this->m_write - this->m_read;
}

// OFFSET: 0x00C7F0E0
// FUNCTION: fn_20423
void CPacketQueue::InsertData(int bytes, const void* source) {
  const int size = static_cast<int>(this->m_storage.size());
  if ((this->m_write + bytes) > size) {
    const int required = (this->GetSize() + bytes);
    if (required > size) {
      this->m_storage.resize(static_cast<unsigned>(required) * 2u);
      this->m_data = this->m_storage.data();
    }

    std::memmove(this->m_data, this->m_data + this->m_read, this->GetSize());
    this->m_write = this->GetSize();
    this->m_read = 0;
  }

  std::memcpy(this->m_data + this->m_write, source, bytes);
  this->m_write = (this->m_write + bytes);
}

// OFFSET: 0x00C7F160
// FUNCTION: fn_20424
void CPacketQueue::Prepend(int bytes, const void* source) {
  const int available = this->GetSize();
  const int required = (bytes + available);
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
bool CPacketQueue::GetData(int bytes, void* destination) {
  if (this->GetSize() < bytes) {
    return false;
  }

  std::memcpy(destination, this->m_data + this->m_read, bytes);
  this->m_read = (this->m_read + bytes);
  return true;
}

// OFFSET: 0x00C7F2B0
// FUNCTION: fn_20425
bool CPacketQueue::PeekData(int bytes, void* destination) const {
  if (this->m_write - this->m_read < bytes) {
    return false;
  }

  std::memcpy(destination, this->m_data + this->m_read, bytes);
  return true;
}

// OFFSET: 0x00C7F2E0
// FUNCTION: fn_20426
bool CPacketQueue::RemoveData(int bytes) {
  if (this->m_write - this->m_read < bytes) {
    return false;
  }

  this->m_read += bytes;
  return true;
}
