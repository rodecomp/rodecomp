#include "Client/ReplayPacketRecorder.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Resource/SkinPalette.h"
#include "UI/Window/UICntWnd.h"
#include "UI/Window/UIChatRoomWnd.h"
#include "UI/Window/UIBank_NewWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Session.h"
#include "Utility/Text/GroupedInteger.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Window/UILoginWnd.h"
#include "UI/Window/UIMessageBox.h"
#include "UI/Window/UINoticeWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIReplayListBox.h"
#include "UI/Window/UISelectReplayDataWnd.h"
#include "UI/Window/UISelectServerWnd.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIMerInfoWnd.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/UIMessageBoxAutoreturn.h"
#include "UI/Window/UIMonsterInfoWnd.h"
#include "UI/Window/UITalkboxTrapInputWnd.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "UI/Window/UIPetInfoWnd.h"
#include "UI/Window/UIReplayRECControlWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Registry.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Window/UILoginOTPWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Buttons/UIButton.h"
#include "Render/Text/TextContext.h"
#include "Contents/MessageTable.h"
#include "UI/Text/ColoredText.h"
#include "Client/Mode/CMode.h"
#include "UI/Window/UIPayConfirmWnd.h"
#include "UI/Window/UIReplayControlWnd.h"
#include "UI/Window/UIMakeCharWnd.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Replay.h"
#include "Client/Application/Launcher.h"
#include "UI/Lists/UIChatHisBox.h"
#include "Client/Application/ApplicationWindow.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIBarGraph.h"
#include "Utility/Text/FormatString.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include <cstring>
#include <format>
#include "Client/Mode/CGameMode.h"
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>
#include <shellapi.h>
#include <imm.h>
#include <direct.h>
#include <io.h>
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIComboBox.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Item/UIItemWnd.h"
#include "Contents/Sound/SoundMaker.h"
#include "UI/Buttons/UICheckBox.h"
#include "Contents/ClientState.h"
#include "Contents/ForbiddenWords.h"
#include "UI/Window/UINormalMB.h"
#include "UI/Buttons/UINoneVisibleButton.h"
#include "Contents/PetEvolutionMgr.h"
#include "Contents/Items/ItemInfo.h"
#include <mbstring.h>
#include "Contents/Actors/Pc.h"
#include "Contents/SpriteRidingOffset.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/ImfResource.h"
#include "Resource/PaletteResource.h"

// GLOBAL: gl_25153
static const std::string s_messageBoxItemLinkOpening = "<ITEML>";

// GLOBAL: gl_25154
static const std::string s_messageBoxItemLinkClosing = "</ITEML>";

namespace {
  constexpr char prefix[] = "유저인터페이스\\";

  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };

  struct HangameJoinUrl {
    EncryptedName<80, 0xF3CB6F7711A71B6DULL>& operator()() const;
  };

  // INLINED
  std::uint32_t Palette(ResourceMgr& resources, int x, int y) {
    const auto* resource = static_cast<CBitmapRes*>(resources.Load((std::string(prefix) + "colorchip.bmp").c_str()));
    const auto& image = resource->image;
    const std::uint32_t pixel = x >= 0 && y >= 0 && x < image.width && y < image.height ? image.pixels[y * image.width + x] : 0x00FF0000u;

    return ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  }

  // INLINED
  void DrawTextRight(UIWindow& window, FontCache& fonts, const TextLocale& locale, int x, int y, const char* text, int bytes,
                     int family, int height, std::uint32_t color, unsigned char bold) {
    if (!bytes) {
      bytes = static_cast<int>(std::strlen(text));
    }
    TextContext context(*window.geometry.surface);
    context.SetFont(family, height, bold, 0);
    const int width = context.Measure(text, static_cast<int>(std::strlen(text))).cx;
    context.SetColor(color);
    context.Draw(x - width, y, text, bytes);
  }
}

// OFFSET: 0x008A39C0
// FUNCTION: fn_11661
UIMakeCharWnd::UIMakeCharWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_02C4(0), unknown_02C8(0), unknown_02CC(0), unknown_02D0(0), unknown_02D4(0), unknown_02D8(0) {
  this->geometry.unknown_0034 = 0;
  this->m_nameEdit = nullptr;
  this->m_info.sex = static_cast<unsigned char>(g_doramSex);

  const int* preset = reinterpret_cast<const int*>(static_cast<CMode*>(g_charViewMode)->SendMsg(10016, 0, 0, 0, 0));
  this->m_info.stats[0] = static_cast<unsigned char>(preset[0]);
  this->m_info.stats[1] = static_cast<unsigned char>(preset[1]);
  this->m_info.stats[2] = static_cast<unsigned char>(preset[2]);
  this->m_info.stats[3] = static_cast<unsigned char>(preset[3]);
  this->m_info.stats[4] = static_cast<unsigned char>(preset[4]);
  this->m_info.stats[5] = static_cast<unsigned char>(preset[5]);
  this->m_info.headPalette = static_cast<std::int16_t>(preset[6]);
  this->m_info.head = static_cast<std::int16_t>(preset[7]);

  this->m_info.name[0] = 0;
  this->m_info.job = 0;
  this->m_info.jobPoint = 0;
  this->m_info.hp = 100;
  this->m_info.maxHp = 100;
  this->m_info.sp = 100;
  this->m_info.maxSp = 100;
  this->m_info.exp = 0;
  this->m_info.level = 1;
  this->m_info.skillPoint = 1;
  this->m_info.slot = static_cast<unsigned char>(g_selectedSlotIndex);
  this->m_info.headPalette = 0;
  this->m_info.bodyStyle = 0;
  this->m_info.shield = 0;
  this->m_info.accessory2 = 0;
  this->m_info.accessory3 = 0;
  this->m_view.unknown_0148 = 0;
  this->m_view.action = 0;
  this->m_view.frame = 0;
  this->m_view.x = 70;
  this->m_view.y = 123;

  this->m_view.actionNames[0].assign(GetBodyActionResourceName(this->m_info.job, this->m_info.sex, 0, 0));
  this->m_view.spriteNames[0].assign(g_session.GetBodySprName(this->m_info.job, this->m_info.sex, 0, 0));

  if (this->m_info.sex == 1 && this->m_info.head == 13) {
    this->m_info.head = 14;
  }

  int head = this->m_info.head;
  this->m_view.actionNames[1].assign(g_session.GetHeadActName(this->m_info.job, this->m_info.sex, &head));
  this->m_view.spriteNames[1].assign(g_session.GetHeadSprName(this->m_info.job, this->m_info.sex, &head));

  const int bodyPalette = this->m_info.bodyStyle;
  this->m_info.head = static_cast<std::int16_t>(head);
  this->m_view.bodyPalette = bodyPalette;
  if (bodyPalette) {
    this->m_view.bodyPaletteName.assign(g_session.GetBodyPalName(this->m_info.job, this->m_info.sex, bodyPalette, this->m_info.unknown_0058, 0, 0));
  }

  const int headPalette = this->m_info.headPalette;
  this->m_view.headPalette = headPalette;
  if (headPalette) {
    this->m_view.headPaletteName.assign(g_session.GetHeadPalName(head, this->m_info.job, this->m_info.sex, headPalette));
  }

  this->m_view.imfName = GetBodyImfName(this->m_info.job, this->m_info.sex, this->m_info.head, this->m_info.unknown_0058);
  this->geometry.currentAlpha = 0x80;
  this->geometry.targetAlpha = 0xFF;
}

// OFFSET: 0x008A3F60
// FUNCTION: fn_11662
UIMerInfoWnd::UIMerInfoWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008A3FC0
// FUNCTION: fn_11663
UIMessageBox::UIMessageBox(UIWindowContext& windows, UIFrameContext& frames, UIMessageBoxContext& dialog)
    : UIFrameWnd(windows, frames, 0), counting(0), protectedMessage(0), m_dialog(&dialog) {
}

// OFFSET: 0x008A40B0
// FUNCTION: fn_11664
UIMessageBoxAutoreturn::UIMessageBoxAutoreturn(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->counting = 0;
}

// OFFSET: 0x008A4170
// FUNCTION: fn_11665
UIMonsterInfoWnd::UIMonsterInfoWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), info(), unknown_00D4(0) {
}

// INLINED
UIReplayRECControlWnd::~UIReplayRECControlWnd() {
  this->recordStart = 0;
  this->mode = 0;
}

// OFFSET: 0x008A5220
// OFFSET: 0x008A64E0
// FUNCTION: fn_11680
UIMessageBox::~UIMessageBox() = default;

// OFFSET: 0x008B8E60
// FUNCTION: fn_11768
void UIMessageBox::Paint() {
  auto& controls = this->m_dialog->text.buttons;
  const auto load = [&](const char* name) { return static_cast<CBitmapRes*>(controls.resources.Load((std::string(prefix) + name).c_str())); };
  int leftWidth = 0;
  int rightWidth = 0;
  int titleHeight = 0;
  const int width = this->geometry.width;
  const int height = this->geometry.height;

  if (const auto* left = load("basic_interface\\titlebar_left.bmp")) {
    this->DrawBitmap(0, 0, &left->image, false);
    leftWidth = left->image.width;
  }

  if (const auto* right = load("basic_interface\\titlebar_right.bmp")) {
    rightWidth = right->image.width;
    this->DrawBitmap(width - rightWidth, 0, &right->image, false);
  }

  if (const auto* middle = load("basic_interface\\titlebar_mid.bmp")) {
    titleHeight = middle->image.height;
    const int tileWidth = middle->image.width;
    const int extent = width - rightWidth - leftWidth;
    const int tiles = extent / tileWidth + (extent % tileWidth != 0);

    for (int index = 0; index < tiles; ++index) {
      this->DrawBitmap(leftWidth, 0, &middle->image, false);
      leftWidth += tileWidth;
    }
  }

  this->DrawTextAt(controls.fonts, controls.locale, 19, titleHeight - 13, this->caption.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 18, titleHeight - 14, this->caption.c_str(), 0, 0, 12, 0, 0, 0);
  const auto* base = load("basic_interface\\sys_base_off.bmp");
  this->DrawBitmap(3, 3, base ? &base->image : nullptr, true);

  const auto border = Palette(controls.resources, 14, 6);
  DrawRoundedBorder(*this, 0, 17, width, height - 17, border);
  this->FillRegion(0, 17, width, 5, border);
  const auto background = Palette(controls.resources, 2, 2);
  this->FillRegion(1, 17, width - 2, height - 19, background);
  this->FillRegion(2, height - 2, width - 4, 1, background);
}

// OFFSET: 0x008AC410
// FUNCTION: fn_11729
bool UIBank_NewWnd::Slot03() {
  return true;
}

// OFFSET: 0x008AC420
// FUNCTION: fn_11730
bool UIReplayControlWnd::Slot03() {
  return true;
}

// OFFSET: 0x008AC430
// FUNCTION: fn_11731
bool UIReplayRECControlWnd::Slot03() {
  return true;
}

// OFFSET: 0x008B7A90
// FUNCTION: fn_11756
void UIMessageBox::Slot18() {
}

// OFFSET: 0x008B7AA0
// FUNCTION: fn_11757
void UINoticeWnd::Slot18() {
}

// OFFSET: 0x008B7AB0
// FUNCTION: fn_11758
void UISelectReplayDataWnd::Slot18() {
}

// OFFSET: 0x008B7AC0
// FUNCTION: fn_11759
void UISelectServerWnd::Slot18() {
}

// OFFSET: 0x008B8610
// FUNCTION: fn_11765
void UILoginWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x008B8620
// FUNCTION: fn_11766
void UIMakeCharWnd::Paint() {
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\login_interface\\win_make2.bmp")));
  this->DrawBitmap(0, 17, &background->image, false);
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, LookupMessage(0x940), 0);
  this->DrawButtonBar(2, 0, 1);

  SpriteAtlasRecord record{};
  for (int i = 0; i < 2; ++i) {
    CImfRes* offsets = static_cast<CImfRes*>(ResourceMgr::Instance().Load(this->m_view.imfName.c_str()));
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_view.actionNames[i].c_str()));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->m_view.spriteNames[i].c_str()));
    const ActionFrame& frame = action->Frame(this->m_view.action, this->m_view.frame);
    POINT offset = offsets->GetLongPoint(i, this->m_view.action, this->m_view.frame);
    const std::uint32_t* palette = sprite->palette;

    if (i == 1) {
      CActRes* body = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_view.actionNames[0].c_str()));
      const ActionFrame& bodyFrame = body->Frame(this->m_view.action, this->m_view.frame);
      if (frame.anchorCount != 0 && bodyFrame.anchorCount != 0 && frame.anchors[0].unknown_0008 == bodyFrame.anchors[0].unknown_0008) {
        offset.x += bodyFrame.anchors[0].x - frame.anchors[0].x;
        offset.y += bodyFrame.anchors[0].y - frame.anchors[0].y;
      }
    }

    switch (i) {
    case 0:
      if (this->m_view.bodyPalette) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_view.bodyPaletteName.c_str()))->palette;
      }
      break;
    case 1:
      if (this->m_view.headPalette) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_view.headPaletteName.c_str()))->palette;
      }
      break;
    }

    const SpriteFrameCell& cell = frame.Cell(i);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    }

    this->geometry.surface->DrawSurface(
      this->m_view.x + cell.x + offset.x, this->m_view.y + cell.y + offset.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
    );
  }
}

// OFFSET: 0x008B88F0
// FUNCTION: fn_11767
void UIMerInfoWnd::Paint() {
  auto& controls = *this->m_controls;
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\homuninfo_bg.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
  char title[50] = {};
  const unsigned kind = g_session.m_mercenaryClass;
  const int titleId = kind - 6017 <= 9 ? 1181 : kind - 6037 <= 9 ? 1182 : kind - 6027 <= 9 ? 1183 : 1367;
  std::sprintf(title, controls.services.MessageText(titleId));
  this->DrawTextAt(controls.fonts, controls.locale, 5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 24, controls.services.MessageText(407), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 47, controls.services.MessageText(408), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 111, controls.services.MessageText(1184), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 135, controls.services.MessageText(1185), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 159, controls.services.MessageText(1186), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 230, 133, controls.services.MessageText(1187), 0, 0, 12, 0, 0, 0);
  char number[24] = {};
  for (int i = 0; i < 8; ++i) {
    const int value = i == 7 ? (2000 - g_session.m_mercenaryStats[i]) / 10 : g_session.m_mercenaryStats[i];
    const int bytes = std::sprintf(number, "%d", value);
    DrawTextRight(*this, controls.fonts, controls.locale, 79, 24 + 18 * i, number, bytes, 1, 14, 0, 0);
  }
  this->DrawTextAt(controls.fonts, controls.locale, 136, 24, g_session.m_mercenaryName, 0, 0, 12, 0x646464, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 135, 24, g_session.m_mercenaryName, 0, 0, 12, 0, 0, 0);
  char level[12] = {};
  _itoa(g_session.m_mercenaryLevel, level, 10);
  this->DrawTextAt(controls.fonts, controls.locale, 135, 47, level, 0, 0, 12, 0, 0, 0);
  if (g_session.m_mercenaryClass != -1) {
    char text[128] = {};
    int bytes = std::sprintf(text, "%d / %d", g_session.m_mercenaryHp, g_session.m_mercenaryMaxHp);
    this->DrawTextAt(controls.fonts, controls.locale, 218, 70, text, bytes, 1, 14, 0, 0, 0);
    bytes = std::sprintf(text, "%d / %d", g_session.m_mercenarySp, g_session.m_mercenaryMaxSp);
    this->DrawTextAt(controls.fonts, controls.locale, 218, 85, text, bytes, 1, 14, 0, 0, 0);
    std::strftime(text, 128, controls.services.MessageText(1260), _localtime32(&g_session.m_mercenaryExpiration));
    this->DrawTextAt(controls.fonts, controls.locale, 155, 111, text, static_cast<int>(std::strlen(text)), 0, 12, 255, 0, 0);
    bytes = std::sprintf(text, "%d", g_session.m_mercenaryLoyalty);
    this->DrawTextAt(controls.fonts, controls.locale, 170, 133, text, bytes, 1, 14, 0, 0, 0);
    bytes = std::sprintf(text, "%d", g_session.m_mercenarySummons);
    this->DrawTextAt(controls.fonts, controls.locale, 170, 157, text, bytes, 1, 14, 0, 0, 0);
    bytes = std::sprintf(text, "%d", g_session.m_mercenaryKills);
    this->DrawTextAt(controls.fonts, controls.locale, 245, 157, text, bytes, 1, 14, 0, 0, 0);
  }
}

