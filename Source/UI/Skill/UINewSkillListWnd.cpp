#include "Contents/Jobs/JobId.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Skill/UINewSkillListWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIResizer.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Client/Mode/CMode.h"
#include "Utility/Cipher/LocalEncryptedName.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

// INLINED
static int SkillTabOf(int skillId) {
  if (skillId >= NV_BASIC && skillId <= ALL_RESURRECTION) {
    return 0;
  }
  if (skillId >= NV_FIRSTAID && skillId <= MG_ENERGYCOAT) {
    return 0;
  }
  if (skillId >= GS_GLITTERING && skillId <= NJ_ISSEN) {
    return 0;
  }
  if (skillId >= THIRDJOB_BEGIN && skillId <= THIRDJOB_END) {
    return 2;
  }
  if (skillId >= TK_RUN && skillId <= SG_FUSION) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x009B3E70
// FUNCTION: fn_13826
void UINewSkillListWnd::StoreInfo() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009B2C60
// FUNCTION: fn_13821
void UINewSkillListWnd::OnWheel(int delta) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_SKILLDESCRIBEWND);
  if (window && window->geometry.visible) {
    static_cast<UINewSkillListWnd*>(window)->m_scrollBar->OnWheel(delta);
    return;
  }

  if (this->m_scrollable) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta < 0) {
      this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
    } else {
      this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, -step, 0, 0, 0);
    }
  }
}

// OFFSET: 0x009AE730
// FUNCTION: fn_13800
void UINewSkillListWnd::ApplySkillPoints() {
  if (g_session.m_simplifiedSkillList == 0) {
    for (auto it = this->m_plannedSkillLevels.begin(); it != this->m_plannedSkillLevels.end(); ++it) {
      const int id = it->id;
      const int value = it->value;
      CSkillInfo* skill = g_session.m_skillBundle.FindGrouped(id);
      if (skill != nullptr && skill->available != 0) {
        for (int level = skill->learnedLevel + 1; level <= value; ++level) {
          g_modeMgr.gameMode->SendMsg(MM_UPGRADE_SKILLLEVEL, id, 0, 0, 0);
        }
      }
    }
    this->m_plannedSkillLevels.clear();
  } else {
    std::list<CSkillInfo>& live = this->SkillList(this->m_tab, true);
    std::list<CSkillInfo>& pending = this->SkillList(this->m_tab, false);
    auto current = live.begin();
    for (auto it = pending.begin(); it != pending.end(); ++it, ++current) {
      if (current->available != 0 && current->id == it->id && current->level < it->level) {
        for (int i = 0; i < it->level - current->level; ++i) {
          g_modeMgr.gameMode->SendMsg(MM_UPGRADE_SKILLLEVEL, current->id, 0, 0, 0);
        }
      }
    }
  }

  this->m_locked = 0;
  this->LayoutButtons();
}

// GLOBAL: gl_25138
int g_skillListHeadOffsetX;
// GLOBAL: gl_25139
int g_skillListHeadOffsetY;

// OFFSET: 0x009AE930
// FUNCTION: fn_13801
void UINewSkillListWnd::DrawCharacter() {
  std::string bodyImf;
  std::string actions[2];
  std::string sprites[2];
  std::string bodyPal;
  std::string headPal;
  SpriteAtlasRecord record{};

  const int job = g_session.m_job;
  const int sex = g_session.GetCurrentSex();
  int head = g_session.m_head;
  const int bodyPalette = g_session.m_bodyStyle;
  const int headPalette = g_session.m_headPalette;

  actions[0].assign(static_cast<const std::string&>(GetBodyActionResourceName(job, sex, g_session.m_bodyCostume, 0)));
  sprites[0].assign(static_cast<const std::string&>(g_session.GetBodySprName(job, sex, g_session.m_bodyCostume, 0)));
  actions[1].assign(static_cast<const std::string&>(g_session.GetHeadActName(job, sex, &head)));
  sprites[1].assign(static_cast<const std::string&>(g_session.GetHeadSprName(job, sex, &head)));
  if (bodyPalette != 0) {
    bodyPal.assign(static_cast<const std::string&>(g_session.GetBodyPalName(job, sex, bodyPalette, g_session.m_bodyCostume, 0, 0)));
  }
  if (headPalette != 0) {
    headPal.assign(static_cast<const std::string&>(g_session.GetHeadPalName(head, job, sex, headPalette)));
  }
  bodyImf = GetBodyImfName(job, sex, head, g_session.m_bodyCostume);

  for (int layer = 0; layer < 2; ++layer) {
    ResourceMgr::Instance().Load(bodyImf.c_str());
    if (actions[layer].size() == 0 || sprites[layer].size() == 0) {
      continue;
    }

    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actions[layer].c_str()));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprites[layer].c_str()));
    const int frameCount = static_cast<int>(action->FrameCount(23));
    int frame = static_cast<int>(static_cast<float>(timeGetTime() - this->m_lastTick) / 300.0f) / frameCount;
    if (frame >= 3) {
      frame = 0;
      this->m_lastTick = timeGetTime();
    }

    const ActionFrame& actionFrame = action->Frame(23, frame);
    int offsetX = 0;
    int offsetY = 0;
    if (g_session.IsDoramJob(job) == true) {
      int view = 0;
      if (layer == 4) {
        view = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0]);
      } else if (layer == 3) {
        view = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[4]);
      } else if (layer == 2) {
        view = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[8]);
      }
      int x = 0;
      int y = 0;
      g_session.m_hotkeyScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", view, 23, g_session.GetCurrentSex(), &x, &y);
      offsetX = x;
      offsetY = y;
    }

    const std::uint32_t* palette = sprite->palette;
    if (layer == 1) {
      CActRes* bodyAction = static_cast<CActRes*>(ResourceMgr::Instance().Load(actions[0].c_str()));
      const ActionFrame& bodyFrame = bodyAction->Frame(23, frame);
      if (actionFrame.anchorCount != 0 && bodyFrame.anchorCount != 0 && actionFrame.anchors[0].attachmentId == bodyFrame.anchors[0].attachmentId) {
        offsetX += bodyFrame.anchors[0].x - actionFrame.anchors[0].x;
        offsetY += bodyFrame.anchors[0].y - actionFrame.anchors[0].y;
        g_skillListHeadOffsetX = offsetX;
        g_skillListHeadOffsetY = offsetY;
      }
    }
    if (layer == 0) {
      if (bodyPalette != 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(bodyPal.c_str()))->palette;
      }
    } else if (layer == 1) {
      if (headPalette != 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(headPal.c_str()))->palette;
      }
    }

    const SpriteFrameCell& cell = actionFrame.Cell(layer > 1 ? 0 : layer);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }

    const int y = offsetY - 25 + cell.y + this->geometry.height;
    const int clipY = y < 0 ? y : 0;
    this->geometry.surface->DrawSurface(
      cell.x + offsetX + 48, y < 0 ? 0 : y, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f - clipY), image->width,
      image->height + clipY, cell.mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 0, 0
    );
  }
}

