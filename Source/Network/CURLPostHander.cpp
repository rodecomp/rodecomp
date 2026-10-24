#include "Network/CURLPostHander.h"

#include <utility>

// OFFSET: 0x005E9600
// FUNCTION: fn_03955
CCURLPostHander::CCURLPostHander(std::string url)
    : m_enabled(false), m_url(std::move(url)), m_headers(nullptr), m_last(nullptr), m_form(nullptr), m_easy(nullptr) {
  this->m_easy = g_curlMgr->easyInit();
  if (this->m_easy) {
    this->m_headers = g_curlMgr->slistAppend(this->m_headers, "Content-Type: multipart/form-data");
    this->m_headers = g_curlMgr->slistAppend(this->m_headers, "Expect: 100-continue");
    this->m_headers = g_curlMgr->slistAppend(this->m_headers, "Connection: keep-alive");
    this->m_enabled = true;
  }
}

// OFFSET: 0x005E9710
// FUNCTION: fn_03956
// OFFSET: 0x005E97D0
// FUNCTION: fn_03957
CCURLPostHander::~CCURLPostHander() {
  if (this->m_easy) {
    g_curlMgr->easyCleanup(this->m_easy);
    this->m_easy = nullptr;
  }
  if (this->m_form) {
    g_curlMgr->formFree(this->m_form);
    this->m_form = nullptr;
  }
  if (this->m_headers) {
    g_curlMgr->slistFreeAll(this->m_headers);
    this->m_headers = nullptr;
  }
}

// OFFSET: 0x005E9960
// FUNCTION: fn_03958
int CCURLPostHander::AddField(std::string name, std::string value, int type) {
  // Original CURLFORM_COPYNAME=1, caller-selected data option, CURLFORM_END=17.
  return g_curlMgr->formAdd(&this->m_form, &this->m_last, 1, name.c_str(), type, value.c_str(), 17);
}

// OFFSET: 0x005E9AF0
// FUNCTION: fn_03961
std::size_t __cdecl CCURLPostHander::WriteResponse(
  char* data,
  std::size_t size,
  std::size_t count,
  std::string* response
) {
  if (!response) {
    return 0;
  }
  response->append(data, size * count);
  return size * count;
}

// OFFSET: 0x005E9A20
// FUNCTION: fn_03960
int CCURLPostHander::Perform() {
  if (!this->m_easy) {
    return 2;
  }
  g_curlMgr->easySetopt(this->m_easy, 10002, this->m_url.c_str());
  g_curlMgr->easySetopt(this->m_easy, 10023, this->m_headers);
  g_curlMgr->easySetopt(this->m_easy, 47, 1L);
  g_curlMgr->easySetopt(this->m_easy, 10024, this->m_form);
  g_curlMgr->easySetopt(this->m_easy, 20011, WriteResponse);
  g_curlMgr->easySetopt(this->m_easy, 10001, &this->m_response);
  g_curlMgr->easySetopt(this->m_easy, 13, 3L);
  g_curlMgr->easySetopt(this->m_easy, 78, 3L);
  return g_curlMgr->easyPerform(this->m_easy);
}

// OFFSET: 0x005E99F0
// FUNCTION: fn_03959
long CCURLPostHander::ResponseCode() {
  long code;
  g_curlMgr->easyGetinfo(this->m_easy, 2097154, &code);
  return code;
}

// OFFSET: 0x005CDD40
// FUNCTION: fn_03679
std::string CCURLPostHander::Response() const {
  return this->m_response;
}
