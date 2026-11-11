#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Items/UIItemStoreComboBox.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include <cstdio>
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UIString.h"
#include "UI/Login/UIDescriptRace.h"
#include "Contents/Tables/MessageTable.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Resource/MSGSTRINGID.h"
#include <timeapi.h>
namespace {
  constexpr char prefix[] = "유저인터페이스\\";
}

// OFFSET: 0x0086CC20
// FUNCTION: fn_11035
UIComboBoxDefWnd::UIComboBoxDefWnd()
    : UIWindow(0), button(nullptr), list(nullptr), label(nullptr), borderless(0), expanded(0), red(255), green(255), blue(255), borderRed(0),
      borderGreen(0), borderBlue(0), dropdownHeight(40) {
}

// OFFSET: 0x00872AE0
// FUNCTION: fn_11134
UIComboBoxDefWnd::~UIComboBoxDefWnd() = default;

// OFFSET: 0x00885920
// FUNCTION: fn_11319
void UIComboBoxDefWnd::OnCreate(int, int) {
  const std::string path = "유저인터페이스\\basic_interface\\";

  this->button = new UIBitmapButton();
  this->button->SetBitmap((path + "txtbox_btn_a" + ".bmp").c_str(), 0, 0);
  this->button->SetBitmap((path + "txtbox_btn_b" + ".bmp").c_str(), 1, 0);
  this->button->SetBitmap((path + "txtbox_btn_c" + ".bmp").c_str(), 2, 0);

  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  this->list = new UIComboBoxEx();
  this->list->SetSize(this->geometry.width, this->dropdownHeight);
  this->list->Move(0, this->button->BitmapHeight());
  this->AddChild(this->list);

  this->label = new UIStaticText();
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), this->button->BitmapHeight());
  this->label->SetText("(NO SELECT)", 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x00878DF0
// FUNCTION: fn_11213
void UIComboBox_BmpWnd::Create(int width, int height, const char* path, const char* name) {
  UIWindow::Resize(width, height);
  this->name = name;
  const std::string base(path);

  this->button = new UIBitmapButton();
  this->button->SetBitmap((base + "_out.bmp").c_str(), 0, 0);
  this->button->SetBitmap((base + "_over.bmp").c_str(), 1, 0);
  this->button->SetBitmap((base + "_press.bmp").c_str(), 2, 0);
  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  this->list = new UIComboBoxEx();
  this->list->SetSize(this->geometry.width, this->dropdownHeight);
  this->list->Move(0, this->button->BitmapHeight());
  this->AddChild(this->list);

  char text[12] = {};
  std::sprintf(text, "(NO SELECT)");
  this->label = new UIStaticText();
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), this->button->BitmapHeight());
  this->label->SetText(text, 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x00889D60
// FUNCTION: fn_11352
void UIComboBoxDefWnd::Paint() {
  if (this->borderless == 0) {
    RECT rectangle = {0, 0, this->geometry.width, this->geometry.height};
    this->FillSurface(RGB(this->borderRed, this->borderGreen, this->borderBlue), &rectangle);

    SetRect(&rectangle, 1, 1, this->geometry.width - 1, this->geometry.height - 1);
    this->FillSurface(RGB(this->red, this->green, this->blue), &rectangle);
  } else {
    this->ClearSurface(RGB(this->red, this->green, this->blue));
  }
}

// OFFSET: 0x00890B50
// FUNCTION: fn_11434
void UIComboBoxDefWnd::OnLBtnUp(int, int) {
  this->SendMsg(nullptr, UM_COMMAND, ID_OPEN_MEMBER_POS_COMBO, 0, 0, 0);
}

// OFFSET: 0x0089F5C0
// FUNCTION: fn_11623
void UIComboBoxDefWnd::Clear() {
  this->list->Clear();
}

// OFFSET: 0x008A15A0
// FUNCTION: fn_11635
std::string UIComboBoxDefWnd::GetSelectedText() {
  return this->label->tooltip.c_str();
}

// OFFSET: 0x008A25F0
// FUNCTION: fn_11641
void UIComboBoxDefWnd::AddOption(std::string text, std::uint32_t color) {
  this->list->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0);
}