// OFFSET: 0x009AF300
// FUNCTION: fn_13802
void UINewSkillListWnd::DrawSkillList() {
  LuaScript& script = *g_session.m_hotkeyScript;
  int drawn = 0;
  int index = 0;

  for (const CSkillInfo& skill : this->SkillList(this->m_tab, false)) {
    if (index >= this->m_firstRow * this->m_columns) {
      const int level = skill.level;
      const int y = drawn / this->m_columns * 32;
      const int x = drawn % this->m_columns * 32;

      if (this->m_selectedSkill == index) {
        unsigned red, green, blue;
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
        if (skill.level == 0) {
          SkinPaletteChannels(ResourceMgr::Instance(), 10, 2, red, green, blue);
        } else if (skill.spCost == 0) {
          SkinPaletteChannels(ResourceMgr::Instance(), 14, 2, red, green, blue);
        }
        this->FillRegion(x + 104, y + 32, 164, 28, RGB(red, green, blue));
      }

      int shade = 0;
      char text[80];
      g_session.ItemBitmapPath(skill.name, text);
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      if (!bitmap) {
        std::sprintf(text, "유저인터페이스\\item\\am_berserkpitcher.bmp");
        bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      }
      if (bitmap) {
        if (level == 0) {
          shade = 120;
          this->DrawAuxiliary(1, x + 44, y + 34, &bitmap->image, true, false);
        } else {
          this->DrawBitmap(x + 44, y + 34, &bitmap->image, true);
        }
      }

      const int maxLevel = this->SkillAt(index, true).level;
      const int red = level > maxLevel ? 255 : shade;
      const int green = level > maxLevel ? 0 : shade;
      const int textX = x + 112;
      const int textY = y + 47;
      const bool usesLevel = CSkillInfo::UsesLevel(script, static_cast<int>(skill.id));
      if (level != 0) {
        std::sprintf(text, g_serviceType == 12 ? "Nv :" : "Lv :");
        const int labelX = this->m_locked != 0 || !usesLevel ? textX + 1 : textX + 12;
        const std::uint32_t gray = RGB(shade, shade, shade);
        this->DrawTextAt(labelX, textY, text, 0, 0, 12, gray, 0, 0);

        int levelX;
        if (usesLevel) {
          int useLevel = g_session.UseLevel(static_cast<int>(skill.id));
          if (useLevel <= 0 || useLevel > skill.level) {
            useLevel = skill.level;
          }
          levelX = (this->m_locked == 0 ? 37 : 25) + textX;
          std::sprintf(text, "%2d", useLevel);
          this->DrawTextAt(levelX, textY, text, 0, 0, 12, useLevel == skill.level ? gray : 255, 0, 0);
          levelX += 7 + this->TextWidth(text, 0, 0, 12, 0, 0);
          this->DrawTextAt(levelX, textY, "/ ", 0, 0, 12, gray, 0, 0);
          levelX += this->TextWidth("/ ", 0, 0, 12, 0, 0);
          std::sprintf(text, "%2d", skill.level);
        } else {
          std::sprintf(text, "%d", skill.level);
          levelX = textX + 25;
        }
        this->DrawTextAt(levelX, textY, text, 0, 0, 12, RGB(green, green, red), 0, 0);

        if (skill.type == 0) {
          std::sprintf(text, LookupMessage(MSI_PASSIVE));
        } else {
          int sp = static_cast<int>(skill.spCost);
          const int useLevel = g_session.UseLevel(static_cast<int>(skill.id));
          bool found = false;
          if (usesLevel) {
            const std::uint32_t id = skill.id;
            for (const CSkillInfo& live : this->SkillList(this->m_tab, true)) {
              if (live.id == id && live.level != 0) {
                const int amount = skill.UseSpAmount(script, useLevel);
                if (amount != 0 && useLevel <= skill.level) {
                  sp = amount;
                }
                found = true;
                break;
              }
            }
          }
          if (!found) {
            sp = skill.UseSpAmount(script, skill.level);
          }

          const int ap = skill.UseApAmount(script, useLevel > 0 ? useLevel : skill.level);
          if (SkillUsesPercentSp(static_cast<int>(skill.id))) {
            if (ap != 0) {
              std::sprintf(text, "Sp : %d%%, Ap : %d", sp, ap);
            } else {
              std::sprintf(text, "Sp : %d%%", sp);
            }
          } else if (ap != 0) {
            std::sprintf(text, "Sp : %d, Ap : %d", sp, ap);
          } else {
            std::sprintf(text, "Sp : %d", sp);
          }
        }
        this->DrawTextAt(textX + 100, textY, text, 0, 0, 12, gray, 0, 0);
      }

      this->DrawTextAt(textX, y + 35, skill.DisplayName(script), 0, 0, 12, RGB(shade, shade, shade), 0, 0);
      if (++drawn == this->m_visibleRows * this->m_columns) {
        break;
      }
    }
    ++index;
  }
}

// OFFSET: 0x009B0370
// FUNCTION: fn_13805
std::string UINewSkillListWnd::ValueText(int id) {
  for (const NewSkillListValue& entry : this->m_values) {
    if (entry.id == id) {
      if (entry.value > 0) {
        char text[12];
        _itoa(entry.value, text, 10);
        return text;
      }

      break;
    }
  }

  return std::string("");
}

// OFFSET: 0x009B0460
// FUNCTION: fn_13806
CSkillInfo UINewSkillListWnd::SkillAt(int index, bool live) {
  int sequentialIndex = 0;
  CSkillInfo result(0);
  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, live);
  std::list<CSkillInfo>::iterator it = list.begin();
  while (it != list.end()) {
    int key = it->treePosition;
    if (g_session.m_simplifiedSkillList != 0) {
      key = sequentialIndex;
    }
    if (key == index) {
      result = *it;
      break;
    }
    ++it;
    if (g_session.m_simplifiedSkillList != 0) {
      ++sequentialIndex;
    }
  }
  return result;
}

// OFFSET: 0x009B0680
// FUNCTION: fn_13807
CSkillInfo UINewSkillListWnd::VisibleSkill(int row) {
  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
  int index = 0;
  for (std::list<CSkillInfo>::iterator it = list.begin(); it != list.end(); ++it) {
    if (row + this->m_firstRow == index) {
      return *it;
    }
    ++index;
  }
  return CSkillInfo(0);
}

// OFFSET: 0x009B0750
// FUNCTION: fn_13808
std::optional<CSkillInfo*> UINewSkillListWnd::SkillPtr(int index, bool live) {
  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, live);
  if (g_session.m_simplifiedSkillList != 0) {
    if (index >= 0 && static_cast<size_t>(index) < list.size()) {
      auto it = list.begin();
      std::advance(it, index);
      return &*it;
    }
    return std::nullopt;
  }

  for (auto it = list.begin(); it != list.end(); ++it) {
    if (it->treePosition == index) {
      return &*it;
    }
  }

  return std::nullopt;
}

// OFFSET: 0x009B07C0
// FUNCTION: fn_13809
std::list<CSkillInfo>& UINewSkillListWnd::SkillList(int tab, bool live) {
  if (tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab) {
    return g_session.m_skillBundle.extra;
  }

  if (live) {
    return g_session.m_skillBundle.Group(tab);
  }

  return this->m_lists[tab];
}

