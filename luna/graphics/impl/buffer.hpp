#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class VertexBuffer {
    public:
      VertexBuffer() {this->m_handle = -1; this->m_size = 0;}
      VertexBuffer(int gpu, std::size_t size) {
        this->m_handle = impl().buffer.make_vertex(gpu, size);
        this->m_size = size;
      };

      ~VertexBuffer() {
        if(this->m_handle >= 0) {
          impl().buffer.destroy(this->m_handle);
        }
      }
      VertexBuffer(VertexBuffer&& mv) = default;
      VertexBuffer(const VertexBuffer& cpy) = delete;

      inline auto handle() -> std::int32_t {return this->m_handle;}
      
      auto size() {return this->m_size;}
    private:
      std::int32_t m_handle;
      std::size_t m_size;
  };
}
}