// OFFSET: 0x008A2730
// FUNCTION: fn_11642
void UIComboBoxDefWnd::Select(int index) {
  this->list->SendMsg(nullptr, UM_SETCURITEM, index, 0, 0, 0);
  this->label->SetText(this->list->lines[index].text.c_str(), 0);
}

// OFFSET: 0x008A28B0
// FUNCTION: fn_11643
void TrimTrailing(std::string& text, const char* characters) {
  if (!text.empty()) {
    const std::size_t end = text.find_last_not_of(characters) + 1;
    if (end < text.size()) {
      text.erase(end, text.size() - end);
    }
  }
}

// OFFSET: 0x00897E90
// FUNCTION: fn_11544
int UIComboBoxDefWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case UM_COMMAND:
    if (value == ID_OPEN_MEMBER_POS_COMBO) {
      this->expanded = this->expanded == 0;
      this->Resize(
        this->geometry.width, this->expanded ? (this->geometry.height + this->list->geometry.height) : (this->geometry.height - this->list->geometry.height)
      );
      this->parent->Invalidate();
      this->Invalidate();
      this->list->Invalidate();
      if (this->Root()) {
        this->Root()->Invalidate();
      }

      if (this->parent) {
        this->parent->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
    }

    return 0;
  case UM_COMBOBOX_SELECTED:
    if (g_windowMgr.WindowPolicy(3, this->parent->geometry.id) == 0 ||
        value < 0) {
      return 0;
    }

    this->label->SetText(this->list->lines[value].text.c_str(), 0);
    this->SendMsg(nullptr, UM_COMMAND, ID_OPEN_MEMBER_POS_COMBO, 0, 0, 0);
    this->parent->SendMsg(this, UM_COMBOBOX_SELECTED, value, 0, 0, 0);
    return 0;
  case UM_CHANGEMSG:
    if (value >= 0) {
      this->label->SetText(this->list->lines[value].text.c_str(), 0);
      this->Invalidate();
    }

    return 0;
  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00898940
// FUNCTION: fn_11551
int UIItemStoreComboBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case UM_COMMAND:
    if (value == ID_OPEN_MEMBER_POS_COMBO) {
      this->expanded = this->expanded == 0;

      if (this->expanded) {
        this->list->Show(1);
        this->Resize(this->geometry.width, this->list->geometry.height + this->geometry.height);
        this->Move(214, this->geometry.y - this->list->geometry.height);
        this->button->Move(this->geometry.width - this->button->BitmapWidth(), this->geometry.height - this->button->BitmapHeight());
        this->label->Move(2, this->list->geometry.height + 3);
      } else {
        this->list->Show(0);
        this->Resize(this->geometry.width, this->geometry.height - this->list->geometry.height);
        this->Move(214, this->list->geometry.height + this->geometry.y);
        this->button->Move(this->geometry.width - this->button->BitmapWidth(), 0);
        this->label->Move(2, 3);
      }

      this->parent->Invalidate();
      this->list->Invalidate();
      this->Invalidate();
    }

    return 0;
  case UM_COMBOBOX_SELECTED:
    if (value < 0) {
      return 0;
    }

    this->label->SetText(this->list->lines[value].text.c_str(), 0);
    this->SendMsg(nullptr, UM_COMMAND, ID_OPEN_MEMBER_POS_COMBO, 0, 0, 0);
    this->parent->SendMsg(this, UM_COMBOBOX_SELECTED, value, 0, 0, 0);
    return 0;
  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x0086CBA0
// FUNCTION: fn_11034
UIComboBoxDef2Wnd::UIComboBoxDef2Wnd()
    : UIComboBoxDefWnd(), skin(0), alignment(0), textOffset(0), previousTextX(0) {
}

// OFFSET: 0x00872A70
// FUNCTION: fn_11133
UIComboBoxDef2Wnd::~UIComboBoxDef2Wnd() = default;

// OFFSET: 0x00885250
// FUNCTION: fn_11318
void UIComboBoxDef2Wnd::OnCreate(int width, int) {
  std::string path = "유저인터페이스\\UIadventurerAgency\\";
  if (this->skin == 1) {
    path = "유저인터페이스\\login_interface\\";
  }
  this->button = new UIBitmapButton();
  this->button->SetBitmap((path + "btn_dropdown_out" + ".bmp").c_str(), 0, 0);
  this->button->SetBitmap((path + "btn_dropdown_over" + ".bmp").c_str(), 1, 0);
  this->button->SetBitmap((path + "btn_dropdown_press" + ".bmp").c_str(), 2, 0);

  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  auto* list = new UIComboBoxEx2();
  this->list = list;
  list->alignment = this->alignment;
  list->SetSize(width, (this->dropdownHeight + 2));
  list->Move(0, this->button->BitmapHeight());
  this->AddChild(list);

  this->label = new UIStaticText();
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), (this->button->BitmapHeight() - 6));
  this->label->SetText("", 0);

  if (this->alignment == 0) {
    this->label->Move(2, (this->textOffset + 3));
  } else if (this->alignment == 1) {
    this->label->Move(static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f - static_cast<float>(this->TextWidth(std::string(this->label->text).c_str(), 0, 0, 12, 0, 0)) * 0.5f), (this->textOffset + 3));
  }

  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x00889BB0
