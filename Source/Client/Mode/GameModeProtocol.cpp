#include "Network/Connection/Connection.h"

#include <cstdint>

// OFFSET: 0x00D059E0
// FUNCTION: fn_21572
void DecodeSrcDst(const unsigned char* packed, int* srcX, int* srcY, int* dstX, int* dstY, int* subX, int* subY) {
  *srcX = packed[0];
  *srcX = (*srcX << 2) | (packed[1] >> 6);
  *srcY = packed[1] & 0x3F;
  *srcY = (*srcY << 4) | (packed[2] >> 4);
  *dstX = packed[2] & 0x0F;
  *dstX = (*dstX << 6) | (packed[3] >> 2);
  *dstY = packed[3] & 0x03;
  *dstY = (*dstY << 8) | packed[4];
  *subX = packed[5] >> 4;
  *subY = packed[5] & 0x0F;
}

// OFFSET: 0x00D08600
// FUNCTION: fn_21589
void SendPacketHeader(std::uint16_t packetType) {
  GetRagConnection().Send(sizeof(packetType), &packetType);
}

// OFFSET: 0x00D08620
// FUNCTION: fn_21590
void SendPacketHeader2(std::uint16_t packetType) {
  GetRagConnection().Send(sizeof(packetType), &packetType);
}
