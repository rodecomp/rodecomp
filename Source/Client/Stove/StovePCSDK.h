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
    struct __declspec(dllimport) StovePCLogEvent {
      const wchar_t* GetCategory1() const;
      void SetCategory1(const wchar_t* category);
    };

    struct __declspec(dllimport) StovePCToken {
      const wchar_t* GetAccessToken() const;
    };

    struct __declspec(dllimport) StovePCShutdown {
      StovePCShutdown(const StovePCShutdown& other);
      ~StovePCShutdown();

      /* 0000 */ std::uint8_t unknown_0000[12];
    };

    struct __declspec(dllimport) StovePCInitializeParam {
      StovePCInitializeParam();
      ~StovePCInitializeParam();
      void SetEnvironment(const wchar_t* environment);
      void SetApplicationKey(const wchar_t* applicationKey);

      /* 0000 */ std::uint8_t unknown_0000[36];
    };

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
      StovePCPaymentOption();
      StovePCPaymentOption(const StovePCPaymentOption& other);
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

    struct StovePCPurchaseOption : StovePCPaymentOption {
      StovePCPurchaseOption(const StovePCPaymentOption& other) : StovePCPaymentOption(other) {
      }
    };

    struct __declspec(dllimport) StovePCChargeInfo {
      const wchar_t* GetChargeTypeName() const;
      void SetChargeDisplayDeductVal(double value);
      void SetChargeType(int chargeType);
    };

    struct __declspec(dllimport) StovePCOrderProduct {
      StovePCOrderProduct();
      std::int64_t GetProductId() const;
      void SetProductId(std::int64_t productId);

      /* 0000 */ std::uint8_t unknown_0000[20];
    };

    struct __declspec(dllimport) StovePCStartPurchaseParam {
      StovePCStartPurchaseParam();
      ~StovePCStartPurchaseParam();
      void CreateOrderProduct(unsigned int count);
      void SetOrderProduct(int index, const StovePCOrderProduct* orderProduct);
      void SetPurchaseOption(StovePCPurchaseOption option);

      /* 0000 */ std::uint8_t unknown_0000[40];
    };

    struct __declspec(dllimport) StovePCPurchasedProduct {
      const wchar_t* GetCategoryId() const;
    };

    struct __declspec(dllimport) StovePCShopCategory {
      int GetDisplayNumber() const;
    };

    struct __declspec(dllimport) StovePCProduct {
      const wchar_t* GetCategoryId() const;
      double GetSalePrice() const;
      int GetGuidQuantity() const;
      int GetPurchaseLimitCount() const;
      std::int64_t GetSaleBeginDate() const;
      std::int64_t GetSaleEndDate() const;

      /* 0000 */ std::uint8_t unknown_0000[156];
    };

    struct __declspec(dllimport) StovePCPurchaseResult {
      StovePCPurchaseResult(const StovePCPurchaseResult& other);
      ~StovePCPurchaseResult();

      /* 0000 */ std::uint8_t unknown_0000[48];
    };
  }
}

extern "C" {
Stove::PCSDK::Result Base_GetAccessToken(wchar_t* accessToken, int length);
Stove::PCSDK::Result Base_GetUser(Stove::PCSDK::Base::StovePCUser* user);
void Base_RunCallback();
bool Base_RestartAppIfNecessary(const Stove::PCSDK::Base::StovePCInitializeParam* param);
void Base_Initialize(const Stove::PCSDK::Base::StovePCInitializeParam* param, void (*callback)(Stove::PCSDK::CallbackResult));
void Base_GetShutdown(void (*callback)(Stove::PCSDK::CallbackResult, Stove::PCSDK::Base::StovePCShutdown));
Stove::PCSDK::Result IAP_Initialize(const wchar_t* shopKey);
void IAP_StartPurchase(const Stove::PCSDK::IAP::StovePCStartPurchaseParam* param,
                       void (*callback)(Stove::PCSDK::CallbackResult, Stove::PCSDK::IAP::StovePCPurchaseResult));
void IAP_ConfirmPurchase(std::int64_t transactionNumber, void (*callback)(Stove::PCSDK::CallbackResult, bool, int, unsigned int, int, int));
}

#endif
