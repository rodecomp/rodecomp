#include "Render/Primitives/RenderPrimitive.h"

#include <deque>
#include <new>

class RenderCommandPool {
  friend class CRenderer;

public:
  /* 0000 */ std::deque<RenderCommand> m_commands;
  /* 0014 */ std::deque<RenderCommand>::iterator m_current;
};

class CRenderer {
public:
  /* 0000 */ unsigned char padding[0x50];
  /* 0050 */ RenderCommandPool commands;

  RenderCommand* AcquireCommand();
};

// OFFSET: 0x00CA63D0
// FUNCTION: fn_20958
RenderCommand* CRenderer::AcquireCommand() {
  std::deque<RenderCommand>& commands = this->commands.m_commands;
  if (commands.end() == this->commands.m_current) {
    commands.emplace_back();
    this->commands.m_current = commands.end();
    return &commands.back();
  }

  RenderCommand* command = &*this->commands.m_current;
  ++this->commands.m_current;
  new (command) RenderCommand;
  return command;
}