// OFFSET: 0x009B0820
// FUNCTION: fn_13810
void UINewSkillListWnd::InitTabs() {
  const int initialTabCount = this->m_tabs->labels.size();
  const std::uint8_t source[23] = {
    0xEF, 0xF2, 0x0F, 0xAA, 0x08, 0x4E, 0x1B, 0x6F, 0xF1, 0xFC, 0x0F, 0xA6,
    0x24, 0x42, 0x03, 0x57, 0xC4, 0xFF, 0x23, 0x98, 0x0E, 0x42, 0x77
  };
  thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> funcName(source);
  const char* func = funcName.Text();

  const char* defaultTabs[4] = {};
  const char* jobTabs[4] = {};
  g_session.m_hotkeyScript->Call(func, "d>ssss", 0, &defaultTabs[0], &defaultTabs[1], &defaultTabs[2], &defaultTabs[3]);

  const int baseJob = JobNames::BaseJob(g_session.GetDisplayJob());
  int jobKey = baseJob;
  switch (baseJob) {
  case JT_NOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
    jobKey = 0;
    break;
  case JT_SWORDMAN:
  case JT_SWORDMAN_H:
  case JT_SWORDMAN_B:
    jobKey = 1;
    break;
  case JT_MAGICIAN:
  case JT_MAGICIAN_H:
  case JT_MAGICIAN_B:
    jobKey = 2;
    break;
  case JT_ARCHER:
  case JT_ARCHER_H:
  case JT_ARCHER_B:
    jobKey = 3;
    break;
  case JT_ACOLYTE:
  case JT_ACOLYTE_H:
  case JT_ACOLYTE_B:
    jobKey = 4;
    break;
  case JT_MERCHANT:
  case JT_MERCHANT_H:
  case JT_MERCHANT_B:
    jobKey = 5;
    break;
  case JT_THIEF:
  case JT_THIEF_H:
  case JT_THIEF_B:
    jobKey = 6;
    break;
  case JT_KNIGHT:
  case JT_KNIGHT_H:
  case JT_KNIGHT_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
  case JT_DRAGON_KNIGHT:
    jobKey = 7;
    break;
  case JT_PRIEST:
  case JT_PRIEST_H:
  case JT_PRIEST_B:
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    jobKey = 8;
    break;
  case JT_WIZARD:
  case JT_WIZARD_H:
  case JT_WIZARD_B:
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_ARCH_MAGE:
    jobKey = 9;
    break;
  case JT_BLACKSMITH:
  case JT_BLACKSMITH_H:
  case JT_BLACKSMITH_B:
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
  case JT_MEISTER:
    jobKey = 10;
    break;
  case JT_HUNTER:
  case JT_HUNTER_H:
  case JT_HUNTER_B:
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
  case JT_WINDHAWK:
    jobKey = 11;
    break;
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    jobKey = 12;
    break;
  case JT_CRUSADER:
  case JT_CRUSADER_H:
  case JT_CRUSADER_B:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_GUARD_B:
  case JT_IMPERIAL_GUARD:
    jobKey = 14;
    break;
  case JT_MONK:
  case JT_MONK_H:
  case JT_MONK_B:
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    jobKey = 15;
    break;
  case JT_SAGE:
  case JT_SAGE_H:
  case JT_SAGE_B:
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    jobKey = 16;
    break;
  case JT_ROGUE:
  case JT_ROGUE_H:
  case JT_ROGUE_B:
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
  case JT_ABYSS_CHASER:
    jobKey = 17;
    break;
  case JT_ALCHEMIST:
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    jobKey = 18;
    break;
  case JT_BARD:
  case JT_BARD_H:
  case JT_BARD_B:
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_TROUBADOUR:
    jobKey = 19;
    break;
  case JT_DANCER:
  case JT_DANCER_H:
  case JT_DANCER_B:
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
  case JT_TROUVERE:
    jobKey = 20;
    break;
  case JT_SUPERNOVICE:
  case JT_SUPERNOVICE_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    jobKey = 23;
    break;
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_GUNSLINGER_B:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    jobKey = 24;
    break;
  case JT_NINJA:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_NINJA_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    jobKey = 25;
    break;
  case JT_TAEKWON:
  case JT_TAEKWON_B:
    jobKey = 4046;
    break;
  case JT_STAR:
  case JT_STAR_B:
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
  case JT_SKY_EMPEROR:
    jobKey = 4047;
    break;
  case JT_LINKER:
  case JT_LINKER_B:
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_SOUL_ASCETIC:
    jobKey = 4049;
    break;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
    jobKey = 4218;
    break;
  case JT_DRUID:
  case JT_DRUID_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_ALITEA:
    jobKey = 4351;
    break;
  default:
    break;
  }

  g_session.m_hotkeyScript->Call(func, "d>ssss", jobKey, &jobTabs[0], &jobTabs[1], &jobTabs[2], &jobTabs[3]);

  for (int i = this->m_tabs->labels.size(); i < 4; ++i) {
    if (g_session.m_skillBundle.GroupSize(i) > 0) {
      const char* name = jobTabs[i];
      if (!name) {
        name = defaultTabs[i];
      }
      this->m_tabs->AddTab(name, "");
      this->m_tab = i;
    }
  }

  if (initialTabCount == 0) {
    this->m_tabs->AddTab(LookupMessage(MSI_ETC), "");
  }
  this->m_tabs->activeTabIndex = this->m_tab;
}

// OFFSET: 0x009B0E40
// FUNCTION: fn_13811
void UINewSkillListWnd::ResetLists() {
  this->m_locked = 0;
  this->m_skillPoints = g_session.m_skillPoint;
  for (unsigned i = 0; i < 4; ++i) {
    this->m_lists[i] = g_session.m_skillBundle.Group(i);
  }
  this->m_plannedSkillLevels.clear();
  g_session.m_skillBundle.MarkLevelChanges();
}

// OFFSET: 0x009B0EE0
// FUNCTION: fn_13812
bool UINewSkillListWnd::IsExtraTab(int tab) {
  return tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab;
}

// OFFSET: 0x009B0FA0
// FUNCTION: fn_13813
bool UINewSkillListWnd::IsSkillArea(int x, int y) {
  int minX = 76;
  int rowHeight = 56;
  int yOffset = 40;
  bool inside = true;
  if (g_session.m_simplifiedSkillList != 0) {
    rowHeight = 32;
    minX = 40;
    yOffset = 30;
  }
  if (y < this->m_topMargin || y >= this->m_visibleRows * rowHeight + 4 + yOffset) {
    inside = false;
  }
  if (x < minX) {
    inside = false;
  }
  if (g_session.m_simplifiedSkillList == 0 && x > 504) {
    inside = false;
  }
  return inside;
}

