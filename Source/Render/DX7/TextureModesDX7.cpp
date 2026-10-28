#include "Render/DX7/RendererDX7Operations.h"

// OFFSET: 0x0055ED50
// FUNCTION: fn_02116
void RendererDX7Operations::SetTextureMode(int mode) {
  switch (mode) {
    case 0:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 1:
    case 2:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 3:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 4:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 5:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 6:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 7:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 8:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 9:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 10:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 11:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 12:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 13:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 14:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 15:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
      this->device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
  }
}
