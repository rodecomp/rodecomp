#ifndef RAGNAROK_SOURCE_CONTENTS_ATTACKMOTION_H
#define RAGNAROK_SOURCE_CONTENTS_ATTACKMOTION_H

// Length selected by the sprite actor's attack-state transition. job is the
// actor's resolved job, appearance is its original +0x260 field, and equipment
// is the original main equipment value. attackMode distinguishes exactly 0
// and 1; all other integer values retain the default six-frame length.
float AttackMotionLength(int job, int appearance, int equipment, int attackMode);

#endif
