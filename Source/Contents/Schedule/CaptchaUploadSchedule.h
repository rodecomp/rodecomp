#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_CAPTCHAUPLOADSCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_CAPTCHAUPLOADSCHEDULE_H

#include "Contents/Schedule/Schedule.h"

class CCaptchaUploadSchedule : public CSchedule {
public:
  CCaptchaUploadSchedule(const std::uint64_t& timestamp, const std::uint32_t* sessionId, int size, const void* data);
  ~CCaptchaUploadSchedule() override;
  void Process(const std::uint64_t& now) override;

  /* 0030 */ int size;
  /* 0034 */ void* buffer;
  /* 0038 */ std::uint32_t sessionId;
  /* 003C */ int sent;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCaptchaUploadSchedule) == 0x40);
static_assert(offsetof(CCaptchaUploadSchedule, buffer) == 0x34);
static_assert(offsetof(CCaptchaUploadSchedule, sent) == 0x3C);
#endif

#endif
