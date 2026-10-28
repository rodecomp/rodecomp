#include "UI/TextTags.h"

// GLOBAL: gl_24559
std::string navigationTagOpening = "<NAVI>";

// GLOBAL: gl_24560
std::string navigationTagClosing = "</NAVI>";

// GLOBAL: gl_24561
std::string itemTagOpening = "<ITEM>";

// GLOBAL: gl_24562
std::string itemTagClosing = "</ITEM>";

// GLOBAL: gl_24563
std::string itemLinkTagOpening = "<ITEML>";

// GLOBAL: gl_24564
std::string itemLinkTagClosing = "</ITEML>";

// GLOBAL: gl_24558
std::vector<CTagPrototype> g_textTagRegistry = BuildTextTagRegistry();

// OFFSET: 0x009B9170
// FUNCTION: fn_13875
std::vector<CTagPrototype> BuildTextTagRegistry() {
  std::vector<CTagPrototype> registry;
  registry.push_back(CTagPrototype(438, navigationTagOpening, navigationTagClosing));
  registry.push_back(CTagPrototype(464, itemTagOpening, itemTagClosing));
  registry.push_back(CTagPrototype(501, itemLinkTagOpening, itemLinkTagClosing));
  return registry;
}
