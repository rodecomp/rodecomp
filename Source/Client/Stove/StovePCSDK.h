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

  namespace IAP {
    struct __declspec(dllimport) StovePCPaymentOption {
      void SetOperation(int operation);

      /* 0000 */ std::uint8_t unknown_0000[24];
    };

    struct __declspec(dllimport) StovePCTermsOption : StovePCPaymentOption {
      StovePCTermsOption();
    };

    struct __declspec(dllimport) StovePCFetchProductParam {
      StovePCFetchProductParam();
      ~StovePCFetchProductParam();
      void SetCategoryId(const wchar_t* categoryId);
      void SetPageNumber(int pageNumber);
      void SetPageSize(int pageSize);

      /* 0000 */ std::uint8_t unknown_0000[12];
    };

    inline void FetchTermsAgreement(StovePCTermsOption* option, void (*callback)(CallbackResult)) {
      reinterpret_cast<void(__cdecl*)(StovePCTermsOption*, void (*)(CallbackResult))>(0x00E6CD71)(option, callback);
    }

    inline void FetchProducts(StovePCFetchProductParam* param, void (*callback)(CallbackResult)) {
      reinterpret_cast<void(__cdecl*)(StovePCFetchProductParam*, void (*)(CallbackResult))>(0x00E6CD5F)(param, callback);
    }
  }
}

extern "C" {
Stove::PCSDK::Result Base_GetAccessToken(wchar_t* accessToken, int length);
Stove::PCSDK::Result Base_GetUser(Stove::PCSDK::Base::StovePCUser* user);
void Base_RunCallback();
}

#endif
