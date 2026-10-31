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
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowMouseHit.h"
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

struct ClientItemLists {
  void ClearItems1784(int index);
};

// OFFSET: 0x0063EEE0
// FUNCTION: fn_04800
void CStoveMgr::FetchAllProducts() {
  Stove::PCSDK::IAP::StovePCFetchProductParam allParam;
  allParam.SetCategoryId(L"");
  allParam.SetPageNumber(1);
  allParam.SetPageSize(0x7FFFFFFF);
  Stove::PCSDK::IAP::FetchProducts(&allParam, sub_63A860);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);

  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1784(9);
  Stove::PCSDK::IAP::StovePCFetchProductParam categoryParam;
  categoryParam.SetCategoryId(this->m_environment == 2 ? g_stoveCategoryIdA : g_stoveCategoryIdB);
  categoryParam.SetPageNumber(1);
  categoryParam.SetPageSize(0x7FFFFFFF);
  Stove::PCSDK::IAP::FetchProducts(&categoryParam, sub_63A8D0);
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
  option.SetOperation(0);
  Stove::PCSDK::IAP::FetchTermsAgreement(&option, sub_63A7F0);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

// OFFSET: 0x0063E910
// FUNCTION: fn_45460
void CStoveMgr::FetchCategoryProducts() {
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1784(9);
  Stove::PCSDK::IAP::StovePCFetchProductParam param;
  param.SetCategoryId(this->m_environment == 2 ? g_stoveCategoryIdA : g_stoveCategoryIdB);
  param.SetPageNumber(1);
  param.SetPageSize(0x7FFFFFFF);
  Stove::PCSDK::IAP::FetchProducts(&param, sub_63A8D0);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);
}

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
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

struct SessionItemLists {
  void ClearItems1780();
};

// OFFSET: 0x0063AAE0
// FUNCTION: fn_04778
CStoveMgr::CStoveMgr()
    : vtable(nullptr), m_initialized(false), unknown_0008(0), m_environment(1), unknown_0010(0), unknown_0014(0), unknown_0018(), m_accessToken{},
      m_memberNumber{}, m_gameUserId{}, unknown_02B8() {
  this->vtable = reinterpret_cast<void*>(0x0110C63C);
  reinterpret_cast<unsigned char(__thiscall*)(CStoveMgr*)>(0x0063DC50)(this);
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
      const std::uint8_t captionSource[] = {0x27, 0x1D, 0xFA, 0x7E, 0x2A, 0xB1, 0xE1};
      thread_local EncryptedName<sizeof(captionSource), 0x91E147EBBD47C39FULL> captionName(captionSource);
      const char* caption = captionName.Text();
      MessageBoxA(hWnd, message.c_str(), caption, MB_OK);
      g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
    } else if (g_alternateDoubleClickGate == 0) {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
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
    const std::uint8_t captionSource[] = {0x6B, 0x33, 0x2A, 0xD6, 0x8C, 0xAD, 0xA5};
    thread_local EncryptedName<sizeof(captionSource), 0xA1A55B4D1597EDD3ULL> captionName(captionSource);
    const char* caption = captionName.Text();
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
        item.unknown_0078[1] = static_cast<std::uint32_t>(product.GetSaleBeginDate() * 10000 / 10000000);
        item.unknown_0078[2] = static_cast<std::uint32_t>(product.GetSaleEndDate() * 10000 / 10000000);
        g_session.AddItem1784(item, 9);
      }
    }
  } else {
    const std::uint8_t captionSource[] = {0x7B, 0x1D, 0x68, 0x48, 0x9A, 0x1D, 0x3F};
    thread_local EncryptedName<sizeof(captionSource), 0x8B3FEB5B8BD5C3C3ULL> caption(captionSource);
    const std::uint8_t messageSource[] = {0x25, 0x6A, 0xF3, 0xC3, 0x87, 0xA0, 0x73, 0x4E, 0x3A, 0x34, 0xB0, 0x35, 0xF0, 0xCE, 0xEC, 0x43,
                                          0x58, 0x4F, 0xF9, 0xDA, 0x83, 0xD8, 0x7D, 0xD7, 0xB6, 0xDB, 0x02, 0x43, 0x72, 0x21, 0xE3, 0x1A,
                                          0x55, 0x3B, 0xF1, 0xAB, 0xF2, 0xCC, 0xE4, 0x39, 0xC5, 0x36, 0xE3, 0xD5, 0xF0, 0x21, 0xEA, 0x2E,
                                          0x53, 0x77, 0xF9, 0xDA, 0x83, 0xD8, 0x7D, 0xF7};
    thread_local EncryptedName<sizeof(messageSource), 0xF7530137154D8FE5ULL> message(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message.Text()).c_str(), caption.Text(), MB_OK);
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
    const std::uint8_t captionSource[] = {0x71, 0x69, 0x98, 0x54, 0x7A, 0xC5, 0x83};
    thread_local EncryptedName<sizeof(captionSource), 0x078333BB9725B7C9ULL> caption(captionSource);
    const std::uint8_t messageSource[] = {0x67, 0x56, 0xBB, 0x1F, 0x15, 0xC2, 0xCB, 0xB2, 0x78, 0x08, 0xF8, 0xE9, 0x62, 0xAC, 0x54, 0xBF,
                                          0x1A, 0x73, 0xB1, 0x06, 0x11, 0xBA, 0xC5, 0x2B, 0xF4, 0xE7, 0x4A, 0x9F, 0xE0, 0x43, 0x5B, 0xE6,
                                          0x17, 0x07, 0xB9, 0x77, 0x60, 0xAE, 0x5C, 0xC5, 0x87, 0x0A, 0xAB, 0x09, 0x62, 0x43, 0x52, 0xD2,
                                          0x11, 0x4B, 0xB1, 0x06, 0x11, 0xBA, 0xC5, 0x0B};
    thread_local EncryptedName<sizeof(messageSource), 0x0BEB63A5C905B3A7ULL> message(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message.Text()).c_str(), caption.Text(), MB_OK);
  }
}

