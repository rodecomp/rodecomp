#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Client/Stove/StoveMgr.h"
#include "Client/Stove/StovePCSDK.h"
#include "Network/TextEncoding.h"
#include "UI/UIBroadcastBalloon.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
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

void sub_63A7F0(Stove::PCSDK::CallbackResult callbackResult);
void sub_63A8D0(Stove::PCSDK::CallbackResult callbackResult);
void sub_63A860(Stove::PCSDK::CallbackResult callbackResult);
void sub_DD8A20(int value);
void sub_63A780(Stove::PCSDK::CallbackResult callbackResult);
void sub_63AA50(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::Base::StovePCShutdown shutdown);
void sub_63A940(Stove::PCSDK::CallbackResult callbackResult, Stove::PCSDK::IAP::StovePCPurchaseResult purchaseResult);
void sub_63A9D0(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int unknown1, unsigned int count, int unknown2, int unknown3);

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
  IAP_FetchProducts(&allParam, sub_63A860);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);

  g_session.ClearItems1784(9);
  Stove::PCSDK::IAP::StovePCFetchProductParam categoryParam;
  categoryParam.SetCategoryId(this->m_environment == 2 ? g_stoveCategoryIdA : g_stoveCategoryIdB);
  categoryParam.SetPageNumber(1);
  categoryParam.SetPageSize(0x7FFFFFFF);
  IAP_FetchProducts(&categoryParam, sub_63A8D0);
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
  IAP_FetchTermsAgreement(&option, sub_63A7F0);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

// OFFSET: 0x0063E910
// FUNCTION: fn_45460
void CStoveMgr::FetchCategoryProducts() {
  g_session.ClearItems1784(9);
  Stove::PCSDK::IAP::StovePCFetchProductParam param;
  param.SetCategoryId(this->m_environment == 2 ? g_stoveCategoryIdA : g_stoveCategoryIdB);
  param.SetPageNumber(1);
  param.SetPageSize(0x7FFFFFFF);
  IAP_FetchProducts(&param, sub_63A8D0);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
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
  void operator()(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int unknown1, unsigned int count, int unknown2, int unknown3) const;
};


