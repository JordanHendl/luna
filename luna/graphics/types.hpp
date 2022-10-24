#pragma once
#include <cstddef>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace luna {
namespace gfx {
enum class ImageFormat {
  RGBA8,
  BRGA8,
  RGBA32F,
};

struct ImageInfo {
  int gpu = 0;
  size_t width = 1280;
  size_t height = 1024;
  size_t layers = 1;
  ImageFormat format = ImageFormat::RGBA8;
  size_t num_mips = 1;
};

class Renderable {
public:
  Renderable() = default;
  virtual ~Renderable() = default;
  virtual auto render(std::int32_t target) -> void = 0;
  auto set_transform(const glm::mat4& mat) -> void {this->m_transform = mat;}
  auto transform() const -> const glm::mat4& {return this->m_transform;}
protected:
  const glm::vec3* m_pos = nullptr;
  glm::mat4 m_transform;
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
  virtual auto render(std::int32_t target) -> void;
};

class Image2D : public Renderable {
public:
  Image2D(const glm::vec3* position, std::string_view img, std::string_view material);
  virtual ~Image2D();
  virtual auto render(std::int32_t target) -> void;
};

class Billboard : public Renderable {
public:
  Billboard();
  virtual ~Billboard();
  virtual auto render(std::int32_t target) -> void;
};
}
}