#ifndef RAGNAROK_SOURCE_UTILITY_THREADING_NTHREAD_H
#define RAGNAROK_SOURCE_UTILITY_THREADING_NTHREAD_H

#include <cstdint>
#include <string>
#include <windows.h>

class NThread {
public:
  explicit NThread(std::string name);
  virtual ~NThread();
  virtual unsigned Run() = 0;
  virtual void RequestStop() = 0;

  bool Start();
  void Stop();
  void Resume();

protected:
  /* 0004 */ unsigned m_threadId;
  /* 0008 */ bool unknown_0008;
  /* 000C */ std::uint32_t m_state;
  /* 0010 */ HANDLE m_handle;
  /* 0014 */ std::string m_name;

private:
  static unsigned __stdcall Entry(void* parameter);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NThread) == 44);
#endif

#endif
