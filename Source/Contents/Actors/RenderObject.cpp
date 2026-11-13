#include "Contents/Actors/Npc.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/CombatTypes.h"
#include "Contents/Skills/SkillId.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Actors/GrannyGameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Actors/ActorPickNode.h"
#include "Contents/Effects/Core/MsgEffect.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Client/Replay/Replay.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/CharacterRender.h"
#include "Render/DX9/RendererDX9.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/EffectActors.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Skills/EnergyOrbEffect.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Skills/TetraBallEffect.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/World/WorldProximity.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Bitmap/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/ScalarConversion.h"
#include "Contents/Monster/ChampionMonsterMgr.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Ground/TerrainHeight.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>
#include "Contents/Guild/GuildInfo.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

template __declspec(noinline) std::string::basic_string(const std::string&);

// GLOBAL: gl_25173
int g_playerPoisoned;

// GLOBAL: gl_25144
CActorPickNode g_actorPickNode;

// GLOBAL: gl_25186
int g_pickModifierKey;

// GLOBAL: gl_25187
int g_pickInvert;

struct CharacterRenderDrawItem {
  int flags;
  RenderCommand* command;
};

// GLOBAL: gl_24798
int g_multiHitInterval = 200;

class CGrannyCorpse3d : public CGameObject {
public:
  explicit CGrannyCorpse3d(const char* name);
  ~CGrannyCorpse3d() override;
  bool OnProcess() override;
  void Render(int argument) override;

  /* 0010 */ vector3d position;
  /* 001C */ C3dGrannyModelRes* model;
  /* 0020 */ granny_model_instance* instance;
  /* 0024 */ granny_control* control;
  /* 0028 */ RenderCommand* commands[30];
  /* 00A0 */ RenderVertex* vertices[30];
  /* 0118 */ granny_system_clock clock;
  /* 0128 */ granny_system_clock startClock;
  /* 0138 */ float animationTime;
  /* 013C */ unsigned char padding_013C[4];
  /* 0140 */ int tick;
  /* 0144 */ float angle;
  /* 0148 */ unsigned char alpha;
  /* 0149 */ unsigned char padding_0149[3];
  /* 014C */ unsigned brightness;
  /* 0150 */ char name[0x108];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(C3dGrannyGameActor, model) == 0x414);
static_assert(offsetof(C3dGrannyGameActor, motionProgress) == 0x52C);
static_assert(offsetof(C3dGrannyGameActor, motion) == 0x544);
static_assert(offsetof(C3dGrannyGameActor, controls) == 0x658);
static_assert(offsetof(C3dGrannyGameActor, commands) == 0x41C);
static_assert(offsetof(C3dGrannyGameActor, vertices) == 0x494);
static_assert(offsetof(C3dGrannyGameActor, alpha) == 0x53C);
static_assert(offsetof(C3dGrannyGameActor, name) == 0x54C);
static_assert(offsetof(C3dGrannyGameActor, renderState) == 0x650);
static_assert(offsetof(C3dGrannyGameActor, initializeFacing) == 0x6A8);
static_assert(offsetof(C3dGrannyGameActor, texture) == 0xDB8);
static_assert(offsetof(C3dGrannyGameActor, angle) == 0x538);
static_assert(offsetof(C3dGrannyGameActor, brightness) == 0x540);
static_assert(offsetof(C3dGrannyGameActor, meshTextures) == 0x6AC);
static_assert(offsetof(C3dGrannyGameActor, meshTransforms) == 0x814);
static_assert(offsetof(C3dGrannyGameActor, litPositions) == 0xDBC);
static_assert(offsetof(CGrannyCorpse3d, tick) == 0x140);
static_assert(offsetof(CGrannyCorpse3d, alpha) == 0x148);
static_assert(offsetof(CGrannyCorpse3d, startClock) == 0x128);
static_assert(sizeof(CGrannyCorpse3d) == 0x258);
#endif

// INLINED
RenderObjectState::RenderObjectState() : sprite(nullptr), action(nullptr) {
  this->Reset();
}

// INLINED
RenderObjectState::~RenderObjectState() {
  if (this->sprite) {
    this->sprite->Unlock();
  }

  if (this->action) {
    this->action->Unlock();
  }
}

// OFFSET: 0x00CA8730
// FUNCTION: fn_20991
CRenderObject::CRenderObject() {
}

// OFFSET: 0x00CA8F90
// FUNCTION: fn_20998
__declspec(noinline) CRenderObject::~CRenderObject() = default;

// OFFSET: 0x00CAFDC0
// FUNCTION: fn_21089
bool CRenderObject::OnProcess() {
  return false;
}

// OFFSET: 0x00CB0500
// FUNCTION: fn_21094
void CRenderObject::ProcessMotion() {
  this->render.previousAnimation = this->render.animation + this->Get8Dir(this->render.angle);
  g_modeMgr.GetGameMode();
  const float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
  CActRes* action = this->render.action;
  if (this->render.state == 0x2D) {
    action = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\여\\아크비숍_여2.Act"));
  }
  if (action == nullptr) {
    return;
  }

  if (this->render.animationSequenceActive != 0) {
    this->render.sequenceAnimation = this->render.sequenceAnimations[this->render.sequenceIndex];
    this->render.sequenceFrame = this->render.sequenceFrames[this->render.sequenceIndex];
    if (this->render.sequenceFinished != 0) {
      this->render.animation = this->render.sequenceAnimations[4];
      this->render.previousAnimation = this->render.sequenceAnimations[4] + this->Get8Dir(this->render.angle);
      this->render.curMotion = this->render.sequenceFrames[4];
      if (this->render.sequenceDeadline <= 0) {
        this->render.animationSequenceActive = 0;
        this->render.motionType = 1;
      }
    } else {
      this->render.animation = this->render.sequenceAnimation;
      this->render.previousAnimation = this->render.sequenceAnimation + this->Get8Dir(this->render.angle);
      this->render.curMotion = this->render.sequenceFrame;
    }
    if (this->render.sequenceTimed == 0) {
      return;
    }
    if (timeGetTime() >= this->render.sequenceDeadline) {
      this->render.animationSequenceActive = 0;
      this->render.sequenceDeadline = 0;
      this->render.motionType = 1;
      this->render.isMotionFinished = true;
    }
    if (this->render.sequenceFinished == 0 && this->render.sequenceTick % this->render.sequenceInterval == 0) {
      ++this->render.sequenceIndex;
    }
    if (this->render.sequenceIndex > this->render.sequenceLast) {
      if (this->render.sequenceLoop != 0) {
        ++this->render.sequenceTick;
        this->render.sequenceIndex = 0;
        return;
      }
      this->render.sequenceIndex = this->render.sequenceLast;
      this->render.sequenceFinished = 1;
    }
    ++this->render.sequenceTick;
    return;
  }

  if (this->render.timedSequenceActive != 0) {
    if (this->render.sequenceFinished != 0) {
      this->render.animation = this->render.sequenceFinishAnimation;
      this->render.previousAnimation = this->render.sequenceFinishAnimation + this->Get8Dir(this->render.angle);
      this->render.curMotion = this->render.sequenceFinishFrame;
      if (this->render.sequenceDeadline <= 0) {
        this->render.frameSequenceActive = 0;
        this->render.motionType = 1;
      }
    } else {
      this->render.animation = this->render.sequenceAnimation;
      this->render.previousAnimation = this->render.sequenceAnimation + this->Get8Dir(this->render.angle);
      this->render.curMotion = this->render.sequenceFrame;
    }
    if (this->render.sequenceTimed == 0) {
      return;
    }
    if (timeGetTime() >= this->render.sequenceDeadline) {
      this->render.frameSequenceActive = 0;
      this->render.sequenceDeadline = 0;
      this->render.motionType = 1;
      this->render.isMotionFinished = true;
    }
    if (this->render.sequenceFinished == 0) {
      if (this->render.sequenceMode == 3) {
        this->render.sequenceFrame =
          static_cast<unsigned char>(~static_cast<int>(frame * -2.0f / this->render.speed) - this->render.sequenceLast);
      } else {
        this->render.sequenceFrame = static_cast<unsigned char>(static_cast<int>((frame + frame) / this->render.speed));
      }
    }
    if (this->render.sequenceFrame > this->render.sequenceLast) {
      if (this->render.sequenceMode == 2) {
        ++this->render.sequenceTick;
        this->render.sequenceMode = 3;
        this->render.sequenceFrame = 0;
        return;
      }
      this->render.sequenceMode = 0;
      this->render.sequenceFrame = this->render.sequenceLast;
      this->render.sequenceFinished = 1;
    }
    ++this->render.sequenceTick;
    return;
  }

  if (this->render.frameSequenceActive != 0) {
    if (this->render.sequenceFinished != 0) {
      this->render.animation = this->render.sequenceFinishAnimation;
      this->render.previousAnimation = this->render.sequenceFinishAnimation + this->Get8Dir(this->render.angle);
      this->render.curMotion = this->render.sequenceFinishFrame;
      if (this->render.sequenceDeadline <= 0) {
        this->render.frameSequenceActive = 0;
        this->render.motionType = 1;
      }
    } else {
      this->render.animation = this->render.sequenceAnimation;
      this->render.previousAnimation = this->render.sequenceAnimation + this->Get8Dir(this->render.angle);
      this->render.curMotion = this->render.sequenceFrame;
    }
    if (this->render.sequenceTimed == 0) {
      return;
    }
    if (timeGetTime() >= this->render.sequenceDeadline) {
      this->render.frameSequenceActive = 0;
      this->render.sequenceDeadline = 0;
      this->render.motionType = 1;
      this->render.isMotionFinished = true;
    }
    if (this->render.sequenceFinished == 0 && this->render.sequenceTick % this->render.sequenceInterval == 0) {
      if (this->render.sequenceLoop != 0 && this->render.sequenceFinishAnimation == 0 && this->render.sequenceFinishFrame != 0) {
        --this->render.sequenceFrame;
        this->render.sequenceFinishFrame = 0;
      } else {
        ++this->render.sequenceFrame;
      }
    }
    if (this->render.sequenceFrame > this->render.sequenceLast) {
      if (this->render.sequenceLoop == 0) {
        this->render.sequenceFinished = 1;
        this->render.sequenceFrame = this->render.sequenceLast;
      } else if (this->render.sequenceFinishAnimation == 0) {
        this->render.sequenceFinishFrame = 1;
        ++this->render.sequenceTick;
        this->render.sequenceFrame = this->render.sequenceIndex + 1;
        return;
      } else {
        this->render.sequenceFrame = this->render.sequenceIndex;
      }
    }
    ++this->render.sequenceTick;
    return;
  }

  if (this->render.isMotionFinished || this->render.isMotionFreezed) {
    const int last = static_cast<int>(action->FrameCount(this->render.previousAnimation)) - 1;
    if (this->render.curMotion > last) {
      this->render.curMotion = last;
    }
    return;
  }

  int count;
  int motion;
  float progress;
  switch (this->render.motionType) {
    case MT_LOOP:
      this->render.curMotion =
        static_cast<int>(frame / this->render.speed) % static_cast<int>(action->FrameCount(this->render.previousAnimation));
      return;
    case MT_ONETIME:
      count = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      progress = frame / this->render.speed;
      motion = static_cast<int>(progress);
      break;
    case MT_STATIC: {
      if (this->render.state != 2) {
        return;
      }
      count = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      float elapsed = frame;
      motion = static_cast<int>(frame / this->render.speed);
      if (motion == 2 || motion == 3) {
        elapsed = this->render.speed * 4.0f;
        motion = 4;
      }
      progress = elapsed / this->render.speed;
      break;
    }
    default:
      return;
  }

  this->render.curMotion = motion % count;
  progress /= static_cast<float>(count);
  if (progress >= 1.0f) {
    this->render.curMotion = count - 1;
    if (progress >= this->render.motionFinishThreshold) {
      this->render.isMotionFinished = true;
    }
  }
}

// OFFSET: 0x00CB0AD0
// FUNCTION: fn_21096
void CRenderObject::PlayFrameSounds() {
  int lastMotion = static_cast<int>(this->render.lastSoundMotion);
  if (lastMotion != this->render.curMotion || this->render.lastSoundAnimation != static_cast<std::uint32_t>(this->render.animation)) {
    if (this->render.lastSoundAnimation != static_cast<std::uint32_t>(this->render.animation)) {
      this->render.lastSoundMotion = 0;
      lastMotion = 0;
    }
    CActRes* action = this->render.action;
    if (lastMotion < this->render.curMotion) {
      for (int motion = lastMotion + 1; motion <= this->render.curMotion; ++motion) {
        const ActionFrame& actionFrame = action->Frame(this->render.previousAnimation, motion);
        if (actionFrame.sound != -1) {
          std::string sound = action->Sound(actionFrame.sound);
          if (sound.find(".wav", 0) != std::string::npos) {
            CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
            const vector3d& player = static_cast<const CRenderObject*>(world->player)->render.position;
            g_soundMgr->PlayWave(
              sound.c_str(), this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
            );
          }
        }
      }
    } else if (lastMotion > this->render.curMotion) {
      const int count = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      lastMotion = static_cast<int>(this->render.lastSoundMotion);
      if (lastMotion >= count) {
        lastMotion = count - 1;
        this->render.lastSoundMotion = static_cast<std::uint32_t>(lastMotion);
      }
      for (int motion = lastMotion + 1; motion < count; ++motion) {
        const ActionFrame& actionFrame = action->Frame(this->render.previousAnimation, motion);
        if (actionFrame.sound != -1) {
          std::string sound = action->Sound(actionFrame.sound);
          if (sound.find(".wav", 0) != std::string::npos) {
            CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
            const vector3d& player = static_cast<const CRenderObject*>(world->player)->render.position;
            g_soundMgr->PlayWave(
              sound.c_str(), this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
            );
          }
        }
      }
      for (int motion = 0; motion <= this->render.curMotion; ++motion) {
        const ActionFrame& actionFrame = action->Frame(this->render.previousAnimation, motion);
        if (actionFrame.sound != -1) {
          std::string sound = action->Sound(actionFrame.sound);
          if (sound.find(".wav") != std::string::npos) {
            CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
            const vector3d& player = static_cast<const CRenderObject*>(world->player)->render.position;
            g_soundMgr->PlayWave(
              sound.c_str(), this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
            );
          }
        }
      }
    }
  }
  this->render.lastSoundMotion = static_cast<std::uint32_t>(this->render.curMotion);
  this->render.lastSoundAnimation = static_cast<std::uint32_t>(this->render.animation);
}

// OFFSET: 0x00CBF920
// FUNCTION: fn_21115
void CRenderObject::SendMsg(CGameObject*, std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t) {
}

// OFFSET: 0x00CBF930
// FUNCTION: fn_21116
void CRenderObject::SetActionName(const char* name) {
  if (this->render.action) {
    this->render.action->Unlock();
  }

  this->render.action = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));

  if (this->render.action) {
    this->render.action->Lock();
  }
}

// INLINED
void CRenderObject::SetActionName(ResourceMgr&, const char* name) {
  this->SetActionName(name);
}

// OFFSET: 0x00CC2D40
// FUNCTION: fn_21144
void CRenderObject::SetRenderInfo(const float*, float, float) {
}

// OFFSET: 0x00CC2DA0
// FUNCTION: fn_21146
__declspec(noinline) void CGameActor::SetSexBit(int value) {
  this->sex = value & 1;
}

// OFFSET: 0x00CC2E00
// FUNCTION: fn_21151
void CRenderObject::SetSpriteName(const char* name) {
  if (this->render.sprite) {
    this->render.sprite->Unlock();
  }

  this->render.sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(name));

  if (this->render.sprite) {
    this->render.sprite->Lock();
  }
}

// INLINED
void CRenderObject::SetSpriteName(ResourceMgr&, const char* name) {
  this->SetSpriteName(name);
}

// OFFSET: 0x00CC3DC0
// FUNCTION: fn_21154
void CRenderObject::SetTlvert(float x, float y) {
  this->render.screenX = static_cast<int>(x);
  this->render.screenY = static_cast<int>(y);
}

// OFFSET: 0x00CBFAE0
// FUNCTION: fn_21118
void CRenderObject::SetAction(unsigned animation, int, int repeat) {
  float speed = this->render.action->Speed(animation);
  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }
  if (speed > 0.0f) {
    this->render.speed = speed;
  }
  if (this->render.speed < 1.0f) {
    if (g_replayAnimationScaling != 0) {
      speed = Replay::Instance().AnimationScale();
      if (speed > 0.0f) {
        this->render.speed = speed;
      }
    } else {
      this->render.speed = 1.0f;
    }
  }
  speed = this->render.speed * this->render.motionSpeedScale;
  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }
  if (speed > 0.0f) {
    this->render.speed = speed;
  }

  this->render.animation = animation;
  this->render.previousAnimation = animation;
  this->render.curMotion = 0;
  this->render.motionType = repeat;
  this->render.isMotionFinished = 0;
  this->render.motionFinishThreshold = 1.0f;
  this->render.motionSpeedScale = 1.0f;
  this->render.frameSequenceActive = 0;
  this->render.animationSequenceActive = 0;
  this->render.timedSequenceActive = 0;
}

// OFFSET: 0x00CBFC00
// FUNCTION: fn_21119
void CRenderObject::SetColor(int alpha, int red, int green, int blue) {
  unsigned char* channels = reinterpret_cast<unsigned char*>(&this->render.color);
  if (alpha != -1) {
    channels[3] = static_cast<unsigned char>(alpha);
  }
  if (red != -1) {
    channels[2] = static_cast<unsigned char>(red);
  }
  if (green != -1) {
    channels[1] = static_cast<unsigned char>(green);
  }
  if (blue != -1) {
    channels[0] = static_cast<unsigned char>(blue);
  }
}

// OFFSET: 0x00CAF050
// FUNCTION: fn_21078
void CRenderObject::ModifyArgb(int alpha, int red, int green, int blue) {
  if (this->render.colorLocked == 0) {
    unsigned char* argb = reinterpret_cast<unsigned char*>(&this->render.color);
    int newAlpha = argb[3] + alpha;
    int newRed = argb[2] + red;
    int newGreen = argb[1] + green;
    int newBlue = argb[0] + blue;
    argb[3] = static_cast<unsigned char>(std::min(std::max(newAlpha, 0), 255));
    argb[2] = static_cast<unsigned char>(std::min(std::max(newRed, 0), 255));
    argb[1] = static_cast<unsigned char>(std::min(std::max(newGreen, 0), 255));
    argb[0] = static_cast<unsigned char>(std::min(std::max(newBlue, 0), 255));
  }
}

// OFFSET: 0x00CC1C20
// FUNCTION: fn_21125
void CGameActor::SetSpGauge(unsigned char value, int extra) {
  this->hasSp = value;
  this->spGaugeValue = extra;
}

// OFFSET: 0x00CC1D20
// FUNCTION: fn_21127
void CGameActor::RaiseTransformPriority(int value) {
  if (value <= 0 || this->transformPriority < value) {
    this->transformPriority = value;
  }
}

// OFFSET: 0x00CC1E70
// FUNCTION: fn_21130
void CRenderObject::SetSequenceDeadline(std::uint32_t duration) {
  this->render.sequenceDeadline = timeGetTime() + duration;
}

// OFFSET: 0x00CC1E90
// FUNCTION: fn_21131
void CRenderObject::SetSequenceInterval(unsigned char interval) {
  this->render.sequenceInterval = interval;
}

// OFFSET: 0x00CC1EA0
// FUNCTION: fn_21132
void CRenderObject::SetAnimationSequence(
  unsigned char animation0, unsigned char frame0, unsigned char animation1, unsigned char frame1,
  unsigned char animation2, unsigned char frame2, unsigned char animation3, unsigned char frame3,
  unsigned char animation4, unsigned char frame4, int timed, std::uint32_t duration, int loop
) {
  this->render.sequenceAnimations[0] = animation0;
  this->render.sequenceFrames[0] = frame0;
  this->render.sequenceAnimations[1] = animation1;
  this->render.sequenceFrames[1] = frame1;
  this->render.sequenceAnimations[2] = animation2;
  this->render.sequenceFrames[2] = frame2;
  this->render.sequenceAnimations[3] = animation3;
  this->render.sequenceFrames[3] = frame3;
  this->render.sequenceAnimations[4] = animation4;
  this->render.sequenceFrames[4] = frame4;
  this->render.sequenceAnimation = animation0;
  this->render.sequenceFrame = frame0;
  this->render.sequenceIndex = 0;
  this->render.sequenceLast = 4;
  this->render.sequenceLoop = loop;
  this->render.sequenceTimed = timed;
  this->render.sequenceFinished = 0;
  this->render.animationSequenceActive = 1;
  this->render.frameSequenceActive = 0;
  this->render.timedSequenceActive = 0;
  this->render.sequenceDeadline = timed != 0 ? timeGetTime() + duration : 0;
  this->render.sequenceTick = 1;
}

// OFFSET: 0x00CC1FA0
// FUNCTION: fn_21133
void CRenderObject::SetFrameSequence(
  unsigned char animation, unsigned char firstFrame, unsigned char lastFrame, unsigned char finishAnimation,
  unsigned char finishFrame, int timed, std::uint32_t duration, int loop
) {
  this->render.sequenceAnimation = animation;
  this->render.sequenceFrame = firstFrame;
  this->render.sequenceIndex = firstFrame;
  this->render.sequenceLast = lastFrame;
  this->render.sequenceFinishAnimation = finishAnimation;
  this->render.sequenceFinishFrame = finishFrame;
  this->render.sequenceLoop = loop;
  this->render.sequenceTimed = timed;
  this->render.sequenceFinished = 0;
  this->render.frameSequenceActive = 1;
  this->render.timedSequenceActive = 0;
  this->render.animationSequenceActive = 0;
  this->render.sequenceDeadline = timed != 0 ? timeGetTime() + duration : 0;
  this->render.sequenceTick = 1;
}

// OFFSET: 0x00CC2050
// FUNCTION: fn_21134
void CGameActor::SetGroupMemberVisible(int value) {
  this->isGroupMemberVisible = value;
}

// OFFSET: 0x00CC2780
// FUNCTION: fn_21140
void CRenderObject::SetAnimationSpeed(float speed) {
  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }
  if (speed > 0.0f) {
    this->render.speed = speed;
  }
}

// OFFSET: 0x00CC2810
// FUNCTION: fn_21142
void CGameActor::SetHealthState(int value) {
  this->healthState = value;
}

// OFFSET: 0x00CBF970
// FUNCTION: fn_21117
void CGameActor::SetAction(unsigned animation, int motion, int repeat) {
  if (this->objectType == 7) {
    if (!this->render.action || static_cast<int>(this->render.action->animations.size()) <= static_cast<int>(animation)) {
      return;
    }
  }

  CRenderObject::SetAction(animation, motion, repeat);
  this->isNeverAnimation = 0;
  this->attackMotion = -1.0f;
}

// OFFSET: 0x00CC15F0
// FUNCTION: fn_21121
int CRenderObject::HoverHeight() {
  this->render.hoverAngle += 2;
  if (this->render.hoverAngle >= 360) {
    this->render.hoverAngle -= 360;
  }

  const short angle = this->render.hoverAngle;
  const int state = this->render.state;
  if (state == 0 || state == 1 || (this->render.flags & 0x40000) != 0) {
    if (this->render.hoverHeight < 20) {
      if (angle % 30 == 0) {
        ++this->render.hoverHeight;
      }
    } else if (this->render.hoverHeight > 20 && angle % 30 == 0) {
      --this->render.hoverHeight;
    }
  } else if (state == 6) {
    if (this->render.hoverHeight < 100) {
      if (angle % 30 == 0) {
        ++this->render.hoverHeight;
      }
    } else if (this->render.hoverHeight > 100 && angle % 30 == 0) {
      --this->render.hoverHeight;
    }
  } else if (this->render.hoverHeight != 0) {
    --this->render.hoverHeight;
  }

  const short height = static_cast<short>(g_sinTable[angle] * 4.0f + this->render.hoverHeight);
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const short scaled = static_cast<short>(400.0f / mode->camera->camera.desiredDistance * height);
  return scaled < 0 ? 0 : scaled;
}

// OFFSET: 0x00CC1740
// FUNCTION: fn_21122
void CRenderObject::DrawStatusOverlays(SpriteQuad quad, const std::uint32_t* palette) {
  if ((this->render.flags & 0x100) != 0) {
    bool draw = true;
    if (this->render.effectPhase <= 5) {
      quad.color = (static_cast<short>(this->render.effectPhase * 50) << 24) + 0xFFFFFF;
    } else if (this->render.effectPhase <= 8) {
      quad.color = 0xFFFFFFFF;
    } else if (this->render.effectPhase <= 11) {
      quad.color = (static_cast<short>(895 - this->render.effectPhase * 80) << 24) + 0xFFFFFF;
    } else {
      draw = false;
    }

    if (draw) {
      this->DrawQuad(quad, palette, 5, 0.0f, 0.0f, 0);
    }
  }

  if ((this->render.flags & 0x4000) != 0) {
    this->DrawTintedQuad(quad, 5, 5, 255, palette);
  }

  if ((this->render.flags & 0x40000) != 0) {
    this->DrawTintedQuad(quad, 255, 5, 5, palette);
  }
}

