#include "luna/graphics/renderable.hpp"
#include "luna/graphics/material/material_manager.hpp"
#include "luna/error/error.hpp"
#include <glm/glm.hpp>
#include <vector>
namespace luna {
namespace gfx {
template<typename T>
inline auto find_valid_entry(const T& container) -> size_t {
  auto index = 0u;
  for(const auto& a : container) {
    if(!a.valid()) {return index;}
    index++;
  }
  LunaAssert(false, "Ran out of space!");
  return 0;
}

struct TextData {
  glm::vec4 color;

  auto valid() const {
    return false;
  }
};

struct Image2DData {
  std::string material;
  glm::vec4 color;

  auto valid() const {
    return !material.empty();
  }
};

struct RenderableResources {
  std::vector<TextData> text;
  std::vector<Image2DData> image2d;

  RenderableResources() {
    constexpr auto num_renderables = 1024;
    this->text.resize(num_renderables);
    this->image2d.resize(num_renderables);
  }
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
  auto& res = renderable_resources();
  auto id = find_valid_entry(res.image2d);
  this->m_idx = id;
  res.image2d[id].material = material;
}

Image2D::~Image2D() {

}

auto Image2D::render(gfx::CommandBuffer& target) const -> void {

}

auto Image2D::generate_descriptor(const MaterialManager& man) -> void {
  const auto& pipe = man.request("image2d")->pipeline();
  this->m_descriptor = gfx::Descriptor(pipe);

  //@TODO bind necessary data.
}
//class Billboard : public Renderable {
//public:
//  Billboard();
//  virtual ~Billboard();
//  virtual auto render(gfx::CommandBuffer& target) -> void;
//};
}
}