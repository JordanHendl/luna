#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/types.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class Window {
    public:
      Window() {this->m_handle = -1;}
      Window(const WindowInfo& info) {
        this->m_handle = impl().window.make(info);
        this->m_info = info;
      };

      ~Window() {
        if(this->m_handle >= 0) {
          impl().window.destroy(this->m_handle);
        }
      }
      Window(Window&& mv) {*this = std::move(mv);};
      Window(const Window& cpy) = delete;

      inline auto handle() const -> std::int32_t {return this->m_handle;}
      
      auto operator=(Window&& mv) -> Window& {this->m_handle = mv.m_handle; mv.m_handle = -1; this->m_info = mv.m_info; return *this;};
      auto operator=(const Window& cpy) -> Window& = delete;
    private:
      std::int32_t m_handle;
      WindowInfo m_info;
  };
}
}