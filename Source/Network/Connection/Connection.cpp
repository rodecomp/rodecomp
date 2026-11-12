#include "Client/Application/ErrorDialog.h"
#include "Client/Session/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Connection/ConnectionRuntime.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <memory>
#include <string>

// OFFSET: 0x00C7C110
// FUNCTION: fn_20382
CConnection::CConnection() : m_socket(INVALID_SOCKET), m_deferred(0), m_sendDeadline(0) {
  this->m_input.Init(0xA000);
  this->m_output.Init(0xA000);
  this->m_pending.Init(0xA000);
}

// OFFSET: 0x00C7C280
// FUNCTION: fn_20384
CConnection::~CConnection() = default;

// OFFSET: 0x00C7C1C0
// FUNCTION: fn_20383
CRagConnection::CRagConnection() : padding_006C(0) {
}

// INLINED
CRagConnection::~CRagConnection() {
}


namespace {
  // INLINED
  unsigned long ResolveConnectionAddress(std::string address, std::string overrideAddress) {
    if (!overrideAddress.empty()) {
      const std::size_t colon = overrideAddress.rfind(":");
      if (colon != std::string::npos) {
        const std::string host = overrideAddress.substr(0, colon);
        if (hostent* resolved = gethostbyname(host.c_str())) {
          address = inet_ntoa(*reinterpret_cast<in_addr*>(resolved->h_addr_list[0]));
        }
      }
    }
    return inet_addr(address.c_str());
  }

  // INLINED
  u_short ResolveConnectionPort(std::string overrideAddress, int port) {
    if (!overrideAddress.empty()) {
      const std::size_t colon = overrideAddress.rfind(":");
      if (colon != std::string::npos) {
        const std::string portText = overrideAddress.substr(colon + 1);
        port = std::atoi(portText.c_str());
      }
    }
    return htons(static_cast<u_short>(port));
  }
}

// OFFSET: 0x00C7C4D0
// FUNCTION: fn_20389
bool CConnection::Connect(const ConnectionEndpoint& endpoint) {
  this->SetBlock(0);

  this->m_input.Init(0xA000);
  this->m_output.Init(0xA000);
  this->m_pending.Init(0xA000);
  this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (this->m_socket == INVALID_SOCKET) {
    return false;
  }

  u_long nonblocking = 1;
  if (ioctlsocket(this->m_socket, FIONBIO, &nonblocking) != SOCKET_ERROR) {
    SecureZeroMemory(&this->m_address, sizeof(this->m_address));
    this->m_address.sin_addr.s_addr = ResolveConnectionAddress(endpoint.address, endpoint.overrideAddress);
    this->m_address.sin_family = AF_INET;
    this->m_address.sin_port = ResolveConnectionPort(endpoint.overrideAddress, endpoint.port);
    if (connect(this->m_socket, reinterpret_cast<const sockaddr*>(&this->m_address), sizeof(this->m_address)) != SOCKET_ERROR ||
        WSAGetLastError() == WSAEWOULDBLOCK) {
      this->m_status = 0;
      return true;
    }

    this->Close();
    return false;
  }

  ShowErrorDialog("Failed to setup select mode", 0);
  return false;
}

// OFFSET: 0x00C7C830
// FUNCTION: fn_20390
void CConnection::Close() {
  if (this->m_socket != INVALID_SOCKET) {
    closesocket(this->m_socket);
    this->m_socket = INVALID_SOCKET;
    this->m_output.Init(0xA000);
    this->m_pending.Init(0xA000);
  }

  this->m_status = 0;
  g_session.m_options.Values()[5] = 0;
}

// OFFSET: 0x00C7C9C0
// FUNCTION: fn_20392
short CRagConnection::GetPacketType(const char* packet) {
  return *reinterpret_cast<const short*>(packet);
}

// OFFSET: 0x00C7CA40
// FUNCTION: fn_20397
bool CConnection::Receive() {
  for (;;) {
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    fd_set readable;
    FD_ZERO(&readable);
    FD_SET(this->m_socket, &readable);
    const int ready = select(0, &readable, nullptr, nullptr, &timeout);

    if (ready == SOCKET_ERROR || ready <= 0) {
      return true;
    }

    char data[2048];
    const int bytes = g_connectionReceive(this->m_socket, data, sizeof(data), 0);
    if (bytes != SOCKET_ERROR) {
      if (bytes <= 0) {
        break;
      }

      this->m_input.InsertData(bytes, data);
    } else {
      const int error = WSAGetLastError();
      if (error != WSAEWOULDBLOCK && error != WSAENOTCONN) {
        break;
      }
    }
  }

  this->Close();
  return false;
}

