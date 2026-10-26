#ifndef RAGNAROK_SOURCE_UTILITY_NIPADDR_H
#define RAGNAROK_SOURCE_UTILITY_NIPADDR_H

#include <cstdint>
#include <string>

class NIPAddr {
public:
  explicit NIPAddr(std::string text);
  virtual ~NIPAddr();
  static bool IsPrivateAddress(std::uint32_t address);

  /* 0004 */ std::uint32_t address;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NIPAddr) == 8);
#endif

#endif
