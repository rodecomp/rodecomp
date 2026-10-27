#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemResources.h"
#include "Resource/ResourceMgr.h"

#include <cstdlib>
#include <string>

// OFFSET: 0x00DDDA20
// FUNCTION: fn_23391
const char* ItemDragResource(ItemDescriptions& descriptions, const char* id, int identified) {
  static std::string resource;
  resource = "아이템\\";
  const ITEM_DESC& value = descriptions.Find(std::atoi(id));
  const char* name = identified != 0 ? value.identifiedResource : value.unidentifiedResource;
  resource += name ? name : "사과";

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
