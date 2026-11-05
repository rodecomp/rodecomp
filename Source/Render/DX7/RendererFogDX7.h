#ifndef RAGNAROK_SOURCE_RENDER_DX7_RENDERERFOGDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_RENDERERFOGDX7_H

#include <d3d.h>

class CRenderSystemBase;

__declspec(noinline) HRESULT PerspectiveMatrixDX7(D3DMATRIX& matrix, float fov, float aspect, float nearPlane, float farPlane);

#endif
