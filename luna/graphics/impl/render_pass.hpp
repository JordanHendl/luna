#pragma once
#include "luna/graphics/impl/backend.hpp"
#include "luna/graphics/types.hpp"
#include "cstddef"
namespace luna {
namespace gfx {
  class RenderPass {
    public:
      RenderPass() {this->m_handle = -1;}
      RenderPass(RenderPassInfo info, int32_t swapchain = -1) {
        this->m_handle = impl().render_pass.make(info, swapchain);
        this->m_info = info;
      };

      ~RenderPass() {
        if(this->m_handle >= 0) {
          impl().render_pass.destroy(this->m_handle);
        }
      }
      RenderPass(RenderPass&& mv) = default;
      RenderPass(const RenderPass& cpy) = delete;

      auto operator=(RenderPass&& mv) -> RenderPass& = default;
      auto operator=(const RenderPass& cpy) -> RenderPass& = delete;
    private:
      std::int32_t m_handle;
      RenderPassInfo m_info;
  };
}
}