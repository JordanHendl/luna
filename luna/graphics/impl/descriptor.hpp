#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/impl/pipeline.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class Descriptor {
    public:
      Descriptor() {this->m_handle = -1;}
      Descriptor(Pipeline pipeline) {
        this->m_handle = impl().descriptor.make(pipeline.handle());
      };

      ~Descriptor() {
        if(this->m_handle >= 0) {
          impl().render_pass.destroy(this->m_handle);
        }
      }
      Descriptor(Descriptor&& mv) = default;
      Descriptor(const Descriptor& cpy) = delete;

      inline auto handle() const -> std::int32_t {return this->m_handle;}

      auto operator=(Descriptor&& mv) -> Descriptor& = default;
      auto operator=(const Descriptor& cpy) -> Descriptor& = delete;
    private:
      std::int32_t m_handle;
  };
}
}