// OFFSET: 0x00CC1810
// FUNCTION: fn_21123
void CRenderObject::ScaleByPulse(float& scale) const {
  if ((this->render.flags & 0x10) != 0) {
    const unsigned phase = this->render.pulsePhases[0] * 4u + 90u;
    const unsigned index = phase >= 90u ? phase : 90u;
    if (index < 270) {
      scale = ((g_sinTable[index] + 1.0f) * 0.25f + 0.5f) * scale;
    } else {
      scale = scale * 0.5f;
    }
  }

  if ((this->render.flags & 0x400) != 0) {
    const unsigned phase = this->render.pulsePhases[1] * 4u + 90u;
    const unsigned index = phase >= 90u ? phase : 90u;
    if (index < 270) {
      scale = (1.5f - (g_sinTable[index] + 1.0f) * 0.25f) * scale;
    } else {
      scale = scale * 1.5f;
    }
  }
}

// OFFSET: 0x00CC18F0
// FUNCTION: fn_21124
void CRenderObject::ApplySwing(int mode, int& x, int& y, int& outX, int& outY) const {
  if ((this->render.flags & 0x40) == 0) {
    return;
  }

  const int swing = static_cast<int>(180.0f - g_sinTable[this->render.effectPhase * 5 + 90] * 180.0f);
  const int angle = swing >= 360 ? swing - 360 : swing;
  if (mode == 1) {
    x = static_cast<int>(g_sinTable[angle] * 40.0f + x);
    y = static_cast<int>(y - (g_cosTable[angle] * 40.0f - 40.0f));
    outX = x;
    outY = y;
  } else if (mode >= 2) {
    x = static_cast<int>(g_sinTable[angle] * 10.0f + x);
    y = static_cast<int>(y - (g_cosTable[angle] * 10.0f - 10.0f));
  }
}

// OFFSET: 0x00CC1C40
// FUNCTION: fn_21126
__declspec(noinline) void CRenderObject::DrawTintedQuad(SpriteQuad& quad, short red, short green, short blue, const std::uint32_t* palette) {
  const unsigned char phase = this->render.tintPhase;
  if (phase <= 10) {
    quad.color = (((((phase * 15) << 8) + red) << 8) + green << 8) + blue;
  } else if (phase <= 20) {
    quad.color = ((((0xA0 << 8) + red) << 8) + green << 8) + blue;
  } else if (phase <= 50) {
    const short alpha = static_cast<short>((51 - phase) * 5);
    quad.color = ((((alpha << 8) + red) << 8) + green << 8) + blue;
  }

  if (this->render.tintPhase <= 50) {
    this->DrawQuad(quad, palette, 5, 0.0f, 0.0f, 0);
    this->DrawQuad(quad, palette, 5, 0.0f, 0.0f, 0);
  }
}

// OFFSET: 0x00CC2240
// FUNCTION: fn_21138
void CRenderObject::SetInstantValue(int key, std::int64_t value) {
  this->render.instant.Values()[static_cast<int>(key)] = value;
}

// OFFSET: 0x00CC2060
// FUNCTION: fn_21135
void CGameActor::SetGuildInfo(int guildId, int version) {
  if (guildId <= 0) {
    return;
  }

  this->guildId = guildId;
  if (this->GetEffectiveJob(-1) != 20269) {
    if (this->GetEffectiveJob(-1) != JT_GUILD_FLAG && version <= 0) {
      return;
    }
    if (!g_session.IsNpcJob(this->GetEffectiveJob(-1)) && !g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) &&
        !g_session.IsGuardianJob(this->GetEffectiveJob(-1)) && this->GetEffectiveJob(-1) != JT_GUILD_FLAG &&
        this->GetEffectiveJob(-1) != JT_EMPELIUM) {
      return;
    }
  }

  g_guildMgr->SetEmblemVersion(guildId, version);
}

// OFFSET: 0x00CC21F0
// FUNCTION: fn_21136
void CGameActor::SetImfFileName() {
}

// OFFSET: 0x00CC27C0
// FUNCTION: fn_21141
void CGameActor::SetMsg2AI(const float* value, int index) {
  if (index == 0) {
    std::memcpy(this->homunMsg, value, sizeof(this->homunMsg));
  } else if (index == 1) {
    std::memcpy(this->merMsg, value, sizeof(this->merMsg));
  }
}

// OFFSET: 0x00CC2820
// FUNCTION: fn_21143
void CGameActor::SetRenderInfo(const float* bounds, float x, float y) {
  if (this->render.visibilityOptions[1] != 0) {
    // GLOBAL: gl_25185
    static int s_minPickSize = static_cast<int>(g_renderer->UiScaleX() * 40.0f);

    float depth = 0.0f;
    if ((g_pickModifierKey == 0 && g_pickInvert == 0) || (g_pickModifierKey != 0 && g_pickInvert != 0)) {
      if (g_session.GetSiegeMode() != 0) {
        if (IsPlayerJob(this->GetEffectiveJob(-1))) {
          depth = g_session.IsLocalPlayerAid(this->GetGdid()) != 0 ? 100.0f : 400.0f;
        } else if (g_session.IsNpcJob(this->GetEffectiveJob(-1)) == true) {
          depth = 200.0f;
        } else if (g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          depth = this->objectType == 7 ? 50.0f : 400.0f;
        } else if (g_session.IsCompanionJob(this->GetEffectiveJob(-1)) != 0) {
          depth = 200.0f;
        }
      } else {
        if (IsPlayerJob(this->GetEffectiveJob(-1))) {
          depth = 100.0f;
        } else if (g_session.IsNpcJob(this->GetEffectiveJob(-1)) == true) {
          depth = 200.0f;
        } else if (g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          depth = this->objectType == 7 ? 50.0f : 400.0f;
        } else if (g_session.IsCompanionJob(this->GetEffectiveJob(-1)) != 0) {
          depth = 200.0f;
        }
      }
    } else {
      if (g_session.GetSiegeMode() != 0) {
        if (IsPlayerJob(this->GetEffectiveJob(-1))) {
          depth = g_session.IsLocalPlayerAid(this->GetGdid()) != 0 ? 400.0f : 100.0f;
        } else if (g_session.IsNpcJob(this->GetEffectiveJob(-1)) == true) {
          depth = 100.0f;
        } else if (g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          depth = this->objectType == 7 ? 50.0f : 200.0f;
        } else if (g_session.IsCompanionJob(this->GetEffectiveJob(-1)) != 0) {
          depth = 200.0f;
        }
      } else {
        if (IsPlayerJob(this->GetEffectiveJob(-1))) {
          depth = 400.0f;
        } else if (g_session.IsNpcJob(this->GetEffectiveJob(-1)) == true) {
          depth = 100.0f;
        } else if (g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          depth = this->objectType == 7 ? 50.0f : 200.0f;
        } else if (g_session.IsCompanionJob(this->GetEffectiveJob(-1)) != 0) {
          depth = 200.0f;
        }
      }
    }

    switch (this->objectType) {
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      depth = 50.0f;
      break;
    }

    if (!this->modelReady) {
      if (bounds[0] == static_cast<float>(g_renderer->viewport.width) && bounds[1] == static_cast<float>(g_renderer->viewport.height) &&
          bounds[2] == 0.0f && bounds[3] == 0.0f) {
        return;
      }

      ActorPickInfo* info = new ActorPickInfo;
      info->left = std::min(bounds[0], bounds[2]);
      info->top = bounds[1];
      info->depth = depth + bounds[4];
      info->right = std::max(bounds[0], bounds[2]);
      info->bottom = bounds[3];
      info->depthOffset = depth + bounds[4];
      info->accountId = this->render.accountId;
      info->job = this->GetEffectiveJob(-1);
      info->healthState = this->healthState;
      if (this->objectType == 7) {
        info->kind = 3;
      } else {
        info->kind = g_session.IsCompanionJob(this->GetEffectiveJob(-1)) != 0 ? 4 : 0;
      }
      switch (this->objectType) {
      case NPC_MERSOL_TYPE:
      case NPC_ELEMENTAL_TYPE:
      case NPC_ABR_TYPE:
      case NPC_BIONIC_TYPE:
        info->kind = 4;
        break;
      }

      const int width = static_cast<int>(info->right - info->left);
      if (width < s_minPickSize) {
        const float grow = static_cast<float>((s_minPickSize - width) / 2);
        info->right += grow;
        info->left -= grow;
      }
      const int height = static_cast<int>(info->bottom - info->top);
      if (height < s_minPickSize) {
        const float grow = static_cast<float>((s_minPickSize - height) / 2);
        info->bottom += grow;
        info->top -= grow;
      }
      g_actorPickNode.AddPickInfo(info);
    }
  }
  this->SetTlvert(x, y);
}

// OFFSET: 0x00CC8970
// FUNCTION: fn_21194
void CGameActor::PlayGroundSkillEffect(int sender, const int* skill, int argument3, int argument4) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const int level = skill[4];
  const int skillId = skill[0];
  int effectId;
  int motion;
  g_session.GetSkillActionInfo2(skillId, effectId, motion);
  float x;
  float z;
  mode->CellCenter(skill[2], skill[3], x, z);
  const float height = mode->scene->terrain->Height(x, z);
  this->hitTargetAid = 0;

  switch (skillId) {
  case WZ_METEOR:
  case NPC_RAINOFMETEOR:
  case NPC_LOCKON_LASER_ATK:
  case SU_CN_METEOR:
  case SU_CN_METEOR2:
  case AG_VIOLENT_QUAKE_ATK:
  case AG_ALL_BLOOM_ATK:
  case AG_ALL_BLOOM_ATK2:
  case AG_ASTRAL_STRIKE_ATK:
  case AG_CRIMSON_ARROW_ATK: {
    CActRes* action = this->GetActRes();
    this->render.previousAnimation = 0x58;
    this->render.curMotion = action->FrameCount(0x58) - 1;
    break;
  }
  default:
    if (CSkillActionMgr::GetInstance()->UseGroundSkillLastFrame(skillId) == true) {
      this->render.previousAnimation = 0x58;
      CActRes* action = this->GetActRes();
      if (action != nullptr) {
        this->render.curMotion = action->FrameCount(0x58) - 1;
      }
    } else {
      this->Face(x, z);
      this->SetState(motion, 1);
    }
    break;
  }

  if (g_session.PlaySkillGroundEffect(skillId, vector3d{x, height, z}) == true) {
    return;
  }

  vector3d position{x, height, z};
  switch (skillId) {
  case MG_THUNDERSTORM:
    mode->LaunchDummyEffect(&position, 0x1E, EffectLifetime(0x1E) + 10, 0);
    break;
  case PR_BENEDICTIO:
  case ALL_ANGEL_PROTECT:
    mode->LaunchDummyEffect(&position, 0x5B, EffectLifetime(0x5B) + 10, 0);
    break;
  case PR_SANCTUARY:
    mode->LaunchDummyEffect(&position, 0x53, EffectLifetime(0x53) + 10, 0);
    break;
  case PR_MAGNUS:
    mode->LaunchDummyEffect(&position, 0x71, EffectLifetime(0x71) + 10, 0);
    break;
  case WZ_METEOR:
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(0, EF_METEOR_STORM_VISUAL, position.x, position.y, position.z, 0, -1, 0, 0);
    } else {
      mode->LaunchDummyEffect(&position, 0x5C, EffectLifetime(0x5C) + 10, 0);
    }
    break;
  case WZ_VERMILION:
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(0, EF_LORD_OF_VERMILLION_VISUAL, position.x, position.y, position.z, 0, -1, 0, 0);
    } else {
      mode->LaunchDummyEffect(&position, 0x5A, EffectLifetime(0x5A) + 10, 0);
    }
    break;
  case WZ_ICEWALL:
    mode->LaunchDummyEffect(&position, 0x4A, EffectLifetime(0x4A) + 10, 0);
    break;
  case WZ_STORMGUST:
  case NPC_STORMGUST2: {
    const int effect = g_serviceOptions.Get(0xE5) ? 0x2C4 : 0x59;
    mode->LaunchDummyEffect(&position, effect, EffectLifetime(0x59) + 10, 0);
    break;
  }
  case WZ_HEAVENDRIVE:
    mode->LaunchDummyEffect(&position, 0x8E, EffectLifetime(0x8E) + 10, 0);
    break;
  case WZ_QUAGMIRE:
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(0, EF_QUAGMIRE_VISUAL, position.x, position.y, position.z, 0, -1, 0, 0);
    } else {
      mode->LaunchDummyEffect(&position, 0x5F, EffectLifetime(0x5F) + 10, 0);
    }
    break;
  case BS_HAMMERFALL:
    mode->LaunchDummyEffect(&position, 0x66, EffectLifetime(0x66) + 10, 0);
    break;
  case BS_ADRENALINE:
  case BS_ADRENALINE2:
    mode->LaunchDummyEffect(&position, 0x62, EffectLifetime(0x62) + 10, 0);
    break;
  case HT_SKIDTRAP:
    mode->LaunchDummyEffect(&position, 0x45, EffectLifetime(0x45) + 10, 0);
    break;
  case HT_DETECTING:
    mode->LaunchDummyEffect(&position, 0x77, EffectLifetime(0x77) + 10, 0);
    if (this->statusEffects[0] != 0) {
      const std::int64_t address = reinterpret_cast<int>(&position);
      this->statusEffects[0]->SendMsg(nullptr, AM_REQ_ATTACK, address, 0, 0, 0, 0, 0);
    }
    break;
  case AS_VENOMDUST:
    mode->LaunchDummyEffect(&position, 0x7C, EffectLifetime(0x7C) + 10, 0);
    break;
  case SA_VOLCANO:
    mode->LaunchDummyEffect(&position, 0xE1, EffectLifetime(0xE1) + 10, 0);
    break;
  case SA_DELUGE:
    mode->LaunchDummyEffect(&position, 0xEC, EffectLifetime(0xEC) + 10, 0);
    break;
  case SA_VIOLENTGALE:
    mode->LaunchDummyEffect(&position, 0xED, EffectLifetime(0xED) + 10, 0);
    break;
  case SA_LANDPROTECTOR:
    mode->LaunchDummyEffect(&position, 0xEE, EffectLifetime(0xEE) + 10, 0);
    break;
  case CR_SLIMPITCHER: {
    g_soundMgr->PlayWave("assulter_attack.wav", 0.0f, -150.0f, 0.0f, 250, 40, 1.0f, 0);
    int effect;
    if (level < 6) {
      effect = 0x1F1;
    } else if (level < 10) {
      effect = 0x1F2;
    } else {
      effect = 0x1F3;
    }
    mode->LaunchDummyEffect(&position, effect, EffectLifetime(effect) + 10, 0);
    break;
  }
  case HW_GANBANTEIN:
    g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    mode->LaunchDummyEffect(&position, 0xDF, EffectLifetime(0xDF) + 10, 0);
    break;
  case HW_GRAVITATION:
    mode->LaunchDummyEffect(&position, 0x20A, 0x104, 0);
    g_soundMgr->PlayWave("effect\\wizard_earthspike.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case CR_CULTIVATION:
    this->LaunchEffect(EF_BEGINSPELL6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case GS_GROUNDDRIFT:
    g_soundMgr->PlayWave("effect\\그라운드.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case NJ_SUITON:
    g_soundMgr->PlayWave("effect\\수둔.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case NPC_ICEEXPLO:
    this->LaunchEffect(EF_ICECRASH, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\ef_frostdiver.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case NPC_MAXPAIN_ATK:
    this->LaunchEffect(EF_MAXPAIN, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case NPC_MAGMA_ERUPTION:
  case NC_MAGMA_ERUPTION:
    mode->LaunchDummyEffect(&position, 0x66, EffectLifetime(0x66) + 10, 0);
    break;
  case NPC_RAYOFGENESIS:
  case LG_RAYOFGENESIS:
    g_effectMgr->Spawn(0, EF_NEW_RAYOFGENESIS, position.x, position.y, position.z, 0, -1, 0, 0);
    g_soundMgr->PlayWave("effect\\LG_RAYOFGENESIS.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case NPC_RAINOFMETEOR: {
    const float raised = height + 25.0f;
    g_effectMgr->Spawn(0, EF_NEW_METEORSTORM_BOTTOM, x, raised, z, 0, -1, 0, 0);
    g_effectMgr->Spawn(0, EF_NEW_METEORSTORM, x, raised, z, 0, -1, 0, 0);
    g_soundMgr->PlayWave("effect\\wizard_meteor.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  }
  case NPC_GROUNDDRIVE:
    mode->LaunchDummyEffect(&position, 0x577, EffectLifetime(0x577) + 10, 0);
    break;
  case NPC_LOCKON_LASER_ATK: {
    CEffect* effect = g_effectMgr->Spawn(0, EF_HAMMER_OF_GOD, position.x, position.y, position.z, 0, -1, 0, 0);
    if (effect != nullptr) {
      using SetFloat = void (CEffect::*)(float);
      const SetFloat set = reinterpret_cast<SetFloat>(&CEffect::AdvanceFrame);
      (effect->*set)(0.4f);
    }
    break;
  }
  case NC_COLDSLOWER: {
    CRagEffect* effect = this->LaunchEffect(EF_COLDTHROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  }
  case SO_DIAMONDDUST:
    g_soundMgr->PlayWave("effect\\wl_jackfrost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(0, EF_MINEFFECT_DIAMONDDUST, position.x, position.y, position.z, 0, level, 0, 0);
      g_effectMgr->Spawn(0, EF_MINEFFECT_DIAMONDDUST_BOTTOM, position.x, position.y, position.z, 0, level, 0, 0);
    } else {
      g_effectMgr->Spawn(0, EF_NEW_DIAMONDDUST, position.x, position.y, position.z, 0, level, 0, 0);
      g_effectMgr->Spawn(0, EF_NEW_DIAMONDDUST_BOTTOM, position.x, position.y, position.z, 0, level, 0, 0);
    }
    break;
  case RL_HAMMER_OF_GOD:
    g_effectMgr->Spawn(0, EF_HAMMER_OF_GOD, position.x, position.y, position.z, 0, -1, 0, 0);
    break;
  case SU_CN_METEOR:
    g_effectMgr->Spawn(0, 0x452, position.x, position.y, position.z, 0, -1, 0, 0);
    break;
  case SU_CN_METEOR2:
    g_effectMgr->Spawn(0, 0x458, position.x, position.y, position.z, 0, -1, 0, 0);
    break;
  case AG_VIOLENT_QUAKE_ATK:
    g_effectMgr->Spawn(0, EF_VIOLENTQUAKE, position.x, position.y, position.z, 0, -1, 0, 0);
    break;
  case AG_STRANTUM_TREMOR:
  case AG_TORNADO_STORM:
  case AG_FLORAL_FLARE_ROAD:
    this->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
    break;
  case AG_ALL_BLOOM_ATK:
    g_effectMgr->Spawn(0, 0x706, position.x, position.y, position.z, 0, -1, 0, 0);
    break;
  case ABC_ABYSS_STRIKE: {
    g_effectMgr->Spawn(0, EF_ABYSS_STRIKE, position.x, position.y, position.z, 0, -1, 0, 0);
    g_effectMgr->Spawn(0, EF_ABYSS_STRIKE_BOTTOM, position.x, position.y, position.z, 0, -1, 0, 0);
    CEffect* effect = g_effectMgr->Spawn(0, EF_ABYSS_STRIKE_GROUND, position.x, position.y, position.z, 0, -1, 0, 0);
    if (effect != nullptr) {
      effect->SetDelay(0x370);
    }
    effect = g_effectMgr->Spawn(0, EF_ABYSS_STRIKE_GROUND_BOTTOM, position.x, position.y, position.z, 0, -1, 0, 0);
    if (effect != nullptr) {
      effect->SetDelay(0x370);
    }
    break;
  }
  case ABC_ABYSS_SQUARE:
    this->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 500, 0);
    break;
  case WH_DEEPBLINDTRAP:
  case WH_SOLIDTRAP:
  case WH_SWIFTTRAP:
  case WH_FLAMETRAP:
    this->SetFrameSequence(0x18, 0, 2, 0x60, 2, 1, 500, 0);
    break;
  }
}

// OFFSET: 0x00CC2D50
// FUNCTION: fn_21145
void CGameActor::SetResMsg2AI(const float* value, int index) {
  if (index == 0) {
    std::memcpy(this->homunResMsg, value, sizeof(this->homunResMsg));
  } else if (index == 1) {
    std::memcpy(this->merResMsg, value, sizeof(this->merResMsg));
  }
}

// OFFSET: 0x00CC2DC0
// FUNCTION: fn_21147
void CGameActor::SetSprAccessory(int) {
}

// OFFSET: 0x00CC2DD0
// FUNCTION: fn_21148
void CGameActor::SetSprAct(int, int, int) {
}

// OFFSET: 0x00CC2DE0
// FUNCTION: fn_21149
void CGameActor::SetSprHead(int) {
}

// OFFSET: 0x00CC2DF0
// FUNCTION: fn_21150
void CGameActor::SetSprJob(int) {
}

// OFFSET: 0x00CC2E40
// FUNCTION: fn_21152
void CGameActor::SetSprWeapon(int, int) {
}

// INLINED
static bool HasEffectOfType(const std::list<CRagEffect*>& effects, int type) {
  for (CRagEffect* effect : effects) {
    if (effect && effect->state.type == type) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CC2E50
// FUNCTION: fn_21153
void CGameActor::SetState(int state, int resetPosition) {
  g_session.m_homunActionState = 0;
  g_session.m_mercenaryActionState = 0;

  if (this->render.state == 3 || this->isTrickDead != 0 || state == -1) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (this->render.state == 4 && static_cast<unsigned char>(resetPosition) == 1) {
    this->render.position.x = this->damageDestX;
    this->render.position.z = this->damageDestZ;
    this->render.position.y = mode->scene->terrain->Height(this->damageDestX, this->damageDestZ);
  }

  const std::uint32_t aid = this->render.accountId;
  bool isPlayer;
  if (g_session.m_aid != aid && this->GetGdid() >= 1 && g_session.IsPlayerJobId(this->GetEffectiveJob(-1))) {
    isPlayer = true;
  } else {
    isPlayer = false;
  }

  if (state != static_cast<int>(this->render.state) && this->render.state == 1 && isPlayer) {
    CellFloatPos pos;
    pos.x = this->render.position.x;
    pos.z = this->render.position.z;
    mode->scene->AddPcPos(aid, pos);
  }

  int action;
  if (this->GetMonsterTransformJob() > 0) {
    action = this->ActionBase(state);
  } else {
    action = this->WeaponActionBase(state);
  }

  switch (state) {
  case ST_AUTOCOUNTER:
    if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1))) {
      const std::int64_t weapon = this->GetWeapon();
      IsJobWeaponAllowed(this->GetEffectiveJob(-1), this->sex, weapon);
      this->SetAction(action, 4, 1);
      this->render.isMotionFreezed = true;
    } else {
      this->SetAction(16, 4, 1);
      this->render.isMotionFreezed = true;
    }
    break;
  case ST_DAMAGE: {
    if (this->render.frameSequenceActive != 0 || this->render.animationSequenceActive != 0 || this->render.timedSequenceActive != 0) {
      return;
    }

    CRenderObject* target;
    if (this->hitTargetAid == g_session.m_aid) {
      target = mode->scene->player;
    } else {
      target = mode->scene->GetGameActorByAID(this->hitTargetAid);
    }
    if (target) {
      target->SendMsg(nullptr, AM_DELETE_MAGIC_TARGET, 0, 0, 0, 0, 0, 0);
    }
    this->SetAction(action, 5, 1);
    const int job = this->GetEffectiveJob(-1);
    if (job != 0x8d && job != 0xe6) {
      this->RegisterPos();
    }
    break;
  }
  case ST_SKILL:
    if (this->render.state == 6) {
      return;
    }
  case ST_ATTACK:
  case ST_ATTACK2:
    this->SetAction(action, 3, this->shapeShift == 1 ? 2 : 1);
    if (this->render.state != 9 && this->render.state != 2) {
      if (this->hitTargetAid == g_session.m_aid) {
        CRenderObject* target = mode->scene->player;
        if (this->render.accountId != this->hitTargetAid) {
          this->Face(target->render.position.x, target->render.position.z);
        }
      } else {
        CRenderObject* target = mode->scene->GetGameActorByAID(this->hitTargetAid);
        if (target) {
          this->Face(target->render.position.x, target->render.position.z);
        }
      }
    }
    if (this->GetEffectiveJob(-1) == 0x505) {
      this->LaunchAttackProjectile(this->hitTargetAid, 28.0f);
    }
    this->attackMotion = static_cast<float>(this->GetAttackMotion());
    this->RegisterPos();
    break;
  case ST_DEAD:
    this->render.frameSequenceActive = 0;
    this->render.animationSequenceActive = 0;
    this->render.timedSequenceActive = 0;
    this->render.isMotionFreezed = false;
    if (this->bodyState == BODYSTATE_FREEZING) {
      this->SendMsg(nullptr, AM_DELETE_ICEEFFECT, 0, 0, 0, 0, 0, 0);
      this->SendMsg(nullptr, AM_MAKE_ICEBREAKE_FFECT, 0, 0, 0, 0, 0, 0);
    }
    this->render.isLieOnGround = true;
    this->render.isMotionFinished = false;
    this->render.isMotionFreezed = false;
    this->SetAction(action, 4, 1);
    break;
  case ST_STAND:
    this->SetAction(action, 4, 0);
    if (!IsObfuscatedMessage(this->render.accountId)) {
      this->render.isMotionFinished = true;
    }
    this->RegisterPos();
    break;
  case ST_MOVE:
    if (this->isAsuraAttack == 0) {
      this->SetAction(action, 6, 0);
    }
    this->UnRegisterPos();
    break;
  case ST_PICKUP:
    this->isCounter = 0;
    this->render.isMotionFreezed = false;
  case ST_ATTACKWAIT:
    this->SetAction(action, 6, 1);
    break;
  case ST_THROW:
    this->SetAction(action, 3, 1);
    this->RegisterPos();
    break;
  case ST_DANCE:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 1, 3, 0, 0, 1, 99999999, 1);
    break;
  case ST_SING:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 1, 3, action, 1, 1, 99999999, 1);
    break;
  case ST_READYSHOT:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 0, 0, action, 0, 1, 3000, 0);
    break;
  case ST_THROW2:
    this->render.sequenceInterval = 10;
    this->SetAnimationSequence(action, 0, action, 1, action, 2, action, 3, action, 4, 1, 1000, 0);
    break;
  case ST_READYJUMP:
    if (g_session.IsDoramJob(this->GetEffectiveJob(-1)) == true) {
      this->render.sequenceInterval = 10;
      this->SetAnimationSequence(action, 0, action, 0, action, 0, action, 1, action, 1, 1, 3000, 0);
    } else {
      this->render.sequenceInterval = 10;
      this->SetAnimationSequence(action, 0, action, 0, action, 0, action, 1, action, 1, 1, 3000, 0);
      while (HasEffectOfType(this->state.effects, EF_LANDBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LANDBODY, 0, 0, 0, 0, 0);
      }
      while (HasEffectOfType(this->state.effects, EF_JUMPBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_JUMPBODY, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_JUMPBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_PEONG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ST_LANDJUMP:
    this->render.sequenceInterval = 10;
    this->SetAnimationSequence(action, 1, action, 1, action, 1, action, 1, 8, 5, 1, 3000, 0);
    break;
  case ST_R_STORM:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 0, 0, action, 0, 1, 2000, 0);
    this->LaunchEffect(EF_TAE_READY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case ST_A_STORM:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 1, 1, action, 1, 1, 2000, 0);
    break;
  case ST_R_DOWN:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 2, 2, action, 2, 1, 2000, 0);
    this->LaunchEffect(EF_TAE_READY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case ST_SG_DOWN:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 2, 2, action, 2, 1, 1000, 0);
    break;
  case ST_A_DOWN:
    this->render.sequenceInterval = 5;
    this->SetAnimationSequence(0x50, 3, 0x50, 5, 0x50, 7, 0x50, 8, 0x50, 2, 1, 500, 0);
    break;
  case ST_R_TURN:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 3, 3, action, 3, 1, 2000, 0);
    this->LaunchEffect(EF_TAE_READY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case ST_SG_TURN:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 3, 3, action, 3, 1, 1000, 0);
    break;
  case ST_A_TURN:
    this->render.sequenceInterval = 5;
    this->SetAnimationSequence(action, 4, action, 5, action, 6, action, 7, action, 2, 1, 500, 0);
    break;
  case ST_R_COUNTER:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 4, 4, action, 4, 1, 2000, 0);
    this->LaunchEffect(EF_TAE_READY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case ST_A_COUNTER: {
    this->render.sequenceInterval = 4;
    int frame0;
    int frame1;
    int frame2;
    int frame3;
    int frame4;
    if (this->GetEffectiveJob(-1) == 0xfd1 || this->GetEffectiveJob(-1) == 0x1083 || this->GetEffectiveJob(-1) == 0x1090 ||
        this->GetEffectiveJob(-1) == 0x1092) {
      frame0 = 4;
      frame1 = 5;
      frame2 = 6;
      frame3 = 7;
      frame4 = 2;
    } else {
      frame0 = 2;
      frame1 = 3;
      frame2 = 4;
      frame3 = 4;
      frame4 = 4;
    }
    this->SetAnimationSequence(action, frame0, action, frame1, action, frame2, action, frame3, action, frame4, 1, 500, 0);
    break;
  }
  case ST_R_DODGE:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 1, 1, action, 1, 1, 2000, 0);
    this->LaunchEffect(EF_TAE_READY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case ST_A_JUMPKICK:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 5, 5, action, 5, 1, 3000, 0);
    break;
  case ST_NINJASKILL2:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 2, 5, action, 5, 1, 1000, 0);
    break;
  case 0x37:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 2, 4, action, 4, 1, 500, 0);
    break;
  case 0x38:
    this->render.isMotionFinished = false;
  case ST_NINJASKILL1:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 1, action, 1, 1, 500, 0);
    break;
  case 0x39:
    this->render.isMotionFinished = false;
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 5, 5, action, 5, 1, 500, 0);
    break;
  case ST_NINJAREADY: {
    this->render.sequenceInterval = 10;
    int random = rand() % 2;
    random = random < 0 ? 0 : random;
    const int frame = (random != 0) + 2;
    this->SetFrameSequence(action, frame, frame, action, frame, 1, 500, 0);
    break;
  }
  case ST_GUNSKILL1: {
    this->render.sequenceInterval = 6;
    int random = rand() % 2;
    random = random < 0 ? 0 : random;
    if (random != 0) {
      this->SetAnimationSequence(action, 3, action, 2, action, 1, action, 0, action, 0, 1, 1000, 0);
    } else {
      this->SetFrameSequence(action, 0, 3, action, 3, 1, 1000, 0);
    }
    break;
  }
  case ST_GUNSKILL3:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 2, action, 2, 1, 500, 0);
    break;
  case ST_GUNREADY: {
    this->render.sequenceInterval = 10;
    int random = rand() % 2;
    random = random < 0 ? 0 : random;
    const int frame = (random != 0) + 1;
    this->SetFrameSequence(action, frame, frame, action, frame, 1, 500, 0);
    break;
  }
  case ST_GUNREADY2:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 0, 0, action, 0, 1, 500, 0);
    break;
  case ST_MADOSKILL:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 5, 5, action, 5, 1, 500, 0);
    break;
  case ST_MADOSKILL2:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 4, 4, action, 4, 1, 500, 0);
    break;
  case ST_AB_SPECIAL:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x3a:
  case 0x3b:
    this->SetAction(action, 3, 1);
    break;
  case 0x2e:
    this->render.sequenceInterval = 20;
    this->SetAnimationSequence(action, 0, action, 0, action, 0, action, 1, action, 1, 1, 3000, 0);
    while (HasEffectOfType(this->state.effects, EF_LANDBODY)) {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LANDBODY, 0, 0, 0, 0, 0);
    }
    while (HasEffectOfType(this->state.effects, EF_JUMPBODY1)) {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_JUMPBODY1, 0, 0, 0, 0, 0);
    }
    this->LaunchEffect(EF_JUMPBODY1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    this->LaunchEffect(EF_PEONG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2f:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 3, action, 3, 1, 9999999, 1);
    break;
  case ST_GUNSKILL2:
  case 0x30:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 4, 5, action, 5, 1, 500, 0);
    break;
  case 0x31:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 7, action, 7, 1, 1000, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x32:
    this->render.sequenceInterval = 10;
    this->SetFrameSequence(action, 0, 0, action, 0, 1, 9999999, 0);
    break;
  case 0x33:
    this->render.sequenceInterval = 4;
    this->SetAnimationSequence(action, 2, action, 3, action, 4, action, 4, action, 4, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x3e:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 7, action, 7, 1, 500, 0);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x3f:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 7, action, 7, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x40:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 4, action, 4, 1, 500, 0);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x41:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 4, action, 4, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x3c:
  case 0x3d:
  case 0x42:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 0, action, 0, 1, 500, 0);
    break;
  case 0x43:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 5, action, 5, 1, 500, 0);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x44:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 5, action, 5, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x45:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 8, action, 8, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x46:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 7, action, 7, 1, 500, 0);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x4c:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 7, action, 7, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x47:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 4, 8, action, 8, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x48:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 4, action, 4, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x49:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 3, 7, action, 7, 1, 500, 0);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x4d:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 3, 7, action, 7, 1, 500, 0);
    this->render.isMotionFinished = false;
    break;
  case 0x4b:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 2, 5, action, 5, 1, 500, 1);
    this->render.isMotionFinished = false;
    break;
  case 0x4e:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 4, action, 4, 1, 500, 1);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x4f:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 1, 8, action, 4, 1, 500, 1);
    this->render.isMotionFinished = false;
    this->attackMotion = 6.0f;
    break;
  case 0x50:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 0, 5, action, 5, 1, 500, 1);
    this->render.isMotionFinished = false;
    break;
  case 0x51:
    this->render.sequenceInterval = 6;
    this->SetFrameSequence(action, 5, 5, action, 5, 1, 9999999, 0);
    break;
  }

  this->render.previousAnimation = this->Get8Dir(this->render.angle) + this->render.animation;
  const int previous = this->render.state;
  if (previous == 0 || previous == 6) {
    this->render.previousState = previous;
  }
  this->render.state = state;
  if (state == 7 || state == 0x31) {
    this->render.state = 7;
  }
  this->render.motionStartTime = timeGetTime();
}

