#include "Client/Application/ErrorDialog.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/Sprites/SpriteResource.h"

#include <cstdlib>
#include <cstring>
#include <new>
#include <string>

// OFFSET: 0x0075FE90
// FUNCTION: fn_08053
CSprRes::CSprRes(SpriteResourceContext& context) : indexedCount(0), m_context(&context) {
}

// OFFSET: 0x00760BB0
// FUNCTION: fn_08056
CSprRes::~CSprRes() = default;

CSprRes* CSprRes::Create() {
  return new CSprRes(*this->m_context);
}

// OFFSET: 0x0075FF40
// FUNCTION: fn_08054
bool CSprRes::Load(const char* name) {
  auto& context = *this->m_context;
  auto& file = context.file;
  struct CloseFile {
    ResourceBytes& file;

    ~CloseFile() {
      file.Close();
    }
  } close{file};

  if (!file.Open(name)) {
    const std::string message = std::string("Spr :: Cannot find File : ") + name;
    ShowErrorDialog(context.renderSystem, message.c_str(), 0);
    return false;
  }

  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  struct Header {
    std::uint16_t magic;
    std::uint16_t version;
    std::uint16_t indexed;
  } header;
  input.Read(&header, sizeof(header));

  if (header.magic != 0x5053) {
    ShowErrorDialog(context.renderSystem, "Illegal File Format", 0);
    ShowErrorDialog(context.renderSystem, name, 0);
    return false;
  }

  if (header.version > 0x0201) {
    ShowErrorDialog(context.renderSystem, "Unsupported Version. Content may be corrupted", 0);
    return false;
  }

  this->indexedCount = header.indexed;

  if (header.version >= 0x0101) {
    unsigned char colors[1024];
    input.position = input.size - sizeof(colors);
    input.Read(colors, sizeof(colors));
    input.position = sizeof(header);
    ConvertSpritePalette(this->palette, colors, 256);
  }

  std::uint16_t trueColorCount;

  if (header.version >= 0x0200) {
    input.Read(&trueColorCount, sizeof(trueColorCount));
  }

  for (int index = 0; index < this->indexedCount; ++index) {
    std::uint16_t dimensions[2];
    input.Read(dimensions, sizeof(dimensions));
    auto* image = new SpriteRleImage{};
    image->width = static_cast<std::int16_t>(dimensions[0]);
    image->height = static_cast<std::int16_t>(dimensions[1]);
    std::uint16_t encodedBytes;

    if (context.halfWidth == 0 && context.halfHeight == 0) {
      if (header.version < 0x0201) {
        const int count = image->width * image->height;
        auto* pixels = static_cast<std::uint8_t*>(::operator new(count));
        input.Read(pixels, count);
        image->runs = EncodeSpriteRuns(pixels, image->width, image->height, encodedBytes);
        std::free(pixels);
      } else {
        input.Read(&encodedBytes, sizeof(encodedBytes));
        auto* runs = static_cast<std::uint8_t*>(::operator new(encodedBytes));
        image->runs = runs;
        input.Read(runs, encodedBytes);
      }
    } else {
      std::uint8_t* pixels;

      if (header.version >= 0x0201) {
        input.Read(&encodedBytes, sizeof(encodedBytes));
        auto* runs = static_cast<std::uint8_t*>(::operator new(encodedBytes));
        input.Read(runs, encodedBytes);
        pixels = static_cast<std::uint8_t*>(::operator new(dimensions[0] * dimensions[1]));
        const std::uint8_t* current = runs;
        std::uint16_t zeroCount = 0;
        std::uint8_t value;

        for (unsigned y = 0; y < dimensions[1]; ++y) {
          for (unsigned x = 0; x < dimensions[0]; ++x) {
            if (zeroCount == 0) {
              value = *current++;

              if (value == 0) {
                zeroCount = *current++;
              }
            }

            const std::uint16_t before = zeroCount;
            zeroCount = zeroCount != 0 ? zeroCount - 1 : 0;
            pixels[y * dimensions[0] + x] = before == 0 ? value : 0;
          }
        }

        std::free(runs);
      } else {
        const int count = image->width * image->height;
        pixels = static_cast<std::uint8_t*>(::operator new(count));
        input.Read(pixels, count);
      }

      auto* reduced = ReduceSprite(pixels, dimensions[0], dimensions[1], context.halfWidth, context.halfHeight);
      std::free(pixels);
      image->runs = EncodeSpriteRuns(reduced, dimensions[0] / (context.halfWidth + 1), dimensions[1] / (context.halfHeight + 1), encodedBytes);
      std::free(reduced);

      if (context.halfWidth != 0) {
        image->width /= 2;
        image->unknown_0004 = 1;
      }

      if (context.halfHeight != 0) {
        image->height /= 2;
        image->unknown_0004 |= 1u << 16;
      }
    }

    this->images[0].push_back(image);
  }

  if (header.version >= 0x0200) {
    for (unsigned index = 0; index < trueColorCount; ++index) {
      std::uint16_t dimensions[2];
      input.Read(dimensions, sizeof(dimensions));
      auto* image = new SpriteRleImage{};
      image->width = static_cast<std::int16_t>(dimensions[0]);
      image->height = static_cast<std::int16_t>(dimensions[1]);
      auto* pixels = static_cast<std::uint32_t*>(::operator new(static_cast<std::size_t>(image->width * image->height) * sizeof(std::uint32_t)));

      for (int y = image->height - 1; y >= 0; --y) {
        for (int x = 0; x < image->width; ++x) {
          std::uint8_t pixel[4];
          input.Read(pixel, sizeof(pixel));
          pixels[y * image->width + x] = pixel[1] | (pixel[2] << 8) | (pixel[3] << 16) | (static_cast<std::uint32_t>(pixel[0]) << 24);
        }
      }

      const int width = context.halfWidth != 0 ? image->width / 2 : image->width;
      const int height = context.halfHeight != 0 ? image->height / 2 : image->height;
      CTexture* texture = context.renderSystem ? context.renderSystem->CreateTexture(width, height, TextureFormat::A4R4G4B4, false, false) : nullptr;
      image->unknown_0008 = texture;
      texture->Upload(0, 0, image->width, image->height, pixels, 0, 0);
      std::free(pixels);

      if (context.halfWidth != 0) {
        image->width /= 2;
        image->unknown_0004 = 1;
      }

      if (context.halfHeight != 0) {
        image->height /= 2;
        image->unknown_0004 |= 1u << 16;
      }

      this->images[1].push_back(image);
    }
  }

  file.Close();
  return true;
}

// OFFSET: 0x00760B10
// FUNCTION: fn_08055
void CSprRes::Unload() {
  for (unsigned type = 0; type < 2; ++type) {
    for (SpriteRleImage* image : this->images[type]) {
      if (type == 0) {
        std::free(const_cast<std::uint8_t*>(image->runs));
      } else {
        delete static_cast<const CTexture*>(image->unknown_0008);
        image->unknown_0008 = nullptr;
      }

      delete image;
    }

    this->images[type].clear();
  }
}