// OFFSET: 0x008A6E20
// FUNCTION: fn_11712
long long UIBank_NewWnd::TransactionAmount(unsigned char deposit) {
  auto& services = this->m_controls->services;
  std::string text = this->m_amountEdit->Text();
  this->m_amountEdit->SetText("");
  const auto fail = [&](int notice, int status) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(services.MessageText(notice)), 255, 0, 0, 0);
    this->m_status = services.MessageText(status);
    return 0LL;
  };
  if (text == std::string(services.MessageText(2778))) {
    const long long cash = g_session.m_zeny;
    const long long balance = g_session.m_bankBalance;
    const long long excess = cash + balance > 0x7FFFFFFF ? cash + balance - 0x7FFFFFFF : 0;
    if (deposit) {
      if (balance >= 0x7FFFFFFF) {
        return fail(2787, 2783);
      }
      if (g_session.m_zeny == 0) {
        return fail(2770, 2780);
      }
      const long long amount = cash - excess;
      if (amount < 0) {
        return fail(2465, 2780);
      }
      return amount;
    }
    if (balance == 0) {
      return fail(2770, 2780);
    }
    const long long amount = balance - excess;
    return cash + amount > 0x7FFFFFFF ? static_cast<long long>(0x7FFFFFFF - g_session.m_zeny) : amount;
  }
  if (text.empty()) {
    return fail(2779, 2781);
  }
  if (!this->IsNumeric(text)) {
    return fail(2488, 2782);
  }
  const long long amount = _atoi64(text.c_str());
  if (amount > 0x7FFFFFFF) {
    return fail(2768, 2783);
  }
  if (amount < 1) {
    return fail(2769, 2784);
  }
  return amount;
}

// OFFSET: 0x008A6C20
// FUNCTION: fn_11710
long long UIBank_NewWnd::AdjustAmount(int amount) {
  std::string text = this->m_amountEdit->Text();
  this->m_amountEdit->SetText("");
  long long result = amount;
  if (!text.empty() && this->IsNumeric(text)) {
    result += _atoi64(text.c_str());
  }
  if (result > 0x7FFFFFFF) {
    result = 0x7FFFFFFF;
  } else if (result < 0) {
    result = 0;
  }
  return result;
}

// OFFSET: 0x008C4B00
// FUNCTION: fn_11888
bool UIBank_NewWnd::IsNumeric(std::string text) {
  if (text.size() == 0) {
    return false;
  }

  for (auto it = text.begin(); it != text.end(); ++it) {
    if (static_cast<unsigned char>(*it - '0') > 9) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x008B7AD0
// FUNCTION: fn_11760
void UIBank_NewWnd::Paint() {
  auto& controls = *this->m_controls;
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\bank\\bg_bank.bmp")));
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(2771), 0);
  this->DrawBitmap(0, titleHeight, &background->image, false);
  char number[260] = {};
  char text[260] = {};
  this->DrawTextAt(controls.fonts, controls.locale, 30, 29, controls.services.MessageText(2772), 0, 0, 12, 0, 0, 0);
  const int x = this->TextWidth(controls.fonts, controls.locale, controls.services.MessageText(2772), 0, 0, 12, 0, 0) + 43;
  FormatGroupedInteger(g_session.m_bankBalance, number, 260);
  std::sprintf(text, "%s z", number);
  this->DrawTextAt(controls.fonts, controls.locale, x, 29, text, 0, 0, 11, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 30, 46, controls.services.MessageText(2773), 0, 0, 12, 0, 0, 0);
  FormatGroupedInteger(g_session.m_zeny, number, 260);
  std::sprintf(text, "%s z", number);
  this->DrawTextAt(controls.fonts, controls.locale, x, 46, text, 0, 0, 11, 0, 0, 0);
  if (!this->m_status.empty()) {
    const int textWidth = this->TextWidth(controls.fonts, controls.locale, this->m_status.c_str(), 0, 0, 12, 0, 0);
    const int statusX = static_cast<int>(this->geometry.width * 0.5f - textWidth * 0.5f);
    this->DrawTextAt(controls.fonts, controls.locale, statusX, 112, this->m_status.c_str(), 0, 0, 11, 255, 0, 0);
  }
}

// OFFSET: 0x008B7CF0
// FUNCTION: fn_11761
void UIChatRoomWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 20, 6, red, green, blue);
  const int width = this->geometry.width;
  this->FillRegion(width - width * 56 / 280 - 2, 17, 1, this->geometry.height - 38, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);
}

// OFFSET: 0x008B7DA0
// FUNCTION: fn_11762
void UICntWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008A91E0
// FUNCTION: fn_11721
void UIFrameWnd::DrawBorder(int x, int y, int width, int height) {
  if (width == 0) {
    width = this->geometry.width;
  }
  if (height == 0) {
    height = this->geometry.height;
  }
  const auto load = [&](const char* name) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  };
  const int innerWidth = width - 28;
  const int columns = innerWidth / 14 + (innerWidth % 14 != 0);
  for (int i = 0; i < columns; ++i) {
    const auto* bitmap = load("유저인터페이스\\sysbox_mu.bmp");
    this->DrawBitmap(x + 14 + i * 14, y, &bitmap->image, false);
  }
  const auto* bitmap = load("유저인터페이스\\sysbox_lu.bmp");
  this->DrawBitmap(x, y, &bitmap->image, false);
  bitmap = load("유저인터페이스\\sysbox_ru.bmp");
  this->DrawBitmap(x + width - bitmap->image.width, y, &bitmap->image, false);
  const int innerHeight = height - 28;
  const int rows = innerHeight / 14 + (innerHeight % 14 != 0);
  for (int i = 0; i < rows; ++i) {
    bitmap = load("유저인터페이스\\sysbox_lm.bmp");
    this->DrawBitmap(x, y + 14 + i * 14, &bitmap->image, false);
  }
  for (int i = 0; i < rows; ++i) {
    bitmap = load("유저인터페이스\\sysbox_rm.bmp");
    this->DrawBitmap(x + width - bitmap->image.width, y + 14 + i * 14, &bitmap->image, false);
  }
  for (int i = 0; i < columns; ++i) {
    bitmap = load("유저인터페이스\\sysbox_md.bmp");
    this->DrawBitmap(x + 14 + i * 14, y + height - bitmap->image.height, &bitmap->image, false);
  }
  bitmap = load("유저인터페이스\\sysbox_ld.bmp");
  this->DrawBitmap(x, y + height - bitmap->image.height, &bitmap->image, false);
  bitmap = load("유저인터페이스\\sysbox_rd.bmp");
  this->DrawBitmap(x + width - bitmap->image.width, y + height - bitmap->image.height, &bitmap->image, false);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(x + 14, y + 14, innerWidth, innerHeight, RGB(red, green, blue));
}

// INLINED
void UIMessageBox::FormatCountdown() {
  char text[512];
  std::sprintf(text, this->format.c_str(), this->remaining);
  this->messageText = text;
}

// INLINED
void UIMessageBox::AddButton(const char* name, int id, int x, int y) {
  auto& controls = this->m_dialog->text.buttons;
  auto* button = new UIBitmapButton(*this->m_context, controls);
  const char* suffixes[] = {"", "_a", "_b"};

  for (int state = 0; state < 3; ++state) {
    const std::string path = std::string(prefix) + name + suffixes[state] + ".bmp";
    button->SetBitmap(path.c_str(), state, 0);
  }

  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(id);
  this->AddChild(button);
}

// OFFSET: 0x008A7350
// FUNCTION: fn_11713
void UIMessageBox::Configure(
  const char* text, int type, bool defaultAccept, int timeout, const char* title, int width, int height, int modalValue, const char* icon
) {
  this->caption = title;
  this->messageText = text;
  this->format = text;
  this->started = timeGetTime();
  this->deadline = timeGetTime() + static_cast<unsigned>(timeout);
  const unsigned interval = this->deadline - this->started;
  this->remaining = interval / 1000 + (interval % 1000 != 0);
  this->counting = timeout != 0;

  if (this->counting != 0) {
    this->FormatCountdown();
  }

  this->modal = modalValue;
  this->SetSize(width, height);
  this->Slot15(width, height);
  this->Slot22(width, height);
  this->Move((this->m_dialog->screenWidth - 640) / 2 + 185, (this->m_dialog->screenHeight - 480) / 2 + 180);
  int inset = 0;

  if (icon) {
    auto* image = new UIStaticBitmap(*this->m_context, this->m_dialog->text.buttons);
    image->SetBitmap(icon);
    image->SetSize(image->geometry.width, image->geometry.height);
    image->Move(10, 22);
    this->AddChild(image);
    inset = image->geometry.width + image->geometry.x;
  }

  this->viewer = new UITextViewer(*this->m_context, this->m_dialog->text);
  this->viewer->Configure(inset + 10, 22, width - inset - 20, height - 55, 0);
  this->viewer->backgroundColor = Palette(this->m_dialog->text.buttons.resources, 2, 2) | 0xFF000000u;
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->messageText.data()), static_cast<int>(static_cast<double>(width) / 7.5), 0, 0);
  this->m_message0 = type == 2 ? 188 : 185;

  switch (type) {
  case 0:
    this->AddButton("btn_ok", 184, width - 46, height - 24);
    this->m_message0 = 184;
    break;
  case 2:
    this->AddButton("btn_ok", 187, width - 91, height - 24);
    this->AddButton("btn_cancel", 188, width - 46, height - 24);
    this->m_message0 = defaultAccept ? 187 : 188;
    this->m_message1 = 188;
    break;
  case 4:
    this->AddButton("btn_cancel", 185, width - 46, height - 24);
    this->m_message0 = 185;
    break;
  case 5: {
    this->AddButton("btn_ok", 184, width - 46, height - 24);
    this->m_message0 = 184;
    auto& controls = this->m_dialog->text.buttons;

    auto* button = new UITextButton(*this->m_context, controls);
    button->Configure(controls.services.MessageText(3445), 0x00F4934A, 0, 12, 0, 0);
    button->m_hoverColor = 0x00FF7864;
    button->Move(14, 70);
    button->SetId(426);
    button->SetDisabled(0);
    button->Invalidate();
    this->AddChild(button);
    break;
  }
  default:
    break;
  }
}

// OFFSET: 0x008BADC0
// FUNCTION: fn_11791
void UIMessageBox::Slot19() {
  if (this->counting == 0) {
    return;
  }

  if (timeGetTime() > this->deadline) {
    this->m_dialog->response = 187;
    this->counting = 0;
    return;
  }

  const unsigned previous = this->remaining;
  const bool fraction = (this->deadline - timeGetTime()) % 1000 != 0;
  this->remaining = (this->deadline - timeGetTime()) / 1000 + fraction;

  if (previous != this->remaining) {
    this->FormatCountdown();
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->messageText.data()), 38, 0, 0);
  }
}

// OFFSET: 0x008BF950
// FUNCTION: fn_11808
int UIMessageBox::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    this->m_dialog->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);

    if (this->modal != 0) {
      this->m_dialog->response = static_cast<int>(a);
    } else if (a == 184 || a == 187 || a == 188) {
      this->m_dialog->retirement.Schedule(this);
    }

    return 0;
  case 57:
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, a, 38, 0, 0);
    return 0;
  case 67:
    this->format = reinterpret_cast<const char*>(a);
    return 0;
  case 98:
    if (a == 426) {
      auto& controls = this->m_dialog->text.buttons;
      ShellExecuteA(this->m_dialog->text.applicationWindow, "open", controls.services.MessageText(3445), nullptr, nullptr, SW_SHOWNORMAL);
    }

    return 0;
  case 156:
    this->modal = static_cast<int>(a);
    return 0;
  case 157:
    this->protectedMessage = a > 0;
    return 0;
  case 158:
    return this->protectedMessage == 1;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008BFB70
