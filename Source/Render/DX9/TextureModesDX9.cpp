#include "Render/DX9/DeviceStateDX9.h"
#include "Render/DX9/RendererDX9Operations.h"

// OFFSET: 0x00568470
// FUNCTION: fn_02241
void RendererDX9Operations::SetTextureMode(int mode) {
  switch (mode) {
    case 0:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 1:
    case 2:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 3:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 4:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case 5:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 6:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 7:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 8:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 9:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 10:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 11:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 12:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 13:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 14:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case 15:
      this->state.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->state.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->state.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->state.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
      this->state.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->state.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
  }
}
