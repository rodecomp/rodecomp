#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Client/Stove/StoveMgr.h"
#include "Client/Stove/StovePCSDK.h"
#include "Network/TextEncoding.h"
#include "UI/Text/UIBroadcastBalloon.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <windows.h>
#include <shellapi.h>

// GLOBAL: gl_24904
bool g_stoveWaitingCallback;

void OnStoveTermsAgreementFetched(Stove::PCSDK::CallbackResult callbackResult, int unused, const wchar_t* url);
void OnStoveCategoryProductsFetched(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products, unsigned int count);
void OnStoveAllProductsFetched(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products, unsigned int count);
void OnStoveInitialized(Stove::PCSDK::CallbackResult callbackResult);
void OnStoveShutdown(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::Base::StovePCShutdown shutdown);
void OnStovePurchaseStarted(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::IAP::StovePCPurchaseResult purchaseResult);
void OnStovePurchaseConfirmed(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int productCount, unsigned int count, int remainingCount, int resultCode);

// STRING: st_01259
const wchar_t g_stoveCategoryIdA[] = L"44C71118750747D";

// STRING: st_01260
const wchar_t g_stoveCategoryIdB[] = L"14A6891F90E24D7";


// OFFSET: 0x0063EEE0
// FUNCTION: fn_04800
void CStoveMgr::FetchAllProducts() {
  Stove::PCSDK::IAP::StovePCFetchProductParam allParam;
  allParam.SetCategoryId(L"");
  allParam.SetPageNumber(1);
  allParam.SetPageSize(0x7FFFFFFF);
  IAP_FetchProducts(&allParam, OnStoveAllProductsFetched);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);

  g_session.ClearInventoryCategoryItems(9);
  Stove::PCSDK::IAP::StovePCFetchProductParam categoryParam;
  if (this->m_environment == 2) {
    categoryParam.SetCategoryId(g_stoveCategoryIdA);
  } else {
    categoryParam.SetCategoryId(g_stoveCategoryIdB);
  }
  categoryParam.SetPageNumber(1);
  categoryParam.SetPageSize(0x7FFFFFFF);
  IAP_FetchProducts(&categoryParam, OnStoveCategoryProductsFetched);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

// OFFSET: 0x0063F030
// FUNCTION: fn_04801
void FetchStoveTermsAgreement() {
  Stove::PCSDK::IAP::StovePCTermsOption option;
  option.SetOperation(Stove::PCSDK::IAP::StovePCPaymentOperation::DEFAULT);
  IAP_FetchTermsAgreement(&option, OnStoveTermsAgreementFetched);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

// OFFSET: 0x0063E910
// FUNCTION: fn_45460
void CStoveMgr::FetchCategoryProducts() {
  g_session.ClearInventoryCategoryItems(9);
  Stove::PCSDK::IAP::StovePCFetchProductParam param;
  param.SetCategoryId(this->m_environment == 2 ? g_stoveCategoryIdA : g_stoveCategoryIdB);
  param.SetPageNumber(1);
  param.SetPageSize(0x7FFFFFFF);
  IAP_FetchProducts(&param, OnStoveCategoryProductsFetched);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const std::array<char, Length>& source) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

struct AccessTokenFailureMessage {
  EncryptedName<100, 0x3375B1C7DBB5FD55ULL>& operator()() const;
};

struct UserFailureMessage {
  EncryptedName<100, 0x151F77EBEBBB2147ULL>& operator()() const;
};

struct TokenCallbackFailureMessage {
  EncryptedName<100, 0x4F1791FB1503C713ULL>& operator()() const;
};

struct PurchaseCheckingMessage {
  EncryptedName<129, 0x2D3B15774149D5DDULL>& operator()() const;
};

struct InitializeCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult) const;
};

struct TermsAgreementCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult, int unused, const wchar_t* url) const;
};

struct FetchProductsCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products, unsigned int count) const;
};

struct PurchaseResultCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::IAP::StovePCPurchaseResult purchaseResult) const;
};

struct ShutdownCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::Base::StovePCShutdown shutdown) const;
};

struct CategoryProductsCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products, unsigned int count) const;
};

struct ConfirmPurchaseCallback {
  void operator()(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int productCount, unsigned int count, int remainingCount, int resultCode) const;
};

