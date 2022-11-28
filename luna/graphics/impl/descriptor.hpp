#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/impl/pipeline.hpp"
#include <memory>
#include <cstddef>
namespace luna {
namespace gfx {
  class Descriptor {
    public:
      Descriptor() {this->m_handle = -1;}
      Descriptor(const Pipeline& pipeline) {
        this->m_handle = impl().descriptor.make(pipeline.handle());
      };

      ~Descriptor() {
        if(this->m_handle >= 0) {
          impl().descriptor.destroy(this->m_handle);
        }
      }
      Descriptor(Descriptor&& mv) {*this = std::move(mv);};
      Descriptor(const Descriptor& cpy) = delete;

      inline auto handle() const -> std::int32_t {return this->m_handle;}

      auto operator=(Descriptor&& mv) -> Descriptor& {this->m_handle = mv.m_handle; mv.m_handle = -1; return *this;};
      auto operator=(const Descriptor& cpy) -> Descriptor& = delete;
    private:
      std::int32_t m_handle;
  };
}
}