// OFFSET: 0x009B1050
// FUNCTION: fn_13814
void UINewSkillListWnd::OnCreate(int width, int height) {
  this->m_tabs = new UITabControl();
  this->m_tabs->Configure(0, 17, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->m_tabs->Move(0, 6);
  this->AddChild(this->m_tabs);

  {
    std::string minimizeTooltip("");
    this->m_btnHelp = this->AddBaseButton();
    this->m_btnMinimize = this->AddMinimizeButton(minimizeTooltip.c_str(), 0);
    this->m_btnClose = this->AddCloseButton("Alt+S", 0);

    int titleButtonX[3] = {22, this->geometry.width - 30, this->geometry.width - 16};
    UIBitmapButton** titleButtons = &this->m_btnHelp;
    for (int i = 0; i < 3; ++i) {
      titleButtons[i]->Move(titleButtonX[i], 5);
    }
  }

  this->m_scrollBar = new UIScrollBar();
  this->m_scrollBar->Configure(this->geometry.width - 15, 20, 1, this->geometry.height - this->m_topMargin - this->m_bottomMargin);
  this->m_scrollBar->Move(this->geometry.width - 15, 20);
  this->m_scrollBar->Resize(this->m_scrollBar->geometry.width, this->geometry.height - this->m_topMargin - this->m_bottomMargin);
  this->AddChild(this->m_scrollBar);

  {
    std::string normal = "유저인터페이스\\basic_interface\\skill_up_a.bmp";
    std::string hover = "유저인터페이스\\basic_interface\\skill_up_b.bmp";
    std::string pressed = "유저인터페이스\\basic_interface\\skill_up_c.bmp";
    for (int i = 0; i < 10; ++i) {
      this->m_btnLevelUp[i] = new UIBitmapButton();
      this->m_btnLevelUp[i]->SetBitmap(normal.c_str(), 0, 0);
      this->m_btnLevelUp[i]->SetBitmap(hover.c_str(), 1, 0);
      this->m_btnLevelUp[i]->SetBitmap(pressed.c_str(), 2, 0);
      this->m_btnLevelUp[i]->SetSize(this->m_btnLevelUp[i]->BitmapWidth(), this->m_btnLevelUp[i]->BitmapHeight());
      this->m_btnLevelUp[i]->Move(78, 21 + i * 32);
      this->m_btnLevelUp[i]->SetId(i + 239);
      this->AddChild(this->m_btnLevelUp[i]);
    }
  }
  {
    std::string left = "유저인터페이스\\basic_interface\\arw_left.bmp";
    for (int i = 0; i < 42; ++i) {
      this->m_btnSkills[i] = new UIBitmapButton();
      this->m_btnSkills[i]->SetBitmap(left.c_str(), 0, 0);
      this->m_btnSkills[i]->SetBitmap(left.c_str(), 1, 0);
      this->m_btnSkills[i]->SetBitmap(left.c_str(), 2, 0);
      this->m_btnSkills[i]->SetSize(this->m_btnSkills[i]->BitmapWidth(), this->m_btnSkills[i]->BitmapHeight());
      this->m_btnSkills[i]->Move(106, 34 + i * 32);
      this->m_btnSkills[i]->SetId(i);
      this->AddChild(this->m_btnSkills[i]);
    }
  }
  {
    std::string right = "유저인터페이스\\basic_interface\\arw_right.bmp";
    for (int i = 0; i < 42; ++i) {
      this->m_btnSkillUps[i] = new UIBitmapButton();
      this->m_btnSkillUps[i]->SetBitmap(right.c_str(), 0, 0);
      this->m_btnSkillUps[i]->SetBitmap(right.c_str(), 1, 0);
      this->m_btnSkillUps[i]->SetBitmap(right.c_str(), 2, 0);
      this->m_btnSkillUps[i]->SetSize(this->m_btnSkillUps[i]->BitmapWidth(), this->m_btnSkillUps[i]->BitmapHeight());
      this->m_btnSkillUps[i]->Move(198, 34 + i * 32);
      this->m_btnSkillUps[i]->SetId(i + 42);
      this->AddChild(this->m_btnSkillUps[i]);
    }
  }

  {
    std::string baseDir("유저인터페이스\\");
    const char* buttonNames[2][3] = {
      {"btn_apply", "btn_apply_a", "btn_apply_b"},
      {"btn_reset", "btn_reset_a", "btn_reset_b"},
    };
    POINT buttonPos[2] = {
      {this->geometry.width - 100, this->geometry.height - 27},
      {this->geometry.width - 50, this->geometry.height - 27},
    };
    int buttonIds[2] = {271, 363};
    UIBitmapButton** buttons = &this->m_btnApply;
    for (int i = 0; i < 2; ++i) {
      buttons[i] = new UIBitmapButton();
      buttons[i]->SetBitmap((baseDir + buttonNames[i][0] + ".bmp").c_str(), 0, 0);
      buttons[i]->SetBitmap((baseDir + buttonNames[i][1] + ".bmp").c_str(), 1, 0);
      buttons[i]->SetBitmap((baseDir + buttonNames[i][2] + ".bmp").c_str(), 2, 0);
      buttons[i]->SetSize(buttons[i]->BitmapWidth(), buttons[i]->BitmapHeight());
      buttons[i]->Move(buttonPos[i].x, buttonPos[i].y);
      buttons[i]->SetId(buttonIds[i]);
      buttons[i]->SetDisabled(1);
      buttons[i]->Invalidate();
      this->AddChild(buttons[i]);
    }
  }

  this->m_btnDetail = new UIResizer();
  this->m_btnDetail->SetSize(16, 16);
  this->m_btnDetail->SetDirection(3);
  this->m_btnDetail->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->m_btnDetail->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->m_btnDetail->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->m_btnDetail);

  this->InitTabs();
  if (!g_session.m_skillBundle.extraIds.empty()) {
    this->m_tabs->SetFlag(static_cast<int>(this->m_tabs->labels.size()) - 1, true);
  }
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x009B2160
// FUNCTION: fn_13815
void UINewSkillListWnd::Paint() {
  const int left = this->m_tabs->geometry.width;
  this->ClearSurface(0xFFFF00FF);
  this->FillRegion(left, this->m_topMargin, this->geometry.width - left, this->geometry.height - this->m_bottomMargin - this->m_topMargin, 0xFFFFFF);
  DrawRoundedBorder(*this, left, 0, this->geometry.width - left, this->geometry.height, 0xC3C3C3);
  DrawRoundedBorder(*this, left + 2, 2, this->geometry.width - left - 4, this->geometry.height - 4, 0xC3C3C3);

  CBitmapRes* bitmap =
    static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_upbar_left.bmp")));
  this->DrawBitmap(left, 0, bitmap, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_upbar_right.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, 0, bitmap, false);

  int span = this->geometry.width - left - 24;
  int count = span / 5 + (span % 5 != 0);
  for (int i = 0, x = left + 12; i < count; ++i, x += 5) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_upbar_mid.bmp")));
    this->DrawBitmap(x, 0, bitmap, false);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_botbar_left.bmp")));
  this->DrawBitmap(left, this->geometry.height - bitmap->image.height, bitmap, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_botbar_right.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, this->geometry.height - bitmap->image.height, bitmap, false);

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_botbar_mid.bmp")));
  span = this->geometry.width - left - 24;
  count = span / bitmap->image.width + (span % bitmap->image.width != 0);
  for (int i = 0, x = left + 12; i < count; ++i, x += 5) {
    this->DrawBitmap(x, this->geometry.height - bitmap->image.height, bitmap, false);
  }

  this->DrawTextAt(41, 6, LookupMessage(MSI_SKILLLIST), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(40, 5, LookupMessage(MSI_SKILLLIST), 0, 0, 12, 0, 0, 0);

  const int points = this->m_skillPoints;
  char text[80];
  std::memset(text, 0, sizeof(text));
  if (g_session.m_simplifiedSkillList != 0) {
    std::sprintf(text, LookupMessage(MSI_SKILLPOINT), points);
    this->TextWidth(text, 0, 0, 12, 0, 0);
    this->DrawTextAt(30, this->geometry.height - 23, text, 0, 0, 12, 0, 0, 0);
    this->DrawSkillList();
  } else {
    const int total = g_session.m_skillPoint;
    if (points < total) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%s^0000ff%d^000000(^ff0000%d^000000) / %d", LookupMessage(MSI_REFUSE_CHANGE_PASSWD_SITE), points, total - points,
                   total);
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%s%d", LookupMessage(MSI_REFUSE_CHANGE_PASSWD_SITE), total);
    }
    this->DrawWrappedFormattedText(80, this->geometry.height - 23, std::string(text), 0, 0, 0, 0, 12, 0, 0);
    this->DrawSkillTree();
    this->DrawCharacter();
  }

  this->m_needsRedraw = 0;
}

