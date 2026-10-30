#include "Contents/Schedule/CaptchaUploadSchedule.h"

#include "Contents/ShowRankingScheduler.h"
#include "Network/Connection/Connection.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

struct CaptchaUploadHeader {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint32_t sessionId;
};

// OFFSET: 0x006367B0
// FUNCTION: fn_04710
CCaptchaUploadSchedule::CCaptchaUploadSchedule(const std::uint64_t& timestamp, const std::uint32_t* sessionId, int size, const void* data)
    : CSchedule(timestamp, 1000, 1, ++g_schedulerSerial, 0) {
  this->size = size;
  this->buffer = std::malloc(size);
  memcpy_s(this->buffer, size, data, size);
  memcpy_s(&this->sessionId, sizeof(this->sessionId), sessionId, sizeof(this->sessionId));
  this->sent = 0;
}

CCaptchaUploadSchedule::~CCaptchaUploadSchedule() {
  if (this->buffer) {
    std::free(this->buffer);
    this->buffer = nullptr;
  }
}

// OFFSET: 0x00636920
// FUNCTION: fn_04711
CSchedule::~CSchedule() {
}

// OFFSET: 0x006369E0
// FUNCTION: fn_04714
void CCaptchaUploadSchedule::Process(const std::uint64_t& now) {
  if (this->size != this->sent) {
    int chunk = this->size - this->sent;
    if (chunk > 0x400) {
      chunk = 0x400;
    }

    CaptchaUploadHeader* header = new CaptchaUploadHeader;
    header->packetType = 0xA54;
    header->length = static_cast<std::int16_t>(chunk + 8);
    memcpy_s(&header->sessionId, sizeof(header->sessionId), &this->sessionId, sizeof(this->sessionId));

    char* packet = static_cast<char*>(std::malloc(header->length));
    memcpy_s(packet, header->length, header, sizeof(*header));
    memcpy_s(packet + sizeof(*header), header->length, static_cast<char*>(this->buffer) + this->sent, chunk);

    GetRagConnection().Send(header->length, packet);
    std::free(packet);
    delete header;

    this->sent += chunk;
    if (this->size != this->sent) {
      return;
    }
  }

  this->active = false;
}
