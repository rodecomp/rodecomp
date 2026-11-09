#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemResources.h"
#include "Resource/ResourceMgr.h"

#include <cstdlib>
#include <string>

template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

// INLINED
static const char* ItemResourceName(const char* id, int identified) {
  const ITEM_DESC& value = FindItemDescription(std::atoi(id));
  return identified != 0 ? value.identifiedResource : value.unidentifiedResource;
}

// OFFSET: 0x00DDDA20
// FUNCTION: fn_23391
const char* CSession::ItemDragResource(const char* id, int identified) {
  static std::string resource;
  resource = "아이템\\";
  if (ItemResourceName(id, identified) == nullptr) {
    resource += std::string("사과");
  } else {
    resource += ItemResourceName(id, identified);
  }

  return resource.c_str();
}

// OFFSET: 0x00DDDCD0
// FUNCTION: fn_23392
const char* SkillDragResource(ResourceMgr& resources, const char* name) {
  static std::string resource;
  resource = "아이템\\";
  resource += name;

  if (!resources.Exists((resource + ".act").c_str())) {
    resource = "아이템\\am_berserkpitcher";
  }

  return resource.c_str();
}
