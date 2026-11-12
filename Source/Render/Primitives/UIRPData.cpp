#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Renderer.h"
#include "Render/Textures/Texture.h"
#include "Render/Primitives/UIRPData.h"

#include <algorithm>
#include <bit>

// OFFSET: 0x00812780
// FUNCTION: fn_10117
UIRPData::UIRPData() : m_texture(nullptr) {
}

// OFFSET: 0x00812800
// FUNCTION: fn_10118
UIRPData::~UIRPData() {
  if (this->m_texture) {
    delete this->m_texture;
    this->m_texture = nullptr;
  }
  this->m_primitives.clear();
}

// OFFSET: 0x008129D0
// FUNCTION: fn_10120
void UIRPData::Draw(int x, int y, CTexture* texture, std::uint32_t color, int width, int height, bool stretched) {
  if (!texture) {
    return;
  }

  if (stretched == true) {
    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    primitive->texture = texture;
    const int right = x + width;
    const int bottom = y + height;
    primitive->quad[0].x = static_cast<float>(x);
    primitive->quad[0].y = static_cast<float>(y);
    primitive->quad[0].u = g_textureOffsetX / static_cast<float>(texture->TextureWidth());
    primitive->quad[0].v = g_textureOffsetY / static_cast<float>(texture->TextureHeight());
    primitive->quad[1].x = static_cast<float>(right);
    primitive->quad[1].y = static_cast<float>(y);
    primitive->quad[1].u = (static_cast<float>(texture->Width()) + g_textureOffsetX) / static_cast<float>(texture->TextureWidth());
    primitive->quad[1].v = g_textureOffsetY / static_cast<float>(texture->TextureHeight());
    primitive->quad[2].x = static_cast<float>(x);
    primitive->quad[2].y = static_cast<float>(bottom);
    primitive->quad[2].u = g_textureOffsetX / static_cast<float>(texture->TextureWidth());
    primitive->quad[2].v = (static_cast<float>(texture->Height()) + g_textureOffsetX) / static_cast<float>(texture->TextureHeight());
    primitive->quad[3].x = static_cast<float>(right);
    primitive->quad[3].y = static_cast<float>(bottom);
    primitive->quad[3].u = (static_cast<float>(texture->Width()) + g_textureOffsetX) / static_cast<float>(texture->TextureWidth());
    primitive->quad[3].v = (static_cast<float>(texture->Height()) + g_textureOffsetX) / static_cast<float>(texture->TextureHeight());
    primitive->quad[0].z = 0.000001f;
    primitive->quad[0].rhw = 0.999999f;
    primitive->quad[0].diffuse = color;
    primitive->quad[1].z = 0.000001f;
    primitive->quad[1].rhw = 0.999999f;
    primitive->quad[1].diffuse = color;
    primitive->quad[2].z = 0.000001f;
    primitive->quad[2].rhw = 0.999999f;
    primitive->quad[2].diffuse = color;
    primitive->quad[3].z = 0.000001f;
    primitive->quad[3].rhw = 0.999999f;
    primitive->quad[3].diffuse = color;
    this->m_primitives.push_back(primitive);
  } else {
    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    primitive->texture = texture;
    const int imageWidth = texture->Width();
    const int imageHeight = texture->Height();
    if (!width) {
      width = texture->Width();
    }

    if (!height) {
      height = texture->Height();
    }

    const int textureWidth = texture->TextureWidth();
    const int textureHeight = texture->TextureHeight();
    const int right = x + width;
    const float bottom = static_cast<float>(y + height);
    primitive->quad[0].x = static_cast<float>(x);
    primitive->quad[0].y = static_cast<float>(y);
    primitive->quad[0].u = g_textureOffsetX / textureWidth;
    primitive->quad[0].v = g_textureOffsetY / textureHeight;
    primitive->quad[1].x = static_cast<float>(right);
    primitive->quad[1].y = static_cast<float>(y);
    primitive->quad[1].u = (static_cast<float>(width <= imageWidth ? width : imageWidth) + g_textureOffsetX) / textureWidth;
    primitive->quad[1].v = g_textureOffsetY / textureHeight;
    primitive->quad[2].x = static_cast<float>(x);
    primitive->quad[2].y = bottom;
    primitive->quad[2].u = g_textureOffsetX / textureWidth;
    primitive->quad[2].v =
      (height <= imageHeight ? static_cast<float>(height) + g_textureOffsetX : static_cast<float>(imageHeight) + g_textureOffsetY) / textureHeight;
    primitive->quad[3].x = static_cast<float>(right);
    primitive->quad[3].y = bottom;
    primitive->quad[3].u = (static_cast<float>(width <= imageWidth ? width : imageWidth) + g_textureOffsetX) / textureWidth;
    primitive->quad[3].v =
      (height <= imageHeight ? static_cast<float>(height) + g_textureOffsetX : static_cast<float>(imageHeight) + g_textureOffsetY) / textureHeight;
    primitive->quad[0].z = 0.000001f;
    primitive->quad[0].rhw = 0.999999f;
    primitive->quad[0].diffuse = color;
    primitive->quad[1].z = 0.000001f;
    primitive->quad[1].rhw = 0.999999f;
    primitive->quad[1].diffuse = color;
    primitive->quad[2].z = 0.000001f;
    primitive->quad[2].rhw = 0.999999f;
    primitive->quad[2].diffuse = color;
    primitive->quad[3].z = 0.000001f;
    primitive->quad[3].rhw = 0.999999f;
    primitive->quad[3].diffuse = color;
    this->m_primitives.push_back(primitive);
  }
}
