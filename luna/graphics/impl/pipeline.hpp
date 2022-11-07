#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/types.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class Pipeline {
    public:
      Pipeline() {this->m_handle = -1;}
      Pipeline(PipelineInfo info) {
        this->m_handle = impl().pipeline.make(info);
        this->m_info = info;
      };

      ~Pipeline() {
        if(this->m_handle >= 0) {
          impl().pipeline.destroy(this->m_handle);
        }
      }
      Pipeline(Pipeline&& mv) = default;
      Pipeline(const Pipeline& cpy) = delete;

      inline auto handle() -> std::int32_t {return this->m_handle;}
      
      auto operator=(Pipeline&& mv) -> Pipeline& = default;
      auto operator=(const Pipeline& cpy) -> Pipeline& = delete;
    private:
      std::int32_t m_handle;
      PipelineInfo m_info;
  };
}
}