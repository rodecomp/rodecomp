#ifndef RAGNAROK_SOURCE_RENDER_BITMAPDRAWING_H
#define RAGNAROK_SOURCE_RENDER_BITMAPDRAWING_H

class CRenderSystemBase;
class CBitmapRes;

void DrawBitmapResource(CRenderSystemBase& system, int x, int y, const CBitmapRes* bitmap);

#endif
