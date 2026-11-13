#include "Render/Text/FontCache.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Lists/UIChatHisBox.h"

#include "Resource/MSGSTRINGID.h"
#include <cstring>
#include <new>
#include <string>

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

      if (label.find(LookupMessage(MSI_NAMELESS_USER)) != std::string::npos) {
        const std::size_t found = result.find(LookupMessage(MSI_NAMELESS_USER));
        const std::string player = LookupPlayerName(row[index]->tipId);

        if (std::strcmp(player.c_str(), name) == 0) {
          result.replace(found, std::strlen(LookupMessage(MSI_NAMELESS_USER)), name, std::strlen(name));
          const std::string tail = result.substr(position);
          std::string opening = "<";
          opening += name;
          const std::size_t start = tail.find(opening);
          const std::string head = result.substr(0, position + start);
          position += start + std::strlen(opening.c_str()) + 1;
          row[index]->column = this->TextWidth(head.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;

          const std::string bracket = this->BracketName(row[index]->item.DisplayName(0));
          row[index]->SetTooltip(bracket.c_str());
          row[index]->label = bracket;
          row[index]->ResizeToTextClipped(this->TextWidth(bracket.c_str(), 0, 0, 12, 0, 0) + 1, row[index]->geometry.height);
          this->lines[line] = result;
        }
      } else {
        const std::string bracket = this->BracketName(row[index]->item.DisplayName(0));
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

  for (unsigned int index = 0; index < this->historyTexts.size(); ++index) {
    if (this->historyTexts[index].find(g_itemLinkTagBegin) != std::string::npos) {
      this->AddText(this->historyTexts[index].c_str(), this->historyColors[index], this->historyItemIds[index].c_str());
    } else {
      this->AddTextWrapped(this->historyTexts[index].c_str(), this->historyColors[index], this->historyItemIds[index].c_str());
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
  this->lineColors.reserve(240);
  this->history.reserve(240);
  this->historyTexts.reserve(240);
  this->historyColors.reserve(240);
  this->historyItemIds.reserve(240);
}
