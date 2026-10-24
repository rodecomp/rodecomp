#ifndef RAGNAROK_SOURCE_NETWORK_CURLPOSTHANDER_H
#define RAGNAROK_SOURCE_NETWORK_CURLPOSTHANDER_H

#include "Network/CURLMgr.h"

#include <cstddef>
#include <string>

// Original RTTI spelling is Hander, not Handler.
class CCURLPostHander {
public:
  explicit CCURLPostHander(std::string url);
  virtual ~CCURLPostHander();
  int AddField(std::string name, std::string value, int type);
  int Perform();
  long ResponseCode();

  std::string Response() const;

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::string m_url;
  /* 0020 */ curl_slist* m_headers;
  /* 0024 */ curl_httppost* m_last;
  /* 0028 */ curl_httppost* m_form;
  /* 002C */ void* m_easy;
  /* 0030 */ std::string m_response;

private:
  static std::size_t __cdecl WriteResponse(char* data, std::size_t size, std::size_t count, std::string* response);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCURLPostHander) == 72);
#endif

#endif
