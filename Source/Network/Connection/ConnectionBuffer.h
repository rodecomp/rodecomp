#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTION_CONNECTIONBUFFER_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTION_CONNECTIONBUFFER_H

#include <vector>

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

  // INLINED
  char* Data() const {
    return this->m_data + this->m_read;
  }
  unsigned Size() const {
    return static_cast<unsigned>(this->m_storage.size());
  }

private:
  /* 0000 */ char* m_data;
  /* 0004 */ int m_read;
  /* 0008 */ int m_write;
  /* 000C */ std::vector<char> m_storage;
  ConnectionBuffer(const ConnectionBuffer&) = delete;
  ConnectionBuffer& operator=(const ConnectionBuffer&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ConnectionBuffer) == 24);
#endif

#endif