// OFFSET: 0x0063A780
// FUNCTION: fn_04771
void OnStoveInitialized(Stove::PCSDK::CallbackResult callbackResult) {
  static_cast<const InitializeCallback*>(nullptr)->operator()(callbackResult);
}

// OFFSET: 0x0063A7F0
// FUNCTION: fn_04772
void OnStoveTermsAgreementFetched(Stove::PCSDK::CallbackResult callbackResult, int unused, const wchar_t* url) {
  static_cast<const TermsAgreementCallback*>(nullptr)->operator()(callbackResult, unused, url);
}

// OFFSET: 0x0063A860
// FUNCTION: fn_04773
void OnStoveAllProductsFetched(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products, unsigned int count) {
  static_cast<const CategoryProductsCallback*>(nullptr)->operator()(callbackResult, products, count);
}

// OFFSET: 0x0063A8D0
// FUNCTION: fn_04774
void OnStoveCategoryProductsFetched(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products, unsigned int count) {
  static_cast<const FetchProductsCallback*>(nullptr)->operator()(callbackResult, products, count);
}

// OFFSET: 0x0063A940
// FUNCTION: fn_04775
void OnStovePurchaseStarted(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::IAP::StovePCPurchaseResult purchaseResult) {
  static_cast<const PurchaseResultCallback*>(nullptr)->operator()(callbackResult, purchaseResult);
}

// OFFSET: 0x0063A9D0
// FUNCTION: fn_04776
void OnStovePurchaseConfirmed(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int productCount, unsigned int count, int remainingCount, int resultCode) {
  static_cast<const ConfirmPurchaseCallback*>(nullptr)->operator()(callbackResult, purchased, productCount, count, remainingCount, resultCode);
}

// OFFSET: 0x0063AA50
// FUNCTION: fn_04777
void OnStoveShutdown(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::Base::StovePCShutdown shutdown) {
  static_cast<const ShutdownCallback*>(nullptr)->operator()(callbackResult, shutdown);
}


// OFFSET: 0x0063AAE0
// FUNCTION: fn_04778
CStoveMgr::CStoveMgr()
    : m_initialized(false), m_pendingOperation(0), m_environment(1), m_purchaseIdLow(0), m_purchaseIdHigh(0), m_purchaseUrl(), m_accessToken{},
      m_memberNumber{}, m_gameUserId{}, m_applicationId() {
  this->Initialize();
}

CStoveMgr::~CStoveMgr() {
  if (this->m_initialized) {
    Stove::PCSDK::Result result = IAP_UnInitialize();
    result.IsSuccessful();
    result = Base_UnInitialize();
    result.IsSuccessful();
    this->m_initialized = false;
  }
}

// GLOBAL: gl_24905
CStoveMgr* g_stoveMgr;

// OFFSET: 0x0063AF30
// FUNCTION: fn_04781
void ShutdownCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::Base::StovePCShutdown shutdown) const {
  if (callbackResult.GetResult().IsSuccessful()) {
    std::string message =
        EncodeText(CP_ACP, std::wstring(reinterpret_cast<const Stove::PCSDK::Base::StovePCLogEvent&>(shutdown).GetCategory1()));
    if (reinterpret_cast<const Stove::PCSDK::Base::StovePCToken&>(shutdown).GetAccessToken() == nullptr) {
      constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x91E147EBBD47C39FULL);
      const auto& captionSource = captionSourceText.data;
      const char* caption = DecryptName<0x91E147EBBD47C39FULL>(captionSource);
      MessageBoxA(hWnd, message.c_str(), caption, MB_OK);
      g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
    } else if (g_modeMgr.state == 0) {
      g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
    } else {
      UIBroadcastBalloon* balloonWindow = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->EnsureBroadcastBalloon();
      if (balloonWindow != nullptr) {
        BroadcastBalloonMessage balloon;
        balloon.lines.push_back(BroadcastBalloonLine{message, 0x62B0FF, 0});
        balloonWindow->AddMessage(0, balloon);
      }
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0x62B0FF, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(message.c_str()), 0x62B0FF, 0, 0, 0);
    }
  } else {
    constexpr auto captionSourceText = CRYPT_NAME("메시지", 0xA1A55B4D1597EDD3ULL);
    const auto& captionSource = captionSourceText.data;
    const char* caption = DecryptName<0xA1A55B4D1597EDD3ULL>(captionSource);
    MessageBoxA(hWnd, FormatOwnedString(TokenCallbackFailureMessage()().Text()).c_str(), caption, MB_OK);
    PostQuitMessage(0);
  }
}