// FUNCTION: fn_11809
int UIMessageBoxAutoreturn::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    g_windowMgr.unknown_0474 = static_cast<int>(a);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008BFBA0
// FUNCTION: fn_11810
int UIMonsterInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    if (a == 184) {
      g_windowMgr.Schedule(this);
    }

    return 0;
  case 79:
    if (a) {
      this->info = *reinterpret_cast<const MonsterInfo*>(a);
    }

    this->Invalidate();
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008C1DB0
// FUNCTION: fn_11817
int UITalkboxTrapInputWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      TalkboxTrapRequest request;
      request.target = this->target;
      request.text = this->edit->Text();
      g_modeMgr.GetGameMode()->SendMsg(131, reinterpret_cast<int>(&request), 0, 0, 0);
    } else if (a != 185) {
      return 0;
    }

    g_windowMgr.Schedule(this);
    return 0;
  }

  if (message == 68) {
    this->target = *reinterpret_cast<const TalkboxTrapTarget*>(a);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008C1ED0
// FUNCTION: fn_11818
void UIWindowMgr::SetUnknown04DC(const char* text) {
  this->unknown_04DC = text;
}

// OFFSET: 0x008C35A0
// FUNCTION: fn_11824
void UIChatRoomWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x008C35E0
// FUNCTION: fn_11825
void UIHomunInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008B0CA0
// FUNCTION: fn_11744
void UIMerInfoWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);
  {
    const std::string name("유저인터페이스\\btn_fired");
    this->button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->button->SetBitmap((name + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((name + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(187, 44);
    this->button->SetId(350);
    this->AddChild(this->button);
  }
  {
    const std::string name("유저인터페이스\\btn_skill");
    this->button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->button->SetBitmap((name + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((name + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(232, 44);
    this->button->SetId(349);
    this->AddChild(this->button);
  }

  this->hpBar = new UIBarGraphPlayer(*this->m_context);
  this->hpBar->SetValues([]() -> std::int64_t { return g_session.m_mercenaryHp; }, []() -> std::int64_t { return g_session.m_mercenaryMaxHp; });
  this->hpBar->SetSize(85, 9);
  this->hpBar->Move(117, 73);
  this->AddChild(this->hpBar);

  this->spBar = new UIBarGraphPlayer(*this->m_context);
  this->spBar->SetValues([]() -> std::int64_t { return g_session.m_mercenarySp; }, []() -> std::int64_t { return g_session.m_mercenaryMaxSp; });
  this->spBar->SetSize(85, 9);
  this->spBar->Move(117, 88);
  this->AddChild(this->spBar);

  this->m_message0 = 201;
}

// OFFSET: 0x008C3630
// FUNCTION: fn_11827
void UIMerInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008BF720
// FUNCTION: fn_11807
int UIMerInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 0:
    return 0;
  case 6:
    this->m_recorder->Record(this->geometry.id, static_cast<int>(a), b, c, d);

    switch (a) {
    case 184:
      this->Invalidate();
      return 0;
    case 201:
      g_windowMgr.CloseWindow(125);
      return 0;
    case 349:
      if (!g_windowMgr.CloseWindow(126)) {
        g_windowMgr.EnsureWindow(126);
        return 0;
      }

      return 0;
    case 350: {
      CMode* mode = g_modeMgr.GetGameMode();

      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1370), 2, false, false, 0, "Message", 280, 120, 0) == 187) {
        mode->SendMsg(186, 2, 0, 0, 0);
        g_windowMgr.CloseWindow(125);
        g_windowMgr.CloseWindow(126);
        g_session.ResetMercenaryStats();
        g_groundMarkAltEnabled = 0;
        return 0;
      }

      return 0;
    }
    default:
      return 0;
    }
  case 34:
    this->saved = reinterpret_cast<MerInfoWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008C3670
// FUNCTION: fn_11828
void UIPetInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008C36B0
// FUNCTION: fn_11829
void UIReplayRECControlWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x008A6DC0
// FUNCTION: fn_11711
void UIBank_NewWnd::AnimateLoading() {
  for (int index = 0; index < 4; ++index) {
    this->m_loading[index]->Move(-100, -100);
  }

  this->m_loading[static_cast<int>(timeGetTime() % 1000) / 250]->Move(6, 5);
}

// OFFSET: 0x008AE8A0
// FUNCTION: fn_11741
void UILoginOTPWnd::Slot15(int, int) {
  auto& controls = *this->m_controls;
  const std::uint8_t prefixSource[] = {0x8F, 0xC8, 0x2D, 0x7F, 0xC9, 0xF9, 0xFA, 0xB4, 0x89, 0x83, 0x2D, 0x49, 0xB4, 0x8D, 0x63, 0x79};
  thread_local EncryptedName<sizeof(prefixSource), 0x793F370985ED674FULL> prefixName(prefixSource);
  const std::string prefix(prefixName.Text());

  this->unknown_00B4 = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->unknown_00B4->SetSize(80, 16);
  this->unknown_00B4->maximum = 8;
  this->unknown_00B4->Move(99, 95);
  this->unknown_00B4->mask = '*';
  this->unknown_00B4->Background(243, 243, 243);
  this->AddChild(this->unknown_00B4);
  this->unknown_00B4->SetText("");
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->unknown_00B4);

  UIStaticText* label = new UIStaticText(*this->m_context, controls);
  label->SetSize(80, 20);
  label->Move(141 - this->TextWidth(controls.fonts, controls.locale, LookupMessage(3410), 0, 0, 12, 0, 0), 67);
  label->SetText(LookupMessage(3410), 0);
  this->AddChild(label);

  this->timerText = new UIStaticText(*this->m_context, controls);
  this->timerText->SetSize(80, 20);
  this->timerText->Move(150, 66);
  this->timerText->SetText(LookupMessage(3410), 0);
  this->timerText->SetFont(0, 14, 1, 0);
  this->timerText->SetColor(205, 19, 41);
  this->AddChild(this->timerText);

  label = new UIStaticText(*this->m_context, controls);
  label->SetSize(200, 40);
  label->SetText(LookupMessage(3385), 0);
  label->Move(static_cast<int>(141.0f - this->TextWidth(controls.fonts, controls.locale, LookupMessage(3385), 0, 0, 12, 0, 0) * 0.5f), 28);
  this->AddChild(label);

  label = new UIStaticText(*this->m_context, controls);
  label->SetSize(200, 40);
  label->SetText(LookupMessage(3386), 0);
  label->Move(static_cast<int>(141.0f - this->TextWidth(controls.fonts, controls.locale, LookupMessage(3386), 0, 0, 12, 0, 0) * 0.5f), 42);
  this->AddChild(label);

  label = new UIStaticText(*this->m_context, controls);
  label->SetSize(80, 20);
  label->Move(74 - this->TextWidth(controls.fonts, controls.locale, LookupMessage(3255), 0, 0, 12, 0, 0), 98);
  label->SetText(LookupMessage(3255), 0);
  label->SetFont(0, 12, 1, 0);
  label->SetColor(64, 64, 64);
  this->AddChild(label);

  UIBmpTextButton* button = new UIBmpTextButton(*this->m_context, controls);
  const std::uint8_t normalSource[] = {0xDD, 0xAA, 0x53, 0xD3, 0xB1, 0xED, 0x92, 0x28, 0xDB, 0xE1, 0x53, 0xE5, 0xCC, 0x99, 0x0B, 0x89, 0x72,
                                       0x62, 0xFA, 0x47, 0x2E, 0x4A, 0x39, 0x91, 0x78, 0x77, 0xF5, 0x48, 0x12, 0x46, 0x0B, 0x87, 0x69, 0x5A,
                                       0xFC, 0x5D, 0x01, 0x7C, 0x39, 0x8A, 0x6F, 0x68, 0xF2, 0x45, 0x5F, 0x41, 0x3A, 0x95, 0x1D};
  thread_local EncryptedName<sizeof(normalSource), 0xE55723712993051DULL> normalName(normalSource);
  button->SetBitmap(normalName.Text(), 0, 0);
  const std::uint8_t hoverSource[] = {0xE3, 0x5E, 0x17, 0xED, 0x17, 0x87, 0x26, 0xB0, 0xE5, 0x15, 0x17, 0xDB, 0x6A, 0xF3, 0xBF, 0x11,
                                      0x4C, 0x96, 0xBE, 0x79, 0x88, 0x20, 0x8D, 0x09, 0x46, 0x83, 0xB1, 0x76, 0xB4, 0x2C, 0xBF, 0x1F,
                                      0x57, 0xAE, 0xB8, 0x63, 0xA7, 0x16, 0x8C, 0x0B, 0x46, 0x83, 0xF9, 0x75, 0xBA, 0x39, 0xE3};
  thread_local EncryptedName<sizeof(hoverSource), 0x7DE349D717D7F123ULL> hoverName(hoverSource);
  button->SetBitmap(hoverName.Text(), 1, 0);
  const std::uint8_t pressedSource[] = {0xBF, 0x7A, 0x6F, 0xA1, 0xAB, 0x7F, 0xD0, 0xC2, 0xB9, 0x31, 0x6F, 0x97, 0xD6, 0x0B, 0x49, 0x63,
                                        0x10, 0xB2, 0xC6, 0x35, 0x34, 0xD8, 0x7B, 0x7B, 0x1A, 0xA7, 0xC9, 0x3A, 0x08, 0xD4, 0x49, 0x6D,
                                        0x0B, 0x8A, 0xC0, 0x2F, 0x1B, 0xEE, 0x65, 0x7D, 0x1A, 0xA6, 0xDC, 0x75, 0x09, 0xDC, 0x65, 0x0F};
  thread_local EncryptedName<sizeof(pressedSource), 0x0F15B16B5BAFD57FULL> pressedName(pressedSource);
  button->SetBitmap(pressedName.Text(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(186, 92);
  button->SetId(382);
  {
    const std::string text(LookupMessage(3384));
    button->text = text;
    button->Invalidate();
  }
  button->AddTextStyle(0, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(1, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(2, 12, 0x313131, 1, 1, -1000, -1000);
  this->AddChild(button);
  this->m_message0 = 382;
}

// OFFSET: 0x008B84B0
// FUNCTION: fn_11764
void UILoginOTPWnd::Paint() {
  auto& controls = *this->m_controls;
  auto& resources = ResourceMgr::Instance();
  const std::uint8_t source[] = {0x11, 0xF4, 0x57, 0x1F, 0xF9, 0x49, 0x2E, 0xE2, 0x17, 0xBF, 0x57, 0x29, 0x84, 0x3D, 0xB7, 0x43,
                                 0xBE, 0x3C, 0xFE, 0x8B, 0x66, 0xEE, 0x85, 0x5B, 0xB4, 0x29, 0xF1, 0x84, 0x5A, 0xE2, 0xB7, 0x4D,
                                 0xB6, 0x04, 0xF9, 0x80, 0x4E, 0xE8, 0x9F, 0x5F, 0xE0, 0x75, 0xF5, 0x88, 0x49, 0x87};
  thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
  const auto* background = static_cast<CBitmapRes*>(resources.Load(Resolve(name.Text())));
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(3254), 280);
}

// OFFSET: 0x008C3620
// FUNCTION: fn_11826
void UILoginWnd::Slot11() {
  this->SaveLogin();
}

// OFFSET: 0x008C39E0
// FUNCTION: fn_11831
void UILoginWnd::SaveLogin() {
  HKEY key;
  DWORD disposition = 0;

  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) != ERROR_SUCCESS) {
    return;
  }

  char id[52];
  int saveId;

  if (this->saveId != 0) {
    std::strcpy(id, this->idEdit->Text());
    saveId = this->saveId != 0;
    id[50] = 0;
  } else {
    std::memset(id, 0, sizeof(id));
    saveId = 0;
  }

  const std::uint8_t idSource[] = {0x24, 0xFF, 0x95};
  thread_local EncryptedName<sizeof(idSource), 0xC517FB932D95BB6DULL> idName(idSource);
  RegSetValueExA(key, idName.Text(), 0, REG_SZ, reinterpret_cast<const BYTE*>(id), 51);

  const std::uint8_t checkSource[] = {0xE6, 0xD5, 0x28, 0xBA, 0x28, 0x74, 0x36, 0x55, 0xE0, 0x9D};
  thread_local EncryptedName<sizeof(checkSource), 0x03772763F96D9DA5ULL> checkName(checkSource);
  RegSetValueExA(key, checkName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&saveId), 4);

  int bitwise = 1;
  const std::uint8_t bitwiseSource[] = {0xC5, 0x5A, 0x3F, 0x28, 0x70, 0x5A, 0x34, 0x0B};
  thread_local EncryptedName<sizeof(bitwiseSource), 0x0B7109397F6B1387ULL> bitwiseName(bitwiseSource);
  RegSetValueExA(key, bitwiseName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&bitwise), 4);
  RegCloseKey(key);
}

// OFFSET: 0x008C1F00
// FUNCTION: fn_11819
void UIReplayRECControlWnd::ApplyMode() {
  this->Resize(194, 34);

  if (this->mode == 0) {
    this->window_00C0->Move(-100, -100);

    for (int index = 0; index < 5; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->window_00C4->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->window_00EC->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->window_0128->Move(-100, -100);
    this->window_012C->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->window_00BC->Move(110, 5);
    this->button_00C8->Move(136, 5);
    this->button_00C8->SetDisabled(0);
    this->button_00C8->Invalidate();
    this->button_00D0->Move(162, 5);
    this->button_00D0->SetDisabled(0);
    this->button_00D0->Invalidate();
    this->window_00E8->Move(32, 12);
    this->tabs[0]->Move(6, 5);
  } else if (this->mode == 1) {
    this->recordStart = timeGetTime();
    this->window_00BC->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->tabs[4]->Move(-100, -100);
    this->window_00CC->Move(-100, -100);
    this->options[7]->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->window_0128->Move(-100, -100);
    this->window_012C->Move(-100, -100);
    this->window_00C4->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->window_00C0->Move(110, 5);
    this->button_00C8->Move(136, 5);
    this->button_00C8->SetDisabled(1);
    this->button_00C8->Invalidate();
    this->button_00D0->Move(162, 5);
    this->button_00D0->SetDisabled(1);
    this->button_00D0->Invalidate();
    this->window_00E8->Move(32, 12);
    this->tabs[0]->Move(6, 5);
  } else if (this->mode == 2) {
    this->Resize(194, 200);
    this->window_00BC->Move(-100, -100);
    this->window_00C0->Move(-100, -100);
    this->button_00C8->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->window_00E8->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->window_00C4->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->tabs[4]->Move(6, 5);
    this->window_00CC->Move(162, 5);
    this->window_00EC->Move(42, 12);
    this->options[0]->Move(17, 63);
    this->options[1]->Move(17, 81);
    this->options[2]->Move(17, 99);
    this->options[3]->Move(17, 117);
    this->options[4]->Move(-100, -100);
    this->options[5]->Move(104, 81);
    this->options[6]->Move(104, 99);
    this->options[7]->Move(104, 117);
    this->options[9]->Move(104, 63);
    this->window_0128->Move(17, 160);
    this->window_012C->Move(17, 178);
  } else if (this->mode == 3) {
    this->window_00BC->Move(-100, -100);
    this->window_00C0->Move(-100, -100);
    this->button_00C8->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->window_00E8->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->tabs[4]->Move(-100, -100);
    this->window_00CC->Move(-100, -100);
    this->options[7]->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->window_0128->Move(-100, -100);
    this->window_012C->Move(-100, -100);
    this->window_00C4->Move(162, 5);
    this->window_0134->Move(7, 8);
    this->m_focus->Set(this->window_0134);
  }

  this->SendMsg(nullptr, 60, 0, 0, 0, 0);
}

// OFFSET: 0x008B9790
// FUNCTION: fn_11771
void UINoticeWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  const int leftTiles = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int index = 0, y = 17; index < leftTiles; ++index, y += 8) {
    const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  const int rightTiles = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int index = 0, y = 17; index < rightTiles; ++index, y += 8) {
    const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(20, 17, this->geometry.width - 40, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(
    20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue)
  );
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008BFC20
// FUNCTION: fn_11811
int UINoticeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 0:
    this->SendMsg(nullptr, 6, 184, 0, 0, 0);
    return 0;
  case 6:
    if (a == 184) {
      g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      FILE* file = std::fopen("notice_read.txt", "wb");
      const int index = this->m_index;
      unsigned char position;

      if (index <= 0 && this->m_scroll <= 255 && static_cast<unsigned char>(this->m_scroll) != this->m_bottomPositions[index] - 19) {
        position = static_cast<unsigned char>(this->m_scroll);

        if (position != 255) {
          position = static_cast<unsigned char>(this->m_scroll + 19);
          const int limit = this->m_bottomPositions[index] - 19;

          if (this->m_scroll + 19 > limit) {
            position = limit < 0 ? 0 : static_cast<unsigned char>(this->m_bottomPositions[index] - 19);
          }
        }
      } else {
        position = 255;
      }

      std::fwrite(&position, 1, 1, file);
      std::fclose(file);
      g_windowMgr.CloseWindow(93);
    } else if (a == 218) {
      ++this->m_index;

      if (this->m_index > this->m_noticeCount) {
        this->m_index = this->m_noticeCount;
        return 0;
      }

      this->m_scroll = this->m_scrollPositions[this->m_index];
      this->LoadNotice();
    } else if (a == 246) {
      if (--this->m_index < 0) {
        this->m_index = 0;
        return 0;
      }

      this->m_scroll = this->m_scrollPositions[this->m_index];
      this->LoadNotice();
    }

    return 0;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scroll;
    std::intptr_t delta = a;

    if (message == 9) {
      delta = -1;
    } else if (message == 10) {
      delta = 1;
    }

    this->m_scroll = previous + delta;

    if (this->m_scroll > this->m_bottomPositions[this->m_index] - 19) {
      this->m_scroll = this->m_bottomPositions[this->m_index] - 19;
    }

    if (this->m_scroll < this->m_scrollPositions[this->m_index]) {
      this->m_scroll = this->m_scrollPositions[this->m_index];
    }

    this->LoadNotice();
    return this->m_scroll - previous;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008BB070
// FUNCTION: fn_11793
void UINoticeWnd::Slot35(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, 7, 6, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, 7, -6, 0, 0, 0);
  }
}

// OFFSET: 0x008A8AD0
// FUNCTION: fn_11715
void UIPayConfirmWnd::Configure(const std::string& text, int itemId) {
  reinterpret_cast<UIMessageBox*>(this)->Configure(text.c_str(), 2, true, 0, LookupMessage(0xFDF), 0xDC, 0x85, 1, nullptr);

  this->viewer->Resize(0xA5, 0x50);
  this->viewer->Move(7, 0x17);
  this->viewer->backgroundColor = 0xFFFF00FF;
  this->viewer->Invalidate();

  for (UIWindow* child : this->m_children) {
    if (child->geometry.id == 187) {
      child->Move(0x59, 0x6D);
      break;
    }
  }

  for (UIWindow* child : this->m_children) {
    if (child->geometry.id == 188) {
      auto* button = static_cast<UIBitmapButton*>(child);
      button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
      button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
      button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
      button->Resize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(this->geometry.width - 0xE, 3);
      break;
    }
  }

  std::string resource(FindItemDescription(itemId).identifiedResource);
  if (!resource.empty()) {
    this->m_image = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(std::format("유저인터페이스\\collection\\{}.bmp", resource)));
  }
}

