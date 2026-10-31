#include "Resource/ImfResource.h"
#include "File/File.h"

// INLINED
CImfRes::CImfRes() {
  for (int priority = 0; priority < 15; ++priority) {
    this->m_maxAction[priority] = 0;
    for (int action = 0; action < 104; ++action) {
      this->m_maxMotion[priority][action] = 0;
    }
  }
}

// OFFSET: 0x0074F0A0
// FUNCTION: fn_07924
CImfRes* CImfRes::Create() {
  return new CImfRes;
}

// OFFSET: 0x0074F190
// FUNCTION: fn_07925
bool CImfRes::Load(const char* name) {
  CFile file;
  float version;

  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  file.Read(&version, 4);
  if (!(1.01f <= version)) {
    return false;
  }

  int checksum;
  file.Read(&checksum, 4);

  int maxLayer;
  file.Read(&maxLayer, 4);

  for (int layer = 0; layer <= maxLayer; ++layer) {
    std::vector<ImfData> actions;

    int numAction;
    file.Read(&numAction, 4);
    this->m_maxAction[layer] = numAction;

    for (int action = 0; action < numAction; ++action) {
      ImfData data;

      int numMotion;
      file.Read(&numMotion, 4);
      this->m_maxMotion[layer][action] = numMotion;

      for (int motion = 0; motion < numMotion; ++motion) {
        int value;
        file.Read(&value, 4);
        data.priority.push_back(value);
        file.Read(&value, 4);
        data.cx.push_back(value);
        file.Read(&value, 4);
        data.cy.push_back(value);
      }

      actions.push_back(data);
    }

    this->m_imfData.push_back(actions);
  }

  file.Close();
  return true;
}