// OFFSET: 0x00CA9E50
// FUNCTION: fn_21013
void CRenderObject::RenderBlur(
  const SpriteFrameCell* cell, const int* box, float offset, float bottomRhw, float topRhw, const std::uint32_t* palette,
  const SpriteRleImage* image, float scale, short shift
) {
  if ((this->render.flags & 0x1000) == 0) {
    return;
  }

  bottomRhw += 0.1f;
  topRhw += 0.1f;

  SpriteQuad quad;
  for (int direction = 1; direction < 5; ++direction) {
    const float distance = static_cast<float>(this->render.effectPhase) * scale;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    short alpha = static_cast<short>((30 - this->render.effectPhase) * 5);
    if (alpha < 0) {
      alpha = 0;
    }

    const float spread = 400.0f / mode->camera->camera.desiredDistance * distance;
    if (direction == 1) {
      quad.top = static_cast<float>(box[1]);
      quad.left = static_cast<float>(box[0]) + offset + spread;
      quad.bottom = static_cast<float>(box[3]);
      quad.right = static_cast<float>(box[2]) + offset + spread;
    } else if (direction == 2) {
      quad.top = static_cast<float>(box[1]);
      quad.left = static_cast<float>(box[0]) + offset - spread;
      quad.bottom = static_cast<float>(box[3]);
      quad.right = static_cast<float>(box[2]) + offset - spread;
    } else if (direction == 3) {
      quad.top = static_cast<float>(box[1]) + spread;
      quad.bottom = static_cast<float>(box[3]) + spread;
      quad.left = static_cast<float>(box[0]) + offset;
      quad.right = static_cast<float>(box[2]) + offset;
    } else if (direction == 4) {
      quad.top = static_cast<float>(box[1]) - spread;
      quad.bottom = static_cast<float>(box[3]) - spread;
      quad.left = static_cast<float>(box[0]) + offset;
      quad.right = static_cast<float>(box[2]) + offset;
    }

    quad.color = (alpha << 24) + 0xFFFFFF;
    quad.image = image;
    quad.top -= static_cast<float>(shift);
    quad.bottom -= static_cast<float>(shift);
    quad.bottomRhw = bottomRhw;
    quad.topRhw = topRhw;
    quad.angle = static_cast<float>(cell->rotation + static_cast<int>(this->render.spriteAngle));
    this->DrawQuad(quad, palette, 0x201, 0.0f, 0.0f, 0);
  }
}

// OFFSET: 0x00CAA1F0
// FUNCTION: fn_21015
void CRenderObject::ApplySpriteJitter(SpriteQuad* quad, const int* input, float offset, int spread, int base) {
  if ((this->render.flags & 0x2000) == 0) {
    return;
  }

  float jitter[4];
  for (int index = 0; index < 4; ++index) {
    int value;
    if (spread == 0) {
      value = 0;
    } else {
      const int roll = rand() % spread;
      value = roll >= 0 ? roll : 0;
    }

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    jitter[index] = 400.0f / mode->camera->camera.desiredDistance * static_cast<float>(value - base);
  }

  quad->top = static_cast<float>(input[1]) + jitter[0];
  quad->bottom = static_cast<float>(input[3]) + jitter[1];
  quad->left = static_cast<float>(input[0]) + offset + jitter[2];
  quad->right = static_cast<float>(input[2]) + offset + jitter[3];
}

// OFFSET: 0x00CAA2E0
// FUNCTION: fn_21016
void CRenderObject::RenderAura(
  SpriteQuad* quad, const SpriteFrameCell* cell, const int* box, float offset, float bottomRhw, float topRhw,
  const std::uint32_t* palette, const SpriteRleImage* image, int blend, short shift
) {
  if ((this->render.flags & 0x200) == 0) {
    return;
  }

  unsigned char& phaseTick = this->render.effectPhaseBytes[3];
  ++phaseTick;
  if (phaseTick >= 200) {
    phaseTick -= 200;
  }

  for (int step = 5; step < 25; step += 5) {
    float phase = static_cast<float>(phaseTick) * 0.1f + static_cast<float>(step);
    if (phase >= 20.0f) {
      phase -= 20.0f;
    }

    const std::uint16_t alpha = static_cast<std::uint16_t>(100.0f - phase * 5.0f);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const float spread = 400.0f / mode->camera->camera.desiredDistance * phase;

    SpriteQuad trail;
    trail.top = static_cast<float>(box[1]) - spread - static_cast<float>(shift);
    trail.bottom = static_cast<float>(box[3]) + spread - static_cast<float>(shift);
    float left = static_cast<float>(box[0]) + offset;
    float right = static_cast<float>(box[2]) + offset;
    if (left > right) {
      left += spread;
      right -= spread;
    } else {
      left -= spread;
      right += spread;
    }
    trail.left = left;
    trail.right = right;
    trail.image = image;
    trail.color = (static_cast<short>(alpha) << 24) + 0xFFFFFF;
    trail.bottomRhw = bottomRhw - spread * 0.000005f - 0.00001f;
    trail.topRhw = topRhw - spread * 0.000005f - 0.00001f;
    trail.angle = static_cast<float>(static_cast<int>(this->render.spriteAngle) + cell->rotation);
    if (blend != 5) {
      this->DrawQuad(trail, palette, blend, 0.0f, 0.0f, 0);
    }
  }

  quad->color = 0x96FFFFFF;
}

// OFFSET: 0x00CAA0A0
// FUNCTION: fn_21014
void CRenderObject::DrawPulseOutline(
  const SpriteFrameCell& cell, const int* box, float offset, float bottomRhw, float topRhw,
  const std::uint32_t* palette, const SpriteRleImage* image, std::uint32_t color
) {
  if ((this->render.flags & 8) == 0) {
    return;
  }

  unsigned char& phase = this->render.effectPhaseBytes[2];
  ++phase;
  if (phase > 180) {
    phase = 0;
  }

  const float pulse = g_sinTable[phase] * 1.5f + 5.0f;
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float spread = 400.0f / mode->camera->camera.desiredDistance * pulse;

  SpriteQuad outline;
  outline.top = static_cast<float>(box[1]) - spread;
  outline.bottom = static_cast<float>(box[3]) + spread;
  float left = static_cast<float>(box[0]) + offset;
  float right = static_cast<float>(box[2]) + offset;
  if (left > right) {
    left += spread;
    right -= spread;
  } else {
    left -= spread;
    right += spread;
  }
  outline.right = right;
  outline.left = left;
  outline.bottomRhw = bottomRhw - 0.00005f;
  outline.topRhw = topRhw - 0.00005f;
  outline.image = image;
  outline.color = color;
  outline.angle = static_cast<float>(cell.rotation + static_cast<int>(this->render.spriteAngle));
  this->DrawQuad(outline, palette, 5, 0.0f, 0.0f, 0);
}

// OFFSET: 0x00CAB180
// FUNCTION: fn_21025
void CRenderObject::DrawSpriteEffects(
  SpriteQuad* quad, const SpriteFrameCell* cell, const int* box, float offset, float bottomRhw, float topRhw,
  const std::uint32_t* palette, const SpriteRleImage* image, int* blend, std::uint32_t color
) {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == 0xB8) {
      if (!this->render.visible || !this->render.enabled) {
        return;
      }
      break;
    }
  }

  if (static_cast<unsigned char>(color >> 24) < 0xFF) {
    *blend = 1;
  }
  if ((this->render.flags & 1) != 0) {
    *blend = 5;
  }

  this->DrawPulseOutline(*cell, box, offset, bottomRhw, topRhw, palette, image, color);

  if ((this->render.flags & 0x800) != 0) {
    short step = 1;
    for (int distance = 3; distance <= 30; distance += 3, ++step) {
      short alpha;
      if (this->render.effectPhase > 130) {
        alpha = 245 - step * 10 - this->render.effectPhase;
      } else {
        alpha = this->render.effectPhase - (step * 5 + 50);
      }
      if (alpha > 200) {
        alpha = 200;
      }
      if (alpha < 0) {
        alpha = 0;
      }

      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const float spread = 400.0f / mode->camera->camera.desiredDistance * static_cast<float>(distance);

      SpriteQuad shadow;
      shadow.top = static_cast<float>(box[1]) - spread;
      shadow.bottom = static_cast<float>(box[3]) + spread;
      float left = static_cast<float>(box[0]) + offset;
      float right = static_cast<float>(box[2]) + offset;
      if (left > right) {
        left += spread;
        right -= spread;
      } else {
        left -= spread;
        right += spread;
      }
      shadow.left = left;
      shadow.right = right;
      shadow.image = image;
      shadow.color = (alpha << 24) + 0xFFFFFF;
      shadow.bottomRhw = bottomRhw + spread * 0.000005f + 0.00001f;
      shadow.topRhw = topRhw + spread * 0.000005f + 0.00001f;
      shadow.angle = static_cast<float>(static_cast<int>(this->render.spriteAngle) + cell->rotation);
      this->DrawQuad(shadow, palette, 5, 0.0f, 0.0f, 0);
    }
  }

  if ((this->render.flags & 0x80002) != 0) {
    for (int distance = 1; distance <= 2; ++distance) {
      const unsigned char alpha = this->render.outlineAlpha;
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const float spread = 400.0f / mode->camera->camera.desiredDistance * static_cast<float>(distance);

      SpriteQuad glow;
      glow.top = static_cast<float>(box[1]) - spread;
      glow.bottom = static_cast<float>(box[3]) + spread;
      float left = static_cast<float>(box[0]) + offset;
      float right = static_cast<float>(box[2]) + offset;
      if (left > right) {
        left += spread;
        right -= spread;
      } else {
        left -= spread;
        right += spread;
      }
      glow.left = left;
      glow.right = right;
      glow.image = image;
      glow.bottomRhw = bottomRhw + spread * 0.000005f + 0.00001f;
      glow.topRhw = topRhw + spread * 0.000005f + 0.00001f;
      if ((this->render.flags & 2) != 0) {
        glow.color = (alpha << 24) + 0x3737FF;
      } else {
        glow.color = (alpha << 24) + 0x059B05;
      }
      glow.angle = static_cast<float>(static_cast<int>(this->render.spriteAngle) + cell->rotation);
      this->DrawQuad(glow, palette, 5, 0.0f, 0.0f, 0);
    }
  }

  CGameActor* actor = static_cast<CGameActor*>(this);
  actor->RenderBlur(cell, box, offset, bottomRhw, topRhw, palette, image, static_cast<float>(box[3] - box[1]) * 0.05f, 0);
  actor->RenderAura(quad, cell, box, offset, bottomRhw, topRhw, palette, image, *blend, 0);
  actor->ApplySpriteJitter(quad, box, offset, 21, 10);

  if ((this->render.flags & 0x100000) != 0) {
    short angle = static_cast<short>(cell->rotation) + static_cast<short>(this->render.spriteAngle) + 60;
    if (angle >= 360) {
      angle -= 360;
    }
    quad->angle = static_cast<float>(angle);
  }
  if ((this->render.flags & 2) != 0) {
    quad->color = 0x643737FF;
  }
}

// OFFSET: 0x00CAFEE0
// FUNCTION: fn_21090
void CGameActor::ProcessBodyState() {
  switch (this->bodyState) {
  case BODYSTATE_FREEZING:
  case BODYSTATE_STONECURSE:
    this->render.isMotionFreezed = true;
    break;
  }
}

// OFFSET: 0x005A1280
// FUNCTION: fn_03042
void CGameActor::SetSex(int value) {
  this->SetSexBit(value);
}

// OFFSET: 0x005A1290
// FUNCTION: fn_03043
int CGameActor::GetBodyCostume() {
  return 0;
}

// OFFSET: 0x005A12A0
// FUNCTION: fn_03044
std::uint32_t CGameActor::GetStateFlags() {
  return this->stateFlags;
}

// OFFSET: 0x005A12B0
// FUNCTION: fn_03045
int CGameActor::GetGdid() {
  return this->guildId;
}

// OFFSET: 0x005A12C0
// FUNCTION: fn_03046
float* CGameActor::GetExtent(float* extent) const {
  extent[0] = 0.0f;
  extent[1] = 0.0f;
  return extent;
}

// OFFSET: 0x005A12E0
// FUNCTION: fn_03047
bool CGameActor::IsArrowEffectSuppressed() {
  return false;
}

// OFFSET: 0x005A12F0
// FUNCTION: fn_03048
int CGameActor::GetNpcLocationRadius() {
  int radius = 0;
  g_session.m_hotkeyScript->Call("GetNpcLocationRadius", "d>d", this->GetEffectiveJob(-1), &radius);
  return radius;
}

// OFFSET: 0x005A13B0
// FUNCTION: fn_03049
int CGameActor::GetMadogearType() {
  return 0;
}

// OFFSET: 0x005A13C0
// FUNCTION: fn_03050
int CGameActor::GetPkState() {
  return 0;
}

// OFFSET: 0x005A13D0
// FUNCTION: fn_03051
std::int64_t CGameActor::GetWeapon() {
  return -1;
}

// OFFSET: 0x005A1930
// FUNCTION: fn_45454
void CGameActor::SetBodyCostume(int, int) {
}

// OFFSET: 0x005A1960
// FUNCTION: fn_03054
void CGameActor::SetGdid(int value) {
  this->guildId = value;
}

// OFFSET: 0x005A1990
// FUNCTION: fn_03057
void CGameActor::SetHonor(int) {
}

// OFFSET: 0x005A19B0
// FUNCTION: fn_03059
void CGameActor::SetArrowEffectSuppressed(bool) {
}

// OFFSET: 0x005A19C0
// FUNCTION: fn_03060
void CGameActor::SetMadogearType(int) {
}

// OFFSET: 0x005A19D0
// FUNCTION: fn_03061
void CGameActor::SetPropensityInfo(int, int) {
}

// OFFSET: 0x005A19E0
// FUNCTION: fn_03062
void CGameActor::SetRiding(unsigned char value) {
  this->isRiding = value;
}

// OFFSET: 0x005A1A40
// FUNCTION: fn_03068
void CGameActor::RefreshAccessory3() {
}

// OFFSET: 0x005A1A70
// FUNCTION: fn_03071
void CGameActor::SetSprRobe(int, int) {
}

// OFFSET: 0x005A1A80
// FUNCTION: fn_03072
void CGameActor::SetSprShield(int) {
}

// OFFSET: 0x005A1AA0
// FUNCTION: fn_03074
void CGameActor::SetSprWeapon2(int, int) {
}

// OFFSET: 0x00CABD40
// FUNCTION: fn_21037
int CGameActor::GetAttackMotion() {
  const int count = static_cast<int>(this->render.action->FrameCount(this->render.previousAnimation));

  for (int frame = 0; frame < count; ++frame) {
    const int sound = this->render.action->Frame(this->render.previousAnimation, frame).sound;

    if (sound != -1 && std::strcmp(this->render.action->Sound(sound), "atk") == 0) {
      return frame;
    }
  }

  return count - (count != 1 ? 2 : 1);
}

// OFFSET: 0x00CACEF0
// FUNCTION: fn_21048
float* CGameActor::GetMsg2AI(int index) {
  if (index == 0) {
    return this->homunMsg;
  }
  if (index == 1) {
    return this->merMsg;
  }
  return nullptr;
}

// OFFSET: 0x00CAD480
// FUNCTION: fn_21052
float* CGameActor::GetResMsg2AI(int index) {
  if (index == 0) {
    return this->homunResMsg;
  }
  if (index == 1) {
    return this->merResMsg;
  }
  return nullptr;
}

// OFFSET: 0x00CADB70
// FUNCTION: fn_21061
bool CGameActor::HasShapeShift() {
  return this->shapeShift != 0;
}

// OFFSET: 0x00CAE500
// FUNCTION: fn_21074
void CGameActor::MakeCorpse() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float angle = this->render.angle;
  const int motion = this->render.curMotion;
  const int animation = this->render.animation;
  const char* sprite = !this->render.sprite ? nullptr : this->render.sprite->GetName();
  const char* action = !this->render.action ? nullptr : this->render.action->GetName();
  const float x = this->render.position.x;
  const float y = this->render.position.y;
  const float z = this->render.position.z;
  CCorpse* corpse = mode->SpawnCorpse(x, y, z, action, sprite, animation, motion, angle);

  if (corpse) {
    if (static_cast<int>(this->render.spriteJob) > 0) {
      corpse->render.spriteJob = this->render.spriteJob;
    }

    if (CEffect* effect = g_effectMgr->Find(static_cast<int>(this->render.accountId), 2374)) {
      corpse->spriteOffset = effect->scaleParameter;
    }
  }
}

