#include "UI/UIBarGraph.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x0089B4D0
// FUNCTION: fn_11564
void UIBarGraph4::SetValues(const int* current, const int* maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

// OFFSET: 0x0089B4F0
// FUNCTION: fn_11565
void UIBarGraph::SetValue(float value) {
  this->m_value = value;
  this->Invalidate();
}

// OFFSET: 0x0089B510
// FUNCTION: fn_11566
void UIINT64BarGraph::SetValues(std::int64_t current, std::int64_t maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

// OFFSET: 0x0087A2E0
// FUNCTION: fn_11230
void UIINT64BarGraph::FillBackground(std::uint32_t color) {
  this->ClearSurface(color);
}

// OFFSET: 0x0089B540
// FUNCTION: fn_11567
void UIPetInfoBarGraph::SetValue(int value) {
  this->unknown_007C = value;
  this->Invalidate();
}

// OFFSET: 0x0088C970
// FUNCTION: fn_11375
void UIPetInfoBarGraph::Paint() {
  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_left.bmp")));
  const auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_mid.bmp")));
  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_right.bmp")));
  const int span = this->geometry.width - left->image.width - right->image.width;
  this->DrawBitmap(0, 0, &left->image, false);

  for (int x = 0; x < span; ++x) {
    this->DrawBitmap(left->image.width + x, 0, &middle->image, false);
  }

  this->DrawBitmap(this->geometry.width - right->image.width, 0, &right->image, false);

  if (this->unknown_007C != 0) {
    const auto* barLeft = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gzeblue_left.bmp")));
    const auto* barMiddle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gzeblue_mid.bmp")));
    const auto* barRight = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gzeblue_right.bmp")));
    const int fill = static_cast<int>(
      static_cast<float>(this->geometry.width - barRight->image.width - barLeft->image.width) *
      (static_cast<float>(static_cast<unsigned>(this->unknown_007C)) / 100.0f)
    );
    this->DrawBitmap(0, 0, &barLeft->image, true);
    int x = 0;

    for (; x < fill; ++x) {
      this->DrawBitmap(barLeft->image.width + x, 0, &barMiddle->image, true);
    }

    this->DrawBitmap(barMiddle->image.width * x + barLeft->image.width, 0, &barRight->image, true);
  }
}

// OFFSET: 0x0086D380
// FUNCTION: fn_11045
UIINT64BarGraph::UIINT64BarGraph(UIWindowContext& context) : UIWindow(context, 0), m_current(0), m_maximum(0) {
}

// OFFSET: 0x0086E1B0
// FUNCTION: fn_11066
UIPetInfoBarGraph::UIPetInfoBarGraph(UIWindowContext& context) : UIWindow(context, 0), unknown_007C(0) {
}
