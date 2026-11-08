#ifndef RAGNAROK_SOURCE_CLIENT_STOVE_STOVEMGR_H
#define RAGNAROK_SOURCE_CLIENT_STOVE_STOVEMGR_H

#include <cstddef>
#include <string>
#include <vector>

struct ITEM_INFO;


// VTABLE: vt_00291
template <class T> class Singleton2 {
public:
  virtual ~Singleton2() {
  }
};

// VTABLE: vt_00292
class CStoveMgr : public Singleton2<CStoveMgr> {
public:
  CStoveMgr();
  ~CStoveMgr() override;

  /* 0004 */ bool m_initialized;
  /* 0008 */ int m_pendingOperation;
  /* 000C */ int m_environment;
  /* 0010 */ int m_purchaseIdLow;
  /* 0014 */ int m_purchaseIdHigh;
  /* 0018 */ std::string m_purchaseUrl;
  /* 0030 */ char m_accessToken[600];
  /* 0288 */ char m_memberNumber[24];
  /* 02A0 */ char m_gameUserId[24];
  /* 02B8 */ std::wstring m_applicationId;

  const char* GetAccessToken();
  void FetchUser();
  __declspec(noinline) void WaitForCallback();
  void FetchCategoryProducts();
  void FetchAllProducts();
  bool Initialize();
  void StartPurchase(const std::vector<ITEM_INFO>& items);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStoveMgr, m_environment) == 0x000C);
static_assert(offsetof(CStoveMgr, m_purchaseUrl) == 0x0018);
static_assert(offsetof(CStoveMgr, m_accessToken) == 0x0030);
static_assert(offsetof(CStoveMgr, m_memberNumber) == 0x0288);
static_assert(offsetof(CStoveMgr, m_gameUserId) == 0x02A0);
static_assert(offsetof(CStoveMgr, m_applicationId) == 0x02B8);
#endif

extern CStoveMgr* g_stoveMgr;

void FetchStoveTermsAgreement();

#endif
