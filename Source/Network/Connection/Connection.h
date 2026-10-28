#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTION_CONNECTION_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTION_CONNECTION_H

#include "Network/Connection/ConnectionBuffer.h"

#include <winsock2.h>

class PacketLengths;

struct ConnectionEndpoint {
  const char* address;
  int port;
  const char* overrideAddress;
};

class CConnection {
public:
  CConnection();
  virtual ~CConnection();
  virtual bool Connect(const ConnectionEndpoint& endpoint);
  void Close();
  bool Receive();
  bool Flush();
  bool Update();
  void UpdateStatus();
  int Status() const;
  void Send(int bytes, const void* data);
  int Read(void* output, int bytes, unsigned char exact);
  bool ReadPacket(void* output, unsigned short* bytes, const PacketLengths& lengths);
  void Prepend(int bytes, const void* data);
  void ResetDeferred();
  void ResetBuffers();
  void SetDeferred(unsigned char value);

  // INLINED
  SOCKET NativeSocket() const {
    return this->m_socket;
  }
  // INLINED
  ConnectionBuffer& Input() {
    return this->m_input;
  }
  // INLINED
  unsigned& SendDeadline() {
    return this->m_sendDeadline;
  }

protected:
  /* 0004 */ SOCKET m_socket;
  /* 0008 */ sockaddr_in m_address;
  /* 0018 */ unsigned char m_deferred;
  /* 001C */ unsigned m_sendDeadline;
  /* 0020 */ int m_status;
  /* 0024 */ ConnectionBuffer m_output;
  /* 003C */ ConnectionBuffer m_input;
  /* 0054 */ ConnectionBuffer m_pending;
};

class CRagConnection : public CConnection {
public:
  CRagConnection();
  ~CRagConnection() override;

private:
  /* 006C */ unsigned char unknown_006C;
};

CRagConnection& GetRagConnection();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CConnection) == 0x6C);
static_assert(sizeof(CRagConnection) == 0x70);
#endif

#endif