// OFFSET: 0x00CAE640
// FUNCTION: fn_21076
void CGameActor::ShowPartyMemberEffect(std::uint32_t accountId) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode->scene->isPKZone == 0 && mode->scene->isEventPVPMode == 0) {
    return;
  }

  for (int index = 0; index < g_session.PartyCount(); ++index) {
    MessengerMember member = g_session.PartyAt(index);
    if (member.accountId == accountId) {
      for (CRagEffect* effect : this->state.effects) {
        if (effect && effect->state.type == 160) {
          return;
        }
      }

      this->LaunchEffect(EF_PARTY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      return;
    }
  }
}

// OFFSET: 0x00CAE790
// FUNCTION: fn_21077
void CGameActor::LaunchAttackProjectile(int target, float scale) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  switch (this->GetEffectiveJob(-1)) {
  case JT_ARCHER_SKELETON:
  case JT_ORC_ARCHER:
  case JT_GARGOYLE:
  case JT_GOBLIN_ARCHER:
  case JT_RAYDRIC_ARCHER:
  case JT_KOBOLD_ARCHER:
  case JT_ARCHER_GUARDIAN:
  case JT_LIVE_PEACH_TREE:
  case JT_EVIL_CLOUD_HERMIT:
  case JT_G_ARCHER_SKELETON:
  case JT_G_RAYDRIC_ARCHER:
  case JT_G_KOBOLD_ARCHER:
  case JT_G_ORC_ARCHER:
  case JT_STONE_SHOOTER:
  case JT_WOOTAN_SHOOTER:
  case JT_PARASITE:
  case JT_G_EVIL_CLOUD_HERMIT:
  case JT_G_LIVE_PEACH_TREE:
  case JT_G_PARASITE:
  case JT_POTON_CANON:
  case JT_POTON_CANON_1:
  case JT_POTON_CANON_2:
  case JT_POTON_CANON_3:
  case JT_LADY_TANEE:
  case JT_RAGGED_ZOMBIE:
  case JT_PHYLLA:
  case JT_PETAL:
  case JT_MG_CORRUPTION_ROOT:
  case JT_E_MD_NYAILO:
  case JT_MER_ARCHER01:
  case JT_MER_ARCHER02:
  case JT_MER_ARCHER03:
  case JT_MER_ARCHER04:
  case JT_MER_ARCHER05:
  case JT_MER_ARCHER06:
  case JT_MER_ARCHER07:
  case JT_MER_ARCHER08:
  case JT_MER_ARCHER09:
  case JT_MER_ARCHER10:
  case JT_ILL_STONE_SHOOTER:
  case JT_ILL_WOOTAN_SHOOTER:
  case JT_G_ILL_WOOTAN_SHOOTER:
  case JT_RAYDRIC_ARCHER_H:
  case JT_MD_C_CORRUPTION_ROOT:
  case JT_MD_C_RAYDRIC_ARCHER:
  case JT_MD_C_RAY_ARCHER_G:
  case JT_SUMMON_HELLTREE:
  case JT_EP19_LIMACINA:
  case JT_G_EP19_LIMACINA:
  case JT_EP20_MELIBE_ICESLUG:
  case JT_EP20_RGAN_WARLOCK:
  case JT_G_EP20_RGAN_WARLOCK: {
    mode->SpawnArrowEffect(&this->render.position, target, scale, this->GetEffectiveJob(-1));
    return;
  }
  case JT_MANDRAGORA:
  case JT_HYDRA:
  case JT_THA_ODIUM:
  case JT_G_THA_ODIUM:
  case JT_DROSERA:
  case JT_MAVKA:
  case JT_G_MAVKA:
  case JT_ENTWEIHEN:
  case JT_G_ENTWEIHEN_R:
  case JT_G_ENTWEIHEN_H:
  case JT_G_ENTWEIHEN_M:
  case JT_G_ENTWEIHEN_S:
  case JT_NEPENTHES:
  case 0xE94:
  case 0xECF:
  case 0x500B:
  case JT_SUMMON_CREEPER:
  case JT_DEDSERA:
  case JT_EP21_YORTUS_B:
  case JT_EP21_B_YORTUS_B:
  case JT_MEDJED:
  case JT_CP_MEDJED:
  case JT_HEKA:
  case JT_CP_HEKA:
  case JT_GUIDE_OF_DEAD:
  case JT_E_SMART_ANDRE:
  case JT_EP21_YORTUS_B_H:
  case JT_EP21_B_YORTUS_B_H:
  case JT_CH1_FIRE_FLOWER:
  case JT_CH1_SHADIEST:
  case JT_CP_CH1_SHADIEST: {
    mode->SpawnMandraAttack(target, this->GetEffectiveJob(-1), 0.0f);
    return;
  }
  case JT_DISGUISER:
  case JT_G_DISGUISER:
  case JT_MD_PRI_RIGEL_AC:
  case JT_MD_PRI_RIGEL_ACS:
  case JT_SPIRIT_B_FLAME_M:
  case JT_SPIRIT_B_FLAME_L:
  case JT_SPIRIT_B_FLAME_SL:
  case JT_SPIRIT_I_WATER_S:
  case JT_SPIRIT_C_FLAME_L:
  case JT_SPIRIT_C_FLAME_SL:
  case JT_SPIRIT_H_WATER_S:
  case JT_SPIRIT_H_WATER_L:
  case JT_SPIRIT_H_WATER_SL:
  case JT_SPIRIT_R_FLAME_L:
  case JT_SPIRIT_R_FLAME_SL:
  case JT_SPIRIT_C_WATER_S:
  case JT_SPIRIT_C_WATER_L:
  case JT_SPIRIT_C_WATER_SL:
  case JT_VR_MD_DREGG1:
  case JT_VR_MD_DREGG2:
  case JT_BIO_ICE_GARGOYLE:
  case JT_EP21_YORKER_B:
  case JT_EP21_YOSTER_D:
  case JT_EP21_YOSTER_E:
  case JT_EP21_YORMI_B:
  case JT_EP21_YORDOS_A:
  case JT_EP21_YOSCOPUS_B:
  case JT_G_EP21_YORDOS_A:
  case JT_VIOLEN:
  case JT_CP_VIOLEN:
  case JT_EP21_YORDOS_A_H:
  case JT_EP21_YORMI_B_H:
  case JT_EP21_YOSCOPUS_B_H:
  case JT_CH1_SHADOW_JAILER:
  case JT_CH1_SCABIOSA:
  case JT_CH1_LOBELIA:
  case JT_CH1_RA:
  case JT_CH1_MI:
  case JT_CH1_RA_1:
  case JT_CH1_MI_1:
    LaunchBulletSpriteEffect(&this->render.position, target, scale, this->GetEffectiveJob(-1));
    return;
  default:
    return;
  }
}

// OFFSET: 0x00CABEB0
// FUNCTION: fn_21040
int CGameActor::GetMonsterTransformJob() {
  if (this->monsterTransformJob != -1) {
    return this->monsterTransformJob;
  }

  int result = -1;
  if (this->render.accountId == g_session.m_aid) {
    result = g_session.GetPairValue();
  } else {
    if (this->transformJobs[0] != -1) {
      result = this->transformJobs[0];
    }
    if (this->transformJobs[1] != -1) {
      result = this->transformJobs[1];
    }
    if (this->transformJobs[2] != -1) {
      result = this->transformJobs[2];
    }
  }
  return result;
}

// OFFSET: 0x00CABFA0
// FUNCTION: fn_21042
int CGameActor::GetEffectiveJob(int job) {
  if (job == -1) {
    job = this->job;
  }

  if (StateFlag12(this->GetStateFlags())) {
    return JT_MARRIED;
  }

  if (!StateFlag15(this->GetStateFlags())) {
    if (StateFlag16(this->GetStateFlags())) {
      return JT_SANTA;
    }
    if (StateFlag18(this->GetStateFlags())) {
      return JT_SUMMER;
    }
    if (StateFlag27(this->GetStateFlags())) {
      return JT_HANBOK;
    }
    if (StateFlag28(this->GetStateFlags())) {
      return JT_OKTOBERFEST;
    }
    if (this->mountedState == 1) {
      return JT_SUMMER2;
    }
  }

  if (StateFlag05(this->GetStateFlags())) {
    switch (job) {
      case JT_KNIGHT:
        return JT_CHICKEN;
      case JT_CRUSADER:
        return JT_CHICKEN2;
      case JT_KNIGHT_H:
        return JT_CHICKEN_H;
      case JT_CRUSADER_H:
        return JT_CHICKEN2_H;
      case JT_KNIGHT_B:
        return JT_CHICKEN_B;
      case JT_CRUSADER_B:
        return JT_CHICKEN2_B;
      case JT_ROYAL_GUARD:
        return JT_ROYAL_CHICKEN;
      case JT_ROYAL_GUARD_H:
        return JT_ROYAL_CHICKEN_H;
      case JT_ROYAL_GUARD_B:
        return JT_ROYAL_CHICKEN_B;
      case JT_IMPERIAL_GUARD:
        return JT_IMPERIAL_GUARD_CHICKEN;
    }
  }

  if (StateFlag19(this->GetStateFlags())) {
    switch (job) {
      case JT_RUNE_KNIGHT:
        return JT_RUNE_CHICKEN;
      case JT_RUNE_KNIGHT_H:
        return JT_RUNE_CHICKEN_H;
      case JT_RUNE_KNIGHT_B:
        return JT_RUNE_CHICKEN_B;
      case JT_DRAGON_KNIGHT:
        return JT_DRAGON_KNIGHT_CHICKEN;
    }
  } else if (StateFlag23(this->GetStateFlags())) {
    switch (job) {
      case JT_RUNE_KNIGHT:
        return JT_RUNE_CHICKEN2;
      case JT_RUNE_KNIGHT_H:
        return JT_RUNE_CHICKEN2_H;
    }
  } else if (StateFlag24(this->GetStateFlags())) {
    switch (job) {
      case JT_RUNE_KNIGHT:
        return JT_RUNE_CHICKEN3;
      case JT_RUNE_KNIGHT_H:
        return JT_RUNE_CHICKEN3_H;
    }
  } else if (StateFlag25(this->GetStateFlags())) {
    switch (job) {
      case JT_RUNE_KNIGHT:
        return JT_RUNE_CHICKEN4;
      case JT_RUNE_KNIGHT_H:
        return JT_RUNE_CHICKEN4_H;
    }
  } else if (StateFlag26(this->GetStateFlags())) {
    switch (job) {
      case JT_RUNE_KNIGHT:
        return JT_RUNE_CHICKEN5;
      case JT_RUNE_KNIGHT_H:
        return JT_RUNE_CHICKEN5_H;
    }
  }

  if (StateFlag21(this->GetStateFlags())) {
    switch (job) {
      case JT_RANGER:
        return JT_WOLF_RANGER;
      case JT_RANGER_H:
        return JT_WOLF_RANGER_H;
      case JT_RANGER_B:
        return JT_WOLF_RANGER_B;
      case JT_WINDHAWK:
        return JT_WOLF_WINDHAWK;
    }
  }

  if (this->GetMadogearType() != 0) {
    switch (job) {
      case JT_MECHANIC:
        return JT_MADOGEAR;
      case JT_MECHANIC_H:
        return JT_MADOGEAR_H;
      case JT_MECHANIC_B:
        return JT_MADOGEAR_B;
      case JT_MEISTER:
        return JT_MEISTER_MADOGEAR;
    }
  }

  if (StateFlag15(this->GetStateFlags())) {
    switch (job) {
      case JT_STAR:
        return JT_STAR2;
      case JT_STAR_B:
        return JT_STAR2_B;
      case JT_STAR_EMPEROR:
        return JT_STAR2_EMPEROR;
      case JT_STAR_EMPEROR_B:
        return JT_STAR2_EMPEROR_B;
      case JT_SKY_EMPEROR:
        return JT_SKY_EMPEROR2;
    }
  }

  return job;
}

// OFFSET: 0x00CCCAF0
// FUNCTION: fn_21199
void CGameActor::SetModifyFactorOfmotionSpeed(int value) {
  if (value > g_maxMoveSpeed) {
    value = g_maxMoveSpeed;
  }
  const float speed = static_cast<float>(value) / 432.0f;
  this->render.motionSpeedScale = speed;
  this->render.motionSpeed = speed;
}

// OFFSET: 0x005A1A30
// FUNCTION: fn_03067
void CGameActor::SetSprActFull(int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x005A1A20
// FUNCTION: fn_03066
void CGameActor::SetSprAct2(int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x005A19F0
// FUNCTION: fn_03063
void CGameActor::SetSprAccessory2(int) {
}

// OFFSET: 0x005A1A00
// FUNCTION: fn_03064
void CGameActor::SetSprAccessory3(int) {
}

// OFFSET: 0x005A1980
// FUNCTION: fn_03056
void CGameActor::SetHeadPaletteName() {
}

// OFFSET: 0x005A1970
// FUNCTION: fn_03055
void CGameActor::SetHeadPalette(int) {
}

// OFFSET: 0x005A1A50
// FUNCTION: fn_03069
void CNpc::SetSprHead(int) {
}

// OFFSET: 0x005A1A10
// FUNCTION: fn_03065
void CNpc::SetSprAccessory(int) {
}

// OFFSET: 0x005A1AB0
// FUNCTION: fn_03075
void CNpc::SetSprWeapon(int, int) {
}

// OFFSET: 0x005A19A0
// FUNCTION: fn_03058
void CNpc::SetImfFileName() {
}

// OFFSET: 0x005A1A60
// FUNCTION: fn_03070
void CCharacterNpc::SetSprJob(int) {
}

// OFFSET: 0x005A13E0
// FUNCTION: fn_03052
void CCharacterNpc::Render(int) {
  if (this->render.visible == 0 || this->render.enabled == 0) {
    return;
  }

  this->characterRender->actionIndex = this->render.previousAnimation;
  this->characterRender->frameIndex = this->render.curMotion;

  char useBounds = 0;
  std::list<CharacterRenderDrawItem> drawList =
    this->characterRender->BuildDrawList(this->render.visibilityOptions[1], &useBounds);

  CCharacterRender* characterRender = this->characterRender;
  float bounds[5];
  std::memcpy(bounds, &characterRender->renderBounds, sizeof(bounds));
  const float tlvertX = characterRender->screenX;
  const float tlvertY = characterRender->screenY;
  if (useBounds == 1) {
    this->SetRenderInfo(bounds, tlvertX, tlvertY);
  } else {
    this->SetTlvert(tlvertX, tlvertY);
  }

  for (std::list<CharacterRenderDrawItem>::iterator it = drawList.begin(); it != drawList.end(); ++it) {
    static_cast<CRendererDX9*>(g_renderer)->Submit(it->command, static_cast<std::uint16_t>(it->flags));
  }
}

struct CharacterNpcViewInfo {
  /* 0000 */ int aid;
  /* 0004 */ int job;
  /* 0008 */ int padding_0008[10];
  /* 0030 */ int standAccessory;
  /* 0034 */ int padding_0034;
  /* 0038 */ int moveAccessory;
  /* 003C */ int padding_003C[4];
  /* 004C */ int standAccessory2;
  /* 0050 */ int standAccessory3OrMoveAccessory2;
  /* 0054 */ int standBodyPaletteOrMoveAccessory3;
  /* 0058 */ int standHeadPaletteOrMoveBodyPalette;
  /* 005C */ int moveHeadPalette;
  /* 0060 */ int guildId;
  /* 0064 */ int guildEmblemVersion;
  /* 0068 */ int padding_0068[2];
  /* 0070 */ int robe;
  /* 0074 */ int robeAlt;
  /* 0078 */ int standBodyStyle;
  /* 007C */ int moveBodyStyle;
};

// INLINED
static CCharacterRender* NpcCharacterRender(CCharacterNpc* self) {
  return self->characterRender;
}

// INLINED
static __forceinline void FillCharacterNpcRenderPrimary(CCharacterNpc* self, CharacterNpcViewInfo* info) {
  {
    CCharacterRender* render = NpcCharacterRender(self);
    render->position.x = self->render.position.x;
    render->position.y = self->render.position.y;
    render->position.z = self->render.position.z;
  }
  NpcCharacterRender(self)->job = self->job;
  NpcCharacterRender(self)->appearance[0] = self->sex;
  NpcCharacterRender(self)->appearance[1] = self->headType;
  NpcCharacterRender(self)->appearance[2] = info->standHeadPaletteOrMoveBodyPalette;
  NpcCharacterRender(self)->appearance[3] = info->standBodyPaletteOrMoveAccessory3;
  NpcCharacterRender(self)->lowerHeadgear = info->standAccessory;
  NpcCharacterRender(self)->upperHeadgear = info->standAccessory2;
  NpcCharacterRender(self)->middleHeadgear = info->standAccessory3OrMoveAccessory2;
  NpcCharacterRender(self)->SetRobe(info->robe);
  NpcCharacterRender(self)->bodyCostume = info->standBodyStyle;
}

// INLINED
static __forceinline void FillCharacterNpcRenderSecondary(CCharacterNpc* self, CharacterNpcViewInfo* info) {
  {
    CCharacterRender* render = NpcCharacterRender(self);
    render->position.x = self->render.position.x;
    render->position.y = self->render.position.y;
    render->position.z = self->render.position.z;
  }
  NpcCharacterRender(self)->job = self->job;
  NpcCharacterRender(self)->appearance[0] = self->sex;
  NpcCharacterRender(self)->appearance[1] = self->headType;
  NpcCharacterRender(self)->appearance[2] = info->moveHeadPalette;
  NpcCharacterRender(self)->appearance[3] = info->standHeadPaletteOrMoveBodyPalette;
  NpcCharacterRender(self)->lowerHeadgear = info->moveAccessory;
  NpcCharacterRender(self)->upperHeadgear = info->standAccessory3OrMoveAccessory2;
  NpcCharacterRender(self)->middleHeadgear = info->standBodyPaletteOrMoveAccessory3;
  NpcCharacterRender(self)->SetRobe(info->robeAlt);
  NpcCharacterRender(self)->bodyCostume = info->moveBodyStyle;
}

// OFFSET: 0x005A1530
// FUNCTION: fn_03053
void CCharacterNpc::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  CharacterNpcViewInfo* info = reinterpret_cast<CharacterNpcViewInfo*>(static_cast<std::intptr_t>(b));
  switch (message) {
  case AM_INITSTANDENTRY:
    this->CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    this->render.visible = g_session.IsVisibleJob(info->job);
    this->SetGuildInfo(info->guildId, info->guildEmblemVersion);
    if (this->characterRender != nullptr) {
      return;
    }
    this->characterRender = new CCharacterRender(0, 0);
    FillCharacterNpcRenderPrimary(this, info);
    return;
  case AM_STANDENTRY:
    this->CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    if (this->characterRender == nullptr) {
      return;
    }
    FillCharacterNpcRenderPrimary(this, info);
    return;
  case AM_INITMOVEENTRY:
  case AM_INITMOVEENTRY2:
    this->CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    if (this->characterRender != nullptr) {
      return;
    }
    this->characterRender = new CCharacterRender(0, 0);
    FillCharacterNpcRenderSecondary(this, info);
    return;
  case AM_MOVEENTRY:
  case AM_MOVEENTRY2:
    this->CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    if (this->characterRender == nullptr) {
      return;
    }
    FillCharacterNpcRenderSecondary(this, info);
    return;
  default:
    CNpc::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }
}

// OFFSET: 0x00CAC760
// FUNCTION: fn_21044
int CGameActor::GetShapeShiftJob() const {
  switch (this->shapeShift) {
  case 1:
    return 0x1104;
  case 2:
    return 0x1105;
  default:
    return this->job;
  }
}

// OFFSET: 0x00CC1D40
// FUNCTION: fn_21128
void CGameActor::SetTransformJob(int value, int slot) {
  this->transformPriority = 0;
  if (this->render.accountId == g_session.m_aid) {
    g_session.SetPairValue(value, slot);
  } else if (slot == 0) {
    this->transformJobs[0] = value;
  } else if (slot == 1) {
    this->transformJobs[1] = value;
  } else if (slot == 2) {
    this->transformJobs[2] = value;
  }

  if (value > 0) {
    this->SetSprJob(value);
    this->SetState(this->render.state, 1);
    return;
  }

  int job;
  if (g_session.IsCostumeJob(this->GetEffectiveJob(-1)) == true || this->shapeShift == 0) {
    job = this->GetEffectiveJob(-1);
  } else {
    job = this->GetShapeShiftJob();
  }

  this->SetBodyPaletteName();
  this->SetSprJob(job);
  this->SetState(this->render.state, 1);
}

// OFFSET: 0x00CC46D0
// FUNCTION: fn_21166
void CGameActor::SetTetraBall(int index, int on, int value) {
  CTetraBallEffect* ball = nullptr;
  if (on == 1) {
    if (value == 0 || this->movementState[index] == value) {
      return;
    }

    for (int id = 0x4EE; id <= 0x4F1; ++id) {
      ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Find(this->render.accountId, id));
      if (ball != nullptr) {
        break;
      }
    }

    const int type = value & 3;
    if (type == 0) {
      if (ball == nullptr) {
        CTetraBallEffect* spawned = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
        if (spawned != nullptr) {
          spawned->MoveFirstTo(index);
        }
      } else {
        ball->AddBall(0x4EE, index);
      }
    } else if (type == 1) {
      if (ball == nullptr) {
        CTetraBallEffect* spawned = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
        if (spawned != nullptr) {
          spawned->MoveFirstTo(index);
        }
      } else {
        ball->AddBall(0x4F0, index);
      }
    } else if (type == 2) {
      if (ball == nullptr) {
        CTetraBallEffect* spawned = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
        if (spawned != nullptr) {
          spawned->MoveFirstTo(index);
        }
      } else {
        ball->AddBall(0x4EF, index);
      }
    } else if (type == 3) {
      if (ball == nullptr) {
        CTetraBallEffect* spawned = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
        if (spawned != nullptr) {
          spawned->MoveFirstTo(index);
        }
      } else {
        ball->AddBall(0x4F1, index);
      }
    }

    this->movementState[index] = value;
    return;
  }

  for (int id = 0x4EE; id <= 0x4F1; ++id) {
    ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Find(this->render.accountId, id));
    if (ball != nullptr) {
      if (ball->ActiveCount() > 1) {
        ball->RemoveBall(index);
      } else {
        g_effectMgr->Retire(ball, 1);
      }
      break;
    }
  }

  this->movementState[index] = 0;
}

// OFFSET: 0x00CC40C0
// FUNCTION: fn_21157
void CGameActor::SetShapeShift(int value) {
  this->shapeShift = value;
}

// OFFSET: 0x00CC3DE0
// FUNCTION: fn_21155
void CGameActor::SetMotionTintState(const std::uint32_t* value) {
  this->render.motionStartTime = value[0];
  this->render.savedTintColor = value[1];
  this->render.targetTintColor = value[2];
}

// OFFSET: 0x00CC41E0
// FUNCTION: fn_21159
void CGameActor::UnRegisterPos() {
  g_modeMgr.GetGameMode()->SendMsg(MM_DELETEREGISTERPOS, this->render.accountId, 0, 0, 0);
}

// OFFSET: 0x00CC1E30
// FUNCTION: fn_21129
void CGameActor::SetMonsterTransformJob(int value) {
  this->transformPriority = 0;
  this->monsterTransformJob = value;
  if (value > 0) {
    this->SetSprJob(value);
  } else {
    this->SetSprJob(this->GetEffectiveJob(-1));
  }
  this->SetState(this->render.state, 1);
}

// OFFSET: 0x00CABDE0
// FUNCTION: fn_21038
void CRenderObject::ScaleByFadePulse(float& scale) const {
  if ((this->render.flags & 0x10) != 0) {
    const unsigned index = this->render.pulsePhases[0] * 4u + 90u;
    if (index > 270) {
      scale = scale * 0.5f;
    } else {
      scale = ((g_sinTable[index >= 90u ? index : 90u] + 1.0f) * 0.25f + 0.5f) * scale;
    }
  }
}

// OFFSET: 0x00CC4270
// FUNCTION: fn_21161
void CRenderObject::UpdateTlvertex(const float* matrix) {
  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, matrix, &x, &y, &reciprocalDepth);
  this->SetTlvert(x, y);
}

// OFFSET: 0x00CABE60
// FUNCTION: fn_21039
int CGameActor::ActionBase(int state) {
  int base = 16;
  switch (state) {
  case ST_STAND:
    base = 0;
    break;
  case ST_MOVE:
    base = 8;
    break;
  case ST_DAMAGE:
    base = 24;
    break;
  case ST_DEAD:
    base = 32;
    break;
  }

  return base;
}

// OFFSET: 0x00CAC730
// FUNCTION: fn_21043
int ShapeShiftJob(int form, int job) {
  switch (form) {
  case 1:
    return JT_WEREWOLF;
  case 2:
    return JT_WERERAPTOR;
  default:
    return job;
  }
}

