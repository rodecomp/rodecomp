#include "Client/InputCharset.h"
#include "Client/TextInputBuffer.h"
#include "Render/DeviceContext.h"
#include "Render/TextContext.h"
#include "Resource/BitmapImage.h"
#include "UI/EditPainting.h"
#include "UI/EditViewport.h"
#include "UI/UIWindow.h"

#include <algorithm>
#include <bit>
#include <cstring>

namespace {
  int Add(int left, int right) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(left) + static_cast<std::uint32_t>(right));
  }
}

// OFFSET: 0x00856980
// FUNCTION: fn_10755
void EditPainting::Draw() {
  TextContext context(this->surface, this->fonts, this->locale);
  context.SetFont(0, 12, 0, 0);
  const auto measure = [&](const char* text, int bytes) {
    return context.Measure(text, bytes ? bytes : static_cast<int>(std::strlen(text))).cx;
  };
  int x = 3;

  if (this->invalidatesParent == 0 && this->firstVisible != 0) {
    x = Add(3, -measure(this->text.c_str(), this->firstVisible));
  }

  const auto draw = [&](const std::string& value) {
    const bool placeholder =
      this->alternate && !this->focused && this->placeholderEnabled && this->secondColor == 0xFFFFFFFFu;
    context.SetColor(placeholder ? this->placeholderColor : this->color);
    const std::string& displayed = placeholder && value.empty() ? *this->placeholder : value;
    context.Draw(x, this->y, displayed.c_str(), static_cast<int>(displayed.size()));

    if (this->secondColor != 0xFFFFFFFFu) {
      context.SetColor(this->secondColor);
      context.Draw(Add(x, -1), this->y, value.c_str(), static_cast<int>(value.size()));
    }
  };
  const auto highlight = [&](int left, int extent) {
    // Highlight helper samples theme pixel (6, 2).
    const int start = std::max(left, 3);
    const int adjusted = left < 3 ? Add(extent, Add(left, -3)) : extent;

    if (start < this->width && adjusted > 0) {
      std::uint32_t pixel = 0x00FF0000u;

      if (this->colorChip.width > 6 && this->colorChip.height > 2) {
        pixel = this->colorChip.pixels[2 * this->colorChip.width + 6];
      }

      const std::uint32_t color = ((pixel >> 16) & 255) | (pixel & 0xFF00) | ((pixel & 255) << 16);
      RECT rectangle{start, 0, std::min(Add(start, adjusted), this->width), this->height};
      this->surface.Fill(&rectangle, color);
    }
  };
  const auto moveCandidate = [&](UIWindow* window, int offset) {
    if (!window) {
      return;
    }

    int left = Add(offset, this->absoluteX);
    int top = Add(Add(this->height, 6), this->absoluteY);

    if (Add(window->geometry.width, left) > this->screenWidth) {
      left = Add(this->screenWidth, -window->geometry.width);
    }

    if (Add(window->geometry.height, top) > this->screenHeight) {
      top = Add(Add(Add(top, -12), -this->height), -window->geometry.height);
    }

    window->Move(left, top);
  };
  std::string display;

  if (this->focused) {
    this->viewport.Synchronize();
    unsigned begin = static_cast<unsigned>(std::min(this->anchor, this->cursor));
    unsigned end = static_cast<unsigned>(std::max(this->anchor, this->cursor));

    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
    } else {
      const char* prefix = this->alternate ? this->input.candidate.c_str() : this->charset.Prefix(false);
      display = prefix;
      display += this->text;
      const auto prefixBytes = static_cast<unsigned>(std::strlen(prefix));
      begin += prefixBytes;
      end += prefixBytes + static_cast<unsigned>(this->candidateCursor);
    }

    const int left = begin == 0 ? 0 : measure(display.c_str(), static_cast<int>(begin));
    const int right = end == 0 ? 0 : measure(display.c_str(), static_cast<int>(end));

    if (this->input.cursor != this->input.anchor) {
      highlight(Add(x, left), Add(right, -left));
      draw(display);
    } else if (this->candidateCursor != 0) {
      moveCandidate(this->candidateWindow, left);
      moveCandidate(this->readingWindow, right);

      if (((this->tick / 500u) & 1u) == 0) {
        highlight(Add(x, left), std::max(Add(right, -left), 2));
      }

      draw(display);
    } else {
      draw(display);

      if (
        ((this->tick / 500u) & 1u) == 0 &&
        (this->alternate ? this->caretEnabled : (!this->chatInputFocused || this->caretEnabled))
      ) {
        context.SetColor(this->color);
        context.Draw(Add(Add(x, left), -2), this->y, "|", 1);
      }
    }
  } else {
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
    } else {
      display = this->text;
    }

    draw(display);

    if (this->invalidatesParent != 0 && this->parent) {
      this->parent->Invalidate();
    }
  }

  if (std::any_of(this->linkWindows.begin(), this->linkWindows.end(), [](const UIWindow* window) {
        return window->geometry.id == 438;
      })) {
    this->links.Update();
  }
}
