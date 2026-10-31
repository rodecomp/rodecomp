#include "Resource/RmaResource.h"
#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Utility/Tokenizer.h"

#include <cstdlib>
#include <strstream>

// OFFSET: 0x007461C0
// FUNCTION: fn_07770
CRmaRes::CRmaRes() = default;

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

  const char rsxNameTag[] = {'#', 'r', 's', 'x', 'n', 'a', 'm', 'e', '\0'};
  const char actionTag[] = {'#', 'a', 'c', 't', 'i', 'o', 'n', '\0'};
  std::istrstream stream(buffer);
  int model = -1;
  while (stream) {
    tokenizer.Parse(stream);
    if (tokenizer.tokens.size() == 0 || tokenizer[0].size() == 0) {
      continue;
    }

    if (tokenizer[0] == rsxNameTag) {
      ++model;
      this->models.resize(model + 1);
      this->models[model] = tokenizer[1];
    } else if (tokenizer[0] == actionTag) {
      const unsigned int action = std::atoi(tokenizer[1].c_str());
      if (action >= this->actions.size()) {
        this->actions.resize(action + 1);
        this->actionModels.resize(action + 1);
      }

      if (this->actions[action].startFrame != -1) {
        ShowErrorDialog(GetRenderSystemHolder()->system, "중복되는 액션이 있음", 0);
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
      ShowErrorDialog(GetRenderSystemHolder()->system, "Syntax Error in Rma", 0);
    }
  }

  delete[] buffer;
  return true;
}

// OFFSET: 0x00745190
// FUNCTION: fn_07755
void CRmaRes::Unload() {
  this->actions.clear();
  this->models.clear();
  this->actionModels.clear();
}
