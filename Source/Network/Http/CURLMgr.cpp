#include "Network/Http/CURLMgr.h"

#include <new>

// STATIC: 0x013898CC
// GLOBAL: gl_24354
CCURLMgr* g_curlMgr;

// INLINED
CCURLMgr::CCURLMgr() : m_enabled(false), m_module(nullptr) {
  this->m_module = LoadLibraryA("libcurl.dll");
  if (this->m_module) {
    this->globalInit = reinterpret_cast<decltype(globalInit)>(GetProcAddress(m_module, "curl_global_init"));
    this->globalCleanup = reinterpret_cast<decltype(globalCleanup)>(GetProcAddress(m_module, "curl_global_cleanup"));
    this->slistAppend = reinterpret_cast<decltype(slistAppend)>(GetProcAddress(m_module, "curl_slist_append"));
    this->formAdd = reinterpret_cast<decltype(formAdd)>(GetProcAddress(m_module, "curl_formadd"));
    this->easyInit = reinterpret_cast<decltype(easyInit)>(GetProcAddress(m_module, "curl_easy_init"));
    this->easySetopt = reinterpret_cast<decltype(easySetopt)>(GetProcAddress(m_module, "curl_easy_setopt"));
    this->easyPerform = reinterpret_cast<decltype(easyPerform)>(GetProcAddress(m_module, "curl_easy_perform"));
    this->easyCleanup = reinterpret_cast<decltype(easyCleanup)>(GetProcAddress(m_module, "curl_easy_cleanup"));
    this->formFree = reinterpret_cast<decltype(formFree)>(GetProcAddress(m_module, "curl_formfree"));
    this->slistFreeAll = reinterpret_cast<decltype(slistFreeAll)>(GetProcAddress(m_module, "curl_slist_free_all"));
    this->easyGetinfo = reinterpret_cast<decltype(easyGetinfo)>(GetProcAddress(m_module, "curl_easy_getinfo"));
    this->globalInit(3);
    this->m_enabled = true;
  }
}

// INLINED
CCURLMgr::~CCURLMgr() {
  this->globalCleanup();
  if (this->m_module) {
    FreeLibrary(this->m_module);
    this->m_module = nullptr;
  }
}

// OFFSET: 0x005E9290
// FUNCTION: fn_03952
bool CCURLMgr::Initialize() {
  if (g_curlMgr) {
    return false;
  }

  g_curlMgr = new (std::nothrow) CCURLMgr;
  if (!g_curlMgr) {
    return false;
  }

  if (!g_curlMgr->m_enabled) {
    delete g_curlMgr;
    g_curlMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x005E9420
// FUNCTION: fn_03953
void CCURLMgr::Shutdown() {
  if (g_curlMgr) {
    delete g_curlMgr;
    g_curlMgr = nullptr;
  }
}