// OFFSET: 0x0063BD90
// FUNCTION: fn_04784
void CategoryProductsCallback::operator()(Stove::PCSDK::CallbackResult callbackResult, const Stove::PCSDK::IAP::StovePCProduct* products,
                                          unsigned int count) const {
  g_stoveWaitingCallback = false;
  if (callbackResult.GetResult().IsSuccessful()) {
    reinterpret_cast<SessionItemLists*>(&g_session)->ClearItems1780();
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
          const std::uint8_t captionSource[] = {0xAF, 0x27, 0xF2, 0x7A, 0x3A, 0x43, 0x2F};
          thread_local EncryptedName<sizeof(captionSource), 0xC52FB5FBB94FF917ULL> captionName(captionSource);
          const char* caption = captionName.Text();
          const std::uint8_t messageSource[] = {0x06, 0x89, 0x74, 0x9C, 0x4C, 0x5C, 0xF4, 0xE1, 0x93, 0xD9, 0x09, 0x93, 0x43, 0xBB, 0xF1, 0xBC,
                                                0x76, 0xB0, 0x73, 0xBE, 0x41, 0x35, 0xF5, 0xC3, 0x07, 0xA8, 0x77, 0x8A, 0xD9, 0xB3, 0x10, 0x64,
                                                0x9A, 0x67};
          thread_local EncryptedName<sizeof(messageSource), 0x17359BF953C367B3ULL> messageName(messageSource);
          const char* message = messageName.Text();
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
          const std::uint8_t captionSource[] = {0x79, 0x11, 0x6E, 0xF0, 0x56, 0xEB, 0xFD};
          thread_local EncryptedName<sizeof(captionSource), 0x69FD1D9733D3CFC1ULL> captionName(captionSource);
          const char* caption = captionName.Text();
          const std::uint8_t messageSource[] = {0x70, 0x23, 0x30, 0x3A, 0x14, 0x64, 0x26, 0x65, 0xE5, 0x73, 0x4D, 0x35, 0x1B, 0x83, 0x23, 0x38,
                                                0x00, 0x1A, 0x37, 0x18, 0x19, 0x0D, 0x27, 0x47, 0x71, 0x02, 0x33, 0x2C, 0x81, 0x8B, 0xC2, 0xE0,
                                                0xEC, 0xCD};
          thread_local EncryptedName<sizeof(messageSource), 0x93E7A3A1F587CDC5ULL> messageName(messageSource);
          const char* message = messageName.Text();
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
          item.unknown_0078[1] = static_cast<std::uint32_t>(product.GetSaleBeginDate() * 10000 / 10000000);
          item.unknown_0078[2] = static_cast<std::uint32_t>(product.GetSaleEndDate() * 10000 / 10000000);
          g_session.AddItem1784(item, 9);
        }
      } else if (product.GetPurchaseLimitCount() == 0 && product.GetSaleBeginDate() == 0 && product.GetSaleEndDate() == 0) {
        item.unknown_0064 = static_cast<std::uint16_t>(list);
        g_session.AddItem1784(item, list);
      }
    }
  } else {
    const std::uint8_t captionSource[] = {0x89, 0x87, 0x96, 0xC2, 0xA0, 0x3B, 0x65};
    thread_local EncryptedName<sizeof(captionSource), 0x8B65CD61012B5931ULL> captionName(captionSource);
    const char* caption = captionName.Text();
    const std::uint8_t messageSource[] = {0x07, 0x28, 0x5B, 0xEB, 0xAF, 0xDC, 0x45, 0x4C, 0x18, 0x76, 0x18, 0x1D, 0xD8, 0xB2, 0xDA, 0x41,
                                          0x7A, 0x0D, 0x51, 0xF2, 0xAB, 0xA4, 0x4B, 0xD5, 0x94, 0x99, 0xAA, 0x6B, 0x5A, 0x5D, 0xD5, 0x18,
                                          0x77, 0x79, 0x59, 0x83, 0xDA, 0xB0, 0xD2, 0x3B, 0xE7, 0x74, 0x4B, 0xFD, 0xD8, 0x5D, 0xDC, 0x2C,
                                          0x71, 0x35, 0x51, 0xF2, 0xAB, 0xA4, 0x4B, 0xF5};
    thread_local EncryptedName<sizeof(messageSource), 0xF5657D1F3DE5CDC7ULL> messageName(messageSource);
    const char* message = messageName.Text();
    MessageBoxA(hWnd, FormatOwnedString(message).c_str(), caption, MB_OK);
  }
}

