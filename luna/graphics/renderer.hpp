#pragma once 
#include <memory>
#include <string>

namespace luna {
namespace gfx {
class Window;
class Renderable;
class RenderState;
class Renderer {
  public:
    Renderer();
    Renderer(const Window& window);
    ~Renderer();
    auto draw(const Renderable& obj) -> void;
    auto change_mode(const RenderState& state) -> void;
    auto submit();
  private:
    struct RendererData;
    std::unique_ptr<RendererData> m_data;
};
}
}