// OFFSET: 0x009B2590
// FUNCTION: fn_13816
void UINewSkillListWnd::OnLBtnDblClk(int x, int y) {
  if (g_session.GetOption(5) > 0) {
    return;
  }

  if (y < this->m_topMargin) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (this->m_selectedSkill == -1) {
    return;
  }

  CSkillInfo skill = this->SkillAt(this->m_selectedSkill, false);
  if (skill.available != 0 && skill.level != 0) {
    AutomaticSkills& automatic = g_session.m_automaticSkills;
    if (automatic.Contains(skill.id)) {
      g_session.SetOption(automatic.CancelOption(skill.id), 1, 0);
    } else {
      automatic.DisableAll();
    }
    this->SendMsg(nullptr, UM_USESKILL, reinterpret_cast<std::intptr_t>(&skill), 0, 0, 0);
  }
}

// OFFSET: 0x009B26D0
// FUNCTION: fn_13817
void UINewSkillListWnd::OnLBtnDown(int x, int y) {
  if (y < this->m_topMargin) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (this->m_showDetail) {
    this->m_showDetail = false;
    this->UpdateRequiredSkills();
  }

  int oldSelected = this->m_selectedSkill;
  this->m_selectedSkill = this->SkillIndexAt(x, y);
  if (oldSelected != this->m_selectedSkill) {
    this->Invalidate();
  }

  if (this->IsSkillArea(x, y) && !this->m_locked && this->m_selectedSkill != -1 && g_replayAnimationScaling == 0) {
    CSkillInfo skill = this->SkillAt(this->m_selectedSkill, false);
    if (skill.available != 0 && skill.level != 0 && skill.type != 0) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (gameMode) {
        int level = g_session.UseLevel(skill.id);
        if (level == 0) {
          level = skill.level;
        }
        gameMode->drag.BeginSkill(g_windowMgr, ResourceMgr::Instance(), 8, skill.id, skill.name, level, -1);
      }
    }
  }
}

// OFFSET: 0x009B2810
// FUNCTION: fn_13818
void UINewSkillListWnd::OnLBtnUp(int x, int y) {
  if (y < this->m_topMargin) {
    UIFrameWnd::OnLBtnUp(x, y);
    this->StoreInfo();
    return;
  }

  const int gate = g_session.m_simplifiedSkillList;
  if (gate == 0 && this->IsSkillArea(x, y)) {
    CSkillInfo skill = this->SkillAt(this->m_selectedSkill, false);
    if (skill.canPlanLevel != 0 && skill.available != 0) {
      if (this->IncreaseSkillLevel(skill.id)) {
        this->m_needsRedraw = 1;
        this->m_locked = 1;
        this->LayoutButtons();
        this->Invalidate();
      }
    }
  }
}

// OFFSET: 0x009B28F0
// FUNCTION: fn_13819
void UINewSkillListWnd::OnMouseHover(int x, int y) {
  const int gate = g_session.m_simplifiedSkillList;
  if (!this->IsSkillArea(x, y) || (gate != 0 && x > 96)) {
    return;
  }

  const int previous = this->m_selectedSkill;
  this->m_selectedSkill = this->SkillIndexAt(x, y);
  if (this->m_selectedSkill != -1) {
    std::optional<CSkillInfo*> skill = this->SkillPtr(this->m_selectedSkill, false);
    if (skill.has_value() && (*skill)->available != 0) {
      int tooltipX = -10;
      int tooltipY = -20;
      this->ToScreen(tooltipX, tooltipY);
      const char* name = (*skill)->DisplayName(*g_session.m_hotkeyScript);
      g_modeMgr.gameMode->ShowTooltip(name, tooltipX + x, tooltipY + y, -1, 0, 1);
    }
  }

  if (previous != this->m_selectedSkill) {
    if (!this->m_showDetail) {
      this->UpdateRequiredSkills();
    }
    this->m_needsRedraw = 1;
    this->Invalidate();
  }
}

// OFFSET: 0x009B29E0
// FUNCTION: fn_13820
void UINewSkillListWnd::OnRBtnUp(int x, int y) {
  this->m_selectedSkill = this->SkillIndexAt(x, y);
  if (this->m_selectedSkill == -1) {
    this->m_showDetail = false;
    this->m_values.clear();
    for (int tab = 0; tab < static_cast<int>(this->m_tabs->labels.size()); ++tab) {
      UITabControl* tabs = this->m_tabs;
      tabs->colors[tab] = 0xFFFFFFFF;
      tabs->Invalidate();
    }
  } else if (this->m_detailId != this->m_selectedSkill) {
    this->m_showDetail = true;
    this->UpdateRequiredSkills();
  } else {
    this->m_showDetail = !this->m_showDetail;
  }

  if (this->m_showDetail) {
    this->m_detailId = this->m_selectedSkill;
  }

  this->m_needsRedraw = 1;
  this->Invalidate();

  const std::optional<CSkillInfo*> selected = this->SkillPtr(this->m_selectedSkill, false);
  if (!selected.has_value()) {
    return;
  }

  const CSkillInfo& skill = **selected;
  UISkillDescribeWnd* describe = static_cast<UISkillDescribeWnd*>(g_windowMgr.LookupWindow(WID_SKILLDESCRIBEWND));
  if (describe && (describe->m_skillId == static_cast<int>(skill.id) || !skill.available)) {
    g_windowMgr.CloseWindow(WID_SKILLDESCRIBEWND);
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_SKILLDESCRIBEWND);
  if (!window) {
    return;
  }

  window->SendMsg(nullptr, UM_SKILLDESCRIBE, skill.id, skill.upgradable, skill.level, 1);

  int originX = 0;
  int originY = 0;
  this->ToScreen(originX, originY);
  int left = x + originX + 24;
  const int bottom = y + originY;
  if (left + window->geometry.width > g_renderer->viewport.width) {
    left -= window->geometry.width + 48;
  }

  int top = bottom + window->geometry.height > g_renderer->viewport.height ? bottom - window->geometry.height : bottom;
  if (top < 0) {
    top = 0;
  }

  window->Move(left, top);
  g_windowMgr.ShowWindowById(WID_SKILLDESCRIBEWND, 1);
}

