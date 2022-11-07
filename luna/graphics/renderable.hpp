#pragma once
#include "luna/graphics/impl/impl.hpp"
#include <cstddef>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace luna {
class Scene;
namespace gfx {
class CommandBuffer;
class Descriptor;

class Renderable {
public:
  Renderable() = default;
  ~Renderable();

  // pure virtual
  virtual auto render(gfx::CommandBuffer& target) const -> void = 0;
  
  // shared
  auto set_per_frame_info(const Scene* scene, const glm::vec3& pos) -> void;
  
  // inlined
  auto descriptor() const -> const Descriptor& {return this->m_descriptor;}
  auto set_transform(const glm::mat4& mat) -> void {this->m_transform = mat;}
  auto transform() const -> const glm::mat4& {return this->m_transform;}
protected:
  glm::mat4 m_transform;
  gfx::Descriptor m_descriptor;
  int32_t m_idx;
};

class Text : public Renderable {
public: 
  Text(const glm::vec3* position, std::string_view font_name);
  virtual ~Text();
  auto set(std::string_view text) -> void;
  auto append(std::string_view text) -> void;
  auto clear() -> void;
  auto set_color(const glm::vec4& rgba) -> void;
  auto color() -> const glm::vec4&;
  auto toggle_outline() -> bool;
  virtual auto render(gfx::CommandBuffer& target) const -> void;
};

class Image2D : public Renderable {
public:
  Image2D(std::string_view img, std::string_view material);
  virtual ~Image2D();
  virtual auto render(gfx::CommandBuffer& target) const -> void;
};

class Billboard : public Renderable {
public:
  Billboard();
  virtual ~Billboard();
  virtual auto render(gfx::CommandBuffer& target) const -> void;
};
}
}