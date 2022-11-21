#include "luna/graphics/renderable.hpp"
#include <glm/glm.hpp>
#include <vector>
namespace luna {
namespace gfx {
struct TextData {
  glm::vec4 color;
};

struct Image2DData {
  glm::vec4 color;
};

struct RenderableResources {
  std::vector<TextData> text;
  std::vector<Image2DData> image2d;
};

static auto renderable_resources() -> RenderableResources& {
  static auto r = RenderableResources();
  return r;
}


auto Renderable::set_per_frame_info(const Scene* scene, const glm::vec3& pos) -> void {
  
}

Renderable::~Renderable() {

}

Text::Text(const glm::vec3* position, std::string_view font_name) {

}

Text::~Text() {

}

auto Text::set(std::string_view text) -> void {

}

auto Text::append(std::string_view text) -> void {

}

auto Text::clear() -> void {

}

auto Text::set_color(const glm::vec4& rgba) -> void {

}

auto Text::color() -> const glm::vec4& {
  return renderable_resources().text[this->m_idx].color;
}

auto Text::toggle_outline() -> bool {
  return true;
}

auto Text::render(gfx::CommandBuffer& target) const -> void {

}

Image2D::Image2D(std::string_view img, std::string_view material) {

}

Image2D::~Image2D() {

}

auto Image2D::render(gfx::CommandBuffer& target) const -> void {

}

auto Image2D::generate_descriptor(const MaterialManager& man) -> void {
  
}
//class Billboard : public Renderable {
//public:
//  Billboard();
//  virtual ~Billboard();
//  virtual auto render(gfx::CommandBuffer& target) -> void;
//};
}
}