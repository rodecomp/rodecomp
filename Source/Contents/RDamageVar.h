#ifndef RAGNAROK_SOURCE_CONTENTS_RDAMAGEVAR_H
#define RAGNAROK_SOURCE_CONTENTS_RDAMAGEVAR_H

class RDamageVar {
public:
  RDamageVar() : damageValue(0) {}
  virtual ~RDamageVar();

  /* 0004 */ int damageValue;
};

#endif
