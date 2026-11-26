#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/RendererDX7.h"

// OFFSET: 0x0055ED50
// FUNCTION: fn_02116
void CRendererDX7::SetTextureMode(int mode) {
  switch (mode) {
    case TEXTURE_MODE_MODULATE_COLOR_AND_ALPHA:
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
    case TEXTURE_MODE_MODULATE:
    case TEXTURE_MODE_UNKNOWN_2:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case TEXTURE_MODE_ADD:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case TEXTURE_MODE_DECAL_ALPHA:
      this->device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
      this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case TEXTURE_MODE_COLORED_LIGHTMAP:
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
    case TEXTURE_MODE_INVERSE_COLORED_LIGHTMAP:
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
    case TEXTURE_MODE_SINGLE_CHANNEL_LIGHTMAP:
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
    case TEXTURE_MODE_MODULATE_AND_LATE_ADD:
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
    case TEXTURE_MODE_BLEND_TEXTURE_ALPHA:
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
    case TEXTURE_MODE_BLEND_DIFFUSE_ALPHA:
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
    case TEXTURE_MODE_MULTITEXTURE_ADD:
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
    case TEXTURE_MODE_MULTITEXTURE_SUBTRACT:
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
    case TEXTURE_MODE_ADD_THEN_MODULATE:
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
    case TEXTURE_MODE_DETAIL_MODULATE:
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
    case TEXTURE_MODE_DETAIL_ADD:
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
