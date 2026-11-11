#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_AUXILIARYSUBMISSION_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_AUXILIARYSUBMISSION_H

class CRenderer;
struct LightmappedPrimitive;

void SubmitAuxiliaryPrimitive(CRenderer& renderer, LightmappedPrimitive* primitive, const float& depthStart, const float& depthEnd, int reserved = 0);

#endif