// OFFSET: 0x0063AAE0
// FUNCTION: fn_04778
CStoveMgr::CStoveMgr()
    : m_initialized(false), unknown_0008(0), m_environment(1), unknown_0010(0), unknown_0014(0), unknown_0018(), m_accessToken{},
      m_memberNumber{}, m_gameUserId{}, unknown_02B8() {
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
      const char captionSource[] = {'\x27', '\x1D', '\xFA', '\x7E', '\x2A', '\xB1', '\xE1'};
      const char* caption = DecryptName<0x91E147EBBD47C39FULL>(captionSource);
      MessageBoxA(hWnd, message.c_str(), caption, MB_OK);
      g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
    } else if (g_modeMgr.state == 0) {
      g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
    } else {
      UIBroadcastBalloon* balloonWindow = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->EnsureBroadcastBalloon();
      if (balloonWindow != nullptr) {
        BroadcastBalloonMessage balloon{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
        balloon.start = std::chrono::system_clock::now();
        balloon.lines.push_back(BroadcastBalloonLine{message, 0x62B0FF, 0});
        balloonWindow->AddMessage(0, balloon);
      }
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0x62B0FF, 0, 0, 0);
      g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(message.c_str()), 0x62B0FF, 0, 0, 0);
    }
  } else {
    const char captionSource[] = {'\x6B', '\x33', '\x2A', '\xD6', '\x8C', '\xAD', '\xA5'};
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
      item.unknown_0000[1] = 0xFFFFFFFF;
      item.unknown_0014[0] = static_cast<int>(product.GetSalePrice());
      item.unknown_0014[1] = static_cast<int>(product.GetSalePrice());
      item.unknown_0000[0] = 4;
      item.identified = 1;
      item.unknown_005D = 0;
      item.unknown_0060 = 0;
      item.unknown_0090 = reinterpret_cast<const Stove::PCSDK::IAP::StovePCOrderProduct&>(product).GetProductId();
      std::string itemId = EncodeText(
          CP_ACP, std::wstring(reinterpret_cast<const wchar_t*>(reinterpret_cast<const Stove::PCSDK::IAP::StovePCShopCategory&>(product).GetDisplayNumber())));
      item.SetItemId(std::atoi(itemId.c_str()));
      char name[256];
      int capacity = 256;
      item.sub_6D5810(name, capacity, 0);
      item.SetName(name);
      item.identified = 1;
      if (product.GetPurchaseLimitCount() != 0 && product.GetSaleBeginDate() != 0 && product.GetSaleEndDate() != 0) {
        item.unknown_0064 = 8;
        const int guidQuantity = product.GetGuidQuantity();
        item.unknown_0078[3] = product.GetPurchaseLimitCount() - guidQuantity;
        item.quantity = product.GetPurchaseLimitCount();
        item.unknown_0078[1] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleBeginDate()))));
        item.unknown_0078[2] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleEndDate()))));
        g_session.AddItem1784(item, 9);
      }
    }
  } else {
    const char captionSource[] = {'\x7B', '\x1D', '\x68', '\x48', '\x9A', '\x1D', '\x3F'};
    const char* caption = DecryptName<0x8B3FEB5B8BD5C3C3ULL>(captionSource);
    const char messageSource[] = {'\x25', '\x6A', '\xF3', '\xC3', '\x87', '\xA0', '\x73', '\x4E', '\x3A', '\x34', '\xB0', '\x35', '\xF0', '\xCE', '\xEC', '\x43', '\x58', '\x4F', '\xF9', '\xDA', '\x83', '\xD8', '\x7D', '\xD7', '\xB6', '\xDB', '\x02', '\x43', '\x72', '\x21', '\xE3', '\x1A', '\x55', '\x3B', '\xF1', '\xAB', '\xF2', '\xCC', '\xE4', '\x39', '\xC5', '\x36', '\xE3', '\xD5', '\xF0', '\x21', '\xEA', '\x2E', '\x53', '\x77', '\xF9', '\xDA', '\x83', '\xD8', '\x7D', '\xF7'};
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
      const std::int64_t productId = reinterpret_cast<const Stove::PCSDK::IAP::StovePCOrderProduct&>(purchaseResult).GetProductId();
      g_stoveMgr->unknown_0010 = static_cast<int>(productId);
      g_stoveMgr->unknown_0014 = static_cast<int>(productId >> 32);
      g_stoveMgr->unknown_0018 = static_cast<const std::string&>(
          EncodeText(CP_ACP, std::wstring(reinterpret_cast<const Stove::PCSDK::IAP::StovePCPurchasedProduct&>(purchaseResult).GetCategoryId())));
    }
  } else {
    const char captionSource[] = {'\x71', '\x69', '\x98', '\x54', '\x7A', '\xC5', '\x83'};
    const char* caption = DecryptName<0x078333BB9725B7C9ULL>(captionSource);
    const char messageSource[] = {'\x67', '\x56', '\xBB', '\x1F', '\x15', '\xC2', '\xCB', '\xB2', '\x78', '\x08', '\xF8', '\xE9', '\x62', '\xAC', '\x54', '\xBF', '\x1A', '\x73', '\xB1', '\x06', '\x11', '\xBA', '\xC5', '\x2B', '\xF4', '\xE7', '\x4A', '\x9F', '\xE0', '\x43', '\x5B', '\xE6', '\x17', '\x07', '\xB9', '\x77', '\x60', '\xAE', '\x5C', '\xC5', '\x87', '\x0A', '\xAB', '\x09', '\x62', '\x43', '\x52', '\xD2', '\x11', '\x4B', '\xB1', '\x06', '\x11', '\xBA', '\xC5', '\x0B'};
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
    g_session.ClearAllItems1784();
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
          const char captionSource[] = {'\xAF', '\x27', '\xF2', '\x7A', '\x3A', '\x43', '\x2F'};
          const char* caption = DecryptName<0xC52FB5FBB94FF917ULL>(captionSource);
          const char messageSource[] = {'\x06', '\x89', '\x74', '\x9C', '\x4C', '\x5C', '\xF4', '\xE1', '\x93', '\xD9', '\x09', '\x93', '\x43', '\xBB', '\xF1', '\xBC', '\x76', '\xB0', '\x73', '\xBE', '\x41', '\x35', '\xF5', '\xC3', '\x07', '\xA8', '\x77', '\x8A', '\xD9', '\xB3', '\x10', '\x64', '\x9A', '\x67'};
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
          const char captionSource[] = {'\x79', '\x11', '\x6E', '\xF0', '\x56', '\xEB', '\xFD'};
          const char* caption = DecryptName<0x69FD1D9733D3CFC1ULL>(captionSource);
          const char messageSource[] = {'\x70', '\x23', '\x30', '\x3A', '\x14', '\x64', '\x26', '\x65', '\xE5', '\x73', '\x4D', '\x35', '\x1B', '\x83', '\x23', '\x38', '\x00', '\x1A', '\x37', '\x18', '\x19', '\x0D', '\x27', '\x47', '\x71', '\x02', '\x33', '\x2C', '\x81', '\x8B', '\xC2', '\xE0', '\xEC', '\xCD'};
          const char* message = DecryptName<0x93E7A3A1F587CDC5ULL>(messageSource);
          MessageBoxA(hWnd, FormatOwnedString(message, categoryId.c_str()).c_str(), caption, MB_OK);
          continue;
        }
      }

      ITEM_INFO item;
      item.unknown_0000[1] = 0xFFFFFFFF;
      item.unknown_0014[0] = static_cast<int>(product.GetSalePrice());
      item.unknown_0014[1] = static_cast<int>(product.GetSalePrice());
      item.unknown_0000[0] = 4;
      item.identified = 1;
      item.unknown_005D = 0;
      item.unknown_0060 = 0;
      item.unknown_0090 = reinterpret_cast<const Stove::PCSDK::IAP::StovePCOrderProduct&>(product).GetProductId();
      std::string itemId = EncodeText(
          CP_ACP, std::wstring(reinterpret_cast<const wchar_t*>(reinterpret_cast<const Stove::PCSDK::IAP::StovePCShopCategory&>(product).GetDisplayNumber())));
      item.SetItemId(std::atoi(itemId.c_str()));
      char name[256];
      int capacity = 256;
      item.sub_6D5810(name, capacity, 0);
      item.SetName(name);
      item.identified = 1;
      if (list == 8) {
        if (product.GetPurchaseLimitCount() != 0 && product.GetSaleBeginDate() != 0 && product.GetSaleEndDate() != 0) {
          item.unknown_0064 = 8;
          const int guidQuantity = product.GetGuidQuantity();
          item.unknown_0078[3] = product.GetPurchaseLimitCount() - guidQuantity;
          item.quantity = product.GetPurchaseLimitCount();
          item.unknown_0078[1] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleBeginDate()))));
          item.unknown_0078[2] = static_cast<std::uint32_t>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::milliseconds(product.GetSaleEndDate()))));
          g_session.AddItem1784(item, 9);
        }
      } else if (product.GetPurchaseLimitCount() == 0 && product.GetSaleBeginDate() == 0 && product.GetSaleEndDate() == 0) {
        item.unknown_0064 = static_cast<std::uint16_t>(list);
        g_session.AddItem1784(item, list);
      }
    }
  } else {
    const char captionSource[] = {'\x89', '\x87', '\x96', '\xC2', '\xA0', '\x3B', '\x65'};
    const char* caption = DecryptName<0x8B65CD61012B5931ULL>(captionSource);
    const char messageSource[] = {'\x07', '\x28', '\x5B', '\xEB', '\xAF', '\xDC', '\x45', '\x4C', '\x18', '\x76', '\x18', '\x1D', '\xD8', '\xB2', '\xDA', '\x41', '\x7A', '\x0D', '\x51', '\xF2', '\xAB', '\xA4', '\x4B', '\xD5', '\x94', '\x99', '\xAA', '\x6B', '\x5A', '\x5D', '\xD5', '\x18', '\x77', '\x79', '\x59', '\x83', '\xDA', '\xB0', '\xD2', '\x3B', '\xE7', '\x74', '\x4B', '\xFD', '\xD8', '\x5D', '\xDC', '\x2C', '\x71', '\x35', '\x51', '\xF2', '\xAB', '\xA4', '\x4B', '\xF5'};
    const char* message = DecryptName<0xF5657D1F3DE5CDC7ULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063CF60
// FUNCTION: fn_04786
EncryptedName<100, 0x3375B1C7DBB5FD55ULL>& AccessTokenFailureMessage::operator()() const {
  const char source[] = {'\x95', '\x18', '\x0B', '\x0D', '\x77', '\x10', '\x55', '\x8A', '\x8A', '\x46', '\x48', '\xFB', '\x00', '\x7E', '\xCA', '\x87', '\xE8', '\x3D', '\x01', '\x14', '\x73', '\x68', '\x5B', '\x13', '\xE5', '\x29', '\x75', '\x08', '\x07', '\x0A', '\x55', '\xF3', '\xB2', '\x40', '\x72', '\x1C', '\x27', '\x91', '\xB2', '\xEB', '\x94', '\x2B', '\x09', '\x67', '\x78', '\x55', '\x5B', '\x13', '\xEC', '\x53', '\x74', '\x7D', '\x77', '\x10', '\x55', '\xF2', '\xA3', '\x41', '\x66', '\x6E', '\x0E', '\x91', '\xC5', '\xD5', '\xEA', '\x11', '\x99', '\xFB', '\x94', '\xE5', '\x3A', '\x65', '\x10', '\xDD', '\x05', '\x36', '\x77', '\x05', '\xC9', '\x8D', '\x90', '\x30', '\x02', '\x15', '\xE7', '\x08', '\xDB', '\xF3', '\x92', '\xDD', '\x0C', '\x02', '\x71', '\x49', '\xC1', '\xFC', '\xE1', '\x24', '\x9B', '\xDB'};
  thread_local EncryptedName<sizeof(source), 0x3375B1C7DBB5FD55ULL> name(source);
  return name;
}

// OFFSET: 0x0063D060
// FUNCTION: fn_04787
EncryptedName<100, 0x151F77EBEBBB2147ULL>& UserFailureMessage::operator()() const {
  const char source[] = {'\x87', '\xC4', '\x05', '\x3D', '\x5B', '\xD6', '\x3F', '\xAC', '\x98', '\x9A', '\x46', '\xCB', '\x2C', '\xB8', '\xA0', '\xA1', '\xFA', '\xE1', '\x0F', '\x24', '\x5F', '\xAE', '\x31', '\x35', '\xF7', '\xF5', '\x7B', '\x38', '\x2B', '\xCC', '\x3F', '\xD5', '\xA0', '\x9C', '\x7C', '\x2C', '\x0B', '\x57', '\xD8', '\xCD', '\x86', '\xF7', '\x07', '\x57', '\x54', '\x93', '\x31', '\x35', '\xFE', '\x8F', '\x7A', '\x4D', '\x5B', '\xD6', '\x3F', '\xD4', '\xB1', '\x9D', '\x68', '\x5E', '\x22', '\x57', '\xAF', '\xF3', '\xF8', '\xCD', '\x97', '\xCB', '\xB8', '\x23', '\x50', '\x43', '\x02', '\x01', '\x0B', '\x06', '\x5B', '\xC3', '\xA3', '\xAB', '\x82', '\xEC', '\x0C', '\x25', '\xCB', '\xCE', '\xB1', '\xD5', '\x80', '\x01', '\x02', '\x32', '\x5D', '\x8F', '\xAB', '\xDA', '\xF3', '\xF8', '\x95', '\xEB'};
  thread_local EncryptedName<sizeof(source), 0x151F77EBEBBB2147ULL> name(source);
  return name;
}

// OFFSET: 0x0063D160
// FUNCTION: fn_04788
EncryptedName<100, 0x4F1791FB1503C713ULL>& TokenCallbackFailureMessage::operator()() const {
  const char source[] = {'\xD3', '\x22', '\xBD', '\xC3', '\x4B', '\x30', '\x37', '\xF6', '\xCC', '\x7C', '\xFE', '\x35', '\x3C', '\x5E', '\xA8', '\xFB', '\xAE', '\x07', '\xB7', '\xDA', '\x4F', '\x48', '\x39', '\x6F', '\xA3', '\x13', '\xC3', '\xC6', '\x3B', '\x2A', '\x37', '\x8F', '\xF4', '\x7A', '\xC4', '\xD2', '\x1B', '\xB1', '\xD0', '\x97', '\xD2', '\x11', '\xBF', '\xA9', '\x44', '\x75', '\x39', '\x6F', '\xAA', '\x69', '\xC2', '\xB3', '\x4B', '\x30', '\x37', '\x8E', '\xE5', '\x7B', '\xD0', '\xA0', '\x32', '\xB1', '\xA7', '\xA9', '\xAC', '\x2B', '\x2F', '\x35', '\xA8', '\xC5', '\x58', '\x19', '\x56', '\xE7', '\xB3', '\xF8', '\x4B', '\x25', '\xAB', '\xF1', '\xD6', '\x0A', '\xB4', '\xDB', '\xDB', '\x28', '\xB9', '\x8F', '\xD4', '\xE7', '\xBA', '\xCC', '\x4D', '\x69', '\xA3', '\x80', '\xA7', '\x1E', '\x2D', '\x15'};
  thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
  return name;
}

// OFFSET: 0x0063D260
// FUNCTION: fn_04789
void CStoveMgr::OnInitializeCallback(Stove::PCSDK::CallbackResult callbackResult) {
  g_stoveWaitingCallback = false;
  if (!callbackResult.GetResult().IsSuccessful()) {
    const char captionSource[] = {'\xD5', '\x3D', '\xF8', '\x60', '\x2C', '\x37', '\xD7'};
    const char* caption = DecryptName<0x31D7C1EDA345E36DULL>(captionSource);
    const char messageSource[] = {'\xCE', '\x63', '\xFB', '\xA9', '\xF9', '\xE6', '\x92', '\x13', '\xF5', '\x89', '\x87', '\x84', '\x67', '\xAA', '\xB5', '\x1F', '\x29', '\x14', '\x80', '\x99', '\x09', '\x9A', '\xBA', '\x6C', '\xA0', '\x89', '\xF7', '\x8B', '\x19', '\x8E', '\xB5', '\x68', '\x29', '\x68', '\xF9', '\xE8', '\x38', '\x9A', '\xA0', '\x67', '\xC6', '\x1D', '\x9E', '\x71', '\xD9'};
    const char* message = DecryptName<0xD3755DD95F47A909ULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
    PostQuitMessage(0);
  }
}

// OFFSET: 0x0063D490
// FUNCTION: fn_04790
EncryptedName<129, 0x2D3B15774149D5DDULL>& PurchaseCheckingMessage::operator()() const {
  const char source[] = {'\x63', '\x13', '\x89', '\x8D', '\xB2', '\xCE', '\x1B', '\x9C', '\x65', '\x6D', '\x8C', '\x61', '\xBF', '\xBB', '\xFB', '\xE3', '\xFD', '\x14', '\x96', '\x81', '\xA3', '\xA1', '\xF4', '\x99', '\x04', '\xFB', '\x67', '\x6F', '\x59', '\x1F', '\x31', '\x27', '\x83', '\xB3', '\x2F', '\x71', '\x47', '\x25', '\x0B', '\xEC', '\x0B', '\x15', '\x8E', '\x60', '\x57', '\x46', '\x4F', '\x42', '\xAB', '\xB0', '\x69', '\x87', '\x93', '\xD5', '\xF7', '\xEC', '\x2B', '\x6A', '\xE8', '\xFD', '\xDA', '\x35', '\x8A', '\x95', '\x65', '\x10', '\x69', '\xFE', '\xB8', '\xA2', '\xDA', '\x0D', '\x15', '\x11', '\x69', '\x1A', '\xBF', '\xBB', '\xFB', '\xE3', '\x80', '\xF5', '\xF0', '\xB7', '\xB1', '\xA5', '\xFB', '\x96', '\xFD', '\x61', '\xE4', '\xF6', '\xD8', '\xAB', '\xE4', '\x0D', '\x63', '\x13', '\x89', '\x8D', '\xB2', '\xCE', '\xFB', '\xE1', '\xFD', '\x14', '\xED', '\xFA', '\x84', '\x35', '\xFA', '\xDB', '\x6C', '\x0B', '\xFC', '\x8A', '\xC3', '\xDA', '\x8F', '\xF4', '\xF3', '\x8B', '\x79', '\x71', '\x47', '\x25', '\x0B', '\x1D', '\xDD'};
  thread_local EncryptedName<sizeof(source), 0x2D3B15774149D5DDULL> name(source);
  return name;
}

// OFFSET: 0x0063D5D0
// FUNCTION: fn_04791
void ConfirmPurchaseCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, bool purchased, int unknown1, unsigned int count, int unknown2,
                                         int unknown3) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    if (count != 0 && purchased == true) {
      g_modeMgr.GetGameMode()->SendMsg(331, 4, 0, 0, 0);
      g_stoveMgr->FetchAllProducts();
      UIWindow* window = g_windowMgr.LookupWindow(318);
      if (window != nullptr) {
        window->SendMsg(nullptr, 122, 0, 0, 0, 0);
        window->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
    } else {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      const char noticeSource[] = {'\x50', '\x23', '\x4B', '\xBE', '\x58', '\x4F', '\xDF', '\xDF', '\x27', '\x5B', '\x3F', '\xBE', '\x36', '\x47', '\x33', '\x41', '\x5F', '\x63', '\x4E', '\xBB', '\x59', '\x48', '\x4B', '\xB8', '\xCF', '\x9B'};
      const char* notice = DecryptName<0x61FF87ED7BF39BE1ULL>(noticeSource);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(notice), 0xFFE6E6, 0, 0, 0);
    }
  } else {
    const char captionSource[] = {'\x8B', '\xF9', '\x8E', '\xCC', '\x7C', '\xBB', '\x39'};
    const char* caption = DecryptName<0x2D394DBD0F332733ULL>(captionSource);
    const char messageSource[] = {'\x3F', '\x60', '\x99', '\xDB', '\xD5', '\x9E', '\xB9', '\xC4', '\x20', '\x3E', '\xDA', '\x2D', '\xA2', '\xF0', '\x26', '\xC9', '\x42', '\x45', '\x93', '\xC2', '\xD1', '\xE6', '\xB7', '\x5D', '\xAC', '\xD1', '\x68', '\x5B', '\x20', '\x1F', '\x29', '\x90', '\x4F', '\x31', '\x9B', '\xB3', '\xA0', '\xF2', '\x2E', '\xB3', '\xDF', '\x3C', '\x89', '\xCD', '\xA2', '\x1F', '\x20', '\xA4', '\x49', '\x7D', '\x93', '\xC2', '\xD1', '\xE6', '\xB7', '\x7D'};
    const char* message = DecryptName<0x7D993F650D2785FFULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063DB80
// FUNCTION: fn_04794
void CSession::AddItem1784(const ITEM_INFO& item, int list) {
  if (static_cast<unsigned int>(list) < 11) {
    this->unknown_1784[list].push_back(item);
  }
}

// OFFSET: 0x0063DC50
// FUNCTION: fn_04795
bool CStoveMgr::Initialize() {
  if (this->m_initialized == true) {
    return false;
  }

  Stove::PCSDK::Base::StovePCInitializeParam param;
  const char sandboxSource[] = {'\x96', '\x7E', '\x5B', '\xCE', '\x35', '\x9E', '\x50', '\x3A', '\xD8', '\x63', '\x4B', '\xC3', '\x2C', '\x83', '\x0F'};
  const char* sandbox = DecryptName<0x490FFB43A12F0DB9ULL>(sandboxSource);
  if (std::strstr(g_commandLine, sandbox) != nullptr) {
    this->m_environment = 1;
    param.SetEnvironment(L"SANDBOX");
    reinterpret_cast<Stove::PCSDK::Base::StovePCLogEvent&>(param).SetCategory1(L"SB_RAGNAROK_IND");
    param.SetApplicationKey(L"6b57af29764992a85cf793b01d4af15be6047715fd5f4687b23d0a1ba02337f5");
    this->unknown_02B8 = L"SB__SH_IPJVFX";
  } else {
    const char liveSource[] = {'\x5E', '\x8A', '\x2D', '\x9A', '\xF7', '\x78', '\x3A', '\xFD', '\x18', '\x8F', '\x3C', '\xF5'};
    const char* live = DecryptName<0x91651D81F559F971ULL>(liveSource);
    if (std::strstr(g_commandLine, live) != nullptr) {
      this->m_environment = 2;
      param.SetEnvironment(L"LIVE");
      reinterpret_cast<Stove::PCSDK::Base::StovePCLogEvent&>(param).SetCategory1(L"RAGNAROK_IND");
      param.SetApplicationKey(L"9af21dbb708609cd29703f56d002c64ab6f8f5bf0a310388f65cf781a5e8dd8f");
      this->unknown_02B8 = L"RAG_SH_BTLTNL";
    }
  }

  const char skipRestartSource[] = {'\x24', '\x08', '\xAB', '\x0E', '\x3D', '\x02', '\x8E', '\x10', '\x6A', '\x1C', '\x80', '\x15', '\x24', '\x08', '\xBD', '\x73'};
  const char* skipRestart = DecryptName<0x73D1674B61DF7B0BULL>(skipRestartSource);
  if (std::strstr(g_commandLine, skipRestart) == nullptr && Base_RestartAppIfNecessary(&param) == true) {
    const char captionSource[] = {'\x69', '\xC7', '\x20', '\x4C', '\xAA', '\x7D', '\x0F'};
    const char* caption = DecryptName<0x0F0F8B6B8F9D19D1ULL>(captionSource);
    const char messageSource[] = {'\x3E', '\x17', '\x5A', '\x5B', '\xC0', '\x23', '\x4A', '\x75', '\xDB', '\x90', '\xF5', '\xE1', '\x1B', '\xF3', '\x49', '\x77', '\xD5', '\xC5', '\x15', '\xE8', '\x4E', '\xC4', '\x57', '\xF9', '\xD0', '\xA4', '\xF2', '\xCD', '\x62', '\xDB', '\xAF', '\x18', '\xBB', '\xDF', '\x89', '\x92', '\x41', '\x03'};
    const char* message = DecryptName<0xD98F03A52D35636DULL>(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
    return false;
  }

  Base_Initialize(&param, sub_63A780);
  this->WaitForCallback();
  Stove::PCSDK::Result result = IAP_Initialize(this->unknown_02B8.c_str());
  if (result.IsSuccessful()) {
    this->FetchUser();
    Base_GetShutdown(sub_63AA50);
    this->m_initialized = true;
    return true;
  }

  const char captionSource[] = {'\x0F', '\xD9', '\xEA', '\x0E', '\xEA', '\x11', '\x2B'};
  const char* caption = DecryptName<0x812BE72BCD5707B7ULL>(captionSource);
  const char messageSource[] = {'\x66', '\xF7', '\x23', '\x21', '\x43', '\x34', '\xC0', '\x99', '\x5D', '\x1D', '\x5F', '\x0C', '\xDD', '\x78', '\xE7', '\x95', '\x81', '\x80', '\x58', '\x11', '\xB3', '\x48', '\xE8', '\xE6', '\x08', '\x1D', '\x2F', '\x03', '\xA3', '\x5C', '\xE7', '\xE2', '\x81', '\xFC', '\x21', '\x60', '\x82', '\x48', '\xF2', '\xED', '\x6E', '\x89', '\x46', '\xF9', '\x63'};
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

  const char source[] = {'\xA7', '\xEF', '\x70', '\x24', '\x2C', '\x83', '\x6D'};
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
    const char source[] = {'\xB9', '\x89', '\x2A', '\x1C', '\xD2', '\x73', '\x53'};
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
    orderProduct.SetProductId(item.unknown_0090);
    reinterpret_cast<Stove::PCSDK::IAP::StovePCChargeInfo&>(orderProduct).SetChargeDisplayDeductVal(static_cast<int>(item.unknown_0014[0]));
    reinterpret_cast<Stove::PCSDK::IAP::StovePCChargeInfo&>(orderProduct).SetChargeType(item.quantity);
    param.SetOrderProduct(index, &orderProduct);
    ++index;
  }

  Stove::PCSDK::IAP::StovePCPaymentOption option;
  option.SetOperation(Stove::PCSDK::IAP::StovePCPaymentOperation::DEFAULT);
  param.SetPurchaseOption(option);
  IAP_StartPurchase(&param, sub_63A940);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);

  if (this->unknown_0018.size() == 0) {
    const char parametersSource[] = {'\x05', '\x47', '\x7B', '\xE5', '\xA8', '\x7F', '\xFC', '\x46', '\x0E', '\x40', '\x21', '\xFA', '\xB5', '\x36', '\xA7', '\x06', '\x1B', '\x56', '\x21', '\xF6', '\xB4', '\x28', '\xFC', '\x1A', '\x08', '\x41', '\x79', '\xFC', '\xB8', '\x20', '\xFC', '\x3B', '\x2C', '\x74', '\x41', '\xD4', '\x89', '\x0A', '\x98', '\x36', '\x24', '\x7D', '\x4B', '\x95'};
    const char* parameters = DecryptName<0x69D345DB950F336DULL>(parametersSource);
    const char fileSource[] = {'\x19', '\x6F', '\x87', '\x8F', '\xF4', '\x1F', '\xBE', '\xE8', '\x12', '\x68', '\xDD', '\x90', '\xE9', '\x56', '\xE5', '\xA8', '\x07', '\x7E', '\xDD', '\x9C', '\xE8', '\x48', '\xBE', '\xB4', '\x14', '\x69', '\x85', '\x96', '\xE4', '\x40', '\xBE', '\x95', '\x30', '\x5C', '\xBD', '\xBE', '\xD5', '\x6A', '\xDA', '\x98', '\x38', '\x55', '\xB7', '\xFF'};
    const char* file = DecryptName<0xC7912587FFF31B71ULL>(fileSource);
    const char verbSource[] = {'\xFA', '\x25', '\x24', '\x11', '\xFD'};
    const char* verb = DecryptName<0x8F1B5BFD5F6175B5ULL>(verbSource);
    ShellExecuteA(nullptr, verb, file, parameters, nullptr, SW_SHOWNORMAL);
  } else {
    const char* url = this->unknown_0018.c_str();
    const char verbSource[] = {'\x60', '\xCF', '\xF6', '\x95', '\xEF'};
    const char* verb = DecryptName<0x9B17C9EFDBB39F2FULL>(verbSource);
    ShellExecuteA(nullptr, verb, url, url, nullptr, SW_SHOWNORMAL);
    if (g_windowMgr.Confirm(PurchaseCheckingMessage()().Text(), 2, true, false, 0, "Message", 250, 170, 0) == 187) {
      IAP_ConfirmPurchase((static_cast<std::int64_t>(this->unknown_0014) << 32) | static_cast<std::uint32_t>(this->unknown_0010), sub_63A9D0);
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
void CStoveMgr::OnOpenUrlCallback(Stove::PCSDK::CallbackResult callbackResult, int unused, const wchar_t* url) {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    g_modeMgr.GetGameMode()->SendMsg(323, 0, 0, 0, 0);
  } else {
    std::string path = EncodeText(CP_ACP, std::wstring(url));
    const char source[] = {'\x68', '\x51', '\x42', '\x23', '\x03'};
    thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> verb(source);
    ShellExecuteA(nullptr, verb.Text(), path.c_str(), path.c_str(), nullptr, SW_SHOWNORMAL);
  }
}