// OFFSET: 0x008B9940
// FUNCTION: fn_11772
void UIPayConfirmWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, this->m_title.c_str(), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);

  if (this->m_image) {
    this->DrawBitmapScaled(158, 27, 56, 75, this->m_image, 0);
  }
}

// OFFSET: 0x008B9E90
// FUNCTION: fn_11774
void UIReplayControlWnd::Paint() {
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\replay_interface\\replay_base.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}

// OFFSET: 0x008B9ED0
// FUNCTION: fn_11775
void UIReplayRECControlWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, 34, 0xF5B899);
  DrawRoundedBorder(*this, 0, 0, this->geometry.width, 34, 0x945A63);
  DrawRoundedBorder(*this, 1, 1, this->geometry.width - 2, 32, 0xF7C6AD);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, 30, 0xFFFFFF);
  DrawRoundedBorder(*this, 3, 3, this->geometry.width - 6, 28, 0xAD7B84);

  if (this->mode == 2) {
    DrawFilledRoundedBorder(*this, 0, 34, this->geometry.width, this->geometry.height - 34, 0xDEDEDE);
    DrawFilledRoundedBorder(*this, 4, 38, this->geometry.width - 8, 95, 0xFFFFFF);
    DrawFilledRoundedBorder(*this, 4, 138, this->geometry.width - 8, 57, 0xFFFFFF);
    DrawOutlinedText(*this, 10, 42, LookupMessage(1973), 0, 0x7F3C00, 0xFFFFFF, 0, 12, 1, 0);
    DrawOutlinedText(*this, 10, 142, LookupMessage(1974), 0, 0x7F3C00, 0xFFFFFF, 0, 12, 1, 0);
  }
}

// OFFSET: 0x008BA010
// FUNCTION: fn_11776
void UISelectReplayDataWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2084), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->FillRegion(6, 32, 268, 77, 0xFFE7E7E7);
  this->DrawTextAt(controls.fonts, controls.locale, 6, 19, LookupMessage(2085), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 6, 112, LookupMessage(2086), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008BA0B0
// FUNCTION: fn_11777
void UISelectServerWnd::Paint() {
  auto& controls = *this->m_controls;
  const char* title = LookupMessage(2082);

  if (g_modeMgr.GetLoginMode()->cursorType == 30) {
    title = LookupMessage(2083);
  }

  this->DrawTitleBar(controls.fonts, controls.locale, true, title, 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008BA110
// FUNCTION: fn_11778
void UITalkboxTrapInputWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(controls.fonts, controls.locale, 17, 3, LookupMessage(435), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 2, LookupMessage(435), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 17, 23, LookupMessage(436), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 22, LookupMessage(436), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008BA660
// FUNCTION: fn_11787
void UILoginWnd::Slot27(int, int) {
}

// OFFSET: 0x008BADB0
// FUNCTION: fn_11790
void UIMakeCharWnd::Slot19() {
}

// OFFSET: 0x008AD130
// FUNCTION: fn_11734
void UIChatRoomWnd::Move(int x, int y) {
  this->geometry.Move(x, y);
  this->Slot36();
}

// OFFSET: 0x008B1760
// FUNCTION: fn_11746
void UINoticeWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\");
  const char* names[3][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_back", "btn_back_a", "btn_back_b"}, {"btn_next", "btn_next_a", "btn_next_b"}};
  int ids[3] = {184, 246, 218};
  int positions[3][2] = {{534, 376}, {244, 376}, {294, 376}};

  for (int index = 0; index < 3; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 184;

  if (!std::fopen(this->m_patchFile, "r")) {
    FILE* file = std::fopen(this->m_patchFile, "w");
    std::fwrite("공지사항파일없음", 16, 1, file);
    std::fclose(file);
    g_windowMgr.Schedule(this);
  }

  if (!std::fopen("notice_check.txt", "r")) {
    FILE* file = std::fopen("notice_check.txt", "w");
    std::fwrite("없으니만들자", 12, 1, file);
    std::fclose(file);
  }

  unsigned char flag;
  if (!std::fopen("notice_read.txt", "r")) {
    FILE* file = std::fopen("notice_read.txt", "wb");
    flag = 0;
    std::fwrite(&flag, 1, 1, file);
    std::fclose(file);
  }

  FILE* patch = std::fopen(this->m_patchFile, "r");
  FILE* check = std::fopen("notice_check.txt", "r");
  std::fseek(patch, 0, SEEK_END);
  const int patchSize = std::ftell(patch);
  std::fseek(check, 0, SEEK_END);
  const int checkSize = std::ftell(check);
  int changed = checkSize == 0 ? 1000 : 0;
  char* patchText = new char[patchSize + 1];
  char* checkText = new char[checkSize + 1];
  std::fseek(patch, 0, SEEK_SET);
  std::fread(patchText, patchSize, 1, patch);
  patchText[patchSize] = 0;
  std::fclose(patch);
  std::fseek(check, 0, SEEK_SET);
  std::fread(checkText, checkSize, 1, check);
  checkText[checkSize] = 0;
  std::fclose(check);

  int lines[20];
  lines[0] = 0;
  int lineCount = 1;
  this->unknown_0108 = 0;
  for (int i = 0; i < patchSize; ++i) {
    const char c = patchText[i];
    if (c == '\n') {
      patchText[i] = 0;
      lines[lineCount] = i + 1;
      ++lineCount;
      if (lineCount > 20) {
        lineCount = 20;
      }
      ++this->unknown_0108;
    } else if (c == '\r') {
      patchText[i] = 0;
    } else if (c == '^') {
      const int previous = lineCount - 1;
      this->m_bottomPositions[this->m_noticeCount] = this->unknown_0108 - 1;
      ++this->m_noticeCount;
      this->m_scrollPositions[this->m_noticeCount] = this->unknown_0108 + 1;
      lineCount = previous < 0 ? 0 : previous;
    } else if (i < checkSize && static_cast<unsigned int>(i) < 1000 && c != checkText[i]) {
      ++changed;
    }
  }

  if (lineCount > 19) {
    lineCount = 19;
  }

  this->m_bottomPositions[this->m_noticeCount] = this->unknown_0108;

  if (changed == 0) {
    FILE* file = std::fopen("notice_read.txt", "rb");
    std::fread(&flag, 1, 1, file);
    std::fclose(file);
    if (flag == 0xFF) {
      g_windowMgr.Schedule(this);
    }
  } else {
    FILE* file = std::fopen("notice_read.txt", "wb");
    flag = 0;
    std::fwrite(&flag, 1, 1, file);
    std::fclose(file);
  }

  for (int i = 0, y = 22; y < 364; ++i, y += 18) {
    this->m_texts[i]->SetSize(540, 16);
    this->m_texts[i]->SetFont(0, 16, 0, 0);
    if (i < lineCount) {
      this->m_texts[i]->SetText(patchText + lines[i], 1);
    } else {
      this->m_texts[i]->SetText("", 1);
    }
    this->m_texts[i]->Move(30, y);
    this->AddChild(this->m_texts[i]);
  }

  this->m_scrollBar = new UIScrollBar(*this->m_context);
  static_cast<UIScrollBar*>(this->m_scrollBar)->Configure(583, 18, 1, this->geometry.height - 48);
  const int total = this->m_bottomPositions[this->m_index] - this->m_scrollPositions[this->m_index];
  static_cast<UIScrollBar*>(this->m_scrollBar)->SetRange(total - 19, 19, total);
  if (this->m_bottomPositions[this->m_index] - this->m_scrollPositions[this->m_index] < 19) {
    static_cast<UIScrollBar*>(this->m_scrollBar)->SetRange(0, 19, 19);
  }
  static_cast<UIScrollBar*>(this->m_scrollBar)->SetPosition(this->m_scroll);
  this->AddChild(this->m_scrollBar);

  FILE* source = std::fopen(this->m_patchFile, "r");
  FILE* target = std::fopen("notice_check.txt", "w");
  std::fseek(source, 0, SEEK_END);
  std::fseek(target, 0, SEEK_END);
  const int size = std::ftell(source);
  char* copy = new char[size + 1];
  std::fseek(source, 0, SEEK_SET);
  std::fread(copy, size, 1, source);
  copy[size] = 0;
  std::fclose(source);
  for (int i = 0; i < size; ++i) {
    std::fwrite(&copy[i], 1, 1, target);
  }
  std::fclose(target);

  if (changed == 0) {
    FILE* file = std::fopen("notice_read.txt", "rb");
    std::fread(&flag, 1, 1, file);
    std::fclose(file);
    if (flag != 0xFF) {
      this->m_scroll = flag;
    }
    if (this->m_scroll > this->m_bottomPositions[this->m_index] - 19) {
      this->m_scroll = this->m_bottomPositions[this->m_index] - 19;
    }
    if (this->m_scroll < 0) {
      this->m_scroll = 0;
    }
    this->LoadNotice();
  }
}

// OFFSET: 0x008B42D0

// OFFSET: 0x008B42D0
// FUNCTION: fn_11749
void UIReplayRECControlWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\replay_interface\\");
  const char* names[11] = {
    "btn_start_",      "btn_stop_",       "btn_start_",      "btn_save_",       "btn_setup_",      "btn_close_",
    "replay_recrun_1", "replay_recrun_2", "replay_recrun_3", "replay_recrun_4", "replay_recrun_5",
  };
  const char* tooltips[16] = {
    LookupMessage(0x7B9), LookupMessage(0x7BA), LookupMessage(0x7BB), LookupMessage(0x7BC), LookupMessage(0x7BD), LookupMessage(0x7BE),
    LookupMessage(0x7BF), LookupMessage(0x64),  LookupMessage(0x68),  LookupMessage(0x6A),  LookupMessage(0x11B), LookupMessage(0x7C0),
    LookupMessage(0x7C1), LookupMessage(0x7C2), LookupMessage(0x7C3), LookupMessage(0x7C4),
  };
  int ids[6] = {420, 421, 422, 423, 424, 425};

  for (int index = 0; index < 2; ++index) {
    this->buttons[index] = new UIReplayButton(*this->m_context, *this->m_controls);
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 0, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 1, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "b.bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(this->buttons[index]->BitmapWidth(), this->buttons[index]->BitmapHeight());
    this->buttons[index]->Move(-100, -100);
    this->buttons[index]->SetId(ids[index]);
    this->buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->buttons[index]);
  }

  for (int index = 2; index < 6; ++index) {
    this->buttons[index] = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 0, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 1, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "b.bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(this->buttons[index]->BitmapWidth(), this->buttons[index]->BitmapHeight());
    this->buttons[index]->Move(-100, -100);
    this->buttons[index]->SetId(ids[index]);
    this->buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->buttons[index]);
  }

  for (int index = 0; index < 5; ++index) {
    UIStaticBitmap* tab = new UIStaticBitmap(*this->m_context, *this->m_controls);
    this->tabs[index] = tab;
    tab->SetBitmap((prefix + names[index + 6] + ".bmp").c_str());
    tab->SetSize(tab->geometry.width, tab->geometry.height);
    tab->Move(-100, -100);
    tab->SetId(reinterpret_cast<int>(tooltips[index]));
    this->AddChild(tab);
  }

  int textSizes[2][2] = {{50, 12}, {80, 12}};
  const char* labels[2] = {"000:00:00", LookupMessage(0x7B3)};
  for (int index = 0; index < 2; ++index) {
    this->texts[index] = new UIStaticText(*this->m_context, *this->m_controls);
    this->texts[index]->SetSize(textSizes[index][0], textSizes[index][1]);
    this->texts[index]->SetText(labels[index], 0);
    this->texts[index]->Move(-100, -100);
    this->texts[index]->SetColor(255, 255, 255);
    this->AddChild(this->texts[index]);
  }

  std::memcpy(this->recordOptions, &Replay::Instance().header.options, sizeof(this->recordOptions));
  const char* optionNames[12] = {
    "Basicinfo", "Equip", "Item", "Skill", "Guild", "Party", "Chatting", "Shortcut", "Status", "ALL", "User Defined File Name", "Repeated File Check",
  };
  for (int index = 0; index < 10; ++index) {
    UICheckBox* option = new UICheckBox(*this->m_context, *this->m_controls);
    this->options[index] = option;
    option->SetSize(70, 12);
    option->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", optionNames[index]);
    option->Move(-100, -100);
    option->SetChecked(Replay::Instance().header.options.Get(static_cast<ReplayRecordOption>(index)));
    this->AddChild(option);
  }

  for (int index = 0; index < 2; ++index) {
    this->fileOptions[index] = new UICheckBox(*this->m_context, *this->m_controls);
    this->fileOptions[index]->SetSize(150, 12);
    this->fileOptions[index]->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", optionNames[index + 10]);
    this->fileOptions[index]->Move(-100, -100);
    this->fileOptions[index]->SetChecked(0);
    this->AddChild(this->fileOptions[index]);
  }

  const bool pathEnabled = ReplayFileList::Instance().pathEnabled;
  this->fileOptions[0]->SetChecked(pathEnabled);
  this->replayPathEnabled = pathEnabled;
  const bool option119 = ReplayFileList::Instance().unknown_0119;
  this->fileOptions[1]->SetChecked(option119);
  this->replayOption119 = option119;

  UIEditCtrl* edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->window_0134 = edit;
  edit->SetSize(151, 18);
  edit->Move(-100, -100);
  edit->maximum = 80;
  edit->Background(232, 232, 232);
  edit->SetText("");
  if (const char* tooltip = LookupMessage(0x7B4)) {
    edit->tooltip = tooltip;
  }
  this->AddChild(edit);
  edit->SelectAll();
  this->ApplyMode();
  this->Invalidate();
}

// OFFSET: 0x008B5A50
// FUNCTION: fn_11750
void UISelectReplayDataWnd::Slot15(int width, int height) {
  this->m_list = new UIReplayListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(6, 127, width - 12, height - 170, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  const char* names[4][3] = {
    {"btn_ok", "btn_ok_a", "btn_ok_b"},
    {"btn_cancel", "btn_cancel_a", "btn_cancel_b"},
    {"login_interface\\btn_exit", "login_interface\\btn_exit_a", "login_interface\\btn_exit_b"},
    {"replay_interface\\btn_selectserver", "replay_interface\\btn_selectserver_a", "replay_interface\\btn_selectserver_b"},
  };
  int ids[4] = {184, 185, 221, 400};
  int positions[4][2] = {{189, height - 25}, {-100, -100}, {234, height - 25}, {100, height - 25}};

  for (int index = 0; index < 4; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
    if (ids[index] == 221) {
      this->m_serverButton = button;
    }
  }

  this->m_message0 = 184;

  UIStaticText** text = this->m_texts;
  for (int y = 35; y < 110; y += 15) {
    *text = new UIStaticText(*this->m_context, *this->m_controls);
    (*text)->SetSize(width - 12, 15);
    (*text)->SetText("", 0);
    (*text)->Move(12, y);
    (*text)->SetColor(100, 100, 200);
    this->AddChild(*text);
    ++text;
  }
}

// OFFSET: 0x008B7040
// FUNCTION: fn_11752
void UITalkboxTrapInputWnd::Slot15(int, int) {
  this->edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->edit->SetSize(240, 16);
  this->edit->Move(22, 50);
  this->edit->maximum = 78;
  this->edit->Background(242, 242, 242);
  this->AddChild(this->edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x008B7A60
// FUNCTION: fn_11753
void UILoginOTPWnd::Slot18() {
}

// OFFSET: 0x008B7A70
// FUNCTION: fn_11754
void UILoginWnd::Slot18() {
}

// OFFSET: 0x008B7A80
// FUNCTION: fn_11755
void UIMakeCharWnd::Slot18() {
}

// OFFSET: 0x008B8E90
// FUNCTION: fn_11769
void UIMessageBoxAutoreturn::Paint() {
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}

// OFFSET: 0x008BA670
// FUNCTION: fn_11788
void UILoginOTPWnd::Slot19() {
  auto* login = static_cast<CLoginMode*>(g_modeMgr.GetLoginMode());
  if (login == nullptr) {
    return;
  }

  const unsigned remaining = static_cast<unsigned>(60.0 - (timeGetTime() - login->otpStarted) * 0.001);
  const std::uint8_t source[] = {0x6A, 0x91, 0xFD, 0x51, 0x43, 0xED, 0x61, 0x5C, 0x7F, 0x93, 0xAB, 0x35};
  thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> format(source);
  this->timerText->SetText(FormatOwnedString(format.Text(), remaining / 60, remaining % 60).c_str(), 0);
  this->Invalidate();
}

// OFFSET: 0x008BAEF0
// FUNCTION: fn_11792
void UIMessageBoxAutoreturn::Slot19() {
  if (this->counting == 0) {
    return;
  }

  if (timeGetTime() > this->deadline) {
    g_windowMgr.unknown_0474 = 187;
    this->counting = 0;
    return;
  }

  const unsigned previous = this->remaining;
  const bool fraction = (this->deadline - timeGetTime()) % 1000 != 0;
  this->remaining = (this->deadline - timeGetTime()) / 1000 + fraction;

  if (previous != this->remaining) {
    char text[512];
    std::sprintf(text, this->format.c_str(), this->remaining);
    this->messageText = text;
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->messageText.data()), 38, 0, 0);
  }

  const unsigned duration = this->seconds * 1000;
  const unsigned progress = (timeGetTime() + (duration - this->deadline)) * 100 / duration;
  this->gauge->SetValue(static_cast<float>(progress));
}

// OFFSET: 0x008AA9D0
// FUNCTION: fn_11723
void UIItemWnd::DrawNewItemMark(int x, int y) {
  const auto* mark = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\new_item.bmp")));
  this->DrawBitmap(x, y, &mark->image, false);
}

// OFFSET: 0x008AAA10
// FUNCTION: fn_11724
void UIMonsterInfoWnd::DrawResistance(int x, int y, const char* label, int value, std::uint32_t border, std::uint32_t fill) {
  auto& controls = *this->m_controls;
  char text[64];
  wsprintfA(text, "%s:%d", label, value);
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  this->FillRegion(x, y, 70, 16, border);
  this->FillRegion(x + 1, y + 1, 68, 14, fill);
  const int width = this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, (70 - width) / 2 + x, y + 2, text, 0, 0, 12, RGB(red, blue, green), 0, 0);
}

// OFFSET: 0x008B8ED0
// FUNCTION: fn_11770
void UIMonsterInfoWnd::Paint() {
  auto& controls = *this->m_controls;
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const std::uint32_t fill = RGB(red, green, blue);
  this->ClearSurface(fill);
  this->DrawPanelBackground(this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0), 0, "");
  this->DrawButtonBar(2, 0, 1);
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  const std::uint32_t border = RGB(red, green, blue);

  this->DrawTextAt(controls.fonts, controls.locale, 17, 3, LookupMessage(406), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 2, LookupMessage(406), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 30, LookupMessage(407), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 30, this->name.c_str(), 0, 0, 12, 0, 0, 0);

  char text[256];
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.level);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 50, LookupMessage(408), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 50, text, 0, 0, 12, 0, 0, 0);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.hp);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 70, LookupMessage(409), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 70, text, 0, 0, 12, 0, 0, 0);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.defense);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 90, LookupMessage(3910), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 90, text, 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(controls.fonts, controls.locale, 260, 30, LookupMessage(410), 0, 0, 12, 0, 0, 0);
  int message;
  switch (this->info.size) {
  case 0:
    message = 3907;
    break;
  case 1:
    message = 3908;
    break;
  case 2:
    message = 3909;
    break;
  default:
    message = 0;
    break;
  }
  if (message != 0) {
    this->DrawTextAt(controls.fonts, controls.locale, 300, 30, LookupMessage(message), 0, 0, 12, 0, 0, 0);
  }

  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.race);
  this->DrawTextAt(controls.fonts, controls.locale, 260, 50, LookupMessage(411), 0, 0, 12, 0, 0, 0);
  if (static_cast<unsigned short>(this->info.race) <= 9) {
    this->DrawTextAt(controls.fonts, controls.locale, 300, 50, LookupMessage(3887 + static_cast<unsigned short>(this->info.race)), 0, 0, 12, 0, 0, 0);
  }

  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.magicDefense);
  this->DrawTextAt(controls.fonts, controls.locale, 260, 90, LookupMessage(412), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 300, 90, text, 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(controls.fonts, controls.locale, 260, 70, LookupMessage(413), 0, 0, 12, 0, 0, 0);
  const int element = this->info.element % 20;
  if (element >= 0 && element <= 9) {
    this->DrawTextAt(controls.fonts, controls.locale, 300, 70, LookupMessage(3897 + element), 0, 0, 12, 0, 0, 0);
  }

  this->DrawResistance(120, 110, LookupMessage(3898), this->info.resistances[0], border, fill);
  this->DrawResistance(120, 128, LookupMessage(3899), this->info.resistances[1], border, fill);
  this->DrawResistance(120, 146, LookupMessage(3900), this->info.resistances[2], border, fill);
  this->DrawResistance(200, 110, LookupMessage(3901), this->info.resistances[3], border, fill);
  this->DrawResistance(200, 128, LookupMessage(3902), this->info.resistances[4], border, fill);
  this->DrawResistance(200, 146, LookupMessage(3903), this->info.resistances[5], border, fill);
  this->DrawResistance(280, 110, LookupMessage(3904), this->info.resistances[6], border, fill);
  this->DrawResistance(280, 128, LookupMessage(3905), this->info.resistances[7], border, fill);
  this->DrawResistance(280, 146, LookupMessage(3906), this->info.resistances[8], border, fill);

  char spriteName[128];
  StringFormat(spriteName, sizeof(spriteName), static_cast<std::size_t>(-1), "몬스터\\%s.spr", g_session.GetJobName(this->info.job, -1));
  char actionName[128];
  StringFormat(actionName, sizeof(actionName), static_cast<std::size_t>(-1), "몬스터\\%s.act", g_session.GetJobName(this->info.job, -1));
  if (ResourceMgr::Instance().Exists(actionName) && ResourceMgr::Instance().Exists(spriteName)) {
    auto* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));
    auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));
    this->DrawActionSprite(sprite, action, 2, 19, 116, 152, 0, true, nullptr);
  }
}

