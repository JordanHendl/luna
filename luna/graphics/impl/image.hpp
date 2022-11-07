#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/types.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class Image {
    public:
      Image() {this->m_handle = -1;}
      Image(ImageInfo info, const unsigned char* initial_data = nullptr) {
        this->m_handle = impl().image.make(info, initial_data);
        this->m_info = info;
      };

      ~Image() {
        if(this->m_handle >= 0) {
          impl().image.destroy(this->m_handle);
        }
      }
      Image(Image&& mv) = default;
      Image(const Image& cpy) = delete;

      auto operator=(Image&& mv) -> Image& = default;
      auto operator=(const Image& cpy) -> Image& = delete;
      inline auto handle() -> std::int32_t {return this->m_handle;}
      auto info() {return this->m_info;}
    private:
      std::int32_t m_handle;
      ImageInfo m_info;
  };
}
}