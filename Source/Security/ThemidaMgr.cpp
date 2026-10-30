#include "Security/ThemidaMgr.h"

#include <new>
#include <random>

// GLOBAL: gl_24891
CThemidaMgr* g_themidaMgr;

void sub_6563E0(char* first, char* last, void* distribution);

struct ThemidaKeyRandom {
  std::mt19937 engine;
  std::uniform_int_distribution<int> distribution;

  ThemidaKeyRandom(unsigned int seed) : engine(seed), distribution(1, 0xfe) {
  }
};

struct ThemidaKeyEncoderA {
  unsigned char* Generate(const unsigned char* input);
};

struct ThemidaKeyEncoderB {
  unsigned char* Generate(const unsigned char* input);
};

// OFFSET: 0x0076EC00
// FUNCTION: fn_08235
CThemidaMgr::CThemidaMgr() : m_valid(true) {
}

// OFFSET: 0x0076EC10
// FUNCTION: fn_08236
CThemidaMgr::~CThemidaMgr() {
}

// OFFSET: 0x0076EC20
// FUNCTION: fn_08237
unsigned char* ThemidaKeyEncoderA::Generate(const unsigned char* input) {
  auto* output = reinterpret_cast<unsigned char*>(this);
  ThemidaKeyRandom random(std::random_device{}());
  sub_6563E0(reinterpret_cast<char*>(output), reinterpret_cast<char*>(output) + 5, &random);
  output[5] = input[0] ^ output[1] ^ output[0];
  output[6] = input[1] ^ output[2] ^ output[1];
  output[7] = input[2] ^ output[3] ^ output[2];
  output[8] = output[4] ^ input[3] ^ output[3];
  return output;
}

// OFFSET: 0x0076ECC0
// FUNCTION: fn_08238
unsigned char* ThemidaKeyEncoderB::Generate(const unsigned char* input) {
  auto* output = reinterpret_cast<unsigned char*>(this);
  ThemidaKeyRandom random(std::random_device{}());
  sub_6563E0(reinterpret_cast<char*>(output), reinterpret_cast<char*>(output) + 5, &random);
  output[5] = input[0] ^ output[1] ^ output[0];
  output[6] = input[1] ^ output[2] ^ output[1];
  output[7] = input[2] ^ output[3] ^ output[2];
  output[8] = output[4] ^ input[3] ^ output[3];
  return output;
}

// OFFSET: 0x0076EDE0
// FUNCTION: fn_08240
bool CThemidaMgr::Create() {
  if (g_themidaMgr == nullptr) {
    g_themidaMgr = new (std::nothrow) CThemidaMgr();
    if (g_themidaMgr != nullptr) {
      if (g_themidaMgr->m_valid) {
        return true;
      }
      delete g_themidaMgr;
      g_themidaMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x0076EE90
// FUNCTION: fn_08241
void CThemidaMgr::Destroy() {
  if (g_themidaMgr != nullptr) {
    delete g_themidaMgr;
    g_themidaMgr = nullptr;
  }
}
