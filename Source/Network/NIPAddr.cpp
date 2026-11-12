#include <winsock2.h>

#include "Network/NIPAddr.h"

// INLINED
inline void NIPAddr::SetAddress(std::string text) {
  this->address = inet_addr(text.c_str());
}

// OFFSET: 0x007349C0
// FUNCTION: fn_07599
NIPAddr::NIPAddr(std::string text) {
  this->SetAddress(text);
}

// OFFSET: 0x00734BD0
// FUNCTION: fn_07601
NIPAddr::~NIPAddr() {
}

// OFFSET: 0x00734CE0
// FUNCTION: fn_07603
bool NIPAddr::IsPrivateAddress(std::uint32_t address) {
  const WORD low = LOWORD(address);
  const WORD high = HIWORD(address);
  const std::uint32_t swapped = MAKELONG(MAKEWORD(HIBYTE(high), LOBYTE(high)), MAKEWORD(HIBYTE(low), LOBYTE(low)));

  std::uint32_t lower = NIPAddr(std::string("10.0.0.0")).address;
  std::uint32_t upper = NIPAddr(std::string("10.255.255.255")).address;
  if (swapped >= lower && swapped <= upper) {
    return true;
  }

  lower = NIPAddr(std::string("172.16.0.0")).address;
  upper = NIPAddr(std::string("172.31.255.255")).address;
  if (swapped >= lower && swapped <= upper) {
    return true;
  }

  lower = NIPAddr(std::string("192.168.0.0")).address;
  upper = NIPAddr(std::string("192.168.255.255")).address;
  if (swapped >= lower && swapped <= upper) {
    return true;
  }
  return false;
}