// OFFSET: 0x008C3440
// FUNCTION: fn_11823
void UIReplayRECControlWnd::UpdateRecordTime() {
  for (int index = 0; index < 4; ++index) {
    this->tabs[index]->Move(-100, -100);
  }

  this->tabs[timeGetTime() % 1000 / 250]->Move(6, 5);
  if (timeGetTime() - this->recordStart <= 200) {
    return;
  }

  const int seconds = static_cast<int>(timeGetTime() - Replay::Instance().started) / 1000;
  const int minutes = seconds / 60;
  const int hours = minutes / 60;
  if (hours == 100) {
    this->window_00E8->Move(124, 47);
  } else if (hours == 1000) {
    this->window_00E8->Move(116, 47);
  }

  char text[100] = {};
  std::sprintf(text, "%03d:%02d:%02d", hours, minutes % 60, seconds % 60);
  this->window_00E8->SetTextFitted(text);
  this->Invalidate();
  this->InvalidateChildren();
  this->recordStart = timeGetTime();
}

// OFFSET: 0x008C0DF0
// FUNCTION: fn_11814
int UIReplayRECControlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 213:
      if (sender == this->window_0128) {
        ReplayFileList::Instance().pathEnabled = b != 0;
        this->replayPathEnabled = ReplayFileList::Instance().pathEnabled;
        return 0;
      }

      if (sender == this->window_012C) {
        ReplayFileList::Instance().unknown_0119 = b != 0;
        this->replayOption119 = ReplayFileList::Instance().unknown_0119;
        return 0;
      }

      if (sender == this->options[9]) {
        if (b == 1) {
          for (int index = 0; index < 10; ++index) {
            if (index != 9) {
              this->recordOptions[index] = 1;
              static_cast<UICheckBox*>(this->options[index])->SetChecked(1);
            }
          }
        }

        this->recordOptions[9] = b != 0;
      } else if (sender == this->options[0]) {
        if (b == 0) {
          for (int index = 1; index < 10; ++index) {
            this->recordOptions[index] = 0;
            static_cast<UICheckBox*>(this->options[index])->SetChecked(0);
          }
        }

        this->recordOptions[0] = b != 0;
      } else {
        if (b == 1) {
          if (this->options[0]->geometry.unknown_0030 == 0) {
            static_cast<UICheckBox*>(this->options[0])->SetChecked(1);
            this->recordOptions[0] = 1;
          }
        } else if (this->options[9]->geometry.unknown_0030 == 1) {
          static_cast<UICheckBox*>(this->options[9])->SetChecked(0);
          this->recordOptions[9] = 0;
        }

        for (int index = 1; index < 9; ++index) {
          if (sender == this->options[index]) {
            this->recordOptions[index] = b != 0;
          }
        }
      }

      Replay::Instance().SetOptions(this->recordOptions, 10);
      return 0;
    case 420:
      if (!this->replayPathEnabled) {
        this->mode = 1;
        static_cast<CMode*>(g_charViewMode)->SendMsg(274, 0, 0, 0, 0);
        this->ApplyMode();
        return 0;
      }

      this->mode = 3;
      this->ApplyMode();
      return 0;
    case 421:
      this->mode = 0;
      this->ApplyMode();
      static_cast<CMode*>(g_charViewMode)->SendMsg(275, 0, 0, 0, 0);
      return 0;
    case 422: {
      auto* edit = static_cast<UIEditCtrl*>(this->window_0134);
      int error;

      if (edit->Length() <= 0) {
        error = 1972;
      } else if (_mbsstr(reinterpret_cast<const unsigned char*>(edit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
                 _mbsstr(reinterpret_cast<const unsigned char*>(edit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        error = 2812;
      } else {
        ReplayFileList::Instance().SetPath(edit->Text());
        this->m_focus->Set(nullptr);
        edit->SetText("");
        static_cast<CMode*>(g_charViewMode)->SendMsg(274, 0, 0, 0, 0);

        if (Replay::Instance().state == 1) {
          return 0;
        }

        this->mode = 1;
        this->ApplyMode();
        return 0;
      }

      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(error), 0, true, false, 0, "Message", 280, 120, 0);
      return 0;
    }
    case 423:
      this->mode = 2;
      this->ApplyMode();
      return 0;
    case 424:
      this->mode = 0;
      this->ApplyMode();
      return 0;
    case 425:
      this->mode = 0;
      g_windowMgr.CloseWindow(198);
      return 0;
    default:
      return 0;
    }
  case 34:
    this->saved = reinterpret_cast<ReplayRECControlWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case 60:
    this->Invalidate();
    this->InvalidateChildren();
    return 0;
  case 87:
    Replay::Instance().unknown_01F4 = this->geometry.x;
    Replay::Instance().unknown_01F8 = this->geometry.y;
    return 0;
  case 116:
    this->UpdateRecordTime();
    return 0;
  case 123:
  case 251:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008BA820
// FUNCTION: fn_11789
void UILoginWnd::Slot19() {
  if (g_windowMgr.GetHoverWindow() == this->idEdit) {
    this->idEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
    if (this->idPlaceholder == true) {
      const std::uint8_t source[] = {0xA4, 0x31, 0x61};
      thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> placeholder(source);
      if (std::strcmp(placeholder.Text(), this->idEdit->Text()) == 0) {
        this->idEdit->SetText("");
        goto password;
      }
    }
    this->idPlaceholder = false;
  } else {
    if (std::strcmp("", this->idEdit->Text()) == 0) {
      this->idEdit->TextColor(RGB(156, 156, 156), 0xFFFFFFFF);
      this->idPlaceholder = true;
      const std::uint8_t source[] = {0x5E, 0x5F, 0x2D};
      thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> placeholder(source);
      this->idEdit->SetText(placeholder.Text());
    }
    if (this->idPlaceholder == false) {
      this->idEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
    }
  }

password:
  if (g_windowMgr.GetHoverWindow() == this->passwordEdit) {
    HIMC context = ImmGetContext(hWnd);
    DWORD conversion;
    DWORD sentence;
    ImmGetConversionStatus(context, &conversion, &sentence);
    conversion &= ~(IME_CMODE_NATIVE | IME_CMODE_KATAKANA);
    ImmSetConversionStatus(context, IME_CMODE_ALPHANUMERIC, sentence);
    ImmReleaseContext(hWnd, context);
    this->passwordEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
    if (this->passwordPlaceholder == true) {
      const std::uint8_t source[] = {0x99, 0x52, 0xAA, 0x20, 0xAE, 0x96, 0x47, 0x35, 0xC9};
      thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> placeholder(source);
      if (std::strcmp(placeholder.Text(), this->passwordEdit->Text()) == 0) {
        this->passwordEdit->SetText("");
        this->passwordEdit->mask = 0;
        return;
      }
    }
    this->passwordPlaceholder = false;
  } else {
    if (std::strcmp("", this->passwordEdit->Text()) == 0) {
      this->passwordPlaceholder = true;
      this->passwordEdit->TextColor(RGB(156, 156, 156), 0xFFFFFFFF);
      const std::uint8_t source[] = {0x33, 0xB4, 0xB2, 0x2C, 0x6C, 0x6A, 0x4F, 0x8F, 0x63};
      thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> placeholder(source);
      this->passwordEdit->SetText(placeholder.Text());
      this->passwordEdit->mask = 0;
    }
    if (this->passwordPlaceholder != false) {
      return;
    }
    this->passwordEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
  }

  this->passwordEdit->mask = '*';
}

// OFFSET: 0x008C36F0
// FUNCTION: fn_11830
void UIChatRoomWnd::SaveChatLog() {
  int index = 0;
  char path[2048];
  std::sprintf(path, "%s\\Chat", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  std::string name = this->title;
  for (unsigned position = 0; position < name.size(); ++position) {
    const char character = name[position];
    if (character == '\\' || character == '/' || character == ':' || character == '*' || character == '?' || character == '"' || character == '<' ||
        character == '>' || character == '|') {
      name[position] = '_';
    }
  }

  std::sprintf(path, "%s\\Chat\\%s.txt", g_applicationDirectory, name.c_str());
  while (_access(path, 0) == 0) {
    ++index;
    std::sprintf(path, "%s\\Chat\\%s%03d.txt", g_applicationDirectory, name.c_str(), index);
  }

  if (this->history->SaveChatLog(path)) {
    std::string message = path;
    message.append(" is Saved.", 10);
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
  }
}

// OFFSET: 0x008ABDB0
// FUNCTION: fn_11728
void UILoginWnd::LoadLogin() {
  HKEY probe;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &probe) != ERROR_SUCCESS) {
    this->idEdit->SetText("");
    this->passwordEdit->SetText("");
    return;
  }

  RegCloseKey(probe);
  HKEY key;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS) {
    return;
  }

  DWORD type = REG_DWORD;
  DWORD size = 4;
  int saveId;
  const std::uint8_t checkSource[] = {0xB2, 0x8F, 0x7C, 0x4A, 0x5C, 0x2A, 0x0E, 0xA7, 0xB4, 0xC7};
  thread_local EncryptedName<sizeof(checkSource), 0xF14F79170939C7F1ULL> checkName(checkSource);
  RegQueryValueExA(key, checkName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(&saveId), &size);
  saveId = saveId == 1;

  int bitwise;
  const std::uint8_t bitwiseSource[] = {0xDD, 0x8C, 0x39, 0xFC, 0x9E, 0x7A, 0xB8, 0xA5};
  thread_local EncryptedName<sizeof(bitwiseSource), 0xA5FD29D7AB6DC59FULL> bitwiseName(bitwiseSource);
  RegQueryValueExA(key, bitwiseName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(&bitwise), &size);

  char id[52];
  std::memset(id, 0, sizeof(id));
  char password[26] = {};
  type = REG_SZ;
  size = 25;
  const std::uint8_t passwordSource[] = {0xB1, 0x3A, 0xAA, 0x84, 0x82, 0x5C, 0x6B, 0x99, 0xE1};
  thread_local EncryptedName<sizeof(passwordSource), 0xDD3913D5D7F97BE1ULL> passwordName(passwordSource);
  RegQueryValueExA(key, passwordName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(password), &size);
  password[24] = 0;

  type = REG_SZ;
  size = 51;
  const std::uint8_t idSource[] = {0xD2, 0x13, 0x03};
  thread_local EncryptedName<sizeof(idSource), 0x050505679903579BULL> idName(idSource);
  RegQueryValueExA(key, idName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(id), &size);
  id[50] = 0;
  password[24] = 0;
  this->saveId = saveId;

  if (saveId != 0) {
    unsigned char shade;
    bool placeholder = std::strcmp("", id) == 0;
    if (!placeholder) {
      const std::uint8_t compareSource[] = {0xF4, 0xF9, 0xED};
      thread_local EncryptedName<sizeof(compareSource), 0x83EFBFBB37EDBDBDULL> compareName(compareSource);
      placeholder = std::strcmp(compareName.Text(), id) == 0;
    }

    if (placeholder) {
      this->idPlaceholder = true;
      const std::uint8_t textSource[] = {0xEE, 0xB1, 0x7F};
      thread_local EncryptedName<sizeof(textSource), 0xBF2B4B31397FF5A7ULL> textName(textSource);
      this->idEdit->SetText(textName.Text());
      shade = 156;
    } else {
      shade = 0;
      this->idPlaceholder = false;
    }

    this->idEdit->TextColor(RGB(shade, shade, shade), 0xFFFFFFFF);
    this->idEdit->SetText(id);
    this->passwordEdit->SetText("");
  }

  RegCloseKey(key);
}

