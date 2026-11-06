#include "Render/Text/FontCache.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/TipBoxTitles.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Lists/UIChatHisBox.h"

#include <cstring>
#include <new>
#include <string>

extern template std::string::basic_string(const std::string&);
extern template std::string& std::string::replace(std::string::size_type, std::string::size_type, const char*, std::string::size_type);

// OFFSET: 0x00881BB0
// FUNCTION: fn_11300
std::string UIChatHisBox::ResolvePlayerTags(const char* name) {
  std::string result = "";


  for (std::vector<UIItemTagButton*>& row : this->itemTags) {
    const int count = static_cast<int>(row.size());
    int position = 0;

    for (int index = 0; index < count; ++index) {
      const int line = row[index]->line;
      result = this->lines[line];
      const std::string label = row[index]->label;

      if (label.find(LookupMessage(581)) != std::string::npos) {
        const std::size_t found = result.find(LookupMessage(581));
        const std::string player = LookupPlayerName(row[index]->unknown_01F8);

        if (std::strcmp(player.c_str(), name) == 0) {
          result.replace(found, std::strlen(LookupMessage(581)), name, std::strlen(name));
          const std::string tail = result.substr(position);
          std::string opening = "<";
          opening += name;
          const std::size_t start = tail.find(opening);
          const std::string head = result.substr(0, position + start);
          position += start + std::strlen(opening.c_str()) + 1;
          row[index]->column = this->TextWidth(head.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;

          const std::string bracket = this->BracketName(reinterpret_cast<const ITEM_INFO&>(row[index]->unknown_00F8).DisplayName(0));
          row[index]->SetTooltip(bracket.c_str());
          const std::string text = bracket.c_str();
          row[index]->label = text;
          row[index]->ResizeToTextClipped(this->TextWidth(bracket.c_str(), 0, 0, 12, 0, 0) + 1, row[index]->geometry.height);
          this->lines[line] = result;
        }
      } else {
        const std::string bracket = this->BracketName(reinterpret_cast<const ITEM_INFO&>(row[index]->unknown_00F8).DisplayName(0));
        const std::string tail = result.substr(position);
        const std::size_t found = tail.find(bracket);
        const std::string head = result.substr(0, position + found);
        position += found + std::strlen(bracket.c_str());
        row[index]->column = this->TextWidth(head.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;
      }
    }
  }

  this->RebuildHistory();
  return result;
}

// OFFSET: 0x0089C080
// FUNCTION: fn_11581
__declspec(noinline) void UIChatHisBox::RebuildHistory() {
  this->Clear();
  this->ClearNaviLinks();

  for (unsigned int index = 0; index < this->unknown_0100.size(); ++index) {
    if (this->unknown_0100[index].find(g_itemLinkTagBegin) != std::string::npos) {
      this->AddText(this->unknown_0100[index].c_str(), this->unknown_010C[index], this->unknown_0118[index].c_str());
    } else {
      this->AddTextWrapped(this->unknown_0100[index].c_str(), this->unknown_010C[index], this->unknown_0118[index].c_str());
    }
  }

  this->Refresh();
}

// OFFSET: 0x0086C8D0
// FUNCTION: fn_11032
UIChatHisBox::UIChatHisBox() : UIListBox() {
  this->maxLines = 240;
  this->lineHeight = 14;
  this->lines.reserve(240);
  this->unknown_00E4.reserve(240);
  this->history.reserve(240);
  this->unknown_0100.reserve(240);
  this->unknown_010C.reserve(240);
  this->unknown_0118.reserve(240);
}
