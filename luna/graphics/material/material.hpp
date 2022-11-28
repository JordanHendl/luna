#pragma once
#include "luna/graphics/impl/impl.hpp"
#include <string_view>
namespace luna {
namespace gfx {
class RenderPass;
class Material {
public:
  Material();
  Material(const RenderPass& rp, std::string_view vertex, std::string_view fragment);
  ~Material();
  auto pipeline() const -> const gfx::Pipeline& {return this->m_pipeline;};
private:
  gfx::Pipeline m_pipeline;
};
}
}