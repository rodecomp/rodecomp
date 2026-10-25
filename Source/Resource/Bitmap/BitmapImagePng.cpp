#include "Resource/Bitmap/BitmapImage.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <png.h>
#include <streambuf>

namespace {
  class MemoryBuffer : public std::streambuf {
  public:
    MemoryBuffer(const void* data, int bytes) {
      char* begin = const_cast<char*>(static_cast<const char*>(data));
      this->setg(begin, begin, begin + bytes);
    }
  };

  // OFFSET: 0x0074C9D0
  // FUNCTION: fn_07879
  void ReadPng(png_structp png, png_bytep output, png_size_t bytes) {
    static_cast<std::istream*>(png_get_io_ptr(png))->read(reinterpret_cast<char*>(output), bytes);
  }
}

// OFFSET: 0x0074BF10
// FUNCTION: fn_07872
bool BitmapImage::DecodePng(const void* data, int bytes, const char*) {
  unsigned char signature[8] = {};
  if (data) {
    std::memcpy(signature, data, sizeof(signature));
  } else {
    *_errno() = 22;
    _invalid_parameter_noinfo();
  }

  if (png_sig_cmp(signature, 0, sizeof(signature))) {
    return false;
  }

  png_structp png = png_create_read_struct("1.6.36", nullptr, nullptr, nullptr);
  png_infop info;
  if (!png || !(info = png_create_info_struct(png))) {
    png_destroy_read_struct(&png, nullptr, nullptr);
    return false;
  }

  png_infop end = png_create_info_struct(png);
  if (!end) {
    png_destroy_read_struct(&png, &info, nullptr);
    return false;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_read_struct(&png, &info, &end);
    return false;
  }

  MemoryBuffer buffer(data, bytes);
  std::iostream stream(&buffer);
  png_set_read_fn(png, &stream, ReadPng);
  png_read_info(png, info);
  this->width = png_get_image_width(png, info);
  this->height = png_get_image_height(png, info);
  const unsigned colorType = png_get_color_type(png, info);
  const unsigned depth = png_get_bit_depth(png, info);
  png_set_interlace_handling(png);

  if (depth == 16) {
    png_set_strip_16(png);
  }

  if (colorType == PNG_COLOR_TYPE_PALETTE) {
    png_set_palette_to_rgb(png);
  }

  if (colorType == PNG_COLOR_TYPE_GRAY && depth < 8) {
    png_set_expand_gray_1_2_4_to_8(png);
  }

  if (png_get_valid(png, info, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png);
  }

  if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) {
    png_set_filler(png, 255, PNG_FILLER_AFTER);
  }

  if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
    png_set_gray_to_rgb(png);
  }

  png_read_update_info(png, info);
  auto** rows = static_cast<png_bytep*>(std::malloc(sizeof(png_bytep) * static_cast<std::uint32_t>(this->height)));

  for (int y = 0; y < this->height; ++y) {
    rows[y] = static_cast<png_bytep>(std::malloc(png_get_rowbytes(png, info)));
  }

  png_read_image(png, rows);
  const std::uint32_t count = static_cast<std::uint32_t>(this->width) * static_cast<std::uint32_t>(this->height);
  this->pixels = static_cast<std::uint32_t*>(std::malloc(count > 0x3FFFFFFFu ? 0xFFFFFFFFu : count * 4u));

  for (int y = 0; y < this->height; ++y) {
    for (int x = 0; x < this->width; ++x) {
      const unsigned char* pixel = rows[y] + 4u * x;
      this->pixels[static_cast<std::uint32_t>(y) * this->width + x] =
        pixel[2] | (pixel[1] << 8) | (pixel[0] << 16) | (static_cast<std::uint32_t>(pixel[3]) << 24);
    }
  }

  this->alpha = 1;

  for (int y = 0; y < this->height; ++y) {
    std::free(rows[y]);
  }

  std::free(rows);
  png_destroy_read_struct(&png, &info, &end);
  return true;
}