// OFFSET: 0x0063CF60
// FUNCTION: fn_04786
EncryptedName<100, 0x3375B1C7DBB5FD55ULL>& AccessTokenFailureMessage::operator()() const {
  const std::uint8_t source[] = {0x95, 0x18, 0x0B, 0x0D, 0x77, 0x10, 0x55, 0x8A, 0x8A, 0x46, 0x48, 0xFB, 0x00, 0x7E, 0xCA, 0x87,
                                 0xE8, 0x3D, 0x01, 0x14, 0x73, 0x68, 0x5B, 0x13, 0xE5, 0x29, 0x75, 0x08, 0x07, 0x0A, 0x55, 0xF3,
                                 0xB2, 0x40, 0x72, 0x1C, 0x27, 0x91, 0xB2, 0xEB, 0x94, 0x2B, 0x09, 0x67, 0x78, 0x55, 0x5B, 0x13,
                                 0xEC, 0x53, 0x74, 0x7D, 0x77, 0x10, 0x55, 0xF2, 0xA3, 0x41, 0x66, 0x6E, 0x0E, 0x91, 0xC5, 0xD5,
                                 0xEA, 0x11, 0x99, 0xFB, 0x94, 0xE5, 0x3A, 0x65, 0x10, 0xDD, 0x05, 0x36, 0x77, 0x05, 0xC9, 0x8D,
                                 0x90, 0x30, 0x02, 0x15, 0xE7, 0x08, 0xDB, 0xF3, 0x92, 0xDD, 0x0C, 0x02, 0x71, 0x49, 0xC1, 0xFC,
                                 0xE1, 0x24, 0x9B, 0xDB};
  thread_local EncryptedName<sizeof(source), 0x3375B1C7DBB5FD55ULL> name(source);
  return name;
}

