#ifndef RAGNAROK_SOURCE_CONTENTS_SPRITERIDINGOFFSET_H
#define RAGNAROK_SOURCE_CONTENTS_SPRITERIDINGOFFSET_H

#include <string>

struct ActionFrame;

int SpriteRidingOffset(const ActionFrame* frame, unsigned animation, unsigned motion, int job, int sex, int costumeJob);
std::string GetBodyActionResourceName(int job, int sex, int costumeJob, int costume);

#endif