// OFFSET: 0x008AB050
// FUNCTION: fn_11726
int sub_8AB050() {
  return 4;
}

// OFFSET: 0x008AB060
// FUNCTION: fn_11727
int sub_8AB060() {
  return 6;
}

// OFFSET: 0x008A4220
// FUNCTION: fn_11666
UINoticeWnd::UINoticeWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->m_patchFile = "notice_patch.txt";
  for (int i = 0; i < 19; ++i) {
    this->m_texts[i] = new UIStaticText(*this->m_context, *this->m_controls);
  }
  this->unknown_0108 = 20;
  this->unknown_010C = 0;
  this->m_noticeCount = 0;
  this->m_index = 0;
  this->m_scrollPositions[0] = 0;
  this->m_bottomPositions[0] = 0;
  this->m_scroll = 0;
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this);
}

// OFFSET: 0x008AD190
// FUNCTION: fn_11737
void UIBank_NewWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "Exit", 0);
  this->AddBitmapButton(*this->m_controls, 462, 209, 77, "bank\\btn_deposit", LookupMessage(2774));
  this->AddBitmapButton(*this->m_controls, 463, 209, 102, "bank\\btn_withdraw", LookupMessage(2775));
  this->AddBitmapButton(*this->m_controls, 279, 177, 89, "bank\\btn_upper", LookupMessage(2776));
  this->AddBitmapButton(*this->m_controls, 280, 177, 99, "bank\\btn_buttom", LookupMessage(2777));
  this->AddBitmapButton(*this->m_controls, 392, 189, 132, "bank\\btn_10mil", "+ 100,000");
  this->AddBitmapButton(*this->m_controls, 391, 141, 132, "bank\\btn_100mil", "+ 1,000,000");
  this->AddBitmapButton(*this->m_controls, 390, 93, 132, "bank\\btn_1000mil", "+ 10,000,000");
  this->AddBitmapButton(*this->m_controls, 389, 45, 132, "bank\\btn_max", LookupMessage(2778));

  this->m_amountEdit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_amountEdit->SetSize(99, 17);
  this->m_amountEdit->maximum = 10;
  this->m_amountEdit->Move(78, 90);
  this->m_amountEdit->Background(247, 247, 247);
  this->m_amountEdit->Border(198, 198, 198);
  this->m_amountEdit->flat = 0;
  this->m_amountEdit->topPadding = 1;
  this->AddChild(this->m_amountEdit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_amountEdit);
}

// OFFSET: 0x008AD770
// FUNCTION: fn_11739
void UICntWnd::Slot15(int, int) {
  this->m_edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edit->SetSize(80, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 6;
  this->m_edit->Background(232, 232, 232);
  this->AddChild(this->m_edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
  this->m_edit->SelectAll();

  this->m_text = new UIStaticText(*this->m_context, *this->m_controls);
  this->m_text->SetSize(this->geometry.width - 30, 15);
  this->m_text->Move(15, 6);
  this->AddChild(this->m_text);

  const std::string prefix("유저인터페이스\\");
  const int width = this->geometry.width;
  const int height = this->geometry.height;
  UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 55, height - 32);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
}

// OFFSET: 0x008C0B20
// FUNCTION: fn_11813
int UIReplayControlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 401:
      g_modeMgr.gameMode->SendMsg(276, 0, 0, 0, 0);
      return 0;
    case 402:
      g_modeMgr.gameMode->SendMsg(277, 0, 0, 0, 0);
      return 0;
    case 403:
      g_modeMgr.gameMode->SendMsg(278, 0, 0, 0, 0);
      Replay::Instance().speed = 1.0f;
      return 0;
    case 404:
      g_windowMgr.Schedule(this);
      g_modeMgr.gameMode->SendMsg(281, 0, 0, 0, 0);
      Replay::Instance().speed = 1.0f;
      return 0;
    case 405:
      g_modeMgr.gameMode->SendMsg(279, 0, 0, 0, 0);
      this->SendMsg(nullptr, 60, 0, 0, 0, 0);
      return 0;
    case 406:
      g_modeMgr.gameMode->SendMsg(280, 0, 0, 0, 0);
      this->SendMsg(nullptr, 60, 0, 0, 0, 0);
      return 0;
    }
    return 0;
  case 35:
    if (!this->m_paused) {
      this->m_playButton->Move(-100, -100);
      this->m_pauseButton->Move(5, 89);
      this->m_paused = true;
    } else {
      this->m_playButton->Move(5, 89);
      this->m_pauseButton->Move(-100, -100);
      this->m_paused = false;
    }
    this->Invalidate();
    this->InvalidateChildren();
    return 0;
  case 60:
    this->RefreshInfo();
    return 0;
  case 251:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x008C3110
// FUNCTION: fn_11822
bool UIReplayControlWnd::RefreshInfo() {
  const int seconds = static_cast<int>(Replay::Instance().elapsed) / 1000;
  const int minutes = seconds / 60;
  const ReplayTime duration = Replay::Instance().Duration();
  char text[100];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(1955), minutes / 60, minutes % 60, seconds % 60, duration.hours, duration.minutes, duration.seconds);
  this->m_texts[0]->SetTextFitted(text);

  std::memset(text, 0, sizeof(text));
  const int level = Replay::Instance().SpeedLevel();
  int speedMessage;
  if (level == 1) {
    speedMessage = 1956;
  } else if (level == 2) {
    speedMessage = 1957;
  } else if (level == 3) {
    speedMessage = 1958;
  } else if (level == 4) {
    speedMessage = 1959;
  } else if (level == 5) {
    speedMessage = 1960;
  } else if (level == 6) {
    speedMessage = 1961;
  } else {
    speedMessage = (level != 7) + 1962;
  }
  std::sprintf(text, LookupMessage(speedMessage));
  this->m_texts[1]->SetTextFitted(text);

  std::string location;
  if (g_modeMgr.gameMode != nullptr) {
    location = static_cast<CGameMode*>(g_modeMgr.gameMode)->LocationText();
  }
  this->m_texts[3]->SetTextFitted(location.c_str());
  this->Invalidate();
  this->InvalidateChildren();
  return true;
}

// OFFSET: 0x008C14F0
// FUNCTION: fn_11815
int UISelectReplayDataWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 184:
      if (this->m_list->selected != -1) {
        this->UpdateInfo();
        this->Invalidate();
        if (ReplayFileList::Instance().Open(32, 0, 0) == 1) {
          if (Replay::Instance().MatchesService(g_serviceMode)) {
            static_cast<CMode*>(g_charViewMode)->SendMsg(10027, 0, 0, 0, 0);
            g_windowMgr.Schedule(this);
            g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
            return 0;
          }
          Replay::Instance().CloseFile();
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1970), 0, true, true, 0, "Message", 280, 120, 0);
        } else {
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1969), 0, true, true, 0, "Message", 280, 120, 0);
        }
      }
      break;
    case 185:
    case 400:
      Replay::Instance().SetState(1);
      g_windowMgr.Schedule(this);
      static_cast<CMode*>(g_charViewMode)->SendMsg(10025, 0, 0, 0, 0);
      return 0;
    case 221:
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(17), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
        return 0;
      }
      break;
    }
    break;
  case 18:
    this->m_list->SendMsg(nullptr, 18, 0, 0, 0, 0);
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case 19:
    this->m_list->SendMsg(nullptr, 19, 0, 0, 0, 0);
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case 40:
    this->m_list->SendMsg(nullptr, 40, a, 0, 0, 0);
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case 80:
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case 89:
    this->m_serverButton->Move(234, this->m_serverButton->geometry.y);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008C2460
// FUNCTION: fn_11820
bool UISelectReplayDataWnd::UpdateInfo() {
  if (!ReplayFileList::Instance().Select(this->m_list->selected)) {
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted("");
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
    return true;
  }

  if (ReplayFileList::Instance().Open(32, 0, 0) != 1) {
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted(LookupMessage(1969));
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
    return true;
  }

  Replay::Instance().CloseFile();
  char buffer[100];
  std::memset(buffer, 0, sizeof(buffer));
  std::string service;
  switch (Replay::Instance().header.serviceType) {
  case 0:
    service = "Primary";
    break;
  case 1:
    service = "Sakray";
    break;
  case 2:
    service = "Local";
    break;
  case 3:
    service = "InstantEvent";
    break;
  case 4:
    service = "PVP";
    break;
  case 5:
    service = "PK";
    break;
  default:
    service = "Data Error";
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted(LookupMessage(1969));
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
    return false;
  }

  std::sprintf(buffer, LookupMessage(1964), service.c_str());
  this->m_texts[0]->SetTextFitted(buffer);
  std::memset(buffer, 0, sizeof(buffer));

  char name[64];
  std::memcpy(name, Replay::Instance().header.characterName, sizeof(name));
  std::sprintf(buffer, LookupMessage(1965), name);
  this->m_texts[1]->SetTextFitted(buffer);

  std::sprintf(buffer, LookupMessage(1966), Replay::Instance().header.mapName);
  this->m_texts[2]->SetTextFitted(buffer);

  const std::uint32_t date = Replay::Instance().header.key0;
  const std::uint32_t time = Replay::Instance().header.key1;
  std::sprintf(
    buffer, LookupMessage(1967), date & 0xFFFF, static_cast<char>(date >> 16), static_cast<char>(date >> 24), static_cast<char>(time >> 8),
    static_cast<char>(time >> 16), static_cast<char>(time >> 24)
  );
  this->m_texts[3]->SetTextFitted(buffer);

  const ReplayTime duration = Replay::Instance().Duration();
  std::sprintf(buffer, LookupMessage(1968), duration.hours, duration.minutes, duration.seconds);
  this->m_texts[4]->SetTextFitted(buffer);
  return true;
}

// OFFSET: 0x008C18C0
// FUNCTION: fn_11816
int UISelectServerWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 184:
      if (this->m_serverList->selected != -1) {
        g_windowMgr.Schedule(this);
        static_cast<CMode*>(g_charViewMode)->SendMsg(this->m_event, this->m_serverList->selected, 0, 0, 0);
        g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        const char* name = this->m_valueName.c_str();
        this->m_selectedServer = this->m_serverList->selected;
        HKEY key;
        DWORD disposition = 0;
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
          RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_selectedServer), 4);
          RegCloseKey(key);
          return 0;
        }
      }
      break;
    case 185:
      g_windowMgr.Schedule(this);
      static_cast<CMode*>(g_charViewMode)->SendMsg(10006, 0, 0, 0, 0);
      return 0;
    case 221:
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(17), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
        return 0;
      }
      break;
    case 400:
      reinterpret_cast<Replay&>(ReplayFileList::Instance()).SetBaseFrame(reinterpret_cast<std::uint32_t>(&Replay::Instance()));
      ReplayFileList::Instance().unknown_011C = g_session.m_aid;
      ReplayFileList::Instance().unknown_0120 = g_session.SyncUnknown0E98();
      static_cast<CMode*>(g_charViewMode)->SendMsg(10026, 0, 0, 0, 0);
      g_windowMgr.Schedule(this);
      return 0;
    }
    break;
  case 18:
    this->m_serverList->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return 0;
  case 19:
    this->m_serverList->SendMsg(nullptr, 19, 0, 0, 0, 0);
    return 0;
  case 40:
    this->m_serverList->SendMsg(nullptr, 40, this->m_selectedServer, 0, 0, 0);
    return 0;
  case 80: {
    this->m_event = a;
    this->m_valueName = reinterpret_cast<const char*>(b);
    const char* name = this->m_valueName.c_str();
    this->m_selectedServer = g_serviceType;
    HKEY probe;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &probe) == ERROR_SUCCESS) {
      RegCloseKey(probe);
      HKEY key;
      if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
        DWORD type = REG_DWORD;
        DWORD size = 4;
        RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&this->m_selectedServer), &size);
        RegCloseKey(key);
        return 0;
      }
    }
    break;
  }
  case 89:
    this->m_okButton->Move(234, this->m_okButton->geometry.y);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008B99A0