// OFFSET: 0x0063D060
// FUNCTION: fn_04787
EncryptedName<100, 0x151F77EBEBBB2147ULL>& UserFailureMessage::operator()() const {
  const std::uint8_t source[] = {0x87, 0xC4, 0x05, 0x3D, 0x5B, 0xD6, 0x3F, 0xAC, 0x98, 0x9A, 0x46, 0xCB, 0x2C, 0xB8, 0xA0, 0xA1,
                                 0xFA, 0xE1, 0x0F, 0x24, 0x5F, 0xAE, 0x31, 0x35, 0xF7, 0xF5, 0x7B, 0x38, 0x2B, 0xCC, 0x3F, 0xD5,
                                 0xA0, 0x9C, 0x7C, 0x2C, 0x0B, 0x57, 0xD8, 0xCD, 0x86, 0xF7, 0x07, 0x57, 0x54, 0x93, 0x31, 0x35,
                                 0xFE, 0x8F, 0x7A, 0x4D, 0x5B, 0xD6, 0x3F, 0xD4, 0xB1, 0x9D, 0x68, 0x5E, 0x22, 0x57, 0xAF, 0xF3,
                                 0xF8, 0xCD, 0x97, 0xCB, 0xB8, 0x23, 0x50, 0x43, 0x02, 0x01, 0x0B, 0x06, 0x5B, 0xC3, 0xA3, 0xAB,
                                 0x82, 0xEC, 0x0C, 0x25, 0xCB, 0xCE, 0xB1, 0xD5, 0x80, 0x01, 0x02, 0x32, 0x5D, 0x8F, 0xAB, 0xDA,
                                 0xF3, 0xF8, 0x95, 0xEB};
  thread_local EncryptedName<sizeof(source), 0x151F77EBEBBB2147ULL> name(source);
  return name;
}

// OFFSET: 0x0063D160
// FUNCTION: fn_04788
EncryptedName<100, 0x4F1791FB1503C713ULL>& TokenCallbackFailureMessage::operator()() const {
  const std::uint8_t source[] = {0xD3, 0x22, 0xBD, 0xC3, 0x4B, 0x30, 0x37, 0xF6, 0xCC, 0x7C, 0xFE, 0x35, 0x3C, 0x5E, 0xA8, 0xFB,
                                 0xAE, 0x07, 0xB7, 0xDA, 0x4F, 0x48, 0x39, 0x6F, 0xA3, 0x13, 0xC3, 0xC6, 0x3B, 0x2A, 0x37, 0x8F,
                                 0xF4, 0x7A, 0xC4, 0xD2, 0x1B, 0xB1, 0xD0, 0x97, 0xD2, 0x11, 0xBF, 0xA9, 0x44, 0x75, 0x39, 0x6F,
                                 0xAA, 0x69, 0xC2, 0xB3, 0x4B, 0x30, 0x37, 0x8E, 0xE5, 0x7B, 0xD0, 0xA0, 0x32, 0xB1, 0xA7, 0xA9,
                                 0xAC, 0x2B, 0x2F, 0x35, 0xA8, 0xC5, 0x58, 0x19, 0x56, 0xE7, 0xB3, 0xF8, 0x4B, 0x25, 0xAB, 0xF1,
                                 0xD6, 0x0A, 0xB4, 0xDB, 0xDB, 0x28, 0xB9, 0x8F, 0xD4, 0xE7, 0xBA, 0xCC, 0x4D, 0x69, 0xA3, 0x80,
                                 0xA7, 0x1E, 0x2D, 0x15};
  thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
  return name;
}

// OFFSET: 0x0063D260
// FUNCTION: fn_04789
void CStoveMgr::OnInitializeCallback(Stove::PCSDK::CallbackResult callbackResult) {
  g_stoveWaitingCallback = false;
  if (!callbackResult.GetResult().IsSuccessful()) {
    const std::uint8_t captionSource[] = {0xD5, 0x3D, 0xF8, 0x60, 0x2C, 0x37, 0xD7};
    thread_local EncryptedName<sizeof(captionSource), 0x31D7C1EDA345E36DULL> caption(captionSource);
    const std::uint8_t messageSource[] = {0xCE, 0x63, 0xFB, 0xA9, 0xF9, 0xE6, 0x92, 0x13, 0xF5, 0x89, 0x87, 0x84, 0x67, 0xAA, 0xB5, 0x1F,
                                          0x29, 0x14, 0x80, 0x99, 0x09, 0x9A, 0xBA, 0x6C, 0xA0, 0x89, 0xF7, 0x8B, 0x19, 0x8E, 0xB5, 0x68,
                                          0x29, 0x68, 0xF9, 0xE8, 0x38, 0x9A, 0xA0, 0x67, 0xC6, 0x1D, 0x9E, 0x71, 0xD9};
    thread_local EncryptedName<sizeof(messageSource), 0xD3755DD95F47A909ULL> message(messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message.Text()).c_str(), caption.Text(), MB_OK);
    PostQuitMessage(0);
  }
}

