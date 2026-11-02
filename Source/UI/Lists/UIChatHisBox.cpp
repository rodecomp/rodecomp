#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/TipBoxTitles.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Text/ItemLinkDisplay.h"

#include <cstring>
#include <new>
#include <string>

// OFFSET: 0x00881BB0
// FUNCTION: fn_11300
std::string UIChatHisBox::ResolvePlayerTags(const char* name) {
  std::string result = "";

  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  const unsigned short codePage = static_cast<unsigned short>(g_textCodePage);
  const TipBoxTitles titles{g_itemTagOwner->m_entries, g_itemTagOwner->m_missing, g_itemTagOwner->m_maximumId, codePage};
  ItemLinkDisplay display{*this->geometry.surface, this->m_listContext->fonts, this->m_listContext->locale, naming, titles, this->geometry.width};

  for (std::vector<UIItemTagButton*>& row : this->ItemTags()) {
    const int count = static_cast<int>(row.size());
    int position = 0;

    for (int index = 0; index < count; ++index) {
      UIItemTagButton* tag = row[index];
      const int line = tag->line;
      result = this->lines[line];
      const std::string label = tag->label;

      if (label.find(LookupMessage(581)) == std::string::npos) {
        const std::string bracket = display.BracketName(naming.DisplayName(reinterpret_cast<const ITEM_INFO&>(tag->unknown_00F8), 0));
        const std::string tail = result.substr(position);
        const std::size_t found = tail.find(bracket);
        const std::string head = result.substr(0, position + found);
        position += found + std::strlen(bracket.c_str());
        tag->column = this->TextWidth(head.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;
      } else {
        const std::size_t found = result.find(LookupMessage(581));
        const std::string player = LookupPlayerName(tag->unknown_01F8);

        if (std::strcmp(player.c_str(), name) == 0) {
          result.replace(found, std::strlen(LookupMessage(581)), name, std::strlen(name));
          const std::string tail = result.substr(position);
          std::string opening = "<";
          opening += name;
          const std::size_t start = tail.find(opening);
          const std::string head = result.substr(0, position + start);
          position += start + std::strlen(opening.c_str()) + 1;
          tag->column = this->TextWidth(head.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;

          const std::string bracket = display.BracketName(naming.DisplayName(reinterpret_cast<const ITEM_INFO&>(tag->unknown_00F8), 0));
          tag->SetTooltip(bracket.c_str());
          const std::string text = bracket.c_str();
          tag->label = text;
          tag->ResizeToTextClipped(this->TextWidth(bracket.c_str(), 0, 0, 12, 0, 0) + 1, tag->geometry.height);
          this->lines[line] = result;
        }
      }
    }
  }

  this->sub_89C080();
  return result;
}

// OFFSET: 0x0089C080
// FUNCTION: fn_11581
void UIChatHisBox::sub_89C080() {
  this->Clear();
  this->ClearNaviLinks();

  for (unsigned int index = 0; index < this->unknown_0100.size(); ++index) {
    if (this->unknown_0100[index].find("<ITEML>") == std::string::npos) {
      this->AddTextWrapped(this->unknown_0100[index].c_str(), this->unknown_010C[index], this->unknown_0118[index].c_str());
    } else {
      this->AddText(this->unknown_0100[index].c_str(), this->unknown_010C[index], this->unknown_0118[index].c_str());
    }
  }

  this->Refresh();
}

// OFFSET: 0x0086C8D0
// FUNCTION: fn_11032
UIChatHisBox::UIChatHisBox(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list) {
  new (&this->ItemTags()) std::list<std::vector<UIItemTagButton*>>();
  this->maxLines = 240;
  this->lineHeight = 14;
  this->lines.reserve(240);
  this->unknown_00E4.reserve(240);
  this->history.reserve(240);
  this->unknown_0100.reserve(240);
  this->unknown_010C.reserve(240);
  this->unknown_0118.reserve(240);
}
