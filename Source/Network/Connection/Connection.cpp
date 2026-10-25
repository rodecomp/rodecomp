#include "Client/Application/ErrorDialog.h"
#include "Network/Connection/Connection.h"
#include "Network/Connection/ConnectionRuntime.h"
#include "Network/Packets/PacketLengths.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <string>

// OFFSET: 0x00C7C110
// FUNCTION: fn_20382
CConnection::CConnection(ConnectionContext& context) : m_socket(INVALID_SOCKET), m_deferred(0), m_sendDeadline(0), m_context(&context) {
  this->m_input.Reset(0xA000);
  this->m_output.Reset(0xA000);
  this->m_pending.Reset(0xA000);
}

// OFFSET: 0x00C7C280
// FUNCTION: fn_20384
// OFFSET: 0x00C7C2F0
// FUNCTION: fn_20385
CConnection::~CConnection() = default;

// OFFSET: 0x00C7C1C0
// FUNCTION: fn_20383
CRagConnection::CRagConnection(ConnectionContext& context) : CConnection(context), unknown_006C(0) {
}

// OFFSET: 0x00C7C370
// FUNCTION: fn_20386
CRagConnection::~CRagConnection() = default;

// OFFSET: 0x00C7C4D0
// FUNCTION: fn_20389
bool CConnection::Connect(const ConnectionEndpoint& endpoint) {
  this->m_deferred = 0;

  while (this->m_pending.Available() != 0) {
    const int bytes = this->m_pending.Available();
    this->m_output.Append(bytes, this->m_pending.Data());
    this->m_pending.Consume(bytes);
    this->Flush();
  }

  this->m_input.Reset(0xA000);
  this->m_output.Reset(0xA000);
  this->m_pending.Reset(0xA000);
  this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (this->m_socket == INVALID_SOCKET) {
    return false;
  }

  const int noDelay = 1;
  setsockopt(this->m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&noDelay), sizeof(noDelay));
  u_long nonblocking = 1;

  if (ioctlsocket(this->m_socket, FIONBIO, &nonblocking) == SOCKET_ERROR) {
    ShowErrorDialog(this->m_context->renderSystem, "Failed to setup select mode", 0);
    return false;
  }

  std::memset(&this->m_address, 0, sizeof(this->m_address));
  std::string address = endpoint.address;
  const std::string overrideAddress = endpoint.overrideAddress;
  const std::size_t colon = overrideAddress.rfind(':');

  if (!overrideAddress.empty() && colon != std::string::npos) {
    const std::string host = overrideAddress.substr(0, colon);
    if (hostent* resolved = gethostbyname(host.c_str())) {
      in_addr resolvedAddress;
      std::memcpy(&resolvedAddress, resolved->h_addr_list[0], sizeof(resolvedAddress));
      address = inet_ntoa(resolvedAddress);
    }
  }

  this->m_address.sin_addr.s_addr = inet_addr(address.c_str());
  this->m_address.sin_family = AF_INET;
  int port = endpoint.port;

  if (!overrideAddress.empty() && colon != std::string::npos) {
    port = std::atoi(overrideAddress.c_str() + colon + 1);
  }

  this->m_address.sin_port = htons(static_cast<u_short>(port));
  ConnectionAddressCache& cache = this->m_context->addressCache;
  if (cache.session != this->m_context->session || cache.address == 0) {
    cache.address = this->m_address.sin_addr.s_addr;
    cache.session = this->m_context->session;
  }

  this->m_address.sin_addr.s_addr = cache.address;
  if (connect(this->m_socket, reinterpret_cast<const sockaddr*>(&this->m_address), sizeof(this->m_address)) != SOCKET_ERROR ||
      WSAGetLastError() == WSAEWOULDBLOCK) {
    this->m_status = 0;
    return true;
  }

  this->Close();
  return false;
}

// OFFSET: 0x00C7C830
// FUNCTION: fn_20390
void CConnection::Close() {
  if (this->m_socket != INVALID_SOCKET) {
    closesocket(this->m_socket);
    this->m_socket = INVALID_SOCKET;
    this->m_output.Reset(0xA000);
    this->m_pending.Reset(0xA000);
  }

  this->m_status = 0;
  this->m_context->options.Set(5, 0, 0);
}