// FUNCTION: fn_11773
void UIPetInfoWnd::Paint() {
  auto& controls = *this->m_controls;
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  this->DrawPanelBackground(this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0), 0, "");

  const auto* leftCorner = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftCorner->image.height, &leftCorner->image, false);
  const auto* rightCorner = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_rd.bmp")));
  this->DrawBitmap(
    this->geometry.width - rightCorner->image.width, this->geometry.height - rightCorner->image.height, &rightCorner->image, false
  );

  char title[32];
  StringFormat(title, 32, static_cast<std::size_t>(-1), "%s", LookupMessage(584));
  this->DrawTextAt(controls.fonts, controls.locale, 5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 24, LookupMessage(407), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 44, LookupMessage(408), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 69, LookupMessage(585), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 89, LookupMessage(586), 0, 0, 12, 0, 0, 0);

  char level[8];
  char hunger[24];
  const int fullness = g_session.unknown_1044;

  if (fullness >= 0 && fullness <= 10) {
    std::strcpy(hunger, LookupMessage(667));
  } else if (fullness > 10 && fullness <= 25) {
    std::strcpy(hunger, LookupMessage(668));
  } else if (fullness > 25 && fullness <= 75) {
    std::strcpy(hunger, LookupMessage(669));
  } else if (fullness > 75 && fullness <= 90) {
    std::strcpy(hunger, LookupMessage(670));
  } else if (fullness > 90 && fullness <= 100) {
    std::strcpy(hunger, LookupMessage(671));
  } else {
    std::strcpy(hunger, LookupMessage(676));
  }

  _itoa(g_session.unknown_1040, level, 10);
  this->DrawTextAt(controls.fonts, controls.locale, 135, 44, level, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 145, 69, hunger, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 145, 89, this->friendlyText.c_str(), 0, 0, 12, 0, 0, 0);

  const int job = g_session.unknown_103C;

  if (job != -1 && ((job > 1000 && job < 3999) || static_cast<unsigned>(job - 20021) < 11979)) {
    char illust[260];
    std::memset(illust, 0, sizeof(illust));
    g_session.GetPetIllustName(job, g_session.unknown_1038, illust);

    if (auto* image = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(illust)))) {
      this->DrawBitmap(5, 21, &image->image, false);
    }

    char command[] = "< Command >";
    this->FillRegion(160, 44, 110, 18, 0xFAE6E6);
    const int width = this->TextWidth(controls.fonts, controls.locale, command, 0, 0, 12, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 206 - width / 2, 48, command, 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 100, 119, LookupMessage(597), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 100, 139, LookupMessage(g_session.unknown_1038 == 0 ? 600 : 598), 0, 0, 12, 0, 0, 0);

    if (this->unknown_00E4 == 1) {
      this->DrawTextAt(controls.fonts, controls.locale, 100, 159, LookupMessage(2577), 0, 0, 12, 0, 0, 0);
    }
  }
}

// OFFSET: 0x008C2E30
// FUNCTION: fn_11821
void UIPetInfoWnd::UpdateFriendlyGauge() {
  const std::string previous = this->friendlyText;
  int low;
  int high;
  if (g_session.m_petFriendly >= 0 && g_session.m_petFriendly <= 100) {
    if (g_session.GetDisplayJob() >= 4050 && g_session.GetDisplayJob() <= 4052) {
      int message;
      if (g_session.m_petFriendly <= 20) {
        low = 0;
        message = 1022;
        high = 100;
      } else if (g_session.m_petFriendly <= 40) {
        low = 20;
        message = 1021;
        high = 40;
      } else {
        low = 40;
        message = 672;
        high = 100;
      }
      this->friendlyText = LookupMessage(message);
    } else {
      this->friendlyText = LookupMessage(672);
      low = 0;
      high = 100;
    }
  } else if (g_session.m_petFriendly > 100 && g_session.m_petFriendly <= 250) {
    this->friendlyText = LookupMessage(673);
    low = 100;
    high = 250;
  } else if (g_session.m_petFriendly > 250 && g_session.m_petFriendly <= 750) {
    this->friendlyText = LookupMessage(669);
    low = 250;
    high = 750;
  } else if (g_session.m_petFriendly > 750 && g_session.m_petFriendly <= 900) {
    this->friendlyText = LookupMessage(674);
    low = 750;
    high = 900;
  } else if (g_session.m_petFriendly > 900 && g_session.m_petFriendly <= 1000) {
    this->friendlyText = LookupMessage(675);
    low = 900;
    high = 1000;
  } else {
    this->friendlyText = LookupMessage(676);
    low = 0;
    high = 0;
  }
  const float ratio = static_cast<float>(static_cast<unsigned>(g_session.m_petFriendly - low)) / static_cast<float>(static_cast<unsigned>(high - low));
  this->friendlyGauge->SetValue(static_cast<unsigned>(ratio * 100.0f));
  if (!(previous == this->friendlyText)) {
    this->Invalidate();
  }
}

// OFFSET: 0x008B1240
// FUNCTION: fn_11745
void UIMonsterInfoWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\");
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(this->geometry.width - 46, this->geometry.height - 24);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;

  this->unknown_00D4 = g_monsterInfoActorId;
  this->name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(g_monsterInfoActorId);
  const std::size_t separator = this->name.find("#");
  if (separator != std::string::npos) {
    this->name.erase(separator, this->name.size());
  }
  this->name = this->FitDisplayText(100, this->name.c_str(), 0, 12, 0);
}

// OFFSET: 0x008A4300
// FUNCTION: fn_11667
UIPetInfoWnd::UIPetInfoWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), friendlyGauge(nullptr), m_controls(&controls) {
  this->friendlyText.clear();
  this->unknown_00E4 = 0;
  this->unknown_00E8 = 0;
  this->nameEdit = nullptr;
  this->saved = nullptr;
  this->unknown_00BC = 0;
  this->unknown_00C0 = 0;
  this->window_00C4 = nullptr;
}

// OFFSET: 0x008A43E0
// FUNCTION: fn_11668
UIReplayButton::UIReplayButton(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls) {
}

// OFFSET: 0x008A4440
// FUNCTION: fn_11669
UIReplayControlWnd::UIReplayControlWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->m_paused = false;
}

// OFFSET: 0x008A4500
// FUNCTION: fn_11670
UIReplayRECControlWnd::UIReplayRECControlWnd(UIWindowContext& windows, UIFrameContext& frames, InputFocus& focus, int mode)
    : UIFrameWnd(windows, frames, 0), mode(mode), recordStart(0), m_focus(&focus) {
  for (int i = 0; i < 10; ++i) {
    this->options[i] = nullptr;
    this->recordOptions[i] = 0;
  }
}

// OFFSET: 0x008A45A0
// FUNCTION: fn_11671
UISelectReplayDataWnd::UISelectReplayDataWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->m_list = nullptr;
  this->m_serverButton = nullptr;
  this->m_texts[0] = nullptr;
  this->m_texts[1] = nullptr;
  this->m_texts[2] = nullptr;
  this->m_texts[3] = nullptr;
}

// OFFSET: 0x008A4640
// FUNCTION: fn_11672
UISelectServerWnd::UISelectServerWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
  this->m_serverList = nullptr;
  this->m_okButton = nullptr;
}

// OFFSET: 0x008A46E0
// FUNCTION: fn_11673
UITalkboxTrapInputWnd::UITalkboxTrapInputWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& editContext)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls), m_editContext(&editContext) {
}

// OFFSET: 0x008A4E00
// FUNCTION: fn_11677
UICntWnd::~UICntWnd() = default;

// OFFSET: 0x008A5050
// FUNCTION: fn_11679
UILoginWnd::~UILoginWnd() = default;

// OFFSET: 0x008A54C0
// FUNCTION: fn_11681
UIPetInfoWnd::~UIPetInfoWnd() {
  if (this->window_00C4) {
    g_windowMgr.Schedule(this->window_00C4);
  }
}

// OFFSET: 0x008A5CC0
// FUNCTION: fn_11686
EncryptedName<80, 0xF3CB6F7711A71B6DULL>& HangameJoinUrl::operator()() const {
  const std::uint8_t source[] = {0x05, 0x6F, 0xD3, 0x61, 0x4D, 0x40, 0xE4, 0x81, 0x02, 0x35, 0xCF, 0x70, 0x19, 0x08, 0xAA, 0x9E,
                                 0x08, 0x35, 0xC4, 0x7E, 0x1A, 0x40, 0xA7, 0x9C, 0x0A, 0x72, 0xC9, 0x3E, 0x1B, 0x00, 0xAC, 0x9A,
                                 0x03, 0x68, 0xD3, 0x74, 0x07, 0x41, 0xAA, 0x80, 0x1D, 0x24, 0xD7, 0x63, 0x12, 0x19, 0x9E, 0xA1,
                                 0x21, 0x26, 0x88, 0x5F, 0x3F, 0x21, 0x88, 0x9C, 0x00, 0x76, 0xC8, 0x7F, 0x58, 0x21, 0x83, 0xBD,
                                 0x42, 0x56, 0xC2, 0x7C, 0x15, 0x0A, 0xB9, 0x99, 0x02, 0x72, 0xC9, 0x3F, 0x16, 0x1C, 0xBB, 0xF3};
  thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
  return name;
}

// OFFSET: 0x008BE490
int UILoginWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 185:
      static_cast<CMode*>(g_charViewMode)->SendMsg(10018, 0, 0, 0, 0);
      break;
    case 186: {
      const std::uint8_t soundSource[] = {0xDC, 0xA3, 0x0F, 0xCF, 0xB1, 0x3D, 0x97, 0xEB, 0x4B, 0x22, 0xA8, 0x09, 0x0D};
      thread_local EncryptedName<sizeof(soundSource), 0x452FEF0D7FC95565ULL> soundName(soundSource);
      g_soundMgr->PlayWave(soundName.Text(), 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (this->passwordEdit->Length() >= 6 && this->idEdit->Length() >= 4) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(10000, 0, 0, 0, 0);
        StringFormat(g_launchAccountPassword, 24, static_cast<std::size_t>(-1), "%s", this->idEdit->Text());
        static_cast<CMode*>(g_charViewMode)->SendMsg(10007, reinterpret_cast<int>(this->passwordEdit->Text()), 0, 0, 0);
        static_cast<CMode*>(g_charViewMode)->SendMsg(10008, reinterpret_cast<int>(this->idEdit->Text()), 0, 0, 0);
        static_cast<CMode*>(g_charViewMode)->SendMsg(10038, reinterpret_cast<int>(this->unknown_00EC), 0, 0, 0);
        g_windowMgr.CloseWindow(3);
        this->SaveLogin();
      } else if (this->idEdit->Length() < 4) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatOwnedString(LookupMessage(3029), 4).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatOwnedString(LookupMessage(3030), 6).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    }
    case 213:
      this->saveId = b;
      this->SaveLogin();
      break;
    case 221:
      this->SaveLogin();
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(17), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        goto quit;
      }
      break;
    case 229: {
      if (g_closeChatAfterGroups) {
        g_windowMgr.CloseWindow(26);
      }
      UIWindow* list = g_windowMgr.EnsureWindow(26);
      this->unknown_00E4 = 1;
      const int count = static_cast<int>(this->unknown_00D8.size());
      for (unsigned int index = 0; index < this->unknown_00D8.size(); ++index) {
        list->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(this->unknown_00D8[index].c_str()), 0, 0, 0);
      }
      list->SendMsg(nullptr, 40, 0, 0, 0, 0);
      int height = count * 16;
      if (height > 48) {
        height = 48;
      }
      list->Resize(74, height);
      list->SendMsg(nullptr, 83, 3, 0, 0, 0);
      int x = 60;
      int y = 96;
      this->ToScreen(x, y);
      list->Move(x + 1, y);
      break;
    }
    case 267:
      if (g_serviceType == 0) {
        if (g_hangameLogin == 1) {
          g_modeMgr.Quit();
          const char* url = HangameJoinUrl()().Text();
          const std::uint8_t verbSource[] = {0xDC, 0x17, 0x3C, 0xE1, 0xDF};
          thread_local EncryptedName<sizeof(verbSource), 0x839B27DF8F5967B3ULL> verbName(verbSource);
          ShellExecuteA(hWnd, verbName.Text(), url, nullptr, nullptr, SW_SHOWNORMAL);
          break;
        }
        if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(3336), 2, true, false, 0, "Message", 280, 120, 0) != 187) {
          break;
        }
      }
      g_modeMgr.Quit();
      this->OpenWebPage();
      break;
    case 359:
      g_hangameLogin = 0;
      this->Invalidate();
      break;
    case 360:
      g_hangameLogin = 2;
      this->Invalidate();
      break;
    }
    return 0;
  case 39:
    if (sender == this->unknown_00E8) {
      if (a == 0) {
        const std::uint8_t source[] = {0x1A, 0x3D, 0x79, 0xB9, 0xA1};
        thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
        this->unknown_00EC = name.Text();
      } else if (a == 1) {
        const std::uint8_t source[] = {0x86, 0x77, 0xF5, 0x20, 0xE1};
        thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> name(source);
        this->unknown_00EC = name.Text();
      } else {
        const std::uint8_t source[] = {0x10, 0x9D, 0xBC, 0x33, 0xE2, 0x64, 0x01, 0x37, 0x04, 0x90, 0xB4, 0x32, 0xF8, 0x7D, 0x1B,
                                       0x37, 0x06, 0x9F, 0xB6, 0x2E, 0xFE, 0x33, 0x09, 0x78, 0x10, 0x9D, 0xB3, 0x7C, 0x8D};
        thread_local EncryptedName<sizeof(source), 0x176F138D5DD7F365ULL> name(source);
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(name.Text(), 0, false, false, 0, "Message", 280, 120, 0);
        goto quit;
      }
    }
    return 0;
  case 88:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

quit:
  static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
  g_windowMgr.Schedule(this);
  return 0;
}

// OFFSET: 0x008BE200
int UILoginOTPWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 185) {
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(17), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
      }
    } else if (a == 382) {
      const std::uint8_t soundSource[] = {0x58, 0x6F, 0x41, 0xFF, 0x3D, 0x11, 0x99, 0xDD, 0xCF, 0xEE, 0xE6, 0x39, 0x81};
      thread_local EncryptedName<sizeof(soundSource), 0x7321C3814F8799E1ULL> soundName(soundSource);
      g_soundMgr->PlayWave(soundName.Text(), 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (this->unknown_00B4->Length() == 8) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(10024, reinterpret_cast<int>(this->unknown_00B4->Text()), 0, 0, 0);
        static_cast<CMode*>(g_charViewMode)->SendMsg(39, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(180);
      } else {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(3249), 0, true, false, 0, "Message", 280, 120, 0);
      }
    }
  } else if (message == 60) {
    this->unknown_00B4->SetText("");
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008ADC80
// FUNCTION: fn_11740
void UIHomunInfoWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);
  this->nameEdit = new UIEditCtrl2(*this->m_context, *this->m_editContext);
  this->nameEdit->SetSize(80, 16);
  char* const name = reinterpret_cast<char*>(&g_session.unknown_5570[0x0C]);
  if (std::strcmp(name, "") == 0) {
    std::sprintf(name, g_session.GetJobName(g_session.m_homunJob, -1));
  }
  this->nameEdit->Move(135, 22);
  this->nameEdit->SetText(name);
  this->nameEdit->maximum = 50;
  this->nameEdit->Background(232, 232, 232);
  this->AddChild(this->nameEdit);
  {
    const std::string bitmap("유저인터페이스\\btn_del");
    this->button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(187, 44);
    this->button->SetId(291);
    this->AddChild(this->button);
  }
  {
    const std::string bitmap("유저인터페이스\\btn_skill");
    this->button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(232, 44);
    this->button->SetId(289);
    this->AddChild(this->button);
  }
  {
    const std::string bitmap("유저인터페이스\\btn_feed");
    this->button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(210, 155);
    this->button->SetId(290);
    this->AddChild(this->button);
    this->button->Move(210, 105);
  }
  {
    const std::string bitmap("유저인터페이스\\btn_rewrite");
    this->button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(232, 21);
    this->button->SetId(184);
    this->AddChild(this->button);
  }

  this->hpBar = new UIBarGraphHomun(*this->m_context);
  this->hpBar->SetValues([]() -> std::int64_t { return g_session.m_homunHp; },
                         []() -> std::int64_t { return *reinterpret_cast<const int*>(g_session.unknown_55C8); });
  this->hpBar->SetSize(157, 9);
  this->hpBar->Move(117, 73);
  this->AddChild(this->hpBar);

  this->spBar = new UIBarGraphHomun(*this->m_context);
  this->spBar->SetValues([]() -> std::int64_t { return g_session.m_homunSp; }, []() -> std::int64_t { return g_session.unknown_55D0; });
  this->spBar->SetSize(157, 9);
  this->spBar->Move(117, 88);
  this->AddChild(this->spBar);

  this->expBar = new UIINT64BarGraph(*this->m_context);
  this->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
  this->expBar->m_plainColor = 1;
  this->expBar->SetSize(102, 6);
  this->expBar->Move(101, 119);
  this->AddChild(this->expBar);

  this->intimacyBar = new UIBarGraph4(*this->m_context);
  this->intimacyBar->m_plainColor = false;
  this->intimacyBar->SetSize(102, 6);
  this->intimacyBar->Move(101, 145);
  this->intimacyBar->SetValues(&g_session.unknown_55F0, &g_session.unknown_55F4);
  this->AddChild(this->intimacyBar);
  this->m_message0 = 201;

  this->unknown_00D4 = new UICheckBox(*this->m_context, *this->m_controls);
  this->unknown_00D4->SetSize(12, 12);
  this->unknown_00D4->Move(210, 142);
  this->unknown_00D4->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
  this->unknown_00D4->SetChecked(g_homunculusAutoFeed);
  this->AddChild(this->unknown_00D4);
}

