#include "Contents/Map/FlyingShipRouteInfo.h"
#include "Render/Renderer.h"
#include "Render/Projection/ScreenGeometry.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Navigation/UIRoMapWnd.h"

#include <cstring>
#include <map>

// OFFSET: 0x009105F0
// FUNCTION: fn_12503
CFlyingShipRouteInfo::CFlyingShipRouteInfo()
    : padding_0010(0), padding_0014(0), segmentIndex(0), segmentProgress(0), segmentLength(0), travelledDistance(0), routeReset(1), routeType(0), lineColor(0),
      routeState(0) {
}

// OFFSET: 0x00910A70
// FUNCTION: fn_12507
RoMapMarker::RoMapMarker(RoMapMarker&& other) noexcept = default;

// OFFSET: 0x0092B000
// FUNCTION: fn_12657
void CFlyingShipRouteInfo::Draw(UIRoMapWnd* wnd) {
  if (this->lineTextureName.empty()) {
    return;
  }

  std::map<std::string, RoMapMarker> targets;
  for (std::size_t i = 0; i < this->entries.size() - 1; ++i) {
    const RoMapMarker& from = this->entries[i];
    const RoMapMarker& entry = this->entries[i + 1];
    targets.insert(std::pair<std::string, RoMapMarker>(entry.name, entry));

    DrawScreenLine(
      ScreenPoint{from.center[0], from.center[1]}, ScreenPoint{entry.center[0], entry.center[1]}, 2,
      g_texMgr.Load(this->lineTextureName.c_str(), 0, true, true, false), static_cast<std::uint32_t>(this->lineColor)
    );
  }

  const RoMapMarker& from = this->entries[this->segmentIndex];
  const RoMapMarker& to = this->entries[this->segmentIndex + 1];
  wnd->DrawDashedArrow(
    from.center[0], from.center[1], to.center[0], to.center[1], 0x10, 0x10,
    g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_Airf.bmp", 0, true, true, false), 0x14,
    g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_airplen32.bmp", 0, true, true, false), this->segmentProgress, 0xFFFFFFFF, 6
  );

  for (std::map<std::string, RoMapMarker>::iterator it = targets.begin(); it != targets.end(); ++it) {
    wnd->DrawMapMarker(it->second, 1);
  }
}