// OFFSET: 0x00C7CA40
// FUNCTION: fn_20397
bool CConnection::Receive() {
  for (;;) {
    fd_set readable;
    FD_ZERO(&readable);
    FD_SET(this->m_socket, &readable);
    timeval timeout{};

    if (select(0, &readable, nullptr, nullptr, &timeout) <= 0) {
      return true;
    }

    char data[2048];
    const int bytes = g_connectionReceive(this->m_socket, data, sizeof(data), 0);
    if (bytes == SOCKET_ERROR) {
      const int error = WSAGetLastError();
      if (error == WSAEWOULDBLOCK || error == WSAENOTCONN) {
        continue;
      }
    } else if (bytes > 0) {
      this->m_input.Append(bytes, data);
      continue;
    }

    this->Close();
    return false;
  }
}

// OFFSET: 0x00C7CB30
// FUNCTION: fn_20398
bool CConnection::Flush() {
  while (this->m_sendDeadline < timeGetTime()) {
    if (this->m_output.Available() == 0) {
      return true;
    }

    fd_set writable;
    FD_ZERO(&writable);
    FD_SET(this->m_socket, &writable);
    timeval timeout{};
    const int ready = select(0, nullptr, &writable, nullptr, &timeout);

    if (ready != SOCKET_ERROR) {
      if (ready <= 0) {
        return true;
      }

      const int sent = g_connectionSend(this->m_socket, this->m_output.Data(), this->m_output.Available(), 0);
      if (sent != SOCKET_ERROR) {
        this->m_output.Consume(sent);
      } else {
        const int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK && error != WSAENOTCONN) {
          this->Close();
          return false;
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00C7CC30
// FUNCTION: fn_20399
bool CConnection::Update() {
  return this->m_socket == INVALID_SOCKET || (this->Receive() && this->Flush());
}

// OFFSET: 0x00C7CC60
// FUNCTION: fn_20400
void CConnection::UpdateStatus() {
  if (this->m_socket != INVALID_SOCKET) {
    this->m_status = !this->Receive() || !this->Flush();
  }
}

// OFFSET: 0x00C7CA10
// FUNCTION: fn_20394
int CConnection::Status() const {
  return this->m_status;
}

// OFFSET: 0x00C7CE30
// FUNCTION: fn_20405
void CConnection::Send(int bytes, const void* data) {
  timeGetTime();
  if (this->m_socket == INVALID_SOCKET) {
    return;
  }

  this->m_output.Append(bytes, data);
  this->Flush();
}

// OFFSET: 0x00C7CC90
// FUNCTION: fn_20401
int CConnection::Read(void* output, int bytes, unsigned char exact) {
  if (exact != 0) {
    return this->m_input.Read(bytes, output) ? bytes : 0;
  }

  const int available = std::min(bytes, this->m_input.Available());
  this->m_input.Read(available, output);
  return available;
}

// OFFSET: 0x00C7CCE0
// FUNCTION: fn_20402
bool CConnection::ReadPacket(void* output, unsigned short* bytes, const PacketLengths& lengths) {
  unsigned short id;
  if (!this->m_input.Peek(2, &id)) {
    return false;
  }

  unsigned short size;
  if (lengths.Lookup(id).kind == 1) {
    size = static_cast<unsigned short>(lengths.FixedLength(id));
  } else {
    unsigned header;
    if (!this->m_input.Peek(4, &header)) {
      return false;
    }

    size = static_cast<unsigned short>(header >> 16);
  }

  if (!this->m_input.Read(size, output)) {
    return false;
  }

  if (bytes) {
    *bytes = size;
  }

  return true;
}

// OFFSET: 0x00C7CEC0
// FUNCTION: fn_20406
void CConnection::Prepend(int bytes, const void* data) {
  this->m_input.Prepend(bytes, data);
}

// OFFSET: 0x00C7CDB0
// FUNCTION: fn_20403
void CConnection::ResetDeferred() {
  this->m_pending.Reset(0xA000);
}

// OFFSET: 0x00C7CDC0
// FUNCTION: fn_20404
void CConnection::ResetBuffers() {
  this->m_input.Reset(0xA000);
  this->m_output.Reset(0xA000);
  this->m_pending.Reset(0xA000);
}

// OFFSET: 0x00C7CED0
// FUNCTION: fn_20407
void CConnection::SetDeferred(unsigned char value) {
  this->m_deferred = value;
  if (value != 0) {
    return;
  }

  while (this->m_pending.Available() != 0) {
    const int bytes = this->m_pending.Available();
    this->m_output.Append(bytes, this->m_pending.Data());
    this->m_pending.Consume(bytes);
    this->Flush();
  }
}

// OFFSET: 0x00C7D7D0
// FUNCTION: fn_20409
CRagConnection& GetRagConnection(ConnectionContext& context) {
  // STATIC: 0x016FDC98
  // GLOBAL: gl_24490
  static CRagConnection connection(context);
  return connection;
}