// OFFSET: 0x00CAC790
// FUNCTION: fn_21045
static std::string ShapeShiftName(int form, int job) {
  switch (form) {
  case 1:
    switch (job) {
    case JT_KARNOS:
    case JT_KARNOS_B: {
      return OBFUSCATE_BYTES_TEXT("WEREWOLF2", 0xA5FD29D7AB6DC59FULL);
    }
    case JT_ALITEA: {
      return OBFUSCATE_BYTES_TEXT("WEREWOLF3", 0xD97D5DBDBB2F2709ULL);
    }
    default: {
      return OBFUSCATE_BYTES_TEXT("WEREWOLF", 0xED796785D1DF137FULL);
    }
    }
  case 2:
    switch (job) {
    case JT_KARNOS:
    case JT_KARNOS_B: {
      return OBFUSCATE_BYTES_TEXT("WERERAPTOR2", 0x83EFBFBB37EDBDBDULL);
    }
    case JT_ALITEA: {
      return OBFUSCATE_BYTES_TEXT("WERERAPTOR3", 0xBF2B4B31397FF5A7ULL);
    }
    default: {
      return OBFUSCATE_BYTES_TEXT("WERERAPTOR", 0xDD3913D5D7F97BE1ULL);
    }
    }
  default:
    return "";
  }
}

// OFFSET: 0x00CACD00
// FUNCTION: fn_21046
std::string CGameActor::GetShapeShiftName() {
  return ShapeShiftName(this->shapeShift, this->job);
}

// OFFSET: 0x00CACD70
// FUNCTION: fn_21047
int CGameActor::MountType() {
  switch (this->GetEffectiveJob(-1)) {
  case JT_CHICKEN:
  case JT_CHICKEN2:
  case JT_CHICKEN_H:
  case JT_CHICKEN2_H:
  case JT_CHICKEN_B:
  case JT_CHICKEN2_B:
    return 3;
  case JT_RUNE_CHICKEN:
  case JT_RUNE_CHICKEN_H:
  case JT_ROYAL_CHICKEN:
  case JT_ROYAL_CHICKEN_H:
  case JT_WOLF_RANGER:
  case JT_WOLF_RANGER_H:
  case JT_MADOGEAR:
  case JT_MADOGEAR_H:
  case JT_RUNE_CHICKEN2:
  case JT_RUNE_CHICKEN2_H:
  case JT_RUNE_CHICKEN3:
  case JT_RUNE_CHICKEN3_H:
  case JT_RUNE_CHICKEN4:
  case JT_RUNE_CHICKEN4_H:
  case JT_RUNE_CHICKEN5:
  case JT_RUNE_CHICKEN5_H:
  case JT_RUNE_CHICKEN_B:
  case JT_ROYAL_CHICKEN_B:
  case JT_WOLF_RANGER_B:
  case JT_MADOGEAR_B:
  case JT_WOLF_WINDHAWK:
  case JT_MEISTER_MADOGEAR:
  case JT_DRAGON_KNIGHT_CHICKEN:
  case JT_IMPERIAL_GUARD_CHICKEN:
    return 5;
  default:
    return 0;
  }
}

// OFFSET: 0x00CC2200
// FUNCTION: fn_21137
void RenderBounds::Set(float left, float top, float right, float bottom, float offset) {
  this->left = left;
  this->right = right;
  this->top = top;
  this->bottom = bottom;
  this->offset = offset;
}

// OFFSET: 0x00CC4210
// FUNCTION: fn_21160
void RenderBounds::Include(const float* box) {
  const float minX = box[1] < box[3] ? box[1] : box[3];
  const float maxX = box[1] > box[3] ? box[1] : box[3];
  if (this->left > minX) {
    this->left = minX;
  }
  if (this->top > box[0]) {
    this->top = box[0];
  }
  if (maxX > this->right) {
    this->right = maxX;
  }
  if (box[2] > this->bottom) {
    this->bottom = box[2];
  }
}

// OFFSET: 0x00CA9E20
// FUNCTION: fn_21012
bool CGameActor::IsShapeShifted() {
  if (g_session.IsCostumeJob(this->GetEffectiveJob(-1)) == true) {
    return false;
  }
  return this->shapeShift != 0;
}

// OFFSET: 0x00CAA870
// FUNCTION: fn_21018
bool CGameActor::ApplyEffectState(int effectId, bool on) {
  if (effectId < 2485) {
    this->SetEffectState(effectId, on);
    return false;
  }

  int type = 0;
  g_session.m_hotkeyScript->Call("GetEffectType", "d>d", effectId - 2485, &type);
  switch (type) {
  case 0:
    this->SetEffectState(effectId, on);
    break;
  case 1:
    this->SetHatEffect(effectId, on);
    break;
  case 2:
    this->SetEffectState(effectId, on);
    this->SetHatEffect(effectId, on);
    break;
  case 3:
    this->SetEffectInstance(2411, effectId - 2485, on);
    break;
  case 4:
    this->SetEffectInstance(2412, effectId - 2485, on);
    break;
  }
  return false;
}

// OFFSET: 0x00CAA9B0
// FUNCTION: fn_21019
void CGameActor::SetHatEffect(int effectId, bool on) {
  int hatEffect = 0;
  g_session.m_hotkeyScript->Call("GetHatEffectID", "d>d", effectId - 2485, &hatEffect);
  if (hatEffect <= 0) {
    return;
  }

  const std::int64_t value = hatEffect;
  this->SendMsg(this, AM_DELETE_EFFECT, value, 0, 0, 0, 0, 0);

  CPc* pc;
  if (hatEffect == 254 && IsPlayerJob(this->GetEffectiveJob(-1)) && (pc = dynamic_cast<CPc*>(this)) != nullptr) {
    pc->appearanceFlags = on ? pc->appearanceFlags | 0x10 : pc->appearanceFlags ^ 0x10;
    if ((pc->virtue & 0x10) == 0 && (pc->appearanceFlags & 0x10) == 0) {
      return;
    }
  } else if (!on) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(this->render.accountId, hatEffect, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->headAttached = 1;
  } else if (CRagEffect* ragEffect = this->LaunchEffect(hatEffect, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    ragEffect->state.visibilityFlags = 1;
  }
}

// OFFSET: 0x00CAABA0
// FUNCTION: fn_21020
void CGameActor::SetEffectState(int effectId, bool on) {
  if (on) {
    CEffect* effect = g_effectMgr->Spawn(this->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->headAttached = 1;
    }
  } else {
    g_effectMgr->Retire(this->render.accountId, effectId);
  }
}

// OFFSET: 0x00CAAC10
// FUNCTION: fn_21021
void CGameActor::SetEffectInstance(int effectId, int instance, bool on) {
  if (on) {
    g_effectMgr->Spawn(this->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, instance);
  } else {
    g_effectMgr->Retire(this->render.accountId, instance, effectId);
  }
}

// OFFSET: 0x00CAAC80
// FUNCTION: fn_21022
void CGameActor::ResetJobEffects() {
  this->levelAuraType = 0;
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL99, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL99_ORB1, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL99_ORB2, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL150, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL150_SUB, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL160, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_LEVEL160_SUB, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, 1226, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, 1227, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BEGINSPELL_150, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_5, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_6, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_4, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_4, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_2, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_3, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEVEL99_3, 0, 0, 0, 0, 0);

  const std::int64_t first = g_session.m_maxLevelEffect4th;
  this->SendMsg(this, AM_DELETE_EFFECT, first, 0, 0, 0, 0, 0);
  const std::int64_t second = g_session.m_maxLevelEffect4thSub;
  this->SendMsg(this, AM_DELETE_EFFECT, second, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00CAB020
// FUNCTION: fn_21024
__declspec(noinline) void CGameActor::ClearStatusEffectInstance(CGameObject* value) {
  if (this->statusEffects[1] == value) {
    this->statusEffects[1] = 0;
  } else if (this->statusEffects[0] == value) {
    this->statusEffects[0] = 0;
  } else if (this->statusEffects[2] == value) {
    this->statusEffects[2] = 0;
  } else if (this->statusEffects[3] == value) {
    this->statusEffects[3] = 0;
  } else if (this->statusEffects[4] == value) {
    this->statusEffects[4] = 0;
  } else if (this->statusEffects[5] == value) {
    this->statusEffects[5] = 0;
  } else if (this->statusEffects[6] == value) {
    this->statusEffects[6] = 0;
  } else if (this->statusEffects[8] == value) {
    this->statusEffects[8] = 0;
  } else if (this->statusEffects[9] == value) {
    this->statusEffects[9] = 0;
  } else if (this->statusEffects[7] == value) {
    this->statusEffects[7] = 0;
  } else if (this->statusEffects[10] == value) {
    this->statusEffects[10] = 0;
  } else if (this->statusEffects[11] == value) {
    this->statusEffects[11] = 0;
  } else if (this->statusEffects[12] == value) {
    this->statusEffects[12] = 0;
  }
}

// OFFSET: 0x00CAB990
// FUNCTION: fn_21032
const char* CRenderObject::GetActName() {
  if (!this->render.action) {
    return nullptr;
  }

  return this->render.action->GetName();
}

// OFFSET: 0x00CAB9B0
// FUNCTION: fn_21033
CActRes* CRenderObject::GetActRes() {
  return this->render.action;
}

// OFFSET: 0x00CAB9C0
// FUNCTION: fn_21034
int CGameActor::WeaponActionBase(int weaponType) {
  int base = 0;
  switch (weaponType) {
  case ST_AUTOCOUNTER:
    base = IsJobWeaponAllowed(this->GetEffectiveJob(-1), this->sex, this->GetWeapon()) ? 88 : 80;
    break;
  case ST_A_COUNTER:
    switch (this->GetEffectiveJob(-1)) {
    case JT_LINKER:
    case JT_LINKER_B:
    case JT_SOUL_REAPER:
    case JT_SOUL_REAPER_B:
    case JT_SOUL_ASCETIC:
      base = 88;
      break;
    default:
      base = 40;
      break;
    }
    break;
  case ST_ATTACK2:
  case ST_READYSHOT:
  case ST_A_TURN:
  case 49:
  case 53:
  case 54:
  case 59:
  case 70:
  case 73:
  case 76:
  case 77:
    base = 88;
    break;
  case ST_THROW:
  case ST_READYJUMP:
  case ST_THROW2:
  case ST_GUNSKILL3:
  case ST_GUNREADY2:
  case 50:
  case 51:
  case 58:
    base = 40;
    break;
  case ST_DAMAGE:
    base = 48;
    break;
  case ST_ATTACK:
  case ST_MADOSKILL:
  case ST_MADOSKILL2:
  case 52:
  case 60:
  case 62:
  case 63:
  case 69:
  case 71:
  case 72:
  case 78:
  case 79:
    base = 80;
    break;
  case ST_DEAD:
    base = 64;
    break;
  case ST_STAND:
    base = 0;
    break;
  case ST_MOVE:
    base = 8;
    break;
  case ST_PICKUP:
  case ST_LANDJUMP:
  case ST_R_DODGE:
    base = 24;
    break;
  case ST_ATTACKWAIT:
    base = 32;
    break;
  case ST_DANCE:
  case ST_SING:
  case ST_R_STORM:
  case ST_R_DOWN:
  case ST_R_TURN:
  case ST_R_COUNTER:
  case ST_A_JUMPKICK:
  case ST_A_STORM:
  case ST_SG_DOWN:
  case ST_SG_TURN:
  case ST_NINJASKILL1:
  case ST_NINJASKILL2:
  case ST_NINJAREADY:
  case ST_GUNSKILL1:
  case ST_GUNSKILL2:
  case ST_GUNREADY:
  case ST_AB_SPECIAL:
  case 47:
  case 48:
  case 55:
  case 56:
  case 57:
  case 61:
  case 64:
  case 65:
  case 66:
  case 67:
  case 68:
  case 80:
  case 81:
    base = 96;
    break;
  case 74:
  case 75:
    base = 104;
    break;
  }
  return base;
}

// OFFSET: 0x00CABC80
// FUNCTION: fn_21035
int CGameActor::MountHeightOffset() {
  int offset = 0;
  if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) || g_session.IsNpcJob(this->GetEffectiveJob(-1)) ||
      this->render.accountId == g_session.m_aid) {
    if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
      return !this->isRiding ? -20 : 10;
    }

    if (g_session.IsDoramJob(this->GetEffectiveJob(-1))) {
      return !this->isRiding ? -30 : -40;
    }

    if (this->isRiding) {
      offset = 30;
    }
  }
  return offset;
}

// OFFSET: 0x00CABD30
// FUNCTION: fn_21036
int CGameActor::GetShapeShift() {
  return this->shapeShift;
}

// OFFSET: 0x00CC4EA0
// FUNCTION: fn_21172
C3dGrannyGameActor::C3dGrannyGameActor(const char* name) {
  this->model = nullptr;
  this->instance = nullptr;
  this->initializeFacing = 1;
  for (int mesh = 0; mesh < 30; ++mesh) {
    this->commands[mesh] = nullptr;
    this->vertices[mesh] = nullptr;
  }
  this->motion = 0;
  this->alpha = 0xFF;
  std::strcpy(this->name, name);
  this->renderState = 0;
  this->cellTexture = 0;
  for (int index = 0; index < 20; ++index) {
    this->controls[index] = nullptr;
  }
  this->emblemTexture = nullptr;
  this->texture = g_texMgr.CreateTexture(35, 35, 1, false, false);
  this->litPositions.resize(4900);
}

// OFFSET: 0x00CC7ED0
// FUNCTION: fn_21189
void C3dGrannyGameActor::SetSprAct(int job, int value, int) {
  this->SetActorJob(job);
  this->SetSexBit(value);
  this->model = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->name));
  this->instance = GrannyInstantiateModel(this->model->model);
  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    RenderCommand* previous = this->commands[mesh];
    RenderCommand* command = reinterpret_cast<RenderCommand*>(new ModelRenderCommand());
    this->commands[mesh] = command;
    command->AllocateTriangles(this->model->GetTriangleCount(mesh));
    if (previous != nullptr) {
      previous->FreeVertices();
      ::operator delete(previous, 0x28);
    }

    RenderVertex* previousVertices = this->vertices[mesh];
    this->vertices[mesh] = new RenderVertex[this->model->GetTriangleCount(mesh) * 3];
    std::memset(this->vertices[mesh], 0, this->model->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
    if (previousVertices != nullptr) {
      delete[] previousVertices;
    }
  }

  this->lastClock = GrannyGetSystemSeconds();
  this->motionProgress = 0.0f;
  if (job == JT_ZOMBIE_DRAGON || job == JT_HUGELING) {
    this->actorType = 1;
    this->model->baseHeight = 30.0f;
    this->model->topHeight = 30.0f;
  } else {
    this->actorType = job == JT_GUILD_FLAG ? 2 : 0;
  }
}

// OFFSET: 0x00CC5650
// FUNCTION: fn_21176
int C3dGrannyGameActor::GetAttackMotion() {
  return 0;
}

// OFFSET: 0x00CC5D70
// FUNCTION: fn_21181
void C3dGrannyGameActor::ProcessMotionWithDist() {
}

// OFFSET: 0x00CC5B00
// FUNCTION: fn_21179
bool CGrannyCorpse3d::OnProcess() {
  const int tick = this->tick;
  if (tick > 355) {
    this->alpha = static_cast<unsigned char>(600 - tick);
  }

  if (this->tick++ > 600) {
    return false;
  }

  return true;
}

// OFFSET: 0x00CC7D50
// FUNCTION: fn_21188
void C3dGrannyGameActor::SetAction(unsigned animation, int, int repeat) {
  this->render.state = 0;
  switch (animation) {
    case 48:
      this->render.state = 4;
      break;
    case 80:
    case 88:
      this->render.state = 2;
      break;
    case 64:
      this->render.state = 3;
      break;
    case 0:
      this->render.state = 0;
      break;
    case 8:
      this->render.state = 1;
      break;
    case 24:
      this->render.state = 5;
      break;
    case 32:
      this->render.state = 11;
      break;
    case 40:
      this->render.state = 12;
      break;
  }

  this->motion = this->render.state;
  for (int i = 0; i < 20; ++i) {
    this->model->FreeControl(this->controls[i]);
    this->controls[i] = nullptr;
  }

  this->motionProgress = 0;
  if (repeat == 1) {
    this->controls[this->motion] = this->model->PlayAnimation(this->motion, this->instance, this->controls[this->motion], 1);
  } else {
    this->controls[this->motion] = this->model->PlayAnimation(this->motion, this->instance, this->controls[this->motion], 0);
  }
}

// OFFSET: 0x00CC5080
// FUNCTION: fn_21173
CGrannyCorpse3d::CGrannyCorpse3d(const char* name) {
  this->position.x = 0.0f;
  this->position.y = 0.0f;
  this->position.z = 0.0f;
  this->alpha = 0xFF;
  this->instance = nullptr;
  this->control = nullptr;
  std::strcpy(this->name, name);
  this->model = nullptr;
  for (int mesh = 0; mesh < 30; ++mesh) {
    this->commands[mesh] = nullptr;
  }

  this->model = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->name));
  this->instance = GrannyInstantiateModel(this->model->model);
  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    this->commands[mesh] = reinterpret_cast<RenderCommand*>(new ModelRenderCommand());
    this->commands[mesh]->AllocateTriangles(this->model->GetTriangleCount(mesh));
    this->vertices[mesh] = new RenderVertex[this->model->GetTriangleCount(mesh) * 3];
    std::memset(this->vertices[mesh], 0, this->model->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  this->startClock = GrannyGetSystemSeconds();
  this->animationTime = 0;
  this->tick = 0;
  this->control = this->model->PlayAnimation(3, this->instance, this->control, 1);
}

CGrannyCorpse3d::~CGrannyCorpse3d() {
  this->model->FreeControl(this->control);
  for (int mesh = 0; mesh < 30; ++mesh) {
    if (this->commands[mesh] != nullptr) {
      this->commands[mesh]->FreeVertices();
      delete this->commands[mesh];
      this->commands[mesh] = nullptr;
      delete this->vertices[mesh];
      this->vertices[mesh] = nullptr;
    }
  }

  GrannyFreeModelInstance(this->instance);
}

// OFFSET: 0x00CD8E00
// FUNCTION: fn_21305
__declspec(noinline) void CGameMode::SpawnGrannyCorpse(float x, float y, float z, float angle, const char* name) {
  CGrannyCorpse3d* corpse = new CGrannyCorpse3d(name);
  corpse->position.x = x;
  corpse->position.y = y;
  corpse->position.z = z;
  corpse->angle = angle;
  this->scene->objects.push_back(reinterpret_cast<CRenderObject*>(corpse));
}

// OFFSET: 0x00CC8230
// FUNCTION: fn_21193
void CGameActor::ProcessHitMessage(CGameObject*, const ActorHitInfo* info, int, int) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int type = info->type;
  this->hitTargetAid = info->targetAid;
  int damage = info->damage;
  int hitCount = info->hitCount;
  const int extraDamage = info->extraDamage;
  const int cellX = info->cellX;
  const int cellY = info->cellY;
  const int attackSpeed = info->attackSpeed;
  const int motionSpeed = info->motionSpeed;

  CGameActor* target;
  if (g_session.m_aid == info->targetAid) {
    target = static_cast<CGameActor*>(mode->scene->player);
  } else {
    target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(info->targetAid));
    if (target == nullptr) {
      target = static_cast<CGameActor*>(mode->scene->GetSkillByAID(this->hitTargetAid));
    }
  }

  float x;
  float z;
  if (target == nullptr) {
    const CRenderObject* player = static_cast<const CRenderObject*>(mode->scene->player);
    x = this->render.position.x - (player->render.position.x - this->render.position.x);
    z = this->render.position.z - (player->render.position.z - this->render.position.z);
  } else {
    x = target->render.position.x;
    z = target->render.position.z;
    target->GetEffectiveJob(-1);
  }

  if (hitCount <= 0) {
    hitCount = 1;
  } else if (hitCount > 36) {
    hitCount = 1;
  }

  if (type != DMG_MULTI_HIT && type != DMG_MULTI_HIT_ENDURE && type != DMG_MULTI_HIT_CRITICAL) {
    hitCount = 1;
  }

  damage /= hitCount;
  switch (type) {
    case DMG_NORMAL:
    case DMG_ENDURE:
    case DMG_MULTI_HIT:
    case DMG_MULTI_HIT_ENDURE:
    case DMG_CRITICAL:
    case DMG_LUCY_DODGE:
    case DMG_MULTI_HIT_CRITICAL: {
      if (this->isCounter != 0) {
        CRagEffect* effect = this->LaunchEffect(EF_AUTOCOUNTER, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect != nullptr) {
          effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
        }
        this->isCounter = 0;
      }

      this->SetModifyFactorOfmotionSpeed(attackSpeed);
      if (info->targetAid != this->render.accountId) {
        this->SetState(2, 1);
      }

      this->Face(x, z);
      unsigned delay = static_cast<unsigned>(this->attackMotion * this->render.speed * 24.0f);
      if (this->GetEffectiveJob(-1) == JT_ARCHER_SKELETON || this->GetEffectiveJob(-1) == JT_G_ARCHER_SKELETON) {
        delay += 0xC0;
      } else if (this->GetEffectiveJob(-1) == JT_ARCHER_GUARDIAN || this->GetEffectiveJob(-1) == JT_BOW_GUARDIAN) {
        delay += 0x390;
      } else if (this->GetEffectiveJob(-1) == JT_KNIGHT_GUARDIAN || this->GetEffectiveJob(-1) == JT_SWORD_GUARDIAN || this->GetEffectiveJob(-1) == JT_SOLDIER_GUARDIAN) {
        delay += 0x198;
      }

      int flag = -1;
      int action;
      switch (type) {
        case DMG_ENDURE:
        case DMG_MULTI_HIT_ENDURE:
          action = 0x27;
          break;
        case DMG_CRITICAL:
          action = 0x5D;
          flag = 1;
          break;
        case DMG_MULTI_HIT_CRITICAL:
          action = 0x94;
          flag = 1;
          break;
        case DMG_LUCY_DODGE:
          action = this->isCounter != 0 ? 0x1A : 0x5E;
          break;
        default:
          action = 0x1A;
          break;
      }

      if (target == nullptr) {
        return;
      }

      int effectId = 0;
      switch (this->GetEffectiveJob(-1)) {
        case JT_TAEKWON:
        case JT_STAR:
        case JT_STAR2:
        case JT_TAEKWON_B:
        case JT_STAR_B:
        case JT_STAR2_B:
        case JT_STAR_EMPEROR:
        case JT_SOUL_REAPER:
        case JT_STAR_EMPEROR_B:
        case JT_SOUL_REAPER_B:
        case JT_STAR2_EMPEROR:
        case JT_SKY_EMPEROR:
        case JT_SOUL_ASCETIC:
        case JT_SKY_EMPEROR2:
          effectId = 0x1B2;
          break;
        case JT_VIOLEN:
        case JT_CP_VIOLEN:
          effectId = 0x974;
          break;
      }

      const CRenderObject* player = static_cast<const CRenderObject*>(mode->scene->player);
      int column;
      int row;
      mode->WorldCell(player->render.position.x, player->render.position.y, column, row);
      if (info->targetAid == this->render.accountId) {
        effectId = -2;
      }

      ActorDamageMessage message;
      message.motionSpeed = static_cast<float>(motionSpeed) / 288.0f;
      message.targetAid = this->hitTargetAid;
      message.time = timeGetTime() + delay;
      message.attackerAid = this->render.accountId;
      message.cellX = cellX;
      message.cellY = cellY;
      message.hitEffect = flag;
      message.effectId = effectId;
      message.blueDamage = info->blueDamage;
      message.action = action;
      message.damage = damage;
      message.monsterValue = 1;
      message.skillTarget = 0;

      const std::int64_t address = reinterpret_cast<int>(&message);
      if (hitCount == 1) {
        target->SendMsg(this, AM_WILL_BE_ATTACKED, address, 0, 0, 0, 0, 0);
      } else {
        const std::int64_t value = damage;
        target->SendMsg(this, AM_WILL_BE_ATTACKED, address, value, 0, 0, 0, 0);
      }

      const int job = this->GetEffectiveJob(-1);
      if (extraDamage != 0 && (g_session.IsNpcJob(job) || job > 4000 || job == 12)) {
        int total = damage;
        if (hitCount > 1) {
          for (int hit = hitCount - 1; hit != 0; --hit) {
            message.time += g_multiHitInterval / 2;
            total += damage;
            message.damage = damage;
            const std::int64_t value = total;
            target->SendMsg(this, AM_WILL_BE_ATTACKED, address, value, 0, 0, 0, 0);
            message.time -= g_multiHitInterval / 2;
          }
        }

        message.action = action;
        message.time += g_multiHitInterval / 2 + (g_multiHitInterval / 4 + g_multiHitInterval);
        message.damage = extraDamage;
        if (hitCount == 1) {
          target->SendMsg(this, AM_WILL_BE_ATTACKED, address, 0, 0, 0, 0, 0);
        } else {
          const std::int64_t value = total + extraDamage;
          target->SendMsg(this, AM_WILL_BE_ATTACKED, address, value, 0, 0, 0, 0);
        }
        return;
      }

      if (hitCount > 1) {
        int total = damage;
        for (int hit = hitCount - 1; hit != 0; --hit) {
          message.time += g_multiHitInterval;
          total += damage;
          message.damage = damage;
          const std::int64_t value = total;
          target->SendMsg(this, AM_WILL_BE_ATTACKED, address, value, 0, 0, 0, 0);
        }
      }
      break;
    }
    case DMG_PICKUP_ITEM:
      this->SetState(5, 1);
      return;
    case DMG_SIT_DOWN:
      this->SetState(6, 1);
      return;
    case DMG_STAND_UP:
      this->SetState(0, 1);
      return;
  }
}

