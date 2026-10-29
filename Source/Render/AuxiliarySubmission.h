#ifndef RAGNAROK_SOURCE_RENDER_AUXILIARYSUBMISSION_H
#define RAGNAROK_SOURCE_RENDER_AUXILIARYSUBMISSION_H

class CRenderer;
struct LightmappedPrimitive;

void SubmitAuxiliaryPrimitive(CRenderer& renderer, LightmappedPrimitive* primitive, const float& depthStart, const float& depthEnd);

#endif