// OFFSET: 0x009B2CD0
// FUNCTION: fn_13822
void UINewSkillListWnd::UpdateRequiredSkills() {
  this->m_values.clear();
  for (int i = 0; i < static_cast<int>(this->m_tabs->labels.size()); ++i) {
    UITabControl* tabs = this->m_tabs;
    tabs->colors[i] = 0xFFFFFFFF;
    tabs->Invalidate();
  }

  for (CSkillInfo& skill : this->SkillList(this->m_tab, false)) {
    const int key = static_cast<int>(skill.treePosition);
    if (key != -1 && this->m_firstRow * this->m_columns <= key && key == this->m_selectedSkill) {
      this->m_values.push_back({static_cast<int>(skill.id), 0});
      for (std::size_t i = 0; i < skill.entries.size(); ++i) {
        const int id = static_cast<int>(skill.entries[i].skillId);
        const int tab = SkillTabOf(id);
        if (tab != this->m_tab) {
          UITabControl* tabs = this->m_tabs;
          if (tab < static_cast<int>(tabs->labels.size())) {
            tabs->colors[tab] = 0xE1E7FC;
            tabs->Invalidate();
          }
        }

        int level = static_cast<int>(skill.entries[i].requiredLevel);
        for (auto it = this->m_values.begin(); it != this->m_values.end(); ++it) {
          if (it->id == id) {
            level = (std::max)(level, it->value);
            this->m_values.erase(it);
            break;
          }
        }
        skill.entries[i].requiredLevel = level;
        const SkillInfoEntry& entry = skill.entries[i];
        this->m_values.push_back({static_cast<int>(entry.skillId), static_cast<int>(entry.requiredLevel)});
      }
      return;
    }
  }
}

// OFFSET: 0x009B3040
// FUNCTION: fn_13823
void UINewSkillListWnd::UpdateScrollBar() {
  if (g_session.m_simplifiedSkillList != 0) {
    this->m_columns = 1;
    this->m_visibleRows = (this->geometry.height - this->m_bottomMargin - 30) / 32;
  } else {
    this->m_columns = 7;
    this->m_visibleRows = 6;
  }

  if (!this->m_scrollBar) {
    return;
  }

  this->m_scrollBar->Move(this->geometry.width - 15, 20);

  const int visibleRows = this->m_visibleRows;
  const std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
  int total = 0;
  if (g_session.m_simplifiedSkillList != 0) {
    total = list.size() / this->m_columns + (list.size() % this->m_columns != 0);
  } else {
    int maxKey = 0;
    for (const CSkillInfo& skill : list) {
      const int key = static_cast<int>(skill.treePosition);
      if (key > maxKey) {
        maxKey = key;
      }
    }

    const int count = maxKey + 1;
    total = count / this->m_columns + (count % this->m_columns != 0);
  }

  int maxScroll = total - visibleRows;
  if (maxScroll < 0) {
    maxScroll = 0;
  }
  this->m_scrollable = maxScroll != 0;
  if (maxScroll != 0) {
    this->m_scrollBar->SetRange(maxScroll, visibleRows, total);
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
    }
    if (this->m_firstRow > total - this->m_visibleRows) {
      this->m_firstRow = total - this->m_visibleRows;
    }
    this->m_scrollBar->SetPosition(this->m_firstRow);
  } else {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, visibleRows, total);
  }

  this->m_scrollBar->Invalidate();
  this->m_needsRedraw = 1;
}

// OFFSET: 0x009B31C0
// FUNCTION: fn_13824
void UINewSkillListWnd::LayoutButtons() {
  int inset = 0;
  if (g_session.m_simplifiedSkillList != 0) {
    this->m_selectedSkill = -1;
    inset = 15;
    this->m_btnDetail->Move(this->geometry.width - inset, this->geometry.height - inset);
  } else {
    this->m_btnDetail->Move(-100, -100);
  }

  this->m_btnMinimize->Move(this->geometry.width - 30, 5);
  this->m_btnClose->Move(this->geometry.width - 16, 5);
  this->m_btnApply->Move(this->geometry.width - inset - 100, this->geometry.height - 27);
  this->m_btnCancel->Move(this->geometry.width - inset - 50, this->geometry.height - 27);
  this->m_btnApply->SetDisabled(!this->m_locked);
  this->m_btnApply->Invalidate();
  this->m_btnCancel->SetDisabled(!this->m_locked);
  this->m_btnCancel->Invalidate();
  if (!this->m_locked) {
    this->m_btnApply->SetState(0);
    this->m_btnCancel->SetState(0);
  }

  for (int i = 0; i < 10; ++i) {
    this->m_btnLevelUp[i]->Move(-200, -200);
  }
  for (int i = 0; i < 42; ++i) {
    this->m_btnSkills[i]->Move(-200, -200);
    this->m_btnSkillUps[i]->Move(-200, -200);
  }

  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
  if (g_session.m_simplifiedSkillList != 0) {
    for (int row = 0; row < 10; ++row) {
      const int y = row * 32 + 48;
      CSkillInfo skill = this->VisibleSkill(row);
      if (skill.level < static_cast<int>(skill.maxLevel) && row < this->m_visibleRows &&
          static_cast<unsigned int>(this->m_firstRow + row) < list.size() && this->m_skillPoints != 0 && skill.available != 0 &&
          skill.upgradable != 0) {
        this->m_btnLevelUp[row]->Move(78, y - 14);
      }
      if (!this->m_locked && row < this->m_visibleRows && static_cast<unsigned int>(this->m_firstRow + row) < list.size() &&
          skill.available != 0 && skill.level != 0 && CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, skill.id)) {
        this->m_btnSkills[row]->Move(112, y);
        this->m_btnSkillUps[row]->Move(192, y);
      }
    }
  } else if (!this->m_locked) {
    for (std::list<CSkillInfo>::iterator it = list.begin(); it != list.end(); ++it) {
      const unsigned int slot = it->treePosition - this->m_columns * this->m_firstRow;
      if (slot < 42 && it->available != 0 && it->treePosition != 0xFFFFFFFF && it->level != 0 &&
          CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, it->id)) {
        const int y = static_cast<int>(slot) / this->m_columns * 56 + 70;
        const int x = static_cast<int>(slot) % this->m_columns * 68 + 56;
        this->m_btnSkillUps[slot]->Move(x + 55, y);
        this->m_btnSkills[slot]->Move(x, y);
      }
    }
  }
}

