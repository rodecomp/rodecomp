#include "Resource/Models/RmaResource.h"
#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Utility/Text/Tokenizer.h"
#include "Resource/ResourceMgr.h"

#include <cstdlib>
#include <strstream>
#include <cstring>

struct RmaNameTag {
  std::uint32_t first;
  std::uint32_t second;
};

struct RmaActionTag {
  std::uint32_t first;
  std::uint16_t second;
  std::uint8_t third;
};

// OFFSET: 0x007461C0
// FUNCTION: fn_07770
CRmaRes::CRmaRes() {
  this->shadowZoom = 1.0f;
}

// OFFSET: 0x007467D0
// FUNCTION: fn_07778
CRmaRes::~CRmaRes() {
  this->Unload();
}

// OFFSET: 0x007446F0
// FUNCTION: fn_07753
CRmaRes* CRmaRes::Create() {
  return new CRmaRes;
}

// OFFSET: 0x00744820
// FUNCTION: fn_07754
bool CRmaRes::Load(const char* name) {
  CTokenizer tokenizer;
  CFile file;
  file.Open(name, 0, true, false);
  const std::uint32_t size = file.GetSize();
  char* buffer = new char[size + 1];
  file.Read(buffer, size);
  file.Close();
  buffer[size] = '\0';

  std::istrstream stream(buffer);
  int model = -1;
  while (stream) {
    tokenizer.Parse(stream);
    if (tokenizer.tokens.size() == 0 || tokenizer[0].size() == 0) {
      continue;
    }

    if (const auto& token = tokenizer[0];
        [&] {
          const auto size = token.size();
          const auto* tag = reinterpret_cast<const RmaNameTag*>(token.data());
          return size == 8 && tag->first == 0x78737223u && tag->second == 0x656d616eu;
        }()) {
      ++model;
      this->models.resize(model + 1);
      this->models[model] = tokenizer[1];
    } else if (const auto& actionToken = tokenizer[0];
               [](const std::string& token) {
                 const auto size = token.size();
                 const auto* tag = reinterpret_cast<const RmaActionTag*>(token.data());
                 return size == 7 && tag->first == 0x74636123u && tag->second == 0x6f69 && tag->third == 0x6e;
               }(actionToken)) {
      const unsigned int action = std::atoi(tokenizer[1].c_str());
      if (action >= this->actions.size()) {
        this->actions.resize(action + 1);
        this->actionModels.resize(action + 1);
      }

      if (this->actions[action].startFrame != -1) {
        ShowErrorDialog("중복되는 액션이 있음", 0);
      }

      this->actions[action].delay = std::atoi(tokenizer[2].c_str());
      this->actions[action].startFrame = std::atoi(tokenizer[3].c_str());
      this->actions[action].endFrame = std::atoi(tokenizer[4].c_str());
      if (static_cast<int>(tokenizer.tokens.size()) >= 6) {
        this->actions[action].frames.resize(static_cast<int>(tokenizer.tokens.size()) - 5);
        for (int frame = 0; frame < static_cast<int>(tokenizer.tokens.size()) - 5; ++frame) {
          this->actions[action].frames[frame] = std::atoi(tokenizer[frame + 5].c_str());
        }
      }

      this->actionModels[action] = model;
    } else if (tokenizer[0] == "#interiorzoom") {
      this->interiorZoom = static_cast<float>(std::atof(tokenizer[1].c_str()));
    } else if (tokenizer[0] == "#interioroffsetx") {
      this->interiorOffsetX = std::atoi(tokenizer[1].c_str());
    } else if (tokenizer[0] == "#interioroffsety") {
      this->interiorOffsetY = std::atoi(tokenizer[1].c_str());
    } else if (tokenizer[0] == "#interiorframes") {
      this->interiorFrameStart = std::atoi(tokenizer[1].c_str());
      this->interiorFrameEnd = std::atoi(tokenizer[2].c_str());
    } else if (tokenizer[0] == "#shadowzoom") {
      this->shadowZoom = static_cast<float>(std::atof(tokenizer[1].c_str()));
    } else {
      ShowErrorDialog("Syntax Error in Rma", 0);
    }
  }

  delete[] buffer;
  return true;
}

// OFFSET: 0x00745190
// FUNCTION: fn_07755
void CRmaRes::Unload() {
  this->actions.erase(this->actions.begin(), this->actions.end());
  this->models.erase(this->models.begin(), this->models.end());
  this->actionModels.erase(this->actionModels.begin(), this->actionModels.end());
}

// OFFSET: 0x007470C0
// FUNCTION: fn_45821
int CRmaRes::EndFrame(int action) {
  return this->actions[action].endFrame;
}

// OFFSET: 0x00747130
// FUNCTION: fn_45822
int CRmaRes::GetFirstFrame(int action) {
  const auto& frames = this->actions[action].frames;
  if (static_cast<int>(frames.size()) > 0) {
    return frames.front();
  }

  return -1;
}

// OFFSET: 0x007473A0
// FUNCTION: fn_45823
int CRmaRes::GetDelay(int action) {
  return this->actions[action].delay;
}

// OFFSET: 0x007473C0
// FUNCTION: fn_45824
std::string* CRmaRes::GetModel(int action) {
  return &this->models[this->actionModels[action]];
}

// OFFSET: 0x007473E0
// FUNCTION: fn_45825
int CRmaRes::GetStartFrame(int action) {
  return this->actions[action].startFrame;
}

// OFFSET: 0x00747400
// FUNCTION: fn_45826
int CRmaRes::HasFrame(int action, int frame) {
  const auto& frames = this->actions[action].frames;
  const int count = static_cast<int>(frames.size());
  for (int index = 0; index < count; ++index) {
    if (frame == frames[index]) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00747450
// FUNCTION: fn_45827
int CRmaRes::IsValidAction(int action) {
  if (action < this->actions.size() && action >= 0 && this->actions[action].startFrame != -1) {
    return 1;
  }

  return 0;
}

// INLINED
CRmaRes::CRmaRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25547
CRmaRes g_rmaPrototype("rma", "model\\");