// FUNCTION: fn_11351
void UIComboBoxDef2Wnd::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->alignment != 1) {
    return;
  }

  const int x = static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f - static_cast<float>(this->TextWidth(std::string(this->label->text).c_str(), 0, 0, 12, 0, 0)) * 0.5f);

  if (x != this->previousTextX) {
    this->previousTextX = x;
    this->label->Move(static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f - static_cast<float>(this->TextWidth(std::string(this->label->text).c_str(), 0, 0, 12, 0, 0)) * 0.5f), (this->textOffset + 3));
    this->label->Invalidate();
  }
}

// OFFSET: 0x00886C30
// FUNCTION: fn_11329
void UINewComboBoxCtrl::OnCreate(int, int) {
  const std::string base = "유저인터페이스\\basic_interface\\txtbox_btn_";
  this->button = new UIBitmapButton();
  this->button->SetBitmap((base + "a.bmp").c_str(), 0, 0);
  this->button->SetBitmap((base + "b.bmp").c_str(), 1, 0);
  this->button->SetBitmap((base + "c.bmp").c_str(), 2, 0);
  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  this->label = new UIStaticText();
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), this->button->BitmapHeight());
  this->label->SetText("(No Select)", 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x008994B0
// FUNCTION: fn_11555
int UINewComboBoxCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    if (message != UM_COMBOBOX_SELECTED) {
      return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }

    this->dropped = false;
    const int index = static_cast<int>(argument1);
    if (index < 0) {
      return 0;
    }

    if (static_cast<unsigned int>(index) <= this->items.size()) {
      this->label->SetText(this->items[index].c_str(), 0);
    }

    this->selected = index;
    this->parent->SendMsg(this, UM_COMBOBOX_SELECTED, index, 0, 0, 0);
    this->Invalidate();
    this->parent->Invalidate();
    if (this->Root()) {
      this->Root()->Invalidate();
    }
    return 0;
  }

  if (argument1 == ID_OPEN_MEMBER_POS_COMBO) {
    this->dropped = !this->dropped;
    if (g_windowMgr.comboBoxWnd) {
      g_windowMgr.CloseWindow(WID_COMBOBOXWND);
    }

    if (this->dropped) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_COMBOBOXWND);
      const int count = static_cast<int>(this->items.size());
      for (int index = 0; index < count; ++index) {
        window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->items[index].c_str()), 0, 0, 0);
      }
      window->SendMsg(nullptr, UM_SETCURITEM, this->selected, 0, 0, 0);
      window->Resize(this->geometry.width, count * 16);
      static_cast<UIComboBoxWnd*>(window)->SetOwner(this);
      int x = 0;
      int y = this->geometry.height;
      this->ToScreen(x, y);
      window->Move(x, y);
    }
  }
  return 0;
}

// OFFSET: 0x008A1640
// FUNCTION: fn_11636
void UITextViewEX::AddLineLinks(UITextButton* const* first, UITextButton* const* last) {
  std::vector<UITextButton*> links;
  for (; first != last; ++first) {
    links.push_back(*first);
  }

  this->lineLinks.push_back(std::vector<UITextButton*>(links));
}