// OFFSET: 0x00CC5C70
// FUNCTION: fn_21180
void C3dGrannyGameActor::ProcessMotion() {
  this->render.isMotionFinished = this->render.state != 2;
  if (this->render.state != 2 || !(this->render.motionSpeed < 1.0f)) {
    return;
  }

  this->clock = GrannyGetSystemSeconds();
  const float elapsed = GrannyGetSecondsElapsed(&this->lastClock, &this->clock) * 2.0f;
  this->motionProgress += elapsed / this->render.motionSpeed;
  this->model->Animate(this->motionProgress, this->instance);
  this->lastClock = this->clock;

  if (this->motion != 2 || this->controls[2] == nullptr || GrannyGetControlDurationLeft(this->controls[2]) > 0.0f) {
    return;
  }

  this->motion = 0;
  this->controls[this->motion] = this->model->PlayAnimation(0, this->instance, this->controls[0], 0);
  this->render.motionSpeed = 1.0f;
}

// OFFSET: 0x00CC5EE0
// FUNCTION: fn_21183
void CGrannyCorpse3d::Render(int argument) {
  const matrix& view = *reinterpret_cast<const matrix*>(argument);
  CTexture* texture = nullptr;
  std::uint16_t* indices = nullptr;
  RenderCommand* command;

  this->clock = GrannyGetSystemSeconds();
  const float elapsed = GrannyGetSecondsElapsed(&this->startClock, &this->clock) * 2.0f;
  this->animationTime += elapsed;
  this->model->Animate(this->animationTime, this->instance);
  this->startClock = this->clock;

  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const float sine = ClientSine(static_cast<int>(this->angle));
  const float cosine = ClientCosine(static_cast<int>(this->angle));
  scene->terrain->Height(cosine * 0.0f - sine * -40.0f + this->position.x, sine * 0.0f - cosine * 40.0f + this->position.z);
  scene->terrain->Height(cosine * 0.0f - sine * 40.0f + this->position.x, cosine * 40.0f + sine * 0.0f + this->position.z);

  matrix transform;
  std::memcpy(transform.values, g_identityMatrix, sizeof(g_identityMatrix));
  matrix rotation;
  rotation.MakeXRotation(std::strstr(this->name, "90") != nullptr ? 1.570796f : 3.141592f);
  rotation.AppendYRotation((this->angle + 180.0f) / 180.0f * 3.141592f);
  rotation.AppendTranslate(&this->position.x);

  vector3d position{0.0f, 0.0f, 0.0f};
  g_modeMgr.GetGameMode();
  matrix inverse;
  inverse.MakeInverse(view);
  float lightX = inverse.values[0] * 0.0f - inverse.values[3] * 300.0f + inverse.values[6] * 0.0f + inverse.values[9];
  float lightY = inverse.values[1] * 0.0f - inverse.values[4] * 300.0f + inverse.values[7] * 0.0f + inverse.values[10];
  float lightZ = inverse.values[2] * 0.0f - inverse.values[5] * 300.0f + inverse.values[8] * 0.0f + inverse.values[11];
  const float scale = 1.0f / SquareRoot(lightY * lightY + lightX * lightX + lightZ * lightZ);
  lightX *= scale;
  lightY *= scale;
  lightZ *= scale;

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->position.x, view.values, &screenX, &screenY, &reciprocalDepth);

  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    this->model->deformBuffer = this->vertices[mesh];
    int triangleCount;
    const GrannyPNT332Vertex* source = static_cast<const GrannyPNT332Vertex*>(
      this->model->GetMeshRenderData(mesh, &texture, &indices, &triangleCount, &transform, nullptr)
    );
    if (source == nullptr || texture == nullptr) {
      continue;
    }

    matrix local;
    local.Compose(transform, rotation);
    matrix world;
    world.Compose(local, view);

    const std::uint16_t* index = indices;
    int vertexCount = 0;
    for (int triangle = 0; triangle < triangleCount; ++triangle) {
      command = this->commands[mesh];
      for (int corner = 0; corner < 3; ++corner) {
        const GrannyPNT332Vertex& vertex = source[index[vertexCount]];
        RenderVertex& output = command->vertices[vertexCount];
        ++vertexCount;
        position.x = vertex.position[0];
        position.y = vertex.position[1];
        position.z = vertex.position[2];
        g_renderer->viewport.Project(&position.x, world.values, output);
        output.u = vertex.uv[0];
        output.v = vertex.uv[1];
        const float shade = lightX * vertex.normal[0] + lightY * vertex.normal[1] + lightZ * vertex.normal[2];
        const float clamped = 1.0f < shade ? 1.0f : shade;
        this->brightness = static_cast<unsigned>((0.0f > clamped ? 0.0f : clamped) * 100.0f + 155.0f);
        output.diffuse = (this->alpha << 24) + static_cast<unsigned char>(this->brightness) * 0x10101;
      }
    }

    command->texture = texture;
    command->vertexCount = vertexCount;
    g_renderer->queues.Submit(command, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}

// OFFSET: 0x00CC6860
// FUNCTION: fn_21184
void C3dGrannyGameActor::Unused6860(int) {
}

// OFFSET: 0x00CC7950
// FUNCTION: fn_21187
void C3dGrannyGameActor::Unused7950(int) {
}

// OFFSET: 0x00CCC890
// FUNCTION: fn_21197
bool CGameActor::IsSkillWithoutAction(int skillId) {
  switch (skillId) {
  case MG_NAPALMBEAT:
  case AC_SHOWER:
  case MO_COMBOFINISH:
  case TK_STORMKICK:
  case NPC_FATALMENACE:
  case NC_VULCANARM:
  case SC_STARTMARK:
  case LG_CANNONSPEAR:
  case LG_OVERBRAND:
  case SR_EARTHSHAKER:
  case SR_FALLENEMPIRE:
  case SR_RAMPAGEBLASTER:
  case SR_WINDMILL:
  case GN_CART_TORNADO:
  case LG_OVERBRAND_BRANDISH:
  case SJ_FULLMOONKICK:
  case KO_BAKURETSU:
  case KO_HAPPOKUNAI:
  case SU_SV_STEMSPEAR:
  case SU_SVG_SPIRIT:
  case AG_ASTRAL_STRIKE:
  case AG_CRIMSON_ARROW:
  case IQ_MASSIVE_F_BLASTER:
  case IQ_EXPOSION_BLASTER:
  case IQ_FIRST_BRAND:
  case IQ_SECOND_FLAME:
  case IQ_THIRD_FLAME_BOMB:
  case CD_PETITIO:
  case CD_FRAMEN:
  case ABC_CHAIN_REACTION_SHOT:
    return true;
  }

  return CSkillActionMgr::GetInstance()->IsSkillWithoutMotion(skillId);
}

// OFFSET: 0x00CCCAE0
// FUNCTION: fn_21198
int ReturnZero() {
  return 0;
}

// OFFSET: 0x00CC5D80
// FUNCTION: fn_21182
void C3dGrannyGameActor::Render(int argument) {
  this->litPositions.clear();
  this->clock = GrannyGetSystemSeconds();
  const float elapsed = GrannyGetSecondsElapsed(&this->lastClock, &this->clock) * 2.0f;
  this->motionProgress += elapsed;
  this->model->Animate(this->motionProgress, this->instance);
  this->lastClock = this->clock;

  if (this->motion == 2 && this->controls[2] != nullptr && !(GrannyGetControlDurationLeft(this->controls[2]) > 0.0f)) {
    this->motion = 0;
    this->controls[this->motion] = this->model->PlayAnimation(0, this->instance, this->controls[0], 0);
  }

  GrannyActorLight light;
  light.direction = g_weatherEffect.GetWindDirection();
  light.red = 200;
  light.green = 200;
  light.blue = 200;
  light.intensity = 0x80;
  switch (this->renderState) {
  case 1:
  case 2:
    return;
  }

  if (this->actorType == 2) {
    this->RenderShadowed(argument);
  } else {
    this->RenderLit(argument, light);
  }
}

// OFFSET: 0x00CC7180
// FUNCTION: fn_21186
void C3dGrannyGameActor::RenderLit(int argument, const GrannyActorLight& light) {
  const matrix& view = *reinterpret_cast<const matrix*>(argument);
  CTexture* texture = nullptr;
  std::uint16_t* indices = nullptr;
  if (this->render.accountId == 0xFFFFFFFF) {
    return;
  }

  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const float sine = ClientSine(static_cast<int>(this->angle));
  const float cosine = ClientCosine(static_cast<int>(this->angle));
  scene->terrain->Height(
    cosine * 0.0f - sine * -40.0f + this->render.position.x, sine * 0.0f - cosine * 40.0f + this->render.position.z
  );
  scene->terrain->Height(
    cosine * 0.0f - sine * 40.0f + this->render.position.x, cosine * 40.0f + sine * 0.0f + this->render.position.z
  );

  matrix transform;
  std::memcpy(transform.values, g_identityMatrix, sizeof(g_identityMatrix));
  matrix rotation;
  rotation.MakeXRotation(std::strstr(this->name, "90") != nullptr ? 1.570796f : 3.141592f);
  rotation.AppendYRotation((this->angle + 180.0f) / 180.0f * 3.141592f);
  vector3d position;
  position.y = this->render.position.y;
  position.x = this->render.position.x - 2.0f;
  position.z = this->render.position.z - 2.0f;
  rotation.AppendTranslate(&position.x);

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, view.values, &screenX, &screenY, &reciprocalDepth);

  matrix inverse;
  inverse.MakeInverse(view);
  float lightX =
    light.direction.x * inverse.values[0] + light.direction.y * inverse.values[3] + light.direction.z * inverse.values[6] + inverse.values[9];
  float lightY =
    light.direction.x * inverse.values[1] + light.direction.y * inverse.values[4] + light.direction.z * inverse.values[7] + inverse.values[10];
  float lightZ =
    light.direction.x * inverse.values[2] + light.direction.y * inverse.values[5] + light.direction.z * inverse.values[8] + inverse.values[11];
  const float scale = 1.0f / SquareRoot(lightY * lightY + lightX * lightX + lightZ * lightZ);
  lightX *= scale;
  lightY *= scale;
  lightZ *= scale;

  float bounds[5];
  bounds[0] = static_cast<float>(g_renderer->viewport.width);
  bounds[1] = static_cast<float>(g_renderer->viewport.height);
  bounds[2] = 0.0f;
  bounds[3] = 0.0f;
  bounds[4] = reciprocalDepth;

  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    this->model->deformBuffer = this->vertices[mesh];
    int triangleCount;
    const GrannyPNT332Vertex* source = static_cast<const GrannyPNT332Vertex*>(
      this->model->GetMeshRenderData(mesh, &texture, &indices, &triangleCount, &transform, nullptr)
    );
    if (source == nullptr || texture == nullptr) {
      continue;
    }

    matrix local;
    local.Compose(transform, rotation);
    matrix world;
    world.Compose(local, view);

    if (this->job == 0x49D && mesh == 28) {
      this->LaunchEffect(EF_DRAGONSMOKE, vector3d{source[64].position[0], source[64].position[1], source[64].position[2]}, -this->angle);
    }

    RenderCommand* command;
    const std::uint16_t* index = indices;
    int vertexCount = 0;
    for (int triangle = 0; triangle < triangleCount; ++triangle) {
      command = this->commands[mesh];
      for (int corner = 0; corner < 3; ++corner) {
        RenderVertex& output = command->vertices[vertexCount];
        const GrannyPNT332Vertex& vertex = source[index[vertexCount]];
        ++vertexCount;
        position.x = vertex.position[0];
        position.y = vertex.position[1];
        position.z = vertex.position[2];
        g_renderer->viewport.Project(&position.x, world.values, output);
        output.u = vertex.uv[0];
        output.v = vertex.uv[1];
        const float shade = lightX * vertex.normal[0] + lightY * vertex.normal[1] + lightZ * vertex.normal[2];
        const float clamped = 1.0f < shade ? 1.0f : shade;
        this->brightness = static_cast<unsigned>(
          (0.0f > clamped ? 0.0f : clamped) * static_cast<float>(light.intensity) + static_cast<float>(0xFF - light.intensity)
        );
        output.diffuse = (((light.green + 0x38 + this->brightness) << 8) & 0xFF00) +
                         (((light.red + 0x38 + this->brightness) << 16) & 0xFF0000) +
                         ((light.blue + 0x38 + this->brightness) & 0xFF) + (this->alpha << 24);
      }

      this->litPositions.push_back(position);
    }

    command->texture = texture;
    command->vertexCount = vertexCount;
    g_renderer->queues.Submit(command, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    this->meshTextures[mesh] = texture;
    this->meshIndices[mesh] = indices;
    this->meshTriangleCounts[mesh] = triangleCount;
    this->meshTransforms[mesh] = transform;
  }

  if (this->model->ProjectBounds(bounds, view, rotation, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.eye)) {
    this->SetRenderInfo(bounds, screenX, screenY);
  } else {
    this->SetTlvert(screenX, screenY);
  }
}

// OFFSET: 0x00CC68F0
// FUNCTION: fn_21185
void C3dGrannyGameActor::RenderShadowed(int argument) {
  const matrix& view = *reinterpret_cast<const matrix*>(argument);
  CTexture* texture = nullptr;
  std::uint16_t* indices = nullptr;
  if (this->render.accountId == 0xFFFFFFFF) {
    return;
  }

  this->brightness = (this->alpha << 24) + 0xFFFFFF;
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  matrix transform;
  std::memcpy(transform.values, g_identityMatrix, sizeof(g_identityMatrix));
  matrix rotation;
  rotation.MakeXRotation(std::strstr(this->name, "90") != nullptr ? 1.570796f : 3.141592f);
  rotation.AppendYRotation((this->angle + 180.0f) / 180.0f * 3.141592f);
  rotation.AppendTranslate(&this->render.position.x);
  matrix spin;
  spin.MakeZRotation(this->angle / 180.0f * 3.141592f);
  matrix world;
  world.Compose(rotation, view);

  vector3d position{0.0f, 0.0f, 0.0f};
  vector3d light{0.0f, 0.0f, 0.0f};
  mode->camera->frame.Forward(mode->camera->camera, &light.x);
  const vector3d direction = light;
  light.x = direction.y * view.values[3] + direction.x * view.values[0] + direction.z * view.values[6] + view.values[9];
  light.y = direction.y * view.values[4] + direction.x * view.values[1] + direction.z * view.values[7] + view.values[10];
  light.z = direction.y * view.values[5] + direction.x * view.values[2] + direction.z * view.values[8] + view.values[11];
  const float scale = 1.0f / SquareRoot(light.y * light.y + light.x * light.x + light.z * light.z);
  light.x *= scale;
  light.y *= scale;
  light.z *= scale;

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, view.values, &screenX, &screenY, &reciprocalDepth);

  float bounds[5];
  bounds[0] = static_cast<float>(g_renderer->viewport.width);
  bounds[1] = static_cast<float>(g_renderer->viewport.height);
  bounds[2] = 0.0f;
  bounds[3] = 0.0f;
  bounds[4] = reciprocalDepth;

  RenderCommand* command;
  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    this->model->deformBuffer = this->vertices[mesh];
    int triangleCount;
    const GrannyPNT332Vertex* source = static_cast<const GrannyPNT332Vertex*>(
      this->model->GetMeshRenderData(mesh, &texture, &indices, &triangleCount, &transform, nullptr)
    );
    if (source == nullptr) {
      continue;
    }

    if (this->job == 0x49D && mesh == 28) {
      this->LaunchEffect(EF_DRAGONSMOKE, vector3d{source[64].position[0], source[64].position[1], source[64].position[2]}, -this->angle);
    }

    int vertexCount = 0;
    for (int triangle = 0; triangle < triangleCount; ++triangle) {
      command = this->commands[mesh];
      for (int corner = 0; corner < 3; ++corner) {
        RenderVertex& output = command->vertices[vertexCount];
        const GrannyPNT332Vertex& vertex = source[indices[vertexCount]];
        ++vertexCount;
        position.x = vertex.position[0];
        position.y = vertex.position[1];
        position.z = vertex.position[2];
        g_renderer->viewport.Project(&position.x, world.values, output);
        output.u = vertex.uv[0];
        output.v = vertex.uv[1];
        const float shade = light.y * vertex.normal[1] + light.x * vertex.normal[0] + light.z * vertex.normal[2];
        const float limited = 1.0f < shade ? 1.0f : shade;
        this->brightness = static_cast<unsigned>((0.0f > limited ? 0.0f : limited) * 100.0f + 155.0f);
        output.diffuse = static_cast<unsigned char>(this->brightness) * 0x10101 + (this->alpha << 24);
      }
    }

    const vector3d corner = position;
    position.y = spin.values[4] * corner.y + spin.values[1] * corner.x + spin.values[7] * corner.z + spin.values[10];
    position.x = spin.values[0] * corner.x + spin.values[3] * corner.y + spin.values[6] * corner.z + spin.values[9] +
                 static_cast<float>(static_cast<int>(this->render.position.x) % 5);
    position.z = spin.values[5] * corner.y + spin.values[2] * corner.x + spin.values[8] * corner.z + spin.values[11] +
                 static_cast<float>(static_cast<int>(this->render.position.z) % 5);
    this->litPositions.push_back(position);

    CTexture* meshTexture;
    if (mesh == 1) {
      const std::string name = g_guildMgr->GetEmblemFilename(this->GetGdid());
      CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(name));
      if (emblem != nullptr) {
        if (this->emblemTexture == nullptr) {
          this->emblemTexture = g_texMgr.Upload(24, 24, emblem->gif.image.pixels, 0, 0, false, false, false);
        } else {
          this->emblemTexture->Upload(0, 0, 24, 24, emblem->gif.image.pixels, 0, 0);
        }
      }

      meshTexture = this->emblemTexture;
    } else {
      meshTexture = texture;
    }

    command->texture = meshTexture;
    command->vertexCount = vertexCount;
    if (meshTexture != nullptr) {
      g_renderer->queues.Submit(command, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }
  }

  if (this->model->ProjectBounds(bounds, view, rotation, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.eye)) {
    this->SetRenderInfo(bounds, screenX, screenY);
  } else {
    this->SetTlvert(screenX, screenY);
  }
}

