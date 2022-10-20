#pragma once
#include <cstddef>
#include <string>
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
  Renderable();
  virtual ~Renderable();
  virtual auto target() -> std::string_view = 0;
};

class GameObject : Renderable {
  public:
    virtual auto update(float delta_time) -> void = 0;
};
}
}