// OFFSET: 0x0063B4B0
// FUNCTION: fn_04782
void FetchProductsCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products,
                                       unsigned int count) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    for (unsigned int index = 0; index < count; ++index) {
      const Stove::PCSDK::IAP::StovePCProduct& product = products[index];
      ITEM_INFO item;
      item.identity[1] = 0xFFFFFFFF;
      item.prices[0] = static_cast<int>(product.GetSalePrice());
      item.prices[1] = static_cast<int>(product.GetSalePrice());
      item.identity[0] = 4;
      item.identified = 1;
      item.damaged = 0;
      item.refiningLevel = 0;
      item.productId = reinterpret_cast<const Stove::PCSDK::IAP::StovePCOrderProduct&>(product).GetProductId();
      std::string itemId = EncodeText(
          CP_ACP, std::wstring(reinterpret_cast<const wchar_t*>(reinterpret_cast<const Stove::PCSDK::IAP::StovePCShopCategory&>(product).GetDisplayNumber())));
      item.SetItemId(std::atoi(itemId.c_str()));
      char name[256];
      int capacity = 256;
      item.GetIdDisplayName(name, capacity, 0);
      item.SetName(name);
      item.identified = 1;
      if (product.GetPurchaseLimitCount() != 0 && product.GetSaleBeginDate() != 0 && product.GetSaleEndDate() != 0) {
        item.bindOnEquip = 8;
        const int guidQuantity = product.GetGuidQuantity();
        item.saleData[3] = product.GetPurchaseLimitCount() - guidQuantity;
        item.quantity = product.GetPurchaseLimitCount();
        item.saleData[1] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleBeginDate()))));
        item.saleData[2] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleEndDate()))));
        g_session.AddInventoryCategoryItem(item, 9);
      }
    }
  } else {
    constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x8B3FEB5B8BD5C3C3ULL);
    const auto& captionSource = captionSourceText.data;
    const char* caption = DecryptName<0x8B3FEB5B8BD5C3C3ULL>(captionSource);
    constexpr std::array<char, 56> messageSource = CRYPT_NAME("장애가 발생 하였습니다. STOVE 고객센터로 문의 바랍니다.", 0xF7530137154D8FE5ULL);
    const char* message = DecryptName<0xF7530137154D8FE5ULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063BA40
// FUNCTION: fn_04783
void PurchaseResultCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::IAP::StovePCPurchaseResult purchaseResult) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    if (reinterpret_cast<std::intptr_t>(reinterpret_cast<const Stove::PCSDK::IAP::StovePCChargeInfo&>(purchaseResult).GetChargeTypeName()) != 2) {
      CStoveMgr* stoveMgr = g_stoveMgr;
      const std::int64_t productId = reinterpret_cast<const Stove::PCSDK::IAP::StovePCOrderProduct&>(purchaseResult).GetProductId();
      stoveMgr->m_purchaseIdLow = static_cast<int>(productId);
      stoveMgr->m_purchaseIdHigh = static_cast<int>(productId >> 32);
      stoveMgr = g_stoveMgr;
      stoveMgr->m_purchaseUrl = static_cast<const std::string&>(
          EncodeText(CP_ACP, std::wstring(reinterpret_cast<const Stove::PCSDK::IAP::StovePCPurchasedProduct&>(purchaseResult).GetCategoryId())));
    }
  } else {
    constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x078333BB9725B7C9ULL);
    const auto& captionSource = captionSourceText.data;
    const char* caption = DecryptName<0x078333BB9725B7C9ULL>(captionSource);
    constexpr std::array<char, 56> messageSource = CRYPT_NAME("장애가 발생 하였습니다. STOVE 고객센터로 문의 바랍니다.", 0x0BEB63A5C905B3A7ULL);
    const char* message = DecryptName<0x0BEB63A5C905B3A7ULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063BD90