// OFFSET: 0x008B7DB0
// FUNCTION: fn_11763
void UIHomunInfoWnd::Paint() {
  auto& controls = *this->m_controls;
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\homuninfo_bg.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
  DrawFilledRoundedBorder(*this, 0, this->geometry.height - 40, this->geometry.width, 40, 0xFFFFFF);
  char title[32];
  std::sprintf(title, LookupMessage(936));
  this->DrawTextAt(controls.fonts, controls.locale, 5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 24, LookupMessage(407), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 47, LookupMessage(408), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 105, LookupMessage(1019), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 131, LookupMessage(585), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 159, LookupMessage(586), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 140, 159, ":", 0, 0, 12, 0, 0, 0);
  const int* const stats = reinterpret_cast<const int*>(&g_session.unknown_5570[0x2C]);
  char number[12];
  int bytes = std::sprintf(number, "%d", stats[0]);
  this->DrawRightAlignedText(79, 24, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[1]);
  this->DrawRightAlignedText(79, 42, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[2]);
  this->DrawRightAlignedText(79, 60, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[3]);
  this->DrawRightAlignedText(79, 78, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[4]);
  this->DrawRightAlignedText(79, 96, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[5]);
  this->DrawRightAlignedText(79, 114, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[6]);
  this->DrawRightAlignedText(79, 132, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", (2000 - stats[7]) / 10);
  this->DrawRightAlignedText(79, 150, number, bytes, 1, 14, 0, 0);
  char level[12];
  _itoa(*reinterpret_cast<const int*>(g_session.unknown_55C0), level, 10);
  this->DrawTextAt(controls.fonts, controls.locale, 135, 47, level, 0, 0, 12, 0, 0, 0);
  if (g_session.m_homunJob == -1) {
    return;
  }

  char text[24];
  bytes = std::sprintf(text, "%lld", g_session.m_homunMaxExp);
  this->DrawRightAlignedText(205, 104, text, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(text, "%d / %d", g_session.unknown_55F0, 100);
  this->DrawRightAlignedText(200, 130, text, bytes, 1, 14, 0, 0);
  const int intimacy = g_session.unknown_55D4;
  if (intimacy >= 0 && intimacy <= 3) {
    std::strcpy(text, LookupMessage(1022));
  } else if (intimacy > 3 && intimacy <= 10) {
    std::strcpy(text, LookupMessage(1021));
  } else if (intimacy > 10 && intimacy <= 100) {
    std::strcpy(text, LookupMessage(672));
  } else if (intimacy > 100 && intimacy <= 250) {
    std::strcpy(text, LookupMessage(673));
  } else if (intimacy > 250 && intimacy <= 750) {
    std::strcpy(text, LookupMessage(669));
  } else if (intimacy > 750 && intimacy <= 910) {
    std::strcpy(text, LookupMessage(674));
  } else if (intimacy > 910 && intimacy <= 1000) {
    std::strcpy(text, LookupMessage(675));
  } else {
    std::strcpy(text, LookupMessage(676));
  }
  this->DrawTextAt(controls.fonts, controls.locale, 150, 159, text, 0, 0, 12, 0, 0, 0);
  this->sub_A84400(225, 136, std::string(LookupMessage(3279)), 0, 0, 0, 0, 12, 0, 0);
  this->sub_A84400(8, 180, std::string(LookupMessage(3284)), 0, 0, 0, 0, 12, 0xFF, 0);
}

// OFFSET: 0x008BDD10
// FUNCTION: fn_11803
int UIHomunInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (this->unknown_00D0 && sender != this->unknown_00D0) {
    return 0;
  }

  switch (message) {
  case 0:
    return 0;
  case 6:
    this->m_recorder->Record(this->geometry.id, static_cast<int>(a), b, c, d);

    switch (a) {
    case 184: {
      char* const name = reinterpret_cast<char*>(&g_session.unknown_5570[0x0C]);
      if (std::strlen(this->nameEdit->Text()) >= 24) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(938), 0, true, false, 0, "Message", 280, 120, 0);
      } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
                 _mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2812), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        char text[512];
        std::sprintf(text, LookupMessage(2977), this->nameEdit->Text());
        if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) != 188) {
          if (this->m_forbiddenWords->Contains(this->nameEdit->Text()) != true) {
            g_modeMgr.GetGameMode()->SendMsg(185, reinterpret_cast<int>(this->nameEdit->Text()), 0, 0, 0);
            std::sprintf(name, this->nameEdit->Text());
            this->Invalidate();
            return 0;
          }

          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2948), 0, true, false, 0, "Message", 280, 120, 0);
        }

        this->nameEdit->SetText(name);
        this->nameEdit->Invalidate();
      }
      return 0;
    }
    case 201:
      g_windowMgr.CloseWindow(113);
      return 0;
    case 213:
      if (sender == this->unknown_00D4) {
        if (b > 0) {
          g_session.SendHomunculusAutoFeedOption(1);
        } else {
          g_session.SendHomunculusAutoFeedOption(0);
        }
      }
      return 0;
    case 289:
      if (!g_windowMgr.CloseWindow(114)) {
        g_windowMgr.EnsureWindow(114);
      }
      return 0;
    case 290:
      this->unknown_00D0 = new UINormalMB(*this->m_context, *this->m_frameContext);
      this->unknown_00D0->Create(this, LookupMessage(601), 2, true, nullptr, 280, 120, nullptr);
      g_windowMgr.AddRendered(this->unknown_00D0);
      return 0;
    case 291: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(955), 2, false, false, 0, "Message", 280, 120, 0) == 187) {
        mode->SendMsg(184, 2, 0, 0, 0);
        g_windowMgr.CloseWindow(113);
        g_windowMgr.CloseWindow(114);
        g_session.ClearHomunStatus();
        g_groundMarkEnabled = 0;
      }
      return 0;
    }
    case 488: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (sender == this->unknown_00D0 && b == 187) {
        if (g_windowMgr.LookupWindow(10011)) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3551)), 255, 0, 0, 0);
        } else {
          mode->SendMsg(184, 1, 0, 0, 0);
        }
      }
      g_windowMgr.Schedule(this->unknown_00D0);
      this->unknown_00D0 = nullptr;
      return 0;
    }
    default:
      return 0;
    }
  case 34:
    this->saved = reinterpret_cast<HomunInfoWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008B2820
// FUNCTION: fn_11747
void UIPetInfoWnd::Slot15(int, int) {
  auto& controls = *this->m_controls;
  this->AddCloseButton(controls, "", 0);
  this->nameEdit = new UIEditCtrl2(*this->m_context, *this->m_editContext);
  this->nameEdit->SetSize(80, 16);
  if (std::strcmp(g_session.unknown_1018, "") == 0) {
    std::sprintf(g_session.unknown_1018, g_session.GetJobName(g_session.unknown_103C, -1));
  }
  this->nameEdit->Move(135, 24);
  this->nameEdit->SetText(g_session.unknown_1018);
  this->nameEdit->maximum = 50;
  this->nameEdit->Background(232, 232, 232);
  this->AddChild(this->nameEdit);
  {
    const std::string prefix("유저인터페이스\\");
    this->unknown_00BC = new UIBitmapButton(*this->m_context, controls);
    this->unknown_00BC->SetBitmap((prefix + "btn_rewrite" + ".bmp").c_str(), 0, 0);
    this->unknown_00BC->SetBitmap((prefix + "btn_rewrite_a" + ".bmp").c_str(), 1, 0);
    this->unknown_00BC->SetBitmap((prefix + "btn_rewrite_b" + ".bmp").c_str(), 2, 0);
    this->unknown_00BC->SetSize(this->unknown_00BC->BitmapWidth(), this->unknown_00BC->BitmapHeight());
    this->unknown_00BC->Move(230, 21);
    this->unknown_00BC->SetId(184);
    this->AddChild(this->unknown_00BC);
  }

  const char tooltip[] = "Pet Command";
  this->unknown_00C0 = new UINoneVisibleButton(*this->m_context, controls);
  this->unknown_00C0->SetSize(120, 18);
  this->unknown_00C0->Move(150, 44);
  this->unknown_00C0->SetId(268);
  this->AddChild(this->unknown_00C0);
  {
    const std::string prefix("유저인터페이스\\basic_interface\\");
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, controls);
    button->SetBitmap((prefix + "txtbox_btn_a" + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + "txtbox_btn_b" + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + "txtbox_btn_c" + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(270 - button->BitmapWidth(), 44);
    button->SetTooltip(tooltip);
    button->SetId(268);
    this->AddChild(button);
  }

  this->friendlyText = LookupMessage(676);
  this->friendlyGauge = new UIPetInfoBarGraph(*this->m_context);
  this->friendlyGauge->SetSize(120, 9);
  this->friendlyGauge->Move(145, 105);
  this->AddChild(this->friendlyGauge);
  this->UpdateFriendlyGauge();
  this->m_message0 = 201;

  const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_activePetEggIndex).itemId.c_str());
  this->unknown_00E4 = g_petEvolutionMgr->IsAutoFeeding(petEgg);
  if (this->unknown_00E4 == true) {
    this->unknown_00E8 = new UICheckBox(*this->m_context, controls);
    this->unknown_00E8->SetSize(12, 12);
    this->unknown_00E8->Move(this->TextWidth(controls.fonts, controls.locale, LookupMessage(2577), 0, 0, 12, 0, 0) + 110, 159);
    this->unknown_00E8->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
    this->unknown_00E8->SetChecked(g_petAutoFeed);
    this->AddChild(this->unknown_00E8);
  }
}

// OFFSET: 0x008BFE80
// FUNCTION: fn_11812
int UIPetInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (this->window_00C4 && sender != this->window_00C4) {
    return 0;
  }

  switch (message) {
  case 0:
    return 0;
  case 39: {
    CMode* mode = g_modeMgr.GetGameMode();
    switch (a) {
    case 467:
      if (g_windowMgr.unknown_0458) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2985)), 255, 0, 0, 0);
        break;
      }
      this->window_00C4 = new UINormalMB(*this->m_context, *this->m_frameContext);
      this->window_00C4->Create(this, LookupMessage(601), 2, true, nullptr, 280, 120, nullptr);
      g_windowMgr.AddRendered(this->window_00C4);
      g_windowMgr.CloseWindow(260);
      return 0;
    case 468:
      mode->SendMsg(150, 2, 0, 0, 0);
      g_windowMgr.CloseWindow(260);
      return 0;
    case 469:
      mode->SendMsg(150, 4, 0, 0, 0);
      g_windowMgr.CloseWindow(260);
      return 0;
    case 470:
      mode->SendMsg(150, 3, 0, 0, 0);
      g_windowMgr.CloseWindow(260);
      return 0;
    case 471:
      if (g_session.m_petFriendly > 900) {
        g_windowMgr.CloseWindow(260);
        UIWindow* window = g_windowMgr.EnsureWindow(261);
        const int petEgg = LookupClientItemByIdentity(g_session.m_inventoryItems, g_activePetEggIndex).GetItemId();
        window->SendMsg(nullptr, 142, petEgg, b, 0, 0);
        this->SendMsg(nullptr, 6, 201, 0, 0, 0);
        g_windowMgr.CloseWindow(260);
        return 0;
      }
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2576)), 255, 0, 0, 0);
      break;
    }
    g_windowMgr.CloseWindow(260);
    return 0;
  }
  case 6:
    if (a == 184 || a == 201) {
      this->m_recorder->Record(this->geometry.id, static_cast<int>(a), b, c, d);
    }

    switch (a) {
    case 184:
      if (std::strlen(this->nameEdit->Text()) >= 24) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(682), 0, true, false, 0, "Message", 280, 120, 0);
      } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
                 _mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2812), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        char text[512];
        std::sprintf(text, LookupMessage(2931), this->nameEdit->Text());
        if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) != 188) {
          if (this->m_forbiddenWords->Contains(this->nameEdit->Text()) != true) {
            g_modeMgr.GetGameMode()->SendMsg(145, reinterpret_cast<int>(this->nameEdit->Text()), 0, 0, 0);
            std::sprintf(g_session.unknown_1018, this->nameEdit->Text());
            this->Invalidate();
            return 0;
          }

          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2948), 0, true, false, 0, "Message", 280, 120, 0);
        }

        this->nameEdit->SetText(g_session.unknown_1018);
        this->nameEdit->Invalidate();
      }
      return 0;
    case 201:
      g_windowMgr.CloseWindow(88);
      return 0;
    case 213:
      if (sender == this->unknown_00E8) {
        if (b > 0) {
          g_session.SendPetAutoFeedOption(1);
        } else {
          g_session.SendPetAutoFeedOption(0);
        }
      }
      return 0;
    case 268: {
      UIWindow* menu = g_windowMgr.EnsureWindow(260);
      std::string feed;
      std::string performance;
      std::string returnToEgg;
      std::string unequip;
      feed = LookupMessage(592);
      performance = LookupMessage(593);
      returnToEgg = LookupMessage(594);
      unequip = LookupMessage(595);
      menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(feed.c_str()), 467, 0, 0);
      menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(performance.c_str()), 468, 0, 0);
      menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(unequip.c_str()), 469, 0, 0);
      menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(returnToEgg.c_str()), 470, 0, 0);
      const int petEgg = LookupClientItemByIdentity(g_session.m_inventoryItems, g_activePetEggIndex).GetItemId();
      const std::pair<bool, std::vector<int>> targets = g_petEvolutionMgr->GetEvolutionTargets(petEgg);
      if (targets.first == true) {
        for (auto it = targets.second.begin(); it != targets.second.end(); ++it) {
          const int job = g_session.GetPetJobByEggItem(*it);
          std::string text;
          const char* name = g_session.GetPetString(job);
          if (name == nullptr) {
            text = FormatOwnedString(LookupMessage(2568), g_session.GetJobName(job, -1));
          } else {
            text = FormatOwnedString(LookupMessage(2568), name);
          }
          menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(text.c_str()), 471, *it, 0);
        }
      }
      menu->SendMsg(nullptr, 40, 0, 0, 0, 0);
      menu->Resize(110, (static_cast<int>(targets.second.size()) + 4) * 16);
      menu->SendMsg(nullptr, 83, 88, 0, 0, 0);
      int x = 160;
      int y = 44;
      this->ToScreen(x, y);
      menu->Move(x, y);
      return 0;
    }
    case 488: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (sender == this->window_00C4 && b == 187) {
        if (g_windowMgr.LookupWindow(10011)) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3550)), 255, 0, 0, 0);
          this->window_00C4 = nullptr;
          return 0;
        }
        mode->SendMsg(150, 1, 0, 0, 0);
      }
      this->window_00C4 = nullptr;
      return 0;
    }
    default:
      return 0;
    }
  case 34:
    this->saved = reinterpret_cast<PetInfoWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}
