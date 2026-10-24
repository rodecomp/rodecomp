#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTIONBUFFER_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTIONBUFFER_H

#include <vector>

// Original24-byte connection buffer: borrowed data alias, signed read/write
// offsets, owned vector<char>. Offsets become usable only after Reset.
class ConnectionBuffer {
public:
  ConnectionBuffer();
  ~ConnectionBuffer();
  void Reset(unsigned bytes);
  int Available() const;
  void Append(int bytes, const void* source);
  void Prepend(int bytes, const void* source);
  bool Read(int bytes, void* destination);
  bool Peek(int bytes, void* destination) const;
  bool Consume(int bytes);

  // Source view for original direct socket access; no copied packet storage.
  char* Data() const {
    return this->m_data + this->m_read;
  }
  unsigned Size() const {
    return static_cast<unsigned>(this->m_storage.size());
  }

private:
  char* m_data;
  int m_read;
  int m_write;
  std::vector<char> m_storage;
  ConnectionBuffer(const ConnectionBuffer&) = delete;
  ConnectionBuffer& operator=(const ConnectionBuffer&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ConnectionBuffer) == 24);
#endif

#endif
