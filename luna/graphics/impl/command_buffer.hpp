#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/impl/descriptor.hpp"
#include "luna/graphics/types.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class CommandBuffer {
    public:
      CommandBuffer() {this->m_handle = -1;}
      CommandBuffer(int gpu, int32_t parent = -1) {
        this->m_handle = impl().cmd.make(gpu, parent);
      };

      ~CommandBuffer() {
        if(this->m_handle >= 0) {
          impl().cmd.destroy(this->m_handle);
        }
      }

      CommandBuffer(CommandBuffer&& mv) = default;
      CommandBuffer(const CommandBuffer& cpy) = delete;

      auto set_descriptor(const Descriptor& desc) -> void {
        impl().cmd.bind_descriptor(this->m_handle, desc.handle());
      }

      auto operator=(CommandBuffer&& mv) -> CommandBuffer& = default;
      auto operator=(const CommandBuffer& cpy) -> CommandBuffer& = delete;
    private:
      std::int32_t m_handle;
  };
}
}