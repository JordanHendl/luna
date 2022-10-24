#include "luna/graphics/types.hpp"
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

auto Text::render(std::int32_t target) -> void {

}

Image2D::Image2D(const glm::vec3* position, std::string_view img, std::string_view material) {

}

Image2D::~Image2D() {

}

auto Image2D::render(std::int32_t target) -> void {

}


//class Billboard : public Renderable {
//public:
//  Billboard();
//  virtual ~Billboard();
//  virtual auto render(std::int32_t target) -> void;
//};
}
}