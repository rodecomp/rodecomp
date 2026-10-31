#ifndef RAGNAROK_SOURCE_CLIENT_STOVE_STOVEMGR_H
#define RAGNAROK_SOURCE_CLIENT_STOVE_STOVEMGR_H

#include <cstddef>
#include <string>
#include <vector>

struct ITEM_INFO;

namespace Stove::PCSDK {
  struct CallbackResult;
}

class CStoveMgr {
public:
  CStoveMgr();

  /* 0000 */ void* vtable;
  /* 0004 */ bool m_initialized;
  /* 0008 */ int unknown_0008;
  /* 000C */ int m_environment;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ std::string unknown_0018;
  /* 0030 */ char m_accessToken[600];
  /* 0288 */ char m_memberNumber[24];
  /* 02A0 */ char m_gameUserId[24];
  /* 02B8 */ std::wstring unknown_02B8;

  const char* GetAccessToken();
  void FetchUser();
  void WaitForCallback();
  void FetchCategoryProducts();
  void FetchAllProducts();
  void OnInitializeCallback(Stove::PCSDK::CallbackResult callbackResult);
  void OnOpenUrlCallback(Stove::PCSDK::CallbackResult callbackResult, int unused, const wchar_t* url);
  bool Initialize();
  void StartPurchase(const std::vector<ITEM_INFO>& items);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStoveMgr, m_environment) == 0x000C);
static_assert(offsetof(CStoveMgr, unknown_0018) == 0x0018);
static_assert(offsetof(CStoveMgr, m_accessToken) == 0x0030);
static_assert(offsetof(CStoveMgr, m_memberNumber) == 0x0288);
static_assert(offsetof(CStoveMgr, m_gameUserId) == 0x02A0);
static_assert(offsetof(CStoveMgr, unknown_02B8) == 0x02B8);
#endif

extern CStoveMgr* g_stoveMgr;

#endif