// OFFSET: 0x00CC3E50
// FUNCTION: fn_21156
void CGameActor::SetCharm(int type, int count) {
  this->SendMsg(this, AM_DELETE_EFFECT, EF_CHOOKGI_WATER, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_CHOOKGI_GROUND, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_CHOOKGI_FIRE, 0, 0, 0, 0, 0);
  this->SendMsg(this, AM_DELETE_EFFECT, EF_CHOOKGI_WIND, 0, 0, 0, 0, 0);
  if (count <= 0 || StateFlag01(this->GetStateFlags()) || StateFlag02(this->GetStateFlags())) {
    return;
  }

  this->charmType = type;
  this->charmCount = count;
  if (g_effectMgr->HasAllocator(0xE4)) {
    CEffect* effect;
    if (type == 1) {
      effect = g_effectMgr->Spawn(
        this->render.accountId, EF_CHOOKGI_WATER, this->render.position, 0, -1, 0, 0
      );
    } else if (type == 2) {
      effect = g_effectMgr->Spawn(
        this->render.accountId, EF_CHOOKGI_GROUND, this->render.position, 0, -1, 0, 0
      );
    } else if (type == 3) {
      effect = g_effectMgr->Spawn(
        this->render.accountId, EF_CHOOKGI_FIRE, this->render.position, 0, -1, 0, 0
      );
    } else if (type == 4) {
      effect = g_effectMgr->Spawn(
        this->render.accountId, EF_CHOOKGI_WIND, this->render.position, 0, -1, 0, 0
      );
    } else if (type == 5) {
      effect = g_effectMgr->Spawn(
        this->render.accountId, EF_CHOOKGI, this->render.position, 0, -1, 0, 0
      );
    } else {
      return;
    }

    if (effect != nullptr) {
      static_cast<CEnergyOrbEffect*>(effect)->SetCount(count);
    }
  } else {
    CRagEffect* effect;
    if (type == 1) {
      effect = this->LaunchEffect(EF_CHOOKGI_WATER, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else if (type == 2) {
      effect = this->LaunchEffect(EF_CHOOKGI_GROUND, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else if (type == 3) {
      effect = this->LaunchEffect(EF_CHOOKGI_FIRE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else if (type == 4) {
      effect = this->LaunchEffect(EF_CHOOKGI_WIND, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else {
      effect = this->LaunchEffect(EF_CHOOKGI, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }

    vector3d position{static_cast<float>(count), 0.0f, 0.0f};
    if (effect != nullptr) {
      effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00CAA4D0
// FUNCTION: fn_21017
void CGameActor::UpdateMaxLevelAura() {
  if (g_serviceOptions.Get(0x6b) != 0) {
    return;
  }

  if (g_session.IsTreeGroupTwo(this->GetEffectiveJob(-1))) {
    if (g_session.m_maxLevel == static_cast<std::int16_t>(this->characterLevel)) {
      this->SendMsg(nullptr, 0x96, 0, 0, 0, 0, 0, 0);
      return;
    }
    this->hasLevelAura = 0;
    return;
  }

  if (g_session.IsTreeGroupThree(this->GetEffectiveJob(-1)) == true) {
    if (g_session.m_maxBaseLevel4th == static_cast<std::int16_t>(this->characterLevel)) {
      this->SendMsg(nullptr, AM_MAKE_LEVEL4TH_EFFECT, 0, 0, 0, 0, 0, 0);
      return;
    }
    this->hasLevelAura = 0;
    return;
  }

  if (g_session.IsSuperNovice2Job(this->GetEffectiveJob(-1))) {
    if (g_session.m_maxBaseLevelExtend2 == static_cast<std::int16_t>(this->characterLevel)) {
      this->SendMsg(nullptr, AM_MAKE_LEVEL160_EFFECT, 0, 0, 0, 0, 0, 0);
      return;
    }
    this->hasLevelAura = 0;
    return;
  }

  if (g_session.IsExpandedJob(this->GetEffectiveJob(-1))) {
    if (g_session.m_maxBaseLevelUpperJob == static_cast<std::int16_t>(this->characterLevel)) {
      this->SendMsg(nullptr, AM_MAKE_LEVEL160_EFFECT, 0, 0, 0, 0, 0, 0);
      return;
    }
    this->hasLevelAura = 0;
    return;
  }

  if (g_session.IsMercenaryJob(this->GetEffectiveJob(-1))) {
    const int skill = static_cast<std::int16_t>(this->characterLevel);
    if (g_session.m_maxBaseLevelHomun == skill) {
      this->SendMsg(nullptr, 0x93, 0, 0, 0, 0, 0, 0);
      return;
    }
    if (g_session.m_maxBaseLevel != skill || g_session.IsNewMercenaryJob(this->GetEffectiveJob(-1)) != 0) {
      this->hasLevelAura = 0;
      return;
    }
  } else if (g_session.IsDoramJob(this->GetEffectiveJob(-1)) == true) {
    if (g_session.m_maxBaseLevelDoram == static_cast<std::int16_t>(this->characterLevel)) {
      this->SendMsg(nullptr, AM_MAKE_LEVEL160_EFFECT, 0, 0, 0, 0, 0, 0);
      return;
    }
    this->hasLevelAura = 0;
    return;
  } else if (g_session.m_maxBaseLevel != static_cast<std::int16_t>(this->characterLevel)) {
    this->hasLevelAura = 0;
    return;
  }

  this->SendMsg(nullptr, AM_MAKE_LEVEL99_EFFECT, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00CC2340
// FUNCTION: fn_21139
void CGameActor::SetActorJob(int job) {
  this->job = job;
  switch (job) {
  case JT_CHICKEN_H:
    this->job = 0xfa8;
    break;
  case JT_CHICKEN:
    this->job = 7;
    break;
  case JT_CHICKEN2:
    this->job = 0xe;
    break;
  case JT_WOLF_WINDHAWK:
    this->job = 0x10a1;
    break;
  case JT_CHICKEN_B:
    this->job = 0xfbe;
    break;
  case JT_CHICKEN2_H:
    this->job = 0xfaf;
    break;
  case JT_CHICKEN2_B:
    this->job = 0xfc5;
    break;
  case JT_STAR2:
    this->job = 0xfcf;
    break;
  case JT_ROYAL_CHICKEN:
    this->job = 0xfe2;
    break;
  case JT_ROYAL_CHICKEN_H:
    this->job = 0xfe9;
    break;
  case JT_ROYAL_CHICKEN_B:
    this->job = 0x1006;
    break;
  case JT_RUNE_CHICKEN_B:
    this->job = 0x1000;
    break;
  case JT_RUNE_CHICKEN:
  case JT_RUNE_CHICKEN2:
  case JT_RUNE_CHICKEN3:
  case JT_RUNE_CHICKEN4:
  case JT_RUNE_CHICKEN5:
    this->job = 0xfd6;
    break;
  case JT_RUNE_CHICKEN_H:
  case JT_RUNE_CHICKEN2_H:
  case JT_RUNE_CHICKEN3_H:
  case JT_RUNE_CHICKEN4_H:
  case JT_RUNE_CHICKEN5_H:
    this->job = 0xfdc;
    break;
  case JT_WOLF_RANGER:
    this->job = 0xfd8;
    break;
  case JT_WOLF_RANGER_H:
    this->job = 0xfde;
    break;
  case JT_WOLF_RANGER_B:
    this->job = 0x1002;
    break;
  case JT_MADOGEAR:
    this->job = 0xfda;
    break;
  case JT_MADOGEAR_H:
    this->job = 0xfe0;
    break;
  case JT_MADOGEAR_B:
    this->job = 0x1004;
    break;
  case JT_STAR2_B:
    this->job = 0x1082;
    break;
  case JT_STAR2_EMPEROR_B:
    this->job = 0x1091;
    break;
  case JT_MEISTER_MADOGEAR:
    this->job = 0x109d;
    break;
  case JT_DRAGON_KNIGHT_CHICKEN:
    this->job = 0x109c;
    break;
  case JT_IMPERIAL_GUARD_CHICKEN:
    this->job = 0x10a2;
    break;
  case JT_SKY_EMPEROR2:
    this->job = 0x10ce;
    break;
  }

  int slotJob;
  if (g_session.IsCostumeJob(this->GetEffectiveJob(-1)) == true || this->shapeShift == 0) {
    slotJob = this->GetEffectiveJob(-1);
  } else {
    switch (this->shapeShift) {
    case 1:
      slotJob = 0x1104;
      break;
    case 2:
      slotJob = 0x1105;
      break;
    default:
      slotJob = this->job;
      break;
    }
  }

  this->SetSprJob(slotJob);
  this->ResetJobEffects();
  this->UpdateMaxLevelAura();
}

// OFFSET: 0x00CAAFD0
// FUNCTION: fn_21023
void CGameActor::ClearSiegeNameWindow() {
  if (g_session.GetSiegeMode() != 0 && this->sceneNameWindow != nullptr) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->RemoveNotificationWindow(reinterpret_cast<int>(this->sceneNameWindow));
    g_windowMgr.Schedule(this->sceneNameWindow);
    this->sceneNameWindow = nullptr;
  }
}

// OFFSET: 0x00CAE5E0
// FUNCTION: fn_21075
void CGameActor::RefreshGuildEmblemWindow() {
  if (g_session.GetSiegeMode() == 0) {
    return;
  }
  if (g_session.m_aid == this->render.accountId) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGuildMgr* guildMgr = g_guildMgr;
  mode->ShowGuildEmblem(this, this->GetGdid(), guildMgr->GetEmblemVersion(static_cast<std::uint32_t>(this->GetGdid())));
}

// OFFSET: 0x00CC4640
// FUNCTION: fn_21165
void CGameActor::NotifyCellMove(const float* x, const float* z) {
  if (this->GetGdid() < 1) {
    return;
  }

  if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) == 0) {
    return;
  }

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (this->render.accountId == g_session.m_aid) {
    world->PlayerMove(*x, *z);
  } else {
    CellFloatPos pos;
    pos.x = *x;
    pos.z = *z;
    world->PcApproachToPlayer(this->render.accountId, pos);
  }
}

// OFFSET: 0x00CC5750
// FUNCTION: fn_21177
void C3dGrannyGameActor::MakeCorpse() {
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->SpawnGrannyCorpse(
    this->render.position.x, this->render.position.y, this->render.position.z, this->angle, this->name
  );
}

// OFFSET: 0x00CC40D0
// FUNCTION: fn_21158
void CGameActor::ApplyNatureProtection() {
  if (this->shapeShift == 3) {
    return;
  }

  this->savedShapeShift = this->shapeShift;
  this->SetState(0x51, 1);
  this->render.isMotionFreezed = true;
  this->render.specialMotionLocked = 1;
  this->shapeShift = 3;
  this->SetSprJob(this->job);
  const int costume = this->GetMadogearType();
  const int costumeJob = this->GetBodyCostume();
  this->render.name = g_equipmentResourceMgr->GetBodyPalName(
    this->GetEffectiveJob(-1), this->sex, static_cast<int>(this->render.bodyPalette), costumeJob, costume, 0
  );
}

__declspec(noinline) void CGameActor::ProcessViewCheck() {
  if (g_updateTick > 0 && (this->render.effectPhaseState & 0xFFu) == 1) {
    this->render.flags = 0;
    this->render.effectPhaseState &= 0xFFFFFF00u;
  }

  if (StateFlagsClear(this->stateFlags)) {
    this->effectLaunchCnt = 0x8ca0;
    return;
  }

  timeGetTime();
  if (StateFlag00(this->stateFlags)) {
    if (static_cast<unsigned int>(EffectLifetime(0x16)) <= static_cast<unsigned int>(this->effectLaunchCnt)) {
      this->effectLaunchCnt = 0;
      vector3d zero;
      zero.x = 0.0f;
      zero.y = 0.0f;
      zero.z = 0.0f;
      this->LaunchEffect(EF_SIGHT, zero, 0.0f);
    }
  }

  if (StateFlag13(this->stateFlags)) {
    if (static_cast<unsigned int>(EffectLifetime(0x21)) <= static_cast<unsigned int>(this->effectLaunchCnt)) {
      this->effectLaunchCnt = 0;
      vector3d zero;
      zero.x = 0.0f;
      zero.y = 0.0f;
      zero.z = 0.0f;
      this->LaunchEffect(EF_RUWACH, zero, 0.0f);
    }
  }

  ++this->effectLaunchCnt;
}

__declspec(noinline) void CGameActor::ProcessActorEffects() {
  for (auto it = this->willBeAttackedList.begin(); it != this->willBeAttackedList.end();) {
    if (timeGetTime() < it->time) {
      ++it;
      continue;
    }

    ActorDamageMessage& message = *it;
    this->SendMsg(nullptr, message.action, reinterpret_cast<std::uint32_t>(&message.targetAid), 0, 0, 0, 0, 0);
    it = this->willBeAttackedList.erase(it);
  }
}

// OFFSET: 0x00CC59B0
// FUNCTION: fn_21178
bool C3dGrannyGameActor::OnProcess() {
  this->ProcessMotion();
  this->ProcessViewCheck();
  this->ProcessActorEffects();
  this->UpdateHpGauge();

  const bool result = this->ProcessState();

  float facing = this->render.angle;
  if (facing > 360.0f) {
    facing = static_cast<float>(static_cast<int>(facing) % 360);
    this->render.angle = facing;
  }

  if (this->initializeFacing == 1) {
    this->angle = 180.0f - facing;
    this->initializeFacing = 0;
  } else {
    float current = this->angle;
    const int target = static_cast<int>(180.0f - static_cast<int>(facing));
    if (current > 360.0f) {
      current = static_cast<float>(static_cast<int>(current) % 360);
      this->angle = current;
    }
    if (current < 0.0f) {
      current = current + 360.0f;
      this->angle = current;
    }

    int delta = static_cast<int>(current - target);
    if (delta > 360 || delta < -360) {
      delta += delta / -360 * 360;
    }

    const int absolute = delta < 0 ? delta + 360 : delta;
    if (absolute > 12) {
      if (absolute < 180) {
        this->angle = current - 6.0f;
      } else {
        this->angle = current + 6.0f;
      }
    }
  }

  return result;
}

// OFFSET: 0x00CC9F20
// FUNCTION: fn_21195
void CGameActor::SpawnDamageDigits(int, int damage, std::uint32_t color, int type) {
  std::vector<int> digits;
  const int value = damage >= 0 ? damage : 0;
  int remaining = value;
  int digit;
  if (g_serviceOptions.Get(0x10B)) {
    if (value >= 100000000) {
      digit = 11;
      remaining = value / 1000000;
      digits.push_back(digit);
    } else if (value >= 1000000) {
      digit = 10;
      remaining = value / 1000;
      digits.push_back(digit);
    }
  } else if (value > 999999) {
    remaining = 999999;
  }

  do {
    digit = remaining % 10;
    digits.push_back(digit);
    remaining /= 10;
  } while (remaining != 0);

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (type == 22 && this->msgEffectList.size() > 0) {
    for (auto it = this->msgEffectList.begin(); it != this->msgEffectList.end();) {
      CMsgEffect* effect = static_cast<CMsgEffect*>(*it);
      if (effect->m_type == 22) {
        this->ClearStatusEffectInstance(effect);
        effect->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
        it = this->msgEffectList.erase(it);
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->RemoveObject(effect);
      } else {
        ++it;
      }
    }
  }

  int index = 0;
  for (auto it = digits.begin(); it != digits.end(); ++it, ++index) {
    CMsgEffect* effect = new CMsgEffect;
    effect->render.color = color;
    effect->SendMsg(nullptr, AM_SETMASTERGID, static_cast<std::uint32_t>(this->render.accountId), type, 0, 0, 0, 0);
    const std::intptr_t position = reinterpret_cast<std::intptr_t>(&this->render.position);
    const int count = static_cast<int>(digits.size());
    if (value >= 100000000 && g_session.m_damageMillionUnit == 2) {
      if (index == 0) {
        effect->SendMsg(nullptr, AM_SETPOS2, position, *it, static_cast<std::uint32_t>((count - 1) * 3 + 4), 0, 0, 0);
      } else {
        effect->SendMsg(nullptr, AM_SETPOS2, position, *it, static_cast<std::uint32_t>((count - 1) * 3 - index * 8), 0, 0, 0);
      }
    } else {
      effect->SendMsg(nullptr, AM_SETPOS2, position, *it, static_cast<std::uint32_t>((count - index * 2) * 4 - 4), 0, 0, 0);
    }
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    effect->render.useGroundLighting = 0;
    if (type == 22) {
      effect->render.scale = 0.5f;
    }
    mode->AddSceneObject(effect);
    this->msgEffectList.push_back(effect);
  }
}

// OFFSET: 0x005A10B0
// FUNCTION: fn_03040
CCharacterNpc::CCharacterNpc() : CNpc() {
  this->characterRender = nullptr;
}

// INLINED
CCharacterNpc::~CCharacterNpc() {
  if (this->characterRender) {
    delete this->characterRender;
    this->characterRender = nullptr;
  }
}

// OFFSET: 0x00CBFC40
// FUNCTION: fn_21120
void CGameActor::SetStatusStates(int bodyState, std::uint32_t stateFlags, int effectState, bool notify) {
  const int previousBodyState = this->bodyState;
  const std::uint32_t previousFlags = this->stateFlags;
  this->bodyState = bodyState;
  this->stateFlags = stateFlags;
  this->effectState = effectState;
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  unsigned char* const color = reinterpret_cast<unsigned char*>(&this->render.color);

  if (StateFlag00Changed(this->stateFlags, previousFlags) && StateFlag00(this->stateFlags) && world && world->player) {
    const vector3d& player = static_cast<const CRenderObject*>(world->player)->render.position;
    g_soundMgr->PlayWave(
      "effect\\EF_Sight.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
  }

  if (StateFlag13Changed(this->stateFlags, previousFlags) && StateFlag13(this->stateFlags) && world && world->player) {
    const vector3d& player = static_cast<const CRenderObject*>(world->player)->render.position;
    g_soundMgr->PlayWave(
      "effect\\EF_Sight.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
  }

  if (StateFlag01Changed(this->stateFlags, previousFlags)) {
    this->ResetJobEffects();
    if (this->GetEffectiveJob(-1) == 45) {
      this->SendMsg(this, AM_DELETE_EFFECT, EF_WARPZONE2, 0, 0, 0, 0, 0);
    }

    if (StateFlag01(this->stateFlags)) {
      if (notify) {
        this->SendMsg(nullptr, AM_MAKE_SMOKE_FFECT, 0, 0, 0, 0, 0, 0);
        if (this->GetEffectiveJob(-1) != 25 && this->GetEffectiveJob(-1) != 4222) {
          this->LaunchEffect(EF_BASH, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      }

      this->render.enabled = 0;
      if (this->render.accountId != g_session.m_aid && g_session.IsAdminSubMode(7) == 0) {
        this->render.visibilityOptions[0] = 0;
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_TOPRANK, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_REMOVEGAGE, 0, 0, 0, 0, 0, 0);
      }
      if (this->maxHp > 0) {
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag04(this->stateFlags)) {
        this->SendMsg(nullptr, AM_DELETE_BIRD, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag20(this->stateFlags)) {
        this->SendMsg(nullptr, AM_DELETE_WOLF, 0, 0, 0, 0, 0, 0);
      }
      if (!g_session.IsGuildMemberAccount(this->render.accountId)) {
        this->ClearSiegeNameWindow();
      }
      if (this->actorTitleWindow) {
        g_windowMgr.Schedule(this->actorTitleWindow);
        this->actorTitleWindow = nullptr;
      }
    } else {
      if (notify) {
        this->SendMsg(nullptr, AM_MAKE_SMOKE_FFECT, 0, 0, 0, 0, 0, 0);
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
      }

      this->render.enabled = 1;
      if (this->render.accountId != g_session.m_aid) {
        this->render.visibilityOptions[0] = 1;
      }
      if (this->maxHp > 0) {
        this->SendMsg(nullptr, AM_MAKE_CART, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag04(this->stateFlags)) {
        this->SendMsg(nullptr, AM_MAKE_BIRD, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag20(this->stateFlags)) {
        this->SendMsg(nullptr, AM_MAKE_WOLF, 0, 0, 0, 0, 0, 0);
      }
      this->RefreshGuildEmblemWindow();
      if (this->hasLevelAura != 0) {
        this->UpdateMaxLevelAura();
      }
      if (this->GetEffectiveJob(-1) == 45) {
        this->LaunchEffect(EF_WARPZONE2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
  }

  if (StateFlag02Changed(this->stateFlags, previousFlags)) {
    this->ResetJobEffects();
    if (StateFlag02(this->stateFlags)) {
      this->render.visible = 0;
      this->render.enabled = 0;
      if (this->render.accountId != g_session.m_aid && g_session.IsAdminSubMode(7) == 0) {
        this->render.visibilityOptions[0] = 0;
        this->SendMsg(nullptr, AM_REMOVEGAGE, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_TOPRANK, 0, 0, 0, 0, 0);
      }
      if (this->maxHp > 0) {
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag04(this->stateFlags)) {
        this->SendMsg(nullptr, AM_DELETE_BIRD, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag20(this->stateFlags)) {
        this->SendMsg(nullptr, AM_DELETE_WOLF, 0, 0, 0, 0, 0, 0);
      }
      if (!g_session.IsGuildMemberAccount(this->render.accountId)) {
        this->ClearSiegeNameWindow();
      }
      if (this->actorTitleWindow) {
        g_windowMgr.Schedule(this->actorTitleWindow);
        this->actorTitleWindow = nullptr;
      }
    } else {
      this->render.visible = 1;
      this->render.enabled = 1;
      if (this->render.accountId != g_session.m_aid) {
        this->render.visibilityOptions[0] = 1;
      }
      if (this->maxHp > 0) {
        this->SendMsg(nullptr, AM_MAKE_CART, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag04(this->stateFlags)) {
        this->SendMsg(nullptr, AM_MAKE_BIRD, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag20(this->stateFlags)) {
        this->SendMsg(nullptr, AM_MAKE_WOLF, 0, 0, 0, 0, 0, 0);
      }
      this->RefreshGuildEmblemWindow();
      if (this->hasLevelAura != 0) {
        this->UpdateMaxLevelAura();
      }
    }
  }

  if (!StateFlag01(this->stateFlags) && !StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags)) {
    if (StateFlag04Changed(this->stateFlags, previousFlags)) {
      this->SendMsg(nullptr, StateFlag04(this->stateFlags) ? AM_MAKE_BIRD : AM_DELETE_BIRD, 0, 0, 0, 0, 0, 0);
    }
    if (StateFlag20Changed(this->stateFlags, previousFlags)) {
      this->SendMsg(nullptr, StateFlag20(this->stateFlags) ? AM_MAKE_WOLF : AM_DELETE_WOLF, 0, 0, 0, 0, 0, 0);
    }
  }

  if (StateFlag19Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag19(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag23Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag23(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag24Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag24(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag25Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag25(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag26Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag26(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag21Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag21(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag05Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag05(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }
  if (StateFlag15Changed(this->stateFlags, previousFlags)) {
    this->SendMsg(nullptr, StateFlag15(this->stateFlags) ? AM_MAKE_CHICKEN : AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
  }

  if (StateFlag06Changed(this->stateFlags, previousFlags)) {
    this->ResetJobEffects();
    if (StateFlag06(this->stateFlags)) {
      const int admin = g_session.IsAdminSubMode(8);
      this->render.visibilityOptions[0] = 0;
      if (admin == 0) {
        this->render.visible = 0;
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_TOPRANK, 0, 0, 0, 0, 0);
        if (this->msgEffectList.size() != 0) {
          for (CGameObject* object : this->msgEffectList) {
            static_cast<CMsgEffect*>(object)->m_visible = this->render.accountId == g_session.m_aid;
          }
        }
      } else {
        this->render.visible = 1;
      }
      if (this->maxHp > 0) {
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag04(this->stateFlags)) {
        this->SendMsg(nullptr, AM_DELETE_BIRD, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag20(this->stateFlags)) {
        this->SendMsg(nullptr, AM_DELETE_WOLF, 0, 0, 0, 0, 0, 0);
      }
      if (!g_session.IsGuildMemberAccount(this->render.accountId)) {
        this->ClearSiegeNameWindow();
      }
      if (this->actorTitleWindow) {
        g_windowMgr.Schedule(this->actorTitleWindow);
        this->actorTitleWindow = nullptr;
      }
    } else {
      this->render.visible = 1;
      this->render.visibilityOptions[0] = 1;
      if (!this->render.enabled && this->render.accountId != g_session.m_aid) {
        this->render.visibilityOptions[0] = 0;
      }
      if (this->msgEffectList.size() != 0) {
        for (CGameObject* object : this->msgEffectList) {
          static_cast<CMsgEffect*>(object)->m_visible = true;
        }
      }
      if (this->maxHp > 0) {
        this->SendMsg(nullptr, AM_MAKE_CART, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag04(this->stateFlags)) {
        this->SendMsg(nullptr, AM_MAKE_BIRD, 0, 0, 0, 0, 0, 0);
      }
      if (StateFlag20(this->stateFlags)) {
        this->SendMsg(nullptr, AM_MAKE_WOLF, 0, 0, 0, 0, 0, 0);
      }
      this->RefreshGuildEmblemWindow();
      if (this->hasLevelAura != 0) {
        this->UpdateMaxLevelAura();
      }
    }
  }

  const std::uint32_t accountId = this->render.accountId;
  g_championMonsterMgr->ApplyEffects(this->GetEffectiveJob(-1), this, static_cast<int>(accountId));

  if (previousBodyState != this->bodyState) {
    switch (previousBodyState) {
    case OPT1_STONE: {
      const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave(
        "_stone_explosion.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
      break;
    }
    case OPT1_FREEZE: {
      this->SendMsg(nullptr, AM_DELETE_ICEEFFECT, 0, 0, 0, 0, 0, 0);
      this->SendMsg(nullptr, AM_MAKE_ICEBREAKE_FFECT, 0, 0, 0, 0, 0, 0);
      const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave(
        "_frozen_explosion.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
      this->render.isMotionFinished = true;
      this->render.isMotionFreezed = false;
      this->render.colorLocked = 0;
      color[1] = 0xFF;
      color[2] = 0xFF;
      color[0] = 0xFF;
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
      break;
    }
    case OPT1_STUN:
      this->SendMsg(nullptr, AM_DELETE_STUN_EFFECT, 0, 0, 0, 0, 0, 0);
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
      break;
    case OPT1_SLEEP:
      this->SendMsg(nullptr, AM_DELETE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0);
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
      break;
    case OPT1_BURNING:
      this->SendMsg(nullptr, AM_DELETE_BURNING_EFFECT, 0, 0, 0, 0, 0, 0);
      break;
    case OPT1_IMPRISON:
      while (this->HasId(EF_BOTTOM_BASILICA2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BOTTOM_BASILICA2, 0, 0, 0, 0, 0);
      }
      break;
    default:
      break;
    }

    switch (this->bodyState) {
    case OPT1_STONE:
      this->render.isMotionFinished = true;
      this->render.isMotionFreezed = true;
      this->render.colorLocked = 1;
      color[1] = 0x40;
      color[2] = 0x40;
      color[0] = 0x40;
      break;
    case OPT1_FREEZE:
      this->SendMsg(nullptr, AM_MAKE_ICEEFFECT, 0, 0, 0, 0, 0, 0);
      this->render.isMotionFinished = true;
      this->render.isMotionFreezed = true;
      this->render.colorLocked = 1;
      color[1] = 0x80;
      color[2] = 0x00;
      color[0] = 0xFF;
      if (notify) {
        const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
        g_soundMgr->PlayWave(
          "_stonecurse.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
        );
      }
      break;
    case OPT1_STUN:
      this->SendMsg(nullptr, AM_MAKE_STUN_EFFECT, 0, 0, 0, 0, 0, 0);
      if (notify) {
        const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
        g_soundMgr->PlayWave(
          "_stun.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
        );
      }
      break;
    case OPT1_SLEEP:
      this->SendMsg(nullptr, AM_MAKE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0);
      break;
    case OPT1_STONEWAIT:
      this->render.colorLocked = 1;
      color[1] = 0x80;
      color[2] = 0x80;
      color[0] = 0x80;
      break;
    case OPT1_BURNING:
      this->SendMsg(nullptr, AM_MAKE_BURNING_EFFECT, 0, 0, 0, 0, 0, 0);
      break;
    case OPT1_IMPRISON:
      if (!this->HasId(EF_BOTTOM_BASILICA2)) {
        this->LaunchEffect(EF_BOTTOM_BASILICA2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    default:
      break;
    }
  }

  bool poisonSound = false;
  if (this->effectState & 0x80) {
    if (this->render.accountId == g_session.m_aid) {
      g_playerPoisoned = 1;
    }
    this->render.colorLocked = 1;
    color[1] = 0x32;
    color[2] = 0xFF;
    color[0] = 0xFF;
    poisonSound = true;
  } else if (this->effectState & 0x01) {
    if (this->render.accountId == g_session.m_aid) {
      g_playerPoisoned = 1;
    }
    if (!this->HasId(EF_BLACKBODY)) {
      this->render.colorLocked = 1;
      color[1] = 0x80;
      color[2] = 0xFF;
      color[0] = 0xFF;
    }
    poisonSound = true;
  } else if (this->render.accountId == g_session.m_aid) {
    if (g_playerPoisoned == 1) {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_POISON, 0, 0, 0, 0, 0);
    }
    g_playerPoisoned = 0;
  }

  if (poisonSound && notify) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const vector3d& player = static_cast<const CRenderObject*>(mode->scene->player)->render.position;
    g_soundMgr->PlayWave(
      "_poison.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    if (this->render.accountId == g_session.m_aid && !mode->scene->player->HasId(EF_POISON)) {
      this->LaunchEffect(EF_POISON, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (this->effectState & 0x02) {
    this->SendMsg(nullptr, AM_MAKE_CURSE_EFFECT, 0, 0, 0, 0, 0, 0);
    this->render.colorLocked = 1;
    color[1] = 0x32;
    color[2] = 0xC8;
    color[0] = 0x32;
    if (notify) {
      const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave(
        "_curse.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
    }
  } else {
    this->SendMsg(nullptr, AM_DELETE_CURSE_EFFECT, 0, 0, 0, 0, 0, 0);
  }

  if (this->effectState & 0x04) {
    if (notify) {
      const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave(
        "_silence.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
    }
    this->SendMsg(nullptr, AM_MAKE_SILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, AM_DELETE_SILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
  }

  this->SendMsg(nullptr, (this->effectState & 0x20) ? AM_MAKE_ANGELUS_EFFECT : AM_DELETE_ANGELUS_EFFECT, 0, 0, 0, 0, 0, 0);

  if ((this->effectState & 0x08) && notify) {
    const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      "_confusion.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
  }

  if (this->effectState & 0x10) {
    if (this->render.accountId == g_session.m_aid) {
      g_overrideColor = 1;
    }
    if (notify) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const vector3d& player = static_cast<const CRenderObject*>(mode->scene->player)->render.position;
      g_soundMgr->PlayWave(
        "_blind.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
      if (this->render.accountId == g_session.m_aid && !mode->scene->player->HasId(EF_BLIND)) {
        this->LaunchEffect(EF_BLIND, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
  } else if (this->render.accountId == g_session.m_aid) {
    if (static_cast<int>(this->render.instant.Get(16)) == 0) {
      if (g_overrideColor == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BLIND, 0, 0, 0, 0, 0);
      }
      g_overrideColor = 0;
    }
  }

  if (this->bodyState == OPT1_NONE) {
    this->render.isMotionFreezed = false;
  }

  if ((this->effectState & 0x83) == 0 && this->bodyState != OPT1_FREEZE && this->bodyState != OPT1_STONE && this->bodyState != OPT1_STONEWAIT &&
      static_cast<int>(this->GetInstant(3)) != 1 && static_cast<int>(this->GetInstant(12)) != 1 &&
      static_cast<int>(this->GetInstant(13)) != 1 && static_cast<int>(this->GetInstant(14)) != 1 &&
      static_cast<int>(this->GetInstant(15)) != 1) {
    this->render.colorLocked = 0;
    color[1] = 0xFF;
    color[2] = 0xFF;
    color[0] = 0xFF;
  }

  if (this->render.accountId == g_session.m_aid && g_statusRefreshSuppressed == 0) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->ReapplySkillEffects();
  }
}

// OFFSET: 0x005A1950
// FUNCTION: fn_45475
void CGameActor::SetBodyPaletteName() {
}

// OFFSET: 0x005A1940
// FUNCTION: fn_45476
void CGameActor::SetBodyPalette(int) {
}

// OFFSET: 0x00CCA380
// FUNCTION: fn_21196
void CGameActor::ProcessSkillMessage(CGameObject*, const ActorSkillInfo* info, int, int) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int hitCount = info->hitCount;
  const int skillId = info->skillId;
  int type = info->type;
  const int cellX = info->cellX;
  const int cellY = info->cellY;
  const int attackSpeed = info->attackSpeed;
  const int motionSpeed = info->motionSpeed;
  if (hitCount <= 0 || hitCount > 36) {
    hitCount = 1;
  }

  const int level = skillId != KN_PIERCE ? info->level : hitCount;
  const int damage = info->damage / hitCount;
  this->hitTargetAid = info->targetAid;

  CRenderObject* target;
  bool known = false;
  if (g_session.m_aid == info->targetAid) {
    target = static_cast<CRenderObject*>(mode->scene->player);
    known = true;
  } else {
    target = mode->scene->GetGameActorByAID(info->targetAid);
    if (target != nullptr) {
      known = true;
    } else {
      target = mode->scene->GetSkillByAID(this->hitTargetAid);
    }
  }

  vector3d targetPos;
  if (target != nullptr) {
    if (known) {
      this->SendValueMessage(reinterpret_cast<int>(&target->render.position), this->GetEffectCounter(skillId));
    }

    targetPos = target->render.position;
    static_cast<CGameActor*>(target)->GetEffectiveJob(-1);
  } else {
    const CRenderObject* player = static_cast<const CRenderObject*>(mode->scene->player);
    targetPos.x = this->render.position.x - (player->render.position.x - this->render.position.x);
    targetPos.y = this->render.position.y - (player->render.position.y - this->render.position.y);
    targetPos.z = this->render.position.z - (player->render.position.z - this->render.position.z);
  }

  if (type != DMG_MULTI_HIT && type != DMG_MULTI_HIT_ENDURE && type != DMG_MULTI_HIT_CRITICAL) {
    hitCount = 1;
  }

  if (skillId == SC_TRIANGLESHOT || skillId == ABC_CHAIN_REACTION_SHOT || skillId == ABC_FRENZY_SHOT || CSkillActionMgr::GetInstance()->SuppressAttackSkillArrow(skillId)) {
    this->SetArrowEffectSuppressed(true);
  }

  switch (type) {
  case DMG_ENDURE:
  case DMG_SPLASH:
  case DMG_SINGLE:
  case DMG_MULTI_HIT:
  case DMG_MULTI_HIT_ENDURE:
  case DMG_MULTI_HIT_CRITICAL:
  case DMG_SPLASH_ENDURE:
    break;
  default:
    return;
  }

  ActorDamageMessage message;
  const int address = reinterpret_cast<int>(&message);
  if (type == DMG_SPLASH) {
    if (skillId == SC_STARTMARK || skillId == NPC_FATALMENACE) {
      this->SetAnimationSpeed(static_cast<float>(attackSpeed) / 432.0f * 4.0f);
    }

    g_session.PlaySkillEffect(skillId, hitCount, this, static_cast<CGameActor*>(target));
    if (target != nullptr) {
      if (!this->IsSkillWithoutAction(skillId)) {
        const float speed = static_cast<float>(motionSpeed) / 288.0f;
        const unsigned delay = static_cast<unsigned>(this->attackMotion * this->render.speed * 24.0f);
        int effect = -1;
        int followEffect = -1;
        switch (skillId) {
        case AG_DESTRUCTIVE_HURRICANE:
        case AG_CRYSTAL_IMPACT:
        case AG_CRYSTAL_IMPACT_ATK:
        case AG_STORM_CANNON:
        case AG_CRIMSON_ARROW:
        case AG_CRIMSON_ARROW_ATK:
        case AG_FROZEN_SLASH:
        case IQ_OLEUM_SANCTUM:
        case IQ_EXPOSION_BLASTER:
        case IQ_FIRST_BRAND:
        case SHC_FATAL_SHADOW_CROW:
        case MT_AXE_STOMP:
        case MT_RUSH_QUAKE:
        case MT_A_MACHINE:
        case ABC_ABYSS_DAGGER:
        case ABC_ABYSS_STRIKE:
        case WH_HAWKBOOMERANG:
        case ABC_FROM_THE_ABYSS_ATK:
        case NPC_BO_THROWROCK:
        case NPC_BO_WOODEN_ATTACK:
          effect = g_session.FindSkillEffectId(skillId);
          followEffect = g_session.FindSkillFollowEffect(skillId);
          break;
        }

        message.targetAid = this->hitTargetAid;
        message.action = 0x60;
        message.damage = damage;
        message.time = timeGetTime() + delay;
        message.attackerAid = this->render.accountId;
        message.cellX = cellX;
        message.cellY = cellY;
        message.hitEffect = effect;
        message.effectId = followEffect;
        message.monsterValue = level;
        message.motionSpeed = speed;
        message.blueDamage = 0;
        message.skillId = skillId;
        message.skillTarget = 0;
        target->SendMsg(this, AM_WILL_BE_ATTACKED, address, 0, 0, 0, 0, 0);
        return;
      }
    } else if (skillId == SO_CLOUD_KILL || skillId == NPC_CLOUD_KILL || skillId == SU_SV_ROOTTWIST_ATK || skillId == GN_CARTCANNON) {
      return;
    }
  } else if (type == DMG_SPLASH_ENDURE) {
    g_session.PlaySkillEffect(skillId, hitCount, this, static_cast<CGameActor*>(target));
    if (target != nullptr) {
      if (!this->IsSkillWithoutAction(skillId)) {
        const unsigned delay = static_cast<unsigned>(this->attackMotion * this->render.speed * 24.0f);
        message.targetAid = this->hitTargetAid;
        message.action = 0x61;
        message.damage = damage;
        message.time = timeGetTime() + delay;
        message.attackerAid = this->render.accountId;
        message.cellX = cellX;
        message.cellY = cellY;
        message.hitEffect = -1;
        message.effectId = -1;
        message.monsterValue = level;
        message.motionSpeed = static_cast<float>(motionSpeed) / 288.0f;
        message.blueDamage = 0;
        message.skillId = skillId;
        message.skillTarget = 0;
        target->SendMsg(this, AM_WILL_BE_ATTACKED, address, 0, 0, 0, 0, 0);
        return;
      }
    } else if (skillId == SO_CLOUD_KILL || skillId == NPC_CLOUD_KILL) {
      return;
    }
  }

  int castEffect;
  int motion;
  g_session.GetSkillActionInfo2(skillId, castEffect, motion);
  if (skillId != NC_BOOSTKNUCKLE && skillId != NC_PILEBUNKER && skillId != CR_ACIDDEMONSTRATION && skillId != KO_HUUMARANKA && !CSkillActionMgr::GetInstance()->KeepSkillAttackSpeed(skillId)) {
    this->SetModifyFactorOfmotionSpeed(attackSpeed);
  }

  bool keepMotion;
  switch (skillId) {
  case NV_TRICKDEAD:
  case MO_TRIPLEATTACK:
  case MO_EXTREMITYFIST:
  case MO_CHAINCOMBO:
  case MO_COMBOFINISH:
  case CH_TIGERFIST:
  case LG_OVERBRAND:
  case SR_DRAGONCOMBO:
  case SR_EARTHSHAKER:
  case SR_FALLENEMPIRE:
  case LG_OVERBRAND_BRANDISH:
  case SJ_NEWMOONKICK:
  case ABC_CHAIN_REACTION_SHOT_ATK:
    keepMotion = true;
    break;
  case NC_ARMSCANNON:
    keepMotion = type == DMG_MULTI_HIT;
    break;
  default:
    keepMotion = CSkillActionMgr::GetInstance()->KeepSkillAttackMotion(skillId, type);
    break;
  }
  if (!keepMotion) {
    this->SetState(motion, 1);
  }

  if (ShowsCastEffect(skillId) && level != -2) {
    CRagEffect* effect = this->LaunchEffect(castEffect, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(nullptr, AM_SETCOUNT, 0, hitCount, 0, 0, 0, 0);
    }
  }

  if (type != DMG_SPLASH) {
    g_session.PlaySkillEffect(skillId, hitCount, this, static_cast<CGameActor*>(target));
  }

  if (target != nullptr && target != this && !CSkillActionMgr::GetInstance()->KeepSkillAttackMotion(skillId, type) && this->render.state != 9 &&
      this->render.state != 2) {
    this->Face(targetPos.x, targetPos.z);
  }

  const vector3d player = static_cast<const CRenderObject*>(mode->scene->player)->render.position;
  switch (skillId) {
  case ABR_BATTLE_BUSTER:
    g_soundMgr->PlayWave(
      "effect\\MT_SUMMON_ABR_BATTLE_WARIOR.wav", this->render.position.x - player.x, this->render.position.y, this->render.position.z - player.z, 250, 40,
      1.0f, 0
    );
    break;
  case ABR_DUAL_CANNON_FIRE:
    g_soundMgr->PlayWave(
      "effect\\MT_SUMMON_ABR_DUAL_CANNON.wav", this->render.position.x - player.x, this->render.position.y, this->render.position.z - player.z, 250, 40,
      1.0f, 0
    );
    break;
  case ABR_INFINITY_BUSTER:
    g_soundMgr->PlayWave(
      "effect\\MT_SUMMON_ABR_INFINITY.wav", this->render.position.x - player.x, this->render.position.y, this->render.position.z - player.z, 250, 40, 1.0f,
      0
    );
    break;
  }

  unsigned delay = static_cast<unsigned>(this->attackMotion * this->render.speed * 24.0f);
  if (this->GetEffectiveJob(-1) == 0x3F8 || this->GetEffectiveJob(-1) == 0x58C) {
    delay += 0xC0;
  }

  if (target == nullptr) {
    return;
  }

  const int hitEffect = g_session.FindSkillEffectId(skillId);
  int followEffect = g_session.FindSkillFollowEffect(skillId);
  if (followEffect == 186) {
    switch (level) {
    case 2:
      followEffect = 187;
      break;
    case 3:
      followEffect = 188;
      break;
    case 4:
      followEffect = 189;
      break;
    case 5:
      followEffect = 190;
      break;
    }
  }

  CRagEffect* launched = this->LaunchEffect(hitEffect, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (launched != nullptr) {
    launched->SendMsg(nullptr, AM_SETCOUNT, 0, hitCount, 0, 0, 0, 0);
    launched->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&targetPos), 0, 0, 0, 0);
  }

  int effect = skillId < 493 && !(skillId >= 411 && skillId <= 472) ? 0 : -1;
  switch (skillId) {
  case SM_MAGNUM:
  case MG_FIREBALL:
  case MG_FIREBOLT:
  case MS_MAGNUM:
    effect = 0x31;
    break;
  case MG_COLDBOLT:
  case MG_FROSTDIVER:
    effect = 0x33;
    break;
  case MG_LIGHTNINGBOLT:
    effect = 0x34;
    break;
  case ALL_RESURRECTION:
    if (damage != 0) {
      effect = 0x98;
      followEffect = -1;
    }
    break;
  case KN_PIERCE:
  case KN_BRANDISHSPEAR:
  case KN_SPEARSTAB:
  case KN_SPEARBOOMERANG:
  case HT_BLITZBEAT:
  case NPC_WATERATTACK:
  case NPC_GROUNDATTACK:
  case NPC_FIREATTACK:
  case NPC_WINDATTACK:
  case NPC_POISONATTACK:
  case NPC_HOLYATTACK:
  case NPC_UNDEADATTACK:
  case LK_SPIRALPIERCE:
  case NPC_FIREBREATH:
    effect = -1;
    break;
  case WZ_EARTHSPIKE:
  case WZ_HEAVENDRIVE:
  case NPC_GROUNDDRIVE:
    effect = 0x93;
    break;
  case AS_SONICBLOW:
    effect = 0;
    break;
  case NPC_CRITICALSLASH:
    type = 10;
    break;
  case NPC_DARKNESSATTACK:
  case SP_CURSEEXPLOSION:
  case SP_SPA:
    effect = 0xB4;
    break;
  case SL_STIN:
  case SL_STUN:
    effect = 0x225;
    break;
  case GC_CROSSIMPACT:
    effect = 0x61F;
    break;
  case WL_SOULEXPANSION:
    effect = 0x637;
    followEffect = 0x638;
    break;
  case LG_CANNONSPEAR:
    followEffect = 0x583;
    break;
  case LG_OVERBRAND:
  case LG_OVERBRAND_BRANDISH:
    effect = 0x699;
    break;
  case LG_EARTHDRIVE:
    if (g_session.m_aid != this->hitTargetAid) {
      effect = 0x6AC;
    }
    break;
  case WM_REVERBERATION:
    effect = 0x639;
    break;
  case SO_EARTHGRAVE:
    effect = 0x657;
    break;
  case IQ_MASSIVE_F_BLASTER:
    effect = 0x82F;
    followEffect = 0x830;
    break;
  case CD_ARBITRIUM_ATK:
    effect = 0x722;
    break;
  case ABC_DEFT_STAB:
    effect = 0x854;
    break;
  case ABC_FRENZY_SHOT:
    effect = 0x843;
    break;
  }

  int action;
  switch (type) {
  case DMG_ENDURE:
  case DMG_MULTI_HIT_ENDURE:
    action = 0x27;
    break;
  case DMG_SINGLE:
    action = 0x61;
    break;
  case DMG_CRITICAL:
    action = 0x5D;
    break;
  case DMG_MULTI_HIT_CRITICAL:
    action = 0x94;
    break;
  default:
    action = 0x60;
    break;
  }

  message.targetAid = this->hitTargetAid;
  message.action = action;
  message.damage = damage;
  message.time = timeGetTime() + delay;
  message.attackerAid = this->render.accountId;
  message.cellX = cellX;
  message.cellY = cellY;
  message.hitEffect = effect;
  message.effectId = followEffect;
  message.monsterValue = level;
  message.motionSpeed = static_cast<float>(motionSpeed) / 288.0f;
  message.blueDamage = 0;
  message.skillId = skillId;
  message.skillTarget = 0;
  switch (skillId) {
  case MO_COMBOFINISH:
  case CH_TIGERFIST:
  case WL_TETRAVORTEX_FIRE:
  case SR_EARTHSHAKER:
  case SR_FALLENEMPIRE:
  case SR_TIGERCANNON:
    target->SendMsg(this, AM_WILL_BE_ATTACKED, address, damage, 0, 0, 0, 0);
    break;
  case WL_TETRAVORTEX_WATER:
    message.time += 500;
    target->SendMsg(this, AM_WILL_BE_ATTACKED, address, damage, 0, 0, 0, 0);
    break;
  case WL_TETRAVORTEX_WIND:
    message.time += 1000;
    target->SendMsg(this, AM_WILL_BE_ATTACKED, address, damage, 0, 0, 0, 0);
    break;
  case WL_TETRAVORTEX_GROUND:
    message.time += 1500;
    target->SendMsg(this, AM_WILL_BE_ATTACKED, address, damage, 0, 0, 0, 0);
    break;
  case TK_DOWNKICK:
  case TK_TURNKICK:
  case SJ_SOLARBURST:
  case SJ_PROMINENCEKICK:
    message.time += 300;
    target->SendMsg(this, AM_WILL_BE_ATTACKED2, address, damage, 0, 0, 0, 0);
    break;
  case TK_STORMKICK:
  case TK_COUNTER:
  case TK_JUMPKICK:
  case SR_RAMPAGEBLASTER:
  case SR_KNUCKLEARROW:
  case SR_CRESCENTELBOW_AUTOSPELL:
  case SJ_FULLMOONKICK:
  case SJ_FLASHKICK:
    target->SendMsg(this, AM_WILL_BE_ATTACKED2, address, damage, 0, 0, 0, 0);
    break;
  case PR_TURNUNDEAD:
    message.time += 500;
    [[fallthrough]];
  default:
    if (hitCount == 1) {
      target->SendMsg(this, AM_WILL_BE_ATTACKED, address, 0, 0, 0, 0, 0);
    } else {
      target->SendMsg(this, AM_WILL_BE_ATTACKED, address, damage, 0, 0, 0, 0);
    }
    break;
  }

  message.effectId = -1;
  switch (skillId) {
  case MO_TRIPLEATTACK:
    this->SetSequenceInterval(4);
    this->SetFrameSequence(0x58, 0, 7, 0x58, 7, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    break;
  case MO_INVESTIGATE:
  case MO_FINGEROFFENSIVE:
  case SR_RIDEINLIGHTNING:
    if (this->objectType != NPC_MOB_TYPE) {
      this->SetSequenceInterval(10);
      unsigned char animation;
      unsigned char firstFrame;
      unsigned char lastFrame;
      if (g_session.IsDoramJob(this->GetEffectiveJob(-1)) == true) {
        animation = 0x58;
        firstFrame = 4;
        lastFrame = 5;
      } else {
        animation = 0x60;
        firstFrame = 3;
        lastFrame = 4;
      }
      this->SetFrameSequence(animation, firstFrame, lastFrame, animation, lastFrame, 1, 1100, 0);
    }
    break;
  case MO_EXTREMITYFIST:
    if (this->objectType != NPC_MOB_TYPE) {
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x58, 0, 7, 0x58, 7, 1, 3500, 0);
      this->isAsuraAttack = 1;
    }
    break;
  case MO_CHAINCOMBO:
    this->SetFrameSequence(0x60, 5, 5, 0x60, 5, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    static_cast<EffectActor*>(target)->LaunchEffect(EF_TEIHIT1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    this->LaunchEffect(EF_GUMGANG3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(target)->LaunchEffect(EF_CHAINCOMBO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case MO_COMBOFINISH:
    this->SetSequenceInterval(6);
    this->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    static_cast<CMode*>(g_modeMgr.GetGameMode())->SendMsg(MM_QUAKE, 0, 0, 0, 0);
    g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    this->LaunchEffect(EF_GUMGANG3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    this->LaunchEffect(EF_HITLINE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case CH_PALMSTRIKE:
    this->SetFrameSequence(0x60, 3, 4, 0x60, 4, 1, 1, 0);
    static_cast<CMode*>(g_modeMgr.GetGameMode())->SendMsg(MM_QUAKE, 0, 0, 0, 0);
    g_soundMgr->PlayWave("effect\\맹호경파산.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    static_cast<EffectActor*>(target)->LaunchEffect(EF_HITLINE2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case CH_TIGERFIST:
    this->SetSequenceInterval(6);
    this->SetFrameSequence(0x60, 4, 4, 0x60, 4, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    static_cast<CMode*>(g_modeMgr.GetGameMode())->SendMsg(MM_QUAKE, 0, 0, 0, 0);
    this->LaunchEffect(EF_BASH3D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    this->LaunchEffect(EF_GUMGANG3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case CH_CHAINCRUSH: {
    this->LaunchEffect(EF_GUMGANG3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    CRagEffect* follow = this->LaunchEffect(EF_CHEMICAL2DASH, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (follow != nullptr) {
      vector3d position = target->render.position;
      follow->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  }
  case SR_DRAGONCOMBO:
    this->SetSequenceInterval(6);
    this->SetFrameSequence(0x60, 4, 4, 0x60, 4, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    break;
  case SR_SKYNETBLOW:
  case SR_TIGERCANNON:
    this->SetSequenceInterval(6);
    this->SetFrameSequence(0x60, 5, 5, 0x60, 5, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    break;
  case SR_EARTHSHAKER:
    this->SetSequenceInterval(6);
    this->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    if (!this->HasId(EF_ELECTRIC4)) {
      static_cast<CMode*>(g_modeMgr.GetGameMode())->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\SR_EARTHSHAKER.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      this->LaunchEffect(EF_GUMGANG5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_ELECTRIC4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SR_FALLENEMPIRE:
    this->SetSequenceInterval(6);
    this->SetFrameSequence(0x60, 3, 3, 0x60, 3, 1, g_session.FindActorSkillAnimationValue(this->render.accountId), 0);
    static_cast<CMode*>(g_modeMgr.GetGameMode())->SendMsg(MM_QUAKE, 0, 0, 0, 0);
    g_soundMgr->PlayWave("effect\\SR_FALLENEMPIRE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    g_effectMgr->Spawn(target->render.accountId, EF_NEW_FALLENEMPIRE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, EF_NEW_FALLENEMPIRE_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case SR_RAMPAGEBLASTER:
    if (target != this) {
      static_cast<EffectActor*>(target)->LaunchEffect(EF_BLUE_HIT, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      static_cast<EffectActor*>(target)->LaunchEffect(EF_QUAKEBODY3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      CRagEffect* follow = this->LaunchEffect(EF_FIREBALL4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (follow != nullptr) {
        vector3d position = target->render.position;
        follow->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case RL_QD_SHOT:
    static_cast<EffectActor*>(target)->LaunchEffect(EF_TEIHIT1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(target)->LaunchEffect(EF_CHAINCOMBO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case RL_FALLEN_ANGEL:
    this->SetSequenceInterval(4);
    this->SetFrameSequence(0x58, 0, 7, 0x58, 7, 1, 3500, 0);
    break;
  case AG_DEADLY_PROJECTION:
  case AG_SOUL_VC_STRIKE:
  case AG_ASTRAL_STRIKE:
  case TR_RETROSPECTION:
  case TR_SOUNDBLEND:
    this->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
    break;
  case IQ_THIRD_PUNISH:
  case IQ_THIRD_FLAME_BOMB:
  case IQ_THIRD_CONSECRATION:
    this->SetSequenceInterval(4);
    this->SetFrameSequence(0x60, 2, 2, 0x60, 2, 1, 500, 0);
    break;
  }

  if (type != DMG_SINGLE && type != DMG_MULTI_HIT && type != DMG_MULTI_HIT_ENDURE && type != DMG_MULTI_HIT_CRITICAL) {
    return;
  }

  int total = damage;
  for (int hit = 0; hit < hitCount - 1;) {
    const int previous = hit;
    ++hit;
    message.effectId = -1;
    message.skillTarget = hit;
    if (skillId == KN_PIERCE) {
      message.time += g_multiHitInterval + 80;
      total += damage;
      message.action = action;
      message.damage = damage;
      target->SendMsg(this, AM_WILL_BE_ATTACKED, address, total, 0, 0, 0, 0);
      continue;
    }

    message.time += g_multiHitInterval;
    total += damage;
    message.damage = damage;
    if (skillId == GC_CROSSIMPACT) {
    } else if (skillId == MO_CHAINCOMBO) {
      message.effectId = 122;
    } else if (skillId == CH_CHAINCRUSH) {
      message.action = 0x6C;
      this->SendMsg(this, AM_WILL_BE_ATTACKED, address, 0, 0, 0, 0, 0);
    } else if (skillId == CG_ARROWVULCAN) {
    } else if (skillId == WL_CRIMSONROCK) {
      message.effectId = 0x512;
      if (g_serviceOptions.Get(229)) {
        message.effectId = -1;
      }
    } else if (skillId == WL_COMET) {
      if (previous & 1) {
        message.hitEffect = -1;
      } else if (g_serviceOptions.Get(229)) {
        message.hitEffect = 0x601;
        message.effectId = 0x602;
      } else {
        message.hitEffect = 0x588;
        message.effectId = 0x589;
      }
    } else if (skillId == LG_RAYOFGENESIS || skillId == NPC_RAYOFGENESIS) {
      message.effectId = 0x593;
    }

    switch (skillId) {
    case AG_SOUL_VC_STRIKE:
    case IQ_SECOND_FAITH:
    case IQ_SECOND_JUDGEMENT:
    case CD_EFFLIGO:
    case SHC_SAVAGE_IMPACT:
    case SHC_ETERNAL_SLASH:
    case BO_ACIDIFIED_ZONE_WATER:
    case BO_ACIDIFIED_ZONE_GROUND:
    case BO_ACIDIFIED_ZONE_WIND:
    case BO_ACIDIFIED_ZONE_FIRE:
    case EM_DIAMOND_STORM:
    case EM_TERRA_DRIVE:
    case ABC_CHAIN_REACTION_SHOT_ATK:
    case EM_ELEMENTAL_BUSTER_FIRE:
    case EM_ELEMENTAL_BUSTER_WATER:
    case EM_ELEMENTAL_BUSTER_WIND:
    case EM_ELEMENTAL_BUSTER_GROUND:
    case EM_ELEMENTAL_BUSTER_POISON:
    case MH_TWISTER_CUTTER:
    case MH_ABSOLUTE_ZEPHYR:
    case MH_BLAZING_AND_FURIOUS:
      message.effectId = g_session.FindSkillFollowEffect(skillId);
      break;
    case ABC_DEFT_STAB:
      message.hitEffect = -1;
      message.effectId = g_session.FindSkillFollowEffect(skillId);
      break;
    }

    message.action = action;
    int kind = 0x1B;
    switch (skillId) {
    case TK_STORMKICK:
    case TK_DOWNKICK:
    case TK_TURNKICK:
    case TK_COUNTER:
    case TK_JUMPKICK:
    case SR_RAMPAGEBLASTER:
    case SR_KNUCKLEARROW:
    case SR_CRESCENTELBOW_AUTOSPELL:
    case SJ_FULLMOONKICK:
    case SJ_FLASHKICK:
    case SJ_SOLARBURST:
    case SJ_PROMINENCEKICK:
      kind = 0x85;
      break;
    }

    target->SendMsg(this, kind, address, total, 0, 0, 0, 0);
  }
}
