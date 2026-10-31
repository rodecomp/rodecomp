#ifndef RAGNAROK_SOURCE_SECURITY_SECONDPWDRANDOM_H
#define RAGNAROK_SOURCE_SECURITY_SECONDPWDRANDOM_H

// VTABLE: vt_01098
class CSecondPwdRandom {
public:
  using result_type = unsigned int;

  // INLINED
  explicit CSecondPwdRandom(unsigned int seed) : m_seed(seed), m_multiplier(0x3498), m_increment(0x881234), m_count(1) {
  }
  virtual ~CSecondPwdRandom();

  // INLINED
  static constexpr result_type(min)() {
    return 0;
  }
  // INLINED
  static constexpr result_type(max)() {
    return 99;
  }
  // INLINED
  result_type operator()() {
    ++this->m_count;
    this->m_seed = this->m_seed * this->m_multiplier + this->m_increment;
    return this->m_seed % this->m_count;
  }

  /* 0004 */ unsigned int m_seed;
  /* 0008 */ unsigned int m_multiplier;
  /* 000C */ unsigned int m_increment;
  /* 0010 */ unsigned int m_count;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSecondPwdRandom) == 0x14);
#endif

#endif