// OFFSET: 0x00C7CB30
// FUNCTION: fn_20398
bool CConnection::Flush() {
  while (this->m_sendDeadline < timeGetTime()) {
    if (this->m_output.GetSize() == 0) {
      return true;
    }

    fd_set writable;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    FD_ZERO(&writable);
    FD_SET(this->m_socket, &writable);
    const int ready = select(0, nullptr, &writable, nullptr, &timeout);

    if (ready != SOCKET_ERROR) {
      if (ready <= 0) {
        return true;
      }

      const int sent = g_connectionSend(this->m_socket, this->m_output.Data(), this->m_output.Pending(), 0);
      if (sent != SOCKET_ERROR) {
        this->m_output.RemoveData(sent);
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
    if (this->Receive() && this->Flush()) {
      this->m_status = 0;
    } else {
      this->m_status = 1;
    }
  }
}

// OFFSET: 0x00C7CA10
// FUNCTION: fn_20394
int CConnection::Status() const {
  return this->m_status;
}

// OFFSET: 0x00C7CA20
// FUNCTION: fn_20395
void __stdcall Nullsub435() {
}

// OFFSET: 0x00C7CA30
// FUNCTION: fn_20396
void __stdcall Nullsub436() {
}

// OFFSET: 0x00C7CE30
// FUNCTION: fn_20405
void CConnection::Send(int bytes, const void* data) {
  timeGetTime();
  if (this->m_socket == INVALID_SOCKET) {
    return;
  }

  this->m_output.InsertData(bytes, data);
  this->Flush();
}

// OFFSET: 0x00C7CC90
// FUNCTION: fn_20401
int CConnection::Read(void* output, int bytes, unsigned char exact) {
  if (exact != 0) {
    return this->m_input.GetData(bytes, output) ? bytes : 0;
  }

  const int available = std::min(bytes, this->m_input.GetSize());
  this->m_input.GetData(available, output);
  return available;
}

// OFFSET: 0x00C7CCE0
// FUNCTION: fn_20402
bool CConnection::ReadPacket(void* output, unsigned short* bytes, const PacketLengths& lengths) {
  unsigned short id;
  if (!this->m_input.PeekData(2, &id)) {
    return false;
  }

  unsigned short size;
  if (lengths.Lookup(id).kind == 1) {
    size = static_cast<unsigned short>(lengths.FixedLength(id));
  } else {
    unsigned header;
    if (!this->m_input.PeekData(4, &header)) {
      return false;
    }

    size = static_cast<unsigned short>(header >> 16);
  }

  if (!this->m_input.GetData(size, output)) {
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
  this->m_pending.Init(0xA000);
}

// OFFSET: 0x00C7CDC0
// FUNCTION: fn_20404
void CConnection::ResetQueue() {
  this->m_input.Init(0xA000);
  this->m_output.Init(0xA000);
  this->m_pending.Init(0xA000);
}

// OFFSET: 0x00C7CED0
// FUNCTION: fn_20407
void CConnection::SetBlock(unsigned char value) {
  this->m_deferred = value;
  if (value != 0) {
    return;
  }

  if (this->m_pending.GetSize() == 0) {
    return;
  }

  for (int bytes = this->m_pending.Pending(); bytes != 0; bytes = this->m_pending.Pending()) {
    this->m_output.InsertData(bytes, this->m_pending.Data());
    this->m_pending.RemoveData(bytes);
    this->Flush();
  }
}

// OFFSET: 0x00C7D7D0
// FUNCTION: fn_20409
CRagConnection& GetRagConnection() {
  // STATIC: 0x016FDC98
  // GLOBAL: gl_24490
  static CRagConnection connection;
  return connection;
}

struct ConnectionBlock {
  /* 00000 */ ConnectionBlock* next;
  /* 00004 */ unsigned char data[0x41004];
};

struct ConnectionBlockList {
  /* 0000 */ ConnectionBlock* head;
  /* 0004 */ int* count;
};

namespace {
  // GLOBAL: gl_25135
  constexpr unsigned char g_codeMarker[6] = {0x33, 0xC0, 0x33, 0xDB, 0x33, 0xC9};
}

// OFFSET: 0x00C7D930
// FUNCTION: fn_20410
void ReleaseConnectionBlocks(int, ConnectionBlockList& list) noexcept {
  *list.count = 0;
  ConnectionBlock* block = list.head;
  while (block) {
    ConnectionBlock* next = block->next;
    std::allocator<ConnectionBlock>().deallocate(block, 1);
    block = next;
  }
}

// OFFSET: 0x00C7DF30
// FUNCTION: fn_20411
int PacketCodecStub(void* data, std::size_t size, std::size_t* outSize, int) {
  unsigned char buffer[0x400];
  std::memcpy(buffer, data, size);
  std::memcpy(data, buffer, size);
  *outSize = size;
  return 0;
}

// OFFSET: 0x00C7E030
// FUNCTION: fn_20412
const unsigned char* __stdcall FindCodeMarker(const unsigned char* code) {
  while (std::memcmp(code, g_codeMarker, sizeof(g_codeMarker)) != 0) {
    ++code;
  }
  return code;
}
