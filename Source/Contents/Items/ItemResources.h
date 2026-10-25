#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMRESOURCES_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMRESOURCES_H

class ResourceMgr;
struct ItemDescriptions;

const char* ItemDragResource(ItemDescriptions& descriptions, const char* id, int identified);
const char* SkillDragResource(ResourceMgr& resources, const char* name);

#endif
