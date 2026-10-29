#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTION_PACKETQUEUE_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTION_PACKETQUEUE_H

#include <vector>

class CPacketQueue {
public:
  CPacketQueue();
  ~CPacketQueue();
  void Init(unsigned bytes);
  int GetSize() const;
  void InsertData(int bytes, const void* source);
  void Prepend(int bytes, const void* source);
  bool GetData(int bytes, void* destination);
  bool PeekData(int bytes, void* destination) const;
  bool RemoveData(int bytes);

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
  CPacketQueue(const CPacketQueue&) = delete;
  CPacketQueue& operator=(const CPacketQueue&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPacketQueue) == 24);
#endif

#endif