// FUNCTION: fn_04784
void CategoryProductsCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products,
                                          unsigned int count) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    g_session.ClearAllInventoryCategories();
    for (unsigned int index = 0; index < count; ++index) {
      const Stove::PCSDK::IAP::StovePCProduct& product = products[index];
      std::string categoryId = EncodeText(CP_ACP, std::wstring(product.GetCategoryId()));
      int list;
      if (g_stoveMgr->m_environment == 2) {
        if (std::strcmp(categoryId.c_str(), "50F317A54451454") == 0) {
          list = 0;
        } else if (std::strcmp(categoryId.c_str(), "0039EE31BF35469") == 0) {
          list = 1;
        } else if (std::strcmp(categoryId.c_str(), "F73534C437BF46E") == 0) {
          list = 2;
        } else if (std::strcmp(categoryId.c_str(), "098807579EF0460") == 0) {
          list = 3;
        } else if (std::strcmp(categoryId.c_str(), "6409085316FB443") == 0) {
          list = 4;
        } else if (std::strcmp(categoryId.c_str(), "B719A2340C6D4A6") == 0) {
          list = 5;
        } else if (std::strcmp(categoryId.c_str(), "F967B1811E85457") == 0) {
          list = 6;
        } else if (std::strcmp(categoryId.c_str(), "3C068812E673478") == 0) {
          list = 7;
        } else if (std::strcmp(categoryId.c_str(), "44C71118750747D") == 0) {
          list = 8;
        } else {
          const char captionSource[] = CRYPT_CHARS(7, "메시지", 0xC52FB5FBB94FF917ULL);
          const char* caption = DecryptName<0xC52FB5FBB94FF917ULL>(captionSource);
          constexpr std::array<char, 34> messageSource = CRYPT_NAME("등록되지 않은 카테고리입니다 (%s)", 0x17359BF953C367B3ULL);
          const char* message = DecryptName<0x17359BF953C367B3ULL>(messageSource);
          MessageBoxA(hWnd, FormatOwnedString(message, categoryId.c_str()).c_str(), caption, MB_OK);
          continue;
        }
      } else {
        if (std::strcmp(categoryId.c_str(), "6BD73E0487554C5") == 0) {
          list = 0;
        } else if (std::strcmp(categoryId.c_str(), "BA5C75F95797471") == 0) {
          list = 1;
        } else if (std::strcmp(categoryId.c_str(), "6749388AE5EA4AE") == 0) {
          list = 2;
        } else if (std::strcmp(categoryId.c_str(), "E5161FAD921442A") == 0) {
          list = 3;
        } else if (std::strcmp(categoryId.c_str(), "D46B0A499ED9426") == 0) {
          list = 4;
        } else if (std::strcmp(categoryId.c_str(), "0FDC62110703456") == 0) {
          list = 5;
        } else if (std::strcmp(categoryId.c_str(), "82F5E6BE682C4E8") == 0) {
          list = 6;
        } else if (std::strcmp(categoryId.c_str(), "DF69418CA95B465") == 0) {
          list = 7;
        } else if (std::strcmp(categoryId.c_str(), "14A6891F90E24D7") == 0) {
          list = 8;
        } else {
          constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x69FD1D9733D3CFC1ULL);
          const auto& captionSource = captionSourceText.data;
          const char* caption = DecryptName<0x69FD1D9733D3CFC1ULL>(captionSource);
          constexpr std::array<char, 34> messageSource = CRYPT_NAME("등록되지 않은 카테고리입니다 (%s)", 0x93E7A3A1F587CDC5ULL);
          const char* message = DecryptName<0x93E7A3A1F587CDC5ULL>(messageSource);
          MessageBoxA(hWnd, FormatOwnedString(message, categoryId.c_str()).c_str(), caption, MB_OK);
          continue;
        }
      }

      ITEM_INFO item;
      item.identity[1] = 0xFFFFFFFF;
      item.prices[0] = static_cast<int>(product.GetSalePrice());
      item.prices[1] = static_cast<int>(product.GetSalePrice());
      item.identity[0] = 4;
      item.identified = 1;
      item.damaged = 0;
      item.refiningLevel = 0;
      item.productId = reinterpret_cast<const Stove::PCSDK::IAP::StovePCOrderProduct&>(product).GetProductId();
      std::string itemId = EncodeText(
          CP_ACP, std::wstring(reinterpret_cast<const wchar_t*>(reinterpret_cast<const Stove::PCSDK::IAP::StovePCShopCategory&>(product).GetDisplayNumber())));
      item.SetItemId(std::atoi(itemId.c_str()));
      char name[256];
      int capacity = 256;
      item.GetIdDisplayName(name, capacity, 0);
      item.SetName(name);
      item.identified = 1;
      if (list == 8) {
        if (product.GetPurchaseLimitCount() != 0 && product.GetSaleBeginDate() != 0 && product.GetSaleEndDate() != 0) {
          item.bindOnEquip = 8;
          const int guidQuantity = product.GetGuidQuantity();
          item.saleData[3] = product.GetPurchaseLimitCount() - guidQuantity;
          item.quantity = product.GetPurchaseLimitCount();
          item.saleData[1] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleBeginDate()))));
          item.saleData[2] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleEndDate()))));
          g_session.AddInventoryCategoryItem(item, 9);
        }
      } else if (product.GetPurchaseLimitCount() == 0 && product.GetSaleBeginDate() == 0 && product.GetSaleEndDate() == 0) {
        item.bindOnEquip = static_cast<std::uint16_t>(list);
        g_session.AddInventoryCategoryItem(item, list);
      }
    }
  } else {
    const char captionSource[] = CRYPT_CHARS(7, "메시지", 0x8B65CD61012B5931ULL);
    const char* caption = DecryptName<0x8B65CD61012B5931ULL>(captionSource);
    constexpr std::array<char, 56> messageSource = CRYPT_NAME("장애가 발생 하였습니다. STOVE 고객센터로 문의 바랍니다.", 0xF5657D1F3DE5CDC7ULL);
    const char* message = DecryptName<0xF5657D1F3DE5CDC7ULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063CF60
// FUNCTION: fn_04786
EncryptedName<100, 0x3375B1C7DBB5FD55ULL>& AccessTokenFailureMessage::operator()() const {
  OBFUSCATE_STRING_BODY("장애가 발생 하였습니다. 게임을 재실행 해주세요. 문제가 지속될 경우, STOVE 고객센터로 문의 바랍니다.", 0x3375B1C7DBB5FD55ULL);
}

// OFFSET: 0x0063D060
// FUNCTION: fn_04787
EncryptedName<100, 0x151F77EBEBBB2147ULL>& UserFailureMessage::operator()() const {
  OBFUSCATE_STRING_BODY("장애가 발생 하였습니다. 게임을 재실행 해주세요. 문제가 지속될 경우, STOVE 고객센터로 문의 바랍니다.", 0x151F77EBEBBB2147ULL);
}

// OFFSET: 0x0063D160
// FUNCTION: fn_04788
EncryptedName<100, 0x4F1791FB1503C713ULL>& TokenCallbackFailureMessage::operator()() const {
  OBFUSCATE_STRING_BODY("장애가 발생 하였습니다. 게임을 재실행 해주세요. 문제가 지속될 경우, STOVE 고객센터로 문의 바랍니다.", 0x4F1791FB1503C713ULL);
}

// OFFSET: 0x0063D260
// FUNCTION: fn_04789
void InitializeCallback::operator()(Stove::PCSDK::CallbackResult callbackResult) const {
  g_stoveWaitingCallback = false;
  if (!callbackResult.GetResult().IsSuccessful()) {
    const char captionSource[] = CRYPT_CHARS(7, "메시지", 0x31D7C1EDA345E36DULL);
    const char* caption = DecryptName<0x31D7C1EDA345E36DULL>(captionSource);
    const char messageSource[] = CRYPT_CHARS(45, "필수 사전 작업이 실패하여 게임을 종료합니다.", 0xD3755DD95F47A909ULL);
    const char* message = DecryptName<0xD3755DD95F47A909ULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
    PostQuitMessage(0);
  }
}

// OFFSET: 0x0063D490
// FUNCTION: fn_04790
EncryptedName<129, 0x2D3B15774149D5DDULL>& PurchaseCheckingMessage::operator()() const {
  OBFUSCATE_STRING_BODY("\xBE\xC6\xC0\xCC\xC5\xDB \xB1\xB8\xB8\xC5 \xC8\xAE\xC0\xCE \xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9....\x0A\x0A\x0A^ff0000\xC1\xD6\xC0\xC7! Stove \xC6\xE4\xC0\xCC\xC1\xF6\xBF\xA1\xBC\xAD \xB1\xB8\xB8\xC5 \xBF\xCF\xB7\xE1 \xC8\xC4 [\xC8\xAE\xC0\xCE] \xB9\xF6\xC6\xB0\xC0\xBB \xB4\xAD\xB7\xAF\xBE\xDF \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xCC \xC1\xA4\xBB\xF3 \xC1\xF6\xB1\xDE\xB5\xCB\xB4\xCF\xB4\xD9.^000000", 0x2D3B15774149D5DDULL);
}

// OFFSET: 0x0063D5D0
// FUNCTION: fn_04791
void ConfirmPurchaseCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int productCount, unsigned int count, int remainingCount,
                                         int resultCode) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    if (count > 0 && purchased == true) {
      g_modeMgr.GetGameMode()->SendMsg(MM_STOVE_AUTHENTICATION, 4, 0, 0, 0);
      g_stoveMgr->FetchAllProducts();
      UIWindow* window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND);
      if (window != nullptr) {
        window->SendMsg(nullptr, UM_UPDATE_CASHSHOP_BALANCE, 0, 0, 0, 0);
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
    } else {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      constexpr auto noticeSourceText = CRYPT_NAME("구매된 아이템이 없습니다.", 0x61FF87ED7BF39BE1ULL);
      const auto& noticeSource = noticeSourceText.data;
      const char* notice = DecryptName<0x61FF87ED7BF39BE1ULL>(noticeSource);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice), 0xFFE6E6, 0, 0, 0);
    }
  } else {
    constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x2D394DBD0F332733ULL);
    const auto& captionSource = captionSourceText.data;
    const char* caption = DecryptName<0x2D394DBD0F332733ULL>(captionSource);
    constexpr auto messageSourceText = CRYPT_NAME("장애가 발생 하였습니다. STOVE 고객센터로 문의 바랍니다.", 0x7D993F650D2785FFULL);
    const auto& messageSource = messageSourceText.data;
    const char* message = DecryptName<0x7D993F650D2785FFULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063DB80