// OFFSET: 0x0063D490
// FUNCTION: fn_04790
EncryptedName<129, 0x2D3B15774149D5DDULL>& PurchaseCheckingMessage::operator()() const {
  const std::uint8_t source[] = {0x63, 0x13, 0x89, 0x8D, 0xB2, 0xCE, 0x1B, 0x9C, 0x65, 0x6D, 0x8C, 0x61, 0xBF, 0xBB, 0xFB, 0xE3,
                                 0xFD, 0x14, 0x96, 0x81, 0xA3, 0xA1, 0xF4, 0x99, 0x04, 0xFB, 0x67, 0x6F, 0x59, 0x1F, 0x31, 0x27,
                                 0x83, 0xB3, 0x2F, 0x71, 0x47, 0x25, 0x0B, 0xEC, 0x0B, 0x15, 0x8E, 0x60, 0x57, 0x46, 0x4F, 0x42,
                                 0xAB, 0xB0, 0x69, 0x87, 0x93, 0xD5, 0xF7, 0xEC, 0x2B, 0x6A, 0xE8, 0xFD, 0xDA, 0x35, 0x8A, 0x95,
                                 0x65, 0x10, 0x69, 0xFE, 0xB8, 0xA2, 0xDA, 0x0D, 0x15, 0x11, 0x69, 0x1A, 0xBF, 0xBB, 0xFB, 0xE3,
                                 0x80, 0xF5, 0xF0, 0xB7, 0xB1, 0xA5, 0xFB, 0x96, 0xFD, 0x61, 0xE4, 0xF6, 0xD8, 0xAB, 0xE4, 0x0D,
                                 0x63, 0x13, 0x89, 0x8D, 0xB2, 0xCE, 0xFB, 0xE1, 0xFD, 0x14, 0xED, 0xFA, 0x84, 0x35, 0xFA, 0xDB,
                                 0x6C, 0x0B, 0xFC, 0x8A, 0xC3, 0xDA, 0x8F, 0xF4, 0xF3, 0x8B, 0x79, 0x71, 0x47, 0x25, 0x0B, 0x1D,
                                 0xDD};
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
      const std::uint8_t noticeSource[] = {0x50, 0x23, 0x4B, 0xBE, 0x58, 0x4F, 0xDF, 0xDF, 0x27, 0x5B, 0x3F, 0xBE, 0x36, 0x47, 0x33, 0x41,
                                           0x5F, 0x63, 0x4E, 0xBB, 0x59, 0x48, 0x4B, 0xB8, 0xCF, 0x9B};
      thread_local EncryptedName<sizeof(noticeSource), 0x61FF87ED7BF39BE1ULL> noticeName(noticeSource);
      const char* notice = noticeName.Text();
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(notice), 0xFFE6E6, 0, 0, 0);
    }
  } else {
    const std::uint8_t captionSource[] = {0x8B, 0xF9, 0x8E, 0xCC, 0x7C, 0xBB, 0x39};
    thread_local EncryptedName<sizeof(captionSource), 0x2D394DBD0F332733ULL> captionName(captionSource);
    const char* caption = captionName.Text();
    const std::uint8_t messageSource[] = {0x3F, 0x60, 0x99, 0xDB, 0xD5, 0x9E, 0xB9, 0xC4, 0x20, 0x3E, 0xDA, 0x2D, 0xA2, 0xF0, 0x26, 0xC9,
                                          0x42, 0x45, 0x93, 0xC2, 0xD1, 0xE6, 0xB7, 0x5D, 0xAC, 0xD1, 0x68, 0x5B, 0x20, 0x1F, 0x29, 0x90,
                                          0x4F, 0x31, 0x9B, 0xB3, 0xA0, 0xF2, 0x2E, 0xB3, 0xDF, 0x3C, 0x89, 0xCD, 0xA2, 0x1F, 0x20, 0xA4,
                                          0x49, 0x7D, 0x93, 0xC2, 0xD1, 0xE6, 0xB7, 0x7D};
    thread_local EncryptedName<sizeof(messageSource), 0x7D993F650D2785FFULL> messageName(messageSource);
    const char* message = messageName.Text();
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
  const std::uint8_t sandboxSource[] = {0x96, 0x7E, 0x5B, 0xCE, 0x35, 0x9E, 0x50, 0x3A, 0xD8, 0x63, 0x4B, 0xC3, 0x2C, 0x83, 0x0F};
  thread_local EncryptedName<sizeof(sandboxSource), 0x490FFB43A12F0DB9ULL> sandboxName(sandboxSource);
  const char* sandbox = sandboxName.Text();
  if (std::strstr(g_commandLine, sandbox) != nullptr) {
    this->m_environment = 1;
    param.SetEnvironment(L"SANDBOX");
    reinterpret_cast<Stove::PCSDK::Base::StovePCLogEvent&>(param).SetCategory1(L"SB_RAGNAROK_IND");
    param.SetApplicationKey(L"6b57af29764992a85cf793b01d4af15be6047715fd5f4687b23d0a1ba02337f5");
    this->unknown_02B8 = L"SB__SH_IPJVFX";
  } else {
    const std::uint8_t liveSource[] = {0x5E, 0x8A, 0x2D, 0x9A, 0xF7, 0x78, 0x3A, 0xFD, 0x18, 0x8F, 0x3C, 0xF5};
    thread_local EncryptedName<sizeof(liveSource), 0x91651D81F559F971ULL> liveName(liveSource);
    const char* live = liveName.Text();
    if (std::strstr(g_commandLine, live) != nullptr) {
      this->m_environment = 2;
      param.SetEnvironment(L"LIVE");
      reinterpret_cast<Stove::PCSDK::Base::StovePCLogEvent&>(param).SetCategory1(L"RAGNAROK_IND");
      param.SetApplicationKey(L"9af21dbb708609cd29703f56d002c64ab6f8f5bf0a310388f65cf781a5e8dd8f");
      this->unknown_02B8 = L"RAG_SH_BTLTNL";
    }
  }

  const std::uint8_t skipRestartSource[] = {0x24, 0x08, 0xAB, 0x0E, 0x3D, 0x02, 0x8E, 0x10, 0x6A, 0x1C, 0x80, 0x15, 0x24, 0x08, 0xBD, 0x73};
  thread_local EncryptedName<sizeof(skipRestartSource), 0x73D1674B61DF7B0BULL> skipRestartName(skipRestartSource);
  const char* skipRestart = skipRestartName.Text();
  if (std::strstr(g_commandLine, skipRestart) == nullptr && Base_RestartAppIfNecessary(&param) == true) {
    const std::uint8_t captionSource[] = {0x69, 0xC7, 0x20, 0x4C, 0xAA, 0x7D, 0x0F};
    thread_local EncryptedName<sizeof(captionSource), 0x0F0F8B6B8F9D19D1ULL> captionName(captionSource);
    const char* caption = captionName.Text();
    const std::uint8_t messageSource[] = {0x3E, 0x17, 0x5A, 0x5B, 0xC0, 0x23, 0x4A, 0x75, 0xDB, 0x90, 0xF5, 0xE1, 0x1B, 0xF3, 0x49, 0x77,
                                          0xD5, 0xC5, 0x15, 0xE8, 0x4E, 0xC4, 0x57, 0xF9, 0xD0, 0xA4, 0xF2, 0xCD, 0x62, 0xDB, 0xAF, 0x18,
                                          0xBB, 0xDF, 0x89, 0x92, 0x41, 0x03};
    thread_local EncryptedName<sizeof(messageSource), 0xD98F03A52D35636DULL> messageName(messageSource);
    const char* message = messageName.Text();
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

  const std::uint8_t captionSource[] = {0x0F, 0xD9, 0xEA, 0x0E, 0xEA, 0x11, 0x2B};
  thread_local EncryptedName<sizeof(captionSource), 0x812BE72BCD5707B7ULL> captionName(captionSource);
  const char* caption = captionName.Text();
  const std::uint8_t messageSource[] = {0x66, 0xF7, 0x23, 0x21, 0x43, 0x34, 0xC0, 0x99, 0x5D, 0x1D, 0x5F, 0x0C, 0xDD, 0x78, 0xE7, 0x95,
                                        0x81, 0x80, 0x58, 0x11, 0xB3, 0x48, 0xE8, 0xE6, 0x08, 0x1D, 0x2F, 0x03, 0xA3, 0x5C, 0xE7, 0xE2,
                                        0x81, 0xFC, 0x21, 0x60, 0x82, 0x48, 0xF2, 0xED, 0x6E, 0x89, 0x46, 0xF9, 0x63};
  thread_local EncryptedName<sizeof(messageSource), 0x59278F63D79F3DA1ULL> messageName(messageSource);
  const char* message = messageName.Text();
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

  const std::uint8_t source[] = {0xA7, 0xEF, 0x70, 0x24, 0x2C, 0x83, 0x6D};
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
    const std::uint8_t source[] = {0xB9, 0x89, 0x2A, 0x1C, 0xD2, 0x73, 0x53};
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
  option.SetOperation(0);
  param.SetPurchaseOption(option);
  IAP_StartPurchase(&param, sub_63A940);
  g_stoveWaitingCallback = true;
  do {
    Base_RunCallback();
    Sleep(200);
  } while (g_stoveWaitingCallback);

  if (this->unknown_0018.size() == 0) {
    const std::uint8_t parametersSource[] = {0x05, 0x47, 0x7B, 0xE5, 0xA8, 0x7F, 0xFC, 0x46, 0x0E, 0x40, 0x21, 0xFA, 0xB5, 0x36, 0xA7, 0x06,
                                             0x1B, 0x56, 0x21, 0xF6, 0xB4, 0x28, 0xFC, 0x1A, 0x08, 0x41, 0x79, 0xFC, 0xB8, 0x20, 0xFC, 0x3B,
                                             0x2C, 0x74, 0x41, 0xD4, 0x89, 0x0A, 0x98, 0x36, 0x24, 0x7D, 0x4B, 0x95};
    thread_local EncryptedName<sizeof(parametersSource), 0x69D345DB950F336DULL> parametersName(parametersSource);
    const char* parameters = parametersName.Text();
    const std::uint8_t fileSource[] = {0x19, 0x6F, 0x87, 0x8F, 0xF4, 0x1F, 0xBE, 0xE8, 0x12, 0x68, 0xDD, 0x90, 0xE9, 0x56, 0xE5, 0xA8,
                                       0x07, 0x7E, 0xDD, 0x9C, 0xE8, 0x48, 0xBE, 0xB4, 0x14, 0x69, 0x85, 0x96, 0xE4, 0x40, 0xBE, 0x95,
                                       0x30, 0x5C, 0xBD, 0xBE, 0xD5, 0x6A, 0xDA, 0x98, 0x38, 0x55, 0xB7, 0xFF};
    thread_local EncryptedName<sizeof(fileSource), 0xC7912587FFF31B71ULL> fileName(fileSource);
    const char* file = fileName.Text();
    const std::uint8_t verbSource[] = {0xFA, 0x25, 0x24, 0x11, 0xFD};
    thread_local EncryptedName<sizeof(verbSource), 0x8F1B5BFD5F6175B5ULL> verbName(verbSource);
    const char* verb = verbName.Text();
    ShellExecuteA(nullptr, verb, file, parameters, nullptr, SW_SHOWNORMAL);
  } else {
    const char* url = this->unknown_0018.c_str();
    const std::uint8_t verbSource[] = {0x60, 0xCF, 0xF6, 0x95, 0xEF};
    thread_local EncryptedName<sizeof(verbSource), 0x9B17C9EFDBB39F2FULL> verbName(verbSource);
    const char* verb = verbName.Text();
    ShellExecuteA(nullptr, verb, url, url, nullptr, SW_SHOWNORMAL);
    if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(PurchaseCheckingMessage()().Text(), 2, true, false, 0, "Message", 250, 170, 0) == 187) {
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
    const std::uint8_t source[] = {0x68, 0x51, 0x42, 0x23, 0x03};
    thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> verb(source);
    ShellExecuteA(nullptr, verb.Text(), path.c_str(), path.c_str(), nullptr, SW_SHOWNORMAL);
  }
}