// INLINED
static void TrimLeading(std::string& text, const char* characters) {
  if (!text.empty()) {
    const std::size_t first = text.find_first_not_of(characters);
    if (first != 0) {
      text.erase(0, first);
    }
  }
}

// OFFSET: 0x008A1880
// FUNCTION: fn_11638
void UIString::ParseTags(std::string text) {
  TrimLeading(text, " \t");

  if (text[0] != '<') {
    this->text = text;
    return;
  }

  const std::size_t begin = text.find_first_not_of("<");
  const std::size_t end = text.find_first_of(">", begin);
  std::string tags = text.substr(begin, end - begin);
  std::size_t position = 0;

  for (;;) {
    std::size_t index = tags.find_first_of("=", position);
    if (index == std::string::npos) {
      break;
    }

    std::string key = tags.substr(position, index - position);
    if (key.empty()) {
      break;
    }

    TrimLeading(key, " \t");
    TrimTrailing(key, " \t");

    if (key == "image") {
      const std::size_t start = tags.find_first_of("\"", index) + 1;
      index = tags.find_first_of("\"", start);
      this->image = tags.substr(start, index - start);
      TrimLeading(this->image, " \t");
      TrimTrailing(this->image, " \t");
      TrimLeading(this->image, "\"");
      TrimTrailing(this->image, "\"");
    } else if (key == "color") {
      const std::size_t start = tags.find_first_of(" \t", index) + 1;
      index = tags.find_first_of(" \t", start);
      const std::string value = tags.substr(start, index - start);
      const std::string red = value.substr(0, 2);
      const std::string green = value.substr(2, 2);
      const std::string blue = value.substr(4, 2);
      char* stop;
      this->red = std::strtol(red.c_str(), &stop, 16);
      this->green = std::strtol(green.c_str(), &stop, 16);
      this->blue = std::strtol(blue.c_str(), &stop, 16);
    } else if (key == "link") {
      const std::size_t start = tags.find_first_of("\"", index) + 1;
      index = tags.find_first_of("\"", start);
      this->link = tags.substr(start, index - start);
      TrimLeading(this->link, " \t");
      TrimTrailing(this->link, " \t");
      TrimLeading(this->link, "\"");
      TrimTrailing(this->link, "\"");
      this->red = 10;
      this->green = 10;
      this->blue = 255;
    } else if (key == "font") {
      const std::size_t start = tags.find_first_of("\"", index) + 1;
      index = tags.find_first_of("\"", start);
      this->fontName = tags.substr(start, index - start);
      TrimLeading(this->fontName, " \t");
      TrimTrailing(this->fontName, " \t");
      TrimLeading(this->fontName, "\"");
      TrimTrailing(this->fontName, "\"");
    } else if (key == "bold") {
    } else if (key == "size") {
      const std::size_t start = tags.find_first_of(" \t", index) + 1;
      index = tags.find_first_of(" \t", start);
      const std::string value = tags.substr(start, index - start);
      this->fontSize = std::atoi(value.c_str());
    }

    position = index + 1;
  }

  this->text = text.substr(end + 1, text.find_first_of("<", end + 1) - (end + 1));
}

