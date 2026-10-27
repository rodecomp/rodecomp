#include "Utility/Text/Base64.h"

#include <algorithm>

namespace {
  // STRING: st_01143
  const char standardAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  // STRING: st_01144
  const char urlAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
  // GLOBAL: gl_24501
  const char* const alphabets[] = {standardAlphabet, urlAlphabet};

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

// OFFSET: 0x00619D70
std::string EncodeBase64(const std::string& input, bool urlSafe) {
  const std::string bytes(input);
  return EncodeBase64(reinterpret_cast<const unsigned char*>(bytes.c_str()), static_cast<unsigned int>(bytes.size()), urlSafe);
}

// OFFSET: 0x00619E10
// FUNCTION: fn_04414
std::string EncodeBase64(const unsigned char* input, unsigned int length, bool urlSafe) {
  const char padding = urlSafe ? '.' : '=';
  const char* alphabet = alphabets[urlSafe];
  std::string encoded;
  encoded.reserve(4 * ((length + 2) / 3));

  for (unsigned int index = 0; index < length; index += 3) {
    encoded.push_back(alphabet[input[index] >> 2]);
    if (index + 1 < length) {
      encoded.push_back(alphabet[((input[index] & 3) << 4) + (input[index + 1] >> 4)]);
      if (index + 2 < length) {
        encoded.push_back(alphabet[((input[index + 1] & 15) << 2) + (input[index + 2] >> 6)]);
        encoded.push_back(alphabet[input[index + 2] & 63]);
      } else {
        encoded.push_back(alphabet[(input[index + 1] & 15) << 2]);
        encoded.push_back(padding);
      }
    } else {
      encoded.push_back(alphabet[(input[index] & 3) << 4]);
      encoded.push_back(padding);
      encoded.push_back(padding);
    }
  }

  return encoded;
}