// OFFSET: 0x009B3580
// FUNCTION: fn_13825
int UINewSkillListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  int& gate = g_session.m_simplifiedSkillList;
  CMode* gameMode = g_modeMgr.gameMode;

  switch (message) {
  case UM_SETINFO:
    this->m_saved = reinterpret_cast<SkillListWndInfo*>(a);
    if (this->m_saved->IsValid() == 0) {
      this->m_saved->Reset();
    }
    if (gate != 0) {
      this->SendMsg(nullptr, UM_RESIZE, 3, 330, this->m_saved->height, 0);
    } else {
      this->SendMsg(nullptr, UM_RESIZE, 3, 560, 400, 0);
    }
    return 0;
  case UM_RESIZE:
    if (a == 3) {
      if (gate != 0) {
        const int minimum = this->m_tabs->geometry.height + 12;
        if (c < minimum) {
          c = minimum;
        }
        if (c > 400) {
          c = 400;
        }
        c = (c - 400) / 32 * 32 + 400;
      }
      if (this->geometry.width != b || c != this->geometry.height) {
        this->Resize(b, c);
        this->m_scrollBar->Resize(this->m_scrollBar->geometry.width, this->geometry.height - this->m_bottomMargin - this->m_topMargin);
        this->UpdateScrollBar();
        this->LayoutButtons();
        this->Invalidate();
      }
      this->Move(this->m_saved->x, this->m_saved->y);
      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_SKILLLISTWND);
      return 0;
    case ID_APPLY:
      if (g_windowMgr.Confirm(LookupMessage(MSI_APPLY_SKILL_UP), 2, false, false, 0, "Message", 280, 120, 0) == 187) {
        this->ApplySkillPoints();
        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        if (ChatWindow()) {
          g_windowMgr.SetFocus(ChatWindow()->m_suggestions);
          return 0;
        }
      }
      break;
    case ID_RESET:
      this->ResetLists();
      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      return 0;
    case ID_MINI:
      this->StoreInfo();
      this->m_needsRedraw = 1;
      gate = gate == 0;
      if (gate != 0) {
        this->SendMsg(nullptr, UM_RESIZE, 3, 330, this->m_saved->height, 0);
      } else {
        this->SendMsg(nullptr, UM_RESIZE, 3, 560, 400, 0);
      }
      return 0;
    case ID_CHECK: {
      int& flag = g_session.m_showSkillDescription;
      flag = flag == 0;
      return 0;
    }
    case ID_LVLUP1:
    case ID_LVLUP2:
    case ID_LVLUP3:
    case ID_LVLUP4:
    case ID_LVLUP5:
    case ID_LVLUP6:
    case ID_LVLUP7:
    case 246:
    case 247:
    case 248:
      if (this->m_skillPoints != 0) {
        CSkillInfo skill = this->SkillAt(this->m_columns * this->m_firstRow + a - 239, false);
        if (skill.available != 0 && skill.upgradable != 0 && this->IncreaseSkillLevel(skill.id)) {
          this->m_needsRedraw = 1;
          this->m_locked = 1;
          this->LayoutButtons();
          this->Invalidate();
        }
        return 0;
      }
      break;
    case ID_PLUS: {
      const int tab = this->m_tab;
      int next = 0;
      if (tab + 1 != this->m_tabs->TabCount()) {
        next = tab + 1;
      }
      this->m_tabs->SelectTab(next);
      return 0;
    }
    default: {
      g_session.SetOption(7, 1, 0);
      const unsigned int offset = static_cast<unsigned int>(a) - 42;
      const unsigned int slot = offset > 41 ? static_cast<unsigned int>(a) : offset;
      CSkillInfo skill = this->SkillAt(this->m_columns * this->m_firstRow + slot, false);
      if (skill.available != 0) {
        const int current = g_session.UseLevel(skill.id);
        int level = offset > 41 ? current - 1 : current + 1;
        if (level <= 0) {
          level = 1;
        }
        if (level > skill.level) {
          level = skill.level;
        }
        g_session.m_skillBundle.SetSkillValue(skill.id, level);
        this->m_needsRedraw = 1;
        this->Invalidate();
      }
      return 0;
    }
    }
    break;
  case UM_TAB: {
    std::list<CSkillInfo>& list = this->SkillList(a, false);
    if (!this->IsExtraTab(a) && list.size() == 0) {
      this->m_tabs->activeTabIndex = this->m_tab;
      return 0;
    }
    if (this->m_tab != a) {
      this->m_selectedSkill = -1;
      this->m_tab = a;
      this->m_needsRedraw = 1;
      if (this->IsExtraTab(a) == true) {
        this->m_tabs->SetFlag(a, false);
        g_session.m_skillBundle.extraIds.clear();
      }
    }
    this->UpdateScrollBar();
    this->LayoutButtons();
    this->Invalidate();
    return 0;
  }
  case UM_REFRESHITEM:
    this->ResetLists();
    g_session.m_skillBundle.NumberExtra();
    this->UpdateScrollBar();
    this->LayoutButtons();
    this->Invalidate();
    return 0;
  case UM_REFRESH:
    this->InitTabs();
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_firstRow;
    int next;
    if (message == UM_SCROLLVERTPAGEUP) {
      next = previous - this->m_visibleRows + 1;
    } else {
      if (message == UM_SCROLLVERTPAGEDOWN) {
        a = this->m_visibleRows - 1;
      }
      next = a + previous;
    }
    this->m_firstRow = next;
    if (previous != next) {
      this->UpdateScrollBar();
      this->LayoutButtons();
      this->Invalidate();
      next = this->m_firstRow;
    }
    return next - previous;
  }
  case UM_USESKILL: {
    const CSkillInfo* skill = reinterpret_cast<const CSkillInfo*>(a);
    if (skill->level != 0) {
      gameMode->SendMsg(MM_USING_SKILL, a, g_session.UseLevel(skill->id), 0, 0);
      return 0;
    }
    break;
  }
  case UM_REFRESH_EXTRA_SKILL_TAB:
    if (!this->IsExtraTab(this->m_tab)) {
      this->m_tabs->SetFlag(this->m_tabs->TabCount() - 1, !g_session.m_skillBundle.extraIds.empty());
      this->m_tabs->Invalidate();
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// INLINED
static bool HasSkillValue(const std::list<NewSkillListValue>& values, int id) {
  for (const NewSkillListValue& entry : values) {
    if (entry.id == id) {
      return true;
    }
  }
  return false;
}

// INLINED
static int SkillValueOf(const std::list<NewSkillListValue>& values, int id) {
  for (const NewSkillListValue& entry : values) {
    if (entry.id == id) {
      return entry.value;
    }
  }
  return 0;
}

// OFFSET: 0x009B3EB0
// FUNCTION: fn_13827
bool UINewSkillListWnd::IncreaseSkillLevel(int skillId) {
  if (this->m_skillPoints <= 0 || this->m_showDetail) {
    return false;
  }

  const int gate = g_session.m_simplifiedSkillList;
  if (gate == 0) {
    for (auto it = this->m_values.begin(); it != this->m_values.end(); ++it) {
      if (this->m_skillPoints <= 0) {
        break;
      }

      const int id = it->id;
      CSkillInfo* info = g_session.m_skillBundle.FindGrouped(id);
      if (info == nullptr || info->available == 0) {
        continue;
      }

      int target;
      if (id == skillId) {
        target = (std::max)(static_cast<int>(info->learnedLevel), SkillValueOf(this->m_plannedSkillLevels, id)) + 1;
        if (static_cast<int>(info->learnedLevel) >= static_cast<int>(info->maxLevel)) {
          return false;
        }
      } else {
        target = it->value;
      }

      if (target > static_cast<int>(info->maxLevel)) {
        continue;
      }

      const auto planned = std::find_if(this->m_plannedSkillLevels.begin(), this->m_plannedSkillLevels.end(), [id](const NewSkillListValue& value) {
        return value.id == id;
      });
      const int current = (std::max)(static_cast<int>(info->learnedLevel), planned != this->m_plannedSkillLevels.end() ? planned->value : 0);
      if (current >= target) {
        continue;
      }

      if (static_cast<int>(info->canPlanLevel) <= 0) {
        const char* name = info->DisplayName(*g_session.m_hotkeyScript);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatString(LookupMessage(MSI_REFUSE_LOGIN_AUTH), name)), 0xFF, 0, 0, 0);
        return false;
      }

      const int added = (std::min)(this->m_skillPoints, target - current);
      this->m_skillPoints -= added;
      const int level = current + added;

      bool found = false;
      for (NewSkillListValue& value : this->m_plannedSkillLevels) {
        if (value.id == id) {
          value.value = level;
          found = true;
          break;
        }
      }
      if (!found) {
        this->m_plannedSkillLevels.push_back(NewSkillListValue{id, level});
      }
    }

    return true;
  }

  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
  for (CSkillInfo& skill : list) {
    if (static_cast<int>(skill.id) == skillId) {
      if (skill.level >= static_cast<int>(skill.maxLevel)) {
        return false;
      }
      ++skill.level;
      --this->m_skillPoints;
      return true;
    }
  }
  return false;
}

