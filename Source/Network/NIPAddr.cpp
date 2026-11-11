#include <winsock2.h>

#include "Network/NIPAddr.h"

#include "Utility/StdStringCopy.h"

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
  const std::uint32_t swapped = MAKELONG(MAKEWORD(HIBYTE(HIWORD(address)), LOBYTE(HIWORD(address))), MAKEWORD(HIBYTE(LOWORD(address)), LOBYTE(LOWORD(address))));
  std::uint32_t lower;
  std::uint32_t upper;
  {
    NIPAddr first(std::string("10.0.0.0"));
    lower = first.address;
  }
  {
    NIPAddr last(std::string("10.255.255.255"));
    upper = last.address;
  }
  if (swapped >= lower && swapped <= upper) {
    return true;
  }
  {
    NIPAddr first(std::string("172.16.0.0"));
    lower = first.address;
  }
  {
    NIPAddr last(std::string("172.31.255.255"));
    upper = last.address;
  }
  if (swapped >= lower && swapped <= upper) {
    return true;
  }
  {
    NIPAddr first(std::string("192.168.0.0"));
    lower = first.address;
  }
  {
    NIPAddr last(std::string("192.168.255.255"));
    upper = last.address;
  }
  return swapped >= lower && swapped <= upper;
}
