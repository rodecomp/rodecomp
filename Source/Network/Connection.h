#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTION_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTION_H

#include "Client/ClientOptions.h"
#include "Network/ConnectionBuffer.h"

#include <winsock2.h>

class CRenderSystemBase;
struct PacketLengths;

// Source view of original endpoint strings at+0/+20 and port at+16.
// The original trailing string's reservation is not established here.
struct ConnectionEndpoint {
  const char* address;
  int port;
  const char* overrideAddress;
};

// Original patched connect cache is shared across connections, keyed by the
// Client session field. Caller supplies the actual owner values.
struct ConnectionAddressCache {
  unsigned address = 0;
  unsigned session = 0xFFFFFFFFu;
};

struct ConnectionContext {
  ClientOptions& options;
  const unsigned& session;
  ConnectionAddressCache& addressCache;
  CRenderSystemBase* renderSystem;
};

class CConnection {
public:
  explicit CConnection(ConnectionContext& context);
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

  // Source views of original direct owner fields, not an alternate socket API.
  SOCKET NativeSocket() const {
    return this->m_socket;
  }
  ConnectionBuffer& Input() {
    return this->m_input;
  }
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
  ConnectionContext* m_context;
};

// Original derived vtable changes only the deleting destructor entry.
class CRagConnection : public CConnection {
public:
  explicit CRagConnection(ConnectionContext& context);
  ~CRagConnection() override;

private:
  /* 006C */ unsigned char unknown_006C;
};

// Lazily constructs the original process-wide connection. Later calls retain
// the first context; callers must keep that context and its owners alive.
// Destruction releases buffers without implicitly closing the socket.
CRagConnection& GetRagConnection(ConnectionContext& context);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CConnection) == 112);
static_assert(sizeof(CRagConnection) == 116);
#endif

#endif
