#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTMEMORYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTMEMORYMGR_H

class CParticle;
struct vector3d;

bool CreateEffectMemoryMgr();
void DestroyEffectMemoryMgr();
CParticle* CreateParticle(const vector3d* position, int flags);
CParticle* CreateSpriteParticle(const vector3d* position, int flags);
CParticle* CreateEZ2STRParticle(const vector3d* position, int flags);
CParticle* CreateMultiTexParticle(const vector3d* position, int flags);
void FreeParticle(CParticle* particle);

#endif
