#ifndef RAGNAROK_SOURCE_CLIENT_STOVE_STOVEPCSDK_H
#define RAGNAROK_SOURCE_CLIENT_STOVE_STOVEPCSDK_H

#include <cstdint>

namespace Stove::PCSDK {
  struct __declspec(dllimport) Result {
    ~Result();

    bool IsSuccessful() const;

    /* 0000 */ std::uint8_t unknown_0000[12];
  };

  struct __declspec(dllimport) CallbackResult {
    CallbackResult(const CallbackResult& other);
    ~CallbackResult();

    Result GetResult() const;

    /* 0000 */ std::uint8_t unknown_0000[28];
  };

  namespace Base {
    struct __declspec(dllimport) StovePCUser {
      StovePCUser();
      ~StovePCUser();

      std::uint64_t GetMemberNumber() const;
      std::uint64_t GetGameUserId() const;

      /* 0000 */ std::uint8_t unknown_0000[20];
    };
  }
}

extern "C" {
Stove::PCSDK::Result Base_GetAccessToken(wchar_t* accessToken, int length);
Stove::PCSDK::Result Base_GetUser(Stove::PCSDK::Base::StovePCUser* user);
void Base_RunCallback();
}

#endif
