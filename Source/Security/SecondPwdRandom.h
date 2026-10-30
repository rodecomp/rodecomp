#ifndef RAGNAROK_SOURCE_SECURITY_SECONDPWDRANDOM_H
#define RAGNAROK_SOURCE_SECURITY_SECONDPWDRANDOM_H

// VTABLE: vt_01098
class CSecondPwdRandom {
public:
  virtual ~CSecondPwdRandom();

  /* 0004 */ unsigned int m_seed;
  /* 0008 */ unsigned int m_multiplier;
  /* 000C */ unsigned int m_increment;
  /* 0010 */ unsigned int m_count;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSecondPwdRandom) == 0x14);
#endif

#endif