// FUNCTION: fn_04794
void CSession::AddInventoryCategoryItem(const ITEM_INFO& item, int list) {
  if (static_cast<unsigned int>(list) <= 10) {
    this->inventoryCategories[list].push_back(item);
  }
}

// OFFSET: 0x0063DC50
// FUNCTION: fn_04795
bool CStoveMgr::Initialize() {
  if (this->m_initialized == true) {
    return false;
  }

  Stove::PCSDK::Base::StovePCInitializeParam param;
  constexpr auto sandboxSourceText = CRYPT_NAME("/stove_sandbox", 0x490FFB43A12F0DB9ULL);
  const auto& sandboxSource = sandboxSourceText.data;
  const char* sandbox = DecryptName<0x490FFB43A12F0DB9ULL>(sandboxSource);
  if (std::strstr(g_commandLine, sandbox) != nullptr) {
    this->m_environment = 1;
    param.SetEnvironment(L"SANDBOX");
    reinterpret_cast<Stove::PCSDK::Base::StovePCLogEvent&>(param).SetCategory1(L"SB_RAGNAROK_IND");
    param.SetApplicationKey(L"6b57af29764992a85cf793b01d4af15be6047715fd5f4687b23d0a1ba02337f5");
    this->m_applicationId = L"SB__SH_IPJVFX";
  } else {
    constexpr auto liveSourceText = CRYPT_NAME("/stove_live", 0x91651D81F559F971ULL);
    const auto& liveSource = liveSourceText.data;
    const char* live = DecryptName<0x91651D81F559F971ULL>(liveSource);
    if (std::strstr(g_commandLine, live) != nullptr) {
      this->m_environment = 2;
      param.SetEnvironment(L"LIVE");
      reinterpret_cast<Stove::PCSDK::Base::StovePCLogEvent&>(param).SetCategory1(L"RAGNAROK_IND");
      param.SetApplicationKey(L"9af21dbb708609cd29703f56d002c64ab6f8f5bf0a310388f65cf781a5e8dd8f");
      this->m_applicationId = L"RAG_SH_BTLTNL";
    }
  }

  constexpr auto skipRestartSourceText = CRYPT_NAME("/stove_cag_tool", 0x73D1674B61DF7B0BULL);
  const auto& skipRestartSource = skipRestartSourceText.data;
  const char* skipRestart = DecryptName<0x73D1674B61DF7B0BULL>(skipRestartSource);
  if (std::strstr(g_commandLine, skipRestart) == nullptr && Base_RestartAppIfNecessary(&param) == true) {
    constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x0F0F8B6B8F9D19D1ULL);
    const auto& captionSource = captionSourceText.data;
    const char* caption = DecryptName<0x0F0F8B6B8F9D19D1ULL>(captionSource);
    constexpr std::array<char, 38> messageSource = CRYPT_NAME("Stove 클라이언트를 통해 실행해 주세요", 0xD98F03A52D35636DULL);
    const char* message = DecryptName<0xD98F03A52D35636DULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
    return false;
  }

  Base_Initialize(&param, OnStoveInitialized);
  this->WaitForCallback();
  Stove::PCSDK::Result result = IAP_Initialize(this->m_applicationId.c_str());
  if (result.IsSuccessful()) {
    this->FetchUser();
    Base_GetShutdown(OnStoveShutdown);
    this->m_initialized = true;
    return true;
  }

  constexpr auto captionSourceText = CRYPT_NAME("메시지", 0x812BE72BCD5707B7ULL);
  const auto& captionSource = captionSourceText.data;
  const char* caption = DecryptName<0x812BE72BCD5707B7ULL>(captionSource);
  constexpr std::array<char, 45> messageSource = CRYPT_NAME("필수 사전 작업이 실패하여 게임을 종료합니다.", 0x59278F63D79F3DA1ULL);
  const char* message = DecryptName<0x59278F63D79F3DA1ULL>(messageSource);
  MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  return false;
}

