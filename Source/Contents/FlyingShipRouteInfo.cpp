#include "Contents/FlyingShipRouteInfo.h"
#include "Render/Renderer.h"
#include "Render/ScreenGeometry.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Window/UIRoMapWnd.h"

#include <cstring>
#include <map>

// OFFSET: 0x009105F0
// FUNCTION: fn_12503
CFlyingShipRouteInfo::CFlyingShipRouteInfo()
    : unknown_0010(0), unknown_0014(0), unknown_0018(0), unknown_001C(0), unknown_0020(0), unknown_0024(0), unknown_0028(1), unknown_002C(0), unknown_0030(0),
      unknown_0034(0) {
}

// OFFSET: 0x00910A70
// FUNCTION: fn_12507
FlyingShipRouteEntry::FlyingShipRouteEntry(FlyingShipRouteEntry&& other) noexcept = default;

// OFFSET: 0x0092B000
// FUNCTION: fn_12657
void CFlyingShipRouteInfo::Draw(UIRoMapWnd* wnd) {
  if (this->unknown_0038.empty()) {
    return;
  }

  std::map<std::string, RoMapMarker> targets;
  for (std::size_t i = 0; i + 1 < this->entries.size(); ++i) {
    const FlyingShipRouteEntry& from = this->entries[i];
    const FlyingShipRouteEntry& entry = this->entries[i + 1];
    RoMapMarker marker;
    marker.name = entry.name;
    marker.area = {entry.unknown_0018[0], entry.unknown_0018[1], entry.unknown_0018[2], entry.unknown_0018[3], entry.unknown_0028};
    marker.centerX = entry.unknown_002C[0];
    marker.centerY = entry.unknown_002C[1];
    marker.width = entry.unknown_0034;
    marker.height = entry.unknown_0038;
    targets.insert(std::pair<std::string, RoMapMarker>(entry.name, marker));

    CTexture* texture = g_texMgr.Load(this->unknown_0038.c_str(), 0, true, true, false);
    DrawScreenLine(
      g_renderer->primitives, g_renderer->queues, from.unknown_002C[0], from.unknown_002C[1], entry.unknown_002C[0], entry.unknown_002C[1], 2, texture,
      static_cast<std::uint32_t>(this->unknown_0030), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
  }

  const FlyingShipRouteEntry& from = this->entries[this->unknown_0018];
  const FlyingShipRouteEntry& to = this->entries[this->unknown_0018 + 1];
  CTexture* head = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_airplen32.bmp", 0, true, true, false);
  CTexture* body = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_Airf.bmp", 0, true, true, false);
  wnd->DrawDashedArrow(
    from.unknown_002C[0], from.unknown_002C[1], to.unknown_002C[0], to.unknown_002C[1], 0x10, 0x10, body, 0x14, head, this->unknown_001C, 0xFFFFFFFF, 6
  );

  for (std::map<std::string, RoMapMarker>::iterator it = targets.begin(); it != targets.end(); ++it) {
    wnd->DrawMapMarker(it->second, 1);
  }
}