// OFFSET: 0x009AF960
// FUNCTION: fn_13803
void UINewSkillListWnd::DrawSkillTree() {
  LuaScript& script = *g_session.m_hotkeyScript;

  CBitmapRes* emptySlot = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\no_skill.bmp")));
  for (int slot = 0; slot < 42; ++slot) {
    this->DrawBitmap(slot % this->m_columns * 68 + 76, slot / this->m_columns * 56 + 44, emptySlot, true);
  }

  std::list<CSkillInfo>& skills = this->SkillList(this->m_tab, false);
  for (auto it = skills.begin(); it != skills.end(); ++it) {
    const CSkillInfo& skill = *it;
    const int position = static_cast<int>(skill.treePosition);
    if (position == -1) {
      continue;
    }
    const int first = this->m_firstRow * this->m_columns;
    if (position < first) {
      continue;
    }
    const int slot = position - first;
    if (slot >= 42) {
      continue;
    }

    const int level = skill.level;
    const int x = slot % this->m_columns * 68 + 72;
    const int y = slot / this->m_columns * 56;

    if (!this->IsExtraTab(this->m_tab) && HasSkillValue(this->m_values, static_cast<int>(skill.id))) {
      this->FillRegion(x + 3, y + 43, 26, 26, 0xCCC0FF);
      DrawRoundedBorder(*this, x + 2, y + 42, 28, 28, 0xC1B6FF);
    } else if (this->m_skillPoints != 0 && skill.available != 0 && skill.upgradable != 0) {
      this->FillRegion(x + 3, y + 43, 26, 26, 0xF0C4AF);
      DrawRoundedBorder(*this, x + 2, y + 42, 28, 28, 0xF0C4AF);
    } else {
      this->FillRegion(x + 3, y + 43, 26, 26, 0xFFFFFF);
    }

    int shade = 102;
    char text[80];
    g_session.ItemBitmapPath(skill.name, text);
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
    if (!bitmap) {
      std::sprintf(text, "유저인터페이스\\item\\am_berserkpitcher.bmp");
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
    }
    if (bitmap) {
      if (!this->IsExtraTab(this->m_tab) && HasSkillValue(this->m_values, static_cast<int>(skill.id))) {
        this->DrawAuxiliary(2, x + 4, y + 44, &bitmap->image, true, true);
        const std::string value = this->ValueText(static_cast<int>(skill.id));
        if (value.size() != 0) {
          DrawOutlinedText(*this, x + 30, y + 58, value.c_str(), 0, 0xFFFFFF, 0, 0, 11, 1, 0);
        }
      } else if (this->m_skillPoints != 0 && skill.available != 0 && skill.upgradable != 0) {
        this->DrawAuxiliary(4, x + 4, y + 44, &bitmap->image, true, true);
      } else if (level == 0) {
        shade = 120;
        this->DrawAuxiliary(1, x + 4, y + 44, &bitmap->image, true, true);
      } else {
        this->DrawBitmap(x + 4, y + 44, bitmap, true);
      }

      const int planned = SkillValueOf(this->m_plannedSkillLevels, static_cast<int>(skill.id));
      const CSkillInfo* current = g_session.m_skillBundle.FindGrouped(skill.id);
      if (current) {
        int added = planned - current->learnedLevel;
        if (added < 0) {
          added = 0;
        }
        if (added > 0) {
          this->DrawTextAt(x + 30, y + 44, FormatString("+%d", added), 0, 0, 11, 0xFF, 0, 0);
        }
      }
    }

    const int maxLevel = this->SkillAt(static_cast<int>(skill.treePosition), true).level;
    int red = shade;
    int green = shade;
    int blue = shade;
    if (level > maxLevel) {
      red = 127;
      green = 60;
      blue = 255;
    }

    const int textY = y + 70;
    const std::uint32_t gray = (shade << 8 | shade) << 8 | shade;
    const bool usesLevel = CSkillInfo::UsesLevel(script, static_cast<int>(skill.id));
    if (level != 0) {
      if (!usesLevel) {
        std::sprintf(text, "%d", skill.level);
        const int width = this->TextWidth(text, 0, 0, 11, 1, 0);
        DrawOutlinedText(*this, (24 - width) / 2 + x + 4, textY, text, 0, (blue << 8 | green) << 8 | red, 0xFFFFFF, 0, 11, 1, 0);
      } else {
        int useLevel = g_session.UseLevel(static_cast<int>(skill.id));
        if (useLevel <= 0 || useLevel > skill.level) {
          useLevel = skill.level;
        }
        std::sprintf(text, "%d / %d", useLevel, skill.level);
        int textX = (24 - this->TextWidth(text, 0, 0, 11, 1, 0)) / 2 + x + 4;
        std::sprintf(text, "%d", useLevel);
        DrawOutlinedText(*this, textX, textY, text, 0, useLevel == skill.level ? gray : 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);
        textX += this->TextWidth(text, 0, 0, 11, 1, 0);
        DrawOutlinedText(*this, textX, textY, " / ", 0, gray, 0xFFFFFF, 0, 11, 1, 0);
        textX += this->TextWidth(" / ", 0, 0, 11, 1, 0);

        const std::uint32_t id = skill.id;
        int target = SkillValueOf(this->m_plannedSkillLevels, static_cast<int>(id));
        const int grouped = g_session.m_skillBundle.GroupedLevel(id);
        if (target < grouped) {
          target = grouped;
        }
        const CSkillInfo* extra = g_session.m_skillBundle.FindExtra(id);
        if (extra && target < extra->level) {
          target = extra->level;
        }
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", target);
        DrawOutlinedText(*this, textX, textY, text, 0, (blue << 8 | green) << 8 | red, 0xFFFFFF, 0, 11, 1, 0);
      }
    }

    const int nameWidth = this->TextWidth(skill.DisplayName(script), 0, 0, 11, 0, 0);
    int nameOffset = 0;
    if (nameWidth < 48) {
      nameOffset = (48 - nameWidth) / 2;
    }
    this->DrawTextAt(x - 8 + nameOffset, y + 30, this->ClipTextToWidth(skill.DisplayName(script), 48, 0, 11, 0), 0, 0, 11, gray, 0, 0
    );

    if (g_session.m_skillBundle.HasHigherLevel(skill.id) == true) {
      this->DrawBitmap(
        x + 8, y + 58, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\item_skill.bmp"))), true
      );
    }
  }
}

// OFFSET: 0x009AE260
// FUNCTION: fn_13797
UINewSkillListWnd::UINewSkillListWnd() : UIFrameWnd(0) {
  this->m_tab = 0;
  this->m_needsRedraw = 1;
  this->m_selectedSkill = -1;
  this->m_lastTick = timeGetTime();
  this->m_topMargin = 20;
  this->m_bottomMargin = 32;
  this->m_saved = nullptr;
  this->m_columns = 1;
  this->m_visibleRows = 1;
  this->m_showDetail = false;
  this->m_detailId = -1;
}

// OFFSET: 0x009AE530
// FUNCTION: fn_13798
UINewSkillListWnd::~UINewSkillListWnd() {
}