// OFFSET: 0x0063E390
// FUNCTION: fn_04796
const char* CStoveMgr::GetAccessToken() {
  wchar_t accessToken[600];
  std::memset(accessToken, 0, sizeof(accessToken));
  Stove::PCSDK::Result result = Base_GetAccessToken(accessToken, 600);
  if (result.IsSuccessful()) {
    std::string token = EncodeText(CP_ACP, std::wstring(accessToken));
    memcpy_s(this->m_accessToken, sizeof(this->m_accessToken), token.c_str(), std::min(sizeof(this->m_accessToken), token.size() + 1));
    return this->m_accessToken;
  }

  constexpr std::array<char, 7> source = CRYPT_NAME("메시지", 0x1D6D75EDE7CD311FULL);
  thread_local EncryptedName<sizeof(source), 0x1D6D75EDE7CD311FULL> caption(source);
  MessageBoxA(hWnd, FormatOwnedString(AccessTokenFailureMessage()().Text()).c_str(), caption.Text(), MB_OK);
  PostQuitMessage(0);
  return nullptr;
}

// OFFSET: 0x0063E700
// FUNCTION: fn_04797
void CStoveMgr::FetchUser() {
  Stove::PCSDK::Base::StovePCUser user;
  Stove::PCSDK::Result result = Base_GetUser(&user);
  if (result.IsSuccessful()) {
    StringFormat(this->m_memberNumber, sizeof(this->m_memberNumber), _TRUNCATE, "%llu", user.GetMemberNumber());
    StringFormat(this->m_gameUserId, sizeof(this->m_gameUserId), _TRUNCATE, "%llu", user.GetGameUserId());
  } else {
    constexpr auto sourceText = CRYPT_NAME("메시지", 0xD1538513DF975701ULL);
    const auto& source = sourceText.data;
    thread_local EncryptedName<sizeof(source), 0xD1538513DF975701ULL> caption(source);
    MessageBoxA(hWnd, FormatOwnedString(UserFailureMessage()().Text()).c_str(), caption.Text(), MB_OK);
    PostQuitMessage(0);
  }
}