// OFFSET: 0x008A0B10
// FUNCTION: fn_11630
void UIDescriptRace::DrawRaceDescription() {
  if (this->m_race == 1) {
    if (!g_doramCreationEnabled) {
      return;
    }

    this->DrawTextAt(50, 23, LookupMessage(MSI_NEWMAKECHAR_DORAMINFO), 0, 0, 12, 0x99634C, 1, 0);
    this->DrawTextAt(49, 22, LookupMessage(MSI_NEWMAKECHAR_DORAMINFO), 0, 0, 12, 0xFFFFFF, 1, 0);
    std::vector<std::string> lines;
    WrapPlainText(LookupMessage(MSI_CASH_MARK2), lines, 40);
    int y = 22;
    for (unsigned index = 0; index < lines.size(); ++index) {
      this->DrawTextAt(112, y, lines[index].c_str(), 0, 0, 12, 0, 0, 0);
      y += 20;
    }
  } else if (this->m_race == 0) {
    this->DrawTextAt(50, 23, LookupMessage(MSI_NEWMAKECHAR_DORAMJOB), 0, 0, 12, 0x99634C, 1, 0);
    this->DrawTextAt(49, 22, LookupMessage(MSI_NEWMAKECHAR_DORAMJOB), 0, 0, 12, 0xFFFFFF, 1, 0);
    std::vector<std::string> lines;
    WrapPlainText(LookupMessage(MSI_CASH_MARK1), lines, 40);
    int y = 22;
    for (unsigned index = 0; index < lines.size(); ++index) {
      this->DrawTextAt(112, y, lines[index].c_str(), 0, 0, 12, 0, 0, 0);
      y += 20;
    }
  }

  const int job = this->m_race == 1 ? 4218 : 0;
  const int action = this->m_noRace != 0 ? 9 : 1;
  int frame = 0;
  for (int layer = 0; layer < 2; ++layer) {
    CActRes* act = nullptr;
    CSprRes* sprite = nullptr;
    CImfRes* imf = nullptr;
    switch (layer) {
      case 0:
        act = static_cast<CActRes*>(ResourceMgr::Instance().Load(GetBodyActionResourceName(job, g_session.m_sex, job, 0)));
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetBodySprName(job, g_session.m_sex, job, 0)));
        imf = static_cast<CImfRes*>(ResourceMgr::Instance().Load(GetBodyImfName(job, g_session.m_sex, 1, job).c_str()));
        break;
      case 1: {
        int head = 1;
        act = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetHeadActName(job, g_session.m_sex, &head).c_str()));
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetHeadSprName(job, g_session.m_sex, &head).c_str()));
        imf = static_cast<CImfRes*>(ResourceMgr::Instance().Load(GetBodyImfName(job, g_session.m_sex, head, job).c_str()));
        break;
      }
    }

    if (this->m_noRace) {
      if (this->m_createTime + 60 <= timeGetTime()) {
        int next = this->m_frameIndex + 1;
        if (next >= static_cast<int>(act->FrameCount(action))) {
          next = 0;
        }
        this->m_frameIndex = next;
        this->m_createTime = timeGetTime();
      }
      frame = this->m_frameIndex;
    }

    const ActionFrame* actionFrame = &act->Frame(action, frame);
    const std::uint32_t* palette = sprite->palette;
    POINT point = imf->GetLongPoint(layer, action, frame);
    if (layer == 1) {
      const ActionFrame* body = &static_cast<CActRes*>(ResourceMgr::Instance().Load(GetBodyActionResourceName(job, g_session.m_sex, job, 0)))->Frame(action, frame);
      if (actionFrame->anchorCount != 0 && body->anchorCount != 0 && actionFrame->anchors[0].attachmentId == body->anchors[0].attachmentId) {
        point.x += body->anchors[0].x - actionFrame->anchors[0].x;
        point.y += body->anchors[0].y - actionFrame->anchors[0].y;
      }
    }

    const SpriteFrameCell& cell = actionFrame->Cell(layer);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    SpriteAtlasRecord record{};
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }

    this->geometry.surface->DrawSurface(
      cell.x + point.x + 60, cell.y + point.y + 140, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f), image->width,
      image->height, cell.mirrored, image->doubleWidth + 1, image->doubleHeight + 1, 0, 0
    );
  }
}

