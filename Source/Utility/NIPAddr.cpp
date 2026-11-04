#include <winsock2.h>

#include "Utility/NIPAddr.h"

// OFFSET: 0x007349C0
// FUNCTION: fn_07599
NIPAddr::NIPAddr(std::string text) {
  std::string copy(text);
  this->address = inet_addr(copy.c_str());
}

// OFFSET: 0x00734BD0
// FUNCTION: fn_07601
NIPAddr::~NIPAddr() {
}

// OFFSET: 0x00734CE0
// FUNCTION: fn_07603
bool NIPAddr::IsPrivateAddress(std::uint32_t address) {
  const std::uint32_t swapped = (address >> 24) | ((address >> 8) & 0x0000FF00u) | ((address << 8) & 0x00FF0000u) | (address << 24);
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