// OFFSET: 0x0063E9E0
// FUNCTION: fn_04799
void CStoveMgr::StartPurchase(const std::vector<ITEM_INFO>& items) {
  Stove::PCSDK::IAP::StovePCStartPurchaseParam param;
  param.CreateOrderProduct(static_cast<unsigned int>(items.size()));
  int index = 0;
  for (const ITEM_INFO& item : items) {
    Stove::PCSDK::IAP::StovePCOrderProduct orderProduct;
    orderProduct.SetProductId(item.productId);
    reinterpret_cast<Stove::PCSDK::IAP::StovePCChargeInfo&>(orderProduct).SetChargeDisplayDeductVal(static_cast<int>(item.prices[0]));
    reinterpret_cast<Stove::PCSDK::IAP::StovePCChargeInfo&>(orderProduct).SetChargeType(item.quantity);
    param.SetOrderProduct(index, &orderProduct);
    ++index;
  }

  Stove::PCSDK::IAP::StovePCPaymentOption option;
  option.SetOperation(Stove::PCSDK::IAP::StovePCPaymentOperation::DEFAULT);
  param.SetPurchaseOption(option);
  IAP_StartPurchase(&param, OnStovePurchaseStarted);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);

  if (this->m_purchaseUrl.size() == 0) {
    constexpr auto parametersSourceText = CRYPT_NAME("https://cs.onstove.com/service/RAGNAROK_IND", 0x69D345DB950F336DULL);
    const auto& parametersSource = parametersSourceText.data;
    const char* parameters = DecryptName<0x69D345DB950F336DULL>(parametersSource);
    constexpr auto fileSourceText = CRYPT_NAME("https://cs.onstove.com/service/RAGNAROK_IND", 0xC7912587FFF31B71ULL);
    const auto& fileSource = fileSourceText.data;
    const char* file = DecryptName<0xC7912587FFF31B71ULL>(fileSource);
    constexpr auto verbSourceText = CRYPT_NAME("OPEN", 0x8F1B5BFD5F6175B5ULL);
    const auto& verbSource = verbSourceText.data;
    const char* verb = DecryptName<0x8F1B5BFD5F6175B5ULL>(verbSource);
    ShellExecuteA(nullptr, verb, file, parameters, nullptr, SW_SHOWNORMAL);
  } else {
    const char* url = this->m_purchaseUrl.c_str();
    constexpr auto verbSourceText = CRYPT_NAME("OPEN", 0x9B17C9EFDBB39F2FULL);
    const auto& verbSource = verbSourceText.data;
    const char* verb = DecryptName<0x9B17C9EFDBB39F2FULL>(verbSource);
    ShellExecuteA(nullptr, verb, url, url, nullptr, SW_SHOWNORMAL);
    if (g_windowMgr.Confirm(PurchaseCheckingMessage()().Text(), 2, true, false, 0, "Message", 250, 170, 0) == 187) {
      IAP_ConfirmPurchase((static_cast<std::int64_t>(this->m_purchaseIdHigh) << 32) | static_cast<std::uint32_t>(this->m_purchaseIdLow), OnStovePurchaseConfirmed);
      g_stoveWaitingCallback = true;
      do {
        Base_RunCallback();
        Sleep(200);
      } while (g_stoveWaitingCallback);
    }
  }
}

// OFFSET: 0x0063F090
// FUNCTION: fn_04802
void CStoveMgr::WaitForCallback() {
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

// OFFSET: 0x0063CD20
// FUNCTION: fn_04785
void TermsAgreementCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, int unused, const wchar_t* url) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    g_modeMgr.GetGameMode()->SendMsg(MM_REQUEST_CASH_SHOP, 0, 0, 0, 0);
  } else {
    std::string path = EncodeText(CP_ACP, std::wstring(url));
    ShellExecuteA(nullptr, [] {
      constexpr auto sourceText = CRYPT_NAME("OPEN", 0x2FF3C5036D070127ULL);
      const auto& source = sourceText.data;
      thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> verb(source);
      return verb.Text();
    }(), path.c_str(), path.c_str(), nullptr, SW_SHOWNORMAL);
  }
}