// OFFSET: 0x0089F610
// FUNCTION: fn_11624
void UITextViewEX::SetText(std::string text) {
  std::size_t position = 0;
  for (;;) {
    const std::size_t tag = text.find_first_of("<", position);
    std::string segment = text.substr(position, tag - position);
    int attached = segment.find_first_not_of(" \t") == 0;
    TrimLeading(segment, " \t");
    TrimTrailing(segment, " \t");

    std::size_t offset = 0;
    for (;;) {
      const std::size_t caret = segment.find_first_of("^", offset);
      if (offset == caret) {
        const std::size_t next = segment.find_first_of("^", caret + 1);
        std::string colored = segment.substr(caret, next - caret + 7);
        if (colored[0] == '^') {
          std::string color = colored.substr(1, 6);
          std::string words = colored.substr(7, colored.size() - 14);
          std::string red = color.substr(0, 2);
          std::string green = color.substr(2, 2);
          std::string blue = color.substr(4, 2);
          char* stop;
          const int redValue = std::strtol(red.c_str(), &stop, 16);
          const int greenValue = std::strtol(green.c_str(), &stop, 16);
          const int blueValue = std::strtol(blue.c_str(), &stop, 16);
          attached = this->AddWords(words, attached, redValue, greenValue, blueValue);
        }

        offset = next + 7;
        continue;
      }

      if (caret == std::string::npos) {
        this->AddWords(segment.substr(offset, caret - offset), attached, 0, 0, 0);
        break;
      }

      attached = this->AddWords(segment.substr(offset, caret - offset), attached, 0, 0, 0);
      const std::size_t next = segment.find_first_of("^", caret + 1);
      std::string colored = segment.substr(caret, next - caret + 7);
      if (colored[0] == '^') {
        std::string color = colored.substr(1, 6);
        std::string words = colored.substr(7, colored.size() - 14);
        std::string red = color.substr(0, 2);
        std::string green = color.substr(2, 2);
        std::string blue = color.substr(4, 2);
        char* stop;
        const int redValue = std::strtol(red.c_str(), &stop, 16);
        const int greenValue = std::strtol(green.c_str(), &stop, 16);
        const int blueValue = std::strtol(blue.c_str(), &stop, 16);
        attached = this->AddWords(words, attached, redValue, greenValue, blueValue);
      }

      offset = next + 7;
      if (offset >= segment.size()) {
        break;
      }
    }

    if (tag == std::string::npos) {
      break;
    }

    const std::size_t close = text.find("<\\end>", tag);
    segment = text.substr(tag, close - tag);
    UIString* link = new UIString();
    link->SetText(100, 100, segment);
    link->Show(0);
    this->AddChild(link);
    this->extraLinks.push_back(link);
    position = close + 6;
  }

  int x = 0;
  auto first = this->extraLinks.begin();
  auto link = first;
  const auto last = this->extraLinks.end();
  for (; link != last; ++link) {
    UIString* label = static_cast<UIString*>(*link);
    const std::string caption = label->text;
    x += label->geometry.width + 5;
    if (x + 15 > this->geometry.width || caption == "\n") {
      this->AddLineLinks(reinterpret_cast<UITextButton* const*>(std::to_address(first)), reinterpret_cast<UITextButton* const*>(std::to_address(link)));
      first = link;
      x = label->text == "\n" ? 0 : label->geometry.width + 5;
    }
  }

  this->AddLineLinks(reinterpret_cast<UITextButton* const*>(std::to_address(first)), reinterpret_cast<UITextButton* const*>(std::to_address(link)));
  this->Refresh();
}

// OFFSET: 0x008A0480
// FUNCTION: fn_11625
int UITextViewEX::AddWords(std::string text, int attached, int red, int green, int blue) {
  TrimLeading(text, " \t");
  TrimTrailing(text, " \t");

  std::size_t position = 0;
  for (;;) {
    const std::size_t space = text.find_first_of(" \t\n", position);
    std::string word = text.substr(position, space - position);
    if (position == space) {
      if (text[position] != '\n') {
        position = space + 1;
        continue;
      }
      word = "\n";
    } else if (word.size() == 1 && word[0] == '\r') {
      position = space + 1;
      continue;
    }

    if (position >= text.size()) {
      break;
    }

    UIString* label = new UIString();
    label->SetText(100, 100, word);
    label->Show(0);
    label->enabled = attached;
    label->red = red;
    label->green = green;
    label->blue = blue;
    attached = 0;
    this->AddChild(label);
    this->extraLinks.push_back(label);
    if (space == std::string::npos) {
      break;
    }

    position = space + 1;
  }

  return attached;
}

// OFFSET: 0x0086CFE0
// FUNCTION: fn_11040
UIComboBox_BmpWnd::UIComboBox_BmpWnd() : UIComboBoxDefWnd() {
}
