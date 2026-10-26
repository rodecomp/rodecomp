#include "Utility/Text/Base64.h"

#include <algorithm>

namespace {
  // OFFSET: 0x0061AD10
  // FUNCTION: fn_04416
  unsigned Position(unsigned char value) {
    if (value >= 'A' && value <= 'Z') {
      return value - 'A';
    }

    if (value >= 'a' && value <= 'z') {
      return value - 'a' + 26;
    }

    if (value >= '0' && value <= '9') {
      return value - '0' + 52;
    }

    if (value == '+' || value == '-') {
      return 62;
    }

    if (value == '/' || value == '_') {
      return 63;
    }

    return value;
  }
}

// OFFSET: 0x0061AB10
// FUNCTION: fn_04415
std::string InsertLinebreaks(std::string text, unsigned int distance) {
  if (text.empty()) {
    return {};
  }

  for (unsigned int pos = distance; pos < text.size(); pos += distance + 1) {
    text.insert(pos, "\n", 1);
  }

  return text;
}

// OFFSET: 0x006195C0
// FUNCTION: fn_04411
std::string DecodeBase64(std::string encoded, bool removeLinebreaks) {
  if (encoded.empty()) {
    return {};
  }

  if (removeLinebreaks) {
    encoded.erase(std::remove(encoded.begin(), encoded.end(), '\n'), encoded.end());
    return DecodeBase64(encoded, false);
  }

  std::string decoded;
  decoded.reserve(encoded.size() / 4 * 3);

  for (std::size_t pos = 0; pos < encoded.size(); pos += 4) {
    unsigned second = Position(encoded[pos + 1]);
    decoded.push_back(static_cast<char>((Position(encoded[pos]) << 2) + ((second >> 4) & 3)));
    if (pos + 2 < encoded.size() && encoded[pos + 2] != '=' && encoded[pos + 2] != '.') {
      unsigned third = Position(encoded[pos + 2]);
      decoded.push_back(static_cast<char>((second << 4) ^ ((third >> 2) & 15)));
      if (pos + 3 < encoded.size() && encoded[pos + 3] != '=' && encoded[pos + 3] != '.') {
        decoded.push_back(static_cast<char>((third << 6) + Position(encoded[pos + 3])));
      }
    }
  }

  return decoded;
}
